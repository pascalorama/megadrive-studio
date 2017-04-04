#ifndef __mdPlugin_H__
#define __mdPlugin_H__

///////////////////////////////////////////////////////////////////////////////
// Class definition
///////////////////////////////////////////////////////////////////////////////

class wxDynamicLibrary;

#include "include/mdSDK.h"

class mdPlugin
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////

	mdPlugin(const wxString& filename);
	~mdPlugin();

	int LoadRom(const wxString &filename);
	void GetVideoBufferAttr(int *w,int *h,int *bpp,int *rgbformat);
	unsigned int InitEmulator(unsigned char*videobuffer,int bpp);
	int RunFrame(int tick);
	void CloseEmulator(void);
	int StepInto(int t);
	unsigned int DisAsm(char* buff,unsigned int pc);
	void GetRomInfo(t_RomInfo *info);
	unsigned int GetCpuRegister(int cpu_id,int reg_id);
	void SetCallBack(t_Callbacks* calls);

	unsigned int M68K_ReadMemory8(unsigned int addr);
	unsigned int M68K_ReadMemory16(unsigned int addr);
	unsigned int M68K_ReadMemory32(unsigned int addr);

	t_mdinput* GetInput(void);
	unsigned char* GetVDPRegisters(void);

	unsigned char* GetCRAM(void);
	unsigned char* GetVSRAM(void);
	unsigned char* GetVRAM(void);

	unsigned char* GetRAM(void);
	unsigned char* GetROM(void);
	unsigned char* GetZRAM(void);
	unsigned char* GetTilesCache(void);

	int GenerateScreenshot(mdPictureInfo *info,unsigned char* pixels,mdColor *pal);
			
	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////

	typedef	int(*fctLoadRom)(char* rom);
	typedef void (*fctGetVideoBufferAttr)(int *w,int *h,int *bpp,int *rgbformat);
	typedef unsigned int (*fctInitEmulator)(unsigned char*videobuffer,int bpp);
	typedef void (*fctCloseEmulator)(void);
	typedef void (*fctGetRomInfo)(t_RomInfo*);
	typedef int (*fctStepInto)(int t);
	typedef int (*fctRunFrame)(int tick);
	typedef unsigned int (*fctDisAsm)(char* buff,unsigned int pc);
	typedef unsigned int (*fctGetCpuRegister)(int cpu_id,int reg_id);
	typedef void (*fctSetCallBack)(t_Callbacks* calls);
	typedef t_mdinput* (*fctGetInput)(void);
	typedef unsigned char*	(*fctGetVDPRegisters)(void);
	typedef unsigned char*	(*fctGetVRAM)(void);
	typedef unsigned char*	(*fctGetCRAM)(void);
	typedef unsigned char*	(*fctGetVSRAM)(void);

	typedef unsigned char*	(*fctGetROM)(void);
	typedef unsigned char*	(*fctGetRAM)(void);
	typedef unsigned char*	(*fctGetZRAM)(void);
	typedef unsigned char*	(*fctGetTilesCache)(void);

	typedef int (*fctGenerateScreenshot)(mdPictureInfo *info,unsigned char* pixels,mdColor *pal);

	typedef unsigned int (*fctm68kReadMemory8)(unsigned int address);
	typedef unsigned int (*fctm68kReadMemory16)(unsigned int address);
	typedef unsigned int (*fctm68kReadMemory32)(unsigned int address);
    
	wxString			m_FileName;
	wxDynamicLibrary	*m_Library;	

	fctLoadRom	m_fctLoadRom;
	fctGetVideoBufferAttr	m_fctGetVideoBufferAttr;
	fctInitEmulator	m_fctInitEmulator;
	fctRunFrame m_fctRunFrame;
	fctCloseEmulator m_fctCloseEmulator;		
	fctGetRomInfo m_fctGetRomInfo;
	fctStepInto m_fctStepInto;
	fctDisAsm m_fctDisAsm;
	fctGetCpuRegister m_fctGetCpuRegister;
	fctSetCallBack m_fctSetCallBack;
	fctGetInput	m_fctGetInput;

	fctGetVDPRegisters	m_fctGetVDPRegisters;
	fctGetVRAM	m_fctGetVRAM;
	fctGetCRAM	m_fctGetCRAM;
	fctGetVSRAM	m_fctGetVSRAM;

	fctGetROM	m_fctGetROM;
	fctGetRAM	m_fctGetRAM;
	fctGetZRAM	m_fctGetZRAM;
	fctGetTilesCache	m_fctGetTileCache;

	fctGenerateScreenshot m_fctGenerateScreenshot;

	fctm68kReadMemory8 m_fctm68kReadMemory8;
	fctm68kReadMemory16 m_fctm68kReadMemory16;
	fctm68kReadMemory32 m_fctm68kReadMemory32;


	void GetSymbols(void);
	
};

///////////////////////////////////////////////////////////////////////////////
#endif