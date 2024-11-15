#pragma once
class CMap
{
public:
	CMap() {};
	~CMap() {};

	virtual void Initialize();
private:

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