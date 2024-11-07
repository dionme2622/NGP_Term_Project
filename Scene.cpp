#include "Scene.h"
#include "GameFramework.h"

CScene::CScene(CGameFramework* GameFramework)
{
	m_pGameFramework = GameFramework;
}

CScene::~CScene()
{
}

void CScene::Initialize(HWND hwnd, HINSTANCE g_hInst)
{
	hWnd = hwnd;
	hInst = g_hInst;
	GetClientRect(hWnd, &rc);

}

void CScene::DrawImage(HDC hdcDest, RECT pos, HDC hdcSrc, RECT rc, DWORD rop)
{
	TransparentBlt(hdcDest, pos.left, pos.top, pos.right, pos.bottom, hdcSrc, 
							 rc.left,  rc.top,  rc.right,  rc.bottom, RGB(255, 0, 255));
}
	







