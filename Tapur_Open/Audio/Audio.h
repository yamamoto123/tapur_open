// Audio.h: interface for the CAudio class.
//
// define as Singleton Patern
//////////////////////////////////////////////////////////////////////

#if !defined(_AUDIO_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
#define _AUDIO_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TPerror.h"
#include "Wave.h"
#include "WaveIn.h"
#include "WaveOut.h"
#include "WaveInterface.h"
#include "SocketIn.h"
#include "Mixer.h"
#include "AcmMgr.h"
#include "SkypeApi.h"

// MIxer type
#define	MIXER_IN_LINE	0x0001
#define	MIXER_IN_MIC	0x0002
#define	MIXER_OUT_LINE	0x0004
#define	MIXER_OUT_SPK	0x0008

typedef struct {
	WORD nChannels;		// channel
	DWORD nFrequency;	// sample rate
	WORD nBits;			// bit
} AUDIO_FORMAT;

class CAudio
{
private:
	static CAudio *_instance;
protected:
	CAudio() {} // <- このクラスのインスタンス化を抑止
	~CAudio() {}
public:
	static CAudio *Instance();
    static void Instance_Delete();

private:
	CTPerror te;
// Wave Classの使用
	CWaveDevice *pWaveInDev;
	CWaveOut *pWaveOut;
	CWaveDevice *pWaveOutDev;
	CWaveDevice *pVacInDev;
// Mixer Classの使用
	CMixer m_Mixer;
// SkypeAPI Classの使用
	CSkypeApi m_SkypeApi;
	ICallPtr m_pCall;			// SKYPE録音再生用コール

	BOOL m_blSkypeMode;		// Skype再生フラグ(TRUE:Skype再生, FALSE:音声デバイス再生)

	BOOL m_blPlayPause;		// 再生一時停止状態フラグ
//	BOOL m_blPlayPauseVac;	// 再生一時停止状態フラグ(VAC)
	BOOL m_blRecordPause;	// 録音一時停止状態フラグ
	BOOL m_blRecordPauseVac;// 録音一時停止状態フラグ(VAC)

	BOOL m_blPlayingSkype;		// 再生状態フラグ(Skype)
	BOOL m_blPlayPauseSkype;	// 再生一時停止状態フラグ(Skype)
	BOOL m_blRecordingSkype;	// 録音状態フラグ(Skype)
	BOOL m_blRecordPauseSkype;	// 録音一時停止状態フラグ(Skype)

	BOOL SetMic(BOOL blOn);
	BOOL SetSpk(BOOL blOn);

public:
	AUDIO_FORMAT fmt;
	CWaveIn *pWaveIn;
	CWaveIn *pVacIn;
// Socket Classの使用
	CSocketIn *pSocketIn;
	CString	m_PlayFile;		// 再生ファイル名
	CString	m_RecFile;		// 録音ファイル名
	CString	m_RecVacFile;	// VAC録音ファイル名
	CString	m_RecSkypeFile;	// SKYPE録音ファイル名
	void Reset();
	float GetPlaySize();	// ファイルの演奏サイズ
	float GetPlaySize(CString strFile);	// ファイルの演奏サイズ
	BOOL ChkPlayFile();		// ファイルのフォーマットチェック
	BOOL BuildFormat(BOOL blVac);		// 出力用WAVEフォーマットの設定
	BOOL FindFormat(BOOL blVac);		// 出力用WAVEフォーマットの特定
	int RepairWaveFile(CString strFile);	// WAVEファイルの修復
	BOOL ChgWaveFormat(CString strInFile, CString strOutFile, AUDIO_FORMAT fmt, long lngMode);	// WAVEファイルのフォーマット変更
	BOOL MP3CodecRegistry();	// FraunHoffer MP3 codecを使用するためレジストリを変更
	// 保存オプション
	BOOL SetRecordSyncSw(int intSw);
	BOOL SetRecordSyncStereo(int intVal);
	BOOL SetRecordFormat(int intFormatType, CString strCodec, CString strFormat);	// 録音時のフォーマット
	BOOL SetInVacDevice(CString strDev);
	BOOL SetInDevice(CString strDev);
	BOOL SetOutDevice(CString strDev);
	CString GetInDevice(int intNum = 0);
	CString GetOutDevice(int intNum = 0);
	// 通常再生
	BOOL PlayOpen(CString strPlayDev = _T("-1"));
	BOOL Play(BOOL modFull = false);
	BOOL PlayPause();
	BOOL PlayContinue();
	BOOL PlayStop();
	BOOL IsPlaying() { return pWaveOut->IsPlaying(); }
	BOOL IsPlayPause() { return m_blPlayPause; }
	// SKYPE再生
	BOOL PlayOpenSkype(ICallPtr pCall);
	BOOL PlaySkype(BOOL modFull = false);
	BOOL PlayPauseSkype();
	BOOL PlayContinueSkype();
	BOOL PlayStopSkype();
	BOOL IsPlayingSkype() { return m_blPlayingSkype; }
	BOOL IsPlayPauseSkype() { return m_blPlayPauseSkype; }
	// 通常録音
	BOOL RecordOpen(CString strDev, CString strPlayDev1 = _T("-1"), CString strPlayDev2 = _T("-1"));
	BOOL Record(CString	strRecFile);
	BOOL Record(CWaveIn	*pSyncWaveIn);
	BOOL RecPause(BOOL blVirual);
	BOOL RecContinue();
	BOOL RecStop();
	BOOL IsRecording() { return pWaveIn->IsRecording(); }
	BOOL IsRecordPause() { return m_blRecordPause; }
	// VAC録音
	BOOL RecordOpenVac(CString strDev, CString strPlayDev1 = _T("-1"), CString strPlayDev2 = _T("-1"));
	BOOL RecordVac(CString	strRecFile);
	BOOL RecordVac(CWaveIn	*pSyncWaveIn);
	BOOL RecPauseVac(BOOL blVirual);
	BOOL RecContinueVac();
	BOOL RecStopVac();
	BOOL IsRecordingVac() { return pVacIn->IsRecording(); }
	BOOL IsRecordPauseVac() { return m_blRecordPauseVac; }
	// SKYPE録音
	BOOL RecordOpenSkype(ICallPtr pCall);
	BOOL RecordSkype(CString strRecFile, CString strDevOut);
	BOOL RecordSkype(CString strRecFile, CString strPortIn, CString strPortMic, CString strDevOut);
	BOOL RecPauseSkype();
	BOOL RecContinueSkype();
	BOOL RecStopSkype();
	BOOL IsRecordingSkype() { return m_blRecordingSkype; }
	BOOL IsRecordPauseSkype() { return m_blRecordPauseSkype; }
	void EnableStreamErrSkype(BOOL bl);
};

#endif // !defined(_AUDIO_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
