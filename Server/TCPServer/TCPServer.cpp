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
int keyState[2] = {};
//CPlayer* player2 = new CPlayer();

CS_PlayerInputPacket recvPacket;
std::vector<SOCKET> clientSockets; // Ŭ���̾�Ʈ ���� ���
std::mutex clientMutex;            // Ŭ���̾�Ʈ ����Ʈ ��ȣ�� mutex
std::map<int, int> clientInput;


// Ŭ���̾�Ʈ ����Ű ó�� ������
DWORD WINAPI ClientThread(LPVOID arg) {
    SOCKET client_sock = (SOCKET)arg;
    CS_PlayerInputPacket localRecvPacket; // ���� ������ ����
    int retval;

    while (1) {
        // ����Ű �� ����
        retval = recv(client_sock, (char*)&localRecvPacket, sizeof(localRecvPacket), 0);
        if (retval == SOCKET_ERROR || retval == 0) {
            printf("Ŭ���̾�Ʈ ���� ����\n");
            break;
        }

        // ������ �����͸� keyState�� ������Ʈ
        {
            std::lock_guard<std::mutex> lock(clientMutex); // ����ȭ
            keyState[localRecvPacket.playerID] = localRecvPacket.keyState;
        }

        printf("ID[%d], Ű �Է�: %d\n", localRecvPacket.playerID, localRecvPacket.keyState);
    }

    // ���� ���� ó��
    closesocket(client_sock);
    {
        std::lock_guard<std::mutex> lock(clientMutex);
        clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_sock), clientSockets.end());
    }
    printf("Ŭ���̾�Ʈ ó�� ����\n");
    return 0;
}

static int num = 0;

// ��� Ŭ���̾�Ʈ���� �ֱ������� ������ �����ϴ� ������
void GameLogicThread() {
    while (1) {
        m_GameTimer.Tick(120.0f);
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 10ms ���� ����

        // ��� �÷��̾� ���� ������Ʈ
        for (int playerID = 0; playerID < 2; ++playerID) {
            player[playerID]->stop = true;

            // ���� �÷��̾��� �Է� ���¿� ���� ó��
            {
                std::lock_guard<std::mutex> lock(clientMutex); // keyState ���� ����ȭ
                if (keyState[playerID] == DIR_DOWN) player[playerID]->SetDirection(DIR_DOWN), player[playerID]->stop = false;
                else if (keyState[playerID] == DIR_LEFT) player[playerID]->SetDirection(DIR_LEFT), player[playerID]->stop = false;
                else if (keyState[playerID] == DIR_RIGHT) player[playerID]->SetDirection(DIR_RIGHT), player[playerID]->stop = false;
                else if (keyState[playerID] == DIR_UP) player[playerID]->SetDirection(DIR_UP), player[playerID]->stop = false;
            }

            // ĳ���� �̵�
            player[playerID]->Move(m_GameTimer.GetTimeElapsed());

            // �÷��̾� ������ ������Ʈ
            playerData[playerID]->LoadFromPlayer(player[playerID]);
        }

        // ��� �÷��̾� �����͸� Ŭ���̾�Ʈ�鿡�� ����
        SC_PlayersInfoPacket packet;
        for (int i = 0; i < 2; ++i) {
            memcpy(&packet.playerData[i], playerData[i], sizeof(PlayerData));
        }

        clientMutex.lock();
        for (SOCKET client_sock : clientSockets) {
            int retval = send(client_sock, (char*)&packet, sizeof(packet), 0);
            if (retval == SOCKET_ERROR) {
                printf("Ŭ���̾�Ʈ�� ������ ���� ����. ���� ����.\n");
                closesocket(client_sock);
                clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_sock), clientSockets.end());
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
