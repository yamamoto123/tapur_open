// Video.cpp: implementation of the CVideo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tapur.h"
#include "Video.h"
#include "VideoDialog.h"
#include "FileSpec.h"

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

CVideo *CVideo::_instance = 0;

//
//  �@�\     : CVideo�N���X�̃C���X�^���X�쐬
//  
//  �@�\���� : �C���X�^���X�̓A�v���P�[�V�����S�̂ň�����������Ȃ��B
//  �@�@�@�@�@ ����ɂ��A�ݒ�ς݂̃v���p�e�B���V�X�e���S�̂ŋ��p�ł���B
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
CVideo *CVideo::Instance()
{
	if (_instance == 0) {
		_instance = new CVideo;

		_instance->blRecording = FALSE;
		_instance->blPlayingSkype = FALSE;
		_instance->m_PlayFile = _T("");
		_instance->m_RecFile = _T("");

		/// VideoGrabber�Q�Ɨp��Video Dialog�𐶐��i�\���͂��Ȃ��j
		_instance->m_dlgVideo.Create(IDD_VIDEO, NULL);
		_instance->m_pVideoGrab = &(_instance->m_dlgVideo.m_VideoGrab);

		_instance->m_pScreenCap = new CScreenCap(AfxGetMainWnd());
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
void CVideo::Instance_Delete()
{
    if (_instance != 0) {
		delete _instance->m_pScreenCap;

		/// Video Dialog��j��
		_instance->m_pVideoGrab = NULL;
		_instance->m_dlgVideo.DestroyWindow();

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
void CVideo::Reset()
{
	blRecording = FALSE;
	blPlayingSkype = FALSE;
	m_PlayFile = _T("");
	m_RecFile = _T("");
}

//
//  �@�\     : �r�f�I�t�@�C���̍Đ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CVideo::PlayOpen()
{
//	m_blPlayPause = FALSE;
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("PlayOpen"));

	try {
		// �r�f�I��ʃ_�C�A���O�̕\��
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
//  �@�\     : �r�f�I�t�@�C���̍Đ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
		m_pVideoGrab->SetPlayerAudioRendering(FALSE);	// Audio�Đ������Ȃ�
		m_pVideoGrab->SetAutoStartPlayer(FALSE);		// OpenPlayer�ɂ�鎩���Đ����֎~(PAUSE�̂���)
		if ( !m_pVideoGrab->OpenPlayer() ) {
			// Win7+DivX�̊���VideoGrabber�����삵�Ȃ����G���[�łȂ�WMP�Đ��ő�p
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

		// �r�f�I��ʃ_�C�A���O�̔�\��
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
//  �@�\     : �r�f�I�t�@�C����Skype�X�g���[���J�n
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CVideo::PlayOpenSkype(
CString strPath)	// ��ƃt�H���_
{
	try {
		// ��ƃt�H���_���̃t�@�C���폜
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

		/// �r�f�I��ʃ_�C�A���O�̔�\��
		m_dlgVideo.ShowWindow(SW_HIDE);

		/// �Đ��X�g���[���̍�ƃt�H���_�w��
		m_dlgVideo.SetStreamPath(strPath);

		/// VideoGrabber�ݒ�
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
//  �@�\     : �r�f�I�t�@�C����Skype�X�g���[���Đ�
//  
//  �@�\���� : �t���[����A����������B���ۂ̍Đ���tapsrc.ax�t�B���^�ōs��
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CVideo::PlaySkype(
CString strPlayFile)	// �Đ��t�@�C��
{
	blPlayingSkype = TRUE;

	try {
		m_PlayFile = strPlayFile;
		if ( m_PlayFile.GetLength() <= 0 )
		{
			throw _T("Play video file not found.");
		}		
		
		/// �Đ��t�@�C����fps���擾
		int intFps = (int)GetFps();
		if (intFps <= 0) {
			// �s���̏ꍇ�f�t�H���g�l
			CRegDWORD rgFps(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordFps"), 5, TRUE);	// �����l:5(fps)
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
//  �@�\     : �r�f�I�t�@�C����Skype�X�g���[���Đ��ꎞ��~
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �r�f�I�t�@�C����Skype�X�g���[���Đ��ꎞ��~�ĊJ
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �r�f�I�t�@�C����Skype�X�g���[���Đ��I��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : ���k�t�B���^���X�g
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
void CVideo::GetCompressors(CStringList *pStrList)
{
	m_pScreenCap->ListCompressFilter(pStrList);
}

//
//  �@�\     : �^�撆���ۂ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �^��J�n����
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CVideo::RecordOpen(CString strSkypeId)
{
//	m_blRecordPause = FALSE;
_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordOpen Skype"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

	/// SkypeID or Name
	m_dlgVideo.SetSkypeId(strSkypeId);

	/// �r�f�I��ʂ�O�ʕ\�����邩�H
	CRegDWORD rgVSetTop(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordSetTop"), 1, TRUE);	// �����l:1(True)
	m_dlgVideo.m_blVSetTop = (rgVSetTop == 1) ? TRUE : FALSE;

	try {
		/// Capture�̈�̎擾
		/// ���̗̈�T�C�Y����ɂ���
		CRect rect;
		HWND hWnd = NULL;
		hWnd = m_dlgVideo.GetVideoWndLarge(rect);
		if (hWnd) {
		//// Skype�r�f�I����Window�ŋN��
			if (m_dlgVideo.m_blVSetTop) {
				m_dlgVideo.SetTopVideoWndLocal(FALSE);
				m_dlgVideo.SetTopVideoWndLarge(TRUE);
			}
			m_hWndTarge = hWnd;
			return TRUE;
		} 
		hWnd = m_dlgVideo.GetVideoWndLocal(rect);
		if (hWnd) {
		//// Skype�r�f�I���N��
			if (m_dlgVideo.m_blVSetTop) {
				m_dlgVideo.SetTopVideoWndLocal(TRUE);
				m_dlgVideo.SetTopVideoWndLarge(FALSE);
			}
			m_hWndTarge = hWnd;
			return TRUE;
		}
		if (hWnd == NULL) {
		//// Skype�r�f�I���N�����Ă��Ȃ�
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
CString strRecFile)			// �^��t�@�C��
{
CFileSpec fs(strRecFile);
_stprintf_s(te.errstr, MAX_ERRSTR, _T("Record:%s"), fs.FileName());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
	try {
		/// �^�揀��
		m_RecFile = strRecFile;
		/// FPS
		CRegDWORD rgFps(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordFps"), 5, TRUE);	// �����l:5(fps)
		int intFps = (int)rgFps;
		/// �I�[�o�[���C
		CRegDWORD rgOverlay(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordOverlay"), 1, TRUE);	// �����l:1(�I�[�o�[���C����)
		BOOL blOverlay = ((DWORD)rgOverlay == 1) ? TRUE : FALSE;

		/// ScreenCapture �ݒ�
		LPWAVEFORMATEX pWF = (m_pAudioBuffer) ? &m_WaveFormat : NULL;
		if (m_blCompress) {
			//// ���k
			m_pScreenCap->BuilderGraph(m_hWndTarge, intFps, m_strCompress, pWF, m_RecFile, blOverlay);
		} else {
			//// �񈳏k
			m_pScreenCap->BuilderGraph(m_hWndTarge, intFps, NULL, pWF, m_RecFile, blOverlay);
		}
		// �����X�g���[���p���X�g�o�b�t�@�̌���
		if (m_pAudioBuffer) {
			m_pScreenCap->BindAudioBuffer(m_pAudioBuffer);
		}
		//// �t�@�C���T�C�Y����
		CRegDWORD rgSep(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordSep"), 1, TRUE);	// �����l:1(��������)
		BOOL blSep = ((DWORD)rgSep == 1) ? TRUE : FALSE;
		if (blSep) {
			CRegDWORD rgSepSize(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordSepSize"), 2000, TRUE);	// �����l:2000Mbyte
			ULONGLONG ullSz = (ULONGLONG)rgSepSize * (ULONGLONG)(1024 * 1024 * 0.95);  // �����������l������95%�̃T�C�Y��
			m_pScreenCap->SetLimitSize(ullSz);
		} else {
			m_pScreenCap->SetLimitSize(0);
		}

		blRecording = TRUE;

		/// �^��p�^�C�}�[�J�n(�C�x���g��m_dlgVideo�Ŏ�M)
		//SetTimer(m_dlgVideo.m_hWnd, ID_TIMER_VIDEO, 100, NULL);	// 0.1 sec each

		/// �ȉ��̃��\�b�h�̓��b�Z�[�W���荞�݂�������̂ŁA�Ō�Ɏ��s����
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
		/// �^��p�^�C�}�[��~
		//KillTimer(m_dlgVideo.m_hWnd, ID_TIMER_VIDEO);
		
		m_pScreenCap->StopRec();

		/// �r�f�I��ʂ̑O�ʕ\������
		m_dlgVideo.SetTopVideoWndLocal(FALSE);
		m_dlgVideo.SetTopVideoWndLarge(FALSE);

		/// �^��t�@�C���̏������݂�L���ɂ��邽�߁A�ҋ@���̃��b�Z�[�W����������
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
//  �@�\     : ���������̐ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
void CVideo::SetAudio(LPWAVEFORMATEX pWf, CPtrList *pAudioBuffer)
{
	if (pWf) {
		CopyMemory(&m_WaveFormat, pWf, sizeof(WAVEFORMATEX));	// �����iNULL�̏ꍇ�A���Ȃ��j
	}
	m_pAudioBuffer = pAudioBuffer;		// �I�[�f�B�I�X�g���[���o�b�t�@�Q�Ɨp�|�C���^
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
/* �ȉ��͂Ȃ���������
		if (m_pVideoGrab->AVIInfo((LPCTSTR)m_PlayFile, 
			&Duration, &FrameCount, &VideoWidth, &VideoHeight, &VideoFrameRateFps, 
			&AudioBitRateHz, &VideoCodec, &AudioCodec)) {
			tsz = Duration;
		}
*/
/* �ȉ��͂Ȃ����l���P�ɂȂ�ꍇ������
		m_pVideoGrab->SetPlayerFileName(m_PlayFile);
		m_pVideoGrab->SetAutoStartPlayer(FALSE);	// OpenPlayer�ɂ�鎩���Đ����֎~
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
//  �@�\     : �t�@�C����FPS(Frame/sec)�̎擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : Frame/sec�A-1:error
//  
//  ���l     : 
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
//  �@�\     : �t�@�C���̃t�H�[�}�b�g�`�F�b�N
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CVideo::ChkPlayFile()
{
	BOOL blRet = FALSE;

	try {
		if (m_PlayFile.IsEmpty()) return FALSE;
		CFileSpec fs(m_PlayFile);
		if (!fs.Exist()) return FALSE;

		m_pVideoGrab->SetPlayerFileName(m_PlayFile);
		m_pVideoGrab->SetAutoStartPlayer(FALSE);	// OpenPlayer�ɂ�鎩���Đ����֎~(PAUSE�̂���)
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
//  �@�\     : �r�f�I��ʂ̃T�C�Y
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : HWND
//  
//  ���l     : 
//  
HWND CVideo::GetVideoWinLocal(
CString strSkypeId,		// ���M��SkypeID
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
//  �@�\     : �r�f�I��ʃT�C�Y�ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CVideo::SetVideoWinLarge(
LPRECT pRect)
{
	return m_dlgVideo.SetVideoWndLarge(pRect, FALSE); 
}
