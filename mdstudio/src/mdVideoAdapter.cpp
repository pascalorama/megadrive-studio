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

///////////////////////////////////////////////////////////////////////////////
// Constructor

mdVideoAdapter::mdVideoAdapter(HWND hwnd,bool isfullscreen=false)
{
	m_Hwnd=hwnd;

	m_DD=NULL; 
	m_Clipper=NULL;
	m_FrontSurface=NULL; 
	m_BackSurface=NULL; 
	m_BitsPerPixel=0;
	m_BytesPerPixel=0;

	InitVideo(isfullscreen);
}

///////////////////////////////////////////////////////////////////////////////
// Destructor

mdVideoAdapter::~mdVideoAdapter()
{
	if(m_DD!=NULL)
	{
		if(m_BackSurface!=NULL)
		{	m_BackSurface->Release();
			m_BackSurface=NULL;
		}

		if(m_FrontSurface!=NULL)
		{	m_FrontSurface->Release();
			m_FrontSurface=NULL;
		}

		if(m_Clipper!=NULL)
		{	m_Clipper->Release();
			m_Clipper=NULL;
		}

		m_DD->Release();
		m_DD=NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Init Video

int mdVideoAdapter::InitVideo(bool isfullscreen)
{
	HRESULT rval;

	// Create directdraw
	rval = DirectDrawCreateEx(NULL,(LPVOID*) &m_DD, IID_IDirectDraw7, NULL);
	if(rval!=DD_OK)
	{	Alert(rval);
		return(0);
	}

	if(!isfullscreen)
		return InitWindowed();
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Init windowed

int mdVideoAdapter::InitWindowed(void)
{
	HRESULT rval;

	// Set Cooperative level
	rval=m_DD->SetCooperativeLevel(m_Hwnd,DDSCL_NORMAL);
	if(rval!=DD_OK)
	{	Alert(rval);
		return 0;	
	}

	// Create Primary Surface
	ZeroMemory(&m_DDSD, sizeof(DDSURFACEDESC2));
	m_DDSD.dwSize=sizeof(DDSURFACEDESC2);
	m_DDSD.dwFlags=DDSD_CAPS;
	m_DDSD.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;
	rval=m_DD->CreateSurface( &m_DDSD,&m_FrontSurface,NULL);
	if(rval!=DD_OK)
	{	Alert(rval);
		return 0;
	}

	m_FrontSurface->Lock(NULL,&m_DDSD,DDLOCK_WAIT,NULL);
	m_BitsPerPixel=m_DDSD.ddpfPixelFormat.dwRGBBitCount;
	m_FrontSurface->Unlock(NULL);

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
	
	// Create the Clipper
	rval=m_DD->CreateClipper(0,&m_Clipper,NULL);
	if(rval!=DD_OK)
	{	Alert(rval);
		return 0;
	}

	rval=m_Clipper->SetHWnd(0,m_Hwnd);
	if(rval!=DD_OK)
	{	Alert(rval);
		return 0;
	}

	rval=m_FrontSurface->SetClipper(m_Clipper);
	if(rval!=DD_OK)
	{	Alert(rval);	
		return 0;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Set Clipper

int mdVideoAdapter::SetClipper(HWND hwnd)
{
	HRESULT rval;
	rval=m_Clipper->SetHWnd(0,hwnd);
	if(rval!=DD_OK)
	{	Alert(rval);
		return 0;
	}
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// ClearFrontBuffer

int mdVideoAdapter::ClearFrontBuffer(int color)
{
	HRESULT rval;

	DDBLTFX bltfx;
   	bltfx.dwSize=sizeof(DDBLTFX);
	bltfx.dwFillColor= color;

	rval=m_FrontSurface->Blt(NULL,NULL,NULL,DDBLT_WAIT|DDBLT_COLORFILL,&bltfx);
	if(rval!=DD_OK)
	{	return 0;
		Alert(rval);}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Encode a color

int mdVideoAdapter::EncodeColor(unsigned char red,unsigned char green,unsigned char blue)
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
// Alert message

void mdVideoAdapter::Alert(HRESULT val)
{
	switch (val)
	{
		case DDERR_ALREADYINITIALIZED : wxLogError( "DDERR_ALREADYINITIALIZED"); break;
		case DDERR_CANNOTATTACHSURFACE : wxLogError( "DDERR_CANNOTATTACHSURFACE"); break;
		case DDERR_CANNOTDETACHSURFACE : wxLogError( "DDERR_CANNOTDETACHSURFACE"); break;
		case DDERR_CURRENTLYNOTAVAIL : wxLogError( "DDERR_CURRENTLYNOTAVAIL"); break;
		case DDERR_EXCEPTION : wxLogError( "DDERR_EXCEPTION"); break;
		case DDERR_GENERIC : wxLogError( "DDERR_GENERIC"); break;
		case DDERR_HEIGHTALIGN : wxLogError( "DDERR_HEIGHTALIGN"); break;
		case DDERR_INCOMPATIBLEPRIMARY : wxLogError( "DDERR_INCOMPATIBLEPRIMARY"); break;
		case DDERR_INVALIDCAPS : wxLogError( "DDERR_INVALIDCAPS"); break;
		case DDERR_INVALIDCLIPLIST : wxLogError( "DDERR_INVALIDCLIPLIST"); break;
		case DDERR_INVALIDMODE : wxLogError( "DDERR_INVALIDMODE"); break;
		case DDERR_INVALIDOBJECT : wxLogError( "DDERR_INVALIDOBJECT"); break;
		case DDERR_INVALIDPARAMS : wxLogError( "DDERR_INVALIDPARAMS"); break;
		case DDERR_INVALIDPIXELFORMAT : wxLogError( "DDERR_INVALIDPIXELFORMAT"); break;
		case DDERR_INVALIDRECT : wxLogError( "DDERR_INVALIDRECT"); break;
		case DDERR_LOCKEDSURFACES : wxLogError( "DDERR_LOCKEDSURFACES"); break;
		case DDERR_NO3D : wxLogError( "DDERR_NO3D"); break;
		case DDERR_NOALPHAHW : wxLogError( "DDERR_NOALPHAHW"); break;
		case DDERR_NOCLIPLIST : wxLogError( "DDERR_NOCLIPLIST"); break;
		case DDERR_NOCOLORCONVHW : wxLogError( "DDERR_NOCOLORCONVHW"); break;
		case DDERR_NOCOOPERATIVELEVELSET : wxLogError( "DDERR_NOCOOPERATIVELEVELSET"); break;
		case DDERR_NOCOLORKEY : wxLogError( "DDERR_NOCOLORKEY"); break;
		case DDERR_NOCOLORKEYHW : wxLogError( "DDERR_NOCOLORKEYHW"); break;
		case DDERR_NODIRECTDRAWSUPPORT : wxLogError( "DDERR_NODIRECTDRAWSUPPORT"); break;
		case DDERR_NOEXCLUSIVEMODE : wxLogError( "DDERR_NOEXCLUSIVEMODE"); break;
		case DDERR_NOFLIPHW : wxLogError( "DDERR_NOFLIPHW"); break;
		case DDERR_NOGDI : wxLogError( "DDERR_NOGDI"); break;
		case DDERR_NOMIRRORHW : wxLogError( "DDERR_NOMIRRORHW"); break;
		case DDERR_NOTFOUND : wxLogError( "DDERR_NOTFOUND"); break;
		case DDERR_NOOVERLAYHW : wxLogError( "DDERR_NOOVERLAYHW"); break;
		case DDERR_NORASTEROPHW : wxLogError( "DDERR_NORASTEROPHW"); break;
		case DDERR_NOROTATIONHW : wxLogError( "DDERR_NOROTATIONHW"); break;
		case DDERR_NOSTRETCHHW : wxLogError( "DDERR_NOSTRETCHHW"); break;
		case DDERR_NOT4BITCOLOR : wxLogError( "DDERR_NOT4BITCOLOR"); break;
		case DDERR_NOT4BITCOLORINDEX : wxLogError( "DDERR_NOT4BITCOLORINDEX"); break;
		case DDERR_NOT8BITCOLOR : wxLogError( "DDERR_NOT8BITCOLOR"); break;
		case DDERR_NOTEXTUREHW : wxLogError( "DDERR_NOTEXTUREHW"); break;
		case DDERR_NOVSYNCHW : wxLogError( "DDERR_NOVSYNCHW"); break;
		case DDERR_NOZBUFFERHW : wxLogError( "DDERR_NOZBUFFERHW"); break;
		case DDERR_NOZOVERLAYHW : wxLogError( "DDERR_NOZOVERLAYHW"); break;
		case DDERR_OUTOFCAPS : wxLogError( "DDERR_OUTOFCAPS"); break;
		case DDERR_OUTOFMEMORY : wxLogError( "DDERR_OUTOFMEMORY"); break;
		case DDERR_OUTOFVIDEOMEMORY : wxLogError( "DDERR_OUTOFVIDEOMEMORY"); break;
		case DDERR_OVERLAYCANTCLIP : wxLogError( "DDERR_OVERLAYCANTCLIP"); break;
		case DDERR_OVERLAYCOLORKEYONLYONEACTIVE : wxLogError( "DDERR_OVERLAYCOLORKEYONLYONEACTIVE"); break;
		case DDERR_PALETTEBUSY : wxLogError( "DDERR_PALETTEBUSY"); break;
		case DDERR_COLORKEYNOTSET : wxLogError( "DDERR_COLORKEYNOTSET"); break;
		case DDERR_SURFACEALREADYATTACHED : wxLogError( "DDERR_SURFACEALREADYATTACHED"); break;
		case DDERR_SURFACEALREADYDEPENDENT : wxLogError( "DDERR_SURFACEALREADYDEPENDENT"); break;
		case DDERR_SURFACEBUSY : wxLogError( "DDERR_SURFACEBUSY"); break;
		case DDERR_CANTLOCKSURFACE : wxLogError( "DDERR_CANTLOCKSURFACE"); break;
		case DDERR_SURFACEISOBSCURED : wxLogError( "DDERR_SURFACEISOBSCURED"); break;
		case DDERR_SURFACELOST : wxLogError( "DDERR_SURFACELOST"); break;
		case DDERR_SURFACENOTATTACHED : wxLogError( "DDERR_SURFACENOTATTACHED"); break;
		case DDERR_TOOBIGHEIGHT : wxLogError( "DDERR_TOOBIGHEIGHT"); break;
		case DDERR_TOOBIGSIZE : wxLogError( "DDERR_TOOBIGSIZE"); break;
		case DDERR_TOOBIGWIDTH : wxLogError( "DDERR_TOOBIGWIDTH"); break;
		case DDERR_UNSUPPORTED : wxLogError( "DDERR_UNSUPPORTED"); break;
		case DDERR_UNSUPPORTEDFORMAT : wxLogError( "DDERR_UNSUPPORTEDFORMAT"); break;
		case DDERR_UNSUPPORTEDMASK : wxLogError( "DDERR_UNSUPPORTEDMASK"); break;
		case DDERR_VERTICALBLANKINPROGRESS : wxLogError( "DDERR_VERTICALBLANKINPROGRESS"); break;
		case DDERR_WASSTILLDRAWING : wxLogError( "DDERR_WASSTILLDRAWING"); break;
		case DDERR_XALIGN : wxLogError( "DDERR_XALIGN"); break;
		case DDERR_INVALIDDIRECTDRAWGUID : wxLogError( "DDERR_INVALIDDIRECTDRAWGUID"); break;
		case DDERR_DIRECTDRAWALREADYCREATED : wxLogError( "DDERR_DIRECTDRAWALREADYCREATED"); break;
		case DDERR_NODIRECTDRAWHW : wxLogError( "DDERR_NODIRECTDRAWHW"); break;
		case DDERR_PRIMARYSURFACEALREADYEXISTS : wxLogError( "DDERR_PRIMARYSURFACEALREADYEXISTS"); break;
		case DDERR_NOEMULATION : wxLogError( "DDERR_NOEMULATION"); break;
		case DDERR_REGIONTOOSMALL : wxLogError( "DDERR_REGIONTOOSMALL"); break;
		case DDERR_CLIPPERISUSINGHWND : wxLogError( "DDERR_CLIPPERISUSINGHWND"); break;
		case DDERR_NOCLIPPERATTACHED : wxLogError( "DDERR_NOCLIPPERATTACHED"); break;
		case DDERR_NOHWND : wxLogError( "DDERR_NOHWND"); break;
		case DDERR_HWNDSUBCLASSED : wxLogError( "DDERR_HWNDSUBCLASSED"); break;
		case DDERR_HWNDALREADYSET : wxLogError( "DDERR_HWNDALREADYSET"); break;
		case DDERR_NOPALETTEATTACHED : wxLogError( "DDERR_NOPALETTEATTACHED"); break;
		case DDERR_NOPALETTEHW : wxLogError( "DDERR_NOPALETTEHW"); break;
		case DDERR_BLTFASTCANTCLIP : wxLogError( "DDERR_BLTFASTCANTCLIP"); break;
		case DDERR_NOBLTHW : wxLogError( "DDERR_NOBLTHW"); break;
		case DDERR_NODDROPSHW : wxLogError( "DDERR_NODDROPSHW"); break;
		case DDERR_OVERLAYNOTVISIBLE : wxLogError( "DDERR_OVERLAYNOTVISIBLE"); break;
		case DDERR_NOOVERLAYDEST : wxLogError( "DDERR_NOOVERLAYDEST"); break;
		case DDERR_INVALIDPOSITION : wxLogError( "DDERR_INVALIDPOSITION"); break;
		case DDERR_NOTAOVERLAYSURFACE : wxLogError( "DDERR_NOTAOVERLAYSURFACE"); break;
		case DDERR_EXCLUSIVEMODEALREADYSET : wxLogError( "DDERR_EXCLUSIVEMODEALREADYSET"); break;
		case DDERR_NOTFLIPPABLE : wxLogError( "DDERR_NOTFLIPPABLE"); break;
		case DDERR_CANTDUPLICATE : wxLogError( "DDERR_CANTDUPLICATE"); break;
		case DDERR_NOTLOCKED : wxLogError( "DDERR_NOTLOCKED"); break;
		case DDERR_CANTCREATEDC : wxLogError( "DDERR_CANTCREATEDC"); break;
		case DDERR_NODC : wxLogError( "DDERR_NODC"); break;
		case DDERR_WRONGMODE : wxLogError( "DDERR_WRONGMODE"); break;
		case DDERR_IMPLICITLYCREATED : wxLogError( "DDERR_IMPLICITLYCREATED"); break;
		case DDERR_NOTPALETTIZED : wxLogError( "DDERR_NOTPALETTIZED"); break;
		case DDERR_UNSUPPORTEDMODE : wxLogError( "DDERR_UNSUPPORTEDMODE"); break;
		case DDERR_NOMIPMAPHW : wxLogError( "DDERR_NOMIPMAPHW"); break;
		case DDERR_INVALIDSURFACETYPE : wxLogError( "DDERR_INVALIDSURFACETYPE"); break;
		case DDERR_DCALREADYCREATED : wxLogError( "DDERR_DCALREADYCREATED"); break;
		case DDERR_CANTPAGELOCK : wxLogError( "DDERR_CANTPAGELOCK"); break;
		case DDERR_CANTPAGEUNLOCK : wxLogError( "DDERR_CANTPAGEUNLOCK"); break;
		case DDERR_NOTPAGELOCKED : wxLogError( "DDERR_NOTPAGELOCKED"); break;
		case DDERR_NOTINITIALIZED : wxLogError( "DDERR_NOTINITIALIZED"); break;
		default : wxLogError( "Unknown Error %d",val); break;
	}
}