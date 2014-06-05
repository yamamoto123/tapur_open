// CnvAudioDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tapur.h"
#include "CnvAudioDlg.h"
#include "FileSpec.h"
#include "OggVorbis.h"
#include "TPerror.h"
#include "TPsetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CODEC_FILTER_TEXT	_T("MPEG LAYER-3")

#define TARGET_FILTER_TEXT \
   _T("Audio files (*.wav; *.ogg; *.mp3)|*.wav; *.ogg; *.mp3|")\
_T("All Files (*.*)|*.*;|")

/////////////////////////////////////////////////////////////////////////////
// CCnvAudioDlg dialog

CCnvAudioDlg::CCnvAudioDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCnvAudioDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCnvAudioDlg)
	m_idxCodecName = 0;
	m_idxFormatName = 0;
	m_strTarget.Empty();
	//}}AFX_DATA_INIT
}

void CCnvAudioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCnvAudioDlg)
	DDX_Text(pDX, IDC_TARGET, m_strTarget);
	DDX_CBIndex(pDX, IDC_CMB_CODEC, m_idxCodecName);
	DDX_CBIndex(pDX, IDC_CMB_FORMAT, m_idxFormatName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCnvAudioDlg, CDialog)
	//{{AFX_MSG_MAP(CCnvAudioDlg)
	ON_BN_CLICKED(IDC_BROWSETARGET, OnBrowsetarget)
	ON_BN_CLICKED(IDC_RDO_WAV, OnRdoType)
	ON_BN_CLICKED(IDC_RDO_MP3, OnRdoType)
	ON_BN_CLICKED(IDC_RDO_OGG, OnRdoType)
	ON_CBN_SELCHANGE(IDC_CMB_CODEC, OnCmbCodec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCnvAudioDlg message handlers

BOOL CCnvAudioDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// ���̓t�@�C���̎w��
BOOL CCnvAudioDlg::SetSrcFile(CString strFile)
{
	CString str;
	CComboBox *pcmb;
	CButton *rdb;
	int i;
	int intCnt;

	m_strSrcFile = strFile;

	if (m_strSrcFile.IsEmpty()) {
		/// �o�̓t�@�C������\��
		m_strTarget.Empty();
		return TRUE;
	} else {
		/// �\�[�X�t�@�C�����\��
		CFileSpec fs(m_strSrcFile);
		CStatic *ptxt;
		ptxt = (CStatic*)GetDlgItem(IDC_STATIC_SRC);
		ptxt->SetWindowText(fs.GetFileName());

		/// �o�̓t�@�C���������l
		fs.SetFullSpec(FS_MYDOC);	// My Document
		CString strFile = fs.GetFullSpec() + _T("\\Tapur\\Voice\\");
		if (m_strSrcFile.IsEmpty()) {
			strFile.Empty();
		} else {
			fs.SetFullSpec(m_strSrcFile);
			str = fs.FileName();
			strFile += str.Mid(_tcsclen(_T("rdf-")), str.GetLength() - _tcsclen(_T("rdf-")) - _tcsclen(_T("-$$")));
			strFile += fs.Extension().MakeLower();
			fs.SetFullSpec(strFile);
			fs.MkFolder();
		}
		m_strTarget = strFile;	// �o�̓t�@�C����
	}

 	/// MP3�R�[�f�b�N/�t�H�[�}�b�g�R���{�̏������
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_CODEC);
	pcmb->EnableWindow(FALSE);
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_FORMAT);
	pcmb->EnableWindow(FALSE);

	if (m_strSrcFile.MakeLower().Right(4) == _T(".mp3")) {
	/// �\�[�X�t�@�C����MP3�̏ꍇ
		//// �R���g���[���̐���
		rdb = (CButton *)GetDlgItem(IDC_RDO_WAV);
		rdb->SetCheck(0);
		rdb = (CButton *)GetDlgItem(IDC_RDO_OGG);
		rdb->SetCheck(0);
		rdb = (CButton *)GetDlgItem(IDC_RDO_MP3);
		rdb->SetCheck(1);
	} else {
	/// �\�[�X�t�@�C����WAV�̏ꍇ
		//// �R���g���[���̐���
		rdb = (CButton *)GetDlgItem(IDC_RDO_MP3);
		rdb->SetCheck(0);
		rdb = (CButton *)GetDlgItem(IDC_RDO_OGG);
		rdb->SetCheck(0);
		rdb = (CButton *)GetDlgItem(IDC_RDO_WAV);
		rdb->SetCheck(1);
		//// �t�H�[�}�b�g�̊m�F
		if (!m_oAcmMgr.GetFileFormat(m_strSrcFile, &m_wfx)) return TRUE;
		//// ���W�X�g���ǂݍ���
		CRegString rgCodecName;		// MP3�R�[�f�b�N����
		CRegString rgFormatName;	// MP3�t�H�[�}�b�g����
		CString strCodecName;		// MP3�R�[�f�b�N����
		CString strFormatName;		// MP3�t�H�[�}�b�g����
		if (m_wfx.nChannels == 1) {
			// mono:����^�p�ݒ�𗘗p(�Ȃ���Θ^���p)
			rgCodecName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordMP3Codec"), _T(""), TRUE);
			strCodecName = rgCodecName;
			rgFormatName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordMP3Format"), _T(""), TRUE);
			strFormatName = rgFormatName;
			if (strCodecName.IsEmpty()) {
				rgCodecName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordMP3Codec"), _T(""), TRUE);
				strCodecName = rgCodecName;
			}
			if (strFormatName.IsEmpty()) {
				rgFormatName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordMP3Format"), _T(""), TRUE);
				strFormatName = rgFormatName;
			}
		} else {
			// stereo:�^���p�ݒ�𗘗p(�Ȃ���Η���^�p)
			rgCodecName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordMP3Codec"), _T(""), TRUE);
			strCodecName = rgCodecName;
			rgFormatName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordMP3Format"), _T(""), TRUE);
			strFormatName = rgFormatName;
			if (strCodecName.IsEmpty()) {
				rgCodecName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordMP3Codec"), _T(""), TRUE);
				strCodecName = rgCodecName;
			}
			if (strFormatName.IsEmpty()) {
				rgFormatName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordMP3Format"), _T(""), TRUE);
				strFormatName = rgFormatName;
			}
		}

 		/// MP3�R�[�f�b�N�R���{�{�b�N�X����
		pcmb = (CComboBox*)GetDlgItem(IDC_CMB_CODEC);
		pcmb->ResetContent();
		intCnt = m_oAcmMgr.GetDriverList(m_aryCodecName);
		if (intCnt > 0) {
			for(i = 0; i < intCnt; i++) {
				pcmb->AddString(m_aryCodecName.GetAt(i));
			}

			/// MP3�R�[�f�b�N�R���{�{�b�N�X��I��
			m_idxCodecName = 0;
			for(i = 0; i < m_aryCodecName.GetSize(); i++) {
				if (strCodecName == m_aryCodecName.GetAt(i)) {
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

 			/// MP3�t�H�[�}�b�g�R���{�{�b�N�X����
			pcmb = (CComboBox*)GetDlgItem(IDC_CMB_FORMAT);
			pcmb->ResetContent();
			intCnt = m_oAcmMgr.GetFormatList(m_aryFormatName, &m_wfx);
			if (intCnt > 0) {
				for(i = 0; i < intCnt; i++) {
					pcmb->AddString(m_aryFormatName.GetAt(i));
				}

 				/// MP3�t�H�[�}�b�g�R���{�{�b�N�X��I��
				m_idxFormatName = 0;
				for(i = 0; i <  m_aryFormatName.GetSize(); i++) {
					if (strFormatName == m_aryFormatName.GetAt(i)) {
						m_idxFormatName = i;
						break;
					}
				}
			}
		} else {
			//// MP3�R�[�f�b�N�����݂��Ȃ��ꍇ�AMP3�͎g�p�s��
			rdb = (CButton *)GetDlgItem(IDC_RDO_MP3);
			rdb->EnableWindow(0);
		}
	}
	
	UpdateData(FALSE);	// DDX�X�V

	/// �o�̓t�@�C�����E�񂹕\��
	CEdit *pEdit;
	pEdit = (CEdit*)GetDlgItem(IDC_TARGET);
	pEdit->SetSel(0,-1);

	return TRUE;
}

// �o�̓t�@�C���Q��
void CCnvAudioDlg::OnBrowsetarget() 
{
	UpdateData(TRUE);	// DDX�X�V

	CString str;

	CFileDialog dlg(FALSE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_CREATEPROMPT, TARGET_FILTER_TEXT, this);

	/// �f�t�H���g�p�X�̎w��
	CFileSpec fs(FS_MYDOC);	// My Document
	_TCHAR lpstrPath[MAX_PATH];                
	if (m_strTarget.IsEmpty()) {
		_tcscpy_s(lpstrPath, MAX_PATH, fs.GetFullSpec());
		dlg.m_ofn.lpstrInitialDir = lpstrPath;	// CString�͎󂯎��Ȃ�
		dlg.m_ofn.lpstrFile= NULL;
	} else {
		_tcscpy_s(lpstrPath, MAX_PATH, m_strTarget);
		dlg.m_ofn.lpstrInitialDir = NULL;	
		dlg.m_ofn.lpstrFile= lpstrPath;
	}
	//�f�t�H���g�g���q�͊g���q��\�����[�h�ł͐���ɓ��삵�Ȃ��ixx.ogg.wav �̂悤�ɂȂ��Ă��܂��j
	//dlg.m_ofn.lpstrDefExt = _T("wav");

	if(dlg.DoModal() == IDOK)  
	{
		CString strFile = dlg.GetPathName();
		if (strFile.IsEmpty()) return;

		CButton *rd_wav = (CButton *)GetDlgItem(IDC_RDO_WAV);
		CButton *rd_mp3 = (CButton *)GetDlgItem(IDC_RDO_MP3);
		CButton *rd_ogg = (CButton *)GetDlgItem(IDC_RDO_OGG);

		/// �o�̓t�@�C�����̊g���q����`���𔻒�
		fs.SetFullSpec(strFile);
		str = fs.Extension().MakeLower();
		if (str == _T(".mp3")) {
			rd_wav->SetCheck(0);
			rd_mp3->SetCheck(1);
			rd_ogg->SetCheck(0);
		} else if (str == _T(".ogg")) {
			rd_wav->SetCheck(0);
			rd_mp3->SetCheck(0);
			rd_ogg->SetCheck(1);
		} else if (str == _T(".wav")) {
			rd_wav->SetCheck(1);
			rd_mp3->SetCheck(0);
			rd_ogg->SetCheck(0);
		} else {
			/// ���W�I�{�^������`���𔻒�
			if (rd_mp3->GetCheck()) {
				strFile += _T(".mp3");
			} else if (rd_ogg->GetCheck()) {
				strFile += _T(".ogg");
			} else {
				strFile += _T(".wav");
			}
		}

		/// �t�H�[���ɏo�̓t�@�C���ݒ�
		m_strTarget = strFile;
	}

	UpdateData(FALSE);	// DDX�X�V

	/// �o�̓t�@�C�����E�񂹕\��
	CEdit *pEdit;
	pEdit = (CEdit*)GetDlgItem(IDC_TARGET);
	pEdit->SetSel(0,-1);
}

// �^���`���̕ύX��Codec/Foramt���X�g�L��
void CCnvAudioDlg::OnRdoType() 
{
	UpdateData(TRUE);	// DDX�X�V

	CComboBox *pcmb;
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_CODEC);
	pcmb->EnableWindow(FALSE);
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_FORMAT);
	pcmb->EnableWindow(FALSE);

	/// WAV->MP3�ϊ��̎��̂�Codec/Foramt���X�g�L��
	if (m_strSrcFile.MakeLower().Right(4) == _T(".wav")) {
		CButton *rd_mp3 = (CButton *)GetDlgItem(IDC_RDO_MP3);
		if (rd_mp3->GetCheck()) {
			pcmb = (CComboBox*)GetDlgItem(IDC_CMB_CODEC);
			pcmb->EnableWindow(TRUE);
			pcmb = (CComboBox*)GetDlgItem(IDC_CMB_FORMAT);
			pcmb->EnableWindow(TRUE);

			/// FraunHoffer MP3�R�[�f�b�N���g�p�ł��Ȃ��ꍇ�iVista�j
			CString str = m_aryCodecName.GetAt(m_idxCodecName);
			str.MakeUpper();
			int intCnt = m_oAcmMgr.GetFormatList(m_aryFormatName, &m_wfx);
			if (intCnt == 0 && str.Find(ACM_CODEC_MP3) >= 0) {
				CAudio *pAudio = CAudio::Instance();
				pAudio->MP3CodecRegistry();
			}
		}
	}

	UpdateData(FALSE);	// DDX�X�V
}

// MP3�R�[�f�b�N�̕ύX���t�H�[�}�b�g���X�g�X�V
void CCnvAudioDlg::OnCmbCodec() 
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
}

void CCnvAudioDlg::OnOK() 
{
	UpdateData(TRUE);	// DDX�X�V

	CString str;
	CString strFile;
	CString strExt;
	CFileSpec fs;

	/// �\�[�X�t�@�C���m�F
	if (m_strTarget.GetLength() <= 0) {
		str.LoadString(IDS_MSGBOX81);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return;
	}

	/// �p�X�����w��̏ꍇ�AMyDocument���g�p
	if (m_strTarget.Find(_T("\\"), 0) < 0) {
		fs.SetFullSpec(FS_MYDOC);	// My Document
		m_strTarget = fs.GetFullSpec() + _T("\\") + m_strTarget;
	}
	m_strDstFile = m_strTarget;
	fs.SetFullSpec(m_strDstFile);
	strExt = fs.Extension();

	/// �o�͌`���ɂ�鏈������
	CButton *rd_mp3 = (CButton *)GetDlgItem(IDC_RDO_MP3);
	CButton *rd_ogg = (CButton *)GetDlgItem(IDC_RDO_OGG);
	/// ���W�I�{�^������`���𔻒�
	if (rd_mp3->GetCheck()) {
		if (strExt.IsEmpty() || strExt.MakeLower() != _T(".mp3"))	m_strDstFile += _T(".mp3");
		if (CheckTargetFile(m_strDstFile) == FALSE) return;	// �o�̓t�@�C���m�F
		if (m_strSrcFile.MakeLower().Right(4) == _T(".mp3")) {
			CopyFile();
		} else {
			CnvWavMp3();
		}
	} else if (rd_ogg->GetCheck()) {
		if (strExt.IsEmpty() || strExt.MakeLower() != _T(".ogg"))	m_strDstFile += _T(".ogg");
		if (CheckTargetFile(m_strDstFile) == FALSE) return;	// �o�̓t�@�C���m�F
		if (m_strSrcFile.MakeLower().Right(4) == _T(".mp3")) {
			CnvMp3Ogg();
		} else {
			CnvWavOgg();
		}
	} else {
		if (strExt.IsEmpty() || strExt.MakeLower() != _T(".wav"))	m_strDstFile += _T(".wav");
		if (CheckTargetFile(m_strDstFile) == FALSE) return;	// �o�̓t�@�C���m�F
		if (m_strSrcFile.MakeLower().Right(4) == _T(".mp3")) {
			CnvMp3Wav();
		} else {
			CopyFile();
		}
	}

	//UpdateData(FALSE);	// DDX�X�V

	CDialog::OnOK();
}

void CCnvAudioDlg::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	CDialog::OnCancel();
}


/////////////////////////////////////////////////////////////////////////////
// local function

// �o�̓t�@�C���̃`�F�b�N
BOOL CCnvAudioDlg::CheckTargetFile(CString sTargetFile)
{
	CString str;
	CFileSpec fs;

	if (sTargetFile.GetLength() <= 1) {
		str.LoadString(IDS_MSGBOX81);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	fs.SetFullSpec(sTargetFile);
	if( fs.Exist() ) {
		if( fs.IsFolder() ) {
			str.LoadString(IDS_MSGBOX82);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return FALSE;
		} else {
			str.LoadString(IDS_MSGBOX83);
			if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;
		}
	}

	return TRUE;
}

// �t�@�C���R�s�[
BOOL CCnvAudioDlg::CopyFile(BOOL blMsg)
{
	CFileSpec fs;

	fs.SetFullSpec(m_strSrcFile);
	if (!fs.Exist()) return FALSE;
	BeginWaitCursor();
	fs.FileCopy(m_strDstFile, TRUE);
	EndWaitCursor();

	if (blMsg) {
		CString str;
		str.LoadString(IDS_MSGBOX89);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
	}

	return TRUE;
}

// WAV->MP3�ϊ�
BOOL CCnvAudioDlg::CnvWavMp3(BOOL blMsg)
{
	CString str;
	BOOL blRet = FALSE;

	CString strCodec = m_aryCodecName.GetAt(m_idxCodecName);
	CString strFormat = m_aryFormatName.GetAt(m_idxFormatName);

	strCodec.MakeUpper();
	if (strCodec.Find(ACM_CODEC_MP3_LAME)) {
		blRet = m_oAcmMgr.ConvertWavToMp3(this, CAcmMgr::WAV_MP3_LAME, m_strSrcFile, m_strDstFile, strCodec, strFormat, TRUE);
	} else {
		blRet = m_oAcmMgr.ConvertWavToMp3(this, CAcmMgr::WAV_MP3, m_strSrcFile, m_strDstFile, strCodec, strFormat, TRUE);
	}

	if(blRet) {	// OK
		if (blMsg) {
			str.LoadString(IDS_MSGBOX89);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		}
	} else {	// cancel
		str.LoadString(IDS_MSGBOX88);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
	}

	return blRet;
}

// MP3->WAV�ϊ�
BOOL CCnvAudioDlg::CnvMp3Wav(BOOL blMsg)
{
	CString str;
	BOOL blRet = FALSE;

	blRet = m_oAcmMgr.ConvertMp3ToWav(this, m_strSrcFile, m_strDstFile, TRUE);

	if(blRet) {	// OK
		if (blMsg) {
			str.LoadString(IDS_MSGBOX89);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		}
	} else {	// cancel
		str.LoadString(IDS_MSGBOX88);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
	}

	return blRet;
}

// WAV->OGG�ϊ�
BOOL CCnvAudioDlg::CnvWavOgg(BOOL blMsg)
{
	CString str;

	CFileSpec fs(m_strSrcFile);
	if (!fs.Exist()) return FALSE;

	COggVorbis ogg;
	int ret = ogg.Encode(this, m_strSrcFile, m_strDstFile);
	if(ret == 1) {	// OK
		if (blMsg) {
			str.LoadString(IDS_MSGBOX89);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		}
	} else if(ret == 0) {	// cancel
		str.LoadString(IDS_MSGBOX88);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	return TRUE;
}

// MP3->OGG�ϊ�
BOOL CCnvAudioDlg::CnvMp3Ogg(BOOL blMsg)
{
	/// TMP�t�@�C�����o�R���ĕϊ�
	CTPsetup tps(theApp.m_strDataFolder);
	CString strTmpFile = tps.RsfTmpFileName(_T("wav"));

	/// MP3->WAV
	CString strBakFile = m_strDstFile;
	m_strDstFile = strTmpFile;
	BOOL blRet = CnvMp3Wav(FALSE);
	if (blRet) {
		m_strDstFile = strBakFile;

		/// WAV->OGG
		strBakFile = m_strSrcFile;
		m_strSrcFile = strTmpFile;
		blRet = CnvWavOgg(blMsg);
		m_strSrcFile = strBakFile;
	}

	/// TMP�t�@�C�����폜
	CFileSpec fs(strTmpFile);
	if (fs.Exist()) {
		fs.FileDelete();
	}

	return blRet;
}
