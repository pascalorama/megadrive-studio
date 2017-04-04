///////////////////////////////////////////////////////////////////////////////
// app includes

#include "include/mdShared.h"
#include "include/mdVideoAdapter.h"
#include "include/mdVideoSurface.h"
#include "include/mdPlugin.h"
#include "include/mdDibBitmap.h"
#include "include/mdSDK.h"
#include "include/mdDisasm.h"
#include "include/mdBreakPoint.h"
#include "include/mdEmu.h"
#include "include/mdProxy.h"
#include "include/mdFrmMain.h"
#include "include/mdApp.h"

t_Callbacks fct;

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

mdEmu::mdEmu(const wxString &plug)
{
	int w,h,b,r;
	m_Bitmap=NULL;
	m_IsRunning=false;
	m_RomLoaded=false;

	// load the emu plugins
	m_Emu=new mdPlugin(plug);	
	w=h=b=r=0;
	m_RomSize=0;
	m_ReachedBreakPoint=false;
		
	m_Emu->GetVideoBufferAttr(&w,&h,&b,&r);
	b=gVideoAdapter->m_BytesPerPixel;
	if(w>0 && h>0 && b>0 )
	{	m_Bitmap=new mdDibBitmap(w,h,b<<3);		}

	m_Inputs=m_Emu->GetInput();
	m_VDPRegs=m_Emu->GetVDPRegisters();

	// init des callbacks 
	fct.m_fctCheckBreakPoint =  CheckBreakPoint;
	fct.m_fctSetBreakPoint = SetBreakPoint;
	fct.m_fctLogMessage = LogMessage;
	fct.m_fctLogChar= LogChar;
	fct.m_fctGetSymbolName=GetSymbolName;
	m_Emu->SetCallBack(&fct);	

	
}

///////////////////////////////////////////////////////////////////////////////
// delete
///////////////////////////////////////////////////////////////////////////////

mdEmu::~mdEmu()
{	
	FreeRom();

	// clean de la hash
	delete(m_Bitmap);
	m_Bitmap=NULL;

	delete(m_Emu);
	m_Emu=NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Free the rom
///////////////////////////////////////////////////////////////////////////////

void mdEmu::FreeRom(void)
{
	m_Emu->CloseEmulator();

	// efface tous les breakpoints
	gProject->ClearAllBreakPoints();

}

///////////////////////////////////////////////////////////////////////////////
// Load rom
///////////////////////////////////////////////////////////////////////////////

int mdEmu::LoadRom(const wxString &rom)
{
	if(m_RomLoaded==true)
	{	// clean de la hash
		FreeRom();
	}

	int i=m_Emu->LoadRom(rom);
		
	m_Emu->InitEmulator(m_Bitmap->Data,gVideoAdapter->m_BitsPerPixel);

	t_RomInfo info;

	this->GetRomInfo(&info);
	m_RomSize=info.size;

	m_IsRunning=false;
	m_IsPaused=false;
	m_RomLoaded=false;
	if(i!=0)
		m_RomLoaded=true;

	m_PreviousPC=0;
	m_CurrentPC=m_Emu->GetCpuRegister(mdM68K_CPU,mdM68K_REG_PC);
	m_PreviousPC=m_CurrentPC;
	

	return i;
}

///////////////////////////////////////////////////////////////////////////////
// Get the rom information
///////////////////////////////////////////////////////////////////////////////

void mdEmu::GetRomInfo(t_RomInfo *i)
{	m_Emu->GetRomInfo(i);}

///////////////////////////////////////////////////////////////////////////////
// Run Frame
///////////////////////////////////////////////////////////////////////////////

int mdEmu::RunFrame()
{
	int i;
	m_ReachedBreakPoint=false;
	m_PreviousPC=m_Emu->GetCpuRegister(mdM68K_CPU,mdM68K_REG_PC);
	i=m_Emu->RunFrame(487);
	m_CurrentPC=m_Emu->GetCpuRegister(mdM68K_CPU,mdM68K_REG_PC);

	return i;
	
}

///////////////////////////////////////////////////////////////////////////////
// Step into
///////////////////////////////////////////////////////////////////////////////

int mdEmu::StepInto()
{	
	m_ReachedBreakPoint=false;
	m_PreviousPC=m_Emu->GetCpuRegister(mdM68K_CPU,mdM68K_REG_PC);
	int i=m_Emu->StepInto(1);
	m_CurrentPC=m_Emu->GetCpuRegister(mdM68K_CPU,mdM68K_REG_PC);

	return i;
}

///////////////////////////////////////////////////////////////////////////////
// Disasm
///////////////////////////////////////////////////////////////////////////////

unsigned int mdEmu::DisAsm(wxString &buf,unsigned int pc)
{
	char t[128];
	unsigned int p=0;
	p=m_Emu->DisAsm(t,pc);

	wxString w(t);
	buf=w;

	return p;
}

int mdEmu::ExecuteBreakPoint(unsigned int pc)
{
	return gProject->ExecuteBreakPoint((int)pc);
}

bool mdEmu::CheckIsBreakPoint(unsigned int pc)
{
	return gProject->CheckGotBreakPoint((int)pc);
}

void mdEmu::CreateBreakPoint(unsigned int pc)
{	
	mdBreakPoint *bk=gProject->InsertBreakPoint((int)pc);
	if(bk!=NULL)
	{	bk->m_Enabled=true;
		bk->m_Hits=0;
	}

}

void mdEmu::EraseBreakPoint(unsigned int pc)
{	
	gProject->RemoveBreakPoint((int)pc);
}

int mdEmu::GenerateScreenshot(mdPictureInfo *info,unsigned char* pixels,mdColor *pal)
{
	return m_Emu->GenerateScreenshot(info,pixels,pal);
}

unsigned int mdEmu::GetCpuRegister(int cpu_id,int reg_id)
{
	return m_Emu->GetCpuRegister(cpu_id,reg_id);
}

unsigned int mdEmu::M68K_ReadMemory8(unsigned int addr)
{	return m_Emu->M68K_ReadMemory8(addr); }

unsigned int mdEmu::M68K_ReadMemory16(unsigned int addr)
{	return m_Emu->M68K_ReadMemory16(addr); }

unsigned int mdEmu::M68K_ReadMemory32(unsigned int addr)
{	return m_Emu->M68K_ReadMemory32(addr); }

void mdEmu::SetInput(int player_id,int value)
{
	/* if(keystate[SDLK_UP])     input.pad[0] |= INPUT_UP;
    else
    if(keystate[SDLK_DOWN])   input.pad[0] |= INPUT_DOWN;
    if(keystate[SDLK_LEFT])   input.pad[0] |= INPUT_LEFT;
    else
    if(keystate[SDLK_RIGHT])  input.pad[0] |= INPUT_RIGHT;

    if(keystate[SDLK_a])      input.pad[0] |= INPUT_A;
    if(keystate[SDLK_s])      input.pad[0] |= INPUT_B;
    if(keystate[SDLK_d])      input.pad[0] |= INPUT_C;
    if(keystate[SDLK_f])      input.pad[0] |= INPUT_START;
    if(keystate[SDLK_z])      input.pad[0] |= INPUT_X;
    if(keystate[SDLK_x])      input.pad[0] |= INPUT_Y;
    if(keystate[SDLK_c])      input.pad[0] |= INPUT_Z;
    if(keystate[SDLK_v])      input.pad[0] |= INPUT_MODE;
 */


}

unsigned char* mdEmu::GetCRAM(void)
{	return m_Emu->GetCRAM();}

unsigned char* mdEmu::GetVRAM(void)
{	return m_Emu->GetVRAM();}

unsigned char* mdEmu::GetVSRAM(void)
{	return m_Emu->GetVSRAM();}

unsigned char* mdEmu::GetRAM(void)
{	return m_Emu->GetRAM();}

unsigned char* mdEmu::GetZRAM(void)
{	return m_Emu->GetZRAM();}

unsigned char* mdEmu::GetROM(void)
{	return m_Emu->GetROM();}

unsigned char* mdEmu::GetTilesCache(void)
{	return m_Emu->GetTilesCache();}




