//
// startup_window.cpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2011 achellies (achellies at 163 dot com)
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif
#include <winspool.h>

#include "resource.h"
#include "win_impl_base.hpp"
#include "startup_window.hpp"
#include "message_box.hpp"
#include "install_window.hpp"
#include "uninstall_window.hpp"

#include "xdstring.h"
#include <algorithm>
#include <Shlobj.h>

const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kCancelButtonControlName = _T("cancel_btn");

const int kAnimateTimerId = 1001;

StartupWindow::StartupWindow()
{}

StartupWindow::~StartupWindow()
{}

LPCTSTR StartupWindow::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

void StartupWindow::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this;
}

tString StartupWindow::GetSkinFile()
{
	TCHAR szBuf[MAX_PATH] = {0};
	_stprintf_s(szBuf, MAX_PATH - 1, _T("%d"), IDR_XML_STARTUP);
	return szBuf;
}

tString StartupWindow::GetSkinFolder()
{
	return tString(CPaintManagerUI::GetInstancePath());
}

LONG StartupWindow::GetStyle()
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_MINIMIZEBOX);

	return styleValue;
}

void StartupWindow::OnExit(TNotifyUI& msg)
{
	Close();
}

void StartupWindow::Init()
{
	HICON hIcon = LoadIcon(paint_manager_.GetResourceDll(), MAKEINTRESOURCE(IDI_ICON1));
	(HICON)::SendMessage(m_hWnd, WM_SETICON, TRUE, (LPARAM)hIcon);
}

void StartupWindow::OnPrepare(TNotifyUI& msg)
{
	CControlUI* pControl = paint_manager_.FindControl(_T("bg"));
	if (pControl != NULL)
		paint_manager_.SetTimer(pControl, kAnimateTimerId, 2000);
}

void StartupWindow::OnTimer(TNotifyUI& msg)
{
	if (msg.wParam == kAnimateTimerId)
	{
		paint_manager_.KillTimer(msg.pSender);

		AnimateWindow(GetHWND(), 1500, AW_BLEND | AW_HIDE);
		::ShowWindow(GetHWND(), SW_SHOW);	

		CControlUI* pControl = paint_manager_.FindControl(_T("main"));
		if (pControl != NULL)
			pControl->SetVisible(true);

		pControl = paint_manager_.FindControl(_T("animate"));
		if (pControl != NULL)
			pControl->SetVisible(false);
	}
}

void StartupWindow::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, kWindowInit) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, kClick) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0 || _tcsicmp(msg.pSender->GetName(), kCancelButtonControlName) == 0)
		{
			PostQuitMessage(0);
			OnExit(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("install")) == 0)
		{
			OnExit(msg);
			InstallWindow* pFrame = new InstallWindow();
			if( pFrame == NULL )
			{
				OnExit(msg);
				return;
			}
#if defined(WIN32) && !defined(UNDER_CE)
			pFrame->Create(NULL, _T("Mirco Unidrv 驱动安装程序"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 800);
#else
			pFrame->Create(NULL, _T("Mirco Unidrv 驱动安装程序"), UI_WNDSTYLE_FRAME, WS_EX_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
#endif
			pFrame->CenterWindow();
			::ShowWindow(*pFrame, SW_SHOW);			
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("uninstall")) == 0)
		{
			OnExit(msg);
			UnInstallWindow* pFrame = new UnInstallWindow();
			if( pFrame == NULL )
			{
				OnExit(msg);
				return;
			}
#if defined(WIN32) && !defined(UNDER_CE)
			pFrame->Create(NULL, _T("驱动卸载程序"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 800);
#else
			pFrame->Create(NULL, _T("驱动卸载程序"), UI_WNDSTYLE_FRAME, WS_EX_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
#endif
			pFrame->CenterWindow();
			::ShowWindow(*pFrame, SW_SHOW);
		}
	}
	else if (_tcsicmp(msg.sType, kTimer) == 0)
	{
		return OnTimer(msg);
	}
}