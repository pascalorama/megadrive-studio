#ifndef __mdProject_H__
#define __mdProject_H__

///////////////////////////////////////////////////////////////////////////////
// Class definition
///////////////////////////////////////////////////////////////////////////////

class mdSpy;
class mdBreakPoint;
class mdSymbol;
class mdSymbolPair;
class mdSection;
class mdListLine;

///////////////////////////////////////////////////////////////////////////////

WX_DECLARE_HASH_MAP(int, mdSpy*, wxIntegerHash, wxIntegerEqual, mdSpiesList );
WX_DECLARE_HASH_MAP( int, mdBreakPoint*, wxIntegerHash, wxIntegerEqual, mdBreakPointList );
WX_DECLARE_HASH_MAP( int, mdSymbolPair*, wxIntegerHash, wxIntegerEqual, mdSymbolList );

WX_DECLARE_HASH_MAP( int, mdListLine*, wxIntegerHash, wxIntegerEqual, mdListLinePC );
WX_DECLARE_HASH_MAP( int, mdListLine*, wxIntegerHash, wxIntegerEqual, mdListLinePage );

///////////////////////////////////////////////////////////////////////////////

class mdProject
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////
	mdProject();
	~mdProject();

	// Watchers
	mdSpy* InsertSpy(	int IntIndex,
					mdSection *Section,
					wxString &name,
					unsigned int address,
					int length,
					bool isfromarray);
	mdSpy* GetSpy(int IntIndex);
	void RemoveSpy(int IntIndex);
	void ClearAllSpies(void);
	int CountSpies(void);

	// BreakPoints
	mdBreakPoint* InsertBreakPoint(int IntIndex);
	mdBreakPoint*	GetBreakPoint(int pc);
	void ClearAllBreakPoints(void);
	void RemoveBreakPoint(int IntIndex);
	int CountBreakPoint(void);
	int ExecuteBreakPoint(int pc);
	bool CheckGotBreakPoint(int pc);
	
	// Symbols
	mdSymbol*	InsertSymbol(mdSection *Section_ID,wxString &Name,unsigned int Address,int Type,bool CreeParUser);
	void ClearAllSymbols(void);
	int CountSymbols(void);
	mdSymbol* GetSymbol(int type,int pc);
	char* GetSymbolLabelName(int pc);
	char* GetSymbolVariableName(int pc);

	mdSymbolPair* GetSymbolPair(int pc);
	bool CheckSymbolPairExist(int pc);

	void RemoveSymbol(int type,int pc);
    
	char*	GetSymbolName(int pc);

	int	LoadSymbols(int type,wxString& FileName);
	int	mdProject::LoadList(wxString& FileName);

	void AddDefaultSymbols(void);

	wxString DumpFile_Rom;
	wxString DumpFile_68kRam;
	wxString DumpFile_Z80Ram;
	wxString DumpFile_CRam;
	wxString DumpFile_VRam;
	wxString DumpFile_VSRam;

	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////

	mdSpiesList m_Spies;
	mdBreakPointList	m_BreakPoints;
	mdSymbolList	m_Symbols;

	mdListLinePC	m_ListLinePC;
	mdListLinePage	m_ListLinePage;



};

#endif