#ifndef __mdSpiesPanel_H__
#define __mdSpiesPanel_H__

///////////////////////////////////////////////////////////////////////////////
// Class definition
///////////////////////////////////////////////////////////////////////////////
class mdSpy;
class mdEmu;
class mdSection;


class mdSpiesPanel: public wxPanel
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////
	
	mdSpiesPanel(	wxWindow* parent,
					wxWindowID id,									
					const wxPoint& pos,
					const wxSize& size,
					long style,
					const wxString& name);
	~mdSpiesPanel();

	mdSpy* InsertSpy(int list_idx,mdSection *Section_ID,wxString &name,unsigned int address,int length,int arraycnt=1,bool isarrayelement=false);
	mdSpy* InsertSpy(mdSection *Section_ID,wxString &name,unsigned int address,int length,int arraycnt=1,bool isarrayelement=false);
	long GetSelectedIndex();

	void ProcessItem(bool Update,mdSpy* spy);
	void ProcessItem(int list_idx,bool Update,mdSpy* spy);

	void UpdateSpies(bool force=FALSE);

	void SetEmu(mdEmu* emu);

	void Clear(void);

	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////	
	
	
	wxMenu *m_Popup;
	wxListCtrl *m_SpiesList;
	mdEmu* m_Emu;
	void SelectSpy(wxListEvent& event);

	void OnContextMenu(wxContextMenuEvent& event);

	void OnAddSpy(wxCommandEvent& event);
	void OnUpdateSpy(wxCommandEvent& event);
	void OnDeleteSpy(wxCommandEvent& event);
	void OnInsertSpy(wxCommandEvent& event);

	void OnDisplayHexa(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()	
};

///////////////////////////////////////////////////////////////////////////////
#endif