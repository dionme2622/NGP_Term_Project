#pragma once
#include "Player.h"

typedef struct CS_PlayerInputPacket {
<<<<<<< Updated upstream
    int playerID;           // � Ŭ���̾�Ʈ���� Key�� �Է��ߴ��� �˷��ֱ� ���� ID��
    int keyState;           // �Է��� Key �� ��
=======
    int header = 1;
    int playerID = 0;           // � Ŭ���̾�Ʈ���� Key�� �Է��ߴ��� �˷��ֱ� ���� ID��
    int keyState = 0;           // �Է��� Key �� ��
>>>>>>> Stashed changes
} CS_PlayerInputPacket;


typedef struct SC_PlayersInfoPacket {
    CPlayer player1;           // Ŭ���̾�Ʈ���� Player�� �����͸� ������. TODO : Player2�� ������ ���Ŀ� ���� ������ ��
    // TODO : ����� ǳ���� ���� ������ ������ �Ѵ�
    // Board Info
    // Ballon Info
} SC_PlayersInfoPacket;