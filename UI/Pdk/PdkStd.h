#ifndef PdkStandardDrvHeader_H_ // (make sure this is only included once)
#define PdkStandardDrvHeader_H_

#ifndef __cplusplus
#error PdkStd.h included in a non-C++ source file!
#endif

// --------------------------------------------------------------------
//              INCLUDES
// --------------------------------------------------------------------
#pragma warning (disable:4103) // 'filename' : used #pragma pack to change alignment
#pragma warning (disable:4242) // warning C4244: '=' : conversion from 'int' to 'short', possible loss of data
#pragma warning (disable:4244) // warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
#pragma warning (disable:4311) // warning C4311: 'type cast' : pointer truncation from 'HANDLE' to 'int'
#pragma warning (disable:4313) // warning C4313: '_snwprintf' : '%x' in format string conflicts with argument 1 of type 'HWND'

#include "PdkDef.h"	 // Standard includes and defs

#if (!defined(WIN_95) && !defined(WIN_NT))
#pragma message("***STDDrv.h: Must define one of WIN_95 or WIN_NT\n")
#endif

//#define NO_GDIDEFS

#ifdef WIN_95
	#ifndef NO_GDIDEFS

		#define PRINTDRIVER
		#define NOPQ
		#define NOINSTALLFILES         
		#include <print.h>	
		#include <gdidefs.inc>	
	
		typedef struct tagBITMAPFILEHEADER
		{
    		UINT    bfType;
    		DWORD   bfSize;
    		UINT    bfReserved1;		
    		UINT    bfReserved2;
    		DWORD   bfOffBits;
		} BITMAPFILEHEADER;
		typedef BITMAPFILEHEADER*      PBITMAPFILEHEADER;
		typedef BITMAPFILEHEADER FAR* LPBITMAPFILEHEADER;
	
		/* Brush Styles */
		#define BS_SOLID        0
		#define BS_NULL         1
		//#define BS_HOLLOW       BS_NULL
		#define BS_HATCHED      2
		#define BS_PATTERN      3
		#define BS_INDEXED      4
		#define BS_DIBPATTERN   5
	
		/* Pen Styles */
		#define PS_SOLID        0
		#define PS_DASH         1
		#define PS_DOT          2
		#define PS_DASHDOT      3
		#define PS_DASHDOTDOT   4
		#define PS_NULL         5
		#define PS_INSIDEFRAME  6
	
		/* Hatch Styles */
		#define HS_HORIZONTAL   0
		#define HS_VERTICAL     1
		#define HS_FDIAGONAL    2
		#define HS_BDIAGONAL    3
		#define HS_CROSS        4
		#define HS_DIAGCROSS    5
		#define HS_API_MAX      6
	
	#else
		#include <print.h>	

		typedef struct tagDRAWMODE {
		  short    Rop2;           // see below
		  short    bkMode;         // see below
		  unsigned long  bkColor;        // see below
		  unsigned long  TextColor;      // see below
		  short    TBreakExtra;    // see below
		  short    BreakExtra;     // see below
		  short    BreakErr;       // see below
		  short    BreakRem;       // see below
		  short    BreakCount;     // see below
		  short    CharExtra;      // see below
		  unsigned long  LbkColor;       // see below
		  unsigned long  LTextColor;     // see below
		  DWORD    ICMCXform;      // see below
		  short    StretchBltMode; // see below
		  DWORD    eMiterLimit;    // see below
		} DRAWMODE, * LPDRAWMODE; 
		
		#define TC_TT_ABLE	0x0002	   /* can do TT through DDI or brute       */
		#define RESETDEVICE		     128
	#endif

	typedef DEVMODE DEVMODEA;
	typedef DEVMODEA * PDEVMODEA, NEAR * NPDEVMODEA, FAR * LPDEVMODEA;
	typedef VOID FAR * PSECURITY_DESCRIPTOR;     
	typedef ULONG ACCESS_MASK;
	typedef struct _SYSTEMTIME {
		WORD wYear;
		WORD wMonth;
		WORD wDayOfWeek;
		WORD wDay;
		WORD wHour;
		WORD wMinute;
		WORD wSecond;
		WORD wMilliseconds;
	} SYSTEMTIME, * PSYSTEMTIME, FAR * LPSYSTEMTIME;

	#define PdkGetLastError GetLastError	
	#define PdkSetLastError SetLastError	
	#define PdkSetPrinterData DrvSetPrinterData
	#define PdkWritePrinter WritePrinter
	//#include <windowsx.h>
	#define	PdkAllocPtr(size)  GlobalAllocPtr(GMEM_MOVEABLE, size)
	#define	PdkFreePtr(ptr)    GlobalFreePtr(ptr)
	#define	PdkReAllocPtr(ptr, size)  GlobalReAllocPtr(ptr, size, GMEM_MOVEABLE)
	#define PdkGlobalLock(hBuf)      GlobalLock(hBuf)
	#define PdkGlobalUnlock(hBuf)    GlobalUnlock(hBuf)
	#define PdkGlobalFree(hBuf)      GlobalFree(hBuf)
	#define PdkGlobalAlloc(size, flags)   GlobalAlloc(flags, size)
	#define PdkGlobalReAlloc(hBuf, size, flags)  GlobalReAlloc(hBuf, size, flags)
	#define MEMALLOC			PdkAllocPtr
	#define MEMFREE				PdkFreePtr
	#define MEMSET				PdkMemSet
	#define	MEMCMP				_fmemcmp

	#define UNREFERENCED_PARAMETER(P) \
    	{ \
    	    (P) = (P); \
	    } \
    	
	#define	STRNCPYA	lstrcpyn
	#define	STRCPYA		lstrcpy
	#define	STRCPY		lstrcpy

	#define	STRLENA		lstrlen
	#define	STRLEN		STRLENA

    #define	STRCAT		lstrcat
    #define	STRCATA		lstrcat
    #define	STRCMPA		_fstrcmp
    #define	STRCMP		_fstrcmp
    #define	STRNCMP		_fstrncmp
    #define	STRNICMP	_fstrnicmp
    #define	STRSTR		_fstrstr    
	
	#ifndef ULONG_PTR
	#define ULONG_PTR	ULONG
	#endif

	extern "C"
	{
	typedef int (CALLBACK* FONTENUMPROC)(const LPENUMLOGFONT, 
										 const LPNEWTEXTMETRIC, int, LPARAM);
	int     WINAPI EnumFontFamilies(HDC, LPCSTR, FONTENUMPROC, LPSTR);
	HFONT   WINAPI CreateFontIndirect(const LOGFONT FAR*);
	HGDIOBJ WINAPI SelectObject(HDC, HGDIOBJ);
	BOOL    WINAPI DeleteObject(HGDIOBJ);
	COLORREF WINAPI SetTextColor(HDC, COLORREF);
	int     WINAPI SetBkMode(HDC, int);
	}

#endif

#ifdef WIN_NT
/*	#define		EndSpoolPage(x)
	#define		CloseJob(x)			1
	#define		DeleteJob(a,b)		1
	#define		OpenJob(a,b,c)		c
	#define		StartSpoolPage(a)	a*/
	
	#ifdef KERNEL_MODE		
		#define NT_KERNEL 1		
		#include <windef.h>		
		#include <winbase.h>		
		#include <winerror.h>
		#include <wingdi.h>
		extern "C"
		{
			#include <winddi.h>
		}
		#include "WinSpool.h"

		#define PdkGetLastError EngGetLastError	
		#define PdkSetLastError EngSetLastError	
		#define PdkGetPrinter EngGetPrinter	
		#define PdkSetPrinter EngSetPrinter		
		#define PdkSetPrinterData EngSetPrinterData
		#define PdkWritePrinter EngWritePrinter

		#define	PdkAllocPtr(size)  new BYTE[size]
		#define	PdkFreePtr(ptr)    delete [] ptr

		#define MEMALLOC                PdkAllocPtr
		#define MEMFREE                 PdkFreePtr
		#define MEMDUMP                 DebugPrintMemoryObjects
		#define	MEMCMP					memcmp

		#define PdkFreeGlyphSet(ptr)    EngFreeMem(ptr)

		#define FREEMODULE              EngFreeModule

		#define	STRLENA		strlen   
		#define	STRLENW		wcslen
		#define	STRLEN		_tcslen
		#define STRCAT		_tcscat
		#define	STRCATA		strcat
		#define	STRNCPYA	strncpy
		#define	STRCPYA		strcpy
		#define	STRCPY		_tcscpy
		#define	STRNICMP	_strnicmp

		#define MULTIBYTETOUNICODE  EngMultiByteToUnicodeN
		#define UNICODETOMULTIBYTE  EngUnicodeToMultiByteN

	#else // not KERNEL_MODE
		#include <windows.h>
		#include <windef.h>
		#include <winerror.h>
		#include <wingdi.h>
		//extern "C"
		//{
			#include <winddi.h>
		//}

		#include <compstui.h>
		#include <winddiui.h>
		//#include <windowsx.h>

		#define PdkGetLastError GetLastError	
		#define PdkSetLastError SetLastError	
		#define PdkGetPrinter GetPrinter
		#define PdkSetPrinter SetPrinter		
		#define PdkSetPrinterData SetPrinterData
		#define PdkWritePrinter WritePrinter
		

		#define MEMALLOC(size)          PdkMemAlloc(size)
		#define MEMFREE(ptr)            PdkMemFree(ptr)
		#define MEMDUMP                 PdkMemDump
		#define	MEMCMP					memcmp

		#define	PdkAllocPtr(size)       PdkMemAlloc(size)
		#define	PdkFreePtr(ptr)         PdkMemFree(ptr)		
		#define PdkFreeGlyphSet(ptr)    PdkMemFree(ptr)

		#define HEAPCREATE()            HeapCreate(0, 8192, 0)
		#define HEAPDESTROY(hheap)      HeapDestroy(hheap)
		#define HEAPALLOC(hheap,size)   HeapAlloc(hheap, 0, size)
		#define HEAPFREE(hheap,ptr)     HeapFree(hheap, 0, (PVOID) (ptr))

		#define FREEMODULE(hmodule)     UnmapViewOfFile((PVOID) (hmodule))

		#define	STRLENA		strlen   
		#define	STRLENW		wcslen
		#define	STRLEN		_tcslen
		#define STRCAT		_tcscat
		#define	STRCATA		strcat
		#define	STRNCPYA	strncpy
		#define	STRCPYA		strcpy
		#define	STRCPY		_tcscpy
		#define	STRNICMP	_strnicmp

		#define MULTIBYTETOUNICODE  RtlMultiByteToUnicodeN
		#define UNICODETOMULTIBYTE  RtlUnicodeToMultiByteN

	#endif // KERNEL_MODE	            
#endif // WIN_NT

#ifdef WIN_TEST
		#include "windows.h"

		typedef struct tagDRAWMODE {
		  short    Rop2;           // see below
		  short    bkMode;         // see below
		  unsigned long  bkColor;        // see below
		  unsigned long  TextColor;      // see below
		  short    TBreakExtra;    // see below
		  short    BreakExtra;     // see below
		  short    BreakErr;       // see below
		  short    BreakRem;       // see below
		  short    BreakCount;     // see below
		  short    CharExtra;      // see below
		  unsigned long  LbkColor;       // see below
		  unsigned long  LTextColor;     // see below
		  DWORD    ICMCXform;      // see below
		  short    StretchBltMode; // see below
		  DWORD    eMiterLimit;    // see below
		} DRAWMODE, * LPDRAWMODE;

		//#include "windowsx.h"
		#define	PdkAllocPtr(size)  GlobalAllocPtr(GPTR, size)
		#define	PdkFreePtr(ptr)    GlobalFreePtr(ptr)

#endif

void DebugPrintMemoryObjects ();

#ifndef PVOID
	typedef void FAR*           PVOID;
#endif

// BEGIN TJT- ADDED during initial port to PDK
// See ending comment below
#pragma pack(1)	// Tandy: Set here for Win2K
typedef short int INT16;
typedef short int FAR*  LPINT16;
typedef int INT;

typedef unsigned char UBYTE8;
typedef unsigned short int UINT16;
typedef short int SINT16, FAR *LPSINT16;

#if	_WIN32
	typedef struct _RECT16
	{	
		INT16	left;
		INT16	top;
		INT16	right;
		INT16	bottom;
	} RECT16, FAR *LPRECT16;
	
	typedef struct
	{	INT16	x;
		INT16	y;
	} POINT16, FAR *LPPOINT16;
	#define		FP_SEG(x)		((LONG_PTR)x>>16)
	#define		FP_OFF(x)		((LONG_PTR)x&0xFFFF)

#else

	typedef RECT	RECT16;
	typedef LPRECT	LPRECT16;
	
	typedef POINT	POINT16;
	typedef LPPOINT	LPPOINT16;
	// copy from <\WINDDK\2462\inc\ntdef.h>
	#define FIELD_OFFSET(type, field)    ((LONG)(LONG_PTR)&(((type *)0)->field))
	
	#define		FP_SEG(x)		((WORD)(((DWORD)x) >>16))
	#define		FP_OFF(x)		((WORD)(((DWORD)x) & 0xFFFF))
#endif	// _WIN32

#define OS_ALTPOLYPOLYGON   0x4016 // win95
#define OS_WINDPOLYPOLYGON  0x4014 // win95
typedef struct tagOSPOLYPOLY {
	LPLONG lpNumPts ;  // number of POINT structures
	LPPOINT16 lppt;	  // points descripbing the polygons
} OSPOLYPOLY , FAR *LPOSPOLYPOLY;

typedef		LPSTR	FAR	*LPLPSTR;
#ifdef WIN_95
typedef		BYTE	__huge *HPBYTE;
#else
typedef     BYTE         *HPBYTE;
#endif

#define		SWAP(a,b)   {(a) ^= (b); (b) ^=(a); (a) ^=(b);}
#define		DIM(x)	(sizeof(x)/sizeof(x[0]))

#define		BITSET(a,bitPos)		(a |= ((DWORD)1<<(bitPos)))
#define		BITCLEAR(a,bitPos)		(a &= ~((DWORD)1<<(bitPos)))
#define		ISBITSET(a,bitPos)		(a & ((DWORD)1<<(bitPos)))

// The following macro is used to divide fix numbers with
// rounding to the nearest whole number
#define		PDK_DIVIDE(num,den)		(((2*(num)) + (den)) / (2*(den)))

// Result of string comparison
#define EQUAL_STRING	0

// Here lies all the defines related to File IO
#if _WIN32
	#ifdef KERNEL_MODE
		// Tandy: copy from	win2kddk <winddi.h>
		#if	!defined(NT500)
			#define		XFORML			XFORM
			#define		FLOATL			FLOAT
			#define		FO_DBCS_FONT	0X01000000
			#define		BMF_JPEG		9L
			#define		BMF_PNG			10L
			typedef		BOOL   (APIENTRY *PFN_DrvEnableDriver)(ULONG,ULONG,PDRVENABLEDATA);
			#define		DRIVER_OS		"WinNT"
		#else
			#define		DRIVER_OS		"Win2000"
		#endif

		#define		HFILE				HANDLE
		#define		OPENFILE_ERROR		NULL
		#define		CREATEFILE_ERROR	NULL
		#define	WSPRINTF				WSPRINTFW
		INT	WVSPRINTFW(
			PWSTR	buf,
			PCWSTR   fmtstr,
			va_list arglist
		);

		INT	WSPRINTFW(
			PWSTR	buf,
			PCWSTR   fmtstr,
			...
		);

		INT		WSPRINTFA(
			PSTR	buf,
			PCSTR   fmtstr,
			...
		);
		INT	WVSPRINTFA(
			PSTR	buf,
			PCSTR   fmtstr,
			va_list arglist
		);

		PWSTR CopyStr2Unicode(PWSTR pwstr_out,PCSTR pstr_in,INT	len_in);
		int   CopyUnicode2Str(PSTR pstr_out,PCWSTR pwstr_in,INT len_in);

	#else	// _WIN32 or NT500
		#define		HFILE				HANDLE
		#define	GETSYSTEMDIRECTORY(a,b)		GetSystemDirectory(a,b)
		#define	WVSPRINTFA					wvsprintfA
		#define	WSPRINTFA					wsprintfA
		#define	WSPRINTF					wsprintf
		#define	OPENFILE_ERROR				INVALID_HANDLE_VALUE
		#define	CREATEFILE_ERROR			INVALID_HANDLE_VALUE
		#define		DRIVER_OS		"Win2000"
	#endif
#else		// 16-bit
	#define OPENFILEW(a)			_lopen(a, OF_WRITE)	//add for security (Gillem)
	#define	OPENFILE_NtNoPath(a)	_lopen(a, OF_READ)
	#define	OPENFILE_ERROR			HFILE_ERROR
	#define	CREATEFILE_ERROR		HFILE_ERROR
	#define	WVSPRINTFA				wvsprintf
	#define	WSPRINTFA				wsprintf
	#define	WSPRINTF				wsprintf
	#define	GETSYSTEMDIRECTORY(a,b)	GetSystemDirectory(a,b)
	
    int WriteToFile(HFILE hFile,BYTE __huge *lpData, DWORD dwSize);
#endif  //!KERNEL_MODE for File IO



// END TJT


// Project stuff that doesn't change
#include "PdkDbg.h" 
#include "PdkMem.h"
#include "PdkUtil.h"

// --------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// --------------------------------------------------------------------

#endif  //  PdkStandardDrvHeader_H_
