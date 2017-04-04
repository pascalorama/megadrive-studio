#include "include/mdShared.h"
#include "include/mdTextCtrl.h"
#include "include/mdGotoPanel.h"
#include "include/mdFrmMain.h"
#include "include/mdApp.h"
#include "include/mdFrmGoto.h"


BEGIN_EVENT_TABLE(mdFrmGoto, wxFrame)
	EVT_CLOSE(mdFrmGoto::OnClose)    
	EVT_BUTTON(200, mdFrmGoto::OnGo)
	//	EVT_BUTTON(200, mdGotoPanel::OnGo)
	EVT_TEXT_ENTER(150,mdFrmGoto::OnGo)
END_EVENT_TABLE()

mdFrmGoto::mdFrmGoto(	wxFrame *frame,
									const wxChar *title,
									int x, int y, int w, int h)
									: wxFrame(frame, wxID_ANY, title, wxPoint(x, y), wxSize(w, h),wxSTAY_ON_TOP | wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
	this->Centre(wxBOTH);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	
	// ajoute le panneau principal
	m_GotoPanel=new mdGotoPanel(this);
	sizer->Add(m_GotoPanel,1,wxEXPAND ,0);	
	
	// centre
	wxPanel *pnl=new wxPanel(	this,
								-1,
								wxDefaultPosition,
								wxSize(0,0),
								wxTAB_TRAVERSAL,
								"Center");

	wxButton *btn=new wxButton(	pnl,
								200,
								"Go",
								wxPoint(110,0),
								wxSize(50,20));
	sizer->Add(pnl,1,wxEXPAND ,0);	
	
	// finish panel layout
	SetAutoLayout(TRUE);
    SetSizer(sizer);

    sizer->SetSizeHints(this);
    sizer->Fit(this);

	Show(true);	
}

void mdFrmGoto::OnClose(wxCloseEvent& event)
{
	event.Veto();
	this->Hide();
}

void mdFrmGoto::OnGo( wxCommandEvent &event)
{
	unsigned long value=0;
    wxString v=m_GotoPanel->GetAddress();
	if(v.Trim().Length()>0)
	{	v.ToULong(&value,16);
		gFrmMain->GotoAddress(value);
	}

	this->Hide();
}