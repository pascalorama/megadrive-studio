///////////////////////////////////////////////////////////////////////////////
//
//
//
// NOT PORTABLE I'M USING DIB
///////////////////////////////////////////////////////////////////////////////
// Includes 
#include "include/mdShared.h"
#include "include/mdDibBitmap.h"
#include "include/mdVideoAdapter.h"
#include "include/mdVideoSurface.h"

#include "include/mdFrmMain.h"
#include "include/mdApp.h"

#include "include/mdScreen.h"

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(mdScreen, wxPanel)
	EVT_ERASE_BACKGROUND(mdScreen::OnEraseBackground)    
	EVT_PAINT(mdScreen::OnPaint) 

END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

mdScreen::mdScreen(	wxWindow* parent,
					wxWindowID id,									
					const wxPoint& pos,
					const wxSize& size,
					long style,
					const wxString& name)
					:wxPanel(parent,id,pos,size,style,name)
{

	m_Bitmap=NULL;
	m_Surface=NULL;

	m_Zoom=1;
}

///////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////

mdScreen::~mdScreen()
{
	m_Bitmap=NULL;

	delete(m_Surface);
}

///////////////////////////////////////////////////////////////////////////////
// Override to prevent bg flickering
///////////////////////////////////////////////////////////////////////////////

void mdScreen::OnEraseBackground(wxEraseEvent& event)
{
}

///////////////////////////////////////////////////////////////////////////////
// Override to prevent bg flickering
///////////////////////////////////////////////////////////////////////////////

void mdScreen::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	
	if(m_Bitmap!=NULL)
	{	
		RECT rc;
		RECT src;

		int x=0,y=0,w=0,h=0;
		GetClientSize(&w,&h);
		ClientToScreen(&x,&y);
	
		rc.top=y;
		rc.left=x;
		rc.right=rc.left+(w );
		rc.bottom=rc.top+(h );
				
		src.left=0;
		src.right=320 ;
		src.bottom=240;		
		src.top=0;
		
		m_Surface->SetClipper((HWND) GetHandle());

		if(m_Surface->GetVideoAdapter()->m_BytesPerPixel==2)
		{
			unsigned short *b=(unsigned short*) m_Surface->Lock();
			unsigned short *c=(unsigned short*) m_Bitmap->Data;
			unsigned short *ptr_dest=b;
			unsigned short *ptr_src=&c[32];

			for(int y=0;y<240;y++)
			{	ptr_dest=&b[y*320];
				ptr_src=&c[32+(1024*y)];	
				for(int x=0;x<320;x++)
				{	*ptr_dest++=*ptr_src++;
				}
			}
		}else if(m_Surface->GetVideoAdapter()->m_BytesPerPixel==4)
		{
			unsigned int *b=(unsigned int*) m_Surface->Lock();
			unsigned int *c=(unsigned int*) m_Bitmap->Data;

			for(int y=0;y<240;y++)
			{	for(int x=0;x<320;x++)
				{	b[(y*320)+x]=c[32+(1024*y)+x];
				}
			}
		}
		m_Surface->Unlock();
		m_Surface->Draw(&rc,&src);
	}
}

void mdScreen::SetBitmap(mdDibBitmap *bmp)
{	m_Bitmap=bmp;
	if(m_Surface==NULL)
	{	mdVideoAdapter *v=gFrmMain->m_VideoAdapter;
		m_Surface=new mdVideoSurface(v,320*m_Zoom,240*m_Zoom,false);
		m_Surface->GetVideoAdapter()->ClearFrontBuffer(0xff00);
		m_Surface->Clear(255,0,255);
	}
}

///////////////////////////////////////////////////////////////////////////////