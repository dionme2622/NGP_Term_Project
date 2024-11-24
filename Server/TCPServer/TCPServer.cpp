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
    int playerID;           // 어떤 클라이언트에서 Key를 입력했는지 알려주기 위한 ID값
    int keyState;           // 입력한 Key 의 값
} CS_PlayerInputPacket;

typedef struct SC_PlayersInfoPacket {
    CPlayer player;           // 클라이언트에게 Player의 데이터를 보낸다.
} SC_PlayersInfoPacket;

CPlayer* player1 = new CPlayer();
std::vector<SOCKET> clientSockets; // 클라이언트 소켓 목록
std::mutex clientMutex;            // 클라이언트 리스트 보호용 mutex

// 클라이언트 방향키 처리 스레드
DWORD WINAPI ClientThread(LPVOID arg) {
    SOCKET client_sock = (SOCKET)arg;
    char buf[BUFSIZE];
    int dir;
    int retval;

    while (1) {
        // 방향키 값 수신
        retval = recv(client_sock, (char*)&dir, sizeof(dir), 0);
        if (retval == SOCKET_ERROR || retval == 0) {
            printf("클라이언트 연결 종료\n");
            break;
        }

        printf("받은 데이터 : %d\n", dir);
        if (dir >= 37 && dir <=40) {  // 방향키 처리
            player1->Move(dir, 0.03f);
        }
    }

    // 연결 종료 시 소켓 닫기 및 리스트에서 제거
    closesocket(client_sock);
    clientMutex.lock();
    clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_sock), clientSockets.end());
    clientMutex.unlock();
    printf("클라이언트 처리 종료\n");
    return 0;
}

static int num = 0;

// 모든 클라이언트에게 주기적으로 데이터 전송하는 스레드
void GameLogicThread() {
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 100ms 간격으로 실행
        num++;
        printf("%d", num);
        // 패킷 데이터 생성
        SC_PlayersInfoPacket packet;
        memcpy(&packet.player, player1, sizeof(CPlayer));

        // 클라이언트 목록 순회하며 데이터 전송
        clientMutex.lock();
        for (SOCKET client_sock : clientSockets) {
            int retval = send(client_sock, (char*)&packet, sizeof(packet), 0);
            if (retval == SOCKET_ERROR) {
                printf("클라이언트로 데이터 전송 실패. 소켓 제거.\n");
                closesocket(client_sock);
                clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_sock), clientSockets.end());
            }
            else {
                // 패킷 전송 성공 시 출력
                printf("패킷 전송 성공: 클라이언트 소켓 [%d]\n", (int)client_sock);
                printf("보낸 데이터: \n");
                printf("  player1.positionX: %d\n", packet.player.x);
                printf("  player1.positionY: %d\n", packet.player.y);
            }
        }
        clientMutex.unlock();
    }
}


int main(int argc, char* argv[]) {
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

    // 소켓 생성
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

    printf("서버가 클라이언트 연결 대기 중입니다...\n");

    // 주기적으로 데이터를 전송하는 스레드 실행
    std::thread gameLogicThread(GameLogicThread);
    gameLogicThread.detach();

    while (1) {
        // 클라이언트 연결 수락
        struct sockaddr_in clientaddr;
        int addrlen = sizeof(clientaddr);
        SOCKET client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            continue;
        }

        // 접속한 클라이언트 소켓 추가
        clientMutex.lock();
        clientSockets.push_back(client_sock);
        clientMutex.unlock();

        // 클라이언트 처리 스레드 생성
        HANDLE hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) {
            closesocket(client_sock);
        }
        else {
            CloseHandle(hThread);
        }

        // 클라이언트 정보 출력
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
    }

    // 소켓 닫기
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}
