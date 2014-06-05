// RsfVideoDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfVideoDialog.h"
#include "RsfTree.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"
#include ".\rsfvideodialog.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CRsfVideoDialog �_�C�A���O


CRsfVideoDialog::CRsfVideoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRsfVideoDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRsfVideoDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	/// �^�C�}�[������
	m_stPlayChk.blOnTimer = FALSE;
	m_stPlayChk.lngPassTime = 0;
	m_stPlayChk.lngLimitTime = 0;

/*
	// �r�f�I����C���X�^���X�̐���
	m_pVideo = CVideo::Instance();
*/
}

CRsfVideoDialog::~CRsfVideoDialog()
{
}

void CRsfVideoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfVideoDialog)
	DDX_Control(pDX, IDC_STATIC_TIME, m_Time);
	DDX_Control(pDX, IDC_PROGRESS, m_Prog);
	DDX_Control(pDX, IDC_BTN_FILE, m_File);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRsfVideoDialog, CDialog)
	//{{AFX_MSG_MAP(CRsfVideoDialog)
	ON_BN_CLICKED(IDC_PLAY, OnBtnPlay)
	ON_BN_CLICKED(IDC_PAUSE, OnBtnPause)
	ON_BN_CLICKED(IDC_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_FILE, OnBtnFile)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfVideoDialog ���b�Z�[�W �n���h��

int CRsfVideoDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO :  �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B

	return 0;
}

// �i���Ӂj���̃��\�b�h���ŃR���g���[���̃t�H�[�J�X�ݒ�͍s��Ȃ�
// ���Ăяo���t�H�[���Ƀt�H�[�J�X���߂�Ȃ��Ȃ邽��
void CRsfVideoDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	
	//// ���t���ԊǗ��p�^�C�}�[�I��
	StopPlayChkTimer();

	if (_tcsclen(m_strPlayFile) > 0) {
		/// �Đ��I��
		if (m_pVideo->IsPlaying()) {
			m_pVideo->PlayStop();
		}
	}
}

BOOL CRsfVideoDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	// �r�f�I����C���X�^���X�̐���
	m_pVideo = CVideo::Instance();

	/// �{�^��ID��CBitmapButton�I�u�W�F�N�g�Ɋ֘A�t����
	m_Play.AutoLoad(IDC_PLAY, this);
	m_Pause.AutoLoad(IDC_PAUSE, this);
	m_Stop.AutoLoad(IDC_STOP, this);
	//// �{�^���̃L���v�V�����Ɋ֘A�����r�b�g�}�b�v�����[�h����Ȃ��̂ŁA���\�[�XID�ōĎw�肷��B
	m_Play.LoadBitmaps(PLAYU, PLAYD, PLAYF, PLAYX);
	m_Pause.LoadBitmaps(PAUSEU, PAUSED, PAUSEF, PAUSEX);
	m_Stop.LoadBitmaps(STOPU, STOPD, STOPF, STOPX);
	/// ���s���Ƀ{�^�����ʒu���킹����(Play�{�^���ɍ��킹�Ĕz�u�����)
	//// Play�{�^���̍��W���_�C�A���O�{�b�N�X����̑��Έʒu�Ƃ��Ď擾����
	CRect rect;
	m_Play.GetWindowRect(rect);
	ScreenToClient(rect);
	int nTopPos = rect.top;
	int nWidth = rect.right - rect.left;    
	//// Pause�{�^���̈ʒu��ݒ肷��
	m_Pause.SetWindowPos(NULL, rect.right, nTopPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//// Stop�{�^���̈ʒu��ݒ肷��
	m_Stop.SetWindowPos(NULL, rect.right + nWidth, nTopPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	// ��ʍX�V
	UpdateForm();

	// ���t���ԊǗ��p�^�C�}�[�J�n
//	SetTimer(ID_TIMER_RSF_VIDEO, 1000, NULL);	// 1 sec each

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CRsfVideoDialog::OnBtnPlay()
{
	Play();
}

void CRsfVideoDialog::OnBtnPause()
{
	if (_tcsclen(m_strPlayFile) <= 0) return;

	/// �Đ��ꎞ��~
	if (!m_pVideo->IsPlaying()) return;
	m_pVideo->PlayPause();

	/// �Đ��{�^���Ƀt�H�[�J�X
	m_Play.SetFocus();
	/// �ꎞ��~�{�^���𖳌�
	m_Pause.EnableWindow(FALSE);
}

void CRsfVideoDialog::OnBtnStop()
{
	PlayStop();
}

void CRsfVideoDialog::OnBtnFile()
{
	CString str;
	CString strFile;

	/// �����r�f�I�t�@�C���Q��
	CFileDialog fileDlg(TRUE, _T("AVI"), _T("*.avi"), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 
		_T("AVI(*.avi)|*.avi|all(*.*)|*.*||"));
	if (fileDlg.DoModal() != IDOK) return;
	strFile = fileDlg.GetPathName();
	CFileSpec fs(strFile);
	if (!fs.Exist()) return;

	/// �Đ����̃t�@�C�����~����
	PlayStop();

	/// �t�@�C���̃t�H�[�}�b�g�`�F�b�N
	m_pVideo->SetPlayFile(strFile);
	if (!m_pVideo->ChkPlayFile()) {
		str.LoadString(IDS_MSGBOX66);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}

	/// ���݂̉����r�f�I�t�@�C���̃o�b�N�A�b�v
	fs.SetFullSpec(m_strPlayFile);
	if (fs.Exist()) {
		CTPsetup tps(theApp.m_strDataFolder);
		m_strTmpFile = tps.RsfTmpVideoFileName();
		fs.FileMove(m_strTmpFile);
	}

	/// �����r�f�I�t�@�C���̃R�s�[
	fs.SetFullSpec(strFile);
	fs.FileCopy(m_strPlayFile, TRUE, TRUE);

	/// ��ʂ̍X�V
	UpdateForm();
}

// �L�[�{�[�h����̒���
BOOL CRsfVideoDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		/// RETURN KEY : Bitmap�{�^���̎��s
		// �r�b�g�}�b�v�{�^���̓L�[�����Ńf�t�H���g�n���h���iOnOk�j���Ă�ł��܂�����
		if (nVirtKey == VK_RETURN) {
			//// �t�H�[�J�X�R���g���[����Bitmap�{�^���̏ꍇ�̂ݗL��
			CWnd *cWnd = GetFocus();
			if (cWnd->m_hWnd == m_Play.m_hWnd) {
				OnBtnPlay();
				return TRUE;
			} else if (cWnd->m_hWnd == m_Pause.m_hWnd) {
				OnBtnPause();
				return TRUE;
			} else if (cWnd->m_hWnd == m_Stop.m_hWnd) {
				OnBtnStop();
				return TRUE;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CRsfVideoDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	if (nIDEvent == ID_TIMER_RSF_VIDEO) {
		m_stPlayChk.lngPassTime++;	// �^�C�}�[�o�ߎ��Ԃ��X�V
		/// �Đ����̃v���O���X�o�[�X�V
		int intPos, intLow, intUpper;
		if (m_pVideo->IsPlaying()) {
			if (!m_pVideo->IsPlayPause()) {
				intPos = m_Prog.GetPos();
				m_Prog.GetRange(intLow, intUpper);
				if (intPos < intUpper) {
					m_Prog.StepIt();
				} else {
					/// �Đ��I��
					PlayStop();
				}
			}
		} else {
/* IsPlaying()�ɂȂ�܂łɎ��Ԃ�������ꍇ�����邽�߁A�ȉ��̏����͍폜
			/// �Đ��I��
			PlayStop();
			/// �v���O���X�o�[���t���Ɂi�����ځj
			m_Prog.GetRange(intLow, intUpper);
			m_Prog.SetPos(intUpper);
*/
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CRsfVideoDialog::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������

	///�@�o�^�m�F
//	str.LoadString(IDS_MSGBOX60);
//	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return;

	CDialog::OnOK();
}

void CRsfVideoDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B

	//// �o�b�N�A�b�v�t�@�C����߂�
	CFileSpec fs(m_strTmpFile);
	if (fs.Exist()) {
		fs.FileMove(m_strPlayFile);
	}
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function

//
//  �@�\     : ��ʂ̍X�V
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfVideoDialog::UpdateForm() 
{
	/// �Đ����̃t�@�C�����~����
	PlayStop();

	/// �����ݒ�
	m_lngPlayTime = 0;
	m_Play.EnableWindow(FALSE);
	m_Stop.EnableWindow(FALSE);
	m_Pause.EnableWindow(FALSE);
	m_Time.SetText(_T("[ 00:00 ]"));

	CString strSize;
	CFileSpec fs(m_strPlayFile);
	if (fs.Exist()) {
	/// �����r�f�I�t�@�C��������ꍇ
		/// �����r�f�I�t�@�C���̎��ԕ\��
		m_pVideo->SetPlayFile(m_strPlayFile);
		float fTime = m_pVideo->GetPlaySize();
		if (fTime < 0) return FALSE;
		m_lngPlayTime = (long)(fTime+1);
		strSize.Format(_T("[ %02i:%02i ]"), (m_lngPlayTime/60), (m_lngPlayTime%60));
		m_Time.SetText(strSize);
		//// �Đ��{�^����L��
		m_Play.EnableWindow(TRUE);
	}

	UpdateData(FALSE);	// DDX�X�V

	return TRUE;
}

//
//  �@�\     : �r�f�I�Đ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfVideoDialog::Play()
{
	if (_tcsclen(m_strPlayFile) <= 0) return FALSE;

	if (m_pVideo->IsPlaying() && m_pVideo->IsPlayPause()) {
		/// �Đ��ꎞ��~�̕��A
		m_pVideo->PlayContinue();
	} else {
		/// �Đ��J�n
		m_pVideo->PlayOpen();
		if (!m_pVideo->Play(m_strPlayFile)) {
			CString str;
			str.LoadString(IDS_MSGBOX64);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return FALSE;
		}

		/// ���t���ԊǗ��p�^�C�}�[�J�n
		StartPlayChkTimer(m_lngPlayTime);

		/// ��~�{�^����L��
		m_Stop.EnableWindow(TRUE);
	}
	/// �ꎞ��~�{�^����L��
	m_Pause.EnableWindow(TRUE);

	return TRUE;
}

//
//  �@�\     : �r�f�I�Đ���~
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfVideoDialog::PlayStop()
{
	if (_tcsclen(m_strPlayFile) <= 0) return FALSE;

	/// �Đ��I��
	if (m_pVideo->IsPlaying()) {
		m_pVideo->PlayStop();
	}

	//// ���t���ԊǗ��p�^�C�}�[�I��
	StopPlayChkTimer();
	//// �v���O���X�o�[������
//	m_Prog.SetPos(m_lngPlayTime);	

	/// �Đ��{�^���Ƀt�H�[�J�X
	m_Play.SetFocus();
	/// ��~�{�^���𖳌�
	m_Stop.EnableWindow(FALSE);
	/// �ꎞ��~�{�^���𖳌�
	m_Pause.EnableWindow(FALSE);

	return TRUE;
}

//
//  �@�\     : ���t���ԊǗ��p�^�C�}�[�J�n
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfVideoDialog::StartPlayChkTimer(
long lngPlayTime)	// PLAY����
{
	if (!m_stPlayChk.blOnTimer) {
		SetTimer(ID_TIMER_RSF_VIDEO, 1000, NULL);	// 1 sec each(must)
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StartPlayChkTimer"));
	}
	m_stPlayChk.blOnTimer = TRUE;		// �^�C�}�[���s���t���O
	m_stPlayChk.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A
	m_stPlayChk.lngLimitTime = lngPlayTime;		// �^�C�}�[�������Ԃ��Z�b�g

	//// �v���O���X�o�[�J�n
	if (m_Prog) {
		m_Prog.SetStep(1);
		m_Prog.SetRange32(0, m_lngPlayTime);
		m_Prog.SetPos(0);
	}

	return TRUE;
}

//
//  �@�\     : ���t���ԊǗ��p�^�C�}�[�I��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfVideoDialog::StopPlayChkTimer()
{
	if (m_stPlayChk.blOnTimer) {
		KillTimer(ID_TIMER_RSF_VIDEO);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopPlayChkTimer"));
	}
	m_stPlayChk.blOnTimer = FALSE;		// �^�C�}�[���s���t���O
	m_stPlayChk.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A
	m_stPlayChk.lngLimitTime = 0;		// �^�C�}�[�������Ԃ��N���A

	return TRUE;
}

