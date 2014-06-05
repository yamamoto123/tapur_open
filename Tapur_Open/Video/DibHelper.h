//------------------------------------------------------------------------------
// File: DibHelper.H
//
// Desc: DirectShow code - Helper code for bitmap manipulation
//
// Copyright (c) tapur.com.  All rights reserved.
//------------------------------------------------------------------------------


#define HDIB HANDLE

/* DIB macros */
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define RECTWIDTH(lpRect)   ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)  ((lpRect)->bottom - (lpRect)->top)

// Function prototypes
HBITMAP CopyScreenToBitmap(LPRECT lpRect, BYTE *pData, BITMAPINFO *pHeader, BOOL blLogo);
HBITMAP CopyWindowToBitmap(HWND hWnd, LPRECT lpRect, BYTE *pData, BITMAPINFO *pHeader, BOOL blLogo);
