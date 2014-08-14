// QRCodeDg.cpp : implementation of the CQRCodeDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "QRCodeDg.h"
#include "PdkApp.h"
#include "PdkUICtx.h"
#include <atlprint.h>

/////////////////////////////////////////////////////////////////////////////
//
CQRCodeDlg::CQRCodeDlg(CPdkUIContext *pUICtxt)
: CPdkWin(pUICtxt)
{}

LRESULT CQRCodeDlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	CenterWindow(GetParent());

	// First DDX call, hooks up variables to controls.
	//DoDataExchange(false);

	return TRUE;
}