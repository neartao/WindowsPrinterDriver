// ----------------------------------------------------------------------------
//              INCLUDE FILES                  
// ----------------------------------------------------------------------------
#include "PdkStd.h"
#include "PdkWin.h"
#include "PdkUICtx.h"
#include "PdkDbg.h"
#include <limits.h>

// ----------------------------------------------------------------------------
//              GLOBAL DATA DEFINITIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              LOCAL #DEFINES, TYPEDEF'S, and TEMPLATES             
// ----------------------------------------------------------------------------

#ifndef ERROR_FUNCTION_FAILED
#define ERROR_FUNCTION_FAILED 1627L
#endif

// ----------------------------------------------------------------------------
//              LOCAL (STATIC) FUNCTIONS             
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              CLASS ATTRIBUTE (STATIC) INITIALIZATIONS
// ----------------------------------------------------------------------------

BOOL CPdkUIContext::bTraceOn = TRUE;

// ----------------------------------------------------------------------------
//              CLASS METHOD / FUNCTION DEFINITIONS
// ----------------------------------------------------------------------------

// ...........................................................................
//
// NAME:	Constructor
//
// IMPLEMENTATION NOTES:
// ...........................................................................
CPdkUIContext::CPdkUIContext(POEMUIPSPARAM pParam) :
	m_bDeviceContext(true), m_bUpdatePermission(false), m_bModified(false),
	m_numPropSheets(0),  m_fMode(0)
{
	FUNCTION_START(bTraceOn, _T("CPdkUIContext::CPdkUIContext(DEVICE SETTINGS)"));

	m_hPrinter = pParam->hPrinter;

	_tcscpy_s(m_szDriverName, MAX_PATH, pParam->pPrinterName);
	m_szDriverName[_tcslen(pParam->pPrinterName)] = 0x00;

	return;
}

// ...........................................................................
//
// NAME:	CPdkUIContext::~CPdkUIContext		(Destructor)
//
// IMPLEMENTATION NOTES:
// ...........................................................................
CPdkUIContext::~CPdkUIContext()
{
	FUNCTION_START(bTraceOn, _T("CPdkUIContext::~CPdkUIContext"));

	// Destroy tab sheet
	for(int i = 0; i < NumPropSheets(); ++i)
	{
		CPdkWin *pPropSheet = GetPropSheet(i);
		if (pPropSheet != NULL)
			delete pPropSheet;
	}

	return;
}

// ...........................................................................
//
// NAME:	CPdkUIContext::SetPermission
//
// IMPLEMENTATION NOTES:
// ...........................................................................
void
CPdkUIContext::SetPermission(bool bUpdatePermission)
{
	// Set the information into the object
	m_bUpdatePermission = bUpdatePermission;

	return;
}

// ...........................................................................
//
// NAME:	CPdkUIContext::AddPropSheet
//
// IMPLEMENTATION NOTES:
// ...........................................................................
int
CPdkUIContext::AddPropSheet(CPdkWin *pPropSheet)
{
	FUNCTION_START(bTraceOn, _T("CPdkUIContext::AddPropSheet"));

	// We use a very simple static allocation scheme here, so we cannot add
	// more than a reasonable maximum number of sheets.
	if (m_numPropSheets >= MAX_PROP_SHEETS)
	{
		TRACE(_T("#### CPdkUIContext::AddPropSheet: Attempting to add more ")
			  _T("than %d property sheets. Too many!\n"), MAX_PROP_SHEETS);

		return 0;
	}

	// Add the property sheets
	m_arrPropSheets[m_numPropSheets] = pPropSheet;
	++m_numPropSheets;

	TRACE(_T("CPdkUIContext::AddPropSheet: Added %d sheets after this call.\n"),
		  m_numPropSheets);

	return m_numPropSheets;
}

// ...........................................................................
//
// NAME:	CPdkUIContext::NumPropSheets / GetPropSheet
//
// IMPLEMENTATION NOTES:
// ...........................................................................
int
CPdkUIContext::NumPropSheets() const
{
	return m_numPropSheets;
}

CPdkWin *CPdkUIContext::GetPropSheet(int idx) const
{
	if ((idx >= m_numPropSheets) || (idx < 0))
	{
		TRACE(_T("#### CPdkUIContext::NumPropSheets: Index %d does not exist!\n"),
			  idx);
		return NULL;
	}

	return m_arrPropSheets[idx];
}

// ...........................................................................
//
// NAME:	CPdkUIContext::SetModified
//
// IMPLEMENTATION NOTES:
// ...........................................................................
void
CPdkUIContext::SetModified(bool bChanged /* = true */, 
											bool bMatchUserSet /* = false */)
{
	FUNCTION_START(bTraceOn, _T("CPdkUIContext::SetModified"));

	TRACE(_T("UI Ctxt set to '%s' state and bMatchUserSet is '%s'.\n"), 
		  ((bChanged) ? _T("Modified") : _T("Unmodified")),
		  ((bMatchUserSet) ? _T("TRUE") : _T("FALSE")));
	m_bModified = bChanged;
#if 0   // Benjamin mask it for PC4SC4 project
	// Set the user set modified flag to signal that the UI differs from the
	// user setting selected.
	if (bMatchUserSet == false)
	{
		m_bUserSetModified = true;
		m_pDMCurObj->SetUserSetName(CPdkDevMode::USN_UNTITLED);
	}
#endif
	return;
}

// ...........................................................................
//
// NAME:	CPdkUIContext::IsModified
//
// IMPLEMENTATION NOTES:
// ...........................................................................
bool
CPdkUIContext::IsModified() const
{
	return m_bModified;
}

// ...........................................................................
//
// NAME:	GetDriverName
//
// IMPLEMENTATION NOTES:
// ...........................................................................
bool CPdkUIContext::GetDriverName(TCHAR *szBuf, DWORD *pBufLen,
															DWORD *pNeeded)
{
	return copyTString((const TCHAR *)m_szDriverName, szBuf, pBufLen, pNeeded);
}
