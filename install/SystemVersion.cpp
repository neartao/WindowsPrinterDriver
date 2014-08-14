//////////////////////////////////////////////////////////////////////
// SystemVersion.cpp: defination for the SystemVersion class.
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "SystemVersion.h"

Windows_Version::Windows_Version()
{
	_osverOld.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
}

bool Windows_Version::GetVersion9x(WindowsVersion& win_ver, tString& osname)
{
	GetVersionEx (&_osverOld);
	bool result = 0;

	// Windows 95
	if ((_osverOld.dwMajorVersion = 4) && (_osverOld.dwMinorVersion = 0) && (_osverOld.dwPlatformId = 1))
	{
		win_ver = Windows95;
		osname = _T("Windows 95");
		result = true;
	}
	else
	{
		// Windows NT 4.0
		if ((_osverOld.dwMajorVersion = 4) && (_osverOld.dwMinorVersion = 0) && (_osverOld.dwPlatformId = 2))
		{
			win_ver = WindowsNT;
			osname = _T("Windows NT 4.0");
			result = true;
		}
		else
		{
			// Windows 98
			if ((_osverOld.dwMajorVersion = 4) && (_osverOld.dwMinorVersion = 10))
			{
				win_ver = Windows98;
				osname = _T("Windows 98");
				result = true;
			}
			else
			{
				// Windows Me
				if ((_osverOld.dwMajorVersion = 4) && (_osverOld.dwMinorVersion = 90))
				{
					win_ver = WindowsME;
					osname = _T("Windows Me");
					result = true;
				}
			}
		}
	}
	return result;
}

bool Windows_Version::GetVersionNT(WindowsVersion& win_ver, tString& osname)
{
	_osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	GetVersionEx ((LPOSVERSIONINFO)&_osver);

	bool result = false;

	// Windows 2000
	if ((_osver.dwMajorVersion = 5) && (_osver.dwMinorVersion = 0))
	{
		win_ver = Windows2000;
		osname = _T("Windows 2000");
		result = true;
	}
	else
	{
		// Windows XP
		if ((_osver.dwMajorVersion = 5) && (_osver.dwMinorVersion = 1))
		{
			if (_osver.wServicePackMajor = 2)
			{
				result = true;
			}
			else
			{
				result = true;
			}
			win_ver = WindowsXP;
			osname = _T("Windows XP");
		}
		else
		{
			// Windows Server 2003
			if ((_osver.dwMajorVersion = 5) && (_osver.dwMinorVersion = 2))
			{
				win_ver = WindowsServer2003;
				osname = _T("Windows Server 2003");
				result = true;
			}
			else
			{
				// Windows Vista
				if ((_osver.dwMajorVersion = 6) && (_osver.dwMinorVersion = 0) && (_osver.wProductType == 1))
				{
					win_ver = WindowsVista;
					osname = _T("Windows Vista");
					result = true;
				}
				// Windows 7
				else if ((_osver.dwMajorVersion = 6) && (_osver.dwMinorVersion = 1) && (_osver.wProductType == 1))
				{
					win_ver = Windows7;
					osname = _T("Windows 7");
					result = true;
				}
				// Windows Server 2008 
				else if ((_osver.dwMajorVersion = 6) && (_osver.wProductType == 3))
				{
					win_ver = WindowsServer2008;
					osname = _T("Windows Server 2008 ");
					result = true;
				}
			}
		}
	}
	return result;
}