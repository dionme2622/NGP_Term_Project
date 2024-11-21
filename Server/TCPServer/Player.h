#pragma once
#include "stdafx.h"
class CPlayer
{
public:
	int x, y;
	float fx, fy;

	int			speed;			// 캐릭터의 이동속도
	int			ballon_length;	// 물풍선의 길이
	int			ballon_num;		// 물풍선의 개수
	int			direction;		// 캐릭터의 방향
	int			state;			// 캐릭터의 상태
	bool		stop;			// 캐릭터 정지 유무
	int			count;			// 뭔지 모름
//	CBallon* ballon[6];		// 물풍선

public:
	CPlayer();
	~CPlayer();
	
	void SetDirection(int);			// player의 방향벡터를 설정하는 함수
	void SetPosition(int _x, int _y) { x = _x, y = _y, fx = (float)x, fy = (float)y; }
	void SetPosition(float _fx, float _fy);
	void SetState(int _state) { state = _state; }
	void SetStop(bool _stop) { stop = _stop; }
	//void SetBallon(CMap* Map);

	bool GetStop() { return stop; }
	int GetState() { return state; }
	int GetBallonNum() { return ballon_num; }
	int GetBallonLength() { return ballon_length; }
	//CBallon* GetBallon(int i) { return ballon[i]; }

	void Move(int dir, float fTimeElapsed);
};

