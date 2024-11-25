#include "Player.h"

CPlayer::CPlayer()
{
	SetPosition(250, 200);		
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
	Move(fTimeElapsed);
}


void CPlayer::SetPosition(float _fx, float _fy)
{
	fx = _fx;
	fy = _fy;
	x = static_cast<int>(fx);  // �ε��Ҽ��� ��ǥ�� ������ ��ȯ
	y = static_cast<int>(fy);
}

void CPlayer::Move(float fTimeElapsed)
{
	/*stop = true;

	if (dir == DIR_DOWN) SetDirection(DIR_DOWN), stop = false;
	else if (dir == DIR_LEFT) SetDirection(DIR_LEFT), stop = false;
	else if (dir == DIR_RIGHT) SetDirection(DIR_RIGHT), stop = false;
	else if (dir == DIR_UP) SetDirection(DIR_UP), stop = false;*/

	if (!GetStop())
	{
		if (direction == DIR_DOWN) fy += speed * fTimeElapsed;			// down
		else if (direction == DIR_LEFT) fx -= speed * fTimeElapsed;		// left
		else if (direction == DIR_UP) fy -= speed * fTimeElapsed;			// up
		else if (direction == DIR_RIGHT) fx += speed * fTimeElapsed;		// right

		SetPosition(fx, fy);
	}

	//printf("�÷��̾� ��ġ X: %d, Y: %d\n", x, y);
}
