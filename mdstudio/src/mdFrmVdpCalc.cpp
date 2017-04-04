#include "include/mdShared.h"
#include "include/mdTextCtrl.h"
#include "include/mdVdpViewer.h"
#include "include/mdFrmVdpCalc.h"


BEGIN_EVENT_TABLE(mdFrmVdpCalc, wxFrame)
	EVT_CLOSE(mdFrmVdpCalc::OnClose)     
END_EVENT_TABLE()

mdFrmVdpCalc::mdFrmVdpCalc(	wxFrame *frame,
							const wxChar *title,
							int x, int y, int w, int h)
							: wxFrame(frame, wxID_ANY, title, wxPoint(x, y), wxSize(w, h),wxSTAY_ON_TOP | wxMINIMIZE_BOX | wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxCLIP_CHILDREN)
{

	m_Viewer= new mdVdpViewer(this);
	

	this->Centre(wxBOTH);
	Show(true);	
	
}

void mdFrmVdpCalc::OnClose(wxCloseEvent& event)
{
	event.Veto();
	this->Hide();

	
}
