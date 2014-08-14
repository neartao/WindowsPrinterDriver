#ifndef	__DTCUIDRV_H__
#define	__DTCUIDRV_H__

// ----------------------------------------------------------------------------
//					INCLUDES						
// ----------------------------------------------------------------------------

#include "PdkUI.h"
#include "PdkWin.h"
#include "PdkUICtx.h"

// ----------------------------------------------------------------------------
//      #DEFINES, TYPEDEF'S, and TEMPLATES             
// ----------------------------------------------------------------------------
#ifndef MessageLog
#ifdef _DEBUG
#define MessageLog(str) \
    {   \
        HANDLE hfile = ::CreateFile(TEXT("E:\\unidrv.log"), GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  \
        if(CREATEFILE_ERROR != hfile)    \
        {   \
            DWORD dwwrited = 0; \
            ::SetFilePointer(hfile, 0, NULL, FILE_END);    \
            ::WriteFile(hfile, (LPBYTE)(str), strlen(str), &dwwrited, NULL);    \
            ::CloseHandle(hfile); \
        }   \
    }
#else
#define MessageLog(str) 
#endif
#endif
// ----------------------------------------------------------------------------
//	              EXPORTED DATA                          
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              PRIVATE SUPPORT CLASSES                      
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              EXPORTED CLASSES                      
// ----------------------------------------------------------------------------

// ## Forward declarations
//class CDTCUIContext;

// ............................................................................
// CLASS NAME: 		CDTCUIDrv
//
// RESPONSIBILITIES:
//		This singleton class provides implementations to the WinDDK Printer IF
//		DLL entry points.
//
// NOTES:
// ............................................................................
class CDTCUIDrv  : public CPdkUIDrv
{
    static BOOL bTraceOn;
	public:
		// ## Constructors / Destructor
		CDTCUIDrv();
		virtual ~CDTCUIDrv();

		// ....................................................................
		// Win DDK Printer Interface DLL Entry Point Handlers
		// ....................................................................

		// ....................................................................
		// METHOD NAME:		DrvDevicePropertySheets
		// DESCRIPTION:
		//		Calls CPSUI to create property sheet pages that describe a
		//		printer's properties
		//		
		// RETURN VALUE:
		//		If the operation succeeds, returns 1 or greater. Returns
		//		<1 on error.
		// ....................................................................
		virtual LONG DrvDevicePropertySheets(PPROPSHEETUI_INFO pPSUIInfo, 
																LPARAM lParam);

		// ....................................................................
		// METHOD NAME:		DrvDocumentPropertySheets
		// DESCRIPTION:
		//		Calls CPSUI to create property sheet pages that describe a
		//		print document's properties
		//		
		// RETURN VALUE:
		//		If pSUIInfo is NULL and either lParam->fMode is zero or 
		//		lParam->pdmOut is NULL, returns the size in bytes of the 
		//		printer's DEVMODE structure. Otherwise, if the operation 
		//		succeeds, returns 1 or greater. Returns <1 on error.
		// ....................................................................
		virtual LONG DrvDocumentPropertySheets(PPROPSHEETUI_INFO pPSUIInfo, 
																LPARAM lParam);

		// ....................................................................
		// METHOD NAME:		DrvDeviceCapabilities
		// DESCRIPTION:
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual DWORD DrvDeviceCapabilities(HANDLE hPrinter, PWSTR pDeviceName, 
							WORD iDevCap, VOID *pvOutput, DEVMODE *pDevMode);

		// ....................................................................
		// METHOD NAME:		DrvConvertDevMode
		// DESCRIPTION:
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual BOOL DrvConvertDevMode(	LPTSTR pPrinterName, DEVMODE *pdmIn, 
						DEVMODE *pdmOut, PLONG pcbNeeded, DWORD fMode);

		// ....................................................................
		// METHOD NAME:		DrvDocumentEvent
		// DESCRIPTION:
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual INT DrvDocumentEvent(HANDLE hPrinter, HDC hdc, int iEsc,
							ULONG cbIn, ULONG *pbIn, ULONG cbOut, ULONG *pbOut);

	protected:

		// ## Helper Methods

		// Adds a tab to the CPSUI system
		virtual int addDlgTab(CPdkWin *pTab, CPdkUIContext *pUICtxt,
							   PROPSHEETUI_INFO *pPSUIInfo);

		// ## Instance Data

};

#endif	// __DTCUIDRV_H__
