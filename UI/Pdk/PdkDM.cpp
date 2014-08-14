// ----------------------------------------------------------------------------
// Identification:	%M PdkDM.cpp M% %II%
// Checked out:		%TT% %HH%
// Last modified:	%UU% %GG%
//
// %C Copyright (C) 2004 InSciTek Microsystems, Inc. C%
//
// MODULE DESCRIPTION:
//		This class is used to manipulate DEVMODE structures. This base
//		class is intended to provide common behavior/implementation to
//		all product specific devmode classes.
//		
// REVISION HISTORY (MOST RECENT FIRST)
//
//  %LL%
//
//  	07/02/2004  InSciTek
//  		- original
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              INCLUDE FILES                  
// ----------------------------------------------------------------------------

#include "PdkStd.h"
#include "PdkDM.h"

#ifdef WIN_95
	#include <winnet.h>
#endif    

// ----------------------------------------------------------------------------
//              GLOBAL DATA DEFINITIONS
// ----------------------------------------------------------------------------

// Set to true to turn on this category of debug output
static BOOL	m_bTracePdkDevMode = FALSE;

// ----------------------------------------------------------------------------
//              LOCAL #DEFINES, TYPEDEF'S, and TEMPLATES             
// ----------------------------------------------------------------------------

// Constants defining public devmode versions in the DEVMODE::dmSpecVersion field
#define DM_SPECVERSION320   0x0320
#define DM_SPECVERSION400   0x0400
#define DM_SPECVERSION401   0x0401
#define DM_SPECVER_BASE     DM_SPECVERSION320

// Constants for the size of the device name and form name strings
#define CCHDEVICENAME320   32
#define CCHFORMNAME320     32

// Various versions of the public portions of DEVMODE structs are repeated
// here in order to determine their size.
struct DEVMODE320A 
{
    BYTE    dmDeviceName[CCHDEVICENAME320];
    WORD    dmSpecVersion;
    WORD    dmDriverVersion;
    WORD    dmSize;
    WORD    dmDriverExtra;
    DWORD   dmFields;
    short   dmOrientation;
    short   dmPaperSize;
    short   dmPaperLength;
    short   dmPaperWidth;
    short   dmScale;
    short   dmCopies;
    short   dmDefaultSource;
    short   dmPrintQuality;
    short   dmColor;
    short   dmDuplex;
    short   dmYResolution;
    short   dmTTOption;
    short   dmCollate;
    BYTE    dmFormName[CCHFORMNAME320];
    WORD    dmLogPixels;
    DWORD   dmBitsPerPel;
    DWORD   dmPelsWidth;
    DWORD   dmPelsHeight;
    DWORD   dmDisplayFlags;
    DWORD   dmDisplayFrequency;
};

struct DEVMODE320W 
{
    WCHAR   dmDeviceName[CCHDEVICENAME320];
    WORD    dmSpecVersion;
    WORD    dmDriverVersion;
    WORD    dmSize;
    WORD    dmDriverExtra;
    DWORD   dmFields;
    short   dmOrientation;
    short   dmPaperSize;
    short   dmPaperLength;
    short   dmPaperWidth;
    short   dmScale;
    short   dmCopies;
    short   dmDefaultSource;
    short   dmPrintQuality;
    short   dmColor;
    short   dmDuplex;
    short   dmYResolution;
    short   dmTTOption;
    short   dmCollate;
    WCHAR   dmFormName[CCHFORMNAME320];
    WORD    dmLogPixels;
    DWORD   dmBitsPerPel;
    DWORD   dmPelsWidth;
    DWORD   dmPelsHeight;
    DWORD   dmDisplayFlags;
    DWORD   dmDisplayFrequency;
};

struct DMEXTRA400 
{
    DWORD  dmICMMethod;
    DWORD  dmICMIntent;
    DWORD  dmMediaType;
    DWORD  dmDitherType;
    DWORD  dmICCManufacturer;
    DWORD  dmICCModel;
};

struct DMEXTRA401 
{
    DWORD  dmPanningWidth;
    DWORD  dmPanningHeight;
};

#ifdef UNICODE
typedef DEVMODE320W     DEVMODE320;
#else
typedef DEVMODE320A     DEVMODE320;
#endif // UNICODE

// The sizes of the known DEVMODE structures by version.
#define DM_SIZE320  sizeof(DEVMODE320)
#define DM_SIZE400  (DM_SIZE320 + sizeof(DMEXTRA400))
#define DM_SIZE401  (DM_SIZE400 + sizeof(DMEXTRA401))

#ifndef ERROR_FUNCTION_FAILED
#define ERROR_FUNCTION_FAILED 1627L
#endif

// ----------------------------------------------------------------------------
//              LOCAL (STATIC) FUNCTIONS             
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              CLASS ATTRIBUTE (STATIC) INITIALIZATIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              		PUBLIC METHOD DEFINITIONS
// ----------------------------------------------------------------------------