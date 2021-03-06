/*
スクリーンキャプチャクラス
DirectShowを使用
*/

#pragma once

#include <dshow.h>

#define SAFE_RELEASE(x)		{ if (x) x->Release(); x = NULL; }  
#define HELPER_RELEASE(x)	{ if (x) x->Release(); x = NULL; }  
#define SELF_RELEASE(x)		{ if (x) x->Release(); delete x; x = NULL;}  

#define	WM_SCREENCAP_NOTIFY		(WM_APP + 30)

class CScreenCap : public CWnd
{
	DECLARE_DYNAMIC(CScreenCap)

private:
	IGraphBuilder *m_pGraph;		// グラフ  
	ICaptureGraphBuilder2 *m_pBuilder;	// Capture Graph Builder
	IBaseFilter *m_pSrcFilter;		// pushsource desktop filter  
	IBaseFilter *m_pWavFilter;		// pushsource audio filter  
	IBaseFilter *m_pCmpFilter;		// compress filter  
	IBaseFilter *m_pRawFilter;		// ffdshow raw filter  
	IBaseFilter *m_pColFilter;		// Color Space Converter  
	IBaseFilter *m_pMuxFilter;		// avi mux filter  
	IBaseFilter *m_pWriteFilter;	// file write Filter  
	IFileSinkFilter *m_pSink;		// WriteFilter用出力ファイル定義
	IMediaControl *m_pControl;		// Media Controller  
	IMediaEventEx *m_pEvent;		// Media Eventer
	IMediaEventSink *m_pEventSink;	// Media Event Sink

	HRESULT WINAPI FindEnumFilter(const GUID CategoryCLSID, CString FilterName, IBaseFilter **pFilter);
	HRESULT __stdcall GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir,	IPin **ppPin);
	HRESULT __stdcall ConnectFilters(IGraphBuilder *pGraph,	IBaseFilter *pSrc, IBaseFilter *pDest);
	
	CString m_strOutFile;			// 出力AVIファイル
	ULONGLONG m_ullLimitSz;			// 出力制限値（byte）
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT nIDEvent);
	virtual LRESULT OnGraphNotify(WPARAM wParam, LPARAM lParam);

public:  
	CScreenCap(CWnd *pParentWnd);
	virtual ~CScreenCap();

	ST_TIMER m_stApp;		// プログレス用タイマ
	BOOL StartTimer();		// プログレス用タイマ開始
	BOOL StopTimer();		// プログレス用タイマ終了

	void ListCompressFilter(CStringList *pStrList);  
	void BuilderGraph(HWND hwnd, int fps, CString CompressName, LPWAVEFORMATEX pWf, CString SavePath, BOOL blWithLayer, BOOL blLogo = FALSE);  
	BOOL StartRec();  
	BOOL StopRec();
	BOOL PauseRec();
	BOOL ResumeRec();
	BOOL IsRecording();
	BOOL IsRecordPause();
	BOOL BindAudioBuffer(CPtrList *pList);
	void SetLimitSize(ULONGLONG ullSz) { m_ullLimitSz = ullSz; }
};