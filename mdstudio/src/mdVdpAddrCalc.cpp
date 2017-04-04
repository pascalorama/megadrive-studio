#include "include/mdShared.h"
#include "include/mdTextCtrl.h"
#include "include/mdVdpAddrCalc.h"

///////////////////////////////////////////////////////////////////////////////
// events
BEGIN_EVENT_TABLE(mdVdpAddrCalc, wxPanel)
	EVT_COMBOBOX  (50,mdVdpAddrCalc::OnCombo)
	EVT_TEXT(150, mdVdpAddrCalc::OnValueChange) 
	EVT_TEXT(200, mdVdpAddrCalc::OnAddressChange) 
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// constructor
mdVdpAddrCalc::mdVdpAddrCalc(wxWindow* parent,
					 wxWindowID id,
					 const wxPoint& pos,
					 const wxSize& size,
					 long style,
					 const wxString& name):wxPanel(parent,id,pos,size,style,name)
{
	m_UpdateOther=TRUE;

	/*
	Bits CD3-CD0
	0000b : VRAM read
	0001b : VRAM write
	0011b : CRAM write
	0100b : VSRAM read
	0101b : VSRAM write
	1000b : CRAM read

	CD1 CD0 A13 A12 A11 A10 A09 A08     (D31-D24)
    A07 A06 A05 A04 A03 A02 A01 A00     (D23-D16)
     ?   ?   ?   ?   ?   ?   ?   ?      (D15-D8)
    CD5 CD4 CD3 CD2  ?   ?  A15 A14     (D7-D0)

    CDx = VDP code (0-3F)
    Axx = VDP address (00-FFFF)

	#define VDP_ADR(mode,dst)(  (((dst)&0x3fff)<<16) | (((dst)&0xffff)>>14)) | ( ((((mode)&0x3c)>>2)<<4) | (((mode)&3)<<30)  ))
	*/
	
	wxFont fntArial10(10,wxDEFAULT,wxNORMAL,wxBOLD,false,"Arial",wxFONTENCODING_SYSTEM );		
	wxBoxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);

	// init combobox
	m_CmbCode=new wxComboBox(this,50,
								_T(""),
								wxDefaultPosition,
								wxSize(295,24),
								NULL,
								NULL,
								wxCB_READONLY,
								wxDefaultValidator,
								"Combo");	
	// append to combo
	m_CmbCode->Append("0:VRAM read",new int(0));
	m_CmbCode->Append("1:VRAM write",new int(1));
	m_CmbCode->Append("2:Unknown",new int(2));
	m_CmbCode->Append("3:CRAM write",new int(3));
	m_CmbCode->Append("4:VSRAM read",new int(4));
	m_CmbCode->Append("5:VSRAM write",new int(5));
	m_CmbCode->Append("6:Unknown",new int(6));
	m_CmbCode->Append("7:Unknown",new int(7));
	m_CmbCode->Append("8:CRAM read",new int(8));
	
	// set default
	m_CmbCode->SetSelection(0);

	// add to sizer
	sizerTop->Add(m_CmbCode,0,wxEXPAND ,0);	


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
													"Value:",
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

	m_Value=new mdTextCtrl(	pnl,
								150,
								_T(""),
								wxPoint(60,4),
								wxSize(100,20),
								wxTE_PROCESS_ENTER | wxTE_CAPITALIZE,
								*tx);
	m_Value->SetMaxLength(8);

	
	// Address Lable
	wxStaticText *lblAddressValue=new wxStaticText(	pnl,
													-1,
													"Address:",
													wxPoint(4,30),
													wxDefaultSize);
	lblAddressValue->SetFont(fntArial10);

	// textbox address
	m_Address=new mdTextCtrl(	pnl,
								200,
								_T(""),
								wxPoint(60,30),
								wxSize(100,20),
								wxTE_PROCESS_ENTER | wxTE_CAPITALIZE,
								*tx);
	m_Address->SetMaxLength(4);



	sizerTop->Add(pnl,1,wxEXPAND ,0);	


	// finish panel layout
	SetAutoLayout(TRUE);
    SetSizer(sizerTop);

    sizerTop->SetSizeHints(this);
    sizerTop->Fit(this);
}
 
///////////////////////////////////////////////////////////////////////////////
// Destructeur
mdVdpAddrCalc::~mdVdpAddrCalc()
{

}

///////////////////////////////////////////////////////////////////////////////
// Changement de valeur de la combo

void mdVdpAddrCalc::OnCombo( wxCommandEvent &event )
{	
	UpdateValue();		
}

void mdVdpAddrCalc::OnAddressChange( wxCommandEvent &event )
{
	UpdateValue();	
}

void mdVdpAddrCalc::OnValueChange( wxCommandEvent &event )
{	
	UpdateAddress();
}

void mdVdpAddrCalc::UpdateAddress()
{
	int code=0;
	unsigned long value=0;
	unsigned long address=0;
	wxString v;
	wxString a;

	if(m_UpdateOther==TRUE)
	{
		m_UpdateOther=FALSE;
		v=m_Value->GetValue();
		if(v.Trim().Length()>0)
		{	v.ToULong(&value,16);}

		code = ((value & 0xc0000000)>>30) | ((value & 0xf0)>>2);
		address = ((value & 0x3fff0000)>>16) | ((value & 3)<<14);

		a=a.Format("%.4x",address);
		m_Address->SetValue(a);	

		for(int i=0;i<m_CmbCode->GetCount();i++)
		{	int j=*(int*) m_CmbCode->GetClientData(i);
			if(j==code)
			{	m_CmbCode->SetSelection(i);	}
		}
		m_UpdateOther=TRUE;
	}
}

void mdVdpAddrCalc::UpdateValue()
{
	if(m_UpdateOther==TRUE)
	{	int code=*(int*) m_CmbCode->GetClientData(m_CmbCode->GetSelection());
		wxString n,a;
		long address=0;
		long value=0;

		m_UpdateOther=FALSE;

		a=m_Address->GetValue();
		if(a.Trim().Length()>0)
		{	a.ToLong(&address,16);
		}
		value= (  ((address&0x3fff)<<16) | ((address&0xffff)>>14)  | (((code & 0x3c)>>2)<<4) | ((code&3)<<30)  );
		n=n.Format("%.8x",value);
		m_Value->SetValue(n);

		m_UpdateOther=TRUE;
	}
}