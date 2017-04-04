///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "emu/shared.h"
#include "string.h"

///////////////////////////////////////////////////////////////////////////////
//#ifndef DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
//#endif
///////////////////////////////////////////////////////////////////////////////
// genesis plus externs
extern	int load_rom(char *filename);
extern	uint8 reg[0x20];    
extern	uint8 cram[0x80];               /* On-chip color RAM (64x9) */
extern	uint8 vsram[0x80]; 
extern	uint8 vram[0x10000]; 

extern	int8 cart_rom[0x400000];   /* Cartridge ROM */
extern	uint8 work_ram[0x10000];    /* 68K work RAM */
extern	uint8 zram[0x2000];      

extern	uint8 bg_pattern_cache[0x80000];

extern	int		rom_size;
extern	char	rom_filename[256];

///////////////////////////////////////////////////////////////////////////////
// 
unsigned int mypc=0;
t_Callbacks *m_CallBacks=NULL;
int m_GotBreakpoint=0;

///////////////////////////////////////////////////////////////////////////////
// genesis plus externs
unsigned int  m68k_read_disassembler_16(unsigned int address)
{
	return m68k_read_memory_16(address);
}

unsigned int  m68k_read_disassembler_32(unsigned int address)
{	
	return m68k_read_memory_32(address);	
}

DLLEXPORT t_mdinput* GetInput(void)
{
	return (t_mdinput*)&input;
}

DLLEXPORT unsigned char* GetVDPRegisters(void)
{
	return  &reg[0]; 
}

DLLEXPORT unsigned char* GetVRAM(void)
{
	return  &vram[0]; 
}

DLLEXPORT unsigned char* GetCRAM(void)
{
	return  &cram[0]; 
}

DLLEXPORT unsigned char* GetVSRAM(void)
{
	return  &vsram[0]; 
}

DLLEXPORT unsigned char* GetROM(void)
{
	return  &cart_rom[0]; 
}

DLLEXPORT unsigned char* GetRAM(void)
{
	return  &work_ram[0]; 
}

DLLEXPORT unsigned char* GetZRAM(void)
{
	return  &zram[0]; 
}

DLLEXPORT unsigned char* GetTilesCache(void)
{
	 return &bg_pattern_cache[0];
}



///////////////////////////////////////////////////////////////////////////////
// load the rom
DLLEXPORT int LoadRom(char* name)
{
	error("Loading rom %s",name);
	return load_rom(name);	
}

///////////////////////////////////////////////////////////////////////////////
// disam
DLLEXPORT unsigned int DisAsm(char* buff,unsigned int pc)
{	
	return m68k_disassemble(buff,pc, M68K_CPU_TYPE_68000);
}

///////////////////////////////////////////////////////////////////////////////
// disam
DLLEXPORT void SetCallBack(t_Callbacks* calls)
{	
	m_CallBacks=calls;
	
}

///////////////////////////////////////////////////////////////////////////////
// 
DLLEXPORT void GetVideoBufferAttributes(int *w,int *h,int *bpp,int *rgbformat)
{	
	*w=1024;
	*h=512;
	//*bpp=2;
	*bpp=2;
	*rgbformat=0;
}

///////////////////////////////////////////////////////////////////////////////
// Init
DLLEXPORT unsigned int InitEmulator(unsigned char*videobuffer,int bpp)
{	
	memset(&bitmap, 0, sizeof(t_bitmap));
    bitmap.width  = 1024;
    bitmap.height = 512;
    

	if(bpp==15)
	{	bitmap.depth  = 15;
		bitmap.granularity = 2;
	}else if(bpp==16)
	{	bitmap.depth  = 16;
		bitmap.granularity = 2;		 
	}
	else if(bpp==32)
	{	bitmap.depth  = 32;
		bitmap.granularity = 4;	
	}
    bitmap.pitch = (bitmap.width * bitmap.granularity);
    bitmap.data   = (unsigned char *)videobuffer;
    bitmap.viewport.w = 256;
    bitmap.viewport.h = 224;
    bitmap.viewport.x = 0x20;
    bitmap.viewport.y = 0x00;
    bitmap.remap = 1;

    system_init();
    system_reset();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Génére une image 

extern int render_init(void);

#define PACK_CRAM2(d)    ((((d)&0xE00)>>9)|(((d)&0x0E0)>>2)|(((d)&0x00E)<<5))
#define UNPACK_CRAM2(d)  ((((d)&0x1C0)>>5)|((d)&0x038)<<2|(((d)&0x007)<<9))

extern unsigned char tmp_buf[0x400];

DLLEXPORT int GenerateScreenshot(mdPictureInfo *info,unsigned char* pixels,mdColor *pal)
{	int i=0;
	int width = (reg[12] & 1) ? 320 : 256;
	unsigned char *old_buffer=bitmap.data;

	// attribue les nouveau
	bitmap.depth  = 8;
    bitmap.granularity = 1;
    bitmap.pitch = (bitmap.width * bitmap.granularity);
    bitmap.data   = (unsigned char *)pixels;

	// recalcule la cram 
	for(i=0;i<128;i++)
	{	unsigned short temp = *(uint16 *)&cram[i];
        temp = UNPACK_CRAM2(temp);
		temp &=0x0EEE;

		// ----bbb-ggg-rrr-
		if((i & 1)==0)
		{	pal[i/2].Blue = ((temp & 0xe00)>>9) <<5;
			pal[i/2].Green = ((temp & 0xe0)>>5) <<5;
			pal[i/2].Red = ((temp & 0xe)>>1) <<5;
		}
		color_update_8(0x00+i,temp);
        color_update_8(0x40+i,temp);
        color_update_8(0x80+i,temp);
	}
	
	for(i=0;i< frame_end;i++)
	{	
		int j=0;
		unsigned char *out;
		render_line(i);	
		out = (unsigned char *)&pixels[(i * bitmap.pitch)];		        
		//for(j=0;j<width;j++)
		//	out[j]=tmp_buf [j+bitmap.viewport.x];
	}
	
	// revient en 16 bits
	bitmap.depth  = 16;
    bitmap.granularity = 2;
    bitmap.pitch = (bitmap.width * bitmap.granularity);
    bitmap.data   = old_buffer;

	// attribue les infos
	info->BytesPerPixel=1;
	info->Height=frame_end;
	info->Pitch=width;
	info->Width=width;
 
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// run a frame
DLLEXPORT int RunFrame(int t)
{
	int i=0;
	m_GotBreakpoint=0;
	for(i=0;i<262;i++)
	{	if(system_stepcpu(0,487)==1)
		{	break;	}

		if(m_GotBreakpoint==1)
		{	return 0;
		}
	}
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// step into
DLLEXPORT int StepInto(int t)
{	
	m_GotBreakpoint=0;
	return system_stepcpu(0,t);
}

///////////////////////////////////////////////////////////////////////////////
// get a cpu register

DLLEXPORT unsigned int GetCpuRegister(int cpu_id,int reg_id)
{
	switch(reg_id)
	{
		case mdM68K_REG_A0:
		{	return m68k_get_reg(NULL, M68K_REG_A0);
		}break;

		case mdM68K_REG_A1:
		{	return m68k_get_reg(NULL, M68K_REG_A1);
		}break;

		case mdM68K_REG_A2:
		{	return m68k_get_reg(NULL, M68K_REG_A2);
		}break;

		case mdM68K_REG_A3:
		{	return m68k_get_reg(NULL, M68K_REG_A3);
		}break;

		case mdM68K_REG_A4:
		{	return m68k_get_reg(NULL, M68K_REG_A4);
		}break;

		case mdM68K_REG_A5:
		{	return m68k_get_reg(NULL, M68K_REG_A5);
		}break;

		case mdM68K_REG_A6:
		{	return m68k_get_reg(NULL, M68K_REG_A6);
		}break;

		case mdM68K_REG_A7:
		{	return m68k_get_reg(NULL, M68K_REG_A7);
		}break;

		case mdM68K_REG_SR:
		{	return m68k_get_reg(NULL, M68K_REG_SR);
		}break;

		case mdM68K_REG_PC:
		{	return m68k_get_reg(NULL, M68K_REG_PC);
		}break;

		case mdM68K_REG_D0:
		{	return m68k_get_reg(NULL, M68K_REG_D0);
		}break;

		case mdM68K_REG_D1:
		{	return m68k_get_reg(NULL, M68K_REG_D1);
		}break;

		case mdM68K_REG_D2:
		{	return m68k_get_reg(NULL, M68K_REG_D2);
		}break;

		case mdM68K_REG_D3:
		{	return m68k_get_reg(NULL, M68K_REG_D3);
		}break;

		case mdM68K_REG_D4:
		{	return m68k_get_reg(NULL, M68K_REG_D4);
		}break;

		case mdM68K_REG_D5:
		{	return m68k_get_reg(NULL, M68K_REG_D5);
		}break;

		case mdM68K_REG_D6:
		{	return m68k_get_reg(NULL, M68K_REG_D6);
		}break;

		case mdM68K_REG_D7:
		{	return m68k_get_reg(NULL, M68K_REG_D7);
		}break;


	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Get Rom info
DLLEXPORT void GetRomInfo(t_RomInfo *info)
{
	info->size=rom_size;
	strcpy(info->Name,rom_filename);

}

///////////////////////////////////////////////////////////////////////////////
// ferme l'emu

DLLEXPORT void CloseEmulator(void)
{
	gen_shutdown();
    vdp_shutdown();
    render_shutdown();
}

DLLEXPORT unsigned int m68kReadMemory8(unsigned int address)
{
	return m68k_read_memory_8(address);
}

DLLEXPORT unsigned int m68kReadMemory16(unsigned int address)
{
	return m68k_read_memory_16(address);
}

DLLEXPORT unsigned int m68kReadMemory32(unsigned int address)
{
	return m68k_read_memory_32(address);
}




