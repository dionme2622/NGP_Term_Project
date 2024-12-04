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
	int			x, y;			// 캐릭터 x, y 좌표
	float		fx, fy;
public:
	int			speed;			// 캐릭터의 이동속도
	int			ballon_length;	// 물풍선의 길이
	int			ballon_num;		// 물풍선의 개수
	int			direction;		// 캐릭터의 방향
	int			state;			// 캐릭터의 상태
	bool		stop;			// 캐릭터 정지 유무
	CBallon*	ballon[6];		// 물풍선

	int			ID;
	int			xPos, yPos;		// 이미지 x, y 좌표
	float		xPosF, yPosF;
private:
	HBITMAP MainBitmap[9];
public:
	void SetDirection(int);			// playerData의 방향벡터를 설정하는 함수
	void SetPosition(int _x, int _y) { x = _x, y = _y, fx = (float)x, fy = (float)y; }
	void SetPosition(float _fx, float _fy);
	void SetState(int _state) { state = _state; }
	void SetStop(bool _stop) { stop = _stop; }
	void SetBallon(CMap* Map);

	bool GetStop() { return stop; }
	int GetState() { return state; }
	int GetBallonNum() { return ballon_num; }
	int GetBallonLength() { return ballon_length; }
	//CBallon* GetBallon(int i) { return ballon[i]; }

	void Move(float fTimeElapsed);
	
};

