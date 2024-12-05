#include "Scene.h"
#include "GameFramework.h"

int CScene::m_ID;
SOCKET CScene::sock;

CScene::CScene(HWND _hWnd, HINSTANCE _hInst, CGameFramework* GameFramework)
{
	hWnd = _hWnd;
	hInst = _hInst;
	GetClientRect(hWnd, &rc);
	m_pGameFramework = GameFramework;
}

CScene::~CScene()
{
}

void CScene::Initialize()
{
	

}

void CScene::DrawImage(HDC hdcDest, RECT pos, HDC hdcSrc, RECT rc, DWORD rop)
{
	TransparentBlt(hdcDest, pos.left, pos.top, pos.right, pos.bottom, hdcSrc, 
							 rc.left,  rc.top,  rc.right,  rc.bottom, RGB(255, 0, 255));
}

void CScene::SetID(int id)
{
	m_ID = id;
}
	







