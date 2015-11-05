#pragma once
#include "GameObject.h"

#define BUFF_I 1
#define MAGIC_I 2
#define ETERNITY_I 3
#define ATTACK_I 4

class Enemy :
	public GameObject
{
public:
	Enemy(int &unitcount , int x = 0, int y = 0,int type = 0);
	~Enemy();
	virtual void Interaction(GameObject *target);
	virtual void Born()
	{
			finishb = true;
	};
	bool MPUse(int point)
	{
		if (mp >= point)
		{
			mp -= point;
			return true;
		}
		else
			return false;

	}
	virtual int DMGDispose(int vary)
	{
		if (!absorbtimer)
		{
			switch (GetInnerCode())
			{
			case ENEMY_SHIELD:
				if (flag)
					return 0;
				else
				{
					if (GetShield() > 0)
					{
						VaryShield(-1);
						return 0;
					}
					else
						return GameObject::DMGDispose(vary);
				}
				break;
			case ENEMY_ELITE:
				if (GetAim()->GetInnerCode() == CORE_SHIELD)
				{
					HGE *hge = hgeCreate(HGE_VERSION);
					if (hge->Random_Int(1, 5) == 1)
						return 0;
					else
					{
						if (GetShield() > 0)
						{
							VaryShield(-1);
							return 0;
						}
						else
							return GameObject::DMGDispose(vary);
					}
				}
				else
				{
					if (GetSize() != 1)
						return 0;
					if (GetShield() > 0)
					{
						VaryShield(-1);
						return 0;
					}
					else
						return GameObject::DMGDispose(vary);
				}
				break;
			default:
				if (GetShield() > 0)
				{
					VaryShield(-1);
					return 0;
				}
				else
					return GameObject::DMGDispose(vary);
				break;
			}
		}
		else
			return -vary;
	}
	virtual int GetDMG(bool forattack = false)const
	{
		if (forattack)
			return -GameObject::GetDMG()*(atkuptimer ? 2 : 1);
		else
			return GameObject::GetDMG()*(atkuptimer ? 2 : 1);
	};
	virtual bool IsReachable()
	{
		if (!GetAim() || !EnemyEntry())
			return false;
		else
			return true;
	}
	void ResetGotoPos()
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		gotox = GetActStartX() + (GetActEndX() - GetActStartX())*hge->Random_Float(0.3, 0.9);
		gotoy = hge->Random_Float(GetActStartY() + 20, GetActEndY() - 20);
	}
	virtual bool VaryHP(int vary);
	virtual bool VaryHP_Obj(GameObject *obj);
	virtual bool IsEnemy(){ return true; };
	virtual void StartSkill(){ NoChangeRot = true; };
	virtual void EndSkill()
	{
		ResetGotoPos();
		NoChangeRot = false; 
	};
	virtual bool NeedRenderFont(){ return true; };
	virtual void Action();
	virtual void Death();
	inline void RestrictGotoPos()
	{
		if (gotox < GetActStartX())
			gotox = GetActStartX();
		if (gotox > GetActEndX())
			gotox = GetActEndX();
		if (gotoy < GetActStartY())
			gotoy = GetActStartY();
		if (gotoy > GetActEndY())
			gotoy = GetActEndY();
	}
	inline void RestrictGotoPos(int &x,int &y)
	{
		if (x < GetActStartX())
			x = GetActStartX();
		if (x > GetActEndX())
			x = GetActEndX();
		if (y < GetActStartY())
			y = GetActStartY();
		if (y > GetActEndY())
			y = GetActEndY();
	}

	void SpawnEnemy(int innercode);
	void SetAtkUp(){ atkuptimer = 500; }
	void SetAbsorb(){ absorbtimer = 200; }
	virtual int GetShootCD()const
	{
		return exps;
	};
private:
	int waittime;
	int exps;
	int mp;
	float gotox, gotoy;
	bool NoChangeRot;
	int eliteshootcd;
	bool flag;
	int elitetype;
	int skillcost[8];
	int atkuptimer;
	int absorbtimer;
	int customdata;
	bool custombool;
};

