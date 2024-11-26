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

    void LoadFromPlayer(const CPlayer& player)
    {
        x = player.x;
        y = player.y;
        speed = player.speed;
        ballon_length = player.ballon_length;
        ballon_num = player.ballon_num;
        direction = player.direction;
        state = player.state;
        stop = player.stop;
    }
    void ApplyToPlayer(CPlayer& player)
    {
        player.x = x;
        player.y = y;
        player.speed = speed;
        player.ballon_length = ballon_length;
        player.ballon_num = ballon_num;
        player.direction = direction;
        player.state = state;
        player.stop = stop;
    }
};

typedef struct CS_PlayerInputPacket {
    int playerID;           // � Ŭ���̾�Ʈ���� Key�� �Է��ߴ��� �˷��ֱ� ���� ID��
    int keyState;           // �Է��� Key �� ��
} CS_PlayerInputPacket;

typedef struct SC_PlayersInfoPacket {
    PlayerData player[2];           // Ŭ���̾�Ʈ���� Player�� �����͸� ������.
} SC_PlayersInfoPacket;
