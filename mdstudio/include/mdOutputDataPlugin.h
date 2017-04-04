#ifndef __mdOutputDataPlugin_H__
#define __mdOutputDataPlugin_H__

///////////////////////////////////////////////////////////////////////////////
// Class definition
///////////////////////////////////////////////////////////////////////////////

#include "include/mdSDK.h"
class mdDefaultPlugin;

class mdOutputDataPlugin:public mdDefaultPlugin
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////

	mdOutputDataPlugin(const wxString& filename);
	~mdOutputDataPlugin();

	int GetOutputType(void);
	int OutputPicture(wxString &filename,mdPictureInfo *info,unsigned char* pixels,mdColor *pal);

	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////

	typedef int (*fctGetOutputType)(void);
	typedef int (*fctOutputPicture)(const char *filename,mdPictureInfo *info,unsigned char* pixels,mdColor *pal);

	void GetSymbols(void);

	fctGetOutputType m_fctGetOuputType;
	fctOutputPicture m_fctOutputPicture;
	
};

///////////////////////////////////////////////////////////////////////////////
#endif