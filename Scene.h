#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Player.h"

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
	CScene(CGameFramework* GameFramework);
	~CScene();


	virtual void Initialize(HWND hwnd, HINSTANCE g_hInst);
	virtual void ProcessInput() = 0;


	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;


	void DrawImage(HDC hdcDest, RECT pos, HDC hdcSrc, RECT rc, DWORD rop);

public :
	CGameFramework* GetFramework() { return m_pGameFramework;}

private:


protected:
	HWND				hWnd;
	HINSTANCE			hInst;
	RECT				rc;


	HBITMAP				hBit;
	HDC					MemDC;
	HDC					MemDCImage;
	HBITMAP				OldBit[2];

	POINT				cursorPos;

protected:
	CGameFramework*		m_pGameFramework;

public:
	int*				currentscene;
};






