#pragma once
#include "NoticeManager.h"
#include"ObjectPool.h"
#include "Robot.h"
#include "Enemy.h"

#define ENEMYBULLET_MAXSPEED 500

#define BULLET_SPEEDUP 0
#define BULLET_SPEEDDOWN 1
#define BULLET_SPEEDSTAY 2
#define BULLET_TRACKING 3

#define THROWBULLET_CENTER 1

#define BOUNCEBULLET_SPEEDUP 1
#define BOUNCEBULLET_NOPOWERUP 1

class Skill :
	public GameObject
{
public:
	Skill(GameObject *host, int SkillType,int customdata1=0,int customdata2=0) :GameObject(0, 0, 1, 0, 1, 0, host->GetType(), 0, SkillType)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		Myhost = host;
		target = NULL;
		last_time = 0;
		data3 = 0;
		data = 0;
		CanEffectOtherSkill = false;
		switch (SkillType)
		{
		case BULLET:
			SetRot(host->GetRot());//+hge->Random_Float(-0.1,0.1));
			SetImgAngle(GetRot());
			SetPos(host->GetX(), host->GetY());
			SetHP(host->GetDMG());
			SetDMG(host->GetDMG());
			SetSpeed(800);
			SetCollisionRange(15);
			break;
		case ENEMYBULLET:
			data2 = customdata1;
			SetRot(host->GetRot());
			SetImgAngle(GetRot());
			SetPos(host->GetX(), host->GetY());
			SetHP(5);
			SetDMG(1);
			SetSpeed(200);
			SetCollisionRange(5);
			break;
		case ENEMYBOUNCEARROW:
			SetPos(host);
			SetCollisionRange(15);
			SetAim(ObjectPool::GetObjPool().GetEnemyHero(GetType()));
			if (customdata2 != BOUNCEBULLET_NOPOWERUP)
				host->StartSkill();
			else
			{
				last_time = 100;
				SetSpeed(400);
				SetRot(host->GetRot());
				SetImgAngle(GetRot());
			}
			data = 0;
			data2 = customdata1;
			SetDMG(2);
			break;
		case TOXICFIELD:
			last_time = hge->Random_Int(3,6)*50;
			SetDMG(1);
			SetCollisionRange(100);
			SetPos(hge->Random_Int(GetActStartX(), GetActEndX()), hge->Random_Int(GetActStartY(), GetActEndY()));
			break;
		case TBULLET:
		case EMPBULLET:
		case WBULLET:
		case BBULLET:
		case ABBULLET:
			if (customdata1 == BULLET_TRACKING)
			{
				SetAim(ObjectPool::GetObjPool().GetRandomEnemyUnit(GetType()));
				last_time = hge->Random_Int(600,1300);
				data3 = hge->Random_Int(0, 1);
			}
			data2 = customdata1;
			SetRot(host->GetRot());
			SetImgAngle(GetRot());
			SetPos(host->GetX(), host->GetY());
			SetHP(host->GetDMG()/2);
			SetDMG(host->GetDMG());
			SetSpeed(600);
			SetCollisionRange(10);
			break;
		case MISSILE:
			SetPos(host->GetX(), host->GetY());
			SetHP(1);
			SetDMG(1);
			SetRot(hge->Random_Float(0, 2 * M_PI));
			SetSpeed(hge->Random_Float(800, 840));
			SetCollisionRange(10);
			last_time = 100;
			SetAim(ObjectPool::GetObjPool().GetRandomEnemyUnit(GetType()));
			break;
		case MISSILE2:
			SetPos(host->GetX(), host->GetY());
			SetHP(1);
			SetDMG(host->GetDMG());
			SetSpeed(300);
			SetCollisionRange(10);
			last_time = 100;
			SetAim(host->GetX() + (hge->Random_Int(0, 1) ? hge->Random_Int(-100, -50) : hge->Random_Int(50, 100)), host->GetY()+(hge->Random_Int(0, 1) ? hge->Random_Int(-100, -50) : hge->Random_Int(50, 100)));
			target = ObjectPool::GetObjPool().GetRandomEnemyUnit(GetType());
			break;
		case DISPERSEBULLET:
			SetRot(host->GetRot());
			SetImgAngle(GetRot());
			SetPos(host->GetX(), host->GetY());
			SetHP(host->GetDMG()*3);
			SetDMG(2);
			SetSpeed(600);
			SetCollisionRange(10);
			last_time = hge->Random_Int(10,50);
		    data = 2;
			break;
		case THROWBULLET:
			SetPos(host);
			SetDMG(1);
			SetSpeed(600);
			SetCollisionRange(10); 
			SetRot(host->GetRot());
			if (!customdata1)
			{
				data = GetDistance(ObjectPool::GetObjPool().GetEnemyHero(GetType()));
				SetAim(GetX() + cos(GetRot() + hge->Random_Float(-0.5, 0.5) - 1.57)*hge->Random_Float(data - 50, data + 50),
					   GetY() + sin(GetRot() + hge->Random_Float(-0.5, 0.5) - 1.57)*hge->Random_Float(data - 50, data + 50));
				data3 = hge->Random_Float(0.5, 1.2);
			}
			else
			{
				GameObject *obj = ObjectPool::GetObjPool().GetEnemyHero(GetType());
				SetAim(obj->GetX(),obj->GetY());
				data3 = hge->Random_Float(1, 1.2);
			}
			data = GetDistance(GetAimx(), GetAimy());
			SetCollisionRange(-1);
			break;
		case AIMBULLET:
			SetDMG(1);
			SetSpeed(0);
			SetCollisionRange(10);
			SetAim(host);
			last_time = hge->Random_Int(30, 50);
			break;
		case CORESKILL:
			host->StartSkill();
			switch (host->GetInnerCode())
			{
			case CORE_NORMAL:
				last_time = 15;
				break;
			case CORE_LAZER:
				last_time = 20;
				break;
			}
			break;
		case COREBULLET:
			SetRot(host->GetRot());
			SetImgAngle(GetRot());
			SetPos(host->GetX(), host->GetY());
			SetHP(host->GetDMG());
			SetDMG(host->GetDMG());
			SetCollisionRange(5);
			switch (host->GetInnerCode())
			{
			case CORE_NORMAL:
				SetSpeed(600);
				break;
			case CORE_SHOTGUN:
				SetSpeed(400);
				break;
			case CORE_LAZER:
				SetRot(host->GetRot() + hge->Random_Float(-0.1,0.1));
				SetSpeed(2000);
				SetCollisionRange(2);
				break;
			case CORE_MISSILE:
				SetSpeed(500);
				last_time = 100;
				SetAim(host->GetX() + (hge->Random_Int(0, 1) ? hge->Random_Int(-100, -50) : hge->Random_Int(50, 100)), host->GetY() + (hge->Random_Int(0, 1) ? hge->Random_Int(-100, -50) : hge->Random_Int(50, 100)));
				target = ObjectPool::GetObjPool().GetRandomEnemyUnit(GetType());
				break;
			}
			break;
		case TRACKBULLET:
			CanEffectOtherSkill = true;
			SetImgAngle(host->GetRot());
			SetPos(host->GetX(), host->GetY());
			SetHP(host->GetDMG()*5);
			SetDMG(2);
			SetSpeed(1000);
			last_time = 600;
			SetCollisionRange(50);
			SetAim(ObjectPool::GetObjPool().GetEnemyHero(GetType()));
			break;
		case LAZER:
			SetRot(host->GetRot());
			SetImgAngle(host->GetRot());
			SetPos(host->GetX(), host->GetY());
			SetHP(host->GetDMG()*5);
			SetDMG(2);
			SetSpeed(0);
			SetCollisionRange(0,true);
			last_time = 50;
			data2 = customdata1;
			break;
		case TBOMB:
        case BOOM:
			data2 = customdata1;
			SetPos(host->GetX(), host->GetY());
			SetHP(1);
			SetDMG(host->GetDMG());
			SetCollisionRange(50+(GetDMG()-5));
			if (GetCollisionRange() > 150)
				SetCollisionRange(150);
			CanEffectOtherSkill = true;
			break;
		case TONS_OF_TBULLET:
		case TONS_OF_WBULLET:
		case TONS_OF_EMPBULLET:
		case TONS_OF_BBULLET:
		case TONS_OF_ABBULLET:
			last_time = 50;
			break;
		case ICE:
			ObjectPool::GetObjPool().EffectOnEnemies(this);
			Suicide();
		case CLEAN:
			ObjectPool::GetObjPool().EffectOnBullets(this);
			Suicide();
			break;
		case HEAL:
			break;
		case SELFBOOM:
			SetSpeed(100000);
			SetPos(host);
			SetCollisionRange(300);
			SetDMG(host->GetDMG());
			break;
		case ICE_RUNEBOMB:
		case RUNEBOMB:
		{
						 float x, y;
						 hge->Input_GetMousePos(&x, &y);
						 SetPos(x, y);
						 SetAim(x, y);
						 SetSpeed(50000);
						 SetDMG(host->GetDMG()*2);
						 SetCollisionRange(100);
						 last_time = 100;
		}
			break;
		case LIGHTARROW:
			SetPos(host);
			SetCollisionRange(25);
			SetSpeed(30);
			SetDMG(host->GetDMG());
			SetRot(host->GetRot());
			break;
		case BOUNCEARROW_ICE:
		case BOUNCEARROW_LIGHTARROW:
		case BOUNCEARROW:
			last_time = 5;
			SetPos(host);
			SetCollisionRange(0);
			SetSpeed(300);
			SetDMG(host->GetDMG());
			SetRot(host->GetRot());
			break;
		case ICE_TRACKBOMB:
		case HEAL_TRACKBOMB:
		case TRACKBOMB:
			SetAim(ObjectPool::GetObjPool().GetRandomEnemyUnit(GetType()));
			SetSpeed(200);
			SetDMG(0);
			SetPos(host);
			data2 = host->GetDMG();
			SetCollisionRange(10);
			SetRot(host->GetRot());
			break;
		case FLASH:
			break;
		case MISSILES:
			break;
		case ENEMYSKILL:
			data2 = customdata1;
			switch (customdata1)
			{
			case BUFF_I:
				SetPos(host->GetX(), host->GetY());
				SetSpeed(100);
				SetRot(host->GetRot());
				break;
			case ATTACK_I:
				SetPos(host);
				last_time = 100;
				host->StartSkill();
				break;
			case ETERNITY_I:
				last_time = 200;
				host->StartSkill();
				data = host->GetHP();
				break;
			case MAGIC_I:
				host->StartSkill();
				SetCollisionRange(host->GetCollisionRange());
				SetPos(host->GetX(), host->GetY());
				SetSpeed(300);
				SetAim(hge->Random_Int(GetActStartX(), GetActEndX()), hge->Random_Int(GetActStartY(), GetActEndY()));
				break;
			}
			break;
		case ENEMYSKILL2:
			data2 = customdata1;
			switch (customdata1)
			{
			case BUFF_I:
				break;
			case ATTACK_I:
				last_time = 300;
				SetSpeed(400);
				SetPos(host);
				SetRot(host->GetRot());
				SetCollisionRange(host->GetCollisionRange());
				host->StartSkill();
				break;
			case ETERNITY_I:
				last_time = 150;
				SetSpeed(0);
				SetPos(host);
				SetAim(ObjectPool::GetObjPool().GetEnemyHero(GetType()));
				data = host->GetCollisionRange();
				host->SetCollisionRange(-1);
				SetCollisionRange(data);
				host->StartSkill();
				break;
			case MAGIC_I:
				last_time = 40;
				SetPos(host->GetX(), host->GetY());
				data = hge->Random_Float(0, M_PI * 2);
				host->StartSkill();
				break;
			}
			break;
		case ENEMYSKILL3:
			data2 = customdata1;
			switch (customdata1)
			{
			case BUFF_I:
				break;
			case ATTACK_I:
				last_time = 50;
				SetPos(host);
				break;
			case ETERNITY_I:
				last_time = 150;
				host->StartSkill();
				SetPos(host);
				break;
			case MAGIC_I:
				last_time = 100;
				SetPos(host->GetX(), host->GetY());
				break;
			}
			break;
		case ENEMYSKILL4:
			data2 = customdata1;
			switch (customdata1)
			{
			case BUFF_I:
				break;
			case ATTACK_I:
				last_time = 75;
				SetPos(host);
				host->StartSkill();
				break;
			case ETERNITY_I:
				last_time = 25;
				SetPos(host);
				data = hge->Random_Int(0, 1) ? 0 : M_PI_4;
				host->StartSkill();
				break;
			case MAGIC_I:
				break;
			}
			break;
		case ENEMYSKILL5:
			data2 = customdata1;
			switch (customdata1)
			{
			case BUFF_I:
				break;
			case ATTACK_I:
				last_time = 15;
				host->StartSkill();
				break;
			case ETERNITY_I:
				SetCollisionRange(host->GetCollisionRange());
				break;
			case MAGIC_I:
				for (int i = 0; i < 4; i++)
				{
					Skill *skill = new Skill(host, LAZER);
					skill->SetRot(M_PI*i / 2);
					ObjectPool::GetObjPool().InsertObject(skill, true);
				}
				last_time = 75;
				host->StartSkill();
				break;
			}
			break;
		case ENEMYSKILL6:
			data2 = customdata1;
			switch (customdata1)
			{
			case BUFF_I:
				last_time = 100;
				SetPos(host);
				host->StartSkill();
				break;
			case ATTACK_I:
				last_time = 50;
				host->StartSkill();
				SetPos(host);
				break;
			case ETERNITY_I:
				last_time = 50;
				host->StartSkill();
				SetPos(host);
				break;
			case MAGIC_I:
			    target = ObjectPool::GetObjPool().GetEnemyHero(GetType());
				SetSpeed(host->GetSpeed());
				SetCollisionRange(host->GetCollisionRange());
				SetPos(host);
				host->StartSkill();
				last_time = 150;
				break;
			}
			break;
		case BOSSSKILL1:
			data2 = customdata1;
			switch (data2)
			{
			case ENEMY_BOSS_I:
				last_time = 300;
				SetPos(host);
				host->StartSkill();
				break;
			}
			break;
		case BOSSSKILL2:
			data2 = customdata1;
			switch (data2)
			{
			case ENEMY_BOSS_I:
				break;
			}
			break;
		case BOSSSKILL3:
			data2 = customdata1;
			switch (data2)
			{
			case ENEMY_BOSS_I:
				last_time = 200;
				SetPos(this);
				data = hge->Random_Int(0,1);
				data3 = hge->Random_Int(25, 200);
				host->StartSkill();
				break;
			}
			break;
		case BOSSSKILL4:
			data2 = customdata1;
			switch (data2)
			{
			case ENEMY_BOSS_I:
				for (int i = 0; i < 8; i++)
				{
					Skill *skill = new Skill(GetHost(), LAZER, 100);
					skill->SetRot(M_PI*i / 4);
					ObjectPool::GetObjPool().InsertObject(skill, true);
				}
				last_time = 160;
				SetPos(host);
				host->StartSkill();
				break;
			}
			break;
		case BOSSSKILL5:
			data2 = customdata1;
			switch (data2)
			{
			case ENEMY_BOSS_I:
				last_time = 200;
				SetPos(host);
				host->StartSkill();
				break;
			}
			break;
		case BOSSSKILL6:
			data2 = customdata1;
			switch (data2)
			{
			case ENEMY_BOSS_I:
				last_time = 200;
				SetPos(this);
				host->StartSkill();
				break;
			}
			break;
		case BOSSSKILL7:
			data2 = customdata1;
			switch (data2)
			{
			case ENEMY_BOSS_I:
				
				break;
			}
			break;
		case BOSSSKILL8:
			data2 = customdata1;
			switch (data2)
			{
			case ENEMY_BOSS_I:

				break;
			}
			break;
		default:
			SetPos(host);
			break;
		}
	};
	virtual void Interaction(GameObject *target);
	virtual void Born()
	{
		finishb = true;
	};
	virtual void Death();
	virtual void Action();
	virtual void AfterOneEffect();
	virtual bool IsSkill()
	{
			return true;
	};
	GameObject *GetHost()const{ return Myhost; };
	void SkillInteraction(GameObject *target)
	{
		if (!target->IsEffectiveForOtherSkill())
			target->Suicide();
	}
	virtual void SafeOperation()
	{ 
		if (Myhost && !Myhost->avlid)DeleteHost();
		if (target && (!target->avlid || !target->IsReachable()))target = NULL;
		GameObject::SafeOperation();
	};
	virtual bool IsEffectiveForOtherSkill()const{ return CanEffectOtherSkill; };
	virtual int GetDMG(bool forattack = false)const
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		if (!forattack)
			return GameObject::GetDMG()*2;
		else
		{
			if (GetType() == Left)
			{
				int dmg = GameObject::GetDMG();
				return -((dmg+hge->Random_Int(0,5))*(hge->Random_Int(0, 9) ? 1 : 2));
			}
			else
				return GameObject::GetDMG(true);
		}
	};
	void DeleteHost()
	{
		Myhost = NULL;
	}
	~Skill();
private:
	GameObject *Myhost;
	GameObject *target;
	int last_time;
	float data;
	bool CanEffectOtherSkill;
	int data2;
	float data3;
};

