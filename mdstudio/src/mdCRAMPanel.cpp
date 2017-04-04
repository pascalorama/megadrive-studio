///////////////////////////////////////////////////////////////////////////////
//
//
//
// 
///////////////////////////////////////////////////////////////////////////////
// Includes 
#include "include/mdShared.h"
#include "include/mdEmu.h"
#include "include/mdCRAMPanel.h"

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(mdCRAMPanel, wxPanel)
		EVT_PAINT(mdCRAMPanel::OnPaint) 
		EVT_ERASE_BACKGROUND(mdCRAMPanel::OnEraseBackground)    

		EVT_MOTION(mdCRAMPanel::OnMouseMotion) 
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

mdCRAMPanel::mdCRAMPanel(	wxWindow* parent,
							wxWindowID id,									
							const wxPoint& pos,
							const wxSize& size,
							long style,
							const wxString& name)
							:wxPanel(parent,id,pos,size,style,name)
{

	//m_Emu=NULL;
	m_CRAM=NULL;

	//SetToolTip(_T("CRAM Viewer"));
}

///////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////

mdCRAMPanel::~mdCRAMPanel()
{
}

///////////////////////////////////////////////////////////////////////////////

void mdCRAMPanel::OnEraseBackground(wxEraseEvent& event)
{
}

///////////////////////////////////////////////////////////////////////////////
//
#define UNPACK_CRAM(d)  ((((d)&0x1C0)>>5)|((d)&0x038)<<2|(((d)&0x007)<<9))
void mdCRAMPanel::OnPaint(wxPaintEvent& event)
{
	wxFont fnt(10,wxDEFAULT,wxNORMAL,wxNORMAL,false,"Courier New",wxFONTENCODING_SYSTEM );
	int w,h;
	wxPaintDC pdc(this);
	wxBufferedDC dc(&pdc); 

	GetClientSize(&w,&h);
	dc.SetFont(fnt);
	dc.BeginDrawing();

	// dessine le fond
	wxColour f(255,255,255);
	dc.SetPen(f);
	dc.SetBrush(wxBrush(f));
	dc.DrawRectangle(0,0,w,h);
	
	unsigned short *pal=(unsigned short*)m_CRAM;

	for(int j=0;j<4;j++)
	{
		for(int i=0;i<16;i++)
		{
			unsigned short temp=UNPACK_CRAM(pal[(j*16)+i]);
			temp &=0x0EEE;

			// ----bbb-ggg-rrr-
			unsigned char Blue = ((temp & 0xe00)>>9) <<5;
			unsigned char Green = ((temp & 0xe0)>>5) <<5;
			unsigned char Red = ((temp & 0xe)>>1) <<5;
			
			wxColour c(Red,Green,Blue);
			dc.SetBrush(wxBrush(c));

			dc.DrawRectangle(i*16,j*16,16,16);	
		}

	}

	dc.EndDrawing();
}

void mdCRAMPanel::UpdateCRAM()
{	Refresh();
}

void mdCRAMPanel::SetCRAM(unsigned char* cram)
{	m_CRAM=cram;
}

void mdCRAMPanel::OnMouseMotion(wxMouseEvent& event)
{
	int x=event.GetX();
	int y=event.GetY();

    if(x>0 && x<16*16 && y>0 && y<16*4)
	{	y=y/16;
		x=x/16;
		unsigned short *pal=(unsigned short*)m_CRAM;
		unsigned short temp=UNPACK_CRAM(pal[(y*16)+x]);
		temp &=0x0EEE;

		// ----bbb-ggg-rrr-
		unsigned char Blue = ((temp & 0xe00)>>9) <<5;
		unsigned char Green = ((temp & 0xe0)>>5) <<5;
		unsigned char Red = ((temp & 0xe)>>1) <<5;
		wxString b;
		
		b=b.Format("Color[%d][%d]\nValue:$%.2x\nRed    %.3d\nGreen %.3d\nBlue    %.3d",y,x,pal[(y*16)+x],Red,Green,Blue);
		SetToolTip(b);
	}
}

/*void mdCRAMPanel::SetEmu(mdEmu* emu)
{
	m_Emu=emu;
}*/

