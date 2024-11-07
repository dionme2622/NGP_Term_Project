#include "GameFramework.h"
#include "stdafx.h"


CGameFramework::CGameFramework()
{
	scene			= nullptr;

	scenes			= new CScene * [4];		// ¾À 4°³
	currentscene	= PLAYSCENE;			// SceneÀÇ ÀÎµ¦½º
}

CGameFramework::~CGameFramework()
{
}

void CGameFramework::Initialize(HWND hMainWnd, HINSTANCE g_hInst)
{
	hWnd = hMainWnd;
	hInst = g_hInst;
	scenes[0] = new CStartScene();
	scenes[0]->Initialize(hWnd, hInst);

	scenes[1] = new CMenuScene();
	scenes[1]->Initialize(hWnd, hInst);

	scenes[2] = new CLobbyScene();
	scenes[2]->Initialize(hWnd, hInst);

	scenes[3] = new CPlayScene();
	scenes[3]->Initialize(hWnd, hInst);


	scene = scenes[currentscene];
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

	scene->Update();
}

void CGameFramework::Tick()
{

}



void CGameFramework::Render()
{
	scene->Render();
}

void CGameFramework::ProcessInput()
{
	scene->ProcessInput();
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (scene) scene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

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
		NextScene();
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

void CGameFramework::NextScene()
{
	currentscene = (currentscene + 1) % 4;
	scene = scenes[currentscene];
}
