#include "LobbyScene.h"
#include "GameFramework.h"

enum { VILLAGE = 0, PIRATE = 1 };


CLobbyScene::CLobbyScene(HWND _hWnd, HINSTANCE _hInst, CGameFramework* GameFramework) : CScene(_hWnd, _hInst, GameFramework)
{
	

}

CLobbyScene::~CLobbyScene()
{
}

void CLobbyScene::Initialize()
{
	backgroundImage = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LOBBY));						// 배경 이미지

	selecMapImage = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SELECTMAP));						// 맵 선택 이미지

	mapImage = NULL;

	mapImages = new HBITMAP[3];

	mapImages[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_VILLAGEMAP));
	mapImages[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PIRATEMAP));
	mapImages[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_VILLAGEMAP));

	ReadyImage		= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_READY));
}


void CLobbyScene::ProcessInput()
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	GetKeyboardState(pKeysBuffer);

	// Test
	if (pKeysBuffer[VK_UP] & 0xF0) GetFramework()->SetCurMap(VILLAGE);			// Village Map Select
	if (pKeysBuffer[VK_DOWN] & 0xF0) GetFramework()->SetCurMap(PIRATE);			// PIRATE Map Select
	if (pKeysBuffer[VK_RETURN] & 0xF0) GetFramework()->SetCurScene(PLAYSCENE);			// PlayScene으로 넘어간다.
}

void CLobbyScene::Update(float fTimeElapsed)
{
	// TODO : Lobby Scene Update
	xPosF += 64 * 8 * fTimeElapsed;
	while (xPosF >= 44.0f) {
		xPosF = 0;
		xPos += 64;
		if (xPos >= 1152)
			xPos = 0;
	}

	if (recvLobbyPacket.recvNextSceneCall == 1) {
		//TODO
		// 맵 받은걸로 씬 넘기기
		GetFramework()->SetCurScene(PLAYSCENE);
	}

}

void CLobbyScene::Render()
{
	HDC hdc = GetDC(hWnd);
	HDC MemDC = CreateCompatibleDC(hdc);
	HDC MemDCImage = CreateCompatibleDC(hdc);

	hBit = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	OldBit[0] = (HBITMAP)SelectObject(MemDC, hBit);


	OldBit[1] = (HBITMAP)SelectObject(MemDCImage, backgroundImage); // 배경 이미지	
	StretchBlt(MemDC, 0, 0, rc.right, rc.bottom, MemDCImage, 0, 0, 1220, 950, SRCCOPY);

	if (mapImage) {
		OldBit[1] = (HBITMAP)SelectObject(MemDCImage, mapImage);
		StretchBlt(MemDC, 733, 530, 227, 210, MemDCImage, 0, 0, 225, 195, SRCCOPY);
	}

	if (showSelectMap) {
		OldBit[1] = (HBITMAP)SelectObject(MemDCImage, selecMapImage);
		StretchBlt(MemDC, rc.right / 2 - 447, rc.bottom / 2 - 73, 894, 146, MemDCImage, 0, 0, 894, 146, SRCCOPY);
	}

	if (recvLobbyPacket.playerExist[0]) {
		(HBITMAP)SelectObject(MemDCImage, ReadyImage);		// Player1 Ready
		TransparentBlt(MemDC, 65, 200, 100, 100, MemDCImage, xPos, 0, 64, 86, RGB(255, 0, 255));
	}
	if (recvLobbyPacket.playerExist[1]) {
		(HBITMAP)SelectObject(MemDCImage, ReadyImage);		// Player2 Ready
		TransparentBlt(MemDC, 225, 200, 100, 100, MemDCImage, xPos, 86, 64, 86, RGB(255, 0, 255));
	}

	BitBlt(hdc, 0, 0, rc.right, rc.bottom, MemDC, 0, 0, SRCCOPY);
	// 자원 해제
	SelectObject(MemDC, OldBit[0]);
	DeleteObject(hBit);
	DeleteDC(MemDC);
	DeleteDC(MemDCImage);
	ReleaseDC(hWnd, hdc);
}

void CLobbyScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		GetCursorPos(&cursorPos);
		ScreenToClient(hWnd, &cursorPos);

		SelectMap();

		if (mapImage && m_ID == 0) {
			if (cursorPos.x > 773 && cursorPos.x < 1055 &&
				cursorPos.y > 753 && cursorPos.y < 831) {
				sendLobbyPacket.nextSceneCall = true;
			}
		}
		//printf("%d, %d\n", cursorPos.x, cursorPos.y);
		// TODO : 맵 선택
		// GetFrameWork()->SetCurMap(Village);
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		SetCapture(hWnd);
		GetCursorPos(&cursorPos);
		ScreenToClient(hWnd, &cursorPos);

		break;
	default:
		break;
	}
}

void CLobbyScene::SelectMap()
{
	// test
	if (showSelectMap && cursorPos.x > 150 && cursorPos.x < 1050 &&
		cursorPos.y > 370 && cursorPos.y < 520) {

		

		if (cursorPos.x > 150 && cursorPos.x < 1200 / 3) mapData = 1;
		else if (cursorPos.x >= 1200 / 3 && cursorPos.x < 1200 * 2 / 3)	mapData = 2;
		else if (cursorPos.x >= 1200 * 2 / 3 && cursorPos.x < 1200) mapData = 3;

		mapImage = mapImages[mapData - 1];
		sendLobbyPacket.selectedMap = mapData;

		showSelectMap = !showSelectMap;
	}

	if (cursorPos.x > 975 && cursorPos.x < 1155 &&
		cursorPos.y > 640 && cursorPos.y < 705) showSelectMap = !showSelectMap;
}

void CLobbyScene::SendData()
{
	sendLobbyPacket.header.packetType = 2;

	if (pastData != sendLobbyPacket.selectedMap || sendLobbyPacket.nextSceneCall != pastCallData) {
		pastData = sendLobbyPacket.selectedMap;
		pastCallData = sendLobbyPacket.nextSceneCall;
		int retval = send(sock, (char*)&sendLobbyPacket, sizeof(sendLobbyPacket), 0);
	}
}

void CLobbyScene::ReceiveData()
{
	int retval = recv(sock, (char*)&recvLobbyPacket, sizeof(recvLobbyPacket), 0);
	if (recvLobbyPacket.recvNextSceneCall == true) {
		GetFramework()->SetCurMap(recvLobbyPacket.selectedMap - 1);
	}

}


