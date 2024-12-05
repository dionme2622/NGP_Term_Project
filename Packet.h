#pragma once
#include "Player.h"
#include "GameObject.h"
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
    //CBallon*    ballon[6];		// ��ǳ��

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
        needle = playerData->needle;
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
        playerData->needle = needle;
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

typedef struct CS_PlayerInputPacket {
    int playerID = 0;           // � Ŭ���̾�Ʈ���� Key�� �Է��ߴ��� �˷��ֱ� ���� ID��
    int keyState = 0;           // �Է��� Key �� ��
} CS_PlayerInputPacket;


typedef struct SC_PlayersInfoPacket {
    PlayerData playerData[2];           // Ŭ���̾�Ʈ���� Player�� �����͸� ������. TODO : Player2�� ������ ���Ŀ� ���� ������ ��
    // TODO : ����� ǳ���� ���� ������ ������ �Ѵ�
    MapData mapData;
    // Ballon Info
} SC_PlayersInfoPacket;