///////////////////////////////////////////////////////////////////////////////
//
//
//
// 
///////////////////////////////////////////////////////////////////////////////
// Includes 
#include "include/mdShared.h"
#include "include/mdBreakPoint.h"
#include "include/mdSpy.h"
#include "include/mdSymbol.h"
#include "include/mdSymbolPair.h"
#include "include/mdListLine.h"

#include "include/mdSection.h"
#include "include/mdSpiesPanel.h"
#include "include/mdFrmMain.h"
#include "include/mdApp.h"
#include "include/mdEmu.h"

#include "include/mdProject.h"

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////

mdProject::mdProject()
{
	AddDefaultSymbols();
}

mdProject::~mdProject()
{
	ClearAllSpies();
	ClearAllBreakPoints();
	ClearAllSymbols();
}

///////////////////////////////////////////////////////////////////////////////
// Gestion des symboles
///////////////////////////////////////////////////////////////////////////////

void mdProject::AddDefaultSymbols()
{
	// Ajoute des symboles par défaut
	wxString b;

	b=_T("VDP_DATA");
	InsertSymbol(NULL,b,0xc00000,0,false);

	b=_T("VDP_DATA");
	InsertSymbol(NULL,b,0xc00002,0,false);

	b=_T("VDP_CTRL");
	InsertSymbol(NULL,b,0xc00004,0,false);

	b=_T("VDP_CTRL");
	InsertSymbol(NULL,b,0xc00006,0,false);

	b=_T("HV_COUNTER");
	InsertSymbol(NULL,b,0xc00008,0,false);

	b=_T("HV_COUNTER");
	InsertSymbol(NULL,b,0xc0000a,0,false);

	b=_T("HV_COUNTER");
	InsertSymbol(NULL,b,0xc0000c,0,false);

	b=_T("HV_COUNTER");
	InsertSymbol(NULL,b,0xc0000e,0,false);

	b=_T("SN76489_PSG");
	InsertSymbol(NULL,b,0xc00011,0,false);

	b=_T("SN76489_PSG");
	InsertSymbol(NULL,b,0xc00013,0,false);

	b=_T("SN76489_PSG");
	InsertSymbol(NULL,b,0xc00015,0,false);

	b=_T("SN76489_PSG");
	InsertSymbol(NULL,b,0xc00017,0,false);

	b=_T("Z80_BUSREQ");
	InsertSymbol(NULL,b,0xa11100,0,false);

	b=_T("Z80_RESET");
	InsertSymbol(NULL,b,0xa11200,0,false);

	b=_T("MD_VERSION");
	InsertSymbol(NULL,b,0xa10001,0,false);

	b=_T("DATA_PORT_A");
	InsertSymbol(NULL,b,0xa10003,0,false);

	b=_T("DATA_PORT_B");
	InsertSymbol(NULL,b,0xa10005,0,false);

	b=_T("DATA_PORT_C");
	InsertSymbol(NULL,b,0xa10007,0,false);

	b=_T("CTRL_PORT_A");
	InsertSymbol(NULL,b,0xa10009,0,false);

	b=_T("CTRL_PORT_B");
	InsertSymbol(NULL,b,0xa1000b,0,false);

	b=_T("CTRL_PORT_C");
	InsertSymbol(NULL,b,0xa1000d,0,false);
	
	b=_T("TxDATA_PORT_A");
	InsertSymbol(NULL,b,0xa10000,0,false);

	b=_T("RxDATA_PORT_A");
	InsertSymbol(NULL,b,0xa10011,0,false);

	b=_T("S_CTRL_PORT_A");
	InsertSymbol(NULL,b,0xa10013,0,false);

	b=_T("TxDATA_PORT_B");
	InsertSymbol(NULL,b,0xa10015,0,false);

	b=_T("RxDATA_PORT_B");
	InsertSymbol(NULL,b,0xa10017,0,false);

	b=_T("S_CTRL_PORT_B");
	InsertSymbol(NULL,b,0xa10019,0,false);

	b=_T("TxDATA_PORT_C");
	InsertSymbol(NULL,b,0xa1001b,0,false);

	b=_T("RxDATA_PORT_C");
	InsertSymbol(NULL,b,0xa1001d,0,false);

	b=_T("S_CTRL_PORT_C");
	InsertSymbol(NULL,b,0xa1001f,0,false);

	DumpFile_Rom=_T("");
	DumpFile_68kRam=_T("");
	DumpFile_Z80Ram=_T("");
	DumpFile_CRam=_T("");
	DumpFile_VRam=_T("");
	DumpFile_VSRam=_T("");
}

void mdProject::ClearAllSymbols(void)
{
	mdSymbolList::iterator it;
	for( it = m_Symbols.begin(); it != m_Symbols.end(); ++it )
    {	mdSymbolPair *s=(mdSymbolPair*) it->second;
		if(s!=NULL)
		{	
			if(s->Label!=NULL)
				delete(s->Label);

			if(s->Variable!=NULL)
				delete(s->Variable);

			delete(s);
		}
    }
	m_Symbols.clear();
}

int mdProject::CountSymbols(void)
{
	return m_Symbols.size();
}

mdSymbol*	mdProject::GetSymbol(int type,int pc)
{	
	mdSymbolPair *sp=GetSymbolPair(pc);
	if(sp!=NULL)
	{	if(type==0)
			return sp->Variable;
		else
			return sp->Label;
	}
	return NULL;
}

mdSymbolPair*	mdProject::GetSymbolPair(int pc)
{	
	mdSymbolList::iterator it;
	it=m_Symbols.find(pc);
	if(it!=m_Symbols.end())
	{	return (mdSymbolPair*) it->second;
	}
	return NULL;	
}

mdSymbol* mdProject::InsertSymbol(mdSection *Section_ID,wxString &Name,unsigned int Address,int Type,bool UserCreated)
{
	mdSymbol *s=new mdSymbol();
	s->Section=Section_ID;
	s->Address=Address;
	s->Name=Name;
	s->UserCreated=UserCreated;
	
	// vérifie si on a un symbol racine
	mdSymbolPair *sp=GetSymbolPair(Address);
	if(sp==NULL)
	{	sp=new mdSymbolPair();
		if(Type==0)
			sp->Variable=s;
		else
			sp->Label=s;

		m_Symbols[Address]=sp;
	}
	else
	{	if(Type==0)
		{	if(sp->Variable!=NULL)
				delete(sp->Variable);
			sp->Variable=s;
		}
		else
		{	if(sp->Label!=NULL)
				delete(sp->Label);
			sp->Label=s;
		}
	}
	return s;
}

bool mdProject::CheckSymbolPairExist(int pc)
{
	mdBreakPointList::iterator it;
	it=m_BreakPoints.find(pc);
	if(it!=m_BreakPoints.end())
		return true;	
	
	return false;	
}

char* mdProject::GetSymbolName(int pc)
{
	mdSymbolPair *sp=NULL;

	mdSymbolList::iterator it;
	it=m_Symbols.find(pc);
	if(it!=m_Symbols.end())
	{	sp=(mdSymbolPair*) it->second;
		if(sp->Label!=NULL)
			return (char*)sp->Label->Name.c_str();
		
		if(sp->Variable!=NULL)
			return (char*)sp->Variable->Name.c_str();
	}
	return NULL;	
}

char* mdProject::GetSymbolLabelName(int pc)
{
	mdSymbolPair *sp=NULL;

	mdSymbolList::iterator it;
	it=m_Symbols.find(pc);
	if(it!=m_Symbols.end())
	{	sp=(mdSymbolPair*) it->second;
		
		if(sp->Label!=NULL)
			return (char*)sp->Label->Name.c_str();
		else
		{
			if(sp->Variable!=NULL)
				return (char*)sp->Variable->Name.c_str();
		}
		
	}
	return NULL;	
}

char* mdProject::GetSymbolVariableName(int pc)
{
	mdSymbolPair *sp=NULL;

	mdSymbolList::iterator it;
	it=m_Symbols.find(pc);
	if(it!=m_Symbols.end())
	{	sp=(mdSymbolPair*) it->second;
		
		if(sp->Variable!=NULL)
			return (char*)sp->Variable->Name.c_str();
	}
	return NULL;		
}


void mdProject::RemoveSymbol(int type,int pc)
{
	//m_Spies.erase(IntIndex);
	mdSymbolPair *sp=GetSymbolPair(pc);
	if(sp!=NULL)
	{
		if(type==0)
		{	if(sp->Variable!=NULL)
			{	if(sp->Variable->UserCreated==true)
				{	delete(sp->Variable);
					sp->Variable=NULL;
				}
			}
		}
		
		if(type==1)
		{	if(sp->Label!=NULL)
			{	delete(sp->Label);			
				sp->Label=NULL;
			}
		}

		if(sp->Label==NULL && sp->Variable==NULL)
		{	m_Symbols.erase(pc);
			delete(sp);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Gestion des watchers
///////////////////////////////////////////////////////////////////////////////
mdSpy* mdProject::InsertSpy(	int IntIndex,
								mdSection *Section,
								wxString &name,
								unsigned int address,
								int length,
								bool isfromarray)
{
	mdSpy *Spy=new mdSpy();
		
	Spy->SetName(name);
	Spy->SetAddress(address,true);
	Spy->m_Symbol->Section=Section;

	Spy->m_Length=length;
	Spy->m_ListIndex=IntIndex;
	Spy->m_IsFromArray=isfromarray;
	Spy->m_Value=0;
	m_Spies[IntIndex]=Spy;

	return Spy;
}

mdSpy* mdProject::GetSpy(int IntIndex)
{	return (mdSpy*)m_Spies[IntIndex];
}

void mdProject::RemoveSpy(int IntIndex)
{
	m_Spies.erase(IntIndex);
}

void mdProject::ClearAllSpies(void)
{
	mdSpiesList::iterator it;
	for( it = m_Spies.begin(); it != m_Spies.end(); ++it )
    {	mdSpy *s=(mdSpy*) it->second;
		if(s!=NULL)
			delete(s);
    }
	m_Spies.clear();
}

int mdProject::CountSpies(void)
{	
	return m_Spies.size();
}

///////////////////////////////////////////////////////////////////////////////
// Gestion des BerakPoints
///////////////////////////////////////////////////////////////////////////////

mdBreakPoint* mdProject::InsertBreakPoint(int IntIndex)
{
	mdBreakPointList::iterator it;
	it=m_BreakPoints.find(IntIndex);
	if(it==m_BreakPoints.end())
	{	mdBreakPoint *bk=new mdBreakPoint();
		bk->m_Hits=0;
		bk->m_Enabled=TRUE;
		m_BreakPoints[IntIndex]=bk;
		return bk;
	}
	return NULL;
}

bool mdProject::CheckGotBreakPoint(int pc)
{
	mdBreakPointList::iterator it;
	it=m_BreakPoints.find(pc);
	if(it!=m_BreakPoints.end())
		return true;	
	
	return false;
}

void mdProject::RemoveBreakPoint(int IntIndex)
{
	m_BreakPoints.erase(IntIndex);
}

int mdProject::CountBreakPoint(void)
{
	return m_BreakPoints.size();
}

void mdProject::ClearAllBreakPoints(void)
{
	mdBreakPointList::iterator it;
	for( it = m_BreakPoints.begin(); it != m_BreakPoints.end(); ++it )
    {	mdBreakPoint *s=(mdBreakPoint*) it->second;
		if(s!=NULL)
			delete(s);  
    }
	m_BreakPoints.clear();
}

mdBreakPoint*	mdProject::GetBreakPoint(int pc)
{
	return(mdBreakPoint*) m_BreakPoints[pc];	
}

int mdProject::ExecuteBreakPoint(int pc)
{
	mdBreakPointList::iterator it;
	it=m_BreakPoints.find(pc);
	if(it!=m_BreakPoints.end())
	{	// trouver breakpoint incrémente le hits
		mdBreakPoint *value =(mdBreakPoint*) it->second;
		if(value->m_Enabled==true)
		{	value->m_Hits++;
			return 1;
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Load les symboles et étiquettes
///////////////////////////////////////////////////////////////////////////////

int	mdProject::LoadSymbols(int type,wxString& FileName)
{
	// nasm pour commencer
	wxString str="";
	wxTextFile file;
	file.Open(FileName);
	int etat=-1;
	int ligne_cnt=0;
	wxString Section="";

	ClearAllSymbols();
	AddDefaultSymbols();
	
	for ( str = file.GetFirstLine(); !file.Eof(); str = file.GetNextLine() )
	{
		if(ligne_cnt==0)
		{
		
			switch(etat)
			{
				case -1:
				{
					// traite le fichier
					if(str.Contains("   Section ")==true)
					{	// doit virer
						etat=1;
						Section=str.BeforeLast('\'');
						Section=Section.AfterFirst('\'');
						ligne_cnt=5;
					}
					else
					{
						if(str.Contains("Value  Type  Class  Symbol                  Refs   Line File")==true)
						{	etat=1;
							Section=_T("");
							ligne_cnt=2;
						}
					}
					

				}break;

				case 1:
				{
					if(str.Trim().Length()==0)
						etat=-1;
					else
					{	wxString var_name="";
						wxString taille=str.SubString(12,18);
						wxString index=str.SubString(0,9);
						long value=0;
						int addr_index= index.ToLong(&value,16);
						int addresse=value;
						taille=taille.Trim();

						var_name=str.SubString(25,48);
						var_name=var_name.Trim(true);
											
						if(taille.Trim().Length()>0)
						{
							int size=0;
							if(taille.CmpNoCase("byte")==0)
							{	size=1;
							}
							if(taille.CmpNoCase("word")==0)
							{	size=2;
							}
							if(taille.CmpNoCase("long")==0)
							{	size=4;
							}

							if(size>0)
							{	
								if(Section.CmpNoCase("vars")==0)
								{	addresse = 0xff0000 + value;	}	
								
								// attribue une section
								gFrmMain->GetSpiesPanel()->InsertSpy(NULL,var_name,addresse,size,1,false);
							}
						}
						else
						{
							// étiquette
							InsertSymbol(NULL,var_name,value,1,false);		
						}
					}
				
				}break;

			}
		}
		else
		{	ligne_cnt--;	}
		
	}

	file.Close();
	return 0;
}

int	mdProject::LoadList(wxString& FileName)
{
	//// nasm pour commencer
	//wxString str="";
	//wxTextFile file;
	//file.Open(FileName);

	//int nbligne=file.GetLineCount();

	//for ( str = file.GetFirstLine(); !file.Eof(); str = file.GetNextLine() )
	//{		
	//	wxString *s=new wxString(str);
	//}

	//file.Close();

	return 0;
}


