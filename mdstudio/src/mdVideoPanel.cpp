///////////////////////////////////////////////////////////////////////////////
//
//
//
// NOT PORTABLE I'M USING DIB
///////////////////////////////////////////////////////////////////////////////
// Includes 
#include "include/mdShared.h"
#include "include/mdScreen.h"
#include "include/mdVideoPanel.h"

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(mdVideoPanel, wxPanel)

END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

mdVideoPanel::mdVideoPanel(	wxWindow* parent,
					wxWindowID id,									
					const wxPoint& pos,
					const wxSize& size,
					long style,
					const wxString& name)
					:wxPanel(parent,id,pos,size,style,name)
{
	m_Screen=new mdScreen(	this,
							-1,
							wxDefaultPosition,
							wxSize(320,240),
							wxTAB_TRAVERSAL,
							"Center");	

}

///////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////

mdVideoPanel::~mdVideoPanel()
{
}

void mdVideoPanel::SetBitmap(mdDibBitmap *bmp)
{
	m_Screen->SetBitmap(bmp);
}

///////////////////////////////////////////////////////////////////////////////