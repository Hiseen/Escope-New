#include "FontPool.h"
#include <math.h>
#include <sstream>
#include <iostream>
#include <stdlib.h> //要将string类和int类型直接转换最好有这些包含，


template <class T> string m_toStr(T tmp)
{
	stringstream ss;
	ss << tmp;
	return ss.str();
}



FontPool::FontPool()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	delay = 5;
	for (int i = 0; i < 32; i++)
	{
		string a = ".png";
		stringstream ss;
		string str;
		ss << i;
		ss >> str;
		a = str + a;
		HTEXTURE ab = hge->Texture_Load(a.c_str());
		hgeSprite *font = new hgeSprite(ab, 0, 0, hge->Texture_GetWidth(ab), hge->Texture_GetHeight(ab));
		if (i < 30)font->SetHotSpot(6, 8.5);
		if (i == 30)font->SetHotSpot(16.5, 7);
		if (i == 31)font->SetHotSpot(32, 7.5);
		fontpool.push_back(font);
	}
	m_bHasInitialized = true;
}


FontPool::~FontPool()
{
}


void FontPool::InsertFont(float x, float y, int dmg, Type type, bool Need)
{
	if (Need)
	{
		DMGFont font(dmg, x, y, type);
		delaypool.push_back(font);
		delaypool.shrink_to_fit();
	}
}

void FontPool::RenderFont()
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i].IsAlive())
		{
			pool[i].Action();
			if (pool[i].GetDMGType()!=Dodge)
			{
				int num = pool[i].GetNum();
				string str = m_toStr<int>(num);
				int number = str.size();
				for (int j = 0; j < number; j++)
				{
					int pos = atoi(str.substr(j, 1).c_str());
					switch (pool[i].GetDMGType())
					{
					case Damage:
						break;
					case Heal:
						pos += 10;
						break;
					case Passive:
						pos += 20;
						break;
					}
					fontpool[pos]->SetColor(ARGB(pool[i].GetAlpha(), 255, 255, 255));
					if (number%2==1)
					    fontpool[pos]->RenderEx(pool[i].GetX()+(-(number / 2)+j)*(12*pool[i].GetSize()), pool[i].GetY(), 0, pool[i].GetSize(), pool[i].GetSize());
					else
						fontpool[pos]->RenderEx(pool[i].GetX() + (-(((float)number-1) / 2) + j)*(12 * pool[i].GetSize()), pool[i].GetY(), 0, pool[i].GetSize(), pool[i].GetSize());
					//fontpool[pos]->SetColor(ARGB(255, 255, 255, 255));
				}
				if (pool[i].GetDMGType() == Crit)
				{
					fontpool[31]->SetColor(ARGB(pool[i].GetAlpha(), 255, 255, 255));
					fontpool[31]->RenderEx(pool[i].GetX(), pool[i].GetY() - 17, 0, pool[i].GetSize(), pool[i].GetSize());
					//fontpool[31]->SetColor(ARGB(255, 255, 255, 255));
				}
			}
			else
			{
				fontpool[30]->SetColor(ARGB(pool[i].GetAlpha(), 255, 255, 255));
				fontpool[30]->RenderEx(pool[i].GetX(), pool[i].GetY(), 0, pool[i].GetSize(), pool[i].GetSize());
				//fontpool[30]->SetColor(ARGB(255, 255, 255, 255));
			}
		}
		
	}
}

void FontPool::Delete()
{
	if (delay>0)
		delay--;
	else
	{
		delay = 5;
		for (int i = 0; i < delaypool.size(); i++)
			pool.push_back(delaypool[i]);
		delaypool.clear();
	}
	vector<DMGFont>::iterator it;
	for (it = pool.begin(); it != pool.end();)
	if (IsDead(*it))
	{
		it = pool.erase(it);
	}
	else
		++it;
}


