#include "DMGFont.h"

DMGFont::DMGFont(int num, float x1, float y1, Type dmg)
{
	HGE *hge = hgeCreate(HGE_VERSION);
		size = 7.8;
	alpha = 1;
	if (dmg != Crit)
		finalsize = 1;
	else
		finalsize = 1.3;
	x = x1 + hge->Random_Int(-25, 25);
	y = y1 + hge->Random_Int(-25, 25);
	DmgType = dmg;
	if (num > 0)
		DmgType = Heal;
	else
		num = abs(num);
	data=num;
	alive = true;
	finishfirst = false;
}


DMGFont::~DMGFont()
{

}

void DMGFont::Action()
{
	if (!finishfirst)
	{
		if (alpha < 255)
		{
			alpha+=15;
			size-=0.4;
		}
		else
		{
			alpha = 255;
			size = finalsize;
			finishfirst = true;
		}
	}
	else
	{
		y--;
		alpha-=5;
	}
	if (alpha == 0)
		alive = false;
}