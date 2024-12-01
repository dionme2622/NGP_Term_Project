#pragma once

class PlayerData {
public:
    int			x, y;			// 캐릭터 x, y 좌표
    int			speed;			// 캐릭터의 이동속도
    int			ballon_length;	// 물풍선의 길이
    int			ballon_num;		// 물풍선의 개수
    int			direction;		// 캐릭터의 방향
    int			state;			// 캐릭터의 상태
    bool		stop;			// 캐릭터 정지 유무
    //CBallon* ballon[6];		    // 물풍선


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
    int playerID;           // 어떤 클라이언트에서 Key를 입력했는지 알려주기 위한 ID값
    int keyState;           // 입력한 Key 의 값
} CS_PlayerInputPacket;

typedef struct SC_PlayersInfoPacket {
    PlayerData playerData[2];           // 클라이언트에게 Player의 데이터를 보낸다.
    MapData mapData;                    // 클라이언트에게 Map의 데이터를 보낸다.
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