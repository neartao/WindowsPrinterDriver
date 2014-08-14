// SettingDg.cpp : implementation of the CSettingDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "SettingDg.h"
#include "PdkApp.h"
#include <atldlgs.h>

/////////////////////////////////////////////////////////////////////////////
//
CSettingDlg::CSettingDlg(CPdkUIContext *pUICtxt)
: CPdkWin(pUICtxt)
{
}

LRESULT CSettingDlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	CenterWindow(GetParent());

	// First DDX call, hooks up variables to controls.
	//DoDataExchange(false);

	return TRUE;
}

LRESULT CSettingDlg::OnButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	switch (wID)
	{
	case IDC_SAVE_AS:
		{
			CFileDialog dlg(false, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, _T("设置文件\0*.dat\0所有文件\0*.*\0\0"));
			if (dlg.DoModal() == IDOK)
			{}
		}
		break;
	case IDC_DEFAULT:
		break;
	case IDC_SAVE_FILE:
		{
			CFileDialog dlg(false, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, _T("设置文件\0*.dat\0所有文件\0*.*\0\0"));
			if (dlg.DoModal() == IDOK)
			{}
		}
		break;
	case IDC_READ_FILE:
		{
			CFileDialog dlg(true, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, _T("设置文件\0*.dat\0所有文件\0*.*\0\0"));
			if (dlg.DoModal() == IDOK)
			{}
		}
		break;
	case IDC_DELETE:
		break;
	default:
		break;
	}
	return 0;
}
