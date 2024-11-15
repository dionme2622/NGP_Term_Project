#include "StartScene.h"
#include "GameFramework.h"
CStartScene::CStartScene(HWND _hWnd, HINSTANCE _hInst, CGameFramework* GameFramework) : CScene(_hWnd, _hInst, GameFramework)
{
}

CStartScene::~CStartScene()
{
}

void CStartScene::Initialize()
{
}


void CStartScene::ProcessInput()
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	GetKeyboardState(pKeysBuffer);
	if (pKeysBuffer[VK_DOWN] & 0xF0)
	{
		GetFramework()->SetCurScene(MENUSCENE);
	}
}

void CStartScene::Update(float fTimeElapsed)
{
	// TODO : Start Scene Update
}

void CStartScene::Render()
{
	static RECT rect = { 50, 50, 200, 150 };
	rect.left += 1;
	rect.top += 1;
	rect.right += 1;
	rect.bottom += 1;
	//InvalidateRect(hwnd, NULL, TRUE);  // TRUE�� ������ ����� ����ϴ�.
	HDC hdc = GetDC(hWnd);
	MemDC = CreateCompatibleDC(hdc); //--- ���� ���۷� ��� �� �޸� DC ����
	
	hBit = CreateCompatibleBitmap(hdc, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	OldBit[0] = (HBITMAP)SelectObject(MemDC, hBit); //--- MemDC�� hBit �� ȣȯ�ǰ� ������
	// ������ �귯�� ����
	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
	
	FillRect(MemDC, &rect, redBrush);

	BitBlt(hdc, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, MemDC, 0, 0, SRCCOPY);
	// �ڿ� ����
	DeleteObject(redBrush);
	SelectObject(MemDC, OldBit);             // ���� ��Ʈ������ ����
	DeleteObject(hBit);                      // ��Ʈ�� ����
	DeleteDC(MemDC);                            // �޸� DC ����
	ReleaseDC(hWnd, hdc);                       // ȭ�� DC ����
}

void CStartScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:

	case WM_RBUTTONDOWN:
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
