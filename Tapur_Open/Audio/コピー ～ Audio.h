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
#include "Mixer.h"

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

class CAudio : private virtual CTPerror
{
private:
	static CAudio *_instance;
protected:
	CAudio() {} // <- ���̃N���X�̃C���X�^���X����}�~
public:
	static CAudio *Instance();
    static void Instance_Delete();

private:
// Wave Class�̎g�p
	CWaveDevice waveInDev;
	CWaveOut waveOut;
	CWaveDevice waveOutDev;
	CWaveDevice vacInDev;
//	CWaveOut vacOut;		
//	CWaveDevice vacOutDev;	
// Mixer Class�̎g�p
	CMixer m_Mixer;

	BOOL blPlayPause;	// �Đ��ꎞ��~��ԃt���O
//	BOOL blPlayPauseVac;	// �Đ��ꎞ��~��ԃt���O(VAC)
	BOOL blRecordPause;	// �^���ꎞ��~��ԃt���O
	BOOL blRecordPauseVac;	// �^���ꎞ��~��ԃt���O(VAC)

//	BOOL SetWave(BOOL blOn);
	BOOL SetMic(BOOL blOn);
	BOOL SetSpk(BOOL blOn);

public:
	AUDIO_FORMAT fmt;
	CWaveIn waveIn;
	CWaveIn vacIn;
	CString	m_PlayFile;		// �Đ��t�@�C����
	CString	m_RecFile;		// �^���t�@�C����
	CString	m_RecVacFile;	// VAC�^���t�@�C����
	BOOL SetInVacDevice(CString strDev);
	BOOL SetOutVacDevice(CString strDev);
	BOOL SetInDevice(CString strDev);
	BOOL SetOutDevice(CString strDev);
	BOOL PlayOpen(CString strPlayDev = _T("-1"));
	BOOL Play(BOOL modFull = false);
	BOOL PlayPause();
	BOOL PlayContinue();
	BOOL PlayStop();
	BOOL IsPlaying() { return waveOut.IsPlaying(); }
	BOOL IsPlayPause() { return blPlayPause; }
/*
	BOOL PlayOpenVac(CString strDev);
	BOOL PlayVac(BOOL modFull = false);
	BOOL PlayPauseVac();
	BOOL PlayContinueVac();
	BOOL PlayStopVac();
	BOOL PlayBeepVac(long lngSecond);
	BOOL IsPlayingVac() { return vacOut.IsPlaying(); }
	BOOL IsPlayPauseVac() { return blPlayPauseVac; }
*/
	BOOL RecordOpen(CString strDev, CString strPlayDev1 = _T("-1"), CString strPlayDev2 = _T("-1"));
	BOOL Record(CString	strRecFile);
	BOOL Record(CWaveIn	*pSyncWaveIn);
	BOOL SetRecordSyncSw(int intSw);
	BOOL SetRecordSyncStereo(BOOL blVal);
	BOOL RecPause(BOOL blVirual);
	BOOL RecContinue();
	BOOL RecStop();
	BOOL IsRecording() { return waveIn.IsRecording(); }
	BOOL IsRecordPause() { return blRecordPause; }
	BOOL RecordOpenVac(CString strDev, CString strPlayDev1 = _T("-1"), CString strPlayDev2 = _T("-1"));
	BOOL RecordVac(CString	strRecFile);
	BOOL RecordVac(CWaveIn	*pSyncWaveIn);
	BOOL RecPauseVac(BOOL blVirual);
	BOOL RecContinueVac();
	BOOL RecStopVac();
	BOOL IsRecordingVac() { return vacIn.IsRecording(); }
	BOOL IsRecordPauseVac() { return blRecordPauseVac; }
	float GetPlaySize();	// �t�@�C���̉��t�T�C�Y
	BOOL ChkPlayFile();		// �t�@�C���̃t�H�[�}�b�g�`�F�b�N
	BOOL BuildFormat(BOOL blVac);		// �o�͗pWAVE�t�H�[�}�b�g�̐ݒ�
	BOOL FindFormat(BOOL blVac);		// �o�͗pWAVE�t�H�[�}�b�g�̓���
	BOOL ChgWaveFormat(CString strInFile, CString strOutFile, AUDIO_FORMAT fmt, long lngMode);	// WAVE�t�@�C���̃t�H�[�}�b�g�ύX
};

#endif // !defined(_AUDIO_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
