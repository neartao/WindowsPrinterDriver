// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
class CColorCtl : public CMessageMap
{
public:
	CColorCtl() 
	{
		m_hbrBkgnd = CreateSolidBrush(GetSysColor(COLOR_DESKTOP));
	}

	~CColorCtl() { DeleteObject ( m_hbrBkgnd ); }

	BEGIN_MSG_MAP_EX(CColorCtl)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
		MSG_WM_CTLCOLORLISTBOX(OnCtlColorListBox)
		MSG_WM_CTLCOLORBTN(OnCtlColorBtn)
		MSG_WM_CTLCOLORDLG(OnCtlColorDlg)
		MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
		MSG_WM_CTLCOLORSCROLLBAR(OnCtlColorScrollBar)
	END_MSG_MAP()

protected:
	LRESULT OnEraseBkgnd(HDC hDC)
	{
		T* pT = static_cast<T*>(this);
		RECT rcClient;

		pT->GetClientRect ( &rcClient );
		FillRect ( hDC, &rcClient, m_hbrBkgnd );
		SetMsgHandled(FALSE);
		return 1;    // we painted the background
	}

	LRESULT OnCtlColorEdit(HDC hDC, HWND hWnd)
	{
		return (LRESULT)m_hbrBkgnd;
	}

	LRESULT OnCtlColorListBox(HDC hDC, HWND hWnd)
	{
		return (LRESULT)m_hbrBkgnd;
	}

	LRESULT OnCtlColorBtn(HDC hDC, HWND hWnd)
	{
		return (LRESULT)m_hbrBkgnd;
	}

	LRESULT OnCtlColorDlg(HDC hDC, HWND hWnd)
	{
		return (LRESULT)m_hbrBkgnd;
	}

	LRESULT OnCtlColorStatic(HDC hDC, HWND hWnd)
	{
		return (LRESULT)m_hbrBkgnd;
	}

	LRESULT OnCtlColorScrollBar(HDC hDC, HWND hWnd)
	{
		return (LRESULT)m_hbrBkgnd;
	}

protected:
	HBRUSH m_hbrBkgnd;
};

/////////////////////////////////////////////////////////////////////////////
//
class CAboutDlg : public CPdkWin, public CColorCtl<CAboutDlg>
{
public:
    enum { IDD = IDD_ABOUTBOX };

	typedef CColorCtl<CAboutDlg> CColorCtlBase;

    BEGIN_MSG_MAP_EX(CAboutDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
        COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_CALIBRATE, OnCloseCmd)
		//CHAIN_MSG_MAP(CColorCtlBase)
    END_MSG_MAP()

	virtual UINT GetDialogID() { return IDD; }
	
    LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	
    LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

class CDeviceDlg : public CAboutDlg
{
public:
	enum { IDD = IDD_DEVICE_PROPPAGE };

    BEGIN_MSG_MAP_EX(CDeviceDlg)
		CHAIN_MSG_MAP(CAboutDlg)
    END_MSG_MAP()

	virtual UINT GetDialogID() { return IDD; }
};

class CToolDlg : public CAboutDlg
{
public:
	enum { IDD = IDD_TOOL };

    BEGIN_MSG_MAP_EX(CToolDlg)
		CHAIN_MSG_MAP(CAboutDlg)
    END_MSG_MAP()

	virtual UINT GetDialogID() { return IDD; }
};


class CLayoutDlg : public CAboutDlg
{
public:
	enum { IDD = IDD_LAYOUT };

    BEGIN_MSG_MAP_EX(CLayoutDlg)
		CHAIN_MSG_MAP(CAboutDlg)
    END_MSG_MAP()

	virtual UINT GetDialogID() { return IDD; }
};

class COperationDlg : public CAboutDlg
{
public:
	enum { IDD = IDD_OPERATION };

    BEGIN_MSG_MAP_EX(COperationDlg)
		CHAIN_MSG_MAP(CAboutDlg)
    END_MSG_MAP()

	virtual UINT GetDialogID() { return IDD; }
};

// CButtonImpl - CWindowImpl-derived class that implements a button.  We need a
// class like this to do subclassing or DDX.
class CButtonImpl : public CWindowImpl<CButtonImpl, CButton>
{
    BEGIN_MSG_MAP(CButtonImpl)
        MSG_WM_SETCURSOR(OnSetCursor)
    END_MSG_MAP()

	LRESULT OnSetCursor(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg)
	{
		static HCURSOR hcur = LoadCursor ( NULL, IDC_SIZEALL );

		if ( NULL != hcur )
		{
			SetCursor ( hcur );
			return TRUE;
		}
		else
		{
			SetMsgHandled(false);
			return FALSE;
		}
	}
};


// CEditImpl - CWindowImpl-derived class that implements a button.  We need a
// class like this to do subclassing or DDX.
class CEditImpl : public CWindowImpl<CEditImpl, CEdit>
{
    BEGIN_MSG_MAP(CEditImpl)
        MSG_WM_CONTEXTMENU(OnContextMenu)
    END_MSG_MAP()

    void OnContextMenu ( HWND hwndCtrl, CPoint ptClick )
    {
        MessageBox(_T("Edit control handled WM_CONTEXTMENU"));
    }
};


// A tree control that doesn't allow nodes to be collapsed, and has custom
// draw handlers.
class CBuffyTreeCtrl : public CWindowImpl<CBuffyTreeCtrl, CTreeViewCtrl>,
                       public CCustomDraw<CBuffyTreeCtrl>
{
public:
	BEGIN_MSG_MAP(CBuffyTreeCtrl)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(TVN_ITEMEXPANDING, OnItemExpanding)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	LRESULT OnItemExpanding ( NMHDR* phdr )
	{
		NMTREEVIEW* pnmtv = (NMTREEVIEW*) phdr;

		if ( pnmtv->action & TVE_COLLAPSE )
			return TRUE;    // don't allow it
		else
			return FALSE;   // allow it
	}
};

class CDocumentDlg : public CPdkWin, public CWinDataExchange<CDocumentDlg>
{
public:
	enum { IDD = IDD_DOC_PROPPAGE };

    CDocumentDlg();
    ~CDocumentDlg();

	virtual UINT GetDialogID() { return IDD; }

	BEGIN_MSG_MAP(CDocumentDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		//MSG_WM_ERASEBKGND(OnEraseBkgnd)
		COMMAND_ID_HANDLER_EX(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER_EX(IDOK, OnOK)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		NOTIFY_HANDLER_EX(IDC_LIST, LVN_ITEMCHANGED, OnListItemchanged)
		//REFLECT_NOTIFICATIONS() // 不支持自会控件
		ALT_MSG_MAP(1)
		MSG_WM_SETCURSOR(OnSetCursor_OK)
		ALT_MSG_MAP(2)
		MSG_WM_SETCURSOR(OnSetCursor_Exit)
		ALT_MSG_MAP(3)
		// no msg handlers for the list control
	END_MSG_MAP()

    // NOTE: Comment out the DDX_INT and DDX_UINT lines if you want to see
    // DDX_FLOAT handle input like "3.14". The decimal point makes DDX_INT/UINT
    // error because it's not a number.
    BEGIN_DDX_MAP(CDocumentDlg)
        DDX_CONTROL(IDC_EDIT, m_wndEdit)
        DDX_TEXT(IDC_EDIT, m_sEditContents)
        DDX_INT(IDC_EDIT, m_nEditNumber)
        DDX_UINT(IDC_EDIT, m_uEditUint)
        DDX_FLOAT(IDC_EDIT, m_fEditFloat)
        DDX_FLOAT(IDC_EDIT, m_dEditDouble)
        DDX_CHECK(IDC_SHOW_MSG, m_nShowMsg)
        DDX_CONTROL(IDC_TREE, m_wndTree)
    END_DDX_MAP()

    LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);
    void    OnAppAbout(UINT uCode, int nID, HWND hWndCtl);
    void    OnOK(UINT uCode, int nID, HWND hWndCtl);
    void    OnCancel(UINT uCode, int nID, HWND hWndCtl);
    LRESULT OnListItemchanged(NMHDR* phdr);

    LRESULT OnSetCursor_OK(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg);
    LRESULT OnSetCursor_Exit(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg);

    // DDX override
    void OnDataExchangeError(UINT nCtrlID, BOOL bSave);

	LRESULT OnEraseBkgnd(HDC hDC)
	{
		return 1;    // we painted the background
	}

protected:
    CListViewCtrl m_wndList;
    CContainedWindow m_wndOKBtn, m_wndExitBtn;
    CContainedWindowT<CListViewCtrl> m_wndListccw;
    CButtonImpl      m_wndAboutBtn;
    CEditImpl        m_wndEdit;
    CBuffyTreeCtrl   m_wndTree;

    // DDX variables
    CString     m_sEditContents;
    int         m_nEditNumber;
    UINT        m_uEditUint;
    float       m_fEditFloat;
    double      m_dEditDouble;
    int         m_nShowMsg;
};

#endif // !defined(ABOUTDLG_H)
