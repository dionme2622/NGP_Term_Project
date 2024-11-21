#include "GameFramework.h"
#include "stdafx.h"

/*SOCKET CGameFramework::sock = INVALID_SOCKET;  
int CGameFramework::retval = 0;
std::string CGameFramework::keyData = "";    */  

CGameFramework::CGameFramework()
{
	m_pScene			= nullptr;

	m_ppScenes			= new CScene * [4];		// �� 4��
	m_ppMaps			= new CMap * [2];		// Map 4��
	currentscene	= PLAYSCENE;				// Scene�� �ε���
	_tcscpy_s(m_pszFrameRate, _T("("));



	////���� ��� ���� ���� �ʱ�ȭ
	////���� �ʱ�ȭ
	//if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	//	return;

	//// ���� ����
	//sock = socket(AF_INET, SOCK_DGRAM, 0);
	//if (sock == INVALID_SOCKET) {
	//	printf("socket err");
	//}

	//// ���� �ּ� ����ü �ʱ�ȭ
	//memset(&remoteAddr, 0, sizeof(remoteAddr));
	//remoteAddr.sin_family = AF_INET;
	//inet_pton(AF_INET, "127.0.0.1", &remoteAddr.sin_addr);
	//remoteAddr.sin_port = htons(SERVERPORT);
	//// ���� ����
	//if (connect(sock, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr)) == SOCKET_ERROR) {
	//	printf("���� ���� ����");
	//	closesocket(sock);
	//	WSACleanup();
	//	exit(0);
	//}
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

	m_ppScenes[currentscene]->Initialize();
	m_pScene = m_ppScenes[currentscene];																									


	//CreateThread(NULL, 0, SendData, NULL, 0, NULL);

	// ���� ����
	//sock = socket(AF_INET, SOCK_STREAM, 0);
	//if (sock == INVALID_SOCKET) {
	//	printf("���� ���� ����");
	//	WSACleanup();
	//	return;
	//}

	//// ���� �ּ� ����
	//struct sockaddr_in serveraddr;
	//memset(&serveraddr, 0, sizeof(serveraddr));
	//serveraddr.sin_family = AF_INET;
	//inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);
	//serveraddr.sin_port = htons(SERVERPORT);

	//// ���� ����
	//if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
	//	printf("���� ���� ����");
	//	closesocket(sock);
	//	WSACleanup();
	//	exit(0);
	//}

	//printf("���� ���� ����");

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

	// ���� ���� Ű���� ���ڿ��� ��������
	/*std::string pressedKeys = GetPressedKeysAsString();

	if (!pressedKeys.empty()) {
		keyData = pressedKeys.c_str();
	}*/
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




