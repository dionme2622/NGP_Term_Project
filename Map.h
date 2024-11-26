#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Player.h"
#include "Packet.h"

class CMap
{
public:
	CMap() {};
	~CMap() {};

	virtual void Initialize(HINSTANCE _hInst, SC_PlayersInfoPacket receivedPacket);
	virtual void Render(HDC MemDC, HDC MemDCImage);
	virtual void Update(SC_PlayersInfoPacket receivedPacket, float fTimeElapsed);
	
	CPlayer* Getplayer(int i) { return playerData[i]; }
	CBoard GetBoard(int i, int j) { return Board[i][j]; }

	void SetBallon();

	void BallonBoom(CPlayer* playerData, int num);

protected:
	CBoard			Board[13][15];
	CPlayer*		playerData[2];
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

	virtual void Initialize(HINSTANCE _hInst, SC_PlayersInfoPacket receivedPacket);
	virtual void Render(HDC MemDC, HDC MemDCImage);
	virtual void Update(SC_PlayersInfoPacket receivedPacket, float fTimeElapsed);

private:
	HBITMAP TileBitmap;
	HBITMAP Block[7];
};

class CPirate : public CMap
{
public:
	CPirate() {};
	~CPirate() {};

	virtual void Initialize(HINSTANCE _hInst, SC_PlayersInfoPacket receivedPacket);
	virtual void Render(HDC MemDC, HDC MemDCImage);
	virtual void Update(SC_PlayersInfoPacket receivedPacket, float fTimeElapsed);

private:
	HBITMAP TileBitmap;
	HBITMAP block_pirate;
	HBITMAP obstacle_pirate;
};
