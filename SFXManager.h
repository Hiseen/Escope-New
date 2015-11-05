#pragma once
#include "SFXObject.h"
#include <vector>
using namespace std;
class SFXManager
{
public:
	~SFXManager();
	static SFXManager& GetSFXManager()
	{
		static SFXManager ObjPool;
		ObjPool.AssureInitialized();
		return ObjPool;
	}
	void InsertSFX(const char *filename, int ID, bool issfx);
	void PlaySFX(int number,bool changepan=false);
	void SetGlobalSfxVol(int newvol)
	{
		MaxVolForSfx = newvol;
	}
	void SetGlobalMusicVol(int newvol)
	{
		MaxVolForMusic = newvol;
	}
	int GetGlobalMusicVol()const{ return MaxVolForMusic; };
	int GetGlobalSfxVol()const{ return MaxVolForSfx; };
	void UpdatePool();
	void Exit()
	{
		for (int i = 0; i < sfxpool.size(); i++)
		{
			delete sfxpool[i];
			sfxpool[i] = NULL;
		}
		vector<SFXObject*>().swap(sfxpool);
	}
private:
	vector<SFXObject*> sfxpool;
	void AssureInitialized()
	{
		while (!m_bHasInitialized)
			Sleep(0);
	};
	SFXManager()
	{
		MaxVolForMusic = 20;
		MaxVolForSfx = 100;
		symbol = false;
		clean = true;
		m_bHasInitialized = true;
	};
	bool m_bHasInitialized;
	bool clean;
	bool symbol;
	int MaxVolForSfx;
	int MaxVolForMusic;
};

