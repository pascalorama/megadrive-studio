#ifndef __mdRegPanel_H__
#define __mdRegPanel_H__

class mdRegistersDisplay;
class mdEmu;

///////////////////////////////////////////////////////////////////////////////
// Class definition
///////////////////////////////////////////////////////////////////////////////

class mdRegPanel: public wxPanel
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////
	
	mdRegPanel(	wxWindow* parent,
				wxWindowID id,									
				const wxPoint& pos,
				const wxSize& size,
				long style,
				const wxString& name);
	~mdRegPanel();

	void SetCpuType(int id);
	void SetEmu(mdEmu *emu);
	void UpdadeRegisters(void);

	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////	

	mdRegistersDisplay *m_RegsDisplay;
	mdEmu *m_Emu;

	wxCheckBox *m_ChkC;
	wxCheckBox *m_ChkV;
	wxCheckBox *m_ChkZ;
	wxCheckBox *m_ChkN;
	wxCheckBox *m_ChkX;

	DECLARE_EVENT_TABLE()	
};

///////////////////////////////////////////////////////////////////////////////
#endif