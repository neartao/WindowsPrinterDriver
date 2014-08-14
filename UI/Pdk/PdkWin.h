#ifndef PDKWIN_H_ // (make sure this is only included once)
#define PDKWIN_H_

#ifndef __cplusplus
#error PdkWin.h included in a non-C++ source file!
#endif

// --------------------------------------------------------------------
//              INCLUDES
// --------------------------------------------------------------------

#include "PdkBase.h"
#include "PdkDef.h"

// --------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// --------------------------------------------------------------------

// Define frequently undefined message crackers
#ifndef	HANDLE_WM_HELP
	/* void Cls_OnHelp(HWND hwnd, WPARAM wParam, LPARAM lParam) */
	#define HANDLE_WM_HELP(hwnd, wParam, lParam, fn) \
		((fn)((hwnd), wParam, lParam), 0L)
#endif

#ifdef _WIN32
#pragma warning(disable:4236)
#endif

// --------------------------------------------------------------------
//              EXPORTED CLASSES
// --------------------------------------------------------------------

// ## Forward declarations
class CPdkUIContext;

// For access to CPdkWin::windowProcedure() 
extern "C" INT_PTR _export CALLBACK FnPdkWinProcedure( HWND hWnd, UINT iMessage,
                                               WPARAM wParam, LPARAM lParam );

// ## Supporting Types

// ............................................................................
// CLASS NAME:      CPdkWin
//
// REPONSIBILITIES:
//		This class implements the C object behind a generic window. It is used
//		primarily for subclassing other window types, such as static images or
//		specialized buttons.
//		Many protected methods handling Windows messages may be overridden
//		to customize behavior, including the Windows message handler itself so
//		that new messages may be handled.
//
//		This is an abstract class and must be instantiated by a derived class.
//
// NOTES:
// ............................................................................
class CPdkWin: public CFrameWindowImpl<CPdkWin>
{
public:
	DECLARE_WND_CLASS ( _T("Printer WTL window") );

	// ## Constructors / Destructor
	CPdkWin(CPdkUIContext *pUICtxt);
	virtual ~CPdkWin();	

	typedef CFrameWindowImpl<CPdkWin> CPdkWinBase;
	BEGIN_MSG_MAP_EX(CPdkWin)
		MSG_WM_INITDIALOG(OnInitDialog)
		CHAIN_MSG_MAP(CFrameWindowImpl<CPdkWin>)
	END_MSG_MAP()

	virtual UINT GetDialogID() = 0;

	virtual LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/) = 0;

protected:

	// The current UI context
	CPdkUIContext *m_pUICtxt;

	// ## Friend Functions

	// For access CPdkWin::windowProcedure() 
	friend INT_PTR _export CALLBACK FnPdkWinProcedure(HWND hWnd, UINT iMessage,
		WPARAM wParam, LPARAM lParam );

private:

};


#endif 
