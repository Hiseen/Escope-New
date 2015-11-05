#pragma once

#include<vector>
#include "SceneManager.h"
#include "defines.h"
#include "Hero.h"
#ifndef OBJECTPOOL_H_
#define OBJECTPOOL_H_
extern void getRange(int *arr, int size, int *max, int *min);
extern bool Collision(Point *mPoint,const int count, const float angle5, float center_x, float center_y, float px, float py);
extern bool ballcrash(float center_x, float center_y, int r, float px, float py);

struct Posdata
{
	float x, y;
	bool vaild;
};

using namespace std;
class ObjectPool
{
public:
	friend class RenderPool;
	~ObjectPool();
	bool InsertObject(GameObject *vir,bool delay=false);
	void DelectObject();
	void UpdateObject();
	void CreatePool();
	void SkillEffect();
	void FlashObject();
	void CreateMaze();
	void GenerateEnemy();
	void LockOnGameObject(GameObject *obj);
	int IsUPBiggerThanDown(GameObject *host);
	bool ExistMySkill(ObjectType myType);
	GameObject* IsTimeToCatchItem(float y);
	GameObject* GetRandomEnemyUnit(ObjectType myType);
	void EffectOnEnemies(GameObject* skill);
	void EffectOnBullets(GameObject* skill);
	bool IsNeedToShoot(GameObject *me);
	bool IsEnemyHeroUpperThanMe(float y, ObjectType myType,bool feelweak,int hp);
	bool IsTimeToAttackHero(float y, ObjectType myType);
	bool IsTimeToUsePower(float y, ObjectType myType);
	bool IsStageClear();
	void LineEffect(GameObject *host);
	GameObject *GetEnemyHero(ObjectType myType);
	GameObject *GetOurHero(ObjectType myType);
	void ExceptClean();
	int CountForEnemyUnit(ObjectType myType);
	void SetBWbools(const char* filename, int NumOfObject);
	/*
	bool GetSpawning()
	{
		for (int i = 0; i < pool.size();i++)
		if (pool[i]->TellType == BLACKHOLE)
			return true;
		return false;
	}
	*/
	int GetAmountOfObjects()
	{
		return pool.size();
	}
	bool HeroCoreBack()
	{
		return dynamic_cast<Hero*>(GetOurHero(Left))->ForceAllCoreBack();
	}
	GameObject *GetDistanceEnemies(GameObject *obj, int res_dis);
	int GetAmountOfEnemies()
	{
		int count = 0;
		for (int i = 0; i < pool.size(); i++)
		{
			if (pool[i]->IsEnemy())
				count++;
		}
		return count;
	}
	float GetGlobalDelta()const{ return global*count; };
	vector<GameObject*> GetPool()
	{
		return pool;
	}
	void ResetTokens();
	void MazeEvent(int &x, int &y, int &eventname);
	static ObjectPool& GetObjPool()
	{
		static ObjectPool ObjPool;
		ObjPool.AssureInitialized();
		return ObjPool;
	}
	void EndOfWaveClean();
	void AssureInitialized()
	{
		while (!m_bHasInitialized)
			Sleep(0);
	};
	void ResetAll()
	{
		for (int i = 0; i < instantpool.size(); i++)
			InsertObject(instantpool[i]);
		//instantpool.clear();
		for (int i = 0; i < pool.size(); i++)
			pool[i]->avlid = false;
		ObjectPool::DelectObject();
		//pool.clear();
	};
	void Exit()
	{
		while (instantpool.size()>0)
		{
			InsertObject(instantpool[0]);
			instantpool.erase(instantpool.begin());
		}
		for (int i = 0; i < pool.size(); i++)
			pool[i]->avlid = false;
		ObjectPool::DelectObject();
		for (int i = 0; i < BWpics.size(); i++)
		{
			for (int j = 0; j < BWpics[i].width; j++)
				free(BWpics[i].bools[j]);
			free(BWpics[i].bools);
		}
		vector<GameObject*>().swap(pool);
		vector<GameObject*>().swap(instantpool);
		vector<BWpic>().swap(BWpics);
	}

private:
	ObjectPool();
	ObjectPool(const ObjectPool &);
	ObjectPool & operator=(const ObjectPool &);
	vector<GameObject*> pool; 
	vector<GameObject*> instantpool;
	vector<BWpic> BWpics;
	int focusskill;
	int testcount;
	int countfordie;
	float global;
	int countskilleffect;
	bool reverse;
	int count;
	bool m_bHasInitialized;
	//static ObjectPool *ObjPool;
	int getw(int x, int y);
	int search(int x, int y);
	void Make_Maze(int x, int y);
	bool MazeCollision(int x, int y,int &eventname,int &direction);
	bool IsDead(GameObject* vir)
	{
		if (!vir->avlid)
			return true;
		else
			return false;
	}
	void ObjectRun(GameObject* vir)
	{
		if (!vir->finishb)
			vir->Born();
		else
		{
			if (vir->IsDead())
				vir->Death();
			else
			{
				if (vir->GetStopTime())
					vir->Stop();
				else
				    vir->Action();
			}
		}
	}

	BWpic GetBWpic(int number)
	{
		for (int i = 0; i < BWpics.size();i++)
		if (BWpics[i].number == number)return BWpics[i];
		BWpic pic1;
		pic1.bools = nullptr;
		return pic1;
	}
};

#endif