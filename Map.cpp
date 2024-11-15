#include "Map.h"

void CMap::Initialize()
{
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

void CVillage::Initialize()
{
	// TODO : Map이 Village일 때 초기화
	CMap::Initialize();
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

	printf("Village Initialize\n");
}


/// <summary>
/// Pirate Map
/// </summary>

void CPirate::Initialize()
{
	// TODO : Map이 Pirate일 때 초기화
	CMap::Initialize();
	printf("Pirate Initialize\n");

}
