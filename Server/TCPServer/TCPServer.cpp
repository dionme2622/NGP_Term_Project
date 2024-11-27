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
std::vector<SOCKET> clientSockets; // 클라이언트 소켓 목록
std::mutex clientMutex;            // 클라이언트 리스트 보호용 mutex
std::map<int, int> clientInput;


// 클라이언트 방향키 처리 스레드
DWORD WINAPI ClientThread(LPVOID arg) {
    SOCKET client_sock = (SOCKET)arg;
    CS_PlayerInputPacket localRecvPacket; // 로컬 변수로 변경
    int retval;

    while (1) {
        // 방향키 값 수신
        retval = recv(client_sock, (char*)&localRecvPacket, sizeof(localRecvPacket), 0);
        if (retval == SOCKET_ERROR || retval == 0) {
            printf("클라이언트 연결 종료\n");
            break;
        }

        // 수신한 데이터를 keyState에 업데이트
        {
            std::lock_guard<std::mutex> lock(clientMutex); // 동기화
            keyState[localRecvPacket.playerID] = localRecvPacket.keyState;
        }

        printf("ID[%d], 키 입력: %d\n", localRecvPacket.playerID, localRecvPacket.keyState);
    }

    // 연결 종료 처리
    closesocket(client_sock);
    {
        std::lock_guard<std::mutex> lock(clientMutex);
        clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_sock), clientSockets.end());
    }
    printf("클라이언트 처리 종료\n");
    return 0;
}

static int num = 0;

// 모든 클라이언트에게 주기적으로 데이터 전송하는 스레드
void GameLogicThread() {
    while (1) {
        m_GameTimer.Tick(120.0f);
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 10ms 간격 실행

        // 모든 플레이어 상태 업데이트
        for (int playerID = 0; playerID < 2; ++playerID) {
            player[playerID]->stop = true;

            // 현재 플레이어의 입력 상태에 따라 처리
            {
                std::lock_guard<std::mutex> lock(clientMutex); // keyState 접근 동기화
                if (keyState[playerID] == DIR_DOWN) player[playerID]->SetDirection(DIR_DOWN), player[playerID]->stop = false;
                else if (keyState[playerID] == DIR_LEFT) player[playerID]->SetDirection(DIR_LEFT), player[playerID]->stop = false;
                else if (keyState[playerID] == DIR_RIGHT) player[playerID]->SetDirection(DIR_RIGHT), player[playerID]->stop = false;
                else if (keyState[playerID] == DIR_UP) player[playerID]->SetDirection(DIR_UP), player[playerID]->stop = false;
            }

            // 캐릭터 이동
            player[playerID]->Move(m_GameTimer.GetTimeElapsed());

            // 플레이어 데이터 업데이트
            playerData[playerID]->LoadFromPlayer(player[playerID]);
        }

        // 모든 플레이어 데이터를 클라이언트들에게 전송
        SC_PlayersInfoPacket packet;
        for (int i = 0; i < 2; ++i) {
            memcpy(&packet.playerData[i], playerData[i], sizeof(PlayerData));
        }

        clientMutex.lock();
        for (SOCKET client_sock : clientSockets) {
            int retval = send(client_sock, (char*)&packet, sizeof(packet), 0);
            if (retval == SOCKET_ERROR) {
                printf("클라이언트로 데이터 전송 실패. 소켓 제거.\n");
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
