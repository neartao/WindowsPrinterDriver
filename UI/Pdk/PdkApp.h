#ifndef PDKAPP_H_ // (make sure this is only included once)
#define PDKAPP_H_

#ifndef __cplusplus
#error PdkApp.h included in a non-C++ source file!
#endif

// --------------------------------------------------------------------
//              INCLUDES
// --------------------------------------------------------------------
#include "PdkBase.h"

// --------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// --------------------------------------------------------------------

// --------------------------------------------------------------------
//              EXPORTED CLASSES
// --------------------------------------------------------------------


// ....................................................................
// CLASS NAME:      PdkApp
//
// REPONSIBILITIES:
//
// NOTES:
// ....................................................................
class CPdkApp : public CPdkBase
{
	public:
		// ## Constructors / Destructor
		CPdkApp ();  // class constructor
		~CPdkApp (); // class destructor

		// ## Public Members

		// ....................................................................
		// METHOD NAME:		InitApp
		// DESCRIPTION:
		//		Initializes the application/DLL by setting the app/dll instance
		//		handle and names. This method will also open the supplied
		//		resource DLL by trying the array of directories until the
		//		first successful load has been accomplished. m_hInstRes and
		//		m_hInst will both be initialized to hInst. If a separate
		//		DLL is required for resources, use OpenResourceDLL() below.
		//
		//		szResourceDLLPath should contain the full path to the resource 
		//		DLL and m_hInstRes will be set to the return value of the
		//		LoadLibrary call. If szResourceDLLPath is empty length or NULL, 
		//		the current value of m_hInst will be used for m_hInstRes.
		//
		//		The resource DLL will not actually be loaded upon this call.
		//		Instead, it will be loaded the first time it is needed when
		//		accessed through GetResourceInst()
		//
		//		After this call, m_hInst, m_szResourceDLLPath, m_szModName, and
		//		m_szAppName are initialized. m_hInstRes is not initialized.
		//		
		// RETURN VALUE:
		//		Returns 0 (ERROR_SUCCESS) on success
		// ....................................................................
		int InitApp(HINSTANCE hInst, const TCHAR *szModName, 
						const TCHAR *szAppName, const TCHAR *szResourceDLLPath);

		// ....................................................................
		// METHOD NAME:		GetInst
		// DESCRIPTION:
		//		Returns the instance handle of this application or DLL.
		//		
		// RETURN VALUE:
		// ....................................................................
		HINSTANCE GetInst() const;

#ifndef KERNEL_MODE
		// ....................................................................
		// METHOD NAME:		GetResourceInst
		// DESCRIPTION:
		//		Returns the instance handle of this applications resource DLL.
		//		If m_hInstRes has not yet been set, then it will attempt to
		//		execute LoadLibrary to load the DLL using the m_szResourceDLLPath
		//		member. If m_szResourceDLLPath is not set, it will return 
		//		m_hInst.
		//
		//		If will return NULL if the LoadLibrary call failed.
		//		
		// RETURN VALUE:
		// ....................................................................
	#ifndef WIN_95
		HINSTANCE GetResourceInst() const;
	#else
		HINSTANCE GetResourceInst();
	#endif

		// ....................................................................
		// METHOD NAME:		GetVerString
		// DESCRIPTION:
		//		Returns version information in string format for each of the
		//		fields supplied. Any of the supplied strings may be NULL if
		//		this info is not required. Any strings that are non-NULL must
		//		have character lengths of at least maxStrLen.
		//
		//		The first language available in the version resource is used to
		//		supply this information.
		//		
		// RETURN VALUE:
		//		The encapsulated devmode or NULL on error
		// ....................................................................
		bool 
		GetVerString(unsigned int maxStrLen, TCHAR *szComments,
					  TCHAR *szCompanyName, TCHAR *szFileDescription,
					  TCHAR *szFileVersion, TCHAR *szLegalCopyright,
					  TCHAR *szProductName, TCHAR *szProductVersion);

#endif // !KERNEL_MODE
		
		// Name of this module - usually the DLL name
		TCHAR m_szModName[MAX_PATH];

		// Name of this application, usually used for debugging
		TCHAR m_szAppName[MAX_PATH];
		
	private:

		// ## Instance Variables

		// Handle to the default resources of the application / DLL
#ifndef WIN_95
		mutable
#endif
		 HINSTANCE m_hInstRes;

		// Handle to the current instance of this application. If this is a
		// DLL, then it is the HINSTANCE of the DLL.
		HINSTANCE m_hInst;

		// The full path to the resource DLL. If this is the empty string,
		// then m_hInst will be used as the instance handle to the resources.
		TCHAR m_szResourceDLLPath[MAX_PATH];

}; 

// This must be implemented once per DLL, so the PDK can
// get the actual driver object.
CPdkApp* GetPdkAppObj();


#endif  //  PDKAPP_H_
