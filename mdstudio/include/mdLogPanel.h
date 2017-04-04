#ifndef __mdLogPanel_H__
#define __mdLogPanel_H__

///////////////////////////////////////////////////////////////////////////////
// Class definition
///////////////////////////////////////////////////////////////////////////////

class mdLogPanel: public wxPanel
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////
	
	mdLogPanel(	wxWindow* parent,
				wxWindowID id,									
				const wxPoint& pos,
				const wxSize& size,
				long style,
				const wxString& name);
	~mdLogPanel();

	void Flush(void);
	void LogMessage(int level,char* text);
	void LogChar(int level,char text);


	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////	

	wxMenu *m_Popup;
	wxPanel	*m_LogDisplay;
	
	int m_Cnt;
	int m_ScrollPosition;
	int m_LastIndex;

	wxString	m_Text[512];
	int			m_Level[512];
	int			m_LevelShow[10];

	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event);

	void OnContextMenu(wxContextMenuEvent& event);

	void OnClearLog(wxCommandEvent& event);
	void OnSaveLog(wxCommandEvent& event);
	void OnDMA(wxCommandEvent& event);
	void OnIO(wxCommandEvent& event);

	void OnThumbTrack(wxScrollWinEvent &event);
	void OnSize(wxSizeEvent &event);

	DECLARE_EVENT_TABLE()	
};

///////////////////////////////////////////////////////////////////////////////
#endif