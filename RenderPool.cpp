#include "RenderPool.h"


RenderPool::RenderPool()
{
	font = new hgeFont("font2.fnt");
	m_bHasInitialized = true;
}


RenderPool::~RenderPool()
{
}
void RenderPool::SetCombination(int IDforImage, int IDforObject, bool isanim ,int IDforAnim)
{

	if (!isanim)
	{
		box box1;
		box1.imageID = IDforImage;
		box1.ObjectID = IDforObject;
		combs.push_back(box1);
		combs.shrink_to_fit();
	}
	else
	{
		box box1;
		box1.imageID = IDforImage;
		box1.ObjectID = IDforAnim;
		animcombs.push_back(box1);
		animcombs.shrink_to_fit();
	}
}

void RenderPool::RenderEveryObject()
{
	for (int k = 0; k < ObjectPool::GetObjPool().pool.size(); k++)
	{
		GameObject *obj = ObjectPool::GetObjPool().pool[k];
		if (!obj->IsDead() && obj->GetSceneNumber() == SceneManager::GetSceneManager().GetCurrentScene())
		{
			    int nID = -1;
				int num = obj->GetInnerCode();
				//总感觉这个检测超级费速度
					for (int i = 0; i < combs.size(); i++)
					{
						if (combs[i].ObjectID == num)
						{
							nID = combs[i].imageID;
							break;
						}
					}
					if (nID!=-1)
					for (int i = 0; i < pool.size(); i++)
					{
						if (pool[i]->GetID() == nID)
						{
							RenderOne(obj, pool[i]);
							break;
						}
					}
			}
		}
	font->printf(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, HGETEXT_CENTER, "%d", ObjectPool::GetObjPool().pool.size());
}

void RenderPool::RenderOne(GameObject *vir,ImageObject *img)
{
	img->render(vir->GetX(), vir->GetY(), vir->GetImgAngle(), vir->GetSize());
}

void RenderPool::InsertImage(ImageObject* img)
{
	pool.push_back(img);
	pool.shrink_to_fit();
}

void RenderPool::RenderMap()
{
	int Startx = SCREEN_WIDTH / 2 - 50 * 5;
	int Starty = SCREEN_HEIGHT / 2 - 50 * 2;
	for (int i = 0; i < 11;i++)
	for (int j = 0; j < 5; j++)
	{
		char *a;
		if ((abs(i - nowx) == 0 && abs(j - nowy) == 1) || (abs(i - nowx) == 1 && abs(j - nowy) == 0))
		{
			if (map[nowx][nowy].Cleaned)
			{
				switch (map[i][j].type)
				{
				case Normal:
					a = "N";
					break;
				case Boss:
					a = "B";
					break;
				case Core:
					a = "C";
					break;
				case Store:
					a = "S";
					break;
				case SpawnPoint:
					a = "o";
					break;
				}
			}
			else
			{
				if (map[i][j].Cleaned)
				{
					switch (map[i][j].type)
					{
					case Normal:
						a = "N";
						break;
					case Boss:
						a = "B";
						break;
					case Core:
						a = "C";
						break;
					case Store:
						a = "S";
						break;
					case SpawnPoint:
						a = "o";
						break;
					}
				}
				else
					a = " ";
			}
		}
		else
		{
			if (i != nowx || j != nowy)
			{
				switch (map[i][j].type)
				{
				case Normal:
					if (map[i][j].Cleaned)
						a = "N";
					else
						a = " ";
					break;
				case Boss:
					if (map[i][j].Cleaned)
						a = "B";
					else
						a = " ";
					break;
				case Core:
					if (map[i][j].Cleaned)
						a = "C";
					else
						a = " ";
					break;
				case Store:
					if (map[i][j].Cleaned)
						a = "S";
					else
						a = " ";
					break;
				case SpawnPoint:
					a = "o";
					break;
				}
			}
			else
			{
				switch (map[i][j].type)
				{
				case Normal:
					a = "N";
					break;
				case Boss:
					a = "B";
					break;
				case Core:
					a = "C";
					break;
				case Store:
					a = "S";
					break;
				case SpawnPoint:
					a = "o";
					break;
				}
			}
		}
		if (map[i][j].Cleaned)
			font->SetColor(ARGB(120, 255, 255, 255));
		if (i == nowx && j == nowy)
			font->SetColor(ARGB(120, 120, 120, 255));
		font->Render(Startx + 50 * i, Starty + 50 * j, HGETEXT_CENTER, a);
		font->SetColor(ARGB(255, 255, 255, 255));
	}
}




void RenderPool::RenderHero(Hero *hero)
{
	

}

void RenderPool::InsertAnim(AnimObject *anim)
{
	anims.push_back(anim);
	anims.shrink_to_fit();
}

void RenderPool::RenderAnimations()
{
	for (int i = 0; i < anims.size(); i++)
	{
		for (int j = 0; j < ObjectPool::GetObjPool().pool.size(); j++)
		{
			GameObject *obj = ObjectPool::GetObjPool().pool[j];
			if (!obj->IsDead())
			{
				int nID = -1;
				int num = obj->GetInnerCode();
				for (int k = 0; k < animcombs.size(); k++)
				{
					if (animcombs[k].ObjectID == num)
					{
						nID = animcombs[k].imageID;
						break;
					}
				}
				if (nID != -1 && anims[i]->GetID() == nID)
				{
					switch (obj->GetInnerCode())
					{
					case HERO:
					{
								 int time = dynamic_cast<Hero*>(obj)->GetGodTimer();
					             if (time)
					             {
						            if (time % 5 == 0)
							        anims[i]->SetColor(ARGB((GETA(anims[i]->GetColor()) ? 0 : 255), 255, 255, 255));
					             }
					             else
						         anims[i]->SetColor(ARGB(255, 255, 255, 255));
					}
						break;
					}
					RenderOne(obj, anims[i]);
				}
			}
		}
		anims[i]->UpdateStep();
	}
}

void RenderPool::RenderOne(GameObject *vir, AnimObject *anim)
{
	anim->Render(vir->GetX(), vir->GetY(), vir->GetImgAngle(), vir->GetSize());
}

void RenderPool::InstantRender(float x,float y,int ID,float angle,float size,GameObject *host,float changeFPS)
{
	pos p;
	p.posx = x;
	p.posy = y;
	p.ID = ID;
	p.angle = angle;
	p.size = size;
	p.host = host;
	p.changedFPS = changeFPS;
	Instantpos.push_back(p);
}

void RenderPool::RenderDead()
{
	for (int i = 0; i < Instantpos.size(); i++)
	for (int j = 0; j < DeadAnimpool.size(); j++)
	{
		if (Instantpos[i].ID == DeadAnimpool[j]->GetID())
		{
			AnimObject *anim1;
			if (Instantpos[i].changedFPS)
			    anim1 = new AnimObject(DeadAnimpool[j],Instantpos[i].changedFPS);
			else
				anim1 = new AnimObject(DeadAnimpool[j]);
			anim1->ChangePos(Instantpos[i].posx, Instantpos[i].posy);
			anim1->SetAngle(Instantpos[i].angle);
			anim1->SetSize(Instantpos[i].size);
			AnimWithHost ah;
			ah.anim = anim1;
			ah.host = Instantpos[i].host;
			Instantpool.push_back(ah);
		}
	}
	Instantpos.clear();
	for (int i = 0; i < Instantpool.size(); i++)
	{
		if (!Instantpool[i].anim->Isfinish())
		{
			if (Instantpool[i].host)
			{
				if (Instantpool[i].host->IsDead())
				{
					Instantpool[i].host = NULL;
					Instantpool[i].anim->ForceToDie();
				}
				else
				{
					Instantpool[i].anim->ChangePos(Instantpool[i].host->GetX(), Instantpool[i].host->GetY());
					Instantpool[i].anim->Render();
				}
			}
			else
				Instantpool[i].anim->Render();
		}
	}
	bool deleted = false;
	for (int i = 0; i < Instantpool.size();)
	{
		if (Instantpool[i].anim->Isfinish())
		{
			deleted = true;
			delete Instantpool[i].anim;
			Instantpool[i].anim = NULL;
			Instantpool.erase(Instantpool.begin() + i);
		}
		else
			i++;
	}
	if (deleted)
		Instantpool.shrink_to_fit();
}




void RenderPool::InsertDeadAnim(AnimObject *anim)
{
	DeadAnimpool.push_back(anim);
	DeadAnimpool.shrink_to_fit();
}

void RenderPool::Shuffle()
{
	srand(time(NULL));
//	random_shuffle(anims.begin(), anims.end());
	random_shuffle(DeadAnimpool.begin(), DeadAnimpool.end());
}


/*
void RenderPool::RenderTokens()
{
	
	for (int j = 0; j < 7; j++)
	{
		int nID = -1;
		SKILLToken* skill=ObjectPool::GetObjPool().GetTokens(j);
	 int num2 = GetAnimID(skill);
		for (int i = 0; i < animcombs.size(); i++)
		{   
			if (animcombs[i].ObjectID == num2)
			{
				nID = animcombs[i].imageID;
				break;
			}
		}
		for (int i = 0; i < anims.size(); i++)
		{
			if (anims[i]->GetID() == nID)
			{
				RenderOne(dynamic_cast<GameObject*>(skill), anims[i]);
				break;
			}
		}
	}
}
*/


void RenderPool::InsertImageV2(const char* filename, float centerx, float centery, int IDforObj)
{
	int ID = combs.size();
	ImageObject *img = new ImageObject(filename, centerx, centery, ID);
	SetCombination(ID, IDforObj);
	InsertImage(img);
}

void RenderPool::InsertAnimV2(const char* animname, int frames, int fps, float w, float h, float cx, float cy, int IDforObj, bool isinfinte, bool isdeadanim)
{	
	if (!isdeadanim)
	{
		int ID = animcombs.size();
		AnimObject *anim = new AnimObject(animname, frames, fps, w, h, cx, cy, ID, isinfinte);
		SetCombination(ID, 0, true, IDforObj);
		InsertAnim(anim);
	}
	else
	{
		AnimObject *anim = new AnimObject(animname, frames, fps, w, h, cx, cy, IDforObj, isinfinte);
		InsertDeadAnim(anim);
	}
}

void RenderPool::SetCombinationV2(int IDforObject, int IDforObjectnew,bool isanim)
{
	if (isanim)
	{
		int a = -100;
		for (int i = 0; i < animcombs.size(); i++)
		{
			if (animcombs[i].ObjectID == IDforObject)
			{
				a = animcombs[i].imageID;
				break;
			}
		}
		if (a != -100)
			SetCombination(a, 0, true, IDforObjectnew);
	}
	else
	{
		int a = -100;
		for (int i = 0; i < combs.size(); i++)
		{
			if (combs[i].ObjectID == IDforObject)
			{
				a = combs[i].imageID;
				break;
			}
		}
		if (a != -100)
			SetCombination(a,IDforObjectnew);
	}
}

void RenderPool::InsertParticleV2(const char *psi,const char *pic,int ID,bool needchangeemission)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	HTEXTURE texture= hge->Texture_Load(pic);
	hgeSprite *pica = new hgeSprite(texture, 0, 0, hge->Texture_GetWidth(texture), hge->Texture_GetHeight(texture));
	pica->SetHotSpot(hge->Texture_GetWidth(texture) / 2, hge->Texture_GetHeight(texture) / 2);
		pica->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_ZWRITE);
	hgeParticleSystem *a = new hgeParticleSystem(psi, pica);
	hgeParticleSystemInfo info = a->info;
	if (needchangeemission)
	info.nEmission = 60;
	delete a;
	a = NULL;
	delete pica;
	pica=NULL;
	box b;
	b.imageID = particles.size();
	b.ObjectID = ID;
	particlecombs.push_back(b);
	particles.push_back(info);
	particlecombs.shrink_to_fit();
	particles.shrink_to_fit();
}

void RenderPool::SpawnPS(GameObject* obj,int ID)
{
	int imgID=-1;
	for (int i = 0; i < particlecombs.size();i++)
	if (particlecombs[i].ObjectID == ID)
	{
		imgID = particlecombs[i].imageID;
		break;
	}
	hgeParticleSystem *newsys = new hgeParticleSystem(&particles[imgID]);
	newsys->FireAt(obj->GetX(), obj->GetY());
	ParticleWithHost k;
	k.obj = obj;
	k.sys = newsys;
	particlepool.push_back(k);
}

void RenderPool::RenderParticles()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	for (int i = 0; i < particlepool.size(); i++)
	{
		if (particlepool[i].obj)
		{
			if (!particlepool[i].obj->IsDead())
			{
				particlepool[i].sys->SetScale(particlepool[i].obj->GetSize());
				particlepool[i].sys->Update(hge->Timer_GetDelta());
				particlepool[i].sys->MoveTo(particlepool[i].obj->GetX(), particlepool[i].obj->GetY());
				particlepool[i].sys->Render();
			}
			else
			{
				particlepool[i].obj = NULL;
				particlepool[i].sys->Stop();
			}
		}
		else
		{
			particlepool[i].sys->Update(hge->Timer_GetDelta());
			particlepool[i].sys->Render();
		}
	}
	bool deleted = false;
	for (int i = 0; i < particlepool.size();i++)
	if (!particlepool[i].sys->GetParticlesAlive())
	{
		deleted = true;
		delete particlepool[i].sys;
		particlepool[i].sys = NULL;
		particlepool.erase(particlepool.begin() + i);
	}
	if (deleted)
		particlepool.shrink_to_fit();
}



void RenderPool::RenderOnEveryObject()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	for (int k = 0; k < ObjectPool::GetObjPool().pool.size(); k++)
	{
		GameObject *obj = ObjectPool::GetObjPool().pool[k];
		float x = obj->GetX(), y = obj->GetY();
		int col = obj->GetCollisionRange();
		hge->Gfx_RenderLine(x, y, x + col, y);
		hge->Gfx_RenderLine(x, y, x - col, y);
		hge->Gfx_RenderLine(x, y, x, y + col);
		hge->Gfx_RenderLine(x, y, x, y - col);
	}
}
/*
void RenderPool::InsertMazePart(const char* filename,int partnum)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	HTEXTURE h = hge->Texture_Load(filename);
	ImageObject *aimg = new ImageObject(h, MAZEPARTSIZE / 2, MAZEPARTSIZE/2, partnum);
	box aa;
	aa.imageID = mazecombs.size();
	aa.ObjectID = partnum;
	mazecombs.push_back(aa);
	mazepool.push_back(aimg);
}

void RenderPool::RenderMaze()
{
	int tarx = ObjectPool::GetObjPool().startx, tary = ObjectPool::GetObjPool().starty;
	for (int k = 0; k < mazepool.size(); k++)
	{
		for (int i = tarx - 8; i < tarx + 8; i++)
		for (int j = tary - 6; j < tary + 6; j++)
		if (ObjectPool::GetObjPool().map[i][j]==1)
		{
			int num = ObjectPool::GetObjPool().getw(i, j);
			if (mazepool[k]->GetID() == num)
				mazepool[k]->render((i - tarx) * MAZEPARTSIZE + SCREEN_WIDTH / 2 + ObjectPool::GetObjPool().movex, (j - tary) * MAZEPARTSIZE + SCREEN_HEIGHT / 2 + ObjectPool::GetObjPool().movey);
		}
		else if (ObjectPool::GetObjPool().map[i][j] > 1)
		{
			int num = ObjectPool::GetObjPool().map[i][j];
			if (mazepool[k]->GetID() == num)
				mazepool[k]->render((i - tarx) * MAZEPARTSIZE + SCREEN_WIDTH / 2 + ObjectPool::GetObjPool().movex, (j - tary) * MAZEPARTSIZE + SCREEN_HEIGHT / 2 + ObjectPool::GetObjPool().movey);
		}
		
	}
}
*/