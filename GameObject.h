#pragma once
#include "stdafx.h"


class CGameObject
{
public:
	CGameObject();
	~CGameObject();

	virtual void Update(float fTimeElapsed) = 0;
	virtual void Render(HDC MemDC, HDC MemDCImage) = 0;


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
	virtual void Render(HDC MemDC, HDC MemDCImage);

private:
	HBITMAP MainBitmap[2];			// 0 : Bubble, 1 : Explosion

private:
	int			state;   //			0������ �ִ°�      1 ó�� ��ġ�ҋ�			 2 ��!
	int			beforeboomcouont;
	int			startboomcount;

	RECT		boundingBox;

};


class CBoard : public CGameObject
{
public:
	CBoard();
	~CBoard();

	virtual void Update(float fTimeElapsed);
	virtual void Render(HDC MemDC, HDC MemDCImage);

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
	virtual void Render(HDC MemDC, HDC MemDCImage);

public:
	RECT		boundingBox;

};