#include "PlayScene.h"
#include "Windows.h"

CPlayScene::CPlayScene(CGameFramework* GameFramework) : CScene(GameFramework)
{
	player = NULL;

}

CPlayScene::~CPlayScene()
{
}

void CPlayScene::Initialize(HWND hwnd, HINSTANCE g_hInst)
{
	CScene::Initialize(hwnd, g_hInst);

	backgroundImage = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_STAGEFRAME));
}


void CPlayScene::ProcessInput()
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	GetKeyboardState(pKeysBuffer);

	// TODO : Player의 방향 벡터를 설정한다.
	if (pKeysBuffer[VK_UP] & 0xF0) ;
	
}

void CPlayScene::Update(float fTimeElapsed)
{
	// TODO : Play Scene Update

	// 1. Player의 방향벡터에 따라 움직인다.
	// 2. Bitmap의 좌표를 움직여서 애니메이션을 넣는다.
}

void CPlayScene::Render()
{
	HDC hdc = GetDC(hWnd);

	HDC MemDC = CreateCompatibleDC(hdc);
	HDC MemDCImage = CreateCompatibleDC(hdc);

	hBit = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	OldBit[0] = (HBITMAP)SelectObject(MemDC, hBit);
	OldBit[1] = (HBITMAP)SelectObject(MemDCImage, backgroundImage); //--- 배경 이미지	

	//StretchBlt(MemDC, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, MemDCImage, 0, 0, 810, 650, SRCCOPY);
	TransparentBlt(MemDC, 0, 0, rc.right, rc.bottom, MemDCImage, 0, 0, 800, 600, RGB(255, 0, 255));

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
