#pragma once

class PlayerData {
public:
    int			x, y;			// ĳ���� x, y ��ǥ
    int			speed;			// ĳ������ �̵��ӵ�
    int			ballon_length;	// ��ǳ���� ����
    int			ballon_num;		// ��ǳ���� ����
    int			direction;		// ĳ������ ����
    int			state;			// ĳ������ ����
    bool		stop;			// ĳ���� ���� ����
    bool        needle;         // �ٴ� ���� ����


    void LoadFromPlayer(const CPlayer* player)
    {
        x = player->x;
        y = player->y;
        speed = player->speed;
        ballon_length = player->ballon_length;
        ballon_num = player->ballon_num;
        direction = player->direction;
        state = player->state;
        stop = player->stop;
        needle = player->needle;
    }
    void ApplyToPlayer(CPlayer* player)
    {
        player->x = x;
        player->y = y;
        player->speed = speed;
        player->ballon_length = ballon_length;
        player->ballon_num = ballon_num;
        player->direction = direction;
        player->state = state;
        player->stop = stop;
        player->needle = needle;
    }
};

class BoardData {
public:
    int state;
    int block;

    void SetState(int _state) { state = _state; }
};

class MapData {
public:
    BoardData boardData[13][15];
};

typedef struct PacketHeader {           // � Scene���� � ��Ŷ�� ����ؾ� �ϴ��� �˷��ֱ� ���� ��Ŷ ���
    int packetType;                     // 1=PlayerInput, 2=ReadyPacket
} PacketHeader;

typedef struct CS_PlayerInputPacket {
    PacketHeader header;                // 1: SC_PlayersInfoPacket ��� 2 : CS_LobbyPacket ���
    int playerID;                       // � Ŭ���̾�Ʈ���� Key�� �Է��ߴ��� �˷��ֱ� ���� ID��
    int keyState;                       // �Է��� Key �� ��
} CS_PlayerInputPacket;

typedef struct SC_PlayersInfoPacket {   // PlayScene���� ����� ��Ŷ
    PlayerData playerData[2];           // Ŭ���̾�Ʈ���� Player�� �����͸� ������.
    MapData mapData;                    // Ŭ���̾�Ʈ���� Map�� �����͸� ������.
} SC_PlayersInfoPacket;

typedef struct CS_LobbyPacket {         // LobbyScene���� ����� ��Ŷ
    PacketHeader header;                // 1: PlayScene 2 : LobbyScene
    int selectedMap;                    // 1: PlayScene 2 : LobbyScene
    int nextSceneCall;                  // 1�� �Ǹ� LobbyScene���� PlayScene���� �Ѿ��.
}CS_LobbyPacket;

typedef struct SC_LobbyPacket {         // ������ ���� �޴� LobbyPacket
    int selectedMap;                    // �� ���� �� send�� �� ������ �ٽ� recv�Ѵ�.
    bool playerExist[2];                // �÷��̾ connect �Ѵٸ� true�� ����Ǿ� LobbyScene�� �÷��̾� �̹����� ����Ѵ�.
    int nextSceneCall;                  // 1�� �Ǹ� LobbyScene���� PlayScene���� �Ѿ��.
}SC_LobbyPacket;