#pragma once
#include "Player.h"

class PlayerData {
public:
    int			x, y;			// ĳ���� x, y ��ǥ
    float		fx, fy;
    int			speed;			// ĳ������ �̵��ӵ�
    int			ballon_length;	// ��ǳ���� ����
    int			ballon_num;		// ��ǳ���� ����
    int			direction;		// ĳ������ ����
    int			state;			// ĳ������ ����
    bool		stop;			// ĳ���� ���� ����
};

typedef struct CS_PlayerInputPacket {
    int playerID = 0;           // � Ŭ���̾�Ʈ���� Key�� �Է��ߴ��� �˷��ֱ� ���� ID��
    int keyState = 0;           // �Է��� Key �� ��
} CS_PlayerInputPacket;


typedef struct SC_PlayersInfoPacket {
    CPlayer player[2];           // Ŭ���̾�Ʈ���� Player�� �����͸� ������. TODO : Player2�� ������ ���Ŀ� ���� ������ ��
    // TODO : ����� ǳ���� ���� ������ ������ �Ѵ�
    // Board Info
    // Ballon Info
} SC_PlayersInfoPacket;

extern SC_PlayersInfoPacket receivedPacket;
extern int �׽�Ʈ;