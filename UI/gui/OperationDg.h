// OperationDg.h : interface of the COperationDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef OPERATIONLG_H
#define OPERATIONLG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"
#include "Controls.h"

/////////////////////////////////////////////////////////////////////////////
//
enum kCommandPosition
{
	kCommandBegin,
	kBeginDoc,
	kBeginPage,
	kEndPage,
	kEndDoc,
	kCommandEnd,
};


/////////////////////////////////////////////////////////////////////////////
//
class CCommandDlg : public CDialogImpl<CCommandDlg>, public CWinDataExchange<CCommandDlg>
{
public:
	CCommandDlg(kCommandPosition command, COLORREF tColor);

	CCommandDlg(kCommandPosition command, HBRUSH brush);

	~CCommandDlg();

    enum { IDD = IDD_COMMAND };

    BEGIN_MSG_MAP(CCommandDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
        COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
        COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_SEND_COMMAND, OnSendCommandClick)
		COMMAND_ID_HANDLER(IDC_DEFAULT, OnDefaultButtonClick)
		COMMAND_HANDLER_EX(IDC_LOGO_PRINT, CBN_SELCHANGE, OnLogPrintComboxSelected)
		REFLECT_NOTIFICATIONS()
		ALT_MSG_MAP(1)
    END_MSG_MAP()

	BEGIN_DDX_MAP(CCommandDlg)
		DDX_CONTROL(IDC_HEX_COMMAND, m_wndHex)
		DDX_CONTROL(IDC_CUT_OPTION, m_cutOptions)
		DDX_CONTROL(IDC_LOGO_PRINT, m_logPrintOptions)
		DDX_CHECK(IDC_SEND_COMMAND, m_bSendCommand)
		DDX_INT(IDC_FEED_PAPER, m_nFeedPaper)
		DDX_INT_RANGE(IDC_FEED_PAPER, m_nFeedPaper, 0, 100)
	END_DDX_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDefaultButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSendCommandClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOpenCashRadioClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void OnLogPrintComboxSelected(UINT /*uNotifyCode*/, int /*nID*/, CWindow /*wndCtl*/);

	LRESULT OnEraseBkgnd(HDC /*hDC*/);
	LRESULT OnCtlColorStatic(HDC /*hDC*/, HWND /*hWnd*/);

protected:
	kCommandPosition m_commandType;
	CBrush m_hbrBkgnd;

	CHexEditImpl m_wndHex;
	CComboBox m_cashCombox;
	CComboBoxImpl m_cutOptions;
	CComboBoxImpl m_logPrintOptions;
	CContainedWindowT<CComboBox> m_buzzerCombox;
	CUpDownCtrl	m_feedPaperSpin;
	CEditImpl	m_feedPaper;
	
	bool m_bSendCommand;
	int m_nFeedPaper;
};

/////////////////////////////////////////////////////////////////////////////
//
class COperationDlg : public CPdkWin
{
public:
    enum { IDD = IDD_OPERATION };

	COperationDlg(CPdkUIContext *pUICtxt);

    BEGIN_MSG_MAP_EX(COperationDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER(IDC_BEGIN_DOC, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_BEGIN_PAGE, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_END_PAGE, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_END_DOC, OnButtonClick)
    END_MSG_MAP()

	virtual UINT GetDialogID() { return IDD; }
	
    LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	
    LRESULT OnButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif // !defined(OPERATIONLG_H)
