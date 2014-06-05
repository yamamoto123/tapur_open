//------------------------------------------------------------------------------
// File: PushSource.H
//
// Desc: DirectShow code - In-memory push mode desktop capture source filter
//
// Copyright (c) tapur.com.  All rights reserved.
//------------------------------------------------------------------------------

#include <streams.h>

// UNITS = 10 ^ 7  
// UNITS / 30 = 30 fps;
// UNITS / 20 = 20 fps, etc
const REFERENCE_TIME FPS_30 = UNITS / 30;
const REFERENCE_TIME FPS_25 = UNITS / 25;
const REFERENCE_TIME FPS_20 = UNITS / 20;
const REFERENCE_TIME FPS_15 = UNITS / 15;
const REFERENCE_TIME FPS_10 = UNITS / 10;
const REFERENCE_TIME FPS_5  = UNITS / 5;
const REFERENCE_TIME FPS_4  = UNITS / 4;
const REFERENCE_TIME FPS_3  = UNITS / 3;
const REFERENCE_TIME FPS_2  = UNITS / 2;
const REFERENCE_TIME FPS_1  = UNITS / 1;

const REFERENCE_TIME rtDefaultFrameLength = FPS_5;

// Filter name strings
#define g_wszPushDesktop    L"Tapur Capture Filter"


/**********************************************
 *
 *  Class declarations
 *
 **********************************************/

class CPushPinDesktop : public CSourceStream
{
protected:

    int m_FramesWritten;				// To track where we are in the file
    BOOL m_bZeroMemory;                 // Do we need to clear the buffer?
    CRefTime m_rtSampleTime;	        // The time stamp for each sample

    int m_iFrameNumber;
	// paak
    //const REFERENCE_TIME m_rtFrameLength;
    REFERENCE_TIME m_rtFrameLength;
    DWORD m_dwPrevTime;					// 前回サンプリングしたシステム時刻（ミリ秒） 
	HWND m_hWndTarget;					// サンプリング対象ウィンドウ
	BOOL m_blWithLayer;					// レイヤーウィンドウ取得モード（CAPTUREBLTが効かない環境がある）
	BOOL m_blLogo;						// ロゴ表示の有無
	UINT m_uiStatus;					// 状態変数
										// 0:normal
										// 1:キャプチャ対象ウィンドウの消失
										// 2:キャプチャ対象ウィンドウのサイズ変更
										// 3:キャプチャ対象ウィンドウが非表示になった

    RECT m_rScreen;                     // Rect containing entire screen coordinates

    int m_iImageHeight;                 // The current image height
    int m_iImageWidth;                  // And current image width
    int m_iRepeatTime;                  // Time in msec between frames
    int m_nCurrentBitDepth;             // Screen bit depth

    CMediaType m_MediaType;
    CCritSec m_cSharedState;            // Protects our internal state
    CImageDisplay m_Display;            // Figures out our media type for us
	CSource *m_pFilter;					// CPushSourceDesktopフィルタ参照

public:

    CPushPinDesktop(HRESULT *phr, CSource *pFilter);
    ~CPushPinDesktop();

    // Override the version that offers exactly one media type
    HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pRequest);
    HRESULT FillBuffer(IMediaSample *pSample);
    
    // Set the agreed media type and set up the necessary parameters
    HRESULT SetMediaType(const CMediaType *pMediaType);

    // Support multiple display formats
    HRESULT CheckMediaType(const CMediaType *pMediaType);
    HRESULT GetMediaType(int iPosition, CMediaType *pmt);


    // Quality control
	// Not implemented because we aren't going in real time.
	// If the file-writing filter slows the graph down, we just do nothing, which means
	// wait until we're unblocked. No frames are ever dropped.
    STDMETHODIMP Notify(IBaseFilter *pSelf, Quality q)
    {
        return E_FAIL;
    }

	// paak
	BOOL m_blPaused;					// 一時停止フラグ
	BOOL SetCaptureTarget(HWND hWnd, int fps, BOOL blWithLayer, BOOL blLogo);
	UINT GetStatus() { return m_uiStatus; }
	void ResetStatus() { m_uiStatus = 0; }
};


class CPushSourceDesktop : public CSource
{

private:
    // Constructor is private because you have to use CreateInstance
    //CPushSourceDesktop(IUnknown *pUnk, HRESULT *phr);
    //~CPushSourceDesktop();

    CPushPinDesktop *m_pPin;

public:
    CPushSourceDesktop(IUnknown *pUnk, HRESULT *phr);
    ~CPushSourceDesktop();

	static CUnknown * WINAPI CreateInstance(IUnknown *pUnk, HRESULT *phr);  

	// paak
	BOOL SetCaptureTarget(HWND hWnd, int fps, BOOL blWithLayer, BOOL blLogo);
	UINT GetStatus() { return m_pPin->GetStatus(); }
	void ResetStatus() { m_pPin->ResetStatus(); }
	void PausePin() { m_pPin->m_blPaused = TRUE; }
	void ContinuePin() { m_pPin->m_blPaused = FALSE; }
	BOOL IsPausePin() { return m_pPin->m_blPaused; }
};


