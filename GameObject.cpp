#include "GameObject.h"
#include "SceneManager.h"
#include "FontPool.h"
GameObject::GameObject(float x, float y, float size, float dmg, int HP,int collisionrange,ObjectType type,float speed,int innercode)
{
	Myx = x;
	Myy = y;
	Mytype = type;
	Mysize = size;
	MyDMG = dmg;
	MyHP = HP;
	MyMaxHP = HP;
	Mycollisionrange = collisionrange;
	MySpeed = speed;
	MySpeedRate = 1.0f;
	scn = SceneManager::GetSceneManager().GetCurrentScene();
	angle = 0;
	Imgangle = 0;
	avlid = true;
	finishb = false;
	islineskill = false;
	Myinnercode = innercode;
	Shield = 0;
	Scilenced = false;
	myaimx = 0;
	myaimy = 0;
	armorbreaktimer = 0;
	weaktimer = 0;
	toxictimer = 0;
	AB = 1.0f;
	ToxicDMG = 0;
	aimobject = NULL;
	TrueSuicide = false;
	dx = dy = 0;
	AddSpeed = 0;
	issolid = false;
	StopTime = 0;
	MyIsBlock = false;
}


GameObject::~GameObject()
{
}



void GameObject::Action()
{
	if (weaktimer)
		weaktimer--;
	if (armorbreaktimer)
		armorbreaktimer--;
	if (toxictimer)
	{
		if (Myinnercode != HERO)
		{
			if (toxictimer % 25 == 0)
			{
				if (MyHP > ToxicDMG)
				{
					GameObject::VaryHP(-ToxicDMG);
					FontPool::GetFontPool().InsertFont(GetX(), GetY(), -ToxicDMG, Passive, NeedRenderFont());
				}
				else
					FontPool::GetFontPool().InsertFont(GetX(), GetY(), 0, Passive, NeedRenderFont());
			}
		}
		else
		{
			if (toxictimer % 100 == 0)
			{
				if (MyHP > 1)
				{
					GameObject::VaryHP(-1);
				}
			}
		}
		toxictimer--;
	}
	SetPos(GetX() + dx, GetY() + dy);
	if (dy)
		dy *= 0.8;
	if (dy < 0.01)
		dy = 0;
	if (dx)
		dx *= 0.8;
	if (dx < 0.01)
		dx = 0;
};

 void GameObject::BoomEffect(GameObject *speedobj)
 {
	 if (speedobj && !issolid)
	 {
		 float angle=speedobj->GetAngle(this,true)-1.57;
		 dy += speedobj->MySpeed*0.002*sin(angle)*MySpeed*0.002*MySpeedRate;
		 dx += speedobj->MySpeed*0.002*cos(angle)*MySpeed*0.002*MySpeedRate;
	 }
 }


void GameObject::GoWithAnAngle(float angle)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	float dt = hge->Timer_GetDelta();
	if (!IsDead())
	{
		Myx += MySpeed*MySpeedRate*dt*cos(angle);
		Myy += MySpeed*MySpeedRate*dt*sin(angle);
	}
}

double GameObject::GetAngle(GameObject *obj, bool return1)
{
	if (obj)
	{
		float dx, dy;
		float distance, cos = 0;
		dx = Myx - obj->Myx;
		dy = Myy - obj->Myy;
		distance = sqrt((float)(dx*dx + dy*dy));
		if (!return1)
		{
			if ((dx <= 0) && (dy > 0))
			{
				cos = abs((float)dy / distance);
				angle = acos(cos);
			}
			if ((dx < 0) && (dy <= 0))
			{
				cos = abs((float)dx / distance);
				angle = acos(cos) + 1.57;
			}
			if ((dx >= 0) && (dy<0))
			{
				cos = abs((float)dy / distance);
				angle = acos(cos) + 3.14;
			}
			if ((dx>0) && (dy >= 0))
			{
				cos = abs((float)dx / distance);
				angle = acos(cos) - 1.57;
			}
			return 0;
		}
		else
		{
			if ((dx <= 0) && (dy > 0))
			{
				cos = abs((float)dy / distance);
				return acos(cos);
			}
			if ((dx < 0) && (dy <= 0))
			{
				cos = abs((float)dx / distance);
				return acos(cos) + 1.57;
			}
			if ((dx >= 0) && (dy<0))
			{
				cos = abs((float)dy / distance);
				return acos(cos) + 3.14;
			}
			if ((dx>0) && (dy >= 0))
			{
				cos = abs((float)dx / distance);
				return acos(cos) - 1.57;
			}
			return 0;
		}
	}
	else
		return angle;
}

bool GameObject::GoToAnotherObject(GameObject *obj)
{
	if (obj)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		float  dt = hge->Timer_GetDelta();
		if (GetDistance(obj) > MySpeed*MySpeedRate*dt)
		{
			GetAngle(obj);
			GoWithAnAngle(angle - 1.57);
			return false;
		}
		else
		{
			Myx = obj->GetX();
			Myy = obj->GetY();
			return true;
		}
	}
	else
		return false;
}

bool GameObject::GoToAnotherPos(float x, float y)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	float  dt = hge->Timer_GetDelta();
	if (abs(Myx - x) > MySpeed*MySpeedRate*dt || abs(Myy - y) > MySpeed*MySpeedRate*dt)
	{
		float dx, dy;
		float distance, cos1 = 0;
		dx = Myx - x;
		dy = Myy - y;
		distance = sqrt((float)(dx*dx + dy*dy));
		if ((dx <= 0) && (dy > 0))
		{
			cos1 = abs((float)dy / distance);
			angle = acos(cos1);
		}
		if ((dx < 0) && (dy <= 0))
		{
			cos1 = abs((float)dx / distance);
			angle = acos(cos1) + 1.57;
		}
		if ((dx >= 0) && (dy<0))
		{
			cos1 = abs((float)dy / distance);
			angle = acos(cos1) + 3.14;
		}
		if ((dx>0) && (dy >= 0))
		{
			cos1 = abs((float)dx / distance);
			angle = acos(cos1) - 1.57;
		}
		Myx += MySpeed*MySpeedRate*dt*cos(angle-1.57);
		Myy += MySpeed*MySpeedRate*dt*sin(angle-1.57);
	}
	else
	{
		Myx = x;
		Myy = y;
		return true;
	}
	return false;
}

void GameObject::TurnToAim(int data)
{
	if (aimobject)
	{
		float aimangle = GetAngle(aimobject,true);
		if (abs(angle - aimangle) <= M_PI)
		{
			if (abs(angle - aimangle) > M_PI / data)
			{
				if (angle < aimangle)
					angle += M_PI / data;
				else
					angle -= M_PI / data;
			}
			else
				angle = aimangle;
		}
		else
		{
			if (angle>aimangle)
				aimangle += M_PI * 2;
			else
				aimangle -= M_PI * 2;
			if (abs(angle - aimangle) > M_PI / data)
			{
				if (angle < aimangle)
					angle += M_PI / data;
				else
					angle -= M_PI / data;
			}
			else
				angle = aimangle;
		}
	}
}


bool GameObject::GoToAim()
{
	if (aimobject)
	{
		myaimx = aimobject->GetX();
		myaimy = aimobject->GetY();
	}
	HGE *hge = hgeCreate(HGE_VERSION);
	float  dt = hge->Timer_GetDelta();
	if (abs(Myx - myaimx) > MySpeed*MySpeedRate*dt || abs(Myy - myaimy) > MySpeed*MySpeedRate*dt)
	{
		float dx, dy;
		float distance, cos1 = 0;
		dx = Myx - myaimx;
		dy = Myy - myaimy;
		distance = sqrt((float)(dx*dx + dy*dy));
		if ((dx <= 0) && (dy > 0))
		{
			cos1 = abs((float)dy / distance);
			angle = acos(cos1);
		}
		if ((dx < 0) && (dy <= 0))
		{
			cos1 = abs((float)dx / distance);
			angle = acos(cos1) + 1.57;
		}
		if ((dx >= 0) && (dy<0))
		{
			cos1 = abs((float)dy / distance);
			angle = acos(cos1) + 3.14;
		}
		if ((dx>0) && (dy >= 0))
		{
			cos1 = abs((float)dx / distance);
			angle = acos(cos1) - 1.57;
		}
		Myx += MySpeed*MySpeedRate*dt*cos(angle-1.57);
		Myy += MySpeed*MySpeedRate*dt*sin(angle-1.57);
	}
	else
	{
		Myx = myaimx;
		Myy = myaimy;
		return true;
	}
	return false;
}


 void GameObject::Interaction(GameObject *target)
{
	 if (target)
	 if (!target->IsDead())
		 switch (target->GetInnerCode())
	 {
		 case ABBULLET:
			 armorbreaktimer += 125 + target->GetDMG() * 25 / 3;
			 AB = 1.43f + target->GetDMG()*0.1;
			 if (AB<1.5f)
				 AB = 1.5f;
			 else
			 if (AB>2)
				 AB = 2;
			 if (armorbreaktimer > 500)
				 armorbreaktimer = 500;
			 break;
		 case TBOMB:
		 case TBULLET:
		 case TOXICFIELD:
			 ToxicDMG = target->GetDMG()/4;
			 if (Myinnercode != HERO)
				 toxictimer = 150;
			 else
				 toxictimer = 400;
			 break;
		 case WBULLET:
			 weaktimer += 25 + target->GetDMG() * 25 / 5;
			 if (weaktimer > 400)
				 weaktimer = 400;
			 break;
		 case EMPBULLET:
			 SetStopTime(50);
			 break;
	 }
}



 double GameObject::GetAngle(float x, float y, bool return1)
 {
	 float dx, dy;
	 float distance, cos = 0;
	 dx = Myx - x;
	 dy = Myy - y;
	 distance = sqrt((float)(dx*dx + dy*dy));
	 if (!return1)
	 {
		 if ((dx <= 0) && (dy > 0))
		 {
			 cos = abs((float)dy / distance);
			 angle = acos(cos);
		 }
		 if ((dx < 0) && (dy <= 0))
		 {
			 cos = abs((float)dx / distance);
			 angle = acos(cos) + 1.57;
		 }
		 if ((dx >= 0) && (dy<0))
		 {
			 cos = abs((float)dy / distance);
			 angle = acos(cos) + 3.14;
		 }
		 if ((dx>0) && (dy >= 0))
		 {
			 cos = abs((float)dx / distance);
			 angle = acos(cos) - 1.57;
		 }
		 return 0;
	 }
	 else
	 {
		 if ((dx <= 0) && (dy > 0))
		 {
			 cos = abs((float)dy / distance);
			 return acos(cos);
		 }
		 if ((dx < 0) && (dy <= 0))
		 {
			 cos = abs((float)dx / distance);
			 return acos(cos) + 1.57;
		 }
		 if ((dx >= 0) && (dy<0))
		 {
			 cos = abs((float)dy / distance);
			 return acos(cos) + 3.14;
		 }
		 if ((dx>0) && (dy >= 0))
		 {
			 cos = abs((float)dx / distance);
			 return acos(cos) - 1.57;
		 }
		 return 0;
	 }
 }
