#ifndef __mdRegistersDisplay_H__
#define __mdRegistersDisplay_H__

///////////////////////////////////////////////////////////////////////////////
// Class definition
///////////////////////////////////////////////////////////////////////////////

class mdEmu;

class mdRegistersDisplay: public wxPanel
{
	///////////////////////////////////////////////////////////////////////////
	public:
	///////
	
	mdRegistersDisplay(	wxWindow* parent,
								wxWindowID id,									
								const wxPoint& pos,
								const wxSize& size,
								long style,
								const wxString& name);
	~mdRegistersDisplay();
	void SetCpuType(int cpu_id);
	void UpdateRegisters();

	void SetEmu(mdEmu *emu);
	unsigned int GetSR(void);	

	///////////////////////////////////////////////////////////////////////////
	protected:
	//////////	
	
	int m_Position;
	int m_CpuType;
	wxString	m_Registers[20];
	int m_NbRegs;
	mdEmu* m_Emu;
	unsigned int m_SR;

	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event);

	DECLARE_EVENT_TABLE()	
};

///////////////////////////////////////////////////////////////////////////////
#endif