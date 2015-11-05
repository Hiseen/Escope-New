#include "ObjectPool.h"
#include <algorithm>
#include "Skill.h"
ObjectPool::ObjectPool()
{
	focusskill = 0;
	global = 0;
	count = 0;
	testcount = 0;
	countskilleffect = 25;
	reverse = false;
	countfordie = 0;
	m_bHasInitialized = true;
	//主角拥有7个技能：
}


ObjectPool::~ObjectPool()
{
}

void ObjectPool::CreatePool()
{
	pool.clear();
}

bool ObjectPool::InsertObject(GameObject *vir,bool delay)
{
	if (!vir->IsDead())
	{
		if (delay)
		{
			instantpool.push_back(vir);
		}
		else
		{
			pool.push_back(vir);
			swap(pool.begin(), pool.end());
		}
		return true;
	}
	delete vir;
	vir = NULL;
	return false;
}

void ObjectPool::DelectObject()
{
	bool deleted = false;
	for (int i = 0; i < pool.size(); i++)
	{
		pool[i]->SafeOperation();
	}
	for (int i = 0; i < pool.size(); i++)
	{
		if (IsDead(pool[i]))
		{
			delete pool[i];
			pool[i] = NULL;
			deleted = true;
		}
	}
	for (int i = 0; i < pool.size();)
	{
		if (!pool[i])
			pool.erase(pool.begin() + i);
		else
			i++;
	}
	if (deleted)
	pool.shrink_to_fit();
}

void ObjectPool::UpdateObject()
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->GetSceneNumber() == SceneManager::GetSceneManager().GetCurrentScene())
			ObjectRun(pool[i]);
	}
	for (int i = pool.size()-1; i >=0; i--)
	{
		if (pool[i]->IsBlock() && !pool[i]->IsDead() && pool[i]->GetCollisionRange()>0)
		{
			int colrange = pool[i]->GetCollisionRange()-1;
			for (int j = 0; j < pool.size(); j++)
			{
				if (pool[i] != pool[j])
				{
					int colrange2 = pool[j]->GetCollisionRange() - 1;
					double dis = pool[i]->GetDistance(pool[j]);
					if (dis < colrange + colrange2)
					{
						if (pool[j]->IsBlock() && !pool[j]->IsDead() && !pool[j]->IsSoild() && pool[j]->GetCollisionRange()>0)
						{
							float angle = pool[j]->GetAngle(pool[i], true);
							pool[j]->SetPos(pool[j]->GetX() + (cos(angle) - sin(angle))*(colrange + colrange2 - dis), pool[j]->GetY() + (cos(angle) + sin(angle))*(colrange + colrange2 - dis));
							//pool[j]->SetRot(pool[i]->GetAngle(pool[j], true));
							//pool[i]->SetRot(angle);
						}
						if (pool[i]->IsSkill() && pool[j]->IsSkill() && !pool[j]->IsBlock())
						{
							pool[i]->VaryHP_Obj(pool[j]);
							pool[j]->Suicide();
						}
						if (pool[i]->IsCORE() && pool[j]->IsEnemy() && pool[j]->IsSoild())
							pool[i]->Suicide();
					}

				}
			}
		}
	}
	while (instantpool.size()>0)
	{
			InsertObject(instantpool[0]);
			instantpool.erase(instantpool.begin());
	}
	//instantpool.clear();
}

void ObjectPool::SkillEffect()
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->IsSkill() &&!pool[i]->IsDead() && pool[i]->GetCollisionRange()>0)
			LineEffect(pool[i]);
		pool[i]->AfterOneEffect();
		if (pool[i]->GetInnerCode() == HERO)
		{
			for (int j = 0; j < pool.size();j++)
			if (pool[j]->GetCollisionRange()>0 && pool[i]->GetCollisionRange()>0 && pool[i]->GetDistance(pool[j]) < pool[j]->GetCollisionRange() + pool[i]->GetCollisionRange() && pool[j]->IsEnemy())
			{
				pool[i]->VaryHP_Obj(pool[j]);
				pool[j]->Interaction(pool[i]);
			}
		}
	}
}

void ObjectPool::EndOfWaveClean()
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (!pool[i]->IsCORE() && pool[i]->GetInnerCode() != HERO)
			 pool[i]->avlid=false;
	}
}



void ObjectPool::LockOnGameObject(GameObject *obj)
{
	for (int i = 0; i < pool.size(); i++)
	if (pool[i]!=obj)
		pool[i]->SetStopTime(1);
}


void ObjectPool::ExceptClean()
{
	for (int i = 0; i < pool.size(); i++)
	if (pool[i]->GetSceneNumber() != SceneManager::GetSceneManager().GetCurrentScene())
		pool[i]->Suicide();
}

int ObjectPool::IsUPBiggerThanDown(GameObject *host)
{
	/*
	int num1 = 0, num2 = 0, num3 = 0;
	ObjectType myType = host->GetType();
	float y = host->GetY();
	float x = host->GetX();
	if (host->GetShootCD() < 40)
	{
		for (int i = 0; i < pool.size(); i++)
		if (!pool[i]->IsDead() &&pool[i]->GetInnerCode() == MPRECOVER && pool[i]->GetType() == myType)
		{
			if (pool[i]->GetY()>y)
				return 1;
			else
				return -1;
		}

	}


	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->IsSkill())
		{
			if (pool[i]->GetType() != myType)
			{
				int mid = pool[i]->GetY() + (abs(pool[i]->GetX() - x))*sin(pool[i]->GetRot() - 1.57);
				if (mid > ACT_START_Y + 80 && mid < ACT_END_Y - 80)
				{
					if (abs(mid - y) <= pool[i]->GetCollisionRange()*4)
					{
						if (mid < y)
							num1++;
						else
							num2++;
					}
					else
						num3++;
				}
				else
				{
					if (mid >= ACT_END_Y - 80)
						num2++;
					else
						num1++;
					num3 = 1;
				}
			}
		}
	}
	//if (y>ACT_START_Y - 50 && y<ACT_END_Y - 50)
	//{
		if (num1 && num2 && !num3)
			return 2;
		if (num1>num2)
			return 1;
		else
		if (num1 == num2)
			return 0;
		else
			return -1;
	//}
	//else
	//{
	//	if (y <= ACT_START_Y - 50)
	//		return 1;
	//	else
	//		return -1;
	//}
	*/
	return 0;
}


bool ObjectPool::IsNeedToShoot(GameObject *me)
{
	//包含角度转换：
	//优先攻击次序：威胁大的飞行物>威胁大的机器人>飞行物>机器人>道具>敌方英雄
	bool flag = true;
	ObjectType type = me->GetType();
	int rank = 0;
	GameObject *aim = me->GetAim();
		if (aim)
		if (aim->GetDMG()>me->GetHP()*0.2)
			rank = 5;
		else
		if (aim->IsCORE())
			rank = 4;
		else
		if (aim->IsItem())
			rank = 3;
		else
		if (aim->GetInnerCode() == HERO)
			rank = 2;
		if (rank < 5)
		{
			for (int i = 0; i < pool.size(); i++)
			{
				if (pool[i]->GetType() != type && pool[i]->IsReachable())
				{
					if (pool[i]->GetDMG()>me->GetHP()*0.2)
					{
						me->SetAim(pool[i]);
						rank = 5;
					}
					else
					if (rank < 4 && pool[i]->IsCORE())
					{
						me->SetAim(pool[i]);
						rank = 4;
					}
					else
					if (rank < 3 && pool[i]->IsItem())
					{
						me->SetAim(pool[i]);
						rank = 3;
					}
					else
					if (rank < 2 && pool[i]->GetInnerCode() == HERO)
					{
						me->SetAim(pool[i]);
						rank = 2;
					}

				}
			}
		}
		if (me->GetShootCD() >= 80)
			flag = true;
		else
		{
			HGE *hge = hgeCreate(HGE_VERSION);
			float times = 0.2;
			GameObject *a = me->GetAim();
			if (a->GetDMG() > me->GetHP()*0.2)
				flag = true;
			else
			if (a->IsCORE())
				times = 1.5;
			else
			if (a->IsItem())
				times = 1.2;
			if (a->GetHP() < a->GetMaxHP()*0.2)
				times += 1;
			if (hge->Random_Int(0, 100) < me->GetShootCD()*times/2)
				flag = true;
			else
				flag = false;
		}
	me->GetAngle(me->GetAim());
	return flag;
}

bool ObjectPool::IsEnemyHeroUpperThanMe(float y, ObjectType myType,bool feelweak,int hp)
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->GetInnerCode() == HERO && pool[i]->GetType() != myType)
		{
			if (hp < pool[i]->GetHP()*0.8)
				feelweak = true;
			if (!feelweak)
			if (pool[i]->GetY() < y)
				return true;
			else
				return false;
			else
			if (pool[i]->GetY()<ACT_END_Y - 20 && pool[i]->GetY()>ACT_START_Y + 20)
			{
				if (pool[i]->GetDy()>0)
					return true;
				else
					return false;
			}
			else
			{
				if (pool[i]->GetY() <= ACT_START_Y + 20)
					return false;
				else
					return true;
			}
		}
	}
	return false;
}


bool ObjectPool::IsTimeToAttackHero(float y, ObjectType myType)
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->GetInnerCode() == HERO && pool[i]->GetType() != myType)
		{
			if (abs(pool[i]->GetY()-y)<50)
				return true;
			else
				return false;
		}
	}
	return false;
}

GameObject* ObjectPool::GetRandomEnemyUnit(ObjectType myType)
{
	GameObject *obj = NULL;
	for (int i = 0; i < pool.size(); i++)
	if (pool[i]->GetType() != myType &&!pool[i]->IsSkill() && !pool[i]->IsDead() && pool[i]->IsReachable())
	{
		obj = pool[i];
		break;
	}
	if (obj)
	std::random_shuffle(pool.begin(), pool.end());
	return obj;
}

GameObject* ObjectPool::GetDistanceEnemies(GameObject *obj, int res_dis)
{
	GameObject *selectobj = nullptr;
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i] != obj && pool[i]->IsEnemy() && !pool[i]->IsDead() && pool[i]->IsReachable() && obj->GetDistance(pool[i]) < res_dis)
		{
			selectobj = pool[i];
			break;
		}
	}
	if (selectobj)
	std::random_shuffle(pool.begin(), pool.end());
	return selectobj;
}


bool ObjectPool::IsStageClear()
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->GetType()!=Left)
			return false;
	}
	return true;
}

GameObject* ObjectPool::IsTimeToCatchItem(float y)
{
	GameObject *obj = NULL;
	for (int i = 0; i < pool.size();i++)
	if (pool[i]->IsItem() && pool[i]->IsReachable())
		obj=pool[i];
	std::random_shuffle(pool.begin(), pool.end());
	return obj;
}

bool ObjectPool::ExistMySkill(ObjectType myType)
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->IsSkill() && pool[i]->GetDMG()>0 && pool[i]->GetSpeed() && pool[i]->IsReachable() && pool[i]->GetCollisionRange())
			return true;
	}
	return false;
}




GameObject* ObjectPool::GetEnemyHero(ObjectType myType)
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->GetType() != myType && pool[i]->GetInnerCode() == HERO)
			return pool[i];
	}
	return NULL;
}

GameObject* ObjectPool::GetOurHero(ObjectType myType)
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->GetType() == myType && pool[i]->GetInnerCode() == HERO)
			return pool[i];
	}
	return NULL;
}


void ObjectPool::LineEffect(GameObject* host)
{
	ObjectType type = host->GetType();
	float startx = host->GetX(), starty = host->GetY(), rot = host->GetRot()-1.57;
	int range = host->GetCollisionRange();
		for (int i = 0; i < pool.size(); i++)
		{
			float px = pool[i]->GetX();
			float py = pool[i]->GetY();
			if (!host->IsDead() && pool[i]->GetCollisionRange()>0 && pool[i] != host && !pool[i]->IsDead() && pool[i]->IsReachable() && pool[i]->GetType() != type)
			{
				int mid = starty + (abs(startx - px))*sin(rot);
				HGE *hge = hgeCreate(HGE_VERSION);
				if (pool[i]->GetInnerCode() != ENEMY_RECT)
				{
					if (py<mid + range + pool[i]->GetCollisionRange() && py>mid - range - pool[i]->GetCollisionRange())
					{
						if (!host->GetIsLineSkill())
						{
							if (host->GetSpeed()*host->GetSpeedRate()*hge->Timer_GetDelta() >= abs(px - startx) - range - pool[i]->GetCollisionRange() && host->GetDistance(pool[i])<pool[i]->GetCollisionRange() + range)
							{
								if (startx <= px)
								{
									host->Interaction(pool[i]);
									if (host->IsDead())
										host->SetPos(startx + (px - startx)*cos(rot), starty + (py - starty)*sin(rot));
								}
								else
								{
									host->Interaction(pool[i]);
									if (host->IsDead())
										host->SetPos(startx + (startx - px)*cos(rot), starty + (starty - py)*sin(rot));
								}
							}
						}
						else
							host->Interaction(pool[i]);
					}
				}
				else
				{
					//ENEMY_RECT 长宽比 1：7 
					if (py<mid + range + pool[i]->GetCollisionRange() * 7 && py>mid - range - pool[i]->GetCollisionRange() * 7)
					{
						if (!host->GetIsLineSkill())
						{
							if (host->GetSpeed()*host->GetSpeedRate()*hge->Timer_GetDelta() >= abs(px - startx) - range - pool[i]->GetCollisionRange())
							{
								if (startx <= px)
								{
									host->Interaction(pool[i]);
									if (host->IsDead())
										host->SetPos(startx + (px - startx)*cos(rot), starty + (py - starty)*sin(rot));
								}
								else
								{
									host->Interaction(pool[i]);
									if (host->IsDead())
										host->SetPos(startx + (startx - px)*cos(rot), starty + (starty - py)*sin(rot));
								}
							}
						}
						else
							host->Interaction(pool[i]);
						}
					}

			}
		}
}
void ObjectPool::EffectOnBullets(GameObject* skill)
{
	ObjectType a = skill->GetType();
	int innercode = skill->GetInnerCode();
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->GetType() != a && pool[i]->IsSkill() && !pool[i]->IsDead())
		{
			switch (innercode)
			{
			case CLEAN:
				pool[i]->Suicide();
				break;
			}
		}
	}
}

void ObjectPool::EffectOnEnemies(GameObject* skill)
{
	int innercode = skill->GetInnerCode();
	ObjectType a = skill->GetType();
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->GetType()!=a && !pool[i]->IsDead() && !pool[i]->IsCORE())
		{
			switch (innercode)
			{
			case ICE:
					pool[i]->SetStopTime(150);
				break;
			}
		}
	}
}


bool ObjectPool::IsTimeToUsePower(float y, ObjectType myType)
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->GetType() != myType)
		{
			if (pool[i]->GetInnerCode() == HERO && abs(pool[i]->GetY() - y) < 25 && pool[i]->GetSpeed() < STD_SPEED)
				return true;
			if (pool[i]->IsItem() && pool[i]->GetHP() <= pool[i]->GetMaxHP()*0.3)
				return true;
			int code = pool[i]->GetInnerCode();
			if (pool[i]->IsSkill() && pool[i]->GetDMG()>40)
				return true;
		}
	}
	return false;
}


int ObjectPool::CountForEnemyUnit(ObjectType myType)
{
	int count = 0;
	for (int i = 0; i<pool.size();i++)
	if (pool[i]->GetType() != myType)
		count++;
	return count;
}
