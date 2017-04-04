#ifndef _mdEmu_H_
#define _mdEmu_H_

///////////////////////////////////////////////////////////////////////////////
// Class definition
///////////////////////////////////////////////////////////////////////////////

class mdDibBitmap;
class mdPlugin;
#include "mdSDK.h"
class mdDisasm;
class mdBreakPoint;
class mdSpy;
class mdSpiesList;
class mdVideoSurface;

class mdEmu
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////
	mdEmu(const wxString &plug);
	~mdEmu();

	int LoadRom(const wxString &rom);
	int RunFrame(void);
	int StepInto(void);
	unsigned int DisAsm(wxString &buf,unsigned int pc);
	int ExecuteBreakPoint(unsigned int pc);
	bool CheckIsBreakPoint(unsigned int pc);
	void CreateBreakPoint(unsigned int pc);
	void EraseBreakPoint(unsigned int pc);
	void FreeRom(void);

	unsigned int M68K_ReadMemory8(unsigned int addr);
	unsigned int M68K_ReadMemory16(unsigned int addr);
	unsigned int M68K_ReadMemory32(unsigned int addr);

	unsigned int GetCpuRegister(int cpu_id,int reg_id);
	
	void SetInput(int player_id,int value);
	
	unsigned char*	VDPRegs(){	return m_VDPRegs;}
	

	int GenerateScreenshot(mdPictureInfo *info,unsigned char* pixels,mdColor *pal);

	mdDibBitmap* GetBitmap(void){	return m_Bitmap;}
//	mdVideoSurface* GetBitmap(void){	return m_Bitmap;}

	void GetRomInfo(t_RomInfo *i);

	unsigned char* GetCRAM(void);
	unsigned char* GetVRAM(void);
	unsigned char* GetVSRAM(void);
	
	unsigned char* GetRAM(void);
	unsigned char* GetROM(void);
	unsigned char* GetZRAM(void);
	unsigned char* GetTilesCache(void);
    
	// 
	bool m_IsRunning;
	bool m_IsPaused;
	bool m_RomLoaded;
	bool m_ReachedBreakPoint;
	int m_RomSize;

	unsigned int m_PreviousPC;
	unsigned int m_CurrentPC;

	t_mdinput *m_Inputs;

//	mdBreakPointList m_BreakPoints;
    		
	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////
	mdPlugin	*m_Emu;	
	mdDibBitmap *m_Bitmap;
//	mdVideoSurface	*m_Bitmap;

	unsigned char	*m_VDPRegs;
	
	
};

///////////////////////////////////////////////////////////////////////////////

#endif