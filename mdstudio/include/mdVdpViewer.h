#ifndef __mdVdpViewer_H__
#define __mdVdpViewer_H__

///////////////////////////////////////////////////////////////////////////////
//
#include "mdVdpRegInfo.h"
#include "mdTextCtrl.h"

class mdVdpViewer : public wxPanel
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////
    mdVdpViewer(wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = "panel");
    virtual ~mdVdpViewer();

    ///////////////////////////////////////////////////////////////////////////
	private:
	////////
	wxComboBox	*m_CmbIO;
	wxCheckListBox *m_CheckList;
	wxStaticText *m_Value;
	wxStaticText *m_Comment;
	mdTextCtrl *m_NewValue;
	
    DECLARE_EVENT_TABLE()

	void OnCombo( wxCommandEvent &event );
	void OnCheck( wxCommandEvent &event );
	void OnUpdate( wxCommandEvent &event );

	void DisplayDetails(void);
	void UpdateValue();
	void SetValue(int idx,unsigned char value);
	void DisplayRegister(int idx);

	// regs
	VDPRegInfo* m_CurrentReg;
	VDPReg0 m_R0;
	VDPReg1 m_R1;
	VDPReg2 m_R2;
	VDPReg3 m_R3;
	VDPReg4 m_R4;
	VDPReg5 m_R5;
	VDPReg7 m_R7;
	VDPReg10 m_R10;
	VDPReg11 m_R11;
	VDPReg12 m_R12;
	VDPReg13 m_R13;
	VDPReg15 m_R15;
	VDPReg16 m_R16;
	VDPReg17 m_R17;
	VDPReg18 m_R18;
	VDPReg19 m_R19;
	VDPReg20 m_R20;
	VDPReg21 m_R21;
	VDPReg22 m_R22;
	VDPReg23 m_R23;
};

#endif
///////////////////////////////////////////////////////////////////////////////
