#include "NoticeUnit.h"


NoticeUnit::NoticeUnit( const char *titledata, const char *maindata)
{
	this->titledata = new char[strlen(titledata) + 1];
	strcpy_s(this->titledata, strlen(titledata) + 1, titledata);
	this->maindata = new char[strlen(maindata) + 1];
	strcpy_s(this->maindata, strlen(maindata) + 1, maindata);
}
NoticeUnit::NoticeUnit(const NoticeUnit &unit)
{
	titledata = new char[strlen(unit.titledata) + 1];
	strcpy_s(titledata, strlen(unit.titledata) + 1, unit.titledata);
	maindata = new char[strlen(unit.maindata) + 1];
	strcpy_s(maindata, strlen(unit.maindata) + 1, unit.maindata);
}

NoticeUnit::~NoticeUnit()
{
	
	SAFE_DELETE(titledata);
	SAFE_DELETE(maindata);
}
