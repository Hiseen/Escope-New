#include "Item.h"
#include "ObjectPool.h"
#include "RenderPool.h"

Item::Item(int innercode) :GameObject(ACT_CENTER_X,ACT_CENTER_Y,1,0,1,100,None,100,ITEM)
{
	price = 0;
	flag = false;
	itemcode = innercode;
	if (innercode >= CORE_START && innercode <= CORE_END)
		price = 500;
	switch (innercode)
	{
	case ITEM_HEAL_HP:
		price = 100;
		break;
	case ITEM_HEAL_MP:
		price = 150;
		break;
	case ITEM_SHUFFLEHERO:
		price = 300;
		break;
	case ITEM_ADDDMG:
		price = 250;
		break;
	}
	SetBlock(true);
	SetSolid(true);
}


void Item::Action()
{
	GoToAnotherPos(ACT_CENTER_X, ACT_CENTER_Y);
	HGE *hge = hgeCreate(HGE_VERSION);
	float x, y;
	hge->Input_GetMousePos(&x, &y);
	int dis = GetDistance(x, y);
	if (dis < 120)
	{
		ObjectPool::GetObjPool().GetEnemyHero(GetType())->Scilence();
		if (dis < 90)
		{
			if (!flag && hge->Input_KeyDown(HGEK_LBUTTON))
			{
				Hero *hero = dynamic_cast<Hero*>(ObjectPool::GetObjPool().GetEnemyHero(GetType()));
				if (hero->GetCoins() >= price)
				{
					hero->SetCoins(hero->GetCoins() - price);
					//根据innercode执行相应效果，然后suicide;
					if (itemcode >= CORE_START && itemcode <= CORE_END)
					{
						Robot *item = new Robot(this, itemcode, None);
						ObjectPool::GetObjPool().InsertObject(item, true);
					}
					else
					{
						//;
					}
					Suicide();
				}
				else
					flag = 20;
			}
		}
	}
	else
		ObjectPool::GetObjPool().GetEnemyHero(GetType())->UnScilence();
	if (flag)
	{
		flag--;
		int a = flag / 5;
		int b = flag % 5;
		if (a % 2)
			SetX(GetX() + (3 - b) * 2);
		else
			SetX(GetX() - (3 - b) * 2);
	}
};

void Item::Death()
{
	ObjectPool::GetObjPool().GetEnemyHero(GetType())->UnScilence();
	RenderPool::GetRenderPool().CleanNowRoomData();
	avlid = false;
}


Item::~Item()
{
}
