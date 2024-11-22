#pragma once
#include "stdafx.h"
#include "Timer.h"
#include "Scene.h"

#include "StartScene.h"
#include "MenuScene.h"
#include "LobbyScene.h"
#include "PlayScene.h"
#include "Map.h"
#include "Packet.h"

class CScene;

class CGameFramework
{
public:
	//static DWORD WINAPI SendData(LPVOID lpParam);  // 정적 함수로 선언
	//static DWORD WINAPI ReceiveData(LPVOID lpParam); // 정적 함수로 선언
public:
	CGameFramework();
	~CGameFramework();

	void Initialize(HWND hMainWnd, HINSTANCE g_hInst);

	void FrameAdvance();

	void Tick();
	void Update();
	void Render();

	void ProcessInput();
	int GetPressedKeysAsString();
	int GetPressedKeys();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);


	void InitializeCriticalSection();
	static DWORD __stdcall SendData(LPVOID arg);
	static DWORD __stdcall ReceiveData(LPVOID arg);

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


	// 서버 통신 관련
	WSADATA wsa;
	SOCKET sock;
	sockaddr_in remoteAddr;
	int retval;

	HANDLE hSelectEvent;

	CS_PlayerInputPacket sendPacket;  // 정적 대신 멤버 변수로 변경
	SC_PlayersInfoPacket receivedPacket; // 정적 대신 멤버 변수로 변경

	CRITICAL_SECTION cs;
public:

	void SetCurScene(int Scene);				// Scene Set
	
	CMap* GetCurMap() { return m_pMap; }		// Map 변수를 Get
	void SetCurMap(int _map);					// Map 변수를 Set




private:
	_TCHAR						m_pszFrameRate[70];

};

