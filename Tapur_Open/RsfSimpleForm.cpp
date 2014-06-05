// RsfSimpleForm.cpp : implementation file
//

#include "stdafx.h"
#include "Tapur.h"
#include "MainFrm.h"
#include "RsfTree.h"
#include "RsfSimpleForm.h"
#include "RsfAudioDialog.h"
#include "RsfChatDialog.h"
#include "RsfVideoDialog.h"
#include "RsfImgDialog.h"
#include "RsfPrgDialog.h"
#include "Audio.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"
#include "file_access.h"
#include ".\rsfsimpleform.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CRsfSimpleForm

IMPLEMENT_DYNCREATE(CRsfSimpleForm, TVisualFormView)

CRsfSimpleForm::CRsfSimpleForm()
	: TVisualFormView(CRsfSimpleForm::IDD)
{
	//{{AFX_DATA_INIT(CRsfSimpleForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_blAudio = FALSE;
    m_blChat = FALSE;
    m_blVideo = FALSE;
    m_blImg = FALSE;
	m_blBeep = FALSE;
	m_blPrg = FALSE;
	m_idxPrg = 0;

	// �w�i�F�u���V������
	m_brush.CreateSolidBrush(RGB(255,255,255));		// white
}

CRsfSimpleForm::~CRsfSimpleForm()
{
}

void CRsfSimpleForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfSimpleForm)
    DDX_Check(pDX, IDC_CHK_AUDIO, m_blAudio);
    DDX_Check(pDX, IDC_CHK_CHAT, m_blChat);
    DDX_Check(pDX, IDC_CHK_VIDEO, m_blVideo);
    DDX_Check(pDX, IDC_CHK_IMG, m_blImg);
    DDX_Check(pDX, IDC_CHK_BEEP, m_blBeep);
	DDX_Control(pDX, IDC_ST_SIZE_AUDIO, m_lblAudioSize);
	DDX_Control(pDX, IDC_ST_SIZE_CHAT, m_lblChatSize);
	DDX_Control(pDX, IDC_ST_SIZE_VIDEO, m_lblVideoSize);
	DDX_Control(pDX, IDC_ST_SIZE_IMG, m_lblImgSize);
    DDX_Check(pDX, IDC_CHK_PRG, m_blPrg);
	DDX_CBIndex(pDX, IDC_CMB_PRG, m_idxPrg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRsfSimpleForm, TVisualFormView)
	//{{AFX_MSG_MAP(CRsfSimpleForm)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHK_AUDIO, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_CHAT, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_VIDEO, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_IMG, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_BEEP, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_REC, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_VREC, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_DISCONNECT, OnChgForm)
	ON_BN_CLICKED(IDC_BTN_AUDIO, OnButtonAudio)
	ON_BN_CLICKED(IDC_BTN_CHAT, OnButtonChat)
	ON_BN_CLICKED(IDC_BTN_VIDEO, OnButtonVideo)
	ON_BN_CLICKED(IDC_BTN_IMG, OnButtonImage)
	ON_BN_CLICKED(IDC_CHK_PRG, OnChgForm)
	ON_CBN_SELCHANGE(IDC_CMB_PRG, OnChgForm)
	ON_BN_CLICKED(IDC_BTN_PRG, OnButtonPrg)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfSimpleForm diagnostics

#ifdef _DEBUG
void CRsfSimpleForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CRsfSimpleForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRsfSimpleForm message handlers

void CRsfSimpleForm::OnDestroy()
{
	TVisualFormView::OnDestroy();

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

void CRsfSimpleForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	/// ���x���̑����ύX
	m_lblAudioSize.SetBkColor(RGB(255,255,255));	// white
	m_lblChatSize.SetBkColor(RGB(255,255,255));		// white
	m_lblVideoSize.SetBkColor(RGB(255,255,255));	// white
	m_lblImgSize.SetBkColor(RGB(255,255,255));		// white
/*
	//// �t�H�[���̃f�t�H���g�ݒ�̎擾
	CFont *ft = CFormView::GetFont();
	LOGFONT lft;
	ft->GetLogFont(&lft);
	//// �X�V
	m_Title.SetFontStatic(lft.lfFaceName, abs(lft.lfHeight), RGB(0,0,0), FS_VCENTER | FS_UNDERLINED | FS_BOLD);
*/

	/// �A�g�v���O�������R���{�{�b�N�X������
	CString strItm;
	CComboBox *pcmb;
    pcmb = (CComboBox*)GetDlgItem(IDC_CMB_PRG);
	strItm.LoadString(IDS_RSF_PRG1);
	pcmb->AddString(strItm);
	strItm.LoadString(IDS_RSF_PRG11);
	pcmb->AddString(strItm);
	strItm.LoadString(IDS_RSF_PRG901);
	pcmb->AddString(strItm);
	m_idxPrg = 0;	// �擪��I��
	/// �A�g�v���O����ID���X�g������
	m_aryPrgId.Add(_T("1"));	
	m_aryPrgId.Add(_T("11"));	
	m_aryPrgId.Add(_T("901"));

	UpdateData(FALSE);	// DDX�X�V
}

HBRUSH CRsfSimpleForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = TVisualFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������
	
	// TODO: �f�t�H���g�̃u���V���]�݂̂��̂łȂ��ꍇ�ɂ́A�Ⴄ�u���V��Ԃ��Ă�������

	// �t�H�[���w�i�F�𔒂ɕύX
	if (nCtlColor == CTLCOLOR_DLG) {
		return (HBRUSH)m_brush.GetSafeHandle();
	}

	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkColor(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);//�w�i����
//			pDC->SetTextColor(RGB(0,0,255));// �e�L�X�g�F�ݒ�
		return (HBRUSH)m_brush.GetSafeHandle();
	}

	return hbr;
}

BOOL CRsfSimpleForm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		if (nVirtKey == VK_TAB) {
		/// TAB KEY : RsfTree�t�H�[���֑J��
			//// �t�H�[�J�X�Ō�̏ꍇ�̂ݗL��
			CWnd *cWnd = GetFocus();
			if (cWnd->m_hWnd == GetDlgItem(IDC_RDO_REC)->m_hWnd ||
				cWnd->m_hWnd == GetDlgItem(IDC_RDO_VREC)->m_hWnd ||
				cWnd->m_hWnd == GetDlgItem(IDC_RDO_DISCONNECT)->m_hWnd) {
				CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
				CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
				pfview->SetFocus();
				return TRUE;
			}
		}
	}
	
	return TVisualFormView::PreTranslateMessage(pMsg);
}

// �������̓{�^��
void CRsfSimpleForm::OnButtonAudio() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	/// �����t�@�C���̓���
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(m_strSenderId);
	CString strFile = tps.RsfAudioFileName(_T("$"));

	/// ��ʂ̕\��
	CRsfAudioDialog dlg;
	dlg.SetFile(strFile);
	if (dlg.DoModal() == IDOK) {
		/// RSF�t�@�C���̍X�V
		if (!UpdateFile()) return;
		/// ��ʂ̍X�V
		UpdateForm();
	}
}

// �`���b�g�e�L�X�g���̓{�^��
void CRsfSimpleForm::OnButtonChat() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	/// �`���b�g�t�@�C���̓���
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(m_strSenderId);
	CString strFile = tps.RsfChatFileName(_T("$"));

	/// ��ʂ̕\��
	CRsfChatDialog dlg;
	dlg.SetFile(strFile);
	if (dlg.DoModal() == IDOK) {
		/// RSF�t�@�C���̍X�V
		if (!UpdateFile()) return;
		/// ��ʂ̍X�V
		UpdateForm();
	}
}

// �r�f�I���̓{�^��
void CRsfSimpleForm::OnButtonVideo() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	/// �r�f�I�t�@�C���̓���
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(m_strSenderId);
	CString strFile = tps.RsfVideoFileName(_T("$"));

	/// ��ʂ̕\��
	CRsfVideoDialog dlg;
	dlg.SetFile(strFile);
	if (dlg.DoModal() == IDOK) {
		/// RSF�t�@�C���̍X�V
		if (!UpdateFile()) return;
		/// ��ʂ̍X�V
		UpdateForm();
	}
}

// �摜���̓{�^��
void CRsfSimpleForm::OnButtonImage() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	/// �摜�t�@�C���̓���
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(m_strSenderId);
	CString strFile = tps.FindRsfImgFile(_T("$"));
	CString strTmp = tps.RsfImgFileName(_T("$"), _T(""));	// �g���q�Ȃ��t�@�C��

	/// ��ʂ̕\��
	CRsfImgDialog dlg;
	dlg.SetFile(strFile);
	dlg.SetFileNoExt(strTmp);
	if (dlg.DoModal() == IDOK) {
		/// RSF�t�@�C���̍X�V
		if (!UpdateFile()) return;
		/// ��ʂ̍X�V
		UpdateForm();
	}
}

// �A�g�v���O�����ݒ�{�^��
void CRsfSimpleForm::OnButtonPrg() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString str;

	UpdateData(TRUE);	// DDX�X�V

	// �A�g�v���O����ID
	CString strPrgId = m_aryPrgId.GetAt(m_idxPrg);

	/// �v���O����CSV�t�@�C���̓���
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(m_strSenderId);
	CString strFile = tps.RsfPrgFileName(_T("$$$$"), strPrgId);

	/// ��ʂ̕\��
	CComboBox *pcmb;
    pcmb = (CComboBox*)GetDlgItem(IDC_CMB_PRG);
	pcmb->GetLBText(m_idxPrg, str);
	CRsfPrgDialog dlg(this, str);
	dlg.SetFile(strFile);
	if (dlg.DoModal() == IDOK) {
		/// RSF�t�@�C���̍X�V
		if (!UpdateFile()) return;
		/// ��ʂ̍X�V
		UpdateForm();
	}
}

// �t�H�[�����̓C�x���g
// checkbox, radio button ������
void CRsfSimpleForm::OnChgForm() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	// checkbox�̕��p�`�F�b�N
	CButton *chk_audio = (CButton *)GetDlgItem(IDC_CHK_AUDIO);
	CButton *chk_chat = (CButton *)GetDlgItem(IDC_CHK_CHAT);
	CButton *chk_img = (CButton *)GetDlgItem(IDC_CHK_IMG);
	CButton *chk_video = (CButton *)GetDlgItem(IDC_CHK_VIDEO);
	if (chk_img->GetCheck() && chk_video->GetCheck()) {
		UpdateData(FALSE);	// �{�^����Ԃ�߂�
		CString str;
		str.LoadString(IDS_MSGBOX58);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}
	if (!chk_audio->GetCheck() && !chk_chat->GetCheck() && 
		!chk_img->GetCheck() && !chk_video->GetCheck()) {
			UpdateData(FALSE);	// �{�^����Ԃ�߂�
			CString str;
			str.LoadString(IDS_MSGBOX59);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
			return;
	}

	/// RSF�t�@�C���̍X�V
	UpdateFile();
}

/////////////////////////////////////////////////////////////////////////////
// local function

//
//  �@�\     : RSF�t�@�C���̍X�V
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfSimpleForm::UpdateFile() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString str;

	UpdateData(TRUE);	// DDX�X�V

	// ���W�I�{�^���̎Q�ƒ�`�iDDX�͈����ɂ����̂Łj
	// �i�N���X�����o�[�Ő錾����Ɛ���ɓ��삵�Ȃ��j
	CButton *rd_rec = (CButton *)GetDlgItem(IDC_RDO_REC);
	CButton *rd_vrec = (CButton *)GetDlgItem(IDC_RDO_VREC);
	CButton *rd_discon = (CButton *)GetDlgItem(IDC_RDO_DISCONNECT);

	/// �����ݒ�p�����[�^�쐬
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(m_strSenderId);
	CRsfPrm prm;
	CArray<CRsfPrm, CRsfPrm> prmArray;
	//// �������b�Z�[�W
	if (m_blAudio) {
		prm.type = RESP_TYPE_PLAY;
		prm.play_type |= RESP_PLAY_TYPE_AUDIO;
	}
	if (m_blChat) {
		prm.type = RESP_TYPE_PLAY;
		prm.play_type |= RESP_PLAY_TYPE_CHAT;
	}
	if (m_blVideo) {
		prm.type = RESP_TYPE_PLAY;
		prm.play_type |= RESP_PLAY_TYPE_VIDEO;
	}
	if (m_blImg) {
		prm.type = RESP_TYPE_PLAY;
		prm.play_type |= RESP_PLAY_TYPE_IMAGE;
		str = tps.FindRsfImgFile(_T("$"));
		str = str.Right(3);
		str.MakeLower();
		if (str == _T("bmp")) {
			prm.file_type = RESP_FILE_TYPE_BMP;
		} else if (str == _T("jpg")) {
			prm.file_type = RESP_FILE_TYPE_JPEG;
		} else if (str == _T("gif")) {
			prm.file_type = RESP_FILE_TYPE_GIF;
		}
	}
	if (prm.type == RESP_TYPE_PLAY) {
		prmArray.Add(prm);
	}
	//// �^���^�C�v
	prm.Init();
	if (m_blBeep) {
		prm.beep = 1;
	}
	if (rd_rec->GetCheck()) {
		prm.type = RESP_TYPE_RECORD;
	} else if (rd_vrec->GetCheck()) {
		prm.type = RESP_TYPE_VRECORD;
	} else if (rd_discon->GetCheck()) {
		prm.type = RESP_TYPE_DISCONNECT;
	}
	prmArray.Add(prm);
	//// �I��
	if (prm.type != RESP_TYPE_DISCONNECT) {
		prm.Init();
		prm.type = RESP_TYPE_DISCONNECT;
		prmArray.Add(prm);
	}
	//// �A�g�v���O����
	if (m_blPrg) {
		prm.Init();
		prm.type = RESP_TYPE_EXEC;
		CString strPrgId = m_aryPrgId.GetAt(m_idxPrg);
		prm.exec_id = _ttoi(strPrgId);
		prmArray.Add(prm);
	}

	/// �����ݒ�t�@�C���č쐬
	if (!tps.SaveRsfFile(prmArray)){
		return FALSE;
	}

	return TRUE;
}

//
//  �@�\     : ��ʂ̍X�V
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfSimpleForm::UpdateForm() 
{
	/// �J�����g���R�[�h
	if (theApp.m_lngRsflId <= 0) return FALSE;

	/// �Đ����̃t�@�C�����~����
//	PlayStop();

	// StatusBar�̃N���A
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	pFrame->SetStatusText(_T(""), 0);
	pFrame->SetStatusText(_T(""), 1);

	// ���W�I�{�^���̎Q�ƒ�`�iDDX�͈����ɂ����̂Łj
	// �i�N���X�����o�[�Ő錾����Ɛ���ɓ��삵�Ȃ��j
	CButton *rd_rec = (CButton *)GetDlgItem(IDC_RDO_REC);
	CButton *rd_vrec = (CButton *)GetDlgItem(IDC_RDO_VREC);
	CButton *rd_discon = (CButton *)GetDlgItem(IDC_RDO_DISCONNECT);

	/// �����ݒ�
	m_blAudio = FALSE;
    m_blChat = FALSE;
    m_blVideo = FALSE;
    m_blImg = FALSE;
	m_blBeep = FALSE;
	m_blPrg = FALSE;
	rd_rec->SetCheck(0);
	rd_vrec->SetCheck(0);
	rd_discon->SetCheck(1);

	/// �����ݒ�e�[�u����I��
	CTblRsfl tbl(theApp.m_strDBPath);
	CString strWhere;
	_TCHAR buf[64];
	strWhere.Empty();
	_ltot_s(theApp.m_lngRsflId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T("ID") + _T("=") + buf;
	_ltot_s(RESPBASE_TYPE_SIMPLE, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T(" and ") + _T("lngRespType") + _T("=") + buf;
	if (tbl.SelectRec(strWhere) <= 0) {
		return FALSE;
	}

	/// �I�����ꂽ�����ݒ�RSF�����[�h
	CTPsetup tps(theApp.m_strDataFolder);
	if (tbl.lngDefaultFlag == 1) {
		m_strSenderId = _T("0");	// default setting
	} else {
		m_strSenderId = tbl.strSenderId;
	}
	tps.SetCallerId(m_strSenderId);
	UST_RSF *pst = tps.LoadRSF();
	if (pst == NULL) return FALSE;

	/// �I�����ꂽ�����ݒ�RSF���Q��
	UST_RSF_RESP *pst_resp;
	for (int i = 0; i < pst->resp_cnt; i++) {
		pst_resp = pst->resp + i;
		//// BEEP��ON/OFF
		if (pst_resp->beep > 0) {
			m_blBeep = TRUE;
		}
		switch(pst_resp->type) {
		case RESP_TYPE_PLAY:
			//// �Đ����e��ON/OFF
			if (pst_resp->play_type & RESP_PLAY_TYPE_AUDIO) {
				m_blAudio = TRUE;
			}
			if (pst_resp->play_type & RESP_PLAY_TYPE_VIDEO) {
				m_blVideo = TRUE;
			}
			if (pst_resp->play_type & RESP_PLAY_TYPE_IMAGE) {
				m_blImg = TRUE;
			}
			if (pst_resp->play_type & RESP_PLAY_TYPE_CHAT) {
				m_blChat = TRUE;
			}
			break;
		//// �^�����e��ON/OFF
		case RESP_TYPE_RECORD:
			rd_rec->SetCheck(1);
			rd_vrec->SetCheck(0);
			rd_discon->SetCheck(0);
			break;
		case RESP_TYPE_VRECORD:
			rd_rec->SetCheck(0);
			rd_vrec->SetCheck(1);
			rd_discon->SetCheck(0);
			break;
		case RESP_TYPE_DISCONNECT:
			// �S�Ă̐ݒ�Ŏg�p�����̂ŁA�ݒ肷�ׂ����Ƃ͂Ȃ�
			break;
		//// �v���O�����A�g��ON/OFF
		case RESP_TYPE_EXEC:
			m_blPrg = TRUE;
			break;
		default:
			break;
		}
	}

	/// ���������t�@�C���̎��ԕ\��
	CString strPlayFile;
	CString strSize;
	CFileSpec fs;
	strSize = _T("[ 00:00 ]");
	strPlayFile = tps.RsfAudioFileName(_T("$"));
	fs.SetFullSpec(strPlayFile);
	if (fs.Exist()) {
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CAudio *pAudio = pFrame->m_pAudio;
		pAudio->m_PlayFile = strPlayFile;
		float fTime = pAudio->GetPlaySize();
		if (fTime > 0.0) {
			long lngTime = (long)(fTime+1);
			strSize.Format(_T("[ %02i:%02i ]"), (lngTime/60), (lngTime%60));
		}
	}
	m_lblAudioSize.SetText((LPCTSTR)strSize);

	/// �����r�f�I�t�@�C���̎��ԕ\��
	strSize = _T("[ 00:00 ]");
	strPlayFile = tps.RsfVideoFileName(_T("$"));
	fs.SetFullSpec(strPlayFile);
	if (fs.Exist()) {
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CVideo *pVideo = pFrame->m_pVideo;
		pVideo->SetPlayFile(strPlayFile);
		float fTime = pVideo->GetPlaySize();
		if (fTime > 0.0) {
			long lngTime = (long)(fTime+1);
			strSize.Format(_T("[ %02i:%02i ]"), (lngTime/60), (lngTime%60));
		}
	}
	m_lblVideoSize.SetText((LPCTSTR)strSize);

	/// �摜�t�@�C���̃T�C�Y�\��
	strSize = _T("[ 0 Kbytes ]");
	strPlayFile = tps.FindRsfImgFile(_T("$"));
	fs.SetFullSpec(strPlayFile);
	if (fs.Exist()) {
		strSize.Format(_T("[ %d Kbytes ]"), fs.FileSize()/1024);
	}
	m_lblImgSize.SetText((LPCTSTR)strSize);

	/// �`���b�g�e�L�X�g�t�@�C���̃T�C�Y�\��
	strSize = _T("[ 0 bytes ]");
	strPlayFile = tps.RsfChatFileName(_T("$"));
	fs.SetFullSpec(strPlayFile);
	if (fs.Exist()) {
		strSize.Format(_T("[ %d bytes ]"), fs.FileSize());
	}
	m_lblChatSize.SetText((LPCTSTR)strSize);

 	/// �A�g�v���O�����R���{�{�b�N�X��I��
	CString str, strPrgId;
	strPrgId.Format(_T("%d"), pst_resp->exec_id);
	int idx = 0;
	for(int i = 0; i <  m_aryPrgId.GetSize(); i++) {
		str = m_aryPrgId.GetAt(i);
		if (strPrgId == str) {
			idx = i;
			break;
		}
	}
	m_idxPrg = idx;

	/// �A�g�v���O����CSV�t�@�C�������݂��Ȃ��ꍇ�A�e���v���[�gCSV�t�H���_����R�s�[
	CString strBinDir, strFile;
	fs.SetFullSpec(FS_APPDIR);
	strBinDir = fs.GetFullSpec() + _T("bin");	// �e���v���[�gCSV�t�H���_
	for(int i = 0; i <  m_aryPrgId.GetSize(); i++) {
		strPrgId = m_aryPrgId.GetAt(i);
		strFile = tps.RsfPrgFileName(_T("$$$$"), strPrgId);	// �A�g�v���O����CSV�t�@�C��
		fs.SetFullSpec(strFile);
		if (!fs.Exist()) {
			fs.SetFullSpec(strBinDir + "\\" + strPrgId + ".csv");
			if (fs.Exist()) {
				fs.FileCopy(strFile, FALSE);	// �㏑���͂��Ȃ�
			}
		}
	}

	UpdateData(FALSE);	// DDX�X�V

	return TRUE;
}


