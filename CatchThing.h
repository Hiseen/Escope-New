#pragma once
#include "GameObject.h"
class CatchThing :
	public GameObject
{
public:
	CatchThing(GameObject *host,int innercode);
	virtual void Born()
	{
		finishb = true;
	};
	virtual void Action();
	virtual void Death();
	virtual void SetStopTime(int Stoptime){};
	~CatchThing();
private:
	int lasttimer;
	bool lockon;
};

