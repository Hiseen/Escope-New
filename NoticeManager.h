#pragma once
#include <vector>
#include "NoticeUnit.h"
#include"defines.h"
#include "SFXManager.h"
using namespace std;
class NoticeManager
{
public:
	~NoticeManager();
	static NoticeManager& GetNoticeManager()
	{
		static NoticeManager ObjPool;
		ObjPool.AssureInitialized();
		return ObjPool;
	}
	void InsertNotice(const char* title, const char* main);
	void Update();
	void Clean()
	{
		for (int i = 0; i<queue.size(); i++)
		{
			delete queue[i];
			queue.erase(queue.begin() + i);
		}
		vector<NoticeUnit*>().swap(queue);
	};
	void Exit()
	{
		Clean();
		delete left;
		delete right;
		delete font;
		delete font2;
	}
	bool IsOccopied()const
	{
		if (queue.size() > 0)
			return true;
		else
			return false;
	};
private:
	NoticeManager();
    bool m_bHasInitialized;
	void AssureInitialized()
	{
		while (!m_bHasInitialized)
			Sleep(0);
	};
	vector<NoticeUnit*> queue;
	bool occupied;
	int leftx,rightx;
	float alpha;
	hgeSprite *left;
	hgeSprite *right;
	hgeFont *font;
	hgeFont *font2;
};

