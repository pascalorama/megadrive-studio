#ifndef __mdProxy_H__
#define __mdProxy_H__

///////////////////////////////////////////////////////////////////////////////

extern int CheckBreakPoint(unsigned int pc);
extern void SetBreakPoint(unsigned int pc);

extern void LogMessage(int src,char* msg);
extern char* GetSymbolName(int type,unsigned int pc);

extern void LogChar(char msg);

///////////////////////////////////////////////////////////////////////////////
#endif