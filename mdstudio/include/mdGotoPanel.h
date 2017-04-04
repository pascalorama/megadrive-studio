#ifndef __mdGotoPanel_H__
#define __mdGotoPanel_H__

///////////////////////////////////////////////////////////////////////////////
//
#include "mdTextCtrl.h"

class mdGotoPanel : public wxPanel
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////
    mdGotoPanel(wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = "panel");
    virtual ~mdGotoPanel();

	wxString GetAddress(){	return m_Address->GetValue();}

    ///////////////////////////////////////////////////////////////////////////
	private:
	////////
	mdTextCtrl *m_Address;

	void OnGo( wxCommandEvent &event);

	DECLARE_EVENT_TABLE()
	
};

#endif
///////////////////////////////////////////////////////////////////////////////
