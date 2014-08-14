// MaxiCodeDg.cpp : implementation of the CMaxiCodeDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "MaxiCodeDg.h"
#include "PdkApp.h"
#include "PdkUICtx.h"
#include <atlprint.h>

/////////////////////////////////////////////////////////////////////////////
//
CMaxiCodeDlg::CMaxiCodeDlg(CPdkUIContext *pUICtxt)
: CPdkWin(pUICtxt)
{}

LRESULT CMaxiCodeDlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	CenterWindow(GetParent());

	// First DDX call, hooks up variables to controls.
	//DoDataExchange(false);

	return TRUE;
}