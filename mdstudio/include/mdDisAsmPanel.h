#ifndef __mdDisAsmPanel_H__
#define __mdDisAsmPanel_H__

///////////////////////////////////////////////////////////////////////////////
// Class definition
///////////////////////////////////////////////////////////////////////////////

class mdEmu;

class mdDisAsmPanel: public wxWindow
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////
	
	mdDisAsmPanel(	wxWindow* parent,
					wxWindowID id,									
					const wxPoint& pos,
					const wxSize& size,
					long style,
					const wxString& name);
	~mdDisAsmPanel();

	void SetEmu(mdEmu* m);

	void DisAsm();
	void OnSize(wxSizeEvent &event);

	unsigned int m_PageTopPC;
	unsigned int m_PageBottomPC;

	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////
	
	int		m_RowPosition;
	mdEmu	*m_Emu;
	
	wxString m_DisAsmText[256];
	unsigned int m_DisAsmPC[256];
	bool m_IsBreakPoint[256];
	bool m_IsLabel[256];

	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnLeftDoubleClick(wxMouseEvent &event);

	
	DECLARE_EVENT_TABLE()	
};

///////////////////////////////////////////////////////////////////////////////


#endif