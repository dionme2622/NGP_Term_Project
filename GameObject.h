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
	int x, y;		// ��ǥ

	int xPos, yPos;
	float xPosF, yPosF;
	

	// �̹��� ���� ������ ����

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
	int			state;   //			0������ �ִ°�      1 ó�� ��ġ�ҋ�			 2 ��!
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
	int state;				// 1�̸� �Ϲ�Ÿ��, 2�̸� ���, 3�̸� ��ֹ�, 6: �Ź߾�����, 7: ���ٱ������, 8: ��ǳ��������
	int block;				// ���� ��

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