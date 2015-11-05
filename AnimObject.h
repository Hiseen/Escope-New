#pragma once
#include "hgeanim.h"
#ifndef ANIMOBJECT_H_
#define ANIMOBJECT_H_

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif

class AnimObject
{
public:
	AnimObject(const char *name, int nframes, int fps, float w, float h, float centerx, float centery, int aID,bool infinite=false, float x1 = 0, float y1 = 0);
	~AnimObject();
	AnimObject(AnimObject *anim1,float changeFPS=0);
	AnimObject(const AnimObject &obj)
	{
		tex = obj.tex;
		anim = obj.anim;
		ID = obj.ID;
		imgx = obj.imgx;
		imgy = obj.imgy;
		dead = obj.dead;
		angle = obj.angle;
		size = obj.size;
		alreadyplayed = false;
	}
	void Render(float x1, float y1,float rot=0.0f,float size=1.0f);
	void UpdateStep();
	void SetAngle(float newangle){ angle = newangle; };
	int GetID();
	void Render();
	void Stop(){ anim->Stop(); };
	bool Isfinish()const{ return dead; };
	void ForceToDie()
	{ 
		if (!dead)
		{
			dead = true;
			SAFE_DELETE(anim);
		}
	};
	void ChangePos(float x1,float y1);
	bool IsPlaying()const{ return anim->IsPlaying(); };
	void Play(){ anim->Play(); };
	bool IsInfinte()const{ return Isinfinite; };
	void SetSize(float newsize)
	{
		size = newsize;
	}

	void SetColor(DWORD newcolor)
	{
		anim->SetColor(newcolor);
	}
	DWORD GetColor()const
	{
		return anim->GetColor();
	}
	//void Render(float x, float y);
	//bool IsMuTiAction()
	//{
	//	if (steps == 0)
	//		return false;
	//	else return true;
	//}
private:
	HTEXTURE tex;
	hgeAnimation *anim;
	int ID;
	float imgx, imgy;
	bool dead;
	float angle;
	bool Isinfinite;
	bool alreadyplayed;
	float size;
};

#endif