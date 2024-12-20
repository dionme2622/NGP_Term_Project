#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "player.h"

struct SButton
{
	RECT pos;
	RECT size;
	HBITMAP ButtonImage;
};

class CGameFramework;

class CScene
{
public:
	CScene(HWND _hWnd, HINSTANCE _hInst, CGameFramework* GameFramework);
	~CScene();


	virtual void Initialize();
	virtual void ProcessInput() = 0;


	virtual void Update(float fTimeElapsed) = 0;
	virtual void Render() = 0;

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;


	void DrawImage(HDC hdcDest, RECT pos, HDC hdcSrc, RECT rc, DWORD rop);

public :
	CGameFramework* GetFramework() { return m_pGameFramework;}


protected:
	HWND				hWnd;
	HINSTANCE			hInst;
	RECT				rc;


	HBITMAP				hBit;
	HDC					MemDC;
	HDC					MemDCImage;
	HBITMAP				OldBit[3];

	POINT				cursorPos;

protected:
	CGameFramework*		m_pGameFramework;

public:
	int*				currentscene;
};






