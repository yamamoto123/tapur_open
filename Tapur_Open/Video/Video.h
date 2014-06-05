// Video.h: interface for the CVideo class.
//
// define as Singleton Patern
//////////////////////////////////////////////////////////////////////

#if !defined(_VIDEO_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
#define _VIDEO_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TPerror.h"
#include "videograbbervc6.h"
#include "videograbberConst.h"
#include "VideoDialog.h"
#include "ScreenCap.h"

// 仮想カメラフィルターデバイス名
#define TVC_IN_DEV _T("Tapur Virtual Camera")

class CVideo
{
private:
	CTPerror te;
	static CVideo *_instance;
protected:
	CVideo() {} // <- このクラスのインスタンス化を抑止
public:
	static CVideo *Instance();
    static void Instance_Delete();

private:
	CVideoDialog		m_dlgVideo;		// Video Dialog
	CVideoGrabberVC6	*m_pVideoGrab;	// VideoGrabber
	HWND m_hWndTarge;		// 録画対象ウィンドウ
	CString	m_PlayFile;		// 再生ファイル名
	CString	m_RecFile;		// 録画ファイル名
//	BOOL m_blPlayPause;	// 再生一時停止状態フラグ
	BOOL blPlayingSkype;// ストリーム再生状態フラグ
	BOOL blRecording;	// 録画状態フラグ
	BOOL m_blCompress;			// 圧縮可否フラグ
	CString	m_strCompress;		// 圧縮形式名
	WAVEFORMATEX m_WaveFormat;	// 音質（NULLの場合、音なし）
	CPtrList *m_pAudioBuffer;		// オーディオストリームバッファ参照用ポインタ

public:
	CScreenCap			*m_pScreenCap;	// ScreeCapture

	void Reset();	
	void SetPlayFile(CString strFile) {m_PlayFile = strFile;}
	CString GetPlayFile() { return m_PlayFile; }
	CString GetRecFile() { return m_RecFile; }
	//CString GetCompressors() { return m_pVideoGrab->GetVideoCompressors(); }
	//int CountCompressor() { return m_pVideoGrab->GetVideoCompressor(); }
	void GetCompressors(CStringList *pStrList);
	void SetCompress(BOOL blCmp) { m_blCompress = blCmp; }
	void SetCompressName(CString strCmp) { m_strCompress = strCmp; }
	void SetAudio(LPWAVEFORMATEX pWf, CPtrList *pAudioBuffer);
	BOOL PlayOpen();
	BOOL Play(CString strFile);
	BOOL PlayPause();
	BOOL PlayContinue();
	BOOL PlayStop();
	//BOOL IsPlaying() { return m_dlgVideo.IsPlayStatus(ps_Playing); }
	//BOOL IsPlayPause() { return m_dlgVideo.IsPlayStatus(ps_Paused); }
	BOOL IsPlaying() {
		// PAUSEの場合GetIsPlayerPlaying()はFALSEになるため、以下のように記述
		return (m_pVideoGrab->GetIsPlayerPaused() ? TRUE : m_pVideoGrab->GetIsPlayerPlaying()); 
	}
	BOOL IsPlayPause() { return m_pVideoGrab->GetIsPlayerPaused(); }
	BOOL PlayOpenSkype(CString strPath);
	BOOL PlaySkype(CString strPlayFile);
	BOOL PlayPauseSkype();
	BOOL PlayContinueSkype();
	BOOL PlayStopSkype();
	BOOL IsPlayingSkype() { return blPlayingSkype; }
	BOOL RecordOpen(CString strSkypeId);
	BOOL Record(CString	strRecFile);
	BOOL RecPause();
	BOOL RecContinue();
	BOOL RecStop();
	BOOL IsRecording();
	BOOL IsRecordPause() { return m_pScreenCap->IsRecordPause(); }
	float GetPlaySize();	// ファイルの演奏サイズ
	float GetPlaySize(CString strPlayFile);
	float GetFps();			// ファイルのframe/sec
	float GetFps(CString strPlayFile);
	BOOL ChkPlayFile();		// ファイルのフォーマットチェック
	BOOL ChkPlayFile(CString strPlayFile);
	HWND GetVideoWinLocal(CString strSkypeId, LPRECT pRect);	// ビデオ画面サイズ
	HWND GetVideoWinLarge(LPRECT pRect);	// ビデオ画面サイズ
	BOOL SetVideoWinLarge(LPRECT pRect);	// ビデオ画面サイズ設定
};

#endif // !defined(_VIDEO_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
