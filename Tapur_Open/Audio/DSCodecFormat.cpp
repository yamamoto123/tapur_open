// DSCodecFormat.cpp: implementation of the CDSCodecFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DSCodecFormat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDSCodecFormat::CDSCodecFormat()
{
	m_pMediaType = NULL;
}

CDSCodecFormat::~CDSCodecFormat()
{
	if (m_pMediaType != NULL) {
		// Free pMediaType
		if (m_pMediaType->cbFormat != 0) {
			CoTaskMemFree((PVOID)m_pMediaType->pbFormat);

			// Strictly unnecessary but tidier
			m_pMediaType->cbFormat = 0;
			m_pMediaType->pbFormat = NULL;
		}
		if (m_pMediaType->pUnk != NULL) {
			m_pMediaType->pUnk->Release();
			m_pMediaType->pUnk = NULL;
		}
		CoTaskMemFree((PVOID)m_pMediaType);
	}
}

WORD CDSCodecFormat::NumberOfChannels()
{
	if (m_pMediaType != NULL) {
		WAVEFORMATEX* pFormat = (WAVEFORMATEX*) m_pMediaType->pbFormat;
		return pFormat->nChannels;
	}
	return 0;
//	return -1;
}

DWORD CDSCodecFormat::SamplesPerSecond()
{
	if (m_pMediaType != NULL) {
		WAVEFORMATEX* pFormat = (WAVEFORMATEX*) m_pMediaType->pbFormat;
		return pFormat->nSamplesPerSec;
	}
	return 0;
//	return -1;
}

DWORD CDSCodecFormat::BytesPerSec()
{
	if (m_pMediaType != NULL) {
		WAVEFORMATEX* pFormat = (WAVEFORMATEX*) m_pMediaType->pbFormat;
		return pFormat->nAvgBytesPerSec;
	}
	return 0;
//	return -1;
}

WORD CDSCodecFormat::BitsPerSample()
{
	if (m_pMediaType != NULL) {
		WAVEFORMATEX* pFormat = (WAVEFORMATEX*) m_pMediaType->pbFormat;
		return pFormat->wBitsPerSample;
	}
	return 0;
//	return -1;
}
