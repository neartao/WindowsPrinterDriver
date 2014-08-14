// LayoutDg.cpp : implementation of the CLayoutDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "LayoutDg.h"
#include "PdkApp.h"
#include "PdkUICtx.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
//
CLayoutDlg::CLayoutDlg(CPdkUIContext *pUICtxt)
: CPdkWin(pUICtxt)
, m_nZoomValue(100)
{
}

LRESULT CLayoutDlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	CenterWindow(GetParent());

	CButton radio;
	radio.Attach(GetDlgItem(IDC_PRINT_AS_TEXT));
	radio.SetCheck(1);

	radio.Detach();
	radio.Attach(GetDlgItem(IDC_PORTRAIT));
	radio.SetCheck(1);

	CButton checkbox;
	checkbox.Attach(GetDlgItem(IDC_ZOOM));
	checkbox.SetCheck(0);

	::EnableWindow(GetDlgItem(IDC_ZOOM_VALUE), FALSE);
	::EnableWindow(GetDlgItem(IDC_ZOOM_SPIN), FALSE);

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	m_wndOrientation.SetBitmapId(IDB_PORTRAIT);

	m_wndZoom.LimitText(3);
	m_wndZoomSpin.Attach(GetDlgItem(IDC_ZOOM_SPIN));
	m_wndZoomSpin.SetRange(0, 200);
	m_wndZoomSpin.SetBuddy(m_wndZoom);

	PRINTER_DEFAULTS PrtDefault = {NULL, NULL, PRINTER_ALL_ACCESS};
	TCHAR szDriverName[MAX_PATH] = {0};
	HANDLE hPrinter = NULL;
	DWORD	dwBufferLen, dwNeed;
	DWORD	bNeeded = 0;

	dwBufferLen = DIM(szDriverName);
	m_pUICtxt->GetDriverName(szDriverName, &dwBufferLen, &dwNeed);
	
    if(!OpenPrinter((PWSTR)szDriverName, &hPrinter, &PrtDefault))
    {
        if(!OpenPrinter((PWSTR)szDriverName, &hPrinter, NULL))
            return 0;
    }

	DWORD dwReturned = 0;
	EnumForms(hPrinter, 1, NULL, 0, &dwNeed, &dwReturned);
	if (dwNeed > 0)
	{
		FORM_INFO_1* pForms = (FORM_INFO_1*)malloc(dwNeed);
		EnumForms(hPrinter, 1, (LPBYTE)pForms, dwNeed, &dwNeed, &dwReturned);

		for (DWORD i = 0; i < dwReturned; ++i)
		{
			m_wndForms.AddString(pForms[i].pName);
		}
		m_wndForms.SetCurSel(0);
		free(pForms);
	}

	return TRUE;
}

void CLayoutDlg::OnFormsComboxSelected(UINT /*uNotifyCode*/, int /*nID*/, CWindow /*wndCtl*/)
{
	PropSheet_Changed(GetParent(), m_hWnd);
}

LRESULT CLayoutDlg::OnPrintTypeChanged(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return PropSheet_Changed(GetParent(), m_hWnd);
}

LRESULT CLayoutDlg::OnPrintDirectionChanged(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CButton checkbox;
	checkbox.Attach(GetDlgItem(IDC_ROTATE_180));
	BOOL bRotate = checkbox.GetCheck();
	UINT nBitmapId = 0;
	if (wID == IDC_PORTRAIT)
		nBitmapId = bRotate? IDB_PORTRAIT_R: IDB_PORTRAIT;
	else if (wID == IDC_LANDSCAPE)
		nBitmapId = bRotate? IDB_LANDSCAPE_R: IDB_LANDSCAPE;

	m_wndOrientation.SetBitmapId(nBitmapId);

	return PropSheet_Changed(GetParent(), m_hWnd);
}

LRESULT CLayoutDlg::OnRotatePrintChanged(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CButton checkbox;
	checkbox.Attach(GetDlgItem(IDC_ROTATE_180));
	BOOL bRotate = checkbox.GetCheck();
	UINT nBitmapId = 0;

	CButton orientation;
	orientation.Attach(GetDlgItem(IDC_PORTRAIT));

	if (orientation.GetCheck())
		nBitmapId = bRotate? IDB_PORTRAIT_R: IDB_PORTRAIT;
	else
		nBitmapId = bRotate? IDB_LANDSCAPE_R: IDB_LANDSCAPE;

	m_wndOrientation.SetBitmapId(nBitmapId);

	return PropSheet_Changed(GetParent(), m_hWnd);
}

LRESULT CLayoutDlg::OnZoomCheckBoxClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CButton checkbox;
	checkbox.Attach(GetDlgItem(IDC_ZOOM));
	BOOL bChecked = checkbox.GetCheck();

	::EnableWindow(GetDlgItem(IDC_ZOOM_VALUE), bChecked);
	::EnableWindow(GetDlgItem(IDC_ZOOM_SPIN), bChecked);

	PropSheet_Changed(GetParent(), m_hWnd);

	return 0;
}


LRESULT CLayoutDlg::OnReflectNotifications(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hWndChild = NULL;

	switch(uMsg)
	{
	case WM_DRAWITEM:
		if(wParam)	// not from a menu
			hWndChild = ((LPDRAWITEMSTRUCT)lParam)->hwndItem;
		break;
	case WM_MEASUREITEM:
		if(wParam)	// not from a menu
			hWndChild = GetDlgItem(((LPMEASUREITEMSTRUCT)lParam)->CtlID);
		break;
	case WM_COMPAREITEM:
		if(wParam)	// not from a menu
			hWndChild =  ((LPCOMPAREITEMSTRUCT)lParam)->hwndItem;
		break;
	case WM_DELETEITEM:
		if(wParam)	// not from a menu  
			hWndChild =  ((LPDELETEITEMSTRUCT)lParam)->hwndItem;
	default:
		break;
	}

	if (hWndChild == NULL)
	{
		bHandled = FALSE;
		return 1;
	}

	ATLASSERT(::IsWindow(hWndChild));
	return ::SendMessage(hWndChild, OCM__BASE + uMsg, wParam, lParam);
}