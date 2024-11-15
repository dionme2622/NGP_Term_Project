#include "GameFramework.h"
#include "stdafx.h"

// Commit Test
CGameFramework::CGameFramework()
{
	m_pScene			= nullptr;

	m_ppScenes			= new CScene * [4];		// �� 4��
	currentscene	= MENUSCENE;			// Scene�� �ε���
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

	m_GameTimer.Reset();				// Ÿ�̸� �ʱ�ȭ
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
	m_GameTimer.Tick(0.0f);				// fps ���� ����
}



void CGameFramework::Render()
{
	m_pScene->Render();
}

void CGameFramework::ProcessInput()
{
	m_pScene->ProcessInput();

	// ���� ���� Ű���� ���ڿ��� ��������
	std::string pressedKeys = GetPressedKeysAsString();

	if (!pressedKeys.empty()) {
		keyData = pressedKeys.c_str();
	}
}

std::vector<char> CGameFramework::GetPressedKeys()
{
	std::vector<char> pressedKeys;

	// ��� ���� Ű �ڵ�(0x01���� 0xFE����)�� �ݺ�
	for (int key = 0x01; key <= 0xFE; ++key) {
		// Ű�� ���� �ִ��� Ȯ��
		if (GetAsyncKeyState(key) & 0x8000) {
			pressedKeys.push_back(key); // ���� Ű�� �߰�
		}
	}

	return pressedKeys;
}

std::string CGameFramework::GetPressedKeysAsString()
{
	std::string keys;

	// ���� �ִ� Ű ����� ������
	std::vector<char> pressedKeys = GetPressedKeys();

	// �� Ű�� ���ڿ��� ��ȯ�Ͽ� �߰�
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

	//// ������ ����
	//char buf[BUFSIZE];

	//// �Է� ������ ����
	//retval = send(sock, buf, retval, 0);

	//int totalSent = 0;
	//int filesize = 0;		// �����ؾߵ�

	//while (1) {
	//	int bytes = send(sock, buf, retval, 0);

	//	if (totalSent == filesize) {
	//		break;
	//	}
	//}
	return 0;
}


