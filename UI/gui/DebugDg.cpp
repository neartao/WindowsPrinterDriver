// DebugDg.cpp : implementation of the CDebugDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "DebugDg.h"
#include "PdkApp.h"

/////////////////////////////////////////////////////////////////////////////
//
CDebugDlg::CDebugDlg(CPdkUIContext *pUICtxt)
: CPdkWin(pUICtxt)
{
}

LRESULT CDebugDlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	CenterWindow(GetParent());

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	CButton checkbox;
	checkbox.Attach(GetDlgItem(IDC_CONVERTOUTPUT));
	BOOL enable = checkbox.GetCheck();

	::EnableWindow(GetDlgItem(IDC_CONVERT2BITMAP), enable);
	::EnableWindow(GetDlgItem(IDC_CONVERT2JPG), enable);
	::EnableWindow(GetDlgItem(IDC_CONVERT2TIFF), enable);
	::EnableWindow(GetDlgItem(IDC_SENT2PRINTER), enable);
	::EnableWindow(GetDlgItem(IDC_BROWSER), enable);

	return TRUE;
}

LRESULT CDebugDlg::OnButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PropSheet_Changed(GetParent(), m_hWnd);
	switch (wID)
	{
	case IDC_CONVERTOUTPUT:
		{
			CButton checkbox;
			checkbox.Attach(GetDlgItem(IDC_CONVERTOUTPUT));
			BOOL enable = checkbox.GetCheck();

			::EnableWindow(GetDlgItem(IDC_CONVERT2BITMAP), enable);
			::EnableWindow(GetDlgItem(IDC_CONVERT2JPG), enable);
			::EnableWindow(GetDlgItem(IDC_CONVERT2TIFF), enable);
			::EnableWindow(GetDlgItem(IDC_SENT2PRINTER), enable);
			::EnableWindow(GetDlgItem(IDC_BROWSER), enable);
		}
		break;
	}
	return 0;
}
