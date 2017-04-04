///////////////////////////////////////////////////////////////////////////////
//
//
//
// NOT PORTABLE I'M USING DIB
///////////////////////////////////////////////////////////////////////////////
// Includes 
#include "include/mdShared.h"
#include "include/mdScreen.h"
#include "include/mdLogPanel.h"

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////

enum{
	Menu_Clear_Log = 300,
	Menu_Save_Log,
	Menu_DMA,
	Menu_IO,
};

BEGIN_EVENT_TABLE(mdLogPanel, wxPanel)
	EVT_CONTEXT_MENU(mdLogPanel::OnContextMenu)

	EVT_PAINT(mdLogPanel::OnPaint) 
	EVT_ERASE_BACKGROUND(mdLogPanel::OnEraseBackground)  

	EVT_SIZE(mdLogPanel::OnSize)
	EVT_SCROLLWIN_THUMBTRACK(mdLogPanel::OnThumbTrack)

	EVT_MENU(Menu_Clear_Log, mdLogPanel::OnClearLog)
	EVT_MENU(Menu_Save_Log, mdLogPanel::OnSaveLog)

	EVT_MENU(Menu_DMA, mdLogPanel::OnDMA)
	EVT_MENU(Menu_IO, mdLogPanel::OnIO)

END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

mdLogPanel::mdLogPanel(	wxWindow* parent,
					wxWindowID id,									
					const wxPoint& pos,
					const wxSize& size,
					long style,
					const wxString& name)
					:wxPanel(parent,id,pos,size,style | wxVSCROLL | wxSUNKEN_BORDER,name)
{

	// initialise le menu popup
	m_Popup=new wxMenu();
	m_Popup->Append(Menu_Clear_Log, _T("&Clear log"));
//	m_Popup->Append(Menu_Save_Log, _T("&Save Log..."));
	m_Popup->AppendSeparator();
	m_Popup->AppendCheckItem(Menu_DMA, _T("Log DMA"));
	m_Popup->AppendCheckItem(Menu_IO, _T("Log IO Write"));

	m_Popup->Check(Menu_DMA,true);
	m_Popup->Check(Menu_IO,true);
	
	m_Cnt=0;
	m_ScrollPosition=0;
	
	int w,h;
	GetClientSize(&w,&h);
	int page_cnt= (h-5)/12;

	memset(m_LevelShow,1,10);
	
	
	SetScrollbar(wxVERTICAL,0,page_cnt,512,TRUE);	

}

///////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////

mdLogPanel::~mdLogPanel()
{
}

void mdLogPanel::Flush(void)
{	m_Cnt=0;
	for(int i=0;i<512;i++)
		m_Text[i]=_T("");

	m_ScrollPosition=0;
		this->SetScrollPos(wxVERTICAL,0,TRUE);
}

void mdLogPanel::LogChar(int level,char text)
{	
	int w,h;
	GetClientSize(&w,&h);
	int page_cnt= (h-5)/10;

	m_LastIndex=m_Cnt;
	
	m_Level[m_Cnt]=level;
	m_Text[m_Cnt]=m_Text[m_Cnt] +  _T(text);

	if(text==0 || m_Text[m_Cnt].Trim().Length()>512)
	{	m_Cnt++;
		m_Cnt &=511;
		if(m_Cnt==0)
			Flush();

		int i=this->GetScrollPos(wxVERTICAL);	

		if(m_Cnt>=(i+page_cnt))
		{	this->SetScrollPos(wxVERTICAL,i+1,TRUE);
			m_ScrollPosition =i+1;	
		}

		if(m_ScrollPosition>512 || m_Cnt==0)
		{	m_ScrollPosition=0;
			this->SetScrollPos(wxVERTICAL,0,TRUE);
		}
	}
}


void mdLogPanel::LogMessage(int level,char* text)
{	
	if(m_LevelShow[level]==0)
		return;

	int w,h;
	GetClientSize(&w,&h);
	int page_cnt= (h-5)/12;
	m_LastIndex=m_Cnt;
	
	m_Level[m_Cnt]=level;
	m_Text[m_Cnt++]=_T(text);
	
	m_Cnt &=511;

	if(m_Cnt==0)
		Flush();

	int i=this->GetScrollPos(wxVERTICAL);	

	if(m_Cnt>=(i+page_cnt))
	{
		this->SetScrollPos(wxVERTICAL,i+1,TRUE);
		m_ScrollPosition =i+1 ;	
	}

	if(m_ScrollPosition>512 || m_Cnt==0)
	{	m_ScrollPosition=0;
		this->SetScrollPos(wxVERTICAL,0,TRUE);
	}
}

void mdLogPanel::OnThumbTrack(wxScrollWinEvent &event)
{
	if( event.GetOrientation()==wxVERTICAL)
	{	m_ScrollPosition=event.GetPosition();
		this->SetScrollPos(wxVERTICAL,m_ScrollPosition,TRUE);		
		m_ScrollPosition=GetScrollPos(wxVERTICAL);
	}
	Refresh();
}

void mdLogPanel::OnSize(wxSizeEvent &event)
{
	int w,h;
	GetClientSize(&w,&h);
	int page_cnt= (h-5)/12;

	SetScrollbar(wxVERTICAL,0,page_cnt,512,TRUE);	
}

///////////////////////////////////////////////////////////////////////////////
// Menu contextuel
///////////////////////////////////////////////////////////////////////////////


void mdLogPanel::OnContextMenu(wxContextMenuEvent& event)
{
	PopupMenu( m_Popup);
}


void mdLogPanel::OnClearLog(wxCommandEvent& event)
{
	if(wxMessageBox(_T("Do you want to clear the log window ?"), "Confirm",wxYES_NO, this)==wxYES)
	{	Flush();
		this->Refresh();
	}
}


void mdLogPanel::OnSaveLog(wxCommandEvent& event)
{
}

void mdLogPanel::OnDMA(wxCommandEvent& event)
{
	m_LevelShow[1]=!m_LevelShow[1];

	m_Popup->Check(Menu_DMA,m_LevelShow[1]);
	

}

void mdLogPanel::OnIO(wxCommandEvent& event)
{
	m_LevelShow[2]=!m_LevelShow[2];

	m_Popup->Check(Menu_IO,m_LevelShow[2]);
}


///////////////////////////////////////////////////////////////////////////////
// Paint
///////////////////////////////////////////////////////////////////////////////

void mdLogPanel::OnEraseBackground(wxEraseEvent& event)
{
}


void mdLogPanel::OnPaint(wxPaintEvent& event)
{
	wxFont fnt(8,wxDEFAULT,wxNORMAL,wxNORMAL,false,"Arial",wxFONTENCODING_SYSTEM );
	int w,h;
	GetClientSize(&w,&h);
	int text_size=12;

	wxSize si= fnt.GetPixelSize();
	text_size= 13;

	int page_cnt= (h-5)/text_size;

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
	
	for(int i=0;i<page_cnt;i++)
	{
		if(m_ScrollPosition + i<512)
		{
			int y=i*text_size;

			int idx= (m_ScrollPosition + i) & 511;

			if((m_ScrollPosition + i)==m_LastIndex)
			{	
				dc.SetBrush(wxBrush(wxColour(255,255,0)));
				dc.DrawRectangle(0,y,w,text_size+4);
				dc.SetTextForeground(*wxRED);			
			}
			else
			{	dc.SetTextForeground(*wxBLACK);}

			if(m_Level[idx]==1 && m_Text[idx].Length()>0)
			{
				dc.SetBrush(wxBrush(wxColour(0,255,255)));
				dc.DrawRectangle(0,y,30,text_size+4);				
								
				dc.DrawText("DMA", 1,y);
			}

			if(m_Level[idx]==2 && m_Text[idx].Length()>0)
			{
				dc.SetBrush(wxBrush(wxColour(0,255,0)));
				dc.DrawRectangle(0,y,30,text_size);		
				
				//dc.SetTextForeground(*wxWHITE);	
				dc.DrawText("IO", 8,y);
			}
			
			dc.SetTextForeground(*wxBLACK);	
			dc.DrawText(m_Text[idx], 34,y);

		}
	}
		
	dc.EndDrawing();
}



///////////////////////////////////////////////////////////////////////////////