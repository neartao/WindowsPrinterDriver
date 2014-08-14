// ----------------------------------------------------------------------------
//              INCLUDE FILES                  
// ----------------------------------------------------------------------------

#include "PdkStd.h"  
#include "process.h"
#include "stdlib.h"

// ----------------------------------------------------------------------------
//              GLOBAL DATA DEFINITIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              LOCAL #DEFINES, TYPEDEF'S, and TEMPLATES             
// ----------------------------------------------------------------------------

#pragma optimize("qgel", off) // assembler cannot be globally optimized

// ----------------------------------------------------------------------------
//              LOCAL (STATIC) FUNCTIONS             
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              CLASS ATTRIBUTE (STATIC) INITIALIZATIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              CLASS METHOD / FUNCTION DEFINITIONS
// ----------------------------------------------------------------------------

// Enter Debugging
//DebugBreak
//DbgBreakPoint
//_asm {int 3};

// Output to Debug Window
//DbgPrint
//OutputDebugString


extern "C"
void PdkAssertFailedLine(const TCHAR * lpszFileName, int nLine)
{

#ifdef _DEBUG

#ifndef NT_KERNEL
	TCHAR sz[BUFSIZE];
	static TCHAR szTitle[] = _T("Assertion Failed!");
	static TCHAR szMessage[] = _T("%s: File %s, Line %d, %s\r\n");
	static TCHAR szUnknown[] = _T("<unknown application>");

	// assume the debugger or auxiliary port
#ifdef WIN_95
	wsprintf(sz, szMessage, szUnknown, lpszFileName, nLine, szTitle);
#endif

#ifdef WIN_NT
	_stprintf(sz, szMessage, szUnknown, lpszFileName, nLine, szTitle);
#endif
	::OutputDebugString(sz);

	
#ifdef WIN_95
	wsprintf(sz, (LPCTSTR)szMessage, (LPCTSTR)szUnknown, lpszFileName, nLine);
#endif

#ifdef WIN_NT
	_stprintf(sz, (LPCTSTR)szMessage, (LPCTSTR)szUnknown, lpszFileName, nLine);
#endif

	_tcscat (sz, _T("\r\n Turn I3HERE ON in SoftIce to catch break during retry!"));

	int nCode = ::MessageBox(::GetDesktopWindow(), sz, szTitle,
			MB_SYSTEMMODAL | MB_ICONHAND | MB_ABORTRETRYIGNORE);

	if (nCode == IDIGNORE)
	{
		return;     // ignore
	}
	else if (nCode == IDRETRY)
	{
		// break into the debugger (or Dr Watson log)
#ifndef _WIN64
		_asm { int 3 };
#endif
		return; // ignore and continue in debugger to diagnose problem
	}
	// else fall through and call abort

#else
	TRACE (_T("Assertion Failed: file %Fs, line %d\r\n"), lpszFileName, nLine);
	EngDebugBreak();
	
#endif // _WINDOWS

	 // If DLL unload
     // Causes Rundll exception, but unloads DLL.         	
     //if(g_hInstance)
     //     	FreeLibrary((HINSTANCE)g_hInstance);	 
     	
	 abort();
	 
     
#else
	// parameters not used if non-debug
	(void)lpszFileName;
	(void)nLine;
#endif // _DEBUG

}

#pragma optimize("", on)
