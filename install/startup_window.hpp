//
// startup_window.hpp
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

#ifndef STARTUP_WINDOW_HPP
#define STARTUP_WINDOW_HPP

class WindowImplBase;
class StartupWindow : public WindowImplBase
{
public:

	StartupWindow();
	~StartupWindow();

public:

	LPCTSTR GetWindowClassName() const;	

	virtual void OnFinalMessage(HWND hWnd);

	virtual void Init();

	virtual tString GetSkinFile();

	virtual tString GetSkinFolder();

	virtual LONG GetStyle();

protected:	

	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
};

#endif // STARTUP_WINDOW_HPP