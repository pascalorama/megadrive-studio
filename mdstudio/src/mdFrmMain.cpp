///////////////////////////////////////////////////////////////////////////////
//
//

///////////////////////////////////////////////////////////////////////////////
// app includes
#include "include/mdShared.h"
#include "include/mdProgressStatusBar.h"
#include "include/mdDibBitmap.h"
#include "include/mdVideoPanel.h"
#include "include/mdSpiesPanel.h"
#include "include/mdDisAsmPanel.h"
#include "include/mdLogPanel.h"
#include "include/mdRegPanel.h"
#include "include/mdEmu.h"
#include "include/mdFrmVdpCalc.h"
#include "include/mdFrmGoto.h"
#include "include/mdFrmVdpAddrCalc.h"
#include "include/DXKeyboard.h"
#include "include/mdVideoAdapter.h";
#include "include/mdVideoSurface.h"
#include "include/mdVDPStatusPanel.h"
#include "include/mdCRAMPanel.h"
#include "include/mdTileViewerPanel.h"
#include "include/mdFrmDumpBin.h"
#include "include/mdFrmMain.h"
#include "include/mdApp.h"


///////////////////////////////////////////////////////////////////////////////
// ID menu

enum
{   ID_FILE_QUIT = 1,    
	ID_LOAD_ROM,
	ID_RELOAD_ROM,

	ID_SAVE_STATE,
	ID_LOAD_STATE,


	ID_RUN,
	ID_PAUSE,
	ID_STEP_INTO,
	ID_RUN_TO_CURSOR,
	ID_FULLSCREEN,
	ID_STEPFRAME,
	ID_DUMP_ROM,
	ID_DUMP_PICTURE,
	ID_DUMP_CRAM,
	ID_DUMP_RAM,
	ID_DUMP_VRAM,
	ID_DUMP_VSRAM,
	ID_DUMP_Z80RAM,

	ID_VIEW_RENDERING,
	ID_VIEW_LOG,
	ID_VIEW_68KREGISTERS,
	ID_VIEW_WATCHERS,
	ID_VIEW_CRAM,
	ID_VIEW_VDPSTATUS,

	ID_GOTO_ADDRESS,

	ID_VDP_CALC,
	ID_VDPADDR_CALC,
};

///////////////////////////////////////////////////////////////////////////////
// Event Table

BEGIN_EVENT_TABLE(mdFrmMain, wxFrame)	
	EVT_MENU(ID_FILE_QUIT, mdFrmMain::OnQuit)
	EVT_MENU(ID_LOAD_ROM, mdFrmMain::OnLoadRom)
	EVT_MENU(ID_RELOAD_ROM, mdFrmMain::OnReloadRom)




	EVT_MENU(ID_RUN,mdFrmMain::OnRun)
	EVT_MENU(ID_PAUSE,mdFrmMain::OnPause)
	EVT_MENU(ID_STEPFRAME,mdFrmMain::OnStepFrame)
	EVT_MENU(ID_STEP_INTO,mdFrmMain::OnStepInto)

	EVT_MENU(ID_DUMP_PICTURE,mdFrmMain::OnSaveScreenshot)
	EVT_MENU(ID_DUMP_ROM,mdFrmMain::OnDumpROM)
	EVT_MENU(ID_DUMP_RAM,mdFrmMain::OnDumpRAM)
	EVT_MENU(ID_DUMP_CRAM,mdFrmMain::OnDumpCRAM)
	EVT_MENU(ID_DUMP_VRAM,mdFrmMain::OnDumpVRAM)
	EVT_MENU(ID_DUMP_Z80RAM,mdFrmMain::OnDumpZRAM)
	EVT_MENU(ID_DUMP_VSRAM,mdFrmMain::OnDumpVSRAM)

	EVT_MENU(ID_VDP_CALC,mdFrmMain::OnVdpCalc)
	EVT_MENU(ID_VDPADDR_CALC,mdFrmMain::OnVdpAddrCalc)

	//EVT_UPDATE_UI(Menu_SubMenu_Normal,    MyFrame::OnUpdateSubMenuNormal)
	EVT_MENU(ID_VIEW_RENDERING,mdFrmMain::OnViewVideoRendering)
	EVT_MENU(ID_VIEW_LOG,mdFrmMain::OnViewLog)
	EVT_MENU(ID_VIEW_68KREGISTERS,mdFrmMain::OnView68kRegisters)
	EVT_MENU(ID_VIEW_WATCHERS,mdFrmMain::OnView68kWatchers)
	EVT_MENU(ID_VIEW_CRAM,mdFrmMain::OnViewCRAM)
	EVT_MENU(ID_VIEW_VDPSTATUS,mdFrmMain::OnViewVDPStatus)

	EVT_MENU(ID_FULLSCREEN,mdFrmMain::OnViewFullScreen)
	EVT_MENU(ID_GOTO_ADDRESS,mdFrmMain::OnGotoAddress)


	//EVT_KEY_DOWN(mdFrmMain::OnKeyDown)
	//EVT_KEY_UP(mdFrmMain::OnKeyUp)
	
	EVT_IDLE(mdFrmMain::OnIdle)

END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// Constructor

mdFrmMain::mdFrmMain(	wxWindow *parent, 
						const wxWindowID id,
						const wxString& title,
						const wxPoint& pos,
						const wxSize& size,
						const long style)
					   :wxFrame(parent, id, title, pos,size,style)
{
	m_VideoAdapter=new mdVideoAdapter((HWND) GetHandle(),false);

//	InitMenu();

	m_StatusBar = this->CreateStatusBar();
	m_StatusBar->SetStatusText(_T(" Welcome to Megadrive Debug Studio"));
	m_EmuIsRunning=false;

	m_FrmVdpCalc=NULL;
	m_FrmVdpAddrCalc=NULL;
	m_FrmGoto=NULL;
    		
	this->SetSizeHints(800,600);

	m_DInput=new DXKeyboard((HWND)GetHandle(),(HINSTANCE)wxGetInstance());

	this->Maximize();
}

void mdFrmMain::Init(void)
{
	m_Emu=new mdEmu(_T("plugins\\genesisplus.dll"));

	InitMenu();

	InitLayout();
}

///////////////////////////////////////////////////////////////////////////////
// Destructor

mdFrmMain::~mdFrmMain()
{	
	SaveLayout();

	if(m_Emu!=NULL)
	{	delete(m_Emu);
		m_Emu=NULL;
	}

	delete(m_DInput);
	m_DInput=NULL;

	delete(m_VideoAdapter);
	m_VideoAdapter=NULL;

	if(m_VDPStatusDock)
		delete(m_VDPStatusDock);

	if(m_FrmGoto)
		m_FrmGoto->Destroy();

	if(m_FrmVdpCalc)
		m_FrmVdpCalc->Destroy();

	if(m_VideoDock)
		delete(m_VideoDock);
	
	if(m_RegDock)
		delete(m_RegDock);

	if(m_CRAMDock)
		delete(m_CRAMDock);

	if(m_SpiesDock)
		delete(m_SpiesDock);

	if(m_LogDock)
		delete(m_LogDock);

	if(m_TileViewerDock)
		delete(m_TileViewerDock);

	this->Destroy();
}

///////////////////////////////////////////////////////////////////////////////
// Initialize the menu

void mdFrmMain::InitMenu(void)
{	
	m_MenuBar = new wxMenuBar();

	// file
	m_MenuFile = new wxMenu();

	m_MenuFile->Append(	ID_LOAD_ROM,
						_T("Load Rom...\tCtrl+O"),
						_T(" Load Rom"));

	m_MenuFile->Append(	ID_RELOAD_ROM,
						_T("Reload Rom\tCtrl+R"),
						_T(" Reload Rom"));

	m_MenuFile->AppendSeparator();

	m_MenuFile->Append(	ID_FILE_QUIT,
						_T("Quit\tCtrl+Q"),
						_T(" Quit"));

	m_MenuBar->Append(m_MenuFile,"&File");

	// Edit

	m_MenuEdit= new wxMenu();
	
	m_MenuEdit->Append(ID_GOTO_ADDRESS,
						_T("Goto address...\tG"),
						_T(" Goto Address"));

	m_MenuBar->Append(m_MenuEdit,"&Edit");

	// View
	m_MenuView= new wxMenu();
	
	m_MenuView->Append(ID_FULLSCREEN,
						_T("Full Screen"),
						_T(" Full Screen"));

	m_MenuView->AppendSeparator();

	m_MenuView->Append(ID_VIEW_RENDERING,
						_T("Video rendering\tCtrl+1"),
						_T(" Video rendering"));

	m_MenuView->Append(ID_VIEW_68KREGISTERS,
						_T("68K Registers\tCtrl+2"),
						_T(" 68K Registers"));

	m_MenuView->Append(ID_VIEW_WATCHERS,
						_T("Watchers\tCtrl+3"),
						_T("Watchers"));

	m_MenuView->Append(ID_VIEW_CRAM,
						_T("CRAM viewer\tCtrl+4"),
						_T("CRAM viewer"));

	m_MenuView->Append(ID_VIEW_VDPSTATUS,
						_T("VDP Status\tCtrl+5"),
						_T("VDP Status"));

	m_MenuView->Append(ID_VIEW_LOG,
						_T("Log window\tCtrl+6"),
						_T("Log window"));

	m_MenuBar->Append(m_MenuView,"&View");

	// debug
	m_MenuDebug = new wxMenu();

	m_MenuDebug->Append(ID_RUN,
						_T("Run\tF5"),
						_T(" Run"));

	m_MenuDebug->Append(ID_PAUSE,
						_T("Pause\tCtrl+P"),
						_T(" Pause Execution"));

	m_MenuDebug->Append(ID_STEP_INTO,
						_T("Step Into\tF11"),
						_T(" Step Into"));
	
	//m_MenuDebug->Append(ID_RUN_TO_CURSOR,
	//					_T("Run to Cursor\tCtrl+F10"),
	//					_T(" Run to Cursor"));
	
	m_MenuDebug->AppendSeparator();

	m_MenuDebug->Append(ID_STEPFRAME,
						_T("Step Frame\tCtrl+N"),
						_T(" Step Frame"));
	
	m_MenuBar->Append(m_MenuDebug,"&Debug");
	
	// dump
//	m_MenuSymbols = new wxMenu();
//	m_MenuBar->Append(m_MenuSymbols,"&Symbols");

	// dump
	m_MenuDump = new wxMenu();

	m_MenuDump->Append(ID_DUMP_ROM,
						_T("Dump ROM..."),
						_T(" Dump Rom"));

	m_MenuDump->Append(ID_DUMP_RAM,
						_T("Dump 68k RAM..."),
						_T(" Dump 68k Ram"));

	m_MenuDump->Append(ID_DUMP_Z80RAM,
						_T("Dump Z80 RAM..."),
						_T(" Dump Z80 Ram"));

	m_MenuDump->Append(ID_DUMP_VRAM,
						_T("Dump VRAM..."),
						_T(" Dump VRam"));

	m_MenuDump->Append(ID_DUMP_CRAM,
						_T("Dump CRAM..."),
						_T(" Dump CRam"));

	m_MenuDump->Append(ID_DUMP_VSRAM,
						_T("Dump VSRAM..."),
						_T(" Dump VSRam"));

//	m_MenuDump->AppendSeparator();

//	m_MenuDump->Append(ID_DUMP_PICTURE,
//						_T("Save Screenshot...\tF12"),
//						_T(" Save Screenshot"));

	m_MenuBar->Append(m_MenuDump,"D&ump");

	// Tools
	m_MenuTools = new wxMenu();

	m_MenuTools->Append(ID_VDP_CALC,
						_T("VDP Registers Calculator\tC"),
						_T(" VDP Registers Calculator"));

	m_MenuTools->Append(ID_VDPADDR_CALC,
						_T("VDP Address Calculator\tV"),
						_T(" VDP Address Calculator"));

	m_MenuBar->Append(m_MenuTools,"&Tools");


	SetMenuBar (m_MenuBar);
}

///////////////////////////////////////////////////////////////////////////////
// InitLayout
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::InitLayout(void)
{

	// Panel 
	m_DisAsmPanel = new mdDisAsmPanel(	this,
									-1,
									wxDefaultPosition,
									wxSize(500,500),
									wxFULL_REPAINT_ON_RESIZE  ,
									"PanelDisAsm");

	// Screen
	m_VideoDock = new wxDockWindow(	this,
									0,
									_T("Video Rendering"),
									wxPoint(64,64),
									wxSize(320,240),
									"d1");

	m_VideoPanel = new mdVideoPanel(	this,
										-1,
										wxDefaultPosition,
										wxSize(200,200),
										wxTAB_TRAVERSAL,
										"PanelVideo");
	m_VideoDock->SetClient(m_VideoPanel);
		
	// registres
	m_RegDock = new wxDockWindow(	this,
									0,
									_T("Cpu Registers"),
									wxPoint(400,64),
									wxSize(320,240),
									"d2");

	m_RegPanel = new mdRegPanel(	this,
										-1,
										wxDefaultPosition,
										wxSize(200,200),
										wxTAB_TRAVERSAL,
										"PanelReg");
	m_RegDock->SetClient(m_RegPanel);

	// spies
	m_SpiesDock = new wxDockWindow(	this,
									0,
									_T("Watchers"),
									wxPoint(200,64),
									wxSize(320,240),
									"d3");

	m_SpiesPanel = new mdSpiesPanel(	this,
										-1,
										wxDefaultPosition,
										wxSize(200,200),
										wxTAB_TRAVERSAL,
										"PanelSpies");
	m_SpiesDock->SetClient(m_SpiesPanel);

	// log
	m_LogDock = new wxDockWindow(	this,
									0,
									_T("Logs"),
									wxPoint(200,64),
									wxSize(320,240),
									"d4");

	m_LogPanel = new mdLogPanel(	this,
										-1,
										wxDefaultPosition,
										wxSize(200,200),
										wxTAB_TRAVERSAL,
										"PanelLogs");
	m_LogDock->SetClient(m_LogPanel);

	// VDP Status
	m_VDPStatusDock = new wxDockWindow(	this,
										0,
										_T("VDP Status"),
										wxPoint(200,64),
										wxSize(320,240),
										"d5");

	m_VDPStatus = new mdVDPStatus(	this,
										-1,
										wxDefaultPosition,
										wxSize(200,200),
										wxTAB_TRAVERSAL,
										"PanelVDPStat");
	m_VDPStatusDock->SetClient(m_VDPStatus);

	// CRAM
	m_CRAMDock = new wxDockWindow(	this,
									0,
									_T("CRAM Viewer"),
									wxPoint(200,64),
									wxSize(300-16,96),
									"d6");

	m_CRAMPanel = new mdCRAMPanel(	this,
										-1,
										wxDefaultPosition,
										wxSize(200,250),
										wxTAB_TRAVERSAL,
										"PanelCRAM");
	m_CRAMDock->SetClient(m_CRAMPanel);

	// Tile Viewer
	m_TileViewerDock=new wxDockWindow(	this,
										-1,
										_T("VRAM Viewer"),
										wxDefaultPosition,
										wxSize(400,400),
										"d7");

	m_TileViewerPanel=new mdTileViewerPanel(this,
											-1,
											wxDefaultPosition,
											wxDefaultSize,
											wxTAB_TRAVERSAL);
	m_TileViewerDock->SetClient(m_TileViewerPanel);

	// crèe le layount manager
	m_LayoutManager = new wxLayoutManager( this );
    m_LayoutManager->AddDefaultHosts();
	m_LayoutManager->AddDockWindow(m_VideoDock);
	m_LayoutManager->AddDockWindow(m_RegDock);
	m_LayoutManager->AddDockWindow(m_SpiesDock);
	m_LayoutManager->AddDockWindow(m_LogDock);
	m_LayoutManager->AddDockWindow(m_VDPStatusDock);
	m_LayoutManager->AddDockWindow(m_CRAMDock);
	m_LayoutManager->AddDockWindow(m_TileViewerDock);

	// init des bars

	wxHostInfo hi;
    hi = m_LayoutManager->GetDockHost(wxDEFAULT_LEFT_HOST);
    m_LayoutManager->DockWindow(m_VideoDock,hi);
	hi.GetHost()->SetAreaSize(320);

	hi = m_LayoutManager->GetDockHost(wxDEFAULT_RIGHT_HOST);
	m_LayoutManager->DockWindow(m_RegDock,hi);

    hi.GetHost()->SetAreaSize(320);
	
    m_LayoutManager->SetLayout(wxDWF_SPLITTER_BORDERS,m_DisAsmPanel);
	LoadLayout();

	m_VideoDock->Show();
	m_RegDock->Show();
	m_SpiesDock->Show();
	m_LogDock->Show();
	m_VDPStatusDock->Show();
	m_CRAMDock->Show();
	//m_TileViewerDock->Show();

	// init de l'emu dans disasm
	m_DisAsmPanel->SetEmu(m_Emu);
	m_RegPanel->SetEmu(m_Emu);
	m_SpiesPanel->SetEmu(m_Emu);
	m_CRAMPanel->SetCRAM(m_Emu->GetCRAM());
	m_RegPanel->SetCpuType(mdM68K_CPU);
    
	m_RegPanel->UpdadeRegisters();
	m_SpiesPanel->UpdateSpies();
	m_CRAMPanel->UpdateCRAM();
}

///////////////////////////////////////////////////////////////////////////////
// Init Screen
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::InitScreen(void)
{

}

///////////////////////////////////////////////////////////////////////////////
// Start to show the progress bar un status
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::StartSbProgress(const wxString &text,long maxvalue)
{
	wxSetCursor(*wxHOURGLASS_CURSOR);
		
	this->GetStatusBar()->Hide();
	this->SetStatusBar(0);
		
	m_CustomStatusBar = new mdProgressStatusBar(this,text,maxvalue);
	this->SetStatusBar(m_CustomStatusBar);
	((mdProgressStatusBar*)m_CustomStatusBar)->SendSizeEvent();
}

///////////////////////////////////////////////////////////////////////////////
// Update status progress bar
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::UpdateSbProgress(long value)
{
	((mdProgressStatusBar*)m_CustomStatusBar)->SetValue(value);			
}

///////////////////////////////////////////////////////////////////////////////
// End the progress bar display
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::EndSbProgress(void)
{
	wxSetCursor(wxNullCursor);
	
	m_StatusBar->Show();
	this->GetStatusBar()->Hide();	
	this->SetStatusBar(m_StatusBar);	
}

///////////////////////////////////////////////////////////////////////////////
// Timer
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::OnIdle(wxIdleEvent& event)
{
	DWORD current=GetTickCount();
	if(m_Emu->m_IsRunning==true )
	{	
		if(current-last_tick>=15)
		{	EmulateFrame();
			last_tick=current;
		}
		else
			event.RequestMore(true);
	}
	else
	{	// l'emu ne tourne pas
		
		wxGetApp().GetEmuVideoBuffer()->Clear(0,0,0);
		m_VideoPanel->SetBitmap(wxGetApp().GetEmuVideoBuffer());
		m_VideoPanel->Refresh();
	}
}

///////////////////////////////////////////////////////////////////////////////
// Emulate one frame
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::EmulateFrame(void)
{
	if(m_Emu->m_IsPaused==false)
	{	
		m_DInput->Update();
		
		if(m_DInput->Check_Key(DIK_BACKSPACE))
			m_Emu->m_Inputs->pad[0] |= mdINPUT_START;
		else
			m_Emu->m_Inputs->pad[0] &= ~mdINPUT_START;

		if(m_DInput->Check_Key(DIK_UP))
			m_Emu->m_Inputs->pad[0] |= mdINPUT_UP;
		else
			m_Emu->m_Inputs->pad[0] &= ~mdINPUT_UP;

		if(m_DInput->Check_Key(DIK_DOWN))
			m_Emu->m_Inputs->pad[0] |= mdINPUT_DOWN;
		else
			m_Emu->m_Inputs->pad[0] &= ~mdINPUT_DOWN;

		if(m_DInput->Check_Key(DIK_LEFT))
			m_Emu->m_Inputs->pad[0] |= mdINPUT_LEFT;
		else
			m_Emu->m_Inputs->pad[0] &= ~mdINPUT_LEFT;

		if(m_DInput->Check_Key(DIK_RIGHT))
			m_Emu->m_Inputs->pad[0] |= mdINPUT_RIGHT;
		else
			m_Emu->m_Inputs->pad[0] &= ~mdINPUT_RIGHT;

		if(m_DInput->Check_Key(DIK_A))
			m_Emu->m_Inputs->pad[0] |= mdINPUT_A;
		else
			m_Emu->m_Inputs->pad[0] &= ~mdINPUT_A;

		if(m_DInput->Check_Key(DIK_S))
			m_Emu->m_Inputs->pad[0] |= mdINPUT_B;
		else
			m_Emu->m_Inputs->pad[0] &= ~mdINPUT_B;

		if(m_DInput->Check_Key(DIK_D))
			m_Emu->m_Inputs->pad[0] |= mdINPUT_C;
		else
			m_Emu->m_Inputs->pad[0] &= ~mdINPUT_C;

		// justifier
		if(m_DInput->Check_Key(DIK_T))
			m_Emu->m_Inputs->pad[4] |= mdINPUT_A;
		else
			m_Emu->m_Inputs->pad[4] &= ~mdINPUT_A;

		if(m_DInput->Check_Key(DIK_Y))
			m_Emu->m_Inputs->pad[4] |= mdINPUT_START;
		else
			m_Emu->m_Inputs->pad[4] &= ~mdINPUT_START;
		
		wxGetApp().RunFrame();

		if(m_Emu->m_ReachedBreakPoint==true)
		{	m_Emu->m_IsPaused=true; 	}

		if( (m_DisAsmPanel->m_PageTopPC>m_Emu->m_CurrentPC) || (m_DisAsmPanel->m_PageBottomPC<m_Emu->m_CurrentPC))
		{
			m_DisAsmPanel->m_PageTopPC=m_Emu->m_CurrentPC;
		}
		m_DisAsmPanel->DisAsm();
		m_RegPanel->UpdadeRegisters();
		m_SpiesPanel->UpdateSpies();
		m_VDPStatus->Update();
		m_CRAMPanel->UpdateCRAM();
		
	}
	m_VideoPanel->Refresh();

}




void mdFrmMain::GotoAddress(int address)
{
	//if(address<=address<rom_size)
	//{
		m_DisAsmPanel->m_PageTopPC=address;
		m_DisAsmPanel->DisAsm();
	//}
}

///////////////////////////////////////////////////////////////////////////////
// Quit event
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::OnQuit(wxCommandEvent&)
{
	Close(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// Load the rom
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::OnLoadRom(wxCommandEvent&)
{
	m_LogPanel->Flush();
//	m_SpiesPanel->Clear();
	wxGetApp().LoadRom();
	m_DisAsmPanel->m_PageTopPC= m_Emu->m_PreviousPC;
	
	m_DisAsmPanel->DisAsm();
	m_RegPanel->UpdadeRegisters();		
	
	m_SpiesPanel->UpdateSpies();
	m_VDPStatus->Reset();
	m_VDPStatus->Update();
	m_CRAMPanel->UpdateCRAM();
}

void mdFrmMain::OnLoadRom(wxChar* filename)
{
	OnLoadRom(filename,true);
}

void mdFrmMain::OnLoadRom(wxChar *filename,bool clearSpies)
{
	m_LogPanel->Flush();
	wxGetApp().LoadRom(filename);
	m_DisAsmPanel->m_PageTopPC= m_Emu->m_PreviousPC;
	
	m_DisAsmPanel->DisAsm();
	m_RegPanel->UpdadeRegisters();		
	if(clearSpies==true)
		m_SpiesPanel->Clear();

	m_SpiesPanel->UpdateSpies();

	m_VDPStatus->Reset();
	m_VDPStatus->Update();

	m_CRAMPanel->UpdateCRAM();
}

void mdFrmMain::LoadLayout()
{
	wxFileName *f=new wxFileName(wxGetApp().GetAppPath(),"layout.bin");

	

	if(f->FileExists()==TRUE)
	{	wxFileInputStream fi(f->GetFullName());
		wxUtil::ReadWindowLayout( fi, this );
		m_LayoutManager->LoadFromStream( fi );
	}
}

void mdFrmMain::SaveLayout()
{
	wxFileName *f=new wxFileName(wxGetApp().GetAppPath(),"layout.bin");

	wxFileOutputStream fo(f->GetFullName());
    wxUtil::WriteWindowLayout( fo, this );
    m_LayoutManager->SaveToStream( fo );
    
}

///////////////////////////////////////////////////////////////////////////////
// Reload the rom
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::OnReloadRom(wxCommandEvent&)
{
	m_LogPanel->Flush();
	wxGetApp().ReloadRom();
	m_DisAsmPanel->m_PageTopPC= m_Emu->m_PreviousPC;
	
	m_DisAsmPanel->DisAsm();
	m_RegPanel->UpdadeRegisters();		
	//m_SpiesPanel->Clear();
	m_SpiesPanel->UpdateSpies();
	m_VDPStatus->Update();
	m_CRAMPanel->UpdateCRAM();
}

///////////////////////////////////////////////////////////////////////////////
// Pause the emulator
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::OnPause(wxCommandEvent&)
{
	if(m_Emu->m_RomLoaded==true)
	{	m_Emu->m_IsPaused=!m_Emu->m_IsPaused;
	}
}


///////////////////////////////////////////////////////////////////////////////
// Run emulator
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::OnRun(wxCommandEvent&)
{	
	if(!m_Emu->m_RomLoaded)
		return;
	
	last_tick=GetTickCount();
	m_VideoPanel->SetBitmap(wxGetApp().GetEmuVideoBuffer());
	m_Emu->m_IsRunning=true;
	m_Emu->m_IsPaused=false;
}

///////////////////////////////////////////////////////////////////////////////
// step one frame
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::OnStepFrame(wxCommandEvent& event)
{
	if(!m_Emu->m_RomLoaded)
		return;

	if(m_Emu->m_IsPaused==false)
		m_Emu->m_IsPaused=true;

	if(m_Emu->m_IsRunning==false)
		m_Emu->m_IsRunning=true;

	if(m_Emu->m_IsRunning==true)
	{	m_Emu->m_IsPaused=false;
		EmulateFrame();
		m_Emu->m_IsPaused=true;
	}
}

///////////////////////////////////////////////////////////////////////////////
// step into
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::OnStepInto(wxCommandEvent& event)
{
	m_Emu->m_IsPaused=true;
	m_Emu->StepInto();

	if( (m_DisAsmPanel->m_PageTopPC>m_Emu->m_CurrentPC) || (m_DisAsmPanel->m_PageBottomPC<m_Emu->m_CurrentPC))
	{	m_DisAsmPanel->m_PageTopPC=m_Emu->m_CurrentPC;	}

	m_DisAsmPanel->DisAsm();
	m_RegPanel->UpdadeRegisters();
	m_SpiesPanel->UpdateSpies();
	m_VDPStatus->Update();
	m_CRAMPanel->UpdateCRAM();
}

///////////////////////////////////////////////////////////////////////////////
// Menu vue
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::OnViewVideoRendering(wxCommandEvent& event)
{	
	if(m_VideoDock->IsVisible()==true)
	{	m_VideoDock->Show(FALSE);
		
	}
	else
	{	m_VideoDock->SetDocked(FALSE);
		m_VideoDock->Show(TRUE);
		
	}
}

void mdFrmMain::OnViewLog(wxCommandEvent& event)
{
	if(m_LogDock->IsVisible()==true)
	{	m_LogDock->Show(FALSE);
		
	}
	else
	{	m_LogDock->SetDocked(FALSE);
		m_LogDock->Show(TRUE);
			
	}
}

void mdFrmMain::OnView68kRegisters(wxCommandEvent& event)
{
	if(m_RegDock->IsVisible()==true)
	{	m_RegDock->Show(FALSE);
		
	}
	else
	{	m_RegDock->SetDocked(FALSE);
		m_RegDock->Show(TRUE);
			
	}
}

void mdFrmMain::OnView68kWatchers(wxCommandEvent& event)
{
	if(m_SpiesDock->IsVisible()==true)
	{	m_SpiesDock->Show(FALSE);
	
	}
	else
	{	m_SpiesDock->SetDocked(FALSE);
		m_SpiesDock->Show(TRUE);
				
	}
}

void mdFrmMain::OnViewCRAM(wxCommandEvent& event)
{
	if(m_CRAMDock->IsVisible()==true)
	{	m_CRAMDock->Show(FALSE);
		
	}
	else
	{	m_CRAMDock->SetDocked(FALSE);
		m_CRAMDock->Show(TRUE);
					
	}
}

void mdFrmMain::OnViewVDPStatus(wxCommandEvent& event)
{
	if(m_VDPStatusDock->IsVisible()==true)
	{	m_VDPStatusDock->Show(FALSE);
		
	}
	else
	{	m_VDPStatusDock->SetDocked(FALSE);
		m_VDPStatusDock->Show(TRUE);					
	}
}


void mdFrmMain::OnViewFullScreen(wxCommandEvent& event)
{
	if(this->IsFullScreen()==FALSE)
		this->ShowFullScreen(TRUE,wxFULLSCREEN_NOCAPTION | wxFULLSCREEN_NOBORDER);
	else
        this->ShowFullScreen(FALSE);

	m_DisAsmPanel->Refresh();
	m_DisAsmPanel->DisAsm();
}

///////////////////////////////////////////////////////////////////////////////
// Vdp reg calc
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::OnVdpCalc(wxCommandEvent& event)
{
	if(m_FrmVdpCalc==NULL)
	{
		m_FrmVdpCalc = new mdFrmVdpCalc(	this,
											_T("MD VDP Regs Calculator"),
											50, 50, 300, 300);	
	}
		
	m_FrmVdpCalc->Show();
}

void mdFrmMain::OnVdpAddrCalc(wxCommandEvent& event)
{
	if(m_FrmVdpAddrCalc==NULL)
	{
		m_FrmVdpAddrCalc = new mdFrmVdpAddrCalc(	this,
											_T("MD VDP Address Calculator"),
											50, 50, 200, 120);	
	}
		
	m_FrmVdpAddrCalc->Show();
}

void mdFrmMain::FlushLog()
{	
	m_LogPanel->Flush();
}

void mdFrmMain::LogChar(int level,char text)
{
	m_LogPanel->LogChar(level,text);
	m_LogPanel->Refresh();
}

void mdFrmMain::LogMessage(int level,char* text)
{
	m_LogPanel->LogMessage(level,text);
	m_LogPanel->Refresh();	
}


void mdFrmMain::OnKeyDown(wxKeyEvent& event)
{
	mdEmu *e=m_Emu;
	int c=event.GetKeyCode();

	switch(c)
	{
		case WXK_DOWN:
		{
			wxGetApp().LoadRom();

		}break;	
	}
}

///////////////////////////////////////////////////////////////////////////////
// Dump
///////////////////////////////////////////////////////////////////////////////

/*
extern	uint8 cram[0x80];               
extern	uint8 vsram[0x80]; 
extern	uint8 vram[0x10000]; 

extern	int8 cart_rom[0x400000];   
extern	uint8 work_ram[0x10000];   
extern	uint8 zram[0x2000];  
*/

void mdFrmMain::OnDumpROM(wxCommandEvent& event)
{	mdFrmDumpBin *f=new mdFrmDumpBin(this,_T("Dump Rom"));
	f->SetOptions(gProject->DumpFile_Rom,0,gEmu->m_RomSize,0,gEmu->m_RomSize);
	if(f->ShowModal()==wxID_OK)
	{
		// dump
		gApp.DumpMemoryToFile(gProject->DumpFile_Rom,gEmu->GetROM(),f->GetBaseAddress(),f->GetLength());
	}
	f->Destroy();
}
void mdFrmMain::OnDumpRAM(wxCommandEvent& event)
{
	mdFrmDumpBin *f=new mdFrmDumpBin(this,_T("Dump 68k Ram"));
	f->SetOptions(gProject->DumpFile_68kRam,0,0x10000,0,0x10000);
	if(f->ShowModal()==wxID_OK)
	{
		// dump
		gApp.DumpMemoryToFile(gProject->DumpFile_68kRam,gEmu->GetRAM(),f->GetBaseAddress(),f->GetLength());
	}
	f->Destroy();
}
void mdFrmMain::OnDumpCRAM(wxCommandEvent& event)
{
	mdFrmDumpBin *f=new mdFrmDumpBin(this,_T("Dump Color Ram"));
	f->SetOptions(gProject->DumpFile_CRam,0,0x80,0,0x80);
	if(f->ShowModal()==wxID_OK)
	{	// dump
		gApp.DumpMemoryToFile(gProject->DumpFile_CRam,gEmu->GetCRAM(),f->GetBaseAddress(),f->GetLength());
	}
	f->Destroy();

}
void mdFrmMain::OnDumpVRAM(wxCommandEvent& event)
{
	mdFrmDumpBin *f=new mdFrmDumpBin(this,_T("Dump Video Ram"));
	f->SetOptions(gProject->DumpFile_VRam,0,0x10000/4,0,0x10000/4);
	if(f->ShowModal()==wxID_OK)
	{	// dump
		gApp.DumpMemoryToFile(gProject->DumpFile_VRam,gEmu->GetVRAM(),f->GetBaseAddress(),f->GetLength());
	}
	f->Destroy();

}

void mdFrmMain::OnDumpVSRAM(wxCommandEvent& event)
{
	mdFrmDumpBin *f=new mdFrmDumpBin(this,_T("Dump Scroll Ram"));
	f->SetOptions(gProject->DumpFile_VSRam,0,0x80,0,0x80);
	if(f->ShowModal()==wxID_OK)
	{	// dump
		gApp.DumpMemoryToFile(gProject->DumpFile_VSRam,gEmu->GetVSRAM(),f->GetBaseAddress(),f->GetLength());
	}
	f->Destroy();

}

void mdFrmMain::OnDumpZRAM(wxCommandEvent& event)
{
	mdFrmDumpBin *f=new mdFrmDumpBin(this,_T("Dump Z80 Ram"));
	f->SetOptions(gProject->DumpFile_Z80Ram,0,0x2000,0,0x2000);
	if(f->ShowModal()==wxID_OK)
	{	// dump
		gApp.DumpMemoryToFile(gProject->DumpFile_Z80Ram,gEmu->GetZRAM(),f->GetBaseAddress(),f->GetLength(),false);
	}
	f->Destroy();

}

///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::OnKeyUp(wxKeyEvent& event)
{}

void mdFrmMain::OnGotoAddress(wxCommandEvent& event)
{
	if(m_FrmGoto==NULL)
	{
		m_FrmGoto = new mdFrmGoto(	this,
											_T("Goto to address"),
											50, 50, 180, 85);	
		//									50, 50, 180, 85);	
	}
		
	m_FrmGoto->Show();
}


void mdFrmMain::UpdateDisAsm(void)
{
	m_DisAsmPanel->DisAsm();
}

///////////////////////////////////////////////////////////////////////////////
// Enregistre un screenshot
///////////////////////////////////////////////////////////////////////////////

void mdFrmMain::OnSaveScreenshot(wxCommandEvent& event)
{
	wxString f=_T("c:\\test.pcx");
	gApp.OutputPicture(f);
}



