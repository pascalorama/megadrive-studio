///////////////////////////////////////////////////////////////////////////////
//
//

///////////////////////////////////////////////////////////////////////////////
// app includes
#include "include/mdShared.h"
#include "mdDibBitmap.h"

///////////////////////////////////////////////////////////////////////////////
// DibBitmap
///////////////////////////////////////////////////////////////////////////////

mdDibBitmap::mdDibBitmap(int iWidth,int iHeight,BYTE bDepth)
{
	int	iBmiSize=0;
	int	iSurfaceSize=0;

	Width=iWidth;
	Height=iHeight;
	Depth=bDepth;

	switch(Depth)
	{
		case 8:
		{
			iBmiSize=sizeof(BITMAPINFO) + sizeof(RGBQUAD) * 256;
			iSurfaceSize= iWidth  * iHeight * sizeof(BYTE);
		}break;

		case 15:
		case 16:
		{
			iBmiSize=sizeof(BITMAPINFO) + sizeof(DWORD) * 4;
			iSurfaceSize= iWidth  * iHeight * sizeof(WORD);
		}break;

		case 32:
		{	iBmiSize=sizeof(BITMAPINFO) + sizeof(DWORD) * 4;
			iSurfaceSize= iWidth  * iHeight * sizeof(DWORD);
		}break;

		default:
		{
			//TRACE("Unsupported depth\n");
		}
	}

	//-- allocate memory for the bitmap info header
	//
	if( (lpBmi=(LPBITMAPINFO) malloc(iBmiSize))==NULL)
	{
		//TRACE("Error allocating for BitmapInfo\n");
		return;
	}
	ZeroMemory(lpBmi,iBmiSize);

	//-- allocate memory for the DIB Surface
	//
	if( (Data=(BYTE*) malloc(iSurfaceSize))==NULL)
	{
		//TRACE("Error allocating memory for Bitmap Data\n");
		return;
	}
	ZeroMemory(Data,iSurfaceSize);

	//-- initialize the bitmap info header
	//-- Initialize bitmap info header
	//
	lpBmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpBmi->bmiHeader.biWidth = iWidth;
	lpBmi->bmiHeader.biHeight = -(signed)iHeight;		// <-- MINUS MEANS TOP DOWN!!!
	lpBmi->bmiHeader.biPlanes = 1;
	lpBmi->bmiHeader.biSizeImage = 0;
	lpBmi->bmiHeader.biXPelsPerMeter = 0;
	lpBmi->bmiHeader.biYPelsPerMeter = 0;
	lpBmi->bmiHeader.biClrUsed = 0;
	lpBmi->bmiHeader.biClrImportant = 0;
	lpBmi->bmiHeader.biCompression = BI_RGB;

	switch(Depth)
	{
		case 8 :
		{
			//-- For the 8bpp DIB we will create a simple grayscale palette.
			//
			for(int i = 0; i < 256; i++)
			{
				lpBmi->bmiColors[i].rgbRed			= (BYTE)i;
				lpBmi->bmiColors[i].rgbGreen		= (BYTE)i;
				lpBmi->bmiColors[i].rgbBlue		= (BYTE)i;
				lpBmi->bmiColors[i].rgbReserved	= (BYTE)0;
			}

			//-- Set the bpp for this DIB to 8bpp.
			lpBmi->bmiHeader.biBitCount = 8;
		}
		break;
	
		case 15 :
		{
			//-- This is where we will tell the DIB what bits represent what
			//   data. This may look confusing at first but the representation
			//   of the RGB data can be different on different devices. For
			//   example you can have for Hicolor a 565 format. Meaning 5 bits
			//   for red, 6 bits for green and 5 bits for blue or better stated
			//   like RGB. But, the pixel data can also be the other way around,
			//   for example BGR meaning, 5 bits for blue, 6 bits for green and
			//   5 bits for red. This piece of information will tell the bitmap
			//   info header how the pixel data will be stored. In this case in
			//   RGB format in 555 because this is a 15bpp DIB so the highest
			//   bit (bit 15) will not be used.
			//
			DWORD *pBmi = (DWORD*)lpBmi->bmiColors;

			pBmi[0] = 0x00007C00;	// Red mask
			pBmi[1] = 0x000003E0;	// Green mask
			pBmi[2] = 0x0000001F;	// Blue mask
			pBmi[3] = 0x00000000;	// Not used

			//-- 15bpp DIB also use 16 bits to store a pixel.
			lpBmi->bmiHeader.biBitCount = 16;
			lpBmi->bmiHeader.biCompression |= BI_BITFIELDS;
		}
		break;
		
		case 16 :
		{
			//-- Take a look at the remarks written by 15bpp. For this format
			//   it's the same thing, except in this case the mask's will be
			//   different because our format will be 565 (RGB).
			//
			DWORD *pBmi = (DWORD*)lpBmi->bmiColors;

			pBmi[0] = 0x0000F800;	// Red mask
			pBmi[1] = 0x000007E0;	// Green mask
			pBmi[2] = 0x0000001F;	// Blue mask
			pBmi[3] = 0x00000000;	// Not used

			lpBmi->bmiHeader.biBitCount = 16;
			lpBmi->bmiHeader.biCompression |= BI_BITFIELDS;
		}
		break;

		case 24 :
		{
			//-- This is a 1:1 situation. There is no need to set any extra
			//   information.
			lpBmi->bmiHeader.biBitCount = 24;
		}
		break;

		case 32 :
		{
			//-- This may speak for it's self. In this case where using 32bpp.
			//   The format will be ARGB. the Alpha (A) portion of the format
			//   will not be used. The other mask's tell us where the bytes
			//   for the R, G and B data will be stored in the DWORD.
			//
			DWORD *pBmi = (DWORD*)lpBmi->bmiColors;

			pBmi[0] = 0x00FF0000;	// Red mask
			pBmi[1] = 0x0000FF00;	// Green mask
			pBmi[2] = 0x000000FF;	// Blue mask
			pBmi[3] = 0x00000000;	// Not used (Alpha?)

			lpBmi->bmiHeader.biBitCount = 32;
			lpBmi->bmiHeader.biCompression |= BI_BITFIELDS;
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Put Pixel
///////////////////////////////////////////////////////////////////////////////

mdDibBitmap::~mdDibBitmap()
{
    free(Data);
}

///////////////////////////////////////////////////////////////////////////////
// Put Pixel
///////////////////////////////////////////////////////////////////////////////

void mdDibBitmap::PutPixel(int x, int y, BYTE r, BYTE g, BYTE b)
{
	int iOffset = lpBmi->bmiHeader.biWidth * y + x;

	switch(lpBmi->bmiHeader.biBitCount)
	{
		case 8 :
		{	BYTE* p = (BYTE*)Data;
			p[iOffset] = (BYTE)r;
		}
		break;

		case 15 :
		{	WORD* p = (WORD*)Data;
			p[iOffset] = (WORD)(((r & 0xF8) << 7) | ((g & 0xF8) << 2) | b >> 3);
		}
		break;

		case 16 :
		{	WORD* p = (WORD*)Data;
			p[iOffset] = (WORD)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | b >> 3);
		}
		break;

		case 24 :
		{	BYTE* p = (BYTE*)Data;
			p[iOffset * 3 + 0] = r;
			p[iOffset * 3 + 1] = g;
			p[iOffset * 3 + 2] = b;
		}
		break;

		case 32 :
		{	DWORD* p = (DWORD*)Data;
			p[iOffset] = (DWORD)((r << 16) | (g << 8) | b);
		}
		break;
	}
}

void mdDibBitmap::Clear(BYTE r,BYTE g,BYTE b)
{
	WORD c=(WORD)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | b >> 3);
	WORD *p=(WORD*)Data;

	for(int y=0;y<this->Height;y++)
	{
		for(int x=0;x<this->Width;x++)
		{
			p[lpBmi->bmiHeader.biWidth * y + x]=c;			
		}
	}
}
