// Audio.cpp: implementation of the CAudio class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tapur.h"
#include "Audio.h"
#include "FileSpec.h"
#include "TPsetup.h"
#include "WaveFileACM.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
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
//  �@�\     : Audio�N���X�̃C���X�^���X�쐬
//  
//  �@�\���� : �C���X�^���X�̓A�v���P�[�V�����S�̂ň�����������Ȃ��B
//  �@�@�@�@�@ ����ɂ��A�ݒ�ς݂̃v���p�e�B���V�X�e���S�̂ŋ��p�ł���B
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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

		_instance->m_pCall = NULL;			// SKYPE�^���Đ��p�R�[��
		_instance->m_blSkypeMode = FALSE;		// Skype�Đ��t���O(TRUE:Skype�Đ�, FALSE:�����f�o�C�X�Đ�)
		_instance->m_blPlayPause = FALSE;		// �Đ��ꎞ��~��ԃt���O
		_instance->m_blRecordPause = FALSE;	// �^���ꎞ��~��ԃt���O
		_instance->m_blRecordPauseVac = FALSE;// �^���ꎞ��~��ԃt���O(VAC)
		_instance->m_blPlayingSkype = FALSE;		// �Đ���ԃt���O(Skype)
		_instance->m_blPlayPauseSkype = FALSE;	// �Đ��ꎞ��~��ԃt���O(Skype)
		_instance->m_blRecordingSkype = FALSE;	// �^����ԃt���O(Skype)
		_instance->m_blRecordPauseSkype = FALSE;	// �^���ꎞ��~��ԃt���O(Skype)
	}
	return _instance;
}

//
//  �@�\     : Audio�N���X�̃C���X�^���X�̔j��
//  
//  �@�\���� : �A�v���P�[�V�����I�����ł̂݃R�[��
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : CVideo�N���X�̃��Z�b�g
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : VAC�f�o�C�X�̐ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::SetInVacDevice(
CString strDev)	// ���̓f�o�C�X��
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
//  �@�\     : ���̓f�o�C�X�̐ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : �Y�����Ȃ��ꍇ�͏����ݒ�iWAVE_WRAPPER�j�̂܂�
//  
BOOL CAudio::SetInDevice(
CString strDev)	// ���̓f�o�C�X��
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
//  �@�\     : �o�̓f�o�C�X�̐ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : �Y�����Ȃ��ꍇ�͏����ݒ�iWAVE_WRAPPER�j�̂܂�
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
//  �@�\     : ���̓f�o�C�X�̎擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �o�̓f�o�C�X�̎擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : WAVE���́�WAVE�o�̓��C���ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT��������Ȃ��ꍇ�A
//             MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED�Ń��g���C�i�f�o�C�X�h���C�o�̏�Q�H�j
//  
/*
BOOL CAudio::SetWave(
BOOL blOn)		// ON/OFF
{
	BOOL blFind;

	/// WAVE���́�WAVE�o��
	blFind = TRUE;
	// �ȉ��̏����͕ύX�s�i�s����ɂȂ�j
	if ( !m_Mixer.FindControl(MIXERLINE_COMPONENTTYPE_DST_WAVEIN, 
			MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT, MIXERCONTROL_CONTROLTYPE_MIXER) ) {
	if ( !m_Mixer.FindControl(MIXERLINE_COMPONENTTYPE_DST_WAVEIN, 
			MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED, MIXERCONTROL_CONTROLTYPE_MIXER) ) {
	//// WAVE���͂��Ȃ��ꍇPC�ł͈ȉ����L��
	// WAVE���͂�monoral Mixer�̗���������ꍇ�AWAVE���͂̂ݗL��
	if ( !m_Mixer.FindControl(MIXERLINE_COMPONENTTYPE_DST_WAVEIN, 
			MIXERLINE_COMPONENTTYPE_SRC_ANALOG, MIXERCONTROL_CONTROLTYPE_MIXER) ) {		// monoral Mixer
			blFind = FALSE;
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Not find Wave Mixer control"));
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	}
	}
	}
	if (blFind) {
		::Sleep(500);	// WAIT���K�v
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
//  �@�\     : Mic��ON/OFF
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::SetMic(
BOOL blOn)		// ON/OFF
{
	BOOL blFind;

	/// �}�C�N���́�WAVE�o��
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
//  �@�\     : �X�s�[�JON/OFF
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : �A
//  
BOOL CAudio::SetSpk(
BOOL blOn)		// ON/OFF
{
	BOOL blFind;

	/// �X�s�[�J�o��
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
//  �@�\     : �����t�@�C���̍Đ������i�W��/VAC�f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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

		/// pWaveOut���Đ���
		delete pWaveOut;
		pWaveOut = new CWaveOut;

		// �o�̓f�o�C�X
//		SetOutDevice(strDev);
		pWaveOut->m_blPlayDev1 = (strPlayDev1 != _T("-1")) ? TRUE : FALSE;		// DefaultDevice���󔒂̏ꍇ�����邽��
		pWaveOut->m_strPlayDev1 = (pWaveOut->m_blPlayDev1) ? strPlayDev1 : _T("");

		//  �X�s�[�JON
//		SetSpk(TRUE);

		/// �Đ��̊J�n
		pWaveOut->m_blVac = FALSE;
		if (m_PlayFile.MakeLower().Right(4) == _T(".mp3")) {
			//// MP3�Đ�
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
			//// WAV�Đ�
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
		pWaveOut->Close();	// �t�@�C�����[�h�̒��~
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  �@�\     : �����t�@�C���̍Đ��i�W��/VAC�f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::Play(
BOOL modFull)			// �t���Đ��A���荞�ݍĐ�
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
			// �Đ��̊����҂��i�Đ����ԁ{�P�b�j
			for (i = 0; i <= intLen*5; i++) {	// 0.2s x 5
				AfxGetThread()->PumpMessage();	// �K�{
				if(!pWaveOut->IsPlaying()) break;
				::Sleep(200);
			}
			pWaveOut->Close();	// �Đ��I��
		} else {
			if ( !pWaveOut->Play() ) {
				return FALSE;
			}
		}
	}
    catch(CException *e)
    {
		pWaveOut->Close();	// �t�@�C�����[�h�̒��~
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
//  �@�\     : �����t�@�C���̍Đ��ꎞ��~�i�W��/VAC�f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �����t�@�C���̍Đ��ꎞ��~�̍ĊJ�i�W��/VAC�f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �����t�@�C���̍Đ���~�i�W��/VAC�f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
	
		/// MIXER�ݒ��߂�
//		SetMixer(MIXER_IN_MIC | MIXER_IN_LINE | MIXER_OUT_SPK);

	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;

}
//
//  �@�\     : �����t�@�C���̍Đ������iSkype�l�C�e�B�u�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �����t�@�C���̍Đ��iSkype�l�C�e�B�u�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::PlaySkype(
BOOL modFull)			// �t���Đ�
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

		// VoiceAPI�ɂ��Skype�Đ�
		if ( !m_SkypeApi.Play(m_pCall, m_PlayFile) ) {
			return FALSE;
		}

		if (modFull) {
			// �Đ��̊����҂��i�Đ����ԁ{�P�b�j
			for (i = 0; i <= intLen*5; i++) {	// 0.2s x 5
				AfxGetThread()->PumpMessage();	// �K�{
				::Sleep(200);
			}
			m_SkypeApi.PlayStop(m_pCall);	// �Đ��I��
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
//  �@�\     : �����t�@�C���̍Đ��ꎞ��~�iSkype�l�C�e�B�u�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �����t�@�C���̍Đ��ꎞ��~�̍ĊJ�iSkype�l�C�e�B�u�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �����t�@�C���̍Đ���~�iSkype�l�C�e�B�u�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::PlayStopSkype() 
{
	m_blPlayingSkype = FALSE;
	m_blPlayPauseSkype = FALSE;
_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayStopSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {

		// VoiceAPI�ɂ��Skype�Đ���~
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
//  �@�\     : �����t�@�C���̘^�������i�W���f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecordOpen(
CString strDev,			// ���̓f�o�C�X��
CString strPlayDev1,		// ���A���^�C���Đ��p�f�o�C�X��1
CString strPlayDev2)		// ���A���^�C���Đ��p�f�o�C�X��2
{
	m_blRecordPause = FALSE;
	pWaveIn->m_blVirtualPause = FALSE;		// ���z��~������
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordOpen"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {
		if ( pWaveOut->IsPlaying() ) {
			if ( !pWaveOut->Close() ) {
				throw (LPTSTR)(LPCTSTR)pWaveOut->GetError();
			}
		}

		/// pWaveIn���Đ���
		delete pWaveIn;
		pWaveIn = new CWaveIn;

		/// �f�o�C�X�̎w��
		SetInDevice(strDev);
		pWaveIn->m_blPlayDev1 = (strPlayDev1 != _T("-1")) ? TRUE : FALSE;		// DefaultDevice���󔒂̏ꍇ�����邽��
		pWaveIn->m_blPlayDev2 = (strPlayDev2 != _T("-1")) ? TRUE : FALSE;
		pWaveIn->m_strPlayDev1 = (pWaveIn->m_blPlayDev1) ? strPlayDev1 : _T("");
		pWaveIn->m_strPlayDev2 = (pWaveIn->m_blPlayDev2) ? strPlayDev2 : _T("");

		/// �^���̊J�n
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
//  �@�\     : �����t�@�C���̘^���J�n�i�W���f�o�C�X�j
//  
//  �@�\���� : �^���f�[�^���t�@�C���ɕۑ�
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �����t�@�C���̘^�������i�W���f�o�C�X�j
//  
//  �@�\���� : �^���f�[�^���������̓f�o�C�X�ɓ]��
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �����o�b�t�@�t���Ř^�����̘^���ΏۃX�C�b�`
//  
//  �@�\���� : 0:���o�b�t�@�Ɠ����o�b�t�@�̘^���A1:���o�b�t�@�̂݁A2:�����o�b�t�@�̂�
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAudio::SetRecordSyncSw(int intSw)
{
	//pVacIn->SetSyncSw(intSw);
	pSocketIn->SetSyncSw(intSw);

	return TRUE;
}

//
//  �@�\     : �����o�b�t�@�t���Ř^�����̘^������
//  
//  �@�\���� : 0:���o�b�t�@�Ɠ����o�b�t�@�̍����^��(monoral)
//             1:�X�e���I�^��(���S�p��)
//             2:�X�e���I�^��(�����p��)
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAudio::SetRecordSyncStereo(int intVal)
{
	//pVacIn->SetSyncStereo(intVal);
	pSocketIn->SetSyncStereo(intVal);

	return TRUE;
}

//
//  �@�\     : �^�����̃t�H�[�}�b�g
//  
//  �@�\���� : ���݂�MP3�^�����̂ݗL���AWAV�^�����̓t�H�[�}�b�g�Œ�
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAudio::SetRecordFormat(
int intFormatType,		// 0:WAV, 1:MP3
CString strCodec,		// Codec����
CString strFormat)		// Format����
{
	return pSocketIn->SetFormat(intFormatType, strCodec, strFormat);
}

//
//  �@�\     : �����t�@�C���̘^���ꎞ��~�i�W���f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecPause(
BOOL blVirual)	// ���z��~�̎g�p�t���O 
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
//  �@�\     : �����t�@�C���̘^���ꎞ��~�̍ĊJ�i�W���f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecContinue()
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecContinue"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	pWaveIn->m_blVirtualPause = FALSE;		// ���z��~������
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
//  �@�\     : �����t�@�C���̘^����~�i�W���f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecStop() 
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecStop"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	pWaveIn->m_blVirtualPause = FALSE;		// ���z��~������
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
//  �@�\     : �����t�@�C���̘^�������iVAC�f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecordOpenVac(
CString strDev,			// ���̓f�o�C�X��
CString strPlayDev1,		// ���A���^�C���Đ��p�f�o�C�X��1
CString strPlayDev2)		// ���A���^�C���Đ��p�f�o�C�X��2
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordOpenVac"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	pVacIn->m_blVirtualPause = FALSE;		// ���z��~������
	m_blRecordPauseVac = FALSE;

	try {
		if ( pWaveOut->IsPlaying() ) {
			if ( !pWaveOut->Close() ) {
				throw (LPTSTR)(LPCTSTR)pWaveOut->GetError();
			}
		}

		/// pVacIn���Đ���
		delete pVacIn;
		pVacIn = new CWaveIn;

		/// VAC�f�o�C�X�̎w��
		SetInVacDevice(strDev);
		pVacIn->m_blPlayDev1 = (strPlayDev1 != _T("-1")) ? TRUE : FALSE;		// DefaultDevice���󔒂̏ꍇ�����邽��
		pVacIn->m_blPlayDev2 = (strPlayDev2 != _T("-1")) ? TRUE : FALSE;
		pVacIn->m_strPlayDev1 = (pVacIn->m_blPlayDev1) ? strPlayDev1 : _T("");
		pVacIn->m_strPlayDev2 = (pVacIn->m_blPlayDev2) ? strPlayDev2 : _T("");

		/// VAC�^���̊J�n
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
//  �@�\     : �����t�@�C���̘^���J�n�iVAC�f�o�C�X�j
//  
//  �@�\���� : �^���f�[�^���t�@�C���ɕۑ�
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �����t�@�C���̘^�������iVAC�f�o�C�X�j
//  
//  �@�\���� : �^���f�[�^���������̓f�o�C�X�ɓ]��
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �����t�@�C���̘^���ꎞ��~�iVAC�f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecPauseVac(
BOOL blVirual)	// ���z��~�̎g�p�t���O
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
//  �@�\     : �����t�@�C���̘^���ꎞ��~�̍ĊJ�iVAC�f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecContinueVac()
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecContinueVac"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	pVacIn->m_blVirtualPause = FALSE;		// ���z��~������
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
//  �@�\     : �����t�@�C���̘^����~�iVAC�f�o�C�X�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecStopVac() 
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecStopVac"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	pVacIn->m_blVirtualPause = FALSE;		// ���z��~������
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
//  �@�\     : �����t�@�C���̘^�������iSkype�l�C�e�B�u�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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

		/// pSocketIn���Đ���
		delete pSocketIn;
		pSocketIn = new CSocketIn;

		/// ���ʒ����̎w��
		CRegDWORD rgVolSnd(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordVolSnd"), 15, TRUE);	// �����l:15%
		CRegDWORD rgVolRcv(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordVolRcv"), 0, TRUE);		// �����l:0%
		DWORD dwVolSnd = rgVolSnd;
		DWORD dwVolRcv = rgVolRcv;
		pSocketIn->SetVolAdjust((int)dwVolSnd, (int)dwVolRcv);

		/// �^���t�H�[�}�b�g�̎w��(�����l)
		// 16kHz, mono, 16bit �Œ�
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
//  �@�\     : �����t�@�C���̘^���J�n�iSkype�l�C�e�B�u�j
//  
//  �@�\���� : �^���f�[�^���t�@�C���ɕۑ�(�t�@�C���ɒ��ڕۑ�)
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecordSkype(
CString strRecFile,		// �ۑ���t�@�C��
CString strDevOut)		// �����Đ��f�o�C�X
{
CFileSpec fs(strRecFile);
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordSkype:%s"), fs.Extension());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {
		m_RecSkypeFile = strRecFile;

		// VoiceAPI�ɂ��Skype�^��
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
//  �@�\     : �����t�@�C���̘^���J�n�iSkype�l�C�e�B�u�j
//  
//  �@�\���� : �^���f�[�^���o�b�t�@�Ɏ�M(PORT�o�R)
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecordSkype(
CString strRecFile,		// �ۑ���t�@�C��
CString strPortIn,		// ��b�批����M�|�[�g
CString strPortMic,		// �}�C�N������M�|�[�g
CString strDevOut)		// �����Đ��f�o�C�X
{
CFileSpec fs(strRecFile);
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordSkype2:%s"), fs.Extension());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {
		m_RecSkypeFile = strRecFile;

		// Socket���I�[�v��
		UINT uiPortIn = _ttol(strPortIn);
		UINT uiPortMic = _ttol(strPortMic);
		pSocketIn->Open(uiPortIn, uiPortMic);

		// VoiceAPI�ɂ��Skype�^��
		if ( !m_SkypeApi.Record(m_pCall, _T(""), strPortIn, strDevOut) ) {
			return FALSE;
		}
		if ( !m_SkypeApi.RecordMic(m_pCall, _T(""), strPortMic) ) {
			return FALSE;
		}

		// Port�^���J�n
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
//  �@�\     : �����t�@�C���̘^���ꎞ��~�iSkype�l�C�e�B�u�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecPauseSkype()
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecPauseSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	try {
		// Socket�ɂ�鉼�z�I��Skype�^���ꎞ��~
		pSocketIn->Pause();

	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	m_blRecordPauseSkype = TRUE;
	return TRUE;
}

//
//  �@�\     : �����t�@�C���̘^���ꎞ��~�̍ĊJ�iSkype�l�C�e�B�u�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecContinueSkype()
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecContinueSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
	m_blRecordPauseSkype = FALSE;

	try {
		// Socket�ɂ�鉼�z�I��Skype�^���ꎞ��~�ĊJ
		pSocketIn->Continue();

	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

//
//  �@�\     : �����t�@�C���̘^����~�iSkype�l�C�e�B�u�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::RecStopSkype() 
{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecStopSkype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);

	m_blRecordingSkype = FALSE;
	m_blRecordPauseSkype = FALSE;

	try {
		// PORT�ɂ��f�[�^��M�̒��~
		if ( !pSocketIn->Stop() ) {
			return FALSE;
		}

		// VoiceAPI�ɂ��Skype�^����~
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
//  �@�\     : Stream err�̃`�F�b�N�L��/�����iSkype�l�C�e�B�u�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
void CAudio::EnableStreamErrSkype(BOOL bl)
{
	pSocketIn->EnableStreamErr(bl);
}

//
//  �@�\     : �o�͗p�f�o�C�X��WAVE�t�H�[�}�b�g�擾
//  
//  �@�\���� : ����������g���C
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
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
/* �X�J�C�v�̓��m�����̂�
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
//  �@�\     : �t�@�C���̉��t�T�C�Y�̎擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : �b���A-1:error
//  
//  ���l     : 
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
		// (�ȉ��͕s���m)
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
		long usz = wformat.nAvgBytesPerSec;	// 1�b������̃T�C�Y
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
//  �@�\     : �t�@�C���̃t�H�[�}�b�g�`�F�b�N
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
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
//  �@�\     : WAVE�t�@�C���̏C��
//  
//  �@�\���� : �w�b�_�[�̏C���̂�
//  
//  �Ԃ�l   : 1(�C���ς�), 0(�C���Ȃ�)�A-1(error)
//  
//  ���l     : 
//  
int CAudio::RepairWaveFile(CString strFile)
{
	CFile f;
	CFileException e;
	int intRet = 0;

	if (strFile.GetLength() == 0) {
		return -1;
	}

	/// �t�@�C���I�[�v��
	if( !f.Open(strFile, CFile::modeReadWrite | CFile::shareDenyWrite, &e ) ) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), strFile, e.m_cause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
		return -1;
	}

	/// �w�b�_�̏C��
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
//	/// �w�b�_�̊m�F
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
//				// �C���s�v
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
//	/// �w�b�_�̏C��
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
//  �@�\     : WAVE�t�@�C���̃t�H�[�}�b�g�ύX
//  
//  �@�\���� : �������[�h=0:���̓t�@�C���̃t�H�[�}�b�g���o�̓t�@�C���ƈ�v�����ꍇ�A�������Ȃ�
//           : �������[�h=1:���̓t�@�C���̃t�H�[�}�b�g���o�̓t�@�C���ƈ�v�����ꍇ�A�o�̓t�@�C���ɃR�s�[
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAudio::ChgWaveFormat(
CString strInFile,	// ���̓t�@�C��
CString strOutFile,	// �o�̓t�@�C��
AUDIO_FORMAT fmt,	// �o�̓t�H�[�}�b�g
long lngMode)		// �������[�h
{
	int n;
	char *pBuf;
	CWaveFileACM wfIN,wfOUT;
#define	BUF_SIZE	1024*16

	try {
		// ���o�̓t�@�C�����ݒ�
		if (strInFile.GetLength() <= 0 || strOutFile.GetLength() <= 0) return FALSE;
		CFileSpec fs(strInFile);
		if (!fs.Exist()) return FALSE;
	//#ifdef _UNICODE
	//	// wcstombs()�͓��{��𐳏�ɏ������Ȃ�
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

		// ���̓t�@�C��
		if(!wfIN.Open(NULL)) return FALSE;
		/// �t�H�[�}�b�g�m�F
		if (wfIN.wfex.nSamplesPerSec == fmt.nFrequency && 
			wfIN.wfex.wBitsPerSample == fmt.nBits && 
			wfIN.wfex.nChannels == fmt.nChannels) {
			wfIN.Close();
			//// ����t�H�[�}�b�g�̏ꍇ
			if (lngMode == 1) {
				///// ���̂܂܃R�s�[
				fs.FileCopy(strOutFile, TRUE);
			} else {
				///// �������Ȃ�
			}
			return TRUE;
		}

		// �o�̓t�@�C��
//	wfOUT.FormatChooseDlg();
		wfOUT.FormatChoose(fmt.nFrequency, fmt.nBits, fmt.nChannels);	// �o�̓t�H�[�}�b�g
		wfOUT.FormatSet(&(wfIN.wfex));	// ���̓t�H�[�}�b�g
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
//  �@�\     : FraunHoffer MP3 codec���g�p���邽�߃��W�X�g����ύX
//  
//  �@�\���� : Vista�̏ꍇ�̂�
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAudio::MP3CodecRegistry()	
{
	CString str;

	/// MP3 Codec�t�@�C���̑��݃`�F�b�N
	CFileSpec fs(FS_SYSDIR);	// system folder
	CString strCodec = fs.GetFolder();
	strCodec.Replace(_T("\\"), _T("\\\\"));
	strCodec += _T("l3codecp.acm");
	fs.SetFullSpec(strCodec);
	if (!fs.Exist()) {
		// MP3 codec���Ȃ�
		str.LoadString(IDS_MSGBOX85);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	str.LoadString(IDS_MSGBOX87);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;

	/// ���W�X�g���t�@�C���̐���
	CString strReg = theApp.m_strDataFolder + _T("\\mp3codec.reg");
	fs.SetFullSpec(strReg);
	str = _T("Windows Registry Editor Version 5.00\r\n\r\n");
	str += _T("[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Drivers32]\r\n");
	str += _T("\"msacm.l3acm\"=\"") + strCodec + _T("\"\r\n");
	if (!fs.SaveString(str, FALSE, TRUE)) return FALSE;

	/// ���W�X�g���ɓo�^
	HINSTANCE hret = ShellExecute(NULL, NULL, (LPCTSTR)strReg,  NULL, NULL, SW_SHOWNORMAL);
	str.LoadString(IDS_MSGBOX801);
	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);

	return TRUE;
}
