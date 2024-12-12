#pragma once
#include "GameObject.h"


struct SC_PlayersInfoPacket;
struct POS {
	float fx, fy;
	int x, y;
};
struct BITMAP_POS
{
	POS live;
	POS damage;
	POS dead;
	POS escape;
};
class CMap;

class CPlayer
{
public:
	CPlayer() {};
	CPlayer(HINSTANCE _hInst, SC_PlayersInfoPacket receivedPacket, int playerID);
	~CPlayer();

	void Update(SC_PlayersInfoPacket receivedPacket, float fTimeElapsed);
	void Render(HDC MemDC, HDC MemDCImage, CMap* Map);

public :
	BITMAP_POS  bitmap_pos;
	int			x, y;			// ĳ���� x, y ��ǥ
	float		fx, fy;
public:
	int			speed;			// ĳ������ �̵��ӵ�
	int			ballon_length;	// ��ǳ���� ����
	int			ballon_num;		// ��ǳ���� ����
	int			direction;		// ĳ������ ����
	int			state;			// ĳ������ ����
	bool		stop;			// ĳ���� ���� ����
	CBallon*	ballon[6];		// ��ǳ��
	bool		needle;			// �ٴ� ���� ����

	int			ID;
	int			xPos, yPos;		// �̹��� x, y ��ǥ
	float		xPosF, yPosF;
private:
	HBITMAP MainBitmap[9];
public:
	void SetDirection(int);			// playerData�� ���⺤�͸� �����ϴ� �Լ�
	void SetPosition(int _x, int _y) { x = _x, y = _y, fx = (float)x, fy = (float)y; }
	void SetPosition(float _fx, float _fy);
	void SetState(int _state) { state = _state; }
	void SetStop(bool _stop) { stop = _stop; }

	bool GetStop() { return stop; }
	int GetState() { return state; }
	int GetBallonNum() { return ballon_num; }
	int GetBallonLength() { return ballon_length; }

	
};

