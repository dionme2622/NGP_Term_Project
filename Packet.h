#pragma once
#include "Player.h"

class PlayerData {
public:
    int			x, y;			// 캐릭터 x, y 좌표
    float		fx, fy;
    int			speed;			// 캐릭터의 이동속도
    int			ballon_length;	// 물풍선의 길이
    int			ballon_num;		// 물풍선의 개수
    int			direction;		// 캐릭터의 방향
    int			state;			// 캐릭터의 상태
    bool		stop;			// 캐릭터 정지 유무
};

typedef struct CS_PlayerInputPacket {
    int playerID = 0;           // 어떤 클라이언트에서 Key를 입력했는지 알려주기 위한 ID값
    int keyState = 0;           // 입력한 Key 의 값
} CS_PlayerInputPacket;


typedef struct SC_PlayersInfoPacket {
    CPlayer player[2];           // 클라이언트에게 Player의 데이터를 보낸다. TODO : Player2의 정보도 추후에 같이 보내야 함
    // TODO : 보드와 풍선에 대한 정보도 보내야 한다
    // Board Info
    // Ballon Info
} SC_PlayersInfoPacket;

extern SC_PlayersInfoPacket receivedPacket;
extern int 테스트;