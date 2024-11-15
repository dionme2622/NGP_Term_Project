#pragma once
#include "stdafx.h"
#include "GameObject.h"

class CMap
{
public:
	CMap() {};
	~CMap() {};

	virtual void Initialize(HINSTANCE _hInst);
	virtual void Render(HDC MemDC, HDC MemDCImage) {};

protected:
	CBoard			Board[13][15];

	HINSTANCE		hInst;
};

class CVillage : public CMap
{
public :
	CVillage() {};
	~CVillage() {};

	virtual void Initialize(HINSTANCE _hInst);
	virtual void Render(HDC MemDC, HDC MemDCImage);

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

private:
	HBITMAP TileBitmap;

};
