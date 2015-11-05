#include "Enemy.h"
#include"ObjectPool.h"
#include "FontPool.h"
#include "Skill.h"
#include "CatchThing.h"

template <class T>
int getArrayLen(T& array)
{
	return (sizeof(array) / sizeof(array[0]));
}

Enemy::Enemy(int &unitcount,int x, int y,int type) :GameObject(0, 0, 0, 0, 0, 0, Right, 0, 0)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	SetBlock(true);
	customdata = 0;
	custombool = false;
	atkuptimer = 0;
	absorbtimer = 0;
	waittime = 0;
	mp = 2;
	gotox = gotoy = 0;
	flag = false;
	exps = 0;
	NoChangeRot = false;
	SetSize(1);
	eliteshootcd = 50;
	int maxspawn = ENEMY_NORMAL + SceneManager::GetSceneManager().GetWave();
	if (maxspawn >= ENEMY_END)
		maxspawn = ENEMY_END;
	SetInnerCode(type ? type : hge->Random_Int(ENEMY_START,maxspawn));
	SetAim(ObjectPool::GetObjPool().GetEnemyHero(GetType()));
	SetX(x ? x : GetActStartX() + (hge->Random_Int(0, 1) ? -1 : 1)*(GetActEndX() - GetActStartX())*hge->Random_Float(1.2, 1.5));
	SetY(y ? y : GetActStartY() + (hge->Random_Int(0, 1) ? -1 : 1)*hge->Random_Float(1.2, 1.5)*(GetActStartY() - GetActEndY()));
	switch (GetInnerCode())
	{
	case ENEMY_NORMAL:
		SetHP(15);
		SetDMG(1);
		SetSpeed(150);
		SetCollisionRange(20);
		unitcount -= 1;
		exps = 1;
		break;
	case ENEMY_BULLET:
		SetHP(5);
		SetDMG(1);
		SetSpeed(10);
		SetCollisionRange(20);
		gotox = GetX() + hge->Random_Float(-50, 50);
		gotoy = GetY() + hge->Random_Float(-50, 50);
		unitcount -= 2;
		exps = 1;
		break;
	case ENEMY_HIGHSPEED:
		SetHP(25);
		SetDMG(1);
		SetSpeed(600);
		SetCollisionRange(40);
		gotox = GetAimx() + (hge->Random_Int(0, 1) ? hge->Random_Int(-300, -80) : hge->Random_Int(80, 300));
		gotoy = GetAimy() + (hge->Random_Int(0, 1) ? hge->Random_Int(-300, -80) : hge->Random_Int(80, 300));
		unitcount -= 4;
		exps = 2;
		break;
	case ENEMY_FLASH:
		SetHP(10);
		SetDMG(1);
		SetSpeed(100);
		SetCollisionRange(25);
		unitcount -= 4;
		exps = 2;
		break;
	case ENEMY_DOUBLE:
		SetHP(45);
		SetDMG(1);
		SetSpeed(100);
		SetCollisionRange(30);
		unitcount -= 3;
		exps = 1;
		break;
	case ENEMY_DASHER:
		SetHP(35);
		SetDMG(2);
		SetSpeed(80);
		SetCollisionRange(40);
		unitcount -= 2;
		exps = 2;
		break;
	case ENEMY_ZOMBLE:
		SetHP(200);
		SetDMG(2);
		SetSpeed(20);
		SetCollisionRange(50);
		unitcount -= 4;
		exps = 3;
		break;
	case ENEMY_BOMBER:
		SetHP(30);
		SetDMG(4);
		SetSpeed(200);
		SetCollisionRange(40);
		SetRot(hge->Random_Float(0, M_PI * 2));
		unitcount -= 2;
		exps = 2;
		break;
	case ENEMY_BULLET_BOMBER:
		SetHP(30);
		SetDMG(2);
		SetSpeed(150);
		SetCollisionRange(30);
		SetRot(hge->Random_Float(0, M_PI * 2));
		unitcount -= 2;
		exps = 2;
		break;
	case ENEMY_SHIELD:
		SetHP(100);
		SetDMG(1);
		SetSpeed(0);
		SetCollisionRange(10);
		unitcount -= 4;
		exps = 4;
		break;
	case ENEMY_GUARD1:
	case ENEMY_GUARD2:
	case ENEMY_GUARD3:
		SetSolid(true);
		customdata = hge->Random_Int(1, 4);
		SetHP(100);
		SetDMG(2);
		SetSpeed(151);
		SetCollisionRange(10);
		unitcount -= 4;
		exps = 2;
		flag = hge->Random_Int(0, 1);
		eliteshootcd = GetCollisionRange() * hge->Random_Int(1, 10);
		break;
	case ENEMY_FLASH2:
		SetHP(100);
		SetDMG(2);
		SetSpeed(100);
		SetCollisionRange(10);
		unitcount -= 4;
		exps = 2;
		break;
	case ENEMY_BIGBOMB:
		SetHP(800);
		SetSolid(true);
		SetDMG(8);
		SetSpeed(25);
		SetCollisionRange(10);
		SetSize(2);
		SetRot(hge->Random_Float(0, M_PI * 2));
		exps = 2;
		unitcount -= 10;
		break;
	case ENEMY_SUMMONER:
		SetHP(70);
		SetDMG(2);
		SetSpeed(100);
		SetCollisionRange(10);
		gotox = hge->Random_Int(GetActStartX(), GetActEndX());
		gotoy = hge->Random_Int(GetActStartY(), GetActEndY());
		unitcount -= 4;
		exps = 6;
		break;
	case ENEMY_DASHER2:
		SetHP(80);
		SetDMG(2);
		SetSpeed(50);
		SetCollisionRange(10);
		SetRot(-M_PI / 2);
		unitcount -= 4;
		waittime = hge->Random_Int(50, 300);
		exps = 8;
		break;
	case ENEMY_ELITE:
	{   
		int random1 = hge->Random_Int(CORE_START,CORE_END);
		Robot *item = new Robot(this,random1, Right);
		SetAim(item);
		ObjectPool::GetObjPool().InsertObject(item,true);
		SetPos(GetActStartX() + (hge->Random_Int(0, 1) ? (GetActEndX() - GetActStartX())*hge->Random_Float(0.1, 0.2) : (GetActEndX() - GetActStartX())*hge->Random_Float(0.8, 0.9)),
			   GetActStartY() + (hge->Random_Int(0, 1) ? (GetActEndY() - GetActStartY())*hge->Random_Float(0.1, 0.2) : (GetActEndY() - GetActStartY())*hge->Random_Float(0.8, 0.9)));
		gotox = GetActStartX() + (GetActEndX() - GetActStartX())*hge->Random_Float(0.2, 0.8);
		gotoy = hge->Random_Float(GetActStartY()+20, GetActEndY()-20);
		SetDMG(4);
		SetHP(800);
		SetSolid(true);
		SetSpeed(100);
		SetCollisionRange(100);
		waittime = 50;
		exps = 10;
		switch (random1)
		{
		case CORE_HEALHP :
		case CORE_ATK :
		case CORE_HARDSKIN :
		case CORE_SHOOTCD :
		case CORE_SPEED :
		case CORE_SHIELD :
		case CORE_HEALMP :
		case CORE_GETBLOOD :
		case CORE_UPDATE:
		case CORE_GETMP:
		{
							elitetype = BUFF_I;
							int copy[8] = { 2, 6, 8, 6, 8, 4, 0, 0 };
							memcpy(skillcost, copy, sizeof(skillcost));
		}
			break;
		case CORE_ICE :
		case CORE_BOOM :
		case CORE_CLEAN :
		case CORE_RUNEBOMB :
		case CORE_GETSHIELD :
		case CORE_LIGHTARROW :
		case CORE_BOUNCEARROW :
		case CORE_TRACKBOMB :
		case CORE_FLASH :
		case CORE_HEAL :
		{
							   elitetype = MAGIC_I;
							   int copy[8] = { 6, 8, 4, 2, 6, 8, 0, 0 };
							   memcpy(skillcost, copy, sizeof(skillcost));
		}
			break;
		case CORE_ADDMAXHP :
		case CORE_ADDDMG :
		case CORE_ADDMPHEAL :
		case CORE_ADDSPEED:
		{
							  elitetype = ETERNITY_I;
							  int copy[8] = { 8, 4, 8, 4, 6, 6, 0, 0 };
							  memcpy(skillcost, copy, sizeof(skillcost));
		}
			break;
		case CORE_NORMAL :
		case CORE_SHOTGUN :
		case CORE_LAZER :
		case CORE_MISSILE:
		{
							 elitetype = ATTACK_I;
							 int copy[8] = { 4, 4, 4, 2, 1, 6, 0, 0 };
							 memcpy(skillcost, copy, sizeof(skillcost));
		}
			break;
		
		}
		//SetHP(20);
	}
		break;
	case ENEMY_BOSS_I:
		SetDMG(4);
		SetHP(2000);
		SetSolid(true);
		SetSpeed(0);
		SetCollisionRange(150);
		SetPos(ACT_CENTER_X, ACT_CENTER_Y);
		{
			int copy[8] = { 8, 4, 2, 4, 2, 6, 6, 6 };
			memcpy(skillcost, copy, sizeof(skillcost));
		}
		waittime = 50;
		exps = 50;
		unitcount -= 1000;
		break;
	}
	RestrictGotoPos();
};

void Enemy::Action()
{
	if (GetAim() && EnemyEntry())
	{
		GameObject::Action();
		SetImgAngle(GetRot());
		switch (GetInnerCode())
		{
		case ENEMY_DOUBLE:
		case ENEMY_NORMAL:
			GoToAim();
			break;
		case ENEMY_BULLET:
			if (waittime >= 200)
			{
				HGE *hge = hgeCreate(HGE_VERSION);
				for (int i = 0; i < 3; i++)
				{
					Skill *skill = new Skill(this, ENEMYBULLET);
					skill->SetRot(skill->GetRot() + 0.2 - 0.2*i);
					ObjectPool::GetObjPool().InsertObject(skill, true);
				}
				waittime = 0;
			}
			else
				waittime++;
			if (GoToAnotherPos(gotox, gotoy))
			{
				HGE *hge = hgeCreate(HGE_VERSION);
				gotox = GetX() + hge->Random_Float(-50, 50);
				gotoy = GetY() + hge->Random_Float(-50, 50);
				RestrictGotoPos();
			}
			GetAngle(GetAim());
			break;
		case ENEMY_HIGHSPEED:
			if (GetDistance(gotox, gotoy) < 10)
			{
				if (waittime >= 100)
				{
					HGE *hge = hgeCreate(HGE_VERSION);
					flag = !flag;
					if (flag)
					{
						gotox = GetAimx() + (hge->Random_Int(0, 1) ? hge->Random_Int(-300, -80) : hge->Random_Int(80, 300));
						gotoy = GetAimy() + (hge->Random_Int(0, 1) ? hge->Random_Int(-300, -80) : hge->Random_Int(80, 300));
					}
					else
					{
						gotox = GetAimx() + (hge->Random_Int(0, 1) ? hge->Random_Int(-50, -20) : hge->Random_Int(20, 50));
						gotoy = GetAimy() + (hge->Random_Int(0, 1) ? hge->Random_Int(-50, -20) : hge->Random_Int(20, 50));
					}
					RestrictGotoPos();
					waittime = 0;
				}
				else
					waittime++;
			}
			else
				GoToAnotherPos(gotox, gotoy);
			GetAngle(GetAim());
			break;
		case ENEMY_FLASH:
			if (waittime >= 200)
			{
				HGE *hge = hgeCreate(HGE_VERSION);
				flag = !flag;
				if (flag)
				{
					gotox = GetAimx() + (hge->Random_Int(0, 1) ? hge->Random_Int(-300, -80) : hge->Random_Int(80, 300));
					gotoy = GetAimy() + (hge->Random_Int(0, 1) ? hge->Random_Int(-300, -80) : hge->Random_Int(80, 300));
				}
				else
				{
					gotox = GetAimx() + (hge->Random_Int(0, 1) ? hge->Random_Int(-50, -20) : hge->Random_Int(20, 50));
					gotoy = GetAimy() + (hge->Random_Int(0, 1) ? hge->Random_Int(-50, -20) : hge->Random_Int(20, 50));
				}
				RestrictGotoPos();
				SetPos(gotox, gotoy);
				waittime = 0;
			}
			else
			{
				waittime++;
				if (waittime % 75 == 0)
				{
					Skill *skill = new Skill(this, ENEMYBULLET,BULLET_SPEEDSTAY);
					skill->SetSpeed(400);
					ObjectPool::GetObjPool().InsertObject(skill, true);
				}
			}
			GetAngle(GetAim());
			break;
		case ENEMY_DASHER:
			if (waittime>0)
			{
				GoWithAnAngle(GetRot() - 1.57);
				SetSpeed(GetSpeed() - 5);
				waittime--;
			}
			else
			{
				if (GetDistance(GetAim()) < 200)
				{
					waittime = 100;
					SetSpeed(GetSpeed() + 500);
				}
				GoToAim();
			}
			break;
		case ENEMY_BOMBER:
		case ENEMY_BIGBOMB:
		case ENEMY_BULLET_BOMBER:
			GoWithAnAngle(GetRot() - 1.57);
			if (GetX()<GetActStartX() || GetY()<GetActStartY() || GetX()>GetActEndX() || GetY()>GetActEndY())
			{
				if (GetY()>GetActStartY())
					SetRot(-GetRot());
				else
					SetRot(M_PI - GetRot());
				if (GetY()>GetActEndY())
					SetRot(M_PI + GetRot());
			}
			break;
		case ENEMY_ZOMBLE:
			if (GetHP() < 50)
			{
				if (GetSpeed() < 300)
					SetSpeed(GetSpeed() + 5);
			}
			GoToAim();
			break;
		case ENEMY_SHIELD:
			if (waittime >= 250)
			{
				Skill *skill = new Skill(this, THROWBULLET,THROWBULLET_CENTER);
				ObjectPool::GetObjPool().InsertObject(skill, true);
				waittime = 0;
			}
			else
				waittime++;
			break;
		case ENEMY_GUARD1:
			if (flag)
			{
				switch (customdata)
				{
				case 1:
					if (GoToAnotherPos(GetActEndX() - eliteshootcd, GetActStartY()))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 2:
					if (GoToAnotherPos(GetActStartX() + eliteshootcd, GetActStartY()))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 3:
					if (GoToAnotherPos(GetActStartX(), GetActStartY() + eliteshootcd))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 4:
					if (GoToAnotherPos(GetActStartX(), GetActEndY() - eliteshootcd))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				}
			}
			else
			{
				switch (customdata)
				{
				case 1:
					if (GoToAnotherPos(GetActEndX() - eliteshootcd, GetActEndY()))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 2:
					if (GoToAnotherPos(GetActStartX() + eliteshootcd, GetActEndY()))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 3:
					if (GoToAnotherPos(GetActEndX(), GetActStartY() + eliteshootcd))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 4:
					if (GoToAnotherPos(GetActEndX(), GetActEndY() - eliteshootcd))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				}
			}
			switch (customdata)
			{
			case 1:
				SetRot(-M_PI / 2);
				break;
			case 2:
				SetRot(M_PI / 2);
				break;
			case 3:
				SetRot(M_PI);
				break;
			case 4:
				SetRot(0);
				break;
			}
			if (custombool)
			{
				if (waittime >= 100)
				{
					for (int i = 0; i < 4; i++)
					{
						Skill *skill = new Skill(this, ENEMYBULLET, BULLET_SPEEDSTAY);
						skill->SetRot(M_PI*i / 2);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
					waittime = 0;
				}
				else
					waittime++;
			}
			break;
		case ENEMY_GUARD2:
			if (flag)
			{
				switch (customdata)
				{
				case 1:
					if (GoToAnotherPos(GetActEndX() - eliteshootcd, GetActStartY()))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 2:
					if (GoToAnotherPos(GetActStartX() + eliteshootcd, GetActStartY()))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 3:
					if (GoToAnotherPos(GetActStartX(), GetActStartY() + eliteshootcd))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 4:
					if (GoToAnotherPos(GetActStartX(), GetActEndY() - eliteshootcd))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				}
			}
			else
			{
				switch (customdata)
				{
				case 1:
					if (GoToAnotherPos(GetActEndX() - eliteshootcd, GetActEndY()))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 2:
					if (GoToAnotherPos(GetActStartX() + eliteshootcd, GetActEndY()))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 3:
					if (GoToAnotherPos(GetActEndX(), GetActStartY() + eliteshootcd))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 4:
					if (GoToAnotherPos(GetActEndX(), GetActEndY() - eliteshootcd))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				}
			}
			switch (customdata)
			{
			case 1:
				SetRot(-M_PI / 2);
				break;
			case 2:
				SetRot(M_PI / 2);
				break;
			case 3:
				SetRot(M_PI);
				break;
			case 4:
				SetRot(0);
				break;
			}
			if (custombool)
			{
				if (waittime >= 100)
				{
					for (int i = 0; i < 3; i++)
					{
						Skill *skill = new Skill(this, ENEMYBULLET);
						skill->SetRot(skill->GetRot() + 0.2 - 0.2*i);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
					waittime = 0;
				}
				else
					waittime++;
			}
			break;
		case ENEMY_GUARD3:
			if (flag)
			{
				switch (customdata)
				{
				case 1:
					if (GoToAnotherPos(GetActEndX() - eliteshootcd, GetActStartY()))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 2:
					if (GoToAnotherPos(GetActStartX() + eliteshootcd, GetActStartY()))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 3:
					if (GoToAnotherPos(GetActStartX(), GetActStartY() + eliteshootcd))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 4:
					if (GoToAnotherPos(GetActStartX(), GetActEndY() - eliteshootcd))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				}
			}
			else
			{
				switch (customdata)
				{
				case 1:
					if (GoToAnotherPos(GetActEndX() - eliteshootcd, GetActEndY()))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 2:
					if (GoToAnotherPos(GetActStartX() + eliteshootcd, GetActEndY()))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 3:
					if (GoToAnotherPos(GetActEndX(), GetActStartY() + eliteshootcd))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				case 4:
					if (GoToAnotherPos(GetActEndX(), GetActEndY() - eliteshootcd))
					{
						flag = !flag;
						custombool = true;
					}
					break;
				}
			}
			switch (customdata)
			{
			case 1:
				SetRot(-M_PI / 2);
				break;
			case 2:
				SetRot(M_PI / 2);
				break;
			case 3:
				SetRot(M_PI);
				break;
			case 4:
				SetRot(0);
				break;
			}
			if (custombool)
			{
				if (waittime >= 100)
				{
					Skill *skill = new Skill(this, LAZER);
					ObjectPool::GetObjPool().InsertObject(skill, true);
					waittime = 0;
				}
				else
					waittime++;
			}
			break;
		case ENEMY_FLASH2:
			GetAngle(GetAim());
			if (waittime >= 150)
				waittime = 0;
			else
			{
				waittime++;
				if (waittime == 80)
				{
					HGE *hge = hgeCreate(HGE_VERSION);
					SetPos(hge->Random_Int(GetActStartX(), GetActEndX()), hge->Random_Int(GetActStartY(), GetActEndY()));
				}
				if (waittime == 100)
				{
					for (int i = 0; i < 3; i++)
					{
						Skill *skill = new Skill(this, ENEMYBULLET);
						skill->SetRot(skill->GetRot() + 0.2 - 0.2*i);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
				}
			}
			break;
		case ENEMY_SUMMONER:
			GoToAnotherPos(gotox, gotoy);
			if (waittime >= 150)
			{
				HGE *hge = hgeCreate(HGE_VERSION);
				int a = 0;
				Enemy *baby = new Enemy(a, GetX(), GetY(), ENEMY_NORMAL);
				baby->exps = 0;
				ObjectPool::GetObjPool().InsertObject(baby, true);
				waittime = 0;
			}
			else
				waittime++;
			break;
		case ENEMY_DASHER2:
			/*
			if (!flag)
			{
				if (abs(GetX() - GetAim()->GetX()) < GetCollisionRange())
				{
					if (GetY()>GetAim()->GetY())
						SetRot(0);
					else
						SetRot(M_PI);
					flag = !flag;
				}
			}
			else
			{
				if (abs(GetY() - GetAim()->GetY()) < GetCollisionRange())
				{
					if (GetX()>GetAim()->GetX())
						SetRot(-M_PI/2);
					else
						SetRot(M_PI/2);
					flag = !flag;
				}
			}
			*/
			if (!flag)
			{
				if (waittime)
				{
					waittime--;
					float rot = GetRot();
					if (rot == 0 && GetAim()->GetY() < GetY() && abs(GetAim()->GetX() - GetX()) < GetCollisionRange())
						flag = true;
					else if (rot == M_PI && GetAim()->GetY() >= GetY() && abs(GetAim()->GetX() - GetX()) < GetCollisionRange())
						flag = true;
					else if (rot == M_PI / 2 && GetAim()->GetX()>GetX() && abs(GetAim()->GetY() - GetY()) < GetCollisionRange())
						flag = true;
					else if (rot == -M_PI / 2 && GetAim()->GetX() <= GetX() && abs(GetAim()->GetY() - GetY()) < GetCollisionRange())
						flag = true;
					if (flag)
						SetSpeed(300);
					if (IsOutOfBound())
						waittime = 0;
				}
				else
				{
					HGE *hge = hgeCreate(HGE_VERSION);
					waittime = hge->Random_Int(50, 300);
					switch (hge->Random_Int(0, 3))
					{
					case 0:
						SetRot(0);
						break;
					case 1:
						SetRot(M_PI);
						break;
					case 2:
						SetRot(-M_PI / 2);
						break;
					case 3:
						SetRot(M_PI / 2);
						break;
					}
				}
			}
			else
			{
				SetSpeed(GetSpeed() + 5);
				if (IsOutOfBound())
				{
					flag = false;
					if (GetRot() == 0)
						SetRot(M_PI);
					else if (GetRot() == M_PI)
						SetRot(0);
					else if (GetRot() == -M_PI / 2)
						SetRot(M_PI / 2);
					else if (GetRot() == M_PI / 2)
						SetRot(-M_PI / 2);
					SetSpeed(50);
				}
			}
			GoWithAnAngle(GetRot() - 1.57);
			break;
		case ENEMY_ELITE:
				if (!NoChangeRot)
				{
					if (GoToAnotherPos(gotox, gotoy))
						ResetGotoPos();
					GetAngle(ObjectPool::GetObjPool().GetEnemyHero(GetType()));
					if (waittime)
						waittime--;
					else
					{
						HGE *hge = hgeCreate(HGE_VERSION);
						int random = hge->Random_Int(0, 5);
						if (MPUse(skillcost[random]))
						{
							Skill *skill = new Skill(this, ENEMYSKILL + random,elitetype);
							ObjectPool::GetObjPool().InsertObject(skill, true);
						}
						waittime = 50;
						if (GetHP() > GetMaxHP()*0.3)
							mp += 1;
						else
							mp += 4;
					}
				}
				GetAim()->SetPos(GetX(), GetY());
			break;
		case ENEMY_BOSS_I:
			SetPos(ACT_CENTER_X, ACT_CENTER_Y);
			if (!NoChangeRot)
			{
				TurnToAim();
				if (waittime)
					waittime--;
				else
				{
					HGE *hge = hgeCreate(HGE_VERSION);
					int random = hge->Random_Int(0, 5);
					if (MPUse(skillcost[random]))
					{
						Skill *skill = new Skill(this, BOSSSKILL1 + random,ENEMY_BOSS_I);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
					waittime = 50;
					if (GetHP() > GetMaxHP()*0.5)
						mp += 1;
					else
						mp += 4;
				}
			}
			break;
		}
		if (atkuptimer > 0)
			atkuptimer--;
		else
			atkuptimer = 0;
		if (absorbtimer > 0)
			absorbtimer--;
		else
			absorbtimer = 0;

		if (GetX()<GetActStartX())
			SetX(GetActStartX());
		if (GetX() > GetActEndX())
			SetX(GetActEndX());
		if (GetY()<GetActStartY())
			SetY(GetActStartY());
		if (GetY()>GetActEndY())
			SetY(GetActEndY());
		if (GetAim() && GetAim()->GetType() != GetType() && GetDistance(GetAim()) < GetCollisionRange())
			Interaction(GetAim());
	}
}


void Enemy::Interaction(GameObject *target)
{
	if (target->GetInnerCode() == HERO)
	{
		switch (GetInnerCode())
		{
		case ENEMY_BOMBER:
		case ENEMY_BULLET_BOMBER:
			Suicide(true);
		}
	}
		GameObject::Interaction(target);
}


void Enemy::Death()
{
	int a = 0;
	HGE *hge = hgeCreate(HGE_VERSION);
	int colrange = GetCollisionRange();
	switch (GetInnerCode())
	{
	case ENEMY_DOUBLE:
		for (int i = 0; i < 2; i++)
		{
			Enemy *enemy = new Enemy(a, GetX() + hge->Random_Int(-colrange, colrange), GetY() + hge->Random_Int(-colrange, colrange), ENEMY_NORMAL);
			ObjectPool::GetObjPool().InsertObject(enemy, true);
		}
		break;
	case ENEMY_BOMBER:
	{
						 Skill *skill = new Skill(this, TBOMB);
						 skill->SetType(None);
						 ObjectPool::GetObjPool().InsertObject(skill, true);
	}
		break;
	case ENEMY_BULLET_BOMBER:
		for (int i = 0; i < 6; i++)
		{
			Skill *skill = new Skill(this, ENEMYBULLET);
			skill->SetRot(i*M_PI /3);
			skill->SetType(None);
			ObjectPool::GetObjPool().InsertObject(skill, true);
		}
		break;
	case ENEMY_SUMMONER:
		switch (hge->Random_Int(0, 3))
		{
		case 0:
			for (int i = 0; i < 4; i++)
			{
				Enemy *enemy = new Enemy(a, GetX() + hge->Random_Int(-colrange, colrange), GetY() + hge->Random_Int(-colrange, colrange), ENEMY_NORMAL);
				ObjectPool::GetObjPool().InsertObject(enemy, true);
			}
			break;
		case 1:
			for (int i = 0; i < 2; i++)
			{
				Enemy *enemy = new Enemy(a, GetX() + hge->Random_Int(-colrange, colrange), GetY() + hge->Random_Int(-colrange, colrange), ENEMY_DOUBLE);
				ObjectPool::GetObjPool().InsertObject(enemy, true);
			}
			break;
		case 2:
		{
				  Enemy *enemy = new Enemy(a, GetX(), GetY(), ENEMY_FLASH);
				  ObjectPool::GetObjPool().InsertObject(enemy, true);
		}
			break;
		case 3:
		{
				  Enemy *enemy = new Enemy(a, GetX(), GetY(), ENEMY_HIGHSPEED);
				  ObjectPool::GetObjPool().InsertObject(enemy, true);
		}
			break;
		}
		break;
	case ENEMY_BIGBOMB:
		if (exps > 2)
		{
			exps /= 2;
			for (int i = 0; i < 2; i++)
			{
				int data = (2 - (GetSize() - 0.4)) / 0.4;
				Enemy *enemy = new Enemy(a, GetX(), GetY(), ENEMY_BIGBOMB);
				enemy->SetHP(GetMaxHP() / 2);
				enemy->SetSize(GetSize() - 0.4);
				enemy->SetSpeed(25 * data);
				enemy->exps = exps;
				int maxdmg = 100;
				for (int i = 0; i < data; i++)
					maxdmg /= 2;
				enemy->SetDMG(maxdmg);
				ObjectPool::GetObjPool().InsertObject(enemy, true);
			}
		}
		break;
	case ENEMY_ELITE:
		switch (GetAim()->GetInnerCode())
		{
		case CORE_ICE:
			dynamic_cast<Hero*>(ObjectPool::GetObjPool().GetEnemyHero(GetType()))->SetIceMode(false);
			break;
		}
		if (GetAim())
			GetAim()->SetType(None);
		break;
	}
	ObjectPool::GetObjPool().GetEnemyHero(GetType())->Interaction(this);
	if (exps)
	{
		switch (GetInnerCode())
		{
		case ENEMY_ELITE:
			for (int i = 0; i < 10; i++)
			{
				CatchThing *catching = new CatchThing(this, GEM);
				ObjectPool::GetObjPool().InsertObject(catching, true);
			}
			break;
		default:
			for (int i = 0; i < hge->Random_Int(2,4); i++)
			{
				CatchThing *catching = new CatchThing(this, GEM);
				ObjectPool::GetObjPool().InsertObject(catching, true);
			}
			break;
		}
		if (hge->Random_Int(0, 100) < 5)
		{
			CatchThing *catching = new CatchThing(this, CRYSTAL);
			ObjectPool::GetObjPool().InsertObject(catching, true);
		}
		if (hge->Random_Int(0, 100) < 5)
		{
			CatchThing *catching = new CatchThing(this, BATTERY);
			ObjectPool::GetObjPool().InsertObject(catching, true);
		}
	}
	GameObject::Death();
}


Enemy::~Enemy()
{
}


bool Enemy::VaryHP_Obj(GameObject *obj)
{
	int vary = obj->GetDMG(true);
	Type dmgtype = Damage;
	if (abs(vary) > obj->GetDMG())
		dmgtype = Crit;
	if (vary > 0)
		dmgtype = Heal;
	vary = DMGDispose(vary);
	GameObject::VaryHP(vary);
	FontPool::GetFontPool().InsertFont(GetX(), GetY(), vary, dmgtype,NeedRenderFont());
	return true;
}


bool Enemy::VaryHP(int vary)
{
	int varied = DMGDispose(vary);
	if (varied < 0)
	{
		GameObject::VaryHP(varied);
		FontPool::GetFontPool().InsertFont(GetX(), GetY(), varied, Damage, NeedRenderFont());
	}
	else
	{
		if (varied>0)
		{
			GameObject::VaryHP(varied);
			FontPool::GetFontPool().InsertFont(GetX(), GetY(), varied, Heal, NeedRenderFont());
		}
	}
	return true;
}



void Enemy::SpawnEnemy(int innercode)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	int a = 0;
	int colrange = GetCollisionRange();
	int newx = GetX() + (hge->Random_Int(0, 1) ? hge->Random_Int(-colrange*1.5, -colrange*1.2) : hge->Random_Int(colrange*1.2, colrange*1.5));
	int newy = GetY() + (hge->Random_Int(0, 1) ? hge->Random_Int(-colrange*1.5, -colrange*1.2) : hge->Random_Int(colrange*1.2, colrange*1.5));
	RestrictGotoPos(newx, newy);
	Enemy *enemy = new Enemy(a, newx, newy,innercode);
	ObjectPool::GetObjPool().InsertObject(enemy, true);
}