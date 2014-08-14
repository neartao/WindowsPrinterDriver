// ----------------------------------------------------------------------------
//                   INCLUDE FILES                  
// ----------------------------------------------------------------------------

#ifdef NTDDI_VERSION
#undef NTDDI_VERSION
#endif

#define NTDDI_VERSION  NTDDI_LONGHORN

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define _WIN32_WINNT _WIN32_WINNT_LONGHORN

#include "PdkStd.h"
#include "DTCUIDrv.h"
#include "globals.h"
#include "fusutils.h"

#include "AboutDg.h"
#include "OperationDg.h"
#include "UtilityDg.h"
#include "LayoutDg.h"
#include "BarcodeDg.h"
#include "DownloadDg.h"
#include "WatermarkDg.h"
#include "MaxiCodeDg.h"
#include "QRCodeDg.h"
#include "PDF417Dg.h"
#include "SettingDg.h"
#include "DebugDg.h"

// ----------------------------------------------------------------------------
//      LOCAL #DEFINES, TYPEDEF'S, and TEMPLATES             
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//	              GLOBAL DATA DEFINITIONS
// ----------------------------------------------------------------------------

// Set true to turn on debug within this module
BOOL CDTCUIDrv::bTraceOn = TRUE;
static BOOL bTraceDMOn = TRUE;
static BOOL m_bTracePCLUI = TRUE;

// Debug module identifiers
TCHAR achModName[] = L"DTCUIDrv: ";

TCHAR szModule[MAX_PATH];
TCHAR *g_szModule = szModule;	


// ----------------------------------------------------------------------------
//			LOCAL (STATIC) FUNCTIONS             
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//	     CLASS ATTRIBUTE (STATIC) INITIALIZATIONS
// ----------------------------------------------------------------------------

// Stuff for Singleton implementation:
//CPdkUIDrv* CPdkUIDrv::m_instance = NULL;
CDTCUIDrv CPdkUIDrv::m_instance2;

// ----------------------------------------------------------------------------
//	     				PUBLIC METHODS
// ----------------------------------------------------------------------------

// ............................................................................
//
// NAME:	CPdkUIDrv::Instance()
//
// IMPLEMENTATION NOTES:
//	Singleton constructor for base class.
// ............................................................................
CPdkUIDrv* CPdkUIDrv::Instance()
{
	//if (m_instance == NULL)
	{
		//m_instance = new CDTCUIDrv();

		// Create the pathname to the resource DLL and set into application. 
		TCHAR szResourceDLL[MAX_PATH];
        GetModuleFileName(ghInstance, szModule, DIM(szModule));
#ifdef PCLGDI_UI
        _tcscpy(szResourceDLL, szModule);
        int PathLength = _tcslen(szResourceDLL);
        PathLength -= 6;
		_tcscpy(szResourceDLL + PathLength, _T("RC.DLL"));
#else 
        _tcscpy(szResourceDLL, szModule);
        int PathLength = (int)_tcslen(szResourceDLL);
        PathLength -= 5;
        szResourceDLL[PathLength] = _T('R');


#endif


		// Initialize the application
		//m_instance->InitApp(ghInstance, szModule, _T("DTC UI I/F DLL"), szResourceDLL);
		m_instance2.InitApp(ghInstance, szModule, _T("DTC UI I/F DLL"), szResourceDLL);
	}

	//return m_instance;
	return &m_instance2;
}

// ............................................................................
//
// NAME:	GetPdkAppObj
//
// IMPLEMENTATION NOTES:
//	Used by the PDK to get the driver instance.
// ............................................................................
CPdkApp* GetPdkAppObj()
{
	return CPdkUIDrv::Instance();
}

// ............................................................................
//
// NAME:	CDTCUIDrv:CDTCUIDrv		(Constructor)
//
// IMPLEMENTATION NOTES:
// ............................................................................
CDTCUIDrv::CDTCUIDrv()
{
	FUNCTION_START(bTraceOn, _T("CDTCUIDrv::CDTCUIDrv"));

	return;
}

// ............................................................................
//
// NAME:	CDTCUIDrv::~CDTCUIDrv		(Destructor)
//
// IMPLEMENTATION NOTES:
// ............................................................................
CDTCUIDrv::~CDTCUIDrv()
{
	FUNCTION_START(bTraceOn, _T("CDTCUIDrv::~CDTCUIDrv"));

	return;
}

// ............................................................................
//
// NAME:	CDTCUIDrv::DrvDevicePropertySheets
//
// IMPLEMENTATION NOTES:
// ............................................................................
LONG 
CDTCUIDrv::DrvDevicePropertySheets(PPROPSHEETUI_INFO pPSUIInfo, LPARAM lParam)
{
	OS_CAutoLock lock(m_mtxEntry);

	FUNCTION_START(bTraceOn, _T("@@@@ CDTCUIDrv::DrvDevicePropertySheets"));

	// Check basic parameters and extract DevicePropertyHeader from lParam
	if (pPSUIInfo == NULL)
	{
		::PdkSetLastError(ERROR_INVALID_PARAMETER);
		TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: NULL pPSUIInfo\n"));
		return ERR_CPSUI_GETLASTERROR;
	}

	TRACE(_T("CDTCUIDrv::DrvDevicePropertySheets: pPSUIInfo: 0x%08lx lParam: ")
		  _T("0x%08lx pSUIInfo->Reason: %s.\n"), pPSUIInfo, lParam, 
			 ((pPSUIInfo->Reason == PROPSHEETUI_REASON_INIT) ? 
											_T("PROPSHEETUI_REASON_INIT") :
			 (pPSUIInfo->Reason == PROPSHEETUI_REASON_GET_INFO_HEADER) ? 
									_T("PROPSHEETUI_REASON_GET_INFO_HEADER") :
			 (pPSUIInfo->Reason == PROPSHEETUI_REASON_SET_RESULT) ? 
									_T("PROPSHEETUI_REASON_SET_RESULT") :
			 (pPSUIInfo->Reason == PROPSHEETUI_REASON_DESTROY) ? 
							_T("PROPSHEETUI_REASON_DESTROY") : _T("UNKNOWN")));

	switch (pPSUIInfo->Reason)
	{
		case PROPSHEETUI_REASON_INIT:
		{
			// Get the device property header structure
			DEVICEPROPERTYHEADER *pDPH = 
						reinterpret_cast<DEVICEPROPERTYHEADER *>(lParam);
			if (pDPH == NULL)
			{
				::PdkSetLastError(ERROR_INVALID_PARAMETER);
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: NULL pDPH\n"));
				return ERR_CPSUI_GETLASTERROR;
			}
			else if (bTraceOn)
			{
				TRACE(_T("CDTCUIDrv::DrvDevicePropertySheets: pDPH->cbSize = %d, pDPH->Flags")
					  _T(" = 0x%04x, pDPH->hPrinter = 0x%08lx, pDPH->pszPrinterName = %s\n"),
					  pDPH->cbSize, pDPH->Flags, pDPH->hPrinter,
					  (pDPH->pszPrinterName ? pDPH->pszPrinterName : _T("NULL")));
			}

			CPdkUIContext *pUIContext = new CPdkUIContext((POEMUIPSPARAM)(pPSUIInfo->lParamInit));

			// Set the permissions
			if (pDPH->Flags & DPS_NOPERMISSION)
				pUIContext->SetPermission(false);
			else
				pUIContext->SetPermission(true);

										
			// Set the context into the pPSUIInfo structure
			pPSUIInfo->UserData = (ULONG_PTR)pUIContext;
#if (WINVER >= 0x0500)
            {
                DWORD dwWinVer = GetVersion();
                DWORD dwWinMajVer =  (DWORD)(LOBYTE(LOWORD(dwWinVer)));
                if (dwWinMajVer >= 6)
                {
                    DWORD dataSize=0, sizeNeeded=0;
                    //I/O thread not exist,execute I/O thread
                    GetPrinterData(pDPH->hPrinter, _T("PrinterDataSize"), 
                        NULL, (LPBYTE)&dataSize, sizeof(dataSize), &sizeNeeded);
                }
            }
#endif
			// Build Device Settings Property Sheets
			int retVal = 0;

			// 添加应用TAB
			CUtilityDlg *pUtilityDlg = new CUtilityDlg(pUIContext);			
			if ((retVal = addDlgTab(pUtilityDlg, pUIContext, pPSUIInfo)) != 
				ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
					_T("add Device Settings Property Sheet\n"));
				return retVal;
			}

			// 添加下载TAB
			CDownloadDlg *pDownloadDlg = new CDownloadDlg(pUIContext);			
			if ((retVal = addDlgTab(pDownloadDlg, pUIContext, pPSUIInfo)) != 
				ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
					_T("add Device Settings Property Sheet\n"));
				return retVal;
			}

			// 添加版本TAB
			CAboutDlg *pAboutDlg = new CAboutDlg(pUIContext);			
			if ((retVal = addDlgTab(pAboutDlg, pUIContext, pPSUIInfo)) != 
																ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
								 _T("add Device Settings Property Sheet\n"));
				return retVal;
			}

			return 1;
		}
				

		case PROPSHEETUI_REASON_GET_INFO_HEADER:
		{
			// Extract the original device property header structure
			DEVICEPROPERTYHEADER *pDPH = 
				reinterpret_cast<DEVICEPROPERTYHEADER *>(pPSUIInfo->lParamInit);
			if (pDPH == NULL)
			{
				::PdkSetLastError(ERROR_INVALID_PARAMETER);
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: NULL pDPH\n"));
				return ERR_CPSUI_GETLASTERROR;
			}

			// Supply page header information as requested
			PROPSHEETUI_INFO_HEADER *pPSHeaderInfo = 
						reinterpret_cast<PROPSHEETUI_INFO_HEADER *>(lParam);
			pPSHeaderInfo->Flags = PSUIHDRF_PROPTITLE;	// | PSUIHDRF_NOAPPLYNOW;
			pPSHeaderInfo->pTitle = pDPH->pszPrinterName;
			pPSHeaderInfo->hInst = (HINSTANCE)GetResourceInst();
			pPSHeaderInfo->IconID = IDI_CPSUI_PRINTER;	// Is this correct?

			return 1;
		}

        case PROPSHEETUI_REASON_SET_RESULT:
		{
			// Execute set operation with application-supplied status
			// information
			SETRESULT_INFO *pSRInfo = reinterpret_cast<SETRESULT_INFO *>(lParam);

			TRACE(_T("CDTCUIDrv::DrvDevicePropertySheets: pSRInfo->Result: '%s'")
				  _T(" pSRInfo->hSetResult: 0x%08lx\n"),
				  ((pSRInfo->Result == CPSUI_OK) ? _T("CPSUI_OK") :
				   (pSRInfo->Result == CPSUI_CANCEL) ? _T("CPSUI_CANCEL") :
				   (pSRInfo->Result == CPSUI_RESTARTWINDOWS) ? _T("CPSUI_RESTARTWINDOWS") :
				   (pSRInfo->Result == CPSUI_REBOOTSYSTEM) ? _T("CPSUI_REBOOTSYSTEM") :
	 					_T("UNKNOWN!!!")), pSRInfo->hSetResult);

			// Set the result into the UI info
			pPSUIInfo->Result = pSRInfo->Result;

			return 1;	
		}
			
		case PROPSHEETUI_REASON_DESTROY:
		{
			CPdkUIContext *pUIContext = reinterpret_cast<CPdkUIContext *>(
														pPSUIInfo->UserData);

			// Destroy the context including items allocated in PROPSHEETUI_
			// REASON_INIT: config and tab sheet.
			if (pUIContext)
			{
				delete pUIContext;

				pPSUIInfo->UserData = 0;
			}

			// Return a reasonable result 
			// GED TODO: Is this correct?
			pPSUIInfo->Result = CPSUI_OK;

			return 1;			
		}
		
		default:			
		{
			::PdkSetLastError(ERROR_INVALID_PARAMETER);
			TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Invalid ")
													 _T("pPSUIInfo reason\n)"));
			return ERR_CPSUI_GETLASTERROR;
		}
	}

	return 0;
}

// ............................................................................
//
// NAME:	CDTCUIDrv::DrvDocumentPropertySheets
//
// IMPLEMENTATION NOTES:
// ............................................................................
LONG 
CDTCUIDrv::DrvDocumentPropertySheets(PPROPSHEETUI_INFO pPSUIInfo, LPARAM lParam)
{
	OS_CAutoLock lock(m_mtxEntry);

	FUNCTION_START(bTraceOn, _T("@@@@ CDTCUIDrv::DrvDocumentPropertySheets"));

	// Check basic parameters and extract DocumentPropertyHeader from lParam
	if (lParam == NULL)
	{
		::PdkSetLastError(ERROR_INVALID_PARAMETER);
		TRACE(_T("#### CDTCUIDrv::DrvDocumentPropertySheets: Invalid lParam\n"));
		return ERR_CPSUI_GETLASTERROR;
	}

	DOCUMENTPROPERTYHEADER *pDPH = NULL;
	if (pPSUIInfo == NULL)
	{
		pDPH = reinterpret_cast<PDOCUMENTPROPERTYHEADER>(lParam);
		if (!pDPH)
		{
			TRACE(_T("#### CDTCUIDrv::DrvDocumentPropertySheets: ")
				  _T("lParam is NULL, should contain PDOCUMENTPROPERTYHEADER!\n"));
			::PdkSetLastError(ERROR_INVALID_PARAMETER);
			return ERR_CPSUI_GETLASTERROR;
		}
	}
	else
	{
		if (pPSUIInfo->Reason == PROPSHEETUI_REASON_INIT)
			pDPH = reinterpret_cast<PDOCUMENTPROPERTYHEADER>(lParam);
		else
			pDPH = reinterpret_cast<PDOCUMENTPROPERTYHEADER>(pPSUIInfo->lParamInit);

		if (!pDPH)
		{
			TRACE(_T("#### CDTCUIDrv::DrvDocumentPropertySheets: ")
				  _T("pPSUIInfo->lParamInit is NULL!\n"));
			::PdkSetLastError(ERROR_INVALID_PARAMETER);
			return ERR_CPSUI_GETLASTERROR;
		}
	}

	if (bTraceOn && pDPH)
	{
		TRACE(_T("CDTCUIDrv::DrvDocumentPropertySheets: pDPH->cbSize = %d, pDPH->fMode")
			  _T(" = 0x%08lx, pDPH->hPrinter = 0x%08lx, ")
			  _T("pDPH->pdmIn = 0x%08lx, pDPH->pdmOut = 0x%08lx\n"),
			  pDPH->cbSize, pDPH->fMode, pDPH->hPrinter,
			  pDPH->pdmIn, pDPH->pdmOut);
	}

	TRACE(_T("CDTCUIDrv::DrvDocumentPropertySheets: pPSUIInfo: 0x%08lx lParam: ")
		  _T("0x%08lx pSUIInfo->Reason: %s.\n"), pPSUIInfo, lParam, 
			 ((pPSUIInfo->Reason == PROPSHEETUI_REASON_INIT) ? 
											_T("PROPSHEETUI_REASON_INIT") :
			 (pPSUIInfo->Reason == PROPSHEETUI_REASON_GET_INFO_HEADER) ? 
									_T("PROPSHEETUI_REASON_GET_INFO_HEADER") :
			 (pPSUIInfo->Reason == PROPSHEETUI_REASON_SET_RESULT) ? 
									_T("PROPSHEETUI_REASON_SET_RESULT") :
			 (pPSUIInfo->Reason == PROPSHEETUI_REASON_DESTROY) ? 
							_T("PROPSHEETUI_REASON_DESTROY") : _T("UNKNOWN")));

	// Manage property sheets if requested to do so.
	switch (pPSUIInfo->Reason)
	{
		case PROPSHEETUI_REASON_INIT:
		{
			// Build UI context
			CPdkUIContext *pUIContext = new CPdkUIContext((POEMUIPSPARAM)(pPSUIInfo->lParamInit));
			//pUIContext->InitUserSets();

			// Set permissions in the context
			if ((pDPH->fMode & DM_NOPERMISSION) && (pDPH->pdmOut))
				pUIContext->SetPermission(false);
			else
				pUIContext->SetPermission(true);
            pUIContext->m_fMode = pDPH->fMode;

			// Set the context into the pPSUIInfo structure
			pPSUIInfo->UserData = (ULONG_PTR)pUIContext;

			int retVal = 0;

			// 添加排版TAB
			CLayoutDlg *pLayoutDlg = new CLayoutDlg(pUIContext);			
			if ((retVal = addDlgTab(pLayoutDlg, pUIContext, pPSUIInfo)) != 
																ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
								 _T("add Device Settings Property Sheet\n"));
				return retVal;
			}

			// 添加操作TAB
			COperationDlg *pOperationDlg = new COperationDlg(pUIContext);			
			if ((retVal = addDlgTab(pOperationDlg, pUIContext, pPSUIInfo)) != 
																ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
								 _T("add Device Settings Property Sheet\n"));
				return retVal;
			}

			// 添加水印TAB
			CWatermarkDlg *pWatermarkDlg = new CWatermarkDlg(pUIContext);			
			if ((retVal = addDlgTab(pWatermarkDlg, pUIContext, pPSUIInfo)) != 
																ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
								 _T("add Device Settings Property Sheet\n"));
				return retVal;
			}

			// 添加一维条码TAB
			CBarcodeDlg *pBarcodeDlg = new CBarcodeDlg(pUIContext);			
			if ((retVal = addDlgTab(pBarcodeDlg, pUIContext, pPSUIInfo)) != 
																ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
								 _T("add Device Settings Property Sheet\n"));
				return retVal;
			}

			// 添加二维条码PDF417码TAB
			CPDF417Dlg *pPDF417Dlg = new CPDF417Dlg(pUIContext);			
			if ((retVal = addDlgTab(pPDF417Dlg, pUIContext, pPSUIInfo)) != 
																ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
								 _T("add Device Settings Property Sheet\n"));
				return retVal;
			}

			// 添加二维条码QR码TAB
			CQRCodeDlg *pQRCodeDlg = new CQRCodeDlg(pUIContext);			
			if ((retVal = addDlgTab(pQRCodeDlg, pUIContext, pPSUIInfo)) != 
																ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
								 _T("add Device Settings Property Sheet\n"));
				return retVal;
			}

			// 添加二维条码Maxi码TAB
			CMaxiCodeDlg *pMaxiCodeDlg = new CMaxiCodeDlg(pUIContext);			
			if ((retVal = addDlgTab(pMaxiCodeDlg, pUIContext, pPSUIInfo)) != 
																ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
								 _T("add Device Settings Property Sheet\n"));
				return retVal;
			}

			// 添加设置TAB
			CSettingDlg *pSettingDlg = new CSettingDlg(pUIContext);			
			if ((retVal = addDlgTab(pSettingDlg, pUIContext, pPSUIInfo)) != 
																ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
								 _T("add Device Settings Property Sheet\n"));
				return retVal;
			}

			// 添加版本TAB
			CAboutDlg *pAboutDlg = new CAboutDlg(pUIContext);			
			if ((retVal = addDlgTab(pAboutDlg, pUIContext, pPSUIInfo)) != 
																ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
								 _T("add Device Settings Property Sheet\n"));
				return retVal;
			}

#ifdef DEBUG
			// 添加调试TAB
			CDebugDlg *pDebugDlg = new CDebugDlg(pUIContext);			
			if ((retVal = addDlgTab(pDebugDlg, pUIContext, pPSUIInfo)) != 
																ERROR_SUCCESS)
			{
				TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Failed to")
								 _T("add Device Settings Property Sheet\n"));
				return retVal;
			}
#endif
			return 1;
		}
	
		case PROPSHEETUI_REASON_GET_INFO_HEADER:
		{
			// Extract the original device property header structure
			DOCUMENTPROPERTYHEADER *pDPH = 
				reinterpret_cast<DOCUMENTPROPERTYHEADER *>(pPSUIInfo->lParamInit);

			// Supply page header information as requested
			PROPSHEETUI_INFO_HEADER *pPSHeaderInfo = 
						reinterpret_cast<PROPSHEETUI_INFO_HEADER *>(lParam);
			pPSHeaderInfo->Flags = PSUIHDRF_PROPTITLE;	// | PSUIHDRF_NOAPPLYNOW;
			pPSHeaderInfo->pTitle = pDPH->pszPrinterName;
			pPSHeaderInfo->hInst = (HINSTANCE)GetResourceInst();
			pPSHeaderInfo->IconID = IDI_CPSUI_PRINTER;	// Is this correct?
		
			return 1;
		}
		
        case PROPSHEETUI_REASON_SET_RESULT:
		{
			// Execute set operation with application-supplied status
			// information
			SETRESULT_INFO *pSRInfo = reinterpret_cast<SETRESULT_INFO *>(lParam);

			TRACE(_T("CDTCUIDrv::DrvDocumentPropertySheets: pSRInfo->Result: '%s'")
				  _T(" pSRInfo->hSetResult: 0x%08lx\n"),
				  ((pSRInfo->Result == CPSUI_OK) ? _T("CPSUI_OK") :
				   (pSRInfo->Result == CPSUI_CANCEL) ? _T("CPSUI_CANCEL") :
				   (pSRInfo->Result == CPSUI_RESTARTWINDOWS) ? _T("CPSUI_RESTARTWINDOWS") :
				   (pSRInfo->Result == CPSUI_REBOOTSYSTEM) ? _T("CPSUI_REBOOTSYSTEM") :
	 					_T("UNKNOWN!!!")), pSRInfo->hSetResult);

			// Set the result into the UI info
			pPSUIInfo->Result = pSRInfo->Result;
		
			return 1;	
		}

		case PROPSHEETUI_REASON_DESTROY:
		{
			// Destroy the context
			CPdkUIContext *pUIContext = reinterpret_cast<CPdkUIContext *>(
														pPSUIInfo->UserData);

			// Destroy the context including items allocated in PROPSHEETUI_
			// REASON_INIT: config and tab sheet.
			if (pUIContext)
			{		
				delete pUIContext;

				// Undo the reference to the context
				pPSUIInfo->UserData = 0;
			}

			// Return a reasonable result 
			// GED TODO: Is this correct?
			pPSUIInfo->Result = CPSUI_OK;
	
			return 1;			
		}

		default:			
		{
			::PdkSetLastError(ERROR_INVALID_PARAMETER);
			TRACE(_T("#### CDTCUIDrv::DrvDevicePropertySheets: Invalid pPSUIInfo ")
															 _T("reason\n)"));
			return ERR_CPSUI_GETLASTERROR;
		}
	}
}

// ...........................................................................
//
// NAME:	CDTCUI::DrvDeviceCapabilities
//
// IMPLEMENTATION NOTES:
// ...........................................................................
DWORD 
CDTCUIDrv::DrvDeviceCapabilities(HANDLE hPrinter, PWSTR pDeviceName, WORD iDevCap,
							VOID *pvOutput, DEVMODE *pDevMode)
{
	OS_CAutoLock lock(m_mtxEntry);

	FUNCTION_START(bTraceOn, _T("@@@@ CPdkUI::DrvDeviceCapabilities"));
	TRACE(_T("CDTCUIDrv::DrvDeviceCapabilities: pDeviceName: '%s' iDevCap: %s")
		  _T(" pvOutput: 0x%08lx pDevMode: 0x%08lx hPrinter: 0x%08lx\n"), pDeviceName,
		  (iDevCap == DC_FIELDS ? _T("DC_FIELDS") :
			iDevCap == DC_PAPERS ? _T("DC_PAPERS") :
			iDevCap == DC_PAPERSIZE ? _T("DC_PAPERSIZE") :
			iDevCap == DC_MINEXTENT ? _T("DC_MINEXTENT") :
			iDevCap == DC_MAXEXTENT ? _T("DC_MAXEXTENT") :
			iDevCap == DC_BINS ? _T("DC_BINS") :
			iDevCap == DC_DUPLEX ? _T("DC_DUPLEX") :
			iDevCap == DC_SIZE ? _T("DC_SIZE") :
			iDevCap == DC_EXTRA ? _T("DC_EXTRA") :
			iDevCap == DC_VERSION ? _T("DC_VERSION") :
			iDevCap == DC_DRIVER ? _T("DC_DRIVER") :
			iDevCap == DC_BINNAMES ? _T("DC_BINNAMES") :
			iDevCap == DC_ENUMRESOLUTIONS ? _T("DC_ENUMRESOLUTIONS") :
			iDevCap == DC_FILEDEPENDENCIES ? _T("DC_FILEDEPENDENCIES") :
			iDevCap == DC_TRUETYPE ? _T("DC_TRUETYPE") :
			iDevCap == DC_PAPERNAMES ? _T("DC_PAPERNAMES") :
			iDevCap == DC_ORIENTATION ? _T("DC_ORIENTATION") :
			iDevCap == DC_COPIES ? _T("DC_COPIES") :
#if(WINVER >= 0x0400)
			iDevCap == DC_BINADJUST ? _T("DC_BINADJUST") :
			iDevCap == DC_EMF_COMPLIANT ? _T("DC_EMF_COMPLIANT") :
			iDevCap == DC_DATATYPE_PRODUCED ? _T("DC_DATATYPE_PRODUCED") :
			iDevCap == DC_COLLATE ? _T("DC_COLLATE") :
			iDevCap == DC_MANUFACTURER ? _T("DC_MANUFACTURER") :
			iDevCap == DC_MODEL ? _T("DC_MODEL") :
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0500)
			iDevCap == DC_PERSONALITY ? _T("DC_PERSONALITY") :
			iDevCap == DC_PRINTRATE ? _T("DC_PRINTRATE") :
			iDevCap == DC_PRINTRATEUNIT ? _T("DC_PRINTRATEUNIT") :
			iDevCap == DC_PRINTERMEM ? _T("DC_PRINTERMEM") :
			iDevCap == DC_MEDIAREADY ? _T("DC_MEDIAREADY") :
			iDevCap == DC_STAPLE ? _T("DC_STAPLE") :
			iDevCap == DC_PRINTRATEPPM ? _T("DC_PRINTRATEPPM") :
			iDevCap == DC_COLORDEVICE ? _T("DC_COLORDEVICE") :
			iDevCap == DC_NUP ? _T("DC_NUP") :
#endif /* WINVER >= 0x0500 */
			_T("Unrecognized!")), 
		  pvOutput, pDevMode, hPrinter);

//	// Create a configuration object
//	CDTCPrinterConfig ptrCfg(hPrinter);
//
//	// Create a default devmode that is composed of "factory defaults"
//	// for this driver
//	CDTCDevMode curDevMode(&ptrCfg);
//
//	// Merge with document default settings from the
//	// printer specified with pDPH->hPrinter. 
//	if (curDevMode.MergeInPrinterDocDefaults(hPrinter) != ERROR_SUCCESS)
//	{
//		TRACE(_T("#### CDTCUIDrv::DrvDeviceCapabilities: Call to")
//			  _T(" curDevMode.MergeInPrinterDocDefaults() failed!\n"));
//#if (WINVER >= 0x0500)
//		DWORD dwWinVer = GetVersion();
//		DWORD dwWinMajVer =  (DWORD)(LOBYTE(LOWORD(dwWinVer)));
//		if (dwWinMajVer < 6)
//#endif
//		return GDI_ERROR;
//	}
//		
//	// If the caller provided a dev mode, merge that in too!
//	if ((pDevMode != NULL) && (curDevMode.MergeInDevMode(pDevMode) != ERROR_SUCCESS))
//	{
//		TRACE(_T("#### CDTCUIDrv::DrvDeviceCapabilities: Call to")
//			  _T(" curDevMode.MergeInDevMode() failed!\n"));
//		return GDI_ERROR;
//	}
//				
//	// call the helper for the capabilities actually handled
//	CDTCDeviceCaps devCaps;
//	DWORD retVal;
//	int handledCode = 
//		devCaps.DeviceCapabilitiesImpl(&curDevMode, iDevCap, pvOutput, &retVal);
//
//	if (handledCode == 0)
//	{
//		TRACE(_T("CDTCUIDrv::DrvDeviceCapabilities retVal = %ld\n"), retVal);
//		return retVal;
//	}
//	else if (handledCode == 1)
//	{
//		TRACE(_T("CDTCUIDrv::DrvDeviceCapabilities: Parent class did not handle")
//		  _T(" iDevCap query %d. Attempting to handle it in prod spec code.\n"),
//		  iDevCap);
//		
//		// Attempt to handle the iDevCap query ourselves here
//		//switch(iDevCap)
//		{ 	
//			// If we had any product specific queries, they would go here.
//			//case DC_BINS:
//			//default:
//				return GDI_ERROR;
//		}
//	}
//	else	// handledCode == -1
//	{			
//		TRACE(_T("#### CDTCUIDrv::DrvDeviceCapabilities: Call to")
//			  _T(" drvDeviceCapabilitiesImpl failed!\n"));
//		return GDI_ERROR;
//	}
	return GDI_ERROR;
}

// ...........................................................................
//
// NAME:	CDTCUIDrv::DrvConvertDevMode
//
// IMPLEMENTATION NOTES:
// ...........................................................................
BOOL 
CDTCUIDrv::DrvConvertDevMode(LPTSTR pPrinterName, DEVMODE *pdmIn,
							DEVMODE *pdmOut, LONG *pcbNeeded, DWORD fMode)
{
	OS_CAutoLock lock(m_mtxEntry);

	FUNCTION_START(bTraceOn, _T("@@@@ CDTCUIDrv::DrvConvertDevMode"));
	TRACE(_T("CPdkUIDrv::DrvConvertDevMode: pPrinterName: '%s' *pcbNeeded: %ld")
		  _T(" fMode: '%s' pdmIn: 0x%08lx pdmOut: 0x%08lx.\n"),
		  pPrinterName, *pcbNeeded, 
		  ((fMode == CDM_CONVERT) ? _T("CDM_CONVERT") :
		   (fMode == CDM_CONVERT351) ? _T("CDM_CONVERT351") :
		   (fMode == CDM_DRIVER_DEFAULT) ? _T("CDM_DRIVER_DEFAULT") :
		   _T("UNKNOWN")),
		  pdmIn, pdmOut);
	

	return TRUE;
}

// ...........................................................................
//
// NAME:	CDTCUIDrv::DrvDocumentEvent
//
// IMPLEMENTATION NOTES:
// ...........................................................................
#ifdef _WIN64
#include <Psapi.h>
#endif
INT 
CDTCUIDrv::DrvDocumentEvent(HANDLE hPrinter, HDC hdc, int iEsc, ULONG cbIn, 
							ULONG *pbIn, ULONG cbOut, ULONG *pbOut)
{
	INT iRetVal = DOCUMENTEVENT_SUCCESS;
	
	return iRetVal;
}


// ----------------------------------------------------------------------------
//	     				PROTECTED METHODS
// ----------------------------------------------------------------------------

// ...........................................................................
//
// NAME:	CDTCUIDrv::addDlgTab
//
// IMPLEMENTATION NOTES:
// ...........................................................................
int
CDTCUIDrv::addDlgTab(CPdkWin *pTab, CPdkUIContext *pUIContext,
					 PROPSHEETUI_INFO *pPSUIInfo)
{
	FUNCTION_START(bTraceOn, _T("CDTCUIDrv::addDlgTab"));

	// Build Property Sheet Page structure that will build the tab
	PROPSHEETPAGE propSheetPage;
	memset(&propSheetPage, 0, sizeof(PROPSHEETPAGE));
	propSheetPage.dwSize = sizeof(PROPSHEETPAGE);
	propSheetPage.hInstance = GetPdkAppObj()->GetResourceInst();
	propSheetPage.pszTemplate = MAKEINTRESOURCE(pTab->GetDialogID());
#ifdef _WIN64
	propSheetPage.pfnDlgProc = (DLGPROC)FnPdkWinProcedure;
#else	
	propSheetPage.pfnDlgProc = FnPdkWinProcedure;
#endif	
	propSheetPage.lParam = (LPARAM)pTab;
	propSheetPage.dwFlags = PSP_DEFAULT;

	propSheetPage.hActCtx = GetMyActivationContext();
	// Set the flag to indicate that our PROPSHEETPAGE
	// has an Activation Context.
	// The Activation Context indicates with version of
	// comctl for Compstui to create our PROPSHEETPAGE
	// with. To get Themed UI we need to specify comctl v6.
	if ((NULL != propSheetPage.hActCtx) && (INVALID_HANDLE_VALUE != propSheetPage.hActCtx))
	{
		propSheetPage.dwFlags |= PSP_USEFUSIONCONTEXT;
	}

	// Add this page to the UI context. It will take ownership of the
	// pMainDlg memory.
	if (pUIContext->AddPropSheet(pTab) == 0)
	{
		::PdkSetLastError(ERROR_FUNCTION_FAILED);
		TRACE(_T("#### CDTCUIDrv::addDlgTab: Failed to add the tab.\n"));
		return ERR_CPSUI_GETLASTERROR;
	}

	// Create the page and retrieve its handle
	HPROPSHEETPAGE hPage = ::CreatePropertySheetPage(&propSheetPage);
	if (hPage == NULL)
	{
		::PdkSetLastError(ERROR_FUNCTION_FAILED);
		TRACE(_T("#### CDTCUIDrv::addDlgTab: Failed to create and obtain handle")
			  _T(" for the property sheet.\n"));
		return ERR_CPSUI_GETLASTERROR;
	}

	// Add the page through CPSUI
	PFNCOMPROPSHEET fnComPropSheet = pPSUIInfo->pfnComPropSheet;
	HPROPSHEETPAGE hCPSUIPage = reinterpret_cast<HPROPSHEETPAGE>(
		fnComPropSheet(pPSUIInfo->hComPropSheet,
					CPSFUNC_ADD_HPROPSHEETPAGE, (LPARAM)hPage, 0));
	if (hCPSUIPage == NULL)
	{
		TRACE(_T("#### CDTCUIDrv::DrvDocumentPropertySheets: Failed to")
			  _T(" add prop sheet through CPSUI ")
			  _T("ComPropSheet CPSFUNC_ADD_HPROPSHEETPAGE cmd.\n"));
		return ERR_CPSUI_GETLASTERROR;
	}
	TRACE(_T("CDTCUIDrv::DrvDocumentPropertySheets: Created ")
		  _T("prop sheet. hPage: 0x%08lx hCPSUIPage: 0x%08lx.\n"), 
		  hPage, hCPSUIPage);

	return ERROR_SUCCESS;
}