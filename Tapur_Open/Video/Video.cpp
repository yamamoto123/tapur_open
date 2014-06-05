// Video.cpp: implementation of the CVideo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tapur.h"
#include "Video.h"
#include "VideoDialog.h"
#include "FileSpec.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVideo *CVideo::_instance = 0;

//
//  機能     : CVideoクラスのインスタンス作成
//  
//  機能説明 : インスタンスはアプリケーション全体で一つしか生成しない。
//  　　　　　 これにより、設定済みのプロパティがシステム全体で共用できる。
//  
//  返り値   : 
//  
//  備考     : 
//  
CVideo *CVideo::Instance()
{
	if (_instance == 0) {
		_instance = new CVideo;

		_instance->blRecording = FALSE;
		_instance->blPlayingSkype = FALSE;
		_instance->m_PlayFile = _T("");
		_instance->m_RecFile = _T("");

		/// VideoGrabber参照用にVideo Dialogを生成（表示はしない）
		_instance->m_dlgVideo.Create(IDD_VIDEO, NULL);
		_instance->m_pVideoGrab = &(_instance->m_dlgVideo.m_VideoGrab);

		_instance->m_pScreenCap = new CScreenCap(AfxGetMainWnd());
	}
	return _instance;
}

//
//  機能     : Audioクラスのインスタンスの破棄
//  
//  機能説明 : アプリケーション終了時でのみコール
//  
//  返り値   : 
//  
//  備考     : 
//  
void CVideo::Instance_Delete()
{
    if (_instance != 0) {
		delete _instance->m_pScreenCap;

		/// Video Dialogを破棄
		_instance->m_pVideoGrab = NULL;
		_instance->m_dlgVideo.DestroyWindow();

        delete _instance;
        _instance = 0 ;
    }
}

//////////////////////////////////////////////////////////////////////

//
//  機能     : CVideoクラスのリセット
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
void CVideo::Reset()
{
	blRecording = FALSE;
	blPlayingSkype = FALSE;
	m_PlayFile = _T("");
	m_RecFile = _T("");
}

//
//  機能     : ビデオファイルの再生
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CVideo::PlayOpen()
{
//	m_blPlayPause = FALSE;
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("PlayOpen"));

	try {
		// ビデオ画面ダイアログの表示
		m_dlgVideo.ShowWindow(SW_SHOW);
		m_dlgVideo.SetWindowPos(&CWnd::wndTopMost, 0,0,0,0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE);
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("PlayOpen error"));
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : ビデオファイルの再生
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CVideo::Play(CString strPlayFile)	
{
//	m_blPlayPause = FALSE;
CFileSpec fs(strPlayFile);
_stprintf_s(te.errstr, MAX_ERRSTR, _T("Play:%s"), fs.FileName());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

	try {
		m_PlayFile = strPlayFile;
		if ( m_PlayFile.GetLength() <= 0 )
		{
			throw _T("Play video file not found.");
		}

		m_pVideoGrab->SetPlayerFileName(m_PlayFile);
		m_pVideoGrab->SetPlayerAudioRendering(FALSE);	// Audio再生をしない
		m_pVideoGrab->SetAutoStartPlayer(FALSE);		// OpenPlayerによる自動再生を禁止(PAUSEのため)
		if ( !m_pVideoGrab->OpenPlayer() ) {
			// Win7+DivXの環境でVideoGrabberが動作しない→エラーでなくWMP再生で代用
			//throw _T("can't start playing video");
			return FALSE;	
		}
		m_pVideoGrab->RunPlayer();
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, err);
		return FALSE;
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("Play error"));
		return FALSE;
	}

	return TRUE;
}

BOOL CVideo::PlayPause() 
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayPause"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

	try {
		//if (m_dlgVideo.IsPlayStatus(ps_Playing) && !m_dlgVideo.IsPlayStatus(ps_Paused)) {
		if (m_pVideoGrab->GetIsPlayerPlaying() && !m_pVideoGrab->GetIsPlayerPaused()) {
			m_pVideoGrab->PausePlayer();
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("PlayPause error"));
		return FALSE;
	}

//	m_blPlayPause = TRUE;
	return TRUE;
}

BOOL CVideo::PlayContinue() 
{
//	m_blPlayPause = FALSE;
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayContinue"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

	try {
		//if (m_dlgVideo.IsPlayStatus(ps_Paused)) {
		if (m_pVideoGrab->GetIsPlayerPaused()) {
			m_pVideoGrab->RunPlayer();
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("PlayContinue error"));
		return FALSE;
	}

	return TRUE;
}

BOOL CVideo::PlayStop() 
{
//	m_blPlayPause = FALSE;
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayStop"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

	try {
		m_pVideoGrab->ClosePlayer();

		// ビデオ画面ダイアログの非表示
		m_dlgVideo.ShowWindow(SW_HIDE);
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("PlayStop error"));
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : ビデオファイルのSkypeストリーム開始
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CVideo::PlayOpenSkype(
CString strPath)	// 作業フォルダ
{
	try {
		// 作業フォルダ内のファイル削除
		TCHAR strTemp[MAX_PATH];
		TCHAR string[MAX_PATH];
    
		_tcscpy_s(string, MAX_PATH, (LPCTSTR)strPath);
		if (_taccess_s(string, 0) != 0) return FALSE;
    
		WIN32_FIND_DATA info;
		HANDLE hp; 
		_stprintf_s(strTemp, MAX_PATH, _T("%s\\vg*.jpg"), string);
		hp = FindFirstFile(strTemp, &info);
		if (hp != INVALID_HANDLE_VALUE) {
			TRY {
				do
				{
				   _stprintf_s(strTemp, MAX_PATH, _T("%s\\%s"), string, info.cFileName);
				   DeleteFile(strTemp);
				}while(FindNextFile(hp, &info)); 
				FindClose(hp);
			}
			CATCH_ALL(e)
			{
				FindClose(hp);
//				return FALSE;
			}
			END_CATCH_ALL
		}
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("PlayOpenSkype"));

		/// ビデオ画面ダイアログの非表示
		m_dlgVideo.ShowWindow(SW_HIDE);

		/// 再生ストリームの作業フォルダ指定
		m_dlgVideo.SetStreamPath(strPath);

		/// VideoGrabber設定
		m_pVideoGrab->SetFrameGrabber(fg_PreviewStream);
		m_pVideoGrab->SetBurstMode(TRUE);
		m_pVideoGrab->SetBurstType(fc_JpegFile);
		m_pVideoGrab->SetBurstCount(0);
		m_pVideoGrab->SetBurstInterval(0);
		m_pVideoGrab->SetStoragePath((LPCTSTR)strPath);
		m_pVideoGrab->SetAutoFileName(fn_Sequential);
		m_pVideoGrab->SetAutoFilePrefix(_T("vg"));
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, err);
		return FALSE;
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("PlayOpenSkype error"));
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : ビデオファイルのSkypeストリーム再生
//  
//  機能説明 : フレームを連続生成する。実際の再生はtapsrc.axフィルタで行う
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CVideo::PlaySkype(
CString strPlayFile)	// 再生ファイル
{
	blPlayingSkype = TRUE;

	try {
		m_PlayFile = strPlayFile;
		if ( m_PlayFile.GetLength() <= 0 )
		{
			throw _T("Play video file not found.");
		}		
		
		/// 再生ファイルのfpsを取得
		int intFps = (int)GetFps();
		if (intFps <= 0) {
			// 不明の場合デフォルト値
			CRegDWORD rgFps(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordFps"), 5, TRUE);	// 初期値:5(fps)
			intFps = (int)rgFps;	
		}

CFileSpec fs(m_PlayFile);
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlaySkype:%s"), fs.FileName());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		m_pVideoGrab->SetFrameCaptureWithoutOverlay(TRUE);
		m_pVideoGrab->SetAutoRefreshPreview(TRUE); 
		m_pVideoGrab->SetVideoSource(vs_VideoFileOrURL);
		m_pVideoGrab->SetFrameRate((double)intFps);	
//		m_pVideoGrab->SetFrameRate(0.0);	
		m_pVideoGrab->SetVideoSource_FileOrURL((LPCTSTR)strPlayFile);
		m_pVideoGrab->StartPreview();
	} catch(_com_error& e) {
		blPlayingSkype = FALSE;
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(LPTSTR err) {
		blPlayingSkype = FALSE;
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, err);
		return FALSE;
	} catch(...) {
		blPlayingSkype = FALSE;
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("RecordOpen error"));
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : ビデオファイルのSkypeストリーム再生一時停止
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CVideo::PlayPauseSkype() 
{
	try {
		if (blPlayingSkype) {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayPauseSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			m_pVideoGrab->PausePreview();	
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("PlayPauseSkype error"));
		return FALSE;
	}

//	m_blPlayPause = TRUE;
	return TRUE;
}

//
//  機能     : ビデオファイルのSkypeストリーム再生一時停止再開
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CVideo::PlayContinueSkype() 
{
//	m_blPlayPause = FALSE;

	try {
		if (blPlayingSkype) {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayContinueSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			m_pVideoGrab->ResumePreview();	
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("PlayContinueSkype error"));
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : ビデオファイルのSkypeストリーム再生終了
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CVideo::PlayStopSkype() 
{

	try {
		if (blPlayingSkype) {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayStopSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			m_pVideoGrab->StopPreview();
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("PlayStopSkype error"));
		return FALSE;
	}

	blPlayingSkype = FALSE;
	return TRUE;
}

//
//  機能     : 圧縮フィルタリスト
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
void CVideo::GetCompressors(CStringList *pStrList)
{
	m_pScreenCap->ListCompressFilter(pStrList);
}

//
//  機能     : 録画中か否か
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CVideo::IsRecording() 
{ 
	if (blRecording) return TRUE;

	try {
		return m_pScreenCap->IsRecording();
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("IsRecording error"));
		return FALSE;
	}

	return FALSE; 
}

//
//  機能     : 録画開始準備
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CVideo::RecordOpen(CString strSkypeId)
{
//	m_blRecordPause = FALSE;
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordOpen Skype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

	/// SkypeID or Name
	m_dlgVideo.SetSkypeId(strSkypeId);

	/// ビデオ画面を前面表示するか？
	CRegDWORD rgVSetTop(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordSetTop"), 1, TRUE);	// 初期値:1(True)
	m_dlgVideo.m_blVSetTop = (rgVSetTop == 1) ? TRUE : FALSE;

	try {
		/// Capture領域の取得
		/// この領域サイズを基準にする
		CRect rect;
		HWND hWnd = NULL;
		hWnd = m_dlgVideo.GetVideoWndLarge(rect);
		if (hWnd) {
		//// Skypeビデオが別Windowで起動
			if (m_dlgVideo.m_blVSetTop) {
				m_dlgVideo.SetTopVideoWndLocal(FALSE);
				m_dlgVideo.SetTopVideoWndLarge(TRUE);
			}
			m_hWndTarge = hWnd;
			return TRUE;
		} 
		hWnd = m_dlgVideo.GetVideoWndLocal(rect);
		if (hWnd) {
		//// Skypeビデオが起動
			if (m_dlgVideo.m_blVSetTop) {
				m_dlgVideo.SetTopVideoWndLocal(TRUE);
				m_dlgVideo.SetTopVideoWndLarge(FALSE);
			}
			m_hWndTarge = hWnd;
			return TRUE;
		}
		if (hWnd == NULL) {
		//// Skypeビデオが起動していない
			m_dlgVideo.SetTopVideoWndLocal(FALSE);
			m_dlgVideo.SetTopVideoWndLarge(FALSE);
			return FALSE;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, err);
		return FALSE;
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("RecordOpen error"));
		return FALSE;
	}

	return TRUE;
}

BOOL CVideo::Record(
CString strRecFile)			// 録画ファイル
{
CFileSpec fs(strRecFile);
_stprintf_s(te.errstr, MAX_ERRSTR, _T("Record:%s"), fs.FileName());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
	try {
		/// 録画準備
		m_RecFile = strRecFile;
		/// FPS
		CRegDWORD rgFps(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordFps"), 5, TRUE);	// 初期値:5(fps)
		int intFps = (int)rgFps;
		/// オーバーレイ
		CRegDWORD rgOverlay(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordOverlay"), 1, TRUE);	// 初期値:1(オーバーレイする)
		BOOL blOverlay = ((DWORD)rgOverlay == 1) ? TRUE : FALSE;

		/// ScreenCapture 設定
		LPWAVEFORMATEX pWF = (m_pAudioBuffer) ? &m_WaveFormat : NULL;
		if (m_blCompress) {
			//// 圧縮
			m_pScreenCap->BuilderGraph(m_hWndTarge, intFps, m_strCompress, pWF, m_RecFile, blOverlay);
		} else {
			//// 非圧縮
			m_pScreenCap->BuilderGraph(m_hWndTarge, intFps, NULL, pWF, m_RecFile, blOverlay);
		}
		// 音声ストリーム用リストバッファの結合
		if (m_pAudioBuffer) {
			m_pScreenCap->BindAudioBuffer(m_pAudioBuffer);
		}
		//// ファイルサイズ制限
		CRegDWORD rgSep(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordSep"), 1, TRUE);	// 初期値:1(分割する)
		BOOL blSep = ((DWORD)rgSep == 1) ? TRUE : FALSE;
		if (blSep) {
			CRegDWORD rgSepSize(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordSepSize"), 2000, TRUE);	// 初期値:2000Mbyte
			ULONGLONG ullSz = (ULONGLONG)rgSepSize * (ULONGLONG)(1024 * 1024 * 0.95);  // 音声結合を考慮して95%のサイズで
			m_pScreenCap->SetLimitSize(ullSz);
		} else {
			m_pScreenCap->SetLimitSize(0);
		}

		blRecording = TRUE;

		/// 録画用タイマー開始(イベントはm_dlgVideoで受信)
		//SetTimer(m_dlgVideo.m_hWnd, ID_TIMER_VIDEO, 100, NULL);	// 0.1 sec each

		/// 以下のメソッドはメッセージ割り込みを許可するので、最後に実行する
		if (!m_pScreenCap->StartRec()) {
			throw _T("can't start recording video");
		}

	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, err);
		return FALSE;
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("Record error"));
		return FALSE;
	}


	return TRUE;
}

BOOL CVideo::RecPause()
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecPause"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
	try {
		//if ( !m_pVideoGrab->PauseRecording() ) {
		if ( !m_pScreenCap->PauseRec() ) {
			throw _T("can't pause recording video");
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, err);
		return FALSE;
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("RecPause error"));
		return FALSE;
	}

//	m_blRecordPause = TRUE;
	return TRUE;
}

BOOL CVideo::RecContinue()
{
//	m_blRecordPause = FALSE;
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecContinue"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

	try {
		//if ( !m_pVideoGrab->ResumeRecording() ) {
		if ( !m_pScreenCap->ResumeRec() ) {
			throw _T("can't resume recording video");
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, err);
		return FALSE;
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("RecContinue error"));
		return FALSE;
	}

	return TRUE;
}

BOOL CVideo::RecStop() 
{
//	m_blRecordPause = FALSE;
	blRecording = FALSE;
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecStop"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

	try {
		/// 録画用タイマー停止
		//KillTimer(m_dlgVideo.m_hWnd, ID_TIMER_VIDEO);
		
		m_pScreenCap->StopRec();

		/// ビデオ画面の前面表示解除
		m_dlgVideo.SetTopVideoWndLocal(FALSE);
		m_dlgVideo.SetTopVideoWndLarge(FALSE);

		/// 録画ファイルの書き込みを有効にするため、待機中のメッセージを処理する
		MSG msg; 
		while(::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) {
			AfxGetThread()->PumpMessage();
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, err);
		return FALSE;
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("RecStop error"));
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 結合音声の設定
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
void CVideo::SetAudio(LPWAVEFORMATEX pWf, CPtrList *pAudioBuffer)
{
	if (pWf) {
		CopyMemory(&m_WaveFormat, pWf, sizeof(WAVEFORMATEX));	// 音質（NULLの場合、音なし）
	}
	m_pAudioBuffer = pAudioBuffer;		// オーディオストリームバッファ参照用ポインタ
}

//
//  機能     : ファイルの演奏サイズの取得
//  
//  機能説明 : 
//  
//  返り値   : 秒数、-1:error
//  
//  備考     : 
//  
float CVideo::GetPlaySize()
{
	double tsz = 0.0;
	double Duration = 0.0;
	double FrameCount = 0.0;
/*
	long VideoWidth;
	long VideoHeight;
	double VideoFrameRateFps;
	long AudioBitRateHz;
	BSTR VideoCodec;
	BSTR AudioCodec;
*/

	try {
		if (m_PlayFile.IsEmpty()) return -1;
		CFileSpec fs(m_PlayFile);
		if (!fs.Exist()) return -1;

		if (m_pVideoGrab->AVIDuration((LPCTSTR)m_PlayFile, &Duration, &FrameCount)) {
			tsz = Duration;
		}
/* 以下はなぜか落ちる
		if (m_pVideoGrab->AVIInfo((LPCTSTR)m_PlayFile, 
			&Duration, &FrameCount, &VideoWidth, &VideoHeight, &VideoFrameRateFps, 
			&AudioBitRateHz, &VideoCodec, &AudioCodec)) {
			tsz = Duration;
		}
*/
/* 以下はなぜか値が１になる場合がある
		m_pVideoGrab->SetPlayerFileName(m_PlayFile);
		m_pVideoGrab->SetAutoStartPlayer(FALSE);	// OpenPlayerによる自動再生を禁止
		if (!m_pVideoGrab->OpenPlayer()) return -1;
		tsz = m_pVideoGrab->GetPlayerDuration();	// 100 nano sec unit
		m_pVideoGrab->ClosePlayer();
*/
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return -1;
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, err);
		return -1;
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("GetPlaySize error"));
		return -1;
	}

	return (float)(tsz/10000000);
}
float CVideo::GetPlaySize(CString strPlayFile)
{
	m_PlayFile = strPlayFile;
	return GetPlaySize();
}

//
//  機能     : ファイルのFPS(Frame/sec)の取得
//  
//  機能説明 : 
//  
//  返り値   : Frame/sec、-1:error
//  
//  備考     : 
//  
float CVideo::GetFps()
{
	double tsz = -1;
	double Duration = 0.0;
	double FrameCount = 0.0;

	try {
		if (m_PlayFile.IsEmpty()) return -1;
		CFileSpec fs(m_PlayFile);
		if (!fs.Exist()) return -1;

		if (m_pVideoGrab->AVIDuration((LPCTSTR)m_PlayFile, &Duration, &FrameCount)) {
			if (FrameCount > 0.0 && Duration > 0.0) {
				tsz = FrameCount/(Duration/10000000);
			}
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return -1;
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, err);
		return -1;
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("GetFps error"));
		return -1;
	}

	return (float)tsz;
}
float CVideo::GetFps(CString strPlayFile)
{
	m_PlayFile = strPlayFile;
	return GetFps();
}

//
//  機能     : ファイルのフォーマットチェック
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CVideo::ChkPlayFile()
{
	BOOL blRet = FALSE;

	try {
		if (m_PlayFile.IsEmpty()) return FALSE;
		CFileSpec fs(m_PlayFile);
		if (!fs.Exist()) return FALSE;

		m_pVideoGrab->SetPlayerFileName(m_PlayFile);
		m_pVideoGrab->SetAutoStartPlayer(FALSE);	// OpenPlayerによる自動再生を禁止(PAUSEのため)
		blRet = m_pVideoGrab->OpenPlayer();
		m_pVideoGrab->ClosePlayer();
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
		return FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, err);
		return FALSE;
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("ChkPlayFile error"));
		return FALSE;
	}

	return blRet;
}
BOOL CVideo::ChkPlayFile(CString strPlayFile)
{
	m_PlayFile = strPlayFile;
	return ChkPlayFile();
}

//
//  機能     : ビデオ画面のサイズ
//  
//  機能説明 : 
//  
//  返り値   : HWND
//  
//  備考     : 
//  
HWND CVideo::GetVideoWinLocal(
CString strSkypeId,		// 発信者SkypeID
LPRECT pRect)
{
	m_dlgVideo.SetSkypeId(strSkypeId);
	return m_dlgVideo.GetVideoWndLocal(pRect); 
}
HWND CVideo::GetVideoWinLarge(
LPRECT pRect)
{
	return m_dlgVideo.GetVideoWndLarge(pRect); 
}

//
//  機能     : ビデオ画面サイズ設定
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CVideo::SetVideoWinLarge(
LPRECT pRect)
{
	return m_dlgVideo.SetVideoWndLarge(pRect, FALSE); 
}
