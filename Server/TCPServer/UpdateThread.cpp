//#include "UpdateThread.h"
//
//
//void PlayerMove(CHARACTER* Player, bool isDownKeyPressed, bool isLeftKeyPressed, bool isRightKeyPressed, bool isUpKeyPressed)		// Player가 움직이는 함수
//{
//
//	if (isDownKeyPressed)
//	{
//		Player->direction = DOWN;
//		Player->stop = false;
//		if (Player->y >= Board[12][0].y) {}
//		else Player->y += Player->speed;
//		PlayerMeetObstacle(Player);
//	}
//	else if (isLeftKeyPressed)
//	{
//		// 왼쪽 방향키가 눌려있을 때의 이동 로직
//		Player->direction = LEFT;
//		Player->stop = false;
//		if (Player->x <= Board[0][0].x) {}
//		else Player->x -= Player->speed;
//		PlayerMeetObstacle(Player);
//	}
//	else if (isRightKeyPressed)
//	{
//		// 오른쪽 방향키가 눌려있을 때의 이동 로직
//		Player->direction = RIGHT;
//		Player->stop = false;
//		if (Player->x >= Board[0][14].x) {}
//		else Player->x += Player->speed;
//		PlayerMeetObstacle(Player);
//	}
//	else if (isUpKeyPressed)
//	{
//		// 위 방향키가 눌려있을 때의 이동 로직
//		Player->direction = UP;
//		Player->stop = false;
//		if (Player->y <= Board[0][0].y) {}		// if ( (rect1.left < rect2.right) && (rect1.top < rect2.bottom) && (rect1.right > rect2.left) && 			(rect1.bottom > rect2.top) )
//		else Player->y -= Player->speed;
//		PlayerMeetObstacle(Player);
//	}
//}
//void PlayerMeetObstacle(CHARACTER* Player)         
//{
//	if (Player->direction == LEFT)
//	{
//		int i = (Player->x - 30) / 60;
//		int j = (Player->y - 65) / 60;
//		if (Board[j][i].state == 2 || Board[j][i].state == 3 || Board[j][i].state == 40)
//		{
//			Player->x = i * 60 + 60 + 30;
//			j = (Player->y + 60 - 65) / 60;
//			if ((Player->y % 60 >= 45 || Player->y % 60 <= 5) && (Board[j][i].state != 2 && Board[j][i].state != 3 && Board[j][i].state != 40) && (Board[j][i + 1].state != 2 && Board[j][i + 1].state != 3 && Board[j][i + 1].state != 40))
//				Player->y = Player->y + 1;
//		}
//		j = (Player->y + 60 - 65) / 60;
//		if ((Board[j][i].state == 2 || Board[j][i].state == 3 || Board[j][i].state == 40) && Player->y % 60 != 5)
//		{
//			Player->x = i * 60 + 60 + 30;
//			j = (Player->y - 65) / 60;
//			if (Player->y % 60 <= 25 && Player->y % 60 >= 5 && (Board[j][i].state != 2 && Board[j][i].state != 3 && Board[j][i].state != 40) && (Board[j][i + 1].state != 2 && Board[j][i + 1].state != 3 && Board[j][i + 1].state != 40))
//				Player->y = Player->y - 1;
//		}
//	}
//	else if (Player->direction == RIGHT)
//	{
//		int i = (Player->x - 30) / 60;
//		int j = (Player->y - 65) / 60;
//		if (Board[j][i + 1].state == 2 || Board[j][i + 1].state == 3 || Board[j][i + 1].state == 40)
//		{
//			Player->x = i * 60 + 30;
//			j = (Player->y + 60 - 65) / 60;
//			if ((Player->y % 60 >= 45 || Player->y % 60 <= 5) && (Board[j][i + 1].state != 2 && Board[j][i + 1].state != 3 && Board[j][i + 1].state != 40) && (Board[j][i - 1].state != 2 && Board[j][i - 1].state != 3 && Board[j][i - 1].state != 40))
//				Player->y = Player->y + 1;
//		}
//		j = (Player->y + 60 - 65) / 60;
//		if ((Board[j][i + 1].state == 2 || Board[j][i + 1].state == 3 || Board[j][i + 1].state == 40) && Player->y % 60 != 5)
//		{
//			Player->x = i * 60 + 30;
//			j = (Player->y - 65) / 60;
//			if (Player->y % 60 <= 25 && Player->y % 60 >= 5 && (Board[j][i + 1].state != 2 && Board[j][i + 1].state != 3 && Board[j][i + 1].state != 40) && (Board[j][i - 1].state != 2 && Board[j][i - 1].state != 3 && Board[j][i - 1].state != 40))
//				Player->y = Player->y - 1;
//		}
//	}
//	else if (Player->direction == UP)
//	{
//		int i = (Player->x - 30) / 60;
//		int j = (Player->y - 65) / 60;
//		if (Board[j][i].state == 2 || Board[j][i].state == 3 || Board[j][i].state == 40)
//		{
//			Player->y = j * 60 + 60 + 65;
//			i = (Player->x + 60 - 30) / 60;
//			if (Player->x % 60 <= 30 && Player->x % 60 >= 10 && (Board[j][i].state != 2 && Board[j][i].state != 3 && Board[j][i].state != 40) && (Board[j + 1][i].state != 2 && Board[j + 1][i].state != 3 && Board[j + 1][i].state != 40))
//				Player->x = Player->x + 1;
//		}
//		i = (Player->x + 60 - 30) / 60;
//		if ((Board[j][i].state == 2 || Board[j][i].state == 3 || Board[j][i].state == 40) && Player->x % 60 != 30)
//		{
//			Player->y = j * 60 + 60 + 65;
//			i = (Player->x - 30) / 60;
//			if (Player->x % 60 <= 50 && Player->x % 60 >= 30 && (Board[j][i].state != 2 && Board[j][i].state != 3 && Board[j][i].state != 40) && (Board[j + 1][i].state != 2 && Board[j + 1][i].state != 3 && Board[j + 1][i].state != 40))
//				Player->x = Player->x - 1;
//		}
//	}
//	else if (Player->direction == DOWN)
//	{
//		int i = (Player->x - 30) / 60;
//		int j = (Player->y - 65) / 60;
//		if (Board[j + 1][i].state == 2 || Board[j + 1][i].state == 3 || Board[j + 1][i].state == 40)
//		{
//			Player->y = j * 60 + 65;
//			i = (Player->x + 60 - 30) / 60;
//			if (Player->x % 60 <= 30 && Player->x % 60 >= 10 && (Board[j + 1][i].state != 2 && Board[j + 1][i].state != 3 && Board[j + 1][i].state != 40) && (Board[j - 1][i].state != 2 && Board[j - 1][i].state != 3 && Board[j - 1][i].state != 40))
//				Player->x = Player->x + 1;
//		}
//		i = (Player->x + 60 - 30) / 60;
//		if ((Board[j + 1][i].state == 2 || Board[j + 1][i].state == 3 || Board[j + 1][i].state == 40) && Player->x % 60 != 30)
//		{
//			Player->y = j * 60 + 65;
//			i = (Player->x - 30) / 60;
//			if (Player->x % 60 <= 50 && Player->x % 60 >= 30 && (Board[j + 1][i].state != 2 && Board[j + 1][i].state != 3 && Board[j + 1][i].state != 40) && (Board[j - 1][i].state != 2 && Board[j - 1][i].state != 3 && Board[j - 1][i].state != 40))
//				Player->x = Player->x - 1;
//		}
//	}
//}
//void PlayerGetItem(CHARACTER* Player)
//{
//	int i = (Player->x + 30 - 30) / 60;
//	int j = (Player->y + 30 - 65) / 60;
//
//	if (Board[j][i].state == 6)		// 이동속도 아이템
//	{
//		if (Player->speed < 10)
//			Player->speed += 1;
//		Board[j][i].state = 1;
//
//	}
//	else if (Board[j][i].state == 7)		// 물줄기 아이템
//	{
//		if (Player->ballon_length < 6)
//			Player->ballon_length += 1;
//		Board[j][i].state = 1;
//	}
//	else if (Board[j][i].state == 8)		// 물풍선 갯수 아이템
//	{
//		if (Player->ballon_num < 5)
//			Player->ballon_num += 1;
//		Board[j][i].state = 1;
//	}
//}
//void PlayerMeetBallon(CHARACTER* Player)
//{
//	int i = (Player->x + 30 - 30) / 60;
//	int j = (Player->y + 30 - 65) / 60;
//	{
//		if (Board[j][i].state == 5)
//		{
//			if (Player->state == LIVE)
//				Player->state = DAMAGE;
//		}
//	}
//}
//void BallonBoom(CHARACTER* Player, int num)
//{
//	/*if (timer3_count == 0)
//		ssystem->playSound(sound[5], 0, false, &channel2);
//	if (timer7_count == 0)
//		ssystem->playSound(sound[5], 0, false, &channel2);*/
//
//	if (Player->ballon[num].state == 1)
//	{
//		if (Board[(Player->ballon[num].y) / 60][(Player->ballon[num].x) / 60].state == 5)
//			Player->ballon[num].beforeboomcouont = 70;
//		if (Player->ballon[num].beforeboomcouont < 70)
//			Player->ballon[num].beforeboomcouont++;
//		else
//			Player->ballon[num].state = 2;
//	}
//	else if (Player->ballon[num].state == 2)
//	{
//		if (Player->ballon[num].startboomcount < 10)
//		{
//			Board[(Player->ballon[num].y) / 60][(Player->ballon[num].x) / 60].state = 5;
//			for (int j = 1; j < Player->ballon_length; j++)
//			{
//				int x = ((Player->ballon[num].x) + 60 * j) / 60;
//				int y = (Player->ballon[num].y) / 60;
//				if (x < 0 || x == MapXMax)
//					break;
//				if (Board[y][x].state == 1 || Board[y][x].state == 4 || Board[y][x].state == 40 || Board[y][x].state == 6 || Board[y][x].state == 7 || Board[y][x].state == 8 || Board[y][x].state == 9)
//					Board[y][x].state = 5;
//				else if (Board[y][x].state == 2)
//					break;
//				else if (Board[y][x].state == 3)
//					break;
//			}
//			for (int j = 1; j < Player->ballon_length; j++)
//			{
//				int x = ((Player->ballon[num].x) - 60 * j) / 60;
//				int y = (Player->ballon[num].y) / 60;
//				if (x < 0 || x == MapXMax)
//					break;
//				if (Board[y][x].state == 1 || Board[y][x].state == 4 || Board[y][x].state == 40 || Board[y][x].state == 6 || Board[y][x].state == 7 || Board[y][x].state == 8 || Board[y][x].state == 9)
//					Board[y][x].state = 5;
//				else if (Board[y][x].state == 2)
//					break;
//				else if (Board[y][x].state == 3)
//					break;
//			}
//			for (int j = 1; j < Player->ballon_length; j++)
//			{
//				int x = (Player->ballon[num].x) / 60;
//				int y = ((Player->ballon[num].y) + 60 * j) / 60;
//				if (Board[y][x].state == 1 || Board[y][x].state == 4 || Board[y][x].state == 40 || Board[y][x].state == 6 || Board[y][x].state == 7 || Board[y][x].state == 8 || Board[y][x].state == 9)
//					Board[y][x].state = 5;
//				else if (Board[y][x].state == 2)
//					break;
//				else if (Board[y][x].state == 3)
//					break;
//			}
//			for (int j = 1; j < Player->ballon_length; j++)
//			{
//				int x = (Player->ballon[num].x) / 60;
//				int y = ((Player->ballon[num].y) - 60 * j) / 60;
//				if (Board[y][x].state == 1 || Board[y][x].state == 4 || Board[y][x].state == 40 || Board[y][x].state == 6 || Board[y][x].state == 7 || Board[y][x].state == 8 || Board[y][x].state == 9)
//					Board[y][x].state = 5;
//				else if (Board[y][x].state == 2)
//					break;
//				else if (Board[y][x].state == 3)
//					break;
//
//			}
//			Player->ballon[num].beforeboomcouont = 0;
//			Player->ballon[num].startboomcount++;
//		}
//
//		if (Player->ballon[num].startboomcount >= 10)
//		{
//			Board[(Player->ballon[num].y) / 60][(Player->ballon[num].x) / 60].state = 5;
//			for (int j = 1; j < Player->ballon_length; j++)
//			{
//				int x = ((Player->ballon[num].x) + 60 * j) / 60;
//				int y = (Player->ballon[num].y) / 60;
//				if (x < 0 || x == MapXMax)
//					break;
//				if (Board[y][x].state == 1 || Board[y][x].state == 4 || Board[y][x].state == 40 || Board[y][x].state == 6 || Board[y][x].state == 7 || Board[y][x].state == 8 || Board[y][x].state == 9)
//					Board[y][x].state = 5;
//				else if (Board[y][x].state == 2)
//				{
//					ObstacleBreak[y][x] = 1;
//					break;
//				}
//				else if (Board[y][x].state == 3)
//					break;
//			}
//			for (int j = 1; j < Player->ballon_length; j++)
//			{
//				int x = ((Player->ballon[num].x) - 60 * j) / 60;
//				int y = (Player->ballon[num].y) / 60;
//				if (x < 0 || x == MapXMax)
//					break;
//				if (Board[y][x].state == 1 || Board[y][x].state == 4 || Board[y][x].state == 40 || Board[y][x].state == 6 || Board[y][x].state == 7 || Board[y][x].state == 8 || Board[y][x].state == 9)
//					Board[y][x].state = 5;
//				else if (Board[y][x].state == 2)
//				{
//					ObstacleBreak[y][x] = 1;
//					break;
//				}
//				else if (Board[y][x].state == 3)
//					break;
//			}
//			for (int j = 1; j < Player->ballon_length; j++)
//			{
//				int x = (Player->ballon[num].x) / 60;
//				int y = ((Player->ballon[num].y) + 60 * j) / 60;
//				if (Board[y][x].state == 1 || Board[y][x].state == 4 || Board[y][x].state == 40 || Board[y][x].state == 6 || Board[y][x].state == 7 || Board[y][x].state == 8 || Board[y][x].state == 9)
//					Board[y][x].state = 5;
//				else if (Board[y][x].state == 2)
//				{
//					ObstacleBreak[y][x] = 1;
//					break;
//				}
//				else if (Board[y][x].state == 3)
//					break;
//			}
//			for (int j = 1; j < Player->ballon_length; j++)
//			{
//				int x = (Player->ballon[num].x) / 60;
//				int y = ((Player->ballon[num].y) - 60 * j) / 60;
//				if (Board[y][x].state == 1 || Board[y][x].state == 4 || Board[y][x].state == 40 || Board[y][x].state == 6 || Board[y][x].state == 7 || Board[y][x].state == 8 || Board[y][x].state == 9)
//					Board[y][x].state = 5;
//				else if (Board[y][x].state == 2)
//				{
//					ObstacleBreak[y][x] = 1;
//					break;
//				}
//				else if (Board[y][x].state == 3)
//					break;
//
//			}
//			Player->ballon[num].startboomcount++;
//			Player->ballon[num].state = 0;
//			for (int i = 0; i < MapXMax; i++)
//				for (int j = 0; j < MapYMax; j++)
//					if (ObstacleBreak[j][i] == 1)
//					{
//						int n = uid(dre);
//						if (n == 6)
//							Board[j][i].state = 6;
//						else if (n == 7)
//							Board[j][i].state = 7;
//						else if (n == 8)
//							Board[j][i].state = 8;
//						else if (n == 9)
//							Board[j][i].state = 9;
//						else
//							Board[j][i].state = 1;
//					}
//			for (int i = 0; i < MapXMax; i++)
//				for (int j = 0; j < MapYMax; j++)
//					ObstacleBreak[j][i] = 0;
//			Board[(Player->ballon[num].y) / 60][(Player->ballon[num].x) / 60].state = 1;
//			for (int j = 1; j <= Player->ballon_length; j++)
//			{
//				int x = ((Player->ballon[num].x) + 60 * j) / 60;
//				int y = (Player->ballon[num].y) / 60;
//				if (Board[y][x].state == 5)
//					Board[y][x].state = 1;
//				else if (Board[y][x].state == 2)
//					break;
//				else if (Board[y][x].state == 3)
//					break;
//			}
//			for (int j = 1; j <= Player1.ballon_length; j++)
//			{
//				int x = ((Player->ballon[num].x) - 60 * j) / 60;
//				int y = (Player->ballon[num].y) / 60;
//				if (x < 0 || x == MapXMax)
//					break;
//				if (Board[y][x].state == 5)
//					Board[y][x].state = 1;
//				else if (Board[y][x].state == 2)
//					break;
//				else if (Board[y][x].state == 3)
//					break;
//			}
//			for (int j = 1; j <= Player->ballon_length; j++)
//			{
//				int x = (Player->ballon[num].x) / 60;
//				int y = ((Player->ballon[num].y) + 60 * j) / 60;
//				if (Board[y][x].state == 5)
//					Board[y][x].state = 1;
//				else if (Board[y][x].state == 2)
//					break;
//				else if (Board[y][x].state == 3)
//					break;
//			}
//			for (int j = 1; j <= Player->ballon_length; j++)
//			{
//				int x = (Player->ballon[num].x) / 60;
//				int y = ((Player->ballon[num].y) - 60 * j) / 60;
//				if (Board[y][x].state == 5)
//					Board[y][x].state = 1;
//				else if (Board[y][x].state == 2)
//					break;
//				else if (Board[y][x].state == 3)
//					break;
//			}
//			Player->ballon[num].startboomcount = 0;
//		}
//	}
//
//}
//void BallonToObstacle(CHARACTER* Player)
//{
//	for (int i = 0; i < Player->ballon_num; i++)
//	{
//		int xdis = abs(Player->x + 30 - (Player->ballon[i].x + 30 + 30));
//		int ydis = abs(Player->y + 30 - (Player->ballon[i].y + 30 + 65));
//		int dis;
//		if (xdis > ydis)
//			dis = xdis;
//		else
//			dis = ydis;
//		if (Player->ballon[i].state == 1 && dis >= 60)
//			Board[Player->ballon[i].y / 60][Player->ballon[i].x / 60].state = 40;
//	}
//}
//
//
//// 물풍선에서 죽이기
//void PlayerMeetPlayer(CHARACTER* Player1, CHARACTER* Player2)
//{
//	if (Player2->state == DAMAGE)
//	{
//		if ((Player1->x < Player2->x + 60) && (Player2->x < Player1->x + 60) && (Player1->y < Player2->y + 60) && (Player2->y < Player1->y + 60))
//		{
//			Player2->state = DEAD;
//		}
//	}
//}