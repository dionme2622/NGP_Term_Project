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

	state = LIVE;
	xPos = 0, yPos = 0;
	speed = 1;
}

CPlayer::~CPlayer()
{
	// TODO : 비트맵 제거
}

void CPlayer::Update(float fTimeElapsed)
{
	if (!GetStop())
	{
		if (direction == DIR_DOWN || direction == DIR_UP)
		{
			xPos += 64;
			if (xPos >= 512)
				xPos = 0;
		}
		if (direction == DIR_LEFT || direction == DIR_RIGHT)
		{
			xPos += 64;
			if (xPos >= 384)
				xPos = 0;
		}
	}
	else
		xPos = 0;

	Move(fTimeElapsed);
	printf("Player stop : %d\n", stop);

	printf("Player x : %d, y : %d\n", x, y);
}

void CPlayer::Render(HDC MemDC, HDC MemDCImage)
{
	if (state == LIVE)
	{
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
	/*else if (state == DAMAGE)
	{
		speed = 5;
		(HBITMAP)SelectObject(MemDCImage, MainBitmap[4]);
		TransparentBlt(MemDC, x - 10, y - 10, 70, 70, MemDCImage, dead_xPos, dead_yPos, 88, 144, RGB(255, 0, 255));
	}
	else if (state == DEAD)
	{
		speed = 0;
		(HBITMAP)SelectObject(MemDCImage, MainBitmap[5]);
		TransparentBlt(MemDC, x - 10, y - 10, 70, 70, MemDCImage, dead_xPos, dead_yPos, 88, 144, RGB(255, 0, 255));
	}
	else if (state == ESCAPE)
	{
		(HBITMAP)SelectObject(MemDCImage, MainBitmap[6]);
		TransparentBlt(MemDC, x - 10, y - 10, 70, 70, MemDCImage, dead_xPos, dead_yPos, 88, 144, RGB(255, 0, 255));
	}*/
	//printf("Cur Direction: %d\n", direction);
}

void CPlayer::SetDirection(int _direction)
{
	direction = _direction;
}

void CPlayer::SetPosition(int _x, int _y)
{
	x = _x, y = _y;
}

void CPlayer::Move(float fTimeElapsed)
{
	if (!GetStop())
	{
		if (direction == DIR_DOWN) SetPosition(x, (y + speed));

		else if (direction == DIR_LEFT) SetPosition(x - speed, y);

		else if (direction == DIR_UP) SetPosition(x, y - speed);

		else if (direction == DIR_RIGHT) SetPosition(x + speed, y);
	}

}
