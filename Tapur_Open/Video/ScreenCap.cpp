/*
スクリーンキャプチャクラス
DirectShowを使用
*/

#include "stdafx.h"
#include "PushSourceDesktop.h"
#include "PushSourceAudio.h"
#include "ScreenCap.h"
#include "FileSpec.h"

// memory leak 検出
#ifdef	__AFX_H__    // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//
//  機能   : 登録済みフィルタリストからフィルタを検出
//  
//  機能説明 : 
//  
//  返り値 : 
//  
//  備考   : 
//  
HRESULT WINAPI CScreenCap::FindEnumFilter(
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

		if (str.Find(FilterName) >= 0) {
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
HRESULT __stdcall CScreenCap::GetUnconnectedPin(  
IBaseFilter *pFilter,	// フィルタ
PIN_DIRECTION PinDir,	// In or Out
IPin **ppPin)			// 検出されたフィルタ
{  
	IEnumPins *pEnum = NULL;  
	IPin *pPin = NULL;  
	HRESULT hr;
	
	hr = pFilter->EnumPins(&pEnum);  
	if(FAILED(hr)) return hr;  

	while(pEnum->Next(1, &pPin, NULL) == S_OK) {  
		PIN_DIRECTION ThisPinDir;  
		pPin->QueryDirection(&ThisPinDir);  
		if(ThisPinDir == PinDir) {  
			IPin *pTmp = NULL;  
			hr = pPin->ConnectedTo(&pTmp);  
			if(SUCCEEDED(hr)) {  
				SAFE_RELEASE(pTmp);  
			} else {  
				SAFE_RELEASE(pEnum);  
				*ppPin = pPin;  
				return S_OK;  
			}  
		}  
		SAFE_RELEASE(pPin);  
	}  

	SAFE_RELEASE(pEnum);
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
HRESULT __stdcall CScreenCap::ConnectFilters(  
IGraphBuilder *pGraph,		// グラフ
IBaseFilter   *pSrc,		// Srcフィルタ 
IBaseFilter   *pDest)		// Destフィルタ     
{  
	if((pGraph == NULL) || (pSrc == NULL) || ( pDest == NULL)) return E_POINTER;  

	IPin *pInPin = NULL;  
	IPin *pOutPin = NULL;  
	HRESULT hr = GetUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOutPin);  
	if(SUCCEEDED(hr)) { 
		HRESULT hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pInPin);  
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

IMPLEMENT_DYNAMIC(CScreenCap, CWnd)

// コンストラクタ
CScreenCap::CScreenCap(CWnd *pParentWnd)  
{  
	/// タイマー初期化
	m_stApp.blOnTimer = FALSE;
	m_stApp.lngPassTime = 0;

	m_ullLimitSz = 0; 

	m_pGraph = NULL;  
	m_pBuilder = NULL;  
	m_pControl = NULL;
	m_pEvent = NULL;
	m_pEventSink = NULL;
	m_pSrcFilter = NULL;  
	m_pWavFilter = NULL;  
	m_pCmpFilter = NULL;  
	m_pRawFilter = NULL;  
	m_pColFilter = NULL;  
	m_pMuxFilter = NULL;  
	m_pWriteFilter = NULL;  
	m_pSink = NULL;  

	CoInitialize(NULL);  

	// イベントメッセージ処理用の非表示WINDOWを定義
	this->Create(NULL, _T("ScreenCap"), 0, CRect(0, 0, 0, 0), pParentWnd, 0);
}  

 //	デストラクタ
 CScreenCap::~CScreenCap()  
{  
	StopRec();

	CoUninitialize();  

	DestroyWindow();
}  

BEGIN_MESSAGE_MAP(CScreenCap, CWnd)
	ON_MESSAGE(WM_SCREENCAP_NOTIFY, OnGraphNotify)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//
//  機能   : 圧縮フィルタの一覧取得
//  
//  機能説明 : 
//  
//  返り値 : 
//  
//  備考   : 
//  
void CScreenCap::ListCompressFilter(CStringList *pStrList)  
{  
	ICreateDevEnum *pSysDevEnum = NULL;  
	HRESULT hr;  
	IEnumMoniker *pEnumCat = NULL;  

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,  
			CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&pSysDevEnum);  
	if (FAILED(hr)) return;  

	// Enumerate all filters of the selected category  
	// CDEF_DEVMON_CMGR_DEVICE:圧縮マネージャonly
	hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoCompressorCategory, &pEnumCat, CDEF_DEVMON_CMGR_DEVICE);  
	if (FAILED(hr) || pEnumCat == NULL)  
	{  
		SAFE_RELEASE(pSysDevEnum);  
		TRACE("No Compress filter！");  
		return;  
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

		LPTSTR str(varName.bstrVal);  
		SysFreeString(varName.bstrVal);  

		// 動作しないフィルタの排除
		CString strOrg = str;
		strOrg = strOrg.MakeUpper();
		if (strOrg.Find(_T("INDEO")) >= 0) goto LOOP_SECTION;
		if (strOrg.Find(_T("INTEL IYUV")) >= 0) goto LOOP_SECTION;
		if (strOrg.Find(_T("MICROSOFT H.261")) >= 0) goto LOOP_SECTION;
		if (strOrg.Find(_T("MICROSOFT H.263")) >= 0) goto LOOP_SECTION;
		if (strOrg.Find(_T("DECODE")) >= 0) goto LOOP_SECTION;
		if (strOrg.Find(_T("UT VIDEO CODEC YUV420")) >= 0) goto LOOP_SECTION;

		// リストに追加
		if (strOrg.Find(_T("LAGARITH")) >= 0 || strOrg.Find(_T("UT VIDEO CODEC")) >= 0) {
			// 優先的なコーデックを上位に
			pStrList->AddHead(str);  
		} else {
			pStrList->AddTail(str); 
		}

LOOP_SECTION:
		// Cleanup interfaces  
		SAFE_RELEASE(pPropBag);  

		// Intentionally DO NOT release the pMoniker, since it is  
		// stored in a listbox for later use  
	}

	SAFE_RELEASE(pMoniker);  
	SAFE_RELEASE(pEnumCat);  
	SAFE_RELEASE(pSysDevEnum);

	return;  
}  

//
//  機能   : グラフの生成
//  
//  機能説明 : 
//  
//  返り値 : 
//  
//  備考   : 
//  
void CScreenCap::BuilderGraph(
HWND hwnd,				// キャプチャ対象ウィンドウ、NULLの場合スクリーン全体
int fps,				// FPS
CString CompressName,	// 圧縮フィルタ名称、NULLの場合非圧縮
LPWAVEFORMATEX pWf,		// 音質（NULLの場合、音なし）
CString SavePath,		// 出力AVIファイル名 
BOOL blWithLayer,		// オーバーレイ録画
BOOL blLogo)			// ロゴ表示
{  
	HRESULT hr = NULL;
	m_strOutFile = SavePath;

	// 既存の出力AVIファイル削除
	if (_taccess_s(SavePath, 0) == 0) {
		DeleteFile(SavePath);
	}

	// グラフ生成
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,  
					IID_IGraphBuilder, (void **)&m_pGraph);  
	if (FAILED(hr)) return;  
 
    // Capture Graph Builder を作成する。
    hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL,
        CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, 
        (void **)&m_pBuilder);
    if (SUCCEEDED(hr))
    {
        m_pBuilder->SetFiltergraph(m_pGraph);
    }

	// MediaControl interfaces の参照
	m_pGraph->QueryInterface(IID_IMediaControl, (void **) &m_pControl);

	// イベントインタフェースの参照
	m_pGraph->QueryInterface(IID_IMediaEventEx, (LPVOID *)&m_pEvent);
	// イベントハンドルの登録
	HWND hWnd = GetSafeHwnd();
	m_pEvent->SetNotifyWindow((OAHWND)hWnd, WM_SCREENCAP_NOTIFY, NULL);

	// イベントシンクインタフェースの参照
	m_pGraph->QueryInterface(IID_IMediaEventSink, (LPVOID *)&m_pEventSink);

	// PushSource Desktop Filterをグラフに追加
	m_pSrcFilter = new CPushSourceDesktop(NULL, &hr);
	//hr = CoCreateInstance(CLSID_PushSourceDesktop, NULL, CLSCTX_INPROC_SERVER,  
	//		  IID_IBaseFilter, (void **) &pSrcFilter);  
	if ( FAILED(hr) )  
	{  
		TRACE(_T("No Capture Filter"));
		return;  
	}
	m_pGraph->AddFilter(m_pSrcFilter,_T("Tapur Capture Filter"));

	// PushSource Audio Filterをグラフに追加
	if (pWf) {
		m_pWavFilter = new CPushSourceAudio(NULL, &hr);
		if ( FAILED(hr) )  
		{  
			TRACE(_T("No Audio Filter"));
			return;  
		}
		m_pGraph->AddFilter(m_pWavFilter,_T("Tapur Audio Filter"));
	}

	//Compress Filterをグラフに追加
	if (CompressName.IsEmpty()) {
		// 圧縮コーデックを使用しない場合はロゴを表示しない
		blLogo = FALSE;
	} else {
		hr = FindEnumFilter(CLSID_VideoCompressorCategory, CompressName, &m_pCmpFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("Can't init Compress Filter"));
			return;  
		}  
		m_pGraph->AddFilter(m_pCmpFilter, _T("Compress Filter"));

		// ffdshow用
		CString strOrg = CompressName.MakeUpper();
		if (strOrg.Find(_T("FFDSHOW")) >= 0) {
			hr = FindEnumFilter(CLSID_LegacyAmFilterCategory, _T("ffdshow raw video filter"), &m_pRawFilter);  
			if ( FAILED(hr) )  
			{  
				TRACE(_T("Can't init ffdshow raw Filter"));
				return;  
			}  
			m_pGraph->AddFilter(m_pRawFilter, _T("ffdshow raw Filter"));
		}
		// MICROSOFT RLE用
		if (strOrg.Find(_T("MICROSOFT RLE")) >= 0) {
			hr = CoCreateInstance(CLSID_Colour, NULL, CLSCTX_INPROC_SERVER,  
					  IID_IBaseFilter, (void **) &m_pColFilter);  
			if ( FAILED(hr) )  
			{  
				TRACE(_T("Can't init Color Space Converter"));
				return;  
			}  
			m_pGraph->AddFilter(m_pColFilter, _T("Color Space Converter"));
		}
	}

	//AviMux Filterをグラフに追加
	hr = CoCreateInstance(CLSID_AviDest, NULL, CLSCTX_INPROC_SERVER,  
			  IID_IBaseFilter, (void **) &m_pMuxFilter);  
	if ( FAILED(hr) )  
	{  
	  TRACE(_T("Can't init AVI Mux Filter"));
	  return;  
	}  
	m_pGraph->AddFilter(m_pMuxFilter, _T("AVI Mux Filter"));

	//FileWrite Filterをグラフに追加
	hr = CoCreateInstance(CLSID_FileWriter, NULL, CLSCTX_INPROC_SERVER,  
			  IID_IBaseFilter, (void **) &m_pWriteFilter);  
	if ( FAILED(hr) )  
	{  
	  TRACE(_T("Init FileWriter failed!")); 
	  return;  
	}  
	m_pGraph->AddFilter(m_pWriteFilter, _T("File Writer")); 


	//FileWrite Filterに出力ファイル名を設定
	hr = m_pWriteFilter->QueryInterface(IID_IFileSinkFilter,(void **)&m_pSink);  
	if ( FAILED(hr) )  
	{  
	  TRACE(_T("Connect Sink fail!"));
	  return;  
	}  
	m_pSink->SetFileName(SavePath,NULL);  

	// Filter設定
	CPushSourceDesktop *pSrc = (CPushSourceDesktop *)m_pSrcFilter;
	pSrc->ResetStatus();
	pSrc->SetCaptureTarget(hwnd, fps, blWithLayer, blLogo);
	pSrc = NULL;
	if (pWf) {
		CPushSourceAudio *pWav = (CPushSourceAudio *)m_pWavFilter;
		pWav->SetFormat(pWf);
		pWav = NULL;
	}

	//Filterを連結
	if (pWf) {
		hr = ConnectFilters(m_pGraph, m_pWavFilter, m_pMuxFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("pSrcFilter->pWAvFilter fail!"));
			return;  
		}  
	}
	if (CompressName.IsEmpty()) {
		hr = ConnectFilters(m_pGraph, m_pSrcFilter, m_pMuxFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("pSrcFilter->pCmpFilter fail!"));
			return;  
		}  
	} else {
		if (m_pRawFilter) {
			// ffdshow
			hr = ConnectFilters(m_pGraph, m_pSrcFilter, m_pRawFilter);  
			if ( FAILED(hr) )  
			{  
				TRACE(_T("pSrcFilter->pCmpFilter fail!"));
				return;  
			}  
			hr = ConnectFilters(m_pGraph, m_pRawFilter, m_pCmpFilter);  
			if ( FAILED(hr) )  
			{  
				TRACE(_T("pSrcFilter->pRawFilter fail!"));
				return;  
			}  
		} else if (m_pColFilter) {
			// MS RLE
			hr = ConnectFilters(m_pGraph, m_pSrcFilter, m_pColFilter);  
			if ( FAILED(hr) )  
			{  
				TRACE(_T("pSrcFilter->pCmpFilter fail!"));
				return;  
			}  
			hr = ConnectFilters(m_pGraph, m_pColFilter, m_pCmpFilter);  
			if ( FAILED(hr) )  
			{  
				TRACE(_T("pSrcFilter->pRawFilter fail!"));
				return;  
			}  
		} else {
			hr = ConnectFilters(m_pGraph, m_pSrcFilter, m_pCmpFilter);  
			if ( FAILED(hr) )  
			{  
				TRACE(_T("pSrcFilter->pCmpFilter fail!"));
				return;  
			} 
		}
		hr = ConnectFilters(m_pGraph, m_pCmpFilter, m_pMuxFilter);  
		if ( FAILED(hr) )  
		{  
			TRACE(_T("pCmpFilter->pMuxFilter fail!"));
			return;  
		}  
	}
	hr = ConnectFilters(m_pGraph, m_pMuxFilter, m_pWriteFilter);  
	if ( FAILED(hr) )  
	{  
		TRACE(_T("pMuxFilter->pWriteFilter fail!")); 
		return;  
	}  
}  

//
//  機能   : 録画開始
//  
//  機能説明 : 
//  
//  返り値 : 
//  
//  備考   : 
//  
BOOL CScreenCap::StartRec()  
{  
	if(!m_pControl) return FALSE;
	
	m_pControl->Run();
			
	// タイマー開始
	StartTimer();

	return TRUE;
}  

//
//  機能   : 録画終了
//  
//  機能説明 : 
//  
//  返り値 : 
//  
//  備考   : 
//  
BOOL CScreenCap::StopRec()  
{  
	if(!m_pControl) return FALSE;
	
	m_pControl->Stop();
	
	StopTimer();	// タイマー停止

	SAFE_RELEASE(m_pGraph);  
	SAFE_RELEASE(m_pBuilder);  
	SAFE_RELEASE(m_pControl);  
	SAFE_RELEASE(m_pEvent);  
	SAFE_RELEASE(m_pEventSink);  
	//SAFE_RELEASE(m_pSrcFilter);  例外になる
	//SAFE_RELEASE(m_pWavFilter);  
	SAFE_RELEASE(m_pCmpFilter);  
	SAFE_RELEASE(m_pRawFilter);  
	SAFE_RELEASE(m_pColFilter);  
	SAFE_RELEASE(m_pMuxFilter);  
	SAFE_RELEASE(m_pWriteFilter);  
	SAFE_RELEASE(m_pSink); 

	return TRUE;
}

//
//  機能   : 録画一時停止
//  
//  機能説明 : 音声フィルタの停止はACM側で調整されるので不要
//  
//  返り値 : 
//  
//  備考   : m_pControlで停止しないので独自実装（フラグのみ変わるが不安定）
//  
BOOL CScreenCap::PauseRec()
{
	//HRESULT hr;

	//if(!m_pControl) return FALSE;
	//if(!m_pBuilder) return FALSE;

	//REFERENCE_TIME pstart = 0;
 //   REFERENCE_TIME pstop = 0;
 //   hr = m_pBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, NULL,
 //       &pstart, &pstop, 0, 0); 	
	//if(FAILED(hr)) return FALSE;

	//hr = m_pControl->Pause();	
	//if(FAILED(hr)) return FALSE;

	if(!m_pSrcFilter) return FALSE;
	CPushSourceAudio *pWav = (CPushSourceAudio *)m_pWavFilter;
	pWav->PausePin();
	pWav = NULL;
	CPushSourceDesktop *pSrc = (CPushSourceDesktop *)m_pSrcFilter;
	pSrc->PausePin();
	pSrc = NULL;

	return TRUE;
}

//
//  機能   : 録画一時停止再開
//  
//  機能説明 : 音声フィルタの停止はACM側で調整されるので不要
//  
//  返り値 : 
//  
//  備考   : m_pControlで停止しないので独自実装
//  
BOOL CScreenCap::ResumeRec()
{
	//HRESULT hr;

	//if(!m_pControl) return FALSE;
	//if(!m_pBuilder) return FALSE;

	//REFERENCE_TIME pstart = 0;
 //   REFERENCE_TIME pstop = MAXLONGLONG;
 //   hr = m_pBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, NULL,
 //       &pstart, &pstop, 0, 0); 
	//if(FAILED(hr)) return FALSE;

	//hr = m_pControl->Run();
	//if(FAILED(hr)) return FALSE;

	if(!m_pSrcFilter) return FALSE;
	CPushSourceAudio *pWav = (CPushSourceAudio *)m_pWavFilter;
	pWav->ContinuePin();
	pWav = NULL;
	CPushSourceDesktop *pSrc = (CPushSourceDesktop *)m_pSrcFilter;
	pSrc->ContinuePin();
	pSrc = NULL;

	return TRUE;
}

//
//  機能   : 録画中か否か
//  
//  機能説明 : 
//  
//  返り値 : 
//  
//  備考   : 
//  
BOOL CScreenCap::IsRecording()
{
	if(!m_pControl) return FALSE;
	
	OAFilterState fs;
	m_pControl->GetState(100, &fs);
	if (fs != State_Running && fs != State_Paused) return FALSE;
	return TRUE;
}

//
//  機能   : 録画停止中か否か
//  
//  機能説明 : 
//  
//  返り値 : 
//  
//  備考   : 
//  
BOOL CScreenCap::IsRecordPause()
{
	//if(!m_pControl) return FALSE;
	//
	//OAFilterState fs;
	//m_pControl->GetState(100, &fs);
	//if (fs != State_Paused) return FALSE;

	CPushSourceAudio *pWav = (CPushSourceAudio *)m_pWavFilter;
	BOOL blWav = pWav->IsPausePin();
	pWav = NULL;
	CPushSourceDesktop *pSrc = (CPushSourceDesktop *)m_pSrcFilter;
	BOOL blSrc = pSrc->IsPausePin();
	pSrc = NULL;

	return (blWav && blSrc);
}

//
//  機能   : 音声ストリーム用リストバッファの結合
//  
//  機能説明 : 内部のバッファを外部のポインタに割り当てる
//  
//  返り値 : 
//  
//  備考   : 
//  
BOOL CScreenCap::BindAudioBuffer(CPtrList *pList)
{
	if (!m_pWavFilter) return FALSE;

	CPushSourceAudio *pWav = (CPushSourceAudio *)m_pWavFilter;
	pWav->SetListBuffer(pList);
	pWav = NULL;
	return TRUE;
}


// イベントが発生すると呼び出される
LRESULT CScreenCap::OnGraphNotify(WPARAM wParam, LPARAM lParam)
{
	long evCode;
	LONG param1, param2;
	BOOL bComplete = false;
	UINT uiStatus = 0;

	// イベントを全て取得
	if (m_pEvent == NULL) return NOERROR;
	while (SUCCEEDED(m_pEvent->GetEvent(&evCode, &param1, &param2, 0))) {
		m_pEvent->FreeEventParams(evCode, param1, param2);
		switch (evCode) {
            case EC_USERABORT:		
				// 終了理由
				CPushSourceDesktop *pSrc = (CPushSourceDesktop *)m_pSrcFilter;
				uiStatus = pSrc->GetStatus();
				pSrc = NULL;
				// uiStatus=0:ファイルサイズ制限

				bComplete = true;

				// メインフレームへのイベント通知
				AfxGetMainWnd()->PostMessage(WM_SCREENCAP_NOTIFY, uiStatus, evCode);
				break;
    //        case EC_ERRORABORT:		// Pauseでもコールされる
    //        case EC_COMPLETE:		// Resumeでもコールされる
				//// 終了理由
				//CPushSourceDesktop *pSrc = (CPushSourceDesktop *)m_pSrcFilter;
				//uiStatus = pSrc->GetStatus();
				//pSrc = NULL;
				//if (uiStatus == 0) {	// Pause or Resume
				//	break;
				//}
    //            
				//bComplete = true;

				//// メインフレームへのイベント通知
				//AfxGetMainWnd()->PostMessage(WM_SCREENCAP_NOTIFY, uiStatus, evCode);
				//break;
		}
	}

    if (bComplete)
    {
		StopRec(); // 重要! グラフを停止する必要がある!
    }

	return NOERROR;
}


//
//  機能     : タイマ開始
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CScreenCap::StartTimer()
{
	if (!m_stApp.blOnTimer) {
		SetTimer(ID_TIMER_SCREENCAP, 1000, NULL);	// 1 sec each
	}
	m_stApp.blOnTimer = TRUE;		// タイマー実行中フラグ
	m_stApp.lngPassTime = 0;		// タイマー経過時間をクリア

	return TRUE;
}

//
//  機能     : タイマ終了
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CScreenCap::StopTimer()
{
	if (m_stApp.blOnTimer) {
		KillTimer(ID_TIMER_SCREENCAP);
	}
	m_stApp.blOnTimer = FALSE;	// タイマー実行中フラグ
	m_stApp.lngPassTime = 0;	// タイマー経過時間をクリア

	return TRUE;
}

// タイマ処理
void CScreenCap::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == ID_TIMER_SCREENCAP) {
		m_stApp.lngPassTime++;	// タイマー経過時間を更新

		// ファイルサイズチェック
		if (m_ullLimitSz <= 0) return;
		CFileSpec fs(m_strOutFile);
		if (!fs.Exist()) return;
		if (fs.FileSize() > m_ullLimitSz) {
			// 録音終了イベント通知
			m_pEventSink->Notify(EC_USERABORT, 0, 0);
		}
	}

	CWnd::OnTimer(nIDEvent);
}
