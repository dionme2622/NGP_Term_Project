#include "GameFramework.h"
#include "stdafx.h"

CRITICAL_SECTION cs;

CGameFramework::CGameFramework()
{
	m_pScene			= nullptr;

	m_ppScenes			= new CScene * [4];		// �� 4��
	m_ppMaps			= new CMap * [2];		// Map 4��
	currentscene		= PLAYSCENE;			// Scene�� �ε���
	_tcscpy_s(m_pszFrameRate, _T("("));

	//���� ��� ���� ���� �ʱ�ȭ
	//���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// ���� ����
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		printf("socket err");
	}

	hSelectEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hRecvEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// ���� �ּ� ����ü �ʱ�ȭ
	memset(&remoteAddr, 0, sizeof(remoteAddr));
	remoteAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &remoteAddr.sin_addr);
	remoteAddr.sin_port = htons(SERVERPORT);
	// ���� ����
	if (connect(sock, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr)) == SOCKET_ERROR) {
		printf("���� ���� ����");
		closesocket(sock);
		WSACleanup();
		exit(0);
	}
	InitializeCriticalSection();

	int retval;
	retval = recv(sock, (char*)&sendPacket.playerID, sizeof(sendPacket.playerID), 0); // ����Ű ������ ����


	CreateThread(NULL, 0, CGameFramework::SendData, this, 0, NULL);
	CreateThread(NULL, 0, CGameFramework::ReceiveData, this, 0, NULL);

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

	m_pScene->SetID(sendPacket.playerID);

	SetEvent(hSelectEvent);
	WaitForSingleObject(hRecvEvent, INFINITE);

	m_ppScenes[currentscene]->Initialize();

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

	m_GameTimer.GetFrameRate(m_pszFrameRate + 1, 37);
	size_t nLength = _tcslen(m_pszFrameRate);

	::SetWindowText(hWnd, m_pszFrameRate);
}

void CGameFramework::Tick()
{
	m_GameTimer.Tick(120.0f);				// fps ����
}



void CGameFramework::Render()
{
	m_pScene->Render();
}

void CGameFramework::ProcessInput()
{
	m_pScene->ProcessInput();

	/*char pressedKeys = GetPressedKeysAsString();

	if (pressedKeys)
		sendPacket.keyState = pressedKeys;*/
}

//int CGameFramework::GetPressedKeys()
//{
//	char pressedKey = '0';
//	
//	// ��� ���� Ű �ڵ�(0x01���� 0xFE����)�� �ݺ�
//	for (int key = 0x01; key <= 0xFE; ++key) {
//	    // Ű�� ���� �ִ��� Ȯ��
//	    if (GetAsyncKeyState(key) & 0x8000) {
//	        pressedKey = key; // ���� Ű�� �߰�
//	    }
//	}
//	
//	return pressedKey;
//}
//
//int CGameFramework::GetPressedKeysAsString()
//{
//	// ���� �ִ� Ű ����� ������
//	char pressedKeys = GetPressedKeys();
//	
//	if (!pressedKeys) pressedKeys = '0';
//	
//	return pressedKeys;
//}

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

	WaitForSingleObject(pFramework->hSelectEvent, INFINITE);

	while (1) {
		pFramework->m_pScene->SendData(pFramework->sock);
	}

	return 0;
}

DWORD __stdcall CGameFramework::ReceiveData(LPVOID arg) {
	CGameFramework* pFramework = reinterpret_cast<CGameFramework*>(arg);

	while (1) {
		if (pFramework->m_pScene) {
			pFramework->m_pScene->ReceiveData(pFramework->sock);
			SetEvent(pFramework->hRecvEvent);
		}
	}
	return 0;
}


