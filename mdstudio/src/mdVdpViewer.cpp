#include "include/mdShared.h"
#include "include/mdEmu.h"
#include "include/mdFrmMain.h"
#include "include/mdApp.h"
#include "include/mdTextCtrl.h"
#include "include/mdVdpViewer.h"

///////////////////////////////////////////////////////////////////////////////
// events
BEGIN_EVENT_TABLE(mdVdpViewer, wxPanel)
    EVT_COMBOBOX  (50,mdVdpViewer::OnCombo)
	EVT_CHECKLISTBOX(100,mdVdpViewer::OnCheck) 
	EVT_BUTTON(200, mdVdpViewer::OnUpdate)
	EVT_TEXT_ENTER(150,mdVdpViewer::OnUpdate)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// constructor
mdVdpViewer::mdVdpViewer(wxWindow* parent,
					 wxWindowID id,
					 const wxPoint& pos,
					 const wxSize& size,
					 long style,
					 const wxString& name):wxPanel(parent,id,pos,size,style,name)
{
	wxFont fntArial10(10,wxDEFAULT,wxNORMAL,wxBOLD,false,"Arial",wxFONTENCODING_SYSTEM );
	
	m_CurrentReg=NULL;
	wxBoxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);

	// init combobox
	m_CmbIO=new wxComboBox(this,50,
								_T(""),
								wxDefaultPosition,
								wxSize(295,24),
								NULL,
								NULL,
								wxCB_READONLY,
								wxDefaultValidator,
								"Combo");	
	// append to combo
	m_CmbIO->Append(m_R0.GetName(),&m_R0);
	m_CmbIO->Append(m_R1.GetName(),&m_R1);
	m_CmbIO->Append(m_R2.GetName(),&m_R2);
	m_CmbIO->Append(m_R3.GetName(),&m_R3);
	m_CmbIO->Append(m_R4.GetName(),&m_R4);
	m_CmbIO->Append(m_R5.GetName(),&m_R5);
	m_CmbIO->Append(m_R7.GetName(),&m_R7);
	m_CmbIO->Append(m_R10.GetName(),&m_R10);
	m_CmbIO->Append(m_R11.GetName(),&m_R11);
	m_CmbIO->Append(m_R12.GetName(),&m_R12);
	m_CmbIO->Append(m_R13.GetName(),&m_R13);
	m_CmbIO->Append(m_R15.GetName(),&m_R15);
	m_CmbIO->Append(m_R16.GetName(),&m_R16);
	m_CmbIO->Append(m_R17.GetName(),&m_R17);
	m_CmbIO->Append(m_R18.GetName(),&m_R18);
	m_CmbIO->Append(m_R19.GetName(),&m_R19);
	m_CmbIO->Append(m_R20.GetName(),&m_R20);
	m_CmbIO->Append(m_R21.GetName(),&m_R21);
	m_CmbIO->Append(m_R22.GetName(),&m_R22);
	m_CmbIO->Append(m_R23.GetName(),&m_R23);

	// set default
	m_CurrentReg=&m_R0;	
	m_CmbIO->SetSelection(0);

	// add to sizer
	sizerTop->Add(m_CmbIO,0,wxEXPAND ,0);	

	// add the check list box
	m_CheckList=new wxCheckListBox(	this,
									100,
									wxDefaultPosition,
									wxSize(295, 125), 
									NULL,
									NULL,
									wxLB_EXTENDED);
	
	DisplayDetails();
	
	sizerTop->Add(m_CheckList,0,wxEXPAND ,0);	

	// centre
	wxPanel *pnl=new wxPanel(	this,
								-1,
								wxDefaultPosition,
								wxSize(0,150),
								wxTAB_TRAVERSAL,
								"Center");

	//pnl->SetBackgroundColour(wxColour(*wxBLACK));

	// label value
	wxStaticText *lblHeaderValue=new wxStaticText(	pnl,
													-1,
													"Value:",
													wxPoint(4,4),
													wxDefaultSize);
	lblHeaderValue->SetFont(fntArial10);

	// label contenant la valeur
	m_Value=new wxStaticText(	pnl,
								-1,
								"$00",
								wxPoint(50,4),
								wxDefaultSize);
	m_Value->SetFont(fntArial10);

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

	m_NewValue=new mdTextCtrl(	pnl,
								150,
								_T(""),
								wxPoint(190,4),
								wxSize(50,20),
								wxTE_PROCESS_ENTER | wxTE_CAPITALIZE,
								*tx);
	m_NewValue->SetMaxLength(2);

	wxButton *btn=new wxButton(	pnl,
								200,
								"Update",
								wxPoint(240,4),
								wxSize(50,20));
	// text
	m_Comment=new wxStaticText(	pnl,
								250,
								_T(""),
								wxPoint(0,25),
								wxSize(294,100),
								wxSUNKEN_BORDER | wxST_NO_AUTORESIZE);	


	sizerTop->Add(pnl,0,wxEXPAND ,0);	

	UpdateValue();
	
	// finish panel layout
	SetAutoLayout(TRUE);
    SetSizer(sizerTop);

    sizerTop->SetSizeHints(this);
    sizerTop->Fit(this);
}
 
///////////////////////////////////////////////////////////////////////////////
// Destructeur
mdVdpViewer::~mdVdpViewer()
{

}

///////////////////////////////////////////////////////////////////////////////
// Changement de valeur de la combo
void mdVdpViewer::OnCombo( wxCommandEvent &event )
{
  	m_CurrentReg=(VDPRegInfo*) m_CmbIO->GetClientData(m_CmbIO->GetSelection());
	DisplayDetails();
	UpdateValue();
}

void mdVdpViewer::OnCheck(wxCommandEvent &event)
{
	int nSel = event.GetSelection();
	if(m_CurrentReg!=NULL)
	{
		int v=m_CurrentReg->GetValue();	
		
		if(m_CheckList->IsChecked(nSel)==false)
		{	// uncheck
			int mask=(int) ~(1<<((7-nSel)));
			v&=mask;
		}
		else
		{	//check
			v |= (int) (1<<((7-nSel)));
		}

		m_CurrentReg->SetValue((unsigned char)v);
		UpdateValue();
	}
}

///////////////////////////////////////////////////////////////////////////////
// Affiche
void mdVdpViewer::DisplayDetails(void)
{
	if(m_CurrentReg!=NULL)
	{	m_CheckList->Clear();
		
		for(int i=0;i<8;i++)
		{	
			bool b=false;
			int v=m_CurrentReg->GetValue();
			if((v & (1<<((7-i))))!=0)
				b=true;
			
			m_CheckList->Append(m_CurrentReg->GetBitDescription(7-i));		
			m_CheckList->Check(i,b);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Update string
void mdVdpViewer::UpdateValue(void)
{
	wxString n;
	if(m_CurrentReg!=NULL)
	{	n=n.Format("$%.2x",m_CurrentReg->GetValue());
		n=n.Upper();
		m_Value->SetLabel(n);
		m_Comment->SetLabel(m_CurrentReg->GetComment());
	}
	else
	{	m_Value->SetLabel("$00");
		m_Comment->SetLabel("");
	}
}

///////////////////////////////////////////////////////////////////////////////
// Update value base on textbox
void mdVdpViewer::OnUpdate( wxCommandEvent &event )
{
	wxString n;
	long i=0;
	n=m_NewValue->GetValue();

	if(n.Trim().Length()>0)
	{	n.ToLong(&i,16);	
	}
	m_NewValue->SetValue(_T(""));
	m_CurrentReg->SetValue(i);

	DisplayDetails();
	UpdateValue();	
}

void mdVdpViewer::SetValue(int idx,unsigned char value)
{
	VDPRegInfo dummy;
	VDPRegInfo *regs[24]={	(VDPRegInfo*)	&m_R0,
							(VDPRegInfo*)	&m_R1,
							(VDPRegInfo*)	&m_R2,
							(VDPRegInfo*)	&m_R3,
							(VDPRegInfo*)	&m_R4,
							(VDPRegInfo*)	&m_R5,
							&dummy,
							(VDPRegInfo*)	&m_R7,
							&dummy,
							&dummy,
							(VDPRegInfo*)	&m_R10,
							(VDPRegInfo*)	&m_R11,
							(VDPRegInfo*)	&m_R12,
							(VDPRegInfo*)	&m_R13,
							&dummy,
							(VDPRegInfo*)	&m_R15,
							(VDPRegInfo*)	&m_R16,
							(VDPRegInfo*)	&m_R17,
							(VDPRegInfo*)	&m_R18,
							(VDPRegInfo*)	&m_R19,
							(VDPRegInfo*)	&m_R20,
							(VDPRegInfo*)	&m_R21,
							(VDPRegInfo*)	&m_R22,
							(VDPRegInfo*)	&m_R23,
						};

	if(idx<24)
		regs[idx]->SetValue(value);
}

void mdVdpViewer::DisplayRegister(int idx)
{
	VDPRegInfo dummy;
	
	VDPRegInfo *regs[24]={	(VDPRegInfo*)	&m_R0,
							(VDPRegInfo*)	&m_R1,
							(VDPRegInfo*)	&m_R2,
							(VDPRegInfo*)	&m_R3,
							(VDPRegInfo*)	&m_R4,
							(VDPRegInfo*)	&m_R5,
							&dummy,
							(VDPRegInfo*)	&m_R7,
							&dummy,
							&dummy,
							(VDPRegInfo*)	&m_R10,
							(VDPRegInfo*)	&m_R11,
							(VDPRegInfo*)	&m_R12,
							(VDPRegInfo*)	&m_R13,
							&dummy,
							(VDPRegInfo*)	&m_R15,
							(VDPRegInfo*)	&m_R16,
							(VDPRegInfo*)	&m_R17,
							(VDPRegInfo*)	&m_R18,
							(VDPRegInfo*)	&m_R19,
							(VDPRegInfo*)	&m_R20,
							(VDPRegInfo*)	&m_R21,
							(VDPRegInfo*)	&m_R22,
							(VDPRegInfo*)	&m_R23,
						};

	if(idx<24)
	{	unsigned char* r=gEmu->VDPRegs();
		regs[idx]->SetValue(r[idx]);
		DisplayDetails();
		UpdateValue();	
	}
}
