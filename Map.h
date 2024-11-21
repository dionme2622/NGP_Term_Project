#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "player.h"
#include "Packet.h"

class CMap
{
public:
	CMap() {};
	~CMap() {};

	virtual void Initialize(HINSTANCE _hInst, SC_PlayersInfoPacket _PlayersInfoPacket);
	virtual void Render(HDC MemDC, HDC MemDCImage);
	virtual void Update(float fTimeElapsed);
	
	CPlayer* Getplayer() { return player; }
	CBoard GetBoard(int i, int j) { return Board[i][j]; }

	void SetBallon();

	void BallonBoom(CPlayer* player, int num);

protected:
	CBoard			Board[13][15];
	CPlayer*		player;
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

	virtual void Initialize(HINSTANCE _hInst, SC_PlayersInfoPacket PlayersInfoPacket);
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

	virtual void Initialize(HINSTANCE _hInst, SC_PlayersInfoPacket PlayersInfoPacket);
	virtual void Render(HDC MemDC, HDC MemDCImage);
	virtual void Update(float fTimeElapsed);

private:
	HBITMAP TileBitmap;
	HBITMAP block_pirate;
	HBITMAP obstacle_pirate;
};
