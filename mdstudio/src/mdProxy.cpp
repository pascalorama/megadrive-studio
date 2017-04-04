///////////////////////////////////////////////////////////////////////////////
// Proxies functions
//
///////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include "wx/app.h"
    #include "wx/frame.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// app includes
#include "include/mdEmu.h"
#include "include/mdFrmMain.h"
#include "include/mdDibBitmap.h"
#include "include/mdShared.h"
#include "include/mdApp.h"

///////////////////////////////////////////////////////////////////////////////
//
int CheckBreakPoint(unsigned int pc)
{
	if(gEmu->ExecuteBreakPoint((int)pc)==1)
	{	gEmu->m_ReachedBreakPoint=true;
		return 1;
	}
	return 0;
}

void SetBreakPoint(unsigned int pc)
{
	gEmu->CreateBreakPoint(pc);
}

void LogMessage(int level,char* msg)
{
	gFrmMain->LogMessage(level,msg);
}

void LogChar(char msg)
{
	gFrmMain->LogChar(0,msg);
}

char* GetSymbolName(int type,unsigned int pc)
{
	switch(type)
	{
		case 0:
		{	return gProject->GetSymbolVariableName(pc);
		}	break;

		case 1:
		{	return gProject->GetSymbolLabelName(pc);
		}break;

		default:
		{	return gProject->GetSymbolName(pc);}break;
	}
	
	return NULL;

}
