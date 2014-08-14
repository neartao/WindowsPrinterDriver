// BarcodeDg.cpp : implementation of the CBarcodeDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "BarcodeDg.h"
#include "PdkApp.h"
#include "PdkUICtx.h"
#include <atlprint.h>

/////////////////////////////////////////////////////////////////////////////
//
CBarcodeDlg::CBarcodeDlg(CPdkUIContext *pUICtxt)
: CPdkWin(pUICtxt)
, m_nItemWidth(3)
, m_nItemHeight(163)
{}

LRESULT CBarcodeDlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	CenterWindow(GetParent());

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	m_wndItemWidthSpin.Attach(GetDlgItem(IDC_BARCODE_ITEM_WIDTH_SPIN));
	m_wndItemWidthSpin.SetRange(2, 6);
	m_wndItemWidthSpin.SetBuddy(m_wndItemWidth);
	m_wndItemWidth.LimitText(1);

	m_wndItemHeightSpin.Attach(GetDlgItem(IDC_BARCODE_ITEM_HEIGHT_SPIN));
	m_wndItemHeightSpin.SetRange(1, 255);
	m_wndItemHeightSpin.SetBuddy(m_wndItemHeight);
	m_wndItemHeight.LimitText(3);

	m_wndBarcodeFontName.AddString(_T("1D Code1"));
	m_wndBarcodeFontName.AddString(_T("1D Code2"));
	m_wndBarcodeFontName.AddString(_T("1D Code3"));
	m_wndBarcodeFontName.AddString(_T("1D Code4"));
	m_wndBarcodeFontName.AddString(_T("1D Code5"));
	m_wndBarcodeFontName.AddString(_T("1D Code6"));
	m_wndBarcodeFontName.AddString(_T("1D Code7"));
	m_wndBarcodeFontName.AddString(_T("1D Code8"));
	m_wndBarcodeFontName.SetCurSel(0);

	m_wndBarcodeType.AddString(_T("UPC-A"));
	m_wndBarcodeType.AddString(_T("UPC-E"));
	m_wndBarcodeType.AddString(_T("EAN8"));
	m_wndBarcodeType.AddString(_T("EAN13"));
	m_wndBarcodeType.AddString(_T("Code39"));
	m_wndBarcodeType.AddString(_T("ITF"));
	m_wndBarcodeType.AddString(_T("Codabar"));
	m_wndBarcodeType.AddString(_T("Code93"));
	m_wndBarcodeType.AddString(_T("Code128"));
	m_wndBarcodeType.SetCurSel(0);

	m_wndHRI.AddString(_T("不打印"));
	m_wndHRI.AddString(_T("在条码上部"));
	m_wndHRI.AddString(_T("在条码下部"));
	m_wndHRI.AddString(_T("在条码上部和下部"));
	m_wndHRI.SetCurSel(0);

	CButton radio;
	radio.Attach(GetDlgItem(IDC_HRI_FONTA));
	radio.SetCheck(1);
	::EnableWindow(GetDlgItem(IDC_HRI_FONTA), FALSE);
	::EnableWindow(GetDlgItem(IDC_HRI_FONTB), FALSE);

	return TRUE;
}

void CBarcodeDlg::OnHRIPositionComboxSelected(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (uNotifyCode == CBN_SELCHANGE && wndCtl.m_hWnd == m_wndHRI.m_hWnd)
	{
		BOOL enableOption = m_wndHRI.GetCurSel() != 0;

		::EnableWindow(GetDlgItem(IDC_HRI_FONTB), enableOption);
		::EnableWindow(GetDlgItem(IDC_HRI_FONTA), enableOption);

		PropSheet_Changed(GetParent(), m_hWnd);
	}
}

LRESULT CBarcodeDlg::OnHRIFontTypeChanged(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return PropSheet_Changed(GetParent(), m_hWnd);
}

LRESULT CBarcodeDlg::OnEditChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// 1毫米 = 0.0393700787 inch
	// 8点/毫米, 384点/行，203dpi

	PropSheet_Changed(GetParent(), m_hWnd);
	if (m_wndItemHeight.m_hWnd == hWndCtl)
	{
		DoDataExchange(true);

		// pixelx:=GetDeviceCaps(Printer.Handle,   LOGPIXELSX);
		// pixel_value:=trunc(MM/25.4*pixelx);
		PRINTER_DEFAULTS PrtDefault = {NULL, NULL, PRINTER_ALL_ACCESS};
		TCHAR szDriverName[MAX_PATH] = {0};;
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

		CPrinterDC dc(hPrinter);
		int nPixelX = GetDeviceCaps(dc.m_hDC, LOGPIXELSX);

		// 像素换算为毫米 x * 25.4 / px 
		float fMillimeter = m_nItemHeight * 25.4 / nPixelX;

		// 磅数÷2.845=毫米数
		float  fPound = fMillimeter * 2.845;

		TCHAR szBuf[MAX_PATH] = {0};
		_stprintf_s(szBuf, MAX_PATH, _T("%.2f"), fMillimeter);
		::SetWindowText(GetDlgItem(IDC_BARCODE_HEIGHT_MM), szBuf);

		_stprintf_s(szBuf, MAX_PATH, _T("%d"), fPound);
		::SetWindowText(GetDlgItem(IDC_BARCODE_HEIGHT_POUND), szBuf);
	}

	return 0;
}