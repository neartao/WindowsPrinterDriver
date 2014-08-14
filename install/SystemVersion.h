//////////////////////////////////////////////////////////////////////
// SystemVersion.h: interface for the SystemVersion class.
//////////////////////////////////////////////////////////////////////
#ifndef SYSTEMVERSION_H
#define SYSTEMVERSION_H

#include <Windows.h>
#include <WinNT.h>

typedef enum WindowsVersion
{
	Windows95,
	Windows98,
	WindowsME,
	WindowsNT,
	Windows2000,
	WindowsXP,
	WindowsServer2003,
	WindowsVista,
	WindowsServer2008,
	Windows7,
};

class Windows_Version
{
public:
	Windows_Version();

	bool GetVersion9x(WindowsVersion& win_ver, tString& osname);

	bool GetVersionNT(WindowsVersion& win_ver, tString& osname);	

private:
	OSVERSIONINFO _osverOld;
	OSVERSIONINFOEX _osver;
};

#endif // SYSTEMVERSION_H