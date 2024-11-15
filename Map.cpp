#include "Map.h"

void CMap::Initialize(HINSTANCE _hInst)
{
	hInst = _hInst;
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			Board[i][j].SetPosition((j * 60 + 30), (i * 60 + 65));
			Board[i][j].SetState(1);
		}
	}
}


/// <summary>
/// Village Map
/// </summary>

void CVillage::Initialize(HINSTANCE _hInst)
{
	// TODO : Map이 Village일 때 초기화
	CMap::Initialize(_hInst);
	for (int i = 2; i < 13; i++)
	{
		Board[0][i].SetState(2);
		Board[12][i].SetState(2);
	}
	for (int i = 4; i < 11; i++)
	{
		Board[1][i].SetState(2);
		Board[11][i].SetState(2);
	}
	Board[1][1].SetState(3); Board[1][3].SetState(3); Board[1][11].SetState(3); Board[1][13].SetState(3);
	Board[3][1].SetState(3); Board[3][3].SetState(3); Board[3][11].SetState(3); Board[3][13].SetState(3);
	Board[5][1].SetState(3); Board[5][3].SetState(3); Board[5][11].SetState(3); Board[5][13].SetState(3);
	Board[5][1].SetState(3); Board[5][3].SetState(3); Board[5][11].SetState(3); Board[5][13].SetState(3);
	Board[7][1].SetState(3); Board[7][3].SetState(3); Board[7][11].SetState(3); Board[7][13].SetState(3);
	Board[6][5].SetState(3); Board[6][7].SetState(3); Board[6][9].SetState(3);	Board[10][9].SetState(3);
	Board[9][1].SetState(3); Board[9][3].SetState(3); Board[9][11].SetState(3); Board[9][13].SetState(3);
	Board[4][6].SetState(3); Board[4][8].SetState(3); Board[8][6].SetState(3);	Board[8][8].SetState(3);

	for (int i = 0; i < 5; i++)
	{
		Board[2][i].SetState(2);
		Board[4][i].SetState(2);
		Board[8][i].SetState(2);
		Board[10][i].SetState(2);
	}
	for (int i = 10; i < 15; i++)
	{
		Board[2][i].SetState(2);
		Board[4][i].SetState(2);
		Board[8][i].SetState(2);
		Board[10][i].SetState(2);
	}
	Board[3][0].SetState(2); Board[3][5].SetState(2); Board[3][9].SetState(2); Board[3][14].SetState(2);
	Board[10][0].SetState(2); Board[10][5].SetState(2); Board[10][9].SetState(2); Board[10][14].SetState(2);
	for (int i = 5; i < 10; i++)
	{
		Board[5][i].SetState(2);
		Board[7][i].SetState(2);
	}
	Board[10][2].SetState(1); Board[10][5].SetState(1); Board[10][9].SetState(1); Board[10][12].SetState(1);

	// Resource
	TileBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));

	printf("Village Initialize\n");

}
void CVillage::Render(HDC MemDC, HDC MemDCImage)
{
	(HBITMAP)SelectObject(MemDCImage, TileBitmap); //--- 배경 이미지
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			StretchBlt(MemDC, Board[i][j].x, Board[i][j].y, 60, 60, MemDCImage, 0, 0, 40, 40, SRCCOPY);                  // Tile
		}
	}
}


/// <summary>
/// Pirate Map
/// </summary>

void CPirate::Initialize(HINSTANCE _hInst)
{
	// TODO : Map이 Pirate일 때 초기화
	CMap::Initialize(_hInst);

	for (int i = 0; i < 4; i++)
	{
		Board[0][i].SetState(2);
	}
	Board[0][7].SetState(2);
	for (int i = 11; i < 15; i++)
	{
		Board[0][i].SetState(2);
	}
	// 1줄
	Board[1][0].SetState(2);
	Board[1][1].SetState(3);
	Board[1][2].SetState(2);
	for (int i = 4; i < 7; i++)
	{
		Board[1][i].SetState(2);
	}
	for (int i = 8; i < 11; i++)
	{
		Board[1][i].SetState(2);
	}
	Board[1][12].SetState(2);
	Board[1][13].SetState(3);
	Board[1][14].SetState(2);
	// 2줄
	Board[2][0].SetState(2);
	Board[2][1].SetState(2);
	Board[2][3].SetState(2);
	Board[2][5].SetState(2);
	Board[2][7].SetState(2);
	Board[2][9].SetState(2);
	Board[2][11].SetState(2);
	Board[2][13].SetState(2);
	Board[2][14].SetState(2);
	// 3줄
	for (int i = 3; i < 13; i++) Board[i][0].SetState(2);
	for (int i = 7; i < 11; i++) Board[i][1].SetState(2);
	
	for (int i = 8; i < 10; i++) Board[i][2].SetState(2);
	
	for (int i = 9; i < 13; i++) Board[i][3].SetState(2);

	for (int i = 10; i < 13; i++) Board[i][4].SetState(2);
	
	for (int i = 11; i < 13; i++) Board[i][5].SetState(2);
	
	for (int i = 6; i < 12; i++) Board[12][i].SetState(2);

	for (int i = 9; i < 12; i++) Board[11][i].SetState(2);

	for (int i = 3; i < 13; i++) Board[i][14].SetState(2);

	for (int i = 10; i < 12; i++) Board[10][i].SetState(2);

	for (int i = 11; i < 14; i++) Board[9][i].SetState(2);
	
	for (int i = 12; i < 14; i++) Board[8][i].SetState(2);
	
	Board[7][13].SetState(2);
	for (int i = 4; i < 11; i++) Board[3][i].SetState(2);
	
	for (int i = 3; i < 12; i++) Board[4][i].SetState(2);
	for (int i = 4; i < 11; i++) Board[5][i].SetState(2);
	
	for (int i = 3; i < 6; i++) Board[6][i].SetState(2);
	
	for (int i = 9; i < 12; i++) Board[6][i].SetState(2);
	
	for (int i = 5; i < 10; i++)
	{
		Board[7][i].SetState(2);
		Board[8][i].SetState(2);
	}
	Board[9][7].SetState(2);
	for (int i = 3; i < 7; i++)
	{
		Board[i][2].SetState(2);
		Board[i][12].SetState(2);
	}
	Board[7][3].SetState(2);
	Board[7][11].SetState(2);
	Board[8][4].SetState(2);
	Board[8][10].SetState(2);
	Board[9][5].SetState(2);
	Board[9][9].SetState(2);
	for (int i = 6; i < 9; i++) Board[10][i].SetState(2);
	Board[10][13].SetState(2);
	Board[11][1].SetState(3);
	Board[11][13].SetState(3);
	Board[6][6].SetState(3); Board[6][7].SetState(3); Board[6][8].SetState(3);


	// Resource
	TileBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP12));

}
void CPirate::Render(HDC MemDC, HDC MemDCImage)
{
	(HBITMAP)SelectObject(MemDCImage, TileBitmap); //--- 배경 이미지
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			StretchBlt(MemDC, Board[i][j].x, Board[i][j].y, 60, 60, MemDCImage, 0, 0, 40, 40, SRCCOPY);                  // Tile
		}
	}
}

