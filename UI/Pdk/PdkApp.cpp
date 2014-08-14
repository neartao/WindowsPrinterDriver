
// ----------------------------------------------------------------------------
//              INCLUDE FILES                  
// ----------------------------------------------------------------------------

#include "PdkStd.h"
#include "PdkMem.h"
#include "PdkApp.h"

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

// ----------------------------------------------------------------------------
//              CLASS METHOD / FUNCTION DEFINITIONS
// ----------------------------------------------------------------------------

// ...........................................................................
//
// NAME:	CPdkApp::CPdkApp
//
// IMPLEMENTATION NOTES:
// ...........................................................................
CPdkApp::CPdkApp () : m_hInst(NULL), m_hInstRes(NULL)
{
	m_szModName[0] = 0;
	m_szAppName[0] = 0;
	m_szResourceDLLPath[0] = 0;

	return;
};

// ...........................................................................
//
// NAME:	CPdkApp::~CPdkApp
//
// IMPLEMENTATION NOTES:
// ...........................................................................
CPdkApp::~CPdkApp ()
{
	return;
};

// ...........................................................................
//
// NAME:	CPdkApp::InitApp
//
// IMPLEMENTATION NOTES:
// ...........................................................................
int 
CPdkApp::InitApp(HINSTANCE hInst, const TCHAR *szModName, 
						const TCHAR *szAppName, const TCHAR *szResourceDLLPath)
{
	m_hInst = hInst;

	if (szModName)
		_tcscpy(m_szModName, szModName);
	if (szAppName)
		_tcscpy(m_szAppName, szAppName);
	if (szResourceDLLPath)
		_tcscpy(m_szResourceDLLPath, szResourceDLLPath);

	return ERROR_SUCCESS;
}

// ...........................................................................
//
// NAME:	CPdkApp::GetInst
//
// IMPLEMENTATION NOTES:
// ...........................................................................
HINSTANCE
CPdkApp::GetInst() const
{
	return m_hInst;
}

#ifndef KERNEL_MODE
// ...........................................................................
//
// NAME:	CPdkApp::GetResourceInst
//
// IMPLEMENTATION NOTES:
// ...........................................................................
HINSTANCE
#ifndef WIN_95
	CPdkApp::GetResourceInst() const
#else
	CPdkApp::GetResourceInst()
#endif
{
	// If we have not yet loaded the resource DLL yet, do it now
	if (m_hInstRes == NULL)
	{
		// If we have an empty path, then use m_hInst
		if (_tcslen(m_szResourceDLLPath) == 0)
		{
			m_hInstRes = m_hInst;
		}
		else
		{
			TRACE(_T("PdkApp::GetResourceInst: Loading resource DLL from '%s' now.\n"),
				  m_szResourceDLLPath);

			// Execute a load library to bring in the DLL
			if ((m_hInstRes = LoadLibrary(m_szResourceDLLPath)) == NULL)
			{
				m_hInstRes = m_hInst;

				TRACE(_T("#### PdkApp::GetResourceInst: Failed to load resource DLL"));
			}
		}
	}

	return m_hInstRes;
}

// ...........................................................................
//
// NAME:	CPdkApp::GetVerString
//
// IMPLEMENTATION NOTES:
// ...........................................................................
bool 
CPdkApp::GetVerString(unsigned int maxStrSize, TCHAR *szComments,
					  TCHAR *szCompanyName, TCHAR *szFileDescription,
					  TCHAR *szFileVersion, TCHAR *szLegalCopyright,
					  TCHAR *szProductName, TCHAR *szProductVersion)
{
    struct TTransArray
    {
        WORD LanguageID, CharacterSet;
    };

	// Get pathname of this executable
	// If hInst is NULL, GetModuleFileName will retrieve the module name of 
	// the executable of this process.
	TCHAR szModuleFullPath[256];
	szModuleFullPath[0] = _T('\0');
	::GetModuleFileName(m_hInst, szModuleFullPath, sizeof(szModuleFullPath));
	TRACE(_T("CPdkApp::GetVerString: szModuleFullPath: '%s'\n"), 
		  szModuleFullPath);

	// Get size of the current file version structure
	DWORD dwVerHnd;
	DWORD verInfoSize = ::GetFileVersionInfoSize(szModuleFullPath, &dwVerHnd);
	if ((verInfoSize == 0) || (maxStrSize == 0))
	{
		TRACE(_T("#### CPdkApp::GetVerString: verInfoSize: %ld, ")
			  _T("maxStrSize: %d\n"), verInfoSize, maxStrSize);
		return false;
	}
    
	// Allocate space for file information block and retrieve it
	char *pMem;				// Pointer to mem cpy of version resource
	TCHAR *szCurInfo = NULL;// Pointer into pMem block for currently requested
							// information.
	unsigned int curInfoLen;	// Length of info in szCurInfo in characters
	if ((pMem = (char *)PdkMemAlloc((size_t)verInfoSize)) == NULL)
	{
		TRACE(_T("#### CPdkApp::GetVerString: failed to alloc.\n"));
		return false;
	}

	if (::GetFileVersionInfo(szModuleFullPath, dwVerHnd, verInfoSize, pMem) == 0)
	{
		TRACE(_T("#### CPdkApp::GetVerString: GetFileVersionInfo() returned zero.\n"));
		return false;
	}

	// Get the array of available languages for this version block
	::VerQueryValue(pMem, _T("\\VarFileInfo\\Translation"), 
											(void **)&szCurInfo, &curInfoLen);
	TTransArray *arrAvailLangs = (TTransArray *)szCurInfo;

	// Get the Comments information.
	TCHAR szQuery[256];
	if (szComments)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\Comments"),
					arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szComments, szCurInfo, min(maxStrSize, curInfoLen));
			szComments[maxStrSize - 1] = '\0';
		}
	}

	// Get the CompanyName information
	if (szCompanyName)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\CompanyName"),
					arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szCompanyName, szCurInfo, min(maxStrSize, curInfoLen));
			szCompanyName[maxStrSize - 1] = '\0';
		}
	}

	// Get the FileDescription information
	if (szFileDescription)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\FileDescription"),
					arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szFileDescription, szCurInfo, min(maxStrSize, curInfoLen));
			szFileDescription[maxStrSize - 1] = '\0';
		}
	}

	// Get the FileVersion information
	if (szFileVersion)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\FileVersion"),
					arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szFileVersion, szCurInfo, min(maxStrSize, curInfoLen));
			szFileVersion[maxStrSize - 1] = '\0';
		}
	}

	// Get the LegalCopyright information
	if (szLegalCopyright)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\LegalCopyright"),
					arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szLegalCopyright, szCurInfo, min(maxStrSize, curInfoLen));
			szLegalCopyright[maxStrSize - 1] = '\0';
		}
	}

	// Get the ProductName information
	if (szProductName)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\ProductName"),
					arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szProductName, szCurInfo, min(maxStrSize, curInfoLen));
			szProductName[maxStrSize - 1] = '\0';
		}
	}

	// Get the ProductVersion information
	if (szProductVersion)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\ProductVersion"),
					arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szProductVersion, szCurInfo, min(maxStrSize, curInfoLen));
			szProductVersion[maxStrSize - 1] = '\0';
		}
	}

	// Free the space for file information block
	PdkMemFree(pMem);

	return true;
}
#endif // !KERNEL_MODE

