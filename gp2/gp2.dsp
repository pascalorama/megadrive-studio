# Microsoft Developer Studio Project File - Name="gp2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=gp2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gp2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gp2.mak" CFG="gp2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gp2 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "gp2 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gp2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GP2_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GP2_EXPORTS" /D INLINE=__inline /D __inline__=__inline /D "LSB_FIRST" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x80c /d "NDEBUG"
# ADD RSC /l 0x80c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "gp2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GP2_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GP2_EXPORTS" /D INLINE=__inline /D __inline__=__inline /D "LSB_FIRST" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x80c /d "_DEBUG"
# ADD RSC /l 0x80c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\sources\mdstudio\plugins\genesisplus.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "gp2 - Win32 Release"
# Name "gp2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "emu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\emu\fileio.c
# End Source File
# Begin Source File

SOURCE=.\emu\genesis.c
# End Source File
# Begin Source File

SOURCE=.\emu\io.c
# End Source File
# Begin Source File

SOURCE=.\emu\loadrom.c

!IF  "$(CFG)" == "gp2 - Win32 Release"

!ELSEIF  "$(CFG)" == "gp2 - Win32 Debug"

# ADD CPP /MTd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\emu\mem68k.c
# End Source File
# Begin Source File

SOURCE=.\emu\membnk.c
# End Source File
# Begin Source File

SOURCE=.\emu\memvdp.c
# End Source File
# Begin Source File

SOURCE=.\emu\memz80.c
# End Source File
# Begin Source File

SOURCE=.\emu\render.c
# End Source File
# Begin Source File

SOURCE=.\emu\system.c
# End Source File
# Begin Source File

SOURCE=.\emu\vdp.c
# End Source File
# End Group
# Begin Group "m68k"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\emu\m68k\m68kcpu.c
# End Source File
# Begin Source File

SOURCE=.\emu\m68k\m68kdasm.c
# End Source File
# Begin Source File

SOURCE=.\emu\m68k\m68kopac.c
# End Source File
# Begin Source File

SOURCE=.\emu\m68k\m68kopdm.c
# End Source File
# Begin Source File

SOURCE=.\emu\m68k\m68kopnz.c
# End Source File
# Begin Source File

SOURCE=.\emu\m68k\m68kops.c
# End Source File
# End Group
# Begin Group "sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\emu\sound\fm.c
# End Source File
# Begin Source File

SOURCE=.\emu\sound\sn76496.c
# End Source File
# Begin Source File

SOURCE=.\emu\sound\sound.c
# End Source File
# End Group
# Begin Group "cpu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\emu\cpu\z80.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\emu\win\error.c
# End Source File
# Begin Source File

SOURCE=.\main.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\emu\cpu\cpuintrf.h
# End Source File
# Begin Source File

SOURCE=.\emu\win\error.h
# End Source File
# Begin Source File

SOURCE=.\emu\fileio.h
# End Source File
# Begin Source File

SOURCE=.\emu\sound\fm.h
# End Source File
# Begin Source File

SOURCE=.\emu\genesis.h
# End Source File
# Begin Source File

SOURCE=.\emu\hcnt.h
# End Source File
# Begin Source File

SOURCE=.\emu\hvc.h
# End Source File
# Begin Source File

SOURCE=.\emu\io.h
# End Source File
# Begin Source File

SOURCE=.\emu\loadrom.h
# End Source File
# Begin Source File

SOURCE=.\emu\m68k\m68kconf.h
# End Source File
# Begin Source File

SOURCE=.\emu\m68k\m68kcpu.h
# End Source File
# Begin Source File

SOURCE=.\emu\m68k\m68kops.h
# End Source File
# Begin Source File

SOURCE=.\emu\macros.h
# End Source File
# Begin Source File

SOURCE=.\emu\mem68k.h
# End Source File
# Begin Source File

SOURCE=.\emu\membnk.h
# End Source File
# Begin Source File

SOURCE=.\emu\memvdp.h
# End Source File
# Begin Source File

SOURCE=.\emu\memz80.h
# End Source File
# Begin Source File

SOURCE=.\emu\win\osd.h
# End Source File
# Begin Source File

SOURCE=.\emu\cpu\osd_cpu.h
# End Source File
# Begin Source File

SOURCE=.\emu\render.h
# End Source File
# Begin Source File

SOURCE=.\emu\shared.h
# End Source File
# Begin Source File

SOURCE=.\emu\sound\sn76496.h
# End Source File
# Begin Source File

SOURCE=.\emu\sound\sound.h
# End Source File
# Begin Source File

SOURCE=.\emu\system.h
# End Source File
# Begin Source File

SOURCE=.\emu\types.h
# End Source File
# Begin Source File

SOURCE=.\emu\vcnt.h
# End Source File
# Begin Source File

SOURCE=.\emu\vdp.h
# End Source File
# Begin Source File

SOURCE=.\emu\cpu\z80.h
# End Source File
# Begin Source File

SOURCE=.\emu\cpu\z80daa.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
