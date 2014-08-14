// Identification:	%M PdkTrace.cpp M% %II%
// Checked out:		%TT% %HH%
// Last modified:	%UU% %GG%
//
// %C Copyright (C) 2004 InSciTek Microsystems, Inc. C%
//
// MODULE DESCRIPTION:
//
//		
// REVISION HISTORY (MOST RECENT FIRST)
//
//  %LL%
//
//  07/02/2004  Guy
//  - original
//

// ----------------------------------------------------------------------------
//              INCLUDE FILES                  
// ----------------------------------------------------------------------------

#include "PdkStd.h" 
#ifndef	KERNEL_MODE
	#include "PdkLog.h"
#endif
#ifdef	KERNEL_MODE
extern "C" {
//	#include "utils.h"
}
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

BOOL	g_bLog					= TRUE;
BOOL	g_bTrace				= TRUE;
BOOL	g_bTraceFunctionEntry	= TRUE;
BOOL	g_bTraceFinisher		= FALSE;

#undef _TEMP_LOG
#ifdef _TEMP_LOG
HANDLE g_dbgLogFile = NULL;
OS_CLockable g_mtxEntry;
#endif // _TEMP_LOG

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

#ifdef _DEBUG  
	extern "C" 
	void cdecl PdkTrace(const TCHAR * lpszFormat, ...)
	{
		if (g_bTrace)
		{
			va_list args;
			va_start(args, lpszFormat);
	
#if KERNEL_MODE
			CHAR	achFormat[256];
			CHAR	achMod[256];
			ULONG	len;

			memset(achFormat, 0, 256);
			memset(achMod, 0, 256);
			EngUnicodeToMultiByteN(achFormat, 255, &len, (WCHAR *)lpszFormat, wcslen(lpszFormat) * 2);
			EngUnicodeToMultiByteN(achMod, 255, &len, g_szModule, wcslen(g_szModule) * 2);
			achFormat[255] = '\0';
			strcat(achMod, ": ");
			EngDebugPrint(achMod, achFormat, args);
#else		
			int nBuf = 0;
			TCHAR szBuffer[BUFSIZE];

			// Output string
			#ifndef WIN_95
				nBuf = wsprintf(szBuffer, _T("<0x%lx>:"), GetCurrentThreadId());
			#endif // WIN_95
			nBuf += wsprintf(&szBuffer[nBuf], _T("%s: "), g_szModule);
			// Indent two spaces if this is not an Enter/Exit message
			if ((lpszFormat[0] != _T('E')) || (
					(lpszFormat[1] != _T('x')) && (lpszFormat[1] != _T('n'))))
			{
				nBuf += wsprintf(&szBuffer[nBuf], _T("  "));
			}
			nBuf += wvsprintf(&szBuffer[nBuf], lpszFormat, args);
			if(nBuf >= sizeof(szBuffer))
			{
				szBuffer[BUFSIZE - 1] = _T('\0');  //make sure null terminated
				ASSERT(nBuf < sizeof(szBuffer));
			}	
			
			#if 0
			if (g_bLog)
			{
				PdkLog log;
				log.log("%s %s %s" ,g_szModule, _T(": "), szBuffer);
            }
			#endif // 0
	
			OutputDebugString(szBuffer);

#ifdef _TEMP_LOG
			if (!g_dbgLogFile)
			{
				TCHAR fileName[BUFSIZE];
				_tcscpy(fileName, _T("C:\\dtc"));
				_ultot(GetCurrentProcessId(), &fileName[_tcslen(fileName)], 10);
				_tcscat(fileName, _T(".log"));
				g_dbgLogFile = ::CreateFile(fileName, GENERIC_READ|GENERIC_WRITE,
								   FILE_SHARE_WRITE|FILE_SHARE_READ, NULL,
								   OPEN_ALWAYS, 0, NULL);
			}
			if (g_dbgLogFile)
			{
				OS_CAutoLock lock(g_mtxEntry);
				char aszBuf[BUFSIZE];
				CopyUnicode2Str(aszBuf, szBuffer, BUFSIZE-1);
				aszBuf[BUFSIZE-1] = 0;
				DWORD bytesWritten = 0;
				::WriteFile(g_dbgLogFile, aszBuf, strlen(aszBuf), &bytesWritten, NULL);
				::FlushFileBuffers(g_dbgLogFile);
			}
#endif // _TEMP_LOG

#endif
			va_end(args);
		}
	}
#endif
