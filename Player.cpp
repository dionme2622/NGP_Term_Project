#include "Player.h"

enum {
LIVE = 0, DAMAGE, DEAD, ESCAPE
};

CPlayer::CPlayer(HINSTANCE _hInst)
{
	MainBitmap[0] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_DOWN));			// 캐릭터 아래 모습
	MainBitmap[1] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_LEFT));			// 캐릭터 왼쪽 모습
	MainBitmap[2] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_RIGHT));         // 캐릭터 오른쪽 모습
	MainBitmap[3] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_UP));            // 캐릭터 위 모습
	MainBitmap[4] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_DAMAGE));        // 캐릭터 피격
	MainBitmap[5] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_DEAD));          // 캐릭터 사망
	MainBitmap[6] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_ESCAPE));        // 캐릭터 탈출
	MainBitmap[7] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_WIN));           // 캐릭터 승리
	MainBitmap[8] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_Arrow));         // 캐릭터 머리 위 화살표

	state = LIVE;
	fx = 0.0f, fy = 0.0f;
	xPos = 0, yPos = 0;
	xPosF = 0.0f, yPosF = 0.0f;
	speed = 150;
}

CPlayer::~CPlayer()
{
	// TODO : 비트맵 제거
}

void CPlayer::Update(float fTimeElapsed)
{
	if (state == LIVE)
	{
		if (!GetStop()) {
			float frameSpeed = 64.0f * 15 * fTimeElapsed;  // 부드러운 이동을 위한 속도 계산

			if (direction == DIR_DOWN || direction == DIR_UP) {
				xPosF += frameSpeed;  // 부동소수점 좌표 업데이트
				while (xPosF >= 64.0f) {  // 64보다 크면 한 칸씩 이동
					xPosF = 0.0f;  // 64만큼 넘으면 빼고
					xPos += 64;       // xPos는 64씩 증가
					if (xPos >= 512)   // 한 주기를 넘어가면 초기화
						xPos = 0;
				}
			}

			if (direction == DIR_LEFT || direction == DIR_RIGHT) {
				xPosF += frameSpeed;
				while (xPosF >= 64.0f) {  // 64보다 크면 한 칸씩 이동
					xPosF = 0.0f;
					xPos += 64;
					if (xPos >= 384)  // 한 주기를 넘어가면 초기화
						xPos = 0;
				}
			}
		}
		else {
			xPosF = 0.0f;
			xPos = 0;
		}
	}
	else if (state == DAMAGE)
	{
		xPosF += 88.0f * 5.0f * fTimeElapsed;
		while (xPosF >= 88.0f) {  // 88보다 크면 한 칸씩 이동
			if (xPos >= 1144) {
				break;  // xPos가 1232 이상이면 이동을 멈추고 while 루프를 종료
			}

			xPosF = 0.0f;  // 88만큼 넘으면 빼고
			xPos += 88;       // xPos는 88씩 증가

			if (xPos >= 1144) {
				xPos = 1144;  // xPos가 1232 이상이 되면 1232로 고정
				break; // xPos가 1232 이상이면 더 이상 이동하지 않음
			}
		}

		if (xPos == 1144) {
			// xPos가 1232에 도달하면 3초 타이머 시작
			static float stateChangeTime = 0.0f;  // 타이머 변수 (static으로 설정하여 값 유지)
			static bool bDeadStateTriggered = false; // 상태 변경 트리거 상태

			// 타이머 초기화는 한번만 하고 이후에는 계속 누적됨
			if (!bDeadStateTriggered) {
				bDeadStateTriggered = true;  // 상태 변경 트리거
			}

			// 경과한 시간을 확인하여 3초가 지나면 DEAD 상태로 전환
			stateChangeTime += fTimeElapsed;  // 경과 시간 추가
			printf("%f\n", stateChangeTime);

			if (stateChangeTime >= 2.0f) {  // 3초가 지나면
				xPosF = 0.0f;
				xPos = 0;
				state = DEAD;  // 상태를 DEAD로 변경
				bDeadStateTriggered = false; // 상태 변경 트리거 초기화
			}
		}
	}
	else if (state == DEAD) 
	{
		xPosF += 88.0f * 5.0f * fTimeElapsed;
		while (xPosF >= 88.0f) {  //88보다 크면 한 칸씩 이동
			xPosF = 0.0f;  // 64만큼 넘으면 빼고
			xPos += 88;       // xPos는 64씩 증가
			if (xPos >= 968)   // 한 주기를 넘어가면 초기화
				xPos = 0;
		}
	}
	else if (state == ESCAPE) 
	{
		xPosF += 88.0f * 10.0f * fTimeElapsed;
		while (xPosF >= 88.0f) {  //88보다 크면 한 칸씩 이동
			xPosF = 0.0f;  // 64만큼 넘으면 빼고
			xPos += 88;       // xPos는 64씩 증가
			if (xPos >= 616) xPos = 0;
		}
	}
	Move(fTimeElapsed);
	//printf("Player stop : %d\n", stop);	// DEBUG

	printf("Player x : %d, y : %d\n", x, y);	// DEBUG
}

void CPlayer::Render(HDC MemDC, HDC MemDCImage)
{
	if (state == LIVE)
	{
		speed = 150;
		if (direction == DIR_DOWN)
		{
			(HBITMAP)SelectObject(MemDCImage, MainBitmap[0]); //--- 아래
			if (!stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, xPos, yPos, 64, 76, RGB(255, 0, 255));
			if (stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, 0, yPos, 64, 76, RGB(255, 0, 255));
		}
		else if (direction == DIR_LEFT)
		{
			(HBITMAP)SelectObject(MemDCImage, MainBitmap[1]); //--- 왼쪽
			if (!stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, xPos, yPos, 64, 76, RGB(255, 0, 255));
			if (stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, 0, yPos, 64, 76, RGB(255, 0, 255));
		}
		else if (direction == DIR_RIGHT)
		{
			(HBITMAP)SelectObject(MemDCImage, MainBitmap[2]); //--- 오른쪽
			if (!stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, xPos, yPos, 64, 76, RGB(255, 0, 255));
			if (stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, 0, yPos, 64, 76, RGB(255, 0, 255));
		}
		else if (direction == DIR_UP)
		{
			(HBITMAP)SelectObject(MemDCImage, MainBitmap[3]); //--- 위
			if (!stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, xPos, yPos, 64, 76, RGB(255, 0, 255));
			if (stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, 0, yPos, 64, 76, RGB(255, 0, 255));
		}
	}
	else if (state == DAMAGE)
	{
		speed = 50;
		(HBITMAP)SelectObject(MemDCImage, MainBitmap[4]);
		TransparentBlt(MemDC, x - 10, y - 10, 70, 70, MemDCImage, xPos, yPos, 88, 144, RGB(255, 0, 255));
	}
	else if (state == DEAD)
	{
		speed = 0;
		(HBITMAP)SelectObject(MemDCImage, MainBitmap[5]);
		TransparentBlt(MemDC, x - 10, y - 10, 70, 70, MemDCImage, xPos, yPos, 88, 144, RGB(255, 0, 255));
	}
	else if (state == ESCAPE)
	{
		speed = 0;
		(HBITMAP)SelectObject(MemDCImage, MainBitmap[6]);
		TransparentBlt(MemDC, x - 10, y - 10, 70, 70, MemDCImage, xPos, yPos, 88, 144, RGB(255, 0, 255));
	}

	(HBITMAP)SelectObject(MemDCImage, MainBitmap[8]); //--- Player1 화살표
	TransparentBlt(MemDC, x + 20, y - 30, 24, 20, MemDCImage, 0, 0, 24, 28, RGB(255, 0, 255));
}

void CPlayer::SetDirection(int _direction)
{
	direction = _direction;
}


void CPlayer::SetPosition(float _fx, float _fy) {
	fx = _fx;
	fy = _fy;
	x = static_cast<int>(fx);  // 부동소수점 좌표를 정수로 변환
	y = static_cast<int>(fy);
}

void CPlayer::Move(float fTimeElapsed)
{
	if (!GetStop())
	{
		if (direction == DIR_DOWN) fy += speed * fTimeElapsed;
		else if (direction == DIR_LEFT) fx -= speed * fTimeElapsed;
		else if (direction == DIR_UP) fy -= speed * fTimeElapsed;
		else if (direction == DIR_RIGHT) fx += speed * fTimeElapsed;

		SetPosition(fx, fy);
	}

}
