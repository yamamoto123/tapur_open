//------------------------------------------------------------------------------
// File: PushSource.H
//
// Desc: DirectShow code - In-memory push mode wave pcm source filter
//
// Copyright (c) tapur.com.  All rights reserved.
//------------------------------------------------------------------------------

#include <streams.h>
#include "WaveBuffer.h"

// Filter name strings
#define g_wszPushAudio    L"Tapur Push Audio Filter"


/**********************************************
 *
 *  Class declarations
 *
 **********************************************/

class CPushPinAudio : public CSourceStream
{
protected:

    int m_FramesWritten;				// To track where we are in the file
    BOOL m_bZeroMemory;                 // Do we need to clear the buffer?
    CRefTime m_rtSampleTime;	        // The time stamp for each sample

	// paak
    DWORD m_dwPrevTime;					// �O��T���v�����O�����V�X�e�������i�~���b�j 
	CPtrList *m_pListOfBuffer;			// ���͉����o�b�t�@�̎Q�Ɓi���̂͊O���j
	WAVEFORMATEX m_Wf;					// �����t�H�[�}�b�g

    CMediaType m_MediaType;
    CCritSec m_cSharedState;            // Protects our internal state
    CImageDisplay m_Display;            // Figures out our media type for us
	CSource *m_pFilter;					// CPushSourcAudio�t�B���^�Q��

	void FreeListOfBuffer();						// ���͉����o�b�t�@�̉��
	CWaveBuffer* GetBuffer();						// �擪�f�[�^�o�b�t�@�̎Q��
	bool DelBuffer();								// �擪�f�[�^�o�b�t�@�̍폜

public:

    CPushPinAudio(HRESULT *phr, CSource *pFilter);
    ~CPushPinAudio();

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
	BOOL m_blPaused;					// �ꎞ��~�t���O
	BOOL SetFormat(LPWAVEFORMATEX pWf);		// �t�H�[�}�b�g�ݒ�
	void SetListBuffer(CPtrList *pList) { m_pListOfBuffer = pList; }	// �o�b�t�@�Q��
};


class CPushSourceAudio : public CSource
{

private:
    // Constructor is private because you have to use CreateInstance
    //CPushSourceAudio(IUnknown *pUnk, HRESULT *phr);
    //~CPushSourceAudio();

    CPushPinAudio *m_pPin;

public:
    CPushSourceAudio(IUnknown *pUnk, HRESULT *phr);
    ~CPushSourceAudio();

	static CUnknown * WINAPI CreateInstance(IUnknown *pUnk, HRESULT *phr);  

	// paak
	BOOL SetFormat(LPWAVEFORMATEX pWf);
	void SetListBuffer(CPtrList *pList);
	void PausePin() { m_pPin->m_blPaused = TRUE; }
	void ContinuePin() { m_pPin->m_blPaused = FALSE; }
	BOOL IsPausePin() { return m_pPin->m_blPaused; }
};


