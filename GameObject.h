#pragma once
#include "stdafx.h"

class CMap;

class CGameObject
{
public:
	CGameObject();
	~CGameObject();

	virtual void Update(float fTimeElapsed) = 0;
	virtual void Render(HDC MemDC, HDC MemDCImage, CMap* Map);


public:
	int x, y;		// 좌표

	int xPos, yPos;
	float xPosF, yPosF;
	

	// 이미지 담을 무언가의 변수

};

class CBallon : public CGameObject
{
public:
	CBallon() {};
	CBallon(HINSTANCE hInst);
	~CBallon();

	virtual void Update(float fTimeElapsed);
	virtual void Render(HDC MemDC, HDC MemDCImage, CMap* Map);

private:
	HBITMAP MainBitmap[2];			// 0 : Bubble, 1 : Explosion

public:
	int			state;   //			0가지고 있는거      1 처음 설치할떄			 2 펑!
	int			beforeboomcount;
	int			startboomcount;

	RECT		boundingBox;

public :
	int GetState() { return state; }
	void SetState(int _state) { state = _state; }
};


class CBoard : public CGameObject
{
public:
	CBoard();
	~CBoard();

	virtual void Update(float fTimeElapsed);

	int GetState() { return state; }

	void SetPosition(int _x, int _y) { x = _x, y = _y; }
	void SetState(int _state) { state = _state; }
private:
	int state;				// 1이면 일반타일, 2이면 블록, 3이면 장애물, 6: 신발아이템, 7: 물줄기아이템, 8: 물풍선아이템
	int block;				// 뭔지 모름

	RECT		boundingBox;
};





class CItem : public CGameObject
{
public:
	CItem();
	~CItem();

	virtual void Update(float fTimeElapsed);
	virtual void Render(HDC MemDC, HDC MemDCImage, CMap* Map);

public:
	RECT		boundingBox;

};