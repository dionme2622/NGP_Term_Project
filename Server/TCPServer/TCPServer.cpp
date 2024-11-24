#include "Common.h"
#include "Player.h"
#include "Timer.h"
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

#define SERVERPORT 9000
#define BUFSIZE    50

typedef struct CS_PlayerInputPacket {
    int playerID;           // � Ŭ���̾�Ʈ���� Key�� �Է��ߴ��� �˷��ֱ� ���� ID��
    int keyState;           // �Է��� Key �� ��
} CS_PlayerInputPacket;

typedef struct SC_PlayersInfoPacket {
    CPlayer player;           // Ŭ���̾�Ʈ���� Player�� �����͸� ������.
} SC_PlayersInfoPacket;

CPlayer* player1 = new CPlayer();
std::vector<SOCKET> clientSockets; // Ŭ���̾�Ʈ ���� ���
std::mutex clientMutex;            // Ŭ���̾�Ʈ ����Ʈ ��ȣ�� mutex

// Ŭ���̾�Ʈ ����Ű ó�� ������
DWORD WINAPI ClientThread(LPVOID arg) {
    SOCKET client_sock = (SOCKET)arg;
    char buf[BUFSIZE];
    int dir;
    int retval;

    while (1) {
        // ����Ű �� ����
        retval = recv(client_sock, (char*)&dir, sizeof(dir), 0);
        if (retval == SOCKET_ERROR || retval == 0) {
            printf("Ŭ���̾�Ʈ ���� ����\n");
            break;
        }

        printf("���� ������ : %d\n", dir);
        if (dir >= 37 && dir <=40) {  // ����Ű ó��
            player1->Move(dir, 0.03f);
        }
    }

    // ���� ���� �� ���� �ݱ� �� ����Ʈ���� ����
    closesocket(client_sock);
    clientMutex.lock();
    clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_sock), clientSockets.end());
    clientMutex.unlock();
    printf("Ŭ���̾�Ʈ ó�� ����\n");
    return 0;
}

static int num = 0;

// ��� Ŭ���̾�Ʈ���� �ֱ������� ������ �����ϴ� ������
void GameLogicThread() {
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 100ms �������� ����
        num++;
        printf("%d", num);
        // ��Ŷ ������ ����
        SC_PlayersInfoPacket packet;
        memcpy(&packet.player, player1, sizeof(CPlayer));

        // Ŭ���̾�Ʈ ��� ��ȸ�ϸ� ������ ����
        clientMutex.lock();
        for (SOCKET client_sock : clientSockets) {
            int retval = send(client_sock, (char*)&packet, sizeof(packet), 0);
            if (retval == SOCKET_ERROR) {
                printf("Ŭ���̾�Ʈ�� ������ ���� ����. ���� ����.\n");
                closesocket(client_sock);
                clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_sock), clientSockets.end());
            }
            else {
                // ��Ŷ ���� ���� �� ���
                printf("��Ŷ ���� ����: Ŭ���̾�Ʈ ���� [%d]\n", (int)client_sock);
                printf("���� ������: \n");
                printf("  player1.positionX: %d\n", packet.player.x);
                printf("  player1.positionY: %d\n", packet.player.y);
            }
        }
        clientMutex.unlock();
    }
}


int main(int argc, char* argv[]) {
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

        // Ŭ���̾�Ʈ ���� ���
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));
    }

    // ���� �ݱ�
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}
