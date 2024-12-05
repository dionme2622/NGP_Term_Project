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
	// TODO : Bitmap, Map, playerData�� �����͸� Initialize �Ѵ�.
	
	MAP->Initialize(hInst, receivedPacket, m_ID);		// ���õ� Map�� Initialize

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
	int pressedKeys = 0; // �ʱ�ȭ

	if (GetAsyncKeyState(VK_UP) & 0x8000) pressedKeys |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) pressedKeys |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) pressedKeys |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) pressedKeys |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) pressedKeys |= KEY_SPACE;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) pressedKeys |= KEY_SHIFT;
	return pressedKeys; // ��Ʈ �÷��� ��ȯ
}

void CPlayScene::Update(float fTimeElapsed)
{
	// TODO : Play Scene Update
	// 1. playerData�� ���⺤�Ϳ� ���� �����δ�.
	// 2. Bitmap�� ��ǥ�� �������� �ִϸ��̼��� �ִ´�.
	MAP->Update(receivedPacket, fTimeElapsed);	

	// ������ Ű �Է� ���� �����͵��� ������
}

void CPlayScene::Render()
{
	HDC hdc = GetDC(hWnd);	

	HDC MemDC = CreateCompatibleDC(hdc);
	HDC MemDCImage = CreateCompatibleDC(hdc);

	hBit = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	OldBit[0] = (HBITMAP)SelectObject(MemDC, hBit);
	OldBit[1] = (HBITMAP)SelectObject(MemDCImage, backgroundImage); //--- ��� �̹���	
	TransparentBlt(MemDC, 0, 0, rc.right, rc.bottom, MemDCImage, 0, 0, 800, 600, RGB(255, 0, 255));
	MAP->Render(MemDC, MemDCImage);			// ���õ� Map�� Render �Ѵ�.
	//playerData->Render(MemDC, MemDCImage);		// playerData Render


	BitBlt(hdc, 0, 0, rc.right, rc.bottom, MemDC, 0, 0, SRCCOPY);
	// �ڿ� ����
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

	// ���� ���¿� ���� ���°� �ٸ��� ������ ����
	if (tempData != currentKeyState) {
		tempData = currentKeyState;
		printf("SendData - KeyState Sent: %d\r", tempData);

		// Ű ���¸� ������ ����
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
//		EnterCriticalSection(&pFramework->cs); // ����ȭ ����
//
//		// ���� Ű �Է� ���¸� ��������
//		int currentKeyState = pFramework->GetPressedKeys();
//		pFramework->sendPacket.keyState = currentKeyState;
//
//		// ���� ���¿� ���� ���°� �ٸ��� ������ ����
//		if (tempData != currentKeyState) {
//			tempData = currentKeyState;
//			printf("SendData - KeyState Sent: %d\r", tempData);
//
//			// Ű ���¸� ������ ����
//			send(pFramework->sock, (char*)&pFramework->sendPacket, sizeof(pFramework->sendPacket), 0);
//		}
//		LeaveCriticalSection(&pFramework->cs); // ����ȭ ����
//	}
//
//	return 0;
//}