#pragma once
#include "Timer.h"
#include "Scene.h"

#include "StartScene.h"
#include "MenuScene.h"
#include "LobbyScene.h"
#include "PlayScene.h"


#include "stdafx.h"

class CScene;

class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework();

	void Initialize(HWND hMainWnd, HINSTANCE g_hInst);

	void FrameAdvance();

	void Tick();
	void Update();
	void Render();

	void ProcessInput();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
private:
	HWND			hWnd;
	HINSTANCE		hInst;

	//CTimer			timer;			// Å¸ÀÌ¸Ó

	int				currentscene;			// SceneÀÇ ÀÎµ¦½º
	CScene*			m_pScene;			// ¾À
	CScene**		m_ppScenes;			// ¸ðµç ¾À

	HDC hdc, MemDC, MemDCImage;
	PAINTSTRUCT ps;



public:
	void SetCurScene(int Scene);				//µð¹ö±ë¿ë ¾À ³Ñ±â±â






private:
	static DWORD __stdcall ClientMain(LPVOID arg);



};

