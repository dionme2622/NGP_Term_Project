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

std::vector<SOCKET> clientSockets; // 클라이언트 소켓 목록
std::mutex clientMutex;            // 클라이언트 리스트 보호용 mutex

CS_PlayerInputPacket recvPacket;

static int num = 0;



// 클라이언트 방향키 처리 스레드
DWORD WINAPI ClientThread(LPVOID arg) {
    SOCKET client_sock = (SOCKET)arg;
    char buf[BUFSIZE];
    int retval;

    // 소켓을 비블로킹 모드로 설정
    u_long mode = 1; // 1 = 비블로킹
    ioctlsocket(client_sock, FIONBIO, &mode);

    // 주기적으로 num 값을 증가시키기 위한 시간 체크 변수
    auto last_time = std::chrono::steady_clock::now();

    while (1) {
        // 현재 시간 확인
        auto now = std::chrono::steady_clock::now();

        // 1초(1000ms)가 경과하면 num 증가
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_time).count() >= 10) {
            num++;
            printf("num 증가: %d\n", num);

            if (recvPacket.keyState == DIR_DOWN) player[0]->SetDirection(DIR_DOWN), player[0]->stop = false;
            else if (recvPacket.keyState == DIR_LEFT) player[0]->SetDirection(DIR_LEFT), player[0]->stop = false;
            else if (recvPacket.keyState == DIR_RIGHT) player[0]->SetDirection(DIR_RIGHT), player[0]->stop = false;
            else if (recvPacket.keyState == DIR_UP) player[0]->SetDirection(DIR_UP), player[0]->stop = false;

            for (int i = 0; i < 2; i++) player[i]->Move(m_GameTimer.GetTimeElapsed());
            for (int i = 0; i < 2; i++) playerData[i]->LoadFromPlayer(player[i]);

            last_time = now; // 마지막 시간을 현재 시간으로 업데이트
        }

        // 방향키 값 수신
        retval = recv(client_sock, (char*)&recvPacket, sizeof(recvPacket), 0);

        if (retval == SOCKET_ERROR) {
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK) {
                // 비블로킹 모드에서 데이터를 기다리는 중
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue; // 다른 작업 없이 다시 루프 실행
            }
            else {
                printf("클라이언트 연결 종료 (에러: %d)\n", err);
                break;
            }
        }
        else if (retval == 0) {
            // 연결 종료
            printf("클라이언트 연결 종료\n");
            break;
        }
        else {
            // 데이터를 정상적으로 수신
            printf("키 입력: %d\n", recvPacket.keyState);
        }
    }

    return 0;
}




// 모든 클라이언트에게 주기적으로 데이터 전송하는 스레드
void GameLogicThread() {
    while (1) {
        m_GameTimer.Tick(120.0f);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100ms 간격으로 실행
        //num++;
        //printf("%d", num);
        // 패킷 데이터 생성
        SC_PlayersInfoPacket packet;
        memcpy(&packet.playerData[0], playerData[0], sizeof(PlayerData));
        memcpy(&packet.playerData[1], playerData[1], sizeof(PlayerData));


        // 클라이언트 목록 순회하며 데이터 전송
        clientMutex.lock();
        player[0]->stop = true;
        player[1]->stop = true;



        for (SOCKET client_sock : clientSockets) {
            int retval = send(client_sock, (char*)&packet, sizeof(packet), 0);

            //printf("packet size : %d\r", packet.player[1].direction);

            if (retval == SOCKET_ERROR) {
                printf("클라이언트로 데이터 전송 실패. 소켓 제거.\n");
                closesocket(client_sock);
                clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_sock), clientSockets.end());
            }
            else {
                // 패킷 전송 성공 시 출력
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

        static int ID;
        retval = send(client_sock, (char*)&ID, sizeof(ID), 0);
        ++ID;

        // 접속한 클라이언트 정보 출력
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
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
    }

    // 소켓 닫기
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}
