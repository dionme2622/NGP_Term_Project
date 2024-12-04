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
	int			state;   //			0������ �ִ°�      1 ó�� ��ġ�ҋ�			 2 ��!
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

	int			speed;			// ĳ������ �̵��ӵ�
	int			temp_speed;		// ĳ���� �ӵ�
	int			ballon_length;	// ��ǳ���� ����
	int			ballon_num;		// ��ǳ���� ����
	int			direction;		// ĳ������ ����
	int			state;			// ĳ������ ����
	bool		stop;			// ĳ���� ���� ����
	float		count;	
	CBallon* ballon[6];		// ��ǳ��

public:
	CPlayer();
	~CPlayer();
	
	void Update(float fTimeElapsed);

	void SetDirection(int dir) { direction = dir; }			// player�� ���⺤�͸� �����ϴ� �Լ�
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

