/*
AVI�ϊ��N���X
���݂͕����C���X�^���X�ɑΉ��ł��Ȃ��istatic���g���Ă��邽�߁j
*/
#include "stdafx.h"
#include "Tapur.h"
#include "AviManager.h"
#include "FileSpec.h"


// �R�[���o�b�N�Ŏg�p���邽�߁A�N���X�����o�[�ɂ͂ł��Ȃ�
static int m_intPercent = 0;
static CProgressWnd *m_pWndProgressVfw = NULL;

// CopyAviFile()�̃R�[���o�b�N
LONG FAR PASCAL SaveCallback(int nPercent)
{
	// Report Progress
	if (m_intPercent != nPercent) {
		m_intPercent = nPercent;
		m_pWndProgressVfw->SetPos(nPercent);
		m_pWndProgressVfw->PeekAndPump();
	}
	if (m_pWndProgressVfw->Cancelled()) {
		return AVIERR_USERABORT;
	}

	return AVIERR_OK;
}


//
//  �@�\   : �o�^�ς݃t�B���^���X�g����t�B���^�����o
//  
//  �@�\���� : 
//  
//  �Ԃ�l : 
//  
//  ���l   : 
//  
HRESULT WINAPI CAviManager::FindEnumFilter(
const GUID CategoryCLSID,		// �t�B���^�̏����J�e�S�����w��
CString FilterName,				// �t�B���^��
IBaseFilter **pFilter)			// ���o���ꂽ�t�B���^
{  
	ICreateDevEnum *pSysDevEnum = NULL;  
	HRESULT hr;  
	IEnumMoniker *pEnumCat = NULL;  

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,  
				CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&pSysDevEnum);  
	if (FAILED(hr)) return hr;  

	// Enumerate all filters of the selected category  
	hr = pSysDevEnum->CreateClassEnumerator(CategoryCLSID, &pEnumCat, 0);  
	if (FAILED(hr) || pEnumCat == NULL)  
	{  
		SAFE_RELEASE(pSysDevEnum);  
		TRACE("No filter�I");  
		return S_FALSE;  
	}  

	// Enumerate all filters using the category enumerator  
	IMoniker *pMoniker = NULL;  
	ULONG cFetched = 0;  
	VARIANT varName = {0};  
	//int nFilters=0;  

	// Enumerate all items associated with the moniker  
	while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)  
	{  
		IPropertyBag *pPropBag;  

		// Associate moniker with a file  
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);  
		if (FAILED(hr)) continue;  

		// Read filter name from property bag  
		varName.vt = VT_BSTR;  
		hr = pPropBag->Read(_T("FriendlyName"), &varName, 0);  
		if (FAILED(hr)) continue;  

		CString str(varName.bstrVal);  
		SysFreeString(varName.bstrVal);  

		if(str == FilterName)  
		{  
			hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)pFilter);  
			break;  
		}  

		// Cleanup interfaces  
		SAFE_RELEASE(pPropBag);  

		// Intentionally DO NOT release the pMoniker, since it is  
		// stored in a listbox for later use  
	}  

	SAFE_RELEASE(pMoniker);  
	SAFE_RELEASE(pEnumCat);  
	SAFE_RELEASE(pSysDevEnum);  
	return hr;  
}  

//
//  �@�\   : �t�B���^�s�������o
//  
//  �@�\���� : 
//  
//  �Ԃ�l : 
//  
//  ���l   : 
//  
HRESULT __stdcall CAviManager::GetUnconnectedPin(  
IBaseFilter *pFilter,	// �t�B���^
PIN_DIRECTION PinDir,	// In or Out
IPin **ppPin,			// ���o���ꂽ�t�B���^
GUID mediaType,			// PIN�̃��f�B�A�^�C�v
int intCnt)				// ���Ԗڂ̃s�����H
{  
	IEnumPins *pEnumPins = NULL;  
	IPin *pPin = NULL;  
	IEnumMediaTypes *pEnumTypes = NULL;
	AM_MEDIA_TYPE *pType = NULL;
	HRESULT hr;
	
	hr = pFilter->EnumPins(&pEnumPins);  
	if(FAILED(hr)) return hr;  

	while(pEnumPins->Next(1, &pPin, NULL) == S_OK) {  
		PIN_DIRECTION ThisPinDir;  
		pPin->QueryDirection(&ThisPinDir);  
		if(ThisPinDir == PinDir) {
			// ���f�B�A�^�C�v�̃`�F�b�N
			if (mediaType != GUID_NULL) {
				BOOL blFind = FALSE;
				GUID mj = GUID_NULL;
				hr = pPin->EnumMediaTypes(&pEnumTypes);
				if(SUCCEEDED(hr)){
					while(pEnumTypes->Next(1, &pType, NULL) == S_OK) {
						mj = pType->majortype;
						if (pType->cbFormat != 0){
							CoTaskMemFree((PVOID)pType->pbFormat);
						}
						SAFE_RELEASE(pType->pUnk);
						CoTaskMemFree(pType);
						if(mj == mediaType) {
							blFind = TRUE;
							break;
						}
					}
				}
				SAFE_RELEASE(pEnumTypes);
				if(!blFind) goto LOOP_SECTION;
			}

			IPin *pTmp = NULL;  
			hr = pPin->ConnectedTo(&pTmp);  
			if(SUCCEEDED(hr)) {  
				SAFE_RELEASE(pTmp);  
			} else {  
				if (--intCnt < 1) {
					SAFE_RELEASE(pEnumPins);  
					*ppPin = pPin;
					return S_OK;
				}
			}  
		}

LOOP_SECTION:
		SAFE_RELEASE(pPin);  
	}  

	SAFE_RELEASE(pEnumPins);
	return E_FAIL;  
} 

//
//  �@�\   : �t�B���^��A������
//  
//  �@�\���� : 
//  
//  �Ԃ�l : 
//  
//  ���l   : 
//  
HRESULT __stdcall CAviManager::ConnectFilters(  
IGraphBuilder *pGraph,		// �O���t
IBaseFilter   *pSrc,		// Src�t�B���^ 
IBaseFilter   *pDest,		// Dest�t�B���^     
GUID mediaTypeSrc,			// SrcPIN�̃��f�B�A�^�C�v
GUID mediaTypeDest)			// DestPIN�̃��f�B�A�^�C�v
{  
	if((pGraph == NULL) || (pSrc == NULL) || ( pDest == NULL)) return E_POINTER;  

	IPin *pInPin = NULL;  
	IPin *pOutPin = NULL;  
	HRESULT hr = GetUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOutPin, mediaTypeSrc);  
	if(SUCCEEDED(hr)) { 
		HRESULT hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pInPin, mediaTypeDest);  
		if(FAILED(hr)) {
			SAFE_RELEASE(pOutPin);  
			return hr;  
		}

		hr = pGraph->Connect(pOutPin,pInPin);  
	}

	SAFE_RELEASE(pInPin);  
	SAFE_RELEASE(pOutPin);  
	return hr;  
}  

IMPLEMENT_DYNAMIC(CAviManager, CWnd)

// �R���X�g���N�^
CAviManager::CAviManager(CWnd *pParentWnd)
{
	/// �^�C�}�[������
	m_stApp.blOnTimer = FALSE;
	m_stApp.lngPassTime = 0;

	m_pWndProgress = NULL;
	m_pParentWnd = pParentWnd;

	m_pGraph = NULL;  
	m_pControl = NULL;
	m_pEvent = NULL;
	m_pEventSink = NULL;
	m_pAviSrcFilter = NULL;  
	m_pWavSrcFilter = NULL;  
	m_pMuxFilter = NULL;  
	m_pSplitFilter = NULL;
	m_pWriteFilter = NULL;
	m_pSrcAvi = NULL;
	m_pSrcWav = NULL;
	m_pSink = NULL;  
	m_pSeek = NULL;

	CoInitialize(NULL);  

	AVIFileInit();

	// �C�x���g���b�Z�[�W�����p�̔�\��WINDOW���`
	this->Create(NULL, _T("AviManager"), 0, CRect(0, 0, 0, 0), m_pParentWnd, 0);
}

// �f�X�g���N�^
CAviManager::~CAviManager()
{
	Release();

	CoUninitialize();  

	AVIFileExit();

	DestroyWindow();
}

BEGIN_MESSAGE_MAP(CAviManager, CWnd)
	ON_MESSAGE(WM_AVIMGR_NOTIFY, OnGraphNotify)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// ���\�[�X�����[�X
void CAviManager::Release()
{
	SAFE_RELEASE(m_pGraph);  
	SAFE_RELEASE(m_pControl);  
	SAFE_RELEASE(m_pEvent);  
	SAFE_RELEASE(m_pEventSink);  
	SAFE_RELEASE(m_pAviSrcFilter);  
	SAFE_RELEASE(m_pWavSrcFilter);  
	SAFE_RELEASE(m_pMuxFilter);  
	SAFE_RELEASE(m_pSplitFilter);  
	SAFE_RELEASE(m_pWriteFilter);  
	SAFE_RELEASE(m_pSrcAvi); 
	SAFE_RELEASE(m_pSrcWav); 
	SAFE_RELEASE(m_pSink); 
	SAFE_RELEASE(m_pSeek); 
}

//
//  �@�\     : �����f�[�^�̗L��
//  
//  �@�\���� : VFW�g�p�ADirectShow�ł͕s�H
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAviManager::ChkAudio(
CString strInFile) 		// ����AVI�t�@�C��
{
	HRESULT hr = NULL;
	BOOL blRet = FALSE;
	AVIFILEINFO fi;
	AVISTREAMINFO si;
	PAVIFILE pavi = NULL;
	PAVISTREAM pstm = NULL;

	TRY {
		if (AVIFileOpen(&pavi, strInFile, OF_READ | OF_SHARE_DENY_NONE, NULL) != 0)
		{  
			TRACE(_T("Can't open AVI File"));
			throw(_T("Can't open AVI File"));
		}  
		if (AVIFileInfo(pavi, &fi, sizeof(AVIFILEINFO)) != 0)
		{  
			TRACE(_T("Can't get AVI info"));
			throw(_T("Can't get AVI info"));
		}  
		for (DWORD dw = 0; dw < fi.dwStreams; dw++) {
			if (AVIFileGetStream(pavi, &pstm, 0, dw) != 0) {
				blRet = FALSE;
				break;
			}
			if (AVIStreamInfo(pstm, &si, sizeof(AVISTREAMINFO)) != 0) {
				blRet = FALSE;
				break;
			}			
			if (si.fccType == streamtypeAUDIO) {
				blRet = TRUE;
				break;
			}
			AVIStreamRelease(pstm);
			pstm = NULL;
		}
	}
    CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("ChkAudio error:%s\n"), szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		blRet = FALSE;
    }
	END_CATCH_ALL

	if (pstm) AVIStreamRelease(pstm);
	if (pavi) AVIFileRelease(pavi);

	return blRet;
}

//
//  �@�\     : WAV+AVI->AVI����
//  
//  �@�\���� : VFW�g�p�iMAX�PGB�j
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAviManager::CopyAviFileVfw(
CString strOutFile,		// �o��AVI�t�@�C��
CString strInFile, 		// ����AVI�t�@�C��
CString strWavFile) 	// ����WAV�t�@�C��
{
	BOOL blRet = TRUE;
	int i;
	AVICOMPRESSOPTIONS opt[2];

	TRY {
		for (i = 0; i < 2; i++) {
			m_pstm[i] = NULL;
			m_popt[i] = NULL;
		}

		/// ���̓X�g���[���I�[�v��
		m_pParentWnd->BeginWaitCursor();
		if (AVIStreamOpenFromFile(&m_pstm[0], strInFile, streamtypeVIDEO, 0,
											OF_READ | OF_SHARE_DENY_NONE,NULL) != 0) {
			blRet = FALSE;
			goto PROC_END;
		}
		if (AVIStreamOpenFromFile(&m_pstm[1], strWavFile, streamtypeAUDIO,0,
											OF_READ | OF_SHARE_DENY_NONE,NULL) != 0) {
			blRet = FALSE;
			goto PROC_END;
		}
		m_pParentWnd->EndWaitCursor();

		/// �ۑ��I�v�V�������
		for (i = 0; i < 2; i++) {
			m_popt[i] = &opt[i];
			memset(m_popt[i], 0, sizeof(AVICOMPRESSOPTIONS));
		}
		blRet = AVISaveOptions(m_pParentWnd->m_hWnd, ICMF_CHOOSE_DATARATE | ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_PREVIEW, 2, m_pstm, m_popt);
		if (!blRet) {
			goto PROC_END;
		}

		// �v���O���X���b�Z�[�W�쐬
		if (m_pWndProgressVfw == NULL) {
			m_pWndProgressVfw = new CProgressWnd(m_pParentWnd);
		}
		//m_pWndProgress->GoModal();
		CString strMsg;
		strMsg.LoadString(IDS_MSGBOX91);
		strMsg = _T("AVI") + strMsg;
		m_pWndProgress->SetText(strMsg);

		// ���������i���[�J�X���b�h���ł͎g�p�ł��Ȃ��H�j
		long lngRet = AVISaveV(strOutFile, NULL, (AVISAVECALLBACK)SaveCallback, 2, m_pstm, m_popt);
		if (lngRet == AVIERR_OK) {
			blRet = TRUE;
		} else {
			blRet = FALSE;
		}
	}
    CATCH_ALL(e)
    {
		blRet = FALSE;
    }
	END_CATCH_ALL

PROC_END:
	m_pWndProgressVfw->EndWaitCursor();

	// ���\�[�X���
	if (m_popt[0] != NULL) AVISaveOptionsFree(2, m_popt);
	if (m_pstm[0] != NULL) AVIStreamRelease(m_pstm[0]);
	if (m_pstm[1] != NULL) AVIStreamRelease(m_pstm[1]);

	// �v���O���X���b�Z�[�W���
	if (m_pWndProgressVfw != NULL) {
		if (!m_pWndProgressVfw->Cancelled()) {
			m_pWndProgressVfw->Hide();
		}
		delete m_pWndProgressVfw;
		m_pWndProgressVfw = NULL;
	}

	return blRet;
}

//
//  �@�\     : WAV+AVI->AVI����
//  
//  �@�\���� : DirectShow�g�p
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAviManager::CopyAviFileDs(
CString strOutFile,		// �o��AVI�t�@�C��
CString strInFile, 		// ����AVI�t�@�C��
CString strWavFile, 	// ����WAV�t�@�C��
BOOL blModal)			// Modal or not
{
	HRESULT hr = NULL;
	BOOL blRet = TRUE;

	m_strOutFile = strOutFile;
	m_strInFile = strInFile;
	m_strWavFile = strWavFile;

	TRY {
		// �O���t����
		hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,  
						IID_IGraphBuilder, (void **)&m_pGraph);  
		if (FAILED(hr)) return FALSE;  
	 
		// MediaControl interfaces �̎Q��
		m_pGraph->QueryInterface(IID_IMediaControl, (void **) &m_pControl);
		if(!m_pControl) return FALSE;

		// �C�x���g�C���^�t�F�[�X�̎Q��
		m_pGraph->QueryInterface(IID_IMediaEventEx, (LPVOID *)&m_pEvent);
		// �C�x���g�n���h���̓o�^
		HWND hWnd = GetSafeHwnd();
		m_pEvent->SetNotifyWindow((OAHWND)hWnd, WM_AVIMGR_NOTIFY, NULL);

		// �C�x���g�V���N�C���^�t�F�[�X�̎Q��
		m_pGraph->QueryInterface(IID_IMediaEventSink, (LPVOID *)&m_pEventSink);

		// WAV File Source���O���t�ɒǉ�
		m_pGraph->AddSourceFilter(strWavFile, _T("WAV Reader"), &m_pWavSrcFilter);
		if (m_pWavSrcFilter == NULL)  
		{  
			TRACE(_T("Can't init WAV Reader"));
			throw(_T("Can't init WAV Reader"));
		}  

		// AVI File Source���O���t�ɒǉ�
		// AddFilter�ł�PIN���擾�ł��Ȃ�
		m_pGraph->AddSourceFilter(strInFile, _T("AVI Reader"), &m_pAviSrcFilter);
		if (m_pAviSrcFilter == NULL)  
		{  
			TRACE(_T("Can't init AVI Reader"));
			throw(_T("Can't init AVI Reader"));
		}  

		//Avi Splitter Filter���O���t�ɒǉ�
		hr = CoCreateInstance(CLSID_AviSplitter, NULL, CLSCTX_INPROC_SERVER,  
				  IID_IBaseFilter, (void **) &m_pSplitFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("Can't init AVI Splitter Filter"));
			throw(_T("Can't init AVI Splitter Filter"));
		}  
		m_pGraph->AddFilter(m_pSplitFilter, _T("AVI Splitter Filter"));

		//AviMux Filter���O���t�ɒǉ�
		hr = CoCreateInstance(CLSID_AviDest, NULL, CLSCTX_INPROC_SERVER,  
				  IID_IBaseFilter, (void **) &m_pMuxFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("Can't init AVI Mux Filter"));
			throw(_T("Can't init AVI Mux Filter"));
		}  
		m_pGraph->AddFilter(m_pMuxFilter, _T("AVI Mux Filter"));

		// �����ς�SEEK�p
		hr = m_pMuxFilter->QueryInterface(IID_IMediaSeeking, (void**)&m_pSeek);
		if ( FAILED(hr) )  
		{  
			TRACE(_T("Can't init Mux MediaSeeking"));
			throw(_T("Can't init Mux MediaSeeking"));
		}  

		//FileWrite Filter���O���t�ɒǉ�
		hr = CoCreateInstance(CLSID_FileWriter, NULL, CLSCTX_INPROC_SERVER,  
				  IID_IBaseFilter, (void **) &m_pWriteFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("Init FileWriter failed!")); 
			throw(_T("Init FileWriter failed!")); 
		}  
		m_pGraph->AddFilter(m_pWriteFilter, _T("File Writer")); 

		// FileWrite Filter�ɏo�̓t�@�C������ݒ�
		hr = m_pWriteFilter->QueryInterface(IID_IFileSinkFilter,(void **)&m_pSink);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("Output AVI file fail!"));
			throw(_T("Output AVI file fail!"));
		}  
		m_pSink->SetFileName(strOutFile, NULL);  

		//Filter��A��
		hr = ConnectFilters(m_pGraph, m_pWavSrcFilter, m_pMuxFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("m_pWavSrcFilter->m_pMuxFilter fail!"));
			throw(_T("m_pWavSrcFilter->m_pMuxFilter fail!"));
		}  
		hr = ConnectFilters(m_pGraph, m_pAviSrcFilter, m_pSplitFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("m_pAviSrcFilter->m_pSplitFilter fail!"));
			throw(_T("m_pAviSrcFilter->m_pSplitFilter fail!"));
		}  
		hr = ConnectFilters(m_pGraph, m_pSplitFilter, m_pMuxFilter, MEDIATYPE_Video);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("m_pAviSrcFilter->m_pMuxFilter fail!"));
			throw(_T("m_pAviSrcFilter->m_pMuxFilter fail!"));
		}  
		hr = ConnectFilters(m_pGraph, m_pMuxFilter, m_pWriteFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("pMuxFilter->pWriteFilter fail!")); 
			throw(_T("pMuxFilter->pWriteFilter fail!")); 
		}  

		// ��������
		hr = m_pControl->Run();
		if ( FAILED(hr) )  
		{  
			TRACE(_T("m_pControl->Run fail!")); 
			throw(_T("m_pControl->Run fail!")); 
		}  

		// �v���O���X���b�Z�[�W�쐬
		if (m_pWndProgress == NULL) {
			m_pWndProgress = new CProgressWnd(m_pParentWnd);
		}
		CString strMsg;
		strMsg.LoadString(IDS_MSGBOX91);
		strMsg = _T("AVI") + strMsg;
		m_pWndProgress->SetText(strMsg);

		if (blModal) {
			// �v���O���X���[�v�i�P�b�P�ʁj
			REFERENCE_TIME rtTotal, rtNow;
			int nPercent;
			OAFilterState fs;
			while(1) {
				if (m_pControl == NULL) break;
				if (m_pSeek == NULL) break;
				if (m_pWndProgress == NULL) break;

				hr = m_pControl->GetState(100, &fs);
				if (hr != S_OK) goto CONTINUE_RUN;
				if (fs == State_Stopped) break;

				hr = m_pSeek->GetDuration(&rtTotal);
				hr = m_pSeek->GetCurrentPosition(&rtNow);
				if (SUCCEEDED(hr))
				{
					// Report Progress
					if (rtTotal <= rtNow) {
						break;
					} else {
						nPercent = (int)((double)rtNow / (double)rtTotal * 100.0);
						if (m_pWndProgress) {
							m_pWndProgress->SetPos(nPercent);
							m_pWndProgress->PeekAndPump();
						}
					}
					if (m_pWndProgress) {
						if (m_pWndProgress->Cancelled()) {
							blRet = FALSE;
							break;
						}
					}
				}
CONTINUE_RUN:
				Sleep(1000);
				AfxGetThread()->PumpMessage();
			}
		} else {
			// �v���O���X�p�^�C�}�[�J�n
			StartTimer();
		}
	}
    CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("CopyAviFileDs error:%s\n"), szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		blRet = FALSE;
    }
	END_CATCH_ALL

	return blRet;
}

// �C�x���g����������ƌĂяo�����
LRESULT CAviManager::OnGraphNotify(WPARAM wParam, LPARAM lParam)
{
	long evCode;
	LONG param1, param2;
	BOOL bComplete = false;

	// �C�x���g��S�Ď擾
	if (m_pEvent == NULL) return NOERROR;
	while (SUCCEEDED(m_pEvent->GetEvent(&evCode, &param1, &param2, 0))) {
		m_pEvent->FreeEventParams(evCode, param1, param2);
		switch (evCode) {
            case EC_USERABORT:
            case EC_ERRORABORT:
            case EC_COMPLETE:
                bComplete = true;
				break;
		}
	}

    if (bComplete)
    {
		m_pControl->Stop(); // �d�v! �O���t���~����K�v������!
		Release();			// ���\�[�X����i�t�@�C��������j
		
		StopTimer();	// �v���O���X�p�^�C�}�[��~

		// �v���O���X���b�Z�[�W���
		if (m_pWndProgress != NULL) {
			if (!m_pWndProgress->Cancelled()) {
				m_pWndProgress->Hide();
			}
			delete m_pWndProgress;
			m_pWndProgress = NULL;
		}

		// ������ɏo��AVI�����AVI�Ɉړ�
		if (m_blMove) {
			CFileSpec fs(m_strOutFile);
			BOOL blRet = fs.FileMove(m_strInFile);
		}
    }

	return NOERROR;
}

//
//  �@�\     : �v���O���X�p�^�C�}�J�n
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAviManager::StartTimer()
{
	if (!m_stApp.blOnTimer) {
		SetTimer(ID_TIMER_AVIMGR, 1000, NULL);	// 1 sec each
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("StartTimer:%s %s"), _T(POTM_APPNAME), _T(POTM_VERSION));
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, te.errstr);
	}
	m_stApp.blOnTimer = TRUE;		// �^�C�}�[���s���t���O
	m_stApp.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A

	return TRUE;
}

//
//  �@�\     : �v���O���X�p�^�C�}�I��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAviManager::StopTimer()
{
	if (m_stApp.blOnTimer) {
		KillTimer(ID_TIMER_AVIMGR);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopTimer"));
	}
	m_stApp.blOnTimer = FALSE;	// �^�C�}�[���s���t���O
	m_stApp.lngPassTime = 0;	// �^�C�}�[�o�ߎ��Ԃ��N���A

	return TRUE;
}

// �v���O���X�p�^�C�}����
void CAviManager::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == ID_TIMER_AVIMGR) {
		m_stApp.lngPassTime++;	// �^�C�}�[�o�ߎ��Ԃ��X�V

		REFERENCE_TIME rtTotal, rtNow;
		int nPercent;
		OAFilterState fs;
		HRESULT hr = m_pControl->GetState(100, &fs);
		if (hr != S_OK) {
			goto ONTIMER_END;
		}
		if (fs == State_Stopped) {
			goto ONTIMER_END;
		}

		hr = m_pSeek->GetDuration(&rtTotal);
		hr = m_pSeek->GetCurrentPosition(&rtNow);
		if (SUCCEEDED(hr))
		{
			// Report Progress
			if (rtTotal == rtNow) {
				goto ONTIMER_END;
			} else {
				nPercent = (int)((double)rtNow / (double)rtTotal * 100.0);
				m_pWndProgress->SetPos(nPercent);
				m_pWndProgress->PeekAndPump();
			}
			if (m_pWndProgress->Cancelled()) {
				goto ONTIMER_END;
			}
		}
	}
	
	goto ONTIMER_NEXT;
ONTIMER_END:
	StopTimer();	// �v���O���X�p�^�C�}�[��~
	// �v���O���X���b�Z�[�W���
	if (m_pWndProgress != NULL) {
		if (!m_pWndProgress->Cancelled()) {
			m_pWndProgress->Hide();
		}
		delete m_pWndProgress;
		m_pWndProgress = NULL;
	}
ONTIMER_NEXT:
	CWnd::OnTimer(nIDEvent);
}

