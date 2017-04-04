///////////////////////////////////////////////////////////////////////////////
//
//
//
// NOT PORTABLE I'M USING DIB
///////////////////////////////////////////////////////////////////////////////
// Includes 

#include "include/mdShared.h"
#include "include/DXKeyboard.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

DXKeyboard::DXKeyboard(HWND hwnd,HINSTANCE hinst)
{
	HRESULT rval=DirectInputCreate(hinst,DIRECTINPUT_VERSION,&lpdi,NULL);
	if(rval!=DI_OK)
	{	DIError(rval);}

	if(lpdi)
	{
		rval=lpdi->CreateDevice(GUID_SysKeyboard,&lpdi_keyboard,NULL);
		if(rval!=DI_OK)
		{	DIError(rval);			
		}

		rval=lpdi_keyboard->SetCooperativeLevel(hwnd,DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		if(rval!=DI_OK)
		{	DIError(rval);			
		}

		rval=lpdi_keyboard->SetDataFormat(&c_dfDIKeyboard);
		if(rval!=DI_OK)
		{	DIError(rval);		
		}

		rval=lpdi_keyboard->Acquire();
		if(rval!=DI_OK)
		{	DIError(rval);			
		}
	}
}

DXKeyboard::~DXKeyboard()
{
	if(lpdi_keyboard)
	{	memset(keyboard_state,0,sizeof(keyboard_state));
		lpdi_keyboard->Unacquire();
		if(lpdi_keyboard != NULL) { lpdi_keyboard->Release(); lpdi_keyboard = NULL; }
	}
	if(lpdi)
		lpdi->Release();
}


void DXKeyboard::Update(void)
{
	if(lpdi_keyboard)
	{	HRESULT i_rval=lpdi_keyboard->GetDeviceState(256,keyboard_state);

		if(i_rval!=DI_OK)
		{
			if(i_rval==DIERR_INPUTLOST || i_rval == DIERR_NOTACQUIRED)
			{	if(lpdi_keyboard)
					lpdi_keyboard->Acquire();
			}
		}
	}
}

int DXKeyboard::Check_Key(int keynum)
{	int ret=0;
	if(lpdi_keyboard)
	{	if(keyboard_state[keynum] & 0x80)
		{	ret=1;}
	}
	return ret;
}


void DXKeyboard::DIError(HRESULT rval)
{
	wxLogError("Error initializing keyboard");
	//MessageBox(hwnd,"Error initializing input","P-O-R Direct input",MB_OK| MB_ICONEXCLAMATION);

}