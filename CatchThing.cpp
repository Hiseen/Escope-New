#include "CatchThing.h"
#include "ObjectPool.h"

CatchThing::CatchThing(GameObject *host, int innercode) :GameObject(host->GetX(), host->GetY(), 1, 0, 1, -1, None, 0, innercode)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	lasttimer = 150;
	SetAim(ObjectPool::GetObjPool().GetOurHero(Left));
	SetRot(hge->Random_Float(0, M_PI * 2));
	SetSpeed(hge->Random_Int(200, 350));
	switch (innercode)
	{
	case CRYSTAL:
		SetCollisionRange(-10);
		break;
	case BATTERY:
		SetCollisionRange(-5);
		break;
	}
	lockon = false;
}




void CatchThing::Action()
{

	if (!lockon)
	{
		if (GetSpeed() > 0)
		{
			GoWithAnAngle(GetRot() - 1.57);
			SetSpeed(GetSpeed()*0.95 - 1);
		}
		else
		{
			if (lasttimer)
				lasttimer--;
			else
				Suicide();
			if (GetDistance(GetAim()) < 150)
			{
				lockon = true;
				SetSpeed(50);
			}
		}
	}
	else
	{
		if (GoToAim())
			Suicide();
		SetSpeed(GetSpeed()*1.1);
	}
}

void CatchThing::Death()
{
	if (GetAim())
	{
		Hero *hero = dynamic_cast<Hero*>(GetAim());
		switch (GetInnerCode())
		{
		case BATTERY:
			GetAim()->VaryHP(5);
			break;
		case GEM:
			hero->SetCoins(hero->GetCoins() + 1);
			break;
		case CRYSTAL:
			hero->GetCrystal(1);
			break;
		}
	}
	avlid = false;
}













CatchThing::~CatchThing()
{
}
