// Audio.cpp: implementation of the CAudio class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tapur.h"
#include "Audio.h"
#include "FileSpec.h"
#include "TPsetup.h"
#include "WaveFileACM.h"

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

CAudio *CAudio::_instance = 0;

//
//  機能     : Audioクラスのインスタンス作成
//  
//  機能説明 : インスタンスはアプリケーション全体で一つしか生成しない。
//  　　　　　 これにより、設定済みのプロパティがシステム全体で共用できる。
//  
//  返り値   : 
//  
//  備考     : 
//  
CAudio *CAudio::Instance()
{
	if (_instance == 0) {
		_instance = new CAudio;
		_instance->pWaveInDev = new CWaveDevice;
		_instance->pWaveOutDev = new CWaveDevice;
		_instance->pVacInDev = new CWaveDevice;
		_instance->pWaveOut = new CWaveOut;
		_instance->pWaveIn = new CWaveIn;
		_instance->pVacIn = new CWaveIn;
		_instance->pSocketIn = new CSocketIn;

		_instance->m_pCall = NULL;			// SKYPE録音再生用コール
		_instance->m_blSkypeMode = FALSE;		// Skype再生フラグ(TRUE:Skype再生, FALSE:音声デバイス再生)
		_instance->m_blPlayPause = FALSE;		// 再生一時停止状態フラグ
		_instance->m_blRecordPause = FALSE;	// 録音一時停止状態フラグ
		_instance->m_blRecordPauseVac = FALSE;// 録音一時停止状態フラグ(VAC)
		_instance->m_blPlayingSkype = FALSE;		// 再生状態フラグ(Skype)
		_instance->m_blPlayPauseSkype = FALSE;	// 再生一時停止状態フラグ(Skype)
		_instance->m_blRecordingSkype = FALSE;	// 録音状態フラグ(Skype)
		_instance->m_blRecordPauseSkype = FALSE;	// 録音一時停止状態フラグ(Skype)
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
void CAudio::Instance_Delete()
{
    if (_instance != 0) {
		delete _instance->pWaveInDev;
		delete _instance->pWaveOutDev;
		delete _instance->pVacInDev;
		delete _instance->pWaveOut;
		delete _instance->pWaveIn;
		delete _instance->pVacIn;
		delete _instance->pSocketIn;
		_instance->pWaveInDev = NULL;
		_instance->pWaveOutDev = NULL;
		_instance->pVacInDev = NULL;
		_instance->pWaveOut = NULL;
		_instance->pWaveIn = NULL;
		_instance->pVacIn = NULL;
		_instance->pSocketIn = NULL;
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
void CAudio::Reset()
{
		m_pCall = NULL;			
		m_blSkypeMode = FALSE;		
		m_blPlayPause = FALSE;		
		m_blRecordPause = FALSE;	
		m_blRecordPauseVac = FALSE;
		m_blPlayingSkype = FALSE;		
		m_blPlayPauseSkype = FALSE;	
		m_blRecordingSkype = FALSE;	
		m_blRecordPauseSkype = FALSE;	
		m_PlayFile = _T("");
		m_RecFile = _T("");
		m_RecVacFile = _T("");
		m_RecSkypeFile = _T("");
}

//
//  機能     : VACデバイスの設定
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::SetInVacDevice(
CString strDev)	// 入力デバイス名
{
	UINT i = 0;
	CString strWavDevice;

	try {
		for (i = 0; i < CWaveInterface::GetWaveInCount(); i++) {
			strWavDevice = CWaveInterface::GetWaveInName(i);
			if (!strWavDevice.IsEmpty()) {
				if (strWavDevice.CompareNoCase(strDev) == 0 ) {
					pVacInDev->SetDevice(i);
					pVacIn->SetDevice(*pVacInDev);
					break;
				}
			}
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, _T("Input device not found."));
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 入力デバイスの設定
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 該当がない場合は初期設定（WAVE_WRAPPER）のまま
//  
BOOL CAudio::SetInDevice(
CString strDev)	// 入力デバイス名
{
	UINT i = 0;
	CString strWavDevice;

	try {
		if (strDev.IsEmpty()) {
			pWaveInDev->SetDevice(WAVE_MAPPER);
			pWaveIn->SetDevice(*pWaveInDev);
			return TRUE;
		}

		for (i = 0; i < CWaveInterface::GetWaveInCount(); i++) {
			strWavDevice = CWaveInterface::GetWaveInName(i);
TRACE(_T("%s\n"), strWavDevice);
			if (!strWavDevice.IsEmpty()) {
				if (strWavDevice.CompareNoCase(strDev) == 0 ) {
					pWaveInDev->SetDevice(i);
					pWaveIn->SetDevice(*pWaveInDev);
					break;
				}
			}
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, _T("Set Input device error"));
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 出力デバイスの設定
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 該当がない場合は初期設定（WAVE_WRAPPER）のまま
//  
BOOL CAudio::SetOutDevice(CString strDev)
{
	UINT i = 0;
	CString strWavDevice;

	try {
		if (strDev.IsEmpty()) {
			pWaveOutDev->SetDevice(WAVE_MAPPER);
			pWaveOut->SetDevice(*pWaveOutDev);
			return TRUE;
		}

		for (i = 0; i < CWaveInterface::GetWaveOutCount(); i++) {
			strWavDevice = CWaveInterface::GetWaveOutName(i);
TRACE(_T("%s\n"), strWavDevice);
			if (!strWavDevice.IsEmpty()) {
				if (strWavDevice.CompareNoCase(strDev) == 0 ) {
					pWaveOutDev->SetDevice(i);
					pWaveOut->SetDevice(*pWaveOutDev);
					break;
				}
			}
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, _T("Set Output device error"));
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 入力デバイスの取得
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
CString CAudio::GetInDevice(int intNum)	// 0 - n
{
	UINT i = 0;
	CString strWavDevice;

	strWavDevice.Empty();
	try {
		for (i = 0; i < CWaveInterface::GetWaveInCount(); i++) {
			if (i == intNum) {
				strWavDevice = CWaveInterface::GetWaveInName(i);
				break;
			}
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, _T("Get Input device error"));
	}

	return strWavDevice;
}

//
//  機能     : 出力デバイスの取得
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
CString CAudio::GetOutDevice(int intNum)	// 0 - n
{
	UINT i = 0;
	CString strWavDevice;

	strWavDevice.Empty();
	try {
		for (i = 0; i < CWaveInterface::GetWaveOutCount(); i++) {
			if (i == intNum) {
				strWavDevice = CWaveInterface::GetWaveOutName(i);
				break;
			}
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, _T("Get Output device error"));
	}

	return strWavDevice;
}

//
//  機能     : WAVE入力→WAVE出力ライン設定
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : MIXERLINE_COMPONENTTYPE_SRC_WAVEOUTが見つからない場合、
//             MIXERLINE_COMPONENTTYPE_SRC_UNDEFINEDでリトライ（デバイスドライバの障害？）
//  
/*
BOOL CAudio::SetWave(
BOOL blOn)		// ON/OFF
{
	BOOL blFind;

	/// WAVE入力→WAVE出力
	blFind = TRUE;
	// 以下の順序は変更不可（不安定になる）
	if ( !m_Mixer.FindControl(MIXERLINE_COMPONENTTYPE_DST_WAVEIN, 
			MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT, MIXERCONTROL_CONTROLTYPE_MIXER) ) {
	if ( !m_Mixer.FindControl(MIXERLINE_COMPONENTTYPE_DST_WAVEIN, 
			MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED, MIXERCONTROL_CONTROLTYPE_MIXER) ) {
	//// WAVE入力がない場合PCでは以下が有効
	// WAVE入力とmonoral Mixerの両方がある場合、WAVE入力のみ有効
	if ( !m_Mixer.FindControl(MIXERLINE_COMPONENTTYPE_DST_WAVEIN, 
			MIXERLINE_COMPONENTTYPE_SRC_ANALOG, MIXERCONTROL_CONTROLTYPE_MIXER) ) {		// monoral Mixer
			blFind = FALSE;
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Not find Wave Mixer control"));
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	}
	}
	}
	if (blFind) {
		::Sleep(500);	// WAITが必要
		if (blOn) {
			m_Mixer.SetSelect(TRUE);
		} else {
			m_Mixer.SetSelect(FALSE);
		}
	}

	return TRUE;
}
*/

//
//  機能     : MicのON/OFF
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::SetMic(
BOOL blOn)		// ON/OFF
{
	BOOL blFind;

	/// マイク入力→WAVE出力
	blFind = TRUE;
	if ( !m_Mixer.FindControl(MIXERLINE_COMPONENTTYPE_DST_WAVEIN, 
			MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE, MIXERCONTROL_CONTROLTYPE_MIXER) ) {
			blFind = FALSE;
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Not find Mic control"));
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	}
	if (blFind) {
		if (blOn) {
				m_Mixer.SetSelect(TRUE);
		} else {
				m_Mixer.SetSelect(FALSE);
		}
	}

	return TRUE;
}

//
//  機能     : スピーカON/OFF
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 、
//  
BOOL CAudio::SetSpk(
BOOL blOn)		// ON/OFF
{
	BOOL blFind;

	/// スピーカ出力
	blFind = TRUE;
	if ( !m_Mixer.FindControl(MIXERLINE_COMPONENTTYPE_DST_SPEAKERS, 
			MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED, MIXERCONTROL_CONTROLTYPE_MUTE) ) {
			blFind = FALSE;
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Not find Speaker control"));
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	}
	if (blFind) {
		if (blOn) {
			m_Mixer.SetControlValue(FALSE);
		} else {
			m_Mixer.SetControlValue(TRUE);	// mute
		}
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの再生準備（標準/VACデバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlayOpen(			
CString strPlayDev1)
{
	CWaveOut::PlayType type;

	m_blPlayPause = FALSE;
CFileSpec fs(m_PlayFile);
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayOpen:%s:%s"), strPlayDev1, fs.Extension());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {
		if ( pWaveOut->IsPlaying() ) {
			if ( !pWaveOut->Close() ) {
				throw (LPTSTR)(LPCTSTR)pWaveOut->GetError();
			}
		}

		/// pWaveOutを再生成
		delete pWaveOut;
		pWaveOut = new CWaveOut;

		// 出力デバイス
//		SetOutDevice(strDev);
		pWaveOut->m_blPlayDev1 = (strPlayDev1 != _T("-1")) ? TRUE : FALSE;		// DefaultDeviceが空白の場合があるため
		pWaveOut->m_strPlayDev1 = (pWaveOut->m_blPlayDev1) ? strPlayDev1 : _T("");

		//  スピーカON
//		SetSpk(TRUE);

		/// 再生の開始
		pWaveOut->m_blVac = FALSE;
		if (m_PlayFile.MakeLower().Right(4) == _T(".mp3")) {
			//// MP3再生
			type = CWaveOut::MP3;
			pWaveOut->m_oAcmMgr.SetCnvType(CAcmMgr::MP3_WAV);
			MPEGLAYER3WAVEFORMAT mwf;
			WAVEFORMATEX wfx;
			if ( !pWaveOut->m_oAcmMgr.GetFileFormat(m_PlayFile, &mwf)) return FALSE;
			if ( !pWaveOut->m_oAcmMgr.FindFormat(&mwf, &wfx)) return FALSE;
			pWaveOut->m_wave.BuildFormat(wfx.nChannels, wfx.nSamplesPerSec, wfx.wBitsPerSample);
			if ( !pWaveOutDev->IsOutputFormat(pWaveOut->m_wave) ) return FALSE;
			if ( !pWaveOut->m_oAcmMgr.Read(m_PlayFile)) return FALSE;
		} else {
			//// WAV再生
			type = CWaveOut::WAV;
			if ( !pWaveOut->m_wave.LoadFormat(m_PlayFile, FALSE)) return FALSE;
			if ( !pWaveOutDev->IsOutputFormat(pWaveOut->m_wave) ) return FALSE;
			if ( !pWaveOut->m_wave.Load(m_PlayFile, FALSE)) return FALSE;
		}

		pWaveOut->SetPlayFormat(fmt.nChannels, fmt.nFrequency, fmt.nBits);
		pWaveOut->SetDevice(*pWaveOutDev);
		BOOL bl = pWaveOut->Open(type);
		if (bl == FALSE) {
			throw (LPTSTR)(LPCTSTR)pWaveOut->GetError();
		}
	} catch(LPTSTR err) {
		pWaveOut->Close();	// ファイルロードの中止
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの再生（標準/VACデバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::Play(
BOOL modFull)			// フル再生、割り込み再生
{
	int i = 0;

	m_blPlayPause = FALSE;
_stprintf_s(te.errstr, MAX_ERRSTR, _T("Play"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {
		if ( m_PlayFile.GetLength() <= 0 )
		{
			throw _T("Play file not found.");
		}

		if (modFull) {
			long intLen = (long)GetPlaySize() + 1;
			if ( !pWaveOut->Play() ) {
//			if ( !pWaveOut->FullPlay() ) {
				return FALSE;
//				throw (LPTSTR)(LPCTSTR)pWaveOut->GetError();
			}
			// 再生の完了待ち（再生時間＋１秒）
			for (i = 0; i <= intLen*5; i++) {	// 0.2s x 5
				AfxGetThread()->PumpMessage();	// 必須
				if(!pWaveOut->IsPlaying()) break;
				::Sleep(200);
			}
			pWaveOut->Close();	// 再生終了
		} else {
			if ( !pWaveOut->Play() ) {
				return FALSE;
			}
		}
	}
    catch(CException *e)
    {
		pWaveOut->Close();	// ファイルロードの中止
		CString strErr;
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		strErr.Format(_T("Play error:%s:%s\n"), szCause, (LPTSTR)(LPCTSTR)pWaveOut->GetError());
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, strErr);
		e->Delete();
		return FALSE;
    }

	return TRUE;
}

//
//  機能     : 音声ファイルの再生一時停止（標準/VACデバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlayPause() 
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayPause"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	try {
		if ( !pWaveOut->Pause() ) {
			throw (LPTSTR)(LPCTSTR)pWaveOut->GetError();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	m_blPlayPause = TRUE;
	return TRUE;
}

//
//  機能     : 音声ファイルの再生一時停止の再開（標準/VACデバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlayContinue() 
{
	m_blPlayPause = FALSE;
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayContinue"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {
		if ( !pWaveOut->Continue() ) {
			throw (LPTSTR)(LPCTSTR)pWaveOut->GetError();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの再生停止（標準/VACデバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlayStop() 
{
	m_blPlayPause = FALSE;
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayStop"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {
		if ( !pWaveOut->Close() ) {
			throw (LPTSTR)(LPCTSTR)pWaveOut->GetError();
		}
	
		/// MIXER設定を戻す
//		SetMixer(MIXER_IN_MIC | MIXER_IN_LINE | MIXER_OUT_SPK);

	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;

}
//
//  機能     : 音声ファイルの再生準備（Skypeネイティブ）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlayOpenSkype(ICallPtr pCall)	
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayOpenSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	m_blPlayingSkype = FALSE;
	m_blPlayPauseSkype = FALSE;

	try {
		m_pCall = pCall;
		if (m_blPlayingSkype) {
			if ( !m_SkypeApi.PlayStop(m_pCall) ) {
				 return FALSE;
			}
		}
	} catch(LPTSTR err) {
		m_blSkypeMode = FALSE;
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの再生（Skypeネイティブ）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlaySkype(
BOOL modFull)			// フル再生
{
	int i = 0;

CFileSpec fs(m_PlayFile);
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlaySkype:%s"), fs.Extension());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	m_blPlayingSkype = TRUE;
	m_blPlayPauseSkype = FALSE;

	try {
		if ( m_PlayFile.GetLength() <= 0 ) {
			throw _T("Play file not found.");
		}

		long intLen = (long)GetPlaySize() + 1;

		// VoiceAPIによるSkype再生
		if ( !m_SkypeApi.Play(m_pCall, m_PlayFile) ) {
			return FALSE;
		}

		if (modFull) {
			// 再生の完了待ち（再生時間＋１秒）
			for (i = 0; i <= intLen*5; i++) {	// 0.2s x 5
				AfxGetThread()->PumpMessage();	// 必須
				::Sleep(200);
			}
			m_SkypeApi.PlayStop(m_pCall);	// 再生終了
		}
	}
    catch(CException *e)
    {
		m_blSkypeMode = FALSE;
		CString strErr;
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		strErr.Format(_T("Play error:%s\n"), szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, strErr);
		e->Delete();
		return FALSE;
    }

	return TRUE;
}

//
//  機能     : 音声ファイルの再生一時停止（Skypeネイティブ）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlayPauseSkype() 
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayPauseSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {

	} catch(LPTSTR err) {
		m_blSkypeMode = FALSE;
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	m_blPlayPauseSkype = TRUE;
	return TRUE;
}

//
//  機能     : 音声ファイルの再生一時停止の再開（Skypeネイティブ）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlayContinueSkype() 
{
	m_blPlayPauseSkype = FALSE;
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayContinueSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {

	} catch(LPTSTR err) {
		m_blSkypeMode = FALSE;
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの再生停止（Skypeネイティブ）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlayStopSkype() 
{
	m_blPlayingSkype = FALSE;
	m_blPlayPauseSkype = FALSE;
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayStopSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {

		// VoiceAPIによるSkype再生停止
		if ( !m_SkypeApi.PlayStop(m_pCall) ) {
			return FALSE;
		}

	} catch(LPTSTR err) {
		m_blSkypeMode = FALSE;
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	m_blSkypeMode = FALSE;
	return TRUE;
}

//
//  機能     : 音声ファイルの録音準備（標準デバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecordOpen(
CString strDev,			// 入力デバイス名
CString strPlayDev1,		// リアルタイム再生用デバイス名1
CString strPlayDev2)		// リアルタイム再生用デバイス名2
{
	m_blRecordPause = FALSE;
	pWaveIn->m_blVirtualPause = FALSE;		// 仮想停止を解除
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordOpen"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {
		if ( pWaveOut->IsPlaying() ) {
			if ( !pWaveOut->Close() ) {
				throw (LPTSTR)(LPCTSTR)pWaveOut->GetError();
			}
		}

		/// pWaveInを再生成
		delete pWaveIn;
		pWaveIn = new CWaveIn;

		/// デバイスの指定
		SetInDevice(strDev);
		pWaveIn->m_blPlayDev1 = (strPlayDev1 != _T("-1")) ? TRUE : FALSE;		// DefaultDeviceが空白の場合があるため
		pWaveIn->m_blPlayDev2 = (strPlayDev2 != _T("-1")) ? TRUE : FALSE;
		pWaveIn->m_strPlayDev1 = (pWaveIn->m_blPlayDev1) ? strPlayDev1 : _T("");
		pWaveIn->m_strPlayDev2 = (pWaveIn->m_blPlayDev2) ? strPlayDev2 : _T("");

		/// 録音の開始
		CWave monWave;
		monWave.BuildFormat(fmt.nChannels, fmt.nFrequency, fmt.nBits, FALSE);
		if (!pWaveInDev->IsInputFormat(monWave)) {
			throw _T("Format non support");
		}
		pWaveIn->m_blVac = FALSE;
		pWaveIn->SetWaveFormat(monWave.GetFormat());
		pWaveIn->SetDevice(*pWaveInDev);
		if ( !pWaveIn->Open() ) {
			throw (LPTSTR)(LPCTSTR)pWaveIn->GetError();
		}

	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの録音開始（標準デバイス）
//  
//  機能説明 : 録音データをファイルに保存
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::Record(CString strRecFile)
{
CFileSpec fs(strRecFile);
_stprintf_s(te.errstr, MAX_ERRSTR, _T("Record:%s"), fs.Extension());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	try {
		m_RecFile = strRecFile;
		if ( !pWaveIn->Record(strRecFile) ) {
			throw (LPTSTR)(LPCTSTR)pWaveIn->GetError();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの録音準備（標準デバイス）
//  
//  機能説明 : 録音データを音声入力デバイスに転送
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::Record(CWaveIn *pSyncWaveIn)
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("Record"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	try {
		if ( !pWaveIn->Record(pSyncWaveIn) ) {
			throw (LPTSTR)(LPCTSTR)pWaveIn->GetError();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 同期バッファ付きで録音時の録音対象スイッチ
//  
//  機能説明 : 0:自バッファと同期バッファの録音、1:自バッファのみ、2:同期バッファのみ
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAudio::SetRecordSyncSw(int intSw)
{
	//pVacIn->SetSyncSw(intSw);
	pSocketIn->SetSyncSw(intSw);

	return TRUE;
}

//
//  機能     : 同期バッファ付きで録音時の録音音質
//  
//  機能説明 : 0:自バッファと同期バッファの合成録音(monoral)
//             1:ステレオ録音(完全パン)
//             2:ステレオ録音(調整パン)
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAudio::SetRecordSyncStereo(int intVal)
{
	//pVacIn->SetSyncStereo(intVal);
	pSocketIn->SetSyncStereo(intVal);

	return TRUE;
}

//
//  機能     : 録音時のフォーマット
//  
//  機能説明 : 現在はMP3録音時のみ有効、WAV録音時はフォーマット固定
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAudio::SetRecordFormat(
int intFormatType,		// 0:WAV, 1:MP3
CString strCodec,		// Codec名称
CString strFormat)		// Format名称
{
	return pSocketIn->SetFormat(intFormatType, strCodec, strFormat);
}

//
//  機能     : 音声ファイルの録音一時停止（標準デバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecPause(
BOOL blVirual)	// 仮想停止の使用フラグ 
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecPause"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	pWaveIn->m_blVirtualPause = blVirual;

	try {
		if ( !pWaveIn->Pause() ) {
			throw (LPTSTR)(LPCTSTR)pWaveIn->GetError();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	m_blRecordPause = TRUE;
	return TRUE;
}

//
//  機能     : 音声ファイルの録音一時停止の再開（標準デバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecContinue()
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecContinue"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	pWaveIn->m_blVirtualPause = FALSE;		// 仮想停止を解除
	m_blRecordPause = FALSE;

	try {
		if ( !pWaveIn->Continue() ) {
			throw (LPTSTR)(LPCTSTR)pWaveIn->GetError();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの録音停止（標準デバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecStop() 
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecStop"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	pWaveIn->m_blVirtualPause = FALSE;		// 仮想停止を解除
	m_blRecordPause = FALSE;

	try {
		if ( !pWaveIn->Close() ) {
			throw (LPTSTR)(LPCTSTR)pWaveIn->GetError();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの録音準備（VACデバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecordOpenVac(
CString strDev,			// 入力デバイス名
CString strPlayDev1,		// リアルタイム再生用デバイス名1
CString strPlayDev2)		// リアルタイム再生用デバイス名2
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordOpenVac"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	pVacIn->m_blVirtualPause = FALSE;		// 仮想停止を解除
	m_blRecordPauseVac = FALSE;

	try {
		if ( pWaveOut->IsPlaying() ) {
			if ( !pWaveOut->Close() ) {
				throw (LPTSTR)(LPCTSTR)pWaveOut->GetError();
			}
		}

		/// pVacInを再生成
		delete pVacIn;
		pVacIn = new CWaveIn;

		/// VACデバイスの指定
		SetInVacDevice(strDev);
		pVacIn->m_blPlayDev1 = (strPlayDev1 != _T("-1")) ? TRUE : FALSE;		// DefaultDeviceが空白の場合があるため
		pVacIn->m_blPlayDev2 = (strPlayDev2 != _T("-1")) ? TRUE : FALSE;
		pVacIn->m_strPlayDev1 = (pVacIn->m_blPlayDev1) ? strPlayDev1 : _T("");
		pVacIn->m_strPlayDev2 = (pVacIn->m_blPlayDev2) ? strPlayDev2 : _T("");

		/// VAC録音の開始
		CWave monWave;
		monWave.BuildFormat(fmt.nChannels, fmt.nFrequency, fmt.nBits, TRUE);
		if (!pVacInDev->IsInputFormat(monWave)) {
			throw _T("Not supported format");
		}
		pVacIn->m_blVac = TRUE;
		pVacIn->SetWaveFormat(monWave.GetFormat());
		pVacIn->SetDevice(*pVacInDev);
		if ( !pVacIn->Open() ) {
			throw (LPTSTR)(LPCTSTR)pVacIn->GetError();
		}

	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの録音開始（VACデバイス）
//  
//  機能説明 : 録音データをファイルに保存
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecordVac(CString strRecFile)
{
CFileSpec fs(strRecFile);
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordVac:%s"), fs.Extension());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	try {
		m_RecVacFile = strRecFile;
		if ( !pVacIn->Record(strRecFile) ) {
			throw (LPTSTR)(LPCTSTR)pVacIn->GetError();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの録音準備（VACデバイス）
//  
//  機能説明 : 録音データを音声入力デバイスに転送
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecordVac(CWaveIn *pSyncWaveIn)
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordVac"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	try {
		if ( !pVacIn->Record(pSyncWaveIn) ) {
			throw (LPTSTR)(LPCTSTR)pVacIn->GetError();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの録音一時停止（VACデバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecPauseVac(
BOOL blVirual)	// 仮想停止の使用フラグ
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecPauseVac"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	pVacIn->m_blVirtualPause = blVirual;

	try {
		if ( !pVacIn->Pause() ) {
			throw (LPTSTR)(LPCTSTR)pVacIn->GetError();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	m_blRecordPauseVac = TRUE;
	return TRUE;
}

//
//  機能     : 音声ファイルの録音一時停止の再開（VACデバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecContinueVac()
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecContinueVac"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	pVacIn->m_blVirtualPause = FALSE;		// 仮想停止を解除
	m_blRecordPauseVac = FALSE;

	try {
		if ( !pVacIn->Continue() ) {
			throw (LPTSTR)(LPCTSTR)pVacIn->GetError();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの録音停止（VACデバイス）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecStopVac() 
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecStopVac"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	pVacIn->m_blVirtualPause = FALSE;		// 仮想停止を解除
	m_blRecordPauseVac = FALSE;

	try {
		if ( !pVacIn->Close() ) {
			throw (LPTSTR)(LPCTSTR)pVacIn->GetError();
		}

//		if (pVacIn->GetNumSamples == 0) return FALSE;
//		CWave monWave = pVacIn->MakeWave();
//		monWave.Save(m_RecFile);
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの録音準備（Skypeネイティブ）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecordOpenSkype(ICallPtr pCall)
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordOpenSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	m_blRecordingSkype = FALSE;
	m_blRecordPauseSkype = FALSE;

	try {
		m_pCall = pCall;
		if (m_blPlayingSkype) {
			if ( !m_SkypeApi.PlayStop(m_pCall) ) {
				 return FALSE;
			}
		}

		/// pSocketInを再生成
		delete pSocketIn;
		pSocketIn = new CSocketIn;

		/// 音量調整の指定
		CRegDWORD rgVolSnd(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordVolSnd"), 15, TRUE);	// 初期値:15%
		CRegDWORD rgVolRcv(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordVolRcv"), 0, TRUE);		// 初期値:0%
		DWORD dwVolSnd = rgVolSnd;
		DWORD dwVolRcv = rgVolRcv;
		pSocketIn->SetVolAdjust((int)dwVolSnd, (int)dwVolRcv);

		/// 録音フォーマットの指定(初期値)
		// 16kHz, mono, 16bit 固定
		CWave monWave;
		monWave.BuildFormat(1, 16000, 16, FALSE);
		pSocketIn->SetWaveFormat(monWave.GetFormat());

	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの録音開始（Skypeネイティブ）
//  
//  機能説明 : 録音データをファイルに保存(ファイルに直接保存)
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecordSkype(
CString strRecFile,		// 保存先ファイル
CString strDevOut)		// 同時再生デバイス
{
CFileSpec fs(strRecFile);
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordSkype:%s"), fs.Extension());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {
		m_RecSkypeFile = strRecFile;

		// VoiceAPIによるSkype録音
		if ( !m_SkypeApi.Record(m_pCall, strRecFile, _T(""), strDevOut) ) {
			return FALSE;
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	m_blRecordingSkype = TRUE;
	m_blRecordPauseSkype = FALSE;
	return TRUE;
}

//
//  機能     : 音声ファイルの録音開始（Skypeネイティブ）
//  
//  機能説明 : 録音データをバッファに受信(PORT経由)
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecordSkype(
CString strRecFile,		// 保存先ファイル
CString strPortIn,		// 会話先音声受信ポート
CString strPortMic,		// マイク音声受信ポート
CString strDevOut)		// 同時再生デバイス
{
CFileSpec fs(strRecFile);
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordSkype2:%s"), fs.Extension());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {
		m_RecSkypeFile = strRecFile;

		// Socketをオープン
		UINT uiPortIn = _ttol(strPortIn);
		UINT uiPortMic = _ttol(strPortMic);
		pSocketIn->Open(uiPortIn, uiPortMic);

		// VoiceAPIによるSkype録音
		if ( !m_SkypeApi.Record(m_pCall, _T(""), strPortIn, strDevOut) ) {
			return FALSE;
		}
		if ( !m_SkypeApi.RecordMic(m_pCall, _T(""), strPortMic) ) {
			return FALSE;
		}

		// Port録音開始
		if ( !pSocketIn->Record(m_RecSkypeFile)) {
			return FALSE;
		}

	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	m_blRecordingSkype = TRUE;
	m_blRecordPauseSkype = FALSE;
	return TRUE;
}

//
//  機能     : 音声ファイルの録音一時停止（Skypeネイティブ）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecPauseSkype()
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecPauseSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {
		// Socketによる仮想的なSkype録音一時停止
		pSocketIn->Pause();

	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	m_blRecordPauseSkype = TRUE;
	return TRUE;
}

//
//  機能     : 音声ファイルの録音一時停止の再開（Skypeネイティブ）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecContinueSkype()
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecContinueSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	m_blRecordPauseSkype = FALSE;

	try {
		// Socketによる仮想的なSkype録音一時停止再開
		pSocketIn->Continue();

	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの録音停止（Skypeネイティブ）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::RecStopSkype() 
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecStopSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	m_blRecordingSkype = FALSE;
	m_blRecordPauseSkype = FALSE;

	try {
		// PORTによるデータ受信の中止
		if ( !pSocketIn->Stop() ) {
			return FALSE;
		}

		// VoiceAPIによるSkype録音停止
		if ( !m_SkypeApi.RecordStopMic(m_pCall) ) {
			return FALSE;
		}
		if ( !m_SkypeApi.RecordStop(m_pCall) ) {
			return FALSE;
		}

	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : Stream errのチェック有効/無効（Skypeネイティブ）
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
void CAudio::EnableStreamErrSkype(BOOL bl)
{
	pSocketIn->EnableStreamErr(bl);
}

//
//  機能     : 出力用デバイスのWAVEフォーマット取得
//  
//  機能説明 : 高音質からトライ
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAudio::FindFormat(BOOL blVac)
{
	fmt.nChannels = 0;
	fmt.nFrequency = 0;
	fmt.nBits = 0;

	WORD nChannels;
	DWORD nFrequency;
	WORD nBits;

	CWave monWave;
	CWaveDevice waveDev;
/* スカイプはモノラルのみ
	nChannels = 2; nFrequency = 48000; nBits = 16;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 2; nFrequency = 44100; nBits = 16;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 2; nFrequency = 22050; nBits = 16;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 2; nFrequency = 11025; nBits = 16;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 2; nFrequency = 8000; nBits = 16;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 2; nFrequency = 48000; nBits = 8;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 2; nFrequency = 44100; nBits = 8;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 2; nFrequency = 22050; nBits = 8;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 2; nFrequency = 11025; nBits = 8;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 2; nFrequency = 8000; nBits = 8;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
*/
	nChannels = 1; nFrequency = 48000; nBits = 16;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 1; nFrequency = 44100; nBits = 16;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 1; nFrequency = 22050; nBits = 16;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 1; nFrequency = 11025; nBits = 16;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 1; nFrequency = 8000; nBits = 16;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 1; nFrequency = 48000; nBits = 8;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 1; nFrequency = 44100; nBits = 8;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 1; nFrequency = 22050; nBits = 8;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 1; nFrequency = 11025; nBits = 8;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;
	nChannels = 1; nFrequency = 8000; nBits = 8;
	monWave.BuildFormat(nChannels, nFrequency, nBits, blVac);
	if ( waveDev.IsInputFormat(monWave) ) goto PROC_END;

	return FALSE;

PROC_END:
	fmt.nChannels = nChannels;
	fmt.nFrequency = nFrequency;
	fmt.nBits = nBits;
	return TRUE;
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
float CAudio::GetPlaySize(CString strFile)
{
	if (strFile.IsEmpty()) return -1;
	CFileSpec fs(strFile);
	if (!fs.Exist()) return -1;

	float fsz = 0.0;
	if (strFile.MakeLower().Right(4) == _T(".mp3")) {
		CMP3Info mp3;
		if (!mp3.Open(strFile)) return -1;
		fsz = (float)mp3.GetLength();
		// (以下は不正確)
		//TCHAR length[255];
		//CString strCmd;
		//strCmd.Format(_T("open \"%s\" alias tempSong"), strFile);
		//mciSendString(strCmd,
		//			NULL,
		//			0,
		//			NULL/*hWnd*/);
		//mciSendString(_T("status tempSong length"),
		//			length,
		//			255,
		//			NULL/*hWnd*/);
		//mciSendString(_T("close tempSong"), NULL, 0, NULL/*hWnd*/);
		//fsz = (float)_ttol(length) / (float)1000;
	} else {
		CWave wv;
		wv.LoadFormat(strFile, FALSE);
		WAVEFORMATEX wformat = wv.GetFormat();
		long usz = wformat.nAvgBytesPerSec;	// 1秒あたりのサイズ
		if (usz <= 0) return -1;
		long tsz = (long)fs.FileSize();
		fsz = (float)tsz/(float)usz;
	}

	return fsz;
}
float CAudio::GetPlaySize()
{
	return GetPlaySize(m_PlayFile);
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
BOOL CAudio::ChkPlayFile()
{
	if (m_PlayFile.IsEmpty()) return FALSE;
	CFileSpec fs(m_PlayFile);
	if (!fs.Exist()) return FALSE;

	CWave wv;
	wv.LoadFormat(m_PlayFile, FALSE);
	return pWaveOutDev->IsOutputFormat(wv);
}

//
//  機能     : WAVEファイルの修復
//  
//  機能説明 : ヘッダーの修正のみ
//  
//  返り値   : 1(修正済み), 0(修正なし)、-1(error)
//  
//  備考     : 
//  
int CAudio::RepairWaveFile(CString strFile)
{
	CFile f;
	CFileException e;
	int intRet = 0;

	if (strFile.GetLength() == 0) {
		return -1;
	}

	/// ファイルオープン
	if( !f.Open(strFile, CFile::modeReadWrite | CFile::shareDenyWrite, &e ) ) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), strFile, e.m_cause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
		return -1;
	}

	/// ヘッダの修正
	TRY {
		DWORD dwBuf = 0;
		DWORD dwFileSize = (DWORD)(f.GetLength() - 8);
		DWORD dwNum = dwFileSize - 36;
		f.Seek(4, CFile::begin);
		f.Read(&dwBuf, sizeof(dwFileSize));
		if (dwBuf != dwFileSize) {
			f.Seek(4, CFile::begin);
			f.Write(&dwFileSize, sizeof(dwFileSize));
			f.Flush();
			intRet = 1;
		}
		f.Seek(40, CFile::begin);
		f.Read(&dwBuf, sizeof(dwNum));
		if (dwBuf != dwNum) {
			f.Seek(40, CFile::begin);
			f.Write(&dwNum, sizeof(dwNum));
			f.Flush();
			intRet = 1;
		}
		f.Close();
	}
    CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("File header write error:%s, %s\n"), strFile, szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
		f.Close();
		return -1;
    }
	END_CATCH_ALL

	return intRet;
}
//BOOL CAudio::RepairWaveFile(CString strFile)
//{
//	CFile f;
//	CFileException e;
//	DWORD dwBuf = 0;
//	DWORD dwFileSize = 0;
//	DWORD dwNum = 0;
//
//	if (strFile.GetLength() == 0) {
//		return FALSE;
//	}
//
//	/// ヘッダの確認
//	if( !f.Open(strFile, CFile::modeRead | CFile::shareDenyNone, &e ) ) {
//		_stprintf_s(te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), strFile, e.m_cause);
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
//		return FALSE;
//	}
//
//	TRY {
//		dwFileSize = (DWORD)(f.GetLength() - 8);
//		dwNum = dwFileSize - 36;
//		f.Seek(4, CFile::begin);
//		f.Read(&dwBuf, 4);
//		if (dwBuf == dwFileSize) {
//			f.Seek(40, CFile::begin);
//			f.Read(&dwBuf, 4);
//			if (dwBuf == dwNum) {
//				// 修正不要
//				f.Close();
//				return TRUE;
//			}
//		}
//		f.Close();
//	}
//    CATCH_ALL(e)
//    {
//		_TCHAR   szCause[1024];
// 		e->GetErrorMessage(szCause, 1024);
//		_stprintf_s(te.errstr, MAX_ERRSTR, _T("File header write error:%s, %s\n"), strFile, szCause);
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
//		f.Close();
//		return FALSE;
//    }
//	END_CATCH_ALL
//
//	/// ヘッダの修正
//	if( !f.Open(strFile, CFile::modeWrite | CFile::shareDenyWrite, &e ) ) {
//		_stprintf_s(te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), strFile, e.m_cause);
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
//		return FALSE;
//	}
//
//	TRY {
//		f.Seek(4, CFile::begin);
//		f.Write(&dwFileSize, 4);
//		f.Seek(40, CFile::begin);
//		f.Write(&dwNum, 4);
//		f.Flush();
//		f.Close();
//	}
//    CATCH_ALL(e)
//    {
//		_TCHAR   szCause[1024];
// 		e->GetErrorMessage(szCause, 1024);
//		_stprintf_s(te.errstr, MAX_ERRSTR, _T("File header write error:%s, %s\n"), strFile, szCause);
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
//		f.Close();
//		return FALSE;
//    }
//	END_CATCH_ALL
//
//	return TRUE;
//}

//
//  機能     : WAVEファイルのフォーマット変更
//  
//  機能説明 : 処理モード=0:入力ファイルのフォーマットが出力ファイルと一致した場合、何もしない
//           : 処理モード=1:入力ファイルのフォーマットが出力ファイルと一致した場合、出力ファイルにコピー
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAudio::ChgWaveFormat(
CString strInFile,	// 入力ファイル
CString strOutFile,	// 出力ファイル
AUDIO_FORMAT fmt,	// 出力フォーマット
long lngMode)		// 処理モード
{
	int n;
	char *pBuf;
	CWaveFileACM wfIN,wfOUT;
#define	BUF_SIZE	1024*16

	try {
		// 入出力ファイル名設定
		if (strInFile.GetLength() <= 0 || strOutFile.GetLength() <= 0) return FALSE;
		CFileSpec fs(strInFile);
		if (!fs.Exist()) return FALSE;
	//#ifdef _UNICODE
	//	// wcstombs()は日本語を正常に処理しない
	//	DWORD dwLen;
	//	dwLen = ::WideCharToMultiByte(CP_ACP,0,strInFile,-1,NULL,0,NULL,NULL);
	//	::WideCharToMultiByte(CP_ACP, 0, strInFile, -1, wfIN.FileName, dwLen, NULL, NULL);
	//	dwLen = ::WideCharToMultiByte(CP_ACP,0,strOutFile,-1,NULL,0,NULL,NULL);
	//	::WideCharToMultiByte(CP_ACP, 0, strOutFile, -1, wfOUT.FileName, dwLen, NULL, NULL);
	//#else
	//	size_t sz;
	//	wcstombs_s(&sz, wfIN.FileName, sizeof(wfIN.FileName), CStringW(strInFile), MAX_PATH);
	//	wcstombs_s(&sz, wfOUT.FileName, sizeof(wfOUT.FileName), CStringW(strOutFile), MAX_PATH);
	//#endif
		_tcscpy_s(wfIN.FileName, MAX_PATH, strInFile);
		_tcscpy_s(wfOUT.FileName, MAX_PATH, strOutFile);

		// 入力ファイル
		if(!wfIN.Open(NULL)) return FALSE;
		/// フォーマット確認
		if (wfIN.wfex.nSamplesPerSec == fmt.nFrequency && 
			wfIN.wfex.wBitsPerSample == fmt.nBits && 
			wfIN.wfex.nChannels == fmt.nChannels) {
			wfIN.Close();
			//// 同一フォーマットの場合
			if (lngMode == 1) {
				///// そのままコピー
				fs.FileCopy(strOutFile, TRUE);
			} else {
				///// 何もしない
			}
			return TRUE;
		}

		// 出力ファイル
//	wfOUT.FormatChooseDlg();
		wfOUT.FormatChoose(fmt.nFrequency, fmt.nBits, fmt.nChannels);	// 出力フォーマット
		wfOUT.FormatSet(&(wfIN.wfex));	// 入力フォーマット
		if(!wfOUT.Create(NULL)) {
			wfIN.Close();
			return FALSE;
		}
		pBuf=(char *)GlobalAlloc(GMEM_FIXED,BUF_SIZE);
		if(pBuf==NULL) {
			wfOUT.Close();
			wfIN.Close();
			return FALSE;
		}

		do {
			n=wfIN.Read(pBuf,BUF_SIZE);
			if(n==-1) break;

			if(wfOUT.Write(pBuf,n)==-1) {
				n=-1;
				break;
			}
		} while(n==BUF_SIZE);

		GlobalFree((HGLOBAL)pBuf);
		wfOUT.Close();
		wfIN.Close();
	} catch(...) {
		if(pBuf!=NULL) GlobalFree((HGLOBAL)pBuf);
		wfOUT.Close();
		wfIN.Close();
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("wave format change error:%s->%s"), strInFile, strOutFile);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : FraunHoffer MP3 codecを使用するためレジストリを変更
//  
//  機能説明 : Vistaの場合のみ
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAudio::MP3CodecRegistry()	
{
	CString str;

	/// MP3 Codecファイルの存在チェック
	CFileSpec fs(FS_SYSDIR);	// system folder
	CString strCodec = fs.GetFolder();
	strCodec.Replace(_T("\\"), _T("\\\\"));
	strCodec += _T("l3codecp.acm");
	fs.SetFullSpec(strCodec);
	if (!fs.Exist()) {
		// MP3 codecがない
		str.LoadString(IDS_MSGBOX85);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	str.LoadString(IDS_MSGBOX87);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;

	/// レジストリファイルの生成
	CString strReg = theApp.m_strDataFolder + _T("\\mp3codec.reg");
	fs.SetFullSpec(strReg);
	str = _T("Windows Registry Editor Version 5.00\r\n\r\n");
	str += _T("[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Drivers32]\r\n");
	str += _T("\"msacm.l3acm\"=\"") + strCodec + _T("\"\r\n");
	if (!fs.SaveString(str, FALSE, TRUE)) return FALSE;

	/// レジストリに登録
	HINSTANCE hret = ShellExecute(NULL, NULL, (LPCTSTR)strReg,  NULL, NULL, SW_SHOWNORMAL);
	str.LoadString(IDS_MSGBOX801);
	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);

	return TRUE;
}
