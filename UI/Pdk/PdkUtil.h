#ifndef	__PDKUTIL_H__
#define	__PDKUTIL_H__

// --------------------------------------------------------------------
//              INCLUDES
// --------------------------------------------------------------------

// --------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// --------------------------------------------------------------------

typedef struct tagArFonts{
  int        nFonts;
  int        cySpace;
  HDC        hdc;
  LOGFONT    *lf;
  TEXTMETRIC *tm;
  int        *Type;
} ARFONTS, *PARFONTS;


// --------------------------------------------------------------------
//              EXPORTED CLASSES
// --------------------------------------------------------------------
//LPSTR DigitToString(LONG no,LPSTR lpString_in,BOOL fUnicode);
//
//bool IntersectRectSSS(LPRECT16 lprcl_out,LPRECT16 lprcl_in1,LPRECT16 lprcl_in2);
//
//void WordStr2ByteStr(LPBYTE  pByte,LPSINT16 pWord,int count);

char* WStrDup2AStr(WCHAR* szWStr);
void FreeAStr(char **ppAStr);

WORD TranslMotorolaWord(WORD motorola_short);

DWORD TranslMotorolaLong(DWORD motorola_long);

#ifndef KERNEL_MODE		
int CopyUnicode2Str(PSTR pstr_out,PCWSTR pwstr_in,INT len_in);
int CopyStr2Unicode(PWSTR pwstr_out,PCSTR pstr_in,INT	len_in);//add---
#endif

BOOL BinaryToHexString(
    __in_bcount(nBinaryLen) LPBYTE  lpBinary,
    __out_bcount(nHexLen)   LPBYTE  lpHex,
                            DWORD   nBinaryLen, //  num bytes to process in lpBinary
                            DWORD   nHexLen     //  num bytes in dest buffer lpHex.
    );

BOOL HexStringToBinary(
    __in_bcount(nHexLen)     LPBYTE  lpHex,
    __out_bcount(nBinaryLen) LPBYTE  lpBinary,
                             DWORD   nHexLen,    //  num bytes in src buffer lpHex.
                             DWORD   nBinaryLen, //  num bytes in dest buffer lpBinary
    __out                    DWORD  *lpnBinBytes //  num bytes written to dest buffer lpBinary
    );

bool copyTString(const TCHAR *szSrc, TCHAR *szDest, DWORD *pBufLen, DWORD *pNeeded);

PARFONTS BuildFontList(HDC hdcIn, LPINT retnFaces);

const char* GetDriverConfig(const char* name);

// Override of MS GetPrinterData method that does NOT write into
// pData if the funciton fails!
DWORD PdkGetPrinterData(
    IN DRVPRINTER   hPrinter,
    IN LPTSTR  pValueName,
    LPDWORD  pType,
    OUT LPBYTE   pData,
    IN DWORD    nSize,
    OUT LPDWORD  pcbNeeded);

// ............................................................................
// CLASS NAME: OS_CLockable
//
// RESPONSIBILITIES:
//		Mutex object.
//
// NOTES:
// ............................................................................
class OS_CLockable 
{
	public:
		// ## Constructor / Destructor
		OS_CLockable();
		~OS_CLockable();

		// ....................................................................
		// METHOD NAME:	Acquire
		// DESCRIPTION:
		//		Acquires a lock, blocking.
		// PARAMETERS:
		//		none
		// RETURN VALUE:
		//		none
		// ....................................................................
		void Acquire();

		// ....................................................................
		// METHOD NAME:	Release
		// DESCRIPTION:
		//		Releases the lock.
		//		object.
		// PARAMETERS:
		//		none
		// RETURN VALUE:
		//		none
		// ....................................................................
		void Release();
		
	private:
        DWORD dwVersion;
        DWORD dwWindowsMajorVersion;
        DWORD dwWindowsMinorVersion;
		// ## Instance Variables
		#ifdef _WIN32
		CRITICAL_SECTION m_critSec;
		#endif

		#ifdef __INCvxWorksh
		SEM_ID m_critSec;
		#endif

        // ## declarations with no implementations (prevent accidental use)

        OS_CLockable (const OS_CLockable&);
        OS_CLockable& operator= (const OS_CLockable&);
}; 

// ............................................................................
// CLASS NAME: OS_CAutoLock
//
// RESPONSIBILITIES:
//		Provides automatic locking and unlocking of an OS_CLockable object
//		within a code scope.
//
// NOTES:
// ............................................................................
class OS_CAutoLock
{
	public:

		// ## Constructors
		OS_CAutoLock(OS_CLockable &lockable) : m_lockable(lockable)
										{lockable.Acquire(); };

		// ## Destructor
		~OS_CAutoLock() { m_lockable.Release();};

	private:

		// The controlled lockable object
		OS_CLockable &m_lockable;

        // ## declarations with no implementations (prevent accidental use)

        OS_CAutoLock ();
        OS_CAutoLock (const OS_CAutoLock&);
        OS_CAutoLock& operator= (const OS_CAutoLock&);
};


#endif
