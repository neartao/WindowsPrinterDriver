// AboutDg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef ABOUTDG_H
#define ABOUTDG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"
#include "Controls.h"

/////////////////////////////////////////////////////////////////////////////
//
class CAboutDlg : public CPdkWin, CWinDataExchange<CAboutDlg>
{
public:
    enum { IDD = IDD_ABOUTBOX };

	CAboutDlg(CPdkUIContext *pUICtxt);

    BEGIN_MSG_MAP_EX(CAboutDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER(IDC_FEED_BACK, OnButtonClick)
		COMMAND_ID_HANDLER(IDC_CHECK_UPDATE, OnButtonClick)
		//REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

	BEGIN_DDX_MAP(CAboutDlg)
		DDX_CONTROL(IDC_FEED_BACK, m_feedback)
		DDX_CONTROL(IDC_CHECK_UPDATE, m_checkupdate)
	END_DDX_MAP()

	virtual UINT GetDialogID() { return IDD; }
	
    LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	
    LRESULT OnButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

protected:
	CButtonImpl m_feedback;
	CButtonImpl m_checkupdate;
};

#endif // !defined(ABOUTDG_H)
