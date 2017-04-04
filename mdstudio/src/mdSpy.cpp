///////////////////////////////////////////////////////////////////////////////
//
//
//
// 
///////////////////////////////////////////////////////////////////////////////
// Includes 
#include "include/mdShared.h"
#include "include/mdSymbol.h"
#include "include/mdFrmMain.h"
#include "include/mdApp.h"

#include "include/mdSpy.h"


mdSpy::mdSpy()
{
	m_Address=0;
	m_Value=0;
	m_Length=1;
	m_Name=_T("");
	
	m_ListIndex=-1;
	m_ArraySize=1;
	m_IsFromArray=false;
	m_DisplayHexa=false;
}

mdSpy::~mdSpy()
{
}

void mdSpy::SetAddress(int adr,bool UpdateSymbols)
{
	m_Address=adr;

	// remplace le symbol si existe ou cree
	if(UpdateSymbols==true)
		m_Symbol=gProject->InsertSymbol(NULL,m_Name,adr,0,true);

}

void mdSpy::SetName(wxString &name)
{
	m_Name=name;
}

wxString& mdSpy::GetName(void)
{	return m_Name;
}

int mdSpy::GetAddress(void)
{	return m_Address;
}