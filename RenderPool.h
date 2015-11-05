#pragma once
#include <time.h>
#include <algorithm>
#include <vector>
#include "hgefont.h"
#include "ImageObject.h"
#include "AnimObject.h"
#include "hgeparticle.h"
#include "ObjectPool.h"
#ifndef RENDERPOOL_H_
#define RENDERPOOL_H_

using namespace std;


enum RoomType
{
	Empty,Banned,SpawnPoint,Normal,Core,Store,Boss
};

struct RoomData
{
	bool Cleaned;
	RoomType type;
	//道具房：道具名称
	int Customdata;
};



struct ParticleWithHost
{
	hgeParticleSystem* sys;
	GameObject *obj;
};

struct AnimWithHost
{
	AnimObject* anim;
	GameObject* host;
};

struct box
{
	int imageID;
	int ObjectID;
};
struct pos
{
	float posx, posy;
	int ID;
	float angle;
	float size;
	float changedFPS;
	GameObject *host;
};

class RenderPool
{
public:
	friend class ObjectPool;
	~RenderPool();
	static RenderPool& GetRenderPool()
	{
		static RenderPool pool;
		pool.AssureInitialized();
		return pool;
	}
	void RenderAnimations();
	void InsertImageV2(const char* filename, float centerx, float centery, int IDforObj);
	void InsertAnimV2(const char* animname, int frames, int fps, float w, float h, float cx, float cy, int IDforObj, bool isinfinte=false,bool isdeadanim=false);
	void InsertParticleV2(const char* psi, const char *pic, int ID, bool  needchangeemission = false);
	void InsertMazePart(const char* filename, int partnum);
	void SpawnPS(GameObject *obj,int ID);
	void SetCombination(int IDforImage,int IDforObject,bool isanim=false,int IDforAnim=0);
	void SetCombinationV2(int IDforObject, int IDforObjectnew,bool isanim=false);
	void SetParticleCombV2(int IDforObject, int IDforObjectnew);
	void RenderEveryObject();
	void RenderParticles();
	void RenderMap();
	void InsertImage(ImageObject *img);
	void InsertAnim(AnimObject *anim);
	void RenderAfterBlur();
	void RenderHero(Hero *hero);
	void InsertDeadAnim(AnimObject *anim);
	void RenderDead();
	void Shuffle();
	void RenderTokens();
	void RenderOnEveryObject();
	void InstantRender(float x, float y, int ID, float angle = 0.0f, float size = 1.0f, GameObject* host = nullptr, float changeFPS=0);
	void AssureInitialized()
	{
		while (!m_bHasInitialized)
			Sleep(0);
	};
	void ResetAll()
	{
		Instantpool.clear();
		Instantpos.clear();
	};
	void InitMap()
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		for (int i = 0; i < 11; i++)
		for (int j = 0; j < 5; j++)
		{
			map[i][j].Cleaned = false;
			map[i][j].type = Empty;
			map[i][j].Customdata = 0;
		}
		//分配出生点：
		int ran1 = hge->Random_Int(0, 10), ran2 = hge->Random_Int(0, 4);
		map[ran1][ran2].Cleaned = true;
		map[ran1][ran2].type = SpawnPoint;
		nowx = ran1;
		nowy = ran2;
		int ran3 = hge->Random_Int(0, 10), ran4 = hge->Random_Int(0, 4);
		//分配Boss：
		while (1)
		{
			if (map[ran3][ran4].type == Empty)
			{
				map[ran3][ran4].type = Boss;
				break;
			}
			else
			{
				ran3 = hge->Random_Int(0, 10);
				ran4 = hge->Random_Int(0, 4);
			}
		}
		//分配Core：
		int count = 0;
		while (1)
		{
			int ran5 = hge->Random_Int(0, 10), ran6 = hge->Random_Int(0, 4);
			if (map[ran5][ran6].type == Empty)
			{
				map[ran5][ran6].type = Core;
				count++;
				if (count == 10)
					break;
			}
		}
		//分配商店：
		count = 0;
		while (1)
		{
			HGE *hge = hgeCreate(HGE_VERSION);
			int ran5 = hge->Random_Int(0, 10), ran6 = hge->Random_Int(0, 4);
			if (map[ran5][ran6].type == Empty)
			{
				map[ran5][ran6].type = Store;
				map[ran5][ran6].Customdata = (hge->Random_Int(0, 5) ? hge->Random_Int(ITEM_START, ITEM_END) : hge->Random_Int(CORE_START, CORE_END));
				count++;
				if (count == 5)
					break;
			}
		}
		//分配禁用房间：
		//for (int i = 0; i < 11;i++)
		//for (int j = 0; j < 5; j++)
		//{
			//if ((abs(i - ran3)>5 || abs(i - ran1)>5) && (abs(j - ran4) > 2 || abs(j - ran2) > 2))
			//{
			//	map[i][j].type = Banned;
			//}
		//}
		for (int i = 0; i < 11; i++)
		for (int j = 0; j < 5; j++)
		{
			if (map[i][j].type == Empty)
				map[i][j].type = Normal;
		}
	}

	void CleanNowRoomData()
	{
		map[nowx][nowy].Customdata = 0;
	}

	RoomData GetNowRoom()
	{
		return map[nowx][nowy];
	}
	void CleanNowRoom()
	{
		map[nowx][nowy].Cleaned = true;
	}
	void MoveNowRoom(int dx, int dy)
	{
		int fx = nowx + dx, fy = nowy + dy;
		if (fx > 10)
			fx = 10;
		if (fx < 0)
			fx = 0;
		if (fy > 4)
			fy = 4;
		if (fy < 0)
			fy = 0;
		if (map[nowx][nowy].Cleaned)
		{
			nowx = fx;
			nowy = fy;
		}
		else
		{
			if (map[fx][fy].Cleaned)
			{
				nowx = fx;
				nowy = fy;
			}
		}
	}
	void EnterNowRoom()
	{
		SceneManager::GetSceneManager().SetGameMode(MazeEvent);
	}


	void Exit()
	{
		for (int i = 0; i < Instantpool.size(); i++)
			Instantpool[i].anim->ForceToDie();
		for (int i = 0; i < pool.size(); i++)
		{
			pool[i]->ForceToDie();
			delete pool[i];
			pool[i] = NULL;
		}
		for (int i = 0; i < anims.size(); i++)
		{
			anims[i]->ForceToDie();
			delete anims[i];
			anims[i] = NULL;
		}
		for (int i = 0; i < DeadAnimpool.size(); i++)
		{
			DeadAnimpool[i]->ForceToDie();
			delete DeadAnimpool[i];
			DeadAnimpool[i] = NULL;
		}
		for (int i = 0; i < particlepool.size(); i++)
		{
			delete particlepool[i].sys;
			particlepool[i].obj = NULL;
		}
		vector<ImageObject*>().swap(pool);
		vector<AnimObject*>().swap(anims);
		vector<AnimWithHost>().swap(Instantpool);
		vector<AnimObject*>().swap(DeadAnimpool);
		vector<pos>().swap(Instantpos);
		vector<box>().swap(combs);
		vector<box>().swap(animcombs);
		vector<box>().swap(particlecombs);
		vector<ParticleWithHost>().swap(particlepool);
		vector<hgeParticleSystemInfo>().swap(particles);
		delete font;
	}

private:
	RenderPool();
	RenderPool(const RenderPool &);
	RenderPool & operator=(const RenderPool &);
	vector<ImageObject*> pool;
	vector<AnimObject*> anims;
	vector<AnimWithHost> Instantpool;
	vector<AnimObject*> DeadAnimpool;
	vector<pos> Instantpos;
	vector<box> combs;
	vector<box> animcombs;
	vector<box> particlecombs;
	vector<ParticleWithHost> particlepool;
	vector<hgeParticleSystemInfo> particles;
	RoomData map[11][5];
	bool m_bHasInitialized;
	void RenderOne(GameObject *vit, ImageObject *img);
	void RenderOne(GameObject *vit, AnimObject *anim);
	hgeFont *font;
	int nowx, nowy;
};

#endif