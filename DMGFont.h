#pragma once
#include"hge.h"
#ifndef DMGFONT_H_
#define DMGFONT_H_
enum Type
{
	Damage,
	Heal,
	Passive,
	Crit,
	Dodge
};
class DMGFont
{
public:
	DMGFont(int num,float x1,float y1,Type dmg);
	void Action();
	Type GetDMGType()const{ return DmgType; };
	int GetNum()const{ return data; };
	float GetAlpha()const{ return alpha; };
	float GetSize()const{ return size; };
	float GetX()const{ return x; };
	float GetY()const{ return y; };
	~DMGFont();
	bool IsAlive()const{ return alive; };
private:
	float size;
	float alpha;
	float x, y;
	int data;
	Type DmgType;
	bool alive;
	float finalsize;
	bool finishfirst;
};
#endif