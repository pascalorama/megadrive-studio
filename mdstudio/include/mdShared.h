#ifndef __MDSHARED_H__
#define __MDSHARED_H__

//#include "windows.h"

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include "wx/app.h"
    #include "wx/frame.h"
	#include "wx/menu.h"
#endif

#include "wx/layoutmanager.h"
#include "wx/dockwindow.h"
#include "wx/dockhost.h"
#include "wx/pane.h"
#include "wx/util.h"
#include "wx/slidebar.h"
#include "wx/toolbutton.h"
#include "wx/toolbar.h"
#include "wx/dcbuffer.h"
#include "wx/dynlib.h"
#include "wx/listctrl.h"
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/stream.h>
#include <wx/wfstream.h>
/*
#include "wx/layoutmanager.h"
#include "wx/dockwindow.h"
#include "wx/dockhost.h"
#include "wx/pane.h"
#include "wx/util.h"
#include "wx/slidebar.h"
#include "wx/toolbutton.h"
#include "wx/toolbar.h"
*/


#include "mdSDK.h"



#define APP_TITLE	"Megadrive Developper Studio"

#define	gApp				wxGetApp()
#define gFrmMain			gApp.GetFrmMain()
#define gEmu				gApp.GetFrmMain()->GetEmu()
#define gProject			gApp.GetProject()
#define gVideoAdapter		gApp.GetFrmMain()->m_VideoAdapter

#endif