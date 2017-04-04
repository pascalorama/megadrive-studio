///////////////////////////////////////////////////////////////////////////////
//
//
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include "wx/app.h"
    #include "wx/frame.h"
#endif
#include <wx/filename.h>
#include <wx/stream.h>
#include <wx/wfstream.h>


///////////////////////////////////////////////////////////////////////////////
// app includes
#include "include/mdFrmMain.h"
#include "include/mdEmu.h"
#include "include/mdDibBitmap.h"
#include "include/mdProxy.h"
#include "include/mdShared.h"
#include "include/mdDefaultPlugin.h"
#include "include/mdOutputDataPlugin.h"
#include "include/mdProject.h"
#include "include/mdSpiesPanel.h"
#include "include/mdDisAsmPanel.h"
#include "include/mdApp.h"



///////////////////////////////////////////////////////////////////////////////
//
IMPLEMENT_APP(mdApp)

///////////////////////////////////////////////////////////////////////////////
// Initialize
///////////////////////////////////////////////////////////////////////////////

void substring(char *dst,char *buf,int start,int length)
{	
	int l=strlen(buf);	
	if(start+length<=l && l>0)
	{	if(length==0)
		{	length=l-start;}	
		int i=0;
		for(i=0;i<length;i++)
		{	dst[i]=buf[start+i];
		}
		dst[i]=0;
	}	
}


bool mdApp::OnInit()
{
	// calcule le chemin de l'app
	m_AppPath = wxTheApp->argv[0];
	wxChar sep;
#if defined(__WXMSW__)
    sep = '\\';
#else
    sep = '/';
#endif
	while ((m_AppPath.Last() != sep) && (m_AppPath.Length()>1)) {
		m_AppPath.RemoveLast();
	} 

	// init the video buffer
//	gEmu=new mdEmu(_T("plugins\\genesisplus.dll"));

	char Buffer[256]={""};
	char b[256]={""};
	GetModuleFileName(GetModuleHandle(NULL), Buffer, sizeof(Buffer));
	
	substring(b,Buffer,0,strlen(Buffer)-strlen("mdstudio.exe"));
	strcat(b,"plugins\\");
	
	m_OutputPlugins=new mdPluginsList<mdOutputDataPlugin>(b,_T("out_*.dll"));
	int i= m_OutputPlugins->GetCount();
	
	// vérifie si fichier license
	//char VNBuffer[1024];
 //   char SNBuffer[1024];
 //   char randBuffer[256];
 //   DWORD SerNo=0;
 //   DWORD SYSFlags=0;
 //   DWORD MCL=0;
 //   BOOL f2;
 //   f2 = GetVolumeInformation("c:\\", VNBuffer, 1024, &SerNo, &MCL, &SYSFlags, SNBuffer, 1024);
 //   if(SerNo==0)
 //   {	wxLogError("No license granted!");
	//	return false;
	//}
	//
	//SerNo=0xc41fe00c;
	//
	//srand((unsigned)time( NULL ) );
	//int dd=rand();
	//				
	//// encrypte 
	//char Table1[1024] ={"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,0123456789&%*/\<>-_"};
	//char Table2[1024] ={"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_-5236197048<>/\&%*.,"};
	////char Table2[1024] ={"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_-ZIGRDVMAQS<>/\&%*.,"};
	//
	//char SerNoString[256];
	//char OutSerial[256];
	//sprintf(SerNoString,"%x",SerNo);
	//int cnt=0;
	//for(int i=0;i<strlen(SerNoString);i++)
	//{	char c=SerNoString[i];
	//	int idx=0;
	//					
	//	// retrouves le char
	//	for(int j=0;j<strlen(Table1);j++)
	//	{	if(Table1[j]==c)
	//		{	idx=j;
	//			break;
	//		}
	//	}				
	//	char cc=Table2[j];
	//	OutSerial[cnt]=cc;
	//	cnt++;				
	//}
	//OutSerial[cnt]='0';
	//
	//sprintf(randBuffer,"%.5d%.8s%.5d",dd,OutSerial,rand());
	//		
	//FILE *f=NULL;
	//char key_filename[256]={""};
	//	
	//strcat(key_filename,b);
	//strcat(key_filename,"license.key");
	//
	//f=fopen(key_filename,"rt");
	//if(f==NULL)
	//{	wxLogError("MDStudio not registered!\nthanks to send e-mail to pastorama@hotmail.com with the key (respect case):\n%s",SerNoString);
	//	return false;
	//}
	//char rbuf[2048];
	//if(fread(rbuf,1,1024,f)==0)	
	//{	wxLogError("MDStudio not registered!\nthanks to send e-mail to pastorama@hotmail.com with the key (respect case):\n%s",SerNoString);
	//	return false;	
	//}
	//
	//fclose(f);
	//	
	//if(strncmp(&rbuf[5],OutSerial,8))	
	//{
	//	wxLogError("Wrong licensed!!!\nthanks to send e-mail to pastorama@hotmail.com with the key (respect case):\n%s",SerNoString);
	//	return false;	
	//}
	//
	
	
	
		
	//** ouvre la fenetre
	m_FrmMain = new mdFrmMain(	(wxFrame *)NULL, -1, _T(APP_TITLE),
								wxDefaultPosition, wxDefaultSize,
								wxDEFAULT_FRAME_STYLE | wxMINIMIZE |
								wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN );							
	
	m_FrmMain->Init();
	// initialisation de directinput
	HINSTANCE hinst=(HINSTANCE)wxGetInstance(); 

	// show main window
	m_FrmMain->Show(TRUE);
	SetTopWindow(m_FrmMain);

	if(this->argc==2)
	{	m_FrmMain->OnLoadRom(argv[1],false);	}
		    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Exit
///////////////////////////////////////////////////////////////////////////////

int mdApp::OnExit()
{	
	m_Project.ClearAllBreakPoints();
	m_Project.ClearAllSpies();
	
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Load Rom
///////////////////////////////////////////////////////////////////////////////

bool mdApp::ReloadRom(void)
{

	if(m_RomFileName.Length()>0)
	{
		if(gEmu->LoadRom(m_RomFileName)==0)
		{
			wxLogError("Unable to load %s",m_RomFileName);
			m_RomFileName="";
		}	
		else
		{
			wxString title;
			title = APP_TITLE;
			title +="- ";
			title+= m_RomFileName;

			m_FrmMain->GetSpiesPanel()->Clear();

			// génère le fichier map
			wxFileName *f=new wxFileName(m_RomFileName);
			wxString map_file="";
			
			// charge les symboles
			map_file =f->GetPath() +_T("\\") + f->GetName() +_T(".map");
			wxFileName *m=new wxFileName(map_file);
			if(m->FileExists()==true)
			{	m_FrmMain->GetDisAsmPanel()->m_PageTopPC=gEmu->m_PreviousPC;
				m_Project.LoadSymbols(0,map_file);
			}
			else
				m_Project.ClearAllSymbols();
			




			delete(f);
			delete(m);

			m_FrmMain->SetStatusText(" Rom loaded");
			m_FrmMain->SetTitle(title); 
			
		}
	}
	return true;
}

bool mdApp::LoadRom(wxChar* filename)
{
		//::wxGetInstance();

		if(gEmu->LoadRom(filename)==0)
		{
			wxLogError("Unable to load %s",filename);
			m_RomFileName="";
		}	
		else
		{
			m_FrmMain->GetSpiesPanel()->Clear();

			wxFileName *f=new wxFileName(filename);

			wxString title;
			title = APP_TITLE;
			title +="- ";
			title+= f->GetName()+"." + f->GetExt();
			
			// génère le fichier map
			wxString map_file="";
			map_file =f->GetPath() +_T("\\") + f->GetName() +_T(".map");			
			wxFileName *m=new wxFileName(map_file);
			if(m->FileExists()==true)
			{	m_FrmMain->GetDisAsmPanel()->m_PageTopPC=gEmu->m_PreviousPC;
				m_Project.LoadSymbols(0,map_file);
			}
			else
				m_Project.ClearAllSymbols();

			
			



			m_FrmMain->SetStatusText(" Rom loaded");
			m_RomFileName=filename;
			m_FrmMain->SetTitle(title); 
			
			
			delete(m);
			delete(f);
		}
	return true;
}

bool mdApp::LoadRom(void)
{	
	wxFileDialog	*fd= new wxFileDialog(	NULL,
											_T("Choose a Megadrive/Genesis rom file"),
											"",
											"",
											"Binary or SuperMagicDrive (*.bin;*.smd)|*.bin;*.smd",
											wxOPEN | wxCHANGE_DIR );

	if(fd->ShowModal()==wxID_OK)
	{
		if(gEmu->LoadRom(fd->GetPath())==0)
		{
			wxLogError("Unable to load %s",fd->GetFilename());
			m_RomFileName="";
		}	
		else
		{
			m_FrmMain->GetSpiesPanel()->Clear();

			wxString title;
			title = APP_TITLE;
			title +="- ";
			title+= fd->GetFilename();

			// génère le fichier map
			wxFileName *f=new wxFileName(fd->GetFilename());
			wxString map_file="";
			wxString dir=fd->GetDirectory();
			map_file =dir +"\\" + f->GetName() +_T(".map");
			
			wxFileName *m=new wxFileName(map_file);
			if(m->FileExists()==true)
			{	m_FrmMain->GetDisAsmPanel()->m_PageTopPC=gEmu->m_PreviousPC;
				m_Project.LoadSymbols(0,map_file);
			}
			else
				m_Project.ClearAllSymbols();

			

			// charge le fichier listing
			wxString lst_file="";
			lst_file =dir +_T("\\") + f->GetName() +_T(".lst");			
			wxFileName *l=new wxFileName(lst_file);
			if(l->FileExists()==true)
			{	m_Project.LoadList(lst_file);
			}
			delete(l);

			delete(f);
			delete(m);

			m_FrmMain->SetStatusText(" Rom loaded");
			m_RomFileName= fd->GetDirectory() + _T("\\")+ fd->GetFilename();
			m_FrmMain->SetTitle(title); 
			
		}
	;}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Load Rom
///////////////////////////////////////////////////////////////////////////////

int mdApp::RunFrame(void)
{
	return gEmu->RunFrame();
}

///////////////////////////////////////////////////////////////////////////////
// GetVideo Buffer
///////////////////////////////////////////////////////////////////////////////

mdDibBitmap* mdApp::GetEmuVideoBuffer(void)
{
	return gEmu->GetBitmap();
}

///////////////////////////////////////////////////////////////////////////////
// Save a picture
///////////////////////////////////////////////////////////////////////////////

int mdApp::OutputPicture(wxString& filename)
{
	//int GenerateScreenshot(mdPictureInfo *info,unsigned char* pixels,mdColor *pal

	mdPictureInfo info;
	mdColor pal[256];
	unsigned char *pixels=NULL;
	unsigned char *screen=NULL;

	info.BytesPerPixel=1;
	info.Height=512;
	info.Width=1024;
	info.Pitch=1024;

	pixels=(unsigned char*)malloc(10240*512);
	//if(!pixels)

	memset(pixels,128,1024*512);

	for(int i=0;i<64;i++)
	{	pal[i].Blue=i;
		pal[i].Green=i;
		pal[i].Red=i;
	}

	// rechercher du bon plugin
	mdOutputDataPlugin *p=(mdOutputDataPlugin*)m_OutputPlugins->Item(0);
	gEmu->GenerateScreenshot(&info,pixels,pal);

	// reformat the picture
	screen=(unsigned char*) malloc(info.Width * info.Height);
	//if(!screen)

	for(int j=0;j<info.Height;j++)
	{	for(int i=0;i<info.Width;i++)
		{	screen[(j*info.Width) + i]=pixels[(j*1024)+i];
		}
	}
	
	p->OutputPicture(filename,&info,screen,pal);

	free(pixels);
	free(screen);

	return 0;		
}

mdProject* mdApp::GetProject(void)
{	return &m_Project;}

int mdApp::DumpMemoryToFile(wxString& FileName,unsigned char* Data,int base,int length,bool SwapByte)
{
	int ret=0;
	wxFileName *f=new wxFileName(FileName);
	wxString b;
	wxFileOutputStream fd(f->GetFullPath());
	if(fd.Ok())
	{	ret=1;
		
		if(SwapByte==true)
		{
			if( (length & 1)==0)
			{
				unsigned char *mem=(unsigned char*)malloc(length);
				int i;
				for(i = 0; i < length; i += 2)
				{	mem[i+0] = Data[base +i+1];
					mem[i+1] = Data[base +i+0];
				}
				fd.Write(mem,length);
				if(fd.LastWrite()==0)
				ret=0;

				free(mem);
			}
			else
			{	wxLogError("Address or length not aligned, unable to swap bytes");
				ret=0;
			}
		}
		else
		{
			fd.Write(&Data[base],length);
			if(fd.LastWrite()==0)
				ret=0;
		}
		
	}
	
	if(ret==0)
		wxLogError(b.Format("Unable to write to %s",FileName));

	fd.Close();

	return ret;
}