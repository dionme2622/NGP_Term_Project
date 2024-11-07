#include "StartScene.h"
CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

void CStartScene::Initialize(HWND hwnd, HINSTANCE g_hInst)
{
	CScene::Initialize(hwnd, g_hInst);
}


void CStartScene::ProcessInput()
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	GetKeyboardState(pKeysBuffer);
	if (pKeysBuffer[VK_DOWN] & 0xF0)
	{
		/*framework->currentscene = LOBBYSCENE;
		*framework->scene = framework->scenes[framework->currentscene];*/
	}
	/*if (pKeysBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
	if (pKeysBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
	if (pKeysBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
	if (pKeysBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;

	if (pKeysBuffer['W'] & 0xF0) dwDirection |= DIR_FORWARD;
	if (pKeysBuffer['S'] & 0xF0) dwDirection |= DIR_BACKWARD;
	if (pKeysBuffer['A'] & 0xF0) dwDirection |= DIR_LEFT;*/
}

void CStartScene::Update()
{
}

void CStartScene::Render()
{
	static RECT rect = { 50, 50, 200, 150 };
	rect.left += 1;
	rect.top += 1;
	rect.right += 1;
	rect.bottom += 1;
	//InvalidateRect(hwnd, NULL, TRUE);  // TRUE로 설정해 배경을 지웁니다.
	HDC hdc = GetDC(hWnd);
	MemDC = CreateCompatibleDC(hdc); //--- 더블 버퍼로 사용 할 메모리 DC 생성
	
	hBit = CreateCompatibleBitmap(hdc, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	OldBit[0] = (HBITMAP)SelectObject(MemDC, hBit); //--- MemDC와 hBit 를 호환되게 선택함
	// 빨간색 브러시 생성
	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
	
	FillRect(MemDC, &rect, redBrush);

	BitBlt(hdc, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, MemDC, 0, 0, SRCCOPY);
	// 자원 해제
	DeleteObject(redBrush);
	SelectObject(MemDC, OldBit);             // 이전 비트맵으로 복구
	DeleteObject(hBit);                      // 비트맵 삭제
	DeleteDC(MemDC);                            // 메모리 DC 삭제
	ReleaseDC(hWnd, hdc);                       // 화면 DC 해제
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
