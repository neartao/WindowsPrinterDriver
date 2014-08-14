// MaxiCodeDg.h : interface of the CMaxiCodeDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef MAXICODEDG_H
#define MAXICODEDG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"
#include "Controls.h"

/////////////////////////////////////////////////////////////////////////////
//
class CMaxiCodeDlg : public CPdkWin, public CWinDataExchange<CMaxiCodeDlg>
{
public:
    enum { IDD = IDD_MAXICODE };

	CMaxiCodeDlg(CPdkUIContext *pUICtxt);

    BEGIN_MSG_MAP_EX(CMaxiCodeDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
    END_MSG_MAP()

	virtual UINT GetDialogID() { return IDD; }
	
    LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	

protected:
};

#endif // !defined(MAXICODEDG_H)
