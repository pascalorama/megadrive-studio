#ifndef __mdSDK_H__
#define __mdSDK_H__

///////////////////////////////////////////////////////////////////////////////
// Data type
///////////////////////////////////////////////////////////////////////////////

/*typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long int uint32;

typedef signed char int8;
typedef signed short int int16;
typedef signed long int int32;
*/
///////////////////////////////////////////////////////////////////////////////
// define

#define mdERR_UNKNOWN	0
#define	mdOK			1
#define mdKO			0

#define mdERR_OUTMEMORY	50
#define	mdERR_OPENINGFILE		51
#define	mdERR_SAVINGFILE		52
#define	mdERR_BADBPP			53
#define mdERR_UNSUPPORTED		54
#define mdERR_FILEUNSUPPORTED	55
#define mdERR_COPYDATA			56
#define mdERR_NOOUTPUT			57
#define mdERR_NOINPUT			58
#define mdERR_COMPRESSION		59

#define mdMAX_COLORS		256
#define DLLEXPORT	extern "C" __declspec(dllexport)

typedef int (*fctCheckBreakPoint)(unsigned int pc);
typedef void (*fctSetBreakPoint)(unsigned int pc);
typedef void (*fctLogMessage)(int src,char *msg);
typedef void (*fctLogChar)(char msg);
typedef char* (*fctGetSymbolName)(int type,unsigned int pc);

///////////////////////////////////////////////////////////////////////////////
// Structures

typedef struct{
	char	Name[256];
	int		size;
}t_RomInfo;

typedef struct{
	fctCheckBreakPoint	m_fctCheckBreakPoint;
	fctSetBreakPoint	m_fctSetBreakPoint;
	fctLogMessage		m_fctLogMessage;
	fctLogChar			m_fctLogChar;
	fctGetSymbolName	m_fctGetSymbolName;

}t_Callbacks;

typedef struct
{
    //unsigned char dev[8];     /* Can be any of the DEVICE_* values */
    //unsigned long pad[8];    /* Can be any of the INPUT_* bitmasks */
    //unsigned long system;    /* Can be any of the SYSTEM_* bitmasks */
	unsigned char   dev[8];     /* Can be any of the DEVICE_* values */
  unsigned long  pad[8];     /* Can be any of the INPUT_* bitmasks */
  unsigned char   padtype[8]; /* 3BUTTONS or 6BUTTONS gamepad */
  unsigned char   system[2];            /* Can be any of the SYSTEM_* bitmasks */
  unsigned char   max;                  /* maximum number of connected devices */
  unsigned char   current;              /* current PAD number (4WAYPLAY) */
  int     analog[3][2];         /* analog devices */
  int     x_offset;
  int     y_offset;
}t_mdinput;

/*
typedef struct{
	unsigned char *ram,
	unsigned char *rom,
	unsigned char *vram,
	unsigned char *vsram,
	unsigned char *cram,
	unsigned char *vdp_reg[20]
}t_Memory;
*/
typedef struct
{	int Width;
	int Height;
	int	BytesPerPixel;
	int Pitch;	
}mdPictureInfo;

typedef struct
{	unsigned char	Alpha;
	unsigned char	Red,Green,Blue;		
}mdColor;

#define mdOutputType_Raw		0
#define mdOutputType_Picture	1

///////////////////////////////////////////////////////////////////////////////
// Registres

#define mdM68K_CPU		0
#define mdM68K_REG_PC	0
#define mdM68K_REG_D0	1
#define mdM68K_REG_D1	2
#define mdM68K_REG_D2	3
#define mdM68K_REG_D3	4
#define mdM68K_REG_D4	5
#define mdM68K_REG_D5	6
#define mdM68K_REG_D6	7
#define mdM68K_REG_D7	8
#define mdM68K_REG_A0	9
#define mdM68K_REG_A1	10
#define mdM68K_REG_A2	11
#define mdM68K_REG_A3	12
#define mdM68K_REG_A4	13
#define mdM68K_REG_A5	14
#define mdM68K_REG_A6	15
#define mdM68K_REG_A7	16
#define mdM68K_REG_SR	17
#define mdM68K_REG_SP	16

#define mdINPUT_MODE      (0x00000800)
#define mdINPUT_Z         (0x00000400)
#define mdINPUT_Y         (0x00000200)
#define mdINPUT_X         (0x00000100)
#define mdINPUT_START     (0x00000080)
#define mdINPUT_C         (0x00000040)
#define mdINPUT_B         (0x00000020)
#define mdINPUT_A         (0x00000010)
#define mdINPUT_LEFT      (0x00000008)
#define mdINPUT_RIGHT     (0x00000004)
#define mdINPUT_DOWN      (0x00000002)
#define mdINPUT_UP        (0x00000001)



#endif