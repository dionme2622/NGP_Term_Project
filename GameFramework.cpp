#include "GameFramework.h"
#include "stdafx.h"

// Commit Test
CGameFramework::CGameFramework()
{
	m_pScene			= nullptr;

	m_ppScenes			= new CScene * [4];		// ¾À 4°³
	currentscene	= STARTSCENE;			// SceneÀÇ ÀÎµ¦½º
}

CGameFramework::~CGameFramework()
{
}

void CGameFramework::Initialize(HWND hMainWnd, HINSTANCE g_hInst)
{
	hWnd = hMainWnd;
	hInst = g_hInst;
	m_ppScenes[0] = new CStartScene(this);
	m_ppScenes[0]->Initialize(hWnd, hInst);
	
	m_ppScenes[1] = new CMenuScene(this);
	m_ppScenes[1]->Initialize(hWnd, hInst);

	m_ppScenes[2] = new CLobbyScene(this);
	m_ppScenes[2]->Initialize(hWnd, hInst);

	m_ppScenes[3] = new CPlayScene(this);
	m_ppScenes[3]->Initialize(hWnd, hInst);

	m_pScene = m_ppScenes[currentscene];

	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);
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

	m_pScene->Update();
}

void CGameFramework::Tick()
{

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

	/*switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}*/
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{
		/*if (LOWORD(wParam) == WA_INACTIVE)
			m_GameTimer.Stop();
		else
			m_GameTimer.Start();*/
		break;
	}
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:

	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
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
}

DWORD __stdcall CGameFramework::ClientMain(LPVOID arg)
{
	return 0;
}


