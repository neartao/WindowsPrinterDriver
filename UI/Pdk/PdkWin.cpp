// ----------------------------------------------------------------------------
//              INCLUDE FILES                  
// ----------------------------------------------------------------------------

#include "PdkWin.h" 
#include "commctrl.h"

// ----------------------------------------------------------------------------
//              GLOBAL DATA DEFINITIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              LOCAL #DEFINES, TYPEDEF'S, and TEMPLATES             
// ----------------------------------------------------------------------------

#ifdef WS_EX_CONTEXTHELP 
	#undef  WS_EX_CONTEXTHELP	
#endif                       
#define WS_EX_CONTEXTHELP       0x00000400L

// ----------------------------------------------------------------------------
//              LOCAL (STATIC) FUNCTIONS             
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              CLASS ATTRIBUTE (STATIC) INITIALIZATIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              PUBLIC METHOD DEFINITIONS
// ----------------------------------------------------------------------------

// ...........................................................................
//
// NAME:	CPdkWin::CPdkWin		(Constructor)
//
// IMPLEMENTATION NOTES:
// ...........................................................................
CPdkWin::CPdkWin(CPdkUIContext *pUICtxt) : m_pUICtxt(pUICtxt)
{
	return;
}

// ...........................................................................
//
// NAME:	CPdkWin::~CPdkWin		(Destructor)
//
// IMPLEMENTATION NOTES:
// ...........................................................................
CPdkWin::~CPdkWin()
{
	return;
}

// ----------------------------------------------------------------------------
//              	PUBLIC FUNCTION DEFINITIONS
// ----------------------------------------------------------------------------

// ...........................................................................
//
// NAME:	CPdkWinProcedure
//
// IMPLEMENTATION NOTES:
//		Used as message handler when subclassing a window
// ...........................................................................
extern "C" INT_PTR _export CALLBACK 
FnPdkWinProcedure(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	CPdkWin* pTab = NULL;

	if (iMessage == WM_INITDIALOG)
	{
		PROPSHEETPAGE *pPropSheetPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);
		ASSERT(pPropSheetPage != NULL);
		pTab = reinterpret_cast<CPdkWin*>(pPropSheetPage->lParam);

		if (pTab)
		{
			//CDialogImpl
			pTab->SubclassWindow(hWnd);

			return pTab->OnInitDialog(hWnd, lParam);
			//return CFrameWindowImpl< CPdkWin >::WindowProc(hWnd, iMessage, wParam, lParam);
		}
	}

	return FALSE;
}

// ----------------------------------------------------------------------------
//              	STATIC FUNCTION DEFINITIONS
// ----------------------------------------------------------------------------
