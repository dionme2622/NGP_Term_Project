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
	
	MAP->Initialize(hInst, receivedPacket);		// ���õ� Map�� Initialize

	// Resource
	backgroundImage = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_STAGEFRAME));
}


void CPlayScene::ProcessInput()
{
	char pressedKeys = GetPressedKeysAsChar();
	if (pressedKeys)
		sendPacket.keyState = pressedKeys;


	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	GetKeyboardState(pKeysBuffer);


}

int CPlayScene::GetPressedKeysAsChar()
{
	// ���� �ִ� Ű ����� ������
	char pressedKeys = GetPressedKey();

	if (!pressedKeys) pressedKeys = '0';

	return pressedKeys;
}

int CPlayScene::GetPressedKey()
{
	char pressedKey = '0';

	// ��� ���� Ű �ڵ�(0x01���� 0xFE����)�� �ݺ�
	for (int key = 0x01; key <= 0xFE; ++key) {
		// Ű�� ���� �ִ��� Ȯ��
		if (GetAsyncKeyState(key) & 0x8000) {
			pressedKey = key; // ���� Ű�� �߰�
		}
	}

	return pressedKey;
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

void CPlayScene::SendData(SOCKET sock)
{
	if (pastData != sendPacket.keyState) {
		pastData = sendPacket.keyState;
		int retval = send(sock, (char*)&sendPacket, sizeof(sendPacket), 0);
		printf("%d, ID : %d\n", sendPacket.keyState, sendPacket.playerID);
	}
}

void CPlayScene::ReceiveData(SOCKET sock)
{
	int retval = recv(sock, (char*)&receivedPacket, sizeof(receivedPacket), 0);
}
