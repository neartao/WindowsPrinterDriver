#ifndef PDKUICTX_H_
#define PDKUICTX_H_

#ifndef __cplusplus
#error PdkUICtx.h included in a non-C++ source file!
#endif

// --------------------------------------------------------------------
//              INCLUDES
// --------------------------------------------------------------------
#include "PdkDef.h"
#include "PdkStd.h"
#include "PdkDM.h"
#include "PdkWin.h"
#include <WINDDIUI.H>
#include <PRINTOEM.H>

// --------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// --------------------------------------------------------------------

// --------------------------------------------------------------------
//              EXPORTED CLASSES
// --------------------------------------------------------------------

// ## Forward Declarations

// ....................................................................
// CLASS NAME:      CPdkUIContext
//
// NOTES:
//	This class is intended for use by both Device and Document
//	property sheets implementations.  In the Device case, DevMode
//	objects should not be used.
//
//	A context is constructed with one of the two constructors and
//	SetPermission() and SetCPSUIInfo().
// ....................................................................
class CPdkUIContext
{
    static BOOL bTraceOn;
	public:

		// ## Constructors / Destructor
		CPdkUIContext(POEMUIPSPARAM pParam);

		virtual ~CPdkUIContext();

        DWORD m_fMode;
		// ....................................................................
		// METHOD NAME:		SetPermissions
		// DESCRIPTION:
		//		Called at construction to indicate to this context whether or 
		//		not the user of this session has update permission.
		// ....................................................................
		void SetPermission(bool bUpdatePermission);

		// ....................................................................
		// METHOD NAME:		AddPropSheet
		// DESCRIPTION:
		//		Called at Construction to add a property sheet that has been 
		//		created for this UI session to this list of internal property 
		//		sheets. Returns the number of property sheets that the 
		//		context has in its array after the property sheet supplied by
		//		this call has been added. Returns 0 on error.
		// ....................................................................
		int AddPropSheet(CPdkWin *pPropSheet);

		// ....................................................................
		// METHOD NAME:		IsDeviceContext/IsDocumentContext
		// DESCRIPTION:
		//		Returns true if this UI context is intended to support a
		//		Device/Document Property Sheet dialog session.
		// ....................................................................
		bool IsDeviceContext() const { return m_bDeviceContext; }
		bool IsDocumentContext() const { return !m_bDeviceContext; }

		// ....................................................................
		// METHOD NAME:		HavePermission
		// DESCRIPTION:
		//		Queries whether or not update permission 
		//		is available. Use only after SetPermission has been called.
		// ....................................................................
		bool HavePermission() const { return m_bUpdatePermission; }

		DRVPRINTER PrinterHandle() const { return m_hPrinter; };

		// ....................................................................
		// METHOD NAME:		NumPropSheets / GetPropSheet
		// DESCRIPTION:
		//		Accessors to find the number of property sheets stored with
		//		this context and to retrieve each by index. GetPropSheet
		//		returns NULL on error.
		// ....................................................................
		int NumPropSheets() const;
		CPdkWin *GetPropSheet(int idx) const;

		// ....................................................................
		// METHOD NAME:		SetModified
		// DESCRIPTION:
		// 		Sets the modified state as indicated. The client should set this 
		// 		true whenever the state changes from the initial context or if
		// 		it changes from the last CommitCtxt() or 
		// 		ResetCtxtToInitialValues() calls. Note that CommitCtxt() and 
		//		ResetCtxtToInitialValues() will clear the modified state.
		//
		//		When bMatchesUserSet is false, then this indicates that the
		//		current DM does not match the current user setting.
		//
		//		Use IsModified to query the value after SetModified is called.
		// ....................................................................
		void SetModified(bool bChanged = true, bool bMatchesUserSet = false);
		bool IsModified() const;

		//====================================================================
		//
		// NAME: GetDriverName
		//
		// DESCRIPTION:
		//	Copies a the null-terminated driver name to the 
		//	client supplied buffer.
		//	The string will be truncated if the buffer is not large enough.
		//
		// PARAMETERS:
		//	rscID		i	Resource ID of string to load.
		//	szBuf		o	Client-owned buffer to which string is copied.
		//	pBufLen		i/o	Length, in TCHAR, of client-owned buffer.
		//	pNeeded		o	Number of TCHAR truncated.
		//
		// RETURN CODES:
		//	Returns true if the name was found, false if not.
		//
		// NOTES:
		//
		//====================================================================
		virtual bool GetDriverName(TCHAR *szBuf, DWORD *pBufLen, 
															DWORD *pNeeded);

	protected:

		// ## Instance Data

		// Handle to the current printer
		DRVPRINTER m_hPrinter;

		// True if this context supports Device Settings... and false if 
		// it supports Document Properties
		bool m_bDeviceContext;

		// True if the user can update the data in displayed dialogs
		bool m_bUpdatePermission;

		#ifdef WIN_NT
		// CPSUI information - address of CPSUI's ComPropSheet() function and
		// the handle to the sheet group parent.
		PFNCOMPROPSHEET m_pfnComPropSheet;
		HANDLE m_hComPropSheet;
		#endif

		// Array of sibling CPSUI tab sheets created in this context
		int m_numPropSheets;
		enum {MAX_PROP_SHEETS = 20};
		CPdkWin *m_arrPropSheets[MAX_PROP_SHEETS];

		// Tracks the modified state of this context. Should be true whenever
		// the context has been changed since its initial setting or since the
		// last time CommitCtxt() or ResetCtxtToInitialValues() has been called.
		bool m_bModified;

		TCHAR m_szDriverName[MAX_PATH];

	private:

		// Hide the copy and assignment constructors to prevent their use.
		CPdkUIContext(const CPdkUIContext&);
		CPdkUIContext& operator=(const CPdkUIContext&);

};


#endif  //  PdkUICtx_H_
