#ifndef	__PdkMEM_H__
#define	__PdkMEM_H__

// --------------------------------------------------------------------
//              INCLUDES
// --------------------------------------------------------------------

// --------------------------------------------------------------------
//      		#DEFINES, TYPEDEF'S, and TEMPLATES
// --------------------------------------------------------------------

// --------------------------------------------------------------------
//              EXPORTED CLASSES
// --------------------------------------------------------------------

LPVOID PdkMemAlloc(size_t bytes);
VOID PdkMemFree(LPVOID ptr);
VOID PdkMemDump(VOID);
void *PdkMemCpy(LPBYTE lpDest, LPBYTE hpSource, size_t lCount);
void *PdkMemSet(LPBYTE hpDest, BYTE c, size_t lCount);
int   PdkMemCmp(LPBYTE hpBuf1, LPBYTE hpBuf2, size_t lCount);

#ifdef _WIN32
#ifdef KERNEL_MODE
// Emulation of memory manager heap functions

#define MEMTAG		'mspD'
#define HEAPMEMTAG	'hspD'

typedef struct _HEAP {
	struct _HEAP *  pNext;
} HEAP, *PHEAP, **HHEAP;

HANDLE HEAPCREATE(VOID);
BOOL HEAPDESTROY(HANDLE hHandle);
PVOID HEAPALLOC( HANDLE hHandle, DWORD  dwSize);
BOOL HEAPFREE(HANDLE hHandle, PVOID ptr);

#else // KERNEL_MODE

typedef HANDLE				  HHEAP;

#endif // KERNEL_MODE
#endif // _WIN32

#ifndef _WIN32

DWORD	FAR PASCAL GetWinFlags(void);

#define WF_PMODE	0x0001
#define WF_CPU286	0x0002
#define WF_CPU386	0x0004
#define WF_CPU486	0x0008
#define WF_STANDARD	0x0010
#define WF_WIN286	0x0010
#define WF_ENHANCED	0x0020
#define WF_WIN386	0x0020
#define WF_CPU086	0x0040
#define WF_CPU186	0x0080
#define WF_LARGEFRAME	0x0100
#define WF_SMALLFRAME	0x0200
#define WF_80x87	0x0400
#define WF_PAGING	0x0800
#define WF_WLO		0x8000

WORD PdkGetWinFlags(void);
WORD PdkMemoryAvailable(void);
HANDLE	PdkAllocateImageMemory(WORD width,
							   LPWORD lpheight,
							   LPDWORD lpMemAllocated);
#define Lock_Image_Memory(hBuf)      PdkGlobalLock(hBuf)
#define Unlock_Image_Memory(hBuf)    PdkGlobalUnlock(hBuf)
#define Free_Image_Memory(hBuf)      PdkGlobalFree(hBuf)
void PdkMemSetHuge(BYTE _huge * hpDest, int val, DWORD count);

#define WinFlags (PdkGetWinFlags())
#define ProtectMode (WinFlags & WF_PMODE)
#define RealMode (!ProtectMode)
#define StandardMode (ProtectMode && (WinFlags & WF_STANDARD))

#endif // !_WIN32

#endif	// __PdkMEM_H__
