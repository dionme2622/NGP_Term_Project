#include "PlayScene.h"
#include "stdafx.h"
#include "GameFramework.h"

#define MAP GetFramework()->GetCurMap()

enum {
	UP = 0, DOWN, LEFT, RIGHT
};

CPlayScene::CPlayScene(HWND _hWnd, HINSTANCE _hInst, CGameFramework* GameFramework) : CScene(_hWnd, _hInst, GameFramework)
{
	player = NULL;

}

CPlayScene::~CPlayScene()
{
}

void CPlayScene::Initialize()
{
	// TODO : Bitmap, Map, Player�� �����͸� Initialize �Ѵ�.
	
	GetFramework()->GetCurMap()->Initialize(hInst);		// ���õ� Map�� Initialize


	// Resource
	backgroundImage = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_STAGEFRAME));

}


void CPlayScene::ProcessInput()
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	GetKeyboardState(pKeysBuffer);

	// TODO : Player�� ���� ���͸� �����Ѵ�.
	if (pKeysBuffer[VK_UP] & 0xF0) player->SetDirection(UP);
	if (pKeysBuffer[VK_DOWN] & 0xF0) player->SetDirection(DOWN);
	if (pKeysBuffer[VK_LEFT] & 0xF0) player->SetDirection(LEFT);
	if (pKeysBuffer[VK_RIGHT] & 0xF0) player->SetDirection(RIGHT);

}

void CPlayScene::Update(float fTimeElapsed)
{
	// TODO : Play Scene Update

	// 1. Player�� ���⺤�Ϳ� ���� �����δ�.
	// 2. Bitmap�� ��ǥ�� �������� �ִϸ��̼��� �ִ´�.
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
