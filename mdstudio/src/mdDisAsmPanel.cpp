///////////////////////////////////////////////////////////////////////////////
//
//
//
// 
///////////////////////////////////////////////////////////////////////////////
// Includes 
#include "include/mdShared.h"
#include "include/mdEmu.h"
#include "include/mdFrmMain.h"
#include "include/mdApp.h"
#include "include/mdDisAsmPanel.h"

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(mdDisAsmPanel, wxWindow)
	EVT_PAINT(mdDisAsmPanel::OnPaint) 
	EVT_ERASE_BACKGROUND(mdDisAsmPanel::OnEraseBackground)    
	EVT_SIZE(mdDisAsmPanel::OnSize)

	EVT_LEFT_DCLICK(mdDisAsmPanel::OnLeftDoubleClick)
END_EVENT_TABLE()

#define TOP_MARGIN	5
#define TAILLE 15

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

mdDisAsmPanel::mdDisAsmPanel(	wxWindow* parent,
								wxWindowID id,									
								const wxPoint& pos,
								const wxSize& size,
								long style,
								const wxString& name)
								:wxWindow(parent,id,pos,size,style,name)
{
	wxColour c(255,255,235);
	this->SetBackgroundColour(c);
	m_RowPosition=0;
	m_Emu=NULL;
	m_PageTopPC=gEmu->GetCpuRegister(mdM68K_CPU,mdM68K_REG_PC);
}

mdDisAsmPanel::~mdDisAsmPanel()
{

}

void mdDisAsmPanel::OnEraseBackground(wxEraseEvent& event)
{
}

void mdDisAsmPanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC pdc(this);
	wxBufferedDC dc(&pdc); 

	int w,h,page_cnt=0;
	wxString a;
	wxSystemSettings sys;
	wxFont fnt(10,wxDEFAULT,wxNORMAL,wxNORMAL,false,"Courier New",wxFONTENCODING_SYSTEM );
	wxColour c=sys.GetColour(wxSYS_COLOUR_BTNFACE);
		
	GetClientSize(&w,&h);
	page_cnt= (h-5)/TAILLE;
	
	dc.BeginDrawing();

	dc.SetFont(fnt);
	// dessin du rectangle de break point
	dc.SetPen(c);
	dc.SetBrush(wxBrush(c));
	dc.DrawRectangle(0,0,20,h);
	dc.SetPen(*wxBLACK);
	dc.DrawLine(19,0,20,h*2);

	// dessine le fond
	wxColour f(255,255,255);
	dc.SetPen(f);
	dc.SetBrush(wxBrush(f));
	dc.DrawRectangle(20,0,w-20,h);

	int prev=0;
	if(m_Emu->m_RomLoaded==true)
	{
		for(int i=0;i<page_cnt;i++)
		{	
			prev=m_DisAsmPC[i];
			wxString b;
			
			if(m_IsLabel[i]==false)
			{
				if(m_IsBreakPoint[i]==false)
				{	
					if(prev==m_Emu->m_CurrentPC)
					{	// affiche la sélection
						dc.SetBrush(wxBrush(wxColour(255,255,0)));
						dc.DrawRectangle(25,(TOP_MARGIN+i*TAILLE)-2,w,19);
					}				
					dc.SetTextForeground(*wxBLACK);
				}
				else
				{	
					if(prev==m_Emu->m_CurrentPC)
					{	// affiche la sélection
						dc.SetBrush(wxBrush(wxColour(255,0,0)));
						dc.DrawRectangle(25,(TOP_MARGIN+i*TAILLE)-2,w,19);
						dc.SetTextForeground(*wxWHITE);		
					}
					else
						dc.SetTextForeground(*wxRED);		
				}
				b=b.Format("%.6x",prev); 
				
				dc.DrawText(a.Format("0x%s    %s",b.Upper().c_str(), m_DisAsmText[i].c_str()), 30,TOP_MARGIN+i*TAILLE);
			}
			else
			{	dc.SetTextForeground(*wxBLACK);
				b=b.Format("%.6x",prev); 				
				dc.DrawText(a.Format("0x%s  %s",b.Upper().c_str(), m_DisAsmText[i].c_str()), 30,TOP_MARGIN+i*TAILLE);
			}
		}
	}
	dc.SetPen(*wxBLACK);
	dc.DrawLine(100,0,100,h*2);


	dc.EndDrawing();
}

void mdDisAsmPanel::SetEmu(mdEmu *emu)
{
	m_Emu=emu;
}

void mdDisAsmPanel::DisAsm()
{	
	int w,h;
	GetClientSize(&w,&h);
	int page_cnt= (h-5)/TAILLE;
	unsigned int pc=m_PageTopPC;

	int prev=0;
	if(m_Emu->m_RomLoaded==true)
	{
		for(int i=0;i<page_cnt;i++)
		{	prev=pc;
			m_DisAsmPC[i]=prev;
			m_IsBreakPoint[i]=false;
			m_IsLabel[i]=false;

			char *c=gProject->GetSymbolLabelName(prev);
			if(c!=NULL)
			{	m_DisAsmText[i]=m_DisAsmText[i].Format("%s",c);
				m_IsBreakPoint[i]=false;
				m_IsLabel[i]=true;

				pc+=m_Emu->DisAsm(m_DisAsmText[++i],pc);
				m_DisAsmPC[i]=prev;
				
				m_IsBreakPoint[i]=false;
				if(m_Emu->CheckIsBreakPoint(prev)==true)
					m_IsBreakPoint[i]=true;

				m_IsLabel[i]=false;
			}
			else
			{	if(m_Emu->CheckIsBreakPoint(prev)==true)
					m_IsBreakPoint[i]=true;

				pc+=m_Emu->DisAsm(m_DisAsmText[i],pc);						
			}			
		}
		m_PageBottomPC=prev;
	}
	this->Refresh();
}

void mdDisAsmPanel::OnSize(wxSizeEvent &event)
{
}

void mdDisAsmPanel::OnLeftDoubleClick(wxMouseEvent &event)
{
	long x,y;
	x=y=0;
	event.GetPosition(&x,&y);
	y-=TOP_MARGIN;
	// recherche la ligne sur laquelle on est
	if((gEmu->m_IsPaused || gEmu->m_IsRunning==false) && gEmu->m_RomLoaded==true)
	{	int pos=y/TAILLE;	
		unsigned int pc=m_DisAsmPC[pos];
		if(m_IsLabel[pos]==false)
		{
			if(m_IsBreakPoint[pos]==false)
			{	m_IsBreakPoint[pos]=true;
				gEmu->CreateBreakPoint(pc);
				this->Refresh();
			}	
			else
			{	// supprime le break point
				m_IsBreakPoint[pos]=false;
				gEmu->EraseBreakPoint(pc);
				this->Refresh();			
			}
		}
	}



}
