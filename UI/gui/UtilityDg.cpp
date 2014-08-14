// UtilityDg.cpp : implementation of the CUtilityDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "UtilityDg.h"
#include "PdkApp.h"

/////////////////////////////////////////////////////////////////////////////
//
CControlFont::CControlFont(TCHAR command_char, LPCTSTR lpstrCommand, COLORREF tColor)
: m_tChar(command_char)
{
	m_hbrBkgnd = CreateSolidBrush(tColor);

	ATLASSERT(lpstrCommand != NULL);

	m_szCommand = lpstrCommand;
}

CControlFont::~CControlFont()
{
	DeleteObject ( m_hbrBkgnd );
}

LRESULT CControlFont::OnEraseBkgnd(HDC hDC)
{
	RECT rcClient;

	GetClientRect ( &rcClient );
	FillRect ( hDC, &rcClient, m_hbrBkgnd );
	return 1;    // we painted the background
}

LRESULT CControlFont::OnCtlColorStatic(HDC hDC, HWND hWnd)
{
	return (LRESULT)m_hbrBkgnd.m_hBrush;
}

LRESULT CControlFont::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    CenterWindow(GetParent());

	DoDataExchange(false);

	TCHAR szBuf[MAX_PATH] = {0};

	_stprintf_s(szBuf, MAX_PATH, _T("%c"), m_tChar);
	::SetWindowText(GetDlgItem(IDC_CONTROL_FONT_CHAR), szBuf);

	_stprintf_s(szBuf, MAX_PATH, _T("%x"), m_tChar);
	::SetWindowText(GetDlgItem(IDC_CONTROL_FONT_HEX), szBuf);

	return TRUE;
}

LRESULT CControlFont::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(true);

    EndDialog(wID);
    return 0;
}


/////////////////////////////////////////////////////////////////////////////
//
CControlFontTable::CControlFontTable(COLORREF tColor)
: m_bkColor(tColor)
, m_szDriverName(_T("DJ-1580IÇý¶¯³ÌÐò"))
{
	m_hbrBkgnd = CreateSolidBrush(tColor);
}

CControlFontTable::~CControlFontTable()
{
	DeleteObject ( m_hbrBkgnd );
}

LRESULT CControlFontTable::OnEraseBkgnd(HDC hDC)
{
	RECT rcClient;

	GetClientRect ( &rcClient );
	FillRect ( hDC, &rcClient, m_hbrBkgnd );
	return 1;    // we painted the background
}

LRESULT CControlFontTable::OnCtlColorStatic(HDC hDC, HWND hWnd)
{
	return (LRESULT)m_hbrBkgnd.m_hBrush;
}

LRESULT CControlFontTable::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    CenterWindow(GetParent());

	DoDataExchange(false);

	DWORD dwStyle = m_cfList.GetExtendedListViewStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_SHOWSELALWAYS;
	m_cfList.SetExtendedListViewStyle(dwStyle);
	m_cfList.InsertColumn(0, _T("Char"), LVCFMT_LEFT, 45, 0);
	m_cfList.InsertColumn(1, _T("Hex"), LVCFMT_LEFT, 45, 0);
	m_cfList.InsertColumn(2, _T("Command"), LVCFMT_LEFT, 255, 0);

	for (int ch = 0x20, i = 0; ch <= 0x7F; ++ch, ++i)
	{
		TCHAR szBuf[MAX_PATH] = {0};
		TCHAR szBuf2[MAX_PATH] = {0};

		_stprintf_s(szBuf, MAX_PATH, _T("%c"), ch);
		_stprintf_s(szBuf2, MAX_PATH, _T("%x"), ch);

		m_cfList.InsertItem(i, szBuf);
		m_cfList.SetItemText(i, 1, szBuf2);
	}

	return TRUE;
}

LRESULT CControlFontTable::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(true);

    EndDialog(wID);
    return 0;
}

LRESULT CControlFontTable::OnOperationCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(true);

	int index = m_cfList.GetSelectedIndex();
	if (index < 0)
		return 0;

	switch (wID)
	{
	case IDC_EDIT:
		{
			CString szText;
			m_cfList.GetItemText(index, 2, szText);
			CControlFont dlg(index + 0x20, szText, m_bkColor);
			if (dlg.DoModal() == IDOK)
			{
				m_cfList.SetItemText(index, 2, dlg.m_szCommand);
			}
		}
		break;
	case IDC_DELETE:
		{
			m_cfList.SetItemText(index, 2, _T(""));
		}
		break;
	}

	return 0;
}

LRESULT CControlFontTable::OnNotifyHandlerEX(LPNMHDR pnmh)
{
	DoDataExchange(true);
	int index = m_cfList.GetSelectedIndex();
	if (index < 0)
		return 0;

	CString szText;
	m_cfList.GetItemText(index, 2, szText);
	CControlFont dlg(index + 0x20, szText, m_bkColor);
	if (dlg.DoModal() == IDOK)
	{
		m_cfList.SetItemText(index, 2, dlg.m_szCommand);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
CUtilityDlg::CUtilityDlg(CPdkUIContext *pUICtxt)
: CPdkWin(pUICtxt)
{}

LRESULT CUtilityDlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	CenterWindow(GetParent());

	// First DDX call, hooks up variables to controls.
	//DoDataExchange(false);

	return TRUE;
}

LRESULT CUtilityDlg::OnButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (IDC_CONTROL_FONT == wID)
	{
		CDC dc = GetDC();
		LOGBRUSH lb = {0};
		dc.GetCurrentBrush().GetLogBrush(&lb);
		CControlFontTable dlg(lb.lbColor);
		dlg.DoModal();
	}
	return 0;
}
