#pragma once
#include "stdafx.h"

class CBallon 
{
public:
	CBallon() {
		state = 0;
		beforeboomcount = 0.0f;
		startboomcount = 0.0f;
	};
	~CBallon() {};

public:
	int			x, y;
	int			state;   //			0가지고 있는거      1 처음 설치할떄			 2 펑!
	float		beforeboomcount;
	float		startboomcount;

public:
	int GetState() { return state; }
	void SetState(int _state) { state = _state; }
};

class CPlayer
{
public:
	int x, y;
	float fx, fy;

	int			speed;			// 캐릭터의 이동속도
	int			temp_speed;		// 캐릭터 속도
	int			ballon_length;	// 물풍선의 길이
	int			ballon_num;		// 물풍선의 개수
	int			direction;		// 캐릭터의 방향
	int			state;			// 캐릭터의 상태
	bool		stop;			// 캐릭터 정지 유무
	float		count;	
	CBallon* ballon[6];		// 물풍선

public:
	CPlayer();
	~CPlayer();
	
	void Update(float fTimeElapsed);

	void SetDirection(int dir) { direction = dir; }			// player의 방향벡터를 설정하는 함수
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

	void Move(float fTimeElapsed);
};

