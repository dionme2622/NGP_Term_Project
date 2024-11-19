#include <stdbool.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <random>

#define MAX_BALLONS 6
#define MapXMax 15
#define MapYMax 13

using namespace std;
default_random_engine dre;
uniform_int_distribution<int> uid{ 1,9 };

enum { DOWN = 2000, LEFT = 2001, RIGHT = 2002, UP = 2003, LIVE = 2004, DAMAGE = 2005, DEAD = 2006, LOGIN = 2007, PLAY = 2008, VILLAGE = 2009, PIRATE = 2010, TEST = 2011, REVIVE = 2012 };

typedef struct Ballon {
	int x;
	int y;
	int state;
	int beforeboomcouont;
	int startboomcount;
}Ballon;

typedef struct BOARD
{
	int x, y;
	int state;
	int block;
};

typedef struct CHARACTER
{
	int x, y;			// x, y ��ǥ
	int speed;			// ĳ������ �̵��ӵ�
	int ballon_length;	// ��ǳ���� ����
	int ballon_num;		// ��ǳ���� ����
	int direction;		// ĳ������ ����
	int state;			// ĳ������ ����
	bool stop;			// ĳ���� ���� ����
	int count;
	int player;
	Ballon ballon[6];
} CHARACTER;

CHARACTER Player1, Player2;
BOARD Board[13][15];
int ObstacleBreak[MapYMax][MapXMax];

void PlayerMove(CHARACTER* Player, bool isDownKeyPressed, bool isLeftKeyPressed, bool isRightKeyPressed, bool isUpKeyPressed);
void PlayerMeetObstacle(CHARACTER* Player);
void PlayerGetItem(CHARACTER* Player);
void PlayerMeetBallon(CHARACTER* Player);
void BallonBoom(CHARACTER* Player, int num);
void BallonToObstacle(CHARACTER* Player);
void PlayerMeetPlayer(CHARACTER* Player1, CHARACTER* Player2);
void InitializePlayer(CHARACTER* player, int playerNumber) {
    player->speed = 3;
    player->ballon_num = 1;
    player->ballon_length = 5;
    player->stop = true;
    player->direction = DOWN;
    player->state = LIVE;
    player->player = playerNumber;

    // ��ǳ�� �� �Ѿ� �ʱ�ȭ
    for (int i = 0; i < MAX_BALLONS; ++i) {
        player->ballon[i].state = 0;
    }
}


