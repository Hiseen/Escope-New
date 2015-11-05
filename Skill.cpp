#include "Skill.h"
#include"FontPool.h"
#include "RenderPool.h"


Skill::~Skill()
{
}



void Skill::Action()
{
	switch (GetInnerCode())
	{
	case TBULLET:
	case ABBULLET:
	case WBULLET:
	case EMPBULLET:
	case BBULLET:
	case BULLET:
		SetImgAngle(GetRot());
		GoWithAnAngle(GetRot() - 1.57);
		if (GetX()<ACT_START_X || GetX()>ACT_END_X || GetY()<ACT_START_Y || GetY()>ACT_END_Y)
			Suicide();
		switch (data2)
		{
		case BULLET_TRACKING:
			if (last_time)
			{
				if (GetSpeed() > 100)
					SetSpeed(GetSpeed()*0.95);
				else
				{
					if (GetAim() && GetDistance(GetAim()) < 500)
						TurnToAim(400);
					else
						SetRot(GetRot() + 0.002*(data3 ? -1 : 1));
					SetSpeed(GetSpeed() - 0.02);
					if (GetSpeed() < 1)
						Suicide();
				}
				last_time--;
			}
			else
				Suicide();
			break;
		default:
			if ((GetType() != Left ? GetSpeed()<ENEMYBULLET_MAXSPEED : true))
			SetSpeed(GetSpeed()*1.01);
		}
		break;
	case ENEMYBULLET:
		SetImgAngle(GetRot());
		switch (data2)
		{
		case BULLET_SPEEDUP:
			if (GetSpeed()<ENEMYBULLET_MAXSPEED)
			SetSpeed(GetSpeed()*1.015);
			break;
		case BULLET_SPEEDDOWN:
			SetSpeed(GetSpeed()*0.95);
			break;
		}
		GoWithAnAngle(GetRot() - 1.57);
		if (GetX()<ACT_START_X || GetX()>ACT_END_X || GetY()<ACT_START_Y || GetY()>ACT_END_Y)
			Suicide();
		break;
		/*
	case MBULLET:
		if (target)
		{
			HGE *hge = hgeCreate(HGE_VERSION);
			//if (GetSpeed()>8)
			SetSpeed(GetSpeed() + hge->Random_Float(1, 8));
			if (GetType() == Left)
			{		
					if (data && abs(data - GetAngle(target, true))>M_PI)
					{
						SetRot(GetRot() - M_PI * 2);
						last_time = 1;
					}
					data = GetAngle(target, true);
					if (abs(data - GetRot()) > 0.05)
						SetRot(GetRot() + (data - GetRot()) / 30);
					else
						SetRot(data);
					if (last_time)
						last_time++;
					if (last_time > 20 || GetRot()>M_PI * 4 || GetRot()<-M_PI * 2)
						SetRot(data);
					if (target->GetInnerCode() == HERO && GetX() > ACT_END_X - 150)
						target = NULL;
			}
			else
			{
					if (data && abs(data - GetAngle(target, true)) > M_PI)
					{
						SetRot(GetRot() - M_PI * 2);
						last_time = 1;
					}
					data = GetAngle(target, true);
					if (abs(data - GetRot()) > 0.05)
						SetRot(GetRot() + (data - GetRot()) / 30);
					else
						SetRot(data);
					if (last_time)
						last_time++;
					if (last_time > 20 || GetRot()>M_PI*4 || GetRot()<-M_PI*2)
						SetRot(data);
					if (target->GetInnerCode() == HERO && GetX() < ACT_START_X + 150)
						target = NULL;
			}
			SetImgAngle(GetRot());
		}
		else
		{
			if (GetX()<ACT_START_X || GetX()>ACT_END_X || GetY()<ACT_START_Y || GetY()>ACT_END_Y)
				Suicide();
		}
			GoWithAnAngle(GetRot()-1.57);
			if (GetX()<ACT_START_X-500 || GetX()>ACT_END_X+500 || GetY()<ACT_START_Y-500 || GetY()>ACT_END_Y+500)
				Suicide();
		break;
	case TONS_OF_MBULLET:
		if (GetHost())
		{
			HGE *hge = hgeCreate(HGE_VERSION);
			for (int i = 0; i < (GetDMG() - 15) / 3; i++)
			{
				Skill *skill = new Skill(GetHost(), MBULLET);
				skill->SetSpeed(hge->Random_Float(200, 300));
				skill->SetDMG(GetDMG()/5);
				ObjectPool::GetObjPool().InsertObject(skill, true);
			}
		}
		Suicide();
		break;
		*/
	case TONS_OF_ABBULLET:
		if (GetHost())
		{
			if (last_time)
			{
				switch (last_time)
				{
				case 50:
				case 20:
				case 10:
					for (int i = 0; i < 10; i++)
					{
						Skill* skill = new Skill(GetHost(), ABBULLET);
						skill->SetRot(skill->GetRot() - 0.4 + i*0.08);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
				}
				last_time--;
			}
			else
				Suicide();
		}
		else
		Suicide();
		break;
	case TONS_OF_WBULLET:
		if (GetHost())
		{
			if (last_time)
			{
				switch (last_time)
				{
				case 50:
				case 20:
				case 10:
					for (int i = 0; i < 10; i++)
					{
						Skill* skill = new Skill(GetHost(), WBULLET);
						skill->SetRot(skill->GetRot() - 0.4 + i*0.08);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
				}
				last_time--;
			}
			else
				Suicide();
		}
		else
		Suicide();
		break;
	case TONS_OF_TBULLET:
		if (GetHost())
		{
			if (last_time)
			{
				switch (last_time)
				{
				case 50:
				case 20:
				case 10:
					for (int i = 0; i < 10; i++)
					{
						Skill* skill = new Skill(GetHost(), TBULLET);
						skill->SetRot(skill->GetRot() - 0.4 + i*0.08);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
				}
				last_time--;
			}
			else
				Suicide();
		}
		else
		Suicide();
		break;
	case TONS_OF_BBULLET:
		if (GetHost())
		{
			if (last_time)
			{
				switch (last_time)
				{
				case 50:
				case 20:
				case 10:
					for (int i = 0; i < 10; i++)
					{
						Skill* skill = new Skill(GetHost(), BBULLET);
						skill->SetRot(skill->GetRot() - 0.4 + i*0.08);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
				}
				last_time--;
			}
			else
				Suicide();
		}
		else
		Suicide();
		break;
	case TONS_OF_EMPBULLET:
		if (GetHost())
		{
			if (last_time)
			{
				switch (last_time)
				{
				case 50:
				case 20:
				case 10:
					for (int i = 0; i < 10; i++)
					{
						Skill* skill = new Skill(GetHost(), EMPBULLET);
						skill->SetRot(skill->GetRot() - 0.4 + i*0.08);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
				}
				last_time--;
			}
			else
				Suicide();
		}
		else
		Suicide();
		break;
	case CORESKILL:
		if (last_time)
		{
			if (GetHost())
			switch (GetHost()->GetInnerCode())
			{
			case CORE_NORMAL:
				if (last_time % 5==0)
				{
					Skill *bullet = new Skill(this->GetHost(), COREBULLET);
					ObjectPool::GetObjPool().InsertObject(bullet, true);
				}
				break;
			case CORE_LAZER:
				if (last_time % 2 == 0)
				{
					Skill *bullet = new Skill(GetHost(), COREBULLET);
					ObjectPool::GetObjPool().InsertObject(bullet, true);
				}
				break;
			}
			last_time--;
		}
		else
		{
			if (GetHost())
			{
				switch (GetHost()->GetInnerCode())
				{
				case CORE_MISSILE:
				{
									  Skill *skill = new Skill(GetHost(), COREBULLET);
									  ObjectPool::GetObjPool().InsertObject(skill, true);
				}
					break;
				case CORE_SHOTGUN:
					for (int i = 0; i < 3; i++)
					{
						Skill *skill = new Skill(GetHost(), COREBULLET);
						skill->SetRot(skill->GetRot()-0.1 + 0.1*i);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
					break;
				}
				GetHost()->EndSkill();
			}
			Suicide();
		}
		break;
	case COREBULLET:
		if (GetHost())
		{
			switch (GetHost()->GetInnerCode())
			{
			case CORE_MISSILE:
				if (last_time && target)
				{
					float t = (100 - (float)last_time) / 100;
					float posx = (1 - t)*(1 - t)*GetX() + 2 * t*(1 - t)*GetAimx() + t*t*target->GetX(), posy = (1 - t)*(1 - t)*GetY() + 2 * t*(1 - t)*GetAimy() + t*t*target->GetY();
					GetAngle(posx, posy);
					SetPos(posx, posy);
					last_time--;
				}
				else
				{
					GoWithAnAngle(GetRot() - 1.57);
					SetSpeed(GetSpeed()*1.02);
					if (GetX()<ACT_START_X || GetX()>ACT_END_X || GetY()<ACT_START_Y || GetY()>ACT_END_Y)
						Suicide();
				}
				SetImgAngle(GetRot());
				break;
			default:
				GoWithAnAngle(GetRot() - 1.57);
				if (GetX()<ACT_START_X || GetX()>ACT_END_X || GetY()<ACT_START_Y || GetY()>ACT_END_Y)
					Suicide();
				break;
			}
		}
		else
		{
			GoWithAnAngle(GetRot() - 1.57);
			if (GetX()<ACT_START_X || GetX()>ACT_END_X || GetY()<ACT_START_Y || GetY()>ACT_END_Y)
				Suicide();
		}
		break;
	case THROWBULLET:
		if (!GoToAim())
		{
			float var = GetDistance(GetAimx(), GetAimy())/data;
			float var2 = var*var;
			SetSpeed(320 * var2- 320 * var + 600);
			SetSize(-data3*6.6*var2 + data3*6.6*var + data3);
		}
		else
		{
			Skill *skill = new Skill(this, BOOM);
			skill->SetSize(data3);
			ObjectPool::GetObjPool().InsertObject(skill, true);
			Suicide();
		}
		break;
	case AIMBULLET:
		if (last_time)
			last_time--;
		else
		{
			SetSpeed(GetSpeed() + 15);
			if (GoToAim())
				Suicide();
		}
		break;
	case TRACKBULLET:
		if (last_time)
		{
			if (GetSpeed() > 50)
				SetSpeed(GetSpeed()*0.9);
			else
				TurnToAim(50);
			last_time--;
			GoWithAnAngle(GetRot() - 1.57);
		}
		else
			Suicide();
		break;
	case MISSILES:
		if (GetHost())
		for (int i = 0; i < 20; i++)
		{
			HGE *hge = hgeCreate(HGE_VERSION);
			Skill *skill = new Skill(this->GetHost(), hge->Random_Int(0,4)?MISSILE:MISSILE2);
			skill->SetRot(0.314*i); //+ hge->Random_Float(-1, 1) - M_PI);
			ObjectPool::GetObjPool().InsertObject(skill, true);
		}
		Suicide();
		break;
	case MISSILE:
		if (last_time>0)
		{
			if (GetSpeed() > 10)
				SetSpeed(GetSpeed()*0.93);
			else
			{
				HGE *hge = hgeCreate(HGE_VERSION);
				last_time = 0;
				SetSpeed(hge->Random_Int(0,50));
			}
		}
		else
		{
			HGE *hge = hgeCreate(HGE_VERSION);
			if (last_time>-50)
				GetAngle(GetAim());
				SetSpeed(GetSpeed()+hge->Random_Float(1,5));
				last_time--;
				SetSpeed(GetSpeed()+4);
			if (last_time<-200)
			if (GetX()<ACT_START_X || GetX()>ACT_END_X || GetY()<ACT_START_Y || GetY()>ACT_END_Y)
				Suicide();
		}
		SetImgAngle(GetRot());
		GoWithAnAngle(GetRot() - 1.57);
		break;
	case MISSILE2:
		if (last_time && target)
		{
			float t = (100 - (float)last_time) / 150;
			float posx = (1 - t)*(1 - t)*GetX() + 2 * t*(1 - t)*GetAimx() + t*t*target->GetX(), posy = (1 - t)*(1 - t)*GetY() + 2 * t*(1 - t)*GetAimy() + t*t*target->GetY();
			GetAngle(posx, posy);
			SetPos(posx, posy);
			last_time--;
		}
		else
		{
			GoWithAnAngle(GetRot() - 1.57);
			SetSpeed(GetSpeed()*1.02);
			if (GetX()<ACT_START_X || GetX()>ACT_END_X || GetY()<ACT_START_Y || GetY()>ACT_END_Y)
				Suicide();
		}
		SetImgAngle(GetRot());
		break;
	case LAZER:
		SetImgAngle(GetRot());
		if (last_time)
		{
			last_time--;
			if (!last_time)
			{
				SetSpeed(0);
				SetCollisionRange(25, true);
			}
			else
			{
				if (GetHost())
				{
					SetPos(GetHost()->GetX(), GetHost()->GetY());
				}
				else
					Suicide();
			}
		}
		else
		{
			if (data2)
			{
				data2--;
				if (GetHost())
				{
					SetPos(GetHost()->GetX(), GetHost()->GetY());
				}
				else
					Suicide();
			}
			else
			    Suicide();
		}
		break;
	case DISPERSEBULLET:
		if (last_time)
		{
			GoWithAnAngle(GetRot() - 1.57);
			last_time--;
		}
		else
		{
			if (GetHost() && data>0)
			{
				HGE *hge = hgeCreate(HGE_VERSION);
				for (int i = 0; i < 8; i++)
				{
					Skill *skill = new Skill(GetHost(), DISPERSEBULLET);
					skill->SetPos(GetX(), GetY());
					skill->SetRot(GetRot() + M_PI / 8 - (M_PI / 4)*(i + 1));
					skill->SetDMG(1);
					skill->SetSpeed(GetSpeed()*0.4);
					skill->data = data - 1;
					skill->last_time = 20;
					ObjectPool::GetObjPool().InsertObject(skill, true);
				}
			}
			Suicide();
		}
		if (GetX()<ACT_START_X || GetX()>ACT_END_X || GetY()<ACT_START_Y || GetY()>ACT_END_Y)
			Suicide();
		break;
	case ENEMYBOUNCEARROW:
		if (last_time<100)
		{
			GetAngle(GetAim());
			SetImgAngle(GetRot());
			last_time++;
			if (last_time == 100)
			{
				if (GetHost())
				GetHost()->EndSkill();
				SetSpeed(600);
			}
		}
		else
		{
			GoWithAnAngle(GetRot() - 1.57);
			if (data<1)
			{
				if (data2!=BOUNCEBULLET_SPEEDUP)
				SetSpeed(GetSpeed()*1.05);
			}
			if (GetX()<GetActStartX() || GetY()<GetActStartY() || GetX()>GetActEndX() || GetY()>GetActEndY())
			{
				data++;
				if (data2 != BOUNCEBULLET_SPEEDUP)
					SetSpeed(GetSpeed()*0.7);
				else
					SetSpeed(GetSpeed() + 50);
				if (GetY()>GetActStartY())
					SetRot(-GetRot());
				else
					SetRot(M_PI - GetRot());
				if (GetY()>GetActEndY())
					SetRot(M_PI + GetRot());
				SetPos(GetX(), GetY());
				SetImgAngle(GetRot());
			}
			if (data > GetSize()+2)
			{
				Suicide();
				Skill *skill = new Skill(this, BOOM);
				ObjectPool::GetObjPool().InsertObject(skill, true);
			}
		}	
		break;
	case ICE_RUNEBOMB:
	case RUNEBOMB:
		if (last_time)
		{
			if (last_time == 99)
			{
				SetCollisionRange(0);
				SetDMG(GetDMG() / 4);
			}
			else
			if (last_time < 81)
			{
				if (last_time % 10 == 0)
				{
					int times = last_time / 10;
					SetRot(M_PI * 2 * times / (float)8);
					SetCollisionRange(50);
					SetX(GetAimx() + 100 * (cos(GetRot()) - sin(GetRot())));
					SetY(GetAimy() + 100 * (cos(GetRot()) + sin(GetRot())));
				}
				else
					SetCollisionRange(0);
			}
			last_time--;
		}
		break;
	case GETSHIELD:
		if (GetHost())
			GetHost()->VaryShield(1 + ObjectPool::GetObjPool().GetAmountOfEnemies());
		Suicide();
		break;
	case LIGHTARROW:
		GoWithAnAngle(GetRot() - 1.57);
		SetSpeed(GetSpeed()*1.2);
		if (GetX()<ACT_START_X || GetX()>ACT_END_X || GetY()<ACT_START_Y || GetY()>ACT_END_Y)
			Suicide();
		break;
	case BOUNCEARROW_LIGHTARROW:
		if (GetSpeed()<800)
		SetSpeed(GetSpeed()*1.1);
	case BOUNCEARROW_ICE:
	case BOUNCEARROW:
		if (GetAim())
			GoToAim();
		else
			GoWithAnAngle(GetRot() - 1.57);
		if (last_time)
			last_time--;
		else
			SetCollisionRange(15);
		if (GetX()<ACT_START_X || GetX()>ACT_END_X || GetY()<ACT_START_Y || GetY()>ACT_END_Y)
			Suicide();
		break;
	case ICE_TRACKBOMB:
	case HEAL_TRACKBOMB:
	case TRACKBOMB:
		if (GetAim())
		{
			SetSpeed(GetSpeed() + 5);
			GoToAim();
			if (GetDistance(GetAim()) < 20)
			{
				if (data < 2)
					data += 0.01;
				else
				{
					data = 2;
					SetAim(NULL);
				}
				SetCollisionRange(50 * data);
			}
		}
		else
			Suicide();
		break;
	case HEAL:
		if (GetHost())
		{
			GetHost()->VaryHP(5);
			Suicide();
		}
		break;
	case FLASH:
		if (GetHost())
		{
			HGE* hge = hgeCreate(HGE_VERSION);
			float x, y;
			hge->Input_GetMousePos(&x, &y);
			GetHost()->SetPos(x, y);
			Suicide();
		}
		break;
	case ENEMYSKILL:
		if (GetHost())
		{
			switch (data2)
			{
			case BUFF_I:
				if (last_time >= 25)
				{
					last_time = 0;
					Skill *skill = new Skill(this, BOOM);
				    ObjectPool::GetObjPool().InsertObject(skill, true);
				}
				else
					last_time++;
				GoWithAnAngle(GetRot() - 1.57);
				if (IsOutOfBound())
					Suicide();
				break;
			case MAGIC_I:
				SetSpeed(GetSpeed() + 5);
				if (GoToAim())
				{
					for (int i = 0; i < 8; i++)
					{
						Skill *skill = new Skill(GetHost(), EMPBULLET);
						skill->SetRot(M_PI*i / 4);
						skill->SetDMG(1);
						skill->SetSpeed(300);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
					GetHost()->EndSkill();
					Suicide();
				}
				GetHost()->SetPos(GetX(), GetY());
				break;
			case ETERNITY_I:
				if (last_time)
					last_time--;
				else
				{
					GetHost()->VaryHP(20 + (data - GetHost()->GetHP()) * 4);
					GetHost()->EndSkill();
					Suicide();
				}
				break;
			case ATTACK_I:
				if (last_time)
				{
					last_time--;
					if (last_time % 8 == 0)
					{
						Skill *skill = new Skill(GetHost(), THROWBULLET);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
				}
				else
				{
					GetHost()->EndSkill();
					Suicide();
				}
				break;
			}
			GetHost()->GetAim()->SetPos(GetHost()->GetX(), GetHost()->GetY());
		}
		else
			Suicide();
		break;
	case ENEMYSKILL2:
		if (GetHost())
		{
			switch (data2)
			{
			case BUFF_I:
				dynamic_cast<Enemy*>(GetHost())->SetAtkUp();
				Suicide();
				break;
			case MAGIC_I:
				if (last_time)
				{
					SetRot(data + M_PI*2*last_time / (float)40);
					Skill *bullet = new Skill(GetHost(), ENEMYBULLET);
					bullet->SetX(GetX() + 100 * (cos(GetRot()) - sin(GetRot())));
					bullet->SetY(GetY() + 100 * (cos(GetRot()) + sin(GetRot())));
					bullet->SetSpeed(100);
					bullet->SetRot(GetRot());
					bullet->SetStopTime(last_time);
					ObjectPool::GetObjPool().InsertObject(bullet, true);
					last_time--;
				}
				else
				{
					GetHost()->EndSkill();
					Suicide();
				}
				break;
			case ETERNITY_I:
				if (last_time)
				{
					if (last_time > 100)
					{
						last_time--;
						GetHost()->SetSize(GetHost()->GetSize() + 0.01);
					}
					else
					{
						if (last_time != 1)
						{
							if (GetSpeed() < 300)
								SetSpeed(GetSpeed() + 2.5);
							else
								last_time = 1;
						}
						else
						{
							if (GetSpeed()>10)
								SetSpeed(GetSpeed() - 2.5);
							else
								last_time = 0;
						}
						if (GetDistance(GetAim())<GetCollisionRange()/2)
							last_time = 0;
						else
							GoToAim();
						SetPos(GetX(),GetY());
						GetHost()->SetPos(this);
					}
				}
				else
				{
					if (GetHost()->GetSize()>1)
						GetHost()->SetSize(GetHost()->GetSize() - 0.1);
					else
					{
						GetHost()->SetSize(1);
						CanEffectOtherSkill = true;
						SetType(None);
					}
				}
				break;
			case ATTACK_I:
				if (last_time)
				{
					last_time--;
					GoWithAnAngle(GetRot() - 1.57);
					GetHost()->SetPos(this);
					if (GetX()<GetActStartX() || GetY()<GetActStartY() || GetX()>GetActEndX() || GetY()>GetActEndY())
					{
						if (GetY()>GetActStartY())
							SetRot(-GetRot());
						else
							SetRot(M_PI - GetRot());
						if (GetY()>GetActEndY())
							SetRot(M_PI + GetRot());
					}
				}
				else
				{
					Suicide();
					GetHost()->EndSkill();
				}
				break;
			}
			GetHost()->GetAim()->SetPos(GetHost()->GetX(), GetHost()->GetY());
		}
		else
			Suicide();
		break;
	case ENEMYSKILL3:
		if (GetHost())
		{
			switch (data2)
			{
			case BUFF_I:
				dynamic_cast<Enemy*>(GetHost())->SetAbsorb();
				Suicide();
				break;
			case MAGIC_I:
				if (last_time)
				{
					last_time--;
					switch (last_time)
					{
					case 80:
					case 60:
					case 10:
					{
							   Skill* skill = new Skill(GetHost(), MISSILE2);
							   ObjectPool::GetObjPool().InsertObject(skill, true);
					}
					}
				}
				else
					Suicide();
				break;
			case ETERNITY_I:
				if (last_time)
				{
					last_time--;
					switch (last_time)
					{
					case 100:
					case 80:
					case 30:
					{
							   HGE *hge = hgeCreate(HGE_VERSION);
							   Skill *skill = new Skill(GetHost(), TRACKBULLET);
							   skill->SetRot(hge->Random_Float(0, M_PI * 2));
							   ObjectPool::GetObjPool().InsertObject(skill, true);
					}
					}
				}
				else
				{
					Suicide();
					GetHost()->EndSkill();
				}
				break;
			case ATTACK_I:
				if (last_time)
				{
					last_time--;
					if (last_time % 3 == 0)
					{
						HGE *hge = hgeCreate(HGE_VERSION);
						Skill *skill = new Skill(GetHost(), AIMBULLET);
						skill->SetPos(hge->Random_Float(GetActStartX(), GetActEndX()), hge->Random_Float(GetActStartY(), GetActEndY()));
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
				}
				else
					Suicide();
				break;
			}
			GetHost()->GetAim()->SetPos(GetHost()->GetX(), GetHost()->GetY());
		}
		else
			Suicide();
		break;
	case ENEMYSKILL4:
		if (GetHost())
		{
			switch (data2)
			{
			case BUFF_I:
			{
						   Skill* skill = new Skill(GetHost(), ENEMYBOUNCEARROW);
						   skill->SetSize(3);
						   ObjectPool::GetObjPool().InsertObject(skill, true);
						   Suicide();
			}
				break;
			case MAGIC_I:
				for (int i = 0; i < 5; i++)
				{
					Skill* skill = new Skill(GetHost(), EMPBULLET);
					skill->SetRot(GetHost()->GetRot() - 0.4 + 0.2*i);
					skill->SetSpeed(300);
					skill->SetDMG(1);
					ObjectPool::GetObjPool().InsertObject(skill, true);
				}
				Suicide();
				break;
			case ETERNITY_I:
				if (last_time)
				{
					last_time--;
					if (last_time == 15)
					{
						for (int i = 0; i < 4; i++)
						{
							Skill *skill = new Skill(GetHost(), BBULLET, BULLET_TRACKING);
							skill->SetRot(data*M_PI_4 + M_PI*i / 2);
							skill->SetSpeed(600);
							ObjectPool::GetObjPool().InsertObject(skill, true);
						}
					}
				}
				else
				{
					Suicide();
					GetHost()->EndSkill();
				}
				break;
			case ATTACK_I:
				if (last_time)
				{
					last_time--;
					if (last_time % 25 == 0)
					{
						int times = last_time / 25;
						for (int i = 0; i < 4; i++)
						{
							Skill *skill = new Skill(GetHost(), TBULLET);
							skill->SetRot(times*M_PI_4+M_PI*i / 2);
							skill->SetSpeed(400);
							ObjectPool::GetObjPool().InsertObject(skill, true);
						}
					}
				}
				else
				{
					Suicide();
					GetHost()->EndSkill();
				}
				break;
			}
			GetHost()->GetAim()->SetPos(GetHost()->GetX(), GetHost()->GetY());
		}
		else
			Suicide();
		break;
	case ENEMYSKILL5:
		if (GetHost())
		{
			switch (data2)
			{
			case BUFF_I:
			{
						   Skill *skill = new Skill(GetHost(), MISSILES);
						   ObjectPool::GetObjPool().InsertObject(skill, true);
						   Suicide();
			}
				break;
			case MAGIC_I:
				if (last_time)
					last_time--;
				else
				{
					Suicide();
					GetHost()->EndSkill();
				}
				break;
			case ETERNITY_I:
			{
							   HGE *hge = hgeCreate(HGE_VERSION);
							   GetHost()->SetPos(hge->Random_Int(GetActStartX(), GetActEndX()), hge->Random_Int(GetActStartY(), GetActEndY()));
							   Suicide();
			}
				break;
			case ATTACK_I:
				if (last_time)
				{
					last_time--;
					if (last_time % 5 == 0)
					{
						Skill *skill = new Skill(GetHost(), ENEMYBULLET,BULLET_SPEEDSTAY);
						skill->SetSpeed(800);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
				}
				else
				{
					GetHost()->EndSkill();
					Suicide();
				}
				break;
			}
			GetHost()->GetAim()->SetPos(GetHost()->GetX(), GetHost()->GetY());
		}
		else
			Suicide();
		break;
	case ENEMYSKILL6:
		if (GetHost())
		{
			switch (data2)
			{
			case BUFF_I:
				if (last_time)
				{
					last_time--;
					switch (last_time)
					{
					case 80:
					case 50:
					case 20:
					case 10:
					{
							   HGE *hge = hgeCreate(HGE_VERSION);
							   Skill *skill = new Skill(GetHost(), DISPERSEBULLET);
							   skill->SetRot(GetAngle(ObjectPool::GetObjPool().GetEnemyHero(GetType()), true) + hge->Random_Float(-0.2, 0.2));
							   ObjectPool::GetObjPool().InsertObject(skill, true);
					}
						break;
					}
				}
				else
				{
					GetHost()->EndSkill();
					Suicide();
				}
				break;
			case MAGIC_I:
				if (last_time)
				{
					last_time--;
					if (GetSpeed() < 400)
						SetSpeed(GetSpeed() + 2);
					if (target)
					{
						GoToAnotherObject(target);
						if (GetDistance(target)<80)
						    last_time = 0;
					}
					GetHost()->SetPos(GetX(), GetY());
				}
				else
				{
					Skill *skill = new Skill(GetHost(), BOOM);
					skill->SetSize(2.5);
					ObjectPool::GetObjPool().InsertObject(skill, true);
					GetHost()->EndSkill();
					Suicide();
				}
				break;
			case ETERNITY_I:
				if (last_time)
					last_time--;
				else
				{
					HGE *hge = hgeCreate(HGE_VERSION);
					for (int i = 0; i < 3; i++)
						dynamic_cast<Enemy*>(GetHost())->SpawnEnemy((hge->Random_Int(0,1)?ENEMY_BOMBER:ENEMY_BULLET_BOMBER));
					GetHost()->EndSkill();
					Suicide();
				}
				break;
			case ATTACK_I:
				if (last_time)
					last_time--;
				else
				{
					for (int i = 0; i < 2;i++)
					dynamic_cast<Enemy*>(GetHost())->SpawnEnemy(ENEMY_DASHER);
					GetHost()->EndSkill();
					Suicide();
				}
				break;
			}
			GetHost()->GetAim()->SetPos(GetHost()->GetX(), GetHost()->GetY());
		}
		else
			Suicide();
		break;
	case BOSSSKILL1:
		switch (data2)
		{
		case ENEMY_BOSS_I:
			if (last_time)
			{
				last_time--;
				if (last_time % 50 == 0)
				{
					HGE *hge = hgeCreate(HGE_VERSION);
					float dis = GetDistance(ObjectPool::GetObjPool().GetEnemyHero(GetType()));
					for (int i = 0; i < 20; i++)
					{
						Skill *bullet = new Skill(GetHost(), THROWBULLET);
						bullet->SetAim(GetX() + dis * (cos(M_PI*i / 10) - sin(M_PI*i / 10)), GetY() + dis * (cos(M_PI*i / 10) + sin(M_PI*i / 10)));
						bullet->SetSpeed(300);
						bullet->data3 = 1;
						bullet->data = GetDistance(GetAimx(), GetAimy());
						ObjectPool::GetObjPool().InsertObject(bullet, true);
					}
					data3 = hge->Random_Float(0, M_PI * 2);
					int times = last_time / 50;
					if (times % 2 == 0)
					{
						for (int i = 0; i < 40; i++)
						{
							Skill *bullet = new Skill(GetHost(), ENEMYBULLET, BULLET_SPEEDSTAY);
							bullet->SetSpeed(350);
							bullet->SetRot(data3 + M_PI*i / 20);
							ObjectPool::GetObjPool().InsertObject(bullet, true);
						}
					}
				}
			}
			else
			{
				Suicide();
				GetHost()->EndSkill();
			}
			break;
		}
		break;
	case BOSSSKILL2:
		switch (data2)
		{
		case ENEMY_BOSS_I:
			for (int i = 0; i < 2; i++)
			{
				HGE *hge = hgeCreate(HGE_VERSION);
				Skill *skill = new Skill(GetHost(), TOXICFIELD);
				skill->SetPos(ACT_START_X + hge->Random_Float(0 + 0.7*i, 0.3 + 0.7*i)*(ACT_END_X - ACT_START_X),skill->GetY());
				ObjectPool::GetObjPool().InsertObject(skill, true);
			}
			Suicide();
			break;
		}
		break;
	case BOSSSKILL3:
		switch (data2)
		{
		case ENEMY_BOSS_I:
			if (last_time)
			{
				GetHost()->SetRot(GetHost()->GetRot() + (data ? 0.02 : -0.02));
				last_time--;
				if (last_time % 3 == 0)
				{
					for (int i = 0; i < 4; i++)
					{
						Skill *skill = new Skill(GetHost(), ENEMYBULLET,BULLET_SPEEDSTAY);
						skill->SetRot(skill->GetRot()+M_PI*i / 2);
						skill->SetSpeed(500);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
				}
				if (data3)
				    data3--;
				else
				{
					HGE *hge = hgeCreate(HGE_VERSION);
					data3 = hge->Random_Int(25, 200);
					if (!data)
						data = 1;
					else
						data = 0;
				}
			}
			else
			{
				Suicide();
				GetHost()->EndSkill();
			}
			break;
		}
		break;
	case BOSSSKILL4:
		switch (data2)
		{
		case ENEMY_BOSS_I:
			if (last_time)
				last_time--;
			else
			{
				Suicide();
				GetHost()->EndSkill();
			}
			break;
		}
		break;
	case BOSSSKILL5:
		switch (data2)
		{
		case ENEMY_BOSS_I:
			if (last_time)
			{
				GetHost()->GetAngle(ObjectPool::GetObjPool().GetEnemyHero(GetType()));
				last_time--;
				switch (last_time)
				{
				case 150:
				case 100:
				case 70:
				case 50:
				{
						   Skill *skill = new Skill(GetHost(), ENEMYBOUNCEARROW, BOUNCEBULLET_SPEEDUP, BOUNCEBULLET_NOPOWERUP);
						   ObjectPool::GetObjPool().InsertObject(skill, true);
				}
				}
			}
			else
			{
				GetHost()->EndSkill();
				Suicide();
			}
			break;
		}
		break;
	case BOSSSKILL6:
		switch (data2)
		{
		case ENEMY_BOSS_I:
			if (last_time)
			{
				last_time--;
				GetHost()->GetAngle(ObjectPool::GetObjPool().GetEnemyHero(GetType()));
				switch (last_time)
				{
				case 180:
				case 130:
				case 70:
				case 40:
					for (int i = 0; i < 5; i++)
					{
						Skill *skill = new Skill(GetHost(), ENEMYBULLET, BULLET_SPEEDSTAY);
						skill->SetSpeed(450);
						skill->SetRot(skill->GetRot() + 0.4 - i*0.2);
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
				}
			}
			else
			{
				Suicide();
				GetHost()->EndSkill();
			}
			break;
		}
		break;
	case BOSSSKILL7:
		switch (data2)
		{
		case ENEMY_BOSS_I:

			break;
		}
		break;
	case BOSSSKILL8:
		switch (data2)
		{
		case ENEMY_BOSS_I:
			break;
		}
		break;
	case TOXICFIELD:
		if (last_time)
			last_time--;
		else
			Suicide();
		break;
	}
}


void Skill::Interaction(GameObject *target)
{
	bool hited = false;
	if (target)
	{
		if (!target->IsSkill())
		{
			switch (GetInnerCode())
			{
			case TBULLET:
			case ABBULLET:
			case WBULLET:
			case EMPBULLET:
				hited = true;
				target->VaryHP_Obj(this);
				target->Interaction(this);
				target->BoomEffect(this);
				Suicide();
				break;
			case BULLET:
				hited = true;
				if (GetHost())
					target->VaryHP_Obj(GetHost());
				else
					target->VaryHP(-GetDMG());
				target->BoomEffect(this);
				Suicide();
				break;
			case ENEMYBULLET:
			case BBULLET:
				hited = true;
				target->VaryHP_Obj(this);
				target->BoomEffect(this);
				Suicide();
				break;
			case LIGHTARROW:
				target->VaryHP_Obj(this);
				SetDMG(GetDMG() / 2 * 1.25);
				break;
			case BOUNCEARROW_LIGHTARROW:
				if (!target->IsSkill())
				{
					target->VaryHP_Obj(this);
					GameObject *obj = ObjectPool::GetObjPool().GetDistanceEnemies(target, 150);
					int dmg = GetDMG()*0.55;
					if (obj)
					{
						Skill *skill = new Skill(this, BOUNCEARROW_LIGHTARROW);
						skill->SetDMG(dmg);
						skill->SetAim(obj);
						skill->GetAngle(obj);
						if (dmg <= 5)
							skill->data = data + 1;
						if (dmg > 5 || data <= 8)
							ObjectPool::GetObjPool().InsertObject(skill, true);
						else
						{
							delete skill;
							skill = NULL;
						}
					}
					SetAim(NULL);
				}
				break;
			case BOUNCEARROW_ICE:
				if (!target->IsSkill())
				{
					target->VaryHP_Obj(this);
					target->SetStopTime(100);
					GameObject *obj = ObjectPool::GetObjPool().GetDistanceEnemies(target, 150);
					int dmg = GetDMG()*0.45;
					if (obj)
					{
						Skill *skill = new Skill(this, BOUNCEARROW_ICE);
						skill->SetDMG(dmg);
						skill->SetAim(obj);
						skill->GetAngle(obj);
						if (dmg <= 5)
							skill->data = data + 1;
						if (dmg > 5 || data <= 8)
							ObjectPool::GetObjPool().InsertObject(skill, true);
						else
						{
							delete skill;
							skill = NULL;
						}
					}
					Suicide();
				}
				break;
			case BOUNCEARROW:
				if (!target->IsSkill())
				{
					target->VaryHP_Obj(this);
					GameObject *obj = ObjectPool::GetObjPool().GetDistanceEnemies(target, 150);
					int dmg = GetDMG()*0.45;
					if (obj)
					{
						Skill *skill = new Skill(this, BOUNCEARROW);
						skill->SetDMG(dmg);
						skill->SetAim(obj);
						skill->GetAngle(obj);
						if (dmg <= 5)
							skill->data = data + 1;
						if (dmg > 5 || data <= 8)
							ObjectPool::GetObjPool().InsertObject(skill, true);
						else
						{
							delete skill;
							skill = NULL;
						}
					}
					Suicide();
				}
				break;
			case COREBULLET:
			case AIMBULLET:
			case MISSILE:
			case MISSILE2:
			case DISPERSEBULLET:
				target->VaryHP_Obj(this);
				Suicide();
				break;
			case TBOMB:
				target->VaryHP_Obj(this);
				target->Interaction(this);
				break;
			case BOOM:
			case SELFBOOM:
			case RUNEBOMB:
				if (target->GetInnerCode() != HERO)
				{
					int dmg = GetDMG(true) * 30 / GetDistance(target);
					if (dmg < -300)
						dmg = -300;
					target->VaryHP(dmg);
				}
				else
					target->VaryHP(-2);
				switch (data2)
				{
				case ICE_TRACKBOMB:
					target->SetStopTime(250);
					break;
				case HEAL_TRACKBOMB:
					if (Myhost)
						Myhost->VaryHP(1);
					break;
				default:
					target->BoomEffect(this);
					break;
				}
				break;
			case ICE_RUNEBOMB:
			case ICE_BOOM:
				if (target->GetInnerCode() != HERO)
				{
					int dmg = GetDMG(true) * 30 / GetDistance(target);
					if (dmg < -300)
						dmg = -300;
					target->VaryHP(dmg);
				}
				else
					target->VaryHP(-2);
				target->SetStopTime(250);
				break;
			case ENEMYSKILL2:
				if (data2 == ETERNITY_I && GetType() == None)
				{
					if (target != GetHost())
					{
						if (target->IsEnemy() || target->IsSkill())
							target->Suicide(true);
					}
				}
				break;
			case LAZER:
			case TRACKBULLET:
			case ENEMYBOUNCEARROW:
				target->VaryHP_Obj(this);
				break;
			case TOXICFIELD:
				if (target->VaryHP_Obj(this) && GetHost())
				GetHost()->VaryHP((GetHost()->GetMaxHP() - GetHost()->GetHP())*0.05);
				break;
			}
			if (GetHost() && GetHost()->GetInnerCode() == HERO &&hited)
				GetHost()->Interaction2(this);
			if (target->IsDead() && target->IsItem() && target->GetInnerCode() != ITEM)
			{
				if (GetHost())
					target->Interaction(GetHost());
			}
		}
		else
		{
			if (IsEffectiveForOtherSkill())
				SkillInteraction(target);
		}
	}
}


void Skill::AfterOneEffect()
{
	switch (GetInnerCode())
	{
	case TBOMB:
	case BOOM:
	case SELFBOOM:
	case ICE_BOOM:
		Suicide();
		break;
	case ENEMYSKILL2:
		if (data2 == ETERNITY_I && GetType() == None)
		{
			if (GetHost())
			{
				GetHost()->SetCollisionRange(data);
				GetHost()->EndSkill();
			}
			Suicide();
		}
		break;
	}
}


void Skill::Death()
{
	switch (GetInnerCode())
	{
	case BBULLET:
		if (Myhost)
		{
			Skill *boom = new Skill(Myhost, BOOM);
			boom->SetPos(this);
			ObjectPool::GetObjPool().InsertObject(boom, true);
		}
		break;
	case ICE_TRACKBOMB:
	case HEAL_TRACKBOMB:
	case TRACKBOMB:
		if (Myhost)
		{
			if (!data)
			{
				Skill *boom = new Skill(Myhost, BOOM, GetInnerCode());
				boom->SetPos(this);
				ObjectPool::GetObjPool().InsertObject(boom, true);
			}
			else
			{
				Skill *boom = new Skill(Myhost, BOOM, GetInnerCode());
				boom->SetSize(1 + data);
				boom->SetPos(this);
				boom->SetDMG(data2*data * 2);
				ObjectPool::GetObjPool().InsertObject(boom, true);
			}
		}
		break;
	case DISPERSEBULLET:
		if (last_time && GetHost() && data>0)
		{
			HGE *hge = hgeCreate(HGE_VERSION);
			for (int i = 0; i < 8; i++)
			{
				Skill *skill = new Skill(GetHost(), DISPERSEBULLET);
				skill->SetPos(GetX(), GetY());
				skill->SetRot(GetRot() + M_PI / 8 - (M_PI / 4)*(i + 1));
				skill->SetDMG(1);
				skill->data = data-1;
				skill->SetSpeed(GetSpeed()*0.4);
				skill->last_time = 20;
				ObjectPool::GetObjPool().InsertObject(skill, true);
			}
		}
		break;
	}
	GameObject::Death();
}