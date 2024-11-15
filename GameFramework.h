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

	//CTimer			timer;			// Ÿ�̸�

	int				currentscene;			// Scene�� �ε���
	CScene*			m_pScene;			// ��
	CScene**		m_ppScenes;			// ��� ��

	HDC hdc, MemDC, MemDCImage;
	PAINTSTRUCT ps;



public:
	void SetCurScene(int Scene);				//������ �� �ѱ��






private:
	static DWORD __stdcall ClientMain(LPVOID arg);



};

