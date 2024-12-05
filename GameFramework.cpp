#include "GameFramework.h"
#include "stdafx.h"

CRITICAL_SECTION cs;

CGameFramework::CGameFramework()
{
	m_pScene			= nullptr;

	m_ppScenes			= new CScene * [4];		// 씬 4개
	m_ppMaps			= new CMap * [2];		// Map 4개
	currentscene		= PLAYSCENE;			// Scene의 인덱스
	_tcscpy_s(m_pszFrameRate, _T("("));

	//서버 통신 관련 변수 초기화
	//윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// 소켓 생성
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		printf("socket err");
	}

	hSelectEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hRecvEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// 소켓 주소 구조체 초기화
	memset(&remoteAddr, 0, sizeof(remoteAddr));
	remoteAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &remoteAddr.sin_addr);
	remoteAddr.sin_port = htons(SERVERPORT);
	// 서버 연결
	if (connect(sock, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr)) == SOCKET_ERROR) {
		printf("서버 연결 실패");
		closesocket(sock);
		WSACleanup();
		exit(0);
	}
	InitializeCriticalSection();

	int retval;
	retval = recv(sock, (char*)&sendPacket.playerID, sizeof(sendPacket.playerID), 0); // 방향키 데이터 수신
	ClientID = sendPacket.playerID;

	CreateThread(NULL, 0, CGameFramework::SendData, this, 0, NULL);
	CreateThread(NULL, 0, CGameFramework::ReceiveData, this, 0, NULL);

	CloseHandle(hSelectEvent);
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

	WaitForSingleObject(hRecvEvent, INFINITE);

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
	if (GetPressedKeys())
		sendPacket.keyState = GetPressedKeys();
}

int CGameFramework::GetPressedKeys()
{
	int pressedKeys = 0; // 초기화

	if (GetAsyncKeyState(VK_UP) & 0x8000) pressedKeys |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) pressedKeys |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) pressedKeys |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) pressedKeys |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) pressedKeys |= KEY_SPACE;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) pressedKeys |= KEY_SHIFT;
	return pressedKeys; // 비트 플래그 반환
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

	int tempData = -1;

	WaitForSingleObject(pFramework->hSelectEvent, INFINITE);
	while(1) {
		EnterCriticalSection(&pFramework->cs); // 동기화 시작

		// 현재 키 입력 상태를 가져오기
		int currentKeyState = pFramework->GetPressedKeys();
		pFramework->sendPacket.keyState = currentKeyState;

		// 이전 상태와 현재 상태가 다르면 서버로 전송
		if (tempData != currentKeyState) {
			tempData = currentKeyState;
			printf("SendData - KeyState Sent: %d\r", tempData);

			// 키 상태를 서버로 전송
			send(pFramework->sock, (char*)&pFramework->sendPacket, sizeof(pFramework->sendPacket), 0);
		}
		LeaveCriticalSection(&pFramework->cs); // 동기화 종료
	}

	return 0;
}

DWORD __stdcall CGameFramework::ReceiveData(LPVOID arg) {
	CGameFramework* pFramework = reinterpret_cast<CGameFramework*>(arg);

	int retval;
	bool initEventSet = false; // Event 해제 여부 확인

	while (1) {
		retval = recv(pFramework->sock, (char*)&pFramework->receivedPacket, sizeof(SC_PlayersInfoPacket), 0);
		if (retval > 0) {
			if (!initEventSet) {
				// 처음 recv가 호출되면 이벤트 해제
				initEventSet = true;
			}

			if (pFramework->m_pScene) {
				pFramework->m_pScene->ReceiveData(pFramework->receivedPacket, pFramework->GetClientID());
				//printf("p1 : %d, p2 : %d\r", pFramework->receivedPacket.playerData[0].x, pFramework->receivedPacket.playerData[1].x); // DEBUG
				//printf("board state: %d\n", pFramework->receivedPacket.mapData.boardData[10][0].state);		// DEBUG
				SetEvent(pFramework->hRecvEvent);
			}
		}
		else if (retval == SOCKET_ERROR) {
			printf("recv() 실패: %d\n", WSAGetLastError());
			break; // 오류 발생 시 루프 종료
		}
	}
	return 0;
}


