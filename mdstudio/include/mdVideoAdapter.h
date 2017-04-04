///////////////////////////////////////////////////////////////////////////////
// mdVideoAdapter
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __mdVideoAdapter_H__
#define __mdVideoAdapter_H__

///////////////////////////////////////////////////////////////////////////////
// Include

#define		INITGUID
#define		DIRECTDRAW_VERSION 0x0700
#include	<ddraw.h>

#define		RGB565(r, g, b)((r >> 3)<< 11)|((g>>2)<<5)|((b>>3)<<0)
#define		RGB888(r, g, b)(r<<16)|(g<<8)|b;

///////////////////////////////////////////////////////////////////////////////
// Class definition
///////////////////////////////////////////////////////////////////////////////

class mdVideoAdapter
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////

	LPDIRECTDRAW7			GetDirectDraw(void){return m_DD;}
	LPDIRECTDRAWSURFACE7	GetFrontSurface(void ){	return m_FrontSurface;}
	LPDIRECTDRAWSURFACE7	GetBackSurface(void){	return m_BackSurface;}
	DDSCAPS2*				GetDDSCaps(void){	return &m_DDSCaps; }
	DDSURFACEDESC2*			GetDDSD(void){ return &m_DDSD;}

	mdVideoAdapter(HWND hwnd,bool isfullscreen);
	~mdVideoAdapter();
	
	int InitVideo(bool isfullscreen);
	int InitWindowed(void);
	int SetClipper(HWND hwnd);
	int ClearFrontBuffer(int color);

	int EncodeColor(unsigned char red,unsigned char green,unsigned char blue);	
	void Alert(HRESULT val);	

	int	m_BitsPerPixel;
	int m_BytesPerPixel;

	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////

	HWND	m_Hwnd;
	LPDIRECTDRAW7 m_DD; 
	LPDIRECTDRAWCLIPPER		m_Clipper;
	LPDIRECTDRAWSURFACE7	m_FrontSurface; 
	LPDIRECTDRAWSURFACE7	m_BackSurface; 
	DDSCAPS2 m_DDSCaps; 
	DDSURFACEDESC2 m_DDSD;

	
};

///////////////////////////////////////////////////////////////////////////////

#endif