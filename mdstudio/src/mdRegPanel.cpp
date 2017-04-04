///////////////////////////////////////////////////////////////////////////////
//
//
//
// 
///////////////////////////////////////////////////////////////////////////////
// Includes 
#include "include/mdShared.h"
#include "include/mdEmu.h"
#include "include/mdRegistersDisplay.h"
#include "include/mdRegPanel.h"

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(mdRegPanel, wxPanel)

END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

mdRegPanel::mdRegPanel(	wxWindow* parent,
						wxWindowID id,									
						const wxPoint& pos,
						const wxSize& size,
						long style,
						const wxString& name)
						:wxPanel(parent,id,pos,size,style,name)
{
	// crèe le panel
	m_RegsDisplay= new mdRegistersDisplay (	this,
											-1,
											wxDefaultPosition,
											wxSize(240,180),
											wxTAB_TRAVERSAL | wxSUNKEN_BORDER,
											"Registers");

	m_ChkC=new wxCheckBox(	this,
							-1,
							"C",
							wxPoint(250,0),
							wxDefaultSize);

	m_ChkV=new wxCheckBox(	this,
							-1,
							"V",
							wxPoint(250,15),
							wxDefaultSize);

	m_ChkZ=new wxCheckBox(	this,
							-1,
							"Z",
							wxPoint(250,30),
							wxDefaultSize);

	m_ChkN=new wxCheckBox(	this,
							-1,
							"N",
							wxPoint(250,45),
							wxDefaultSize);

	m_ChkX=new wxCheckBox(	this,
							-1,
							"X",
							wxPoint(250,60),
							wxDefaultSize);

	m_ChkC->Disable();
	m_ChkV->Disable();
	m_ChkZ->Disable();
	m_ChkN->Disable();
	m_ChkX->Disable();
	
}

///////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////

mdRegPanel::~mdRegPanel()
{
}

void mdRegPanel::SetCpuType(int id)
{
	if(m_RegsDisplay!=NULL)
	{
		m_RegsDisplay->SetCpuType(id);
	}
}

void mdRegPanel::SetEmu(mdEmu* emu)
{
	m_Emu=emu;
	m_RegsDisplay->SetEmu(emu);
}

void mdRegPanel::UpdadeRegisters(void)
{
	unsigned int i=0;
	m_RegsDisplay->UpdateRegisters();
	i=m_RegsDisplay->GetSR();

	// carry
	if(i & 0x1)
	{	m_ChkC->SetValue(true);	}
	else
	{	m_ChkC->SetValue(false);	}

	// overflow
	if(i & 0x2)
	{	m_ChkV->SetValue(true);	}
	else
	{	m_ChkV->SetValue(false);	}

	// Zero
	if(i & 0x4)
	{	m_ChkZ->SetValue(true);	}
	else
	{	m_ChkZ->SetValue(false);	}

	// Negative
	if(i & 0x8)
	{	m_ChkN->SetValue(true);	}
	else
	{	m_ChkN->SetValue(false);	}

	// Negative
	if(i & 0x10)
	{	m_ChkX->SetValue(true);	}
	else
	{	m_ChkX->SetValue(false);	}


	this->Refresh();
}
///////////////////////////////////////////////////////////////////////////////