#include "GameObject.h"

CGameObject::CGameObject()
{
}

CGameObject::~CGameObject()
{
}

CBoard::CBoard()
{
}

CBoard::~CBoard()
{
}

void CBoard::Update(float fTimeElapsed)
{

}

void CBoard::Render(HDC MemDC, HDC MemDCImage)
{
}

CBallon::CBallon(HINSTANCE hInst)
{
	printf("Ballon »ý¼º\n");
	MainBitmap[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Bubble));
	MainBitmap[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Explosion));
}

CBallon::~CBallon()
{
}

void CBallon::Update(float fTimeElapsed)
{
	xPosF += 44 * 15 * fTimeElapsed;
	while (xPosF >= 44.0f) {
		xPosF = 0;
		xPos += 44;
		if (xPos >= 132)
			xPos = 0;
	}
}

void CBallon::Render(HDC MemDC, HDC MemDCImage)
{
	//(HBITMAP)SelectObject(MemDCImage, MainBitmap[0]);      // ¹°Ç³¼±
	//for (int i = 0; i < 13; i++)
	//{
	//	for (int j = 0; j < 15; j++)
	//	{
	//		if (Board[i][j].state == 4 || Board[i][j].state == 40)
	//		{

	//			TransparentBlt(MemDC, j * 60 + 30, i * 60 + 65, 60, 60, MemDCImage, xPos, 0, 44, 41, RGB(255, 0, 255));   // ¹°Ç³¼± ¼³Ä¡
	//		}
	//	}
	//}

	//(HBITMAP)SelectObject(MemDCImage, explosion);      // ¹°Ç³¼± Æø¹ß
	//for (int i = 0; i < 13; i++)
	//{
	//	for (int j = 0; j < 15; j++)
	//	{
	//		if (Board[i][j].state == 5)
	//		{

	//			TransparentBlt(MemDC, j * 60 + 30, i * 60 + 65, 60, 60, MemDCImage, 0, 0, 40, 40, RGB(255, 0, 255));   // ¹°Ç³¼± ¼³Ä¡
	//		}
	//	}
	//}
}

CItem::CItem()
{
}

CItem::~CItem()
{
}

void CItem::Update(float fTimeElapsed)
{
}

void CItem::Render(HDC MemDC, HDC MemDCImage)
{
}
