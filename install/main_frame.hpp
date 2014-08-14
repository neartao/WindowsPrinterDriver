//
// main_frame.hpp
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

#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include "SystemVersion.h"

class WindowImplBase;
struct lua_State;
class MainFrame : public WindowImplBase
{
public:

	MainFrame();
	~MainFrame();

public:

	LPCTSTR GetWindowClassName() const;	

	virtual void OnFinalMessage(HWND hWnd);

	virtual void Init();

	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	virtual tString GetSkinFile();

	virtual tString GetSkinFolder();

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LONG GetStyle();

protected:	

	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);

	void UpdateNavigateButtonsStatus();

	void AddProgressInfo(const tString& info, int progress, bool red = false);

	tString GetDriverConfig(const TCHAR* name);

	void PrinterInstalledFinish();

	static bool IsPrinterDriverInstalled(const tString& driver_name);
	static bool IsPrinterAdded(tString printer_name, tString& valid_name);

	static DWORD WINAPI InstallPrinterDriver(LPVOID pVoid);

	void ConfigSerialPort();

protected:
	int back_pages_;
	int next_pages_;
	int total_pages_;
	bool driver_install_started_;
	bool driver_installed_;
	bool set_as_default_;

	WindowsVersion win_ver_;
	tString osname_;
	tString driver_source_path_;
	tString port_name_;
	tString error_info_;

	OS_CLockable os_locker_;

	lua_State* lua_state_;
	HANDLE install_thread_;

	//vector<tString> progress_info_;
};

#endif // MAINFRAME_HPP