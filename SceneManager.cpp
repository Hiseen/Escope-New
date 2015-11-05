#include "SceneManager.h"





SceneManager::~SceneManager()
{
}

void SceneManager::GoToOneScene(SceneNumber scenenumber,bool isneedclean,bool needexceptclean)
{
	needclean = isneedclean;
	this->needexceptclean = needexceptclean;
	currentscene = scenenumber;
};


void SceneManager::CleanOneScene(bool &initclean)
{
	initclean = false;
	if (needclean)
	{
		needclean = false;
		ObjectPool::GetObjPool().ResetAll();
	}
	if (needexceptclean)
	{
		needexceptclean = false;
		ObjectPool::GetObjPool().ExceptClean();
	}
}