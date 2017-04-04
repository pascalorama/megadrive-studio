#ifndef __HEXATEXTBOX_H__
#define __HEXATEXTBOX_H__

///////////////////////////////////////////////////////////////////////////////
class mdTextCtrl:public wxTextCtrl
{
	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////

	///////////////////////////////////////////////////////////////////////////
	public:
	///////
	mdTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value = "", const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxTextCtrlNameStr);

	virtual ~mdTextCtrl(){}

	void OnKeydown(wxKeyEvent& event);
	void OnContextMenu(wxContextMenuEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif