#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Player.h"

class CMap
{
public:
	CMap() {};
	~CMap() {};

	virtual void Initialize(HINSTANCE _hInst);
	virtual void Render(HDC MemDC, HDC MemDCImage) {};
	virtual void Update(float fTimeElapsed) = 0;
	
	CPlayer* GetPlayer() { return player; }
protected:
	CBoard			Board[13][15];
	CPlayer*		player;
	HINSTANCE		hInst;
};

class CVillage : public CMap
{
public :
	CVillage() {};
	~CVillage() {};

	virtual void Initialize(HINSTANCE _hInst);
	virtual void Render(HDC MemDC, HDC MemDCImage);
	virtual void Update(float fTimeElapsed);

private:
	HBITMAP TileBitmap;
	HBITMAP Block[7];
};

class CPirate : public CMap
{
public:
	CPirate() {};
	~CPirate() {};

	virtual void Initialize(HINSTANCE _hInst);
	virtual void Render(HDC MemDC, HDC MemDCImage);
	virtual void Update(float fTimeElapsed);

private:
	HBITMAP TileBitmap;
	HBITMAP block_pirate;
	HBITMAP obstacle_pirate;
};
