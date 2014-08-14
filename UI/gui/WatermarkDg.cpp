// WatermarkDg.cpp : implementation of the CWatermarkDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "WatermarkDg.h"
#include "PdkApp.h"
#include <atldlgs.h>
#include "PdkUICtx.h"

/////////////////////////////////////////////////////////////////////////////
//
CWatermarkDlg::CWatermarkDlg(CPdkUIContext *pUICtxt)
: CPdkWin(pUICtxt)
, m_nAngle(45)
, m_nSize(100)
, m_nBrightness(50)
{}

LRESULT CWatermarkDlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	CenterWindow(GetParent());

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	m_wndWatermark.AddString(_T("无水印"));
	m_wndWatermark.AddString(_T("位图水印"));
	m_wndWatermark.AddString(_T("文字水印"));
	m_wndWatermark.SetCurSel(0);

	m_wndWatermarkText.AddString(_T("CONFIDENTIAL"));
	m_wndWatermarkText.AddString(_T("DO NOT COPY"));
	m_wndWatermarkText.AddString(_T("TOP SECRET"));
	m_wndWatermarkText.AddString(_T("DRAFT"));
	m_wndWatermarkText.AddString(_T("PROOF"));
	m_wndWatermarkText.AddString(_T("FINAL"));
	m_wndWatermarkText.AddString(_T("COPY"));
	m_wndWatermarkText.AddString(_T("FILE COPY"));

	DWORD cb = 0;
	GetPrinter(m_pUICtxt->PrinterHandle(), 2, NULL, 0, &cb);
	BYTE *pBBuf = (BYTE *)malloc(cb);
	if (pBBuf != NULL)
	{
		// now actually try to get the printer info structure
		GetPrinter(m_pUICtxt->PrinterHandle(), 2, pBBuf, cb, &cb);

		// now check if a name string is present
		PRINTER_INFO_2 *pPI2 = (PRINTER_INFO_2*)pBBuf;

		PARFONTS parFonts;
		INT nFaces = 0;
		HDC hdc = CreateDC (pPI2->pDriverName, pPI2->pPrinterName, pPI2->pPortName, NULL);
		parFonts = BuildFontList (hdc, &nFaces);
		//填写字体列表
		for (int i = 0; i < nFaces; i++)
		{
			m_wndFontType.AddString(((parFonts + i)->lf)->lfFaceName);
		}
		LocalFree(parFonts);
		m_wndFontType.SelectString(-1, _T("宋体"));
		DeleteDC(hdc);

		// free the info buffer
		free(pBBuf);
	}

	m_wndFontStyle.AddString(_T("常规"));
	m_wndFontStyle.AddString(_T("斜体"));
	m_wndFontStyle.AddString(_T("粗体"));
	m_wndFontStyle.AddString(_T("粗斜体"));
	m_wndFontStyle.SetCurSel(0);

	m_wndWatermarkPosition.AddString(_T("Left-Top"));
	m_wndWatermarkPosition.AddString(_T("Center-Top"));
	m_wndWatermarkPosition.AddString(_T("Right-Top"));
	m_wndWatermarkPosition.AddString(_T("Left-Middle"));
	m_wndWatermarkPosition.AddString(_T("Center-Middle"));
	m_wndWatermarkPosition.AddString(_T("Right-Middle"));
	m_wndWatermarkPosition.AddString(_T("Left-Bottom"));
	m_wndWatermarkPosition.AddString(_T("Center-Bottom"));
	m_wndWatermarkPosition.AddString(_T("Right-Bottom"));
	m_wndWatermarkPosition.AddString(_T("Tiled"));
	m_wndWatermarkPosition.SetCurSel(4);

	for (int i = 8; i <= 72; ++i)
	{
		TCHAR szBuf[MAX_PATH] = {0};
		_stprintf_s(szBuf, MAX_PATH, _T("%d"), i);
		m_wndFontSize.AddString(szBuf);
	}
	m_wndFontSize.SelectString(-1, _T("48"));

	m_wndAngleTrack.Attach(GetDlgItem(IDC_ANGLE_SLIDER));
	m_wndAngleTrack.SetRange(0, 360);
	m_wndAngleTrack.SetPos(m_nAngle);

	m_wndSizeTrack.Attach(GetDlgItem(IDC_WATERMARK_SIZE_SLIDER));
	m_wndSizeTrack.SetRange(1, 100);
	m_wndSizeTrack.SetPos(m_nSize);

	m_wndBrightnessTrack.Attach(GetDlgItem(IDC_BRIGHTNESS_SLIDER));
	m_wndBrightnessTrack.SetRange(1, 255);
	m_wndBrightnessTrack.SetPos(m_nBrightness);

	BOOL enableWatermark = m_wndWatermark.GetCurSel() != 0;
	BOOL enableTextWatermark = m_wndWatermark.GetCurSel() == 2;

	::EnableWindow(GetDlgItem(IDC_BROWSER), enableWatermark && !enableTextWatermark);
	::EnableWindow(GetDlgItem(IDC_FILE), enableWatermark && !enableTextWatermark);

	::EnableWindow(GetDlgItem(IDC_WATERMARK_TEXT), enableWatermark && enableTextWatermark);
	::EnableWindow(GetDlgItem(IDC_FONT_TYPE), enableWatermark && enableTextWatermark);
	::EnableWindow(GetDlgItem(IDC_FONT_STYLE), enableWatermark && enableTextWatermark);
	::EnableWindow(GetDlgItem(IDC_FONT_SIZE), enableWatermark && enableTextWatermark);
	::EnableWindow(GetDlgItem(IDC_ANGLE_SLIDER), enableWatermark && enableTextWatermark);

	::EnableWindow(GetDlgItem(IDC_WATERMARK_SIZE_SLIDER), enableWatermark);
	::EnableWindow(GetDlgItem(IDC_BRIGHTNESS_SLIDER), enableWatermark);
	::EnableWindow(GetDlgItem(IDC_PIINT_POSITION), enableWatermark);

	return TRUE;
}


void CWatermarkDlg::OnWatermarkTypeChanged(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (uNotifyCode == CBN_SELCHANGE && m_wndWatermark.m_hWnd == wndCtl.m_hWnd)
	{
		BOOL enableWatermark = m_wndWatermark.GetCurSel() != 0;
		BOOL enableTextWatermark = m_wndWatermark.GetCurSel() == 2;

		::EnableWindow(GetDlgItem(IDC_BROWSER), enableWatermark && !enableTextWatermark);
		::EnableWindow(GetDlgItem(IDC_FILE), enableWatermark && !enableTextWatermark);

		::EnableWindow(GetDlgItem(IDC_WATERMARK_TEXT), enableWatermark && enableTextWatermark);
		::EnableWindow(GetDlgItem(IDC_FONT_TYPE), enableWatermark && enableTextWatermark);
		::EnableWindow(GetDlgItem(IDC_FONT_STYLE), enableWatermark && enableTextWatermark);
		::EnableWindow(GetDlgItem(IDC_FONT_SIZE), enableWatermark && enableTextWatermark);
		::EnableWindow(GetDlgItem(IDC_ANGLE_SLIDER), enableWatermark && enableTextWatermark);

		::EnableWindow(GetDlgItem(IDC_WATERMARK_SIZE_SLIDER), enableWatermark);
		::EnableWindow(GetDlgItem(IDC_BRIGHTNESS_SLIDER), enableWatermark);
		::EnableWindow(GetDlgItem(IDC_PIINT_POSITION), enableWatermark);

		PropSheet_Changed(GetParent(), m_hWnd);
	}
}

LRESULT CWatermarkDlg::OnBrowserBtnClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFileDialog dlg(true, _T("*.bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, _T("位图文件\0*.bmp\0所有文件\0*.*\0\0"));

	if (dlg.DoModal() == IDOK)
	{
		m_szWatermarkBitmap = dlg.m_szFileName;

		DoDataExchange(false);
	}

	return 0;
}

LRESULT CWatermarkDlg::OnTrackBarChanged(UINT /*nSBCode*/, UINT /*nPos*/, CScrollBar pScrollBar)
{
	if (pScrollBar.m_hWnd == m_wndAngleTrack.m_hWnd || pScrollBar.m_hWnd == m_wndSizeTrack.m_hWnd || m_wndBrightnessTrack.m_hWnd == pScrollBar.m_hWnd)
	{
		m_nAngle = m_wndAngleTrack.GetPos();
		m_nSize = m_wndSizeTrack.GetPos();
		m_nBrightness = m_wndBrightnessTrack.GetPos();

		DoDataExchange(false);

		return 0;
	}
	SetMsgHandled(FALSE);
	return 0;
}
