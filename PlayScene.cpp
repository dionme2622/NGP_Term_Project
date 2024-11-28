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
	
	MAP->Initialize(hInst, receivedPacket);		// 선택된 Map의 Initialize

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
	// 눌려 있는 키 목록을 가져옴
	char pressedKeys = GetPressedKey();

	if (!pressedKeys) pressedKeys = '0';

	return pressedKeys;
}

int CPlayScene::GetPressedKey()
{
	char pressedKey = '0';

	// 모든 가상 키 코드(0x01부터 0xFE까지)를 반복
	for (int key = 0x01; key <= 0xFE; ++key) {
		// 키가 눌려 있는지 확인
		if (GetAsyncKeyState(key) & 0x8000) {
			pressedKey = key; // 눌린 키를 추가
		}
	}

	return pressedKey;
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
