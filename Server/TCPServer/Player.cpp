#include "Player.h"

CPlayer::CPlayer()
{
	SetPosition(400, 600);		
	direction = DIR_DOWN;
	stop = false;
	temp_speed = 100;
	speed = temp_speed;
	state = LIVE;
	ballon_length = 2;
	ballon_num = 1;
	count = 0.0f;
	needle = true;
	for (int i = 0; i < 6; i++) ballon[i] = new CBallon();
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(float fTimeElapsed)
{
	if (state == LIVE) speed = temp_speed, count = 0.0f;
	else if (state == DAMAGE)
	{
		speed = 50;
		count += fTimeElapsed;
		printf("count : %f\n", count);
		if (count > 4.0f) {
			count = 0; 
			SetState(DEAD);
		}
		printf("STATE: %d\n", state);
	}
	else if (state == DEAD) speed = 0;
	else if (state == ESCAPE) {
		speed = 0;
		count += fTimeElapsed;
		if (count > 0.5f) {
			count = 0;
			SetState(LIVE);
		}
	}
	Move(fTimeElapsed);
}


void CPlayer::SetPosition(float _fx, float _fy)
{
	fx = _fx;
	fy = _fy;
	x = static_cast<int>(fx);  // �ε��Ҽ��� ��ǥ�� ������ ��ȯ
	y = static_cast<int>(fy);
}

void CPlayer::EscapeBallon()
{
	if (needle) {
		SetState(ESCAPE);
		needle = false;		// �ٴþ������� �����ϰ� �ִٸ� ��ǳ���� Ż���ϰ� false�� �����.
	}
}

void CPlayer::Move(float fTimeElapsed)
{
	if (!GetStop())
	{
		if (direction == DIR_DOWN) y += speed * (fTimeElapsed * 1.4);			// down
		else if (direction == DIR_LEFT) x -= speed * fTimeElapsed;		// left
		else if (direction == DIR_UP) y -= speed * fTimeElapsed;			// up
		else if (direction == DIR_RIGHT) x += speed * (fTimeElapsed * 1.4);		// right

	}
}
