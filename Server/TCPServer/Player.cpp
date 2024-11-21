#include "Player.h"

CPlayer::CPlayer()
{
	x = 50, y = 50;
	fx = 50.0f, fy = 50.0f;
	stop = true;
	speed = 150;
	state = LIVE;
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
	if (dir == DIR_STOP) stop = true;
	else stop = false;

	if (!GetStop())
	{
		if (dir == DIR_DOWN) fy += speed * fTimeElapsed;			// down
		else if (dir == DIR_LEFT) fx -= speed * fTimeElapsed;		// left
		else if (dir == DIR_UP) fy -= speed * fTimeElapsed;		// up
		else if (dir == DIR_RIGHT) fx += speed * fTimeElapsed;		// right

		SetPosition(fx, fy);
	}

	printf("�÷��̾� ��ġ X: %d, Y: %d\n", x, y);
}
