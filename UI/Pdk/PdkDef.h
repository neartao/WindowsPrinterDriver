#ifndef PdkDef_H_ // (make sure this is only included once)
#define PdkDef_H_

// --------------------------------------------------------------------
//              INCLUDES
// --------------------------------------------------------------------
//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//#define STRICT
#define _WTL_USE_CSTRING

#define _ATL_USE_CSTRING
#define _ATL_USE_CSTRING_FLOAT
#define _ATL_USE_DDX_FLOAT

#include <atlbase.h>
#include <atlapp.h>
#include <shellapi.h>
extern CAppModule _Module;
#include <atlwin.h>
#include <atlframe.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlddx.h>

#if _ATL_VER < 0x0700
#undef BEGIN_MSG_MAP
#define BEGIN_MSG_MAP(x) BEGIN_MSG_MAP_EX(x)
#endif

// Necessary for compiling under VC.
#if(!defined(WINVER) || (WINVER < 0x0500))
	#undef WINVER
	#define WINVER          0x0500
#endif
#if(!defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0500))
	#undef _WIN32_WINNT
	#define _WIN32_WINNT    0x0500
#endif

// Isolation define for using ComCtrl v6.
#ifndef ISOLATION_AWARE_ENABLED
    #define ISOLATION_AWARE_ENABLED
#endif

// We want strict type casting
#ifndef STRICT
    #define STRICT
#endif // STRICT

#include <stddef.h>
#ifdef	__cplusplus
   #include <new.h>
#endif
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <sys\types.h>
#include <memory.h>
#include <winerror.h>

#include <STDDEF.H>
#include <STDLIB.H>
#include <OBJBASE.H>
#include <STDARG.H>
#include <STDIO.H>
#include <WINDEF.H>
#include <WINERROR.H>
#include <WINBASE.H>
#include <TCHAR.H>
#include <EXCPT.H>
#include <ASSERT.H>


#ifndef WIN_95
#pragma warning(disable : 4005)
#pragma warning(disable : 4996)
#endif
// --------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// --------------------------------------------------------------------
extern TCHAR * g_szModule;

#ifdef WIN_95
	extern TCHAR g_DecimalSeperator;

	typedef char				CHAR;
	typedef char *			 	LPTSTR;
	typedef const char *	   	LPCTSTR;
	typedef unsigned long		ULONG;
	typedef short *				LPSHORT;
	typedef float               FLOAT;
	typedef FLOAT				FLOATL;

	typedef int	bool;
	#define true 1
	#define false 0

	#ifndef IN
	#define IN
	#endif
	
	#ifndef OUT
	#define OUT
	#endif

	#ifndef CONST
	#define CONST               const
	#endif

	typedef unsigned long SIZE_T;
	typedef unsigned int HDEV;
	typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
	typedef WCHAR *PWCHAR;
	typedef WCHAR __far * LPWCH;
	typedef CONST WCHAR __far * LPCWCH;
	typedef CONST WCHAR __far * PCWSTR;
	typedef WCHAR *NWPSTR;
	typedef WCHAR __far * LPWSTR;	
	typedef CONST WCHAR __far * LPCWSTR;
	
	typedef double LONGLONG;
	typedef double ULONGLONG;
	typedef LONGLONG *PLONGLONG;
	typedef ULONGLONG *PULONGLONG;

	#define DRVPRINTER LPSTR
#endif

#define MAX_FILE 260
#define BUFSIZE 512 
#define CCHBINNAME		24
#define CCHDEVCAPSTR	64
#define _MAX_PATH 260
#define _MAX_PORT  32  
#define _MAX_TABS  16

#define PDK_NONE	(-10) // must not interfer with CB_ERR, etc.

typedef struct tagLONGPOINT  	
{					
    long x; 
    long y;

} LONGPOINT, * LPLONGPOINT;

typedef struct  tagLONGRECT
{
	long	top;
	long	left;
	long	bottom;
	long	right;
} LONGRECT, * LPLONGRECT;

// Remove __huge for NT.
#ifdef WIN_NT
	#define __huge 
//	#define _fmemcpy memcpy
//	#define _fmemset memset
//	#define _fmemcmp memcmp
#endif

#ifdef WIN_95
	typedef	char __huge*	HPTR;
#endif
#ifdef	WIN_NT
	typedef	char *	HPTR;
	#define DRVPRINTER HANDLE
#endif

#define ABS(Value) ((Value) > 0 ? (Value) : (-(Value)))
#define WITHIN(value,lower,upper) ((value) >= (lower) && (value) <= (upper))
// determine number of elements in an array (not bytes)
#define _countof(array) (sizeof(array)/sizeof(array[0]))

#if (WINVER >= 0x0600) || (_MSC_VER >= 1400)
#define _export 
#endif

//#if( _MSC_VER < 1000 )  // If using Visual C++ version earlier than 4.0 ...
//	// Visual C++ 4.0 already has this defined.
//	inline void *operator new(size_t size, void *ptr) {return ptr;}
//#endif // _MSC_VER < 1000

// Borland C++ stuff
#ifdef __BORLANDC__
   #define _chsize chsize
   #define _atoi atoi
   #define _itoa itoa
   typedef UINT FAR*  PUINT;
#endif


// Be sure we are compiling in either LARGE or HUGE memory model.
#ifndef _WIN32
	#ifdef __BORLANDC__
		#if !defined( __LARGE__ )  &&  !defined( __HUGE__ )
			#error ATTENTION! You should compile using LARGE or HUGE memory models.
		#endif
	#else
		#if !defined( M_I86LM )  &&  !defined( M_I86HM )
			#error ATTENTION! You should compile using LARGE or HUGE memory models.
		#endif
	#endif
#endif


// ******************** Debugging Support **********************************

// Allows the user of the standard Visual C++ Debug mode to trigger our
// debugging environment.
#ifdef _DEBUG
    #define DEBUG
#endif

#define FILLER      0xCCCC

#define FILL_INT8   0xCC
#define FILL_INT16  0xCCCC
#define FILL_INT32  0xCCCCCCCC



////////////////////////////////////////////////////////////////////////////
// ASSERT Diagnostic support

#ifdef _DEBUG	
	#define THIS_FILE          _T(__FILE__)
	#ifdef ASSERT
		#undef ASSERT
	#endif
	#define ASSERT(f)          ((f) ? (void)0 : \
							   ::PdkAssertFailedLine(THIS_FILE, __LINE__))
	#define VERIFY(f)          ASSERT(f)

#ifdef	__cplusplus
	extern "C"
	{
#endif
		void cdecl  PdkTrace(const TCHAR * pszFormat, ...);
		void PdkAssertFailedLine(const TCHAR * lpszFileName, int nLine);
#ifdef	__cplusplus
	}
#endif
	#define TRACE              /*__noop*/PdkTrace
	
#else
	#ifdef ASSERT
		#undef ASSERT
	#endif
#if (WINVER >= 0x0600)
	#define ASSERT(f)          __noop
#else
	#define ASSERT(f)          ((void)0)
#endif
	#define VERIFY(f)          ((void)(f)) 
#ifdef	__cplusplus
#define	INLINE	inline
#else
#define	INLINE
#endif
#ifndef _WIN32
	INLINE void cdecl PdkTrace(const TCHAR * pszFormat, ...){};
	#define TRACE	          PdkTrace
#else
#if (WINVER >= 0x0600) || defined(_WIN64)
	#define TRACE	          __noop
#else
	#define TRACE	          __noop//((void)0)
#endif
#endif // WIN32

#endif

#define		SEEKFILE_BEGIN		0
#define		SEEKFILE_CURRENT	1
#define		SEEKFILE_END		2


#include "PdkDbg.h"

#endif //PdkDef_H_
