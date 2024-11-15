#pragma once
#include "stdafx.h"


class CGameObject
{
public:
	CGameObject();
	~CGameObject();

	virtual void Update() = 0;
	virtual void Render() = 0;


public:
	int x, y;		// 좌표

	// 이미지 담을 무언가의 변수

};

class CBallon : public CGameObject
{
public:
	CBallon();
	~CBallon();

	virtual void Update();
	virtual void Render();

private:
	int			state;   //			0가지고 있는거      1 처음 설치할떄			 2 펑!
	int			beforeboomcouont;
	int			startboomcount;

	RECT		boundingBox;

};

class CCharacter : public CGameObject
{
public:
	CCharacter() = default;
	~CCharacter() = default;

private:
	int			speed;			// 캐릭터의 이동속도
	int			ballon_length;	// 물풍선의 길이
	int			ballon_num;		// 물풍선의 개수
	int			direction;		// 캐릭터의 방향
	int			state;			// 캐릭터의 상태
	bool		stop;			// 캐릭터 정지 유무
	int			count;			// 뭔지 모름
	CBallon		ballon[6];		// 물풍선

	RECT		boundingBox;
};


class CBoard : public CGameObject
{
public:
	CBoard();
	~CBoard();

	virtual void Update();
	virtual void Render();

	void SetPosition(int _x, int _y) { x, y = _x, _y; }
	void SetState(int _state) { state = _state; }
private:
	int state;				// 1이면 일반타일, 2이면 블록, 3이면 장애물, 6: 신발아이템, 7: 물줄기아이템, 8: 물풍선아이템
	int block;				// 뭔지 모름

	RECT		boundingBox;
};





class CItem : public CGameObject
{
public:
	CItem();
	~CItem();

	virtual void Update();
	virtual void Render();

public:
	RECT		boundingBox;

};