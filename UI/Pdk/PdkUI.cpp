// ----------------------------------------------------------------------------
//              INCLUDE FILES                  
// ----------------------------------------------------------------------------
#include "PdkStd.h"
#include "PdkUI.h"
#include "PdkDbg.h"
#include "PdkApp.h"

#include <prsht.h>

#if WINVER==0x0400
extern "C"{
#include <libproto.h>
}
#endif

// ----------------------------------------------------------------------------
//              GLOBAL DATA DEFINITIONS
// ----------------------------------------------------------------------------

BOOL g_bDumpParams = FALSE;     // Flag for dumping function parameters.

// Set to true to turn on this category of debug output
static BOOL m_bTracePdkUI = TRUE;

// ----------------------------------------------------------------------------
//              LOCAL #DEFINES, TYPEDEF'S, and TEMPLATES             
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              LOCAL (STATIC) FUNCTIONS             
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              CLASS ATTRIBUTE (STATIC) INITIALIZATIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              		PUBLIC METHOD DEFINITIONS
// ----------------------------------------------------------------------------

// ...........................................................................
//
// NAME:	CPdkUIDrv::CPdkUIDrv
//
// IMPLEMENTATION NOTES:
// ...........................................................................
CPdkUIDrv::CPdkUIDrv()
{
	FUNCTION_START(m_bTracePdkUI, _T("CPdkUIDrv::CPdkUIDrv"));
}

// ...........................................................................
//
// NAME:	CPdkUIDrv::~CPdkUIDrv
//
// IMPLEMENTATION NOTES:
// ...........................................................................
CPdkUIDrv::~CPdkUIDrv()
{
	FUNCTION_START(m_bTracePdkUI, _T("CPdkUIDrv::~CPdkUIDrv"));

	return;
}

// ...........................................................................
//
// NAME:	CPdkUIDrv::DevQueryPrintEx
//
// IMPLEMENTATION NOTES:
//		This method simply returns true for now. The product specific driver
//		knows about the currently loaded media configuration, it should
//		override this method and return false if the job specified in pDQPInfo
//		is not compatible with the current configuration.
// ...........................................................................
BOOL 
CPdkUIDrv::DevQueryPrintEx(PDEVQUERYPRINT_INFO pDQPInfo)
{
	OS_CAutoLock lock(m_mtxEntry);

	FUNCTION_START(m_bTracePdkUI, _T("@@@@ CPdkUIDrv::DevQueryPrintEx"));

	// Say that the job is printable and not to be held.
	return TRUE;
}

// ...........................................................................
//
// NAME:	CPdkUIDrv::DrvPrinterEvent
//
// IMPLEMENTATION NOTES:
// ...........................................................................
BOOL 
CPdkUIDrv::DrvPrinterEvent(LPWSTR pPrinterName, INT DriverEvent, DWORD Flags, 
																LPARAM lParam)
{
	OS_CAutoLock lock(m_mtxEntry);

    FUNCTION_START(m_bTracePdkUI, _T("@@@@ CPdkUIDrv::DrvPrinterEvent"));
	TRACE(_T("CPdkUIDrv:: Printer: '%s' Event: '%s' Flags: %ld lParam: %ld\n"), 
					pPrinterName, 
		  (DriverEvent == PRINTER_EVENT_ADD_CONNECTION) ? _T("AddConn") :
		  #if 0 // (WINVER >= 0x0500)
		  (DriverEvent == PRINTER_EVENT_ATTRIBUTES_CHANGED) ? _T("AttrChanged") :
		  #endif
		  (DriverEvent == PRINTER_EVENT_CACHE_DELETE) ? _T("CacheDelete") :
		  (DriverEvent == PRINTER_EVENT_CACHE_REFRESH) ? _T("CacheRefresh") :
		  (DriverEvent == PRINTER_EVENT_DELETE_CONNECTION) ? _T("DelConn") :
		  (DriverEvent == PRINTER_EVENT_DELETE) ? _T("Delete") :
		  (DriverEvent == PRINTER_EVENT_INITIALIZE) ? _T("Initialize") :
	  			_T("UNKNOWN EVENT!"),
		  Flags, lParam);

	HANDLE hPrinter = NULL;
	BOOL bRetVal = FALSE;

	// Retrieve a handle to the specified printer
	if (pPrinterName)
	{
		if (::OpenPrinter(pPrinterName, &hPrinter, NULL) == 0)
		{
			TRACE(_T("#### CDTCUIDrv::DrvPrinterEvent: OpenPrinter() failed."));
			return FALSE;
		}
	}

	// Extract information from the Flags parameter
	bool bAllowUI = ((Flags & PRINTER_EVENT_FLAG_NO_UI) == 0);

	// Switch to the correct event handler. These will usually be overridden
	// by the descendant class when special handling is required.
	switch(DriverEvent)
	{
		case PRINTER_EVENT_ADD_CONNECTION:
			bRetVal = printerEventAddConn(hPrinter, bAllowUI);
			break;
		#if 0 // (WINVER >= 0x0500)
		case PRINTER_EVENT_ATTRIBUTES_CHANGED:
			bRetVal = printerEventAttrChanged(hPrinter,
						static_cast<PRINTER_EVENT_ATTRIBUTES_INFO *>(lParam));
			break;
		#endif
		case PRINTER_EVENT_CACHE_DELETE:
			bRetVal = printerEventCacheDelete(hPrinter);
			break;
		case PRINTER_EVENT_CACHE_REFRESH:
			bRetVal = printerEventCacheRefresh(hPrinter);
			break;
		case PRINTER_EVENT_DELETE_CONNECTION:
			bRetVal = printerEventDelConn(hPrinter, bAllowUI);
			break;
		case PRINTER_EVENT_DELETE:
			bRetVal = printerEventDelete(hPrinter);
			break;
		case PRINTER_EVENT_INITIALIZE:
			bRetVal = printerEventInitialize(hPrinter);
			break;
		default:
			break;
	}

	// Close the printer. Ignore error codes.
	if (::ClosePrinter(hPrinter) == 0)
	{
		TRACE(_T("#### CDTCUIDrv::DrvConvertDevMode: ClosePrinter()")
			  _T(" called failed."));
	}

	return bRetVal;
}

// ...........................................................................
//
// NAME:	CPdkUIDrv::DrvUpgradePrinter
//
// IMPLEMENTATION NOTES:
// ...........................................................................
BOOL 
CPdkUIDrv::DrvUpgradePrinter(DWORD  Level, LPBYTE  pDriverUpgradeInfo)
{
	OS_CAutoLock lock(m_mtxEntry);

    FUNCTION_START(m_bTracePdkUI, _T("@@@@ CPdkUIDrv::DrvUpgradePrinter"));
	TRACE(_T("CPdkUIDrv::DrvUpgradePrinter: Level: %ld\n"), Level);

	// Return success by default.
    return TRUE;
}

// ...........................................................................
//
// NAME:	CPdkUIDrv::DrvDocumentEvent
//
// IMPLEMENTATION NOTES:
// ...........................................................................
INT 
CPdkUIDrv::DrvDocumentEvent(HANDLE hPrinter, HDC hdc, int iEsc, ULONG cbIn, 
							ULONG *pbIn, ULONG cbOut, ULONG *pbOut)
{
	OS_CAutoLock lock(m_mtxEntry);

    FUNCTION_START(m_bTracePdkUI, _T("@@@@ CPdkUIDrv::DrvDocumentEvent"));

    return DOCUMENTEVENT_UNSUPPORTED;
}

// ...........................................................................
//
// NAME:	CPdkUIDrv::DevQueryPrint
//
// IMPLEMENTATION NOTES:
//		This is a deprecated method - use DevQueryPrintEx instead.
// ...........................................................................
BOOL 
CPdkUIDrv::DevQueryPrint(HANDLE hPrinter, DEVMODE* pDevMode, DWORD* pResID)
{
	OS_CAutoLock lock(m_mtxEntry);

	FUNCTION_START(m_bTracePdkUI, _T("@@@@ CPdkUIDrv::DevQueryPrint"));
	TRACE(_T("#### CPdkUIDrv::DevQueryPrint: Unsupported & deprecated method!"));

	*pResID = 0;

	return TRUE;
}

// ...........................................................................
//
// NAME:	CPdkUIDrv::DrvAdvancedDocumentProperties
//
// IMPLEMENTATION NOTES:
// ...........................................................................
LONG 
CPdkUIDrv::DrvAdvancedDocumentProperties(	HWND		hwnd,
											HANDLE		hPrinter,
											PWSTR		lpszDevName,
											PDEVMODE	pdmOutput,
											PDEVMODE	pdmInput)
{
	OS_CAutoLock lock(m_mtxEntry);

	FUNCTION_START(m_bTracePdkUI, _T("@@@@ CPdkUIDrv::DrvAdvancedDocumentProperties"));
	TRACE(_T("#### CPdkUIDrv::DrvAdvancedDocumentProperties: Unsupported and ")
		  _T("deprecated method!"));

	return 0;
}

// ...........................................................................
//
// NAME:	CPdkUIDrv::DrvDocumentProperties
//
// IMPLEMENTATION NOTES:
// ...........................................................................
LONG 
CPdkUIDrv::DrvDocumentProperties(	HWND hwnd, HANDLE hPrinter, 
								PWSTR lpszDeviceName, DEVMODEPDK *pdmOutput, 
								DEVMODEPDK *pdmInput, DWORD fMode)
{
	OS_CAutoLock lock(m_mtxEntry);

	FUNCTION_START(m_bTracePdkUI, _T("@@@@ CPdkUIDrv::DrvDocumentProperties"));
	TRACE(_T("#### CPdkUIDrv::DrvDocumentProperties: Unsupported and ")
		  _T("deprecated method!"));

    return 0;
}

// ...........................................................................
//
// NAME:	CPdkUIDrv::PrinterProperties
//
// IMPLEMENTATION NOTES:
// ...........................................................................
BOOL 
CPdkUIDrv::PrinterProperties(HWND hWnd, HANDLE  hPrinter)
{
	OS_CAutoLock lock(m_mtxEntry);

	FUNCTION_START(m_bTracePdkUI, _T("@@@@ CPdkUIDrv::PrinterProperties"));
	TRACE(_T("#### CPdkUIDrv::PrinterProperties: Unsupported and ")
		  _T("deprecated method!"));

	return IDOK;
}

