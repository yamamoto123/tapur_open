// MainTool.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "NewMenu.h"
#include "Tapur.h"
#include "MainFrm.h"
#include "SkypeEvents.h"
#include "MainTool.h"
#include "TPutil.h"
#include "BalloonHelp.h"
#include "TMsgBox.h"
#include ".\maintool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// CMainFrame���̃N���X��`���ċA���Ă��܂��̂ŁA�����o�[�ϐ��Ƃ��Ă͒�`�ł��Ȃ��B
CMainFrame *m_pFrame;
CAccessEvents *m_pAccess;

/////////////////////////////////////////////////////////////////////////////
// CMainTool

BEGIN_MESSAGE_MAP(CMainTool, CToolBar)
	//{{AFX_MSG_MAP(CMainTool)
	ON_UPDATE_COMMAND_UI(ID_VRECORD_START, OnUpdateVRecordStart)
	ON_UPDATE_COMMAND_UI(ID_RECORD_START, OnUpdateRecordStart)
	ON_UPDATE_COMMAND_UI(ID_RECORD_PAUSE, OnUpdateRecordPause)
	ON_UPDATE_COMMAND_UI(ID_RECORD_STOP, OnUpdateRecordStop)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()


CMainTool::CMainTool()
{
	m_lngRecordTime = 0;
	//// ��������C���X�^���X�̐���
	//m_pAudio = CAudio::Instance();
	//// �r�f�I����N���X�̃C���X�^���X����
	//m_pVideo = CVideo::Instance();
	m_pAudio = NULL;
	m_pVideo = NULL;
}

CMainTool::~CMainTool()
{
	m_lngRecordTime = 0;
	m_pAudio = NULL;
	m_pVideo = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CMainTool ���b�Z�[�W �n���h��

int CMainTool::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO :  �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B

	return 0;
}

void CMainTool::OnDestroy() 
{
	CToolBar::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	/// �v���p�e�B�̉��
	m_pFrame = NULL;
	m_pAccess = NULL;

	// �^�����ԊǗ��p�^�C�}�[�j��
	m_lngRecordTime = 0;
	KillTimer(ID_TIMER_MAINTOOL);
}

void CMainTool::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	if (nIDEvent == ID_TIMER_MAINTOOL) {	// (?)�Ȃ����}�E�X���c�[���o�[��ɂ���ƃC�x���g���������Ă��܂�����
		/// �^�����ԕ\��
		_TCHAR strTmp[64];
		BOOL blRet = m_pAudio->IsRecordPauseSkype();
		if (!blRet) {	// ��~���̓J�E���g���Ȃ�
			++m_lngRecordTime;
			_stprintf_s(strTmp, sizeof(strTmp)/sizeof(strTmp[0]), _T("%02i:%02i"), (m_lngRecordTime/60), (m_lngRecordTime%60));
			m_wndStatic.SetText(strTmp);
		}
		/// �^���E�^��MAX���Ԃ��o�߂����ꍇ�ASTOP�{�^��������
		DWORD dwRecordLimit = 0;
		if (m_pVideo->IsRecording()) {
			CRegDWORD rgVRecordLimit(_T("Software\\Tapur\\Tapur\\Settings\\VRecordLimitTime"), 600, TRUE);	// �����l:10min
			dwRecordLimit = rgVRecordLimit;
		} else {
			blRet = m_pAudio->IsRecordingSkype();
			if (blRet) {
				CRegDWORD rgRecordLimit(_T("Software\\Tapur\\Tapur\\Settings\\RecordLimitTime"), 3600, TRUE);		// �����l:60min
				dwRecordLimit = rgRecordLimit;
			}
		}
		/// �ʘb���I�����Ă���̂�prgFinished�C�x���g���������Ȃ��ꍇ�A�I�������R�[��
		if(m_pFrame->m_pAccessEvents->m_pCall != NULL) {
			if (m_pFrame->m_pAccessEvents->m_pCall->Status == prgFinished) {
				CTPerror te;
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("Force event:prgFinished"));
				//m_pFrame->m_pAccessEvents->CallStatusChanged(NULL, prgFinished);
				KillTimer(ID_TIMER_MAINTOOL);	// �^�C�}�[�I��(�d�v�I�F���ꂪ�Ȃ��ƃ^�C�}���������[�v���Ă��܂�)
				m_pFrame->PushRecordStop();		// STOP�{�^��������
			}
		}

		if (dwRecordLimit > 0 && (long)dwRecordLimit <= m_lngRecordTime) {
			KillTimer(ID_TIMER_MAINTOOL);	// �^�C�}�[�I��
			//// ���b�Z�[�W�\��
			///// STOP�{�^����ɕ\��
			CToolBarCtrl &tbc = GetToolBarCtrl();
			UINT idx = tbc.CommandToIndex(ID_RECORD_STOP);
			RECT rc;
			tbc.GetItemRect(idx, &rc);
			CPoint pnt((rc.left+rc.right)/2, (rc.top+rc.bottom)/2);
			CString strTitle;
			CString strMsg;
			if (m_pVideo->IsRecording()) {
				strTitle.LoadString(IDS_MSGBOX75);
				strMsg.LoadString(IDS_MSGBOX77);
			} else {
				strTitle.LoadString(IDS_MSGBOX74);
				strMsg.LoadString(IDS_MSGBOX76);
			}
			CBalloonHelp::LaunchBalloon(strTitle, strMsg, pnt, IDI_INFORMATION, 
				CBalloonHelp::unSHOW_CLOSE_BUTTON|CBalloonHelp::unSHOW_TOPMOST, 
				this, _T(""), 0);
			//// STOP�{�^��������
			m_pFrame->PushRecordStop();		
		}
	}

	CToolBar::OnTimer(nIDEvent);
}

// �^��{�^���̏�ԑJ��
void CMainTool::OnUpdateVRecordStart(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������

	CToolBarCtrl &tbc = GetToolBarCtrl();
	if (m_pAudio == NULL || m_pVideo == NULL) {
		pCmdUI->Enable(FALSE);
		return;
	}
	
	/// �^�����i������^���ł͂Ȃ��j�͖���
	BOOL blRet = m_pAudio->IsRecordingSkype();
	if (blRet && !m_pVideo->IsRecording() && theApp.m_lngAppStatus == 1) {
		if (tbc.IsButtonEnabled(ID_VRECORD_START)) {
			pCmdUI->Enable(FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_VRECORD_START, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_VRECORD_START)) {
			pCmdUI->Enable(TRUE); 
			m_pFrame->GetMenu()->EnableMenuItem(ID_VRECORD_START, MF_ENABLED | MF_BYCOMMAND);
		}
	}
}

// �^���{�^���̏�ԑJ��
void CMainTool::OnUpdateRecordStart(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������

	CToolBarCtrl &tbc = GetToolBarCtrl();
	if (m_pAudio == NULL || m_pVideo == NULL) {
		pCmdUI->Enable(FALSE);
		return;
	}
	
	/// ����^��/�^�撆�͖���
	if (m_pVideo->IsRecording() && theApp.m_lngAppStatus == 1) {
		if (tbc.IsButtonEnabled(ID_RECORD_START)) {
			pCmdUI->Enable(FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_START, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_RECORD_START)) {
			pCmdUI->Enable(TRUE); 
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_START, MF_ENABLED | MF_BYCOMMAND);
		}
	}
}

// �^���ꎞ��~�{�^���̏�ԑJ��
void CMainTool::OnUpdateRecordPause(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������

	CToolBarCtrl &tbc = GetToolBarCtrl();
	if (m_pAudio == NULL || m_pVideo == NULL) {
		pCmdUI->Enable(FALSE);
		return;
	}
	
	/// �^�����̂ݘ^���ꎞ��~�\
	BOOL blRet = m_pAudio->IsRecordingSkype();
	if (!blRet || !theApp.m_lngAppStatus == 1) {
		if (tbc.IsButtonEnabled(ID_RECORD_PAUSE)) {
			pCmdUI->Enable(FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_PAUSE, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_RECORD_PAUSE)) {
			/// �{�^���̃A�C�R���ݒ�
			if (m_pVideo->IsRecording()) {
				ReplaceIcon(7, IDI_T_VRECORD_PAUSE);	// �c�[���o�[�A�C�R���̕ύX
			} else {
				ReplaceIcon(7, IDI_T_RECORD_PAUSE);	// �c�[���o�[�A�C�R���̕ύX
			}
			/// �{�^���̗L����
			pCmdUI->Enable(TRUE); 
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_PAUSE, MF_ENABLED | MF_BYCOMMAND);
		}
	}
}

// �^���I���{�^���̏�ԑJ��
void CMainTool::OnUpdateRecordStop(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������

	CToolBarCtrl &tbc = GetToolBarCtrl();
	if (m_pAudio == NULL || m_pVideo == NULL) {
		pCmdUI->Enable(FALSE);
		return;
	}
	
	/// �^�����̂ݘ^���I���\
	BOOL blRet = m_pAudio->IsRecordingSkype();
	if (!blRet || !theApp.m_lngAppStatus == 1) {
		if (tbc.IsButtonEnabled(ID_RECORD_STOP)) {
			pCmdUI->Enable(FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_STOP, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_RECORD_STOP)) {
			/// �{�^���̃A�C�R���ݒ�
			if (m_pVideo->IsRecording()) {
				ReplaceIcon(8, IDI_T_VRECORD_STOP);	// �c�[���o�[�A�C�R���̕ύX
			} else {
				ReplaceIcon(8, IDI_T_RECORD_STOP);	// �c�[���o�[�A�C�R���̕ύX
			}
			/// �{�^���̗L����
			pCmdUI->Enable(TRUE); 
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_STOP, MF_ENABLED | MF_BYCOMMAND);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainTool �O�����\�b�h

//
//  �@�\     : �����ݒ�
//  
//  �@�\���� : ���CMainFrame����������Ă���K�v�����邽�߁A�C�x���g�łȂ��ACMainFrame����R�[��
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainTool::Init(
CWnd *pWnd)		// CMainFrame�n���h�����w��
{
	// ��������C���X�^���X�̐���
	m_pAudio = CAudio::Instance();
	/// �r�f�I����N���X�̃C���X�^���X����
	m_pVideo = CVideo::Instance();

	/// �v���p�e�B�̐ݒ�
	m_pFrame = (CMainFrame*)pWnd;
	m_pAccess = m_pFrame->m_pAccessEvents;

	/// �{�^���A�C�R���̍����ւ�
	// 16bit�A�C�R�����g��\�����邽�߁A���I�Ƀ��[�h����i�c�[���o�[�̃r�b�g�}�b�v�͖����j
	ReplaceIcon(5, IDI_T_VRECORD);
	ReplaceIcon(6, IDI_T_RECORD);
	ReplaceIcon(7, IDI_T_RECORD_PAUSE);
	ReplaceIcon(8, IDI_T_RECORD_STOP);

	/// �^���ΏۑI���{�^���̍X�V
	UpadateRecordMod();

	return TRUE;
}

/* EnableButton() ��OnUpdate�C�x���g�ł����g�p�ł��Ȃ�
BOOL CMainTool::UpdateToolBar() 
{
	CToolBarCtrl &tbc = GetToolBarCtrl();

	/// �^��{�^���̏�ԑJ��
	//// �^�撆�͖���
	if (m_pAudio->IsRecording() && !m_pVideo->IsRecording()) {
		if (tbc.IsButtonEnabled(ID_VRECORD_START)) {
			tbc.EnableButton(ID_VRECORD_START, FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_VRECORD_START, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_VRECORD_START)) {
			tbc.EnableButton(ID_VRECORD_START, TRUE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_VRECORD_START, MF_ENABLED | MF_BYCOMMAND);
		}
	}

	/// �^���{�^���̏�ԑJ��
	//// �^�撆�͖���
	if (m_pVideo->IsRecording()) {
		if (tbc.IsButtonEnabled(ID_RECORD_START)) {
			tbc.EnableButton(ID_RECORD_START, FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_START, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_RECORD_START)) {
			tbc.EnableButton(ID_RECORD_START, TRUE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_START, MF_ENABLED | MF_BYCOMMAND);
		}
	}

	/// �^���ꎞ��~�{�^���̏�ԑJ��
	//// �^�����̂ݘ^���ꎞ��~�\
	if (!m_pAudio->IsRecordingVac() || !theApp.m_lngAppStatus == 1) {
		if (tbc.IsButtonEnabled(ID_RECORD_PAUSE)) {
			tbc.EnableButton(ID_RECORD_PAUSE, FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_PAUSE, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_RECORD_PAUSE)) {
			/// �{�^���̃A�C�R���ݒ�
			if (m_pVideo->IsRecording()) {
				ReplaceIcon(7, IDI_T_VRECORD_PAUSE);	// �c�[���o�[�A�C�R���̕ύX
			} else {
				ReplaceIcon(7, IDI_T_RECORD_PAUSE);		// �c�[���o�[�A�C�R���̕ύX
			}
			/// �{�^���̗L����
			tbc.EnableButton(ID_RECORD_PAUSE, TRUE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_PAUSE, MF_ENABLED | MF_BYCOMMAND);
		}
	}

	/// �^���I���{�^���̏�ԑJ��
	//// �^�����̂ݘ^���I���\
	if (!m_pAudio->IsRecordingVac() || !theApp.m_lngAppStatus == 1) {
		if (tbc.IsButtonEnabled(ID_RECORD_STOP)) {
			tbc.EnableButton(ID_RECORD_STOP, FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_STOP, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_RECORD_STOP)) {
			/// �{�^���̃A�C�R���ݒ�
			if (m_pVideo->IsRecording()) {
				ReplaceIcon(8, IDI_T_VRECORD_STOP);	// �c�[���o�[�A�C�R���̕ύX
			} else {
				ReplaceIcon(8, IDI_T_RECORD_STOP);	// �c�[���o�[�A�C�R���̕ύX
			}
			/// �{�^���̗L����
			tbc.EnableButton(ID_RECORD_STOP, TRUE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_STOP, MF_ENABLED | MF_BYCOMMAND);
		}
	}

//	tbc.RedrawWindow();
//	tbc.InvalidateRect(NULL);					// �c�[���o�[�̕\�������t���b�V��

	return TRUE;
}
*/

//
//  �@�\     : �^���ΏۑI���{�^���̍X�V
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainTool::UpadateRecordMod()
{
	/// �^���Ώۏ�Ԃ̃��W�X�g���擾
	CRegDWORD rgRecMod(_T("Software\\Tapur\\Tapur\\Settings\\RecordMod"), 0, TRUE);
	DWORD dwRecMod = rgRecMod;

	/// Audio�C���X�^���X�֐ݒ蔽�f
	m_pAudio->SetRecordSyncSw(dwRecMod);

 	/// �^���ΏۑI���{�^���̍X�V
	switch (dwRecMod) {
    case 1:		// ��M�����̂�
		ReplaceIcon(10, IDI_T_RECORD_RCV);	// �c�[���o�[�A�C�R���̕ύX
		break;
    case 2:		// ���M�����̂�
		ReplaceIcon(10, IDI_T_RECORD_SND);	// �c�[���o�[�A�C�R���̕ύX
		break;
    case 0:		// ����M����
	default:
		ReplaceIcon(10, IDI_T_RECORD_BOTH);	// �c�[���o�[�A�C�R���̕ύX
		break;
	}

	return TRUE;
}

//
//  �@�\     : �r�f�I�^��J�n�C�x���g
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : CMainFrame�ł����C�x���g���擾�ł��Ȃ����߁ACMainFrame�ł��̃��\�b�h���R�[��
//  
BOOL CMainTool::VRecordStart(
CString strDate,// �^���J�n����
BOOL blChat,	// �`���b�g���b�Z�[�W�𑗐M���邩
BOOL blSilent)	// �T�C�����g���[�h
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString str;

	/// ����ԉ������͖���
	if (theApp.m_lngAppStatus == 2) {
		if (!blSilent) {
			str.LoadString(IDS_MSGBOX73);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		}
		return FALSE;
	}

	/// �r�f�I��M���̂ݗL��
	if (!m_pAccess->IsVTalking()) {
		if (!blSilent) {
			str.LoadString(IDS_MSGBOX72);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		}
		return FALSE;
	}

	if (m_pVideo->IsRecording()) {
		if (m_pVideo->IsRecordPause()) {
			/// �^��ꎞ��~�̕��A
			m_pVideo->RecContinue();
		}
	} else {
	/// �^��J�n
		CRegDWORD rgEnableRecWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVideoRecordWarning"), 1, TRUE);	// �����l:1(True)
		CRegDWORD rgEnableRecStopWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVideoRecordStopWarning"), 1, TRUE);	// �����l:1(True)
		//// �x���`���b�g�̊m�F���
		if (blChat && !blSilent) {
			if ((DWORD)rgEnableRecWarning == 1 || (DWORD)rgEnableRecStopWarning == 1) {
				CRegDWORD rgChk(_T("Software\\Tapur\\Tapur\\Settings\\EnableVideoRecordConfirm"), 1, TRUE);		// �����l:1
				BOOL bl = (rgChk > 0) ? TRUE : FALSE;
				if (bl) {
					CString strTitle;
					CString strMsg;
					strTitle.LoadString(IDS_TMSGBOX_TITLE2);
					strMsg.LoadString(IDS_TMSGBOX_MSG2);
					CTMsgBox msgbox(NULL, strTitle, strMsg, TRUE);
					msgbox.Show(TRUE);
					if (!msgbox.GetResult()) {
						return FALSE;
					}
					if (msgbox.GetCheck()) {
						rgChk = 0;
					}
				}
			}
		}

		CString strSender = m_pAccess->GetSender();
		if (strSender.IsEmpty()) return FALSE;

		// �^���̊J�n�i�^�挋������SocketIn�I�u�W�F�N�g���K�v�Ȃ̂ŁA�����ŊJ�n���Ă��܂��j
		if (!RecordStart(strDate, FALSE, TRUE)) return FALSE;

		/// ���k�`���̎w��
		CRegDWORD rgVCmp(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordCompress"), 1, TRUE);		// �����l:1(���k)
		BOOL bl = (rgVCmp > 0) ? TRUE : FALSE;
		m_pVideo->SetCompress(bl);
		CRegString rgVCmpName(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordCompressName"), _T("Lagarith lossless codec"), TRUE);		// �����l:"Lagarith"
		CString strCmp = rgVCmpName;
		m_pVideo->SetCompressName(strCmp);
		/// ���������̎w��
		CRegDWORD rgAudio(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordWithAudio"), 1, TRUE);	// �����l:1(�����t�^��)
		bl = (rgAudio > 0) ? TRUE : FALSE;
		if (bl) {
			///// �^�������̎w��imono/stereo/natural stereo�j
			CRegDWORD rgStereo(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordStereo"), 1, TRUE);		// �����l:stereo(���S�p��)
			int intVal = rgStereo;
			m_pAudio->SetRecordSyncStereo(intVal);
			WAVEFORMATEX Fmt = m_pAudio->pSocketIn->GetWaveFormat();		// �ۑ��pWAVE�t�H�[�}�b�g
			CPtrList *pList = m_pAudio->pSocketIn->GetOutListBuffer();		// �����X�g���[���p�o�b�t�@
			m_pVideo->SetAudio(&Fmt, pList);
		} else {
			m_pVideo->SetAudio(NULL, NULL);
		}
		/// �ʘb�ؒf�ɔ����āA�����R�[�h���u�^�����v�ɐݒ�
		m_pAccess->m_ProcCode = _T("$$");	
		/// TMP�t�@�C���Ř^��J�n
		if (m_pVideo->RecordOpen(strSender)) {
			CString strRecordFile = m_pAccess->m_Setup.RdfVideoFileName(strDate, m_pAccess->m_ProcCode);
			m_pVideo->Record(strRecordFile);
		}
		/// �����Ɍx���`���b�g���t
		if (blChat) {
			if ((DWORD)rgEnableRecWarning == 1) {
				str.LoadString(IDS_TAB_VREC_WARNING);
				CRegString rgRecWarning = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordWarning"), str, TRUE);
				str = rgRecWarning;
				if (!str.IsEmpty()) m_pAccess->SendChat(str);
			}
		}
	}

	return TRUE;
}

//
//  �@�\     : �^���J�n�C�x���g
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : CMainFrame�ł����C�x���g���擾�ł��Ȃ����߁ACMainFrame�ł��̃��\�b�h���R�[��
//  
BOOL CMainTool::RecordStart(
CString strDate,// �^���J�n����
BOOL blChat,	// �`���b�g���b�Z�[�W�𑗐M���邩
BOOL blSilent)	// �T�C�����g���[�h
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString str;

	CToolBarCtrl &tbc = GetToolBarCtrl();

	/// ����ԉ������͖���
	if (theApp.m_lngAppStatus == 2) {
		if (!blSilent) {
			str.LoadString(IDS_MSGBOX73);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		}
		return FALSE;
	}

	/// �ʘb���̂ݗL��
	if (!m_pAccess->IsTalking(TRUE)) {
		if (!blSilent) {
			str.LoadString(IDS_MSGBOX70);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		}
		return FALSE;
	}

	BOOL blRet = m_pAudio->IsRecordingSkype();
	if (blRet) {
		if (m_pAudio->IsRecordPauseSkype()) {
			/// �^���ꎞ��~�̕��A
			m_pAudio->RecContinueSkype();
		}
	} else {
	/// �^���J�n
		CRegDWORD rgEnableRecWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVoiceRecordWarning"), 1, TRUE);	// �����l:1(True)
		CRegDWORD rgEnableRecStopWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVoiceRecordStopWarning"), 1, TRUE);	// �����l:1(True)
		//// �x���`���b�g�̊m�F���
		if (blChat && !blSilent) {
			if ((DWORD)rgEnableRecWarning == 1 || (DWORD)rgEnableRecStopWarning == 1) {
				CRegDWORD rgChk(_T("Software\\Tapur\\Tapur\\Settings\\EnableVoiceRecordConfirm"), 1, TRUE);		// �����l:1
				BOOL bl = (rgChk > 0) ? TRUE : FALSE;
				if (bl) {
					CString strTitle;
					CString strMsg;
					strTitle.LoadString(IDS_TMSGBOX_TITLE1);
					strMsg.LoadString(IDS_TMSGBOX_MSG1);
					CTMsgBox msgbox(NULL, strTitle, strMsg, TRUE);
					msgbox.Show(TRUE);
					if (!msgbox.GetResult()) {
						return FALSE;
					}
					if (msgbox.GetCheck()) {
						rgChk = 0;
					}
				}
			}
		}

		/// �J�E���^�N���A
		m_lngRecordTime = 0;
		m_wndStatic.SetText(_T("00:00"));
		/// �ʘb�ؒf�ɔ����āA�����R�[�h���u�^�����v�ɐݒ�
		m_pAccess->m_ProcCode = _T("$$");

		/// ��M������SkypeAPI�ŉ����^���J�n
		if (m_pAudio->RecordOpenSkype(m_pAccess->m_pCall)) {
			///// �^�������̎w��imono/stereo/natural stereo�j
			CRegDWORD rgStereo(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordStereo"), 1, TRUE);		// �����l:stereo(���S�p��)
			int intVal = rgStereo;
			m_pAudio->SetRecordSyncStereo(intVal);
			///// �^���Ώۂ̎w��i��M����/MIC����/�����j
			CRegDWORD rgRecMod(_T("Software\\Tapur\\Tapur\\Settings\\RecordMod"), 0, TRUE);
			DWORD dwVal = rgRecMod;
			m_pAudio->SetRecordSyncSw(dwVal);
			///// �ۑ��`���̎w��iWAV/MP3�j
			CRegDWORD rgFormatType(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordType"), 0, TRUE);		// �����l:0(WAV)
			int intFormatType = rgFormatType;
			CRegString rgCodecName(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordMP3Codec"), _T(""), TRUE);
			CString strCodec = rgCodecName;
			CRegString rgFormatName(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordMP3Format"), _T(""), TRUE);
			CString strFormat = rgFormatName;
			if (m_pVideo->IsRecording()) {
				// �^�掞��WAV�ɌŒ�
				intFormatType = 0;
				// �^��p�����o�b�t�@�̐������w��
				 m_pAudio->pSocketIn->EnableOutBuffer(TRUE);
			} 
			m_pAudio->SetRecordFormat(intFormatType, strCodec, strFormat);
			///// �^���t�@�C����
			CString strRecordFile;
			if (intFormatType > 0) {
				strRecordFile = m_pAccess->m_Setup.RdfMp3FileName(strDate, m_pAccess->m_ProcCode);
			} else {
				strRecordFile = m_pAccess->m_Setup.RdfWavFileName(strDate, m_pAccess->m_ProcCode);
			}
			///// �^���J�n
			CRegDWORD rgSkypePortOut = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortOut"), 42805, TRUE);
			CRegDWORD rgSkypePortMic = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortMic"), 42806, TRUE);
			CString strPortOut, strPortMic;
			strPortOut.Format(_T("%d"), (DWORD)rgSkypePortOut);
			strPortMic.Format(_T("%d"), (DWORD)rgSkypePortMic);
			if (m_pAudio->RecordSkype(strRecordFile, strPortOut, strPortMic, m_pAccess->m_defOutDevice)) {
				//// �^�����ԊǗ��p�^�C�}�[�J�n
				SetTimer(ID_TIMER_MAINTOOL, 1000, NULL);	// 1 sec each
			} else {
				CTPerror te;
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("RecordStart error"));
				return FALSE;
			}
		}

		theApp.m_lngAppStatus = 1;		// �ʘb�^���t���O
		m_wndStatic.SetTextColor(RGB(0,0,0));	//black
		/// �����Ɍx���`���b�g���t
		if (blChat) {
			if ((DWORD)rgEnableRecWarning == 1) {
				str.LoadString(IDS_TAB_REC_WARNING);
				CRegString rgRecWarning = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordWarning"), str, TRUE);
				str = rgRecWarning;
				if (!str.IsEmpty()) m_pAccess->SendChat(str);
			}
		}
	}

	return TRUE;
}

//
//  �@�\     : �^��ꎞ�J�n�E�ĊJ�C�x���g
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : CMainFrame�ł����C�x���g���擾�ł��Ȃ����߁ACMainFrame�ł��̃��\�b�h���R�[��
//  
BOOL CMainTool::VRecordPause() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	if (!m_pVideo->IsRecording()) return FALSE;

	if (m_pVideo->IsRecordPause()) {
		/// �^��ꎞ��~�̕��A
		m_pVideo->RecContinue();
	} else {
		/// �^��ꎞ��~
		m_pVideo->RecPause();
	}

	return TRUE;
}

//
//  �@�\     : �^���ꎞ�J�n�E�ĊJ�C�x���g
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : CMainFrame�ł����C�x���g���擾�ł��Ȃ����߁ACMainFrame�ł��̃��\�b�h���R�[��
//  
BOOL CMainTool::RecordPause() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	if (!m_pAudio->IsRecordingSkype()) return FALSE;

	if (m_pAudio->IsRecordPauseSkype()) {
		/// �^���ꎞ��~�̕��A
		m_pAudio->RecContinueSkype();
	} else {
		/// �^���ꎞ��~
		m_pAudio->RecPauseSkype();
	}

	return TRUE;
}

//
//  �@�\     : �^��I���C�x���g
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : CMainFrame�ł����C�x���g���擾�ł��Ȃ����߁ACMainFrame�ł��̃��\�b�h���R�[��
//  
BOOL CMainTool::VRecordStop(
BOOL blChat)	// �`���b�g���b�Z�[�W�𑗐M���邩
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString str;

	if (!m_pVideo->IsRecording()) return FALSE;

	/// �^�抮��
	// RecordStop()�Ř^��ۑ����s��

	/// �����Ɍx���`���b�g���t
	if (blChat) {
		CRegDWORD rgEnableRecWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVideoRecordStopWarning"), 1, TRUE);	// �����l:1(True)
		if ((DWORD)rgEnableRecWarning == 1) {
			str.LoadString(IDS_TAB_VRECSTOP_WARNING);
			CRegString rgRecStopWarning = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordStopWarning"), str, TRUE);
			str = rgRecStopWarning;
			if (!str.IsEmpty()) m_pAccess->SendChat(str);
		}
	}

	return TRUE;
}

//
//  �@�\     : �^���I���C�x���g
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : CMainFrame�ł����C�x���g���擾�ł��Ȃ����߁ACMainFrame�ł��̃��\�b�h���R�[��
//  
BOOL CMainTool::RecordStop(
BOOL blChat)	// �`���b�g���b�Z�[�W�𑗐M���邩
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString str;

	/// �^�C�}�[�I��
	KillTimer(ID_TIMER_MAINTOOL);

	/// �J�E���^�N���A
	m_lngRecordTime = 0;
	m_wndStatic.SetText(_T("00:00"));
	m_wndStatic.SetTextColor(RGB(153,153,153));	//gray

	BOOL blRet = m_pAudio->IsRecordingSkype();
	if (!blRet) return FALSE;

	/// �^���^�抮��
	m_pAccess->FinishRecord();
	theApp.m_lngAppStatus = 0;	// APP��ԃt���O�����Z�b�g�iFinishRecord�Ŏg�p����̂ŁA���̌�j

	/// �����R�[�h���N���A
	m_pAccess->m_ProcCode.Empty();	
	/// �����Ɍx���`���b�g���t
	if (blChat) {
		CRegDWORD rgEnableRecWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVoiceRecordStopWarning"), 1, TRUE);	// �����l:1(True)
		if ((DWORD)rgEnableRecWarning == 1) {
			str.LoadString(IDS_TAB_RECSTOP_WARNING);
			CRegString rgRecStopWarning = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordStopWarning"), str, TRUE);
			str = rgRecStopWarning;
			if (!str.IsEmpty()) m_pAccess->SendChat(str);
		}
	}

	return TRUE;
}

//
//  �@�\     : �^���ΏۑI���{�^���C�x���g
//  
//  �@�\���� : �h���b�v�_�E�����X�g�̕\��
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : CMainFrame�ł����C�x���g���擾�ł��Ȃ����߁ACMainFrame�ł��̃��\�b�h���R�[��
//  
void CMainTool::RecordMod() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	CWnd *pWnd = &(m_pFrame->m_wndToolBar);
	UINT nID = IDR_DROP_RECMOD;

	// load and display popup menu
	CNewMenu menu;
	menu.SetMenuDrawMode(CNewMenu::STYLE_XP);
	menu.LoadMenu(nID);
	menu.LoadToolBar(IDR_RECMOD_TOOL);
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup);

	CRect rc;
	pWnd->SendMessage(TB_GETRECT, ID_RECORD_MOD, (LPARAM)&rc);
	pWnd->ClientToScreen(&rc);

	pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		rc.left, rc.bottom, AfxGetMainWnd(), &rc);
}

//
//  �@�\     : TOOLBAR���StaticText�𐶐�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainTool::CreateStatic(
CLabel& txtStatic,	// CStatic�Q��
CString strText,	// �\��������
UINT nIndex,		// �c�[���o�[���index
UINT nID,			// �c�[���o�[��̃{�^��ID
int nWidth)			// ������̕\����
{
	// Create the static text
	SetButtonInfo(nIndex, nID, TBBS_SEPARATOR, nWidth);

	CRect rect;
	GetItemRect(nIndex, &rect);
	rect.top += 1;
	rect.bottom -= 1;

	if (!txtStatic.Create(strText, WS_VISIBLE, rect, this, nID))
	{
		TRACE(_T("Failed to create static window...\n"));
		return FALSE;
	}

	txtStatic.SetFontName(_T("System"));	// ���̃t�H���g�ł̓T�C�Y�w��ł��Ȃ�
	txtStatic.SetTextColor(RGB(153,153,153));	//gray
	txtStatic.SetFontSize(18);
	txtStatic.SetSunken(TRUE);
	
	return TRUE;
} 

//
//  �@�\     : �c�[���o�[�A�C�R���̕ύX
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainTool::ReplaceIcon(
int intPos,		// �Ώۃ{�^���ʒu�i0�`�j
WORD iconId)	// ICON ID
{
	CToolBarCtrl &tbc = GetToolBarCtrl();
	CImageList *pImageList = tbc.GetImageList();// ���݂̃c�[���o�[�̃C���[�W
	HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(iconId)); 
	pImageList->Replace(intPos, hIcon);			// �{�^���C���[�W��ύX
	tbc.SetImageList(pImageList);				// �c�[���o�[�̃C���[�W��ύX
	tbc.InvalidateRect(NULL);					// �c�[���o�[�̕\�������t���b�V��

	return TRUE;
}

