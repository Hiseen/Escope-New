#pragma once
#include"defines.h"
class GameObject
{
public:
	GameObject(float x,float y,float size,float dmg,int HP,int collisionrange,ObjectType type,float speed,int innercode);
	float GetX()const{ return Myx; };
	float GetY()const{ return Myy; };
	float GetSize()const{ return Mysize; };
	void SetType(ObjectType type)
	{
		Mytype = type;
	}
	virtual int GetDMG(bool forattack=false)const
	{
		if (forattack)
			return -MyDMG*(weaktimer ? 0.5 : 1);
		else
			return MyDMG*(weaktimer ? 0.5 : 1);
	};
	virtual void AfterOneEffect(){};
	virtual float GetSpeed()const{ return MySpeed; };
	virtual void Scilence(){ Scilenced = true; };
	virtual void UnScilence(){ Scilenced = false; };
	virtual bool GetScilenced(){ return Scilenced; };
	virtual int GetShootCD()const{ return 0; };
	virtual void SetShootCD(int cd){};
	virtual float GetDy(){ return 0; };
	virtual bool IsEffectiveForOtherSkill()const{ return false; };
	virtual bool VaryHP(int vary)
	{
		MyHP += vary;
		if (MyHP > MyMaxHP)
			MyHP = MyMaxHP;
		return true;
	}
	virtual bool VaryHP_Obj(GameObject *obj)
	{
		int vary = obj->GetDMG(true);
		MyHP += vary;
		return true;
	}
	virtual int GetMaxHP()const{ return MyMaxHP; };
	virtual void SetMaxHP(int newHP)
	{
		MyMaxHP = newHP;
	}
	void SetSpeed(float Speed)
	{
		MySpeed = Speed;
	}
	void SetSpeedRate(float SpeedRate)
	{
		MySpeedRate = SpeedRate;
	}
	float GetSpeedRate()const
	{
		return MySpeedRate;
	}
	void  SetDMG(int newdmg)
	{
		MyDMG = newdmg;
	}
	virtual void SetX(float x)
	{
		Myx = x;
	}
	void SetY(float y)
	{
		Myy = y;
	}
	void SetSize(float newsize)
	{
		Mysize = newsize;
	}
	virtual bool SetHP(int newHP)
	{
		if (finishb)
		{
			if (newHP < MyMaxHP)
				MyHP = newHP;
			else
				MyHP = MyMaxHP;
		}
		else
		{
			MyHP = newHP;
			MyMaxHP = newHP;
		}
		return true;
	}
	virtual bool IsCORE(){ return false; };
	virtual void Interaction(GameObject *target);
	virtual void Interaction2(GameObject *skill){};
	virtual int DMGDispose(int DMG){ return DMG*(armorbreaktimer ? AB : 1); };
	virtual bool GoToAnotherObject(GameObject *obj);
	virtual bool GoToAnotherPos(float x, float y);
     bool GoToAim();
	 void SetPos(GameObject *game)
	 {
		 Myx = game->Myx;
		 Myy = game->Myy;
	 }
	virtual void Born(){ finishb = true; };
	virtual bool IsSkill(){ return false; };
	virtual void Action();
	virtual void Death(){ avlid = false; };
	virtual bool IsDead()
	{
		if (MyHP <= 0)
			return true;
		else
			return false;
	};
	virtual bool Suicide(bool istrue=false)
	{
		if (istrue)
			TrueSuicide = true;
		MyHP = 0;
		return true;
	}
	bool IsTrueSuicide()const{ return TrueSuicide; };
	void GoWithAnAngle(float angle);
	virtual SceneNumber GetSceneNumber(){ return scn; };
    int GetInnerCode()const{ return Myinnercode; };
	void SetInnerCode(int inner){ Myinnercode = inner; };
	virtual bool IsReachable(){ return true; };
	virtual bool IsEnemy(){ return false; };
	int GetHP()const{ return MyHP; };
	inline int GetActStartX()const { return ACT_START_X + Mycollisionrange; };
	inline int GetActStartY()const { return ACT_START_Y + Mycollisionrange; };
	inline int GetActEndX()const { return ACT_END_X - Mycollisionrange; };
	inline int GetActEndY()const { return ACT_END_Y - Mycollisionrange; };
    int GetCollisionRange(){ return Mycollisionrange*Mysize; };
	bool GetIsLineSkill()const{ return islineskill; };
	void SetCollisionRange(int newcol, bool setisline = false)
	{
		islineskill = setisline;
		Mycollisionrange = newcol; 
	};
	float GetImgAngle()const{ return Imgangle; };
	void SetImgAngle(float img)
	{
		Imgangle = img;
	}
	virtual float GetRot()const{ return angle; };
	void SetRot(float angle2)
	{
		angle = angle2;
	}
	double GetDistance(GameObject *obj)
	{
		return sqrtf((Myx - obj->Myx)*(Myx - obj->Myx) + (Myy - obj->Myy)*(Myy - obj->Myy));
	};
	double GetDistance(float x,float y)
	{
		return sqrtf((Myx - x)*(Myx - x) + (Myy - y)*(Myy - y));
	};
	double GetDistance(float x1, float y1,float x2,float y2)
	{
		return sqrtf((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	};
	virtual void SetPos(float newposx, float newposy)
	{
		Myx = newposx;
		Myy = newposy;
		if (Myx < GetActStartX())
			Myx = GetActStartX();
		if (Myx > GetActEndX())
			Myx = GetActEndX();
		if (Myy < GetActStartY())
			Myy = GetActStartY();
		if (Myy > GetActEndY())
			Myy = GetActEndY();
	};
	bool IsOutOfBound()
	{
		if (Myx <= GetActStartX())
		    return true;
		if (Myx >= GetActEndX())
			return true;
		if (Myy <= GetActStartY())
			return true;
		if (Myy >= GetActEndY())
			return true;
		return false;
	}

	bool EnemyEntry()
	{
		if (!AddSpeed)
		{
			if (GetX()<ACT_START_X || GetX()>ACT_END_X || GetY()<ACT_START_Y || GetY()>ACT_END_Y)
			{
				HGE *hge = hgeCreate(HGE_VERSION);
				AddSpeed = hge->Random_Int(500, 600);
				MySpeed += AddSpeed;
				return false;
			}
			else
				return true;
		}
		else
		{
			GoToAnotherPos(ACT_CENTER_X, ACT_CENTER_Y);
			if (GetX()>ACT_START_X&& GetX()<ACT_END_X && GetY()>ACT_START_Y && GetY()<ACT_END_Y)
			if (AddSpeed > 10)
			{
				HGE *hge = hgeCreate(HGE_VERSION);
				float Myspeed2 = AddSpeed;
				AddSpeed *= hge->Random_Float(0.8, 0.9);
				MySpeed -= Myspeed2 - AddSpeed;
			}
			else
			{
				AddSpeed = 0;
			}
			return false;
		}

	}

	void TurnToAim(int data=100);
	double GetAngle(GameObject *obj, bool return1=false);
	double GetAngle(float x,float y, bool return1=false);
	int GetStopTime()
	{
		return StopTime;
	}
	virtual void SetStopTime(int Stoptime)
	{
		StopTime = Stoptime;
	}
	virtual void Stop()
	{
		if (StopTime > 0)
			StopTime--;
		else
			StopTime = 0;
	}
	virtual bool IsItem(){ return false; };
	virtual bool NeedRenderFont(){ return false; };
	bool IsSoild()const{ return issolid; };
	void VaryShield(int vary){ Shield += vary; };
	int GetShield(){ return Shield; };
	void BoomEffect(GameObject *speedobj);
	void SetBlock(bool block)
	{
		MyIsBlock = block;
	}
	bool IsBlock()const
	{
		return MyIsBlock;
	}
	void SetSolid(bool solid)
	{
		issolid = solid;
	}
	virtual void StartSkill(){};
	virtual void EndSkill(){};
	virtual void SafeOperation()
	{
		if (aimobject && !aimobject->avlid)
			aimobject = NULL;
	};
	float GetAimx()const{ return myaimx; };
	float GetAimy()const{ return myaimy; };
	void AddDxDy(float newdx, float newdy)
	{
		dx += newdx;
		dy += newdy;
	}
	void SetAim(float x, float y)
	{
		myaimx = x;
		myaimy = y;
		if (myaimx < GetActStartX())
			myaimx = GetActStartX();
		if (myaimx > GetActEndX())
		    myaimx = GetActEndX();
		if (myaimy < GetActStartY())
			myaimy = GetActStartY();
		if (myaimy > GetActEndY())
			myaimy = GetActEndY();
	};
	void SetAim(GameObject *aim)
	{
		aimobject = aim;
		if (aim)
		{
			myaimx = aim->Myx;
			myaimy = aim->Myy;
		}
	}
	GameObject *GetAim()const
	{
		return aimobject;
	}
	ObjectType GetType()const{ return Mytype; };
	bool avlid;
	bool finishb;
	int armorbreaktimer;
	int weaktimer;
	int toxictimer;
	int ToxicDMG;
	float AB;
	~GameObject();
private:
	int MyHP;
	float MySpeed;
	float MySpeedRate;
	SceneNumber scn;
	ObjectType Mytype;
	int Mycollisionrange;
	float Myx, Myy;
	float Mysize;
	float angle;
	float Imgangle;
	int MyDMG;
	int Myinnercode;
	int MyMaxHP;
	bool islineskill;
	int Shield;
	bool Scilenced;
	float myaimx, myaimy;
	float dx, dy;
	GameObject *aimobject;
	bool TrueSuicide;
	float AddSpeed;
	bool issolid;
	int StopTime;
	bool MyIsBlock;
};

