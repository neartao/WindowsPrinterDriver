// Identification:	%M PdkDbg.h M% %I  I%
// Checked out:		%TT% %HH%
// Last modified:	%U  U% %G  G%
//
// %C Copyright (C) 2004 InSciTek Microsystems, Inc. C%
//
// HEADER FILE DESCRIPTION:
//
//		
// REVISION HISTORY (MOST RECENT FIRST)
//
//  %L PdkDbg.h L%
//
//  07/02/2004  Guy
//  - original
//

#ifndef _PDKDEBUG__H
#define _PDKDEBUG__H

#include "PdkDef.h"

#ifndef __cplusplus
#error PdkDbg.h included in a non-C++ source file!
#endif

#ifndef MessageLog
#ifdef _DEBUG
#define MessageLog(str) \
    {   \
        HANDLE hfile = ::CreateFile(TEXT("E:\\PC4SC4.log"), GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  \
        if(CREATEFILE_ERROR != hfile)    \
        {   \
            DWORD dwwrited = 0; \
            ::SetFilePointer(hfile, 0, NULL, FILE_END);    \
            ::WriteFile(hfile, (LPBYTE)(str), strlen(str), &dwwrited, NULL);    \
            ::CloseHandle(hfile); \
        }   \
    }
#else
#define MessageLog(str) 
#endif
#endif
// --------------------------------------------------------------------
//              INCLUDES
// --------------------------------------------------------------------

// --------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// --------------------------------------------------------------------

#ifdef _DEBUG

#	define MESSAGEBEEP(n)			{ 	OutputDebugString(__FILE__); \
										OutputDebugString("\n");     \
										MessageBeep(n); }
#	define INT_3 _asm int 3

void VerifyPad();
#else

#	define MESSAGEBEEP(n)			(0)
#	define INT_3 

#endif // _DEBUG


typedef enum 
{
	NOTINIT = -1,
	NOLOG   = 0,	// None
	LOGFILE	= 1,	// File
	LOGDEBUG = 2	// OutputDebugString
} LOGTYPE;

#ifdef __cplusplus
extern "C"
{
#endif

	void RemoveLog();
	void __cdecl LogData(LPSTR format, ...);
	void __cdecl LogHexData (char * dataPtr, DWORD dataCnt);	

#ifdef __cplusplus
}
#endif

extern	BOOL	g_bLog;
extern	BOOL	g_bTrace;
extern	BOOL	g_bTraceFunctionEntry;
extern	BOOL	g_bTraceFinisher;


// --------------------------------------------------------------------
//              EXPORTED CLASSES
// --------------------------------------------------------------------

#ifdef	__cplusplus


// ....................................................................
// CLASS NAME:      CPushTrace
//
// REPONSIBILITIES:
//
// NOTES:
// ....................................................................
class CPushTrace
{
public:
	CPushTrace(BOOL bNewVal)	{m_bSaveTraceFlag = g_bTrace;	g_bTrace = bNewVal;};
	~CPushTrace()				{g_bTrace = m_bSaveTraceFlag;};

	// helper methods to turn the flag on/off inside a function
	void	Toggle()			{g_bTrace = !g_bTrace;};
	void	Set(BOOL bNewVal)	{g_bTrace = bNewVal;};

private:
	BOOL	m_bSaveTraceFlag;
};

#ifdef KERNEL_MODE
	#define kErrMsgEnter	_T("Enter: %S\n")
	#define kErrMsgExit		_T("Exit : %S\n")
#else
	#define kErrMsgEnter	_T("Enter: %s\n")
	#define kErrMsgExit		_T("Exit : %s\n")
#endif

// ....................................................................
// CLASS NAME:      CFunctionEntryExitTrace
//
// REPONSIBILITIES: function entry/exit tracing
//
// NOTES:
// ....................................................................
class CFunctionEntryExitTrace
{
public:
	CFunctionEntryExitTrace(TCHAR * fnname, BOOL bLocalTFE=FALSE) {m_saveFnname = fnname; m_bLocalTFE = bLocalTFE; if (g_bTraceFunctionEntry||bLocalTFE) TRACE(kErrMsgEnter, fnname);};
	~CFunctionEntryExitTrace() { if (g_bTraceFunctionEntry||m_bLocalTFE) TRACE(kErrMsgExit, m_saveFnname); };
private:
	TCHAR FAR *	m_saveFnname;
	BOOL		m_bLocalTFE;
};

#ifdef	_DEBUG
	// a generic 'push' of the trace flag, so we can trace parts of a function/module
	#define	PUSHTRACE(b)		CPushTrace	_pushtrace(b)

	#define	FUNCTION_START(b,s)	CPushTrace	_pushtrace(b);\
								CFunctionEntryExitTrace	_FunctionEntryExitTrace(s);
	#define	FUNC_START(b1,s,b2)	CPushTrace	_pushtrace(b1);\
								CFunctionEntryExitTrace	_FunctionEntryExitTrace(s, b2);
#else
	#define	PUSHTRACE(b)
	#define	FUNCTION_START(b,s)
	#define	FUNC_START(b1,s,b2)
#endif	// debug

#else	// !__cplusplus
	#define	PUSHTRACE(b)	
	#define	FUNCTION_START(b,s)
	#define	FUNC_START(b1,s,b2)
	#define	FUNCTION_END(s)
#endif	// _cplusplus


#endif //_PDKDEBUG__H
