#include "GameFramework.h"
#include "stdafx.h"

CRITICAL_SECTION cs;

CGameFramework::CGameFramework()
{
	m_pScene			= nullptr;

	m_ppScenes			= new CScene * [4];		// 씬 4개
	m_ppMaps			= new CMap * [2];		// Map 4개
	currentscene		= MENUSCENE;			// Scene의 인덱스
	_tcscpy_s(m_pszFrameRate, _T("("));
}

CGameFramework::~CGameFramework()
{
}

void CGameFramework::Initialize(HWND hMainWnd, HINSTANCE g_hInst)
{

	hWnd = hMainWnd;
	hInst = g_hInst;

	m_ppMaps[0] = new CVillage();
	m_ppMaps[1] = new CPirate();
	m_pMap = m_ppMaps[0];

	m_ppScenes[0] = new CStartScene(hWnd, hInst, this);
	m_ppScenes[1] = new CMenuScene(hWnd, hInst, this);
	m_ppScenes[2] = new CLobbyScene(hWnd, hInst, this);
	m_ppScenes[3] = new CPlayScene(hWnd, hInst, this);

	m_pScene = m_ppScenes[currentscene];																									

	m_ppScenes[currentscene]->Initialize();

	m_GameTimer.Reset();				// 타이머 초기화
}

void CGameFramework::FrameAdvance()
{
	ProcessInput();
	Update();
	Render();
}

void CGameFramework::Update()
{
	Tick();

	float fTimeElapsed = m_GameTimer.GetTimeElapsed();
	m_pScene->Update(fTimeElapsed);

	m_GameTimer.GetFrameRate(m_pszFrameRate + 1, 37);
	size_t nLength = _tcslen(m_pszFrameRate);

	::SetWindowText(hWnd, m_pszFrameRate);

	if (m_pScene->IsServerConnected() && m_bPause == false) {
		CreateThread(NULL, 0, CGameFramework::SendData, this, 0, NULL);
		CreateThread(NULL, 0, CGameFramework::ReceiveData, this, 0, NULL);
		m_bPause = true;
	}
}

void CGameFramework::Tick()
{
	m_GameTimer.Tick(120.0f);				// fps 제한
}



void CGameFramework::Render()
{
	m_pScene->Render();
}

void CGameFramework::ProcessInput()
{
	m_pScene->ProcessInput();
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene) m_pScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{

	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{	
		if (LOWORD(wParam) == WA_INACTIVE)
			m_GameTimer.Stop();
		else
			m_GameTimer.Start();
		break;
	}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		break;
	}
	return(0);
}

void CGameFramework::SetCurScene(int Scene)
{
	m_pScene = m_ppScenes[Scene];
	m_pScene->Initialize();
}


void CGameFramework::SetCurMap(int Map)
{
	m_pMap = m_ppMaps[Map];
}


void CGameFramework::InitializeCriticalSection()
{
	::InitializeCriticalSection(&cs);
}


DWORD __stdcall CGameFramework::SendData(LPVOID arg) {
	CGameFramework* pFramework = reinterpret_cast<CGameFramework*>(arg);

	//WaitForSingleObject(pFramework->hSelectEvent, INFINITE);

	while (1) {
		pFramework->m_pScene->SendData();
	}

	return 0;
}

DWORD __stdcall CGameFramework::ReceiveData(LPVOID arg) {
	CGameFramework* pFramework = reinterpret_cast<CGameFramework*>(arg);

	while (1) {
		if (pFramework->m_pScene) {
			pFramework->m_pScene->ReceiveData();
			//SetEvent(pFramework->hRecvEvent);
		}
	}
	return 0;
}


