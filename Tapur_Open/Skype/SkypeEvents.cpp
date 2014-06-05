// SkypeEvents.cpp

#include "stdafx.h"
#include <shlobj.h>		// SHGetSpecialFolderPath()
#include "Tapur.h"
#include "SkypeEvents.h"
#include "MsgTree.h"
#include "MsgList.h"
#include "FileSpec.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "TPutil.h"
#include "MainFrm.h"
#include "ProgressWnd.h"
#include "Tokenizer.h"
#include "BalloonHelp.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


CAccessEvents::CAccessEvents()
{
	SetMyParent(NULL);
	
	m_pCall == NULL;
	m_ProcCode.Empty();
	blSkypeVideoVersion = FALSE;
//	theApp.m_blSenderCall = FALSE;
	theApp.m_lngAppStatus = 0;

	///// �r�f�I+���������N���X�̏�����
	//for (int i = 0; i < sizeof(m_pAviManager)/sizeof(m_pAviManager[0]); i++) {
	//	m_pAviManager[i] = new CAviManager(AfxGetMainWnd());
	//}

	/// ��������N���X�̃C���X�^���X����
	m_pAudio = CAudio::Instance();
	/// �r�f�I����N���X�̃C���X�^���X����
	m_pVideo = CVideo::Instance();
}

CAccessEvents::~CAccessEvents()
{
	SetMyParent(NULL);	
}

// �ʘb���̉�������
BOOL CAccessEvents::Response()
{
	CString str;
	_bstr_t bStr;
	CString strTmp;	
	CString strPlayFile;	// �Đ����b�Z�[�W�t�@�C��
	UST_RSF_RESP st_resp;
	UST_RSF_RESP *pst_resp = &st_resp;
	CFileSpec fs;
	long lngPlaySize = 0;	// �ő�PLAY����
	BOOL blRet;

	try {
		if (IsResponsing()) return TRUE;		// ���݉������̏ꍇ
		SetResponsing(TRUE);					// �������t���O��ON
		/// �����������\���`�F�b�N
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		if (pFrame->m_flgEnable == 0) goto Response_END;	// ���������̏ꍇ
		if (m_pCall == NULL) goto Response_END;
		if (m_CallStatus.blSenderCall) goto Response_END;		// ���[�U���M�̏ꍇ
		//if (m_pCall->Status != prgInProgress) goto Response_END;
		if (m_ProcCode.IsEmpty()) goto Response_END;
TRACE(_T("m_ProcCode=%s\n"), m_ProcCode);
		theApp.m_lngAppStatus = 2;		// APP��ԃt���O�𗯎�^�ɂ���
		//// �����R�[�h�ɑΉ���������
		m_Setup.GetResponse(m_ProcCode, pst_resp);
		switch (pst_resp->type) {
		case RESP_TYPE_PLAY:	// �Đ��t�F�[�Y
			if (pFrame->m_stPlayChk.blOnTimer) break;
			///// BEEP
			if (pst_resp->beep > 0) {
				if (!PlayBeep(pst_resp->beep)) {
					str.LoadString(IDS_MSGBOX61);
					te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
				}
			}
			if (pst_resp->play_type & RESP_PLAY_TYPE_CHAT) {
			///// �`���b�g���b�Z�[�W
				strPlayFile = m_Setup.RsfChatFileName(m_ProcCode);
				fs.SetFullSpec(strPlayFile);
				if (fs.Exist()) {
					if (!fs.LoadString(&str)) {
						str.LoadString(IDS_MSGBOX102);
						te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
					}
					if (!str.IsEmpty()) SendChat(str, m_CallStatus.strPartnerId);
				}
			}
			/// �r�f�I���L���ȏꍇ
			if (IsSkypeVideo(m_CallStatus.strPartnerId)) {
				if (pst_resp->play_type & RESP_PLAY_TYPE_VIDEO) {
					///// �r�f�I�Đ�
					strPlayFile = m_Setup.RsfVideoFileName(m_ProcCode);
					fs.SetFullSpec(strPlayFile);
					if (fs.Exist()) {
						////// ���z�J�����֎w��(���W�X�g���o�R)
						CRegDWORD rgMode(_T("Software\\Tapur\\VirtualCamera\\mode"), 0, TRUE);
						rgMode = 1;
						strTmp = m_Setup.RsfTmpFolder();
						if (strTmp.Right(1) == _T("\\")) strTmp.Delete(strTmp.GetLength() - 1);
						CRegString rgPath(_T("Software\\Tapur\\VirtualCamera\\frmpath"), _T(""), TRUE);
						rgPath = strTmp;
						///// �Đ��t�@�C����fps���擾
						m_pVideo->SetPlayFile(strPlayFile);
						int intFps = (int)m_pVideo->GetFps();
						if (intFps <= 0) {
							// �s���̏ꍇ�f�t�H���g�l
							CRegDWORD rgFps(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordFps"), 5, TRUE);	// �����l:5(fps)
							intFps = (int)rgFps;	
						}
						CRegDWORD rgFps(_T("Software\\Tapur\\VirtualCamera\\fps"), 0, TRUE);
						rgFps = intFps;		// fps
						///// �t���[�������J�n
						long lngTime = (long)(m_pVideo->GetPlaySize() + 1.0);		// 1sec�]�T����������
						if (lngTime > lngPlaySize) lngPlaySize = lngTime;
						m_pVideo->PlayOpenSkype(strTmp);
						if (!m_pVideo->PlaySkype(strPlayFile)) {
							str.LoadString(IDS_MSGBOX64);
							te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
						}
						////// SKYPE�r�f�I�f�o�C�X�ύX
// �C�x���g�������ł̓t���[���t�@�C������������Ȃ����߁A�����ŃR�[�����Ă�����
//						SetSkypeVDevice(TVC_IN_DEV);
//						RefreshSkypeVideo(m_CallStatus.lngCallId);
					}
				}
				if (pst_resp->play_type & RESP_PLAY_TYPE_IMAGE) {
					///// �Î~��Đ�
					strPlayFile = m_Setup.FindRsfImgFile(m_ProcCode);
					fs.SetFullSpec(strPlayFile);
					if (fs.Exist()) {
						////// ���z�J�����֎w��(���W�X�g���o�R)
						CRegDWORD rgMode(_T("Software\\Tapur\\VirtualCamera\\mode"), 0, TRUE);
						rgMode = 0;
						CRegString rgPath = CRegString(_T("Software\\Tapur\\VirtualCamera\\imgfile"), _T(""), TRUE);
						rgPath = strPlayFile;
						CRegDWORD rgFps(_T("Software\\Tapur\\VirtualCamera\\fps"), 0, TRUE);
						rgFps = 0;		// fps
						////// SKYPE�r�f�I�f�o�C�X�ύX
						SetSkypeVDevice(TVC_IN_DEV);
						RefreshSkypeVideo(m_CallStatus.lngCallId);
					}
				}
			}
			if (pst_resp->play_type & RESP_PLAY_TYPE_AUDIO) {
			///// �����Đ�
				strPlayFile = m_Setup.RsfAudioFileName(m_ProcCode);
				fs.SetFullSpec(strPlayFile);
				if (fs.Exist()) {
					m_pAudio->m_PlayFile = strPlayFile;
					long lngTime = (long)m_pAudio->GetPlaySize();
					if (lngTime > lngPlaySize) lngPlaySize = lngTime;
					m_pAudio->PlayOpen();				// ���j�^�[�Đ�
					m_pAudio->Play();

					m_pAudio->PlayOpenSkype(m_pCall);	// SkypeAPI�Đ�
					blRet = m_pAudio->PlaySkype();
					if (!blRet) {
						str.LoadString(IDS_MSGBOX61);
						te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
					}
				}
			}

			/// �����R�[�h���X�V
			m_ProcCode += _T("$");		

			/// �������b�Z�[�W�Đ��I���҂��^�C�}�̋N��
			pFrame->StartPlayChkTimer(lngPlaySize);
			break;
		case RESP_TYPE_RECORD:
		case RESP_TYPE_VRECORD:
			//// ���ɘ^�����̏ꍇ�������Ȃ�
			if (pFrame->m_stRecordMax.blOnTimer) break;
			if (pst_resp->type == RESP_TYPE_VRECORD) {
				//// 4.0�̏ꍇ�A�����I�ɃR�}���h�𑗂�Ȃ��ƕ\������Ȃ��ꍇ������(�ȉ��ł����삵�Ȃ�)
				if (m_pCall != NULL) {
					bStr = m_pCall->GetVideoStatus();
					str.Format(_T("%s"), (LPCTSTR)bStr);
					str.MakeUpper();
					if (str.Find(_T("VIDEO_RECEIVE_ENABLED")) >= 0 || str.Find(_T("VIDEO_BOTH_ENABLED")) >= 0) {
						m_pCall->StartVideoRecieve();
					}	
				}
				//// �r�f�I�^�悪����ꍇ��Skype��\������i�ŏ�������Ă���ꍇ������̂Łj
				ShowVideoWin(m_CallStatus.strPartnerName, FALSE);
			}
			//// �Đ����̉����E�r�f�I���~
			if (m_pAudio->IsPlaying()) {
				m_pAudio->PlayStop();
			}
			if (m_pAudio->IsPlayingSkype()) {
				m_pAudio->PlayStopSkype();
			}
			if (m_pVideo->IsPlaying()) {
				m_pVideo->PlayStop();
			}
			if (m_pVideo->IsPlayingSkype()) {
				m_pVideo->PlayStopSkype();
			} 
			//// BEEP
			if (pst_resp->beep > 0) {
				if (!PlayBeep(pst_resp->beep)) {
					CString str;
					str.LoadString(IDS_MSGBOX61);
					te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
				}
			}
			// �}�C�N��MUTE
			m_pAccess->Mute = -1;	// TRUE
			//// �^��/�^��J�n
			if (pst_resp->type == RESP_TYPE_VRECORD) {
				// �p�~�iVer1.00�j�F����^���͎����̃r�f�I�̓I�t
				// Skype3.5�ȍ~�ł͎����̃r�f�I�͕ʉ�ʂ̂��߁B�܂��A�����̃r�f�I�I�t�ŉ摜�T�C�Y���ϓ����邽��
				//RefreshSkypeVideo(m_CallStatus.lngCallId, TRUE);// ����^���͎����̃r�f�I�̓I�t

				int intRet = StartRecord(TRUE);
			} else {
				StartRecord(FALSE);
			}
			break;
		case RESP_TYPE_DISCONNECT:
			///// BEEP
			if (pst_resp->beep > 0) {
				if (!PlayBeep(pst_resp->beep)) {
					str.LoadString(IDS_MSGBOX61);
					te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
				}
			}

			m_pCall->Status = prgFinished;		// �ؒf

			////// SKYPE�r�f�I�f�o�C�X��߂�
			if (IsSkypeVideo()) SetSkypeVDevice(m_defVDevice);
			break;
		case RESP_TYPE_EXEC:
			////// �A�g�v���O���������s
			ExecResp(pst_resp->exec_id);
			break;
		default:
			break;
		}
	}
    catch(CException *e)
    {
		m_ProcCode.Empty();		// �����R�[�h��������
		/// SKYPE�r�f�I�f�o�C�X��߂�
		if (IsSkypeVideo()) SetSkypeVDevice(m_defVDevice);
		/// �ؒf
		//str.Format(_T("SET CALL %d STATUS FINISHED"), m_CallStatus.lngCallId);
		//m_pAccess->SendCommand(_bstr_t((LPCTSTR)str));
		m_pCall->Status = prgFinished;
		SetResponsing(FALSE);					// �������t���O��OFF
		// error message
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("unknown error:%s\n"), szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		e->Delete();
		return FALSE;
	}

	goto Response_END;

Response_END:
	SetResponsing(FALSE);					// �������t���O��OFF
	return TRUE;
}

// Skype API �����s����ƃl�X�g����Result�C�x���g���������₷���̂ŁA�Ȃ�ׂ����s���Ȃ������ǂ�
HRESULT CAccessEvents::Result(BSTR Description)
{
	HRESULT ret = S_OK;

	/// prgInProgress�C�x���g���������Ȃ��ꍇ������̂ŁA�ŏ��̉��������̏ꍇ�̂݃R�[��
	//if (m_ProcCode == _T("$")) {
	//	ret = Response() ? S_OK : S_FALSE;
	//	return ret;
	//}

	return ret;
}

HRESULT CAccessEvents::CallStatusChanged(ICallPtr pChangedCall, SkypeCallProgress Status)
{
	CString str;
	_bstr_t bStr;
	CRegDWORD rgWaitAnswer;
	DWORD dw;
	long id;
	BOOL blRet;

	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	//if (pFrame->m_flgEnable == 0) return S_OK;	// ���������̏ꍇ

	try {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("CallStatusChanged:%d"), Status);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
TRACE(_T("SkypeCallProgress:%d\n"), Status);
		switch (Status)
		{
		case prgUnknown:	// Unknown call status
			break;
		case prgUnplaced:	// Call was never placed
			break;
		case prgRouting:	// Call is currently being routed
			break;
		case prgEarlyMedia:	// Early media is being played
			break;
		case prgFailed:		// Call Failed
			break;
		case prgRinging:	// Ringing
			if (pFrame->m_flgEnable == 0) return S_OK;	// ���������̏ꍇ
			if (pChangedCall == NULL) break;
			/// ��b��or�����҂��̏ꍇ�������Ȃ�
			if (m_pCall != NULL) {
				if (IsTalking() || pFrame->m_stAnswerWait.blOnTimer) {
					break;
				} else {
					//// ���̂�Call�I�u�W�F�N�g���c���Ă���ꍇ�N���A����
					m_pCall == NULL;
				}
			}

			/// ���݂�Skype Call�N���X
			m_pCall = pChangedCall;
			m_CallStatus.SetCall(m_pCall);
			m_Setup.SetCallerId(m_CallStatus.strPartnerId);	// ���M��ID
			m_ProcCode.Empty();						// �����R�[�h��������
			m_pAudio->Reset();						// �����I�u�W�F�N�g�̏�����
			m_pVideo->Reset();						// �r�f�I�I�u�W�F�N�g�̏�����
			if (m_CallStatus.blSenderCall) break;	// ���[�U���M�̏ꍇ
			SetResponsing(FALSE);					// �������t���O��OFF
			SetCallFinish(FALSE);					// �ʘb�I�����t���O��OFF
			/// ���M�҂ɑΉ����������ݒ�����[�h
			if (LoadSetup()) {
				/// ���W�X�g���̏����l�`�F�b�N
				rgWaitAnswer = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\WaitAnswerTime"), 10, TRUE);
				dw = rgWaitAnswer;
				if (dw == 0) {
					if (m_pAudio->IsPlaying()) {
						m_pAudio->PlayStop();
					} 
					if (m_pAudio->IsPlayingSkype()) {
						m_pAudio->PlayStopSkype();
					} 
					/// �d�b�ɏo��
					m_ProcCode = _T("$");		// �ŏ��̉���
					pChangedCall->Status = prgInProgress;
				} else {
					/// ����ԉ����҂��^�C�}�̋N��
					pFrame->StartAnswerWaitTimer();
				}
			}
			break;
		case prgInProgress:	// Call is in progress
			if (pFrame->m_flgEnable == 1 && !m_ProcCode.IsEmpty()) {
			// ����ԏ���
				/// ����ԉ����҂��^�C�}�����̏I��
				pFrame->StopAnswerWaitTimer();
				// �����������R�[��
				if (!Response()) return S_FALSE;
			} else if (pFrame->m_flgAutoEnable == 1 && m_ProcCode.IsEmpty()) {
			// �����^������
				pFrame->OnRecordStart();
			}
			break;
		case prgOnHold:		// Call is placed on hold
			if (pFrame->m_flgEnable == 0) return S_OK;	// ���������̏ꍇ
			// ����ԑΉ����ۗ̕��͏I�������Ƃ���
			if (theApp.m_lngAppStatus == 2) {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("Abort hold call"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
				id = pChangedCall->GetID();
				if (id > 0) {
					pChangedCall->Status = prgFinished;
				}
			}
			break;
	/// �ʘb���I�����Ă���ꍇ
		case prgFinished:	// Call is finished
		case prgMissed:		// Call was missed
			// Ring���ɂ���������ꍇ�����遨�R�[���������Ă���̂Ŏd���Ȃ��iVer1.2�j
		case prgRefused:	// Call was refused
		case prgCancelled:	// Call was cancelled
			if (IsCallFinishing()) break;		// ���ݒʘb�I�����̏ꍇ
			SetCallFinish(TRUE);				// �ʘb�I�����t���O��ON
			if (pChangedCall != NULL) {		// NULL:�����I��
				/// �֌W�̂Ȃ��R�[���i�������ɂ������Ă����ʂ̃R�[���j�̏ꍇ�I���������Ȃ�
				if ((m_pCall == NULL) ||
					(m_pCall != NULL && pFrame->m_stAnswerWait.blOnTimer)) {
					bStr = pChangedCall->PartnerHandle;
					str.Format(_T("%s"), (LPCTSTR)bStr);
					if (m_CallStatus.strPartnerId != str)  {
						SetCallFinish(FALSE);					// �ʘb�I�����t���O��OFF
						break;
					}
				}
				/// ������b���̏ꍇ�������Ȃ��i��c�ʘb�E�ʘb���̒��M�Ȃǂ̏ꍇ�j
				if (IsTalking()) {
					SetCallFinish(FALSE);					// �ʘb�I�����t���O��OFF
					break;
				}
			}
			/// ����Ԙ^���p�^�C�}�[�̏I��
			pFrame->StopAnswerWaitTimer();
			pFrame->StopRecordMaxTimer();
			pFrame->StopPlayChkTimer();

			if (m_pVideo->IsPlaying()) {
				m_pVideo->PlayStop();
			} 
			if (m_pVideo->IsPlayingSkype()) {
				m_pVideo->PlayStopSkype();
			} 
			if (m_pAudio->IsPlaying()) {
				m_pAudio->PlayStop();
			} 
			if (m_pAudio->IsPlayingSkype()) {
				m_pAudio->PlayStopSkype();
			} 
			//Ver1.00 �r�f�I�I����̉�ʂ��c��̂ō폜
			// Sleep(1000);	// �C�x���g����������ꍇ������̂ŁA�����҂�
			if (theApp.m_lngAppStatus == 1) {
			/// ��b�^��
				blRet = m_pAudio->IsRecordingSkype();
				if (blRet) {
					if (pFrame->m_wndToolBar.VRecordStop(TRUE)) {
						pFrame->m_wndToolBar.RecordStop(FALSE);
					} else {
						pFrame->m_wndToolBar.RecordStop(TRUE);
					}
				}
			} else if (theApp.m_lngAppStatus == 2 && m_ProcCode == _T("$$")) {
			/// ����^���i�^�����ĂȂ��Ă��ʘb�L�^�͎c���j
			// �������Ԃ̘^���I���̓^�C�}�ōs���̂ŁA�����ł͎��s���Ȃ�
				/// �^���^�抮��
				FinishRecord();
				//// �}�C�N��MUTE��߂��iSkype�������Ŗ߂����O�̂��߁j
				//m_pAccess->Mute = 0;	//FALSE
				/// �����R�[�h���X�V
				m_ProcCode += _T("$");		
			}
			if (IsSkypeVideo()) SetSkypeVDevice(m_defVDevice);	// SKYPE�r�f�I�f�o�C�X��߂�
			/// ����ԏI�����ɘA�g�v���O���������s
			if (theApp.m_lngAppStatus == 2) {
				UST_RSF_RESP st_resp;
				UST_RSF_RESP *pst_resp = &st_resp;
				//CString strCode = _T("$$$$");
				// �ؒf�̃^�C�~���O�ŉ����R�[�h���ς���Ă��܂��i�b��I�ɗ����`�F�b�N�j
				m_Setup.GetResponse(m_ProcCode, pst_resp);
				if (pst_resp->exec_id < 1) {
					m_Setup.GetResponse(m_ProcCode + _T("$"), pst_resp);
				}
				ExecResp(pst_resp->exec_id);
			}
			m_pCall = NULL;			// �ʘb���I���iFinishRecord()���g�p����̂ŁA���̈ʒu�Łj
			m_ProcCode.Empty();		// �����R�[�h��������
			theApp.m_lngAppStatus = 0;		// APP��ԃt���O�����Z�b�g
			SetResponsing(FALSE);			// �������t���O��OFF
			SetCallFinish(FALSE);			// �ʘb�I�����t���O��OFF
			pFrame->SetFocus();				// TaskTray�̕\���X�V�̂��߂Ƀt�H�[�J�X���ڂ�
			break;
		case prgBusy:		// Destination is busy
			break;
		default:
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Undefined call status:%d"), Status);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			break;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		goto PROC_ERR;
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, err);
		goto PROC_ERR;
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("unknown error"));
		goto PROC_ERR;
	}

	return S_OK;

PROC_ERR:
	// �ʘb�̃G���[�I���������i��O������������͔̂�����j
	m_pCall = NULL;
	m_ProcCode.Empty();		// �����R�[�h��������
	theApp.m_lngAppStatus = 0;		// APP��ԃt���O�����Z�b�g
	SetResponsing(FALSE);			// �������t���O��OFF
	SetCallFinish(FALSE);			// �ʘb�I�����t���O��OFF
	/// SKYPE�r�f�I�f�o�C�X��߂�
	if (IsSkypeVideo()) SetSkypeVDevice(m_defVDevice);
	/// Skype�Ɛڑ��؂ꂵ�Ă���\�����������Đڑ�
	pFrame->ConnectSkype();

	return S_FALSE;
}

HRESULT CAccessEvents::Command(BSTR SkypeCommand)
{

	return S_OK;
}

HRESULT CAccessEvents::UserStatusChanged(SkypeOnlineStatus Status)
{
	try {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("UserStatusChanged:%d"), Status);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		switch (Status)
		{
		case olsUnknown:	
			break;
		case olsOffline:	
			break;
		case olsOnline:
			/// �i070218�j�I�����C���Ȃ̂�API�ڑ��ʒm�����Ă��Ȃ������ꍇ
			if (theApp.m_blSkypeConnect == FALSE) {
				APIStatusChanged(apiAttachSuccess);
			}
			break;
		case olsAway:	
			break;
		case olsNotAvail:	
			break;
		case olsDoNotDisturb:	
			break;
		case olsInvisible:	
			break;
		case olsSkypeMe:	
			break;
		case olsLoggedOut:	
			break;
		default:
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Undefined Online status:%d"), Status);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			break;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return S_FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, err);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CAccessEvents::MuteChanged(VARIANT_BOOL Mute)
{
	return S_OK;
}

HRESULT CAccessEvents::ConnectionStatusChanged(SkypeConnectionStatus Status)
{
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();

	try {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("ConnectionStatusChanged:%d"), Status);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		switch (Status)
		{
		case conUnknown:	
			break;
		case conOnline:		// Skype is online
			break;
		case conConnecting:	// Skype is connecting
			break;
		case conPausing:	// Skype's connectivity has been suspended  
			break;
		case conOffline:	// Skype is offline  
			break;
		case conLoggedOut:	// Skype's user has logged out
			/// �O��g�p�������[�U���g�p
			InitDummyUser();
			theApp.m_blSkypeConnect = FALSE;		// �ڑ��t���O
			pFrame->m_flgEnable = 0;		// ���������t���O�𖳌�
			pFrame->m_flgAutoEnable = 0;	// �����^�������t���O�𖳌�
			/// ��ʂ̍X�V�i�\������Ă���ꍇ�j
			pFrame->UpdateForm();
			break;
		default:
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Undefined Connection status:%d"), Status);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			break;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return S_FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, err);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CAccessEvents::MessageReceived(IMessagePtr pSkypeMessage)
{
	return S_OK;
}

HRESULT CAccessEvents::MessageSent(IMessagePtr pSkypeMessage)
{
	return S_OK;
}

HRESULT CAccessEvents::APIStatusChanged(SkypeAPIAttachmentStatus Status)
{
	int ret = 0;
	_bstr_t bStr;
	CString str, str1;
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRegDWORD rgDisable;
	DWORD dw;

	try {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("APIStatusChanged:%d"), Status);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		switch (Status)
		{
		case apiAttachUnknown:	
			break;
		case apiAttachSuccess:	
			if (theApp.m_blSkypeConnect == TRUE) break;		// �ڑ��ς�
			theApp.m_blSkypeConnect = TRUE;		// �ڑ��t���O
			/// �X�J�C�v�ڑ��҂��^�C�}�[�̏I��
			pFrame->StopConnectTimer();
			/// �Â����O�t�@�C���̍폜
			if (!te.DelOldLog()) {
				throw _T("Can't clean log files");
			}	
			/// Skype�f�o�C�X�̏�����
			if (!InitDevice()) {
				//// �G���[�̏ꍇ�V�X�e���I��
				pFrame->DestroyWindow();
				return S_FALSE;
			}
			/// DB�t�@�C���̑��݃`�F�b�N�����݂��Ȃ��ꍇ�V�K�쐬����
			if (!InitUserData()) {
				throw _T("Can't create user database");
			}
			/// �f�[�^�x�[�X�̃o�[�W�����A�b�v
			if (!VerUpUserData()) {
				throw _T("Can't version up user database");
			}	
			/// ���������t���O��ݒ�
			//// ���W�X�g���̏����l�`�F�b�N
			rgDisable = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\InitDisable"), 0, TRUE);
			dw = rgDisable;
			pFrame->m_flgEnable = (dw == 1) ? 0 : 1;
			/// �����^�������t���O��ݒ�
			//// ���W�X�g���̏����l�`�F�b�N
			rgDisable = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableAutoVoiceRecord"), 0, TRUE);
			dw = rgDisable;
			pFrame->m_flgAutoEnable = dw;
			/// �o���[�����b�Z�[�W�̕\��
			str1.LoadString(IDS_MSGBOX5);
			pFrame->m_TrayIcon.ShowBalloon(str1, str, NIIF_INFO, 10);	// ? 10 sec �ȏ�K�{
			/// �c�[���`�b�v�X�V
			pFrame->InitTooltip();	// �c�[���`�b�v�̏�����
			/// ��ʂ̍X�V�i�\������Ă���ꍇ�j
			pFrame->UpdateForm();
			break;
		case apiAttachPendingAuth:	
			break;
		case apiAttachRefused:	
			/// �O��g�p�������[�U���g�p
			InitDummyUser();
			theApp.m_blSkypeConnect = FALSE;		// �ڑ��t���O
			pFrame->m_flgEnable = 0;		// ���������t���O�𖳌�
			pFrame->m_flgAutoEnable = 0;	// �����^�������t���O�𖳌�
			/// �o���[�����b�Z�[�W�̕\��
			if (!pFrame->m_stConnect.blOnTimer) {
				//// �X�J�C�v�Ď��^�C�}�[�N�����͐ڑ��s���b�Z�[�W�͏o���Ȃ�
				str.LoadString(IDS_MSGBOX0);
				str = _T("Tapur - ") + str;
				str1.LoadString(IDS_MSGBOX1);
				if (!pFrame->m_TrayIcon.ShowBalloon(str1, str, NIIF_INFO, 20)) {
					//// ��Windows Shell�ł͕\������Ȃ����߁A���b�Z�[�W�{�b�N�X�ő�p
					str += _T("\n\n") + str1;
					AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
				}
				/// �c�[���`�b�v�X�V
				pFrame->m_TrayIcon.SetTooltipText(IDS_TOOLTIP_DISCON);
			}
			/// ��ʂ̍X�V�i�\������Ă���ꍇ�j
			pFrame->UpdateForm();
			/// �X�J�C�v�ڑ��҂��^�C�}�̋N��
			if (pFrame->m_stConnect.blOnTimer == FALSE) {
				pFrame->StartConnectTimer();
			}
			break;
		case apiAttachNotAvailable:	
			break;
		case apiAttachAPIAvail:	
			break;
		default:
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Undefined API status:%d"), Status);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			break;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return S_FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, err);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CAccessEvents::CallHistoryChanged(void)
{
	return S_OK;
}

HRESULT CAccessEvents::IMHistoryChanged(void)
{
	return S_OK;
}

HRESULT CAccessEvents::Error(LONG Number, BSTR Description)
{
	return S_OK;
}

HRESULT CAccessEvents::OnlineStatusChanged(IUserPtr pSkypeUser, SkypeOnlineStatus Status)
{

	try {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("OnlineStatusChanged:%d"), Status);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		switch (Status)
		{
		case olsUnknown:	
			break;
		case olsOffline:	// PSTN
			break;
		case olsOnline:	
			break;
		case olsAway:	
			break;
		case olsNotAvail:
			break;
		case olsDoNotDisturb:	
			break;
		case olsInvisible:	
			break;
		case olsSkypeMe:	
			break;
		case olsLoggedOut:	
			break;
		default:
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Undefined Online status:%d"), Status);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			break;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return S_FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, err);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CAccessEvents::ChatMessageReceived(IChatMessagePtr pSkypeChatMessage)
{
	return S_OK;
}

HRESULT CAccessEvents::ChatMessageSent(IChatMessagePtr pSkypeChatMessage)
{
	return S_OK;
}

//
//  �@�\     : AccessEvents����
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : STDMETHODIMP
//  
//  ���l     : Invoke�ɂ��n�����C�x���g�̈���DISPPARAMS�̔z��͋t�����Ɏg�p�����i�X�^�b�N�`���H�j
//  
STDMETHODIMP CAccessEvents::Invoke(DISPID dispid, REFIID riid, LCID lcid, 
	WORD wFlags, DISPPARAMS* pdispParams, VARIANT* pvarResult, 
	EXCEPINFO* pexcepinfo, UINT* puArgErr)
{
	if (riid != IID_NULL) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_UNKNOWNINTERFACE"));
		return DISP_E_UNKNOWNINTERFACE;
	}

	HRESULT hr;
	CComVariant varArg1;
	CComVariant varArg2;
	USES_CONVERSION;

	_bstr_t bstr1;
	_bstr_t bstr2;

	if (!(wFlags & DISPATCH_METHOD)) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_MEMBERNOTFOUND"));
		return DISP_E_MEMBERNOTFOUND;
	}

#ifdef _DEBUG
//_stprintf_s(te.errstr, MAX_ERRSTR, _T("Skype AccessEvents:%d"), dispid);
//te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
#endif
	switch (dispid)
	{
	case 1:	// Result
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_BSTR, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		bstr1 = _bstr_t(varArg1.bstrVal, true);
		varArg1.Clear();
		return Result(bstr1);
	case 2:	// CallStatusChanged
		if (pdispParams->cArgs < 2) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_I4, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		hr = varArg2.ChangeType(VT_DISPATCH, (&pdispParams->rgvarg[1]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return CallStatusChanged((ICall *)varArg2.pdispVal, (SkypeCallProgress)varArg1.lVal);
	case 3:	// Command
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_BSTR, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		bstr1 = _bstr_t(varArg1.bstrVal, true);
		varArg1.Clear();
		return Command(bstr1);
	case 4:	// UserStatusChanged
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_I4, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return UserStatusChanged((SkypeOnlineStatus)varArg1.lVal);
	case 5:	// MuteChanged
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_BOOL, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return MuteChanged(varArg1.boolVal);
	case 6:	// ConnectionStatusChanged
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_I4, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return ConnectionStatusChanged((SkypeConnectionStatus)varArg1.lVal);
	case 7:	// MessageReceived
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_DISPATCH, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return MessageReceived((IMessage *)varArg1.pdispVal);
	case 8:	// MessageSent
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_DISPATCH, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return MessageSent((IMessage *)varArg1.pdispVal);
	case 9:	// APIStatusChanged
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_I4, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return APIStatusChanged((SkypeAPIAttachmentStatus)varArg1.lVal);
	case 10:	// CallHistoryChanged
		return CallHistoryChanged();
	case 11:	// IMHistoryChanged
		return IMHistoryChanged();
	case 12:	// Error
		if (pdispParams->cArgs < 2) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_BSTR, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		hr = varArg2.ChangeType(VT_I4, (&pdispParams->rgvarg[1]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		bstr1 = _bstr_t(varArg1.bstrVal, true);
		varArg1.Clear();
		return Error(varArg2.lVal, bstr1);
	case 13:	// OnlineStatusChanged
		if (pdispParams->cArgs < 2) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_I4, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		hr = varArg2.ChangeType(VT_DISPATCH, (&pdispParams->rgvarg[1]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return OnlineStatusChanged((IUser *)varArg2.pdispVal, (SkypeOnlineStatus)varArg1.lVal);
	case 14:	// ChatMessageReceived
/* �G���[���o��ꍇ������̂Ŗ���
		if (pdispParams->cArgs < 2) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_BSTR, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		hr = varArg2.ChangeType(VT_I4, (&pdispParams->rgvarg[1]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return ChatMessageReceived((IChatMessage *)NULL);
*/
		return S_OK;
	case 15:	// ChatMessageSent
/* �g��Ȃ��̂Ŗ���
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_DISPATCH, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return ChatMessageSent((IChatMessage *)varArg1.pdispVal);
*/
		return S_OK;
	default:
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Skype AccessEvents:DISP_E_MEMBERNOTFOUND:%d"), dispid);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return DISP_E_MEMBERNOTFOUND;
	}

	return S_OK;
}


//
//  �@�\     : BEEP���o��
//  
//  �@�\���� : �X�s�[�J�E���C�������Đ�
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAccessEvents::PlayBeep(
long lngSecond)			// �Đ�����(���g�p)
{
	BOOL blRet = FALSE;

	CTPsetup tps(theApp.m_strDataFolder);
	CString strFile = tps.RsfBeepFileName();
	CFileSpec fs(strFile);
	if (fs.Exist()) {
		m_pAudio->m_PlayFile = fs.GetFullSpec();
		m_pAudio->PlayOpen();				// ���j�^�[�Đ�
		m_pAudio->Play();

		m_pAudio->PlayOpenSkype(m_pCall);	// SkypeAPI�Đ�
		if (m_pAudio->PlaySkype(TRUE)) {	// full play now
			m_pAudio->PlayStopSkype();
			m_pAudio->PlayStop();
			return TRUE;
		}
		m_pAudio->PlayStopSkype();
		m_pAudio->PlayStop();
	}

	// �G���[�������Ƀo�b�N�A�b�v�t�@�C���Ń��g���C
	strFile = tps.RsfBeepFileName(_T("1"));
	fs.SetFullSpec(strFile);
	if (fs.Exist()) {
		m_pAudio->m_PlayFile = fs.GetFullSpec();
		m_pAudio->PlayOpen();				// ���j�^�[�Đ�
		m_pAudio->Play();

		m_pAudio->PlayOpenSkype(m_pCall);	// SkypeAPI�Đ�
		if (m_pAudio->PlaySkype(TRUE)) {	// full play now
			m_pAudio->PlayStopSkype();
			m_pAudio->PlayStop();
			return TRUE;
		}
		m_pAudio->PlayStopSkype();
		m_pAudio->PlayStop();
	}

	// �G���[�������Ƀo�b�N�A�b�v�t�@�C���Ń��g���C
	strFile = tps.RsfBeepFileName(_T("1"));
	fs.SetFullSpec(strFile);
	if (fs.Exist()) {
		m_pAudio->m_PlayFile = fs.GetFullSpec();
		m_pAudio->PlayOpen();				// ���j�^�[�Đ�
		m_pAudio->Play();

		m_pAudio->PlayOpenSkype(m_pCall);	// SkypeAPI�Đ�
		if (m_pAudio->PlaySkype(TRUE)) {	// full play now
			m_pAudio->PlayStopSkype();
			m_pAudio->PlayStop();
			return TRUE;
		}
		m_pAudio->PlayStopSkype();
		m_pAudio->PlayStop();
	}

	return blRet;
}

//
//  �@�\     : �X�J�C�v�R�[��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAccessEvents::SendCall(
CString strSender)		// SKYPE��
{
	CString str;

	/// Skype�̏�Ԋm�F
	if (!theApp.m_blSkypeConnect) {
		str.LoadString(IDS_MSGBOX2);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}
	int intStatus = m_pAccess->GetConnectionStatus();
	if (intStatus != conOnline) {
		str.LoadString(IDS_MSGBOX15);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}
	ICallCollectionPtr pCallCol = m_pAccess->GetActiveCallList();
	if (pCallCol->GetCount() > 0) {
		str.LoadString(IDS_MSGBOX16);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	/// �m�F
	str.LoadString(IDS_MSGBOX14);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;

	/// Skype�ɔ��M
	_TCHAR _str[MAX_LEN];
	_tcscpy_s(_str, MAX_LEN, strSender);
//	theApp.m_blSenderCall = TRUE;	// ���[�U���M�t���O���Z�b�g
// SkypeAPI 1.0.1.06 �ł̓G���[�ƂȂ邽��
	_bstr_t bStr;
	//str.Empty();
	//str = str + _T("CALL ") + strSender;
	//bStr = _bstr_t((LPCTSTR)str);
	//m_pAccess->SendCommand(bStr);
	bStr = _bstr_t((LPCTSTR)strSender);
	ICallPtr pCall = m_pAccess->PlaceCall(bStr);
	
	return TRUE;
}

//
//  �@�\     : �ʘb���̃��[�U�Ƀ`���b�g���M
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAccessEvents::SendChat(
CString strMsg,		// ���M���b�Z�[�W
CString strUserId)	// ���M��X�J�C�vID�A��̏ꍇ�A�N�e�B�u�ȑS���[�U�Ώ�
{
	long i = 0;
	long lngCnt = 0;
	_bstr_t bStrMsg;
	_bstr_t bStrHandle;
	ICallPtr pCall;
	IMessagePtr pMsg;
	_variant_t vt;

	bStrMsg = _bstr_t((LPCTSTR)strMsg);

	try {
		if (strUserId.IsEmpty()) {
			ICallCollectionPtr pCallCol = m_pAccess->GetActiveCallList();
			if (pCallCol == NULL) goto PROC_END;
			lngCnt = pCallCol->GetCount();
			if (lngCnt <= 0) goto PROC_END;
			for(i = 1; i <= lngCnt; i++) {
				vt = i;
				pCall = pCallCol->GetItem(vt);
				/// P2P�R�[���̏ꍇ�̂ݑ��M��
				if (!(pCall->Type == ctypIncomingP2P || pCall->Type == ctypOutgoingP2P)) continue;
				bStrHandle = pCall->PartnerHandle;
				pMsg = m_pAccess->SendMessageA(pCall->PartnerHandle, bStrMsg);
//				m_pAccess->SendMessage(pCall->PartnerHandle, bStrMsg);
			}
		} else {
			pMsg = m_pAccess->SendMessageA((LPCTSTR)strUserId, bStrMsg);
//			m_pAccess->SendMessage((LPCTSTR)strUserId, bStrMsg);
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("SendChat error:%d/%d"), i, lngCnt);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
PROC_END:
	return FALSE;
}

//
//  �@�\     : �ʘb�����ۂ�
//  
//  �@�\���� : ��c���l���B�P����ł���b���p�����Ă���ΗL���Ƃ���B
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : m_pAccess->GetActiveCallList()���G���[�ɂȂ�₷���H
//  
BOOL CAccessEvents::IsTalking(
BOOL blUpdate)	// �ʘb���̃R�[�����������ꍇ�Am_pCall�ɔ��f����
{
	long i = 0;
	long lngCnt = 0;
	_bstr_t bStr;
	CString strTmp;
	ICallPtr pCall;
	_variant_t vt;

//	if (m_pCall == NULL) return FALSE;

	try {
		ICallCollectionPtr pCallCol = m_pAccess->GetActiveCallList();
		if (pCallCol == NULL) goto PROC_END;
		lngCnt = pCallCol->GetCount();
		if (lngCnt <= 0) goto PROC_END;
		for(i = 1; i <= lngCnt; i++) {
			vt = i;
			pCall = pCallCol->GetItem(vt);
			if (pCall->Status == prgInProgress) {
				if (blUpdate) {
					m_pCall = pCall;
					m_CallStatus.SetCall(m_pCall);
					m_Setup.SetCallerId(m_CallStatus.strPartnerId);	// ���M��ID
				}
				return TRUE;
			}
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("IsTalking call error:%d/%d"), i, lngCnt);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

PROC_END:
	return FALSE;
}

//
//  �@�\     : �r�f�I�ʘb�����ۂ�
//  
//  �@�\���� : ��M�f�����Ȃ����FALSE�Ƃ���B
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : API���s����Ȃ��߁A�r�f�I�E�B���h�E�̑��݂Ŕ���
//  
BOOL CAccessEvents::IsVTalking()
{
	CString str;
	_bstr_t bStr;
	_bstr_t bStrRet;

	/// ��b��Ԃ��m�F
	if (!IsTalking(TRUE)) return FALSE;

	/// �r�f�I��M��Ԃ��m�F
	try {
		RECT rct;
		LPRECT pRect = &rct;
		if (m_pVideo->GetVideoWinLarge(pRect)) return TRUE;
		if (m_pVideo->GetVideoWinLocal(m_CallStatus.strPartnerName, pRect)) return TRUE;
		//str.Format(_T("GET CALL %d VIDEO_RECEIVE_STATUS"), m_CallStatus.lngCallId);
		//bStr = _bstr_t((LPCTSTR)str);
		//bStrRet = m_pAccess->SendBlockingCommand(bStr);
		//// ex) "CALL <id> VIDEO_RECEIVE_STATUS RUNNING"
		//str.Format(_T("%s"), (LPCTSTR)bStrRet);
		//if (str.Find(_T("RUNNING")) >= 0 || str.Find(_T("STARTING")) >= 0) {
		//	return TRUE;
		//}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("IsVTalking call error:%d"), m_CallStatus.lngCallId);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return FALSE;
}

//
//  �@�\     : ���M�҂ɑΉ����������ݒ�����[�h
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAccessEvents::LoadSetup()
{
	_bstr_t bStr;
	CString strTmp;

	if (m_pCall == NULL) return FALSE;

	/// ���M�҂ɑΉ����������ݒ�����[�h
	bStr = m_CallStatus.strPartnerId;			// ���M�Җ��i�X�J�C�v���j
	strTmp.Format(_T("%s"), (LPCTSTR)bStr);
	m_Setup.SetDataDir(theApp.m_strDataFolder);		// �f�[�^�t�H���_
	m_Setup.SetCallerId(strTmp);					// ���M��ID
	
	/// RSF�ݒ�̃��[�h
	if (m_Setup.LoadRSF() == NULL) return FALSE;

	return TRUE;
}

//
//  �@�\     : ���M�҂̖���orID
//  
//  �@�\���� : ���̂��Ȃ��ꍇID
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
CString CAccessEvents::GetSender()
{
	_bstr_t bStr;
	CString strRet;

	strRet.Empty();
	if (m_pCall == NULL) return strRet;

	bStr = m_CallStatus.strPartnerName;			// ���M�Җ��i�\�����j
	strRet.Format(_T("%s"), (LPCTSTR)bStr);	
	if (strRet.IsEmpty()) {
		bStr = m_CallStatus.strPartnerId;			// ���M�Җ��i�X�J�C�v���j
		strRet.Format(_T("%s"), (LPCTSTR)bStr);
	}

	return strRet;
}

//
//  �@�\     : �^���E�^��J�n���̏���
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 1-audio only, 2-video only, 3-audio & video, 0-error
//  
//  ���l     : 
//  
int CAccessEvents::StartRecord(BOOL blVideo)
{
	int intRet = 0;
	CString strRecordFile;
	CString strDate = m_Setup.GetDateStr();

	try {
		CString strSender = GetSender();
		if (strSender.IsEmpty()) return 0;

		m_Setup.SetCallerId(m_CallStatus.strPartnerId);

		if (blVideo) {
		//// �r�f�I�^��J�n
			///// �r�f�I��M��Ԃ��m�F
			if (IsVTalking()) {
				///// ���k�`���̎w��
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
					//// �^�������̎w��imono�j
					m_pAudio->SetRecordSyncStereo(0);
					WAVEFORMATEX Fmt = m_pAudio->pSocketIn->GetWaveFormat();		// �ۑ��pWAVE�t�H�[�}�b�g
					CPtrList *pList = m_pAudio->pSocketIn->GetOutListBuffer();		// �����X�g���[���p�o�b�t�@
					m_pVideo->SetAudio(&Fmt, pList);
				} else {
					m_pVideo->SetAudio(NULL, NULL);
				}
				///// �r�f�I�^��J�n
				if (m_pVideo->RecordOpen(strSender)) {
					strRecordFile = m_Setup.RdfVideoFileName(strDate, m_ProcCode);	// �^��t�@�C��
					if (m_pVideo->Record(strRecordFile)) {
						intRet = 2;
					}
				}
			}
		}

		/// SkypeAPI�ŉ����^���J�n
		if (m_pAudio->RecordOpenSkype(m_pCall)) {
			//// ����d�̎���Stream error �`�F�b�N�͕s�v�i�Z���Ԙ^���ƍ������₷���j
			m_pAudio->EnableStreamErrSkype(FALSE);
			//// �^�������̎w��imono�j
			m_pAudio->SetRecordSyncStereo(0);
			//// �^���Ώۂ̎w��i��M�����j
			m_pAudio->SetRecordSyncSw(1);
			//// �ۑ��`���̎w��iWAV/MP3�j
			CRegDWORD rgFormatType(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordType"), 0, TRUE);		// �����l:0(WAV)
			int intFormatType = rgFormatType;
			CRegString rgCodecName(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordMP3Codec"), _T(""), TRUE);
			CString strCodec = rgCodecName;
			CRegString rgFormatName(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordMP3Format"), _T(""), TRUE);
			CString strFormat = rgFormatName;
			if (m_pVideo->IsRecording()) {
				// �^�掞��WAV�ɌŒ�
				intFormatType = 0;
				// �^��p�����o�b�t�@�̐������w��
				m_pAudio->pSocketIn->EnableOutBuffer(TRUE);
			} 
			m_pAudio->SetRecordFormat(intFormatType, strCodec, strFormat);
			//// �^���t�@�C����
			CString strRecordFile;
			if (intFormatType > 0) {
				strRecordFile = m_Setup.RdfMp3FileName(strDate, m_ProcCode);
			} else {
				strRecordFile = m_Setup.RdfWavFileName(strDate, m_ProcCode);
			}
			//// �|�[�g�̎w��i�g�p���Ȃ�MIC�|�[�g���K�v�j
			CRegDWORD rgSkypePortOut = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortOut"), 42805, TRUE);
			CRegDWORD rgSkypePortMic = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortMic"), 42806, TRUE);
			CString strPortOut, strPortMic;
			strPortOut.Format(_T("%d"), (DWORD)rgSkypePortOut);
			strPortMic.Format(_T("%d"), (DWORD)rgSkypePortMic);
			if (m_pAudio->RecordSkype(strRecordFile, strPortOut, strPortMic, m_defOutDevice)) {
			//if (m_pAudio->RecordSkype(strRecordFile, m_defOutDevice)) {
				intRet += 1;
				/// ����Ԙ^��MAX���ԃ`�F�b�N�^�C�}�̋N��
				CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
				pFrame->StartRecordMaxTimer();
			} else {
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Start Record error"));
				return FALSE;
			}
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Start Record error"));
		return FALSE;
	}

	return intRet;
}
//
//  �@�\     : �^���E�^�抮�����̃e�[�u���ۑ�
//  
//  �@�\���� : ��M���̍X�V���s�Ȃ�
//  
//  �Ԃ�l   : 
//  
//  ���l     : �T���x�[�W���ꂽ�f�[�^�̓o�^�ɂ��g�p�����
//  
BOOL CAccessEvents::FinishRecordTbl(
CString strAudioFile,	// �^���t�@�C��
CString strVideoFile,	// �^��t�@�C��
BOOL blRepair)			// �T���x�[�W�̏ꍇ
{
	_bstr_t bStr;
	CString str;
	CString strTmp;
	CString strWhere;
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();

	try {
		/// �t�@�C�������烌�R�[�h�J�n�������Z�o
		CString strDate = m_Setup.GetAudioFileDate(strAudioFile);
		if (strDate.IsEmpty()) strDate = m_Setup.GetDateStr();
		/// RDFL�e�[�u���Əƍ�
		strWhere.Empty();
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		strWhere = strWhere + _T(" and ") + _T("lngSystemNew") + _T("=") + strDate;
		CTblRdfl tbl(theApp.m_strDBPath);
		if (tbl.CountRec(strWhere) > 0) return FALSE;	// ���ɓo�^�ς�

		/// RDFL�e�[�u���֕ۑ�
		tbl.lngMsgType = (strVideoFile.IsEmpty()) ? RDF_TYPE_AUDIO : RDF_TYPE_VIDEO;
		//bStr = m_CallStatus.strPartnerId;		// m_Setup.CallerId�̓f�t�H���g�l�̏ꍇ������̂Ŏg�p�s��
		//m_Setup.SetDataDir(theApp.m_strDataFolder);	// �f�[�^�t�H���_
		//m_Setup.SetCallerId(m_CallStatus.strPartnerId);	// ���M��ID
		if (strAudioFile.IsEmpty()) {
			// �^������Ȃ������ꍇ
			tbl.strSenderId = m_CallStatus.strPartnerId;	// ���M�Җ��i�X�J�C�v���j
		} else {
 			tbl.strSenderId = m_Setup.GetAudioFileCallerId(strAudioFile);	// ���M�Җ��i�X�J�C�v���j
		}
		if (m_pCall != NULL) {		// �T���x�[�W�łȂ��ꍇ
			bStr = m_CallStatus.strPartnerName;
			tbl.strSenderName = m_CallStatus.strPartnerName;// ���M�Җ��i�\�����j
			bStr = m_pCall->Subject;
			strTmp.Empty();
			if (bStr.length() > 0) {
				strTmp.Format(_T("%s"), (LPCTSTR)bStr);
			}
			tbl.strTitle = strTmp;						// CALL SUBJECT
			tbl.lngRespBaseId = m_Setup.GetRespId();
			tbl.lngClsCode1 = (long)m_pCall->Type + 1;		// �R�[���^�C�v
		}
		tbl.strReceiverId = theApp.m_strUsrId;		// ��M�Җ��i�X�J�C�v���j
		tbl.strReceiverName = theApp.m_strUsrName;	// ��M�Җ��i�\�����j
		if (theApp.m_lngAppStatus == 1) {
			tbl.lngRespType = RESPBASE_TYPE_NOTHING;	// ���ژ^���E�^��
		} else if (theApp.m_lngAppStatus == 2) {
			tbl.lngRespType = RESPBASE_TYPE_SIMPLE;		// �����
		} else {
			if (blRepair) {
				tbl.lngRespType = RESPBASE_TYPE_NOTHING;	// �T���x�[�W���ꂽ�ꍇ
				tbl.strTitle.LoadString(IDS_REPAIR_NAME);	// CALL SUBJECT
			} else {
				return FALSE;	// �C�x���g�d���ŃR�[�������ꍇ������i11.04.24�j
			}
		}
		tbl.strRemarks1 = m_Setup.GetDataFolder();		// �f�[�^�p�X
		tbl.InsertRec(strDate);

		/// ���[�U�e�[�u���֕ۑ�
		CTblPerson ptbl(theApp.m_strDBPath);
		if (ptbl.ExistTbl()) {
			// �e�[�u�������݂���ꍇ�̂�
			CString strWhere = _T("strId='") + m_CallStatus.strPartnerId + _T("'");
			long lngCnt = ptbl.SelectRec(strWhere);
			ptbl.strId = m_CallStatus.strPartnerId;			// �ʘb����(�X�J�C�vID);
			ptbl.strName = m_CallStatus.strPartnerFullName;	// �ʘb����(�t����);
			ptbl.strAbbName = m_CallStatus.strPartnerName;	// �ʘb����(�X�J�C�v��);
			ptbl.lngBirthday = m_CallStatus.lngBirthday;	// �a����;
			ptbl.lngSex = m_CallStatus.lngSex;	// ���ʁiusexUnknown = -1, usexNotSpecified = 0, usexMale = 1, usexFemale = 2)
			ptbl.strLanguage = m_CallStatus.strLanguage;	// ����
			ptbl.strLanguageCode = m_CallStatus.strLanguageCode;// ����R�[�h
			ptbl.strCountry = m_CallStatus.strCountry;		// �Z���i���j
			ptbl.strCountryCode = m_CallStatus.strCountryCode;	// �Z���i���R�[�h�j
			ptbl.strProvince = m_CallStatus.strProvince;	// �Z���i�B/���j
			ptbl.strCity = m_CallStatus.strCity;			// �Z���i�s�j
			ptbl.strTel = m_CallStatus.strPhoneHome;		// �d�b�i����j
			ptbl.strWkTel = m_CallStatus.strPhoneOffice;	// �d�b�i�I�t�B�X�j
			ptbl.strMbTel = m_CallStatus.strPhoneMobile;	// �d�b�i���o�C���j
			ptbl.strWeb = m_CallStatus.strHomepage;			// HP
			ptbl.strAbout = m_CallStatus.strAbout;			// ���ȏЉ�
			ptbl.strMood = m_CallStatus.strMoodText;		// MoodText
			ptbl.lngTimeZone = m_CallStatus.lngTimezone;	// Timezone
			ptbl.lngSystemFlag = 0;		// �O�̂��ߍ폜�t���O��ݒ�
			if (lngCnt > 0) {
				// �X�V
				ptbl.UpdateRec(ptbl.ID);
			} else {
				// �ǉ�
				ptbl.InsertRec();
			}
		}

		/// ��M�����\������Ă���ꍇ�A���b�Z�[�W���X�g���X�V
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CMsgTree *pfv = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
		if (pfv->m_hActive == pfv->m_hFolderDef) {
			CMsgList *pmv = (CMsgList *)pFrame->m_pMsgList->GetWnd();
			pmv->UpdateForm();
			//// �V�K�A�C�e����I��
			pmv->m_wndList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		}

		/// �V�����b�Z�[�W���X�V
		pFrame->IncNewMsgCnt(1);
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Record Table error"));
		return FALSE;
	}

	return TRUE;
}

//
//  �@�\     : �^���E�^�抮�����̏���
//  
//  �@�\���� : �e�[�u���ۑ�
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAccessEvents::FinishRecord()
{
	int n = 0;
	_bstr_t bStr;
	CString str;
	CString strTmp;
	CString strAudioFile = _T("");
	CString strVideoFile = _T("");
	CString strRecordFile;
	CFileSpec fs;
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CWaitCursor();		// �����v

	if (m_pCall == NULL) return FALSE;

	try {
		/// �^��I��
		if (m_pVideo->IsRecording()) {
			strVideoFile = m_pVideo->GetRecFile();
			m_pVideo->RecStop();
		}
		//// �r�f�I�t�@�C���̐����҂��̏ꍇ�����邽�߁A���g���C���[�v����
		str.LoadString(IDS_MSGBOX79);
		str += " ";
		strTmp = str;
		n = 180;	// max 1.5min
		while(n--) {
			if(!m_pVideo->IsRecording()) break;
			if (n % 3 == 1) {
				pFrame->SetStatusText(str + _T("+"));
			} else if (n % 3 == 2) {
				pFrame->SetStatusText(str + _T("-"));
			} else {
				pFrame->SetStatusText(str + _T("*"));
				str += "*";
				if (str.Find(_T("********************")) > 0) {
					str = strTmp;	// ���b�Z�[�W��߂�
				}
			}
			Sleep(500); 
			AfxGetThread()->PumpMessage();
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't finish recording video."));
	}

	try {
		/// �^���I��
		if (m_pAudio->IsRecordingSkype()) {
			strAudioFile = m_pAudio->m_RecSkypeFile;
			m_pAudio->RecStopSkype();
		}
		//// �����t�@�C���̐����҂��̏ꍇ�����邽�߁A���g���C���[�v����
		str.LoadString(IDS_MSGBOX78);
		str += " ";
		strTmp = str;
		n = 180;	// max 1.5min
		while(n--) {
			if(!m_pAudio->IsRecordingSkype()) break;
			if (n % 3 == 1) {
				pFrame->SetStatusText(str + _T("+"));
			} else if (n % 3 == 2) {
				pFrame->SetStatusText(str + _T("-"));
			} else {
				pFrame->SetStatusText(str + _T("*"));
				str += "*";
				if (str.Find(_T("********************")) > 0) {
					str = strTmp;	// ���b�Z�[�W��߂�
				}
			}
			Sleep(500);
			AfxGetThread()->PumpMessage();
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't finish recording voice."));
	}

	// �^���E�^�抮�����̃e�[�u���ۑ�
	FinishRecordTbl(strAudioFile, strVideoFile);

	//// �r�f�I�{�����t�@�C������
	//ULONGLONG ullSz = 0;
	///// �t�@�C���`�F�b�N
	//if (strVideoFile.IsEmpty() || strAudioFile.IsEmpty()) return TRUE;
	//fs.SetFullSpec(strVideoFile);
	//if (!fs.Exist()) return TRUE;
	//ullSz += fs.FileSize();
	//fs.SetFullSpec(strAudioFile);
	//if (!fs.Exist()) return TRUE;
	//CRegDWORD rgAudio(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordWithAudio"), 1, TRUE);	// �����l:1(�����t�^��)
	//if ((DWORD)rgAudio == 0) return TRUE;
	//ullSz += fs.FileSize();
	///// �ꎞ�t�@�C���`�F�b�N
	//if (fs.FreeDisk() < ullSz) {
	//	//// �o���[�����b�Z�[�W�\��
	//	///// STOP�{�^����ɕ\��
	//	CToolBarCtrl &tbc = pFrame->m_wndToolBar.GetToolBarCtrl();
	//	UINT idx = tbc.CommandToIndex(ID_RECORD_STOP);
	//	RECT rc;
	//	tbc.GetItemRect(idx, &rc);
	//	CPoint pnt((rc.left+rc.right)/2, (rc.top+rc.bottom)/2);
	//	CString strTitle;
	//	CString strMsg;
	//	strTitle.LoadString(IDS_MSGBOX131);
	//	strMsg.LoadString(IDS_MSGBOX95);
	//	CBalloonHelp::LaunchBalloon(strTitle, strMsg, pnt, IDI_INFORMATION, 
	//		CBalloonHelp::unSHOW_CLOSE_BUTTON|CBalloonHelp::unSHOW_TOPMOST, 
	//		&pFrame->m_wndToolBar, _T(""), 0);
	//	return TRUE;
	//}
	//CTPsetup tps(theApp.m_strDataFolder);
	//CString strTmpFile = tps.RdfTmpVideoFileName();
	//fs.SetFullSpec(strTmpFile);
	//fs.MkFolder();
	//// ��������(�A�����[�_���Ŏ��s)
	///// ���g�p�̃r�f�I+���������N���X���g�p
	//for (int i = 0; i < sizeof(m_pAviManager)/sizeof(m_pAviManager[0]); i++) {
	//	if (m_pAviManager[i]->m_stApp.blOnTimer) continue;
	//	m_pAviManager[i]->SetMove(TRUE);
	//	m_pAviManager[i]->CopyAviFileDs(strTmpFile, strVideoFile, strAudioFile, FALSE);
	//	break;
	//}

	return TRUE;
}

//
//  �@�\     : �A�g�v���O�����̎��s
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAccessEvents::ExecResp(int intPrgId)
{
	if (intPrgId < 1) return FALSE;

	CString str, strPrgId;
	strPrgId.Format(_T("%d"), intPrgId);
	CString strFile;	// �A�g�v���O����CSV�t�@�C��
	if (m_Setup.GetRespId() == 1) {
		// �f�t�H���g�ݒ莞
		m_Setup.SetCallerId(_T("0"));
		strFile = m_Setup.RsfPrgFileName(_T("$$$$"), strPrgId);
	} else {
		strFile = m_Setup.RsfPrgFileName(_T("$$$$"), strPrgId);
	}
	CFileSpec fs(strFile);
	if (!fs.Exist()) {
		AfxTrace(_T("%s\n"), strFile);
		str.LoadString(IDS_MSGBOX107);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
		return FALSE;
	}
	// �A�g�v���O����CSV�̓��e���擾
	CString strCsv;
	if (!fs.LoadString(&strCsv)) {
		AfxTrace(_T("%s\n"), strCsv);
		str.LoadString(IDS_MSGBOX107);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
		return FALSE;
	}

	CTokenizer tok(strCsv, _T(","));
	CString cs, strExeFile;
	CStringArray aryArg;
	int i = 0;
	BOOL blExe = TRUE;
	while(tok.Next(cs)) {
		//if (i == 0) �\��
		if (i == 1) {
			// ���s�t�@�C���擾
			strExeFile = cs;
			if (strExeFile.Find(_T('\\')) < 0) {
				// �p�X���w�聨�e���v���[�gCSV�t�H���_��ǉ�
				fs.SetFullSpec(FS_APPDIR);
				strExeFile = fs.GetFullSpec() + _T("bin\\") + strExeFile;
			}
			fs.SetFullSpec(strExeFile);
			if (!fs.Exist()) {
				str.LoadString(IDS_MSGBOX107);
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
				blExe = FALSE;
				return FALSE;
			}
		} else if (2 <= i) {
			// �����擾
			if (cs.CompareNoCase(_T("$AudioFile")) == 0) {				// �^���t�@�C��
				strFile = m_pAudio->m_RecSkypeFile;
				fs.SetFullSpec(strFile);
				if (fs.Exist()) {
					aryArg.Add(strFile);
				} else {
					aryArg.Add(_T(""));
				}
			} else if (cs.CompareNoCase(_T("$VideoFile")) == 0) {			// �^��t�@�C��
				strFile = m_pVideo->GetRecFile();
				fs.SetFullSpec(strFile);
				if (fs.Exist()) {
					aryArg.Add(strFile);
				} else {
					aryArg.Add(_T(""));
				}
			} else if (cs.CompareNoCase(_T("$PartnerId")) == 0) {			// �ʘb����(�X�J�C�vID)
					aryArg.Add(m_CallStatus.strPartnerId);
			} else if (cs.CompareNoCase(_T("$PartnerName")) == 0) {		// �ʘb����(�X�J�C�v��)
					aryArg.Add(m_CallStatus.strPartnerName);
			} else if (cs.CompareNoCase(_T("$UsrId")) == 0) {				// ���[�U(�X�J�C�vID)
					aryArg.Add(m_CallStatus.strUsrId);
			} else if (cs.CompareNoCase(_T("$UsrName")) == 0) {			// ���[�U(�X�J�C�v��)
					aryArg.Add(m_CallStatus.strUsrName);
			} else if (cs.CompareNoCase(_T("$PartnerAddress")) == 0) {		// �ʘb����Z��
					str = m_CallStatus.strCountry;
					str += m_CallStatus.strProvince;
					str += m_CallStatus.strCity;
					aryArg.Add(str);
			} else if (cs.CompareNoCase(_T("$PartnerPhoneHome")) == 0) {	// �ʘb����d�b�i����j
					aryArg.Add(m_CallStatus.strPhoneHome);
			} else if (cs.CompareNoCase(_T("$PartnerPhoneOffice")) == 0) {	// �ʘb����d�b�i�I�t�B�X�j
					aryArg.Add(m_CallStatus.strPhoneOffice);
			} else if (cs.CompareNoCase(_T("$PartnerPhoneMobile")) == 0) {	// �ʘb����d�b�i���o�C���j
					aryArg.Add(m_CallStatus.strPhoneMobile);
			} else {
				if (cs.Find(_T("(")) >= 0 && cs.Find(_T(")")) >= 0) {
					// �����\���̂܂܂̏ꍇ
					aryArg.Add(_T(""));
				} else {
					aryArg.Add(cs);
				}
			}
		}
		i++;
	}

	if (blExe) {
		// ���s
		fs.SetFullSpec(strExeFile);
		strExeFile = _T("\"") + strExeFile + _T("\"");
		CString strPrm = _T("");
		for (i = 0; i < aryArg.GetCount(); i++) {
			if (i > 0) strPrm = strPrm + _T(" ");
			strPrm = strPrm + _T("\"") + aryArg.GetAt(i) + _T("\"");
		}
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, strExeFile + _T(" ") + strPrm);
		HINSTANCE ret;
		SetCurrentDirectory(fs.GetFolder());
		if (strExeFile.Right(5).CompareNoCase(_T(".bat\"")) == 0) {
			// bat�̎��s
			ret = ShellExecute(NULL, _T("open"), strExeFile,  strPrm, fs.GetFolder(), SW_HIDE);
		} else {
			// exe�̎��s
			ret = ShellExecute(NULL, NULL, strExeFile,  strPrm, fs.GetFolder(), SW_SHOWNORMAL);
		}
		if ((int)ret <= 32) {
			str.LoadString(IDS_MSGBOX107);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
			return FALSE;
		}
	}

	return TRUE;
}

//
//  �@�\     : Skype�f�o�C�X�̐ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : Ver1.0�ȍ~�̓f�o�C�X�ύX�͕s�v
//  
//BOOL CAccessEvents::SetSkypeDeviceTry(
//CString strInDev,	// ���̓f�o�C�X
//CString strOutDev)	// �o�̓f�o�C�X
//{
//	long i;
//	CString str;
//	_bstr_t bStr;
//	CString strTmpIn;
//	CString strTmpOut;
//	// GetAudioIn(), GetAudioOut()�͌��ʂ̃R�s�[��n���Ȃ����߁A��p�̕ϐ��Ŏ擾����
//	_bstr_t bStrInDev;
//	_bstr_t bStrOutDev;
//
//	try {
//		/// Skype���o��WAVE�f�o�C�X�̊m�F
//		// Windows�W���̏ꍇ�͋󕶎��񂪕Ԃ�
//		bStrInDev = m_pAccess->GetAudioIn();
//		strTmpIn.Format(_T("%s"), (LPCTSTR)bStrInDev);
//		bStrOutDev = m_pAccess->GetAudioOut();
//		strTmpOut.Format(_T("%s"), (LPCTSTR)bStrOutDev);
//
//		/// �����̃f�o�C�X���ƈقȂ�ΕύX
//		if (strTmpIn.CompareNoCase(strInDev) != 0) {
//			bStr = _bstr_t((LPCTSTR)strInDev);
//			//// �ݒ肪���f����Ȃ��ꍇ���g���C����
//			for(i = 0; i < 10; i++) {
//				m_pAccess->PutAudioIn(bStr);
//				bStrInDev = m_pAccess->GetAudioIn();
//				strTmpIn.Format(_T("%s"), (LPCTSTR)bStrInDev);
//				if (strTmpIn.CompareNoCase(strInDev) == 0) break;
//			}
//		}
//		if (strTmpOut.CompareNoCase(strOutDev) != 0) {
//			bStr = _bstr_t((LPCTSTR)strOutDev);
//			//// �ݒ肪���f����Ȃ��ꍇ���g���C����
//			for(i = 0; i < 10; i++) {
//				m_pAccess->PutAudioOut(bStr);
//				bStrOutDev = m_pAccess->GetAudioOut();
//				strTmpOut.Format(_T("%s"), (LPCTSTR)bStrOutDev);
//				if (strTmpOut.CompareNoCase(strOutDev) == 0) break;
//			}
//		}
//	} catch(_com_error& e) {
//		if (e.ErrorInfo()) {
//			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
//		} else {
//			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
//		}
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
//		return FALSE;   
//	} catch(...) {
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't change Skype devices."));
//		return FALSE;
//	}
//
//	return TRUE;
//}

//
//  �@�\     : Skype�f�o�C�X�̐ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : Ver1.0�ȍ~�̓f�o�C�X�ύX�͕s�v
//  
//BOOL CAccessEvents::SetSkypeDevice(
//CString strInDev,	// ���̓f�o�C�X
//CString strOutDev,	// �o�̓f�o�C�X
//BOOL blSilence)		// �G���[���b�Z�[�W�}�����[�h
//{
//	if (theApp.m_lngAppStatus == 0) return TRUE;
//
//	if (SetSkypeDeviceTry(strInDev, strOutDev)) return TRUE;
//
//	if (!blSilence) {
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't change Skype devices."));
//	}
//	return FALSE;
//}

//
//  �@�\     : Skype�r�f�I���L���ȃo�[�W�������H
//  
//  �@�\���� : �L���ȃo�[�W�����̏ꍇ�̂�TRUE
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : "GET SKYPEVERSION"���\�b�h�͗�O���N�����₷���̂ŁA�R�[���͍ŏ����ɂ���
//  
BOOL CAccessEvents::IsSkypeVideoVersion()
{
	CString str;
	_bstr_t bStr;
	CString strRet;
	_bstr_t bStrRet;
	CString strCmp;

	try {
	/* Skype2.5����Win2000�ł�VIDEO���L���ȏꍇ������
		//// Windows VERSION �`�F�b�N
		if (!WinVersion.IsXPorLater()) return FALSE;
	*/
		//// SKYPE VERSION �`�F�b�N
		//bStr = _bstr_t(_T("GET SKYPEVERSION"));
		//bStrRet = m_pAccess->SendBlockingCommand(bStr);
		//strRet.Format(_T("%s"), (LPCTSTR)bStrRet);
		//strCmp = _T("SKYPEVERSION ");
		//str = strRet.Mid(strCmp.GetLength(), 1);
		bStrRet = m_pAccess->GetVersion();
		strRet.Format(_T("%s"), (LPCTSTR)bStrRet);
		str = strRet.Left(1);
		int intVer = _ttoi((LPCTSTR)str);
		if (intVer < 2) {
			return FALSE;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("IsSkypeVideoVersion error:%s"), strRet);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

//
//  �@�\     : Skype�r�f�I���g�p�\���H
//  
//  �@�\���� : ����M�̗��҂��g�p�\�ȏꍇ�̂�TRUE
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAccessEvents::IsSkypeVideo(
CString strUsr)		// �ʘb�惆�[�U�iEmpty�̏ꍇ�A�����̂݃`�F�b�N�j
{
	//CString str;
	//_bstr_t bStr;
	//CString strRet;
	//_bstr_t bStrRet;
	//CString strCmp;

	if (!blSkypeVideoVersion) return FALSE;

// �ȉ��̃`�F�b�N�͎����ɂ��Ă͓��삵�Ȃ�
	//str.Format(_T("GET USER %s IS_VIDEO_CAPABLE"), theApp.m_strUsrId);
	//bStr = _bstr_t((LPCTSTR)str);
	//bStrRet = m_pAccess->SendBlockingCommand(bStr);
	//strRet.Format(_T("%s"), (LPCTSTR)bStrRet);
	//str = strRet.Right(4);
	//str.MakeUpper();
	//if (str != _T("TRUE")) {
	//	return FALSE;
	//}

// �R���^�N�g���X�g�ɂȂ�����̓`�F�b�N�ł��Ȃ�
	/// ����̊��`�F�b�N
	//if (!strUsr.IsEmpty()) {
	//	str.Format(_T("GET USER %s IS_VIDEO_CAPABLE"), strUsr);
	//	bStr = _bstr_t((LPCTSTR)str);
	//	bStrRet = m_pAccess->SendBlockingCommand(bStr);
	//	strRet.Format(_T("%s"), (LPCTSTR)bStrRet);
	//	str = strRet.Right(4);
	//	str.MakeUpper();
	//	if (str != _T("TRUE")) {
	//		return FALSE;
	//	}
	//}

	return TRUE;
}

//
//  �@�\     : �r�f�I�\�������t���b�V��
//  
//  �@�\���� : �r�f�I�ʘb���̏ꍇ�A�ύX�𔽉f�����邽�߂�ON/OFF���K�v
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAccessEvents::RefreshSkypeVideo(
long lngCall,		// CALL ID
BOOL blStop)		// �r�f�I���M���~
{
	CString str;
	_bstr_t bStr;
	CString strRet;
	_bstr_t bStrRet;

	try {
		if (m_pCall == NULL) return FALSE;

		/// �r�f�I���M���\���H
		bStrRet = m_pCall->GetVideoStatus();
		strRet.Format(_T("%s"), (LPCTSTR)bStrRet);
		strRet.MakeUpper();
		if (strRet.Find(_T("VIDEO_SEND_ENABLED")) < 0 && strRet.Find(_T("VIDEO_BOTH_ENABLED")) < 0) {
			return FALSE;
		}	

		/// ��U�r�f�I���M���~
		m_pCall->StopVideoSend();

		/// �r�f�I���M���J�n
		if (!blStop) {
			m_pCall->StartVideoSend();
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("RefreshSkypeVideo error:%s"), str);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

//
//  �@�\     : Skype�r�f�I�f�o�C�X�̎擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : CString
//  
//  ���l     : 
//
//CString CAccessEvents::GetSkypeVDevice()
//{
//	CString str;
//	_bstr_t bStr;
//	CString strTmpIn;
//	_bstr_t bStrInDev;
//
//	try {
//		// Windows�W���̏ꍇ�͋󕶎��񂪕Ԃ�
//		bStr = _bstr_t(_T("GET VIDEO_IN"));
//		bStrInDev = m_pAccess->SendBlockingCommand(bStr);
//		if (bStrInDev.length() > 9) {		// more "VIDEO_IN "
//			strTmpIn.Format(_T("%s"), (LPCTSTR)bStrInDev);
//			strTmpIn.Delete(0, 9);	// cut "VIDEO_IN "
//		} else {
//			strTmpIn.Empty();
//		}
//	} catch(...) {
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't change Skype video devices."));
//		return _T("");
//	}
//
//	return strTmpIn;
//}

//
//  �@�\     : Skype�r�f�I�f�o�C�X�̐ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAccessEvents::SetSkypeVDeviceTry(
CString strDev)		// �f�o�C�X
{
	long i;
	CString str;
	_bstr_t bStr;
	CString strTmpIn;
	_bstr_t bStrInDev;

	try {
//		::Sleep(500);	// WAIT���K�v?

		/// Skype����VIDEO�f�o�C�X�̊m�F
		// Windows�W���̏ꍇ�͋󕶎��񂪕Ԃ�
		bStrInDev = m_pAccess->GetVideoIn();
		strTmpIn.Format(_T("%s"), (LPCTSTR)bStrInDev);

		/// �����̃f�o�C�X���ƈقȂ�ΕύX
		if (strTmpIn.CompareNoCase(strDev) != 0) {
			bStr = _bstr_t((LPCTSTR)strDev);
			//// �ݒ肪���f����Ȃ��ꍇ���g���C����
			for(i = 0; i < 10; i++) {
				m_pAccess->PutVideoIn(bStr);
				bStrInDev = m_pAccess->GetVideoIn();
				strTmpIn.Format(_T("%s"), (LPCTSTR)bStrInDev);
				if (strTmpIn.CompareNoCase(strDev) == 0) break;
			}
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("SetSkypeVDeviceTry error:%s"), str);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

//
//  �@�\     : Skype�r�f�I�f�o�C�X�̐ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAccessEvents::SetSkypeVDevice(
CString strDev,		// ���̓f�o�C�X
BOOL blSilence)		// �G���[���b�Z�[�W�}�����[�h
{
	BOOL blRet = FALSE;

	/// ���g���C�t���Ńf�o�C�X�ύX
//	int i;
//	for(i = 0; i < 10; i++) {
		if (SetSkypeVDeviceTry(strDev)) {
			blRet = TRUE;
//			break;
		}
//	}

	if (!blSilence && !blRet) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't change Skype video devices."));
		return blRet;
	}

	return blRet;
}

//
//  �@�\     : Tapur�f�o�C�X�̐ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAccessEvents::SetAppDevice(
CString strInDev,	// ���̓f�o�C�X
CString strOutDev)	// �o�̓f�o�C�X
{
	m_pAudio->SetInDevice(strInDev);
	m_pAudio->SetOutDevice(strOutDev);

	return TRUE;
}

//
//  �@�\     : Skype�f�o�C�X�̏�����
//  
//  �@�\���� : �����f�o�C�X�̃o�b�N�A�b�v
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAccessEvents::InitDevice(void)
{
	CString str;
	_bstr_t bStr;

	try {
		// 13/07/14 �I�t���C������ConnectionStatus���擾�ł��Ȃ����ߔp�~�iAPIStatusChanged:0�Őڑ��m�F�Ƃ���j
		/// Skype�Ƃ̐ڑ���ԃ`�F�b�N
		//int intStatus = m_pAccess->GetConnectionStatus();
		//if (!(intStatus == conConnecting || intStatus == conOnline)) {
		//	str.LoadString(IDS_MSGBOX7);
		//	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		//	AfxThrowUserException();
		//}

		/// PROTOCOL �w��
		m_pAccess->Protocol = 6;	// skype3.0�`
//		m_pAccess->Protocol = 5;	// skype1.4�`

		/// ���ۂɎg�p�\��PROTOCOL�̎擾
		m_lngProtocol = m_pAccess->Protocol;
		bStr = m_pAccess->Version;
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Skype Ver:%s Protocol:%d"), (LPCTSTR)bStr, m_lngProtocol);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

		//// ���[�U���i�X�J�C�v���j�̎擾
		IUserPtr pUser;
		bStr = m_pAccess->CurrentUserHandle;
		if ( bStr.length() == 0 ) {
			str.LoadString(IDS_MSGBOX8);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			AfxThrowUserException( );
		}
		theApp.m_strUsrId.Format(_T("%s"), (LPCTSTR)bStr);
		pUser = m_pAccess->GetUser(bStr);
		bStr = pUser->FullName;
		if ( bStr.length() > 0 ) {
			theApp.m_strUsrName.Format(_T("%s"), (LPCTSTR)bStr);
		}

		/// Skype���o��WAVE�f�o�C�X�̓���
		// Windows�W���̏ꍇ�͋󕶎��񂪕Ԃ�
		bStr = m_pAccess->GetAudioIn();
		m_defInDevice.Format(_T("%s"), (LPCTSTR)bStr);
		bStr = m_pAccess->GetAudioOut();
		m_defOutDevice.Format(_T("%s"), (LPCTSTR)bStr);

		/// Skype���o�̓f�o�C�X��"Tapur Virtual Cable"�̏ꍇ�A�f�t�H���g�ɕύX
		//if (m_defInDevice.Left(5).CompareNoCase(_T("Tapur")) == 0) {
		//	bStr = _bstr_t("");
		//	m_pAccess->PutAudioIn(bStr);
		//	m_defInDevice.Empty();
		//}
		//if (m_defOutDevice.Left(5).CompareNoCase(_T("Tapur")) == 0) {
		//	bStr = _bstr_t("");
		//	m_pAccess->PutAudioOut(bStr);
		//	m_defOutDevice.Empty();
		//}

		// �A�v���̃f�o�C�X��ݒ�
		m_pAudio->SetInDevice(m_defInDevice);
		m_pAudio->SetOutDevice(m_defOutDevice);

		/// �T�E���h�f�o�C�X�̃t�H�[�}�b�g����
		m_pAudio->FindFormat(FALSE);

		/// �r�f�I���L���ȏꍇ
		if (IsSkypeVideoVersion()) {
			blSkypeVideoVersion = TRUE;

			/// Skype�r�f�I�f�o�C�X�̓���
			// Windows�W���̏ꍇ�͋󕶎��񂪕Ԃ�
			bStr = m_pAccess->GetVideoIn();
			m_defVDevice.Format(_T("%s"), (LPCTSTR)bStr);
			//m_defVDevice = GetSkypeVDevice();

			/// Skype�r�f�I�f�o�C�X��"Tapur Virtual Camera"�̏ꍇ�A�f�t�H���g�ɕύX
			if (m_defVDevice.GetLength() < 5) return TRUE;
			if (m_defVDevice.Left(5).CompareNoCase(_T("Tapur")) == 0) {
				bStr = _bstr_t("");
				m_pAccess->PutVideoIn(bStr);
				m_defVDevice.Empty();
			} else {
// �p�~�iVer1.05�j:�N�����̓f�o�C�X�ύX���G���[�ɂȂ�₷������
				///// SKYPE�r�f�I�f�o�C�X�ύX�e�X�g
				//// �C���X�g�[�������Skype���f�o�C�X��F�����Ȃ��ꍇ�����邽��
				//if (!SetSkypeVDevice(TVC_IN_DEV, TRUE)) {
				//	CString strErr;
				//	strErr.LoadString(IDS_MSGBOX7);
				//	AfxMessageBox(strErr, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
				//	AfxThrowUserException();
				//}
				//SetSkypeVDevice(m_defVDevice);
			}
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
//		return FALSE;   
		return TRUE;	// Skype�ڑ���OK�Ȃ�AAPI�֘A�G���[�͌������iVer1.2�j 
    } catch( CUserException* e ) {
		e->Delete();
        return FALSE;   
	} catch( CException* e ) {
		CString strErr;
		_TCHAR   szCause[MAX_LEN];
 		e->GetErrorMessage(szCause, MAX_LEN);
		strErr.Format(_T("Init Skype device error:%s\n"), szCause);
		AfxMessageBox(strErr);
		e->Delete();
	}

	return TRUE;
}


//
//  �@�\     : ���[�U�P�ʂ̃f�[�^�x�[�X��������
//  
//  �@�\���� : �t�H���_�����A�����f�[�^�R�s�[
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAccessEvents::InitUserData(void)
{
	int ret = 0;
	_bstr_t bStr;
	CFileSpec fs;
	CFileSpec fstmp;
	_TCHAR strFolder[MAX_PATH];	
	CString strPath;	
	CString str;

	try {
	/// ���݂̃��[�U�t�H���_�ƃf�[�^�̌��o
		//// Application Data�t�H���_�̎擾�iex, C:\Documents and Settings\paak\Application Data)
		ret = ::SHGetSpecialFolderPath(NULL, strFolder, CSIDL_APPDATA, FALSE);
		if ( !ret ) {
			throw _T("can't find data folder.");
		}
		theApp.m_strDataFolder = strFolder;
		theApp.m_strDataFolder += _T(TAPUR_DATA_PATH) + ChkFolder(theApp.m_strUsrId);
		//theApp.m_strDataFolder += _T(TAPUR_DATA_PATH) + theApp.m_strUsrId;
		theApp.m_strDBPath = theApp.m_strDataFolder + _T("\\") + _T(DBFILE_NAME);	// DB�t�@�C�����̓���
		m_Setup.SetDataDir(theApp.m_strDataFolder);	// �f�[�^�t�H���_

	/// DB�t�@�C���̑��݃`�F�b�N�����݂��Ȃ��ꍇ�V�K�쐬����
		fs.SetFullSpec(theApp.m_strDBPath);
		if (!fs.Exist()) {
			//// ���[�U�������݂��Ȃ��ꍇ�A�V�K�Ƀf�[�^�x�[�X���쐬�i�R�s�[�j����
			///// ���[�U�t�H���_�̍쐬
			if (!fs.MkFolder()) {
				throw _T("can't create user data folder.");
			}
			///// DB�t�@�C���̃R�s�[
			fs.SetFullSpec(FS_APPDIR);
			fs.SetFileName(_T("init.db"));
			if (!fs.FileCopy(theApp.m_strDBPath, FALSE)) {
				throw _T("can't create user data.");
			}
		}

	/// ���������f�[�^�̑��݃`�F�b�N�����݂��Ȃ��ꍇ�쐬�i�R�s�[�j����
		CTPsetup tps(theApp.m_strDataFolder, _T("0"));
		//// �������������t�@�C���쐬
		strPath = tps.RsfAudioFileName(_T("$"));
		fs.SetFullSpec(strPath);
		if (!fs.Exist()) {
			///// �������������t�@�C���̃R�s�[
			fs.SetFullSpec(FS_APPDIR);
			fs.SetFileName(_T("greeting.wav"));
			if (!fs.FileCopy(strPath, TRUE)) {
				throw _T("can't create initial response audio file.");
			}
		}
		//// ����BEEP�����t�@�C���̃R�s�[
		strPath = tps.RsfBeepFileName();
		fs.SetFullSpec(strPath);
		if (!fs.Exist()) {
			fs.SetFullSpec(FS_APPDIR);
			fs.SetFileName(_T("beep.wav"));
			if (!fs.FileCopy(strPath, TRUE)) {
				throw _T("can't create initial beep audio file.");
			}
		}
		strPath = tps.RsfBeepFileName(_T("1"));
		fstmp.SetFullSpec(strPath);
		if (!fstmp.Exist()) {
			if (!fs.FileCopy(strPath, TRUE)) {
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, 
					_T("can't create initial beep audio file1."));
			}
		}
		strPath = tps.RsfBeepFileName(_T("2"));
		fstmp.SetFullSpec(strPath);
		if (!fstmp.Exist()) {
			if (!fs.FileCopy(strPath, TRUE)) {
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, 
					_T("can't create initial beep audio file2."));
			}
		}
		//// ���������t�@�C���쐬
		strPath = tps.RsfFileName();
		fs.SetFullSpec(strPath);
		if (!fs.Exist()) {
			if (!tps.SaveRsfFile()){
				throw _T("can't create initial response file.");
			}
		}
		//// ���������ݒ�
		CString strWhere;
		CTblRsfl tbl(theApp.m_strDBPath);
		strWhere.Empty();
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		strWhere = strWhere + _T(" and ") + _T("lngDefaultFlag=1");
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
		if (tbl.CountRec(strWhere) == 0) {
			//// ���͒l
			tbl.strReceiverId = theApp.m_strUsrId;	// ��M�Җ��i�X�J�C�v���j
			tbl.strReceiverName = theApp.m_strUsrName;	// ��M�Җ��i�\�����j
			tbl.lngRespType = RESPBASE_TYPE_SIMPLE;
			tbl.lngDefaultFlag = 1;
			//// insert
			tbl.InsertRec();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, err);
		return FALSE;
	}
		
	return TRUE;
}

//
//  �@�\     : SKYPE�ɐڑ��ł��Ȃ����̕\�����[�U�̓���
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAccessEvents::InitDummyUser(void)
{
	_TCHAR tstrFolder[MAX_PATH];	
	CString str, strtmp;

	/// Application Data�t�H���_���̍ŐV�̃��[�U���g�p
	int ret = ::SHGetSpecialFolderPath(NULL, tstrFolder, CSIDL_APPDATA, FALSE);
	if ( !ret ) return FALSE;
	CString strFolder = tstrFolder;
	strFolder += _T(TAPUR_DATA_PATH);
	//// �ł����t�̐V�������ʃt�H���_�̌���
	if (!SetCurrentDirectory(strFolder)) return FALSE;
	CFileFind finder;
	CTime tm, btm(2000, 1, 1, 0, 0, 0);
	BOOL bWorking = finder.FindFile(_T("*"));
	if (!bWorking) return FALSE;
	str.Empty();
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory()) {
			strtmp = finder.GetFileName();
			if (strtmp.Left(1) == _T(".")) continue;
			finder.GetLastWriteTime(tm);
			if (tm > btm) {
				str = strtmp;
				btm = tm;
			}
		}
	}
	if (str.IsEmpty()) return FALSE;
	theApp.m_strUsrId = str;
	theApp.m_strUsrName.Empty();

	theApp.m_strDataFolder = strFolder;
	theApp.m_strDataFolder += ChkFolder(theApp.m_strUsrId);
	//theApp.m_strDataFolder += theApp.m_strUsrId;
	theApp.m_strDBPath = theApp.m_strDataFolder + _T("\\") + _T(DBFILE_NAME);	// DB�t�@�C�����̓���
	m_Setup.SetDataDir(theApp.m_strDataFolder);	// �f�[�^�t�H���_
		
	return TRUE;
}

//
//  �@�\     : TAB���̃r�f�I��ʂ��E�B���h�E�\���ɂ���
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 1:TAB->�E�B���h�E�\�������A2:���ɃE�B���h�E�\��
//           : 0:TAB��ʁi�r�f�I��ʁj�Ȃ��A-1:TAB->�E�B���h�E�\�����s
//  
//  ���l     : 
//
int CAccessEvents::ShowVideoWin(CString strSkypeId, BOOL blLarge)
{
	RECT rct;
	LPRECT pRect = &rct;
	CPoint point;

	/// ���݃E�B���h�E�\�����m�F
	if (m_pVideo->GetVideoWinLarge(pRect)) return 2;

	/// SkypeWindow��\������
	_bstr_t bStr = _bstr_t(_T("FOCUS"));
	m_pAccess->SendCommand(bStr);

	/// �ʘbTAB��\������
	if (!FocusCallTab(strSkypeId)) return 0;

	if (!blLarge) return 0;

	/// TAB���̃r�f�I��ʈʒu���擾
	if (!m_pVideo->GetVideoWinLocal(strSkypeId, pRect)) return 0;

	/// �O�ʕ\��
	HWND hwnd = ::FindWindow(_T("tSkMainForm.UnicodeClass"), NULL); if (hwnd == NULL) return 0;
	::SetForegroundWindow(hwnd);
	Sleep(100);
	/// �[���I�Ƀ}�E�X���������ăE�B���h�E�\���ɂ���
//	point.x = pRect->right - 40;
//	point.y = pRect->top + 24;
	point.x = pRect->right - 110;
	point.y = pRect->top + 30;
	::SetCursorPos(point.x, point.y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	/// �E�B���h�E�T�C�Y�i�^��T�C�Y�j���K��
	// �E�B���h�E���傫������ƃm�C�Y������₷��
	Sleep(400);		// �E�B���h�E�̕\���҂�
	pRect->top = VECHO_POSY;	// �G���[���b�Z�[�W�i��ʒ����j�̕\���ʒu�������
	pRect->left = VECHO_POSX;
	pRect->right = pRect->left + VECHO_SZX;
	pRect->bottom = pRect->top + VECHO_SZY;
	if (!m_pVideo->SetVideoWinLarge(pRect)) {
		// �}�E�X�C�x���g�����������ꍇ������̂ŁA���g���C
		::SetCursorPos(point.x, point.y);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		Sleep(400);		// �E�B���h�E�̕\���҂�
		if (!m_pVideo->SetVideoWinLarge(pRect)) return -1;
	}

	return 1;
}

//
//  �@�\     : �ʘbTAB��\������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAccessEvents::FocusCallTab(
CString strSkypeId)		// ���[�U����TAB�̌��������t���Ȃ����߁A���݂͖���
{
	RECT rct;
	LPRECT pRect = &rct;
	CPoint point;
	HWND hwnd;
	HWND hVwnd;

	/// Call Tab
	hVwnd = FindTab(_T("tMultiCallPanel"));
	if (hVwnd == NULL) return FALSE;
	if (::IsWindowVisible(hVwnd)) return TRUE;	// ���ɕ\������Ă���

	/// ���݂̕\��TAB
	int intNext = 0;
	hwnd = FindTab(_T("TMainUserList")); if (hwnd == NULL) return FALSE;
	if (::IsWindowVisible(hwnd)) intNext = 2;
	hwnd = FindTab(_T("THistoryList")); if (hwnd == NULL) return FALSE;
	if (::IsWindowVisible(hwnd)) intNext = 1;

	/// �O�ʕ\��
	hwnd = ::FindWindow(_T("tSkMainForm.UnicodeClass"), NULL); if (hwnd == NULL) return FALSE;
	::SetForegroundWindow(hwnd);
	Sleep(100);
	//// Ctrl+Tab���񐔕����s���ACall Tab��\������
	// ���M���͓��삵�Ȃ��炵���E�E�E
	for (int i = 0 ; i < intNext; i++) {
		KeyCtrlTab();
	}

	// �Ċm�F
	if (!::IsWindowVisible(hVwnd)) return FALSE;

	return TRUE;
}

//
//  �@�\     : Skype TAB��Window�n���h���擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : HWND
//  
//  ���l     : 
//  
HWND CAccessEvents::FindTab(CString strTab)
{
	HWND hwnd = ::FindWindow(_T("tSkMainForm.UnicodeClass"), NULL); if (hwnd == NULL) return NULL;
	hwnd = ::FindWindowEx(hwnd, 0, _T("TPanel"), NULL); if (hwnd == NULL) return NULL;
	hwnd = ::FindWindowEx(hwnd, 0, _T("TPanel"), NULL); if (hwnd == NULL) return NULL;
	hwnd = ::FindWindowEx(hwnd, 0, _T("TSkypeTabControl"), NULL); if (hwnd == NULL) return NULL;
	// �^�u�̌���
	hwnd = ::FindWindowEx(hwnd, 0, strTab, NULL); if (hwnd == NULL) return NULL;

	return hwnd;
}

//
//  �@�\     : Ctrl+Tab�L�[�𔭍s����Skype��TAB��؂�ւ���
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
void CAccessEvents::KeyCtrlTab()
{
/*
	INPUT inputKey[4];

	ZeroMemory(inputKey, sizeof(INPUT)*4);
	inputKey[0].type = INPUT_KEYBOARD;
	inputKey[0].ki.wVk = VK_CONTROL;
	inputKey[0].ki.wScan = MapVirtualKey(VK_CONTROL, 0);
	inputKey[1].type = INPUT_KEYBOARD;
	inputKey[1].ki.wVk = VK_TAB;
	inputKey[1].ki.wScan = MapVirtualKey(VK_TAB, 0);
	inputKey[2] = inputKey[0];
	inputKey[2].ki.dwFlags = KEYEVENTF_KEYUP;
	inputKey[3] = inputKey[1];
	inputKey[3].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(4, inputKey, sizeof(INPUT));
*/

	keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), 0, 0);
	keybd_event(VK_TAB, MapVirtualKey(VK_TAB, 0), 0, 0);
	keybd_event(VK_TAB, MapVirtualKey(VK_TAB, 0), KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), KEYEVENTF_KEYUP, 0);
}

//
//  �@�\     : ���[�U�P�ʂ̃f�[�^�x�[�X�̃o�[�W�����A�b�v
//  
//  �@�\���� : ���o�[�W�����̃f�[�^���C��
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : Ver0.90�` : ����WAVE�t�@�C���̃t�H�[�}�b�g�C���i16Khz, 16bit, 1channel�j
//           : Ver0.92�` : ����WAVE�t�@�C���̃t�H�[�}�b�g�C���i48Khz, 16bit, 1channel�j
//           : Ver1.00�` : ����WAVE�t�@�C���̃t�H�[�}�b�g�C���i16Khz, 16bit, 1channel�j
//  
BOOL CAccessEvents::VerUpUserData(void)
{
	long i;
	CString str;
	CString strWhere;
	CString strPath;	
	CString strPathTmp;	
	CFileSpec fs;
	BOOL blFlag = FALSE;
	AUDIO_FORMAT fmt;
	fmt.nBits = 16;
	fmt.nChannels = 1;
	fmt.nFrequency = 16000;

	try {
		CTPsetup tps(theApp.m_strDataFolder);
		CTblRsfl tbl(theApp.m_strDBPath);
		strWhere.Empty();
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		long lngCnt = tbl.SelectRec(strWhere);
		if (lngCnt == 0) return TRUE;

		for (i = 0; i < lngCnt; i++) {
			/// Ver.92-0.99�p����
			if (tbl.strSystemVersion == _T(POTM_VERSION_OLD2) || tbl.strSystemVersion == _T(POTM_VERSION_OLD3) || 
			    tbl.strSystemVersion == _T(POTM_VERSION_OLD4) || tbl.strSystemVersion == _T(POTM_VERSION_OLD5)) {
				if (!blFlag) {
					/// BEEP�t�@�C����16KHz�t�H�[�}�b�g�֕ϊ��i�P�񂾂����s�j
					blFlag = TRUE;
					strPath = tps.RsfBeepFileName();
					fs.SetFullSpec(strPath);
					if (fs.Exist()) {
						strPathTmp = strPath + _T("_tmp");
						//// �ꎞ�t�@�C���Ƃ��Đ���
						if (!m_pAudio->ChgWaveFormat(strPath, strPathTmp, fmt, 0)) {
							str.LoadString(IDS_MSGBOX901);
							AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
						} else {
							//// �ꎞ�t�@�C���������t�@�C���ɃR�s�[
							fs.SetFullSpec(strPathTmp);
							if (fs.Exist()) {
								fs.FileMove(strPath, TRUE);
							}
						}
					}
					strPath = tps.RsfBeepFileName(_T("1"));
					fs.SetFullSpec(strPath);
					if (fs.Exist()) {
						strPathTmp = strPath + _T("_tmp");
						//// �ꎞ�t�@�C���Ƃ��Đ���
						if (!m_pAudio->ChgWaveFormat(strPath, strPathTmp, fmt, 0)) {
							str.LoadString(IDS_MSGBOX901);
							AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
						} else {
							//// �ꎞ�t�@�C���������t�@�C���ɃR�s�[
							fs.SetFullSpec(strPathTmp);
							if (fs.Exist()) {
								fs.FileMove(strPath, TRUE);
							}
						}
					}
					strPath = tps.RsfBeepFileName(_T("2"));
					fs.SetFullSpec(strPath);
					if (fs.Exist()) {
						strPathTmp = strPath + _T("_tmp");
						//// �ꎞ�t�@�C���Ƃ��Đ���
						if (!m_pAudio->ChgWaveFormat(strPath, strPathTmp, fmt, 0)) {
							str.LoadString(IDS_MSGBOX901);
							AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
						} else {
							//// �ꎞ�t�@�C���������t�@�C���ɃR�s�[
							fs.SetFullSpec(strPathTmp);
							if (fs.Exist()) {
								fs.FileMove(strPath, TRUE);
							}
						}
					}
				}
				/// ���������t�@�C��
				if (tbl.lngDefaultFlag == 1) {
					tps.SetCallerId(_T("0"));
				} else {
					tps.SetCallerId(tbl.strSenderId);
				}
				strPath = tps.RsfAudioFileName(_T("$"));
				/// ���������t�@�C����16KHz�t�H�[�}�b�g�֕ϊ�
				fs.SetFullSpec(strPath);
				if (!fs.Exist()) goto NEXT_LOOP;
				strPathTmp = strPath + _T("_tmp");
				//// �ꎞ�t�@�C���Ƃ��Đ���
				if (!m_pAudio->ChgWaveFormat(strPath, strPathTmp, fmt, 0)) {
					str.LoadString(IDS_MSGBOX900);
					AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
					goto NEXT_LOOP;
				}
				//// �ꎞ�t�@�C���������t�@�C���ɃR�s�[
				fs.SetFullSpec(strPathTmp);
				if (!fs.Exist()) goto NEXT_LOOP;
				fs.FileMove(strPath, TRUE);
			}
NEXT_LOOP:
			if (!tbl.NextRec()) break;
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("version up data error"));
		return FALSE;
	}
		
	return TRUE;
}
