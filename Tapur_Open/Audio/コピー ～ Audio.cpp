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
        delete _instance;
        _instance = 0 ;
    }
}

//////////////////////////////////////////////////////////////////////

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
					vacInDev.SetDevice(i);
					vacIn.SetDevice(vacInDev);
					break;
				}
			}
		}
	} catch(...) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, _T("Input device not found."));
		return FALSE;
	}

	return TRUE;
}

/*
//
//  機能     : VACデバイスの設定
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::SetOutVacDevice(
CString strDev)	// 入力デバイス名
{
	UINT i = 0;
	CString strWavDevice;

	try {
		for (i = 0; i < CWaveInterface::GetWaveInCount(); i++) {
			strWavDevice = CWaveInterface::GetWaveInName(i);
			if (!strWavDevice.IsEmpty()) {
				if (strWavDevice.CompareNoCase(strDev) == 0 ) {
					vacOutDev.SetDevice(i);
					vacOut.SetDevice(vacOutDev);
					break;
				}
			}
		}
	} catch(...) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, _T("Input device not found."));
		return FALSE;
	}

	return TRUE;
}
*/

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
			waveInDev.SetDevice(WAVE_MAPPER);
			waveIn.SetDevice(waveInDev);
			return TRUE;
		}

		for (i = 0; i < CWaveInterface::GetWaveInCount(); i++) {
			strWavDevice = CWaveInterface::GetWaveInName(i);
TRACE(_T("%s\n"), strWavDevice);
			if (!strWavDevice.IsEmpty()) {
				if (strWavDevice.CompareNoCase(strDev) == 0 ) {
					waveInDev.SetDevice(i);
					waveIn.SetDevice(waveInDev);
					break;
				}
			}
		}
	} catch(...) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, _T("Input device not found."));
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
			waveOutDev.SetDevice(WAVE_MAPPER);
			waveOut.SetDevice(waveOutDev);
			return TRUE;
		}

		for (i = 0; i < CWaveInterface::GetWaveOutCount(); i++) {
			strWavDevice = CWaveInterface::GetWaveOutName(i);
TRACE(_T("%s\n"), strWavDevice);
			if (!strWavDevice.IsEmpty()) {
				if (strWavDevice.CompareNoCase(strDev) == 0 ) {
					waveOutDev.SetDevice(i);
					waveOut.SetDevice(waveOutDev);
					break;
				}
			}
		}
	} catch(...) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, _T("Output device not found."));
		return FALSE;
	}

	return TRUE;
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
			_stprintf(errstr, _T("Not find Wave Mixer control"));
			ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, errstr);
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
			_stprintf(errstr, _T("Not find Mic control"));
			ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, errstr);
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
			_stprintf(errstr, _T("Not find Speaker control"));
			ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, errstr);
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
//  機能     : 音声ファイルの再生
//  
//  機能説明 : スピーカ出力
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlayOpen(			
CString strPlayDev1)		// リアルタイム再生用デバイス名1
{
	blPlayPause = FALSE;

	try {
		if ( waveOut.IsPlaying() ) {
			if ( !waveOut.Close() ) {
				throw (LPTSTR)(LPCTSTR)waveOut.GetError();
			}
		}

		// 出力デバイス
//		SetOutDevice(strDev);
		waveOut.m_blPlayDev1 = (strPlayDev1 != _T("-1")) ? TRUE : FALSE;		// DefaultDeviceが空白の場合があるため
		waveOut.m_strPlayDev1 = (waveOut.m_blPlayDev1) ? strPlayDev1 : _T("");

		//  スピーカON
//		SetSpk(TRUE);

		/// 再生の開始
		if ( !waveOut.m_wave.LoadFormat(m_PlayFile, FALSE)) return FALSE;
		if ( !waveOutDev.IsOutputFormat(waveOut.m_wave) ) return FALSE;
		if ( !waveOut.m_wave.Load(m_PlayFile, FALSE)) return FALSE;
		waveOut.m_blVac = FALSE;
		waveOut.SetPlayFormat(fmt.nChannels, fmt.nFrequency, fmt.nBits);
		waveOut.SetDevice(waveOutDev);
		if ( !waveOut.Open() ) {
			throw (LPTSTR)(LPCTSTR)waveOut.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの再生
//  
//  機能説明 : スピーカ出力
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::Play(
BOOL modFull)			// フル再生、割り込み再生
{
	int i;

	blPlayPause = FALSE;

	try {
		if ( m_PlayFile.GetLength() <= 0 )
		{
			throw _T("Play file not found.");
		}

		if (modFull) {
			if ( !waveOut.Play() ) {
//			if ( !waveOut.FullPlay() ) {
				throw (LPTSTR)(LPCTSTR)waveOut.GetError();
			}
			// 再生の完了待ち（最大30min）
			for (i = 0; i < (30*60*2); i++) {
				if(!waveOut.IsPlaying()) break;
				::Sleep(500);
			}
		} else {
			if ( !waveOut.Play() ) {
				return FALSE;
			}
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

BOOL CAudio::PlayPause() 
{
	try {
		if ( !waveOut.Pause() ) {
			throw (LPTSTR)(LPCTSTR)waveOut.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	blPlayPause = TRUE;
	return TRUE;
}

BOOL CAudio::PlayContinue() 
{
	blPlayPause = FALSE;

	try {
		if ( !waveOut.Continue() ) {
			throw (LPTSTR)(LPCTSTR)waveOut.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

BOOL CAudio::PlayStop() 
{
	blPlayPause = FALSE;

	try {
		if ( !waveOut.Close() ) {
			throw (LPTSTR)(LPCTSTR)waveOut.GetError();
		}
	
		/// MIXER設定を戻す
//		SetMixer(MIXER_IN_MIC | MIXER_IN_LINE | MIXER_OUT_SPK);

	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

/*
//
//  機能     : システムBEEP音の再生(VAC)
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlayBeepVac(
long lngSecond)			// 再生時間(未使用)
{
	CTPsetup tps(theApp.m_strDataFolder);
	CString strFile = tps.RsfBeepFileName();
	CFileSpec fs(strFile);
	if (!fs.Exist()) return FALSE;

	m_PlayFile = fs.GetFullSpec();
	PlayOpenVac(VAC_OUT_DEV1);
	if (!PlayVac(TRUE)) {	// full play now
		return FALSE;
	}
	PlayStopVac();
	return TRUE;
}

//
//  機能     : 音声ファイルの再生(VAC)
//  
//  機能説明 : ライン出力
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlayOpenVac(
CString strDev)			// 入力デバイス名
{

	blPlayPauseVac = FALSE;

	try {
		if ( vacOut.IsPlaying() ) {
			if ( !vacOut.Close() ) {
				throw (LPTSTR)(LPCTSTR)vacOut.GetError();
			}
		}

		// VACデバイスの指定
		SetOutVacDevice(strDev);

		/// VAC再生の開始
		CWave monWave;
		if ( !monWave.Load(m_PlayFile, FALSE)) return FALSE;
//		if ( !monWave.Load(m_PlayFile, TRUE)) return FALSE;
		if ( !vacOutDev.IsOutputFormat(monWave) ) {
			return FALSE;
		}
		vacOut.m_blVac = TRUE;
		vacOut.SetDevice(vacOutDev);
		if ( !vacOut.Open() ) {
			throw (LPTSTR)(LPCTSTR)vacOut.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 音声ファイルの再生(VAC)
//  
//  機能説明 : ライン出力
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAudio::PlayVac(
BOOL modFull)			// フル再生、割り込み再生
{
	int i;

	blPlayPauseVac = FALSE;

	try {
		if ( m_PlayFile.GetLength() <= 0 )
		{
			throw _T("Play file not found.");
		}

		if (modFull) {
			if ( !vacOut.FullPlay() ) {
				throw (LPTSTR)(LPCTSTR)vacOut.GetError();
			}
			// 再生の完了待ち（最大30min）
			for (i = 0; i < (30*60*2); i++) {
				if(!vacOut.IsPlaying()) break;
				::Sleep(500);
			}
		} else {
			if ( !vacOut.Play() ) {
				return FALSE;
			}
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

BOOL CAudio::PlayPauseVac() 
{
	try {
		if ( !vacOut.Pause() ) {
			throw (LPTSTR)(LPCTSTR)vacOut.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	blPlayPause = TRUE;
	return TRUE;
}

BOOL CAudio::PlayContinueVac() 
{
	blPlayPauseVac = FALSE;

	try {
		if ( !vacOut.Continue() ) {
			throw (LPTSTR)(LPCTSTR)vacOut.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

BOOL CAudio::PlayStopVac() 
{
	blPlayPauseVac = FALSE;

	try {
		if ( !vacOut.Close() ) {
			throw (LPTSTR)(LPCTSTR)vacOut.GetError();
		}
	
		/// MIXER設定を戻す
//		SetMixer(MIXER_IN_MIC | MIXER_IN_LINE | MIXER_OUT_SPK);

	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}
*/

BOOL CAudio::RecordOpen(
CString strDev,			// 入力デバイス名
CString strPlayDev1,		// リアルタイム再生用デバイス名1
CString strPlayDev2)		// リアルタイム再生用デバイス名2
{
	blRecordPause = FALSE;
	waveIn.m_blVirtualPause = FALSE;		// 仮想停止を解除

	try {
		if ( waveOut.IsPlaying() ) {
			if ( !waveOut.Close() ) {
				throw (LPTSTR)(LPCTSTR)waveOut.GetError();
			}
		}

//		SetMixer(MIXER_IN_MIC | MIXER_IN_LINE | MIXER_OUT_LINE | MIXER_OUT_SPK);

		/// デバイスの指定
		SetInDevice(strDev);
		waveIn.m_blPlayDev1 = (strPlayDev1 != _T("-1")) ? TRUE : FALSE;		// DefaultDeviceが空白の場合があるため
		waveIn.m_blPlayDev2 = (strPlayDev2 != _T("-1")) ? TRUE : FALSE;
		waveIn.m_strPlayDev1 = (waveIn.m_blPlayDev1) ? strPlayDev1 : _T("");
		waveIn.m_strPlayDev2 = (waveIn.m_blPlayDev2) ? strPlayDev2 : _T("");

		/// 録音の開始
		CWave monWave;
		monWave.BuildFormat(fmt.nChannels, fmt.nFrequency, fmt.nBits, FALSE);
		if (!waveInDev.IsInputFormat(monWave)) {
			throw _T("Format non support");
		}
		waveIn.m_blVac = FALSE;
		waveIn.SetWaveFormat(monWave.GetFormat());
		waveIn.SetDevice(waveInDev);
		if ( !waveIn.Open() ) {
			throw (LPTSTR)(LPCTSTR)waveIn.GetError();
		}

	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

BOOL CAudio::Record(CString strRecFile)
{
	try {
		m_RecFile = strRecFile;
		if ( !waveIn.Record(strRecFile) ) {
			throw (LPTSTR)(LPCTSTR)waveIn.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

BOOL CAudio::Record(CWaveIn *pSyncWaveIn)
{
	try {
		if ( !waveIn.Record(pSyncWaveIn) ) {
			throw (LPTSTR)(LPCTSTR)waveIn.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
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
	vacIn.SetSyncSw(intSw);
	return TRUE;
}

//
//  機能     : 同期バッファ付きで録音時の録音音質
//  
//  機能説明 : false:自バッファと同期バッファの合成録音(monoral)、true:ステレオ録音
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAudio::SetRecordSyncStereo(BOOL blVal)
{
	vacIn.SetSyncStereo(blVal);
	return TRUE;
}

BOOL CAudio::RecPause(
BOOL blVirual)	// 仮想停止の使用フラグ 
{
	waveIn.m_blVirtualPause = blVirual;

	try {
		if ( !waveIn.Pause() ) {
			throw (LPTSTR)(LPCTSTR)waveIn.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	blRecordPause = TRUE;
	return TRUE;
}

BOOL CAudio::RecContinue()
{
	waveIn.m_blVirtualPause = FALSE;		// 仮想停止を解除
	blRecordPause = FALSE;

	try {
		if ( !waveIn.Continue() ) {
			throw (LPTSTR)(LPCTSTR)waveIn.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

BOOL CAudio::RecStop() 
{
	waveIn.m_blVirtualPause = FALSE;		// 仮想停止を解除
	blRecordPause = FALSE;

	try {
		if ( !waveIn.Close() ) {
			throw (LPTSTR)(LPCTSTR)waveIn.GetError();
		}

//		if (!blSave) return TRUE;
//		if (waveIn.GetNumSamples == 0) return FALSE;
//		CWave monWave = waveIn.MakeWave();
//		monWave.Save(m_RecFile);
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

BOOL CAudio::RecordOpenVac(
CString strDev,			// 入力デバイス名
CString strPlayDev1,		// リアルタイム再生用デバイス名1
CString strPlayDev2)		// リアルタイム再生用デバイス名2
{
	vacIn.m_blVirtualPause = FALSE;		// 仮想停止を解除
	blRecordPauseVac = FALSE;

	try {
		if ( waveOut.IsPlaying() ) {
			if ( !waveOut.Close() ) {
				throw (LPTSTR)(LPCTSTR)waveOut.GetError();
			}
		}

//		SetMixer(MIXER_IN_MIC | MIXER_OUT_LINE | MIXER_OUT_SPK);

		/// VACデバイスの指定
		SetInVacDevice(strDev);
		vacIn.m_blPlayDev1 = (strPlayDev1 != _T("-1")) ? TRUE : FALSE;		// DefaultDeviceが空白の場合があるため
		vacIn.m_blPlayDev2 = (strPlayDev2 != _T("-1")) ? TRUE : FALSE;
		vacIn.m_strPlayDev1 = (vacIn.m_blPlayDev1) ? strPlayDev1 : _T("");
		vacIn.m_strPlayDev2 = (vacIn.m_blPlayDev2) ? strPlayDev2 : _T("");

		/// VAC録音の開始
		CWave monWave;
		monWave.BuildFormat(fmt.nChannels, fmt.nFrequency, fmt.nBits, TRUE);
		if (!vacInDev.IsInputFormat(monWave)) {
			throw _T("Not supported format");
		}
		vacIn.m_blVac = TRUE;
		vacIn.SetWaveFormat(monWave.GetFormat());
		vacIn.SetDevice(vacInDev);
		if ( !vacIn.Open() ) {
			throw (LPTSTR)(LPCTSTR)vacIn.GetError();
		}

	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

BOOL CAudio::RecordVac(CString strRecFile)
{
	try {
		m_RecVacFile = strRecFile;
		if ( !vacIn.Record(strRecFile) ) {
			throw (LPTSTR)(LPCTSTR)vacIn.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

BOOL CAudio::RecordVac(CWaveIn *pSyncWaveIn)
{
	try {
		if ( !vacIn.Record(pSyncWaveIn) ) {
			throw (LPTSTR)(LPCTSTR)vacIn.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

BOOL CAudio::RecPauseVac(
BOOL blVirual)	// 仮想停止の使用フラグ
{
	vacIn.m_blVirtualPause = blVirual;

	try {
		if ( !vacIn.Pause() ) {
			throw (LPTSTR)(LPCTSTR)vacIn.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	blRecordPauseVac = TRUE;
	return TRUE;
}

BOOL CAudio::RecContinueVac()
{
	vacIn.m_blVirtualPause = FALSE;		// 仮想停止を解除
	blRecordPauseVac = FALSE;

	try {
		if ( !vacIn.Continue() ) {
			throw (LPTSTR)(LPCTSTR)vacIn.GetError();
		}
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

BOOL CAudio::RecStopVac() 
{
	vacIn.m_blVirtualPause = FALSE;		// 仮想停止を解除
	blRecordPauseVac = FALSE;

	try {
		if ( !vacIn.Close() ) {
			throw (LPTSTR)(LPCTSTR)vacIn.GetError();
		}

//		if (vacIn.GetNumSamples == 0) return FALSE;
//		CWave monWave = vacIn.MakeWave();
//		monWave.Save(m_RecFile);
	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
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
float CAudio::GetPlaySize()
{
	if (m_PlayFile.IsEmpty()) return -1;
	CFileSpec fs(m_PlayFile);
	if (!fs.Exist()) return -1;

	CWave wv;
	wv.LoadFormat(m_PlayFile, FALSE);
	WAVEFORMATEX wformat = wv.GetFormat();
	long usz = wformat.nAvgBytesPerSec;	// 1秒あたりのサイズ
	if (usz <= 0) return -1;
	long tsz = fs.FileSize();

	return ((float)tsz/(float)usz);
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
	return waveOutDev.IsOutputFormat(wv);
}

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
	#ifdef _UNICODE
		// wcstombs()は日本語を正常に処理しない
		DWORD dwLen;
		dwLen = ::WideCharToMultiByte(CP_ACP,0,strInFile,-1,NULL,0,NULL,NULL);
		::WideCharToMultiByte(CP_ACP, 0, strInFile, -1, wfIN.FileName, dwLen, NULL, NULL);
		dwLen = ::WideCharToMultiByte(CP_ACP,0,strOutFile,-1,NULL,0,NULL,NULL);
		::WideCharToMultiByte(CP_ACP, 0, strOutFile, -1, wfOUT.FileName, dwLen, NULL, NULL);
	#else
		wcstombs(wfIN.FileName, strInFile, MAX_PATH);
		wcstombs(wfOUT.FileName, strOutFile, MAX_PATH);
	#endif

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
		_stprintf(errstr, _T("wave format change error:%s->%s"), strInFile, strOutFile);
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, errstr);
		return FALSE;
	}

	return TRUE;
}
