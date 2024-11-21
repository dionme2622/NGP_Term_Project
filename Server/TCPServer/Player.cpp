#include "Player.h"

CPlayer::CPlayer()
{
	x = 50, y = 50;
	fx = 50.0f, fy = 50.0f;
	stop = false;
	speed = 150;
}

CPlayer::~CPlayer()
{
}

void CPlayer::SetDirection(int)
{
}

void CPlayer::SetPosition(float _fx, float _fy)
{
	fx = _fx;
	fy = _fy;
	x = static_cast<int>(fx);  // �ε��Ҽ��� ��ǥ�� ������ ��ȯ
	y = static_cast<int>(fy);
}

void CPlayer::Move(int dir, float fTimeElapsed)
{
	if (!GetStop())
	{
		if (dir == 40) fy += speed * fTimeElapsed;			// down
		else if (dir == 37) fx -= speed * fTimeElapsed;		// left
		else if (dir == 38) fy -= speed * fTimeElapsed;		// up
		else if (dir == 39) fx += speed * fTimeElapsed;		// right

		SetPosition(fx, fy);
	}
	printf("�÷��̾� ��ġ X: %d, Y: %d\n", x, y);
}
