///////////////////////////////////////////////////////////////////////////////
//
//

#ifndef __MDFRMMAIN_H__
#define __MDFRMMAIN_H__

class mdEmu;
class wxLayoutManager;
class wxDockWindow;
class mdVideoPanel;
class mdDisAsmPanel;
class mdCRAMPanel;
class mdRegPanel;
class mdSpiesPanel;
class mdFrmVdpCalc;
class mdFrmVdpAddrCalc;
class mdFrmGoto;
class mdVDPStatus;
class mdLogPanel;
class DXKeyboard;
class mdVideoAdapter;
class mdVideoSurface;
class mdTileViewerPanel;

///////////////////////////////////////////////////////////////////////////////
//
class mdFrmMain : public wxFrame
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////  
    mdFrmMain(	wxWindow *parent, const wxWindowID id, const wxString& title,
				const wxPoint& pos, const wxSize& size, const long style);
	~mdFrmMain();

	void StartSbProgress(const wxString &text,long maxvalue);
	void UpdateSbProgress(long value);
	void EndSbProgress(void);
	void SetStatusText(const wxString &text){	m_StatusBar->SetStatusText(text);}

	void Init(void);

	void EmulateFrame(void);

	void FlushLog();
	void LogMessage(int level,char* text);
	void LogChar(int level,char text);

	void OnLoadRom(wxChar* filename);
	void OnLoadRom(wxChar* filename,bool clearSpies);

	void LoadLayout();
	void SaveLayout();

	void GotoAddress(int address);

	void UpdateDisAsm(void);

	mdDisAsmPanel*	GetDisAsmPanel(void){	return m_DisAsmPanel;}
	mdSpiesPanel*	GetSpiesPanel(void){	return m_SpiesPanel;}

	mdVideoAdapter	*m_VideoAdapter;

	mdEmu*	GetEmu(void){	return m_Emu;}

	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////
	

	wxMenuBar	*m_MenuBar;
	wxMenu		*m_MenuFile;
	wxMenu		*m_MenuDebug;
	wxMenu		*m_MenuView;
	wxMenu		*m_MenuDump;
	wxMenu		*m_MenuTools;
	wxMenu		*m_MenuEdit;
	wxMenu		*m_MenuSymbols;
	wxMenu		*m_MenuInputs;

	wxStatusBar		*m_StatusBar;
	wxStatusBar		*m_CustomStatusBar;
	wxPanel			*m_PanelMain;

	mdFrmVdpCalc	*m_FrmVdpCalc;
	mdFrmVdpAddrCalc	*m_FrmVdpAddrCalc;
	mdFrmGoto		*m_FrmGoto;

	wxLayoutManager *m_LayoutManager;
	wxDockWindow	*m_VideoDock;
//	wxDockWindow	*m_DisAsmDock;
	wxDockWindow	*m_RegDock;
	wxDockWindow	*m_SpiesDock;
	wxDockWindow	*m_LogDock;
	wxDockWindow	*m_VDPStatusDock;
	wxDockWindow	*m_CRAMDock;
	wxDockWindow	*m_TileViewerDock;

	mdVideoPanel	*m_VideoPanel;
	mdDisAsmPanel	*m_DisAsmPanel;
	mdRegPanel		*m_RegPanel;
	mdSpiesPanel	*m_SpiesPanel;
	mdLogPanel		*m_LogPanel;
	mdVDPStatus		*m_VDPStatus;
	mdCRAMPanel		*m_CRAMPanel;
	mdTileViewerPanel	*m_TileViewerPanel;

	bool			m_EmuIsRunning;
	DWORD			last_tick;

	DXKeyboard *m_DInput;
	mdEmu	*m_Emu;

	void InitMenu(void);
	void InitLayout(void);

	void InitScreen(void);
	
	void OnQuit(wxCommandEvent& event);
	void OnLoadRom(wxCommandEvent& event);
	

	void OnReloadRom(wxCommandEvent& event);
	void OnRun(wxCommandEvent& event);
	void OnIdle(wxIdleEvent& event);

	void OnStepFrame(wxCommandEvent& event);
	void OnPause(wxCommandEvent& event);
	void OnStepInto(wxCommandEvent& event);

	void OnSaveScreenshot(wxCommandEvent& event);

	void OnDumpROM(wxCommandEvent& event);
	void OnDumpRAM(wxCommandEvent& event);
	void OnDumpCRAM(wxCommandEvent& event);
	void OnDumpVRAM(wxCommandEvent& event);
	void OnDumpZRAM(wxCommandEvent& event);
	void OnDumpVSRAM(wxCommandEvent& event);

	void OnVdpCalc(wxCommandEvent& event);
	void OnVdpAddrCalc(wxCommandEvent& event);

	void OnViewVideoRendering(wxCommandEvent& event);
	void OnViewLog(wxCommandEvent& event);
	void OnView68kRegisters(wxCommandEvent& event);
	void OnView68kWatchers(wxCommandEvent& event);
	void OnViewFullScreen(wxCommandEvent& event);
	void OnViewCRAM(wxCommandEvent& event);
	void OnViewVDPStatus(wxCommandEvent& event);

	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);

	void OnGotoAddress(wxCommandEvent& event);
		
	///////////////////////////////////////////////////////////////////////////
	private:
	////////
	
	DECLARE_EVENT_TABLE()
};

///////////////////////////////////////////////////////////////////////////////
#endif