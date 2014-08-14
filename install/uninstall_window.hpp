//
// uninstall_window.hpp
// ~~~~~~~~~~~~~~~~~~~
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

#ifndef UNINSTALL_WINDOW_HPP
#define UNINSTALL_WINDOW_HPP

class WindowImplBase;
class UnInstallWindow : public WindowImplBase
{
public:

	UnInstallWindow();
	~UnInstallWindow();

public:

	LPCTSTR GetWindowClassName() const;	

	virtual void OnFinalMessage(HWND hWnd);

	virtual void Init();

	virtual tString GetSkinFile();

	virtual tString GetSkinFolder();

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LONG GetStyle();

protected:	

	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);

	void EnumInstalledDriver();

	void AddProgressInfo(int progress);

	static BOOL SystemReboot();

	static BOOL RestartSpoolsv();
	
	static void DoStopSvc(LPCTSTR szSvcName);

	static BOOL StopDependentServices(SC_HANDLE schSCManager, SC_HANDLE schService);

	static void DoStartSvc(LPCTSTR szSvcName);

	static DWORD WINAPI UnInstallPrinterDriver(LPVOID pVoid);

protected:

	HANDLE uninstall_thread_;

	OS_CLockable os_locker_;
	bool driver_uninstalled_;

	tString error_info_;

	vector<tString> printers_uninstalled_list_;
};

#endif // UNINSTALL_WINDOW_HPP