// SettingRecTab.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "SettingRecTab.h"
#include "SettingDialog.h"
#include "SetChatDialog.h"
#include "TabCtrlSSL.h"
#include "Audio.h"
#include "TPutil.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingRecTab �_�C�A���O


CSettingRecTab::CSettingRecTab(CWnd* pParent /*=NULL*/)
	: CTabPageSSL (CSettingRecTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingRecTab)
	m_idxCodecName = 0;
	m_idxFormatName = 0;
	m_idxRecordLimit = 0;
	m_blRecWarning = TRUE;
	m_blRecStopWarning = TRUE;
	m_blEnableAuto = TRUE;
	//}}AFX_DATA_INIT
}


void CSettingRecTab::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingRecTab)
	DDX_CBIndex(pDX, IDC_CMB_CODEC, m_idxCodecName);
	DDX_CBIndex(pDX, IDC_CMB_FORMAT, m_idxFormatName);
	DDX_CBIndex(pDX, IDC_CMB_RECORD_LIMIT, m_idxRecordLimit);
	DDX_Check(pDX, IDC_CHK_REC_WARNING, m_blRecWarning);
	DDX_Control(pDX, IDC_BTN_REC_WARNING, m_btnRecWarning);
	DDX_Check(pDX, IDC_CHK_RECSTOP_WARNING, m_blRecStopWarning);
	DDX_Control(pDX, IDC_BTN_RECSTOP_WARNING, m_btnRecStopWarning);
	DDX_Check(pDX, IDC_CHK_AUTO_ENABLE, m_blEnableAuto);
    DDX_Text(pDX, IDC_EDIT_VOL_SND, m_strVolSnd);
    DDX_Text(pDX, IDC_EDIT_VOL_RCV, m_strVolRcv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingRecTab, CDialog)
	//{{AFX_MSG_MAP(CSettingRecTab)
	ON_BN_CLICKED(IDC_CHK_REC_WARNING, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_RECSTOP_WARNING, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_AUTO_ENABLE, OnChgForm)
	ON_BN_CLICKED(IDC_BTN_REC_WARNING, OnBtnRecWarning)
	ON_BN_CLICKED(IDC_BTN_RECSTOP_WARNING, OnBtnRecStopWarning)
	ON_BN_CLICKED(IDC_RDO_MONORAL, OnRdoStereo)
	ON_BN_CLICKED(IDC_RDO_STEREO, OnRdoStereo)
	ON_BN_CLICKED(IDC_RDO_STEREO2, OnRdoStereo)
	ON_CBN_SELCHANGE(IDC_CMB_RECORD_LIMIT, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_WAV, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_MP3, OnRdoMp3)
	ON_CBN_SELCHANGE(IDC_CMB_CODEC, OnCmbCodec)
	ON_CBN_SELCHANGE(IDC_CMB_FORMAT, OnChgForm)
	ON_EN_CHANGE(IDC_EDIT_VOL_SND, OnChgVolSnd)
	ON_EN_CHANGE(IDC_EDIT_VOL_RCV, OnChgVolRcv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingRecTab ���b�Z�[�W �n���h��

BOOL CSettingRecTab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	int i;
	int intCnt;
	CString str;
	DWORD dw;

	/// ���W�X�g���ǂݍ���
	//// �����񃌃W�X�g���ł͋󔒓��͂����������ꍇ������̂�force=TRUE��
	m_rgEnableRecWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVoiceRecordWarning"), 1, TRUE);	// �����l:1(True)
	m_rgEnableRecStopWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVoiceRecordStopWarning"), 1, TRUE);	// �����l:1(True)
	m_rgEnableAuto = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableAutoVoiceRecord"), 0, TRUE);	// �����l:0(False)
	m_rgVolSnd = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordVolSnd"), 15, TRUE);	// �����l:15%
	m_rgVolRcv = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordVolRcv"), 0, TRUE);		// �����l:0%

	/// �^�����x���`���b�g
	dw = m_rgEnableRecWarning;
	m_blRecWarning = (dw == 1) ? TRUE : FALSE;
	dw = m_rgEnableRecStopWarning;
	m_blRecStopWarning = (dw == 1) ? TRUE : FALSE;
 	/// �^����������
	m_rgRecordLimit = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\RecordLimitTime"), 3600);		// �����l:60min
 	/// monoral or stereo�i���S�p���jor  stereo�i�����p���j
	m_rgStereo = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordStereo"), 1, TRUE);		// �����l:stereo ??force=TRUE�łȂ��ƃ��W�X�g������������Ȃ�
 	/// WAV or MP3
	m_rgFormatType = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordType"), 0, TRUE);		// �����l:0(WAV)
	m_rgCodecName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordMP3Codec"), _T(""), TRUE);
	m_rgFormatName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordMP3Format"), _T(""), TRUE);
 	/// �����^��
	dw = m_rgEnableAuto;
	m_blEnableAuto = (dw == 1) ? TRUE : FALSE;
	/// ���ʒ���
	dw = m_rgVolSnd;
	m_strVolSnd.Format(_T("%d"), dw);
	dw = m_rgVolRcv;
	m_strVolRcv.Format(_T("%d"), dw);

	/// �R���{�{�b�N�X������
	CComboBox *pcmb;
 	//// �^������
    m_aryRecordLimit.Add(_T("5"));
    m_aryRecordLimit.Add(_T("10"));
    m_aryRecordLimit.Add(_T("15"));
    m_aryRecordLimit.Add(_T("20"));
    m_aryRecordLimit.Add(_T("30"));
    m_aryRecordLimit.Add(_T("45"));
    m_aryRecordLimit.Add(_T("60"));
    m_aryRecordLimit.Add(_T("90"));
    m_aryRecordLimit.Add(_T("120"));
    m_aryRecordLimit.Add(_T("180"));
    m_aryRecordLimit.Add(_T("240"));
    m_aryRecordLimit.Add(_T("360"));
    m_aryRecordLimit.Add(_T("480"));
    m_aryRecordLimit.Add(_T("720"));
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_RECORD_LIMIT);
	for(i = 0; i < m_aryRecordLimit.GetSize(); i++) {
		pcmb->AddString(m_aryRecordLimit.GetAt(i));
	}
 	//// �^���R���{�{�b�N�X��I��
	dw = m_rgRecordLimit;
	dw /= 60;	// sec -> min
	for(i = 0; i <  m_aryRecordLimit.GetSize(); i++) {
		str = m_aryRecordLimit.GetAt(i);
		if (dw == (DWORD)_ttoi(str)) {
			m_idxRecordLimit = i;
			break;
		}
	}
 	//// MP3�R�[�f�b�N
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_CODEC);
	intCnt = m_oAcmMgr.GetDriverList(m_aryCodecName);
	if (intCnt > 0) {
		for(i = 0; i < intCnt; i++) {
			pcmb->AddString(m_aryCodecName.GetAt(i));
		}
 		//// MP3�R�[�f�b�N�R���{�{�b�N�X��I��
		str = m_rgCodecName;
		for(i = 0; i <  m_aryCodecName.GetSize(); i++) {
			if (str == m_aryCodecName.GetAt(i)) {
				m_idxCodecName = i;
				break;
			}
		}
		str = m_aryCodecName.GetAt(m_idxCodecName);
		str.MakeUpper();
		if (str.Find(ACM_CODEC_MP3_LAME) >= 0) {
			m_oAcmMgr.FindDriver(CAcmMgr::WAV_MP3_LAME, str);
		} else {
			m_oAcmMgr.FindDriver(CAcmMgr::WAV_MP3, str);
		}
 		//// MP3�t�H�[�}�b�g
		pcmb = (CComboBox*)GetDlgItem(IDC_CMB_FORMAT);
		CWave wv;
		if (m_rgStereo == 0) {
			wv.BuildFormat(1, 16000, 16);	//monoral
		} else {
			wv.BuildFormat(2, 16000, 16);	//stereo
		}
		m_wfx = wv.GetFormat();
		intCnt = m_oAcmMgr.GetFormatList(m_aryFormatName, &m_wfx);
		if (intCnt > 0) {
			for(i = 0; i < intCnt; i++) {
				pcmb->AddString(m_aryFormatName.GetAt(i));
			}
 			//// MP3�t�H�[�}�b�g�R���{�{�b�N�X��I��
			str = m_rgFormatName;
			for(i = 0; i <  m_aryFormatName.GetSize(); i++) {
				if (str == m_aryFormatName.GetAt(i)) {
					m_idxFormatName = i;
					break;
				}
			}
		}
	} else {
		//// MP3�R�[�f�b�N�����݂��Ȃ��ꍇ�A�����I��WAV��I��
		m_rgFormatType = 0;	// WAV
		//// MP3��I��s��
		CButton *rd = (CButton *)GetDlgItem(IDC_RDO_MP3);
		rd->EnableWindow(0);
	}

 	//// monoral or stereo��I��
	dw = m_rgStereo;
	// ���W�I�{�^���̎Q�ƒ�`�iDDX�͈����ɂ����̂Łj
	// �i�N���X�����o�[�Ő錾����Ɛ���ɓ��삵�Ȃ��j
	CButton *rd_monoral = (CButton *)GetDlgItem(IDC_RDO_MONORAL);
	CButton *rd_stereo1 = (CButton *)GetDlgItem(IDC_RDO_STEREO1);
	CButton *rd_stereo2 = (CButton *)GetDlgItem(IDC_RDO_STEREO2);
	if (dw == 1) {
		rd_monoral->SetCheck(0);
		rd_stereo1->SetCheck(1);
		rd_stereo2->SetCheck(0);
	} else if (dw == 2) {
		rd_monoral->SetCheck(0);
		rd_stereo1->SetCheck(0);
		rd_stereo2->SetCheck(1);
	} else {
		rd_monoral->SetCheck(1);
		rd_stereo1->SetCheck(0);
		rd_stereo2->SetCheck(0);
	}
 	//// WAV or MP3��I��
	dw = m_rgFormatType;
	CButton *rd_wav = (CButton *)GetDlgItem(IDC_RDO_WAV);
	CButton *rd_mp3 = (CButton *)GetDlgItem(IDC_RDO_MP3);
	if (dw == 1) {
		rd_wav->SetCheck(0);
		rd_mp3->SetCheck(1);
	} else {
		rd_wav->SetCheck(1);
		rd_mp3->SetCheck(0);
	}
	
	UpdateData(FALSE);	// DDX�X�V
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// �^�����x�����b�Z�[�W�̐ݒ�
void CSettingRecTab::OnBtnRecWarning() 
{
	UpdateData(TRUE);	// DDX�X�V

	CSetChatDialog dlg;
	dlg.SetRegKey(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordWarning"));
	if (dlg.DoModal() != IDOK) return;
}
void CSettingRecTab::OnBtnRecStopWarning() 
{
	UpdateData(TRUE);	// DDX�X�V

	CSetChatDialog dlg;
	dlg.SetRegKey(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordStopWarning"));
	if (dlg.DoModal() != IDOK) return;
}

// �^���`���̕ύX��MP3�t�H�[�}�b�g���X�g�X�V
void CSettingRecTab::OnRdoStereo() 
{
	UpdateData(TRUE);	// DDX�X�V

	int i;

	/// MP3�t�H�[�}�b�g���X�g���X�V
	CWave wv;
	CButton *rd_monoral = (CButton *)GetDlgItem(IDC_RDO_MONORAL);
	if (rd_monoral->GetCheck()) {
		if (m_wfx.nChannels == 1) goto PROC_END;	// �t�H�[�}�b�g�̕ύX���s�v
		wv.BuildFormat(1, 16000, 16);	//monoral
	} else {
		if (m_wfx.nChannels == 2) goto PROC_END;	// �t�H�[�}�b�g�̕ύX���s�v
		wv.BuildFormat(2, 16000, 16);	//stereo
	}
	m_wfx = wv.GetFormat();
	int intCnt = m_oAcmMgr.GetFormatList(m_aryFormatName, &m_wfx);
	CComboBox *pcmb = (CComboBox*)GetDlgItem(IDC_CMB_FORMAT);
	pcmb->ResetContent();
	for(i = 0; i < intCnt; i++) {
		pcmb->AddString(m_aryFormatName.GetAt(i));
	}
 	/// MP3�t�H�[�}�b�g�R���{�{�b�N�X��I��
	m_idxFormatName = 0;

	UpdateData(FALSE);	// DDX�X�V

PROC_END:
	/// �K�p�{�^���̗L����
	OnChgForm();
}

// MP3�R�[�f�b�N�̕ύX���t�H�[�}�b�g���X�g�X�V
void CSettingRecTab::OnCmbCodec() 
{
	UpdateData(TRUE);	// DDX�X�V

	int i;

	/// MP3�R�[�f�b�N��I��
	CString str = m_aryCodecName.GetAt(m_idxCodecName);
	str.MakeUpper();
	if (str.Find(ACM_CODEC_MP3_LAME) >= 0) {
		m_oAcmMgr.FindDriver(CAcmMgr::WAV_MP3_LAME, str);
	} else {
		m_oAcmMgr.FindDriver(CAcmMgr::WAV_MP3, str);
	}

	/// MP3�t�H�[�}�b�g���X�g���X�V
	int intCnt = m_oAcmMgr.GetFormatList(m_aryFormatName, &m_wfx);
	if (intCnt == 0 && str.Find(ACM_CODEC_MP3) >= 0) {
		//// FraunHoffer MP3�R�[�f�b�N���g�p�ł��Ȃ��ꍇ�iVista�j
		CAudio *pAudio = CAudio::Instance();
		pAudio->MP3CodecRegistry();
	}

	/// MP3�t�H�[�}�b�g�R���{�{�b�N�X���X�V
	CComboBox *pcmb = (CComboBox*)GetDlgItem(IDC_CMB_FORMAT);
	pcmb->ResetContent();
	for(i = 0; i < intCnt; i++) {
		pcmb->AddString(m_aryFormatName.GetAt(i));
	}
 	/// MP3�t�H�[�}�b�g�R���{�{�b�N�X��I��
	m_idxFormatName = 0;

	UpdateData(FALSE);	// DDX�X�V

	/// �K�p�{�^���̗L����
	OnChgForm();
}

// MP3�̑I���FMP3�R�[�f�b�N���g�p�ł��Ȃ��ꍇ�iVista�j�̃`�F�b�N
void CSettingRecTab::OnRdoMp3() 
{
	UpdateData(TRUE);	// DDX�X�V

	/// MP3�R�[�f�b�N��I��
	CString str = m_aryCodecName.GetAt(m_idxCodecName);
	str.MakeUpper();

	/// MP3�t�H�[�}�b�g���X�g���X�V
	int intCnt = m_oAcmMgr.GetFormatList(m_aryFormatName, &m_wfx);
	if (intCnt == 0 && str.Find(ACM_CODEC_MP3) >= 0) {
		//// FraunHoffer MP3�R�[�f�b�N���g�p�ł��Ȃ��ꍇ�iVista�j
		CAudio *pAudio = CAudio::Instance();
		pAudio->MP3CodecRegistry();
	}

	UpdateData(FALSE);	// DDX�X�V

	/// �K�p�{�^���̗L����
	OnChgForm();
}

// ���ʒ��ߕύX�`�F�b�N�C�x���g
void CSettingRecTab::OnChgVolSnd() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CString strOld = m_strVolSnd;
	UpdateData(TRUE);	// DDX�X�V

	// �����̂ݗL��
	if (!ChkNumber(m_strVolSnd)) {
		m_strVolSnd = strOld;
		UpdateData(FALSE);	// DDX�X�V
		return;
	}
	// -400 <-> 400
	int intVal = _ttoi(m_strVolSnd);
	if (intVal < -400 || intVal > 400) {
		m_strVolSnd = strOld;
		UpdateData(FALSE);	// DDX�X�V
		return;
	}

	UpdateData(FALSE);	// DDX�X�V

	/// �K�p�{�^���̗L����
	OnChgForm();
}
void CSettingRecTab::OnChgVolRcv() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CString strOld = m_strVolRcv;
	UpdateData(TRUE);	// DDX�X�V

	// �����̂ݗL��
	if (!ChkNumber(m_strVolRcv)) {
		m_strVolRcv = strOld;
		UpdateData(FALSE);	// DDX�X�V
		return;
	}
	// -400 <-> 400
	int intVal = _ttoi(m_strVolRcv);
	if (intVal < -400 || intVal > 400) {
		m_strVolRcv = strOld;
		UpdateData(FALSE);	// DDX�X�V
		return;
	}

	UpdateData(FALSE);	// DDX�X�V

	/// �K�p�{�^���̗L����
	OnChgForm();
}

// �t�H�[�����̓C�x���g
void CSettingRecTab::OnChgForm() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	/// �K�p�{�^���̗L����
	CWnd *cWnd;
	cWnd = GetParentOwner();
	CSettingDialog *pDlg = (CSettingDialog *)cWnd;
	pDlg->EnableApply();
}

/////////////////////////////////////////////////////////////////////////////
// local function

//
//  �@�\     : ���W�X�g���֕ۑ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
void CSettingRecTab::Save() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	CString str;
	DWORD dw;

	UpdateData(TRUE);	// DDX�X�V

	/// ���W�X�g���X�V
 	//// �^�����x�����b�Z�[�W
	m_rgEnableRecWarning = (m_blRecWarning) ? 1 : 0;
	m_rgEnableRecStopWarning = (m_blRecStopWarning) ? 1 : 0;
 	//// �^����������
	str = m_aryRecordLimit.GetAt(m_idxRecordLimit);
	dw = (DWORD)_ttoi(str);
	dw *= 60;	// min -> sec
	m_rgRecordLimit = dw;
 	//// monoral or stereo�ݒ�
	// ���W�I�{�^���̎Q�ƒ�`�iDDX�͈����ɂ����̂Łj
	// �i�N���X�����o�[�Ő錾����Ɛ���ɓ��삵�Ȃ��j
	CButton *rd_monoral = (CButton *)GetDlgItem(IDC_RDO_MONORAL);
	CButton *rd_stereo2 = (CButton *)GetDlgItem(IDC_RDO_STEREO2);
	if (rd_monoral->GetCheck()) {
		m_rgStereo = 0;
	} else if (rd_stereo2->GetCheck()) {
		m_rgStereo = 2;
	} else {
		m_rgStereo = 1;
	}
 	//// WAV or MP3
	CButton *rd_mp3 = (CButton *)GetDlgItem(IDC_RDO_MP3);
	if (rd_mp3->GetCheck()) {
		m_rgFormatType = 1;
	} else {
		m_rgFormatType = 0;
	}
 	//// MP3�R�[�f�b�N
	if (m_aryCodecName.GetCount() > 0) {
		str = m_aryCodecName.GetAt(m_idxCodecName);
		m_rgCodecName = str;
	}
 	//// MP3�t�H�[�}�b�g
	if (m_aryFormatName.GetCount() > 0) {
		str = m_aryFormatName.GetAt(m_idxFormatName);
		m_rgFormatName = str;
	}
 	//// �����^��
	m_rgEnableAuto = (m_blEnableAuto) ? 1 : 0;
	//// ���ʒ���
	m_rgVolSnd = (DWORD)_ttoi(m_strVolSnd);
	m_rgVolRcv = (DWORD)_ttoi(m_strVolRcv);
}


BOOL CSettingRecTab::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	SHORT rc;
	CWnd *cWnd;

    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		/// TAB KEY : ��ʃt�H�[���փt�H�[�J�X�J��
		/// TAB+Ctrl KEY : �R���g���[���^�u�J��
		if (nVirtKey == VK_TAB) {
			cWnd = GetFocus();
			rc = ::GetKeyState(VK_CONTROL);
			if( ( rc & 0x8000 ) != 0 ) {
				cWnd = cWnd->GetParentOwner();
				CSettingDialog *pDlg = (CSettingDialog *)cWnd;
				CTabCtrlSSL *pTabCtrl = &(pDlg->m_tabCtrl);
				int pageid = pTabCtrl->GetSSLActivePage();
				int idx = pTabCtrl->GetSSLPageIndex(pageid);
				int idxMax = pTabCtrl->GetSSLPageCount();
				idx = (idx == (idxMax-1)) ? 0 : idx+1;
				pTabCtrl->ActivateSSLPage(idx);
				return TRUE;
			} else {
				//// �t�H�[�J�X�Ō�̃R���g���[���̂ݗL��
				if (cWnd->m_hWnd == GetDlgItem(IDC_CMB_FORMAT)->m_hWnd) {
					cWnd = cWnd->GetParentOwner();
					cWnd->SetFocus();
					return TRUE;
				}
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
