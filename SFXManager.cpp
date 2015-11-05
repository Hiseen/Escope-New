#include "SFXManager.h"



SFXManager::~SFXManager()
{
}

void SFXManager::PlaySFX(int number, bool changepan)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	for (int i = 0; i < sfxpool.size();i++)
	if (sfxpool[i]->GetNumber() == number && !sfxpool[i]->IsPlaying())
	{
		sfxpool[i]->ReadyToPlay();
		if (changepan)
			sfxpool[i]->SetPitch(hge->Random_Float(0.2,1.8));
		if(!sfxpool[i]->IsSfx())clean = false;
	}
}


void SFXManager::UpdatePool()
{
	if (!clean)
	{
		for (int j = 0; j < sfxpool.size(); j++)
		{
			if (sfxpool[j]->IsPlaying())//&& !sfxpool[j]->IsReady())
			{
				symbol = true;
				if (sfxpool[j]->Stop())
				{
					clean = true;
					symbol = false;
				}
			}
		}
		if(!symbol)clean = true;
	}
	if (clean)
	for (int i = 0; i < sfxpool.size();i++)
	if (sfxpool[i]->IsReady())
	{
		if (!sfxpool[i]->IsSfx())
		    sfxpool[i]->Play(MaxVolForMusic);
		else
			sfxpool[i]->Play(MaxVolForSfx);
	}
}


void SFXManager::InsertSFX(const char *filename, int ID,bool issfx)
{
	SFXObject *sfx = new SFXObject(filename, ID, issfx);
	sfxpool.push_back(sfx);
}