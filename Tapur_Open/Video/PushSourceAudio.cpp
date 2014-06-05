//------------------------------------------------------------------------------
// File: PushSourceAudio.cpp
//
// Desc: DirectShow code - In-memory push mode wave pcm source filter
//       Provides PCM datas of the user's Audio as a continuously updating stream.
//
// Copyright (c) tapur.com.  All rights reserved.
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <initguid.h>	//DEFINE_GUIDを使用する場合は必須(リンクでエラーになる)
#include "PushSourceAudio.h"

// {46CBEB9B-6B22-4b2d-B682-D080DD375A5B}
DEFINE_GUID(CLSID_PushSourceAudio, 
0x46cbeb9b, 0x6b22, 0x4b2d, 0xb6, 0x82, 0xd0, 0x80, 0xdd, 0x37, 0x5a, 0x5b);

// Filter setup data
const AMOVIESETUP_MEDIATYPE sudOpPinTypes =
{
    &MEDIATYPE_Audio,       // Major type
    &MEDIASUBTYPE_PCM      // Minor type MEDIASUBTYPE_WAVEはだめ
};

const AMOVIESETUP_PIN sudOutputPinAudio = 
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

const AMOVIESETUP_FILTER sudPushSourceAudio =
{
    &CLSID_PushSourceAudio,	// Filter CLSID
    g_wszPushAudio,			// String name
    MERIT_DO_NOT_USE,       // Filter merit
    1,                      // Number pins
    &sudOutputPinAudio		// Pin details
};


// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance.
// We provide a set of filters in this one DLL.

//CFactoryTemplate g_Templates[3] = 
//{
//    { 
//      g_wszPushAudio,               // Name
//      &CLSID_PushSourceAudio,       // CLSID
//      CPushSourceAudio::CreateInstance, // Method to create an instance of MyComponent
//      NULL,                           // Initialization function
//      &sudPushSourceAudio           // Set-up information (for filters)
//    },
//};
//int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);    


/**********************************************
 *
 *  CPushPinAudio Class
 *  
 *
 **********************************************/

CPushPinAudio::CPushPinAudio(HRESULT *phr, CSource *pFilter)
        : CSourceStream(NAME("Push Source Audio"), phr, pFilter, L"Out"),
        m_FramesWritten(0),
        m_bZeroMemory(0),
		m_dwPrevTime(0)			// paak
{
	m_pListOfBuffer = NULL;
	m_rtSampleTime = 0;
	m_blPaused = FALSE;
	m_pFilter = pFilter;
}

CPushPinAudio::~CPushPinAudio()
{   
	DbgLog((LOG_TRACE, 3, TEXT("Audio written") ));
}

// ストリームのフォーマット指定
BOOL CPushPinAudio::SetFormat(LPWAVEFORMATEX pWf)
{
	if (pWf) {
		CopyMemory(&m_Wf, pWf, sizeof(WAVEFORMATEX));
	}

	return TRUE;
}

//
// GetMediaType
//
// Returns the supported media types in order of preferred  types (starting with iPosition=0)
//
HRESULT CPushPinAudio::GetMediaType(int iPosition, CMediaType *pmt)
{
    CheckPointer(pmt,E_POINTER);
    CAutoLock cAutoLock(m_pFilter->pStateLock());

    if(iPosition < 0)
        return E_INVALIDARG;

    // Do we have more items to offer
    if (iPosition > 0)
        return VFW_S_NO_MORE_ITEMS;

	//WAVEFORMATEX *pwfxin = (WAVEFORMATEX *)m_mt.pbFormat;

    WAVEFORMATEXTENSIBLE *pwfx = 
        (WAVEFORMATEXTENSIBLE *)pmt->AllocFormatBuffer(sizeof(WAVEFORMATEXTENSIBLE));

	pwfx->Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	pwfx->Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	pwfx->Format.nChannels = m_Wf.nChannels;
	pwfx->Format.nSamplesPerSec = m_Wf.nSamplesPerSec;
	pwfx->Format.wBitsPerSample = m_Wf.wBitsPerSample;
	pwfx->Format.nAvgBytesPerSec = pwfx->Format.nSamplesPerSec * pwfx->Format.wBitsPerSample * pwfx->Format.nChannels / 8;
	pwfx->Format.nBlockAlign = pwfx->Format.wBitsPerSample * pwfx->Format.nChannels / 8;
	pwfx->dwChannelMask = (1 << pwfx->Format.nChannels) - 1;
	pwfx->Samples.wValidBitsPerSample = pwfx->Format.wBitsPerSample;
	pwfx->SubFormat = MEDIASUBTYPE_PCM;

	pmt->SetFormat((BYTE*)pwfx, sizeof(WAVEFORMATEXTENSIBLE));

    pmt->SetType(&MEDIATYPE_Audio);
    pmt->SetFormatType(&FORMAT_WaveFormatEx);
    pmt->SetTemporalCompression(FALSE);

	// Work out the GUID for the subtype from the header info.
    GUID SubTypeGUID = MEDIASUBTYPE_PCM;
    pmt->SetSubtype(&SubTypeGUID);
    pmt->SetSampleSize(1);

    return NOERROR;

} // GetMediaType


//
// CheckMediaType
//
// We will accept 8, 16, 24 or 32 bit video formats, in any
// image size that gives room to bounce.
// Returns E_INVALIDARG if the mediatype is not acceptable
//
HRESULT CPushPinAudio::CheckMediaType(const CMediaType *pmt)
{
    CheckPointer(pmt,E_POINTER);

    if((*(pmt->Type()) != MEDIATYPE_Audio) ||   // we only output audio
        !(pmt->IsFixedSize()))                  // in fixed size samples
    {                                                  
        return E_INVALIDARG;
    }

    // Check for the subtypes we support
    const GUID *SubType = pmt->Subtype();
    if (SubType == NULL)
        return E_INVALIDARG;

    if(*SubType != MEDIASUBTYPE_PCM)
    {
        return E_INVALIDARG;
    }


	//if (pmt->FormatLength() != sizeof(WAVEFORMATEX)) // no compressed types allowed
	//{
	//	return E_INVALIDARG;
	//}
	//WAVEFORMATEX *pFormatIn = (WAVEFORMATEX *)pmt->Format();
	//if (pFormatIn->wFormatTag != WAVE_FORMAT_PCM)
	//{
	//	return E_INVALIDARG;
	//}

	CopyMediaType(&m_mt, pmt);

    return S_OK;  // This format is acceptable.

} // CheckMediaType


//
// DecideBufferSize
//
// This will always be called after the format has been sucessfully
// negotiated. So we have a look at m_mt to see what size image we agreed.
// Then we can ask for buffers of the correct size to contain them.
//
HRESULT CPushPinAudio::DecideBufferSize(IMemAllocator *pAlloc,
                                      ALLOCATOR_PROPERTIES *pProperties)
{
    CheckPointer(pAlloc,E_POINTER);
    CheckPointer(pProperties,E_POINTER);

    CAutoLock cAutoLock(m_pFilter->pStateLock());
    HRESULT hr = NOERROR;

	// get input dimensions
	WAVEFORMATEX *pwfx = (WAVEFORMATEX *)m_mt.Format();
    pProperties->cBuffers = 6;
	//int size = pwfx->nAvgBytesPerSec / 2;
	int size = pwfx->nAvgBytesPerSec;
    pProperties->cbBuffer = size; 
    ASSERT(pProperties->cbBuffer);

    // Ask the allocator to reserve us some sample memory, NOTE the function
    // can succeed (that is return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted

    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties,&Actual);
    if (FAILED(hr)) {
        return hr;
    }

    return NOERROR;

} // DecideBufferSize


//
// SetMediaType
//
// Called when a media type is agreed between filters
//
HRESULT CPushPinAudio::SetMediaType(const CMediaType *pmt)
{
    CAutoLock cAutoLock(m_pFilter->pStateLock());

    // Pass the call up to my base class
    HRESULT hr = CSourceStream::SetMediaType(pmt);

    if(SUCCEEDED(hr))
    {
		WAVEFORMATEX *pwfx = (WAVEFORMATEX *)m_mt.Format();
        if (pwfx == NULL)
            return E_UNEXPECTED;

        // Save the current media type
        m_MediaType = *pmt;
        hr = S_OK;
    } 

    return hr;
} // SetMediaType


// This is where we insert the DIB bits into the video stream.
// FillBuffer is called once for every sample in the stream.
HRESULT CPushPinAudio::FillBuffer(IMediaSample *pSample)
{
	BYTE *pData;
    long cbData;

	CheckPointer(pSample, E_POINTER);

    CAutoLock cAutoLockShared(&m_cSharedState);

    // Access the sample's data buffer
    pSample->GetPointer(&pData);
    cbData = pSample->GetSize();

    // Check that we're still using Audio
    ASSERT(m_mt.formattype == FORMAT_WaveFormatEx);

	WAVEFORMATEX *pwfx = (WAVEFORMATEX *)m_mt.Format();


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

	// 入力バッファ状態チェック
	if (m_pListOfBuffer == NULL)  {
		Sleep(10);  //CPU負荷減少
		goto NODATA_SECTION;
	}
	long lngCnt = (long)m_pListOfBuffer->GetCount();
	// 競合を防ぐため、バッファを１つ残して処理(バッファの生成待ち)
	if (lngCnt <= 1) {
		Sleep(10);  //CPU負荷減少
		goto NODATA_SECTION;
	}

	POSITION pos = m_pListOfBuffer->GetHeadPosition();
	if (pos == NULL) goto NODATA_SECTION;
	CWaveBuffer *pBuf = (CWaveBuffer *)m_pListOfBuffer->GetAt(pos);
	if (!pBuf) goto NODATA_SECTION;
	if (pBuf->m_blActive) goto NODATA_SECTION;		// バッファ書き込み中
	// 入力バッファをストリームバッファにコピー
	long lngSz = pBuf->GetNumSamples() * pBuf->GetSampleSize();		// 録音終了時では小さくなる
	if (!pBuf->m_blDead) {
		BYTE *pDat = (BYTE *)pBuf->GetBuffer();
		CopyMemory(pData, pBuf->GetBuffer(), lngSz);
	}
	// バッファの解放
	if (pBuf) {
		delete pBuf;
		pBuf = NULL;
	}
	m_pListOfBuffer->RemoveHead();

	// フレームの開始時間を計算
	DWORD dwFrame = lngSz * 1000 / pwfx->nAvgBytesPerSec;	// １バッファのミリ秒
	DWORD dwNow = timeGetTime();
	REFERENCE_TIME rtStart = m_rtSampleTime;
    REFERENCE_TIME rtStop  = rtStart + MILLISECONDS_TO_100NS_UNITS(dwFrame);
	m_rtSampleTime = rtStop;

    DbgLog((LOG_TRACE, 1, "rtStart:%ld", rtStart ));
    DbgLog((LOG_TRACE, 1, "rtStop:%ld", rtStop ));
TRACE(_T("audio rtStart:%ld\n"), rtStart);
TRACE(_T("audio rtStop:%ld\n"), rtStop);
	
	pSample->SetTime(&rtStart, &rtStop);
    pSample->SetDiscontinuity(FALSE);
    pSample->SetSyncPoint(TRUE);
    pSample->SetActualDataLength(lngSz);

	return S_OK;

NODATA_SECTION:
	REFERENCE_TIME rtTime = m_rtSampleTime;
	pSample->SetTime(&rtTime, &rtTime);
    pSample->SetDiscontinuity(FALSE);
    pSample->SetSyncPoint(FALSE);
    pSample->SetActualDataLength(0);
	return S_OK;
}


/**********************************************
 *
 *  CPushSourceAudio Class
 *
 **********************************************/

CPushSourceAudio::CPushSourceAudio(IUnknown *pUnk, HRESULT *phr)
           : CSource(NAME("PushSourceAudio"), pUnk, CLSID_PushSourceAudio)
{
    // The pin magically adds itself to our pin array.
    m_pPin = new CPushPinAudio(phr, this);

	if (phr)
	{
		if (m_pPin == NULL)
			*phr = E_OUTOFMEMORY;
		else
			*phr = S_OK;
	}  
}

CPushSourceAudio::~CPushSourceAudio()
{
    delete m_pPin;
}

// paak
BOOL CPushSourceAudio::SetFormat(LPWAVEFORMATEX pWf)
{
	if (m_pPin) {
		return m_pPin->SetFormat(pWf);
	}

	return FALSE;
}

void CPushSourceAudio::SetListBuffer(CPtrList *pList)
{
	if (m_pPin) {
		m_pPin->SetListBuffer(pList);
	}
}


CUnknown * WINAPI CPushSourceAudio::CreateInstance(IUnknown *pUnk, HRESULT *phr)
{
    CPushSourceAudio *pNewFilter = new CPushSourceAudio(pUnk, phr );

	if (phr)
	{
		if (pNewFilter == NULL) 
			*phr = E_OUTOFMEMORY;
		else
			*phr = S_OK;
	}
    return pNewFilter;

}

