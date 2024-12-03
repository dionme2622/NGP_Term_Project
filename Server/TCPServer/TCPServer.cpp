#include "Common.h"
#include "Player.h"
#include "Timer.h"
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include "Packet.h"

#define SERVERPORT      9000
#define BUFSIZE         50
#define BEFOREBOOMCOUNT 1.5f
#define STARTBOOMCOUNT  0.5f

CGameTimer m_GameTimer;
int dir = 0;

CPlayer* player[2];
PlayerData* playerData[2];
MapData* mapData;
int keyState[2] = {};
//CPlayer* player2 = new CPlayer();

std::vector<SOCKET> clientSockets; // 클라이언트 소켓 목록
std::mutex clientMutex;            // 클라이언트 리스트 보호용 mutex
std::map<int, int> clientInput;

SC_PlayersInfoPacket packet;       // Client로 보내는 패킷 구조체


void Initialize();
void PlayerMeetObstacle(CPlayer* Player);
void PlayerGetItem(CPlayer* Player);
void BallonBoom(CPlayer* Player, int num, float fTimeElapsed);

std::default_random_engine dre;
std::uniform_int_distribution<int> uid{ 3,8 };

int ObstacleBreak[13][15];


DWORD WINAPI ClientThread(LPVOID arg) {
    SOCKET client_sock = (SOCKET)arg;
    CS_PlayerInputPacket localRecvPacket; // 로컬 변수로 변경
    int retval;

    while (1) {
        // 1. 헤더만 읽기
        PacketHeader header;
        retval = recv(client_sock, (char*)&header, sizeof(PacketHeader), 0);
        if (retval <= 0 || retval != sizeof(PacketHeader)) {
            printf("클라이언트 연결 종료 또는 헤더 수신 실패\n");
            break;
        }

        // 2. 패킷 타입 확인
        printf("수신된 패킷 타입: %d\n", header.packetType);

        // 3. 패킷 타입에 따라 처리
        if (header.packetType == 1) { // PlayerInputPacket
            CS_PlayerInputPacket inputPacket;
            retval = recv(client_sock, (char*)&inputPacket + sizeof(PacketHeader),
                sizeof(CS_PlayerInputPacket) - sizeof(PacketHeader), 0);
            if (retval <= 0 || retval != (sizeof(CS_PlayerInputPacket) - sizeof(PacketHeader))) {
                printf("PlayerInput 패킷 수신 실패\n");
                break;
            }

            // KeyState 업데이트
            {
                std::lock_guard<std::mutex> lock(clientMutex);
                keyState[inputPacket.playerID] = inputPacket.keyState;
            }
            printf("ID[%d], 키 입력: %d\n", inputPacket.playerID, inputPacket.keyState);
        }
        else if (header.packetType == 2) { // LobbyPacket
            CS_LobbyPacket lobbyPacket;
            retval = recv(client_sock, (char*)&lobbyPacket + sizeof(PacketHeader),
                sizeof(CS_LobbyPacket) - sizeof(PacketHeader), 0);
            if (retval <= 0 || retval != (sizeof(CS_LobbyPacket) - sizeof(PacketHeader))) {
                printf("LobbyPacket 수신 실패\n");
                break;
            }

            printf("Ready 패킷 수신: Map=%d, NextScene=%s\n",
                lobbyPacket.selectedMap, lobbyPacket.nextSceneCall ? "true" : "false");

            // SC_LobbyPacket 작성 및 브로드캐스트
            SC_LobbyPacket responsePacket;
            responsePacket.mapData = *mapData;
            responsePacket.nextSceneCall = lobbyPacket.nextSceneCall;

            // 클라이언트들에게 브로드캐스트
            std::lock_guard<std::mutex> lock(clientMutex);
            for (SOCKET sock : clientSockets) {
                retval = send(sock, (char*)&responsePacket, sizeof(SC_LobbyPacket), 0);
                if (retval == SOCKET_ERROR) {
                    printf("클라이언트로 데이터 전송 실패. 소켓 제거.\n");
                    closesocket(sock);
                    clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), sock), clientSockets.end());
                }
                else {
                    printf("클라이언트로 SC_LobbyPacket 전송 완료.\n");
                }
            }
        }
        else {
            printf("알 수 없는 패킷 타입: %d\n", header.packetType);
            break;
        }
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
                else if (keyState[playerID] == SPACE) {
                    if (player[playerID]->state == DAMAGE) return;
                    for (int i = 0; i < player[playerID]->ballon_num; i++)
                    {
                        if (player[playerID]->ballon[i]->state == 0)
                        {
                            player[playerID]->ballon[i]->x = (player[playerID]->x + 30 - 30) / 60 * 60;
                            player[playerID]->ballon[i]->y = (player[playerID]->y + 30 - 65) / 60 * 60;
                            if (packet.mapData.boardData[player[playerID]->ballon[i]->y / 60][player[playerID]->ballon[i]->x / 60].state == 1)
                            {
                                player[playerID]->ballon[i]->SetState(1);
                                packet.mapData.boardData[player[playerID]->ballon[i]->y / 60][player[playerID]->ballon[i]->x / 60].SetState(4);
                            }
                        }
                    }
                }
            }

            // 캐릭터 이동
            player[playerID]->Move(m_GameTimer.GetTimeElapsed());
            PlayerMeetObstacle(player[playerID]);       // 플레이어와 장애물 충돌처리
            PlayerGetItem(player[playerID]);            // 플레이어와 아이템 충돌처리
            BallonBoom(player[playerID], 0, m_GameTimer.GetTimeElapsed());
            BallonBoom(player[playerID], 1, m_GameTimer.GetTimeElapsed());
            BallonBoom(player[playerID], 2, m_GameTimer.GetTimeElapsed());
            BallonBoom(player[playerID], 3, m_GameTimer.GetTimeElapsed());
            BallonBoom(player[playerID], 4, m_GameTimer.GetTimeElapsed());
            BallonBoom(player[playerID], 5, m_GameTimer.GetTimeElapsed());

            // 플레이어 데이터 업데이트
            playerData[playerID]->LoadFromPlayer(player[playerID]);
        }

        // 모든 플레이어 데이터를 클라이언트들에게 전송
        for (int i = 0; i < 2; ++i) {
            memcpy(&packet.playerData[i], playerData[i], sizeof(PlayerData));   // Player들의 데이터를 클라이언트에게 보낸다.
        }
        //memcpy(&packet.mapData, mapData, sizeof(MapData));                      // Map의 데이터를 클라이언트에게 보낸다.

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
    mapData = new MapData();
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

        Initialize();       // 맵 초기화 TODO : 좀 이쁘게 수정하기 (임시로 해둔 것)

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


void Initialize() {
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 15; j++) {
            packet.mapData.boardData[i][j].SetState(1);
        }
    }

    for (int i = 2; i < 13; i++)
    {
        packet.mapData.boardData[0][i].SetState(2);
        packet.mapData.boardData[12][i].SetState(2);
    }
    for (int i = 4; i < 11; i++)
    {
        packet.mapData.boardData[1][i].SetState(2);
        packet.mapData.boardData[11][i].SetState(2);
    }
    packet.mapData.boardData[1][1].SetState(3); packet.mapData.boardData[1][3].SetState(3); packet.mapData.boardData[1][11].SetState(3); packet.mapData.boardData[1][13].SetState(3);
    packet.mapData.boardData[3][1].SetState(3); packet.mapData.boardData[3][3].SetState(3); packet.mapData.boardData[3][11].SetState(3); packet.mapData.boardData[3][13].SetState(3);
    packet.mapData.boardData[5][1].SetState(3); packet.mapData.boardData[5][3].SetState(3); packet.mapData.boardData[5][11].SetState(3); packet.mapData.boardData[5][13].SetState(3);
    packet.mapData.boardData[5][1].SetState(3); packet.mapData.boardData[5][3].SetState(3); packet.mapData.boardData[5][11].SetState(3); packet.mapData.boardData[5][13].SetState(3);
    packet.mapData.boardData[7][1].SetState(3); packet.mapData.boardData[7][3].SetState(3); packet.mapData.boardData[7][11].SetState(3); packet.mapData.boardData[7][13].SetState(3);
    packet.mapData.boardData[6][5].SetState(3); packet.mapData.boardData[6][7].SetState(3); packet.mapData.boardData[6][9].SetState(3);	packet.mapData.boardData[10][9].SetState(3);
    packet.mapData.boardData[9][1].SetState(3); packet.mapData.boardData[9][3].SetState(3); packet.mapData.boardData[9][11].SetState(3); packet.mapData.boardData[9][13].SetState(3);
    packet.mapData.boardData[4][6].SetState(3); packet.mapData.boardData[4][8].SetState(3); packet.mapData.boardData[8][6].SetState(3);	packet.mapData.boardData[8][8].SetState(3);

    for (int i = 0; i < 5; i++)
    {
        packet.mapData.boardData[2][i].SetState(2);
        packet.mapData.boardData[4][i].SetState(2);
        packet.mapData.boardData[8][i].SetState(2);
        packet.mapData.boardData[10][i].SetState(2);
    }
    for (int i = 10; i < 15; i++)
    {
        packet.mapData.boardData[2][i].SetState(2);
        packet.mapData.boardData[4][i].SetState(2);
        packet.mapData.boardData[8][i].SetState(2);
        packet.mapData.boardData[10][i].SetState(2);
    }
    packet.mapData.boardData[3][0].SetState(2); packet.mapData.boardData[3][5].SetState(2); packet.mapData.boardData[3][9].SetState(2); packet.mapData.boardData[3][14].SetState(2);
    packet.mapData.boardData[10][0].SetState(2); packet.mapData.boardData[10][5].SetState(2); packet.mapData.boardData[10][9].SetState(2); packet.mapData.boardData[10][14].SetState(2);
    for (int i = 5; i < 10; i++)
    {
        packet.mapData.boardData[5][i].SetState(2);
        packet.mapData.boardData[7][i].SetState(2);
    }
    packet.mapData.boardData[10][2].SetState(1); packet.mapData.boardData[10][5].SetState(1); packet.mapData.boardData[10][9].SetState(1); packet.mapData.boardData[10][12].SetState(1);
}

void PlayerMeetObstacle(CPlayer* Player)         // 수정 후
{
    if (Player->direction == DIR_LEFT)
    {
        int i = (Player->x - 30) / 60;
        int j = (Player->y - 65) / 60;
        if (packet.mapData.boardData[j][i].state == 2 || packet.mapData.boardData[j][i].state == 3 || packet.mapData.boardData[j][i].state == 40)
        {
            Player->x = i * 60 + 60 + 30;
            j = (Player->y + 60 - 65) / 60;
            if ((Player->y % 60 >= 45 || Player->y % 60 <= 5) && (packet.mapData.boardData[j][i].state != 2 && packet.mapData.boardData[j][i].state != 3 && packet.mapData.boardData[j][i].state != 40) && (packet.mapData.boardData[j][i + 1].state != 2 && packet.mapData.boardData[j][i + 1].state != 3 && packet.mapData.boardData[j][i + 1].state != 40))
                Player->y = Player->y + 1;
        }
        j = (Player->y + 60 - 65) / 60;
        if ((packet.mapData.boardData[j][i].state == 2 || packet.mapData.boardData[j][i].state == 3 || packet.mapData.boardData[j][i].state == 40) && Player->y % 60 != 5)
        {
            Player->x = i * 60 + 60 + 30;
            j = (Player->y - 65) / 60;
            if (Player->y % 60 <= 25 && Player->y % 60 >= 5 && (packet.mapData.boardData[j][i].state != 2 && packet.mapData.boardData[j][i].state != 3 && packet.mapData.boardData[j][i].state != 40) && (packet.mapData.boardData[j][i + 1].state != 2 && packet.mapData.boardData[j][i + 1].state != 3 && packet.mapData.boardData[j][i + 1].state != 40))
                Player->y = Player->y - 1;
        }
    }
    else if (Player->direction == DIR_RIGHT)
    {
        int i = (Player->x - 30) / 60;
        int j = (Player->y - 65) / 60;
        if (packet.mapData.boardData[j][i + 1].state == 2 || packet.mapData.boardData[j][i + 1].state == 3 || packet.mapData.boardData[j][i + 1].state == 40)
        {
            Player->x = i * 60 + 30;
            j = (Player->y + 60 - 65) / 60;
            if ((Player->y % 60 >= 45 || Player->y % 60 <= 5) && (packet.mapData.boardData[j][i + 1].state != 2 && packet.mapData.boardData[j][i + 1].state != 3 && packet.mapData.boardData[j][i + 1].state != 40) && (packet.mapData.boardData[j][i - 1].state != 2 && packet.mapData.boardData[j][i - 1].state != 3 && packet.mapData.boardData[j][i - 1].state != 40))
                Player->y = Player->y + 1;
        }
        j = (Player->y + 60 - 65) / 60;
        if ((packet.mapData.boardData[j][i + 1].state == 2 || packet.mapData.boardData[j][i + 1].state == 3 || packet.mapData.boardData[j][i + 1].state == 40) && Player->y % 60 != 5)
        {
            Player->x = i * 60 + 30;
            j = (Player->y - 65) / 60;
            if (Player->y % 60 <= 25 && Player->y % 60 >= 5 && (packet.mapData.boardData[j][i + 1].state != 2 && packet.mapData.boardData[j][i + 1].state != 3 && packet.mapData.boardData[j][i + 1].state != 40) && (packet.mapData.boardData[j][i - 1].state != 2 && packet.mapData.boardData[j][i - 1].state != 3 && packet.mapData.boardData[j][i - 1].state != 40))
                Player->y = Player->y - 1;
        }
    }
    else if (Player->direction == DIR_UP)
    {
        int i = (Player->x - 30) / 60;
        int j = (Player->y - 65) / 60;
        if (packet.mapData.boardData[j][i].state == 2 || packet.mapData.boardData[j][i].state == 3 || packet.mapData.boardData[j][i].state == 40)
        {
            Player->y = j * 60 + 60 + 65;
            i = (Player->x + 60 - 30) / 60;
            if (Player->x % 60 <= 30 && Player->x % 60 >= 10 && (packet.mapData.boardData[j][i].state != 2 && packet.mapData.boardData[j][i].state != 3 && packet.mapData.boardData[j][i].state != 40) && (packet.mapData.boardData[j + 1][i].state != 2 && packet.mapData.boardData[j + 1][i].state != 3 && packet.mapData.boardData[j + 1][i].state != 40))
                Player->x = Player->x + 1;
        }
        i = (Player->x + 60 - 30) / 60;
        if ((packet.mapData.boardData[j][i].state == 2 || packet.mapData.boardData[j][i].state == 3 || packet.mapData.boardData[j][i].state == 40) && Player->x % 60 != 30)
        {
            Player->y = j * 60 + 60 + 65;
            i = (Player->x - 30) / 60;
            if (Player->x % 60 <= 50 && Player->x % 60 >= 30 && (packet.mapData.boardData[j][i].state != 2 && packet.mapData.boardData[j][i].state != 3 && packet.mapData.boardData[j][i].state != 40) && (packet.mapData.boardData[j + 1][i].state != 2 && packet.mapData.boardData[j + 1][i].state != 3 && packet.mapData.boardData[j + 1][i].state != 40))
                Player->x = Player->x - 1;
        }
    }
    else if (Player->direction == DIR_DOWN)
    {
        int i = (Player->x - 30) / 60;
        int j = (Player->y - 65) / 60;
        if (packet.mapData.boardData[j + 1][i].state == 2 || packet.mapData.boardData[j + 1][i].state == 3 || packet.mapData.boardData[j + 1][i].state == 40)
        {
            Player->y = j * 60 + 65;
            i = (Player->x + 60 - 30) / 60;
            if (Player->x % 60 <= 30 && Player->x % 60 >= 10 && (packet.mapData.boardData[j + 1][i].state != 2 && packet.mapData.boardData[j + 1][i].state != 3 && packet.mapData.boardData[j + 1][i].state != 40) && (packet.mapData.boardData[j - 1][i].state != 2 && packet.mapData.boardData[j - 1][i].state != 3 && packet.mapData.boardData[j - 1][i].state != 40))
                Player->x = Player->x + 1;
        }
        i = (Player->x + 60 - 30) / 60;
        if ((packet.mapData.boardData[j + 1][i].state == 2 || packet.mapData.boardData[j + 1][i].state == 3 || packet.mapData.boardData[j + 1][i].state == 40) && Player->x % 60 != 30)
        {
            Player->y = j * 60 + 65;
            i = (Player->x - 30) / 60;
            if (Player->x % 60 <= 50 && Player->x % 60 >= 30 && (packet.mapData.boardData[j + 1][i].state != 2 && packet.mapData.boardData[j + 1][i].state != 3 && packet.mapData.boardData[j + 1][i].state != 40) && (packet.mapData.boardData[j - 1][i].state != 2 && packet.mapData.boardData[j - 1][i].state != 3 && packet.mapData.boardData[j - 1][i].state != 40))
                Player->x = Player->x - 1;
        }
    }
}

void PlayerGetItem(CPlayer* Player)
{
    int i = (Player->x + 30 - 30) / 60;
    int j = (Player->y + 30 - 65) / 60;

    if (packet.mapData.boardData[j][i].state == 6)		// 이동속도 아이템
    {
        if (Player->speed < 200)
            Player->speed += 20;
        printf("Player speed : %d\n", Player->speed);
        packet.mapData.boardData[j][i].state = 1;

    }
    else if (packet.mapData.boardData[j][i].state == 7)		// 물줄기 아이템
    {
        if (Player->ballon_length < 6)
            Player->ballon_length += 1;
        packet.mapData.boardData[j][i].state = 1;
    }
    else if (packet.mapData.boardData[j][i].state == 8)		// 물풍선 갯수 아이템
    {
        if (Player->ballon_num < 5)
            Player->ballon_num += 1;
        packet.mapData.boardData[j][i].state = 1;
    }
}

void BallonBoom(CPlayer* Player, int num, float fTimeElapsed)
{
    ////물풍선 놓고 터지기 전
    if (Player->ballon[num]->GetState() == 1)
    {
        if (packet.mapData.boardData[(Player->ballon[num]->y) / 60][(Player->ballon[num]->x) / 60].state == 5)
            Player->ballon[num]->beforeboomcount = BEFOREBOOMCOUNT;
        if (Player->ballon[num]->beforeboomcount < BEFOREBOOMCOUNT)     // 풍선은 1.5초가 지나면 터진다
            Player->ballon[num]->beforeboomcount += fTimeElapsed;
        else
            Player->ballon[num]->SetState(2);
    }
    else if (Player->ballon[num]->GetState() == 2)
    {
        if (Player->ballon[num]->startboomcount < STARTBOOMCOUNT)       // 폭발의 지속시간은 0.5초 이다
        {
            packet.mapData.boardData[(Player->ballon[num]->y) / 60][(Player->ballon[num]->x) / 60].SetState(5);
            for (int j = 1; j < Player->GetBallonLength(); j++) // right
            {
                int x = ((Player->ballon[num]->x) + 60 * j) / 60;
                int y = (Player->ballon[num]->y) / 60;
                if (x < 0 || x == 15)
                    break;
                if (packet.mapData.boardData[y][x].state == 1 || packet.mapData.boardData[y][x].state == 4 || packet.mapData.boardData[y][x].state == 40 || packet.mapData.boardData[y][x].state == 6 || packet.mapData.boardData[y][x].state == 7 || packet.mapData.boardData[y][x].state == 8 || packet.mapData.boardData[y][x].state == 9)
                    packet.mapData.boardData[y][x].SetState(5);
                else if (packet.mapData.boardData[y][x].state == 2)
                    break;
                else if (packet.mapData.boardData[y][x].state == 3)
                    break;
            }
            for (int j = 1; j < Player->GetBallonLength(); j++)   // left
            {
                int x = ((Player->ballon[num]->x) - 60 * j) / 60;
                int y = (Player->ballon[num]->y) / 60;
                if (x < 0 || x == 15)
                    break;
                if (packet.mapData.boardData[y][x].state == 1 || packet.mapData.boardData[y][x].state == 4 || packet.mapData.boardData[y][x].state == 40 || packet.mapData.boardData[y][x].state == 6 || packet.mapData.boardData[y][x].state == 7 || packet.mapData.boardData[y][x].state == 8 || packet.mapData.boardData[y][x].state == 9)
                    packet.mapData.boardData[y][x].SetState(5);
                else if (packet.mapData.boardData[y][x].state == 2)
                    break;
                else if (packet.mapData.boardData[y][x].state == 3)
                    break;
            }
            for (int j = 1; j < Player->GetBallonLength(); j++)   // up
            {
                int x = (Player->ballon[num]->x) / 60;
                int y = ((Player->ballon[num]->y) + 60 * j) / 60;
                if (packet.mapData.boardData[y][x].state == 1 || packet.mapData.boardData[y][x].state == 4 || packet.mapData.boardData[y][x].state == 40 || packet.mapData.boardData[y][x].state == 6 || packet.mapData.boardData[y][x].state == 7 || packet.mapData.boardData[y][x].state == 8 || packet.mapData.boardData[y][x].state == 9)
                    packet.mapData.boardData[y][x].SetState(5);
                else if (packet.mapData.boardData[y][x].state == 2)
                    break;
                else if (packet.mapData.boardData[y][x].state == 3)
                    break;
            }
            for (int j = 1; j < Player->GetBallonLength(); j++) // down
            {
                int x = (Player->ballon[num]->x) / 60;
                int y = ((Player->ballon[num]->y) - 60 * j) / 60;
                if (packet.mapData.boardData[y][x].state == 1 || packet.mapData.boardData[y][x].state == 4 || packet.mapData.boardData[y][x].state == 40 || packet.mapData.boardData[y][x].state == 6 || packet.mapData.boardData[y][x].state == 7 || packet.mapData.boardData[y][x].state == 8 || packet.mapData.boardData[y][x].state == 9)
                    packet.mapData.boardData[y][x].SetState(5);
                else if (packet.mapData.boardData[y][x].state == 2)
                    break;
                else if (packet.mapData.boardData[y][x].state == 3)
                    break;

            }
            Player->ballon[num]->beforeboomcount = 0.0f;
            Player->ballon[num]->startboomcount += fTimeElapsed;
        }

        if (Player->ballon[num]->startboomcount >= STARTBOOMCOUNT)
        {
            packet.mapData.boardData[(Player->ballon[num]->y) / 60][(Player->ballon[num]->x) / 60].SetState(5);
            for (int j = 1; j < Player->GetBallonLength(); j++)
            {
                int x = ((Player->ballon[num]->x) + 60 * j) / 60;
                int y = (Player->ballon[num]->y) / 60;
                if (x < 0 || x == 15)
                    break;
                if (packet.mapData.boardData[y][x].state == 1 || packet.mapData.boardData[y][x].state == 4 || packet.mapData.boardData[y][x].state == 40 || packet.mapData.boardData[y][x].state == 6 || packet.mapData.boardData[y][x].state == 7 || packet.mapData.boardData[y][x].state == 8 || packet.mapData.boardData[y][x].state == 9)
                    packet.mapData.boardData[y][x].SetState(5);
                else if (packet.mapData.boardData[y][x].state == 2)
                {
                    ObstacleBreak[y][x] = 1;
                    break;
                }
                else if (packet.mapData.boardData[y][x].state == 3)
                    break;
            }
            for (int j = 1; j < Player->GetBallonLength(); j++)
            {
                int x = ((Player->ballon[num]->x) - 60 * j) / 60;
                int y = (Player->ballon[num]->y) / 60;
                if (x < 0 || x == 15)
                    break;
                if (packet.mapData.boardData[y][x].state == 1 || packet.mapData.boardData[y][x].state == 4 || packet.mapData.boardData[y][x].state == 40 || packet.mapData.boardData[y][x].state == 6 || packet.mapData.boardData[y][x].state == 7 || packet.mapData.boardData[y][x].state == 8 || packet.mapData.boardData[y][x].state == 9)
                    packet.mapData.boardData[y][x].SetState(5);
                else if (packet.mapData.boardData[y][x].state == 2)
                {
                    ObstacleBreak[y][x] = 1;
                    break;
                }
                else if (packet.mapData.boardData[y][x].state == 3)
                    break;
            }
            for (int j = 1; j < Player->GetBallonLength(); j++)
            {
                int x = (Player->ballon[num]->x) / 60;
                int y = ((Player->ballon[num]->y) + 60 * j) / 60;
                if (packet.mapData.boardData[y][x].state == 1 || packet.mapData.boardData[y][x].state == 4 || packet.mapData.boardData[y][x].state == 40 || packet.mapData.boardData[y][x].state == 6 || packet.mapData.boardData[y][x].state == 7 || packet.mapData.boardData[y][x].state == 8 || packet.mapData.boardData[y][x].state == 9)
                    packet.mapData.boardData[y][x].SetState(5);
                else if (packet.mapData.boardData[y][x].state == 2)
                {
                    ObstacleBreak[y][x] = 1;
                    break;
                }
                else if (packet.mapData.boardData[y][x].state == 3)
                    break;
            }
            for (int j = 1; j < Player->GetBallonLength(); j++)
            {
                int x = (Player->ballon[num]->x) / 60;
                int y = ((Player->ballon[num]->y) - 60 * j) / 60;
                if (packet.mapData.boardData[y][x].state == 1 || packet.mapData.boardData[y][x].state == 4 || packet.mapData.boardData[y][x].state == 40 || packet.mapData.boardData[y][x].state == 6 || packet.mapData.boardData[y][x].state == 7 || packet.mapData.boardData[y][x].state == 8 || packet.mapData.boardData[y][x].state == 9)
                    packet.mapData.boardData[y][x].SetState(5);
                else if (packet.mapData.boardData[y][x].state == 2)
                {
                    ObstacleBreak[y][x] = 1;
                    break;
                }
                else if (packet.mapData.boardData[y][x].state == 3)
                    break;

            }
            Player->ballon[num]->startboomcount += fTimeElapsed;
            Player->ballon[num]->SetState(0);
            for (int i = 0; i < 15; i++)
                for (int j = 0; j < 13; j++)
                    if (ObstacleBreak[j][i] == 1)
                    {
                        int n = uid(dre);
                        if (n == 6)
                            packet.mapData.boardData[j][i].SetState(6);
                        else if (n == 7)
                            packet.mapData.boardData[j][i].SetState(7);
                        else if (n == 8)
                            packet.mapData.boardData[j][i].SetState(8);
                        else
                            packet.mapData.boardData[j][i].SetState(1);
                    }
            for (int i = 0; i < 15; i++)
                for (int j = 0; j < 13; j++)
                    ObstacleBreak[j][i] = 0;
            packet.mapData.boardData[(Player->ballon[num]->y) / 60][(Player->ballon[num]->x) / 60].SetState(1);
            for (int j = 1; j <= Player->GetBallonLength(); j++)
            {
                int x = ((Player->ballon[num]->x) + 60 * j) / 60;
                int y = (Player->ballon[num]->y) / 60;
                if (packet.mapData.boardData[y][x].state == 5)
                    packet.mapData.boardData[y][x].SetState(1);
                else if (packet.mapData.boardData[y][x].state == 2)
                    break;
                else if (packet.mapData.boardData[y][x].state == 3)
                    break;
            }
            for (int j = 1; j <= Player->GetBallonLength(); j++)
            {
                int x = ((Player->ballon[num]->x) - 60 * j) / 60;
                int y = (Player->ballon[num]->y) / 60;
                if (x < 0 || x == 15)
                    break;
                if (packet.mapData.boardData[y][x].state == 5)
                    packet.mapData.boardData[y][x].SetState(1);
                else if (packet.mapData.boardData[y][x].state == 2)
                    break;
                else if (packet.mapData.boardData[y][x].state == 3)
                    break;
            }
            for (int j = 1; j <= Player->GetBallonLength(); j++)
            {
                int x = (Player->ballon[num]->x) / 60;
                int y = ((Player->ballon[num]->y) + 60 * j) / 60;
                if (packet.mapData.boardData[y][x].state == 5)
                    packet.mapData.boardData[y][x].SetState(1);
                else if (packet.mapData.boardData[y][x].state == 2)
                    break;
                else if (packet.mapData.boardData[y][x].state == 3)
                    break;
            }
            for (int j = 1; j <= Player->GetBallonLength(); j++)
            {
                int x = (Player->ballon[num]->x) / 60;
                int y = ((Player->ballon[num]->y) - 60 * j) / 60;
                if (packet.mapData.boardData[y][x].state == 5)
                    packet.mapData.boardData[y][x].SetState(1);
                else if (packet.mapData.boardData[y][x].state == 2)
                    break;
                else if (packet.mapData.boardData[y][x].state == 3)
                    break;
            }
            Player->ballon[num]->startboomcount = 0.0f;
        }
    }
}