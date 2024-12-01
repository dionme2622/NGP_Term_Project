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
    //CBallon* ballon[6];		    // ��ǳ��


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
    int mapIndex;
    BoardData boardData[13][15];
};

typedef struct PacketHeader{
    int packetType; // 1=PlayerInput, 2=ReadyPacket
} PacketHeader;

typedef struct CS_PlayerInputPacket {
    PacketHeader header;
    int playerID;           // � Ŭ���̾�Ʈ���� Key�� �Է��ߴ��� �˷��ֱ� ���� ID��
    int keyState;           // �Է��� Key �� ��
} CS_PlayerInputPacket;

typedef struct SC_PlayersInfoPacket {
    PlayerData playerData[2];           // Ŭ���̾�Ʈ���� Player�� �����͸� ������.
    MapData mapData;                    // Ŭ���̾�Ʈ���� Map�� �����͸� ������.
} SC_PlayersInfoPacket;

typedef struct CS_LobbyPacket {
    PacketHeader header;
    int selectedMap;
    bool nextSceneCall;
}CS_LobbyPacket;

typedef struct SC_LobbyPacket {
    MapData mapData;
    bool nextSceneCall = false;
}SC_LobbyPacket;