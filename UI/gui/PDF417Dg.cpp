// PDF417Dg.cpp : implementation of the CPDF417Dlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "PDF417Dg.h"
#include "PdkApp.h"
#include "PdkUICtx.h"
#include <atlprint.h>

/////////////////////////////////////////////////////////////////////////////
//
CPDF417Dlg::CPDF417Dlg(CPdkUIContext *pUICtxt)
: CPdkWin(pUICtxt)
, m_nItemWidth(7)
, m_nItemHeight(25)
, m_nColumnNum(7)
, m_nCellNum(8)
{}

LRESULT CPDF417Dlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	CenterWindow(GetParent());

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	m_wndItemWidthSpin.Attach(GetDlgItem(IDC_BARCODE_ITEM_WIDTH_SPIN));
	m_wndItemWidthSpin.SetRange(1, 7);
	m_wndItemWidthSpin.SetBuddy(m_wndItemWidth);
	m_wndItemWidth.LimitText(1);

	m_wndItemHeightSpin.Attach(GetDlgItem(IDC_BARCODE_ITEM_HEIGHT_SPIN));
	m_wndItemHeightSpin.SetRange(2, 25);
	m_wndItemHeightSpin.SetBuddy(m_wndItemHeight);
	m_wndItemHeight.LimitText(2);


	m_wndColumnNumSpin.Attach(GetDlgItem(IDC_COLUMN_NUM_SPIN));
	m_wndColumnNumSpin.SetRange(1, 30);
	m_wndColumnNumSpin.SetBuddy(m_wndColumnNum);
	m_wndColumnNum.LimitText(2);

	m_wndCellNumSpin.Attach(GetDlgItem(IDC_CELL_NUM_SPIN));
	m_wndCellNumSpin.SetRange(3, 90);
	m_wndCellNumSpin.SetBuddy(m_wndCellNum);
	m_wndCellNum.LimitText(2);

	m_wndErrorCorrection.AddString(_T("纠错级别0"));
	m_wndErrorCorrection.AddString(_T("纠错级别1"));
	m_wndErrorCorrection.AddString(_T("纠错级别2"));
	m_wndErrorCorrection.AddString(_T("纠错级别3"));
	m_wndErrorCorrection.AddString(_T("纠错级别4"));
	m_wndErrorCorrection.AddString(_T("纠错级别5"));
	m_wndErrorCorrection.AddString(_T("纠错级别6"));
	m_wndErrorCorrection.AddString(_T("纠错级别7"));
	m_wndErrorCorrection.AddString(_T("纠错级别8"));
	m_wndErrorCorrection.SetCurSel(1);

	return TRUE;
}

void CPDF417Dlg::OnErrorCorrectionComboxSelected(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (uNotifyCode == CBN_SELCHANGE && wndCtl.m_hWnd == m_wndErrorCorrection.m_hWnd)
	{
		BOOL enableOption = m_wndErrorCorrection.GetCurSel() != 0;

		PropSheet_Changed(GetParent(), m_hWnd);
	}
}

LRESULT CPDF417Dlg::OnEditChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	PropSheet_Changed(GetParent(), m_hWnd);

	return 0;
}