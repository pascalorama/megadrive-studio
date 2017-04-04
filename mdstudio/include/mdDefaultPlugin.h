#ifndef __mdDefaultPlugin_H__
#define __mdDefaultPlugin_H__

///////////////////////////////////////////////////////////////////////////////
// Class definition
///////////////////////////////////////////////////////////////////////////////

class wxDynamicLibrary;

#include "include/mdSDK.h"

class mdDefaultPlugin
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////

	mdDefaultPlugin(const wxString& filename);
	~mdDefaultPlugin();

	wxString GetDescription(void);
	wxString GetAuthor(void);
	wxString GetContactInfo(void);
	wxString GetBuildVersion(void);
	wxString GetID(void);
	wxString GetExt(void);

	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////

	typedef char* (*fctGetDescription)(void);
	typedef char* (*fctGetAuthor)(void);
	typedef char* (*fctGetContactInfo)(void);
	typedef char* (*fctGetBuildVersion)(void);
	typedef char* (*fctGetID)(void);
	typedef char* (*fctGetExt)(void);

	wxString			m_FileName;
	wxDynamicLibrary	*m_Library;	

	void GetSymbols(void);

	fctGetDescription m_fctGetDescription;
	fctGetAuthor m_fctGetAuthor;
	fctGetContactInfo m_fctGetContactInfo;
	fctGetBuildVersion m_fctGetBuildVersion;
	fctGetID m_fctGetID;
	fctGetExt m_fctGetExt;
	
};

///////////////////////////////////////////////////////////////////////////////
#endif