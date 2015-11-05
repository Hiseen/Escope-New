#pragma once

#include "hge.h"


#define MENUBGM  1
#define GAMEBGM  2
#define FINALBGM 3
#define BUTTONSFX 4
#define BUTTONSFX2 5
#define BULLETBOOMSFX 6
#define BOSSBGM 7
#define RESULTBGM 8
#define SHOOTSFX 9
#define ENEMYBOOMSFX1 10
#define ENEMYBOOMSFX2 11
#define HITSFX1 15
#define HITSFX2 16
#define GETEXPSFX 17
#define NOTICESFX 18
#define LVUPSFX 19
#define GETSKILLSFX 20

class SFXObject
{
public:
	SFXObject(const char* filename,int musicnum1,bool issfx)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		music = hge->Effect_Load(filename);
		vol = 0;
		sfx = issfx;
		channel = NULL;
		musicnum = musicnum1;
		ready = false;
		pitch = 1.0f;
	};
	~SFXObject()
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		hge->Effect_Free(music);
	}
	;
	void Play(int maxvol);
	bool Stop();
	int GetNumber()const { return musicnum; };
	void ReadyToPlay(){ ready = true; }
	bool IsReady()const{ return ready; };
	bool IsSfx()const{ return sfx; };
	void SetPitch(float newpitch)
	{
		pitch = newpitch;
	}
	bool IsPlaying()const
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		if (channel)
			return hge->Channel_IsPlaying(channel);
		return false;
	}
private:
	HEFFECT music;
	HCHANNEL channel;
	int vol;
	bool sfx;
	int musicnum;
	bool ready;
	float pitch;
};

