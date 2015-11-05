#include "SFXObject.h"










void SFXObject::Play(int maxvol)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	if (!sfx)
	{
		if (!channel)
			channel = hge->Effect_PlayEx(music, vol, 0, 1, true);
		else
			hge->Channel_SetVolume(channel, maxvol);
		if (vol < maxvol)
			vol+=2;
		else
		if (ready)ready = false;
	}
	else
	{
		hge->Effect_PlayEx(music,maxvol,0,pitch);
		if (ready)ready = false;
	}
}


bool SFXObject::Stop()
{
	if (ready)ready = false;
	HGE *hge = hgeCreate(HGE_VERSION);
	if (channel  && hge->Channel_IsPlaying(channel) && !sfx)
	{
		
		hge->Channel_SetVolume(channel, vol);
		if (vol > 0)
			vol-=2;
		else
		{
				hge->Channel_Stop(channel);
				channel = NULL;
				return true;
		}
	}
	return false;

}