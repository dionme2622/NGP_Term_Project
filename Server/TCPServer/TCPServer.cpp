#include "Common.h"
#include "Player.h"
#include "Timer.h"
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include "Packet.h"

#define SERVERPORT 9000
#define BUFSIZE    50

CGameTimer m_GameTimer;
int dir = 0;

CPlayer* player[2];
PlayerData* playerData[2];

//CPlayer* player2 = new CPlayer();

std::vector<SOCKET> clientSockets; // Ŭ���̾�Ʈ ���� ���
std::mutex clientMutex;            // Ŭ���̾�Ʈ ����Ʈ ��ȣ�� mutex

CS_PlayerInputPacket recvPacket;

static int num = 0;



// Ŭ���̾�Ʈ ����Ű ó�� ������
DWORD WINAPI ClientThread(LPVOID arg) {
    SOCKET client_sock = (SOCKET)arg;
    char buf[BUFSIZE];
    int retval;

    // ������ ����ŷ ���� ����
    u_long mode = 1; // 1 = ����ŷ
    ioctlsocket(client_sock, FIONBIO, &mode);

    // �ֱ������� num ���� ������Ű�� ���� �ð� üũ ����
    auto last_time = std::chrono::steady_clock::now();

    while (1) {
        // ���� �ð� Ȯ��
        auto now = std::chrono::steady_clock::now();

        // 1��(1000ms)�� ����ϸ� num ����
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_time).count() >= 10) {
            num++;
            printf("num ����: %d\n", num);

            if (recvPacket.keyState == DIR_DOWN) player[0]->SetDirection(DIR_DOWN), player[0]->stop = false;
            else if (recvPacket.keyState == DIR_LEFT) player[0]->SetDirection(DIR_LEFT), player[0]->stop = false;
            else if (recvPacket.keyState == DIR_RIGHT) player[0]->SetDirection(DIR_RIGHT), player[0]->stop = false;
            else if (recvPacket.keyState == DIR_UP) player[0]->SetDirection(DIR_UP), player[0]->stop = false;

            for (int i = 0; i < 2; i++) player[i]->Move(m_GameTimer.GetTimeElapsed());
            for (int i = 0; i < 2; i++) playerData[i]->LoadFromPlayer(player[i]);

            last_time = now; // ������ �ð��� ���� �ð����� ������Ʈ
        }

        // ����Ű �� ����
        retval = recv(client_sock, (char*)&recvPacket, sizeof(recvPacket), 0);

        if (retval == SOCKET_ERROR) {
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK) {
                // ����ŷ ��忡�� �����͸� ��ٸ��� ��
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue; // �ٸ� �۾� ���� �ٽ� ���� ����
            }
            else {
                printf("Ŭ���̾�Ʈ ���� ���� (����: %d)\n", err);
                break;
            }
        }
        else if (retval == 0) {
            // ���� ����
            printf("Ŭ���̾�Ʈ ���� ����\n");
            break;
        }
        else {
            // �����͸� ���������� ����
            printf("Ű �Է�: %d\n", recvPacket.keyState);
        }
    }

    return 0;
}




// ��� Ŭ���̾�Ʈ���� �ֱ������� ������ �����ϴ� ������
void GameLogicThread() {
    while (1) {
        m_GameTimer.Tick(120.0f);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100ms �������� ����
        //num++;
        //printf("%d", num);
        // ��Ŷ ������ ����
        SC_PlayersInfoPacket packet;
        memcpy(&packet.playerData[0], playerData[0], sizeof(PlayerData));
        memcpy(&packet.playerData[1], playerData[1], sizeof(PlayerData));


        // Ŭ���̾�Ʈ ��� ��ȸ�ϸ� ������ ����
        clientMutex.lock();
        player[0]->stop = true;
        player[1]->stop = true;



        for (SOCKET client_sock : clientSockets) {
            int retval = send(client_sock, (char*)&packet, sizeof(packet), 0);

            //printf("packet size : %d\r", packet.player[1].direction);

            if (retval == SOCKET_ERROR) {
                printf("Ŭ���̾�Ʈ�� ������ ���� ����. ���� ����.\n");
                closesocket(client_sock);
                clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_sock), clientSockets.end());
            }
            else {
                // ��Ŷ ���� ���� �� ���
                printf("Player[1] - x: %d, y: %d, direction: %d, state: %d\n",
                    packet.playerData[1].x,
                    packet.playerData[1].y,
                    packet.playerData[1].direction,
                    packet.playerData[1].state);
            }
        }
        clientMutex.unlock();
    }
}


int main(int argc, char* argv[]) {
    m_GameTimer.Reset();
    player[0] = new CPlayer();
    player[1] = new CPlayer();
    playerData[0] = new PlayerData();
    playerData[1] = new PlayerData();

    int retval;
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

    // ���� ����
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    printf("������ Ŭ���̾�Ʈ ���� ��� ���Դϴ�...\n");

    // �ֱ������� �����͸� �����ϴ� ������ ����
    std::thread gameLogicThread(GameLogicThread);
    gameLogicThread.detach();

    while (1) {
        // Ŭ���̾�Ʈ ���� ����
        struct sockaddr_in clientaddr;
        int addrlen = sizeof(clientaddr);
        SOCKET client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            continue;
        }

        static int ID;
        retval = send(client_sock, (char*)&ID, sizeof(ID), 0);
        ++ID;

        // ������ Ŭ���̾�Ʈ ���� ���
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));
        // ������ Ŭ���̾�Ʈ ���� �߰�
        clientMutex.lock();
        clientSockets.push_back(client_sock);
        clientMutex.unlock();

        // Ŭ���̾�Ʈ ó�� ������ ����
        HANDLE hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) {
            closesocket(client_sock);
        }
        else {
            CloseHandle(hThread);
        }
    }

    // ���� �ݱ�
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}
