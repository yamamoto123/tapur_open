// DSCodec.cpp: implementation of the CDSCodec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DSCodec.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDSCodec::CDSCodec()
{
	m_pMoniker = NULL;
	m_szCodecName.Empty();
}

CDSCodec::~CDSCodec()
{
	int nNumberOfFormat = GetSize();
	for (int i=0; i<nNumberOfFormat; i++) {
		delete GetAt(i);
	}
	RemoveAll();

	if (m_pMoniker != NULL) {
		m_pMoniker->Release();
		m_pMoniker = NULL;
	}
}

void CDSCodec::BuildCodecFormatArray()
{
	if (m_pMoniker == NULL) return;

	HRESULT			hr;
	IBaseFilter		*pBaseFilter = NULL;

	// Retrieve the IBaseFilter
	hr = m_pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**) &pBaseFilter);
	if (FAILED(hr)) {
		// ERROR HERE
		return;
	}

	// Enumerate Pin
	IEnumPins	*pEnumPins = NULL;
	hr = pBaseFilter->EnumPins(&pEnumPins);
	if (FAILED(hr)) {
		// ERROR HERE
		pBaseFilter->Release();
		return;
	}

	// Find the output Pin
	IPin	* pPin = NULL;
	while (pEnumPins->Next(1, &pPin, 0) == S_OK) {
		PIN_DIRECTION direction;
		pPin->QueryDirection(&direction);
		if (direction == PINDIR_OUTPUT) {
			// Retrieve the IAMStreamConfig
			IAMStreamConfig	*pStreamConfig = NULL;
			hr = pPin->QueryInterface(IID_IAMStreamConfig, (void**) &pStreamConfig);
			if (SUCCEEDED(hr)) {
				int nCount = 0, nSize = 0;
				pStreamConfig->GetNumberOfCapabilities(&nCount, &nSize);
				for (int i=0; i<nCount; i++) {
					AM_MEDIA_TYPE* pMediaType = NULL;
					AUDIO_STREAM_CONFIG_CAPS confCaps;
					hr = pStreamConfig->GetStreamCaps(i, &pMediaType, (BYTE*)&confCaps);
					if (SUCCEEDED(hr)) {
						CDSCodecFormat *pCodecFormat = new CDSCodecFormat();
						pCodecFormat->m_pMediaType = pMediaType;
						Add(pCodecFormat);
					}
				}
				pStreamConfig->Release();
			}
		}
		pPin->Release();
	}

	pEnumPins->Release();
	pBaseFilter->Release();
}
