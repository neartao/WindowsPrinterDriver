// DownloadDg.h : interface of the CDownloadDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef DOWNLOADDG_H
#define DOWNLOADDG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"
#include "Controls.h"

/////////////////////////////////////////////////////////////////////////////
//
class CDownloadDlg : public CPdkWin, public CWinDataExchange<CDownloadDlg>
{
public:
	enum { IDD = IDD_DOWNLOAD };

	CDownloadDlg(CPdkUIContext *pUICtxt);

	BEGIN_MSG_MAP_EX(CDownloadDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER(IDC_BROWSER, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_DOWNLOAD_FILE, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_ADD, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_DELETE, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_DOWNLOAD_ALL, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_CLEAR_ALL, OnButtonClick)

		MESSAGE_HANDLER(WM_DRAWITEM, OnReflectNotifications)
		MESSAGE_HANDLER(WM_MEASUREITEM, OnReflectNotifications)
		MESSAGE_HANDLER(WM_COMPAREITEM, OnReflectNotifications)
		MESSAGE_HANDLER(WM_DELETEITEM, OnReflectNotifications)

		NOTIFY_HANDLER_EX(IDC_LOG_LIST, NM_CLICK, OnNotifyHandlerEX)
		//REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDownloadDlg)
		DDX_TEXT(IDC_FILE, m_szDownloadFile)
		DDX_CONTROL(IDC_LOG_LIST, m_logoList)
		DDX_CONTROL(IDC_LOG_PREVIEW, m_wndImage)
	END_DDX_MAP()

	virtual UINT GetDialogID() { return IDD; }

	LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	
	LRESULT OnButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNotifyHandlerEX(LPNMHDR pnmh);
	LRESULT OnReflectNotifications(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

protected:
	CString m_szDownloadFile;

	CODStaticImageImpl m_wndImage;
	CListViewCtrlImpl m_logoList;
};

#endif // !defined(DOWNLOADDG_H)
