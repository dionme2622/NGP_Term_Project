#include "Player.h"

CPlayer::CPlayer()
{
	SetPosition(400, 200);		
	direction = DIR_DOWN;
	stop = false;
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
	

	if (!GetStop())
	{
		if (direction == DIR_DOWN) fy += speed * fTimeElapsed;			// down
		else if (direction == DIR_LEFT) fx -= speed * fTimeElapsed;		// left
		else if (direction == DIR_UP) fy -= speed * fTimeElapsed;			// up
		else if (direction == DIR_RIGHT) fx += speed * fTimeElapsed;		// right

		SetPosition(fx, fy);
	}

}
