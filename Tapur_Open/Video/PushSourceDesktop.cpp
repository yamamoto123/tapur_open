//------------------------------------------------------------------------------
// File: PushSourceDesktop.cpp
//
// Desc: DirectShow code - In-memory push mode desktop capture source filter
//       Provides an image of the user's desktop as a continuously updating stream.
//
// Copyright (c) tapur.com.  All rights reserved.
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <initguid.h>	//DEFINE_GUIDを使用する場合は必須(リンクでエラーになる)
#include "PushSourceDesktop.h"
#include "DibHelper.h"

// {851F34A0-7998-4031-AD41-120786239B71}
DEFINE_GUID(CLSID_PushSourceDesktop, 
0x851f34a0, 0x7998, 0x4031, 0xad, 0x41, 0x12, 0x7, 0x86, 0x23, 0x9b, 0x71);

// Filter setup data
const AMOVIESETUP_MEDIATYPE sudOpPinTypes =
{
    &MEDIATYPE_Video,       // Major type
    &MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN sudOutputPinDesktop = 
{
    L"Output",      // Obsolete, not used.
    FALSE,          // Is this pin rendered?
    TRUE,           // Is it an output pin?
    FALSE,          // Can the filter create zero instances?
    FALSE,          // Does the filter create multiple instances?
    &CLSID_NULL,    // Obsolete.
    NULL,           // Obsolete.
    1,              // Number of media types.
    &sudOpPinTypes  // Pointer to media types.
};

const AMOVIESETUP_FILTER sudPushSourceDesktop =
{
    &CLSID_PushSourceDesktop,// Filter CLSID
    g_wszPushDesktop,       // String name
    MERIT_DO_NOT_USE,       // Filter merit
    1,                      // Number pins
    &sudOutputPinDesktop    // Pin details
};


// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance.
// We provide a set of filters in this one DLL.
// baseclassとのリンク用ダミー定義
CFactoryTemplate g_Templates[3] = 
{
    { 
      g_wszPushDesktop,               // Name
      &CLSID_PushSourceDesktop,       // CLSID
      CPushSourceDesktop::CreateInstance, // Method to create an instance of MyComponent
      NULL,                           // Initialization function
      &sudPushSourceDesktop           // Set-up information (for filters)
    },
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);    


/**********************************************
 *
 *  CPushPinDesktop Class
 *  
 *
 **********************************************/

CPushPinDesktop::CPushPinDesktop(HRESULT *phr, CSource *pFilter)
        : CSourceStream(NAME("Push Source Desktop"), phr, pFilter, L"Out"),
        m_FramesWritten(0),
        m_bZeroMemory(0),
        m_iFrameNumber(0),
        m_rtFrameLength(FPS_5), // Capture and display desktop 5 times per second
        m_nCurrentBitDepth(32),
		m_dwPrevTime(0),		// paak
		m_hWndTarget(NULL),		// paak
		m_blWithLayer(FALSE),	// paak
		m_blLogo(FALSE),	// paak
		m_uiStatus(0)			// paak
{
	m_rtSampleTime = 0;
	m_blPaused = FALSE;
	m_pFilter = pFilter;
}

CPushPinDesktop::~CPushPinDesktop()
{   
    DbgLog((LOG_TRACE, 3, TEXT("Frames written %d"), m_iFrameNumber));
}

// paak
#define HIMETRIC_PER_INCH   2540
#define MAP_LOGHIM_TO_PIX(x,ppli)   ( ((ppli)*(x) + HIMETRIC_PER_INCH/2) / HIMETRIC_PER_INCH )

// paak
BOOL CPushPinDesktop::SetCaptureTarget(HWND hWnd, int fps, BOOL blWithLayer, BOOL blLogo)
{
	RECT rc;

	m_blWithLayer = blWithLayer;
	m_blLogo = blLogo;

	if( fps == 30 ){
		m_rtFrameLength = FPS_30;
	} else if( fps == 25 ){
		m_rtFrameLength = FPS_25;
	} else if( fps == 20 ){
		m_rtFrameLength = FPS_20;
	} else if( fps == 15 ){
		m_rtFrameLength = FPS_15;
	} else if( fps == 10 ){
		m_rtFrameLength = FPS_10;
	} else {
		m_rtFrameLength = FPS_5;
	}

	if (hWnd) {
		// window capture
		if (!::GetWindowRect(hWnd, &rc)) hWnd = NULL;
		if (hWnd) {
			// Save dimensions for later use in FillBuffer()
			m_hWndTarget = hWnd;
			m_iImageWidth  = rc.right - rc.left;
			m_iImageHeight = rc.bottom - rc.top;
		}
	}

	if (hWnd == NULL) {
		// scrren capture
		// Get the device context of the main display
		HDC hDC;
		hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);

		// Get the dimensions of the main desktop window
		m_rScreen.left   = m_rScreen.top = 0;
		m_rScreen.right  = GetDeviceCaps(hDC, HORZRES);
		m_rScreen.bottom = GetDeviceCaps(hDC, VERTRES);

		// Save dimensions for later use in FillBuffer()
		m_iImageWidth  = m_rScreen.right  - m_rScreen.left;
		m_iImageHeight = m_rScreen.bottom - m_rScreen.top;

		// Release the device context
		DeleteDC(hDC);
	}

	return TRUE;
}

//
// GetMediaType
//
// Prefer 5 formats - 8, 16 (*2), 24 or 32 bits per pixel
//
// Prefered types should be ordered by quality, with zero as highest quality.
// Therefore, iPosition =
//      0    Return a 32bit mediatype
//      1    Return a 24bit mediatype
//      2    Return 16bit RGB565
//      3    Return a 16bit mediatype (rgb555)
//      4    Return 8 bit palettised format
//      >4   Invalid
//
HRESULT CPushPinDesktop::GetMediaType(int iPosition, CMediaType *pmt)
{
    CheckPointer(pmt,E_POINTER);
    CAutoLock cAutoLock(m_pFilter->pStateLock());

    if(iPosition < 0)
        return E_INVALIDARG;

    // Have we run off the end of types?
    if(iPosition > 4)
        return VFW_S_NO_MORE_ITEMS;

    VIDEOINFO *pvi = (VIDEOINFO *) pmt->AllocFormatBuffer(sizeof(VIDEOINFO));
    if(NULL == pvi)
        return(E_OUTOFMEMORY);

    // Initialize the VideoInfo structure before configuring its members
    ZeroMemory(pvi, sizeof(VIDEOINFO));

    switch(iPosition)
    {
        case 0:
        {    
            // Return our highest quality 32bit format

            // Since we use RGB888 (the default for 32 bit), there is
            // no reason to use BI_BITFIELDS to specify the RGB
            // masks. Also, not everything supports BI_BITFIELDS
            pvi->bmiHeader.biCompression = BI_RGB;
            pvi->bmiHeader.biBitCount    = 32;
            break;
        }

        case 1:
        {   // Return our 24bit format
            pvi->bmiHeader.biCompression = BI_RGB;
            pvi->bmiHeader.biBitCount    = 24;
            break;
        }

        case 2:
        {       
            // 16 bit per pixel RGB565

            // Place the RGB masks as the first 3 doublewords in the palette area
            for(int i = 0; i < 3; i++)
                pvi->TrueColorInfo.dwBitMasks[i] = bits565[i];

            pvi->bmiHeader.biCompression = BI_BITFIELDS;
            pvi->bmiHeader.biBitCount    = 16;
            break;
        }

        case 3:
        {   // 16 bits per pixel RGB555

            // Place the RGB masks as the first 3 doublewords in the palette area
            for(int i = 0; i < 3; i++)
                pvi->TrueColorInfo.dwBitMasks[i] = bits555[i];

            pvi->bmiHeader.biCompression = BI_BITFIELDS;
            pvi->bmiHeader.biBitCount    = 16;
            break;
        }

        case 4:
        {   // 8 bit palettised

            pvi->bmiHeader.biCompression = BI_RGB;
            pvi->bmiHeader.biBitCount    = 8;
            pvi->bmiHeader.biClrUsed     = iPALETTE_COLORS;
            break;
        }
    }

    // Adjust the parameters common to all formats
    pvi->bmiHeader.biSize       = sizeof(BITMAPINFOHEADER);
    pvi->bmiHeader.biWidth      = m_iImageWidth;
    pvi->bmiHeader.biHeight     = m_iImageHeight;
    pvi->bmiHeader.biPlanes     = 1;
    pvi->bmiHeader.biSizeImage  = GetBitmapSize(&pvi->bmiHeader);
    pvi->bmiHeader.biClrImportant = 0;

    SetRectEmpty(&(pvi->rcSource)); // we want the whole image area rendered.
    SetRectEmpty(&(pvi->rcTarget)); // no particular destination rectangle

    pmt->SetType(&MEDIATYPE_Video);
    pmt->SetFormatType(&FORMAT_VideoInfo);
    pmt->SetTemporalCompression(FALSE);

    // Work out the GUID for the subtype from the header info.
    const GUID SubTypeGUID = GetBitmapSubtype(&pvi->bmiHeader);
    pmt->SetSubtype(&SubTypeGUID);
    pmt->SetSampleSize(pvi->bmiHeader.biSizeImage);

    return NOERROR;

} // GetMediaType


//
// CheckMediaType
//
// We will accept 8, 16, 24 or 32 bit video formats, in any
// image size that gives room to bounce.
// Returns E_INVALIDARG if the mediatype is not acceptable
//
HRESULT CPushPinDesktop::CheckMediaType(const CMediaType *pMediaType)
{
    CheckPointer(pMediaType,E_POINTER);

    if((*(pMediaType->Type()) != MEDIATYPE_Video) ||   // we only output video
        !(pMediaType->IsFixedSize()))                  // in fixed size samples
    {                                                  
        return E_INVALIDARG;
    }

    // Check for the subtypes we support
    const GUID *SubType = pMediaType->Subtype();
    if (SubType == NULL)
        return E_INVALIDARG;

    if(    (*SubType != MEDIASUBTYPE_RGB8)
        && (*SubType != MEDIASUBTYPE_RGB565)
        && (*SubType != MEDIASUBTYPE_RGB555)
        && (*SubType != MEDIASUBTYPE_RGB24)
        && (*SubType != MEDIASUBTYPE_RGB32))
    {
        return E_INVALIDARG;
    }

    // Get the format area of the media type
    VIDEOINFO *pvi = (VIDEOINFO *) pMediaType->Format();

    if(pvi == NULL)
        return E_INVALIDARG;

    // Check if the image width & height have changed
    if(    pvi->bmiHeader.biWidth   != m_iImageWidth || 
       abs(pvi->bmiHeader.biHeight) != m_iImageHeight)
    {
        // If the image width/height is changed, fail CheckMediaType() to force
        // the renderer to resize the image.
        return E_INVALIDARG;
    }

    // Don't accept formats with negative height, which would cause the desktop
    // image to be displayed upside down.
    if (pvi->bmiHeader.biHeight < 0)
        return E_INVALIDARG;

    return S_OK;  // This format is acceptable.

} // CheckMediaType


//
// DecideBufferSize
//
// This will always be called after the format has been sucessfully
// negotiated. So we have a look at m_mt to see what size image we agreed.
// Then we can ask for buffers of the correct size to contain them.
//
HRESULT CPushPinDesktop::DecideBufferSize(IMemAllocator *pAlloc,
                                      ALLOCATOR_PROPERTIES *pProperties)
{
    CheckPointer(pAlloc,E_POINTER);
    CheckPointer(pProperties,E_POINTER);

    CAutoLock cAutoLock(m_pFilter->pStateLock());
    HRESULT hr = NOERROR;

    VIDEOINFO *pvi = (VIDEOINFO *) m_mt.Format();
    pProperties->cBuffers = 1;
    pProperties->cbBuffer = pvi->bmiHeader.biSizeImage;

    ASSERT(pProperties->cbBuffer);

    // Ask the allocator to reserve us some sample memory. NOTE: the function
    // can succeed (return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted.
    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties,&Actual);
    if(FAILED(hr))
    {
        return hr;
    }

    // Is this allocator unsuitable?
    if(Actual.cbBuffer < pProperties->cbBuffer)
    {
        return E_FAIL;
    }

    // Make sure that we have only 1 buffer (we erase the ball in the
    // old buffer to save having to zero a 200k+ buffer every time
    // we draw a frame)
    ASSERT(Actual.cBuffers == 1);
    return NOERROR;

} // DecideBufferSize


//
// SetMediaType
//
// Called when a media type is agreed between filters
//
HRESULT CPushPinDesktop::SetMediaType(const CMediaType *pMediaType)
{
    CAutoLock cAutoLock(m_pFilter->pStateLock());

    // Pass the call up to my base class
    HRESULT hr = CSourceStream::SetMediaType(pMediaType);

    if(SUCCEEDED(hr))
    {
        VIDEOINFO * pvi = (VIDEOINFO *) m_mt.Format();
        if (pvi == NULL)
            return E_UNEXPECTED;

        switch(pvi->bmiHeader.biBitCount)
        {
            case 8:     // 8-bit palettized
            case 16:    // RGB565, RGB555
            case 24:    // RGB24
            case 32:    // RGB32
                // Save the current media type and bit depth
                m_MediaType = *pMediaType;
                m_nCurrentBitDepth = pvi->bmiHeader.biBitCount;
                hr = S_OK;
                break;

            default:
                // We should never agree any other media types
                ASSERT(FALSE);
                hr = E_INVALIDARG;
                break;
        }
    } 

    return hr;

} // SetMediaType


// This is where we insert the DIB bits into the video stream.
// FillBuffer is called once for every sample in the stream.
HRESULT CPushPinDesktop::FillBuffer(IMediaSample *pSample)
{
	BYTE *pData;
    long cbData;
	const DWORD DEFINE_PAUSE_TIME = 2000;	// 1000は低スペックPCではあり得る

	CheckPointer(pSample, E_POINTER);

    CAutoLock cAutoLockShared(&m_cSharedState);

	// 一時停止処理
	while(m_blPaused){
		Sleep(10);  //CPU負荷減少
		FILTER_STATE fs;
		HRESULT hr = m_pFilter->GetState(100, &fs);
		if (hr != S_OK) {
			break;
		}
		if (fs != State_Running) {
			break;
		}
	}

    // Access the sample's data buffer
    pSample->GetPointer(&pData);
    cbData = pSample->GetSize();

    // Check that we're still using video
    ASSERT(m_mt.formattype == FORMAT_VideoInfo);

    VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)m_mt.pbFormat;

	// Copy the DIB bits over into our filter's output buffer.
    // Since sample size may be larger than the image size, bound the copy size.
    int nSize = min(pVih->bmiHeader.biSizeImage, (DWORD) cbData);

    DbgLog((LOG_TRACE, 1, "|1:%d", pVih->bmiHeader.biSize  ));
    DbgLog((LOG_TRACE, 1, "|2:%d", pVih->bmiHeader.biWidth  ));
    DbgLog((LOG_TRACE, 1, "|3:%d", pVih->bmiHeader.biHeight  ));
    DbgLog((LOG_TRACE, 1, "|4:%d", pVih->bmiHeader.biPlanes  ));
    DbgLog((LOG_TRACE, 1, "|5:%d", pVih->bmiHeader.biBitCount  ));
    DbgLog((LOG_TRACE, 1, "|6:%d", pVih->bmiHeader.biCompression  ));

	HDIB hDib = NULL;
	RECT rc;
	if (m_hWndTarget) {
		// window check
		if (!::GetWindowRect(m_hWndTarget, &rc)) {
			// no window
			m_uiStatus = 1;
			goto ABORT_SECTION;
		}
		if (m_iImageWidth != (rc.right - rc.left) || m_iImageHeight != (rc.bottom - rc.top)) {
			// capture size changed
			m_uiStatus = 2;
			goto ABORT_SECTION;
		}
		if (!IsWindowVisible(m_hWndTarget)) {
			// window is invisible
			m_uiStatus = 3;
			// 処理は継続
		}
		// window capture
		if (m_blWithLayer) {
			hDib = CopyWindowToBitmap(NULL, &rc, pData, (BITMAPINFO *) &(pVih->bmiHeader), m_blLogo);
		} else {
			hDib = CopyWindowToBitmap(m_hWndTarget, &rc, pData, (BITMAPINFO *) &(pVih->bmiHeader), m_blLogo);
		}
		if (hDib == NULL) {
			// no window
			m_uiStatus = 1;
			goto ABORT_SECTION;
		}
	} else {
		// screen capture
		hDib = CopyScreenToBitmap(&m_rScreen, pData, (BITMAPINFO *) &(pVih->bmiHeader), m_blLogo);
	}
    if (hDib)
        DeleteObject(hDib);

	// Set the timestamps that will govern playback frame rate.
	// If this file is getting written out as an AVI,
	// then you'll also need to configure the AVI Mux filter to 
	// set the Average Time Per Frame for the AVI Header.
    // The current time is the sample's start.
    //REFERENCE_TIME rtStart = m_iFrameNumber * m_rtFrameLength;
    //REFERENCE_TIME rtStop  = rtStart + m_rtFrameLength;

// デバッグ用の意図的遅延
//Sleep(1000);

	// WAIT処理
	DWORD dwFrame = (DWORD)(m_rtFrameLength / 10000);
	DWORD dwNow = timeGetTime();
	if (m_dwPrevTime == 0) m_dwPrevTime = dwNow;
	DWORD dwSamp = dwNow - m_dwPrevTime;
	if (dwSamp < dwFrame) {
		// サンプリングが早い場合、待機する
		Sleep(dwFrame - dwSamp);
	} else {
		// サンプリングが遅い場合、CPUをフルに使っているので負荷分散する
		Sleep(10);
	}

	// フレームの開始時間を計算
	// 間に合わない場合はフレームをスキップして合わせる
	dwNow = timeGetTime();
	dwSamp = dwNow - m_dwPrevTime;
//TRACE(_T("dwSamp:%ld\n"), dwSamp);
	// サンプリング時間が長い場合は、一時停止後と推測される
	if (dwSamp > DEFINE_PAUSE_TIME) dwSamp = dwFrame;
	m_rtSampleTime += MILLISECONDS_TO_100NS_UNITS(dwSamp);
    REFERENCE_TIME rtStop  = m_rtSampleTime / m_rtFrameLength * m_rtFrameLength;	// フレーム単位
	REFERENCE_TIME rtStart = rtStop - m_rtFrameLength;
	//REFERENCE_TIME rtStart = m_rtSampleTime;
	//REFERENCE_TIME rtStop  = rtStart + MILLISECONDS_TO_100NS_UNITS(dwSamp);
	//REFERENCE_TIME rtStart = MILLISECONDS_TO_100NS_UNITS(m_dwPrevTime - m_dwStartTime) / m_rtFrameLength * m_rtFrameLength;
    //REFERENCE_TIME rtStop  = MILLISECONDS_TO_100NS_UNITS(dwNow - m_dwStartTime) / m_rtFrameLength * m_rtFrameLength;
	m_dwPrevTime = dwNow;

    DbgLog((LOG_TRACE, 1, "rtStart:%ld", rtStart ));
    DbgLog((LOG_TRACE, 1, "rtStop:%ld", rtStop ));
//TRACE(_T("rtStart:%ld\n"), rtStart);
//TRACE(_T("rtStop:%ld\n"), rtStop);

	pSample->SetTime(&rtStart, &rtStop);
    m_iFrameNumber++;

    pSample->SetSyncPoint(TRUE);	
	//pSample->SetDiscontinuity(TRUE);

    return S_OK;

ABORT_SECTION:
	m_pFilter->NotifyEvent(EC_USERABORT, 0, 0);
	return S_FALSE;

// 以下の方法だと静止画が連続してしまう 
//NODATA_SECTION:
//	REFERENCE_TIME rtTime = m_rtSampleTime;
//	pSample->SetTime(&rtTime, &rtTime);
//    pSample->SetDiscontinuity(FALSE);
//    pSample->SetSyncPoint(FALSE);
//    pSample->SetActualDataLength(0);
//	return S_OK;
}

/**********************************************
 *
 *  CPushSourceDesktop Class
 *
 **********************************************/

CPushSourceDesktop::CPushSourceDesktop(IUnknown *pUnk, HRESULT *phr)
           : CSource(NAME("PushSourceDesktop"), pUnk, CLSID_PushSourceDesktop)
{
    // The pin magically adds itself to our pin array.
    m_pPin = new CPushPinDesktop(phr, this);

	if (phr)
	{
		if (m_pPin == NULL)
			*phr = E_OUTOFMEMORY;
		else
			*phr = S_OK;
	}  
}

CPushSourceDesktop::~CPushSourceDesktop()
{
    delete m_pPin;
}

// paak
BOOL CPushSourceDesktop::SetCaptureTarget(HWND hWnd, int fps, BOOL blWithLayer, BOOL blLogo)
{
	if (m_pPin) {
		return m_pPin->SetCaptureTarget(hWnd, fps, blWithLayer, blLogo);
	}

	return FALSE;
}

CUnknown * WINAPI CPushSourceDesktop::CreateInstance(IUnknown *pUnk, HRESULT *phr)
{
    CPushSourceDesktop *pNewFilter = new CPushSourceDesktop(pUnk, phr );

	if (phr)
	{
		if (pNewFilter == NULL) 
			*phr = E_OUTOFMEMORY;
		else
			*phr = S_OK;
	}
    return pNewFilter;

}

