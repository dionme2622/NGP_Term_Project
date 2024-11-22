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
	//static DWORD WINAPI SendData(LPVOID lpParam);  // ���� �Լ��� ����
	//static DWORD WINAPI ReceiveData(LPVOID lpParam); // ���� �Լ��� ����
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

	CGameTimer		m_GameTimer;		// Ÿ�̸�

	int				currentscene;		// Scene�� �ε���
	CScene*			m_pScene;			// ��
	CScene**		m_ppScenes;			// ��� ��

	CMap*			m_pMap;				// Map
	CMap**			m_ppMaps;			// Map �迭


	HDC hdc, MemDC, MemDCImage;
	PAINTSTRUCT ps;


	// ���� ��� ����
	WSADATA wsa;
	SOCKET sock;
	sockaddr_in remoteAddr;
	int retval;

	HANDLE hSelectEvent;

	CS_PlayerInputPacket sendPacket;  // ���� ��� ��� ������ ����
	SC_PlayersInfoPacket receivedPacket; // ���� ��� ��� ������ ����

	CRITICAL_SECTION cs;
public:

	void SetCurScene(int Scene);				// Scene Set
	
	CMap* GetCurMap() { return m_pMap; }		// Map ������ Get
	void SetCurMap(int _map);					// Map ������ Set




private:
	_TCHAR						m_pszFrameRate[70];

};

