#ifndef __mdFrmDumpBin_H__
#define __mdFrmDumpBin_H__

class mdTextCtrl;

///////////////////////////////////////////////////////////////////////////////
class mdFrmDumpBin : public wxDialog
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////
    mdFrmDumpBin(	wxWindow *frame, const wxChar *title);
    virtual ~mdFrmDumpBin(){};

	void SetOptions(wxString& filename,int default_base,int length,int max_base,int max_lenght);

	int GetBaseAddress();
	int GetLength();

    ///////////////////////////////////////////////////////////////////////////
	private:
	////////    
	mdTextCtrl *m_BaseAddress;
	mdTextCtrl *m_Length;

	int MaxLength;
	int MaxBase;

	void OnClose(wxCloseEvent& event);
	void OnDump(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);


	wxString *FileName;

    DECLARE_EVENT_TABLE()
};

#endif
///////////////////////////////////////////////////////////////////////////////