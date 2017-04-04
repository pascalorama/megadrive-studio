///////////////////////////////////////////////////////////////////////////////
//
//

///////////////////////////////////////////////////////////////////////////////
// app includes
#include "include/mdShared.h"
#include "include/mdSpy.h"
#include "include/mdSpiesPanel.h"
#include "include/mdTextCtrl.h"
#include "include/mdFrmSpyAttr.h"

///////////////////////////////////////////////////////////////////////////////
// Event Table

BEGIN_EVENT_TABLE(mdFrmSpyAttr, wxDialog)	
	EVT_BUTTON(-1, mdFrmSpyAttr::OnButton)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// Constructor

mdFrmSpyAttr::mdFrmSpyAttr(	wxWindow *parent)
							:wxDialog(parent, -1, wxString(_T("Add/Update Spy Attributes")))    
{
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->SetMinSize(300,165);
	m_UpdateMode=false;
	m_InsertIndex=-1;

	// création panel central
	wxPanel *PnlCenter=new wxPanel(	this,
									-1,
									wxDefaultPosition,
									wxSize(0,80),
									wxTAB_TRAVERSAL,
									"Center");	

	// Label Nom:
	wxStaticText *m_LblName = new wxStaticText(PnlCenter,-1,
												"Name",
												wxPoint(10,8),
												wxDefaultSize,
												wxALIGN_LEFT ,
													"statictext");
	// textbox pour le nom
	m_TxtName = new wxTextCtrl(	PnlCenter,
								-1,
								"",
								wxPoint(100,8),
								wxSize(184,20),
								wxALIGN_LEFT,
								wxDefaultValidator,
								"textbox");
	m_TxtName->SetMaxLength(30);

	// addresse
	wxStaticText *m_LblAddress = new wxStaticText(PnlCenter,-1,
													"Address (hex)",
													wxPoint(10,32),
													wxDefaultSize,
													wxALIGN_LEFT,
													"statictext");
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

	wxTextValidator *txNum=new wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
	txNum->SetIncludes(inc);

	m_TxtAddress = new mdTextCtrl(PnlCenter,
									-1,
									_T(""),
									wxPoint(100,32),
									wxSize(184,20),
									wxALIGN_LEFT |  wxTE_CAPITALIZE,
									*txNum,
									"textbox");
	m_TxtAddress->SetMaxLength(10);
	
	// combobox pour la taille
	wxStaticText *m_LblSize = new wxStaticText(PnlCenter,-1,
													"Size",
													wxPoint(10,32+24),
													wxDefaultSize,
													wxALIGN_LEFT,
													"statictext");
	
	m_CmbSize= new wxComboBox(	PnlCenter,
								-1,
								_T("Byte"),
								wxPoint(100,32+24),
								wxSize(50,20),
								NULL,
								NULL,
								wxCB_READONLY,
								wxDefaultValidator,
								"ComboSize");

	m_CmbSize->Append(_T("Byte"));
	m_CmbSize->Append(_T("Word"));
	m_CmbSize->Append(_T("Long"));

	m_CmbSize->SetValue(_T("Byte"));

	// textbox pour la taille du tableau
	wxStaticText *m_LblArraySize = new wxStaticText(PnlCenter,-1,
													"Array size",
													wxPoint(10,32+24+24),
													wxDefaultSize,
													wxALIGN_LEFT,
													"statictext");
	
	m_CmbArraySize= new wxComboBox(	PnlCenter,
									-1,
									_T("1"),
									wxPoint(100,32+24+24),
									wxSize(50,20),
									NULL,
									NULL,
									wxCB_READONLY,
									wxDefaultValidator,
									"ComboArray");
	
	for(int i=1;i<100;i++)
	{
		wxString b;
		b=b.Format("%d",i);
		m_CmbArraySize->Append(b);
	}
	m_CmbArraySize->SetValue(_T("1"));

	//
	m_ChkHexa=new wxCheckBox(	PnlCenter,
								-1,
								_T("Hex display"),
								wxPoint(100,32+24+24+24));
	m_ChkHexa->SetValue(TRUE);

	// ajoute le panneau principal
	sizer->Add(PnlCenter,1,wxEXPAND ,0);	

	// ajoute les boutions
	wxBoxSizer *sizerBtn = new wxBoxSizer(wxHORIZONTAL);

	m_BtnOk= new wxButton(this,-1,_T("&Ok"));
	m_BtnOk->SetDefault();
	
	sizerBtn->Add(m_BtnOk,0,wxALIGN_RIGHT | wxALL,5);

	m_BtnCancel= new wxButton(this,wxID_CANCEL,_T("&Cancel"));
	sizerBtn->Add(m_BtnCancel,0,wxALIGN_RIGHT | wxALL,5);

	// Add the button sizer to the top one -
	sizer->Add(sizerBtn,0,wxALIGN_RIGHT| wxALL,NULL);
		
	// Finish window layout
	SetAutoLayout(TRUE);
    SetSizer(sizer);

    sizer->SetSizeHints(this);
    sizer->Fit(this);
	m_SpiesPanel=NULL;

	this->Centre(wxBOTH);	
}

///////////////////////////////////////////////////////////////////////////////
// Destructor

mdFrmSpyAttr::~mdFrmSpyAttr()
{	
	this->Destroy();
}

void mdFrmSpyAttr::SetUpdateSpy(mdSpy *spy)
{
	m_id=spy->m_ListIndex;
	m_UpdateMode=true;
	
	m_CmbArraySize->Disable();
	m_TxtAddress->Disable();

	if(spy!=NULL)
	{
		wxString b;
		b=b.Format("%x",spy->GetAddress());

		m_TxtName->SetValue(spy->GetName());
		if(spy->m_Length==1)
		{	m_CmbSize->SetSelection(0);	}
		else
		{	m_CmbSize->SetSelection(spy->m_Length/2);}
		
		m_TxtAddress->SetValue(b);

		m_ChkHexa->SetValue(spy->m_DisplayHexa);				
	}
}

void mdFrmSpyAttr::SetSpiesPanel(mdSpiesPanel *panel)
{
	m_SpiesPanel=panel;
}

///////////////////////////////////////////////////////////////////////////////
// Sélection est bonne

void mdFrmSpyAttr::OnButton(wxCommandEvent& event)
{
	if(event.GetEventObject()==m_BtnOk)
	{	
		bool noErrors=true;

		// nom est obligatoire
		if(m_TxtName->GetValue().Trim().Length()==0)
		{	wxLogError("Spy name is mandatory");
			m_TxtName->SetFocus();
			noErrors=false;
		}

		// addresse est obligatoire
		if(m_TxtAddress->GetValue().Trim().Length()==0)
		{	wxLogError("Address is mandatory");
			m_TxtName->SetFocus();
			noErrors=false;		
		}

		// vérifie si le symbole n'existe pas déja
		
		
		if(noErrors)
		{
			// remplit les valeurs
			long addri=0;
			int size=m_CmbSize->GetSelection()*2;
			int arraysize=m_CmbArraySize->GetSelection()+1;
			wxString addr=m_TxtAddress->GetValue();
			
			addr.ToLong(&addri,16);
			
			if(size==0) size=1;

			mdSpy *spy=new mdSpy();
			spy->SetName(m_TxtName->GetValue());
			spy->SetAddress(addri,false);
			spy->m_Length=size;
			spy->m_ArraySize=arraysize;

			spy->m_DisplayHexa = m_ChkHexa->GetValue();

			if(m_UpdateMode==true)
				spy->m_ListIndex=m_id;
			
			if(m_SpiesPanel!=NULL)
			{	
				if(m_UpdateMode==true)
				{	m_SpiesPanel->ProcessItem(m_UpdateMode,spy);					
				}
				else
				{
					if(m_InsertIndex!=-1)
						m_SpiesPanel->ProcessItem(m_InsertIndex,m_UpdateMode,spy);
					else
						m_SpiesPanel->ProcessItem(m_UpdateMode,spy);
				}			
			}


			delete(spy);

			EndModal(wxOK);
		}		
	}
	else
	{	event.Skip();
	}	
}
