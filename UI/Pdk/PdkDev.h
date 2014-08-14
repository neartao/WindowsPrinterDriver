// Identification:	%M PdkDev.h M% %I  I%
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
//  %L PdkDev.h L%
//
//  07/02/2004  Guy
//  - original
//

#ifndef CPdkDevice_H_ // (make sure this is only included once)
#define CPdkDevice_H_

#ifndef __cplusplus
#error PdkDev.h included in a non-C++ source file!
#endif

// --------------------------------------------------------------------
//              INCLUDES
// --------------------------------------------------------------------
#include "PdkBase.h"
#include "PdkOutBs.h"
#include "PdkJL.h"
#include "PdkPdl.h"

// --------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// --------------------------------------------------------------------

#define PDEVTYPE_BITMAP 0
#define PDEVTYPE_PDEV 1

typedef struct 
{
    short		pdType;			/* REQUIRED: Bitmap - 0  or Device - 1 */
} PDEVICE, FAR * LPPDEVICE;

#ifdef NO_GDIDEFS  
#undef RC_NONE
#define RC_NONE 0
#define PTTYPE POINT

//      DC Management Flags
#define DC_SPDevice   0000001 // Seperate PDevice required per device/filename
#define DC_1PDevice   0000002 // Only 1 PDevice allowed per device/filename
#define DC_IgnoreDFNP 0000004 // Ignore device/filename pairs when matching
#endif


#ifdef WIN_NT
#define dpHorzSize ulHorzSize
#define dpVertSize ulVertSize
#define dpHorzRes ulHorzRes
#define dpVertRes ulVertRes
#define dpLogPixelsX ulLogPixelsX
#define dpLogPixelsY ulLogPixelsY
#endif

#ifdef NO_GDIDEFS  
typedef struct
{
    short int dpVersion;
    short int dpTechnology;
    short int dpHorzSize;
    short int dpVertSize;
    short int dpHorzRes;
    short int dpVertRes;
    short int dpBitsPixel;
    short int dpPlanes;
    short int dpNumBrushes;
    short int dpNumPens;
    short int futureuse;
    short int dpNumFonts;
    short int dpNumColors;
    short int dpDEVICEsize;
    unsigned short int dpCurves;
    unsigned short int dpLines;
    unsigned short int dpPolygonals;
    unsigned short int dpText;
    unsigned short int dpClip;
    unsigned short int dpRaster;
    short int dpAspectX;
    short int dpAspectY;
    short int dpAspectXY;
    short int dpStyleLen;
    PTTYPE    dpMLoWin;
    PTTYPE    dpMLoVpt;
    PTTYPE    dpMHiWin;
    PTTYPE    dpMHiVpt;
    PTTYPE    dpELoWin;
    PTTYPE    dpELoVpt;
    PTTYPE    dpEHiWin;
    PTTYPE    dpEHiVpt;
    PTTYPE    dpTwpWin;
    PTTYPE    dpTwpVpt;
    short int dpLogPixelsX;
    short int dpLogPixelsY;
    short int dpDCManage;
    short int dpCaps1;
    long  int dpSpotSizeX;
    long  int dpSpotSizeY;
    short int dpPalColors;
    short int dpPalReserved;
    short int dpPalResolution;
} GDIINFO; 
	
typedef GDIINFO *PGDIINFO;
typedef GDIINFO FAR *LPGDIINFO;

#define     DF_MAPSIZE	    1

// Copied from GDIDEFS.INC
typedef     struct  {
	short int   dfType;
	short int   dfPoints;
	short int   dfVertRes;
	short int   dfHorizRes;
	short int   dfAscent;
	short int   dfInternalLeading;
	short int   dfExternalLeading;
	BYTE	    dfItalic;
	BYTE	    dfUnderline;
	BYTE	    dfStrikeOut;
	short int   dfWeight;
	BYTE	    dfCharSet;
	short int   dfPixWidth;
	short int   dfPixHeight;
	BYTE	    dfPitchAndFamily;
	short int   dfAvgWidth;
	short int   dfMaxWidth;
	BYTE	    dfFirstChar;
	BYTE	    dfLastChar;
	BYTE	    dfDefaultChar;
	BYTE	    dfBreakChar;
	short int   dfWidthBytes;
	unsigned long int   dfDevice;
	unsigned long int   dfFace;
	unsigned long int   dfBitsPointer;
	unsigned long int   dfBitsOffset;
	BYTE dfReservedByte;
	unsigned short dfMaps[DF_MAPSIZE];
} FONTINFO;

typedef FONTINFO *PFONTINFO;
typedef FONTINFO FAR *LPFONTINFO;

#endif


// --------------------------------------------------------------------
//              EXPORTED CLASSES
// --------------------------------------------------------------------

// ....................................................................
// CLASS NAME:      CPdkDevice
//
// REPONSIBILITIES: This class will contain all the Device info, this
//                  class will be what will allow for NT and 95 GDIINFO 
//                  structures to exist. This will be a linch pin for
//                  common code.  The idea is that the PDEVICE for both
//                  NT and 95 will be created on Enable and destroyed on 
//                  Disable.  This class will hold all Device dependent 
//                  information.
//
// NOTES:
// ....................................................................


class CPdkDevice// : public CPdkBase
{
public:   
#ifdef WIN_95
	BITMAP		pdBitmap;
	HDEV		hdev;  
#endif

#ifdef WIN_NT
    DWORD		dwID;
	HDEV		hdev;  
	HSURF		hSurface;
	int         cPalColors;
	HPALETTE	hpal;
	BOOL		bInPage;
	DWORD		cPages;
	HANDLE		hheap;
	HANDLE		hPrinter;
	HANDLE		hModule;
	LPTSTR		pwstrDocName;
#endif
    
    DWORD       dwFlags;            // a bunch of flags defined above.
    BOOL		bDocStarted;
    WORD       iPageNumber;        // page number of current page.

    char 		pPort[MAX_FILE];    /* Output port */
    char 		pModel[MAX_PATH];   /* Model Name */
    HDC 		hdc;                /* Handle to output display context */
    HANDLE	    hJob;               /* Spooling job number */
    BOOL        SpoolPageStarted;   /* Kludge 25JAN90 for openfile err */
    BOOL	    PageStarted;        /* Page started? */
    BOOL	    bFirstPage;
    BOOL	    bFirstBand;
        
	PALETTEENTRY    *pPal;          // If this is pal managed, this is the pal

    BOOL        bOutputSetup;
    PdkOutput   *pOut;
	GDIINFO     m_GDIInfo;

#ifdef WIN_NT
	DEVINFO     m_DEVInfo;
#endif

	DWORD		m_dwROPOverride;

	CPdkDevice();    // Default constructor for creation outside our code!
	virtual ~CPdkDevice();

   virtual void Disable();
	
	virtual void GetGDIInfo(ULONG cjGdiInfo, GDIINFO *pGDIInfo) = 0;
#if _WIN32
	virtual bool SetDeviceConfiguration(DRVPRINTER szDestDevType) = 0;
	virtual bool SetDeviceConfiguration(DRVPRINTER szDestDevType,
                                        LPWSTR szDriver) = 0;
	virtual void GetDevInfo(ULONG cb, PDEVINFO pdevinfo) = 0;
#else // !_WIN32
	virtual bool SetDeviceConfiguration(LPTSTR szName) = 0;
#endif // _WIN32

	int   GetPhysPageSize(LPDEVMODE lpdm ,LPPOINT lpPageSize);
	int   GetPhysPageOffset(LPDEVMODE lpdm ,LPPOINT lpPageOffset);  
	LPPALETTEENTRY GetPalette ();
	static LPPALETTEENTRY GetDefaultPalette ();
	int   QueryESC (LPDEVMODE lpdm, LPINT lpEscNum);
    
	void RemovePrinterConfig();
	void RemoveDevMode();
    
	virtual void DumpPDev();
	
    bool MergeInPDev(CPdkDevice *pDevIn);

	CPdkPrinterConfig* GetPdkPrinterConfig() {return(pConfig);}
	CPdkDevMode* GetPdkDevMode() {return(pDM);}
	CPdkJobLang* GetCPdkJobLang() {return(pPJL);}
	CPdkPDL* GetPdkPDL() {return(pPDL);}
	
	void SetCPdkJobLang(CPdkJobLang *pjl);
	void SetPdkPDL(CPdkPDL *ppdl);
	
protected:
	CPdkDevMode			*pDM;
    CPdkJobLang         *pPJL;
	CPdkPrinterConfig   *pConfig;
    CPdkPDL             *pPDL;

	DWORD               m_prnID;
    PALETTEENTRY        *pWindowsSystemPalette;
	
	void normalizeGDIPoint (long LX1, long LY1,PPOINT pPoint1, long LX2,
                            long LY2,PPOINT pPoint2);
	void rotateDimensions (GDIINFO* pGDIinfo);
	void scaleGDIInfo (GDIINFO* pGDIInfo);

#ifndef NT_KERNEL
	BOOL copyFile (const char *source, const char *target);
	void deleteFile (const char *fileName);
	BOOL renameFile (const char *oldFileName, const char *newFileName);
#endif

    
};

typedef CPdkDevice FAR * LPPDEVPDK;

// --------------------------------------------------------------------
//              EXPORTED FUNCTIONS
// --------------------------------------------------------------------

#ifdef WIN_95

BOOL IsPdkPDev(LPPDEVICE pdev);

LPPDEVPDK GetPdkPDev(LPPDEVICE pdev);

#endif // WIN_95


#endif  //  CPdkDevice_H_
