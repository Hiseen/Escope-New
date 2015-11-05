#include "NoticeManager.h"


NoticeManager::NoticeManager()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	HTEXTURE lefttexture = hge->Texture_Load("left_notice.png");
	left = new hgeSprite(lefttexture, 0, 0, 32, 128);
	HTEXTURE righttexture = hge->Texture_Load("right_notice.png");
	right = new hgeSprite(righttexture, 0, 0, 32, 128);
	left->SetHotSpot(32, 64);
	right->SetHotSpot(0, 64);
	font = new hgeFont("font2.fnt");
	font2 = new hgeFont("font3.fnt");
	occupied = false;
	rightx = ACT_END_X;
	leftx = ACT_START_X;
	alpha = 255;
	m_bHasInitialized = true;
}


void NoticeManager::InsertNotice(const char* title, const char* main)
{
	NoticeUnit *a = new NoticeUnit(title, main);
	queue.push_back(a);
	SFXManager::GetSFXManager().PlaySFX(NOTICESFX);
}

void NoticeManager::Update()
{
	if (queue.size() > 0)
		occupied = true;
	else
		occupied = false;
	if (occupied)
	{
		left->Render(leftx, ACT_START_Y + (ACT_END_Y - ACT_START_Y) / 2);
		right->Render(rightx, ACT_START_Y + (ACT_END_Y - ACT_START_Y) / 2);
		const int distination1 = (ACT_END_X - ACT_START_X) / 2 + 128;
		const int distination2 = (ACT_END_X - ACT_START_X) / 2 - 128;
		if (rightx > distination1)
			rightx -= (rightx - distination1) / 10 + 1;
		if (leftx < distination2)
			leftx += (distination2 - leftx) / 10 + 1;
		if (rightx <= distination1 && leftx >= distination2)
		{
			rightx = distination1;
			leftx = distination2;
			right->SetColor(ARGB(alpha, 255, 255, 255));
			left->SetColor(ARGB(alpha, 255, 255, 255));
			font->SetColor(ARGB(alpha, 255, 255, 255));
			font2->SetColor(ARGB(alpha, 255, 255, 255));
			font->Render((ACT_END_X - ACT_START_X) / 2, ACT_START_Y + (ACT_END_Y - ACT_START_Y) / 2-48, HGETEXT_CENTER, queue[0]->GetTitle());
			font2->Render((ACT_END_X - ACT_START_X) / 2, ACT_START_Y + (ACT_END_Y - ACT_START_Y) / 2, HGETEXT_CENTER, queue[0]->GetMainData());
			alpha-=3.5;
		}
		if (alpha <= 0)
		{
			delete queue[0];
			queue.erase(queue.begin());
			queue.shrink_to_fit();
			rightx = ACT_END_X;
			leftx = ACT_START_X;
			alpha = 255;
			right->SetColor(ARGB(255, 255, 255, 255));
			left->SetColor(ARGB(255, 255, 255, 255));
		}

	}



}


NoticeManager::~NoticeManager()
{
}
