#pragma once
#include "GameObject.h"
class Item :
	public GameObject
{
public:
	Item(int innercode);
	~Item();
	virtual void Born()
	{
		if (GetSize() < 1)
			SetSize(GetSize() + 0.1);
		else
		    finishb = true; 
	};
	virtual bool IsSkill(){ return false; };
	virtual void Action();
	virtual void Death();
	virtual bool IsReachable(){ return false; }
private:
	int price;
	int flag;
	int itemcode;
};

