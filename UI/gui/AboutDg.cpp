// AboutDg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "AboutDg.h"
#include "PdkApp.h"

/////////////////////////////////////////////////////////////////////////////
//
CAboutDlg::CAboutDlg(CPdkUIContext *pUICtxt)
: CPdkWin(pUICtxt)
{}

LRESULT CAboutDlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	CenterWindow(GetParent());

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	HWND hStatic = GetDlgItem(IDC_DRIVER_NAME);
	::SetWindowText(hStatic, _T("Hello WTL!"));

	TCHAR szBuf[MAX_PATH] = {0};
	TCHAR *szComments = new TCHAR[MAX_PATH + 1];
	TCHAR *szCompanyName = new TCHAR[MAX_PATH + 1];
	TCHAR *szFileDescription = new TCHAR[MAX_PATH + 1];
	TCHAR *szFileVersion = new TCHAR[MAX_PATH + 1];
	TCHAR *szLegalCopyright = new TCHAR[MAX_PATH + 1];
	TCHAR *szProductName = new TCHAR[MAX_PATH + 1];
	TCHAR *szProductVersion = new TCHAR[MAX_PATH + 1];

	GetPdkAppObj()->GetVerString(MAX_PATH, szComments,\
					  szCompanyName, szFileDescription,\
					  szFileVersion, szLegalCopyright,\
					  szProductName, szProductVersion);

	hStatic = GetDlgItem(IDC_DRIVER_VERSION);
	_stprintf_s(szBuf, MAX_PATH, _T("Çý¶¯°æ±¾ºÅ£º%s"), szProductVersion);
	::SetWindowText(hStatic, szBuf);

	hStatic = GetDlgItem(IDC_DRIVER_NAME);
	_stprintf_s(szBuf, MAX_PATH, _T("%s"), szProductName);
	::SetWindowText(hStatic, szBuf);
	
	hStatic = GetDlgItem(IDC_DRIVER_COPYRIGHT);
	_stprintf_s(szBuf, MAX_PATH, _T("%s"), szLegalCopyright);
	::SetWindowText(hStatic, szBuf);

	delete[] szComments;
	delete[] szCompanyName;
	delete[] szFileDescription;
	delete[] szFileVersion;
	delete[] szLegalCopyright;
	delete[] szProductName;
	delete[] szProductVersion;

	return TRUE;
}

LRESULT CAboutDlg::OnButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LPCTSTR lpstrURL = _T("www.google.com.hk");

	switch (wID)
	{
	case IDC_FEED_BACK:
		lpstrURL = _T("http://www.seuic.com/index.asp");
		break;
	case IDC_CHECK_UPDATE:
		lpstrURL = _T("http://www.seuic.com/products/pro1-1xx.asp?pid=64");
		break;
	}

	ShellExecute(NULL, _T("open"), lpstrURL, NULL, NULL, SW_SHOW);

	return 0;
}