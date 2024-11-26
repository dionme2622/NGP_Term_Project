#include "Common.h"
#include "Player.h"
#include "Timer.h"
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

#define SERVERPORT 9000
#define BUFSIZE    50

CGameTimer m_GameTimer;
int dir = 0;

typedef struct CS_PlayerInputPacket {
    int playerID;           // � Ŭ���̾�Ʈ���� Key�� �Է��ߴ��� �˷��ֱ� ���� ID��
    int keyState;           // �Է��� Key �� ��
} CS_PlayerInputPacket;

typedef struct SC_PlayersInfoPacket {
    CPlayer player[2];           // Ŭ���̾�Ʈ���� Player�� �����͸� ������.
} SC_PlayersInfoPacket;

CPlayer* player[2];


//CPlayer* player2 = new CPlayer();

std::vector<SOCKET> clientSockets; // Ŭ���̾�Ʈ ���� ���
std::mutex clientMutex;            // Ŭ���̾�Ʈ ����Ʈ ��ȣ�� mutex

CS_PlayerInputPacket recvPacket;


// Ŭ���̾�Ʈ ����Ű ó�� ������
DWORD WINAPI ClientThread(LPVOID arg) {
    SOCKET client_sock = (SOCKET)arg;
    char buf[BUFSIZE];
 
    int retval;

    while (1) {
        // ����Ű �� ����
        retval = recv(client_sock, (char*)&recvPacket, sizeof(recvPacket), 0);
        if (retval == SOCKET_ERROR || retval == 0) {
            printf("Ŭ���̾�Ʈ ���� ����\n");
            break;
        }
        else if (retval == 0) {
            break; // ���� ����
        }
        //system("cls");
        buf[retval] = '\0'; // ������ ���ڿ� ���� ó��
       
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
        m_GameTimer.Tick(120.0f);
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 100ms �������� ����
        //num++;
        //printf("%d", num);
        // ��Ŷ ������ ����
        SC_PlayersInfoPacket packet;
        memcpy(&packet.player[0], player[0], sizeof(CPlayer));
        memcpy(&packet.player[1], player[1], sizeof(CPlayer));

        
        // Ŭ���̾�Ʈ ��� ��ȸ�ϸ� ������ ����
        clientMutex.lock();
        player[0]->stop = true;
        player[1]->stop = true;

        if (recvPacket.playerID == 0)       // TODO : Player1�� ����
        {
            if (recvPacket.keyState == DIR_DOWN) player[0]->SetDirection(DIR_DOWN), player[0]->stop = false;
            else if (recvPacket.keyState == DIR_LEFT) player[0]->SetDirection(DIR_LEFT), player[0]->stop = false;
            else if (recvPacket.keyState == DIR_RIGHT) player[0]->SetDirection(DIR_RIGHT), player[0]->stop = false;
            else if (recvPacket.keyState == DIR_UP) player[0]->SetDirection(DIR_UP), player[0]->stop = false;
        }
        else if(recvPacket.playerID == 1)                               // TODO : Player2�� ����
        {
            if (recvPacket.keyState == DIR_DOWN) player[1]->SetDirection(DIR_DOWN), player[1]->stop = false;
            else if (recvPacket.keyState == DIR_LEFT) player[1]->SetDirection(DIR_LEFT), player[1]->stop = false;
            else if (recvPacket.keyState == DIR_RIGHT) player[1]->SetDirection(DIR_RIGHT), player[1]->stop = false;
            else if (recvPacket.keyState == DIR_UP) player[1]->SetDirection(DIR_UP), player[1]->stop = false;
        }

        /*if (recvPacket.keyState == DIR_DOWN) player[recvPacket.playerID]->SetDirection(DIR_DOWN), player[recvPacket.playerID]->stop = false;
        else if (recvPacket.keyState == DIR_LEFT) player[recvPacket.playerID]->SetDirection(DIR_LEFT), player[recvPacket.playerID]->stop = false;
        else if (recvPacket.keyState == DIR_RIGHT) player[recvPacket.playerID]->SetDirection(DIR_RIGHT), player[recvPacket.playerID]->stop = false;
        else if (recvPacket.keyState == DIR_UP) player[recvPacket.playerID]->SetDirection(DIR_UP), player[recvPacket.playerID]->stop = false;*/
        
        for(int i = 0; i < 2; i++) player[i]->Move(m_GameTimer.GetTimeElapsed());
       

        //player[0]->Update(0.03f);

        for (SOCKET client_sock : clientSockets) {
            int retval = send(client_sock, (char*)&packet, sizeof(packet), 0);

            printf("packet size : %d\r", packet.player[1].direction);
            
            if (retval == SOCKET_ERROR) {
                printf("Ŭ���̾�Ʈ�� ������ ���� ����. ���� ����.\n");
                closesocket(client_sock);
                clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_sock), clientSockets.end());
            }
            else {
                // ��Ŷ ���� ���� �� ���
                printf("Player[1] - x: %d, y: %d, direction: %d, state: %d\n",
                    packet.player[1].x,
                    packet.player[1].y,
                    packet.player[1].direction,
                    packet.player[1].state);
            }
        }
        clientMutex.unlock();
    }
}


int main(int argc, char* argv[]) {
    m_GameTimer.Reset();
    player[0] = new CPlayer();
    player[1] = new CPlayer();
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
