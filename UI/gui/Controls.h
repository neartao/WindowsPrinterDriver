// 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef CONTROLS_H
#define CONTROLS_H

//CODStaticImageImpl - CWindowImpl-derived class that implements a image static.  We need a
// class like this to do subclassing or DDX.
class CODStaticImageImpl : public CWindowImpl<CODStaticImageImpl, CStatic>,
	public COwnerDraw<CODStaticImageImpl>
{
public:
	BEGIN_MSG_MAP_EX(CODStaticImageImpl)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CODStaticImageImpl>, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP();

	void SetImageFile(const CString& szFile)
	{
		m_nBitmapId = 0;
		m_szImagePath = szFile;
		Invalidate();
	}

	void SetBitmapId(UINT id)
	{
		m_nBitmapId = id;
		m_szImagePath.Empty();
		Invalidate();
	}

	void DrawItem (LPDRAWITEMSTRUCT lpdis)
	{
		if (!m_szImagePath.IsEmpty())
		{
			RECT rcClient;
			GetClientRect(&rcClient);
			CDCHandle dc = lpdis->hDC;
			dc.SaveDC();

			HBITMAP       hBitmap, hOldBitmap;
			HPALETTE      hPalette, hOldPalette;
			BITMAP        bm;
			if( LoadBitmapFromBMPFile( m_szImagePath.LockBuffer(), &hBitmap, &hPalette ) )
			{
				GetObject(hBitmap, sizeof(BITMAP), &bm);
				HDC hMemDC = CreateCompatibleDC(dc.m_hDC);
				hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
				hOldPalette = dc.SelectPalette(hPalette, FALSE);
				dc.RealizePalette();

				dc.StretchBlt(rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,\
					hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY );

				SelectObject(hMemDC, hOldBitmap);
				DeleteObject(hBitmap);
				dc.SelectPalette(hOldPalette, FALSE);
				DeleteObject(hPalette);
			}
			m_szImagePath.UnlockBuffer();

			dc.RestoreDC(-1);
		}
		else if (m_nBitmapId > 0)
		{
			RECT rcClient;
			GetClientRect(&rcClient);
			CDCHandle dc = lpdis->hDC;
			dc.SaveDC();

			CBitmap bitmap;
			_U_STRINGorID id (m_nBitmapId);
			bitmap.LoadBitmap(id);

			HBITMAP       hOldBitmap;
			BITMAP        bm;

			GetObject(bitmap.m_hBitmap, sizeof(BITMAP), &bm);
			HDC hMemDC = CreateCompatibleDC(dc.m_hDC);
			hOldBitmap = (HBITMAP)SelectObject(hMemDC, bitmap.m_hBitmap);

			dc.BitBlt(rcClient.left + (rcClient.right - rcClient.left - bm.bmWidth) / 2, \
				rcClient.top + (rcClient.bottom - rcClient.top - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight,\
				hMemDC, 0, 0, SRCCOPY );

			SelectObject(hMemDC, hOldBitmap);


			dc.RestoreDC(-1);
		}

		return;
	}

private:
	BOOL LoadBitmapFromBMPFile( LPTSTR szFileName, HBITMAP *phBitmap,
		HPALETTE *phPalette )
	{
		BITMAP  bm;

		*phBitmap = NULL;
		*phPalette = NULL;

		// Use LoadImage() to get the image loaded into a DIBSection
		*phBitmap = (HBITMAP)LoadImage( NULL, szFileName, IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
		if( *phBitmap == NULL )
			return FALSE;

		// Get the color depth of the DIBSection
		GetObject(*phBitmap, sizeof(BITMAP), &bm );
		// If the DIBSection is 256 color or less, it has a color table
		if( ( bm.bmBitsPixel * bm.bmPlanes ) <= 8 )
		{
			HDC           hMemDC;
			HBITMAP       hOldBitmap;
			RGBQUAD       rgb[256];
			LPLOGPALETTE  pLogPal;
			WORD          i;

			// Create a memory DC and select the DIBSection into it
			hMemDC = CreateCompatibleDC( NULL );
			hOldBitmap = (HBITMAP)SelectObject( hMemDC, *phBitmap );
			// Get the DIBSection's color table
			GetDIBColorTable( hMemDC, 0, 256, rgb );
			// Create a palette from the color tabl
			pLogPal = (LOGPALETTE *)malloc( sizeof(LOGPALETTE) + (256*sizeof(PALETTEENTRY)) );
			pLogPal->palVersion = 0x300;
			pLogPal->palNumEntries = 256;
			for(i=0;i<256;i++)
			{
				pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
				pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
				pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
				pLogPal->palPalEntry[i].peFlags = 0;
			}
			*phPalette = CreatePalette( pLogPal );
			// Clean up
			free( pLogPal );
			SelectObject( hMemDC, hOldBitmap );
			DeleteDC( hMemDC );
		}
		else   // It has no color table, so use a halftone palette
		{
			HDC    hRefDC;

			hRefDC = GetDC();
			*phPalette = CreateHalftonePalette( hRefDC );
			ReleaseDC( hRefDC );
		}
		return TRUE;

	}

protected:
	CString m_szImagePath;
	UINT	m_nBitmapId;
};

// CButtonImpl - CWindowImpl-derived class that implements a button.  We need a
// class like this to do subclassing or DDX.
class CButtonImpl : public CWindowImpl<CButtonImpl, CButton>
{
    BEGIN_MSG_MAP(CButtonImpl)
    END_MSG_MAP()
};


// CEditImpl - CWindowImpl-derived class that implements a edit.  We need a
// class like this to do subclassing or DDX.
class CEditImpl : public CWindowImpl<CEditImpl, CEdit>
{
    BEGIN_MSG_MAP(CEditImpl)
    END_MSG_MAP()
};

// CComboBoxImpl - CWindowImpl-derived class that implements a combox.  We need a
// class like this to do subclassing or DDX.
class CComboBoxImpl : public CWindowImpl<CComboBoxImpl, CComboBox>
{
	BEGIN_MSG_MAP(CComboBoxImpl)
	END_MSG_MAP()
};

// CListViewCtrlImpl - CWindowImpl-derived class that implements a listview.  We need a
// class like this to do subclassing or DDX.
class CListViewCtrlImpl : public CWindowImpl<CListViewCtrlImpl, CListViewCtrl>
{
	BEGIN_MSG_MAP(CListViewCtrlImpl)
	END_MSG_MAP()
};

// CHexEditImpl - CWindowImpl-derived class that implements a hex edit.  We need a
// class like this to do subclassing or DDX.
class CHexEditImpl : public CWindowImpl<CHexEditImpl, CEdit>
{
public:
    BEGIN_MSG_MAP(CHexEditImpl)
		MSG_WM_CHAR(OnChar)
		MSG_WM_CONTEXTMENU(OnContextMenu)
    END_MSG_MAP()

protected:
    void OnContextMenu ( HWND hwndCtrl, CPoint ptClick )
    {
        MessageBox(_T("Edit control handled WM_CONTEXTMENU"));
    }

	LRESULT OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		switch (nChar) 
		{ 
		case 0x08:
			// Process a backspace.
			break;
		case 0x0A:
			// Process a linefeed.
			break;
		case 0x1B:
			// Process an escape.
			break;
		case 0x09:
			// Process a tab.
			break;
		case 0x0D:
			// Process a carriage return.
			break;
		default:
			// Process displayable characters.
			if (isdigit(nChar) || ((nChar >= 'A' && nChar <= 'F') || ((nChar >= 'a' && nChar <= 'f'))))
			{
				DefWindowProc(m_pCurrentMsg->message, toupper(nChar), m_pCurrentMsg->lParam);
				//bool insert = false;
				//TCHAR szBuf[MAX_PATH + 1] = {0};
				//POINT pt = {0};			

				//GetWindowText(szBuf, MAX_PATH);
				//CString strText(szBuf);

				//GetCaretPos(&pt);
				//int index = CharFromPos(pt);

				//_stprintf(szBuf, _T("\nxxxxxxxxxxxxxx%d, pt.x = %d, pt.y = %d\n"), index, pt.x, pt.y);
				//OutputDebugString(szBuf);

				//if (index < 0 && !strText.IsEmpty())
				//	index = strText.GetLength() - 1;

				//if (index < 0)
				//	index = 0;

				//OutputDebugString(strText);
				//if (index >= 0 && index < strText.GetLength())
				//{
				//	_stprintf(szBuf, _T("\nxxxxxxxxxxxxxx%d, pt.x = %d, pt.y = %d, %c\n"), index, pt.x, pt.y, strText[index]);
				//	OutputDebugString(szBuf);
				//}

				////			  |
				////			  |
				////			  | insert here
				//// case : 12 1|
				////
				//if (index > 0 && !strText.IsEmpty() && strText.GetLength() == index && strText[index - 1] != ' ')
				//{
				//	insert = true;
				//	DefWindowProc(m_pCurrentMsg->message, toupper(nChar), m_pCurrentMsg->lParam);
				//	DefWindowProc(m_pCurrentMsg->message, ' ', m_pCurrentMsg->lParam);
				//}

				////			  |
				////			  |
				////			  | insert here
				//// case : 12 1|F 34
				////
				//if (!insert && index >= 1 && !strText.IsEmpty() && strText.GetLength() >= index + 1 && strText[index - 1] != ' ' && strText[index] != ' ')
				//{
				//	insert = true;
				//	DefWindowProc(m_pCurrentMsg->message, toupper(nChar), m_pCurrentMsg->lParam);
				//	DefWindowProc(m_pCurrentMsg->message, ' ', m_pCurrentMsg->lParam);
				//	DefWindowProc(m_pCurrentMsg->message, '0', m_pCurrentMsg->lParam);					
				//}

				////			   |
				////			   |
				////			   | insert here
				//// case : 12 1F| 34
				////
				//if (!insert && index > 1 && !strText.IsEmpty() && strText.GetLength() > index + 1 && strText[index - 1] != ' ')
				//{
				//	insert = true;
				//	DefWindowProc(m_pCurrentMsg->message, ' ', m_pCurrentMsg->lParam);
				//	DefWindowProc(m_pCurrentMsg->message, toupper(nChar), m_pCurrentMsg->lParam);
				//	DefWindowProc(m_pCurrentMsg->message, '0', m_pCurrentMsg->lParam);
				//}

				////			 |
				////			 |
				////			 | insert here
				//// case : 12 |1F 34
				////
				//if (!insert && index > 0 && !strText.IsEmpty() && strText.GetLength() >= index + 1 && strText[index] != ' ')
				//{
				//	insert = true;
				//	DefWindowProc(m_pCurrentMsg->message, toupper(nChar), m_pCurrentMsg->lParam);
				//	DefWindowProc(m_pCurrentMsg->message, '0', m_pCurrentMsg->lParam);
				//	DefWindowProc(m_pCurrentMsg->message, ' ', m_pCurrentMsg->lParam);
				//}

				////			 |
				////			 |
				////			 | insert here
				//// case : 12 |
				////
				//if (!insert)
				//	DefWindowProc(m_pCurrentMsg->message, toupper(nChar), m_pCurrentMsg->lParam);
			}
			break;
		}
		return TRUE;
	}
};

#endif // !defined(CONTROLS_H)
