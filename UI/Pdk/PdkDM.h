#ifndef PDKDM_H_ 
#define PDKDM_H_

#ifndef __cplusplus
#error PdkDM.h included in a non-C++ source file!
#endif

// ----------------------------------------------------------------------------
//              INCLUDES
// ----------------------------------------------------------------------------

#ifdef WIN_NT
#include <WinDef.h>
#include <WinGDI.h>
#include <winspool.h>
#elif defined(WIN_95)
#include <print.h>
#include "PdkDef.h"
#else
#error "Must define an operating system"
#endif

// ----------------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// ----------------------------------------------------------------------------

// Driver version number for DEVMODE_PDKEXT::version. Defines the version
// of the Pdk that supports the product specific code
#define PDK_VERSION      	(0x0001)

// Pdk signature for DEVMODE_PDKEXT::signature so that we can recognize our
// own dev modes.
#define PDK_DRIVER_SIGNATURE (0x6B647544)  // Signature: 'kdpD' - 'Dpdk' in mem


// ............................................................................
// STRUCTURE NAME: 		DEVMODE_PDKEXT
//
// RESPONSIBILITIES:
// 		The DEVMODE extension structure used to hold information common to
//		all Pdk derived product specific drivers in the private areas of the
//		DEVMODE structure. It is a "header" structure to this private area.
//
// NOTES:
// ............................................................................
typedef struct
{
	// Size of this structure. When this structure changes, it should only
	// add fields to the end of this structurre and increment pdkVersion field
	DWORD pdkSize;

	// Pdk signature
	DWORD   pdkSignature;

	// Pdk Version Number
	DWORD   pdkVersion;

	// Driver family signature. This should be constant for a particular
	// family of drivers. If the driver for this family increases its version,
	// then the product specific portion of the devmode should increase in
	// size, keeping all fields from the previous version in place. Then,
	// the DEVMODE::dmDriverVersion constant should be incremented. However,
	// this field is left the same so that we can recognize this devmode as
	// coming from the same family of drivers
	DWORD productSignature;
	
	// Name of this DM's user set
	TCHAR szUserSetName[CCHFORMNAME];

} DEVMODE_PDKEXT, FAR * LPDEVMODE_PDKEXT;


// ............................................................................
// STRUCTURE NAME: 		DEVMODEPDK
//
// RESPONSIBILITIES:
// 		The following structure, DEVMODEPDK, is the base header for the product
// 		specific devmode structure.  Each product specific devmode structure 
// 		must start with the exact same fields as this structure.  Both 
// 		structures can be used to overlay the same data.
//
// NOTES:
// ............................................................................
typedef struct
{
	// Public (OS-defined) portion of the DEVMODE
    DEVMODE dmPublic; 

	// Pdk standard header that begins the private portion of the DEVMODE
	// for every instance of a product specific driver. 
	DEVMODE_PDKEXT dmPdkInfo; 
	
} DEVMODEPDK, FAR * LPDEVMODEPDK;

// --------------------------------------------------------------------
//              EXPORTED CLASSES
// --------------------------------------------------------------------

#ifdef	WIN_NT
	#ifdef	MAKING_DRVUILIB
		#ifdef DrvUILIB_EXPORTS
			#undef	DRVUILIB_API
			#define DRVUILIB_API __declspec(dllexport)
		#else
			#undef	DRVUILIB_API
			#define DRVUILIB_API __declspec(dllimport)
		#endif
	#else
		#define	DRVUILIB_API
	#endif
#else
	#define	DRVUILIB_API
#endif


#endif  //  PDKDM_H_
