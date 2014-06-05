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

// ���z�J�����t�B���^�[�f�o�C�X��
#define TVC_IN_DEV _T("Tapur Virtual Camera")

class CVideo
{
private:
	CTPerror te;
	static CVideo *_instance;
protected:
	CVideo() {} // <- ���̃N���X�̃C���X�^���X����}�~
public:
	static CVideo *Instance();
    static void Instance_Delete();

private:
	CVideoDialog		m_dlgVideo;		// Video Dialog
	CVideoGrabberVC6	*m_pVideoGrab;	// VideoGrabber
	HWND m_hWndTarge;		// �^��ΏۃE�B���h�E
	CString	m_PlayFile;		// �Đ��t�@�C����
	CString	m_RecFile;		// �^��t�@�C����
//	BOOL m_blPlayPause;	// �Đ��ꎞ��~��ԃt���O
	BOOL blPlayingSkype;// �X�g���[���Đ���ԃt���O
	BOOL blRecording;	// �^���ԃt���O
	BOOL m_blCompress;			// ���k�ۃt���O
	CString	m_strCompress;		// ���k�`����
	WAVEFORMATEX m_WaveFormat;	// �����iNULL�̏ꍇ�A���Ȃ��j
	CPtrList *m_pAudioBuffer;		// �I�[�f�B�I�X�g���[���o�b�t�@�Q�Ɨp�|�C���^

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
		// PAUSE�̏ꍇGetIsPlayerPlaying()��FALSE�ɂȂ邽�߁A�ȉ��̂悤�ɋL�q
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
	float GetPlaySize();	// �t�@�C���̉��t�T�C�Y
	float GetPlaySize(CString strPlayFile);
	float GetFps();			// �t�@�C����frame/sec
	float GetFps(CString strPlayFile);
	BOOL ChkPlayFile();		// �t�@�C���̃t�H�[�}�b�g�`�F�b�N
	BOOL ChkPlayFile(CString strPlayFile);
	HWND GetVideoWinLocal(CString strSkypeId, LPRECT pRect);	// �r�f�I��ʃT�C�Y
	HWND GetVideoWinLarge(LPRECT pRect);	// �r�f�I��ʃT�C�Y
	BOOL SetVideoWinLarge(LPRECT pRect);	// �r�f�I��ʃT�C�Y�ݒ�
};

#endif // !defined(_VIDEO_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
