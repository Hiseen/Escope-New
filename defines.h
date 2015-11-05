#pragma once
#include "hge.h"
#include <math.h>
#include <time.h>
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif

#define GAME_FPS 50

enum ObjectType
{
	Left, Right, None
};


enum GameMode
{
	Undecided, MazeMode, WaveMode, StageMode, MazeEvent
};


enum SceneNumber
{
	LoadingScene, MenuScene, GameScene, PauseScene, GameOverScene, HelpScene, LogoScene, MapScene, LevelUpScene
};



struct Point
{
	Point()
	{
		x = 0; y = 0;
	}
	float x, y;
	void  MakePoint(float x1, float y1, float dx = 0, float dy = 0)
	{
		x = x1 - dx;
		y = y1 - dy;
	}
	void CorrectPoint(float centerx, float centery)
	{
		x -= centerx;
		//y += centery;
	}
};
struct BWpic
{
	bool **bools;
	int number;
	int width;
	int height;
};
static void getRange(int *arr, int size, int *max, int *min)
{
	int *p = arr, *endp = arr + size;
	if (size % 2 != 0)
	{
		*max = *min = *p++;
	}
	else
	{
		if (*p > *(p + 1))
		{
			*max = *p;
			*min = *(p + 1);
		}
		else
		{
			*max = *(p + 1);
			*min = *p;
		}
		p += 2;
	}
	while (p != endp)
	{
		if (*p > *(p + 1))
		{
			if (*p > *max) *max = *p;
			if (*(p + 1) < *min) *min = *(p + 1);
		}
		else
		{
			if (*(p + 1) > *max) *max = *(p + 1);
			if (*(p) < *min) *min = *p;
		}
		p += 2;
	}
};

static bool Collision(Point *mPoint, const int count, const float angle5, float center_x, float center_y, float px, float py)
{
	int count_point = count;
	Point cmpoint[10];
	memcpy(cmpoint, mPoint, count_point * sizeof(Point));
	for (int i = 0; i < count_point; i++)
	{
		float xx1 = cmpoint[i].x, yy1 = cmpoint[i].y;
		if (angle5>-4.71 || angle5 < -7.85) yy1 = -yy1;
		cmpoint[i].x = cos(angle5)*xx1 - sin(angle5)*yy1 + center_x;
		cmpoint[i].y = cos(angle5)*yy1 + sin(angle5)*xx1 + center_y;
	}
	/*
	HGE *hge = hgeCreate(HGE_VERSION);
	hge->Gfx_BeginScene();
	for (int i = 0; i < count_point; i++)
	{
	if (i != count_point - 1)
	hge->Gfx_RenderLine(cmpoint[i].x, cmpoint[i].y, cmpoint[i + 1].x, cmpoint[i + 1].y, ARGB(255, 255, 0, 0));
	else
	hge->Gfx_RenderLine(cmpoint[i].x, cmpoint[i].y, cmpoint[0].x, cmpoint[0].y,ARGB(255, 255, 0, 0));
	}
	hge->Gfx_EndScene();
	*/
	for (int i = 0; i < count_point; i++)
	{
		if (i != count_point - 1)
		{
			float k = 0;
			if (cmpoint[i].x - cmpoint[i + 1].x != 0) k = (cmpoint[i].y - cmpoint[i + 1].y) / (cmpoint[i].x - cmpoint[i + 1].x);
			else  k = 10000000;
			int bcs[10], maxs, mins;
			float dx = py - px*k;
			for (int j = 0; j < count_point; j++)
				bcs[j] = cmpoint[j].y - cmpoint[j].x*k;
			getRange(bcs, count_point, &maxs, &mins);
			if (dx>maxs || dx < mins)	return false;
		}
		else
		{
			float k = 0;
			if (cmpoint[i].x - cmpoint[0].x != 0) k = (cmpoint[i].y - cmpoint[0].y) / (cmpoint[i].x - cmpoint[0].x);
			else  k = 10000000;
			int bcs[10], maxs, mins;
			float dx = py - px*k;
			for (int j = 0; j < count_point; j++)
				bcs[j] = cmpoint[j].y - cmpoint[j].x*k;
			getRange(bcs, count_point, &maxs, &mins);
			if (maxs > dx && dx > mins) return true;
			else return false;
		}
	}
}

static bool ReturnValid(int &count, int max, int min)
{
	if (count <= 0)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		count = hge->Random_Int(max, min);
		return true;
	}
	else
	{
		count--;
		return false;
	}
}

static bool CollisionBW(BWpic bools, float Targetx, float Targety, double angle, float myselfx, float myselfy, float size)
{
	if (bools.bools != nullptr)
	{
		double angle1 = -angle;
		//HGE *hge = hgeCreate(HGE_VERSION);
		//HWND hwnd=hge->System_GetState(HGE_HWND);
		//HDC dc = GetDC(hwnd);
		//for (int i = 0; i < bools.width;i++)
		//for (int j = 0; j < bools.height;j++)
		//if (bools.bools[i][j])SetPixel(dc, i, j, RGB(0, 0, 0));
		//else
		//	SetPixel(dc, i, j, RGB(255, 255, 255));
		int fx, fy;
		int x = (Targetx - myselfx);// +bools.width / 2;
		int y = (Targety - myselfy);// +bools.height / 2;
		fx = x*cos(angle1) - y*sin(angle1);
		fy = y*cos(angle1) + x*sin(angle1);
		fx /= size;
		fy /= size;
		fx += (bools.width / 2);
		fy += (bools.height / 2);
		//SetPixel(dc, fx, fy, RGB(50, 50, 50));
		if (fx<0 || fy<0 || fx>bools.width - 1 || fy>bools.height - 1)
			return false;
		else
			return bools.bools[fx][fy];
	}
	else
		return false;
}
static bool ballcrash(float center_x, float center_y, int r, float px, float py)
{
	int d1 = px - center_x;
	int d2 = py - center_y;
	if (r*r < d1*d1 + d2*d2)return false;
	else return true;
}
/*
DWORD WINAPI ThreadProc(LPVOID)
{
HANDLE hEvent = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);
::WaitForSingleObject(hEvent, INFINITE);
::CloseHandle(hEvent);
return 0L;
}
*/

static void AddUntil(int &stage, int &charge, int max, bool &finish)
{
	if (charge < max)
	{
		charge++;
	}
	else
	{
		charge = 0;
		finish = true;
	}
}

static void random(int a[], int n)
{
	int index, tmp, i;
	srand(time(NULL));
	for (i = 0; i <n; i++)
	{
		index = rand() % (n - i) + i;
		if (index != i)
		{
			tmp = a[i];

			a[i] = a[index];

			a[index] = tmp;
		}
	}
}

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 1280
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 720
#endif

#define ACT_START_X 0
#define ACT_END_X SCREEN_WIDTH
#define ACT_START_Y 150
#define ACT_END_Y SCREEN_HEIGHT
#define ACT_CENTER_X ACT_START_X*0.5+ACT_END_X*0.5
#define ACT_CENTER_Y ACT_START_Y*0.5+ACT_END_Y*0.5

#define CRYSTAL -1
#define GEM -2
#define BATTERY -3

#define ITEM_HEAL_HP -4
#define ITEM_HEAL_MP -5
#define ITEM_ADDDMG -6
#define ITEM_SHUFFLEHERO -7
#define ITEM_START ITEM_SHUFFLEHERO
#define ITEM_END ITEM_HEAL_HP


#define HERO 1
#define BULLET 2
#define BOOM 3
#define ITEM 4
#define CORESKILL 7
#define COREBULLET 8
#define MISSILE 9
#define MISSILE2 10
//ArmorBreak
#define ABBULLET 13
//Toxic
#define TBULLET 14
//Weak
#define WBULLET 15
//EMP
#define EMPBULLET 16
//Bomb
#define BBULLET 17
#define TONS_OF_BBULLET 18
#define TONS_OF_EMPBULLET 19
#define TONS_OF_TBULLET 20
#define TONS_OF_ABBULLET 21
#define TONS_OF_WBULLET 22
#define ENEMYBULLET 23

#define TRACKBULLET 29
#define LAZER 30
#define MISSILES 31
#define DISPERSEBULLET 32
#define TBOMB 33

#define TOXICFIELD 42
#define ENEMYBOUNCEARROW 43
#define THROWBULLET 44
#define AIMBULLET 45

#define BOSSSKILL1 60
#define BOSSSKILL2 61
#define BOSSSKILL3 62
#define BOSSSKILL4 63
#define BOSSSKILL5 64
#define BOSSSKILL6 65
#define BOSSSKILL7 66
#define BOSSSKILL8 67



#define ENEMYSKILL 90
#define ENEMYSKILL2 91
#define ENEMYSKILL3 92
#define ENEMYSKILL4 93
#define ENEMYSKILL5 94
#define ENEMYSKILL6 95


//核心们
// I型：
//攻击类
#define CORE_NORMAL 100
#define CORE_SHOTGUN 101
#define CORE_LAZER 102
#define CORE_MISSILE 103
// Buff类
#define CORE_HEALHP 104
#define CORE_ATK 105
#define CORE_HARDSKIN 106
#define CORE_SHOOTCD 107
#define CORE_SPEED 108
#define CORE_SHIELD 109
#define CORE_HEALMP 110
#define CORE_GETBLOOD 111
#define CORE_UPDATE 112
#define CORE_GETMP 113
// 技能类
#define CORE_ICE 114
#define CORE_BOOM 115
#define CORE_CLEAN 116
#define CORE_RUNEBOMB 117
#define CORE_GETSHIELD 118
#define CORE_LIGHTARROW 119
#define CORE_BOUNCEARROW 120
#define CORE_TRACKBOMB 121
#define CORE_FLASH 122
#define CORE_HEAL 123
//永久类
#define CORE_ADDMAXHP 124
#define CORE_ADDDMG 125
#define CORE_ADDMPHEAL 126
#define CORE_ADDSPEED 127

#define CORE_START CORE_NORMAL
#define CORE_END CORE_ADDSPEED
// II型：
// 暂无

#define ENEMY_NORMAL 200
#define ENEMY_BULLET 201
#define ENEMY_HIGHSPEED 202
#define ENEMY_FLASH 203
#define ENEMY_DOUBLE 204
#define ENEMY_DASHER 205
#define ENEMY_ZOMBLE 206
#define ENEMY_BOMBER 207
#define ENEMY_BULLET_BOMBER 208
#define ENEMY_SHIELD 209
#define ENEMY_GUARD1 210
#define ENEMY_GUARD2 211
#define ENEMY_GUARD3 212
#define ENEMY_FLASH2 213
#define ENEMY_BIGBOMB 214
#define ENEMY_SUMMONER 215
#define ENEMY_DASHER2 216
#define ENEMY_START ENEMY_NORMAL
#define ENEMY_END ENEMY_DASHER2


#define ENEMY_ELITE 250

#define ENEMY_BOSS_I 260

//特殊敌人：
#define ENEMY_RECT 280

//激活技能 +300
#define ICE 414
#define SELFBOOM 415
#define CLEAN 416
#define RUNEBOMB 417
#define GETSHIELD 418
#define LIGHTARROW 419
#define BOUNCEARROW 420
#define TRACKBOMB 421
#define FLASH 422
#define HEAL 423
//合成技能
#define ICE_TRACKBOMB 424
#define HEAL_TRACKBOMB 425
#define BOUNCEARROW_ICE 426
#define BOUNCEARROW_LIGHTARROW 427
#define ICE_RUNEBOMB 428
#define ICE_BOOM 429







//技能图标 +600
#define ICON_HEALHP 704
#define ICON_ATK 705
#define ICON_HARDSKIN 706
#define ICON_SHOOTCD 707
#define ICON_SPEED 708
#define ICON_SHIELD 709
#define ICON_HEALMP 710
#define ICON_GETBLOOD 711
#define ICON_UPDATE 712
#define ICON_GETMP 713

#define ICON_ICE 714
#define ICON_SELFBOOM 715
#define ICON_CLEAN 716
#define ICON_RUNEBOMB 717
#define ICON_GETSHIELD 718
#define ICON_LIGHTARROW 719
#define ICON_BOUNCEARROW 720
#define ICON_TRACKBOMB 721
#define ICON_FLASH 722
#define ICON_HEAL 723

#define ICON_ICE_TRACKBOMB 724
#define ICON_HEAL_TRACKBOMB 725
#define ICON_BOUNCEARROW_ICE 726
#define ICON_BOUNCEARROW_LIGHTARROW 727
#define ICON_ICE_RUNEBOMB 728
#define ICON_ICE_BOOM 729