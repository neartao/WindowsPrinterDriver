// ----------------------------------------------------------------------------
//                   INCLUDE FILES
// ----------------------------------------------------------------------------
#include "PdkStd.h"
#include <winspool.h>
extern "C"
{
#include <lstate.h>
#include <lauxlib.h>
#include <lua.h>	
#include <lualib.h>
}

// ----------------------------------------------------------------------------
//	              GLOBAL DATA DEFINITIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//      LOCAL #DEFINES, TYPEDEF'S, and TEMPLATES
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//			LOCAL (STATIC) FUNCTIONS
// ----------------------------------------------------------------------------

static BOOL bTraceOn = TRUE;

// ----------------------------------------------------------------------------
//	     CLASS ATTRIBUTE (STATIC) INITIALIZATIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//				PUBLIC METHOD DEFINITIONS
// ----------------------------------------------------------------------------
//LPSTR DigitToString(LONG no,LPSTR lpString_in,BOOL fUnicode)
//{
//	long		base_no = 10;
//	long		base_count = 1;
//	int			string_ndx = 0;
//	LPTSTR		lpUnicodeStr = (LPTSTR)lpString_in;
//	LPSTR		lpString = lpString_in;
//	BOOL		fMinus = no < 0 ? TRUE : FALSE;
//	
//	if (fMinus)
//		no = -no;
//	while (1)
//	{
//		if (base_no > no)
//			break;
//		base_no *= 10;
//		base_count ++;
//	}
//
//	if (fMinus)
//	{
//		if (fUnicode)
//			lpUnicodeStr[string_ndx++] = L'-';
//		else
//			lpString[string_ndx++] = '-';
//	}
//	
//	// if no=12, base_no=100, base_count=2
//	while (base_count)
//	{
//		base_no /= 10;
//		if (fUnicode)
//			lpUnicodeStr[string_ndx++] = (TCHAR)(no/base_no + '0');
//		else
//			lpString[string_ndx++] = (BYTE)(no/base_no + '0');
//		no -= no/base_no*base_no;
//		base_count --;
//		if (base_count && base_count % 3 == 0)
//		{
//			if (fUnicode)
//				lpUnicodeStr[string_ndx++] = L',';
//			else
//				lpString[string_ndx++] = ',';
//		}
//	}
//	if (fUnicode)
//		lpUnicodeStr[string_ndx++] = 0;
//	else
//		lpString[string_ndx++] = 0;
//
//	return lpString_in;
//}
//
//
//
//
//bool IntersectRectSSS(LPRECT16 lprcl_out,LPRECT16 lprcl_in1,LPRECT16 lprcl_in2)
//{
//	RECT16	rc = {0,0,0,0};
//	bool	fOk = false;
//
//	if (!lprcl_in1
//		|| !lprcl_in2
//		|| lprcl_in2->left > lprcl_in2->right
//		|| lprcl_in2->top > lprcl_in2->bottom
//		|| lprcl_in1->left > lprcl_in1->right
//		|| lprcl_in1->top > lprcl_in1->bottom
//		)
//	{
//       	TRACE(_T("IntersectRectLLS meets error!\n"));
//		goto quit;
//	}
//	rc.top 	= max (lprcl_in1->top, lprcl_in2->top);
//	rc.left	= max (lprcl_in1->left, lprcl_in2->left) ;
//	rc.bottom	= min (lprcl_in1->bottom, lprcl_in2->bottom) ;
//	rc.right 	= min (lprcl_in1->right, lprcl_in2->right) ;
//	fOk = true;
//	if( (rc.top >= rc.bottom) || (rc.left >= rc.right) )
//	{
//		rc.top = rc.left = rc.bottom = rc.right = 0; 
//		fOk = false;
//	}
//quit:
//	if (lprcl_out)
//		*lprcl_out = rc;
//	return fOk;
//}
//
//


void WordStr2ByteStr(LPBYTE  pByte,LPSINT16 pWord,int count)
{
	while(count --)
	{
		*pByte = (BYTE)(*pWord);
		pByte ++;
		pWord ++;
	}
}

#ifndef WIN_95
char* WStrDup2AStr(WCHAR* szWStr)
{
	if (!szWStr)
		return NULL;

	int len = (int)wcslen(szWStr) + 1;
	char* pAStr = (char *)PdkMemAlloc((size_t)len);

	if (pAStr)
		WordStr2ByteStr((LPBYTE)pAStr, (LPSINT16)szWStr, len);

	return pAStr;
}
#endif

void FreeAStr(char **ppAStr)
{
	if (*ppAStr)
	{
		PdkMemFree(*ppAStr);
		*ppAStr = NULL;
	}
}


WORD TranslMotorolaWord(WORD motorola_short)
{
	LPBYTE	motorola_bytes = (LPBYTE)&motorola_short;

	union INTEL_MOTOROLA_DWORD
	{
		BYTE	bByte[2];
		WORD	wWord;
	} wReturn;

	wReturn.bByte[0] = motorola_bytes[1];
	wReturn.bByte[1] = motorola_bytes[0];

	return wReturn.wWord;
}

DWORD TranslMotorolaLong(DWORD motorola_long)
{
	BYTE FAR *motorola_bytes = (BYTE FAR *)&motorola_long;

	union INTEL_MOTOROLA_DWORD
	{
		BYTE	bByte[4];
		DWORD	dwDword;
	} dwReturn;

	dwReturn.bByte[0] = motorola_bytes[3];
	dwReturn.bByte[1] = motorola_bytes[2];
	dwReturn.bByte[2] = motorola_bytes[1];
	dwReturn.bByte[3] = motorola_bytes[0];

	return dwReturn.dwDword;
}


#if !defined(KERNEL_MODE) && !defined(WIN_95)		
//====================================================================
//
// NAME: CopyUnicode2Str
//
// DESCRIPTION:
//		Convert a Unicde string to an ANSI string.
//
// PARAMETERS:
//		pstr_out	Pointer to buffer for holding ANSI string
//		pwstr_in   Pointer to Unicode string
//		len_in  Maximum number of ANSI characters to copy
//				If len_in is 0 or negative, the number of characters in
//              pwstr_in is assumed.
//
// RETURN CODES:
//		Number of characters translated.
//
// NOTES:
//
//====================================================================
int CopyUnicode2Str(PSTR pstr_out,PCWSTR pwstr_in,INT len_in)
{
	//INT len = (INT)STRLEN(pwstr_in) + 1;

	//if (len_in <= 0)
	//	len_in = len;

	int len = 0;
	len = WideCharToMultiByte(CP_ACP,0,pwstr_in,-1,pstr_out,0,NULL,NULL);


	return(WideCharToMultiByte(CP_ACP,0,pwstr_in,-1,pstr_out,len,NULL,NULL));
}

int CopyStr2Unicode(PWSTR pwstr_out,PCSTR pstr_in,INT len_in)
{
	int len = 0;
	len = MultiByteToWideChar(CP_ACP,0,pstr_in,-1,pwstr_out,0);


	return (MultiByteToWideChar(CP_ACP,0,pstr_in,-1,pwstr_out,len));
}

#endif // !define(KERNEL_MODE) && !defined(WIN_95)


BOOL BinaryToHexString(
    __in_bcount(nBinaryLen) LPBYTE  lpBinary,
    __out_bcount(nHexLen)   LPBYTE  lpHex,
                            DWORD   nBinaryLen, //  num bytes to process in lpBinary
                            DWORD   nHexLen     //  num bytes in dest buffer lpHex.
    )
{
    //  how do I translate TCHAR to ascii?
    //   use  Set GetDlgItemTextA
    //  add NULL termination to lpHex

    //  return FALSE if dest buffer exhausted


    DWORD  dwSrc, dwDst ;
    BOOL   bHexmode = FALSE ;
    BYTE  Nibble ;

    for(dwSrc = dwDst = 0 ; dwSrc < nBinaryLen ; dwSrc++)
    {
        if(lpBinary[dwSrc] < 0x21  ||  lpBinary[dwSrc] > 0x7e
            ||  lpBinary[dwSrc] == '<')
        {
            //  enter hexmode if not already
            if(!bHexmode)
            {
                if(dwDst + 5 >  nHexLen)
                    return(FALSE);
                lpHex[dwDst++] = '<' ;
                bHexmode = TRUE ;
            }
            else if(dwDst + 4 >  nHexLen)
                return(FALSE);

            Nibble = (lpBinary[dwSrc]  >> 4) & 0x0f  ;
            if(Nibble < 0x0a)
                lpHex[dwDst++] = '0' + Nibble ;
            else
                lpHex[dwDst++] = 'A' + Nibble - 0x0a ;
            // loNibble
            Nibble = lpBinary[dwSrc]  & 0x0f  ;
            if(Nibble < 0x0a)
                lpHex[dwDst++] = '0' + Nibble ;
            else
                lpHex[dwDst++] = 'A' + Nibble - 0x0a ;
        }
        else
        {
            //  exit hexmode if not already
            if(bHexmode)
            {
                lpHex[dwDst++] = '>' ;
                bHexmode = FALSE ;
            }
            if(dwDst + 2 >  nHexLen)
                return(FALSE);
            lpHex[dwDst++] =  lpBinary[dwSrc];
        }
    }
    if(bHexmode)
    {
        lpHex[dwDst++] = '>' ;
        bHexmode = FALSE ;
    }
    lpHex[dwDst] = '\0' ;  // null terminate string.
    return(TRUE);
}



BOOL HexStringToBinary(
    __in_bcount(nHexLen)     LPBYTE  lpHex,
    __out_bcount(nBinaryLen) LPBYTE  lpBinary,
                             DWORD   nHexLen,    //  num bytes in src buffer lpHex.
                             DWORD   nBinaryLen, //  num bytes in dest buffer lpBinary
    __out                    DWORD  *lpnBinBytes //  num bytes written to dest buffer lpBinary
    )
{
    //  how do I translate TCHAR to ascii?
    //   use  Set GetDlgItemTextA

    //  return FALSE if dest buffer exhausted


    DWORD  dwSrc, dwDst ;
    BOOL   bHexmode = FALSE, bHiByte ;
    BYTE  Nibble ;

    for(dwSrc = dwDst = 0 ; (dwSrc < nHexLen) && lpHex[dwSrc] ; dwSrc++)
    {
        if(bHexmode)  //  hexmode processing:
                            //  recognize only 0-9, a-f, A-F and >
                            //  all other chars are ignored.
        {
            if(lpHex[dwSrc] >= '0'  &&  lpHex[dwSrc] <= '9')
            {
                //  digits
                Nibble =   lpHex[dwSrc] - '0' ;
            }
            else if(lpHex[dwSrc] >= 'a'  &&  lpHex[dwSrc] <= 'f')
            {
                //  lower case hex digits
                Nibble =   0x0a + lpHex[dwSrc] - 'a' ;
            }
            else if(lpHex[dwSrc] >= 'A'  &&  lpHex[dwSrc] <= 'F')
            {
                //  upper case hex digits
                Nibble =   0x0a + lpHex[dwSrc] - 'A' ;
            }
            else if(lpHex[dwSrc] == '>')
            {
                    bHexmode = FALSE ;
                    continue;   // do not attempt to save anything.
            }
            else
                continue;   // totally ignore unexpected characters.
            if(bHiByte)
            {
                lpBinary[dwDst] = Nibble << 4 ;
                bHiByte = FALSE ;
            }
            else  // lowByte processing
            {
                if(dwDst + 1 >  nBinaryLen)
                {
                    *lpnBinBytes = dwDst ;
                    return(FALSE);
                }
                lpBinary[dwDst++] |= Nibble ;
                bHiByte = TRUE ;
            }
        }
        else if(lpHex[dwSrc] == '<')
        {
                bHiByte = bHexmode = TRUE ;
        }
        else
        {
            if(dwDst + 1 >  nBinaryLen)
            {
                *lpnBinBytes = dwDst ;
                return(FALSE);
            }
            lpBinary[dwDst++] = lpHex[dwSrc] ;
        }
    }
    *lpnBinBytes = dwDst ;
    return(TRUE);
}

bool copyTString(const TCHAR *szSrc, TCHAR *szDest, DWORD *pBufLen, DWORD *pNeeded)
{
	DWORD remaining = (DWORD)(szSrc ? _tcslen(szSrc) : 0);

	// check input parameters
	if (szDest == NULL || pBufLen == NULL || *pBufLen == 0)
	{
		// invlaid buffer info supplied
		if (pNeeded) *pNeeded = remaining;
		return false;
	}

	// return value if available
    if (szSrc)
	{
		DWORD copied = *pBufLen - 1;
		if (remaining > copied)
		{
			// truncate the string
			_tcsncpy(szDest, szSrc, (size_t)copied);
			szDest[copied] = TCHAR(0);
			*pBufLen = copied;
			if (pNeeded) *pNeeded = remaining - copied;
		}
		else
		{
			// copy the whole thing
			_tcscpy(szDest, szSrc);
			*pBufLen = remaining;
			if (pNeeded) *pNeeded = 0;

		}
        return true;
    }
	else
	{
		*pBufLen = 0;
		if (pNeeded) *pNeeded = 0;
		return false;
	}
}

int APIENTRY MyEnumFaces(LPLOGFONT, LPTEXTMETRIC, DWORD, LPVOID);
int APIENTRY MyEnumCopy (LPLOGFONT, LPTEXTMETRIC, DWORD, LPVOID);
int APIENTRY MyEnumCount(LPLOGFONT, LPTEXTMETRIC, DWORD, LPINT);

HDC      hdcGlobal;
PARFONTS parFontsGlobal;
int      iFace,jFont;
int      nFaces;

PARFONTS BuildFontList(HDC hdcIn, LPINT retnFaces)
{
	nFaces = 0;

	/* hdcGlobal is global variable also used by the callback functions. */
	hdcGlobal = hdcIn;

	/* count the total number of face names. */
	EnumFonts (hdcGlobal, NULL, (FONTENUMPROC)MyEnumCount, (LPARAM)&nFaces);


	/* allocate the pointer to the array of PArFont structures. */
	parFontsGlobal = (PARFONTS)LocalAlloc (LPTR, sizeof(ARFONTS) * (nFaces+1));

	/* step through all fonts again.  For each one fill a LOGFONT and
	* a TEXTMETRIC stucture.
	*/
	iFace = 0;
	EnumFonts (hdcGlobal, NULL, (FONTENUMPROC)MyEnumFaces, (LPARAM)NULL);

	*retnFaces = nFaces;
	return parFontsGlobal;
}

int APIENTRY	MyEnumFaces(
							LPLOGFONT  lpLogFont,
							LPTEXTMETRIC lpTEXTMETRICs,
							DWORD  fFontType,
							LPVOID  lpData)
{
	int nFonts;


	UNREFERENCED_PARAMETER (lpTEXTMETRICs);
	UNREFERENCED_PARAMETER (fFontType);
	UNREFERENCED_PARAMETER (lpData);


	nFonts = 0;
	EnumFonts (hdcGlobal, lpLogFont->lfFaceName, (FONTENUMPROC)MyEnumCount, (LPARAM)&nFonts);


	parFontsGlobal[iFace].lf   = (LPLOGFONT)LocalAlloc (LPTR, sizeof(LOGFONT)    * nFonts);
	parFontsGlobal[iFace].tm   = (LPTEXTMETRIC)LocalAlloc (LPTR, sizeof(TEXTMETRIC) * nFonts);
	parFontsGlobal[iFace].Type = (LPINT)LocalAlloc (LPTR, sizeof(int)        * nFonts);

	if ((parFontsGlobal[iFace].lf   == NULL) ||
		(parFontsGlobal[iFace].tm   == NULL) ||
		(parFontsGlobal[iFace].Type == NULL)) {
			return FALSE;
	}

	parFontsGlobal[iFace].nFonts = nFonts;

	jFont = 0;
	EnumFonts (hdcGlobal, lpLogFont->lfFaceName, (FONTENUMPROC)MyEnumCopy, (LPARAM)NULL);

	iFace++;

	return TRUE;
}
int APIENTRY	MyEnumCopy(
						   LPLOGFONT  lpLogFont,
						   LPTEXTMETRIC lpTEXTMETRICs,
						   DWORD  fFontType,
						   LPVOID  lpData)
{
	LOGFONT    *lplf;
	TEXTMETRIC *lptm;
	int        *pType;

	UNREFERENCED_PARAMETER (lpData);

	lplf  = parFontsGlobal[iFace].lf;
	lptm  = parFontsGlobal[iFace].tm;
	pType = parFontsGlobal[iFace].Type;

	lplf[jFont]  = *lpLogFont;
	lptm[jFont]  = *lpTEXTMETRICs;
	pType[jFont] = fFontType;

	jFont++;
	return TRUE;
}
int APIENTRY	MyEnumCount(
							LPLOGFONT  lpLogFont,
							LPTEXTMETRIC lpTEXTMETRICs,
							DWORD  fFontType,
							LPINT  lpData)
{
	UNREFERENCED_PARAMETER (lpLogFont);
	UNREFERENCED_PARAMETER (lpTEXTMETRICs);
	UNREFERENCED_PARAMETER (fFontType);

	(*lpData)++;
	return TRUE;
}

// Override of MS GetPrinterData method that does NOT write into
// pData if the funciton fails!
DWORD PdkGetPrinterData(
    IN DRVPRINTER   hPrinter,
    IN LPTSTR  pValueName,
    LPDWORD  pType,
    OUT LPBYTE   pData,
    IN DWORD    nSize,
    OUT LPDWORD  pcbNeeded)
{
	//FUNCTION_START(bTraceOn, _T("PdkGetPrinterData"));

	#ifndef _WIN32
	if (pType == NULL)
	{
		TRACE(_T("#### PdkGetPrinterData: pType parameter is required for 9X."));
		return ERROR_INVALID_PARAMETER;
	}
	#endif // !_WIN32
	
	DWORD retVal;

	#ifdef _WIN32
		// Call GetPrinterData to find availability and size of key
		DWORD regType;
		retVal =
		#ifdef KERNEL_MODE
			EngGetPrinterData(hPrinter, pValueName, &regType, pData, 0, pcbNeeded);
		#else // !KERNEL_MODE
			GetPrinterData(hPrinter, pValueName, &regType, pData, 0, pcbNeeded);
		#endif

		if (retVal == ERROR_FILE_NOT_FOUND)
		{
			//TRACE(_T("#### PdkGetPrinterData: Key '%s' not found.\n"), pValueName);
			return retVal;
		}

		if (retVal != ERROR_MORE_DATA)
		{
			TRACE(_T("#### PdkGetPrinterData: Unexpected retVal: %ld. Should be")
				  _T(" ERROR_MORE_DATA.\n"), retVal);
			return retVal;
		}
		if (*pcbNeeded != nSize)
		{
			TRACE(_T("#### PdkGetPrinterData: Sizes don't match! *pcbNeeded: %ld")
				  _T(" nSize: %ld\n"), *pcbNeeded, nSize);
			return ERROR_MORE_DATA;
		}
	#endif // _WIN32
	
	// Call GetPrinterData to get the actual data value
	retVal =
	#ifdef _WIN32
		#ifdef KERNEL_MODE
			EngGetPrinterData(hPrinter, pValueName, pType, pData, nSize, pcbNeeded);
		#else // !KERNEL_MODE
			GetPrinterData(hPrinter, pValueName, pType, pData, nSize, pcbNeeded);
		#endif
	#else // !_WIN32
			DrvGetPrinterData(hPrinter, pValueName, pType, pData, nSize, pcbNeeded);

			if (retVal == ERROR_CANTREAD)
			{
				//TRACE(_T("#### PdkGetPrinterData: Key '%s' not found.\n"), pValueName);
				return ERROR_FILE_NOT_FOUND;
			}
	#endif

	if (retVal != NO_ERROR)
	{
		TRACE(_T("#### PdkGetPrinterData: Failure getting actual data!")
			  _T(" retVal: %ld\n"), retVal);
		return retVal;
	}

	return NO_ERROR;
}

// ----------------------------------------------------------------------------
//	              OS_CLockable METHOD DEFINITIONS
// ----------------------------------------------------------------------------

// ...........................................................................
//
// NAME:	OS_CLockable::OS_CLockable		(Constructor)
//
// IMPLEMENTATION NOTES:
// ...........................................................................
OS_CLockable::OS_CLockable()
{
#if !defined(KERNEL_MODE) && !defined(WIN_95)
    dwVersion = GetVersion();
    dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
    if(dwWindowsMajorVersion < 6)
	::InitializeCriticalSection(&m_critSec);
#endif // !defined(KERNEL_MODE) && !defined(WIN_95)
	
	return;
};

// ...........................................................................
//
// NAME:	OS_CLockable::~OS_CLockable		(Destructor)
//
// IMPLEMENTATION NOTES:
// ...........................................................................
OS_CLockable::~OS_CLockable()
{
#if !defined(KERNEL_MODE) && !defined(WIN_95)
    if(dwWindowsMajorVersion < 6)
	::DeleteCriticalSection(&m_critSec);
#endif // !defined(KERNEL_MODE) && !defined(WIN_95)
	
	return;
};

// ...........................................................................
//
// NAME:	OS_CLockable::Acquire
//
// IMPLEMENTATION NOTES:
// ...........................................................................
void
OS_CLockable::Acquire()
{
#if !defined(KERNEL_MODE) && !defined(WIN_95)
    if(dwWindowsMajorVersion < 6)
	::EnterCriticalSection(&m_critSec);
#endif // !defined(KERNEL_MODE) && !defined(WIN_95)
};

// ...........................................................................
//
// NAME:	OS_CLockable::Release
//
// IMPLEMENTATION NOTES:
// ...........................................................................
void
OS_CLockable::Release()
{
#if !defined(KERNEL_MODE) && !defined(WIN_95)
    if(dwWindowsMajorVersion < 6)
	::LeaveCriticalSection(&m_critSec);
#endif // !defined(KERNEL_MODE) && !defined(WIN_95)
};

/*	config.lua
	entity={
		["drivername"]="DJ-1580",
	};

	function getDriverConfig(name)
		return entity[name];
	end
*/
const char* GetDriverConfig(const char* name)
{
	lua_State * L = lua_open();        //创建lua运行环境
	luaL_openlibs(L);

	int ret = 0 ;
	ret = luaL_loadfile(L,"config.lua") ;      //加载lua脚本文件
	ret = lua_pcall(L,0,0,0) ;
	lua_getglobal(L, "getDriverConfig");             //函数名
	lua_pushlstring(L, name, strlen(name));

	//运行函数并把结果压入栈  
	lua_call(L, 1, 1);
	const char * c=(const char *)lua_tostring(L,-1);
	lua_pop(L, 1);
	lua_close(L) ;                     //释放lua运行环境
	return c;
}