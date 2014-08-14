// WatermarkDg.h : interface of the CWatermarkDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef WATERMARKDG_H
#define WATERMARKDG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"
#include "Controls.h"

/////////////////////////////////////////////////////////////////////////////
//
class CWatermarkDlg : public CPdkWin, public CWinDataExchange<CWatermarkDlg>
{
public:
    enum { IDD = IDD_WATERMARK };

	CWatermarkDlg(CPdkUIContext *pUICtxt);

    BEGIN_MSG_MAP_EX(CWatermarkDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_HANDLER_EX(IDC_WATERMARK_TYPE, CBN_SELCHANGE, OnWatermarkTypeChanged)
		COMMAND_ID_HANDLER(IDC_BROWSER, OnBrowserBtnClick)
		MSG_WM_HSCROLL(OnTrackBarChanged)
		MSG_WM_HSCROLL(OnTrackBarChanged)
		MSG_WM_HSCROLL(OnTrackBarChanged)
    END_MSG_MAP()

	BEGIN_DDX_MAP(CWatermarkDlg)
		DDX_CONTROL(IDC_WATERMARK_TYPE, m_wndWatermark);
		DDX_CONTROL(IDC_WATERMARK_TEXT, m_wndWatermarkText)
		DDX_CONTROL(IDC_FONT_TYPE, m_wndFontType)
		DDX_CONTROL(IDC_FONT_STYLE, m_wndFontStyle)
		DDX_CONTROL(IDC_FONT_SIZE, m_wndFontSize)
		DDX_TEXT(IDC_FILE, m_szWatermarkBitmap)
		DDX_INT_RANGE(IDC_ANGLE, m_nAngle, 0, 360)
		DDX_INT_RANGE(IDC_WATERMARK_SIZE, m_nSize, 1, 100)
		DDX_INT_RANGE(IDC_BRIGHTNESS, m_nBrightness, 1, 255)
		DDX_CONTROL(IDC_LOG_PREVIEW, m_wndImage)
		DDX_CONTROL(IDC_PIINT_POSITION, m_wndWatermarkPosition)
	END_DDX_MAP()

	virtual UINT GetDialogID() { return IDD; }
	
    LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	
    void OnWatermarkTypeChanged(UINT /*uNotifyCode*/, int /*nID*/, CWindow /*wndCtl*/);
	LRESULT OnBrowserBtnClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTrackBarChanged(UINT /*nSBCode*/, UINT /*nPos*/, CScrollBar /*pScrollBar*/);

protected:
	CComboBoxImpl m_wndWatermark;
	CComboBoxImpl m_wndWatermarkText;
	CComboBoxImpl m_wndFontType;
	CComboBoxImpl m_wndFontStyle;
	CComboBoxImpl m_wndFontSize;
	CComboBoxImpl m_wndWatermarkPosition;

	CTrackBarCtrl m_wndAngleTrack;
	CTrackBarCtrl m_wndSizeTrack;
	CTrackBarCtrl m_wndBrightnessTrack;

	CODStaticImageImpl m_wndImage;

	int m_nAngle;
	int m_nSize;
	int m_nBrightness;

	CString m_szWatermarkBitmap;
};

#endif // !defined(WATERMARKDG_H)
