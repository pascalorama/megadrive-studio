///////////////////////////////////////////////////////////////////////////////
// Emulator plugin definition
//
//
// 
///////////////////////////////////////////////////////////////////////////////
// Includes 
#include "include/mdShared.h"
#include "include/mdPlugin.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

mdPlugin::mdPlugin(const wxString &filename)
{
	m_Library=NULL;
	m_Library=new wxDynamicLibrary(filename);
	m_FileName = filename;

	m_fctLoadRom=NULL;
	m_fctGetVideoBufferAttr=NULL;
	m_fctInitEmulator=NULL;
	m_fctRunFrame=NULL;
	m_fctCloseEmulator=NULL;
	m_fctGetRomInfo=NULL;
	m_fctStepInto=NULL;
	m_fctDisAsm=NULL;
	m_fctGetCpuRegister=NULL;
	m_fctSetCallBack=NULL;
	m_fctGenerateScreenshot=NULL;
	m_fctGetInput=NULL;
	m_fctGetVDPRegisters=NULL;

	m_fctGetVRAM=NULL;
	m_fctGetCRAM=NULL;
	m_fctGetVSRAM=NULL;

	m_fctGetROM=NULL;
	m_fctGetRAM=NULL;
	m_fctGetZRAM=NULL;
	m_fctGetTileCache=NULL;
	
	m_fctm68kReadMemory16=NULL;
	m_fctm68kReadMemory32=NULL;
	m_fctm68kReadMemory8=NULL;

	GetSymbols();
}

///////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////

mdPlugin::~mdPlugin()
{
	if(m_Library!=NULL)
	{	if(m_Library->IsLoaded()==TRUE)
			m_Library->Unload();

		delete(m_Library);
		m_Library=NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
// GetSymbols
///////////////////////////////////////////////////////////////////////////////

void mdPlugin::GetSymbols(void)
{
	if(m_Library->IsLoaded()==TRUE)
	{			
		m_fctLoadRom=(fctLoadRom)	m_Library->GetSymbol("LoadRom");		
		m_fctGetVideoBufferAttr=(fctGetVideoBufferAttr)
									m_Library->GetSymbol("GetVideoBufferAttributes");
		m_fctInitEmulator=(fctInitEmulator)
									m_Library->GetSymbol("InitEmulator");
		m_fctRunFrame=(fctRunFrame)
									m_Library->GetSymbol("RunFrame");

		m_fctCloseEmulator=(fctCloseEmulator)
									m_Library->GetSymbol("CloseEmulator");

		m_fctGetRomInfo=(fctGetRomInfo)
									m_Library->GetSymbol("GetRomInfo");

		m_fctStepInto=(fctStepInto)
									m_Library->GetSymbol("StepInto");

		m_fctDisAsm=(fctDisAsm)
									m_Library->GetSymbol("DisAsm");

		m_fctGetCpuRegister=(fctGetCpuRegister)
									m_Library->GetSymbol("GetCpuRegister");

		m_fctSetCallBack=(fctSetCallBack)
									m_Library->GetSymbol("SetCallBack");

		m_fctm68kReadMemory8=(fctm68kReadMemory8)
									m_Library->GetSymbol("m68kReadMemory8");

		m_fctm68kReadMemory16=(fctm68kReadMemory16)
									m_Library->GetSymbol("m68kReadMemory16");

		m_fctm68kReadMemory32=(fctm68kReadMemory32)
									m_Library->GetSymbol("m68kReadMemory32");

		m_fctGenerateScreenshot=(fctGenerateScreenshot)
									m_Library->GetSymbol("GenerateScreenshot");

		m_fctGetInput=(fctGetInput)
									m_Library->GetSymbol("GetInput");

		m_fctGetVDPRegisters=(fctGetVDPRegisters)
									m_Library->GetSymbol("GetVDPRegisters");

		m_fctGetVRAM=(fctGetVRAM)
									m_Library->GetSymbol("GetVRAM");

		m_fctGetCRAM=(fctGetCRAM)
									m_Library->GetSymbol("GetCRAM");

		m_fctGetVSRAM=(fctGetVSRAM)
									m_Library->GetSymbol("GetVSRAM");

		m_fctGetRAM=(fctGetRAM)
									m_Library->GetSymbol("GetRAM");

		m_fctGetROM=(fctGetROM)
									m_Library->GetSymbol("GetROM");

		m_fctGetZRAM=(fctGetZRAM)
									m_Library->GetSymbol("GetZRAM");
		
		m_fctGetTileCache=(fctGetTilesCache)
									m_Library->GetSymbol("GetTilesCache");
	}
}

///////////////////////////////////////////////////////////////////////////////
// Load a rom
///////////////////////////////////////////////////////////////////////////////

int mdPlugin::LoadRom(const wxString &filename)
{
	if(m_Library->IsLoaded()==TRUE)
	{			
		if(m_fctLoadRom)
		{	return m_fctLoadRom((char*)filename.c_str());	}		
	}

	return 0;
}

void mdPlugin::GetVideoBufferAttr(int *w,int *h,int *bpp,int *rgbformat)
{
	if(m_Library->IsLoaded()==TRUE)
	{			
		if(m_fctGetVideoBufferAttr)
		{	m_fctGetVideoBufferAttr(w,h,bpp,rgbformat);	}		
	}
}

unsigned int mdPlugin::InitEmulator(unsigned char *vbuf,int bpp)
{
	if(m_Library->IsLoaded()==TRUE)
	{			
		if(m_fctInitEmulator)
		{	return m_fctInitEmulator(vbuf,bpp);	}		
	}
	
	return 0;
}

int mdPlugin::RunFrame(int tick)
{
	if(m_Library->IsLoaded()==TRUE)
	{			
		if(m_fctRunFrame)
		{	return m_fctRunFrame(tick);	}		
	}
	
	return 0;
}

int mdPlugin::StepInto(int tick)
{
	if(m_Library->IsLoaded()==TRUE)
	{			
		if(m_fctStepInto)
		{	return m_fctStepInto(tick);	}		
	}
	return 0;
}

void mdPlugin::CloseEmulator(void)
{
	if(m_Library->IsLoaded()==TRUE)
	{			
		if(m_fctCloseEmulator)
		{	m_fctCloseEmulator();	}		
	}		
}

void mdPlugin::GetRomInfo(t_RomInfo *info)
{
	if(m_Library->IsLoaded()==TRUE)
	{			
		if(m_fctGetRomInfo)
		{	m_fctGetRomInfo(info);	}		
	}		
}

unsigned int mdPlugin::DisAsm(char* buf,unsigned int pc)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctDisAsm)
		{	return m_fctDisAsm(buf,pc);
		}
	}
	return 0;
}

unsigned int mdPlugin::GetCpuRegister(int cpu_id,int reg_id)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctGetCpuRegister)
		{	return m_fctGetCpuRegister(cpu_id,reg_id);}
	}

	return 0;
}

void mdPlugin::SetCallBack(t_Callbacks *calls)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctSetCallBack)
			m_fctSetCallBack(calls);
	}
}

unsigned int mdPlugin::M68K_ReadMemory8(unsigned int addr)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctm68kReadMemory8)
			return m_fctm68kReadMemory8(addr);
	}

	return 0xff;
}

unsigned int mdPlugin::M68K_ReadMemory16(unsigned int addr)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctm68kReadMemory16)
			return m_fctm68kReadMemory16(addr);
	}

	return 0xffff;

}

unsigned int mdPlugin::M68K_ReadMemory32(unsigned int addr)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctm68kReadMemory32)
			return m_fctm68kReadMemory32(addr);
	}

	return 0xffffffff;

}

int mdPlugin::GenerateScreenshot(mdPictureInfo *info,unsigned char* pixels,mdColor *pal)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctGenerateScreenshot)
			return m_fctGenerateScreenshot(info,pixels,pal);
	}

	return 0xffff;


	return 0;
}

t_mdinput* mdPlugin::GetInput(void)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctGetInput)
			return m_fctGetInput();
	}
	return NULL;
}

unsigned char*mdPlugin::GetVDPRegisters(void)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctGetVDPRegisters)
			return m_fctGetVDPRegisters();
	}
	return NULL;
}

unsigned char*mdPlugin::GetVRAM(void)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctGetVRAM)
			return m_fctGetVRAM();
	}
	return NULL;
}

unsigned char*mdPlugin::GetCRAM(void)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctGetCRAM)
			return m_fctGetCRAM();
	}
	return NULL;
}

unsigned char*mdPlugin::GetVSRAM(void)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctGetVSRAM)
			return m_fctGetVSRAM();
	}
	return NULL;
}

unsigned char*mdPlugin::GetRAM(void)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctGetRAM)
			return m_fctGetRAM();
	}
	return NULL;
}

unsigned char*mdPlugin::GetROM(void)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctGetROM)
			return m_fctGetROM();
	}
	return NULL;
}

unsigned char*mdPlugin::GetZRAM(void)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctGetZRAM)
			return m_fctGetZRAM();
	}
	return NULL;
}

unsigned char*mdPlugin::GetTilesCache(void)
{
	if(m_Library->IsLoaded()==TRUE)
	{
		if(m_fctGetTileCache)
			return m_fctGetTileCache();
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////