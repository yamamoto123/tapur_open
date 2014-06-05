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
        delete _instance;
        _instance = 0 ;
    }
}

//////////////////////////////////////////////////////////////////////

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
//  �@�\     : VAC�f�o�C�X�̐ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::SetOutVacDevice(
CString strDev)	// ���̓f�o�C�X��
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
			_stprintf(errstr, _T("Not find Wave Mixer control"));
			ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, errstr);
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
//  �@�\     : �����t�@�C���̍Đ�
//  
//  �@�\���� : �X�s�[�J�o��
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::PlayOpen(			
CString strPlayDev1)		// ���A���^�C���Đ��p�f�o�C�X��1
{
	blPlayPause = FALSE;

	try {
		if ( waveOut.IsPlaying() ) {
			if ( !waveOut.Close() ) {
				throw (LPTSTR)(LPCTSTR)waveOut.GetError();
			}
		}

		// �o�̓f�o�C�X
//		SetOutDevice(strDev);
		waveOut.m_blPlayDev1 = (strPlayDev1 != _T("-1")) ? TRUE : FALSE;		// DefaultDevice���󔒂̏ꍇ�����邽��
		waveOut.m_strPlayDev1 = (waveOut.m_blPlayDev1) ? strPlayDev1 : _T("");

		//  �X�s�[�JON
//		SetSpk(TRUE);

		/// �Đ��̊J�n
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
//  �@�\     : �����t�@�C���̍Đ�
//  
//  �@�\���� : �X�s�[�J�o��
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::Play(
BOOL modFull)			// �t���Đ��A���荞�ݍĐ�
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
			// �Đ��̊����҂��i�ő�30min�j
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
	
		/// MIXER�ݒ��߂�
//		SetMixer(MIXER_IN_MIC | MIXER_IN_LINE | MIXER_OUT_SPK);

	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}

/*
//
//  �@�\     : �V�X�e��BEEP���̍Đ�(VAC)
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::PlayBeepVac(
long lngSecond)			// �Đ�����(���g�p)
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
//  �@�\     : �����t�@�C���̍Đ�(VAC)
//  
//  �@�\���� : ���C���o��
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::PlayOpenVac(
CString strDev)			// ���̓f�o�C�X��
{

	blPlayPauseVac = FALSE;

	try {
		if ( vacOut.IsPlaying() ) {
			if ( !vacOut.Close() ) {
				throw (LPTSTR)(LPCTSTR)vacOut.GetError();
			}
		}

		// VAC�f�o�C�X�̎w��
		SetOutVacDevice(strDev);

		/// VAC�Đ��̊J�n
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
//  �@�\     : �����t�@�C���̍Đ�(VAC)
//  
//  �@�\���� : ���C���o��
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAudio::PlayVac(
BOOL modFull)			// �t���Đ��A���荞�ݍĐ�
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
			// �Đ��̊����҂��i�ő�30min�j
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
	
		/// MIXER�ݒ��߂�
//		SetMixer(MIXER_IN_MIC | MIXER_IN_LINE | MIXER_OUT_SPK);

	} catch(LPTSTR err) {
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, err);
		return FALSE;
	}

	return TRUE;
}
*/

BOOL CAudio::RecordOpen(
CString strDev,			// ���̓f�o�C�X��
CString strPlayDev1,		// ���A���^�C���Đ��p�f�o�C�X��1
CString strPlayDev2)		// ���A���^�C���Đ��p�f�o�C�X��2
{
	blRecordPause = FALSE;
	waveIn.m_blVirtualPause = FALSE;		// ���z��~������

	try {
		if ( waveOut.IsPlaying() ) {
			if ( !waveOut.Close() ) {
				throw (LPTSTR)(LPCTSTR)waveOut.GetError();
			}
		}

//		SetMixer(MIXER_IN_MIC | MIXER_IN_LINE | MIXER_OUT_LINE | MIXER_OUT_SPK);

		/// �f�o�C�X�̎w��
		SetInDevice(strDev);
		waveIn.m_blPlayDev1 = (strPlayDev1 != _T("-1")) ? TRUE : FALSE;		// DefaultDevice���󔒂̏ꍇ�����邽��
		waveIn.m_blPlayDev2 = (strPlayDev2 != _T("-1")) ? TRUE : FALSE;
		waveIn.m_strPlayDev1 = (waveIn.m_blPlayDev1) ? strPlayDev1 : _T("");
		waveIn.m_strPlayDev2 = (waveIn.m_blPlayDev2) ? strPlayDev2 : _T("");

		/// �^���̊J�n
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
	vacIn.SetSyncSw(intSw);
	return TRUE;
}

//
//  �@�\     : �����o�b�t�@�t���Ř^�����̘^������
//  
//  �@�\���� : false:���o�b�t�@�Ɠ����o�b�t�@�̍����^��(monoral)�Atrue:�X�e���I�^��
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAudio::SetRecordSyncStereo(BOOL blVal)
{
	vacIn.SetSyncStereo(blVal);
	return TRUE;
}

BOOL CAudio::RecPause(
BOOL blVirual)	// ���z��~�̎g�p�t���O 
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
	waveIn.m_blVirtualPause = FALSE;		// ���z��~������
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
	waveIn.m_blVirtualPause = FALSE;		// ���z��~������
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
CString strDev,			// ���̓f�o�C�X��
CString strPlayDev1,		// ���A���^�C���Đ��p�f�o�C�X��1
CString strPlayDev2)		// ���A���^�C���Đ��p�f�o�C�X��2
{
	vacIn.m_blVirtualPause = FALSE;		// ���z��~������
	blRecordPauseVac = FALSE;

	try {
		if ( waveOut.IsPlaying() ) {
			if ( !waveOut.Close() ) {
				throw (LPTSTR)(LPCTSTR)waveOut.GetError();
			}
		}

//		SetMixer(MIXER_IN_MIC | MIXER_OUT_LINE | MIXER_OUT_SPK);

		/// VAC�f�o�C�X�̎w��
		SetInVacDevice(strDev);
		vacIn.m_blPlayDev1 = (strPlayDev1 != _T("-1")) ? TRUE : FALSE;		// DefaultDevice���󔒂̏ꍇ�����邽��
		vacIn.m_blPlayDev2 = (strPlayDev2 != _T("-1")) ? TRUE : FALSE;
		vacIn.m_strPlayDev1 = (vacIn.m_blPlayDev1) ? strPlayDev1 : _T("");
		vacIn.m_strPlayDev2 = (vacIn.m_blPlayDev2) ? strPlayDev2 : _T("");

		/// VAC�^���̊J�n
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
BOOL blVirual)	// ���z��~�̎g�p�t���O
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
	vacIn.m_blVirtualPause = FALSE;		// ���z��~������
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
	vacIn.m_blVirtualPause = FALSE;		// ���z��~������
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
float CAudio::GetPlaySize()
{
	if (m_PlayFile.IsEmpty()) return -1;
	CFileSpec fs(m_PlayFile);
	if (!fs.Exist()) return -1;

	CWave wv;
	wv.LoadFormat(m_PlayFile, FALSE);
	WAVEFORMATEX wformat = wv.GetFormat();
	long usz = wformat.nAvgBytesPerSec;	// 1�b������̃T�C�Y
	if (usz <= 0) return -1;
	long tsz = fs.FileSize();

	return ((float)tsz/(float)usz);
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
	return waveOutDev.IsOutputFormat(wv);
}

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
	#ifdef _UNICODE
		// wcstombs()�͓��{��𐳏�ɏ������Ȃ�
		DWORD dwLen;
		dwLen = ::WideCharToMultiByte(CP_ACP,0,strInFile,-1,NULL,0,NULL,NULL);
		::WideCharToMultiByte(CP_ACP, 0, strInFile, -1, wfIN.FileName, dwLen, NULL, NULL);
		dwLen = ::WideCharToMultiByte(CP_ACP,0,strOutFile,-1,NULL,0,NULL,NULL);
		::WideCharToMultiByte(CP_ACP, 0, strOutFile, -1, wfOUT.FileName, dwLen, NULL, NULL);
	#else
		wcstombs(wfIN.FileName, strInFile, MAX_PATH);
		wcstombs(wfOUT.FileName, strOutFile, MAX_PATH);
	#endif

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
		_stprintf(errstr, _T("wave format change error:%s->%s"), strInFile, strOutFile);
		ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_WAVE, errstr);
		return FALSE;
	}

	return TRUE;
}
