#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "player.h"


class CMap
{
public:
	CMap() {};
	~CMap() {};

	virtual void Initialize(HINSTANCE _hInst);
	virtual void Render(HDC MemDC, HDC MemDCImage);
	virtual void Update(float fTimeElapsed);
	
	Cplayer* Getplayer() { return player; }
	CBoard GetBoard(int i, int j) { return Board[i][j]; }

	void SetBallon();

	void BallonBoom(Cplayer* player, int num);

protected:
	CBoard			Board[13][15];
	Cplayer*		player;
	HINSTANCE		hInst;

private:
	// Attribute
	int xPos, yPos;
	float xPosF, yPosF;


	// Resource
	HBITMAP BallonBitmap[2];
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
