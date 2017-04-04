///////////////////////////////////////////////////////////////////////////////
//
//

#ifndef __MDAPP_H__
#define __MDAPP_H__

///////////////////////////////////////////////////////////////////////////////
// class definition
///////////////////////////////////////////////////////////////////////////////

#include "include/mdPluginList.h"
#include "include/mdProject.h"

class mdEmu;
class mdDibBitmap;
class mdOutputDataPlugin;
class mdProject;

class mdApp : public wxApp
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////  
    virtual bool OnInit();
	virtual int OnExit();

	bool LoadRom(void);
	bool LoadRom(wxChar* filename);
	bool ReloadRom(void);
	int RunFrame(void);

	mdFrmMain*	GetFrmMain(void){	return m_FrmMain;}
	
	wxString& GetRomFileName(void){	return m_RomFileName;}
	mdDibBitmap* GetEmuVideoBuffer(void);

	int OutputPicture(wxString &filename);
	wxString& GetAppPath(void){	return m_AppPath;}

	mdProject*	GetProject(void);

	int DumpMemoryToFile(wxString& FileName,unsigned char* Data,int base,int length,bool SwapByte=true);

	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////
	mdFrmMain	*m_FrmMain;
//	mdEmu	*m_Emu;
	wxString m_RomFileName;
	wxString m_AppPath;
	mdProject	m_Project;

	mdPluginsList<mdOutputDataPlugin> *m_OutputPlugins;

	
	

	///////////////////////////////////////////////////////////////////////////
	private:
	////////

	
};

DECLARE_APP(mdApp)

///////////////////////////////////////////////////////////////////////////////
#endif