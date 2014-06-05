//------------------------------------------------------------------------------
// File: DibHelper.cpp
//
// Desc: DirectShow code - In-memory push mode capture source filter
//       Helper routines for manipulating bitmaps.
//
// Copyright (c) tapur.com.  All rights reserved.
//------------------------------------------------------------------------------

#include "Forcelib.h"
#include <windows.h>
#include "dibhelper.h"

#define LOGO_TEXT TEXT("Tapur for Skype - www.tapur.com")

static void AddText(HDC hDC)
{
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255,255,255));		// white
	TextOut(hDC, 8, 8, LOGO_TEXT, wcslen(LOGO_TEXT));
}

HBITMAP CopyScreenToBitmap(LPRECT lpRect, BYTE *pData, BITMAPINFO *pHeader,
BOOL blLogo)	// ロゴの有無
{
    HDC         hScrDC, hMemDC;         // screen DC and memory DC
    HBITMAP     hBitmap, hOldBitmap;    // handles to deice-dependent bitmaps
    int         nX, nY, nX2, nY2;       // coordinates of rectangle to grab
    int         nWidth, nHeight;        // DIB width and height
    int         xScrn, yScrn;           // screen resolution

    // check for an empty rectangle
    if (IsRectEmpty(lpRect))
      return NULL;

    // create a DC for the screen and create
    // a memory DC compatible to screen DC   
    hScrDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
    hMemDC = CreateCompatibleDC(hScrDC);

    // get points of rectangle to grab
    nX  = lpRect->left;
    nY  = lpRect->top;
    nX2 = lpRect->right;
    nY2 = lpRect->bottom;

    // get screen resolution
    xScrn = GetDeviceCaps(hScrDC, HORZRES);
    yScrn = GetDeviceCaps(hScrDC, VERTRES);

    //make sure bitmap rectangle is visible
    if (nX < 0)
        nX = 0;
    if (nY < 0)
        nY = 0;
    if (nX2 > xScrn)
        nX2 = xScrn;
    if (nY2 > yScrn)
        nY2 = yScrn;

    nWidth  = nX2 - nX;
    nHeight = nY2 - nY;

    // create a bitmap compatible with the screen DC
    hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);

    // select new bitmap into memory DC
    hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);

    // bitblt screen DC to memory DC
    BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);

	// Text impose
	if (blLogo) {
		AddText(hMemDC);
	}

	// select old bitmap back into memory DC and get handle to
    // bitmap of the screen   
    hBitmap = (HBITMAP)  SelectObject(hMemDC, hOldBitmap);

    // Copy the bitmap data into the provided BYTE buffer
    GetDIBits(hScrDC, hBitmap, 0, nHeight, pData, pHeader, DIB_RGB_COLORS);

    // clean up
    DeleteDC(hScrDC);
    DeleteDC(hMemDC);

    // return handle to the bitmap
    return hBitmap;
}

//
// blWnd	TRUE:window画像、FALSE:エリア画像
HBITMAP CopyWindowToBitmap(
HWND hWnd,		// windowハンドル、NULL:lpRectのエリア画像を使用
LPRECT lpRect, BYTE *pData, BITMAPINFO *pHeader,
BOOL blLogo)	// ロゴの有無
{
    HDC hWinDC = NULL, hMemDC = NULL;				// window DC and memory DC
    HBITMAP hBitmap = NULL, hOldBitmap = NULL;    // handles to deice-dependent bitmaps
	int iImageWidth = lpRect->right - lpRect->left;
	int iImageHeight = lpRect->bottom - lpRect->top;

	if (hWnd) {
		// get a device context for the window
		hWinDC = GetWindowDC(hWnd);
		if (hWinDC == NULL) return NULL;	// no window
	} else {
		hWinDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	}
	hMemDC = CreateCompatibleDC(hWinDC);

    // create a bitmap compatible with the screen DC
    hBitmap = CreateCompatibleBitmap(hWinDC, iImageWidth, iImageHeight);

    // select new bitmap into memory DC
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// bitblt the window to the bitmap
	if (hWnd) {
		// CAPTUREBLT:レイヤードウィンドウのコピーに必要
		BitBlt(hMemDC, 0, 0, iImageWidth, iImageHeight, hWinDC, 0, 0, SRCCOPY);
		//if (!PrintWindow(hWnd, hMemDC, 0))  return NULL;
	} else {
		BitBlt(hMemDC, 0, 0, iImageWidth, iImageHeight, hWinDC, lpRect->left, lpRect->top, SRCCOPY | CAPTUREBLT);
	}

	// Text impose
	if (blLogo) {
		AddText(hMemDC);
	}

    // select old bitmap back into memory DC and get handle to
    // bitmap of the screen   
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

	// Copy the bitmap data into the provided BYTE buffer
    GetDIBits(hWinDC, hBitmap, 0, iImageHeight, pData, pHeader, DIB_RGB_COLORS);

    // clean up
    DeleteDC(hWinDC);
    DeleteDC(hMemDC);

    // return handle to the bitmap
    return hBitmap;
}