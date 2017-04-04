///////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////
// Includes 

#include "include/mdShared.h"
#include "include/mdSpy.h"
#include "include/mdEmu.h"
#include "include/mdFrmSpyAttr.h"
#include "include/mdProject.h"
#include "include/mdDisAsmPanel.h"
#include "include/mdfrmmain.h"
#include "include/mdApp.h"
#include "include/mdSection.h";
#include "include/mdSpiesPanel.h"

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////

enum{
	Menu_Add_Spy = 300,
	Menu_Delete_Spy,
	Menu_Insert_Spy,
	Menu_Update_Spy,
	Menu_Display_Hexa
};

BEGIN_EVENT_TABLE(mdSpiesPanel, wxPanel)
	EVT_LIST_ITEM_SELECTED(100, mdSpiesPanel::SelectSpy) 
	EVT_CONTEXT_MENU(mdSpiesPanel::OnContextMenu)

	EVT_MENU(Menu_Add_Spy, mdSpiesPanel::OnAddSpy)
	EVT_MENU(Menu_Update_Spy, mdSpiesPanel::OnUpdateSpy)
	EVT_MENU(Menu_Delete_Spy, mdSpiesPanel::OnDeleteSpy)
	EVT_MENU(Menu_Insert_Spy, mdSpiesPanel::OnInsertSpy)

	EVT_MENU(Menu_Display_Hexa, mdSpiesPanel::OnDisplayHexa)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

mdSpiesPanel::mdSpiesPanel(	wxWindow* parent,
					wxWindowID id,									
					const wxPoint& pos,
					const wxSize& size,
					long style,
					const wxString& name)
					:wxPanel(parent,id,pos,size,style,name)
{
	
	wxBoxSizer *sizerv = new wxBoxSizer(wxVERTICAL);
	
	// ajoute la listbox
	m_SpiesList=new wxListCtrl(	this,
								100,
								wxDefaultPosition,
								wxSize(320,200),
								wxLC_REPORT | wxLC_SINGLE_SEL );

	sizerv->Add(m_SpiesList,1,wxEXPAND ,0);	

	// init de la list box
	wxListItem itemCol;

	itemCol.SetText(_T("Name"));
	itemCol.SetWidth(130);
    m_SpiesList->InsertColumn(0, itemCol);

    itemCol.SetWidth(75);
    itemCol.SetText(_T("Address"));
    m_SpiesList->InsertColumn(1, itemCol);

	itemCol.SetText(_T("Value"));
	itemCol.SetWidth(100);
    m_SpiesList->InsertColumn(2, itemCol);

	// init du menu popup
	m_Popup=new wxMenu();
	m_Popup->Append(Menu_Add_Spy, _T("&Add Watcher..."));
	m_Popup->Append(Menu_Insert_Spy, _T("&Insert Watcher..."));
	m_Popup->Append(Menu_Update_Spy, _T("&Update Watcher..."));
    m_Popup->Append(Menu_Delete_Spy, _T("&Delete Watcher"));

	m_Popup->AppendSeparator();
	m_Popup->Append(Menu_Display_Hexa, _T("&Display Hexadeximal"));

	// finish panel layout
	SetAutoLayout(TRUE);
    SetSizer(sizerv);

    sizerv->SetSizeHints(this);
    sizerv->Fit(this);

}

///////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////

mdSpiesPanel::~mdSpiesPanel()
{
	// supprime tous les watchers
	
}

///////////////////////////////////////////////////////////////////////////////
// Insert Item
///////////////////////////////////////////////////////////////////////////////
//void InsertSpy(int list_idx,mdSection *Section_ID,wxString &name,unsigned int address,int length,int arraycnt=1,bool isarrayelement=false);
//void InsertSpy(mdSection *Section_ID,wxString &name,unsigned int address,int length,int arraycnt=1,bool isarrayelement=false);

mdSpy* mdSpiesPanel::InsertSpy(mdSection *Section,wxString &name,unsigned int address,int length,int arraycnt,bool isfromarray)
{
	return InsertSpy(-1,Section,name,address,length,arraycnt,isfromarray);
}

mdSpy* mdSpiesPanel::InsertSpy(int list_idx,mdSection *Section,wxString &name,unsigned int address,int length,int arraycnt,bool isfromarray)
{
	wxString b;
	mdSpy *s=NULL;
	int nb=m_SpiesList->GetItemCount();
	int i=m_SpiesList->GetItemCount();
	
	if(list_idx!=-1)
		i=list_idx;

	m_SpiesList->InsertItem(i,name);
	m_SpiesList->SetItemData(i,nb);
	
	// crèe l'espion
	s=gProject->InsertSpy(nb,Section,name,address,length,isfromarray);
	s->m_Value=0;

	m_SpiesList->SetItem(i, 1,b.Format("0x%.2x",address));
	
	switch(s->m_Length)
	{	/*
		case 1:
		{	m_SpiesList->SetItem(i, 2,b.Format("0x%.2x",0));
		}break;

		case 2:
		{	m_SpiesList->SetItem(i, 2,b.Format("0x%.4x",0));
				
		}break;

		case 4:
		{	m_SpiesList->SetItem(i, 2,b.Format("0x%.8x",0));
		}break;
		*/
	
		case 1:
			{	
				if(s->m_DisplayHexa)
					m_SpiesList->SetItem(i, 2,b.Format("0x%.2x",0));
				else
					m_SpiesList->SetItem(i, 2,b.Format("%.10d",0));
			}break;

			case 2:
			{	if(s->m_DisplayHexa)
					m_SpiesList->SetItem(i, 2,b.Format("0x%.4x",0));			
				else
					m_SpiesList->SetItem(i, 2,b.Format("%.10d",0));			
			}break;

			case 4:
			{	if(s->m_DisplayHexa)
					m_SpiesList->SetItem(i, 2,b.Format("0x%.8x",0));
				else
					m_SpiesList->SetItem(i, 2,b.Format("%.10d",0));
			}break;

	}
	
	gFrmMain->UpdateDisAsm();

	return s;
}

///////////////////////////////////////////////////////////////////////////////
// context menu
///////////////////////////////////////////////////////////////////////////////

void mdSpiesPanel::OnContextMenu(wxContextMenuEvent& event)
{
	long idx=GetSelectedIndex();

	if(idx!=-1)
	{
		long listi=m_SpiesList->GetItemData(idx);
		mdSpy *spy=gProject->GetSpy(listi);
	
		if(spy->m_DisplayHexa)
			m_Popup->SetLabel(Menu_Display_Hexa,_T("Display Decimal"));
		else
			m_Popup->SetLabel(Menu_Display_Hexa,_T("Display Hexadecimal"));
	}
	PopupMenu( m_Popup);
}

///////////////////////////////////////////////////////////////////////////////
// Get the index of the selected item
///////////////////////////////////////////////////////////////////////////////

long mdSpiesPanel::GetSelectedIndex()
{
	long item = -1;
    for ( ;; )
    {   item = m_SpiesList->GetNextItem(item,
                                     wxLIST_NEXT_ALL,
                                     wxLIST_STATE_SELECTED); 
        if ( item == -1 )
            break; 

		return item;
    } 
	
	return -1;
}

///////////////////////////////////////////////////////////////////////////////
// process l'item
///////////////////////////////////////////////////////////////////////////////

void mdSpiesPanel::ProcessItem(bool Update,mdSpy* spy)
{
	ProcessItem(-1,Update,spy);
}

void mdSpiesPanel::ProcessItem(int list_idx,bool Update,mdSpy* spy)
{
	long idx=GetSelectedIndex();

	if(Update==true)
	{	// maj l'item
		//int i=spy->m_ListIndex;
		int i=GetSelectedIndex();
		wxString b;

		if(list_idx!=-1)
			i=list_idx;

		long listi=m_SpiesList->GetItemData(i);
		mdSpy *s=gProject->GetSpy(listi);
		
		s->m_Length = spy->m_Length;
		s->SetName(spy->GetName());
		s->SetAddress(spy->GetAddress(),true);
		s->m_DisplayHexa=spy->m_DisplayHexa;
		
		m_SpiesList->SetItem(i, 0,s->GetName());
		m_SpiesList->SetItem(i, 1,b.Format("0x%.2x",s->GetAddress()));

		switch(s->m_Length)
		{
			case 1:
			{	
				if(spy->m_DisplayHexa)
					m_SpiesList->SetItem(i, 2,b.Format("0x%.2x",0));
				else
					m_SpiesList->SetItem(i, 2,b.Format("%.10d",0));
			}break;

			case 2:
			{	if(spy->m_DisplayHexa)
					m_SpiesList->SetItem(i, 2,b.Format("0x%.4x",0));			
				else
					m_SpiesList->SetItem(i, 2,b.Format("%.10d",0));			
			}break;

			case 4:
			{	if(spy->m_DisplayHexa)
					m_SpiesList->SetItem(i, 2,b.Format("0x%.8x",0));
				else
					m_SpiesList->SetItem(i, 2,b.Format("%.10d",0));
			}break;
		}		
	}
	else
	{	// ajoute un item
		if(spy->m_ArraySize==1)
			this->InsertSpy(list_idx,NULL,spy->GetName(),spy->GetAddress(),spy->m_Length,spy->m_ArraySize);
		else
		{
			unsigned int addr=spy->GetAddress();
			for(int i=0;i<spy->m_ArraySize;i++)
			{	wxString b;
				b=b.Format("%s[%d]",spy->GetName(),i);

				int qq=0;
				if(list_idx!=-1)
					qq=list_idx;

				this->InsertSpy(qq,NULL,b,addr,spy->m_Length,1,true);
				addr+= spy->m_Length;
			}
		
		}
	}

	gFrmMain->UpdateDisAsm();
}

///////////////////////////////////////////////////////////////////////////////
// Add spy
///////////////////////////////////////////////////////////////////////////////

void mdSpiesPanel::OnInsertSpy(wxCommandEvent& event)
{
	mdFrmSpyAttr *dlg=new mdFrmSpyAttr(this);
	dlg->SetSpiesPanel(this);
	dlg->m_InsertIndex=GetSelectedIndex();
	dlg->ShowModal();	
	dlg->Destroy();
}

void mdSpiesPanel::OnAddSpy(wxCommandEvent& event)
{
	mdFrmSpyAttr *dlg=new mdFrmSpyAttr(this);
	dlg->SetSpiesPanel(this);
	dlg->ShowModal();	
	dlg->Destroy();

	UpdateSpies(1);
}

///////////////////////////////////////////////////////////////////////////////
// Update spy
///////////////////////////////////////////////////////////////////////////////

void mdSpiesPanel::OnUpdateSpy(wxCommandEvent& event)
{
	long idx=GetSelectedIndex();
	if(idx!=-1)
	{	long listi=m_SpiesList->GetItemData(idx);
		mdSpy *spy=gProject->GetSpy(listi);

		// force 
		mdFrmSpyAttr *dlg=new mdFrmSpyAttr(this);
		dlg->SetUpdateSpy(spy);
		dlg->SetSpiesPanel(this);
		dlg->ShowModal();	
		dlg->Destroy();
	}
}

///////////////////////////////////////////////////////////////////////////////
// Supprime l'espion
///////////////////////////////////////////////////////////////////////////////

void mdSpiesPanel::OnDeleteSpy(wxCommandEvent& event)
{
	long idx=GetSelectedIndex();

	if(idx!=-1)
	{
		long listi=m_SpiesList->GetItemData(idx);
		mdSpy *spy=gProject->GetSpy(listi);
		wxString b;
		b=b.Format("Do you want to delete \"%s\" spy ?",spy->GetName().c_str());
		
		if(wxMessageBox(b, "Confirm",wxYES_NO, this)==wxYES)
		{	
			
			m_SpiesList->DeleteItem(idx);
			// supprime dans la liste
			gProject->RemoveSymbol(0,spy->GetAddress());
			gProject->RemoveSpy(listi);			
			gFrmMain->UpdateDisAsm();
			
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////
// Select the spy
///////////////////////////////////////////////////////////////////////////////

void mdSpiesPanel::SelectSpy(wxListEvent& event)
{	
	long w=event.m_itemIndex;
	//mdSpy *s=(mdSpy*) m_Spies[w];
	//wxMessageBox(s->m_Name);

//	long i=event.GetData();
//	i=i;

	/*
	wxListItem l;
	long i;

	l.SetId(w);
	if(m_SpiesList->GetItem(l)==true)
	{
		if(l.m_mask & wxLIST_MASK_DATA )
			i=l.GetData();
	}
	i=i;
	*/
}

void mdSpiesPanel::OnDisplayHexa(wxCommandEvent& event)
{
	long idx=GetSelectedIndex();

	if(idx!=-1)
	{
		long listi=m_SpiesList->GetItemData(idx);
		mdSpy *spy=gProject->GetSpy(listi);
		spy->m_DisplayHexa=!spy->m_DisplayHexa;

		UpdateSpies(TRUE);		
	}	
}

///////////////////////////////////////////////////////////////////////////////
// Update the values
///////////////////////////////////////////////////////////////////////////////

void mdSpiesPanel::UpdateSpies(bool force_update)
{
	for(int i=0;i<m_SpiesList->GetItemCount();i++)
	{	
		long listi=m_SpiesList->GetItemData(i);
		mdSpy *spy=gProject->GetSpy(listi);
		unsigned int data=0;
		
		// lit depuis la mémoire
		
		switch(spy->m_Length)
		{
			case 1:
			{	data=m_Emu->M68K_ReadMemory8(spy->GetAddress());
			}break;

			case 2:
			{	data=m_Emu->M68K_ReadMemory16(spy->GetAddress());
			}break;

			case 4:
			{	data=m_Emu->M68K_ReadMemory32(spy->GetAddress());
			}break;
		}

		// rafraichit 
		if(spy->m_Value!=data  || force_update==TRUE)
		{	wxString b;
			spy->m_Value=data;

			switch(spy->m_Length)
			{
				case 1:
				{	
					if(spy->m_DisplayHexa)
						m_SpiesList->SetItem(i, 2,b.Format("0x%.2x",data));
					else
						m_SpiesList->SetItem(i, 2,b.Format("%.10d",data));
				}break;

				case 2:
				{	if(spy->m_DisplayHexa)
						m_SpiesList->SetItem(i, 2,b.Format("0x%.4x",data));	
					else
						m_SpiesList->SetItem(i, 2,b.Format("%.10d",data));
				}break;

				case 4:
				{	if(spy->m_DisplayHexa)
						m_SpiesList->SetItem(i, 2,b.Format("0x%.8x",data));
					else
						m_SpiesList->SetItem(i, 2,b.Format("%.10d",data));
				}break;
			}		
		}
		
	}
}

///////////////////////////////////////////////////////////////////////////////
// Effaces
///////////////////////////////////////////////////////////////////////////////

void mdSpiesPanel::Clear(void)
{
	gProject->ClearAllSpies();
	m_SpiesList->DeleteAllItems();
	gFrmMain->UpdateDisAsm();

}

void mdSpiesPanel::SetEmu(mdEmu* emu)
{
	m_Emu=emu;
}

///////////////////////////////////////////////////////////////////////////////