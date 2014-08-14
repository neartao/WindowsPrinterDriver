// LayoutDg.h : interface of the CLayoutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef LAYOUTDG_H
#define LAYOUTDG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"
#include "Controls.h"

/////////////////////////////////////////////////////////////////////////////
//
class CLayoutDlg : public CPdkWin, public CWinDataExchange<CLayoutDlg>
{
public:
    enum { IDD = IDD_LAYOUT };

	CLayoutDlg(CPdkUIContext *pUICtxt);

    BEGIN_MSG_MAP_EX(CLayoutDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER(IDC_ZOOM, OnZoomCheckBoxClick)
		COMMAND_ID_HANDLER(IDC_PORTRAIT, OnPrintDirectionChanged)
		COMMAND_ID_HANDLER(IDC_LANDSCAPE, OnPrintDirectionChanged)
		COMMAND_ID_HANDLER(IDC_ROTATE_180, OnRotatePrintChanged)
		COMMAND_ID_HANDLER(IDC_PRINT_AS_BITMAP, OnPrintTypeChanged)
		COMMAND_ID_HANDLER(IDC_PRINT_AS_TEXT, OnPrintTypeChanged)
		COMMAND_HANDLER_EX(IDC_PAPER_SIZE, CBN_SELCHANGE, OnFormsComboxSelected)

		MESSAGE_HANDLER(WM_DRAWITEM, OnReflectNotifications)
		MESSAGE_HANDLER(WM_MEASUREITEM, OnReflectNotifications)
		MESSAGE_HANDLER(WM_COMPAREITEM, OnReflectNotifications)
		MESSAGE_HANDLER(WM_DELETEITEM, OnReflectNotifications)
		//REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

	BEGIN_DDX_MAP(CLayoutDlg)
		DDX_CONTROL(IDC_ZOOM_VALUE, m_wndZoom)
		DDX_CONTROL(IDC_PAPER_SIZE, m_wndForms)
		DDX_INT_RANGE(IDC_ZOOM_VALUE, m_nZoomValue, 0, 200)
		DDX_CONTROL(IDC_ORIENTATION, m_wndOrientation)
	END_DDX_MAP()

	virtual UINT GetDialogID() { return IDD; }
	
    LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	
	LRESULT OnPrintDirectionChanged(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRotatePrintChanged(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPrintTypeChanged(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnZoomCheckBoxClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void OnFormsComboxSelected(UINT /*uNotifyCode*/, int /*nID*/, CWindow /*wndCtl*/);

	LRESULT OnReflectNotifications(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

protected:
	CUpDownCtrl	m_wndZoomSpin;
	CEditImpl	m_wndZoom;
	CComboBoxImpl m_wndForms;
	int m_nZoomValue;

	CODStaticImageImpl m_wndOrientation;
};

#endif // !defined(LAYOUTDG_H)
