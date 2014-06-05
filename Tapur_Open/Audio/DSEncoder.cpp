// DSEncoder.cpp: implementation of the CDSEncoder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DSEncoder.h"
#include "ProgressWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDSEncoder::CDSEncoder()
{
	m_pGraphBuilder = NULL;
	BuildCodecArray();
}

CDSEncoder::~CDSEncoder()
{
	// Creal the codec collection
	int nNumberOfCodec = GetSize();
	for (int i=0; i<nNumberOfCodec; i++) {
		delete GetAt(i);
	}
	RemoveAll();
}

void CDSEncoder::BuildCodecArray()
{
	HRESULT				hr;
	ICreateDevEnum*		pSysDevEnum = NULL;
	IEnumMoniker*		pEnum = NULL;
	IMoniker*			pMoniker = NULL;

	// System Device Enumerator
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**) &pSysDevEnum);
	if (FAILED(hr)) {
		// ERROR HERE
		return;
	}

	// Moniker Enumerator
	hr = pSysDevEnum->CreateClassEnumerator(CLSID_AudioCompressorCategory, &pEnum, 0);
	if (FAILED(hr)) {
		// ERROR HERE
		pSysDevEnum->Release();
		return;
	}

	// Cycle throught IMoniker collection
	while (pEnum->Next(1, &pMoniker, NULL) == S_OK) {
		// New instance of CDSCodec
		CDSCodec *pCodec = new CDSCodec();
		pCodec->m_pMoniker = pMoniker;

		// Retrieve codec name
		IPropertyBag *pPropertyBag = NULL;
		hr = pMoniker->BindToStorage(NULL, NULL, IID_IPropertyBag, (void**) &pPropertyBag);
		if (SUCCEEDED(hr)) {
			VARIANT var;
			VariantInit(&var);
			pPropertyBag->Read(L"FriendlyName", &var, NULL);
			CString szTempName(var.bstrVal);
			pCodec->m_szCodecName = szTempName;
			VariantClear(&var);
			// Add new instance to collection
			Add(pCodec);
			pCodec->BuildCodecFormatArray();
		} else {
			delete pCodec;
		}
	}

	// Lib駻ation des enumerators
	pEnum->Release();
	pSysDevEnum->Release();
}

HRESULT CDSEncoder::AddFilterByClsid(IGraphBuilder *pGraph, LPCWSTR wszName, const GUID& clsid, IBaseFilter **ppF)
{
    *ppF = NULL;
    HRESULT hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER,
       IID_IBaseFilter, (void**)ppF);
    if (SUCCEEDED(hr))
    {
        hr = pGraph->AddFilter((*ppF), wszName);
    }
    return hr;
}

BOOL CDSEncoder::SetFilterFormat(AM_MEDIA_TYPE* pStreamFormat, IBaseFilter* pBaseFilter)
{
	HRESULT hr;
	BOOL retVal = FALSE;

	// Pin enumeration
	IEnumPins* pEnumPins = NULL;
	hr = pBaseFilter->EnumPins(&pEnumPins);
	if (FAILED(hr)) {
		// ERROR HERE
		return FALSE;
	}

	IPin* pPin = NULL;
	while (pEnumPins->Next(1, &pPin, 0) == S_OK) {
		PIN_DIRECTION sDirection;
		pPin->QueryDirection(&sDirection);
		// Output Pin ?
		if (sDirection == PINDIR_OUTPUT) {
			IAMStreamConfig* pStreamConfig = NULL;
			hr = pPin->QueryInterface(IID_IAMStreamConfig, (void**) &pStreamConfig);
			if (SUCCEEDED(hr)) {
				hr = pStreamConfig->SetFormat(pStreamFormat);
				if (SUCCEEDED(hr)) {
					retVal = TRUE;
				}
				pStreamConfig->Release();
			}
		}
		pPin->Release();
	}

	// Free memory
	pEnumPins->Release();

	return retVal;
}

int CDSEncoder::BuildGraph(CWnd *hWin, CString szSrcFileName, CString szDestFileName, int nCodec, int nFormat)
{
	HRESULT hr;
	IBaseFilter *pParser = NULL, *pCodec = NULL, *pMux = NULL, *pDest = NULL;
	IFileSinkFilter* pSink = NULL;
//	IFileSourceFilter* pSourceFilter = NULL;
	int ret = -1;

	TRY {
		GUID CLSID_WavParser;
		UuidFromString(_T("3C78B8E2-6C4D-11D1-ADE2-0000F8754B99"), &CLSID_WavParser);
		GUID CLSID_WavDest;
		UuidFromString(_T("D51BD5A1-7548-11CF-A520-0080C77EF58A"), &CLSID_WavDest);

		// GraphBuilder construction
		hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**) &m_pGraphBuilder);
		if (SUCCEEDED(hr)) {
			// Parse filter
			hr = AddFilterByClsid(m_pGraphBuilder, L"Parser", CLSID_WavParser, &pParser);

			// ACM codec filter
			IMoniker* pMoniker = GetAt(nCodec)->m_pMoniker;;
			pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**) &pCodec);
			hr = m_pGraphBuilder->AddFilter(pCodec, L"ACM Codec");

			// Mux filter
			hr = AddFilterByClsid(m_pGraphBuilder, L"WavDest", CLSID_WavDest, &pMux);

			// Output file filter
			hr = AddFilterByClsid(m_pGraphBuilder, L"File Writer", CLSID_FileWriter, &pDest);
			pDest->QueryInterface(IID_IFileSinkFilter, (void**) &pSink);
			pSink->SetFileName(szDestFileName.AllocSysString(), NULL);
			pSink->Release();

			// Calculate output file size
			CFileStatus fileStatus;
			CFile::GetStatus(szSrcFileName, fileStatus);
			int nInputFileSize = (int)fileStatus.m_size;
			// Assuming 16kHz, 16bits, stereo
			int nMediaTime = (int) (nInputFileSize / (16000 * 2 * 2));
	//		int nMediaTime = (int) (nInputFileSize / (44000 * 2 * 2));
			WAVEFORMATEX *pWav = (WAVEFORMATEX *) GetAt(nCodec)->GetAt(nFormat)->m_pMediaType->pbFormat;
			int nOutputSize = nMediaTime * (pWav->nAvgBytesPerSec);

			// Check for output File
			try {
#ifdef _UNICODE
				if (_waccess(szDestFileName, 0) == 0) {
					CFile::Remove(szDestFileName);
				}
#else
				if (_access(szDestFileName, 0) == 0 {
					CFile::Remove(szDestFileName);
				}
#endif
			} catch(...) {
				// nothing to do
			}

			// Render Graph
			hr = m_pGraphBuilder->RenderFile(szSrcFileName.AllocSysString(), NULL);
			if (SUCCEEDED(hr)) {
				// Set Codec property
				hr = SetFilterFormat(GetAt(nCodec)->GetAt(nFormat)->m_pMediaType, pCodec);
				// Retrieve control interfaces
				IMediaControl* pMediaControl = NULL;
				hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void**) &pMediaControl);
				if (SUCCEEDED(hr)) {
					hr = pMediaControl->Run();
					// start encoding
					if (SUCCEEDED(hr)) {
						long nCode = 0;
						IMediaEvent *pMediaEvent = NULL;
						m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void**) &pMediaEvent);
//						int nPercentComplete = 0;

						/// プログレスメッセージ作成
						CString strMsg;
						CProgressWnd m_wndProgress;
						m_wndProgress.Create(hWin);
						m_wndProgress.GoModal();
						strMsg.LoadString(IDS_MSGBOX91);
						strMsg = _T("MP3") + strMsg;
						m_wndProgress.SetText(strMsg);

						// Wait until job complete
						while (nCode != EC_COMPLETE) {
							pMediaEvent->WaitForCompletion(1000, &nCode);
							// Report Progress
							if (m_wndProgress.Cancelled()) {
								break;
							}
							CFile::GetStatus(szDestFileName, fileStatus);
							int intPercent = (int)((fileStatus.m_size*100)/nOutputSize);
							CString szPercent;
							szPercent.Format(_T("%d%%"), intPercent);
							m_wndProgress.SetText(strMsg, szPercent);
							m_wndProgress.SetPos(intPercent);
							m_wndProgress.PeekAndPump();
						}

						pMediaControl->Stop();
						m_wndProgress.Hide();
						if (m_wndProgress.Cancelled()) {
							ret = 0;
						} else {
							ret = 1;
						}
						pMediaEvent->Release();
					} else {
						_TCHAR szError[1024];
						AMGetErrorText(hr, szError, 1024);
						CString szDesc(szError);
						AfxMessageBox(szDesc, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
						ret= -1;
					}

					pMediaControl->Release();
				}
			}

			// Free interfaces
			pCodec->Release();
			pParser->Release();
			pMux->Release();
			pDest->Release();

			m_pGraphBuilder->Release();
			m_pGraphBuilder = NULL;
		}
	}
    CATCH_ALL(e)
    {
		CString strErr;
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		strErr.Format(_T("File encoder error:%s\n"), szCause);
		AfxMessageBox(strErr);
		ret = -1;
    }
	END_CATCH_ALL

	return ret;
}
