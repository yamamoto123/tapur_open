/*
AVI変換クラス
現在は複数インスタンスに対応できない（staticを使っているため）
*/
#include "stdafx.h"
#include "Tapur.h"
#include "AviManager.h"
#include "FileSpec.h"


// コールバックで使用するため、クラスメンバーにはできない
static int m_intPercent = 0;
static CProgressWnd *m_pWndProgressVfw = NULL;

// CopyAviFile()のコールバック
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
//  機能   : 登録済みフィルタリストからフィルタを検出
//  
//  機能説明 : 
//  
//  返り値 : 
//  
//  備考   : 
//  
HRESULT WINAPI CAviManager::FindEnumFilter(
const GUID CategoryCLSID,		// フィルタの所属カテゴリを指定
CString FilterName,				// フィルタ名
IBaseFilter **pFilter)			// 検出されたフィルタ
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
		TRACE("No filter！");  
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
//  機能   : フィルタピンを検出
//  
//  機能説明 : 
//  
//  返り値 : 
//  
//  備考   : 
//  
HRESULT __stdcall CAviManager::GetUnconnectedPin(  
IBaseFilter *pFilter,	// フィルタ
PIN_DIRECTION PinDir,	// In or Out
IPin **ppPin,			// 検出されたフィルタ
GUID mediaType,			// PINのメディアタイプ
int intCnt)				// 何番目のピンか？
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
			// メディアタイプのチェック
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
//  機能   : フィルタを連結する
//  
//  機能説明 : 
//  
//  返り値 : 
//  
//  備考   : 
//  
HRESULT __stdcall CAviManager::ConnectFilters(  
IGraphBuilder *pGraph,		// グラフ
IBaseFilter   *pSrc,		// Srcフィルタ 
IBaseFilter   *pDest,		// Destフィルタ     
GUID mediaTypeSrc,			// SrcPINのメディアタイプ
GUID mediaTypeDest)			// DestPINのメディアタイプ
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

// コンストラクタ
CAviManager::CAviManager(CWnd *pParentWnd)
{
	/// タイマー初期化
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

	// イベントメッセージ処理用の非表示WINDOWを定義
	this->Create(NULL, _T("AviManager"), 0, CRect(0, 0, 0, 0), m_pParentWnd, 0);
}

// デストラクタ
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

// リソースリリース
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
//  機能     : 音声データの有無
//  
//  機能説明 : VFW使用、DirectShowでは不可？
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAviManager::ChkAudio(
CString strInFile) 		// 入力AVIファイル
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
//  機能     : WAV+AVI->AVI合成
//  
//  機能説明 : VFW使用（MAX１GB）
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAviManager::CopyAviFileVfw(
CString strOutFile,		// 出力AVIファイル
CString strInFile, 		// 入力AVIファイル
CString strWavFile) 	// 入力WAVファイル
{
	BOOL blRet = TRUE;
	int i;
	AVICOMPRESSOPTIONS opt[2];

	TRY {
		for (i = 0; i < 2; i++) {
			m_pstm[i] = NULL;
			m_popt[i] = NULL;
		}

		/// 入力ストリームオープン
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

		/// 保存オプション画面
		for (i = 0; i < 2; i++) {
			m_popt[i] = &opt[i];
			memset(m_popt[i], 0, sizeof(AVICOMPRESSOPTIONS));
		}
		blRet = AVISaveOptions(m_pParentWnd->m_hWnd, ICMF_CHOOSE_DATARATE | ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_PREVIEW, 2, m_pstm, m_popt);
		if (!blRet) {
			goto PROC_END;
		}

		// プログレスメッセージ作成
		if (m_pWndProgressVfw == NULL) {
			m_pWndProgressVfw = new CProgressWnd(m_pParentWnd);
		}
		//m_pWndProgress->GoModal();
		CString strMsg;
		strMsg.LoadString(IDS_MSGBOX91);
		strMsg = _T("AVI") + strMsg;
		m_pWndProgress->SetText(strMsg);

		// 合成処理（ワーカスレッド内では使用できない？）
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

	// リソース解放
	if (m_popt[0] != NULL) AVISaveOptionsFree(2, m_popt);
	if (m_pstm[0] != NULL) AVIStreamRelease(m_pstm[0]);
	if (m_pstm[1] != NULL) AVIStreamRelease(m_pstm[1]);

	// プログレスメッセージ解放
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
//  機能     : WAV+AVI->AVI合成
//  
//  機能説明 : DirectShow使用
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAviManager::CopyAviFileDs(
CString strOutFile,		// 出力AVIファイル
CString strInFile, 		// 入力AVIファイル
CString strWavFile, 	// 入力WAVファイル
BOOL blModal)			// Modal or not
{
	HRESULT hr = NULL;
	BOOL blRet = TRUE;

	m_strOutFile = strOutFile;
	m_strInFile = strInFile;
	m_strWavFile = strWavFile;

	TRY {
		// グラフ生成
		hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,  
						IID_IGraphBuilder, (void **)&m_pGraph);  
		if (FAILED(hr)) return FALSE;  
	 
		// MediaControl interfaces の参照
		m_pGraph->QueryInterface(IID_IMediaControl, (void **) &m_pControl);
		if(!m_pControl) return FALSE;

		// イベントインタフェースの参照
		m_pGraph->QueryInterface(IID_IMediaEventEx, (LPVOID *)&m_pEvent);
		// イベントハンドルの登録
		HWND hWnd = GetSafeHwnd();
		m_pEvent->SetNotifyWindow((OAHWND)hWnd, WM_AVIMGR_NOTIFY, NULL);

		// イベントシンクインタフェースの参照
		m_pGraph->QueryInterface(IID_IMediaEventSink, (LPVOID *)&m_pEventSink);

		// WAV File Sourceをグラフに追加
		m_pGraph->AddSourceFilter(strWavFile, _T("WAV Reader"), &m_pWavSrcFilter);
		if (m_pWavSrcFilter == NULL)  
		{  
			TRACE(_T("Can't init WAV Reader"));
			throw(_T("Can't init WAV Reader"));
		}  

		// AVI File Sourceをグラフに追加
		// AddFilterではPINが取得できない
		m_pGraph->AddSourceFilter(strInFile, _T("AVI Reader"), &m_pAviSrcFilter);
		if (m_pAviSrcFilter == NULL)  
		{  
			TRACE(_T("Can't init AVI Reader"));
			throw(_T("Can't init AVI Reader"));
		}  

		//Avi Splitter Filterをグラフに追加
		hr = CoCreateInstance(CLSID_AviSplitter, NULL, CLSCTX_INPROC_SERVER,  
				  IID_IBaseFilter, (void **) &m_pSplitFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("Can't init AVI Splitter Filter"));
			throw(_T("Can't init AVI Splitter Filter"));
		}  
		m_pGraph->AddFilter(m_pSplitFilter, _T("AVI Splitter Filter"));

		//AviMux Filterをグラフに追加
		hr = CoCreateInstance(CLSID_AviDest, NULL, CLSCTX_INPROC_SERVER,  
				  IID_IBaseFilter, (void **) &m_pMuxFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("Can't init AVI Mux Filter"));
			throw(_T("Can't init AVI Mux Filter"));
		}  
		m_pGraph->AddFilter(m_pMuxFilter, _T("AVI Mux Filter"));

		// 処理済みSEEK用
		hr = m_pMuxFilter->QueryInterface(IID_IMediaSeeking, (void**)&m_pSeek);
		if ( FAILED(hr) )  
		{  
			TRACE(_T("Can't init Mux MediaSeeking"));
			throw(_T("Can't init Mux MediaSeeking"));
		}  

		//FileWrite Filterをグラフに追加
		hr = CoCreateInstance(CLSID_FileWriter, NULL, CLSCTX_INPROC_SERVER,  
				  IID_IBaseFilter, (void **) &m_pWriteFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("Init FileWriter failed!")); 
			throw(_T("Init FileWriter failed!")); 
		}  
		m_pGraph->AddFilter(m_pWriteFilter, _T("File Writer")); 

		// FileWrite Filterに出力ファイル名を設定
		hr = m_pWriteFilter->QueryInterface(IID_IFileSinkFilter,(void **)&m_pSink);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("Output AVI file fail!"));
			throw(_T("Output AVI file fail!"));
		}  
		m_pSink->SetFileName(strOutFile, NULL);  

		//Filterを連結
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

		// 合成処理
		hr = m_pControl->Run();
		if ( FAILED(hr) )  
		{  
			TRACE(_T("m_pControl->Run fail!")); 
			throw(_T("m_pControl->Run fail!")); 
		}  

		// プログレスメッセージ作成
		if (m_pWndProgress == NULL) {
			m_pWndProgress = new CProgressWnd(m_pParentWnd);
		}
		CString strMsg;
		strMsg.LoadString(IDS_MSGBOX91);
		strMsg = _T("AVI") + strMsg;
		m_pWndProgress->SetText(strMsg);

		if (blModal) {
			// プログレスループ（１秒単位）
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
			// プログレス用タイマー開始
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

// イベントが発生すると呼び出される
LRESULT CAviManager::OnGraphNotify(WPARAM wParam, LPARAM lParam)
{
	long evCode;
	LONG param1, param2;
	BOOL bComplete = false;

	// イベントを全て取得
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
		m_pControl->Stop(); // 重要! グラフを停止する必要がある!
		Release();			// リソース解放（ファイルを解放）
		
		StopTimer();	// プログレス用タイマー停止

		// プログレスメッセージ解放
		if (m_pWndProgress != NULL) {
			if (!m_pWndProgress->Cancelled()) {
				m_pWndProgress->Hide();
			}
			delete m_pWndProgress;
			m_pWndProgress = NULL;
		}

		// 処理後に出力AVIを入力AVIに移動
		if (m_blMove) {
			CFileSpec fs(m_strOutFile);
			BOOL blRet = fs.FileMove(m_strInFile);
		}
    }

	return NOERROR;
}

//
//  機能     : プログレス用タイマ開始
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAviManager::StartTimer()
{
	if (!m_stApp.blOnTimer) {
		SetTimer(ID_TIMER_AVIMGR, 1000, NULL);	// 1 sec each
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("StartTimer:%s %s"), _T(POTM_APPNAME), _T(POTM_VERSION));
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, te.errstr);
	}
	m_stApp.blOnTimer = TRUE;		// タイマー実行中フラグ
	m_stApp.lngPassTime = 0;		// タイマー経過時間をクリア

	return TRUE;
}

//
//  機能     : プログレス用タイマ終了
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAviManager::StopTimer()
{
	if (m_stApp.blOnTimer) {
		KillTimer(ID_TIMER_AVIMGR);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopTimer"));
	}
	m_stApp.blOnTimer = FALSE;	// タイマー実行中フラグ
	m_stApp.lngPassTime = 0;	// タイマー経過時間をクリア

	return TRUE;
}

// プログレス用タイマ処理
void CAviManager::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == ID_TIMER_AVIMGR) {
		m_stApp.lngPassTime++;	// タイマー経過時間を更新

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
	StopTimer();	// プログレス用タイマー停止
	// プログレスメッセージ解放
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

