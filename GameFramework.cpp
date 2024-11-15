#include "GameFramework.h"
#include "stdafx.h"

// Commit Test
CGameFramework::CGameFramework()
{
	m_pScene			= nullptr;

	m_ppScenes			= new CScene * [4];		// 씬 4개
	currentscene	= MENUSCENE;			// Scene의 인덱스
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
}

void CGameFramework::Tick()
{
	m_GameTimer.Tick(0.0f);				// fps 제한 없음
}



void CGameFramework::Render()
{
	m_pScene->Render();
}

void CGameFramework::ProcessInput()
{
	m_pScene->ProcessInput();

	// 현재 눌린 키들을 문자열로 가져오기
	std::string pressedKeys = GetPressedKeysAsString();

	if (!pressedKeys.empty()) {
		keyData = pressedKeys.c_str();
	}
}

std::vector<char> CGameFramework::GetPressedKeys()
{
	std::vector<char> pressedKeys;

	// 모든 가상 키 코드(0x01부터 0xFE까지)를 반복
	for (int key = 0x01; key <= 0xFE; ++key) {
		// 키가 눌려 있는지 확인
		if (GetAsyncKeyState(key) & 0x8000) {
			pressedKeys.push_back(key); // 눌린 키를 추가
		}
	}

	return pressedKeys;
}

std::string CGameFramework::GetPressedKeysAsString()
{
	std::string keys;

	// 눌려 있는 키 목록을 가져옴
	std::vector<char> pressedKeys = GetPressedKeys();

	// 각 키를 문자열로 변환하여 추가
	for (int key : pressedKeys) {
		keys += std::to_string(key) + " ";
	}

	return keys;
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
}

DWORD __stdcall CGameFramework::ClientMain(LPVOID arg)
{
	//int retval;

	//// 데이터 전송
	//char buf[BUFSIZE];

	//// 입력 데이터 전송
	//retval = send(sock, buf, retval, 0);

	//int totalSent = 0;
	//int filesize = 0;		// 조정해야됨

	//while (1) {
	//	int bytes = send(sock, buf, retval, 0);

	//	if (totalSent == filesize) {
	//		break;
	//	}
	//}
	return 0;
}


