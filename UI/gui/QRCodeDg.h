// QRCodeDg.h : interface of the CQRCodeDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef QRCODEDG_H
#define QRCODEDG_H

#include "PdkWin.h"
#include "globals.h"
#include "resource.h"
#include "Controls.h"

/////////////////////////////////////////////////////////////////////////////
//
class CQRCodeDlg : public CPdkWin, public CWinDataExchange<CQRCodeDlg>
{
public:
    enum { IDD = IDD_QRCODE };

	CQRCodeDlg(CPdkUIContext *pUICtxt);

    BEGIN_MSG_MAP_EX(CQRCodeDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
    END_MSG_MAP()

	virtual UINT GetDialogID() { return IDD; }
	
    LRESULT OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);	

protected:
};

#endif // !defined(QRCODEDG_H)
