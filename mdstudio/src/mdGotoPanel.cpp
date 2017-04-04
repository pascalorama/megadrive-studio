//#include "include/mdShared.h"
//#include "include/mdTextCtrl.h"
//#include "include/mdGotoPanel.h"

#include "include/mdShared.h"
#include "include/mdTextCtrl.h"
#include "include/mdFrmGoto.h"
#include "include/mdGotoPanel.h"

#include "include/mdfrmmain.h"
#include "include/mdApp.h"

///////////////////////////////////////////////////////////////////////////////
// events
BEGIN_EVENT_TABLE(mdGotoPanel, wxPanel)
//	EVT_BUTTON(200, mdGotoPanel::OnGo)
//	EVT_TEXT_ENTER(150,mdFrmGoto::OnGo)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// constructor
mdGotoPanel::mdGotoPanel(wxWindow* parent,
					 wxWindowID id,
					 const wxPoint& pos,
					 const wxSize& size,
					 long style,
					 const wxString& name):wxPanel(parent,id,pos,size,style,name)
{

	wxFont fntArial10(10,wxDEFAULT,wxNORMAL,wxBOLD,false,"Arial",wxFONTENCODING_SYSTEM );		
	wxBoxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);

	// centre
	wxPanel *pnl=new wxPanel(	this,
								-1,
								wxDefaultPosition,
								wxSize(0,150),
								wxTAB_TRAVERSAL,
								"Center");

	// label value
	wxStaticText *lblHeaderValue=new wxStaticText(	pnl,
													-1,
													"Address:",
													wxPoint(4,4),
													wxDefaultSize);
	lblHeaderValue->SetFont(fntArial10);
	
	// Text Value:
	// textbox d'encodage
	wxArrayString inc;
	inc.Add(_T("a"));
	inc.Add(_T("b"));
	inc.Add(_T("c"));
	inc.Add(_T("d"));
	inc.Add(_T("e"));
	inc.Add(_T("f"));
	inc.Add(_T("A"));
	inc.Add(_T("B"));
	inc.Add(_T("C"));
	inc.Add(_T("D"));
	inc.Add(_T("E"));
	inc.Add(_T("F"));
	inc.Add(_T("0"));
	inc.Add(_T("1"));
	inc.Add(_T("2"));
	inc.Add(_T("3"));
	inc.Add(_T("4"));
	inc.Add(_T("5"));
	inc.Add(_T("6"));
	inc.Add(_T("7"));
	inc.Add(_T("8"));
	inc.Add(_T("9"));

	wxTextValidator *tx=new wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
	tx->SetIncludes(inc);

	m_Address=new mdTextCtrl(	pnl,
								150,
								_T(""),
								wxPoint(60,4),
								wxSize(100,20),
								wxTE_PROCESS_ENTER | wxTE_CAPITALIZE,
								*tx);
	m_Address->SetMaxLength(8);
	

	sizerTop->Add(pnl,1,wxEXPAND ,0);	

	// finish panel layout
	SetAutoLayout(TRUE);
    SetSizer(sizerTop);

    sizerTop->SetSizeHints(this);
    sizerTop->Fit(this);

	m_Address->SelectAll();
}
 
///////////////////////////////////////////////////////////////////////////////
// Destructeur
mdGotoPanel::~mdGotoPanel()
{

}

void mdGotoPanel::OnGo( wxCommandEvent &event)
{
	unsigned long value=0;
    wxString v=m_Address->GetValue();
	if(v.Trim().Length()>0)
	{	v.ToULong(&value,16);
		gFrmMain->GotoAddress(value);
	}
}



