
#include <afxwin.h>
#include "FontPool.h"
#include"atlimage.h"
#include<math.h>
#include<MMSystem.h>
#include<vector>
#include<malloc.h>
#include <dshow.h>
#include <fstream> 
#include <map>
#include "defines.h"

#define SPAWN_WACUUM 100

HGE *hge = 0;

#pragma comment(lib,"legacy_stdio_definitions.lib")
#pragma comment(lib,"strmiids.lib")
#pragma comment(lib,"quartz.lib")
#pragma comment(lib,"hge.lib")
#pragma comment(lib,"hgehelp.lib")
#pragma comment(lib,"winmm.lib")
using namespace std;

#include <vld.h>
#include "NoticeManager.h"
#include "RenderPool.h"
#include"SFXManager.h"
#include "Skill.h"
#include "Enemy.h"
#include "Item.h"
#include "hgeresource.h"
// 1920 1080
// 1280 720


//控制多线程的全局变量




CWinThread* pThread2;

volatile bool stop = false;
bool GameStart = false;
bool wavestart = false;
bool havespawned = false;
int spawntype = 0;
float x=0, y=0;
int i = 20;
int test = ENEMY_BOSS_I;
int spawnenemytype = 0;
int spawntimer=0;

DWORD  ThreadFunc2(LPVOID lpraram)
{
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);//得到程序模块名称，全路径
	*_tcsrchr(exeFullPath, _T('\\')) = 0;
	CString str2 = exeFullPath;
	IGraphBuilder * pGraph = NULL;
	IMediaControl * pControl = NULL;
	IMediaEvent * pEvent = NULL;
	//改动了！
	IVideoWindow* m_pVidWin = NULL;
	HWND m_hwnd;
	m_hwnd = hge->System_GetState(HGE_HWND);
	//不支持com组件的就会失败!
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))MessageBox(NULL, "不支持COM组件的初始化，导致动画未被载入", "初始化错误！", MB_OK);
	//可能会失败!
	CString strx, stry;
	strx = "\\Gecore Final.mov";
	stry = str2 + strx;
	USES_CONVERSION;
	LPCWSTR pwcStr = A2CW((LPCSTR)stry);
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
	hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
	hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&m_pVidWin);
	hr = pGraph->RenderFile(pwcStr, NULL);
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	hr = m_pVidWin->SetWindowPosition(0, 0, rc.right, rc.bottom);
	hr = m_pVidWin->put_Owner((OAHWND)m_hwnd);
	hr = m_pVidWin->put_MessageDrain((OAHWND)m_hwnd);
	hr = m_pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	if (SUCCEEDED(hr))
	{
		hr = pControl->Run();
		if (SUCCEEDED(hr))
		{
			long evCode;
			pEvent->WaitForCompletion(INFINITE, &evCode);
		}
	}
	pControl->Release();
	pEvent->Release();
	pGraph->Release();
	m_pVidWin->Release();
	CoUninitialize();
	AfxEndThread(2000);
	return 0;
}



DWORD  ThreadFunc(LPVOID lpraram)
{
	RenderPool::GetRenderPool().InsertAnimV2("heroship.png", 1, 1, 50, 50, 25, 25, HERO, true);
	//RenderPool::GetRenderPool().InsertAnimV2("bullet.png", 1, 1, 50, 50, 25, 25, ENEMY_TANK, true);
	RenderPool::GetRenderPool().InsertAnimV2("item.png", 1, 1, 50, 50, 25, 25, ENEMY_BULLET, true);
	RenderPool::GetRenderPool().SetCombinationV2(ENEMY_BULLET, ENEMY_ELITE, true);
	RenderPool::GetRenderPool().InsertAnimV2("herobullet.png", 1, 1, 32, 32, 16, 16, BULLET, true);
	RenderPool::GetRenderPool().SetCombinationV2(BULLET, MISSILE,true);
	RenderPool::GetRenderPool().SetCombinationV2(BULLET, MISSILE2, true);
	RenderPool::GetRenderPool().SetCombinationV2(BULLET, ABBULLET, true);
	RenderPool::GetRenderPool().SetCombinationV2(BULLET, BBULLET, true);
	RenderPool::GetRenderPool().SetCombinationV2(BULLET, TBULLET, true);
	RenderPool::GetRenderPool().SetCombinationV2(BULLET, WBULLET, true);
	RenderPool::GetRenderPool().SetCombinationV2(BULLET, EMPBULLET, true);
	RenderPool::GetRenderPool().SetCombinationV2(BULLET, ENEMYBULLET, true);
	RenderPool::GetRenderPool().SetCombinationV2(BULLET, ENEMYBOUNCEARROW, true);
	RenderPool::GetRenderPool().InsertAnimV2("trackbullet.png", 1, 1, 50, 50, 25, 25, TRACKBULLET, true);
	RenderPool::GetRenderPool().InsertAnimV2("robot_normal.png", 1, 1, 32, 32, 16, 16,CORE_NORMAL, true);
	RenderPool::GetRenderPool().SetCombinationV2(CORE_NORMAL, CORE_LAZER, true);
	RenderPool::GetRenderPool().SetCombinationV2(CORE_NORMAL, CORE_MISSILE, true);
	RenderPool::GetRenderPool().SetCombinationV2(CORE_NORMAL, CORE_SHOTGUN, true);
	RenderPool::GetRenderPool().SetCombinationV2(CORE_NORMAL, CORE_HEALHP, true);
	RenderPool::GetRenderPool().SetCombinationV2(CORE_NORMAL, CORE_ATK, true);
	RenderPool::GetRenderPool().SetCombinationV2(CORE_NORMAL, CORE_SHIELD, true);
	RenderPool::GetRenderPool().SetCombinationV2(CORE_NORMAL, CORE_HEALMP, true);
	RenderPool::GetRenderPool().SetCombinationV2(CORE_NORMAL, CORE_SPEED, true);
	RenderPool::GetRenderPool().SetCombinationV2(CORE_NORMAL, CORE_SHOOTCD, true);
	RenderPool::GetRenderPool().InsertAnimV2("robotbullet.png", 1, 1, 32, 32, 16, 16, COREBULLET, true);
	RenderPool::GetRenderPool().SetCombinationV2(COREBULLET, DISPERSEBULLET, true);
	RenderPool::GetRenderPool().SetCombinationV2(COREBULLET, THROWBULLET, true);
	RenderPool::GetRenderPool().InsertAnimV2("MPrecover.png", 1, 1, 32, 32, 16, 16,ENEMY_NORMAL, true);
	for (int i = ENEMY_NORMAL + 1; i < ENEMY_END;i++)
		RenderPool::GetRenderPool().SetCombinationV2(ENEMY_NORMAL, i, true);
	//RenderPool::GetRenderPool().InsertAnimV2("block.png", 1, 1, 50, 50, 25, 25, ENEMY_SHOTGUN, true);
	RenderPool::GetRenderPool().InitMap();
	AfxEndThread(1000);
	return 0;
}




bool SpawnEnemies(int spawntype,float &x,float &y,int &i,int spawnenemytype=0)
{
	switch (spawntype)
	{
	case 0:
		if (i > 0)
		{
			if (!x)
				x = ACT_START_X + (ACT_END_X - ACT_START_X)*hge->Random_Float(1.2, 1.5);
			if (!y)
				y = 20;
			else
				y += (ACT_END_Y - ACT_START_Y - 20) / 9;
			Enemy *enemy = new Enemy(i,0,0);
			//enemy->SetDelayAction(SPAWN_WACUUM * (9 - i));
			ObjectPool::GetObjPool().InsertObject(enemy);
		}
		else
		{
			i = 20+SceneManager::GetSceneManager().GetWave();
			havespawned = true;
			return false;
		}
		break;
	case 1:

		break;
	case 2:
		break;

	}
	return true;
}



bool RenderFunc()
{
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0xFF000000);
	switch (SceneManager::GetSceneManager().GetCurrentScene())
	{
	case GameScene:
		switch (SceneManager::GetSceneManager().GetCurrentMode())
		{
		case MazeMode:
			RenderPool::GetRenderPool().RenderMap();
			break;
		case MazeEvent:
			RenderPool::GetRenderPool().RenderEveryObject();
			RenderPool::GetRenderPool().RenderAnimations();
			RenderPool::GetRenderPool().RenderOnEveryObject();
			FontPool::GetFontPool().RenderFont();
			NoticeManager::GetNoticeManager().Update();
			break;
		}
	}
	hge->Gfx_EndScene();
	return false;
}

bool FrameFunc()
{
	switch (SceneManager::GetSceneManager().GetCurrentScene())
	{
	case GameScene:
		if (!GameStart)
		{
			Hero *newhero1 = new Hero(Left);
			ObjectPool::GetObjPool().InsertObject(newhero1);
			GameStart = true;
		}
		if (hge->Input_GetKeyState(HGEK_W))
			stop = true;
		switch (SceneManager::GetSceneManager().GetCurrentMode())
		{
		case MazeEvent:
			SceneManager::GetSceneManager().GameTime++;
			if (!wavestart)
			{
				if (!havespawned && (RenderPool::GetRenderPool().GetNowRoom().type != Store ? !RenderPool::GetRenderPool().GetNowRoom().Cleaned : RenderPool::GetRenderPool().GetNowRoom().Customdata))
				{
					ObjectPool::GetObjPool().GetOurHero(Left)->UnScilence();
					wavestart = true;
					RenderPool::GetRenderPool().CleanNowRoom();
					if (RenderPool::GetRenderPool().GetNowRoom().type != Store)
						SceneManager::GetSceneManager().WaveClear();
				}
				if (RenderPool::GetRenderPool().GetNowRoom().type != Store &&RenderPool::GetRenderPool().GetNowRoom().type != SpawnPoint)
				{
					if (ObjectPool::GetObjPool().IsStageClear() && ObjectPool::GetObjPool().HeroCoreBack() && hge->Input_KeyUp(HGEK_M))
					{
						SceneManager::GetSceneManager().SetGameMode(MazeMode);
						ObjectPool::GetObjPool().EndOfWaveClean();
					}
				}
				else
				{
					if (ObjectPool::GetObjPool().HeroCoreBack() && hge->Input_KeyUp(HGEK_M))
					{
						SceneManager::GetSceneManager().SetGameMode(MazeMode);
						ObjectPool::GetObjPool().EndOfWaveClean();
					}
				}
			}
			else
			{
				//Spawn Monsters
				switch (RenderPool::GetRenderPool().GetNowRoom().type)
				{
				case Normal:
					if (spawntimer <= SPAWN_WACUUM)
						spawntimer++;
					else
					{
						wavestart = SpawnEnemies(spawntype, x, y, i, spawnenemytype);
						spawntimer = 0;
					}
					break;
				case Core:
				{
							 int a = 0;
							 Enemy *enemy = new Enemy(a, 0, 0, ENEMY_ELITE);
				ObjectPool::GetObjPool().InsertObject(enemy);
				wavestart = false;
				havespawned = true;
				}
					break;
				case Store:
				{
							  Item *item = new Item(RenderPool::GetRenderPool().GetNowRoom().Customdata);
							  ObjectPool::GetObjPool().InsertObject(item);
							  wavestart = false; 
							  havespawned = true;
				}
					break;
				default:
					wavestart = false;
					havespawned = true;
				}
				
			}
			ObjectPool::GetObjPool().SkillEffect();
			ObjectPool::GetObjPool().UpdateObject();
			ObjectPool::GetObjPool().DelectObject();
			FontPool::GetFontPool().Delete();
			break;
		case MazeMode:
			if (hge->Input_KeyDown(HGEK_W))
				RenderPool::GetRenderPool().MoveNowRoom(0, -1);
			if (hge->Input_KeyDown(HGEK_S))
				RenderPool::GetRenderPool().MoveNowRoom(0, 1);
			if (hge->Input_KeyDown(HGEK_A))
				RenderPool::GetRenderPool().MoveNowRoom(-1, 0);
			if (hge->Input_KeyDown(HGEK_D))
				RenderPool::GetRenderPool().MoveNowRoom(1, 0);
			if (hge->Input_KeyDown(HGEK_SPACE))
			{
				RenderPool::GetRenderPool().EnterNowRoom();
				havespawned = false;
			}
			break;
		}
		break;
	}
	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);
	hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);//屏幕宽度 
	hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);//屏幕高度
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);//设置逻辑函数为FrameFunc函数 
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);//设置绘制函数为RenderFunc函数 
	hge->System_SetState(HGE_TITLE, "EScopeX");
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SHOWSPLASH, false);
	hge->System_SetState(HGE_HIDEMOUSE, false);
	hge->System_SetState(HGE_USESOUND, true);
	hge->System_SetState(HGE_FPS, GAME_FPS);
	hge->System_SetState(HGE_LOGFILE, "GAME.log");
	if(hge->System_Initiate())
	{
		CWinThread* pThread = AfxBeginThread((AFX_THREADPROC)ThreadFunc, NULL);
		TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
		GetModuleFileName(NULL, exeFullPath, MAX_PATH);//得到程序模块名称，全路径
		*_tcsrchr(exeFullPath, _T('\\')) = 0;
		CString str2 = exeFullPath;
		IGraphBuilder * pGraph = NULL;
		IMediaControl * pControl = NULL;
		IMediaEvent * pEvent = NULL;
		//改动了！
		IVideoWindow* m_pVidWin = NULL;
		HWND m_hwnd;
		m_hwnd = hge->System_GetState(HGE_HWND);
		//不支持com组件的就会失败!
		HRESULT hr = CoInitialize(NULL);
		if (FAILED(hr))MessageBox(NULL, "不支持COM组件的初始化，导致动画未被载入", "初始化错误！", MB_OK);
		//可能会失败!
		CString strx, stry;
		strx = "\\Gecore Final.mov";
		stry = str2 + strx;
		USES_CONVERSION;
		LPCWSTR pwcStr = A2CW((LPCSTR)stry);
		hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
		hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
		hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
		hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&m_pVidWin);
		hr = pGraph->RenderFile(pwcStr, NULL);
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		hr = m_pVidWin->SetWindowPosition(0, 0, rc.right, rc.bottom);
		hr = m_pVidWin->put_Owner((OAHWND)m_hwnd);
		hr = m_pVidWin->put_MessageDrain((OAHWND)m_hwnd);
		hr = m_pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
		if (SUCCEEDED(hr))
		{
			hr = pControl->Run();
			if (SUCCEEDED(hr))
			{
				long evCode;
				pEvent->WaitForCompletion(INFINITE, &evCode);
			}
		}

		pControl->Release();
		pEvent->Release();
		pGraph->Release();
		m_pVidWin->Release();
		CoUninitialize();
		DWORD exitcode;
		while (GetExitCodeThread(pThread->m_hThread, &exitcode))
		{
			if (exitcode == 1000)break;
		}
		hge->System_Start();
	}
	else
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}



	ObjectPool::GetObjPool().Exit();
	RenderPool::GetRenderPool().Exit();
	FontPool::GetFontPool().Exit();
	NoticeManager::GetNoticeManager().Exit();
	SFXManager::GetSFXManager().Exit();
	hge->System_Shutdown();
	hge->Release();

	return 0;
}
