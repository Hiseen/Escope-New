#pragma once
#include "hge.h"
#include "hgesprite.h"
#include "hgefont.h"
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif
class NoticeUnit
{
public:
	NoticeUnit(const char *titledata,const char *maindata);
	NoticeUnit(const NoticeUnit &unit);
	const char* GetTitle()const{ return titledata; };
	const char* GetMainData()const{ return maindata; };
	~NoticeUnit();
private:
	char* titledata;
	char* maindata;
};

