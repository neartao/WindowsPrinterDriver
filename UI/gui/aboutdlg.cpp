// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "aboutdlg.h"

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CAboutDlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	CenterWindow(GetParent());
	HWND hStatic = GetDlgItem(IDC_DRIVER_NAME);
	::SetWindowText(hStatic, _T("Hello WTL!"));
	return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (wID == IDC_CALIBRATE)
		MessageBox(_T("Hello WTL!"), _T("WTL"));
	//EndDialog(wID);
	return 0;
}



CDocumentDlg::CDocumentDlg() : m_wndOKBtn(this, 1), m_wndExitBtn(this, 2),
m_wndListccw(this, 3), m_nEditNumber(0),
m_uEditUint(0), m_fEditFloat(0), m_dEditDouble(0),
m_nShowMsg(1)
{
}

CDocumentDlg::~CDocumentDlg()
{
}

LRESULT CDocumentDlg::OnInitDialog ( HWND hwndFocus, LPARAM lParam )
{
	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// Attach m_wndList to the list control.
	m_wndList.Attach ( GetDlgItem(IDC_LIST) );

	// Plain CContainedWindow: attach to OK and Exit buttons
	m_wndOKBtn.SubclassWindow ( GetDlgItem(IDOK) );
	m_wndExitBtn.SubclassWindow ( GetDlgItem(IDCANCEL) );

	// CContainedWindowT: attach to list ctrl
	m_wndListccw.SubclassWindow ( GetDlgItem(IDC_LIST) );

	// CButtonImpl: subclass the About button
	m_wndAboutBtn.SubclassWindow ( GetDlgItem(ID_APP_ABOUT) );

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	// Use the variables attached to the list:
	m_wndList.InsertColumn ( 0, _T("Macro"), LVCFMT_LEFT, 120, 0 );
	m_wndListccw.InsertColumn ( 1, _T("Result"), LVCFMT_LEFT, 90, 1 );

	// Fill in the tree
	HTREEITEM htiSummers, htiHank, htiDawn;

	htiSummers = m_wndTree.InsertItem ( _T("Summers"), TVI_ROOT, TVI_LAST );
	m_wndTree.InsertItem ( _T("Joyce"), htiSummers, TVI_LAST );
	htiHank = m_wndTree.InsertItem ( _T("Hank"), htiSummers, TVI_LAST );
	m_wndTree.InsertItem ( _T("Buffy"), htiHank, TVI_LAST );
	htiDawn = m_wndTree.InsertItem ( _T("Dawn"), htiHank, TVI_LAST );

	HTREEITEM htiVamps = m_wndTree.InsertItem ( _T("Vamps"), TVI_ROOT, htiSummers );
	HTREEITEM htiMaster, htiDarla, htiAngel, htiDru, htiSpike;

	htiMaster = m_wndTree.InsertItem ( _T("The Master"), htiVamps, TVI_LAST );
	htiDarla = m_wndTree.InsertItem ( _T("Darla"), htiMaster, TVI_LAST );
	htiAngel = m_wndTree.InsertItem ( _T("Angel"), htiDarla, TVI_LAST );
	htiDru = m_wndTree.InsertItem ( _T("Drusilla"), htiAngel, TVI_LAST );
	htiSpike = m_wndTree.InsertItem ( _T("Spike"), htiDru, TVI_LAST );

	m_wndTree.Expand ( htiSummers );
	m_wndTree.Expand ( htiHank );

	// Set the focus, and we're done.
	m_wndEdit.SetFocus();

	return FALSE;
}

void CDocumentDlg::OnAppAbout ( UINT uCode, int nID, HWND hWndCtl )
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
}

void CDocumentDlg::OnOK ( UINT uCode, int nID, HWND hWndCtl )
{
	CString str;

	// Transfer data from the controls to member variables.
	if ( !DoDataExchange(true) )
		return;

	m_wndList.DeleteAllItems();
	SetDlgItemText ( IDC_SEL_ITEM, _T("none") );

	m_wndList.InsertItem ( 0, _T("DDX_TEXT") );
	m_wndList.SetItemText ( 0, 1, m_sEditContents );

	str.Format ( _T("%d"), m_nEditNumber );
	m_wndList.InsertItem ( 1, _T("DDX_INT") );
	m_wndList.SetItemText ( 1, 1, str );

	str.Format ( _T("%u"), m_uEditUint );
	m_wndList.InsertItem ( 2, _T("DDX_UINT") );
	m_wndList.SetItemText ( 2, 1, str );

	str.Format ( _T("%f"), m_fEditFloat );
	m_wndList.InsertItem ( 3, _T("DDX_FLOAT(float)") );
	m_wndList.SetItemText ( 3, 1, str );

	str.Format ( _T("%lf"), m_dEditDouble );
	m_wndList.InsertItem ( 4, _T("DDX_FLOAT(double)") );
	m_wndList.SetItemText ( 4, 1, str );

	if ( m_nShowMsg )
		MessageBox ( _T("DDX complete!"), _T("ControlMania1"), MB_ICONINFORMATION );
}

void CDocumentDlg::OnCancel ( UINT uCode, int nID, HWND hWndCtl )
{
	//EndDialog(nID);
}

LRESULT CDocumentDlg::OnListItemchanged ( NMHDR* phdr )
{
	NMLISTVIEW* pnmlv = (NMLISTVIEW*) phdr;
	int nSelItem = m_wndList.GetSelectedIndex();
	CString sMsg;

	// If no item is selected, show "none". Otherwise, show its index.
	if ( -1 == nSelItem )
		sMsg = _T("(none)");
	else
		sMsg.Format ( _T("%d"), nSelItem );

	SetDlgItemText ( IDC_SEL_ITEM, sMsg );
	return 0;   // retval ignored
}

LRESULT CDocumentDlg::OnSetCursor_OK(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg)
{
	static HCURSOR hcur = LoadCursor ( NULL, IDC_HAND );

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

LRESULT CDocumentDlg::OnSetCursor_Exit(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg)
{
	static HCURSOR hcur = LoadCursor ( NULL, IDC_NO );

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

void CDocumentDlg::OnDataExchangeError ( UINT nCtrlID, BOOL bSave )
{
	CString str;

	str.Format ( _T("DDX error during exchange with control: %u"), nCtrlID );
	MessageBox ( str, _T("ControlMania1"), MB_ICONWARNING );

	::SetFocus ( GetDlgItem(nCtrlID) );
}
