#pragma once
#include "GameObject.h"


class CPlayer
{
public:
	CPlayer(HINSTANCE _hInst);
	~CPlayer();

	void Update(float fTimeElapsed);
	void Render(HDC MemDC, HDC MemDCImage);
private:
	int			x, y;			// 캐릭터 x, y 좌표
	int			speed;			// 캐릭터의 이동속도
	int			ballon_length;	// 물풍선의 길이
	int			ballon_num;		// 물풍선의 개수
	int			direction;		// 캐릭터의 방향
	int			state;			// 캐릭터의 상태
	bool		stop;			// 캐릭터 정지 유무
	int			count;			// 뭔지 모름
	CBallon		ballon[6];		// 물풍선

	int			xPos, yPos;		// 이미지 x, y 좌표
private:
	HBITMAP MainBitmap[8];
public:
	void SetDirection(int);			// Player의 방향벡터를 설정하는 함수
	void SetPosition(int _x, int _y);				// Player의 위치 Set

	void SetStop(bool _stop) { stop = _stop; }
	bool GetStop() { return stop; }

	void Move(float fTimeElapsed);
	//이건 뭐지
	/*int count;
	int player;
	Ballon ballon[6];
	Bullet bullet[3];*/
};

