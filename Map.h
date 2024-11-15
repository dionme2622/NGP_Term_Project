#pragma once
#include "stdafx.h"
#include "GameObject.h"

class CMap
{
public:
	CMap() {};
	~CMap() {};

	virtual void Initialize();
protected:
	CBoard			Board[13][15];
};

class CVillage : public CMap
{
public :
	CVillage() {};
	~CVillage() {};

	virtual void Initialize();
private:


};

class CPirate : public CMap
{
public:
	CPirate() {};
	~CPirate() {};

	virtual void Initialize();
private:


};
