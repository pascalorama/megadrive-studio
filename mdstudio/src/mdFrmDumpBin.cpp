#include "include/mdShared.h"
#include "include/mdTextCtrl.h"
#include "include/mdFrmMain.h"
#include "include/mdApp.h"
#include "include/mdFrmDumpBin.h"

///////////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(mdFrmDumpBin, wxDialog)
//	EVT_CLOSE(mdFrmDumpBin::OnClose)    
	EVT_BUTTON(200, mdFrmDumpBin::OnDump)
	EVT_BUTTON(250, mdFrmDumpBin::OnCancel)
END_EVENT_TABLE()
///////////////////////////////////////////////////////////////////////////////
mdFrmDumpBin::mdFrmDumpBin(	wxWindow *frame,
							const wxChar *title)
							: wxDialog(frame, wxID_ANY, title, wxDefaultPosition, wxSize(400, 400),wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
	wxFont fntArial10(10,wxDEFAULT,wxNORMAL,wxBOLD,false,"Arial",wxFONTENCODING_SYSTEM );		
	this->Centre(wxBOTH);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	
	// centre
	wxPanel *pnl=new wxPanel(	this,
								-1,
								wxDefaultPosition,
								wxSize(0,0),
								wxTAB_TRAVERSAL,
								"Center");

	// label value
	wxStaticText *lblAddressValue=new wxStaticText(	pnl,
													-1,
													"$Base Address:",
													wxPoint(8,8),
													wxDefaultSize);
	lblAddressValue->SetFont(fntArial10);
	
	
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

	m_BaseAddress=new mdTextCtrl(	pnl,
								150,
								_T(""),
								wxPoint(120,8),
								wxSize(100,20),
								wxTE_PROCESS_ENTER | wxTE_CAPITALIZE,
								*tx);
	m_BaseAddress->SetMaxLength(8);

	// label value
	wxStaticText *lblLengthValue=new wxStaticText(	pnl,
													-1,
													"$Length:",
													wxPoint(8,30),
													wxDefaultSize);
	lblLengthValue->SetFont(fntArial10);

	m_Length=new mdTextCtrl(	pnl,
								150,
								_T(""),
								wxPoint(120,30),
								wxSize(100,20),
								wxTE_PROCESS_ENTER | wxTE_CAPITALIZE,
								*tx);
	m_Length->SetMaxLength(8);

	
	wxButton *btn=new wxButton(	pnl,
								200,
								"Dump",
								wxPoint(220-60-65,60),
								wxSize(60,30));

	wxButton *btnCancel=new wxButton(	pnl,
								250,
								"Cancel",
								wxPoint(220-60,60),
								wxSize(60,30));
	
	sizer->Add(pnl,1,wxEXPAND ,0);	
	
	// finish panel layout
	SetAutoLayout(TRUE);
    SetSizer(sizer);

    sizer->SetSizeHints(this);
    sizer->Fit(this);

	MaxLength=0;
	MaxBase=0;

	Show(true);	
}

void mdFrmDumpBin::OnDump(wxCommandEvent& event)
{
	wxString b;

	b=m_Length->GetValue();
	if(b.Trim().Length()==0)
	{	wxLogError("Length is mandatory");
		m_Length->SetFocus();
		return;
	}

	b=m_BaseAddress->GetValue();
	if(b.Trim().Length()==0)
	{	wxLogError("Base address is mandatory");
		m_BaseAddress->SetFocus();
		return;
	}

	long base=0;
	long lenght=0;
	
	b=m_Length->GetValue().Trim();
	b.ToLong(&lenght,16);

	b=m_BaseAddress->GetValue().Trim();
	b.ToLong(&base,16);

	if(lenght>MaxLength)
	{	wxLogError("Length too big");
		m_Length->SetFocus();
		return;
	}

	if(base+lenght>MaxBase + MaxLength)
	{	wxLogError("Error selection invalid");
		return;
	}

	wxFileDialog	*fd= new wxFileDialog(	NULL,
											_T("Choose a file to dump"),
											"",
											"",
											"Bin (*.bin)|*.bin",
											wxSAVE | wxCHANGE_DIR |wxOVERWRITE_PROMPT );
	if(fd->ShowModal()==wxID_OK)
	{	*FileName=fd->GetPath();
		this->EndModal(wxID_OK);
	}
}

void mdFrmDumpBin::OnCancel(wxCommandEvent& event)
{
	this->EndModal(wxID_CANCEL);
}

void mdFrmDumpBin::SetOptions(wxString& filename,int default_base,int length,int max_base,int max_lenght)
{
	FileName=&filename;
	wxString b;
	MaxLength=max_lenght;
	MaxBase=max_base;

	b=b.Format("%x",default_base);
	m_BaseAddress->SetValue(b);

	b=b.Format("%x",length);
	m_Length->SetValue(b);

}

int mdFrmDumpBin::GetBaseAddress()
{
	long a=0;

	if(m_BaseAddress->GetValue().Trim().Length()>0)
	{	m_BaseAddress->GetValue().Trim().ToLong(&a,16);	}

	return (int)a;
}

int mdFrmDumpBin::GetLength()
{	long a=0;

	if(m_Length->GetValue().Trim().Length()>0)
	{	m_Length->GetValue().Trim().ToLong(&a,16);	}


	return a;
}
