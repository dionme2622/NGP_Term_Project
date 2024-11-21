#pragma once
#include "Player.h"

typedef struct CS_PlayerInputPacket {
    int playerID;           // 어떤 클라이언트에서 Key를 입력했는지 알려주기 위한 ID값
    int keyState;           // 입력한 Key 의 값
} CS_PlayerInputPacket;


typedef struct SC_PlayersInfoPacket {
    CPlayer player1;           // 클라이언트에게 Player의 데이터를 보낸다. TODO : Player2의 정보도 추후에 같이 보내야 함
    // TODO : 보드와 풍선에 대한 정보도 보내야 한다
    // Board Info
    // Ballon Info
} SC_PlayersInfoPacket;

extern SC_PlayersInfoPacket receivedPacket;
extern int test;