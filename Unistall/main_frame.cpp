//
// main_frame.cpp
// ~~~~~~~~~~~~~~
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

#include "resource.h"
#include "win_impl_base.hpp"
#include "main_frame.hpp"
#include "message_box.hpp"

#include "xdstring.h"
#include <algorithm>
#include <Shlobj.h>

#include <Winspool.h>

const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");
const TCHAR* const kCancelButtonControlName = _T("cancel_btn");


MainFrame::MainFrame()
{}

MainFrame::~MainFrame()
{
	PostQuitMessage(0);
}

LPCTSTR MainFrame::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

void MainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

tString MainFrame::GetSkinFile()
{
	TCHAR szBuf[MAX_PATH] = {0};
	_stprintf_s(szBuf, MAX_PATH - 1, _T("%d"), IDR_SKINXML);
	return szBuf;
}

tString MainFrame::GetSkinFolder()
{
	return tString(CPaintManagerUI::GetInstancePath());
}

LONG MainFrame::GetStyle()
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_SIZEBOX);

	return styleValue;
}

LRESULT MainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
		}
	}
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

LRESULT MainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void MainFrame::OnTimer(TNotifyUI& msg)
{
}

void MainFrame::OnExit(TNotifyUI& msg)
{
	Close();
}

void MainFrame::Init()
{
	HICON hIcon = LoadIcon(paint_manager_.GetResourceDll(), MAKEINTRESOURCE(IDI_ICON1));
	(HICON)::SendMessage(m_hWnd, WM_SETICON, TRUE, (LPARAM)hIcon);
}

void MainFrame::OnPrepare(TNotifyUI& msg)
{
	EnumInstalledDriver();
}

void MainFrame::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, kWindowInit) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, kClick) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0 || _tcsicmp(msg.pSender->GetName(), kCancelButtonControlName) == 0)
		{
			OnExit(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMinButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMaxButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MAXIMIZE);
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
#else
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kRestoreButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_RESTORE);
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
#else
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("select_all_btn")) == 0)
		{
			CContainerUI* pDriverList = static_cast<CContainerUI*>(paint_manager_.FindControl(_T("driver_list")));
			for (int i = 0; (pDriverList != NULL) && (i < pDriverList->GetCount()); ++i)
			{
				COptionUI* pDriver = static_cast<COptionUI*>(pDriverList->GetItemAt(i)->GetInterface(_T("Option")));
				if (pDriver != NULL)
					pDriver->Selected(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("uninstall_btn")) == 0)
		{
			vector<tString> deleted_printer_driver;
			DWORD dwError = 0;
			TCHAR szBuf[MAX_PATH] = {0};

			CContainerUI* pDriverList = static_cast<CContainerUI*>(paint_manager_.FindControl(_T("driver_list")));
			for (int i = 0; (pDriverList != NULL) && (i < pDriverList->GetCount()); ++i)
			{
				COptionUI* pDriver = static_cast<COptionUI*>(pDriverList->GetItemAt(i)->GetInterface(_T("Option")));
				if ((pDriver != NULL) && pDriver->IsSelected())
				{
					// 涉及到的API： DeletePrinter DeletePrinterDriver
					HANDLE hPrinter = NULL;
					_stprintf_s(szBuf, MAX_PATH - 1, pDriver->GetText().GetData());
					PRINTER_DEFAULTS PrtDefault = {NULL, NULL, PRINTER_ALL_ACCESS};
					OpenPrinter(szBuf, &hPrinter, &PrtDefault);
					if (hPrinter != NULL)
					{
						DWORD dwNeeded = 0;
						GetPrinter(hPrinter, 2, NULL, 0, &dwNeeded);
						LPBYTE lpBuffer = (LPBYTE)malloc(dwNeeded);
						if (lpBuffer != NULL)
						{
							GetPrinter(hPrinter, 2, lpBuffer, dwNeeded, &dwNeeded);

							DeletePrinter(hPrinter);

							dwError = GetLastError();

							vector<tString>::const_iterator citer = find(deleted_printer_driver.begin(), deleted_printer_driver.end(), ((PRINTER_INFO_2*)lpBuffer)->pDriverName);
							if (citer == deleted_printer_driver.end())
							{
								deleted_printer_driver.push_back(((PRINTER_INFO_2*)lpBuffer)->pDriverName);
							}

							ClosePrinter(hPrinter);
							free(lpBuffer);
						}
					}
				}
			}
			for (vector<tString>::const_iterator citer = deleted_printer_driver.begin(); citer != deleted_printer_driver.end(); ++citer)
			{
				_stprintf_s(szBuf, MAX_PATH - 1, citer->c_str());
				DeletePrinterDriver(NULL, NULL, szBuf);

				dwError = GetLastError();
			}
			RestartSpoolsv();
			EnumInstalledDriver();

			if (IDYES == DUIMessageBox(m_hWnd, _T("注意：您需要重新启动计算机使设置生效，现在需要重启计算机吗？点击“是”立即重启计算机，点击“否”稍后您自己手动重启计算机。"), _T("重新启动计算机"), MB_YESNO))
				SystemReboot();
		}
	}
	else if (_tcsicmp(msg.sType, kTimer) == 0)
	{
		return OnTimer(msg);
	}
}

LRESULT MainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

void MainFrame::EnumInstalledDriver()
{
	// 清空界面中list中显示的打印机列表
	CContainerUI* pDriverList = static_cast<CContainerUI*>(paint_manager_.FindControl(_T("driver_list")));
	if (pDriverList != NULL)
		pDriverList->RemoveAll();

	// 枚举本地计算机上所有安装的打印机驱动（实际使用时可以只枚举自家的打印机驱动）
	LPBYTE	lpBuffer = NULL;
	DWORD	dwNeeded = 0,dwReturned = 0;

	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 1, NULL, 0, &dwNeeded, &dwReturned);
	if (dwNeeded == 0)
		return;

	lpBuffer = (LPBYTE)malloc(dwNeeded);
	if (lpBuffer == NULL)
		return;

	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 1, lpBuffer, dwNeeded, &dwNeeded, &dwReturned);
	for (DWORD dwIndex = 0; (dwIndex < dwReturned) && (pDriverList != NULL); dwIndex++)
	{
		COptionUI* pDriver = new COptionUI();
		if (pDriver != NULL)
		{
			pDriver->ApplyAttributeList(_T("padding=\"15,0,0,0\" height=\"35\" textpadding=\"20,0,0,0\" align=\"left\" normalimage=\"res='124' restype='png' dest='0,10,15,25'\" hotimage=\"res='125' restype='png' dest='0,10,15,25'\" selectedimage=\"res='126' restype='png' dest='0,10,15,25'\""));
			pDriver->SetText(((PRINTER_INFO_1*)lpBuffer + dwIndex)->pName);
			pDriverList->Add(pDriver);
		}
	}

	LPTSTR lpDefaultPrinter;
	GetDefaultPrinter(NULL, &dwNeeded);

	lpDefaultPrinter = (LPTSTR)malloc(dwNeeded * sizeof(TCHAR));
	GetDefaultPrinter((LPTSTR)lpDefaultPrinter, &dwNeeded);

	free(lpDefaultPrinter);
	free(lpBuffer);
}

//
// Purpose: 
//   Stops the service.
//
// Parameters:
//   None
// 
// Return value:
//   None
//
void MainFrame::DoStopSvc(LPCTSTR szSvcName)
{
	SERVICE_STATUS_PROCESS ssp;
	DWORD dwStartTime = GetTickCount();
	DWORD dwBytesNeeded;
	DWORD dwTimeout = 30000; // 30-second time-out
	DWORD dwWaitTime;

	// Get a handle to the SCM database. 

	SC_HANDLE schSCManager = OpenSCManager( 
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager) 
	{
		return;
	}

	// Get a handle to the service.

	SC_HANDLE schService = OpenService( 
		schSCManager,			// SCM database 
		szSvcName,				// name of service 
		SERVICE_STOP | 
		SERVICE_QUERY_STATUS | 
		SERVICE_ENUMERATE_DEPENDENTS);  

	if (schService == NULL)
	{ 
		//printf("OpenService failed (%d)\n", GetLastError()); 
		CloseServiceHandle(schSCManager);
		return;
	}    

	// Make sure the service is not already stopped.
	if ( !QueryServiceStatusEx( 
		schService, 
		SC_STATUS_PROCESS_INFO,
		(LPBYTE)&ssp, 
		sizeof(SERVICE_STATUS_PROCESS),
		&dwBytesNeeded ) )
	{
		//printf("QueryServiceStatusEx failed (%d)\n", GetLastError()); 
		goto stop_cleanup;
	}

	if ( ssp.dwCurrentState == SERVICE_STOPPED )
	{
		//printf("Service is already stopped.\n");
		goto stop_cleanup;
	}

	// If a stop is pending, wait for it.
	while ( ssp.dwCurrentState == SERVICE_STOP_PENDING ) 
	{
		//printf("Service stop pending...\n");

		// Do not wait longer than the wait hint. A good interval is 
		// one-tenth of the wait hint but not less than 1 second  
		// and not more than 10 seconds. 

		dwWaitTime = ssp.dwWaitHint / 10;

		if( dwWaitTime < 1000 )
			dwWaitTime = 1000;
		else if ( dwWaitTime > 10000 )
			dwWaitTime = 10000;

		Sleep( dwWaitTime );

		if ( !QueryServiceStatusEx( 
			schService, 
			SC_STATUS_PROCESS_INFO,
			(LPBYTE)&ssp, 
			sizeof(SERVICE_STATUS_PROCESS),
			&dwBytesNeeded ) )
		{
			//printf("QueryServiceStatusEx failed (%d)\n", GetLastError()); 
			goto stop_cleanup;
		}

		if ( ssp.dwCurrentState == SERVICE_STOPPED )
		{
			//printf("Service stopped successfully.\n");
			goto stop_cleanup;
		}

		if ( GetTickCount() - dwStartTime > dwTimeout )
		{
			//printf("Service stop timed out.\n");
			goto stop_cleanup;
		}
	}

	// If the service is running, dependencies must be stopped first.
	StopDependentServices(schSCManager, schService);

	// Send a stop code to the service.
	if (!ControlService(
		schService, 
		SERVICE_CONTROL_STOP, 
		(LPSERVICE_STATUS) &ssp))
	{
		//printf( "ControlService failed (%d)\n", GetLastError() );
		goto stop_cleanup;
	}

	// Wait for the service to stop.
	while ( ssp.dwCurrentState != SERVICE_STOPPED ) 
	{
		Sleep( ssp.dwWaitHint );
		if ( !QueryServiceStatusEx( 
			schService, 
			SC_STATUS_PROCESS_INFO,
			(LPBYTE)&ssp, 
			sizeof(SERVICE_STATUS_PROCESS),
			&dwBytesNeeded ) )
		{
			//printf( "QueryServiceStatusEx failed (%d)\n", GetLastError() );
			goto stop_cleanup;
		}

		if ( ssp.dwCurrentState == SERVICE_STOPPED )
			break;

		if ( GetTickCount() - dwStartTime > dwTimeout )
		{
			//printf( "Wait timed out\n" );
			goto stop_cleanup;
		}
	}
	//printf("Service stopped successfully\n");

stop_cleanup:
	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager);
}

BOOL MainFrame::StopDependentServices(SC_HANDLE schSCManager, SC_HANDLE schService)
{
	DWORD i;
	DWORD dwBytesNeeded;
	DWORD dwCount;

	LPENUM_SERVICE_STATUS   lpDependencies = NULL;
	ENUM_SERVICE_STATUS     ess;
	SC_HANDLE               hDepService;
	SERVICE_STATUS_PROCESS  ssp;

	DWORD dwStartTime = GetTickCount();
	DWORD dwTimeout = 30000; // 30-second time-out

	// Pass a zero-length buffer to get the required buffer size.
	if ( EnumDependentServices( schService, SERVICE_ACTIVE, 
		lpDependencies, 0, &dwBytesNeeded, &dwCount ) ) 
	{
		// If the Enum call succeeds, then there are no dependent
		// services, so do nothing.
		return TRUE;
	} 
	else 
	{
		if ( GetLastError() != ERROR_MORE_DATA )
			return FALSE; // Unexpected error

		// Allocate a buffer for the dependencies.
		lpDependencies = (LPENUM_SERVICE_STATUS) HeapAlloc( 
			GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded );

		if ( !lpDependencies )
			return FALSE;

		__try {
			// Enumerate the dependencies.
			if ( !EnumDependentServices( schService, SERVICE_ACTIVE, 
				lpDependencies, dwBytesNeeded, &dwBytesNeeded,
				&dwCount ) )
				return FALSE;

			for ( i = 0; i < dwCount; i++ ) 
			{
				ess = *(lpDependencies + i);
				// Open the service.
				hDepService = OpenService( schSCManager, 
					ess.lpServiceName, 
					SERVICE_STOP | SERVICE_QUERY_STATUS );

				if ( !hDepService )
					return FALSE;

				__try {
					// Send a stop code.
					if ( !ControlService( hDepService, 
						SERVICE_CONTROL_STOP,
						(LPSERVICE_STATUS) &ssp ) )
						return FALSE;

					// Wait for the service to stop.
					while ( ssp.dwCurrentState != SERVICE_STOPPED ) 
					{
						Sleep( ssp.dwWaitHint );
						if ( !QueryServiceStatusEx( 
							hDepService, 
							SC_STATUS_PROCESS_INFO,
							(LPBYTE)&ssp, 
							sizeof(SERVICE_STATUS_PROCESS),
							&dwBytesNeeded ) )
							return FALSE;

						if ( ssp.dwCurrentState == SERVICE_STOPPED )
							break;

						if ( GetTickCount() - dwStartTime > dwTimeout )
							return FALSE;
					}
				} 
				__finally 
				{
					// Always release the service handle.
					CloseServiceHandle( hDepService );
				}
			}
		} 
		__finally 
		{
			// Always free the enumeration buffer.
			HeapFree( GetProcessHeap(), 0, lpDependencies );
		}
	} 
	return TRUE;
}

//
// Purpose: 
//   Starts the service if possible.
//
// Parameters:
//   None
// 
// Return value:
//   None
//
void MainFrame::DoStartSvc(LPCTSTR szSvcName)
{
	SERVICE_STATUS_PROCESS ssStatus; 
	DWORD dwOldCheckPoint; 
	DWORD dwStartTickCount;
	DWORD dwWaitTime;
	DWORD dwBytesNeeded;

	// Get a handle to the SCM database. 
	SC_HANDLE schSCManager = OpenSCManager( 
		NULL,                    // local computer
		NULL,                    // servicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager) 
	{
		//printf("OpenSCManager failed (%d)\n", GetLastError());
		return;
	}

	// Get a handle to the service.
	SC_HANDLE schService = OpenService( 
		schSCManager,			// SCM database 
		szSvcName,				// name of service 
		SERVICE_ALL_ACCESS);	// full access 

	if (schService == NULL)
	{ 
		//printf("OpenService failed (%d)\n", GetLastError()); 
		CloseServiceHandle(schSCManager);
		return;
	}    

	// Check the status in case the service is not stopped.
	if (!QueryServiceStatusEx( 
		schService,                     // handle to service 
		SC_STATUS_PROCESS_INFO,         // information level
		(LPBYTE) &ssStatus,             // address of structure
		sizeof(SERVICE_STATUS_PROCESS), // size of structure
		&dwBytesNeeded ) )              // size needed if buffer is too small
	{
		//printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return; 
	}

	// Check if the service is already running. It would be possible 
	// to stop the service here, but for simplicity this example just returns. 
	if(ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
	{
		//printf("Cannot start the service because it is already running\n");
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return; 
	}

	// Save the tick count and initial checkpoint.
	dwStartTickCount = GetTickCount();
	dwOldCheckPoint = ssStatus.dwCheckPoint;

	// Wait for the service to stop before attempting to start it.
	while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
	{
		// Do not wait longer than the wait hint. A good interval is 
		// one-tenth of the wait hint but not less than 1 second  
		// and not more than 10 seconds. 

		dwWaitTime = ssStatus.dwWaitHint / 10;

		if( dwWaitTime < 1000 )
			dwWaitTime = 1000;
		else if ( dwWaitTime > 10000 )
			dwWaitTime = 10000;

		Sleep( dwWaitTime );

		// Check the status until the service is no longer stop pending.  
		if (!QueryServiceStatusEx( 
			schService,                     // handle to service 
			SC_STATUS_PROCESS_INFO,         // information level
			(LPBYTE) &ssStatus,             // address of structure
			sizeof(SERVICE_STATUS_PROCESS), // size of structure
			&dwBytesNeeded ) )              // size needed if buffer is too small
		{
			//printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
			CloseServiceHandle(schService); 
			CloseServiceHandle(schSCManager);
			return; 
		}

		if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
		{
			// Continue to wait and check.
			dwStartTickCount = GetTickCount();
			dwOldCheckPoint = ssStatus.dwCheckPoint;
		}
		else
		{
			if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
			{
				//printf("Timeout waiting for service to stop\n");
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager);
				return; 
			}
		}
	}

	// Attempt to start the service.

	if (!StartService(
		schService,  // handle to service 
		0,           // number of arguments 
		NULL) )      // no arguments 
	{
		//printf("StartService failed (%d)\n", GetLastError());
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return; 
	}
	//else
	//	printf("Service start pending...\n"); 

	// Check the status until the service is no longer start pending. 

	if (!QueryServiceStatusEx( 
		schService,                     // handle to service 
		SC_STATUS_PROCESS_INFO,         // info level
		(LPBYTE) &ssStatus,             // address of structure
		sizeof(SERVICE_STATUS_PROCESS), // size of structure
		&dwBytesNeeded ) )              // if buffer too small
	{
		//printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return; 
	}

	// Save the tick count and initial checkpoint.

	dwStartTickCount = GetTickCount();
	dwOldCheckPoint = ssStatus.dwCheckPoint;

	while (ssStatus.dwCurrentState == SERVICE_START_PENDING) 
	{ 
		// Do not wait longer than the wait hint. A good interval is 
		// one-tenth the wait hint, but no less than 1 second and no 
		// more than 10 seconds.  
		dwWaitTime = ssStatus.dwWaitHint / 10;

		if( dwWaitTime < 1000 )
			dwWaitTime = 1000;
		else if ( dwWaitTime > 10000 )
			dwWaitTime = 10000;

		Sleep( dwWaitTime );

		// Check the status again.  
		if (!QueryServiceStatusEx( 
			schService,             // handle to service 
			SC_STATUS_PROCESS_INFO, // info level
			(LPBYTE) &ssStatus,             // address of structure
			sizeof(SERVICE_STATUS_PROCESS), // size of structure
			&dwBytesNeeded ) )              // if buffer too small
		{
			//printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
			break; 
		}

		if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
		{
			// Continue to wait and check.

			dwStartTickCount = GetTickCount();
			dwOldCheckPoint = ssStatus.dwCheckPoint;
		}
		else
		{
			if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
			{
				// No progress made within the wait hint.
				break;
			}
		}
	} 

	// Determine whether the service is running.

	if (ssStatus.dwCurrentState == SERVICE_RUNNING) 
	{
		//printf("Service started successfully.\n"); 
	}
	//else 
	//{ 
	//	printf("Service not started. \n");
	//	printf("  Current State: %d\n", ssStatus.dwCurrentState); 
	//	printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode); 
	//	printf("  Check Point: %d\n", ssStatus.dwCheckPoint); 
	//	printf("  Wait Hint: %d\n", ssStatus.dwWaitHint); 
	//} 

	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager);
}

BOOL MainFrame::RestartSpoolsv()
{
	DoStopSvc(_T("Spooler"));
	DoStartSvc(_T("Spooler"));
	return TRUE;
}

BOOL MainFrame::SystemReboot()
{
	// 首先提升权限，然后重启电脑
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 

	// Get a token for this process. 
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		return FALSE; 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 


	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 

	if (GetLastError() != ERROR_SUCCESS) 
		return FALSE; 

	// Shut down the system and force all applications to close.
	if (!ExitWindowsEx(EWX_REBOOT, 0)) 
		return FALSE; 

	return TRUE;
}