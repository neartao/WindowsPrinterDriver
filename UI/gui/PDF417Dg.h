// PDF417Dg.h : interface of the CPDF417Dlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef PDF417DG_H
#define PDF417DG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"
#include "Controls.h"

/////////////////////////////////////////////////////////////////////////////
//
class CPDF417Dlg : public CPdkWin, public CWinDataExchange<CPDF417Dlg>
{
public:
    enum { IDD = IDD_PDF417 };

	CPDF417Dlg(CPdkUIContext *pUICtxt);

    BEGIN_MSG_MAP_EX(CPDF417Dlg)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_CODE_HANDLER(EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_ERROR_CORRECTION, CBN_SELCHANGE, OnErrorCorrectionComboxSelected)
    END_MSG_MAP()

	BEGIN_DDX_MAP(CPDF417Dlg)
		DDX_CONTROL(IDC_BARCODE_ITEM_WIDTH, m_wndItemWidth)
		DDX_INT_RANGE(IDC_BARCODE_ITEM_WIDTH, m_nItemWidth, 1, 7)
		DDX_CONTROL(IDC_BARCODE_ITEM_HEIGHT, m_wndItemHeight)
		DDX_INT_RANGE(IDC_BARCODE_ITEM_HEIGHT, m_nItemHeight, 2, 25)

		DDX_CONTROL(IDC_COLUMN_NUM, m_wndColumnNum)
		DDX_INT_RANGE(IDC_COLUMN_NUM, m_nColumnNum, 1, 30)
		DDX_CONTROL(IDC_CELL_NUM, m_wndCellNum)
		DDX_INT_RANGE(IDC_CELL_NUM, m_nCellNum, 3, 90)

		DDX_CONTROL(IDC_ERROR_CORRECTION, m_wndErrorCorrection)
	END_DDX_MAP()

	virtual UINT GetDialogID() { return IDD; }
	
    LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	
	void OnErrorCorrectionComboxSelected(UINT /*uNotifyCode*/, int /*nID*/, CWindow /*wndCtl*/);
	LRESULT OnEditChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

protected:
	CUpDownCtrl	m_wndItemWidthSpin;
	CEditImpl	m_wndItemWidth;
	int m_nItemWidth;

	CUpDownCtrl	m_wndItemHeightSpin;
	CEditImpl	m_wndItemHeight;
	int m_nItemHeight;

	CUpDownCtrl	m_wndColumnNumSpin;
	CEditImpl	m_wndColumnNum;
	int m_nColumnNum;

	CUpDownCtrl	m_wndCellNumSpin;
	CEditImpl	m_wndCellNum;
	int m_nCellNum;

	CComboBoxImpl m_wndErrorCorrection;
};

#endif // !defined(PDF417DG_H)
