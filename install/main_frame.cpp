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
#include <winspool.h>

#include "resource.h"
#include "win_impl_base.hpp"
#include "main_frame.hpp"
#include "message_box.hpp"

#include "xdstring.h"
#include <algorithm>
#include <Shlobj.h>

extern "C"
{
#include <lstate.h>
#include <lauxlib.h>
#include <lua.h>	
#include <lualib.h>
}

const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");
const TCHAR* const kCancelButtonControlName = _T("cancel_btn");
const TCHAR* const kPropertyControlName = _T("property");
const TCHAR* const kBackButtonControlName = _T("back_btn");
const TCHAR* const kNextButtonControlName = _T("next_btn");

const TCHAR* const kLicenseControlName = _T("license");
const TCHAR* const kLicenseFile = _T("License.txt");

const TCHAR* const kAgreeButtonControlName = _T("agree_btn");
const TCHAR* const kRefuseButtonControlName = _T("fefuse_btn");

const TCHAR* const kPortSelectionControlName = _T("ports");
const TCHAR* const kComSettingControlName = _T("com_setting");
const TCHAR* const kSetDefaultControlName = _T("set_as_default");


MainFrame::MainFrame()
: back_pages_(0)
, next_pages_(0)
, total_pages_(0)
, driver_install_started_(false)
, driver_installed_(false)
, set_as_default_(false)
, lua_state_(NULL)
, install_thread_(NULL)
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
	if (SC_CLOSE == wParam && install_thread_ != NULL)
	{
		ShowWindow();
		return 0;
	}

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
	CTabLayoutUI* pProperty = static_cast<CTabLayoutUI*>(paint_manager_.FindControl(kPropertyControlName));
	if (pProperty != NULL)
		next_pages_ = total_pages_ = pProperty->GetCount() - 1;

	// 填充用户协议
	CRichEditUI* pLicense = static_cast<CRichEditUI*>(paint_manager_.FindControl(kLicenseControlName));
	if (pLicense != NULL)
	{
		pLicense->SetReadOnly();

		CStdString szLicense = paint_manager_.GetInstancePath() + kLicenseFile;

		FILE* pFile = _tfopen(szLicense.GetData(), _T("rb"));
		if (pFile != NULL)
		{
			fseek(pFile, 0, SEEK_END);
			int nSize = ftell(pFile);
			fseek(pFile, 0, SEEK_SET);
			PBYTE pByte = new BYTE[nSize + 1];
			if (pByte != NULL)
			{
				fread(pByte, 1, nSize, pFile);
				pByte[nSize] = 0x00;

				DWORD dwUnicodeTag = MAKEWORD(pByte[0], pByte[1]);
				if (dwUnicodeTag == 0xFEFF)
				{
					CStringXD cstrText((wchar_t*)pByte + 1, nSize / 2 - 1);

					pLicense->SetText(cstrText.GetBuffer());
				}

				delete[] pByte;
			}
			fclose(pFile);
			pFile = NULL;
		}
		pLicense->SetFocus();
		pLicense->SetScrollPos(CSize(0,0));
	}

	// 判断当前系统
	CLabelUI* pOSLabel = static_cast<CLabelUI*>(paint_manager_.FindControl(_T("current_os")));
	if (pOSLabel != NULL)
	{
		Windows_Version winver;

		COptionUI* pWin2k = static_cast<COptionUI*>(paint_manager_.FindControl(_T("win2k")));
		COptionUI* pWinXp = static_cast<COptionUI*>(paint_manager_.FindControl(_T("winxp")));
		COptionUI* pWin2k3s = static_cast<COptionUI*>(paint_manager_.FindControl(_T("win2k3s")));
		COptionUI* pWinVista = static_cast<COptionUI*>(paint_manager_.FindControl(_T("winvista")));
		COptionUI* pWin2k8s = static_cast<COptionUI*>(paint_manager_.FindControl(_T("win2k8s")));
		COptionUI* pWin7 = static_cast<COptionUI*>(paint_manager_.FindControl(_T("win7")));

		if (winver.GetVersionNT(win_ver_, osname_))
		{
			switch (win_ver_)
			{
			case Windows2000:
				pWin2k->Selected(true);
				driver_source_path_ = _T("WinNT\\");
				break;
			case WindowsXP:
				pWinXp->Selected(true);
				driver_source_path_ = _T("WinNT\\");
				break;
			case WindowsServer2003:
				pWin2k3s->Selected(true);
				driver_source_path_ = _T("Win2003\\");
				break;
			case WindowsVista:
				pWinVista->Selected(true);
				driver_source_path_ = _T("Vista\\");
				break;
			case WindowsServer2008:
				pWin2k8s->Selected(true);
				driver_source_path_ = _T("Win2008\\");
				break;
			case Windows7:
				pWin7->Selected(true);
				driver_source_path_ = _T("Win7\\");
				break;
			default:
				break;
			}

			pWin2k->SetEnabled(false);
			pWinXp->SetEnabled(false);
			pWin2k3s->SetEnabled(false);
			pWinVista->SetEnabled(false);
			pWin2k8s->SetEnabled(false);
			pWin7->SetEnabled(false);

			tString szText = _T("当前系统为：");
			szText += osname_;
			pOSLabel->SetText(szText.c_str());
		}
		else
		{
			pWin2k->SetEnabled(false);
			pWinXp->SetEnabled(false);
			pWin2k3s->SetEnabled(false);
			pWinVista->SetEnabled(false);
			pWin2k8s->SetEnabled(false);
			pWin7->SetEnabled(false);

			winver.GetVersion9x(win_ver_, osname_);

			tString szText = _T("{c #FF0000}本驱动包不支持：");
			szText += osname_;
			szText += _T("系统，请与设备提供商联系！{/c}");
			pOSLabel->SetShowHtml();
			pOSLabel->SetText(szText.c_str());
		}
	}

	UpdateNavigateButtonsStatus();
}

void MainFrame::UpdateNavigateButtonsStatus()
{
	CButtonUI* pBackBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(kBackButtonControlName));
	CButtonUI* pNextBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(kNextButtonControlName));

	if (pBackBtn != NULL && pNextBtn != NULL)
	{
		pBackBtn->SetEnabled(back_pages_ != 0);
		pNextBtn->SetEnabled(next_pages_ != 0);

		if (back_pages_ == 0)
		{
			COptionUI* pAgreeBtn = static_cast<COptionUI*>(paint_manager_.FindControl(kAgreeButtonControlName));
			if ((pAgreeBtn != NULL) && !pAgreeBtn->IsSelected())
				pNextBtn->SetEnabled(false);
		}
	}
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
		else if (_tcsicmp(msg.pSender->GetName(), kBackButtonControlName) == 0)
		{
			--back_pages_;
			++next_pages_;
			UpdateNavigateButtonsStatus();

			CTabLayoutUI* pProperty = static_cast<CTabLayoutUI*>(paint_manager_.FindControl(kPropertyControlName));
			if (pProperty != NULL)
			{
				pProperty->SelectItem(total_pages_ - next_pages_);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kNextButtonControlName) == 0)
		{
			if (total_pages_ - next_pages_ == 1)
			{
				COptionUI* pWin2k = static_cast<COptionUI*>(paint_manager_.FindControl(_T("win2k")));
				COptionUI* pWinXp = static_cast<COptionUI*>(paint_manager_.FindControl(_T("winxp")));
				COptionUI* pWin2k3s = static_cast<COptionUI*>(paint_manager_.FindControl(_T("win2k3s")));
				COptionUI* pWinVista = static_cast<COptionUI*>(paint_manager_.FindControl(_T("winvista")));
				COptionUI* pWin2k8s = static_cast<COptionUI*>(paint_manager_.FindControl(_T("win2k8s")));
				COptionUI* pWin7 = static_cast<COptionUI*>(paint_manager_.FindControl(_T("win7")));

				bool bSelectedCorrectOs = false;
				switch (win_ver_)
				{
				case Windows2000:
					bSelectedCorrectOs = pWin2k->IsSelected();
					break;
				case WindowsXP:
					bSelectedCorrectOs = pWinXp->IsSelected();
					break;
				case WindowsServer2003:
					bSelectedCorrectOs = pWin2k3s->IsSelected();
					break;
				case WindowsVista:
					bSelectedCorrectOs = pWinVista->IsSelected();
					break;
				case WindowsServer2008:
					bSelectedCorrectOs = pWin2k8s->IsSelected();
					break;
				case Windows7:
					bSelectedCorrectOs = pWin7->IsSelected();
					break;
				default:
					break;
				}
				if (!bSelectedCorrectOs)
				{
					DUIMessageBox(m_hWnd, _T("请选择正确的操作系统！"), _T("出错啦"));
					return;
				}
			}
			else if (next_pages_ == 0 && driver_install_started_)
				return Close();

			++back_pages_;
			--next_pages_;
			UpdateNavigateButtonsStatus();

			if (next_pages_ == 0 && !driver_install_started_)
			{
				COptionUI* pSetAsDefault = static_cast<COptionUI*>(paint_manager_.FindControl(_T("set_as_default")));
				if (pSetAsDefault != NULL)
					set_as_default_ = pSetAsDefault->IsSelected();

				CComboUI* pPortList = static_cast<CComboUI*>(paint_manager_.FindControl(_T("ports")));
				if (pPortList != NULL)
					port_name_ = pPortList->GetText();

				CButtonUI* pBackBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(kBackButtonControlName));
				CButtonUI* pNextBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(kNextButtonControlName));
				CButtonUI* pCloseBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(kCloseButtonControlName));
				CButtonUI* pCancelBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(kCancelButtonControlName));
				if ((pBackBtn != NULL) && (pNextBtn != NULL) && (pCloseBtn != NULL) && (pCancelBtn != NULL))
				{
					pBackBtn->SetEnabled(false);
					pNextBtn->SetEnabled(false);
					pCloseBtn->SetEnabled(false);
					pCancelBtn->SetEnabled(false);

					driver_install_started_ = true;

					DWORD dwThreadId = 0;
					install_thread_ = CreateThread(NULL, 0, InstallPrinterDriver, (LPVOID)this, 0, &dwThreadId);
				}
			}

			CTabLayoutUI* pProperty = static_cast<CTabLayoutUI*>(paint_manager_.FindControl(kPropertyControlName));
			if (pProperty != NULL)
			{
				pProperty->SelectItem(total_pages_ - next_pages_);
			}
		}
	}
	else if (_tcsicmp(msg.sType, kSelectChanged) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kAgreeButtonControlName) == 0)
		{
			UpdateNavigateButtonsStatus();
		}
		else if (_tcsicmp(msg.pSender->GetName(), kRefuseButtonControlName) == 0)
		{
			UpdateNavigateButtonsStatus();
		}
	}
	else if (_tcsicmp(msg.sType, kItemSelect) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kPortSelectionControlName) == 0)
		{
			CComboUI* pPorts = static_cast<CComboUI*>(msg.pSender);
			CStdString szText = pPorts->GetText();

			CControlUI* pComSetting = paint_manager_.FindControl(kComSettingControlName);
			if (pComSetting != NULL)
				pComSetting->SetVisible((_tcsstr(szText, _T("COM")) != 0));
		}
	}
	else if (_tcsicmp(msg.sType, kTimer) == 0)
	{
		return OnTimer(msg);
	}
}

void MainFrame::PrinterInstalledFinish()
{
	CButtonUI* pBackBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(kBackButtonControlName));
	CButtonUI* pNextBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(kNextButtonControlName));
	CButtonUI* pCloseBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(kCloseButtonControlName));
	CButtonUI* pCancelBtn = static_cast<CButtonUI*>(paint_manager_.FindControl(kCancelButtonControlName));
	if ((pBackBtn != NULL) && (pNextBtn != NULL) && (pCloseBtn != NULL) && (pCancelBtn != NULL))
	{
		pBackBtn->SetEnabled(false);
		pNextBtn->SetEnabled(true);
		pCloseBtn->SetEnabled(true);
		pCancelBtn->SetEnabled(false);

		if (!driver_installed_)
		{
			// 驱动安装失败，提示错误
			AddProgressInfo(error_info_, 0, true);
		}
		else
			AddProgressInfo(_T("添加打印机成功"), 100);

		pNextBtn->SetText(_T("完成"));
	}
}

void MainFrame::AddProgressInfo(const tString& info, int progress, bool red)
{
	OS_CAutoLock lock(os_locker_);

	//progress_info_.push_back(info);

	CListUI* pList = static_cast<CListUI*>(paint_manager_.FindControl(_T("progress_list")));
	if (pList != NULL)
	{
		CListLabelElementUI* pLabelElement = new CListLabelElementUI();
		if (red)
		{
			pList->SetItemShowHtml(true);
			TCHAR szBuf[MAX_PATH] = {0};
			_stprintf_s(szBuf, MAX_PATH, _T("{c #FF0000}%s{/c}"), info.c_str());
			pLabelElement->SetText(szBuf);
		}
		else
			pLabelElement->SetText(info.c_str());
		pList->Add(pLabelElement);
	}

	CProgressUI* pProgress = static_cast<CProgressUI*>(paint_manager_.FindControl(_T("progress")));
	if (pProgress != NULL)
		pProgress->SetValue(progress);
}

tString MainFrame::GetDriverConfig(const TCHAR* name)
{
	CStringXD cstrName(name);
	CStringXDA cstrAnsiName(cstrName);

	int ret = 0 ;
	if (lua_state_ == NULL)
	{
		// 创建lua运行环境
		lua_state_ = lua_open();
		// 加载Lua基本库
		luaopen_base(lua_state_);
		// 加载Lua通用扩展库
		luaL_openlibs(lua_state_);

		CStringXD szConfigFile = paint_manager_.GetInstancePath() + _T("config.lua");
		CStringXDA szAnsiConfigFile(szConfigFile);

		// 加载lua脚本文件
		//ret = luaL_loadfile(lua_state_, szAnsiConfigFile.GetBuffer());
		//ret = lua_pcall(lua_state_,0,0,0);
		ret = luaL_dofile(lua_state_, szAnsiConfigFile.GetBuffer());
	}

	// 函数名
	lua_getglobal(lua_state_, "getDriverConfig");
	lua_pushlstring(lua_state_, cstrAnsiName.GetBuffer(), cstrAnsiName.GetLength());

	// 运行函数并把结果压入栈
	lua_pcall(lua_state_, 1, 1, 0);
	const char * c = (const char *)lua_tostring(lua_state_,-1);
	char szName[MAX_PATH] = {0};
	sprintf_s(szName, MAX_PATH - 1, c);

	lua_pop(lua_state_, 1);

	// 释放lua运行环境
	//lua_close(lua_state_);

	return CStringXD(szName).GetBuffer();
}

DWORD MainFrame::InstallPrinterDriver(LPVOID pVoid)
{
	MainFrame* pThis = (MainFrame*)pVoid;

	// 是否需要提升权限呢，这个需要具体系统进行测试一下哈
	//HANDLE hToken; 
	//TOKEN_PRIVILEGES tkp; 

	//// Get a token for this process. 
	//if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
	//	return -1;

	//LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 

	//tkp.PrivilegeCount = 1;  // one privilege to set    
	//tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	//AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
	//if (GetLastError() != ERROR_SUCCESS) 
	//	return -1;

	DWORD uSize = 0;
	TCHAR szBuf[MAX_PATH] = {0};
	GetPrinterDriverDirectory(NULL, _T("Windows NT x86"), 1, (LPBYTE)szBuf, MAX_PATH, &uSize);
	tString driver_dir_x86 = szBuf;
	driver_dir_x86 += _T("\\");

	tString files_copied_path = pThis->paint_manager_.GetInstancePath();
	files_copied_path += pThis->driver_source_path_;

	DRIVER_INFO_3 drvinfo3;
	FillMemory(&drvinfo3, sizeof(DRIVER_INFO_3), 0);
	drvinfo3.cVersion = (DWORD)3;
// config.lua
	//entity={
	//["DriverName"]="Micro Unidrv",
	//["PrinterName"]="DJ-1580",
	//["Manufacturer"]="DJ",
	//["OEM URL"]="http://www.seuic.com/index.asp",
	//["DriverPath"]="UNIDRV.DLL",
	//["ConfigFile"]="UNIDRVUI.DLL",
	//["HelpFile"]="UNIDRV.HLP",
	//["DataFile"]="Microdrv.GPD",
	//["DataFileDLL"]="Microdrv.dll",
	//["OEMConfigFile"]="MicroDrvUI.dll",
	//["OEMDriverFile"]="MicroDrvUni.dll",
	//["OEMPlugFile"] = "UnidrvPlug.ini"
	//}

	//function getDriverConfig(name)
	//	return entity[name]
	//end
//
	TCHAR szDriverName[MAX_PATH] = {0};
	_stprintf_s(szDriverName, MAX_PATH - 1, pThis->GetDriverConfig(_T("DriverName")).c_str());
	drvinfo3.pName = szDriverName;

	//HKEY hKey = NULL;
	//HKEY hSubkey = NULL;
	////Insert x86 additional drivers to registry
	//RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Print\\Environments\\Windows NT x86\\Drivers\\Version-3\\"), 0, KEY_ALL_ACCESS, &hKey);
	//RegCreateKey(hKey, pThis->GetDriverConfig(_T("DriverName")).c_str(), &hSubkey);
	//if (hSubkey != NULL)
	//{
	//	RegSetValueEx(hSubkey, _T("Dependent Files"), 0, REG_MULTI_SZ, (BYTE*)_T(""), lstrlen(_T(""))+1);
	//	RegSetValueEx(hSubkey, _T("Previous Names"), 0, REG_MULTI_SZ, (BYTE*)_T(""), lstrlen(_T(""))+1);
	//	RegSetValueEx(hSubkey, _T("ColorProfiles"), 0, REG_MULTI_SZ, (BYTE*)_T(""), lstrlen(_T(""))+1);
	//	RegSetValueEx(hSubkey, _T("CoreDependencies"), 0, REG_MULTI_SZ, (BYTE*)_T(""), lstrlen(_T(""))+1);
	//	DWORD Val = 3;
	//	RegSetValueEx(hSubkey, _T("Version"), 0, REG_DWORD, (BYTE*)&Val, sizeof(DWORD));
	//	Val = 0;
	//	RegSetValueEx(hSubkey, _T("TempDir"), 0, REG_DWORD, (BYTE*)&Val, sizeof(DWORD));
	//	Val = 2;
	//	RegSetValueEx(hSubkey, _T("Attributes"), 0, REG_DWORD, (BYTE*)&Val, sizeof(DWORD));
	//	Val = 0;
	//	RegSetValueEx(hSubkey, _T("PrinterDriverAttributes"), 0, REG_DWORD, (BYTE*)&Val, sizeof(DWORD));
	//	RegSetValueEx(hSubkey, _T("Configuration File"), 0, REG_SZ, (BYTE*)pThis->GetDriverConfig(_T("ConfigFile")).c_str(), (lstrlen(pThis->GetDriverConfig(_T("ConfigFile")).c_str()) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("Data File"), 0, REG_SZ, (BYTE*)pThis->GetDriverConfig(_T("DataFile")).c_str(), (lstrlen(pThis->GetDriverConfig(_T("DataFile")).c_str()) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("Driver"), 0, REG_SZ, (BYTE*)pThis->GetDriverConfig(_T("DriverPath")).c_str(), (lstrlen(pThis->GetDriverConfig(_T("DriverPath")).c_str()) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("Help File"), 0, REG_SZ, (BYTE*)pThis->GetDriverConfig(_T("HelpFile")).c_str(), (lstrlen(pThis->GetDriverConfig(_T("HelpFile")).c_str()) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("Monitor"), 0, REG_SZ, (BYTE*)_T(""), (lstrlen(_T("")) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("Datatype"), 0, REG_SZ, (BYTE*)_T("RAW"), (lstrlen(_T("RAW")) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("Manufacturer"), 0, REG_SZ, (BYTE*)pThis->GetDriverConfig(_T("Manufacturer")).c_str(), (lstrlen(pThis->GetDriverConfig(_T("Manufacturer")).c_str()) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("OEM URL"), 0, REG_SZ, (BYTE*)pThis->GetDriverConfig(_T("OEM URL")).c_str(), (lstrlen(pThis->GetDriverConfig(_T("OEM URL")).c_str()) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("HardwareID"), 0, REG_SZ, (BYTE*)_T(""), (lstrlen(_T("")) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("Provider"), 0, REG_SZ, (BYTE*)_T(""), (lstrlen(_T("")) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("Print Processor"), 0, REG_SZ, (BYTE*)_T(""), (lstrlen(_T("")) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("VendorSetup"), 0, REG_SZ, (BYTE*)_T(""), (lstrlen(_T("")) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("InfPath"), 0, REG_SZ, (BYTE*)_T(""), (lstrlen(_T("")) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("DriverDate"), 0, REG_SZ, (BYTE*)_T("01/01/2010"), (lstrlen(_T("01/01/2010")) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("DriverVersion"), 0, REG_SZ, (BYTE*)_T("1.0.0.0"), (lstrlen(_T("1.0.0.0")) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("MinInboxDriverVerDate"), 0, REG_SZ, (BYTE*)_T("01/01/2010"), (lstrlen(_T("01/01/2010")) + 1) * sizeof(TCHAR));
	//	RegSetValueEx(hSubkey, _T("MinInboxDriverVerVersion"), 0, REG_SZ, (BYTE*)_T("1.0.0.0"), (lstrlen(_T("1.0.0.0")) + 1) * sizeof(TCHAR));
	//	RegCloseKey(hSubkey);
	//}
	//RegCloseKey(hKey);

	DWORD dwError = GetLastError();

	pThis->AddProgressInfo(_T("正在准备安装程序..."), 5);

	tString temp;
	tString temp2;

	TCHAR szDriverPath[MAX_PATH] = {0};
	_stprintf_s(szDriverPath, MAX_PATH - 1, _T("%s%s"), driver_dir_x86.c_str(), pThis->GetDriverConfig(_T("DriverPath")).c_str());
	drvinfo3.pDriverPath = szDriverPath;
	temp = files_copied_path + pThis->GetDriverConfig(_T("DriverPath"));
	temp2 = driver_dir_x86 + pThis->GetDriverConfig(_T("DriverPath"));
	CopyFile(temp.c_str(), temp2.c_str() , TRUE);

	TCHAR szConfigFile[MAX_PATH] = {0};
	_stprintf_s(szConfigFile, MAX_PATH - 1, _T("%s%s"), driver_dir_x86.c_str(), pThis->GetDriverConfig(_T("ConfigFile")).c_str());
	drvinfo3.pConfigFile = szConfigFile;
	temp = files_copied_path + pThis->GetDriverConfig(_T("ConfigFile"));
	temp2 = driver_dir_x86 + pThis->GetDriverConfig(_T("ConfigFile"));
	CopyFile(temp.c_str(), temp2.c_str() , TRUE);

	TCHAR szHelpFile[MAX_PATH] = {0};
	_stprintf_s(szHelpFile, MAX_PATH - 1, _T("%s%s"), driver_dir_x86.c_str(), pThis->GetDriverConfig(_T("HelpFile")).c_str());
	drvinfo3.pHelpFile = szHelpFile;
	temp = files_copied_path + pThis->GetDriverConfig(_T("HelpFile"));
	temp2 = driver_dir_x86 + pThis->GetDriverConfig(_T("HelpFile"));
	CopyFile(temp.c_str(), temp2.c_str() , TRUE);

	temp = files_copied_path + _T("STDNAMES.GPD");
	temp2 = driver_dir_x86 + _T("STDNAMES.GPD");
	CopyFile(temp.c_str(), temp2.c_str() , TRUE);

	temp = files_copied_path + _T("TTFSUB.GPD");
	temp2 = driver_dir_x86 + _T("TTFSUB.GPD");
	CopyFile(temp.c_str(), temp2.c_str() , TRUE);

	temp = files_copied_path + _T("UNIRES.DLL");
	temp2 = driver_dir_x86 + _T("UNIRES.DLL");
	CopyFile(temp.c_str(), temp2.c_str() , TRUE);

	pThis->AddProgressInfo(_T("正在解析安装程序包..."), 10);

	TCHAR szDataFile[MAX_PATH] = {0};
	_stprintf_s(szDataFile, MAX_PATH - 1, pThis->GetDriverConfig(_T("DataFile")).c_str());
	temp = files_copied_path + szDataFile;
	temp2 = driver_dir_x86 + _T("3\\") + szDataFile;
	DeleteFile(temp2.c_str());
	temp2 = driver_dir_x86 + szDataFile;
	DeleteFile(temp2.c_str());
	CopyFile(temp.c_str(), temp2.c_str() , FALSE);
	_stprintf_s(szDataFile, MAX_PATH - 1, _T("%s%s"), driver_dir_x86.c_str(), pThis->GetDriverConfig(_T("DataFile")).c_str());
	drvinfo3.pDataFile = szDataFile;

	TCHAR szDataFileDLL[MAX_PATH] = {0};
	_stprintf_s(szDataFileDLL, MAX_PATH - 1, pThis->GetDriverConfig(_T("DataFileDLL")).c_str());
	temp = files_copied_path + szDataFileDLL;
	temp2 = driver_dir_x86 + _T("3\\") + szDataFileDLL;
	DeleteFile(temp2.c_str());
	temp2 = driver_dir_x86 + szDataFileDLL;
	DeleteFile(temp2.c_str());
	CopyFile(temp.c_str(), temp2.c_str(), FALSE);

	pThis->AddProgressInfo(_T("拷贝安装驱动包..."), 20);

	TCHAR szOEMPlugFile[MAX_PATH] = {0};
	_stprintf_s(szOEMPlugFile, MAX_PATH - 1, pThis->GetDriverConfig(_T("OEMPlugFile")).c_str());
	temp = files_copied_path + szOEMPlugFile;
	temp2 = driver_dir_x86 + _T("3\\") + szOEMPlugFile;
	DeleteFile(temp2.c_str());
	temp2 = driver_dir_x86 + szOEMPlugFile;
	DeleteFile(temp2.c_str());
	CopyFile(temp.c_str(), temp2.c_str(), FALSE);

	TCHAR szOEMConfigFile[MAX_PATH] = {0};
	_stprintf_s(szOEMConfigFile, MAX_PATH - 1, pThis->GetDriverConfig(_T("OEMConfigFile")).c_str());
	temp = files_copied_path + szOEMConfigFile;
	temp2 = driver_dir_x86 + _T("3\\") + szOEMConfigFile;
	DeleteFile(temp2.c_str());
	temp2 = driver_dir_x86 + szOEMConfigFile;
	DeleteFile(temp2.c_str());
	CopyFile(temp.c_str(), temp2.c_str(), FALSE);

	TCHAR szOEMDriverFile[MAX_PATH] = {0};
	_stprintf_s(szOEMDriverFile, MAX_PATH - 1, pThis->GetDriverConfig(_T("OEMDriverFile")).c_str());
	temp = files_copied_path + szOEMDriverFile;
	temp2 = driver_dir_x86 + _T("3\\") + szOEMDriverFile;
	DeleteFile(temp2.c_str());
	temp2 = driver_dir_x86 + szOEMDriverFile;
	DeleteFile(temp2.c_str());
	CopyFile(temp.c_str(), temp2.c_str(), FALSE);

	TCHAR* pBuf = new TCHAR[7 * MAX_PATH];
	if (pBuf != NULL)
	{
		FillMemory(pBuf, (MAX_PATH * 7) * sizeof(TCHAR), 0);
		TCHAR* pBased = pBuf;

		_stprintf_s(pBased, MAX_PATH - 1, _T("STDNAMES.GPD"));
		pBased[_tcslen(pBased)] = _T('\0');

		pBased += _tcslen(pBased) + 1;
		_stprintf_s(pBased, MAX_PATH - 1, _T("TTFSUB.GPD"));
		pBased[_tcslen(pBased)] = _T('\0');

		pBased += _tcslen(pBased) + 1;
		_stprintf_s(pBased, MAX_PATH - 1, _T("UNIRES.DLL"));
		pBased[_tcslen(pBased)] = _T('\0');

		pBased += _tcslen(pBased) + 1;
		_stprintf_s(pBased, MAX_PATH - 1, _T("%s"), szDataFileDLL);
		pBased[_tcslen(pBased)] = _T('\0');

		pBased += _tcslen(pBased) + 1;
		_stprintf_s(pBased, MAX_PATH - 1, _T("%s"), szOEMPlugFile);
		pBased[_tcslen(pBased)] = _T('\0');

		pBased += _tcslen(pBased) + 1;
		_stprintf_s(pBased, MAX_PATH - 1, _T("%s"), szOEMConfigFile);
		pBased[_tcslen(pBased)] = _T('\0');

		pBased += _tcslen(pBased) + 1;
		_stprintf_s(pBased, MAX_PATH - 1, _T("%s"), szOEMDriverFile);
		pBased[_tcslen(pBased)] = _T('\0');
		pBased[_tcslen(pBased) + 1] = _T('\0');

		//pBased += _tcslen(pBased) + 1;
		drvinfo3.pDependentFiles = pBuf;
		//drvinfo3.pDependentFiles = _T("Microdrv.DLL\0MicroDrvUI.DLL\0UnidrvPlug.ini\0MicroDrvUni.DLL\0STDNAMES.GPD\0TTFSUB.GPD\0UNIRES.DLL\0\0");// some files omitted
		drvinfo3.pMonitorName = NULL;
		drvinfo3.pDefaultDataType = _T("RAW");

		pThis->AddProgressInfo(_T("添加打印机驱动..."), 60);
		DWORD dwError = GetLastError();

		// 检查一下当前系统是否已经安装了该驱动, 如果先前没有安装过则添加，有则直接添加打印机
		if (IsPrinterDriverInstalled(szDriverName) || AddPrinterDriver(NULL, 3, (LPBYTE)&drvinfo3))
		{
			PRINTER_INFO_2 prtinfo2 = {0};

			tString print_name = pThis->GetDriverConfig(_T("PrinterName"));
			print_name += (_tcsstr(pThis->port_name_.c_str(), _T("COM")) != NULL) ? _T("(S)") : _T("(P)");

			if (_tcsstr(pThis->port_name_.c_str(), _T("COM")) != NULL)
				pThis->ConfigSerialPort();

			TCHAR szPortName[MAX_PATH] = {0};
			_stprintf_s(szPortName, MAX_PATH - 1, _T("%s:"), pThis->port_name_.c_str());

			prtinfo2.pServerName = NULL;			
			prtinfo2.pShareName = NULL;
			prtinfo2.pPortName = szPortName;
			prtinfo2.pDriverName = szDriverName;
			prtinfo2.pDatatype = _T("RAW");
			prtinfo2.pPrintProcessor = _T("WinPrint");
			prtinfo2.Attributes = PRINTER_ATTRIBUTE_LOCAL;

			// 检查一下本地该打印机名称是否已经存在
			if (!IsPrinterAdded(print_name, print_name))
			{
				TCHAR szPrinterName[MAX_PATH] = {0};
				_stprintf_s(szPrinterName, MAX_PATH - 1, print_name.c_str());
				prtinfo2.pPrinterName = szPrinterName;

				pThis->AddProgressInfo(_T("添加打印机..."), 90);
				if (AddPrinter(NULL, 2, (LPBYTE)&prtinfo2))
				{
					pThis->driver_installed_ = true;

					if (pThis->set_as_default_)
						SetDefaultPrinter(szPrinterName);
				}
				else
				{
					dwError = GetLastError();
					TCHAR szBuf[MAX_PATH] = {0};
					_stprintf_s(szBuf, MAX_PATH, _T("添加打印机失败！错误码：%d"), dwError);
					pThis->error_info_ = szBuf;
				}
			}
			else
				pThis->error_info_ = print_name, pThis->error_info_ += _T(" 打印机已经存在!");
		}
		else
		{
			dwError = GetLastError();
			TCHAR szBuf[MAX_PATH] = {0};
			_stprintf_s(szBuf, MAX_PATH, _T("添加打印机驱动失败！错误码：%d"), dwError);
			pThis->error_info_ = szBuf; 
		}

		delete[] pBuf;
	}
	else
		pThis->error_info_ = _T("申请内存失败，内存不足!");

	pThis->PrinterInstalledFinish();

	if (pThis->lua_state_ != NULL)
		lua_close(pThis->lua_state_);
	pThis->lua_state_ = NULL;
	CloseHandle(pThis->install_thread_);
	pThis->install_thread_ = NULL;

	ExitThread(0);

	return 0;
}

bool MainFrame::IsPrinterDriverInstalled(const tString& driver_name)
{
	if (driver_name.empty()) return false;

	LPBYTE	lpBuffer = NULL;
	DWORD	dwNeeded = 0, dwReturned = 0;

	EnumPrinterDrivers(NULL, NULL, 1, NULL, 0, &dwNeeded, &dwReturned);
	if (dwNeeded == 0)
		return false;

	lpBuffer = (LPBYTE)malloc(dwNeeded);
	if (lpBuffer == NULL)
		return false;

	EnumPrinterDrivers(NULL, NULL, 1, lpBuffer, dwNeeded, &dwNeeded, &dwReturned);
	for (DWORD dwIndex = 0; dwIndex < dwReturned; dwIndex++)
		if (_tcsicmp(((DRIVER_INFO_1*)lpBuffer + dwIndex)->pName, driver_name.c_str()) == 0)
			return free(lpBuffer), true;

	free(lpBuffer);

	return false;
}

bool MainFrame::IsPrinterAdded(tString printer_name, tString& valid_name)
{
	bool bPrinterAdded = false;
	valid_name = printer_name;

	if (printer_name.empty()) return bPrinterAdded;
	
	LPBYTE	lpBuffer = NULL;
	DWORD	dwNeeded = 0, dwReturned = 0;
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 1, NULL, 0, &dwNeeded, &dwReturned);
	if (dwNeeded == 0)
		return bPrinterAdded;

	lpBuffer = (LPBYTE)malloc(dwNeeded);
	if (lpBuffer == NULL)
		return bPrinterAdded;

	int nSufixCount = 1;
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 1, lpBuffer, dwNeeded, &dwNeeded, &dwReturned);
	for (DWORD dwIndex = 0; dwIndex < dwReturned; ++dwIndex)
	{
		if (_tcsicmp(((PRINTER_INFO_1*)lpBuffer + dwIndex)->pName, printer_name.c_str()) == 0)
		{
			if (nSufixCount > 4)
			{
				bPrinterAdded = true;
				break;
			}
			++nSufixCount;

			valid_name += _T("(1)");
			printer_name = valid_name;
			dwIndex = -1;
			bPrinterAdded = false;
		}
	}

	free(lpBuffer);

	return bPrinterAdded;
}

void MainFrame::ConfigSerialPort()
{
	enum FlowControl
	{
		NoFlowControl,
		CtsRtsFlowControl,
		CtsDtrFlowControl,
		DsrRtsFlowControl,
		DsrDtrFlowControl,
		XonXoffFlowControl
	};

	enum Parity
	{    
		NoParity = 0,
		OddParity = 1,
		EvenParity = 2,
		MarkParity = 3,
		SpaceParity = 4
	};

	enum StopBits
	{
		OneStopBit,
		OnePointFiveStopBits,
		TwoStopBits
	};

	DWORD dwLastError = GetLastError();
	COMMCONFIG config = {0};
	DWORD dwSize = sizeof(COMMCONFIG);
	config.dcb.DCBlength = sizeof(DCB);

	if (GetDefaultCommConfig(port_name_.c_str(), &config, &dwSize))
	{
		CComboUI* pBaudRate = static_cast<CComboUI*>(paint_manager_.FindControl(_T("baud_rate")));
		CComboUI* pByteSize = static_cast<CComboUI*>(paint_manager_.FindControl(_T("byte_size")));
		CComboUI* pParity = static_cast<CComboUI*>(paint_manager_.FindControl(_T("parity")));
		CComboUI* pStopBits = static_cast<CComboUI*>(paint_manager_.FindControl(_T("stop_bits")));
		CComboUI* pProtocal = static_cast<CComboUI*>(paint_manager_.FindControl(_T("protocal")));
		if (pBaudRate != NULL && pByteSize != NULL && pParity != NULL && pStopBits != NULL && pProtocal != NULL)
		{
			DWORD dwBaudRate = static_cast<DWORD>(_ttoi(pBaudRate->GetText()));
			BYTE ByteSize = static_cast<BYTE>(_ttoi(pByteSize->GetText()));
			BYTE StopBits = static_cast<BYTE>(pStopBits->GetCurSel());
			BYTE Parity = static_cast<BYTE>(pParity->GetCurSel());

			// Update DCB.

			//Setup the baud rate
			config.dcb.BaudRate = dwBaudRate; 

			//Setup the Parity
			switch (Parity)
			{
			case EvenParity:  
				{
					config.dcb.Parity = EVENPARITY;
					break;
				}
			case MarkParity:  
				{
					config.dcb.Parity = MARKPARITY;
					break;
				}
			case NoParity:    
				{
					config.dcb.Parity = NOPARITY;
					break;
				}
			case OddParity:   
				{
					config.dcb.Parity = ODDPARITY;
					break;
				}
			case SpaceParity: 
				{
					config.dcb.Parity = SPACEPARITY;
					break;
				}
			default:          
				{
					ASSERT(FALSE);            
					break;
				}
			}

			//Setup the data bits
			config.dcb.ByteSize = ByteSize;

			//Setup the stop bits
			switch (StopBits)
			{
			case OneStopBit:           
				{
					config.dcb.StopBits = ONESTOPBIT; 
					break;
				}
			case OnePointFiveStopBits: 
				{
					config.dcb.StopBits = ONE5STOPBITS;
					break;
				}
			case TwoStopBits:          
				{
					config.dcb.StopBits = TWOSTOPBITS;
					break;
				}
			default:                   
				{
					ASSERT(FALSE);
					break;
				}
			}

			//Setup the flow control
			DWORD fc = NoFlowControl;
			CStdString szProtocal = pProtocal->GetText();
			if (_tcsicmp(szProtocal, _T("Xon / Xoff")) == 0)
				fc = XonXoffFlowControl;
			else if (_tcsicmp(szProtocal, _T("硬件")) == 0)
				fc = CtsRtsFlowControl;

			config.dcb.fDsrSensitivity = FALSE;

			switch (fc)
			{
			case NoFlowControl:
				{
					config.dcb.fOutxCtsFlow = FALSE;
					config.dcb.fOutxDsrFlow = FALSE;
					config.dcb.fOutX = FALSE;
					config.dcb.fInX = FALSE;
					break;
				}
			case CtsRtsFlowControl:
				{
					config.dcb.fOutxCtsFlow = TRUE;
					config.dcb.fOutxDsrFlow = FALSE;
					config.dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
					config.dcb.fOutX = FALSE;
					config.dcb.fInX = FALSE;
					break;
				}
			case CtsDtrFlowControl:
				{
					config.dcb.fOutxCtsFlow = TRUE;
					config.dcb.fOutxDsrFlow = FALSE;
					config.dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
					config.dcb.fOutX = FALSE;
					config.dcb.fInX = FALSE;
					break;
				}
			case DsrRtsFlowControl:
				{
					config.dcb.fOutxCtsFlow = FALSE;
					config.dcb.fOutxDsrFlow = TRUE;
					config.dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
					config.dcb.fOutX = FALSE;
					config.dcb.fInX = FALSE;
					break;
				}
			case DsrDtrFlowControl:
				{
					config.dcb.fOutxCtsFlow = FALSE;
					config.dcb.fOutxDsrFlow = TRUE;
					config.dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
					config.dcb.fOutX = FALSE;
					config.dcb.fInX = FALSE;
					break;
				}
			case XonXoffFlowControl:
				{
					config.dcb.fOutxCtsFlow = FALSE;
					config.dcb.fOutxDsrFlow = FALSE;
					config.dcb.fOutX = TRUE;
					config.dcb.fInX = TRUE;
					config.dcb.XonChar = 0x11;
					config.dcb.XoffChar = 0x13;
					config.dcb.XoffLim = 100;
					config.dcb.XonLim = 100;
					break;
				}
			default:
				{
					ASSERT(FALSE);
					break;
				}
			}

			if (SetDefaultCommConfig(port_name_.c_str(), &config, sizeof(config)))
			{
			}
		}

		dwLastError = GetLastError();
	}
	dwLastError = GetLastError();

	return;
}