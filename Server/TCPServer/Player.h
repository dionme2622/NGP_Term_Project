#pragma once
#include "stdafx.h"
class CPlayer
{
public:
	int x, y;
	float fx, fy;

	int			speed;			// ĳ������ �̵��ӵ�
	int			ballon_length;	// ��ǳ���� ����
	int			ballon_num;		// ��ǳ���� ����
	int			direction;		// ĳ������ ����
	int			state;			// ĳ������ ����
	bool		stop;			// ĳ���� ���� ����
	int			count;			// ���� ��
//	CBallon* ballon[6];		// ��ǳ��

public:
	CPlayer();
	~CPlayer();
	
	void SetDirection(int);			// player�� ���⺤�͸� �����ϴ� �Լ�
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

