#pragma once
#include "GameObject.h"


class CPlayer
{
public:
	CPlayer(HINSTANCE _hInst);
	~CPlayer();

	void Update(float fTimeElapsed);
	void Render(HDC MemDC, HDC MemDCImage);
private:
	int			x, y;			// ĳ���� x, y ��ǥ
	float		fx, fy;
	int			speed;			// ĳ������ �̵��ӵ�
	int			ballon_length;	// ��ǳ���� ����
	int			ballon_num;		// ��ǳ���� ����
	int			direction;		// ĳ������ ����
	int			state;			// ĳ������ ����
	bool		stop;			// ĳ���� ���� ����
	int			count;			// ���� ��
	CBallon		ballon[6];		// ��ǳ��

	int			xPos, yPos;		// �̹��� x, y ��ǥ
	float		xPosF, yPosF;
private:
	HBITMAP MainBitmap[8];
public:
	void SetDirection(int);			// Player�� ���⺤�͸� �����ϴ� �Լ�
	void SetPosition(float _fx, float _fy);

	void SetStop(bool _stop) { stop = _stop; }
	bool GetStop() { return stop; }

	void Move(float fTimeElapsed);
	//�̰� ����
	/*int count;
	int player;
	Ballon ballon[6];
	Bullet bullet[3];*/
};

