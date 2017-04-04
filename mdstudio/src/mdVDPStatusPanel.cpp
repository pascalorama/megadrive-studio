///////////////////////////////////////////////////////////////////////////////
//
//
//
// 
///////////////////////////////////////////////////////////////////////////////
// Icludes 
#include "include/mdShared.h"

#include "include/mdEmu.h"
//#include "include/mdVDPStatusPanel.h"
#include "include/mdShared.h"
#include "include/mdVDPStatusPanel.h"

#include "include/mdfrmmain.h"
#include "include/mdApp.h"

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(mdVDPStatus, wxPanel)
//		EVT_PAINT(mdVDPStatus::OnPaint) 
		EVT_ERASE_BACKGROUND(mdVDPStatus::OnEraseBackground)    
		//EVT_LISTBOX_DCLICK(100, func) 
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

mdVDPStatus::mdVDPStatus(	wxWindow* parent,
							wxWindowID id,									
							const wxPoint& pos,
							const wxSize& size,
							long style,
							const wxString& name)
							:wxPanel(parent,id,pos,size,style,name)
{

	wxBoxSizer *sizerv = new wxBoxSizer(wxVERTICAL);
	
	// ajoute la listbox
	m_RegsList=new wxListCtrl(	this,
								100,
								wxDefaultPosition,
								wxSize(320,200),
								wxLC_REPORT | wxLC_SINGLE_SEL );

	sizerv->Add(m_RegsList,1,wxEXPAND ,0);	

	// init de la list box
	wxListItem itemCol;

	itemCol.SetText(_T("N°"));
	itemCol.SetWidth(50);
    m_RegsList->InsertColumn(0, itemCol);

    itemCol.SetWidth(50);
    itemCol.SetText(_T("Value"));
    m_RegsList->InsertColumn(1, itemCol);

	itemCol.SetText(_T("Description"));
	itemCol.SetWidth(350);
    m_RegsList->InsertColumn(2, itemCol);

	// finish panel layout
	SetAutoLayout(TRUE);
    SetSizer(sizerv);

    sizerv->SetSizeHints(this);
    sizerv->Fit(this);

	// init
	wxString b;
	for(int i=0;i<24;i++)
	{	m_RegsList->InsertItem(i,b.Format("#%.2x",i));
		m_RegsList->SetItem(i, 1,b.Format("0x%.2x",0x0));
		m_RegsList->SetItem(i, 2,_T(""));
		prev_Regs[i]=0xff;
	}
	this->Update();
	
}

///////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////

mdVDPStatus::~mdVDPStatus()
{
}

void mdVDPStatus::OnEraseBackground(wxEraseEvent& event)
{
}

void mdVDPStatus::Reset(void)
{
	unsigned char *Reg=gEmu->VDPRegs();
	for(int i=0;i<24;i++)
	{	prev_Regs[i]=0;
		Reg[i]=0;
	}
	Update();
}

///////////////////////////////////////////////////////////////////////////////

void mdVDPStatus::Update(void)
{
	//m_SpiesList->SetItem(i, 2,b.Format("0x%.2x",0));
	unsigned char *Reg=gEmu->VDPRegs();
	wxString b;

	for(int ii=0;ii<24;ii++)
	{	
		//if(m_RegsList->GetItemData
		if(prev_Regs[ii]!=Reg[ii])
		{	unsigned char v=Reg[ii];
			m_RegsList->SetItem(ii,1,b.Format("0x%.2x",Reg[ii]));
			prev_Regs[ii]=v;

			b=_T("");
			switch(ii)
			{
				case 0:
				{
					if(v&0x1)
						b+="[Display Off]";
					
					if(v&0x2)
						b+="[HCount latch]";

					if(v&0x4)
						b+="[Pal. select]";

					if(v&0x10)
						b+="[HBlank On]";

					m_RegsList->SetItem(ii,2,b);
					

				}break;

				case 1:
				{
					if(v&0x4)
						b+="[Genesis VDP]";

					if(v&0x10)
						b+="[DMA On]";

					if(v&0x20)
						b+="[VBlank On]";

					if(v&0x40)
						b+="[Display On]";

					m_RegsList->SetItem(ii,2,b);

				}break;

				case 2:
				{
					int xx=0;
					int m= v & 0x38;
					xx=(int)(m<<10);
					
					b=b.Format("Plane A   $%.4x",xx);

					m_RegsList->SetItem(ii,2,b);

				}break;

				case 3:
				{
					int xx=0;
					int m= v & 0x3e;
					xx=(int)(m<<10);    
					b=b.Format("Window  $%.4x",xx);	

					m_RegsList->SetItem(ii,2,b);
				
				}break;

				case 4:
				{
					int xx=0;
					int m= v & 0x07;
					xx=(int)(m<<13);
					b=b.Format("Plane B   $%.4x",xx);

					m_RegsList->SetItem(ii,2,b);
								
				}break;

				case 5:
				{	int xx=0;
					int m= v & 0x7f;
					xx=(int)(m<<9);
					b=b.Format("Sprite     $%.4x",xx);

					m_RegsList->SetItem(ii,2,b);
				
				}break;

				case 7:
				{
					int p= v & 0x0f;
					int c= (v & 0x30)>>4;        	
					b=_T("BackColor Pal[" + b.Format("%d",c)  + "][" + b.Format("%d",p) +"]");
				
					m_RegsList->SetItem(ii,2,b);

				}break;

				case 10:
				{
					b=b.Format("HCount %d",v);

					m_RegsList->SetItem(ii,2,b);
								
				}break;

				case 11:
				{
					int l=v & 1;
      				int h=(v & 2)>>1;	
					int v=(v & 4)>>2;

					b=_T("[VScroll: ");
					if(v==1)
						b+=_T("2 Cells");
					else
						b+=_T("Full screen");
					b+=_T("][");

					b+=_T("\nHScroll: ");
					if(h==0 && l==0)
					{	b+=_T("Full screen");}

					if(h==0 && l==1)
					{	b+=_T("Line");}

					if(h==1 && l==0)
					{	b+=_T("Cell");}

					if(h==1 && l==1)
					{	b+=_T("Line");}
					b+=_T("]");

					m_RegsList->SetItem(ii,2,b);
				
				}break;

				case 12:
				{
					int l0=(v & 2)>>1;
					int l1=(v & 4)>>2;
					int r0=(v & 0x80)>>7;
					int r1=(v & 1);

					
					if(l0==0 && l1==0)
						b+=_T("[No interlace]");

					if(l0==0 && l1==1)
						b+=_T("[Interlace Normal res.]");

					if(l0==1 && l1==0)
						b+=_T("[No interlace]");

					if(l0==1 && l1==1)
						b+=_T("[Interlace 2x res.]");

					// res
					b+=_T("[Screen ");

					if(r0==0 && r1==0)
						b+=_T("32 cells");

					if(r0==0 && r1==1)
						b+=_T("40 cells (not recommended)");

					if(r0==1 && r1==0)
						b+=_T("Invalid");

					if(r0==1 && r1==1)
						b+=_T("40 cells");
					b+=_T("]");

					m_RegsList->SetItem(ii,2,b);				
				
				}break;

				case 13:
				{	int addr= (v & 0x3f)<<10;              
					b=b.Format("HScroll    $%.4x",addr);	
					m_RegsList->SetItem(ii,2,b);
				
				}break;

				case 15:
				{
					b=b.Format("Auto increment by %d bytes",v);	
					m_RegsList->SetItem(ii,2,b);
				
				}break;

				case 16:
				{
					int h0=v & 1;
					int h1=(v & 2)>>1;
					int v0=(v & 0x10)>>4;
					int v1=(v & 0x20)>>5;

					b=_T("[Tables W. ");
					if(h0==0 && h1==0)
						b+=_T("32 cells");

					if(h0==0 && h1==1)
						b+=_T("Invalid");

					if(h0==1 && h1==0)
						b+=_T("64 cells");

					if(h0==1 && h1==1)
						b+=_T("128 cells");
					b+=_T("][");

					b+=_T("Tables H. ");

					if(v0==0 && v1==0)
						b+=_T("32 cells");

					if(v0==1 && v1==0)
						b+=_T("64 cells");

					if(v0==0 && v1==1)
						b+=_T("Invalid");

					if(v0==1 && v1==1)
						b+=_T("128 cells");
					b+=_T("]");
				
					m_RegsList->SetItem(ii,2,b);
				
				}break;

				case 17:
				{
					int i= ((v & 0x1f)<<1)-1;
					int r=(v & 0x80);
					wxString t;

					if(i<0)
						i=0;

					if(r==0 && i==0)
					{	b=_T("No window");
					}
					else
					{	if(r==0 && i!=0)
						{	b=_T("Window H from Cell 0 to ");
							b+=t.Format("%d",i);
						}

						if(r!=1 && r!=0)
						{	b=_T("Window H from Cell ");
							b+= t.Format("%d",i);
							b+=_T(" to end");
						}
					}
					m_RegsList->SetItem(ii,2,b);
				
				}break;

				case 18:
				{
					int i= ((v & 0x1f)<<3)-1;
					int d=(v & 0x80);
					wxString t;
					
					if(i<0) i=0;

					if(d==0 && i==0)
					{	b=_T("No window");
					}
					else
					{	if(d==0 && i!=0)
						{	b=_T("Window V from line 0 to ");
							b+= t.Format("%d",i);
											
						}

						if(d!=1 && d!=0)
						{	b=_T("Window V from line ");
							b+= t.Format("%d",i);
							b+=_T(" to end");
							
						}
					}
					m_RegsList->SetItem(ii,2,b);
				
				}break;

				case 19:
				case 20:
				case 21:
				case 22:
				case 23:
				{
					int dma_src=0;
					int dma_dst=0;
					int dma_len=0;
					wxString t;

					dma_len= (Reg[19]& 0xff) | (Reg[20]& 0xff)<<8;
					b=b.Format("DMA Len $%x",dma_len);
					m_RegsList->SetItem(19,2,b);

					//dma_src=(Reg[21]& 0xff) | ((Reg[22]& 0xff)<<8) | ((Reg[23] & 0x3f)<<22);
					dma_src = ((Reg[23] & 0x7F) << 17 | Reg[22] << 9 | Reg[21] << 1) & 0xFFFFFE;
					b=b.Format("DMA Src $%x",dma_src);
					m_RegsList->SetItem(21,2,b);

					int d6=(Reg[23] & 0x40)>>6;
					int d7=(Reg[23] & 0x80)>>7;
					
					b=_T("\nDMA Mode ");
					if(d7==0)
					{	b+=_T("68k->VDP");	}

					if(d7==1 && d6==0)
						b+=_T("VRAM Fill");

					if(d7==1 && d6==1)
						b+=_T("VRAM Copy");	
					m_RegsList->SetItem(23,2,b);
					
					
				
				}break;
			}

			
		}
	}
}