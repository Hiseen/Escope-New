#include "Hero.h"
#include "Skill.h"
#include "ObjectPool.h"
#include "FontPool.h"
#include "SceneManager.h"

void Hero::Action()
{
	GameObject::Action();
	HGE *hge = hgeCreate(HGE_VERSION);
	if (!AI)
	{
		if (startangle == finalangle)
		{
			float x, y;
			hge->Input_GetMousePos(&x, &y);
			double ang = GetAngle(x, y, true);
			if (ang < M_PI / 4 && ang >= -M_PI / 4)
			{
				if (startangle == 3 * M_PI_2)
					finalangle = M_PI * 2;
				else
				if (startangle != M_PI * 2)
					finalangle = 0;
			}
			else
			if (ang >= M_PI / 4 && ang < M_PI * 3 / 4)
				finalangle = M_PI_2;
			else
			if (ang >= M_PI * 3 / 4 && ang < M_PI * 5 / 4)
				finalangle = M_PI;
			else
			if (ang >= M_PI * 5 / 4 || ang < -M_PI / 4)
			{
				if (startangle == 0)
					finalangle = -M_PI_2;
				else
				if (startangle != -M_PI_2)
					finalangle = 3 * M_PI_2;
			}
		}
		if (abs(GameObject::GetRot() - finalangle)>0.05)
		{
			SetRot(GameObject::GetRot() + (finalangle - GameObject::GetRot()) / 5);
		}
		else
		{
			if (finalangle == -M_PI_2)
				finalangle = M_PI_2 * 3;
			if (finalangle == M_PI * 2)
				finalangle = 0;
			startangle = finalangle;
			SetRot(finalangle);
		}
		SetImgAngle(GameObject::GetRot());
		switch (GetType())
		{
		case Left:
			if (speedrate == 1.0f)
			{
				if (hge->Input_GetKeyState(HGEK_W))
				if (dy > -MAX_SPEED)
				{
					//if (dy > 0)
					//	dy = -GetSpeed();
					//else
					dy -= GetSpeed();
				}
				if (hge->Input_GetKeyState(HGEK_S))
				if (dy<MAX_SPEED)
				{
					//if (dy < 0)
					//	dy = GetSpeed();
					//else
					dy += GetSpeed();
				}
				if (hge->Input_GetKeyState(HGEK_A))
				if (dx>-MAX_SPEED)
				{
					//if (dx>0)
					//	dx = -GetSpeed();
					//else
					dx -= GetSpeed();
				}
				if (hge->Input_GetKeyState(HGEK_D))
				if (dx < MAX_SPEED)
				{
					//if (dx<0)
					//	dx = GetSpeed();
					//else
					dx += GetSpeed();
				}
			}
			for (int i = 0; i < STD_CORES; i++)
			{
				if (COREs[i] && hge->Input_GetKeyState(HGEK_1 + i))
					dynamic_cast<Robot*>(COREs[i])->UseSkill();
			}
			if (!cannotskill&& !GetScilenced())
			{
				float x, y;
				hge->Input_GetMousePos(&x, &y);
				if (x>ACT_START_X && x<ACT_END_X && y>ACT_START_Y && y < ACT_END_Y)
				{
					if (startangle == finalangle && hge->Input_GetKeyState(HGEK_LBUTTON))
						BulletShoot();
					if (!skilldelay && hge->Input_KeyUp(HGEK_RBUTTON))
					{
						PowerSystem();
						skilldelay = 5;
					}
				}
			}
			break;
		}
	}
	else
	{
		/*
		//移动模块
		if (power >= 0.5)
		{
		choosemove = ObjectPool::GetObjPool().IsUPBiggerThanDown(this);
		switch (choosemove)
		{
		case 1:
		dy += GetSpeed()*(((absorb | reflect) ? -1 : 1));
		power -= 0.5;
		break;
		case -1:
		dy -= GetSpeed()*(((absorb | reflect) ? -1 : 1));
		power -= 0.5;
		break;
		case 0:
		break;
		}
		}
		if (SceneManager::GetSceneManager().GetNowTurn() == GetType())
		{
		if (!turninit)
		{
		MyTurnInit();
		turninit = true;
		}
		if (power < 1)
		{
		if (!ObjectPool::GetObjPool().ExistMySkill(GetType()))
		{
		SceneManager::GetSceneManager().EndTurn();
		turninit = false;
		}
		}
		else
		{
		//攻击模块
		if (powerdelay <= 0 || !GetAim())
		{
		usingpower = ObjectPool::GetObjPool().IsNeedToShoot(this);
		powerdelay = hge->Random_Int(25, 50);
		}
		else
		powerdelay--;
		if (usingpower)
		{
		selectitem = 0;
		int needrest = 0;
		for (int i = 0; i < 3; i++)
		if (items[i])
		{
		if (power >= items[i]->GetDMG())
		selectitem = i + 1;
		else
		needrest++;
		}
		if (needrest != 3)
		{
		if (!selectitem)
		{
		if (ObjectPool::GetObjPool().CountForEnemyUnit(GetType()) > 12 && power >= 50)
		PowerSystem();
		else
		BulletShoot();
		}
		else
		{
		GameObject *bullet = new Skill(this, items[selectitem - 1]->GetInnerCode() + 1);
		ObjectPool::GetObjPool().InsertObject(bullet, true);
		power -= items[selectitem - 1]->GetDMG();
		//items[selectitem - 1]->Suicide();
		//items[selectitem - 1] = NULL;
		}
		}
		else
		usingpower = false;
		}
		//更换目标模块
		if (!changetargettime)
		{
		SetAim(NULL);
		changetargettime = hge->Random_Int(400, 600);
		}
		else
		changetargettime--;
		}
		}
		GetAngle(GetAim());



		if (shootdelay)
		feelweak = true;
		else
		feelweak = false;
		*/
	}
	dy *= friction;
	dx *= friction;
	for (int i = 0; i < STD_CORES; i++)
	if (COREs[i])
	{
		if (COREs[i]->GetType() != GetType() && COREs[i]->GetSpeed())
		{
			if (COREs[i]->GetDistance(SCREEN_WIDTH*(0.75 + 0.05*i), 75)>20)
				COREs[i]->GoToAnotherPos(SCREEN_WIDTH*(0.75 + 0.05*i), 75);
			else
			{
				COREs[i]->SetPos(SCREEN_WIDTH*(0.75 + 0.05*i), 75);
				COREs[i]->SetSpeed(0);
				COREs[i]->SetType(GetType());
				dynamic_cast<Robot*>(COREs[i])->SetHost(this);
			}
		}
	}
	float finaly = GetY() + dy*GetSpeedRate();
	float finalx = GetX() + dx*GetSpeedRate();
	if (finalx<GetActStartX() || finalx>GetActEndX())
		dx = -dx;
	if (finaly<GetActStartY() || finaly>GetActEndY())
		dy = -dy;
	SetPos(finalx, finaly);
	if (skilldelay > 0)
		skilldelay--;
	else
		skilldelay = 0;
	if (healhpstart)
	{
		healhp++;
		if (healhp == GAME_FPS*5)
		{
			VaryHP(1);
			healhp = 0;
		}
	}
	if (healmpstart)
	{
		healmp++;
		if (healmp == GAME_FPS*5)
		{
			GetCrystal(1);
			healmp = 0;
		}
	}
	if (getshieldstart)
	{
		getshield++;
		if (getshield == GAME_FPS*5)
		{
			if (!GetShield())
				VaryShield(1);
			getshield = 0;
		}
	}
	if (shootdelay > 0)
		shootdelay -= 1;
	else
		shootdelay = 0;
	if (IsGod)
	{
		if (GodTimer > 0)
			GodTimer--;
		else
		{
			IsGod = false;
			//SetCollisionRange(STD_COLLISIONRANGE);
		}
	}
	//if (recoverpower<0.1)
	//recoverpower += 0.0005;
	if (power < POWER_TO_CRYSTAL)
		power += 0.01;
	else
	{
		if (MP < MAX_CRYSTAL)
		{
			GetCrystal(1);
			power = 0;
		}
		else
			power = POWER_TO_CRYSTAL;
	}
	if (speedrate < 1.0f)
		speedrate += 0.05;
	else
		speedrate = 1.0f;
	//if (bulletcost>0)
	//	bulletcost -= 0.05;
	SetSpeedRate(speedrate);
}


void Hero::Interaction(GameObject *target)
{
	GameObject::Interaction(target);
	bool finded = false;
	if (target->IsCORE() && target->GetType() == None)
	{
		for (int i = 0; i < STD_CORES; i++)
		{
			if (!COREs[i])
			{
				COREs[i] = target;
				target->SetSpeed(800);
				finded = true;
				break;
			}
		}
			if (!finded)
				target->Interaction(this);
			return;
	}
	if (!target->IsDead())
		switch (target->GetInnerCode())
	{
		case CORE_ATK:
			extraDMG = 2;
			break;
		case CORE_SPEED:
			SetSpeed(GetSpeed()*2);
			break;
		case CORE_GETBLOOD:
			KillsHeal = true;
			killshealcount = 0;
			break;
		case CORE_HARDSKIN:
			hardskin = true;
			break;
		case CORE_SHOOTCD:
			shootcd = 3;
			break;
		case CORE_HEALHP:
			healhpstart = true;
			break;
		case CORE_HEALMP:
			healmpstart = true;
			break;
		case CORE_SHIELD:
			getshield = true;
			break;
		case CORE_GETMP:
			GetCrystal(5);
			break;
		case CORE_UPDATE:
			for (int i = 0; i < STD_CORES; i++)
			{
				Robot *obj = dynamic_cast<Robot*>(COREs[i]);
				if (obj->IsActive() && obj->GetCoreType() == Attack)
				{
					obj->SetDMG(GetDMG() * 2);
					obj->SetShootCD(obj->GetShootCD() / 2);
				}
			}
			break;

	}
	else
	{
		switch (target->GetInnerCode())
		{
		case CORE_ATK:
			extraDMG = 1;
			break;
		case CORE_SPEED:
			SetSpeed(GetSpeed() / 2);
			break;
		case CORE_GETBLOOD:
			KillsHeal = false; 
			killshealcount = 0;
			break;
		case CORE_HARDSKIN:
			hardskin = false;
			break;
		case CORE_SHOOTCD:
			shootcd = STD_SHOOTCD;
			break;
		case CORE_HEALHP:
			healhpstart = false;
			break;
		case CORE_HEALMP:
			healmpstart = false;
			break;
		case CORE_SHIELD:
			getshield = false;
			break;
		case CORE_UPDATE:
			for (int i = 0; i < STD_CORES; i++)
			{
				Robot *obj = dynamic_cast<Robot*>(COREs[i]);
				if (obj->IsActive() && obj->GetCoreType() == Attack)
				{
					obj->SetDMG(GetDMG() / 2);
					obj->SetShootCD(obj->GetShootCD() * 2);
				}
			}
			break;

		}
	}
	if (target->IsEnemy())
	{
		if (!target->IsTrueSuicide())
		{
			//Exp += target->GetShootCD();
			if (KillsHeal)
			{
				killshealcount++;
				if (killshealcount == 3)
				{
					killshealcount = 0;
					VaryHP(1);
				}
			}
			power += target->GetShootCD();
		}
	}
	//LevelSystem();
}



Hero::~Hero()
{
}

bool Hero::VaryHP_Obj(GameObject *obj)
{
	int vary = obj->GetDMG(true);
	if (!IsGod)
	{
		int varied = DMGDispose(vary);
		if (varied < 0)
		{
			GameObject::VaryHP(varied);
			IsGod = true;
			GodTimer = 100;
			speedrate = 0.3;
			//SetCollisionRange(0);
			if (!obj->IsSkill())
			{
				float angle = GetAngle(obj, true) - 1.57;
				float angle2 = obj->GetAngle(this,true);
				float d = GetCollisionRange() + obj->GetCollisionRange() - GetDistance(obj);
				obj->AddDxDy(-(cos(angle2) - sin(angle2))*d*1.2, -(cos(angle2) + sin(angle2))*d*1.2);
				dx += sin(angle) * 10;
				dy += cos(angle) * 10;
				//AddDxDy((cos(angle) - sin(angle)) * 15, (cos(angle) + sin(angle)) * 15);
			}
		}
		else
		{
			if (varied == 0)
				FontPool::GetFontPool().InsertFont(GetX(), GetY(), 0, Dodge, NeedRenderFont());
			else
				GameObject::VaryHP(varied);
		}
		return true;
	}
	else
		return false;
}



bool Hero::VaryHP(int vary)
{
   	if (!IsGod)
	{
		int varied = DMGDispose(vary);
		if (varied < 0)
		{
			GameObject::VaryHP(varied);
			IsGod = true;
			GodTimer = 100;
		}
		else
		{
			if (varied == 0)
				FontPool::GetFontPool().InsertFont(GetX(), GetY(), 0, Dodge, NeedRenderFont());
			else
			{
				GameObject::VaryHP(varied);
			}
		}
		return true;
	}
	else
		return false;
}



int Hero::DMGDispose(int DMG)
{
	if (DMG < 0)
	{
			if (!absorb)
			{
				if (!GetShield())
				{
					if (hardskin)
						return -1;
					else
						return DMG*(armorbreaktimer ? AB : 1);
				}
				else
				{
					VaryShield(-1);
					return 0;
				}
			}
			else
				return -DMG;
	}
	else
	{
		return DMG;
	}
};

void Hero::Interaction2(GameObject *skill)
{
	if (suckblood)
		VaryHP(skill->GetDMG());
};



void Hero::PowerSystem()
{
	if (MP>=1)
	{
		recoverpower = 0;
		int type = 0;
		if (!heroskill)
		{
			switch (bulletmode)
			{
			case Bomb:
				type = TONS_OF_BBULLET;
				break;
			case ArmorBreak:
				type = TONS_OF_ABBULLET;
				break;
			case EMP:
				type = TONS_OF_EMPBULLET;
				break;
			case Toxic:
				type = TONS_OF_TBULLET;
				break;
			case Weak:
				type = TONS_OF_WBULLET;
				break;
			}
		}
		else
		{
			skilltimes -= 1;
			type = heroskill;
			if (!skilltimes)
				heroskill = 0;
		}
		if (type)
		{
			Skill *bullet = new Skill(this, type);
			ObjectPool::GetObjPool().InsertObject(bullet, true);
			MP -= 1;
		}
	}
}
void Hero::BulletShoot()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	if ((uzimode?true:!shootdelay))
	{
		//for (int i = 0; i < 3; i++)
		//{
			//GameObject *bullet = new Skill(this, BULLET);
			GameObject *bullet1 = new Skill(this, BULLET);
			if (GetRot() == 0)
			{
				bullet1->SetRot(GetRot() + (dx / MAX_SPEED)*M_PI_4/4);
				bullet1->SetSpeed(bullet1->GetSpeed() + abs(dy * 3));
			}
			else
			if (GetRot() == M_PI)
			{
				bullet1->SetRot(GetRot() - (dx / MAX_SPEED)*M_PI_4/4);
				bullet1->SetSpeed(bullet1->GetSpeed() + abs(dy * 3));
			}
			else
			if (GetRot() == M_PI / 2)
			{
				bullet1->SetRot(GetRot() + (dy / MAX_SPEED)*M_PI_4/4);
				bullet1->SetSpeed(bullet1->GetSpeed() + abs(dx * 3));
			}
			else
			{
				bullet1->SetRot(GetRot() - (dy / MAX_SPEED)*M_PI_4/4);
				bullet1->SetSpeed(bullet1->GetSpeed() + abs(dx * 3));
			}
			//double delta = GetAngle(GetX() + dx, GetY() + dy,true);
			//bullet1->SetRot(GetRot() + (delta - GetRot())*0.05);
			//float rot = GetRot() - M_PI/2;
			//if (bulletpos)
				//bullet->SetPos(GetX() + 10 * (cos(rot) - sin(rot)), GetY() + 10 * (cos(rot) + sin(rot)));
			//else
				//bullet1->SetPos(GetX() + 10 * (cos(rot) + sin(rot)), GetY() - 10 * (cos(rot) - sin(rot)));
				//bullet->Born();
				bullet1->Born();
			//bullet->SetRot(GetRot() - 0.02 + 0.02*i);
			//ObjectPool::GetObjPool().InsertObject(bullet, true);
			ObjectPool::GetObjPool().InsertObject(bullet1, true);
			//if (!uzimode)
			//{
				//power -= 1;
				//recoverpower = 0;
			//}
		//}
		shootdelay = shootcd;
		//bulletpos = !bulletpos;
	}
}

void Hero::InitBlocks()
{
	
}

void Hero::InitSkill()
{
	//HGE *hge = hgeCreate(HGE_VERSION);
	//Item *item = new Item(NULL, hge->Random_Int(0, 1) ? 100 + hge->Random_Int(0, 8) * 2 : 24 + hge->Random_Int(0, 17) * 2);
	//item->Interaction(this);
	//items[0] = item;
	//item->SetPos((GetType() == Left ? ACT_START_X + 50 : ACT_END_X - 50 * 3), ACT_END_Y - 100);
	//ObjectPool::GetObjPool().InsertObject(item, true);
}

void Hero::MyTurnInit()
{
}



void Hero::LevelSystem()
{
	if (LV < 10)
	{
		if (Exp >= MaxExp)
		{
			Exp = 0;
			LV++;
			MaxExp *= 1.5;
			//shootcd--;
			if (LV == 10)
			{
				SetMaxHP(GetMaxHP() * 2);
				VaryHP(GetMaxHP() / 2);
				Exp = MaxExp;
			}
			else
			{
				SetMaxHP(GetMaxHP()+LV*20);
				VaryHP(LV * 20);
			}
		}
	}
}


void Hero::Stop()
{
	GameObject::Stop();
	/*
	if (dx > 2)
		dx = 2;
	if (dx<-2)
		dx = -2;
	if (dy>2)
		dy = 2;
	if (dy<-2)
		dy = 2;
	HGE *hge = hgeCreate(HGE_VERSION);
	if (hge->Input_GetKeyState(HGEK_W))
	if (dy>-2)
		dy -= 0.05;
	if (hge->Input_GetKeyState(HGEK_S))
	if (dy<2)
		dy += 0.05;
	if (hge->Input_GetKeyState(HGEK_A))
	if (dx>-2)
		dx -= 0.05;
	if (hge->Input_GetKeyState(HGEK_D))
	if (dx<2)
		dx += 0.05;
	float finaly = GetY() + dy;
	float finalx = GetX() + dx;
	if (finalx<GetActStartX() || finalx>GetActEndX())
		dx = -dx;
	if (finaly<GetActStartY() || finaly>GetActEndY())
		dy = -dy;
	SetPos(finalx, finaly);
	if (speedrate < 1.0f)
		speedrate += 0.05;
	SetSpeedRate(speedrate);
	*/
}



/*
void Hero::PowerSystem()
{
	if (!selectitem)
	counttime++;
	if (counttime >= 10)
	{
		if (selectitem <= 0 || !items[selectitem - 1])
		{
			if (power >= 2)
			{
				switch (bulletmode)
				{
				case UZI:
					if (counttime % 2 == 0)
					{
						GameObject *bullet = new Skill(this, BULLET3);
						ObjectPool::GetObjPool().InsertObject(bullet, true);
					}
					break;
				default:
					extraDMG++;
					break;
				}
				power -= 2;
			}
		}
	}
}
*/
/*
void Hero::BulletShoot()
{
	if (!shootdelay || counttime>=10)
	{
		if (counttime < 10)
		{
			int type=BULLET;
			switch (bulletmode)
			{
			case UZI:
				type = BULLET2;
			    break;
			case ArmorBreak:
				type = ABBULLET;
				break;
			case Magic:
				type = MBULLET;
				break;
			case EMP:
				type = EMPBULLET;
				break;
			case Toxic:
				type = TBULLET;
				break;
			case Weak:
				type = WBULLET;
				break;
			}
			GameObject *bullet = new Skill(this, type);
			ObjectPool::GetObjPool().InsertObject(bullet, true);
		}
		else
		{
			int type = 0;
			switch (bulletmode)
			{
			case Bomb:
				type = BULLET;
				break;
			case UZI:
				type = 0;
				break;
			case ArmorBreak:
				type = ABBULLET;
				break;
			case Magic:
				type = TONS_OF_MBULLET;
				break;
			case EMP:
				type = EMPBULLET;
				break;
			case Toxic:
				type = TBULLET;
				break;
			case Weak:
				type = WBULLET;
				break;
			}
			if (type)
			{
				Skill *bullet =new Skill(this, type);
				bullet->SetSize(GetSize() + 0.5*((float)GetDMG() / 65));
				ObjectPool::GetObjPool().InsertObject(bullet, true);
			}
		}
		shootdelay = shootcd;
	}
}
*/