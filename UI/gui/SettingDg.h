// SettingDg.h : interface of the CSettingDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef SETTINGDG_H
#define SETTINGDG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"
#include "Controls.h"

/////////////////////////////////////////////////////////////////////////////
//
class CSettingDlg : public CPdkWin, public CWinDataExchange<CSettingDlg>
{
public:
    enum { IDD = IDD_SETTING };

	CSettingDlg(CPdkUIContext *pUICtxt);

    BEGIN_MSG_MAP_EX(CSettingDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER(IDC_RESTORE_FACTORY_SETTINGS, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_SAVE_AS, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_DEFAULT, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_SAVE_FILE, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_READ_FILE, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_DELETE, OnButtonClick)
    END_MSG_MAP()

	virtual UINT GetDialogID() { return IDD; }
	
    LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	
    LRESULT OnButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

protected:
};

#endif // !defined(SETTINGDG_H)
