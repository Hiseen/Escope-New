#include "Robot.h"
#include "Skill.h"
#include "RenderPool.h"
#include "FontPool.h"
Robot::~Robot()
{
}

void Robot::Action()
{
	switch (GetType())
	{
	case Left:
		if (!startattack)
		{
			if (!trigger)
			{
				if (coretype != Eternity)
				{
					HGE *hge = hgeCreate(HGE_VERSION);
					float x, y;
					hge->Input_GetMousePos(&x, &y);
					int dis = GetDistance(x, y);
					if (dis < 20)
					{
						if (hge->Input_KeyDown(HGEK_LBUTTON))
						{
							trigger = true;
							backx = GetX();
							backy = GetY();
						}
						else
						{
							if (hge->Input_KeyDown(HGEK_RBUTTON))
								Suicide(true);
						}
					}
				}
			}
			else
			{
				ObjectPool::GetObjPool().LockOnGameObject(this);
				if (runback)
				{
					if (GoToAnotherPos(backx,backy))
					{
						trigger = false;
						runback = false;
						SetSpeed(0);
					}
				}
				else
				{
					HGE *hge = hgeCreate(HGE_VERSION);
					float x, y;
					hge->Input_GetMousePos(&x, &y);
					SetPos(x, y);
					if (hge->Input_KeyUp(HGEK_LBUTTON))
					{
     					if (x<ACT_START_X || x>ACT_END_X || y<ACT_START_Y || y>ACT_END_Y)
						{
							runback = true;
							SetSpeed(1200);
						}
						else
						{
							switch (coretype)
							{
								//攻击型
								//开始攻击
							case Attack:
								SetCollisionRange(40);
								SetSolid(true);
								SetBlock(true);
								break;
								//触发技能型
								//跟hero做intercation将自身赋给hero然后自杀
							case Magic:
								dynamic_cast<Hero*>(ObjectPool::GetObjPool().GetOurHero(GetType()))->SetSkill(GetInnerCode() + 300);
								Suicide();
								break;
								//BUFF型
								//施放技能 
							case Buff:
							{
										 int count = 0;
										 Hero *a = dynamic_cast<Hero*>(ObjectPool::GetObjPool().GetOurHero(GetType()));
										 for (int i = 0; i < STD_CORES; i++)
										 {
											 Robot *b = dynamic_cast<Robot*>(a->GetCOREs(i));
											 if (b && b->IsActive())
											 {
												 if (b->GetInnerCode() == GetInnerCode())
												 {
													 b->Suicide();
													 a->Interaction(b);
												 }
												 else
												 if (b->GetCoreType() == Buff)
													 count++;
											 }

										 }
										 SetSpeed(1500);
										 SetAim(25+count * 50, 175);
							}
								break;
							}
							startattack = true;
						}
					}
				}
			}
		}
		else
		{
			switch (coretype)
			{
			case Attack:
				if (GetAim())
					GetAngle(GetAim());
				if (shootdelay)
					shootdelay--;
				else
				{
					if (!GetAim())
						SetAim(ObjectPool::GetObjPool().GetRandomEnemyUnit(GetType()));
					else
					{
						Skill *skill = new Skill(this, CORESKILL);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
					shootdelay = std_shootdelay;
				}
				break;
			case Buff:
				if (GoToAim())
				{
					if (!buffstart)
					{
						ObjectPool::GetObjPool().GetOurHero(GetType())->Interaction(this);
						buffstart = true;
					}
					if (shootdelay)
						shootdelay--;
					else
					{
						Suicide();
						if (host)
							host->Interaction(this);
					}
				}
				break;
			}
			HGE *hge = hgeCreate(HGE_VERSION);
			if (hge->Input_KeyDown(HGEK_RBUTTON))
			{
				float x, y;
				hge->Input_GetMousePos(&x, &y);
				if (GetDistance(backx, backy, x, y) < 20)
				{
					Suicide(true);
				}
			}
		}
		break;
	case None:
		if (!GetSpeed())
		{
			HGE *hge = hgeCreate(HGE_VERSION);
			float x, y;
			hge->Input_GetMousePos(&x, &y);
			int dis = GetDistance(x, y);
			if (dis < 50)
			{
				ObjectPool::GetObjPool().GetEnemyHero(GetType())->Scilence();
				if (dis < 40)
				{
					if (hge->Input_KeyDown(HGEK_LBUTTON))
						ObjectPool::GetObjPool().GetEnemyHero(GetType())->Interaction(this);
					else
					{
						if (hge->Input_KeyDown(HGEK_RBUTTON))
						{
							Suicide(true);
							tremble = 0;
						}
					}
				}
			}
			else
				ObjectPool::GetObjPool().GetEnemyHero(GetType())->UnScilence();
		}
		else
			ObjectPool::GetObjPool().GetEnemyHero(GetType())->UnScilence();
		break;
	}
	if (tremble)
	{
		tremble--;
		int a = tremble / 5;
		int b = tremble % 5;
		if (a % 2)
			SetX(GetX() + (3 - b) * 2);
		else
			SetX(GetX() - (3 - b) * 2);
	}
}

bool Robot::VaryHP_Obj(GameObject *obj)
{
	GameObject::VaryHP(-1);
	//if (!obj->IsSkill())
	//{
		//float angle2 = obj->GetAngle(this, true);
		//float d = GetCollisionRange() + obj->GetCollisionRange() - GetDistance(obj);
		//obj->AddDxDy(-(cos(angle2) - sin(angle2))*d*1.2, -(cos(angle2) + sin(angle2))*d*1.2);
//	}
	return true;
}

bool Robot::VaryHP(int vary)
{
	GameObject::VaryHP(-1);
		return true;
}


void Robot::Interaction(GameObject *target)
{
	switch (target->GetInnerCode())
	{
	case HERO:
		if (!tremble)
		tremble = 20;
		break;
	}
}
int Robot::DMGDispose(int vary)
{
	if (GetShield())
	{
		VaryShield(-1);
		godmode = true;
		godtimer = 250;
		return 0;
	}
	return GameObject::DMGDispose(vary);
}



void Robot::UseSkill()
{
	if (!startattack)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		float x, y;
		hge->Input_GetMousePos(&x, &y);
		backx = GetX();
		backy = GetY();
		switch (coretype)
		{
			//攻击型
			//开始攻击
		case Attack:
			if (x<ACT_START_X || x>ACT_END_X || y<ACT_START_Y || y>ACT_END_Y)
			{
				if (!tremble)
					tremble = 20;
				return;
			}
			else
			{
				SetCollisionRange(40);
				SetPos(x, y);
			}
			break;
			//触发技能型
			//跟hero做intercation将自身赋给hero然后自杀
		case Magic:
			break;
			//BUFF型
			//施放技能 
		case Buff:
		{
					 int count = 0;
					 Hero *a = dynamic_cast<Hero*>(ObjectPool::GetObjPool().GetOurHero(GetType()));
					 for (int i = 0; i < STD_CORES; i++)
					 {
						 Robot *b = dynamic_cast<Robot*>(a->GetCOREs(i));
						 if (b && b->IsActive())
						 {
							 if (b->GetInnerCode() == GetInnerCode())
							 {
								 b->Suicide();
								 a->Interaction(b);
							 }
							 else
							 if (b->GetCoreType() == Buff)
								 count++;
						 }

					 }
					 SetSpeed(1500);
					 SetAim(25 + count * 50, 175);
		}
			break;
		}
		startattack = true;
	}
}


void Robot::Death()
{
	if (tremble < 10)
		tremble++;
	else
	{
		GameObject *obj=ObjectPool::GetObjPool().GetEnemyHero(GetType());
		if (obj)
			obj->UnScilence();
		avlid = false;
	}
}