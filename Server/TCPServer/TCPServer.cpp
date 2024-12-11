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
<<<<<<< HEAD
int map_num;
=======
int map_num = 0;
//CPlayer* player2 = new CPlayer();
>>>>>>> parent of 44d4b8a (수정)

std::vector<SOCKET> clientSockets; // Ŭ���̾�Ʈ ���� ���
std::mutex clientMutex;            // Ŭ���̾�Ʈ ����Ʈ ��ȣ�� mutex
std::map<int, int> clientInput;

SC_PlayersInfoPacket packet;       // Client�� ������ ��Ŷ ����ü


void Map_Initialize(int);
void PlayerMeetObstacle(CPlayer* Player);
void PlayerGetItem(CPlayer* Player);
void BallonBoom(CPlayer* Player, int num, float fTimeElapsed);
void PlayerMeetBallon(CPlayer* Player);
void BallonToObstacle(CPlayer* Player);
void PlayerMeetPlayer(CPlayer* Player1, CPlayer* Player2);

std::default_random_engine dre;
std::uniform_int_distribution<int> uid{ 3,9 };

int ObstacleBreak[13][15];

int recv_all(SOCKET sock, char* buf, int len) {
    int total = 0;
    int bytes;
    while (total < len) {
        bytes = recv(sock, buf + total, len - total, 0);
        if (bytes <= 0) return bytes;  // ���� �Ǵ� ���� ����
        total += bytes;
    }
    return total;
}


static int check = 0;
SC_LobbyPacket sc_lobbyPacket;

DWORD WINAPI ClientThread(LPVOID arg) {
    SOCKET client_sock = (SOCKET)arg;
    int retval;

    while (1) {
        // 1. ����� �б�
        PacketHeader header;
   
        retval = recv(client_sock, (char*)&header, sizeof(header), 0);
        if (retval <= 0 || retval != sizeof(PacketHeader)) {
            printf("Ŭ���̾�Ʈ ���� ���� �Ǵ� ��� ���� ����\n");
            break;
        }

        // 2. ��Ŷ Ÿ�� Ȯ��
        

        // 3. ��Ŷ Ÿ�Կ� ���� ó��
        if (header.packetType == 1) { // PlayerInputPacket
            CS_PlayerInputPacket inputPacket;
            retval = recv(client_sock, (char*)&inputPacket + sizeof(PacketHeader),
                sizeof(CS_PlayerInputPacket) - sizeof(PacketHeader), 0);
            if (retval <= 0 || retval != (sizeof(CS_PlayerInputPacket) - sizeof(PacketHeader))) {
                printf("PlayerInput ��Ŷ ���� ����\n");
                break;
            }

            // KeyState ������Ʈ
            {
                std::lock_guard<std::mutex> lock(clientMutex);
                keyState[inputPacket.playerID] = inputPacket.keyState;
            }
            printf("ID[%d], Ű �Է�: %d\n", inputPacket.playerID, inputPacket.keyState);
        }
        else if (header.packetType == 2) { // LobbyPacket
            CS_LobbyPacket cs_lobbyPacket;
            retval = recv_all(client_sock, (char*)&cs_lobbyPacket + sizeof(PacketHeader),
                sizeof(CS_LobbyPacket) - sizeof(PacketHeader));
            if (retval <= 0 || retval != (sizeof(CS_LobbyPacket) - sizeof(PacketHeader))) {
                printf("LobbyPacket ���� ����\n");
                break;
            }
            map_num = cs_lobbyPacket.selectedMap;
            sc_lobbyPacket.selectedMap = cs_lobbyPacket.selectedMap;
            sc_lobbyPacket.nextSceneCall = cs_lobbyPacket.nextSceneCall;

            Map_Initialize(map_num);                
                   
            if(sc_lobbyPacket.nextSceneCall == 1)
                check = 1;
            

            // Ŭ���̾�Ʈ�鿡�� ��ε�ĳ��Ʈ
            std::lock_guard<std::mutex> lock(clientMutex);          // Ŭ���̾�Ʈ���� �����͸� ������ ���� �����Ͱ� �ٲ��� �ʰ� �ϱ� ���ؼ�
            for (SOCKET sock : clientSockets) {
                retval = send(sock, (char*)&sc_lobbyPacket, sizeof(sc_lobbyPacket), 0);
                printf("�κ���Ŷ ����\n");
                if (retval == SOCKET_ERROR) {
                    printf("Ŭ���̾�Ʈ�� ������ ���� ����. ���� ����.\n");
                    closesocket(sock);
                    clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), sock), clientSockets.end());
                }
                else {
                    printf("Ŭ���̾�Ʈ�� SC_LobbyPacket ���� �Ϸ�.\n");
                }
            }
        }
        else {

            break;
        }
    }

    // ���� ���� ó��
    closesocket(client_sock);
    {
        std::lock_guard<std::mutex> lock(clientMutex);
        clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_sock), clientSockets.end());
    }
    check = 0;
    printf("Ŭ���̾�Ʈ ó�� ����\n");
    return 0;
}

// ��� Ŭ���̾�Ʈ���� �ֱ������� ������ �����ϴ� ������
void GameLogicThread() {
    while (1) {
        if (check == 1)
        {
            m_GameTimer.Tick(120.0f);
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 10ms ���� ����

            // ��� �÷��̾� ���� ������Ʈ
            for (int playerID = 0; playerID < 2; ++playerID) {
                player[playerID]->stop = true;

                // ���� �÷��̾��� �Է� ���¿� ���� ó��
                {
                    std::lock_guard<std::mutex> lock(clientMutex); // keyState ���� ����ȭ
                    if (keyState[playerID] & KEY_DOWN) player[playerID]->SetDirection(DIR_DOWN), player[playerID]->stop = false;
                    if (keyState[playerID] & KEY_LEFT) player[playerID]->SetDirection(DIR_LEFT), player[playerID]->stop = false;
                    if (keyState[playerID] & KEY_RIGHT) player[playerID]->SetDirection(DIR_RIGHT), player[playerID]->stop = false;
                    if (keyState[playerID] & KEY_UP) player[playerID]->SetDirection(DIR_UP), player[playerID]->stop = false;
                    if (keyState[playerID] & KEY_SHIFT) player[playerID]->EscapeBallon();
                    if (keyState[playerID] & KEY_SPACE) {
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

                // ĳ���� �̵�
                player[playerID]->Update(m_GameTimer.GetTimeElapsed());
                PlayerMeetObstacle(player[playerID]);       // �÷��̾�� ��ֹ� �浹ó��
                PlayerGetItem(player[playerID]);            // �÷��̾�� ������ �浹ó��
                BallonBoom(player[playerID], 0, m_GameTimer.GetTimeElapsed());
                BallonBoom(player[playerID], 1, m_GameTimer.GetTimeElapsed());
                BallonBoom(player[playerID], 2, m_GameTimer.GetTimeElapsed());
                BallonBoom(player[playerID], 3, m_GameTimer.GetTimeElapsed());
                BallonBoom(player[playerID], 4, m_GameTimer.GetTimeElapsed());
                BallonBoom(player[playerID], 5, m_GameTimer.GetTimeElapsed());
                PlayerMeetBallon(player[playerID]);
                BallonToObstacle(player[playerID]);
                PlayerMeetPlayer(player[0], player[1]);
                PlayerMeetPlayer(player[1], player[0]);
            }
            
            // �÷��̾� ������ ������Ʈ
            for (int playerID = 0; playerID < 2; playerID++) playerData[playerID]->LoadFromPlayer(player[playerID]);
            // ��� �÷��̾� �����͸� Ŭ���̾�Ʈ�鿡�� ����
            for (int i = 0; i < 2; ++i) {
                memcpy(&packet.playerData[i], playerData[i], sizeof(PlayerData));   // Player���� �����͸� Ŭ���̾�Ʈ���� ������.
            }

            clientMutex.lock();
            for (SOCKET client_sock : clientSockets) {
                int retval = send(client_sock, (char*)&packet, sizeof(packet), 0);
                printf("��Ŷ ����\n");

                if (retval == SOCKET_ERROR) {
                    printf("Ŭ���̾�Ʈ�� ������ ���� ����. ���� ����.\n");
                    closesocket(client_sock);
                    clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_sock), clientSockets.end());
                }
            }
            clientMutex.unlock();
        }
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
    //gameLogicThread.detach();

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
        sc_lobbyPacket.playerExist[ID] = true;
        ++ID;
        

        // ������ Ŭ���̾�Ʈ ���� ���
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));
        // ������ Ŭ���̾�Ʈ ���� �߰�
        clientMutex.lock();
        clientSockets.push_back(client_sock);       // ���ؽ��� �����尡 ���� ��?? 
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

void Map_Initialize(int map_num) {             // ���� �ʱ�ȭ
    if (map_num == 1) // cs_lobbyPacket.selectedMap = 1�̸� Village ���� �ʱ�ȭ �Ѵ�.
    {
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
    else if (map_num == 2)  // cs_lobbyPacket.selectedMap = 2�̸� Pirate ���� �ʱ�ȭ �Ѵ�.
    {
        for (int i = 0; i < 4; i++)
        {
            packet.mapData.boardData[0][i].SetState(2);
        }
        packet.mapData.boardData[0][7].SetState(2);
        for (int i = 11; i < 15; i++)
        {
            packet.mapData.boardData[0][i].SetState(2);
        }
        // 1��
        packet.mapData.boardData[1][0].SetState(2);
        packet.mapData.boardData[1][1].SetState(3);
        packet.mapData.boardData[1][2].SetState(2);
        for (int i = 4; i < 7; i++)
        {
            packet.mapData.boardData[1][i].SetState(2);
        }
        for (int i = 8; i < 11; i++)
        {
            packet.mapData.boardData[1][i].SetState(2);
        }
        packet.mapData.boardData[1][12].SetState(2);
        packet.mapData.boardData[1][13].SetState(3);
        packet.mapData.boardData[1][14].SetState(2);
        // 2��
        packet.mapData.boardData[2][0].SetState(2);
        packet.mapData.boardData[2][1].SetState(2);
        packet.mapData.boardData[2][3].SetState(2);
        packet.mapData.boardData[2][5].SetState(2);
        packet.mapData.boardData[2][7].SetState(2);
        packet.mapData.boardData[2][9].SetState(2);
        packet.mapData.boardData[2][11].SetState(2);
        packet.mapData.boardData[2][13].SetState(2);
        packet.mapData.boardData[2][14].SetState(2);
        // 3��
        for (int i = 3; i < 13; i++) packet.mapData.boardData[i][0].SetState(2);
        for (int i = 7; i < 11; i++) packet.mapData.boardData[i][1].SetState(2);

        for (int i = 8; i < 10; i++) packet.mapData.boardData[i][2].SetState(2);

        for (int i = 9; i < 13; i++) packet.mapData.boardData[i][3].SetState(2);

        for (int i = 10; i < 13; i++) packet.mapData.boardData[i][4].SetState(2);

        for (int i = 11; i < 13; i++) packet.mapData.boardData[i][5].SetState(2);

        for (int i = 6; i < 12; i++) packet.mapData.boardData[12][i].SetState(2);

        for (int i = 9; i < 12; i++) packet.mapData.boardData[11][i].SetState(2);

        for (int i = 3; i < 13; i++) packet.mapData.boardData[i][14].SetState(2);

        for (int i = 10; i < 12; i++) packet.mapData.boardData[10][i].SetState(2);

        for (int i = 11; i < 14; i++) packet.mapData.boardData[9][i].SetState(2);

        for (int i = 12; i < 14; i++) packet.mapData.boardData[8][i].SetState(2);

        packet.mapData.boardData[7][13].SetState(2);
        for (int i = 4; i < 11; i++) packet.mapData.boardData[3][i].SetState(2);

        for (int i = 3; i < 12; i++) packet.mapData.boardData[4][i].SetState(2);
        for (int i = 4; i < 11; i++) packet.mapData.boardData[5][i].SetState(2);

        for (int i = 3; i < 6; i++) packet.mapData.boardData[6][i].SetState(2);

        for (int i = 9; i < 12; i++) packet.mapData.boardData[6][i].SetState(2);

        for (int i = 5; i < 10; i++)
        {
            packet.mapData.boardData[7][i].SetState(2);
            packet.mapData.boardData[8][i].SetState(2);
        }
        packet.mapData.boardData[9][7].SetState(2);
        for (int i = 3; i < 7; i++)
        {
            packet.mapData.boardData[i][2].SetState(2);
            packet.mapData.boardData[i][12].SetState(2);
        }
        packet.mapData.boardData[7][3].SetState(2);
        packet.mapData.boardData[7][11].SetState(2);
        packet.mapData.boardData[8][4].SetState(2);
        packet.mapData.boardData[8][10].SetState(2);
        packet.mapData.boardData[9][5].SetState(2);
        packet.mapData.boardData[9][9].SetState(2);
        for (int i = 6; i < 9; i++) packet.mapData.boardData[10][i].SetState(2);
        packet.mapData.boardData[10][13].SetState(2);
        packet.mapData.boardData[11][1].SetState(3);
        packet.mapData.boardData[11][13].SetState(3);
        packet.mapData.boardData[6][6].SetState(3); packet.mapData.boardData[6][7].SetState(3); packet.mapData.boardData[6][8].SetState(3);
    }
}

void PlayerMeetObstacle(CPlayer* Player)         // ���� ��
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

    if (packet.mapData.boardData[j][i].state == 6)		// �̵��ӵ� ������
    {
        if (Player->temp_speed < 200)
            Player->temp_speed += 20;
        printf("Player speed : %d\n", Player->temp_speed);
        packet.mapData.boardData[j][i].state = 1;

    }
    else if (packet.mapData.boardData[j][i].state == 7)		// ���ٱ� ������
    {
        if (Player->ballon_length < 6)
            Player->ballon_length += 1;
        packet.mapData.boardData[j][i].state = 1;
    }
    else if (packet.mapData.boardData[j][i].state == 8)		// ��ǳ�� ���� ������
    {
        if (Player->ballon_num < 5)
            Player->ballon_num += 1;
        packet.mapData.boardData[j][i].state = 1;
    }
    else if (packet.mapData.boardData[j][i].state == 9)		// �ٴ� ������
    {
        Player->needle = true;
        packet.mapData.boardData[j][i].state = 1;
    }
}
void BallonBoom(CPlayer* Player, int num, float fTimeElapsed)
{
    ////��ǳ�� ���� ������ ��
    if (Player->ballon[num]->GetState() == 1)
    {
        if (packet.mapData.boardData[(Player->ballon[num]->y) / 60][(Player->ballon[num]->x) / 60].state == 5)
            Player->ballon[num]->beforeboomcount = BEFOREBOOMCOUNT;
        if (Player->ballon[num]->beforeboomcount < BEFOREBOOMCOUNT)     // ǳ���� 1.5�ʰ� ������ ������
            Player->ballon[num]->beforeboomcount += fTimeElapsed;
        else
            Player->ballon[num]->SetState(2);
    }
    else if (Player->ballon[num]->GetState() == 2)
    {
        if (Player->ballon[num]->startboomcount < STARTBOOMCOUNT)       // ������ ���ӽð��� 0.5�� �̴�
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
                        else if (n == 9)
                            packet.mapData.boardData[j][i].SetState(9);
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
void PlayerMeetBallon(CPlayer* Player)
{
    int i = (Player->x + 30 - 30) / 60;
    int j = (Player->y + 30 - 65) / 60;
    {
        if (packet.mapData.boardData[j][i].state == 5)
        {
            if (Player->state == LIVE)
                Player->state = DAMAGE;
        }
    }
}
void BallonToObstacle(CPlayer* Player)
{
    for (int i = 0; i < Player->ballon_num; i++)
    {
        int xdis = abs(Player->x + 30 - (Player->ballon[i]->x + 30 + 30));
        int ydis = abs(Player->y + 30 - (Player->ballon[i]->y + 30 + 65));
        int dis;
        if (xdis > ydis)
            dis = xdis;
        else
            dis = ydis;
        if (Player->ballon[i]->state == 1 && dis >= 60)
            packet.mapData.boardData[Player->ballon[i]->y / 60][Player->ballon[i]->x / 60].state = 40;
    }
}
void PlayerMeetPlayer(CPlayer* Player1, CPlayer* Player2)
{
    if (Player2->state == DAMAGE)
    {
        if ((Player1->x < Player2->x + 60) && (Player2->x < Player1->x + 60) && (Player1->y < Player2->y + 60) && (Player2->y < Player1->y + 60))
        {
            Player2->state = DEAD;
        }
    }
}