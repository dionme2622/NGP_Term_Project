#include "Player.h"

CPlayer::CPlayer(int map_num, int playerID)
{
	// 맵에 따라 나뉨
	if (map_num == 1)		// Map 이 빌리지 라면
	{
		// Player1 초기화
		if (playerID == 1) {
			// TODO : Player1 초기화
			SetPosition(810, 725);
		}

		else {
			// TODO : Player2 초기화
			SetPosition(90, 725);
		}
	}
	else					// Map 이 Pirate 라면
	{
		// Player1 초기화
		if (playerID == 1) {
			// TODO : Player1 초기화
			SetPosition(750, 725);
		}

		else {
			// TODO : Player2 초기화
			SetPosition(150, 725);
		}
	}
	direction = DIR_DOWN;
	stop = false;
	temp_speed = 100;
	speed = temp_speed;
	state = LIVE;
	ballon_length = 2;
	ballon_num = 1;
	count = 0.0f;
	needle = false;
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
	x = static_cast<int>(fx);  // 부동소수점 좌표를 정수로 변환
	y = static_cast<int>(fy);
}

void CPlayer::EscapeBallon()
{
	if (needle) {
		SetState(ESCAPE);
		needle = false;		// 바늘아이템을 소유하고 있다면 물풍선을 탈출하고 false로 만든다.
	}
}

void CPlayer::Move(float fTimeElapsed)
{
	if (!GetStop())
	{
		// 임시 좌표 변수로 이동 후의 좌표를 계산
		float newX = x;
		float newY = y;

		if (direction == DIR_DOWN) newY += speed * (fTimeElapsed * 1.4); // down
		else if (direction == DIR_LEFT) newX -= speed * fTimeElapsed;   // left
		else if (direction == DIR_UP) newY -= speed * fTimeElapsed;    // up
		else if (direction == DIR_RIGHT) newX += speed * (fTimeElapsed * 1.4); // right

		// 새로운 좌표를 검사하고 범위를 벗어나지 않도록 제한
		if (newX >= 30 && newX <= 870) x = newX; // X축 범위 검사
		if (newY >= 65 && newY <= 785) y = newY; // Y축 범위 검사
	}
}
