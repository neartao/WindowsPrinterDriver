#ifndef PdkLog_H_ // (make sure this is only included once)
#define PdkLog_H_

#ifndef __cplusplus
#error PdkLog.h included in a non-C++ source file!
#endif

// --------------------------------------------------------------------
//              INCLUDES
// --------------------------------------------------------------------
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

// --------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// --------------------------------------------------------------------

// --------------------------------------------------------------------
//              EXPORTED CLASSES
// --------------------------------------------------------------------


// ....................................................................
// CLASS NAME:      PdkLog
//
// REPONSIBILITIES: Debug logging
//
// NOTES:
// ....................................................................

class PdkLog
{
public:
    // ## Constructor/Destructor
    PdkLog ();
	PdkLog (LPSTR szFileName, WORD wSwitch);
	~PdkLog ();  
	
	void deleteFile( );
	void _cdecl log( LPSTR szFormat, ... );
	void error();
	
private:
    char *m_szFile;
    int  m_bWriteFile;
    int	 m_bWriteDebug;
 
}; // end PdkLog

#endif  //  PdkLog_H_
