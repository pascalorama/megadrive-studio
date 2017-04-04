///////////////////////////////////////////////////////////////////////////////
//
//
//
// 
///////////////////////////////////////////////////////////////////////////////
// Includes 
#include "include/mdShared.h"
#include "include/mdEmu.h"
#include "include/mdRegistersDisplay.h"

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(mdRegistersDisplay, wxPanel)
		EVT_PAINT(mdRegistersDisplay::OnPaint) 
		EVT_ERASE_BACKGROUND(mdRegistersDisplay::OnEraseBackground)    
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

mdRegistersDisplay::mdRegistersDisplay(	wxWindow* parent,
											wxWindowID id,									
											const wxPoint& pos,
											const wxSize& size,
											long style,
											const wxString& name)
											:wxPanel(parent,id,pos,size,style,name)
{

	m_Position=0;
	m_Emu=NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////

mdRegistersDisplay::~mdRegistersDisplay()
{
}

///////////////////////////////////////////////////////////////////////////////
// set the kind of processor
void mdRegistersDisplay::SetCpuType(int cpu_id)
{
	m_CpuType=cpu_id;
}

///////////////////////////////////////////////////////////////////////////////

void mdRegistersDisplay::OnEraseBackground(wxEraseEvent& event)
{
}

///////////////////////////////////////////////////////////////////////////////
//
void mdRegistersDisplay::OnPaint(wxPaintEvent& event)
{
	wxFont fnt(10,wxDEFAULT,wxNORMAL,wxNORMAL,false,"Courier New",wxFONTENCODING_SYSTEM );
	int w,h;
	wxPaintDC pdc(this);
	wxBufferedDC dc(&pdc); 

	GetClientSize(&w,&h);
	dc.SetFont(fnt);
	dc.BeginDrawing();

	// dessine le fond
	wxColour f(255,255,255);
	dc.SetPen(f);
	dc.SetBrush(wxBrush(f));
	dc.DrawRectangle(0,0,w,h);
	
	if(m_CpuType==mdM68K_CPU)
	{
		// Ax
		dc.DrawText(m_Registers[0], 5,0 *14);
		dc.DrawText(m_Registers[1], 5,1 *14);
		dc.DrawText(m_Registers[2], 5,2 *14);
		dc.DrawText(m_Registers[3], 5,3 *14);
		dc.DrawText(m_Registers[4], 5,4 *14);
		dc.DrawText(m_Registers[5], 5,5 *14);
		dc.DrawText(m_Registers[6], 5,6 *14);
		dc.DrawText(m_Registers[7], 5,7 *14);

		// Dx
		dc.DrawText(m_Registers[8], 125,0 *14);
		dc.DrawText(m_Registers[9], 125,1*14);
		dc.DrawText(m_Registers[10], 125,2 *14);
		dc.DrawText(m_Registers[11], 125,3*14);
		dc.DrawText(m_Registers[12], 125,4 *14);
		dc.DrawText(m_Registers[13], 125,5 *14);
		dc.DrawText(m_Registers[14], 125,6 *14);
		dc.DrawText(m_Registers[15], 125,7 *14);

		dc.DrawText(m_Registers[16], 5,9 *14);
		dc.DrawText(m_Registers[17], 5,10 *14);
		dc.DrawText(m_Registers[18], 5,11 *14);

		// isp
		dc.DrawText(m_Registers[19], 125,9 *14);
		
	}
	
	dc.EndDrawing();
}

void mdRegistersDisplay::SetEmu(mdEmu* emu)
{
	m_Emu=emu;
}

///////////////////////////////////////////////////////////////////////////////

unsigned int mdRegistersDisplay::GetSR(void)
{
	return m_SR;
}

void mdRegistersDisplay::UpdateRegisters(void)
{
	wxString t;
	wxString v;

	switch(m_CpuType)
	{
		case mdM68K_CPU:
		{
			m_NbRegs=1;

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_A0));
			m_Registers[0]= t.Format("a0 0x%s",v.Upper());
			
			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_A1));
			m_Registers[1]= t.Format("a1 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_A2));
			m_Registers[2]= t.Format("a2 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_A3));
			m_Registers[3]= t.Format("a3 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_A4));
			m_Registers[4]= t.Format("a4 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_A5));
			m_Registers[5]= t.Format("a5 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_A6));
			m_Registers[6]= t.Format("a6 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_A7));
			m_Registers[7]= t.Format("a7 0x%s",v.Upper());

			// Dx
			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_D0));
			m_Registers[8]= t.Format("d0 0x%s",v.Upper());
			
			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_D1));
			m_Registers[9]= t.Format("d1 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_D2));
			m_Registers[10]= t.Format("d2 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_D3));
			m_Registers[11]= t.Format("d3 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_D4));
			m_Registers[12]= t.Format("d4 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_D5));
			m_Registers[13]= t.Format("d5 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_D6));
			m_Registers[14]= t.Format("d6 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_D7));
			m_Registers[15]= t.Format("d7 0x%s",v.Upper());


			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_PC));
			m_Registers[16]= t.Format("pc 0x%s",v.Upper());

			v=v.Format("%.8x",m_Emu->GetCpuRegister(0,mdM68K_REG_SP));
			m_Registers[17]= t.Format("sp 0x%s",v.Upper());

			m_SR=m_Emu->GetCpuRegister(0,mdM68K_REG_SR);
			v=v.Format("%.8x",m_SR);
			m_Registers[18]= t.Format("sr 0x%s",v.Upper());
		
			// is
			if((m_SR & 0x2000)==0x02000)
			{	m_Registers[19]="ISP";		}
			else
			{
				if((m_SR & 0x3000)==0x3000)
				{	m_Registers[19]="MSP";		}
				else
				{
					if((m_SR & 0x2000)==0)
					{	m_Registers[19]="USP";		}
				}			
			}

		}break;
	
	}

	
}