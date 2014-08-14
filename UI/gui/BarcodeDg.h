// BarcodeDg.h : interface of the CBarcodeDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BARCODEDG_H
#define BARCODEDG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"
#include "Controls.h"

/////////////////////////////////////////////////////////////////////////////
//
class CBarcodeDlg : public CPdkWin, public CWinDataExchange<CBarcodeDlg>
{
public:
    enum { IDD = IDD_BARCODE };

	CBarcodeDlg(CPdkUIContext *pUICtxt);

    BEGIN_MSG_MAP_EX(CBarcodeDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_CODE_HANDLER(EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_BARCODE_HRI_POSITION, CBN_SELCHANGE, OnHRIPositionComboxSelected)
		COMMAND_ID_HANDLER(IDC_HRI_FONTA, OnHRIFontTypeChanged)
		COMMAND_ID_HANDLER(IDC_HRI_FONTB, OnHRIFontTypeChanged)
    END_MSG_MAP()

	BEGIN_DDX_MAP(CBarcodeDlg)
		DDX_CONTROL(IDC_BARCODE_ITEM_WIDTH, m_wndItemWidth)
		DDX_INT_RANGE(IDC_BARCODE_ITEM_WIDTH, m_nItemWidth, 2, 6)
		DDX_CONTROL(IDC_BARCODE_ITEM_HEIGHT, m_wndItemHeight)
		DDX_INT_RANGE(IDC_BARCODE_ITEM_HEIGHT, m_nItemHeight, 1, 255)
		DDX_CONTROL(IDC_BARCODE_FONT_NAME, m_wndBarcodeFontName)
		DDX_CONTROL(IDC_BARCODE_TYPE, m_wndBarcodeType)
		DDX_CONTROL(IDC_BARCODE_HRI_POSITION, m_wndHRI)
	END_DDX_MAP()

	virtual UINT GetDialogID() { return IDD; }
	
    LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	
	LRESULT OnHRIFontTypeChanged(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void OnHRIPositionComboxSelected(UINT /*uNotifyCode*/, int /*nID*/, CWindow /*wndCtl*/);
	LRESULT OnEditChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

protected:
	CUpDownCtrl	m_wndItemWidthSpin;
	CEditImpl	m_wndItemWidth;
	int m_nItemWidth;

	CUpDownCtrl	m_wndItemHeightSpin;
	CEditImpl	m_wndItemHeight;
	int m_nItemHeight;

	CComboBoxImpl m_wndBarcodeFontName;
	CComboBoxImpl m_wndBarcodeType;
	CComboBoxImpl m_wndHRI;
};

#endif // !defined(BARCODEDG_H)
