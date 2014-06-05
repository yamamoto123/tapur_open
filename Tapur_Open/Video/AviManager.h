// AviManager.h : �w�b�_�[ �t�@�C��
//

#if !defined(AVIMANAGER_H__2A51C317_11C8_4D0E_ACDE_8DBA3F4FFEF8__INCLUDED_)
#define AVIMANAGER_H__2A51C317_11C8_4D0E_ACDE_8DBA3F4FFEF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vfw.h>
#include <dshow.h>
#include "ProgressWnd.h"

#define SAFE_RELEASE(x)		{ if (x) x->Release(); x = NULL; }  
#define HELPER_RELEASE(x)	{ if (x) x->Release(); x = NULL; }  
#define SELF_RELEASE(x)		{ if (x) x->Release(); delete x; x = NULL;}  

#define	WM_AVIMGR_NOTIFY		(WM_APP + 20)

/////////////////////////////////////////////////////////////////////////////
// CAviManager 

class CAviManager : public CWnd
{
	DECLARE_DYNAMIC(CAviManager)

	friend UINT SaveWorkerThread( LPVOID pParam );

private:
	CTPerror te;

	CProgressWnd *m_pWndProgress;
	CWnd *m_pParentWnd;

	IGraphBuilder *m_pGraph;		// �O���t  
	IBaseFilter *m_pAviSrcFilter;	// AVI reader filter  
	IBaseFilter *m_pWavSrcFilter;	// WAV reader filter  
	IBaseFilter *m_pMuxFilter;		// avi mux filter  
	IBaseFilter *m_pSplitFilter;	// avi splitter filter  
	IBaseFilter *m_pWriteFilter;	// file write Filter  
	IFileSourceFilter *m_pSrcAvi;	// AVI reader�p���̓t�@�C����`
	IFileSourceFilter *m_pSrcWav;	// WAV reader�p���̓t�@�C����`
	IFileSinkFilter *m_pSink;		// WriteFilter�p�o�̓t�@�C����`
	IMediaControl *m_pControl;		// Media Controller  
	IMediaEventEx *m_pEvent;		// Media Eventer
	IMediaEventSink *m_pEventSink;	// Media Event Sink
	IMediaSeeking *m_pSeek;			// Media Seeking

	// AVISave�p�����[�^
	LPAVICOMPRESSOPTIONS m_popt[2];
	PAVISTREAM m_pstm[2];

	HRESULT WINAPI FindEnumFilter(const GUID CategoryCLSID, CString FilterName, IBaseFilter **pFilter);
	HRESULT __stdcall GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir,	IPin **ppPin, GUID mediaType = GUID_NULL, int intCnt = 1);
	HRESULT __stdcall ConnectFilters(IGraphBuilder *pGraph,	IBaseFilter *pSrc, IBaseFilter *pDest, GUID mediaTypeSrc = GUID_NULL, GUID mediaTypeDest = GUID_NULL);
	
	void Release();

	CString m_strOutFile;		// �o��AVI�t�@�C��
	CString m_strInFile; 		// ����AVI�t�@�C��
	CString m_strWavFile; 		// ����WAV�t�@�C��
	BOOL m_blMove;				// ������ɏo��AVI�����AVI�Ɉړ�
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT nIDEvent);
	LRESULT OnGraphNotify(WPARAM wParam, LPARAM lParam);

public:
	CAviManager(CWnd *pParentWnd);
	~CAviManager();

	ST_TIMER m_stApp;		// �v���O���X�p�^�C�}
	BOOL StartTimer();		// �v���O���X�p�^�C�}�J�n
	BOOL StopTimer();		// �v���O���X�p�^�C�}�I��

	BOOL ChkAudio(CString strFile);
	void SetMove(BOOL blMove) {	m_blMove = blMove; }
	BOOL CopyAviFileVfw(CString strOutFile, CString strInFile, CString strWavFile);
	BOOL CopyAviFileDs(CString strOutFile, CString strInFile, CString strWavFile, BOOL blModal = TRUE);
};


#endif // !defined(AVIMANAGER_H__2A51C317_11C8_4D0E_ACDE_8DBA3F4FFEF8__INCLUDED_)
