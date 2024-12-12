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
    bool        needle;         // 바늘 보유 유무


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

typedef struct PacketHeader {           // 어떤 Scene에서 어떤 패킷을 사용해야 하는지 알려주기 위한 패킷 헤더
    int packetType;                     // 1=PlayerInput, 2=ReadyPacket
} PacketHeader;

typedef struct CS_PlayerInputPacket {
    PacketHeader header;                // 1: SC_PlayersInfoPacket 사용 2 : CS_LobbyPacket 사용
    int playerID;                       // 어떤 클라이언트에서 Key를 입력했는지 알려주기 위한 ID값
    int keyState;                       // 입력한 Key 의 값
} CS_PlayerInputPacket;

typedef struct SC_PlayersInfoPacket {   // PlayScene에서 사용할 패킷
    PlayerData playerData[2];           // 클라이언트에게 Player의 데이터를 보낸다.
    MapData mapData;                    // 클라이언트에게 Map의 데이터를 보낸다.
} SC_PlayersInfoPacket;

typedef struct CS_LobbyPacket {         // LobbyScene에서 사용할 패킷
    PacketHeader header;                // 1: PlayScene 2 : LobbyScene
    int selectedMap;                    // 1: PlayScene 2 : LobbyScene
    int nextSceneCall;                  // 1이 되면 LobbyScene에서 PlayScene으로 넘어간다.
}CS_LobbyPacket;

typedef struct SC_LobbyPacket {         // 서버로 부터 받는 LobbyPacket
    int selectedMap;                    // 맵 선택 시 send한 맵 변수를 다시 recv한다.
    bool playerExist[2];                // 플레이어가 connect 한다면 true로 변경되어 LobbyScene에 플레이어 이미지를 출력한다.
    int nextSceneCall;                  // 1이 되면 LobbyScene에서 PlayScene으로 넘어간다.
}SC_LobbyPacket;