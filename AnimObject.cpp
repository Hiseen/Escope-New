#include "AnimObject.h"


AnimObject::AnimObject(const char *name, int nframes, int fps, float w, float h, float centerx, float centery, int aID,bool infinite, float x1, float y1)
{
	HGE* hge = hgeCreate(HGE_VERSION);
	tex = hge->Texture_Load(name);
	anim = new hgeAnimation(tex, nframes, fps, 0, 0, w, h);
	anim->SetHotSpot(centerx, centery);
	ID = aID;
	imgx = x1;
	imgy = y1;
	dead = false;
	angle = 0.0f;
	size = 1.0f;
	Isinfinite = infinite;
}


AnimObject::~AnimObject()
{
	ForceToDie();
	HGE *hge = hgeCreate(HGE_VERSION);
	hge->Texture_Free(tex);
	SAFE_DELETE(anim);
}


void AnimObject::UpdateStep()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	anim->Update(hge->Timer_GetDelta());
}


void AnimObject::Render(float x1, float y1,float rot,float size)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	if (!Isinfinite)
	{
		if (anim->GetFrames() == 1)
			anim->RenderEx(x1, y1, rot,size,size);
		else
		{
			if (!anim->IsPlaying())anim->Play();
			if (anim->IsPlaying() && anim->GetFrame() != anim->GetFrames() - 1)
				anim->RenderEx(x1, y1, rot, size, size);

		}
	}
	else
	{
		if (!anim->IsPlaying())anim->Play();
		if (anim->GetFrame() == anim->GetFrames())
			anim->SetFrame(0);
		anim->RenderEx(x1, y1, rot, size, size);
	}
}

int AnimObject::GetID()
{
	return ID;
}

void AnimObject::Render()
{
	if (anim)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		if (!anim->IsPlaying())
			anim->Play();
		if (anim->GetFrame() < anim->GetFrames() - 1)
		{
			if (anim->GetFrame()>0)alreadyplayed = true;
			anim->RenderEx(imgx, imgy, angle, size, size);
			anim->Update(hge->Timer_GetDelta());
		}
		else
		{
			dead = true;
			SAFE_DELETE(anim);
		}
		if (!dead && alreadyplayed && anim->GetFrame() == 0)
		{
			dead = true;
			SAFE_DELETE(anim);
		}
	}
}


void AnimObject::ChangePos(float x1, float y1)
{
	imgx = x1;
	imgy = y1;
}

AnimObject::AnimObject(AnimObject *anim1, float changeFPS)
{
	tex = anim1->tex;
	if (changeFPS)
	    anim = new hgeAnimation(tex,anim1->anim->GetFrames(),changeFPS,0,0,anim1->anim->GetWidth(),anim1->anim->GetHeight());
	else
		anim = new hgeAnimation(tex, anim1->anim->GetFrames(), anim1->anim->GetSpeed(), 0, 0, anim1->anim->GetWidth(), anim1->anim->GetHeight());
	float x, y;
	anim1->anim->GetHotSpot(&x, &y);
	anim->SetHotSpot(x, y);
	ID = anim1->ID;
	imgx = anim1->imgx;
	imgy = anim1->imgy;
	dead = anim1->dead;
	size = anim1->size;
	alreadyplayed = false;
}
