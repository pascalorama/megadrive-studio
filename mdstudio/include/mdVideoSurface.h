#ifndef __mdVIDEOSURFACE_H__
#define __mdVIDEOSURFACE_H__

///////////////////////////////////////////////////////////////////////////////
// Include

#include	<ddraw.h>

///////////////////////////////////////////////////////////////////////////////
// class definition
///////////////////////////////////////////////////////////////////////////////

class mdVideoAdapter;

class mdVideoSurface
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////
	
  	mdVideoSurface(mdVideoAdapter *video,int width,int heigh,bool usevideomem);
	~mdVideoSurface(void);

	unsigned char* Lock(void);
	void Unlock(void);

	int Clear(int color=0);
	int Clear(int red,int green,int blue);
	int Clear(int color,RECT *rcSrc);
	int EncodeColor(unsigned char red,unsigned char green,unsigned char blue);

	int Draw(RECT* DstRc,RECT* SrcRc);
	int Draw(int dst_x,int dst_y,mdVideoSurface* src,RECT* SrcRc);
	int Draw(RECT* DstRc,mdVideoSurface* src,RECT* SrcRc);
	int DrawWithColorKey(RECT* DstRc,mdVideoSurface* src,RECT* SrcRc);
	int DrawWithColorKey(RECT* DstRc,RECT* SrcRc);
	int CopyRect(RECT* DstRc,RECT* SrcRc);

	int GetBytesPerPixel(void){	return m_BytesPerPixel;}
	int GetBitsPerPixel(void){	return m_BitsPerPixel;}
	int GetPitch(void){	return m_Pitch;}
	int GetWidth(void){	return m_Width;}
	int GetHeight(void){return m_Height;}

	mdVideoAdapter* GetVideoAdapter(void){	return m_Video;}
	LPDIRECTDRAWSURFACE7 GetSurface(void){	return m_Surface;}

	void SetColorKey(unsigned char red,unsigned char green,unsigned char blue);

	void SetClipper(HWND hwnd){ m_Video->SetClipper(hwnd);}

	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////

	LPDIRECTDRAWSURFACE7	m_Surface;
	mdVideoAdapter			*m_Video;

	int		m_Width;
	int		m_Height;
	int		m_Pitch;
	bool	m_UseVideoMem;
	int		m_BitsPerPixel;
	int		m_BytesPerPixel;
	int		m_SurfaceSize;

	int		Create(int width,int height,bool usevideomem);
};

///////////////////////////////////////////////////////////////////////////////

#endif