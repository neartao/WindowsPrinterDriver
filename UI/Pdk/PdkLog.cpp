// ----------------------------------------------------------------------------
//              INCLUDE FILES                  
// ----------------------------------------------------------------------------
#ifndef WIN_95
#include "PdkDef.h"
#else
#include "PdkStd.h"
#endif
#include "PdkLog.h"

// ----------------------------------------------------------------------------
//              GLOBAL DATA DEFINITIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              LOCAL #DEFINES, TYPEDEF'S, and TEMPLATES             
// ----------------------------------------------------------------------------

#ifndef DRV_LOG_FILE
#define DRV_LOG_FILE "C:\\Drv.LOG"
#endif

// ----------------------------------------------------------------------------
//              LOCAL (STATIC) FUNCTIONS             
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              CLASS ATTRIBUTE (STATIC) INITIALIZATIONS
// ----------------------------------------------------------------------------

char * g_szLogFile = DRV_LOG_FILE;

// ----------------------------------------------------------------------------
//              CLASS METHOD / FUNCTION DEFINITIONS
// ----------------------------------------------------------------------------

PdkLog::PdkLog ()
{
	// constructor code here
	m_szFile = new char[512];
	strncpy(m_szFile, g_szLogFile, 512);

	m_bWriteFile 	= 0;
	m_bWriteDebug 	= 0;
	
	#ifdef _DEBUG_FILE
		m_bWriteFile 	= 1;
	#endif
	#ifdef _DEBUG_OUTPUT
		m_bWriteDebug 	= 1;
	#endif

};

PdkLog::PdkLog (LPSTR szFileName, WORD iSwitch)
{
	// constructor code here
	m_szFile = new char[512];
	strncpy(m_szFile, szFileName, 512);

	m_bWriteFile 	= 0;
	m_bWriteDebug 	= 0;

	if (iSwitch & 1)
		m_bWriteFile 	= 1;	
	if (iSwitch & 2)
		m_bWriteDebug 	= 1;

};


PdkLog::~PdkLog()
{
	delete [] m_szFile;
	// destructor code here

};

void PdkLog::deleteFile( )
{
#ifndef KERNEL_MODE
	remove(m_szFile);
#endif
}

void _cdecl PdkLog::log( LPSTR szFormat, ... )
{

#ifdef	WIN_NT
	// Add logging later
#else
	LPSTR szDebug = (LPSTR)PdkAllocPtr(1024);
	char szDate[9];
	char szTime[9];	        
	_strdate( szDate );
	_strtime( szTime ); 
	va_list va; 
	va_start(va, szFormat); 
	
	if (m_bWriteDebug)
	{
	    // Out to debug window
		wvsprintf(szDebug + lstrlen(szDebug), szFormat, (LPSTR)va);
		lstrcat(szDebug, "\n");
		OutputDebugString(szDebug);
	}
				
	#ifndef KERNEL_MODE
	if(m_bWriteFile)
	{
		// Out to File
		wsprintf(szDebug, "%s %s - ", szDate, szTime); 
		wvsprintf(szDebug + lstrlen(szDebug), szFormat, (LPSTR)va);
		//lstrcat(szDebug, "\r\n");	Let user put in string

		HFILE hfFile;
	    hfFile = _lopen(m_szFile, WRITE); 
	    if (hfFile == HFILE_ERROR)
	    {	
	    	// If open fails file may not exist
	    	hfFile = _lcreat(DRV_LOG_FILE, 0); 
	    }
	    if (hfFile != HFILE_ERROR) 
	    {
	        _llseek(hfFile, 0L, 2) ;
	       
	       	// replace \n with \r\n		
			LPSTR pos1 = szDebug;
			LPSTR pos2 = szDebug;
			while (pos2 = strchr(pos2, '\n'))
			{
				_lwrite(hfFile, pos1, (pos2 - pos1));			
				_lwrite(hfFile, "\r\n", 2);	
				pos1 = pos2 + 1;
				pos2++;
			}
			_lwrite(hfFile, pos1, strlen(pos1));			

	        _lclose(hfFile) ;
	    }
	}
	#endif

	PdkFreePtr(szDebug);
#endif

}

void PdkLog::error()
{
	log("Error File:%s Line:%d", __FILE__, __LINE__);
}

