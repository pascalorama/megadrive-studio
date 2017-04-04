#ifndef __VDPREGINFO_H__
#define __VDPREGINFO_H__

/*		m_Name=_T("Pattern Name Table Address for Window");
		m_Index=3;			
		m_Bits[0]=_T();
		m_Bits[1]=_T();
		m_Bits[2]=_T();
		m_Bits[3]=_T();
		m_Bits[4]=_T();
		m_Bits[5]=_T();
		m_Bits[6]=_T();
		m_Bits[7]=_T();			
*/
///////////////////////////////////////////////////////////////////////////////

#define UNUSED "Unused"

class VDPRegInfo
{
	protected:
	wxString m_Name;
	wxString m_Comment;
	wxString m_Bits[8];
	unsigned char m_Value;
	int m_Index;
	wxString t;

	public:
	VDPRegInfo(){	m_Value=0;m_Index=0;}
	~VDPRegInfo(){}

	wxString GetName(void)
	{	wxString n;
		n=n.Format("%.2x: ",m_Index);
		n=n.Upper();
		n+=m_Name;				
		return n;

	}
	unsigned char GetValue(void){	return m_Value;}
	void SetValue(unsigned char v){	m_Value=v;}
	virtual wxString GetComment(void){	return m_Comment;}
	int GetIndex(void){	return m_Index;}

	wxString GetBitDescription(int idx)
	{	
		wxString n;
		n=n.Format("$%.2x: ",idx);
		n=n.Upper();
		n+=m_Bits[idx];				
		return n;				
	}

	

};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 0:

class VDPReg0:public VDPRegInfo
{
	protected:

	public:
	VDPReg0()
	{
		m_Name=_T("Mode Set Register #1");
		m_Index=0;
		
		m_Bits[0]=_T("Display disable");
		m_Bits[1]=_T("M3 (HV counter latch enable)");
		m_Bits[2]=_T("Palette select");
		m_Bits[3]=_T("1= Invalid display setting");
		m_Bits[4]=_T("IE1 (Horizontal interrupt enable)");
		m_Bits[5]=_T(UNUSED);
		m_Bits[6]=_T(UNUSED);
		m_Bits[7]=_T(UNUSED);
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 1:

class VDPReg1:public VDPRegInfo
{
	protected:

	public:
	VDPReg1()
	{
		m_Name=_T("Mode Set Register #2");
		m_Index=1;			
		m_Bits[0]=_T("?");
		m_Bits[1]=_T(UNUSED);
		m_Bits[2]=_T("SMS / Genesis display select");
		m_Bits[3]=_T("M2 (PAL / NTSC)");
		m_Bits[4]=_T("M1 (DMA Enable)");
		m_Bits[5]=_T("IE0 (Vertical Interrupt Enable)");
		m_Bits[6]=_T("DISP (Display Enable)");
		m_Bits[7]=_T("TMS9918 / Genesis display select");
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 2:

class VDPReg2:public VDPRegInfo
{
	protected:

	public:
	VDPReg2()
	{	m_Name=_T("Pattern Name Table Address for Plane A");
		m_Index=2;			
		m_Bits[0]=_T("?");
		m_Bits[1]=_T(UNUSED);
		m_Bits[2]=_T(UNUSED);
		m_Bits[3]=_T("Bit 13 name table address");
		m_Bits[4]=_T("Bit 14 name table address");
		m_Bits[5]=_T("Bit 15 name table address");
		m_Bits[6]=_T(UNUSED);
		m_Bits[7]=_T(UNUSED);			
	}

	wxString GetComment(void)
	{	
		// calcule l'addresse du plan
		int i=0;
		int m= m_Value & 0x38;
		i=(int)(m<<10);

		m_Comment="";
		m_Comment.Append("Pattern name table address for plane A: ");

		wxString n; 
		n=n.Format("$%.4x",i).Upper();
				
		return m_Comment.Append(n);	
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 3:

class VDPReg3:public VDPRegInfo
{
	protected:

	public:
	VDPReg3()
	{	m_Name=_T("Pattern Name Table Address for Window");
		m_Index=3;			
		m_Bits[0]=_T(UNUSED);
		m_Bits[1]=_T("Bit 11 Pattern Name Table Address");
		m_Bits[2]=_T("Bit 12 Pattern Name Table Address");
		m_Bits[3]=_T("Bit 13 Pattern Name Table Address");
		m_Bits[4]=_T("Bit 14 Pattern Name Table Address");
		m_Bits[5]=_T("Bit 15 Pattern Name Table Address");
		m_Bits[6]=_T(UNUSED);
		m_Bits[7]=_T(UNUSED);			
	}

	wxString GetComment(void)
	{	
		int i=0;
		int m= m_Value & 0x3e;
		i=(int)(m<<10);    
		m_Comment="";
		m_Comment.Append("Pattern Name Table Address for Window: ");
		wxString n; 
		n=n.Format("$%.4x",i).Upper();	
		return m_Comment.Append(n);	
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 4:

class VDPReg4:public VDPRegInfo
{
	protected:

	public:
	VDPReg4()
	{	
		m_Name=_T("Pattern Name Table Address for Plane B");
		m_Index=4;			
		m_Bits[0]=_T("Bit 13 Pattern Name Table Address");
		m_Bits[1]=_T("Bit 14 Pattern Name Table Address");
		m_Bits[2]=_T("Bit 15 Pattern Name Table Address");
		m_Bits[3]=_T(UNUSED);
		m_Bits[4]=_T(UNUSED);
		m_Bits[5]=_T(UNUSED);
		m_Bits[6]=_T(UNUSED);
		m_Bits[7]=_T(UNUSED);		
	}

	wxString GetComment(void)
	{	
		int i=0;
		int m= m_Value & 0x07;
		i=(int)(m<<13);
		m_Comment="";
		m_Comment.Append("Pattern Name Table Address for Scroll B: ");
		wxString n; 
		n=n.Format("$%.4x",i).Upper();	
		return m_Comment.Append(n);	
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 5:

class VDPReg5:public VDPRegInfo
{
	protected:

	public:
	VDPReg5()
	{	
		m_Name=_T("Sprite Attribute Table Base Address");
		m_Index=5;			
		m_Bits[0]=_T("Bit 09 Attribute Table Base Address");
		m_Bits[1]=_T("Bit 10 Attribute Table Base Address");
		m_Bits[2]=_T("Bit 11 Attribute Table Base Address");
		m_Bits[3]=_T("Bit 12 Attribute Table Base Address");
		m_Bits[4]=_T("Bit 13 Attribute Table Base Address");
		m_Bits[5]=_T("Bit 14 Attribute Table Base Address");
		m_Bits[6]=_T("Bit 15 Attribute Table Base Address");
		m_Bits[7]=_T(UNUSED);			
	}

	wxString GetComment(void)
	{	
		int i=0;
		int m= m_Value & 0x7f;
		i=(int)(m<<9);
		m_Comment="";
		m_Comment.Append("Sprite Attribute Table Base Address: ");
		wxString n; 
		n=n.Format("$%.4x",i).Upper();	
		m_Comment.Append(n);
		m_Comment.Append("\nIn 40-cell mode, Bit 09 is always forced to zero.");

		return m_Comment;	
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 7:

class VDPReg7:public VDPRegInfo
{
	protected:

	public:
	VDPReg7()
	{	
		m_Name=_T("Background Color");
		m_Index=7;			
		m_Bits[0]=_T("Bit 00 Color Index");
		m_Bits[1]=_T("Bit 01 Color Index");
		m_Bits[2]=_T("Bit 02 Color Index");
		m_Bits[3]=_T("Bit 03 Color Index");
		m_Bits[4]=_T("Bit 00 Palette Index");
		m_Bits[5]=_T("Bit 01 Palette Index");
		m_Bits[6]=_T(UNUSED);
		m_Bits[7]=_T(UNUSED);			
	}

	wxString GetComment(void)
	{	
		// calcule				
		int p= m_Value & 0x0f;
		int c= (m_Value & 0x30)>>4;        	
		wxString n;
        m_Comment=_T("Background color : Palettes[" + n.Format("%d",c)  + "][" + n.Format("%d",p) +"]");
		return m_Comment;	
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 10:

class VDPReg10:public VDPRegInfo
{
	protected:

	public:
	VDPReg10()
	{	
		m_Name=_T("H Interrupt Register");
		m_Index=10;			
		m_Bits[0]=_T("Bit 0 H interrupt Raster");
		m_Bits[1]=_T("Bit 1 H interrupt Raster");
		m_Bits[2]=_T("Bit 2 H interrupt Raster");
		m_Bits[3]=_T("Bit 3 H interrupt Raster");
		m_Bits[4]=_T("Bit 4 H interrupt Raster");
		m_Bits[5]=_T("Bit 5 H interrupt Raster");
		m_Bits[6]=_T("Bit 6 H interrupt Raster");
		m_Bits[7]=_T("Bit 7 H interrupt Raster");				
	}

	wxString GetComment(void)
	{	
		// calcule
		m_Comment=_T("Makes H interrupt timing by number of Raster ");
		m_Comment.Append(t.Format("%d",m_Value));
		m_Comment.Append(_T("\nH interrupt is enabled by IE=1"));
		return m_Comment;	
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 11:

class VDPReg11:public VDPRegInfo
{
	protected:

	public:
	VDPReg11()
	{	
		m_Name=_T("Mode Set Register #3");
		m_Index=11;			
		m_Bits[0]=_T("LSCR (H scroll mode)");
		m_Bits[1]=_T("HSCR (H scroll mode)");
		m_Bits[2]=_T("VSCR (2-cell column based V scrolling)");
		m_Bits[3]=_T("IE2 (enable external interrupts)");
		m_Bits[4]=_T(UNUSED);
		m_Bits[5]=_T(UNUSED);
		m_Bits[6]=_T(UNUSED);
		m_Bits[7]=_T(UNUSED);			
	}

	wxString GetComment(void)
	{	// calcule	
		int l=m_Value & 1;
      	int h=(m_Value & 2)>>1;	
		int v=(m_Value & 4)>>2;

		m_Comment=_T("V Scroll mode: ");
		if(v==1)
			m_Comment+=_T("2 Cells scroll");
		else
			m_Comment+=_T("Full screen scroll");

		m_Comment+=_T("\nH scroll mode: ");
		if(h==0 && l==0)
		{	m_Comment+=_T("Full screen scroll");}

		if(h==0 && l==1)
		{	m_Comment+=_T("Line scroll");}

		if(h==1 && l==0)
		{	m_Comment+=_T("Cell scroll");}

		if(h==1 && l==1)
		{	m_Comment+=_T("Line scroll");}

		return m_Comment;	
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 12:

class VDPReg12:public VDPRegInfo
{
	protected:

	public:
	VDPReg12()
	{	
		m_Name=_T("Mode Set Register #4");
		m_Index=12;			
		m_Bits[0]=_T("RS1 (Horizontal cell mode)");
		m_Bits[1]=_T("LSM0 (Interlace mode setting)");
		m_Bits[2]=_T("LSM1 (Interlace mode setting)");
		m_Bits[3]=_T("S/TE (Shadow & Highlight)");
		m_Bits[4]=_T(UNUSED);
		m_Bits[5]=_T(UNUSED);
		m_Bits[6]=_T(UNUSED);
		m_Bits[7]=_T("RS0 (Horizontal cell mode)");					
	}

	wxString GetComment(void)
	{	// calcule			
		int l0=(m_Value & 2)>>1;
		int l1=(m_Value & 4)>>2;
		int r0=(m_Value & 0x80)>>7;
		int r1=(m_Value & 1);


		m_Comment=_T("Interlace setting: ");

		if(l0==0 && l1==0)
			m_Comment+=_T("No interlace");

		if(l0==0 && l1==1)
			m_Comment+=_T("Interlace (Normal resolution)");

		if(l0==1 && l1==0)
			m_Comment+=_T("No interlace");

		if(l0==1 && l1==1)
			m_Comment+=_T("Interlace (Double resolution)");

		// res
		m_Comment+=_T("\nDisplay width: ");

		if(r0==0 && r1==0)
			m_Comment+=_T("32 cells");

		if(r0==0 && r1==1)
			m_Comment+=_T("40 cells (not recommended)");

		if(r0==1 && r1==0)
			m_Comment+=_T("Invalid display");

		if(r0==1 && r1==1)
			m_Comment+=_T("40 cells");

		return m_Comment;	
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 13:

class VDPReg13:public VDPRegInfo
{
	protected:

	public:
	VDPReg13()
	{	
		m_Name=_T("H Scroll Data Table Base Address");
		m_Index=13;			
		m_Bits[0]=_T("Bit 10 Data Table Base Address");
		m_Bits[1]=_T("Bit 11 Data Table Base Address");
		m_Bits[2]=_T("Bit 12 Data Table Base Address");
		m_Bits[3]=_T("Bit 13 Data Table Base Address");
		m_Bits[4]=_T("Bit 14 Data Table Base Address");
		m_Bits[5]=_T("Bit 15 Data Table Base Address");
		m_Bits[6]=_T(UNUSED);
		m_Bits[7]=_T(UNUSED);			
	}

	wxString GetComment(void)
	{	// calcule
		int addr= (m_Value & 0x3f)<<10;              
		m_Comment=_T("H Scroll Data Table Base Address: ");
		m_Comment.Append(t.Format("$%.4x",addr).Upper());	
		return m_Comment;														
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 15:

class VDPReg15:public VDPRegInfo
{
	protected:

	public:
	VDPReg15()
	{	
		m_Name=_T("Auto Increment Data");
		m_Index=15;			
		m_Bits[0]=_T("Bit 0 Increment Data");
		m_Bits[1]=_T("Bit 1 Increment Data");
		m_Bits[2]=_T("Bit 2 Increment Data");
		m_Bits[3]=_T("Bit 3 Increment Data");
		m_Bits[4]=_T("Bit 4 Increment Data");
		m_Bits[5]=_T("Bit 5 Increment Data");
		m_Bits[6]=_T("Bit 6 Increment Data");
		m_Bits[7]=_T("Bit 7 Increment Data");			
	}

	wxString GetComment(void)
	{	// calcule
		m_Comment=_T("Auto increment data: ");		
		m_Comment.Append(t.Format("$%.2x",m_Value).Upper());	
		return m_Comment;														
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 16:

class VDPReg16:public VDPRegInfo
{
	protected:

	public:
	VDPReg16()
	{	
		m_Name=_T("Scroll Size");
		m_Index=16;			
		m_Bits[0]=_T("HSZ0 (H Name table size)");
		m_Bits[1]=_T("HSZ1 (H Name table size)");
		m_Bits[2]=_T(UNUSED);
		m_Bits[3]=_T(UNUSED);
		m_Bits[4]=_T("VSZ0 (V Name table size)");
		m_Bits[5]=_T("VSZ1 (V Name table size)");
		m_Bits[6]=_T(UNUSED);
		m_Bits[7]=_T(UNUSED);				
	}

	wxString GetComment(void)
	{	// calcule
		int h0=m_Value & 1;
		int h1=(m_Value & 2)>>1;
		int v0=(m_Value & 0x10)>>4;
		int v1=(m_Value & 0x20)>>5;

		m_Comment=_T("Plane A/B H Name Table width: ");
		if(h0==0 && h1==0)
			m_Comment+=_T("32 cells");

		if(h0==0 && h1==1)
			m_Comment+=_T("Invalid");

		if(h0==1 && h1==0)
			m_Comment+=_T("64 cells");

		if(h0==1 && h1==1)
			m_Comment+=_T("128 cells");

		m_Comment+=_T("\nPlane A/B V Name Table height: ");

		if(v0==0 && v1==0)
			m_Comment+=_T("32 cells");

		if(v0==1 && v1==0)
			m_Comment+=_T("64 cells");

		if(v0==0 && v1==1)
			m_Comment+=_T("Invalid");

		if(v0==1 && v1==1)
			m_Comment+=_T("128 cells");
		return m_Comment;														
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 17:

class VDPReg17:public VDPRegInfo
{
	protected:

	public:
	VDPReg17()
	{	
		m_Name=_T("Window H Position");
		m_Index=17;			
		m_Bits[7]=_T("RIGT");
		m_Bits[6]=_T("0 (No effect)");
		m_Bits[5]=_T("0 (No effect)");
		m_Bits[4]=_T("WHP5");
		m_Bits[3]=_T("WHP4");
		m_Bits[2]=_T("WHP3");
		m_Bits[1]=_T("WHP2");
		m_Bits[0]=_T("WHP1");		
				
	}

	wxString GetComment(void)
	{	// calcule
		int i= ((m_Value & 0x1f)<<1)-1;
		int r=(m_Value & 0x80);

		if(i<0)
			i=0;

		if(r==0 && i==0)
		{	m_Comment=_T("No window");
		}
		else
		{	if(r==0 && i!=0)
			{	m_Comment=_T("Window H position from Cell 0 to ");
				m_Comment+= t.Format("%d",i);
			}

			if(r!=1 && r!=0)
			{	m_Comment=_T("Window H position from Cell ");
				m_Comment+= t.Format("%d",i);
				m_Comment+=_T(" to end");
			}
		}		
		return m_Comment;														
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 18:

class VDPReg18:public VDPRegInfo
{
	protected:

	public:
	VDPReg18()
	{	
		m_Name=_T("Window V Position");
		m_Index=17;			
		m_Bits[7]=_T("DOWN");
		m_Bits[6]=_T("0 (No effect)");
		m_Bits[5]=_T("0 (No effect)");
		m_Bits[4]=_T("WHP4");
		m_Bits[3]=_T("WHP3");
		m_Bits[2]=_T("WHP2");
		m_Bits[1]=_T("WHP1");
		m_Bits[0]=_T("WHP0");		
	}

	wxString GetComment(void)
	{	// calcule
		int i= ((m_Value & 0x1f)<<3)-1;
		int d=(m_Value & 0x80);
		
		if(i<0) i=0;

		if(d==0 && i==0)
		{	m_Comment=_T("No window");
		}
		else
		{	if(d==0 && i!=0)
			{	m_Comment=_T("Window V position from line 0 to ");
				m_Comment+= t.Format("%d",i);
								
			}

			if(d!=1 && d!=0)
			{	m_Comment=_T("Window V position from line ");
				m_Comment+= t.Format("%d",i);
				m_Comment+=_T(" to end");
				
			}
		}
		return m_Comment;
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 19:

class VDPReg19:public VDPRegInfo
{
	protected:

	public:
	VDPReg19()
	{	
		m_Name=_T("DMA Length Counter Low");
		m_Index=19;			
		m_Bits[0]=_T("LG0");
		m_Bits[1]=_T("LG1");
		m_Bits[2]=_T("LG2");
		m_Bits[3]=_T("LG3");
		m_Bits[4]=_T("LG4");
		m_Bits[5]=_T("LG5");
		m_Bits[6]=_T("LG6");
		m_Bits[7]=_T("LG7");							
	}

	wxString GetComment(void)
	{	// calcule

		m_Comment=_T("DMA length counter low: $");
		m_Comment+= t.Format("%.2x",m_Value).Upper();				


		return m_Comment;														
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 20:

class VDPReg20:public VDPRegInfo
{
	protected:

	public:
	VDPReg20()
	{	
		m_Name=_T("DMA Length Counter High");
		m_Index=20;			
		m_Bits[0]=_T("LG09");
		m_Bits[1]=_T("LG10");
		m_Bits[2]=_T("LG11");
		m_Bits[3]=_T("LG12");
		m_Bits[4]=_T("LG13");
		m_Bits[5]=_T("LG14");
		m_Bits[6]=_T("LG15");
		m_Bits[7]=_T("LG16");							
	}

	wxString GetComment(void)
	{	// calcule
		m_Comment=_T("DMA length counter high: $");
		m_Comment+= t.Format("%.2x",m_Value).Upper();		
		return m_Comment;														
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 21:

class VDPReg21:public VDPRegInfo
{
	protected:

	public:
	VDPReg21()
	{	
		m_Name=_T("DMA Source Address Low");
		m_Index=21;			
		m_Bits[0]=_T("SA00");
		m_Bits[1]=_T("SA01");
		m_Bits[2]=_T("SA02");
		m_Bits[3]=_T("SA03");
		m_Bits[4]=_T("SA04");
		m_Bits[5]=_T("SA05");
		m_Bits[6]=_T("SA06");
		m_Bits[7]=_T("SA07");							
	}

	wxString GetComment(void)
	{	// calcule
		m_Comment=_T("DMA source address low: $");
		m_Comment+= t.Format("%.2x",m_Value).Upper();		
		return m_Comment;														
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 22:

class VDPReg22:public VDPRegInfo
{
	protected:

	public:
	VDPReg22()
	{	
		m_Name=_T("DMA Source Address Mid");
		m_Index=22;			
		m_Bits[0]=_T("SA09");
		m_Bits[1]=_T("SA10");
		m_Bits[2]=_T("SA11");
		m_Bits[3]=_T("SA12");
		m_Bits[4]=_T("SA13");
		m_Bits[5]=_T("SA14");
		m_Bits[6]=_T("SA15");
		m_Bits[7]=_T("SA16");							
	}

	wxString GetComment(void)
	{	// calcule
		m_Comment=_T("DMA source address mid: $");
		m_Comment+= t.Format("%.2x",m_Value).Upper();		
		return m_Comment;														
	}
};

///////////////////////////////////////////////////////////////////////////////
// REGISTRE 23:

class VDPReg23:public VDPRegInfo
{
	protected:

	public:
	VDPReg23()
	{	
		m_Name=_T("DMA Source Address High");
		m_Index=23;			
		m_Bits[0]=_T("SA17");
		m_Bits[1]=_T("SA18");
		m_Bits[2]=_T("SA19");
		m_Bits[3]=_T("SA20");
		m_Bits[4]=_T("SA21");
		m_Bits[5]=_T("SA22");
		m_Bits[6]=_T("DMD0 (DMA Mode or SA23 in 68k->VDP)");
		m_Bits[7]=_T("DMD1 (DMA Mode)");							
	}

	wxString GetComment(void)
	{	// calcule
		int d6=(m_Value & 0x40)>>6;
		int d7=(m_Value & 0x80)>>7;
		
		m_Comment=_T("DMA source address high: $");
		
		m_Comment+= t.Format("%.2x",(m_Value & 0x3f)).Upper();

		
		m_Comment+=_T("\nDMA Mode: ");
		if(d7==0)
		{	m_Comment+=_T("68k->VDP Transfert DMD0=SA23");	}

		if(d7==1 && d6==0)
			m_Comment+=_T("VRAM Fill");

		if(d7==1 && d6==1)
			m_Comment+=_T("VRAM Copy");	
				
		return m_Comment;
	}
};


#endif