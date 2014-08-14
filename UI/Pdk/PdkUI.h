#ifndef	__PDKUI_H__
#define	__PDKUI_H__

#ifndef __cplusplus
#error PdkUI.h included in a non-C++ source file!
#endif

// --------------------------------------------------------------------
//              INCLUDES
// --------------------------------------------------------------------

#include "PdkDef.h"
#include "PdkDM.h"
#include "PdkUtil.h"
#include "PdkApp.h"
#include "PdkDM.h"

// Windows includes for drivers, gdidefs.inc and windows.h
// conflict, so if in MFC no gdidefs.inc.
#ifdef __AFXWIN_H__  //AFX Enabled!
	#define NO_GDIDEFS
#endif
#ifdef __AFX_H__  	//AFX Enabled!
	#define NO_GDIDEFS
#endif


// --------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// --------------------------------------------------------------------

// --------------------------------------------------------------------
//      		EXPORTED CLASS DEFINITIONS
// --------------------------------------------------------------------
class CDTCUIDrv;
// ............................................................................
// CLASS NAME:      CPdkUIDrv
//
// REPONSIBILITIES: 
//	This class defines the necessary entry points for the UI DLL
//	portion of a PDK-based driver. It is the main class for implementing the
//	functionality of the printer driver interface DLL.
//
// NOTES:
// ............................................................................
class CPdkUIDrv  : public CPdkApp
{
	public:
		// We are a Singleton
		static CPdkUIDrv*	Instance();

		virtual ~CPdkUIDrv();

		// ....................................................................
		// METHOD NAME:		DrvDevicePropertySheets
		// DESCRIPTION:
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual LONG DrvDevicePropertySheets(PROPSHEETUI_INFO *pPSUIInfo, 
															LPARAM lParam) = 0;

		// ....................................................................
		// METHOD NAME:		DrvDocumentPropertySheets
		// DESCRIPTION:
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual LONG DrvDocumentPropertySheets(PROPSHEETUI_INFO *pPSUIInfo, 
															LPARAM lParam) = 0;

		// ....................................................................
		// METHOD NAME:		DrvDeviceCapabilities
		// DESCRIPTION:
		//		Descendant classes should implement this method, using this
		//		class' drvDeviceCapabilities() method to handle normal cases
		//		of iDevCap.
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual DWORD DrvDeviceCapabilities(HANDLE hPrinter,
											PWSTR pDeviceName,
											WORD iDevCap,
											VOID *pvOutput,
											DEVMODE *pDevMode) = 0;

		// ....................................................................
		// METHOD NAME:		DrvConvertDevMode
		// DESCRIPTION:
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual BOOL DrvConvertDevMode(	LPTSTR pPrinterName, DEVMODE *pdmIn, 
						DEVMODE *pdmOut, PLONG pcbNeeded, DWORD fMode) = 0;

		// ....................................................................
		// METHOD NAME:		DevQueryPrintEx
		// DESCRIPTION:
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual BOOL DevQueryPrintEx(PDEVQUERYPRINT_INFO pDQPInfo);

		// ....................................................................
		// METHOD NAME:		DrvPrinterEvent
		// DESCRIPTION:
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual BOOL DrvPrinterEvent(	LPWSTR pPrinterName,
											INT DriverEvent,
											DWORD Flags,
											LPARAM lParam);

		// ....................................................................
		// METHOD NAME:		DrvUpgradePrinter
		// DESCRIPTION:
		// 		This default implementation assumes that all registry settings 
		// 		associated with the product specific driver are compatible 
		// 		across upgrades of the printer driver version or OS version. 
		//		If not, the product specific code should override this method 
		//		and convert the devmode.
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual BOOL DrvUpgradePrinter(DWORD Level, LPBYTE pDriverUpgradeInfo);

		// ....................................................................
		// METHOD NAME:		DrvDocumentEvent
		// DESCRIPTION:
		//		Override this method to provide processing for escape codes
		//		encountered during the printing of a document. This default
		//		implementation simply returns DOCUMENTEVENT_UNSUPPORTED.
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual INT DrvDocumentEvent(HANDLE hPrinter, HDC hdc, int iEsc,
							ULONG cbIn, ULONG *pbIn, ULONG cbOut, ULONG *pbOut);

		// ....................................................................
		// METHOD NAME:		DevQueryPrint
		// DESCRIPTION:
		//		Deprecated to DevQueryPrintEx
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual BOOL DevQueryPrint(HANDLE hPrinter, DEVMODE* pDevMode, DWORD* pResID);

		// ....................................................................
		// METHOD NAME:		DrvAdvancedDocumentProperties
		// DESCRIPTION:
		//		Deprecated to DrvDocumentPropertySheets
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual LONG DrvAdvancedDocumentProperties(	HWND hwnd,
														HANDLE hPrinter,
														PWSTR lpszDevName,
														PDEVMODE pdmOutput,
														PDEVMODE  pdmInput);

		// ....................................................................
		// METHOD NAME:		DrvDocumentProperties
		// DESCRIPTION:
		//		Deprecated to DrvDocumentPropertySheets
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual LONG DrvDocumentProperties(	HWND hwnd,
												HANDLE hPrinter,
												PWSTR lpszDeviceName,
												DEVMODEPDK *pdmOutput,
												DEVMODEPDK *pdmInput,
												DWORD fMode);

		// ....................................................................
		// METHOD NAME:		PrinterProperties
		// DESCRIPTION:
		//		Deprecated
		//		
		// RETURN VALUE:
		// ....................................................................
		virtual BOOL PrinterProperties(HWND hWnd, HANDLE hPrinter);

	protected:

		// constructor
		CPdkUIDrv();

		// ## Helper Methods available to descendant classes

		// Routines to be overridden by product specific code to handle the
		// events of DrvPrinterEvent if specific processing is reqd.
		// ................................................................
		virtual BOOL printerEventAddConn(HANDLE hPrinter, bool bAllowUI) 
																{return TRUE;}
		virtual BOOL printerEventCacheDelete(HANDLE hPrinter) {return TRUE;}
		virtual BOOL printerEventCacheRefresh(HANDLE hPrinter) {return TRUE;}
		virtual BOOL printerEventDelConn(HANDLE hPrinter, bool bAllowUI) 
																{return TRUE;}
		virtual BOOL printerEventDelete(HANDLE hPrinter) {return TRUE;}
		virtual BOOL printerEventInitialize(HANDLE hPrinter) {return TRUE;}
		#if 0 && (WINVER >= 0x0500)	
		virtual BOOL printerEventAttrChanged(HANDLE hPrinter,
					 PRINTER_EVENT_ATTRIBUTES_INFO *pAttrInfo) {return TRUE;}
		#endif

		OS_CLockable m_mtxEntry;

	private:

		//static CPdkUIDrv* m_instance;
		static CDTCUIDrv  m_instance2;

		// ## Helper Methods

		// ## Instance Data
};

#endif	// __PDKUI_H__
