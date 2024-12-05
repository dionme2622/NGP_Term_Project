#include "PlayScene.h"
#include "stdafx.h"
#include "GameFramework.h"
#include "Packet.h"

#define MAP GetFramework()->GetCurMap()

CPlayScene::CPlayScene(HWND _hWnd, HINSTANCE _hInst, CGameFramework* GameFramework) : CScene(_hWnd, _hInst, GameFramework)
{

}

CPlayScene::~CPlayScene()
{
}

void CPlayScene::Initialize()
{
	// TODO : Bitmap, Map, playerData의 데이터를 Initialize 한다.
	
	MAP->Initialize(hInst, receivedPacket, m_ID);		// 선택된 Map의 Initialize

	// Resource
	backgroundImage = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_STAGEFRAME));

	sendPacket.playerID = m_ID;
}


void CPlayScene::ProcessInput()
{
	if (GetPressedKey())
		sendPacket.keyState = GetPressedKey();
}


int CPlayScene::GetPressedKey()
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

void CPlayScene::Update(float fTimeElapsed)
{
	// TODO : Play Scene Update
	// 1. playerData의 방향벡터에 따라 움직인다.
	// 2. Bitmap의 좌표를 움직여서 애니메이션을 넣는다.
	MAP->Update(receivedPacket, fTimeElapsed);	

	// 서버로 키 입력 버퍼 데이터들을 보낸다
}

void CPlayScene::Render()
{
	HDC hdc = GetDC(hWnd);	

	HDC MemDC = CreateCompatibleDC(hdc);
	HDC MemDCImage = CreateCompatibleDC(hdc);

	hBit = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	OldBit[0] = (HBITMAP)SelectObject(MemDC, hBit);
	OldBit[1] = (HBITMAP)SelectObject(MemDCImage, backgroundImage); //--- 배경 이미지	
	TransparentBlt(MemDC, 0, 0, rc.right, rc.bottom, MemDCImage, 0, 0, 800, 600, RGB(255, 0, 255));
	MAP->Render(MemDC, MemDCImage);			// 선택된 Map을 Render 한다.
	//playerData->Render(MemDC, MemDCImage);		// playerData Render


	BitBlt(hdc, 0, 0, rc.right, rc.bottom, MemDC, 0, 0, SRCCOPY);
	// 자원 해제
	SelectObject(MemDC, OldBit[0]);
	DeleteObject(hBit);
	DeleteDC(MemDC);
	DeleteDC(MemDCImage);
	ReleaseDC(hWnd, hdc);
} 

void CPlayScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&cursorPos);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void CPlayScene::SendData()
{
	sendPacket.header.packetType = 1;

	int tempData = -1;

	int currentKeyState = GetPressedKey();
	sendPacket.keyState = currentKeyState;

	// 이전 상태와 현재 상태가 다르면 서버로 전송
	if (tempData != currentKeyState) {
		tempData = currentKeyState;
		printf("SendData - KeyState Sent: %d\r", tempData);

		// 키 상태를 서버로 전송
		send(sock, (char*)&sendPacket, sizeof(sendPacket), 0);
	}


	/*if (pastData != sendPacket.keyState) {
		pastData = sendPacket.keyState;
		int retval = send(sock, (char*)&sendPacket, sizeof(sendPacket), 0);
		printf("%d, ID : %d\n", sendPacket.keyState, sendPacket.playerID);
	}*/
}

void CPlayScene::ReceiveData()
{
	int retval = recv(sock, (char*)&receivedPacket, sizeof(receivedPacket), 0);
}


//DWORD __stdcall CGameFramework::SendData(LPVOID arg) {
//	CGameFramework* pFramework = reinterpret_cast<CGameFramework*>(arg);
//
//	int tempData = -1;
//
//	WaitForSingleObject(pFramework->hSelectEvent, INFINITE);
//	while (1) {
//		EnterCriticalSection(&pFramework->cs); // 동기화 시작
//
//		// 현재 키 입력 상태를 가져오기
//		int currentKeyState = pFramework->GetPressedKeys();
//		pFramework->sendPacket.keyState = currentKeyState;
//
//		// 이전 상태와 현재 상태가 다르면 서버로 전송
//		if (tempData != currentKeyState) {
//			tempData = currentKeyState;
//			printf("SendData - KeyState Sent: %d\r", tempData);
//
//			// 키 상태를 서버로 전송
//			send(pFramework->sock, (char*)&pFramework->sendPacket, sizeof(pFramework->sendPacket), 0);
//		}
//		LeaveCriticalSection(&pFramework->cs); // 동기화 종료
//	}
//
//	return 0;
//}