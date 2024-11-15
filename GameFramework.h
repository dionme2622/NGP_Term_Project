#pragma once
#include "stdafx.h"
#include "Timer.h"
#include "Scene.h"

#include "StartScene.h"
#include "MenuScene.h"
#include "LobbyScene.h"
#include "PlayScene.h"
#include "Map.h"

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

	CGameTimer		m_GameTimer;		// 타이머

	int				currentscene;		// Scene의 인덱스
	CScene*			m_pScene;			// 씬
	CScene**		m_ppScenes;			// 모든 씬

	CMap*			m_pMap;				// Map
	CMap**			m_ppMaps;			// Map 배열


	HDC hdc, MemDC, MemDCImage;
	PAINTSTRUCT ps;

public:

	void SetCurScene(int Scene);				// Scene Set
	
	CMap* GetCurMap() { return m_pMap; }		// Map 변수를 Get
	void SetCurMap(int _map);					// Map 변수를 Set




private:
	





};

