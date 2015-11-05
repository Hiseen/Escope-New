#include "ImageObject.h"



ImageObject::ImageObject(
	const char* filename_for_tex,
	float center_x,
	float center_y,
	int ID1,
	float width,
	float height
	)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	texture = hge->Texture_Load(filename_for_tex);
	if(!width)width = hge->Texture_GetWidth(texture);
	if(!height)height = hge->Texture_GetHeight(texture);
	image = new hgeSprite(texture, 0, 0, width, height);
	image->SetHotSpot(center_x,center_y);
	ID = ID1;
}

ImageObject::ImageObject(HTEXTURE tex, float center_x, float center_y, int ID1)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	texture = tex;
	image = new hgeSprite(texture, 0, 0, hge->Texture_GetWidth(texture),hge->Texture_GetHeight(texture));
	image->SetHotSpot(center_x, center_y);
	ID = ID1;
}



ImageObject::~ImageObject()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	hge->Texture_Free(texture);
	ForceToDie();
}


ImageObject::ImageObject(const ImageObject &obj)
{
	image = obj.image;
	ID = obj.ID;
	texture = obj.texture;
}

int ImageObject::GetID()const
{
	return ID;
}

void ImageObject::render(float x, float y, double angle,double size)
{
	image->RenderEx(x, y, angle, size, size);
}

void ImageObject::RenderBars(float x,float y,float varh, float varw)
{
	//image->SetTextureRect(0, 0, image->GetWidth()*varw, image->GetHeight()*varh);
	image->RenderStretch(x, y, x+image->GetWidth()*varw, y+image->GetHeight()*varh);
}

void ImageObject::Reset()
{
	image->SetTextureRect(0, 0, image->GetWidth(), image->GetHeight());
}



void ImageObject::boxblur(int radius)
{
	HTEXTURE in = texture;
	HGE *hge = hgeCreate(HGE_VERSION);
	int width, height;
	width = hge->Texture_GetWidth(in);
	height = hge->Texture_GetHeight(in);
	HTEXTURE out = hge->Texture_Create(width, height);

	int widthMinus1 = width - 1;
	int heightMinus1 = height - 1;
	int tableSize = 2 * radius + 1;
	unsigned int* divide = new unsigned int[256 * tableSize];
	// the value scope will be 0 to 255, and number of 0 is table size  
	// will get means from index not calculate result again since   
	// color value must be  between 0 and 255.  
	for (int i = 0; i < 256 * tableSize; i++)
		divide[i] = i / tableSize;

	int inIndex = 0;
	unsigned int* in_ = (unsigned int*)hge->Texture_Lock(in, false);
	unsigned int* out_ = (unsigned int*)hge->Texture_Lock(out, false);
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
			in_[outIndex*width + x] = (divide[ta] << 24) | (divide[tr] << 16) | (divide[tg] << 8) | divide[tb];
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
	/*
	inIndex = 0;
	for (int x = 0; x < width; x++) {
		int outIndex = x;
		unsigned int ta = 0, tr = 0, tg = 0, tb = 0; // ARGB -> prepare for the alpha, red, green, blue color value.  
		for (int i = -radius; i <= radius; i++)
		{
			int clamp = (i < 0) ? 0 : (i >(height - 1)) ? (height - 1) : i;
			unsigned int rgb = in_[inIndex + clamp*width]; // read input pixel data here. table size data.  
			ta += (rgb >> 24) & 0xff;
			tr += (rgb >> 16) & 0xff;
			tg += (rgb >> 8) & 0xff;
			tb += rgb & 0xff;
		}

		for (int y = 0; y < height; y++)
		{ 
			out_[outIndex+width*y] = (divide[ta] << 24) | (divide[tr] << 16) | (divide[tg] << 8) | divide[tb]; // calculate the output data.  
			int i1 = y + radius + 1;
			if (i1 > heightMinus1)
				i1 = heightMinus1;
			int i2 = y - radius;
			if (i2 < 0)
				i2 = 0;
			int rgb1 = in_[inIndex + i1*width];
			int rgb2 = in_[inIndex + i2*width];
			ta += ((rgb1 >> 24) & 0xff) - ((rgb2 >> 24) & 0xff);
			tr += ((rgb1 & 0xff0000) - (rgb2 & 0xff0000)) >> 16;
			tg += ((rgb1 & 0xff00) - (rgb2 & 0xff00)) >> 8;
			tb += (rgb1 & 0xff) - (rgb2 & 0xff);
		}
		inIndex += 1; // next (i+ column number * n, n=1.n-1)  
	}
	*/



	for (int i = 0; i < height; i++)
	for (int j = 0; j < width; j++)
	if (in_[i*width + j] >> 24 != 0x00)
		in_[i*width + j] = out_[i*width + j];







	hge->Texture_Unlock(in);
	hge->Texture_Unlock(out);
	hge->Texture_Free(out);
	delete[] divide;
}


void ImageObject::CopyData(int x,int y,HTARGET data)
{
	//方法一（失败）
	//x y 与渲染坐标应一致
	/*
	int H = image->GetHeight()-BORDERY, W = image->GetWidth()-BORDERX;
	HGE *hge = hgeCreate(HGE_VERSION);
	HDC DC = ::GetDC(hge->System_GetState(HGE_HWND));
	HDC memDC = ::CreateCompatibleDC(DC);
	HBITMAP hBitmap, hOldBitmap;
	hBitmap = CreateCompatibleBitmap(DC, W+BORDERX, H+BORDERY);
	hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
	BitBlt(memDC, 0, 0, W, H, DC, x, y, SRCCOPY);
	W += BORDERX; H += BORDERY;
	StretchBlt(memDC, 0, 0, W, H, memDC, 0, 0, W - BORDERX, H - BORDERY, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(memDC, hOldBitmap);
	BITMAPINFO lpBitmapInfo = { 0 };
	lpBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	void *pBits=NULL;
	// lpvBits使用NULL查询位图DIB信息
	if (GetDIBits(DC, hBitmap, 0, 0, NULL, &lpBitmapInfo, DIB_RGB_COLORS))
	{
		// 修改格式，使用修改后的格式获数据Pixel数据
		lpBitmapInfo.bmiHeader.biCompression = BI_RGB;
		lpBitmapInfo.bmiHeader.biPlanes = 1;
		lpBitmapInfo.bmiHeader.biHeight = H;
		lpBitmapInfo.bmiHeader.biWidth = W;
		 pBits = malloc(lpBitmapInfo.bmiHeader.biSizeImage);
		 GetBitmapBits(hBitmap, lpBitmapInfo.bmiHeader.biSizeImage, (LPVOID)pBits);
	}

	DWORD *p = new DWORD[W*H];
	memcpy(p, pBits, lpBitmapInfo.bmiHeader.biSizeImage);
	DWORD *a = hge->Texture_Lock(texture, false);
	for (int i = 0; i < H; i++)
	for (int j = 0; j < W; j++)
	    a[i*W + j] = p[i*W+j];
	free(pBits);
	delete []p;
	ReleaseDC(hge->System_GetState(HGE_HWND), DC);
	DeleteObject(hBitmap);
	DeleteObject(hOldBitmap);
	hge->Texture_Unlock(texture);
	boxblur(texture, 10);
	*/
	//方法二：
	HGE *hge = hgeCreate(HGE_VERSION);
	int W = hge->Texture_GetWidth(texture);
	int H = hge->Texture_GetHeight(texture);
	HSURFACE A = hge->Target_GetSurface(data);
	DWORD *p_out = hge->Texture_Lock(texture, false);
    DWORD *p_data = hge->Surface_Lock(A,true);
    int pixelshift = 0;
	for (int i = 0; i < H; i++)
	{
		//以1024偏移（853+171）
		//pixelshift = 171 * i;
		for (int j = 0; j < W; j++)
		{
		if(p_out[i*W+j]>>24 != 0)//检测是否透明
			p_out[i*W + j] = p_data[i*W + j - pixelshift];   
		}
	}
	hge->Texture_Unlock(texture);
	hge->Surface_Unlock(A);
	hge->Surface_Free(A);
//	boxblur(texture, 2);
}


void ImageObject::MesteryBlur(int x,int y,int r)
{
	int roll = 1;
	float alpha = 100 / r;
	for (float a = 0.5; a <=r; a += 0.5)
	{
		image->SetColor(ARGB(255-alpha*roll, 255, 255, 255));
		image->Render(x + a, y);
		image->Render(x - a, y);
		image->Render(x, y + a);
		image->Render(x, y - a);
		roll++;
	}
	image->SetColor(ARGB(255, 255, 255, 255));
}