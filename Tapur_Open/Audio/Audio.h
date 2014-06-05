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
	CAudio() {} // <- ���̃N���X�̃C���X�^���X����}�~
	~CAudio() {}
public:
	static CAudio *Instance();
    static void Instance_Delete();

private:
	CTPerror te;
// Wave Class�̎g�p
	CWaveDevice *pWaveInDev;
	CWaveOut *pWaveOut;
	CWaveDevice *pWaveOutDev;
	CWaveDevice *pVacInDev;
// Mixer Class�̎g�p
	CMixer m_Mixer;
// SkypeAPI Class�̎g�p
	CSkypeApi m_SkypeApi;
	ICallPtr m_pCall;			// SKYPE�^���Đ��p�R�[��

	BOOL m_blSkypeMode;		// Skype�Đ��t���O(TRUE:Skype�Đ�, FALSE:�����f�o�C�X�Đ�)

	BOOL m_blPlayPause;		// �Đ��ꎞ��~��ԃt���O
//	BOOL m_blPlayPauseVac;	// �Đ��ꎞ��~��ԃt���O(VAC)
	BOOL m_blRecordPause;	// �^���ꎞ��~��ԃt���O
	BOOL m_blRecordPauseVac;// �^���ꎞ��~��ԃt���O(VAC)

	BOOL m_blPlayingSkype;		// �Đ���ԃt���O(Skype)
	BOOL m_blPlayPauseSkype;	// �Đ��ꎞ��~��ԃt���O(Skype)
	BOOL m_blRecordingSkype;	// �^����ԃt���O(Skype)
	BOOL m_blRecordPauseSkype;	// �^���ꎞ��~��ԃt���O(Skype)

	BOOL SetMic(BOOL blOn);
	BOOL SetSpk(BOOL blOn);

public:
	AUDIO_FORMAT fmt;
	CWaveIn *pWaveIn;
	CWaveIn *pVacIn;
// Socket Class�̎g�p
	CSocketIn *pSocketIn;
	CString	m_PlayFile;		// �Đ��t�@�C����
	CString	m_RecFile;		// �^���t�@�C����
	CString	m_RecVacFile;	// VAC�^���t�@�C����
	CString	m_RecSkypeFile;	// SKYPE�^���t�@�C����
	void Reset();
	float GetPlaySize();	// �t�@�C���̉��t�T�C�Y
	float GetPlaySize(CString strFile);	// �t�@�C���̉��t�T�C�Y
	BOOL ChkPlayFile();		// �t�@�C���̃t�H�[�}�b�g�`�F�b�N
	BOOL BuildFormat(BOOL blVac);		// �o�͗pWAVE�t�H�[�}�b�g�̐ݒ�
	BOOL FindFormat(BOOL blVac);		// �o�͗pWAVE�t�H�[�}�b�g�̓���
	int RepairWaveFile(CString strFile);	// WAVE�t�@�C���̏C��
	BOOL ChgWaveFormat(CString strInFile, CString strOutFile, AUDIO_FORMAT fmt, long lngMode);	// WAVE�t�@�C���̃t�H�[�}�b�g�ύX
	BOOL MP3CodecRegistry();	// FraunHoffer MP3 codec���g�p���邽�߃��W�X�g����ύX
	// �ۑ��I�v�V����
	BOOL SetRecordSyncSw(int intSw);
	BOOL SetRecordSyncStereo(int intVal);
	BOOL SetRecordFormat(int intFormatType, CString strCodec, CString strFormat);	// �^�����̃t�H�[�}�b�g
	BOOL SetInVacDevice(CString strDev);
	BOOL SetInDevice(CString strDev);
	BOOL SetOutDevice(CString strDev);
	CString GetInDevice(int intNum = 0);
	CString GetOutDevice(int intNum = 0);
	// �ʏ�Đ�
	BOOL PlayOpen(CString strPlayDev = _T("-1"));
	BOOL Play(BOOL modFull = false);
	BOOL PlayPause();
	BOOL PlayContinue();
	BOOL PlayStop();
	BOOL IsPlaying() { return pWaveOut->IsPlaying(); }
	BOOL IsPlayPause() { return m_blPlayPause; }
	// SKYPE�Đ�
	BOOL PlayOpenSkype(ICallPtr pCall);
	BOOL PlaySkype(BOOL modFull = false);
	BOOL PlayPauseSkype();
	BOOL PlayContinueSkype();
	BOOL PlayStopSkype();
	BOOL IsPlayingSkype() { return m_blPlayingSkype; }
	BOOL IsPlayPauseSkype() { return m_blPlayPauseSkype; }
	// �ʏ�^��
	BOOL RecordOpen(CString strDev, CString strPlayDev1 = _T("-1"), CString strPlayDev2 = _T("-1"));
	BOOL Record(CString	strRecFile);
	BOOL Record(CWaveIn	*pSyncWaveIn);
	BOOL RecPause(BOOL blVirual);
	BOOL RecContinue();
	BOOL RecStop();
	BOOL IsRecording() { return pWaveIn->IsRecording(); }
	BOOL IsRecordPause() { return m_blRecordPause; }
	// VAC�^��
	BOOL RecordOpenVac(CString strDev, CString strPlayDev1 = _T("-1"), CString strPlayDev2 = _T("-1"));
	BOOL RecordVac(CString	strRecFile);
	BOOL RecordVac(CWaveIn	*pSyncWaveIn);
	BOOL RecPauseVac(BOOL blVirual);
	BOOL RecContinueVac();
	BOOL RecStopVac();
	BOOL IsRecordingVac() { return pVacIn->IsRecording(); }
	BOOL IsRecordPauseVac() { return m_blRecordPauseVac; }
	// SKYPE�^��
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
