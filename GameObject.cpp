#include "GameObject.h"
#include "Map.h"
CGameObject::CGameObject()
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::Render(HDC MemDC, HDC MemDCImage, CMap* Map)
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


CBallon::CBallon(HINSTANCE hInst)
{
	MainBitmap[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Bubble));
	MainBitmap[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Explosion));

	state = 0;
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

void CBallon::Render(HDC MemDC, HDC MemDCImage, CMap* Map)
{
	(HBITMAP)SelectObject(MemDCImage, MainBitmap[0]);      // ¹°Ç³¼±
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (Map->GetBoard(i,j).GetState() == 4 || Map->GetBoard(i,j).GetState() == 40)
			{
				TransparentBlt(MemDC, j * 60 + 30, i * 60 + 65, 60, 60, MemDCImage, xPos, 0, 44, 41, RGB(255, 0, 255));   // ¹°Ç³¼± ¼³Ä¡
			}
		}
	}

	//(HBITMAP)SelectObject(MemDCImage, explosion);      // ¹°Ç³¼± Æø¹ß
	//for (int i = 0; i < 13; i++)
	//{
	//	for (int j = 0; j < 15; j++)
	//	{
	//		if (Map->GetBoard(i,j).state == 5)
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

void CItem::Render(HDC MemDC, HDC MemDCImage, CMap* Map)
{
}
