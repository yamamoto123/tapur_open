
// CnvVideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tapur.h"
#include "FileSpec.h"
#include "TPerror.h"
#include "TPsetup.h"
#include "CnvVideoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TARGET_FILTER_TEXT \
   _T("Video files (*.avi)|*.avi|")\
_T("All Files (*.*)|*.*;|")
//#define TARGET_FILTER_TEXT \
//   _T("Video files (*.avi; *.wmv; *.mpg)|*.avi; *.wmv; *.mpg|")\
//_T("All Files (*.*)|*.*;|")


/////////////////////////////////////////////////////////////////////////////
// CCnvVideoDlg dialog

CCnvVideoDlg::CCnvVideoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCnvVideoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCnvVideoDlg)
    m_blAudio = FALSE;		// ��������
	//}}AFX_DATA_INIT
	
	m_pAviManager = NULL;
}

void CCnvVideoDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CCnvVideoDlg)
    DDX_Check(pDX, IDC_CHK_AUDIO, m_blAudio);
	DDX_Text(pDX, IDC_TARGET, m_strTarget);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCnvVideoDlg, CDialog)
//{{AFX_MSG_MAP(CCnvVideoDlg)
	ON_BN_CLICKED(IDC_BROWSETARGET, OnBrowsetarget)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCnvVideoDlg message handlers

BOOL CCnvVideoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	if (m_pAviManager == NULL) {
		m_pAviManager = new CAviManager(this);
	}
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// ���̓t�@�C���̎w��
BOOL CCnvVideoDlg::SetSrcFile(CString strAViFile, CString strWavFile)
{
	CString str;

	m_strSrcFile = strAViFile;
	m_strAudioFile = strWavFile;


	if (m_strSrcFile.IsEmpty()) {
		/// �o�̓t�@�C������\��
		m_strTarget.Empty();
	} else {
		/// �\�[�X�t�@�C�����\��
		CFileSpec fs(m_strSrcFile);
		CStatic *ptxt;
		ptxt = (CStatic*)GetDlgItem(IDC_STATIC_SRC);
		ptxt->SetWindowText(fs.GetFileName());

		/// �o�̓t�@�C���������l
		fs.SetFullSpec(FS_MYDOC);	// My Document
		CString strFile = fs.GetFullSpec() + _T("\\Tapur\\Video\\");
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
	
	UpdateData(FALSE);	// DDX�X�V

	/// �o�̓t�@�C�����E�񂹕\��
	CEdit *pEdit;
	pEdit = (CEdit*)GetDlgItem(IDC_TARGET);
	pEdit->SetSel(0,-1);

	return TRUE;  
}

void CCnvVideoDlg::OnBrowsetarget() 
{
	UpdateData(TRUE);	// DDX�X�V

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
// �f�t�H���g�g���q�͊g���q��\�����[�h�ł͐���ɓ��삵�Ȃ��ixx.wmv.avi �̂悤�ɂȂ��Ă��܂��j
//   dlg.m_ofn.lpstrDefExt = _T("avi");

	if(dlg.DoModal() == IDOK)  
	{
		CString strFile = dlg.GetPathName();
		if (strFile.IsEmpty()) return;

		fs.SetFullSpec(strFile);
		CString strExt = fs.Extension().MakeLower();
		if (strExt != _T(".avi")) {
			strFile += _T(".avi");
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

void CCnvVideoDlg::OnOK() 
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
	if (strExt.IsEmpty() || strExt.MakeLower() != _T(".avi"))	m_strDstFile += _T(".avi");
	if (CheckTargetFile(m_strDstFile) == FALSE) return;	// �o�̓t�@�C���m�F
	if (fs.Exist()) fs.FileDelete();	// �����t�@�C���폜
	CnvAviAvi();

	CDialog::OnOK();
}

void CCnvVideoDlg::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function

BOOL CCnvVideoDlg::CheckTargetFile(CString &sTargetFile)
{
	CString str;
	CFileSpec fs;

	if (sTargetFile.GetLength() <= 1) {
		str.LoadString(IDS_MSGBOX81);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	/// �p�X�����w��̏ꍇ�AMyDocument���g�p
	if (sTargetFile.Find(_T("\\"), 0) < 0) {
		fs.SetFullSpec(FS_MYDOC);	// My Document
		sTargetFile = fs.GetFullSpec() + _T("\\") + sTargetFile;
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

// Avi->Avi
BOOL CCnvVideoDlg::CnvAviAvi()
{
	BOOL blRet = TRUE;
	CString str;
	CFileSpec fs;
	CString strTmpFile;

	fs.SetFullSpec(m_strSrcFile);
	if (!fs.Exist()) return FALSE;

	if (m_blAudio) {
	/// �����t�@�C���ƌ���
		// ���̓t�@�C���T�C�Y�`�F�b�N�i1G Max�j
		//fs.SetFullSpec(m_strSrcFile);
		//if (fs.FileSize() > 1000*1000*1000) {
		//	str.LoadString(IDS_MSGBOX65);
		//	str += "(1GByte)";
		//	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		//	return FALSE;
		//}

		//// ���ɉ����t���`�F�b�N
		if (m_pAviManager->ChkAudio(m_strSrcFile)) {
			str.LoadString(IDS_MSGBOX93);
			if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return TRUE;
		}

		fs.SetFullSpec(m_strAudioFile);
		if (fs.Extension().MakeLower() == _T(".mp3")) {
		//// MP3�̏ꍇ�AWAV�ɕϊ����Č���
			// TMP�t�@�C�����o�R���ĕϊ�
			CTPsetup tps(theApp.m_strDataFolder);
			strTmpFile = tps.RdfTmpAudioFileName();

			// MP3->WAV
			CAcmMgr oAcmMgr;
			blRet = oAcmMgr.ConvertMp3ToWav(this, m_strAudioFile, strTmpFile, TRUE);
			if (blRet) m_strAudioFile = strTmpFile;
		}

		// ���[�_���Ŏ��s
		m_pAviManager->SetMove(FALSE);
		blRet = m_pAviManager->CopyAviFileDs(m_strDstFile, m_strSrcFile, m_strAudioFile);

		// TMP�t�@�C�����폜
		fs.SetFullSpec(strTmpFile);
		if (fs.Exist()) {
			fs.FileDelete();
		}
	} else {
	/// �����Ȃ������̂܂܃R�s�[
		BeginWaitCursor();
		blRet = fs.FileCopy(m_strDstFile, TRUE, TRUE, this->m_hWnd);
		EndWaitCursor();
	}
	if (blRet) {
		str.LoadString(IDS_MSGBOX89);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
	} else {
		str.LoadString(IDS_MSGBOX88);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
	}

	return blRet;
}


