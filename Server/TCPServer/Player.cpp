#include "Player.h"

CPlayer::CPlayer()
{
	SetPosition(200, 200);
	direction = DIR_DOWN;
	stop = true;
	speed = 150;
	state = LIVE;
	ballon_length = 2;
	ballon_num = 1;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(float fTimeElapsed)
{
	Move(direction, fTimeElapsed);
}


void CPlayer::SetPosition(float _fx, float _fy)
{
	fx = _fx;
	fy = _fy;
	x = static_cast<int>(fx);  // 부동소수점 좌표를 정수로 변환
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

	printf("플레이어 위치 X: %d, Y: %d\n", x, y);
}
