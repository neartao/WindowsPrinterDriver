// DebugDg.h : interface of the CDebugDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef DEBUGDG_H
#define DEBUGDG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"
#include "Controls.h"

/////////////////////////////////////////////////////////////////////////////
//
class CDebugDlg : public CPdkWin, public CWinDataExchange<CDebugDlg>
{
public:
    enum { IDD = IDD_DEBUG };

	CDebugDlg(CPdkUIContext *pUICtxt);

    BEGIN_MSG_MAP_EX(CDebugDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER(IDC_CONVERTOUTPUT, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_CONVERT2BITMAP, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_CONVERT2JPG, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_CONVERT2TIFF, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_SENT2PRINTER, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_BROWSER, OnButtonClick)
    END_MSG_MAP()

	BEGIN_DDX_MAP(CDebugDlg)
		DDX_TEXT(IDC_FILE, m_szOutputFolder)
	END_DDX_MAP()

	virtual UINT GetDialogID() { return IDD; }
	
    LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	
    LRESULT OnButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

protected:

	CString m_szOutputFolder;
};

#endif // !defined(SETTINGDG_H)
