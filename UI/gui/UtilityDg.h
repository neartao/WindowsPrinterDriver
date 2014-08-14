// UtilityDg.h : interface of the CUtilityDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef UTILITYDG_H
#define UTILITYDG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"
#include "Controls.h"

/////////////////////////////////////////////////////////////////////////////
//
class CControlFontTable;
class CControlFont : public CDialogImpl<CControlFont>, public CWinDataExchange<CControlFont>
{
	friend class CControlFontTable;
public:
	enum { IDD = IDD_CONTROL_FONT };

	CControlFont(TCHAR command_char, LPCTSTR lpstrCommand, COLORREF tColor);
	~CControlFont();

	BEGIN_MSG_MAP_EX(CControlFont)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
        COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
        COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CControlFont)
		DDX_CONTROL(IDC_CONTROL_FONT_COMMAND, m_wndHex)
		DDX_TEXT(IDC_CONTROL_FONT_COMMAND, m_szCommand)
	END_DDX_MAP()

	LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkgnd(HDC /*hDC*/);
	LRESULT OnCtlColorStatic(HDC /*hDC*/, HWND /*hWnd*/);

protected:
	TCHAR m_tChar;
	CString m_szCommand;
	CBrush m_hbrBkgnd;

	CHexEditImpl m_wndHex;
};

/////////////////////////////////////////////////////////////////////////////
//
class CControlFontTable : public CDialogImpl<CControlFontTable>, public CWinDataExchange<CControlFontTable>
{
public:
	enum { IDD = IDD_CONTROL_FONT_TABLE };

	CControlFontTable(COLORREF tColor);
	~CControlFontTable();

	BEGIN_MSG_MAP_EX(CControlFontTable)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
        COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
        COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_EDIT, OnOperationCmd)
		COMMAND_ID_HANDLER(IDC_DELETE, OnOperationCmd)
		NOTIFY_HANDLER_EX(IDC_CONTROL_FONT_TABLE, NM_DBLCLK, OnNotifyHandlerEX)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CControlFontTable)
		DDX_CONTROL(IDC_CONTROL_FONT_TABLE, m_cfList)
		DDX_TEXT(IDC_DRIVER_NAME, m_szDriverName)
	END_DDX_MAP()

	LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOperationCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkgnd(HDC /*hDC*/);
	LRESULT OnCtlColorStatic(HDC /*hDC*/, HWND /*hWnd*/);
	LRESULT OnNotifyHandlerEX(LPNMHDR pnmh);

protected:
	COLORREF m_bkColor;
	CBrush m_hbrBkgnd;

	CListViewCtrlImpl m_cfList;
	CString m_szDriverName;
};

/////////////////////////////////////////////////////////////////////////////
//
class CUtilityDlg : public CPdkWin, public CWinDataExchange<CUtilityDlg>
{
public:
    enum { IDD = IDD_TOOL };

	CUtilityDlg(CPdkUIContext *pUICtxt);

    BEGIN_MSG_MAP_EX(CUtilityDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER(IDC_PRINT_SELFTEST, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_FEED_PAPER_TEST, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_CUT_PAPER_TEST, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_CONTROL_FONT, OnButtonClick)
    END_MSG_MAP()

	virtual UINT GetDialogID() { return IDD; }
	
    LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	
    LRESULT OnButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

protected:
	CButtonImpl m_feedback;
	CButtonImpl m_checkupdate;
};

#endif // !defined(UTILITYDG_H)
