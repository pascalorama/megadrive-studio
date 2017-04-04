///////////////////////////////////////////////////////////////////////////////
// mdVideoAdapter.cpp
//  
//
// 
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes 
#include "mdShared.h"
#include "mdVideoAdapter.h"
#include "mdVideoSurface.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor

mdVideoSurface::mdVideoSurface(mdVideoAdapter *video,
							int width,
							int height,
							bool usevideomem=false)
{
	m_Video=video;
	m_Surface=NULL;
	m_Pitch=0;
	m_BytesPerPixel=0;
	m_BitsPerPixel=0;
	m_SurfaceSize=0;
	
	Create(width,height,usevideomem);

}

///////////////////////////////////////////////////////////////////////////////
// Destructor

mdVideoSurface::~mdVideoSurface()
{
	if(m_Surface)
	{
		m_Surface->Release();
		m_Surface=NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Create

int mdVideoSurface::Create(int width,int height,bool usevideomem)
{
	long flag;
	DDSURFACEDESC2 *ddsd;
	LPDIRECTDRAW7 dd;
	HRESULT rval;

	m_Width=width;
	m_Height=height;
	m_UseVideoMem=usevideomem;

	if(m_Surface)
	{	m_Surface->Release();
		m_Surface=NULL;
	}

	dd=m_Video->GetDirectDraw();
	if(dd)
	{
		ddsd=m_Video->GetDDSD();

		flag= m_UseVideoMem==1 ? DDSCAPS_VIDEOMEMORY : DDSCAPS_SYSTEMMEMORY;
		ZeroMemory(ddsd,sizeof(*(ddsd)));

		ddsd->dwSize=sizeof(*(ddsd));
		ddsd->dwFlags= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		ddsd->dwWidth=m_Width;
		ddsd->dwHeight=m_Height;
		ddsd->ddsCaps.dwCaps= DDSCAPS_OFFSCREENPLAIN | flag;

		rval=dd->CreateSurface(ddsd,&m_Surface,NULL);
		
		if(rval!=DD_OK && m_UseVideoMem==true)
		{
			ddsd->ddsCaps.dwCaps=	DDSCAPS_OFFSCREENPLAIN |
									DDSCAPS_SYSTEMMEMORY;

			rval=dd->CreateSurface(ddsd,&m_Surface,NULL);
			m_UseVideoMem=false;

			if(rval!=DD_OK)
			{	m_Video->Alert(rval);
				return 0;
			}
		}

		// Get the pitch
		m_Surface->Lock(NULL,ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
		m_Pitch = ddsd->lPitch;
		m_BitsPerPixel=ddsd->ddpfPixelFormat.dwRGBBitCount;

		switch(m_BitsPerPixel)
		{
			case 15:
			case 16:
			{	m_BytesPerPixel=2;
			}break;

			case 24:
			{	m_BytesPerPixel=3;
			}break;

			case 32:
			{	m_BytesPerPixel=4;
			}break;		
		}

		m_SurfaceSize= m_Pitch * ((m_Pitch/m_Width) * m_Height);							

		m_Surface->Unlock(NULL);	
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Lock Surface

unsigned char* mdVideoSurface::Lock(void)
{
	HRESULT rval;
	DDSURFACEDESC2 *ddsd=m_Video->GetDDSD();

	memset(ddsd,0,sizeof(*(ddsd)));
	ddsd->dwSize=sizeof(*(ddsd));
	rval=m_Surface->Lock(NULL,ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);

    if(rval!=DD_OK)
    {	m_Video->Alert(rval);
		return NULL;
	}
    else
	    return (unsigned char*) ddsd->lpSurface;
}

///////////////////////////////////////////////////////////////////////////////
// Unlock Surface

void mdVideoSurface::Unlock(void)
{
	m_Surface->Unlock(NULL);
}

///////////////////////////////////////////////////////////////////////////////
// Clear Surface

int mdVideoSurface::Clear(int color)
{
	HRESULT rval;

	DDBLTFX bltfx;
   	bltfx.dwSize=sizeof(DDBLTFX);
	bltfx.dwFillColor= color;

	rval=m_Surface->Blt(NULL,NULL,NULL,DDBLT_WAIT|DDBLT_COLORFILL,&bltfx);
	if(rval!=DD_OK)
	{	m_Video->Alert(rval);
		return 0;
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Clear a part

int mdVideoSurface::Clear(int color,RECT *rcSrc)
{
	HRESULT rval;


	DDBLTFX bltfx;
   	bltfx.dwSize=sizeof(DDBLTFX);
	bltfx.dwFillColor= color;

	rval=m_Surface->Blt(rcSrc,NULL,rcSrc,DDBLT_WAIT|DDBLT_COLORFILL,&bltfx);						

	if(rval!=DD_OK)
	{	m_Video->Alert(rval);
		return 0;
	}

	return 1;
}


///////////////////////////////////////////////////////////////////////////////
// Clear Surface

int mdVideoSurface::Clear(int red,int green,int blue)
{
	return this->Clear(this->EncodeColor(red,green,blue));
}

///////////////////////////////////////////////////////////////////////////////
// Encode a color

int mdVideoSurface::EncodeColor(unsigned char red,unsigned char green,unsigned char blue)
{
	switch(m_BytesPerPixel)
	{
		case 2:
		{	return RGB565(red,green,blue);
		}break;

		case 4:
		{	return RGB888(red,green,blue);
		}break;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Draw

int mdVideoSurface::Draw(RECT* DstRc,RECT* SrcRc)
{
	HRESULT rval;

	rval= m_Video->GetFrontSurface()->Blt(DstRc,m_Surface,SrcRc,DDBLT_WAIT,NULL);

	if(rval!=DD_OK)
	{	
		switch(rval)
		{
			case DDERR_SURFACELOST:
			{	m_Video->Alert(rval);
			}break;

			case DDERR_INVALIDRECT :
			{	;
			}break;

			default:
			{	m_Video->Alert(rval);
				return 0;		
			}break;
		}		
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Draw

int mdVideoSurface::Draw(int dst_x,int dst_y,mdVideoSurface* src,RECT* SrcRc)
{
	HRESULT rval;


	rval = m_Surface->BltFast(	dst_x,
								dst_y,
								src->GetSurface(),
								SrcRc,
								DDBLTFAST_WAIT);   
	
	if(rval!=DD_OK)
	{	
		switch(rval)
		{
			case DDERR_SURFACELOST:
			{	m_Video->Alert(rval);
			}break;

			case DDERR_INVALIDRECT :
			{	;
			}break;


			default:
			{	m_Video->Alert(rval);
				return 0;		
			}break;
		}		
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Draw

int mdVideoSurface::Draw(RECT* DstRc,mdVideoSurface* src,RECT* SrcRc)
{
	HRESULT rval;

	rval= m_Surface->Blt(DstRc,src->GetSurface(),SrcRc,	DDBLT_WAIT,NULL);

	if(rval!=DD_OK)
	{	
		switch(rval)
		{
			case DDERR_SURFACELOST:
			{	m_Video->Alert(rval);
			}break;

			case DDERR_INVALIDRECT :
			{	;
			}break;


			default:
			{	m_Video->Alert(rval);
				return 0;		
			}break;
		}		
	}

	return 1;

}

///////////////////////////////////////////////////////////////////////////////
// Draw surface with color key

int mdVideoSurface::DrawWithColorKey(RECT* DstRc,mdVideoSurface* src,RECT* SrcRc)
{
	HRESULT rval;

	rval= m_Surface->Blt(DstRc,src->GetSurface(),SrcRc,	DDBLT_WAIT|
														DDBLT_KEYSRC,NULL);

	if(rval!=DD_OK)
	{	
		switch(rval)
		{
			case DDERR_SURFACELOST:
			{	m_Video->Alert(rval);
			}break;

			case DDERR_INVALIDRECT :
			{	;
			}break;


			default:
			{	m_Video->Alert(rval);
				return 0;		
			}break;
		}		
	}

	return 1;

}

///////////////////////////////////////////////////////////////////////////////
// Draw

int mdVideoSurface::DrawWithColorKey(RECT* DstRc,RECT* SrcRc)
{
	HRESULT rval;

	rval= m_Video->GetFrontSurface()->Blt(DstRc,m_Surface,SrcRc,	DDBLT_WAIT|
														DDBLT_KEYSRC,NULL);

	if(rval!=DD_OK)
	{	
		switch(rval)
		{
			case DDERR_SURFACELOST:
			{	m_Video->Alert(rval);
			}break;

			case DDERR_INVALIDRECT :
			{	;
			}break;


			default:
			{	m_Video->Alert(rval);
				return 0;		
			}break;
		}		
	}

	return 1;

}

///////////////////////////////////////////////////////////////////////////////
// Duplicate a part of the surface

int mdVideoSurface::CopyRect(RECT* DstRc,RECT* SrcRc)
{
	HRESULT rval;


	rval = m_Surface->BltFast(	DstRc->left,
								DstRc->top,
								m_Surface,
								SrcRc,
								DDBLTFAST_WAIT);   
	
	if(rval!=DD_OK)
	{	
		switch(rval)
		{
			case DDERR_SURFACELOST:
			{	m_Video->Alert(rval);
			}break;

			case DDERR_INVALIDRECT :
			{	;
			}break;


			default:
			{	m_Video->Alert(rval);
				return 0;		
			}break;
		}
		
		
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Set Color key

void mdVideoSurface::SetColorKey(	unsigned char red,unsigned char green,unsigned char blue)
{
    DDCOLORKEY ddck;
	int color = EncodeColor(red,green,blue);

	ddck.dwColorSpaceHighValue=color;
	ddck.dwColorSpaceLowValue=color;

	m_Surface->SetColorKey(DDCKEY_SRCBLT,&ddck);

}


///////////////////////////////////////////////////////////////////////////////