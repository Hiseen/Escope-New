#pragma once
#include "hgesprite.h"
#ifndef IMAGEOBJECT_H_
#define IMAGEOBJECT_H_
#define BORDERX 10
#define BORDERY 1

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif
class ImageObject
{
public:
	ImageObject(
		const char* filename_for_tex,
		float center_x,
		float center_y,
		int ID1 = 0,
		float width = 0,
		float height=0
		);
	ImageObject(
		HTEXTURE tex,
		float center_x,
		float center_y,
		int ID1 = 0
		);
	~ImageObject();
	ImageObject(const ImageObject &obj);
	int GetID()const;
	void CopyData(int x,int y,HTARGET data);
	void boxblur(int radius);
	void render(float x,float y,double angle=0,double size=1.0);
	void RenderBars(float x, float y, float varh, float varw);
	void MesteryBlur(int x,int y,int r);
	void SetZ(float newz){ image->SetZ(newz); };
	void ForceToDie()
	{
		SAFE_DELETE(image);
	}
	void SetAlpha(int newalpha)
	{
		image->SetColor(ARGB(newalpha, 255, 255, 255));
	}
	void SetColor(DWORD newcolor,bool ncbm)
	{
		if(ncbm)image->SetBlendMode(BLEND_COLORADD | BLEND_ALPHABLEND);
		image->SetColor(newcolor);
	}
	void Reset();
private:
	HTEXTURE texture;
	hgeSprite *image;
	int ID;
	
};
#endif
/*
HTEXTURE boxblur(HTEXTURE in, int radius)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	int width, height;
	width = hge->Texture_GetWidth(in);
	height = hge->Texture_GetHeight(in);

	int widthMinus1 = width - 1;
	int tableSize = 2 * radius + 1;
	unsigned int* divide = new unsigned int[256 * tableSize];

	for (int i = 0; i < 256 * tableSize; i++)
		divide[i] = i / tableSize;

	int inIndex = 0;

	unsigned int* in_ = (unsigned int*)hge->Texture_Lock(in, false);

	for (int y = 0; y < height; y++) {
		int outIndex = y;
		unsigned int ta = 0, tr = 0, tg = 0, tb = 0; 
		for (int i = -radius; i <= radius; i++)
		{
			int clamp = (i < 0) ? 0 : (i >(width - 1)) ? (width - 1) : i;
			unsigned int rgb = in_[inIndex + clamp]; 
			ta += (rgb >> 24) & 0xff;
			tr += (rgb >> 16) & 0xff;
			tg += (rgb >> 8) & 0xff;
			tb += rgb & 0xff;
		}

		for (int x = 0; x < width; x++)
		{
			in_[outIndex*width+x] = (divide[ta] << 24) | (divide[tr] << 16) | (divide[tg] << 8) | divide[tb]; 
			int i1 = x + radius + 1;
			if (i1 > widthMinus1)
				i1 = widthMinus1;
			int i2 = x - radius;
			if (i2 < 0)
				i2 = 0;
			int rgb1 = in_[inIndex + i1];
			int rgb2 = in_[inIndex + i2];

			ta += ((rgb1 >> 24) & 0xff) - ((rgb2 >> 24) & 0xff);
			tr += ((rgb1 & 0xff0000) - (rgb2 & 0xff0000)) >> 16;
			tg += ((rgb1 & 0xff00) - (rgb2 & 0xff00)) >> 8;
			tb += (rgb1 & 0xff) - (rgb2 & 0xff);
		}
		inIndex += width; 
	}

	hge->Texture_Unlock(in);
	delete[] divide;
	return in;
}
*/






