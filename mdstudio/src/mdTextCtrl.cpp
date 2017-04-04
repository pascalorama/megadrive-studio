///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
// app includes
#include "include/mdShared.h"
#include "include/mdTextCtrl.h"

BEGIN_EVENT_TABLE(mdTextCtrl, wxTextCtrl)    
	EVT_CONTEXT_MENU(mdTextCtrl::OnContextMenu)
	EVT_KEY_DOWN(mdTextCtrl::OnKeydown) 
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// Constructor

mdTextCtrl::mdTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size, long style,const wxValidator& validator, const wxString& name)
						:wxTextCtrl(parent,id,value,pos,size,style,validator,name)
{

}



void mdTextCtrl::OnContextMenu(wxContextMenuEvent& event)
{

}



void mdTextCtrl::OnKeydown(wxKeyEvent& event)
{
	if(event.m_controlDown==TRUE && event.m_keyCode==86 )
	{}
	else
		event.Skip();
}