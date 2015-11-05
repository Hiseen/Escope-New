#pragma once
#include "DMGFont.h"
#include "hgesprite.h"
#include <vector>
class GameObject;
#ifndef FONTPOOL_H_
#define FONTPOOL_H_
using namespace std;
class FontPool
{
public:
	~FontPool();
	static FontPool& GetFontPool()
	{
		static FontPool ObjPool;
		ObjPool.AssureInitialized();
		return ObjPool;
	}
	void InsertFont(float x, float y, int dmg, Type type, bool Need = true);
	void InsertFont(GameObject *attackobj,int dmg, GameObject *fontobj, Type type);
	void RenderFont();
	void Delete();
	void ResetAll()
	{
		pool.clear();
		delaypool.clear();
	};
	void Exit()
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		for (int i = 0; i < fontpool.size(); i++)
		{
			hge->Texture_Free(fontpool[i]->GetTexture());
			delete fontpool[i];
		}
		vector<DMGFont>().swap(pool);
		vector<DMGFont>().swap(delaypool);
		vector<hgeSprite*>().swap(fontpool);
	}

private:
	FontPool();
	vector<DMGFont> pool;
	vector<DMGFont> delaypool;
	vector<hgeSprite*> fontpool;
	bool m_bHasInitialized;
	int delay;
	void AssureInitialized()
	{
		while (!m_bHasInitialized)
			Sleep(0);
	};
	bool IsDead(DMGFont dmgfont)
	{
		return !dmgfont.IsAlive();
	}
};
#endif