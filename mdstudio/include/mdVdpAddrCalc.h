#ifndef __mdVdpAddrCalc_H__
#define __mdVdpAddrCalc_H__

///////////////////////////////////////////////////////////////////////////////
//
#include "mdTextCtrl.h"

class mdVdpAddrCalc : public wxPanel
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////
    mdVdpAddrCalc(wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = "panel");
    virtual ~mdVdpAddrCalc();

    ///////////////////////////////////////////////////////////////////////////
	private:
	////////
	wxComboBox	*m_CmbCode;

	wxStaticText *m_Comment;
	mdTextCtrl *m_Address;
	mdTextCtrl *m_Value;
	bool m_UpdateOther;

	/*wxCheckListBox *m_CheckList;
	wxStaticText *m_Value;
	wxStaticText *m_Comment;
	mdTextCtrl *m_NewValue;
	*/
    DECLARE_EVENT_TABLE()

	void OnCombo( wxCommandEvent &event );

	void OnAddressChange( wxCommandEvent &event );
	void OnValueChange( wxCommandEvent &event );

	void UpdateValue();
	void UpdateAddress();

	
};

#endif
///////////////////////////////////////////////////////////////////////////////
