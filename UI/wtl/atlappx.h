#ifndef __ATLAPPX_H__
#define __ATLAPPX_H__

/////////////////////////////////////////////////////////////////////////////
// Additional Instance wrappers
//
// Written by achellies (achellies@163.com)
// Copyright (c) 2010-2011 achellies.
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

#pragma once

#ifndef __cplusplus
   #error ATL requires C++ compilation (use a .cpp suffix)
#endif


namespace WTL
{

template<typename T, typename TMainWindow, bool bInitializeCom = true>
class CWTLApplicationT :
    public CAppModule
{
public:
    //Call this method in the application's entry point(_tWinMain(),ect...)
    int WinMain(HINSTANCE hInstance,LPTSTR lpCmdLine,int nCmdShow)
    {
        UNREFERENCED_PARAMETER(lpCmdLine);
        T* pT = static_cast<T*>(this);
        pT->OnStartup();
        HRESULT hr = S_OK;
        if (bInitializeCom)
        {
            hr = ::CoInitialize(NULL);
            // If you are running on NT 4.0 or higher you can use the following 
            // call instead to make the EXE free threaded. This means that calls 
            // come in on a random RPC thread.
            //  hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
            CHECKHR(hr);
        }
        // This resolves ATL window thunking problem when Microsoft Layer 
        // for Unicode (MSLU) is used
        ::DefWindowProc(NULL, 0, 0, 0L);
        
        pT->OnInitCommonControl();
        hr = Init(NULL, hInstance);
        CHECKHR(hr);
        int nRet = 0;
        {
            CMessageLoop msgLoop;
            AddMessageLoop(&msgLoop);
            TMainWindow wndMain;
            if(wndMain.Create(NULL,CWindow::rcDefault) == NULL)
            {
                ATLTRACE(_T("Failed to create Main window!\n"));
                return 0;
            }
            wndMain.ShowWindow(nCmdShow);
            wndMain.UpdateWindow();
            nRet = msgLoop.Run();
            RemoveMessageLoop();
        }
        Term();
        if (bInitializeCom)
            ::CoUninitialize();
        pT->OnShutdown();
        return nRet;
    }
protected:
    //Overridable methods
    void OnStartup(){}
    void OnShutdown(){}
    void OnInitCommonControl()
    {
        // Add flags to support other controls
        AtlInitCommonControls(ICC_COOL_CLASSES|ICC_BAR_CLASSES|ICC_WIN95_CLASSES);
    }
};
template<typename TMainWindow, bool bInitializeCom = true>
class CWTLApplication :
    public CWTLApplicationT<CWTLApplication<TMainWindow,bInitializeCom>, TMainWindow, bInitializeCom>
{
};

}; // namespace WTL

#endif // __ATLAPPX_H__
