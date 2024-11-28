#include "Player.h"

CPlayer::CPlayer()
{
	SetPosition(400, 600);		
	direction = DIR_DOWN;
	stop = false;
	speed = 100;
	state = LIVE;
	ballon_length = 2;
	ballon_num = 1;
	
	for (int i = 0; i < 6; i++) ballon[i] = new CBallon();
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
	x = static_cast<int>(fx);  // 부동소수점 좌표를 정수로 변환
	y = static_cast<int>(fy);
}

void CPlayer::Move(float fTimeElapsed)
{
	

	//if (!GetStop())
	//{
	//	if (direction == DIR_DOWN) fy += speed * fTimeElapsed;			// down
	//	else if (direction == DIR_LEFT) fx -= speed * fTimeElapsed;		// left
	//	else if (direction == DIR_UP) fy -= speed * fTimeElapsed;			// up
	//	else if (direction == DIR_RIGHT) fx += speed * fTimeElapsed;		// right

	//	SetPosition(fx, fy);
	//}
	if (!GetStop())
	{
		if (direction == DIR_DOWN) y += speed * (fTimeElapsed * 1.4);			// down
		else if (direction == DIR_LEFT) x -= speed * fTimeElapsed;		// left
		else if (direction == DIR_UP) y -= speed * fTimeElapsed;			// up
		else if (direction == DIR_RIGHT) x += speed * (fTimeElapsed * 1.4);		// right

	}
}
