// ----------------------------------------------------------------------------
//              INCLUDE FILES                  
// ----------------------------------------------------------------------------

#include "PdkStd.h"
#include "PdkMem.h"

// ----------------------------------------------------------------------------
//              GLOBAL DATA DEFINITIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              LOCAL #DEFINES, TYPEDEF'S, and TEMPLATES             
// ----------------------------------------------------------------------------

#define	PARAMETERCHECKING	FALSE

// ----------------------------------------------------------------------------
//              LOCAL (STATIC) FUNCTIONS             
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//              CLASS ATTRIBUTE (STATIC) INITIALIZATIONS
// ----------------------------------------------------------------------------

static	BOOL	m_bTraceDrvMem=FALSE;

static DWORD  m_memInUse = 0;

// ----------------------------------------------------------------------------
//              CLASS METHOD / FUNCTION DEFINITIONS
// ----------------------------------------------------------------------------

// ...........................................................................
//
// NAME:	PdkMemAlloc
//
// IMPLEMENTATION NOTES:
// ...........................................................................
LPVOID
PdkMemAlloc(size_t bytes)
{
	#ifdef _WIN32
		#ifdef KERNEL_MODE
			return PdkAllocPtr(bytes);
		#else
			LPVOID ptr = (LPVOID)GlobalAlloc(GMEM_FIXED, (bytes));
		
			if (ptr != NULL)
				m_memInUse += (DWORD)bytes;
			
			return (ptr);
		#endif // KERNEL_MODE
	#else // !_WIN32
		return PdkAllocPtr(bytes);
	#endif // _WIN32
}

// ...........................................................................
//
// NAME:	PdkMemFree
//
// IMPLEMENTATION NOTES:
// ...........................................................................
VOID
PdkMemFree(LPVOID ptr)
{
	#ifdef _WIN32
		#ifdef KERNEL_MODE
			PdkFreePtr(ptr);
		#else
			SIZE_T bytes = GlobalSize((HGLOBAL) ptr);
			
			if ((GlobalFree((HGLOBAL) ptr)) == NULL)
				m_memInUse -= (DWORD)bytes;
		#endif // KERNEL_MODE
	#else // !_WIN32
		PdkFreePtr(ptr);
	#endif // _WIN32
}

// ...........................................................................
//
// NAME:	PdkMemDump
//
// IMPLEMENTATION NOTES:
// ...........................................................................
VOID
PdkMemDump(VOID)
{
#ifndef KERNEL_MODE
	TRACE(_T("PdkMemDump: memInUse = %ld\n"), m_memInUse);
#endif
}

// ...........................................................................
//
// NAME:	PdkMemCpy
//
// IMPLEMENTATION NOTES:
// ...........................................................................
void *PdkMemCpy(LPBYTE lpDest,		// Destination buffer pointer
				LPBYTE lpSource,	// Source buffer pointer
				size_t lCount)		// Count of bytes to move
{
	PUSHTRACE(m_bTraceDrvMem);

	if (lpDest == NULL)
		return(NULL);
	if (lpSource == NULL)
		return(lpDest);

#ifndef	_WIN32
#if	PARAMETERCHECKING
#pragma message("DrvMem parameter checking is ON, this will slow things down considerably")
	if (IsBadWritePtr(lpDest, lCount))
		return(NULL);
	if (IsBadReadPtr(lpSource, lCount))
		return(NULL);
#endif // PARAMETERCHECKING

	// We use memmove here, just in case somebody does a copy with
	// overlapping regions.  It's probably slower than memcopy, but
	// it's also safer.
	return _fmemmove(lpDest, lpSource, lCount);

	#if 0
	UINT	ucount;           // Temporary short int
	BOOL	bReverse = FALSE; // Bottom up?

	// If we are moving less than 64K, we can do it all in one call.
	if (HIWORD(lCount) == 0)
	{
		MEMCMP(lpDest, lpSource, LOWORD(lCount));
		return(lpDest);
	}

	// We are now dealing with >64K of data. If the destination 
	// is lower in memory than the source, we can repetitively 
	// copy 64K blocks from the top of each block. If the destination
	// is higher in memory than the source, we must repetitively move 
	// 64K blocks from the bottom of the source and destination to
	// prevent overwriting the source. We start by adjusting the source
	// and destination pointers to point at the last 64K chunk in the
	// block and set a flag (bReverse). After each block move, we move
	// the source and destination pointers up or down appropriately.
	if (lpDest > lpSource)
	{
		lpSource += (lCount - 65535);  
		lpDest += (lCount - 65535);
		bReverse = TRUE;
	}

	ASSERT(0);
	while (TRUE)
	{
		if (HIWORD(lCount))
			ucount = 65535;       // >64K left; set ucount to 64K
		else
			ucount = LOWORD(lCount);

		_fmemcpy(lpDest, lpSource, ucount);

		lCount -= ucount;        // Reduce the amount left to move
		
		if (lCount == 0)
			return(lpDest);               // All done; return
		if (bReverse)
		{          // Update source/destination pointers
			if (HIWORD(lCount))
			{
				lpDest -= ucount;
				lpSource -= ucount;
			}
			else
			{
				lpDest -= LOWORD(lCount);
				lpSource -= LOWORD(lCount);
			}
		}
		else
		{
			lpDest += ucount;
			lpSource += ucount;
		}
	}
	
	return(lpDest);
	#endif // 0
	
#else // _WIN32
#if	PARAMETERCHECKING
	if (IsBadWritePtr(lpDest, lCount))
		return(NULL);
	if (IsBadReadPtr(lpSource, lCount))
		return(NULL);
#endif // PARAMETERCHECKING
	return(memcpy(lpDest, lpSource, lCount));
#endif // !_WIN32
}

void *PdkMemSet(LPBYTE lpDest, BYTE c, size_t lCount)
{
	PUSHTRACE(m_bTraceDrvMem);
	TRACE(_T("PdkMemSet(%lx, %d, %lx(%ld))\n"), lpDest, c, lCount, lCount);

	if (lpDest == NULL)
		return(NULL);

#ifndef _WIN32
#if	PARAMETERCHECKING
	if (IsBadWritePtr(lpDest, lCount))
		return(NULL);
#endif // PARAMETERCHECKING

	return _fmemset(lpDest, c, lCount);

	#if 0
	UINT	ucount;           // Temporary short int
	// If we are setting less than 64K, we can do it all in one call.
	if (HIWORD(lCount) == 0)
	{
		_fmemset(lpDest, c, LOWORD(lCount));
		return(lpDest);
	}

	ASSERT(0);
	while (TRUE)
	{
		if (HIWORD(lCount))
			ucount = 65535;       // >64K left; set ucount to 64K
		else
			ucount = LOWORD(lCount);
		TRACE(_T("PdkMemSet loop, lpDest=%lx, lCount=%lx(%ld), ucount=%d(%x)\n"), lpDest, lCount, lCount, ucount, ucount);

		_fmemset(lpDest, c, ucount);

		lCount -= ucount;        // Reduce the amount left to move
		
		if (lCount == 0)
			return(lpDest);               // All done; return

		lpDest += ucount;
	}

	return(lpDest);
	#endif // 0
	
#else // _WIN32
#if	PARAMETERCHECKING
	if (IsBadWritePtr(lpDest, lCount))
		return(NULL);
#endif // PARAMETERCHECKING
	return(memset(lpDest, c, lCount));
#endif // !_WIN32
}

/*
*	< 0 buf1 less than buf2 
*	0 buf1 identical to buf2 
*	> 0 buf1 greater than buf2 
*/
int PdkMemCmp(LPBYTE lpBuf1, LPBYTE lpBuf2, size_t lCount)
{
	PUSHTRACE(m_bTraceDrvMem);

	if ( (lpBuf1 == NULL) || (lpBuf2 == NULL) )
		return(0);

#ifndef	_WIN32
#if	PARAMETERCHECKING
	if (IsBadReadPtr(lpBuf1, lCount))
		return(0);
	if (IsBadReadPtr(lpBuf2, lCount))
		return(0);
#endif // PARAMETERCHECKING

	return _fmemcmp(lpBuf1, lpBuf2, lCount);

	#if 0
	UINT	ucount;           // Temporary short int
	int	rc;

	// If we are setting less than 64K, we can do it all in one call.
	if (HIWORD(lCount) == 0)
	{
		return (MEMCMP(lpBuf1, lpBuf2, LOWORD(lCount)));
	}

	ASSERT(0);
	while (TRUE)
	{
		if (HIWORD(lCount))
			ucount = 65535;       // >64K left; set ucount to 64K
		else
			ucount = LOWORD(lCount);

		rc = MEMCMP(lpBuf1, lpBuf2, ucount);
		if (rc != 0)
			return(rc);

		lCount -= ucount;        // Reduce the amount left to move
		
		if (lCount == 0)
			return(0);               // All done; return

		lpBuf1 += ucount;
		lpBuf2 += ucount;
	}

	return(0);
	#endif // 0
	
#else // _WIN32
#if	PARAMETERCHECKING
	if (IsBadReadPtr(lpBuf1, lCount))
		return(0);
	if (IsBadReadPtr(lpBuf2, lCount))
		return(0);
#endif // PARAMETERCHECKING
	return(MEMCMP(lpBuf1, lpBuf2, lCount));
#endif // !_WIN32
}



#ifdef KERNEL_MODE
//=============================================================================
//  Emulation of memory manager heap functions in kernel mode
//=============================================================================


// ...........................................................................
//
// NAME:	HEAPCREATE
//
// IMPLEMENTATION NOTES:
// ...........................................................................
HANDLE
HEAPCREATE(VOID)
{
	HHEAP   hHeap;

	// Heap handle is simply a pointer to PHEAP

	hHeap = (HHEAP) EngAllocMem(0, sizeof(PHEAP), HEAPMEMTAG);

	if (hHeap == NULL) {
		TRACE(_T("ERROR: Failed to create heap.\n"));
	} else
		*hHeap = NULL;

	return (HANDLE) hHeap;
}

// ...........................................................................
//
// NAME:	HEAPDESTROY
//
// IMPLEMENTATION NOTES:
// ...........................................................................
BOOL
HEAPDESTROY(HANDLE hHandle)
{
	PHEAP   pNext, pHeap;
    HHEAP   hHeap = (HHEAP) hHandle;

	if (hHeap == NULL)
	{
		TRACE(_T("WARNING: HEAPDESTROY called w/ NULL handle...ignoring\n"));
		return TRUE;
	}
	
	pHeap = *hHeap;

	// Make sure all memory allocated from this heap
	// are freed when the heap is destroyed.

	while (pHeap != NULL) {
		pNext = pHeap->pNext;
		EngFreeMem(pHeap);
		pHeap = pNext;
	}

	// Free the heap handle itself

	EngFreeMem(hHeap);
    return TRUE;
}

// ...........................................................................
//
// NAME:	HEAPALLOC
//
// IMPLEMENTATION NOTES:
// ...........................................................................
PVOID
HEAPALLOC(HANDLE hHandle, DWORD dwSize)
{
	PHEAP   pHeap;
    HHEAP   hHeap = (HHEAP) hHandle;

	// Allocate memory (with addition space for linked-list pointer)

	pHeap = (PHEAP) EngAllocMem(0, sizeof(HEAP) + dwSize, HEAPMEMTAG);

	if (pHeap == NULL) {
		TRACE(_T("ERROR: Memory allocation failed.\n"));
		return NULL;
	}

	// Insert the newly allocated memory to the head of linked-list

	pHeap->pNext = *hHeap;
	*hHeap = pHeap;

	// Return a pointer to the usable memory

	return ((LPBYTE) pHeap) + sizeof(HEAP);
}

// ...........................................................................
//
// NAME:	HEAPFREE
//
// IMPLEMENTATION NOTES:
// ...........................................................................
BOOL
HEAPFREE(HANDLE hHandle, PVOID ptr)
{
	PHEAP   pPrev, pHeap;
    HHEAP   hHeap = (HHEAP) hHandle;

	if ((hHeap == NULL) || (ptr == NULL))
	{
		TRACE(_T("WARNING: HEAPFREE called w/ NULL handle...ignoring\n"));
		return TRUE;
	}
	// Find the linked-list node corresponding to the memory pointer

	pPrev = NULL;
	pHeap = *hHeap;

	while (pHeap != NULL && ptr != (((LPBYTE) pHeap) + sizeof(HEAP))) {
		pPrev = pHeap;
		pHeap = pHeap->pNext;
	}

	// Make sure the memory pointer was indeed allocated from the heap

	if (pHeap != NULL) {

		// Modified the linked-list

		if (pPrev == NULL)
			*hHeap = pHeap->pNext;
		else
			pPrev->pNext = pHeap->pNext;

		// Free the memory

		EngFreeMem(pHeap);
	}
    return TRUE;
}

#endif  //KERNEL_MODE

#ifdef WIN_95

/*****************************************************************************
 *
 *  GetFreeMemInfo()
 *	Gets the free memory information from the system by a call to DPMI.
 *
 *  Parameters:
 *	lpFreeMemInfo	pointer to a FREEMEMINFO structure to return
 *	lpPageSize	pointer to a DWORD to return size of memory page
 *
 *  Returns:
 *	TRUE if get was successful.
 *
 *****************************************************************************/

typedef struct _freemeinfo {
	DWORD	dwMaxFreeBlock;		// Largest available free block in bytes
	DWORD	dwMaxUnlocked;		// Maximum unlocked page allocation
	DWORD	dwMaxLocked;		// Maximum locked page allocation
	DWORD	dwLinearSize;		// Linear address space size in pages
	DWORD	dwTotUnlocked;		// Total number of unlocked pages
	DWORD	dwNumFreePages;		// Number of free pages
	DWORD	dwTotPhysPages;		// Total number of physical pages
	DWORD	dwFreeLinearSpace;		// Free linear address space in pages
	DWORD	dwSizeSwap; 		// Size of paging file/partition in pages
	DWORD	dwReserved1;
	DWORD	dwReserved2;
	DWORD	dwReserved3;
} FREEMEMINFO, FAR* LPFREEMEMINFO;

typedef DWORD FAR*		LPDWORD;

static BOOL	GetFreeMemInfo (LPFREEMEMINFO lpFreeMemInfo,
				LPDWORD lpPageSize)
{
	DWORD	dwPageSize;
	BOOL	fSuccess = FALSE;

	_asm
	{
		push	di
		mov	ax, 0604h		// Get Page Size DPMI
		int	31h
		jc		error
		mov	word ptr dwPageSize, cx
		mov	word ptr dwPageSize+2, bx
		les	di, lpFreeMemInfo
		mov	ax, 0500h		// Get Free Memory Information DPMI
		int	31h
		jc		error
		mov	fSuccess, 1
	error:
		pop	di
	}

	*lpPageSize = dwPageSize;
	return fSuccess;
}

/*****************************************************************************
 *
 *  PdkGetWinFlags()
 *	Returns the __WinFlags of KERNEL.178.
 *
 *****************************************************************************/
WORD PdkGetWinFlags()
{
    WORD wWinFlags = (WORD) GetWinFlags ();
    return wWinFlags;
}

/*****************************************************************************
 *
 *  PdkMemoryAvailable()
 *	Returns the size of available memory for the raster buffer.
 *
 *  Returns:
 *	The number of 64K segments available for the raster buffer.
 *
 *****************************************************************************/
WORD PdkMemoryAvailable(void)
{
	DWORD	dwWinFlags = GetWinFlags ();
	WORD	wNumSegments;
	FREEMEMINFO FreeMemInfo;
	DWORD	dwPageSize;

	/*
	* Here's how we decide how much to ask for.
	*
	*	Real mode	- 64K (This is all GlobalAlloc will give us)
	*	Standard mode - 286 : 256K, 386 : 512K, 486 : 1M (We do not know
	*			(memory availability so these are guesswork)
	*	Enhanced mode - If we can get the number of lockable pages then
	*			(LockPages * PageSize) * 2/3 + 192Kbytes,
	*			If we can get the Max free block value
	*			MaxFreeBlock / 2
	*			Otherwise we do same as standard mode
	*/
	if ((dwWinFlags & WF_ENHANCED) &&
		GetFreeMemInfo(&FreeMemInfo, &dwPageSize))
	{
		DWORD	dwFreePhysBytes;
	
		if (FreeMemInfo.dwMaxLocked != 0xffffffff)
		{
			dwFreePhysBytes = FreeMemInfo.dwMaxLocked * dwPageSize;
	
			wNumSegments = (dwFreePhysBytes > 0x40000)?
					(WORD)(((dwFreePhysBytes * 2) / 3) / 0x10000) + 3 :
					4;
		}
		else
			wNumSegments = (WORD)(FreeMemInfo.dwMaxFreeBlock / (2 * 0x10000));
	}
	else if (dwWinFlags & (WF_ENHANCED | WF_STANDARD))
	{
		if (dwWinFlags & WF_CPU486)
			wNumSegments = 16;
		else if (dwWinFlags & WF_CPU386)
			wNumSegments = 8;
		else
			wNumSegments = 4;
	}
	else
	{
		// Real mode
		wNumSegments = 1;
	}

	return wNumSegments;
}

HANDLE	PdkAllocateImageMemory(WORD width,
							   LPWORD lpheight,
							   LPDWORD lpMemAllocated)
{
	static const long IMG_BUF_MAX = 60*1024L;
	long	Memory_Len = (long)width * ((long)(*lpheight) + 2 );
	HANDLE	hBuf;

	if (lpMemAllocated)
		*lpMemAllocated = 0;

	if(Memory_Len >= IMG_BUF_MAX)
	{
		Memory_Len = IMG_BUF_MAX;
		if(Memory_Len > (long)width)
		{
			*lpheight	= (WORD)(Memory_Len / width);
			Memory_Len += width * 2;
		} 
		else
		{
			Memory_Len = (long)width * 3;
			*lpheight	= 1;
		}
	}

	while((hBuf = GlobalAlloc(GMEM_MOVEABLE,Memory_Len)) == NULL)
	{
		*lpheight = (*lpheight >> 2);
		Memory_Len = width * ((*lpheight) + 2);
		if ( !(*lpheight))
			return NULL;
	}
	if (lpMemAllocated)
		*lpMemAllocated = Memory_Len;
	return hBuf;
}

#endif // WIN_95

#ifndef _WIN32

// ...........................................................................
//
// NAME:	PdkMemSetHuge
//
// IMPLEMENTATION NOTES: For Windows 9X only.
// ...........................................................................
#define	_64K_Minus_1 (64*1024L-1)
void PdkMemSetHuge(BYTE _huge * hpDest, int val, DWORD count)
{
	while (count > _64K_Minus_1)
	{	
		_fmemset(hpDest, val, (WORD)_64K_Minus_1);
		hpDest += _64K_Minus_1;
		count -= _64K_Minus_1;
	}
	
	if (count)
		_fmemset(hpDest, val, (WORD)count);
}

#endif // !_WIN32

