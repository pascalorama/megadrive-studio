#include "include/mdShared.h"
#include "include/mdTextCtrl.h"
#include "include/mdVDPAddrCalc.h"
#include "include/mdFrmVDPAddrCalc.h"


BEGIN_EVENT_TABLE(mdFrmVdpAddrCalc, wxFrame)
	EVT_CLOSE(mdFrmVdpAddrCalc::OnClose)     
END_EVENT_TABLE()

mdFrmVdpAddrCalc::mdFrmVdpAddrCalc(	wxFrame *frame,
									const wxChar *title,
									int x, int y, int w, int h)
									: wxFrame(frame, wxID_ANY, title, wxPoint(x, y), wxSize(w, h),wxSTAY_ON_TOP | wxMINIMIZE_BOX | wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
	this->Centre(wxBOTH);

	m_VdpAddrCalc=new mdVdpAddrCalc(this);

	Show(true);	
}

void mdFrmVdpAddrCalc::OnClose(wxCloseEvent& event)
{
	event.Veto();
	this->Hide();

}