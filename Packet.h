#pragma once
#include "Player.h"

typedef struct CS_PlayerInputPacket {
    int playerID = 0;           // � Ŭ���̾�Ʈ���� Key�� �Է��ߴ��� �˷��ֱ� ���� ID��
    int keyState = 0;           // �Է��� Key �� ��
} CS_PlayerInputPacket;


typedef struct SC_PlayersInfoPacket {
    CPlayer player1;           // Ŭ���̾�Ʈ���� Player�� �����͸� ������. TODO : Player2�� ������ ���Ŀ� ���� ������ ��
    // TODO : ����� ǳ���� ���� ������ ������ �Ѵ�
    // Board Info
    // Ballon Info
} SC_PlayersInfoPacket;

extern SC_PlayersInfoPacket receivedPacket;