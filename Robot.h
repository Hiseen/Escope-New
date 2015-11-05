#pragma once
#include "GameObject.h"

enum CoreType
{
	Buff,Magic,Attack,Eternity
};


class Robot :
	public GameObject
{
public:
	//collisonrange -150是有道理的：为了不让边界判断生效
	Robot(GameObject* host,int Robotnum, ObjectType Type) :GameObject(0, 0, 1, 5, 5,-150, Type, 0, Robotnum)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		startattack = false;
		buffstart = false;
		tremble = 0;
		this->host = host;
		SetPos(host->GetX(), host->GetY());
		shootdelay = 100;
		godtimer = 0;
		godmode = false;
		cannotskill = false;
		trigger = false;
		SetHP(10);
		SetDMG(20);
		switch (Robotnum)
		{
		case CORE_LAZER:
			SetDMG(5);
		case CORE_NORMAL:
		case CORE_SHOTGUN:
		case CORE_MISSILE:
			coretype = Attack;
			break;
		case CORE_GETMP:
			shootdelay = 0;
			coretype = Buff;
			break;
		case CORE_ATK:
		case CORE_HARDSKIN:
		case CORE_HEALHP:
		case CORE_HEALMP:
		case CORE_GETBLOOD:
		case CORE_SPEED:
		case CORE_SHOOTCD:
		case CORE_SHIELD:
		case CORE_UPDATE:
			shootdelay = 1500;
			coretype = Buff;
			break;
		case CORE_ICE:
		case CORE_BOOM:
		case CORE_LIGHTARROW:
		case CORE_BOUNCEARROW:
		case CORE_CLEAN:
		case CORE_TRACKBOMB:
		case CORE_FLASH:
		case CORE_HEAL:
		case CORE_GETSHIELD:
			coretype = Magic;
			break;
		case CORE_ADDMAXHP:
		case CORE_ADDMPHEAL:
		case CORE_ADDDMG:
		case CORE_ADDSPEED:
			coretype = Eternity;
			break;
		}
		std_shootdelay = shootdelay;
		direction = hge->Random_Int(0,1);
		runback = false;
	};
	~Robot();
	bool IsActive()const{ return startattack; };
	virtual bool IsCORE(){ return true; };
	virtual void Interaction(GameObject *target);
	void UseSkill();
	bool ForceBack()
	{
		if (coretype == Attack)
		{
			if (startattack)
			{
				runback = true;
				startattack = false;
				SetSpeed(1000);
				SetCollisionRange(-150);
			}
			return !trigger;
		}
		return true;
	}
	bool IsAttackMode()const{ return startattack; };
	virtual void Born()
	{
		finishb = true;
	};
	virtual void Death();
	void SetHost(GameObject *a)
	{
		if (!host)
		host = a;
	}
	CoreType GetCoreType()const{ return coretype; };
	virtual int DMGDispose(int vary);
	virtual bool IsReachable(){ return (coretype==Attack && startattack); }
	virtual void Action();
	virtual void AfterOneEffect(){};
	virtual bool IsSkill(){ return false; };
	virtual bool VaryHP(int vary);
	virtual bool VaryHP_Obj(GameObject* obj);
	virtual bool NeedRenderFont(){ return true; };
	virtual void StartSkill(){ cannotskill = true; };
	virtual void EndSkill(){ cannotskill = false; };
	virtual int GetShootCD()const
	{
		return std_shootdelay; 
	};
	virtual void SetShootCD(int cd)
	{
		std_shootdelay = cd;
	};
	virtual void SafeOperation()
	{
		if (host && !host->avlid)DeleteHost();
		GameObject::SafeOperation();
	};
	void DeleteHost()
	{
		host = NULL;
	}
private:
	bool cannotskill;
	int shootdelay; 
	int std_shootdelay;
	bool direction;
	bool IsTotem;
	bool godmode;
	int godtimer;
	bool runback;
	bool trigger;
	bool startattack;
	int  tremble;
	GameObject *host;
	float backx, backy;
	CoreType coretype;
	bool buffstart;
};

