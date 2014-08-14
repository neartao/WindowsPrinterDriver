// Identification:	%M PdkUIEntry.cpp M% %II%
// Checked out:		%TT% %HH%
// Last modified:	%UU% %GG%
//
// %C Copyright (C) 2004 InSciTek Microsystems, Inc. C%
//
// MODULE DESCRIPTION:
// 		This file contains the driver's GDI entry points.  All GDI calls will go
//		through this file.    
//
//		
// REVISION HISTORY (MOST RECENT FIRST)
//
//  %LL%
//
//  07/13/2004  ATC
//  - original
//

// ----------------------------------------------------------------------------
//              INCLUDE FILES                  
// ----------------------------------------------------------------------------

#include "PdkStd.h"
#include "PdkUI.h"
#include "PdkDump.h"

#include <string.h>
#ifdef WIN_95
	#include <winnet.h>
	#define ERROR_SUCCESS 0
#endif    

// ----------------------------------------------------------------------------
//              GLOBAL DATA DEFINITIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              LOCAL #DEFINES, TYPEDEF'S, and TEMPLATES             
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              LOCAL (STATIC) FUNCTIONS             
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              CLASS ATTRIBUTE (STATIC) INITIALIZATIONS
// ----------------------------------------------------------------------------

static	BOOL	m_bTracePdkUIEntry=FALSE;

// ----------------------------------------------------------------------------
//              CLASS METHOD / FUNCTION DEFINITIONS
// ----------------------------------------------------------------------------


#if WINVER != 0x0400
extern "C"
{
#endif

//-----------------------------------------------------------------------------
BOOL 
DevQueryPrintEntry(HANDLE hPrinter, DEVMODE* pDevMode, DWORD* pResID)
{
	FUNCTION_START(m_bTracePdkUIEntry, _T("CEntry: DevQueryPrint"));

    BOOL bRetVal = FALSE;
    bRetVal = CPdkUIDrv::Instance()->DevQueryPrint(hPrinter, pDevMode, pResID);

    return bRetVal;
}

//-----------------------------------------------------------------------------
LONG 
DrvAdvancedDocumentPropertiesEntry(	HWND		hwnd,
								HANDLE		hPrinter,
								PWSTR		lpszDevName,
								PDEVMODE	pdmOutput,
								PDEVMODE	pdmInput)
{
	FUNCTION_START(m_bTracePdkUIEntry, _T("CEntry: DrvAdvancedDocumentProperties"));

    LONG lRetVal = 0;
    lRetVal = CPdkUIDrv::Instance()->DrvAdvancedDocumentProperties(hwnd, hPrinter, lpszDevName, pdmOutput, pdmInput);

    return lRetVal;
}

//-----------------------------------------------------------------------------
DWORD 
DrvDeviceCapabilitiesEntry(HANDLE hPrinter, PWSTR pDeviceName, WORD iDevCap, VOID* pvOutput, DEVMODE* pDevMode)
{
	FUNCTION_START(m_bTracePdkUIEntry, _T("CEntry: DrvDeviceCapabilities"));

    DWORD dwRetVal;
    dwRetVal = CPdkUIDrv::Instance()->DrvDeviceCapabilities(hPrinter, pDeviceName, iDevCap, pvOutput, pDevMode);

    return dwRetVal;
}

//-----------------------------------------------------------------------------
LONG 
DrvDocumentPropertiesEntry(HWND hwnd, HANDLE hPrinter, PWSTR lpszDeviceName, PDEVMODEW pdmOutput, PDEVMODEW pdmInput, DWORD fMode)
{
	FUNCTION_START(m_bTracePdkUIEntry, _T("CEntry: DrvDocumentProperties"));

    LONG lRetVal = 0;
    lRetVal = CPdkUIDrv::Instance()->DrvDocumentProperties(hwnd, hPrinter, lpszDeviceName, (DEVMODEPDK *)pdmOutput, (DEVMODEPDK *)pdmInput, fMode);

    return lRetVal;
}

//-----------------------------------------------------------------------------
BOOL
PrinterPropertiesEntry(HWND hWnd, HANDLE hPrinter)
{
	FUNCTION_START(m_bTracePdkUIEntry, _T("CEntry: PrinterProperties"));

    BOOL bRetVal = FALSE;
    bRetVal = CPdkUIDrv::Instance()->PrinterProperties(hWnd, hPrinter);

    return bRetVal;
}

//-----------------------------------------------------------------------------
BOOL 
DevQueryPrintExEntry(PDEVQUERYPRINT_INFO pDQPInfo)
{
	FUNCTION_START(m_bTracePdkUIEntry, _T("CEntry: DevQueryPrintEx"));

    BOOL bRetVal = FALSE;
    bRetVal = CPdkUIDrv::Instance()->DevQueryPrintEx(pDQPInfo);

    return bRetVal;
}

//-----------------------------------------------------------------------------
BOOL 
DrvConvertDevModeEntry(LPTSTR pPrinterName, PDEVMODE pdmIn, PDEVMODE pdmOut, PLONG pcbNeeded, DWORD fMode)
{
	FUNCTION_START(m_bTracePdkUIEntry, _T("CEntry: DrvConvertDevMode"));

    BOOL bRetVal = FALSE;
    bRetVal = CPdkUIDrv::Instance()->DrvConvertDevMode(pPrinterName, (DEVMODE *)pdmIn, 
											(DEVMODE *)pdmOut, pcbNeeded, fMode);

    return bRetVal;
}


//-----------------------------------------------------------------------------
LONG 
DrvDevicePropertySheetsEntry(PPROPSHEETUI_INFO pPSUIInfo, LPARAM lParam)
{
	FUNCTION_START(m_bTracePdkUIEntry, _T("CEntry: DrvDevicePropertySheets"));

    LONG lRetVal = 0;
    lRetVal = CPdkUIDrv::Instance()->DrvDevicePropertySheets(pPSUIInfo, lParam);

    return lRetVal;
}

//-----------------------------------------------------------------------------
LONG 
DrvDocumentPropertySheetsEntry(PPROPSHEETUI_INFO  pPSUIInfo, LPARAM  lParam)
{
	FUNCTION_START(m_bTracePdkUIEntry, _T("CEntry: DrvDocumentPropertySheets"));

    LONG lRetVal = 0;
    lRetVal = CPdkUIDrv::Instance()->DrvDocumentPropertySheets(pPSUIInfo, lParam);

    return lRetVal;
}


//-----------------------------------------------------------------------------
BOOL 
DrvPrinterEventEntry(LPWSTR pPrinterName, INT DriverEvent, DWORD Flags, LPARAM lParam)
{
	FUNCTION_START(m_bTracePdkUIEntry, _T("CEntry: DrvPrinterEvent"));

    BOOL bRetVal = FALSE;
    bRetVal = CPdkUIDrv::Instance()->DrvPrinterEvent(pPrinterName, DriverEvent, Flags, lParam );

    return bRetVal;
}

//-----------------------------------------------------------------------------
INT 
DrvDocumentEventEntry(HANDLE hPrinter, HDC hdc, int iEsc, ULONG cbIn, PULONG pbIn, 
                 ULONG cbOut, PULONG pbOut)
{
	FUNCTION_START(m_bTracePdkUIEntry, _T("CEntry: DrvDocumentEvent"));

    INT nRetVal = -1;
    nRetVal = CPdkUIDrv::Instance()->DrvDocumentEvent(hPrinter, hdc, iEsc, cbIn, pbIn, cbOut, pbOut);

    return nRetVal;
}

//-----------------------------------------------------------------------------
BOOL
DrvUpgradePrinterEntry(DWORD Level, LPBYTE pDriverUpgradeInfo)
{
	FUNCTION_START(m_bTracePdkUIEntry, _T("CEntry: DrvUpgradePrinter"));

    BOOL bRetVal = FALSE;
    bRetVal = CPdkUIDrv::Instance()->DrvUpgradePrinter(Level, pDriverUpgradeInfo);

    return bRetVal;
}

#if WINVER != 0x0400
}
#endif