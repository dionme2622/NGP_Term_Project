#pragma once
#include "Player.h"
#include "GameObject.h"

class PlayerData {
public:
    int			x, y;			// 캐릭터 x, y 좌표
    int			speed;			// 캐릭터의 이동속도
    int			ballon_length;	// 물풍선의 길이
    int			ballon_num;		// 물풍선의 개수
    int			direction;		// 캐릭터의 방향
    int			state;			// 캐릭터의 상태
    bool		stop;			// 캐릭터 정지 유무
    //CBallon*    ballon[6];		// 물풍선

    void LoadFromPlayer(const CPlayer* playerData)
    {
        x = playerData->x;
        y = playerData->y;
        speed = playerData->speed;
        ballon_length = playerData->ballon_length;
        ballon_num = playerData->ballon_num;
        direction = playerData->direction;
        state = playerData->state;
        stop = playerData->stop;
    }
    void ApplyToPlayer(CPlayer* playerData)
    {
        playerData->x = x;
        playerData->y = y;
        playerData->speed = speed;
        playerData->ballon_length = ballon_length;
        playerData->ballon_num = ballon_num;
        playerData->direction = direction;
        playerData->state = state;
        playerData->stop = stop;
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

typedef struct PacketHeader {
    int packetType; // 1=PlayerInput, 2=ReadyPacket
} PacketHeader;


struct SSendLobbyPacket {
    PacketHeader header;
    int selectedMap;
    int nextSceneCall;
};


struct SRecvLobbyPacket {
    int selectedMap;
    int recvNextSceneCall;
};


typedef struct CS_PlayerInputPacket {
    PacketHeader header;
    int playerID = 0;           // 어떤 클라이언트에서 Key를 입력했는지 알려주기 위한 ID값
    int keyState = 0;           // 입력한 Key 의 값
} CS_PlayerInputPacket;


typedef struct SC_PlayersInfoPacket {
    PlayerData playerData[2];           // 클라이언트에게 Player의 데이터를 보낸다. TODO : Player2의 정보도 추후에 같이 보내야 함
    // TODO : 보드와 풍선에 대한 정보도 보내야 한다
    MapData mapData;
    // Ballon Info
} SC_PlayersInfoPacket;


