#pragma once
#include "Robot.h"

#define STD_SHOOTCD 15
#define STD_AD 5
#define STD_HP 15
#define STD_SPEED 2
#define MAX_SPEED 5
#define STD_CORES 5
#define STD_COLLISIONRANGE 15
#define POWER_TO_CRYSTAL 50
#define MAX_CRYSTAL 6

class Hero :
	public GameObject
{
public:
	enum HeroClass{
		//爆炸子弹 
		Bomb,
		//破甲，造成额外伤害
		ArmorBreak,
		//毒性
		Toxic,
		//沉默
		EMP,
		//减少自身受到的伤害
		Weak,
		//反弹子弹
		Reflect
	};
	Hero(ObjectType type, bool IsAI = false) :GameObject(ACT_START_X + (ACT_END_X - ACT_START_X) / 2, ACT_START_Y + (ACT_END_Y - ACT_START_Y) / 2, 1, STD_AD, STD_HP, STD_COLLISIONRANGE, type, STD_SPEED, HERO)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		//for test
		MP = 5;//1
		heroskill = BOUNCEARROW_LIGHTARROW;//0
		skilltimes = 5;//0
		skilldelay = 0;
		extraDMG = 1;
		IsGod = false;
		bulletmode =(HeroClass)hge->Random_Int(0,4);
		healhpstart = false;
		healmpstart = false;
		getshieldstart = false;
		healhp = 0;
		healmp = 0;
		coins = 0;
		getshield = 0;
		lukpoint = 5;
		changetargettime = hge->Random_Int(200,400);
		speedrate = 1.0f;
		recoverpower = 0;
		absorb = false;
		friction = 0.9;
		uzimode = false;
		hardskin = false;
		LV = 0;
		KillsHeal = false;
		killshealcount = 0;
		Exp = 0;
		MaxExp = 150;
		suckblood = false;
		powerdelay = hge->Random_Int(50,150);
		GodTimer = 0;
		usingpower = false;
		dy = 0;
		dx = 0;
		minddelay = 0;
		power = 0;
		AI = IsAI;
		shootdelay = 0;
		choosemove = 0;
		shootcd = STD_SHOOTCD;
		cannotskill = false;
		feelweak = false;
		SetBlock(true);
		for (int i = 0; i < STD_CORES; i++)
			COREs[i] = nullptr;
		switch (type)
		{
		case Left:
			SetImgAngle(M_PI / 2);
			break;
		case Right:
			SetImgAngle(-M_PI / 2);
			break;
		}
		SetRot(GetImgAngle());
		finalangle = GetRot();
		startangle = finalangle;
	};
	virtual bool VaryHP(int vary);
	virtual bool VaryHP_Obj(GameObject *obj);
	virtual float GetRot()const{ return finalangle; };
	virtual bool SetHP(int newHP)
	{
		GameObject::SetHP(newHP);
		return true;
	}
	int GetGodTimer()
	{
		return GodTimer;
	}
	void InitBlocks();
	void InitSkill();
	//得到的是ICON
	int GetSkill()const{ return heroskill+300; };
	int GetSkillTimes()const{ return skilltimes; };
	int GetCoins()const{ return coins; };
	void SetCoins(int newcoin)
	{
		coins = newcoin;
	}
	virtual void Interaction(GameObject *target);
	virtual void Born()
	{ 
		finishb = true;
	};
	virtual void Action();
	virtual float GetDy(){ return dy; };
	virtual int DMGDispose(int DMG);
	virtual void Death(){ avlid = false; };
	virtual void Interaction2(GameObject *skill);
	virtual int GetDMG(bool forattack = false)const
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		if (!forattack)
			return GameObject::GetDMG()*2;
		else
		{
			int dmg = GameObject::GetDMG();
			return -((dmg+hge->Random_Int(0,4))*(hge->Random_Int(0, 100) < lukpoint ? 2 : 1)*(weaktimer ? 0.5 : 1)*extraDMG);
		}
	};
	virtual bool NeedRenderFont()
	{
		return true; 
	};
	void MyTurnInit();
	bool ForceAllCoreBack()
	{
		for (int i = 0; i < STD_CORES;i++)
		if (COREs[i])
		if (!dynamic_cast<Robot*>(COREs[i])->ForceBack())
			return false;
		return true;

	};
	virtual void StartSkill(){ cannotskill = true; };
	virtual void EndSkill(){ cannotskill = false; };
	int GetShootDelay()const{ return shootdelay; };
	void SetIceMode(bool set)
	{
		if (set)
			friction = 0.99;
		else
			friction = 0.9;
	}
	void PowerSystem();
	void BulletShoot();
	virtual void Stop();
	void SetSkill(int skillname)
	{
		if (skillname == heroskill)
			skilltimes = 5;
		else
		{
			if (!SkillComb(skillname, ICE, TRACKBOMB, ICE_TRACKBOMB))
			if (!SkillComb(skillname, HEAL, TRACKBOMB, HEAL_TRACKBOMB))
			if (!SkillComb(skillname, BOUNCEARROW, ICE, BOUNCEARROW_ICE))
			if (!SkillComb(skillname, BOUNCEARROW, LIGHTARROW, BOUNCEARROW_LIGHTARROW))
			if (!SkillComb(skillname, ICE, RUNEBOMB, ICE_RUNEBOMB))
			if (!SkillComb(skillname, ICE, SELFBOOM, ICE_BOOM))
			{
				heroskill = skillname;
				skilltimes = 5;
			}
		}
	}
	bool SkillComb(int skill1, const int name1, const int name2,const int name3)
	{
		if ((skill1 == name1 && heroskill == name2) || (skill1 == name2&& heroskill == name1))
		{
			heroskill = name3;
			return true;
		}
		return false;
	}
	void GetCrystal(int times)
	{
		MP += times;
		if (MP > MAX_CRYSTAL)
			MP = MAX_CRYSTAL;
	};
	int GetCrystals()const{ return MP; };
	virtual void SetShootCD(int cd)
	{
		if (cd < power)
			recoverpower = 0;
		power = cd;
	};
	virtual int GetShootCD()const{ return power; };
	virtual void SafeOperation()
	{
		for (int i = 0; i < STD_CORES; i++)
		{
			if (COREs[i] && !COREs[i]->avlid)
				COREs[i] = NULL;
		}
		GameObject::SafeOperation();
	};
	virtual bool IsReachable(){ return true; }
	GameObject* GetCOREs(int num)
	{
		if (num <= STD_CORES)
			return COREs[num];
		else
			return NULL;
	}
	virtual float GetSpeed()const{ return GameObject::GetSpeed()*GetSpeedRate(); };
	~Hero();
private:
	void LevelSystem();
	float dy,dx;
	int shootdelay;
	GameObject *COREs[STD_CORES];
	int skilldelay;
	bool AI;
	int minddelay;
	int choosemove;
	bool cannotskill;
	bool feelweak;
	bool Scilenced;
	int shootcd;
	bool IsGod;
	int GodTimer;
	bool absorb;
	bool suckblood;
	HeroClass bulletmode;
	float power;
	float speedrate;
	int LV;
	int Exp;
	int MaxExp;
	int extraDMG;
	int powerdelay;
	bool usingpower;
	int changetargettime;
	float recoverpower;
	int lukpoint;
	bool uzimode;
	float friction;
	int heroskill;
	int skilltimes;
	bool hardskin;
	bool healhpstart;
	bool healmpstart;
	bool getshieldstart;
	int healhp;
	int healmp;
	int getshield;
	float finalangle;
	float startangle;
	int coins;
	int MP;
	bool KillsHeal;
	int killshealcount;
};

