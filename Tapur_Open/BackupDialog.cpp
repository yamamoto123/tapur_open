// BackupDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "BackupDialog.h"
#include "FileSpec.h"
#include "FolderDialog.h"
#include "TPtable.h"
#include "TPsetup.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackupDialog �_�C�A���O


CBackupDialog::CBackupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBackupDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBackupDialog)
	//}}AFX_DATA_INIT

}

void CBackupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBackupDialog)
    DDX_Control(pDX, IDC_EDIT_BACKUP, m_edtBackupPath);
    DDX_Control(pDX, IDC_EDIT_RESTORE, m_edtRestorePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBackupDialog, CDialog)
	//{{AFX_MSG_MAP(CBackupDialog)
	ON_BN_CLICKED(IDC_BROWSE_BACKUP, OnBrowseBackup)
	ON_BN_CLICKED(IDC_BROWSE_RESTORE, OnBrowseRestore)
	ON_BN_CLICKED(IDC_BTN_BACKUP, OnBtnBackup)
	ON_BN_CLICKED(IDC_BTN_RESTORE, OnBtnRestore)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackupDialog ���b�Z�[�W �n���h��


BOOL CBackupDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// �o�b�N�A�b�v��t�H���_�w��{�^��
void CBackupDialog::OnBrowseBackup() 
{
	UpdateData(TRUE);	// DDX�X�V

	CString str;

	// �t�H���_�I��
	CFolderDialog dlg;
	CFileSpec fs(FS_MYDOC);	// �f�t�H���g�t�H���_:My Document
	CString strFolder = fs.GetFullSpec();
	dlg.SetDefaultFolder((LPCTSTR)strFolder);
	
	if(dlg.DoModal())
	{
		strFolder = dlg.GetFolderPath();

		// �f�[�^�t�H���_�ύX
		m_edtBackupPath.SetWindowText(strFolder);

		UpdateData(FALSE);	// DDX�X�V
	}
}

// ���X�g�A����t�H���_�w��{�^��
void CBackupDialog::OnBrowseRestore() 
{
	UpdateData(TRUE);	// DDX�X�V

	CString str;

	// �t�H���_�I��
	CFolderDialog dlg;
	CFileSpec fs(FS_MYDOC);	// �f�t�H���g�t�H���_:My Document
	CString strFolder = fs.GetFullSpec();
	dlg.SetDefaultFolder((LPCTSTR)strFolder);
	
	if(dlg.DoModal())
	{
		strFolder = dlg.GetFolderPath();

		// �f�[�^�t�H���_�ύX
		m_edtRestorePath.SetWindowText(strFolder);

		UpdateData(FALSE);	// DDX�X�V
	}
}

//	�o�b�N�A�b�v�������s�{�^��
void CBackupDialog::OnBtnBackup() 
{
	UpdateData(TRUE);	// DDX�X�V

	CString str;
	CTPsetup tps;
	CString strOrg;
	CString strNew;
	CFileFind finder;

	// �o�b�N�A�b�v��t�H���_
	CString strFolder;
	m_edtBackupPath.GetWindowText(strFolder);
	if (strFolder.IsEmpty()) return;
	CFileSpec fs(strFolder);
	if (!fs.Exist()) return;

	str.LoadString(IDS_MSGBOX911);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return;

	// �o�b�N�A�b�v����
	// �K��̃f�[�^���R�s�[
	fs.SetFullSpec(theApp.m_strDataFolder);	// user data folder
	CString strOrgFolder = fs.GetFolder();	// data folder
	fs.SetFullSpec(strOrgFolder);
	strFolder += _T("\\tapur.backup");
	CFileSpec fs_out(strFolder);
	if (fs_out.Exist()) {
		if (!fs_out.FileDelete(TRUE, this->m_hWnd)) goto PROC_CANCEL;
	}
	if (!fs.FileCopy(strFolder, TRUE, TRUE, this->m_hWnd)) goto PROC_CANCEL;

	// ���[�U�w��p�X�̃f�[�^���R�s�[�i�R�s�[�ς݂͏��O�j
	BOOL bWorking = finder.FindFile(strFolder + _T("\\*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory()) continue;
		str = finder.GetFileName();
		if (str == _T(".") || str == _T("..")) continue;

		CString strTbl = finder.GetFilePath() + _T("\\") + _T(DBFILE_NAME);
		CTblRdfl tbl(strTbl);
		long lngCnt = tbl.SelectRec(_T("lngSystemFlag>=0 order by lngSystemNew desc"));	// all
		while (lngCnt > 0)
		{
			tps.SetCallerId(tbl.strSenderId);
			str = strFolder + _T("\\") + tbl.strReceiverId;
			// wav file
			strOrg = tps.FindRdfWavFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
			strNew = tps.FindRdfWavFileName(str, tbl.dlngSystemNew, _T("$$"));
			if (!strOrg.IsEmpty() && strNew.IsEmpty()) {
				fs.SetFullSpec(strOrg);
				strNew = tps.RdfWavFileName(str + _T(RDF_WAV_PATH), tbl.dlngSystemNew, _T("$$"));
				if (!fs.FileCopy(strNew, TRUE, TRUE, this->m_hWnd)) goto PROC_CANCEL;
			}
			// mp3 file
			strOrg = tps.FindRdfMp3FileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
			strNew = tps.FindRdfMp3FileName(str, tbl.dlngSystemNew, _T("$$"));
			if (!strOrg.IsEmpty() && strNew.IsEmpty()) {
				fs.SetFullSpec(strOrg);
				strNew = tps.RdfMp3FileName(str + _T(RDF_MP3_PATH), tbl.dlngSystemNew, _T("$$"));
				if (!fs.FileCopy(strNew, TRUE, TRUE, this->m_hWnd)) goto PROC_CANCEL;
			}
			// avi file
			strOrg = tps.FindRdfVideoFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
			strNew = tps.FindRdfVideoFileName(str, tbl.dlngSystemNew, _T("$$"));
			if (!strOrg.IsEmpty() && strNew.IsEmpty()) {
				fs.SetFullSpec(strOrg);
				strNew = tps.RdfVideoFileName(str + _T(RDF_AVI_PATH), tbl.dlngSystemNew, _T("$$"));
				if (!fs.FileCopy(strNew, TRUE, TRUE, this->m_hWnd)) goto PROC_CANCEL;
			}

			if (!tbl.NextRec()) break;
			--lngCnt;
		}
	} 

	str.LoadString(IDS_MSGBOX917);
	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
	return;
PROC_CANCEL:
	str.LoadString(IDS_MSGBOX919);
	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
}

//	���X�g�A�������s�{�^��
void CBackupDialog::OnBtnRestore() 
{
	UpdateData(TRUE);	// DDX�X�V

	CString str;
	CFileFind finder;

	// ���X�g�A�f�[�^�t�H���_
	BOOL blFind = FALSE;
	CString strFolder;
	m_edtRestorePath.GetWindowText(strFolder);
	if (strFolder.IsEmpty()) return;
	CFileSpec fs(strFolder);
	if (fs.Exist()) {
		if (fs.GetFileName() == _T("tapur.backup")) {
			blFind = TRUE;
		} else {
			fs.SetFullSpec(strFolder + _T("\\tapur.backup"));
			if (fs.Exist()) {
				strFolder += _T("\\tapur.backup");
				blFind = TRUE;
			}
		}
	}
	if (!blFind) {
		str.LoadString(IDS_MSGBOX913);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return;
	}

	str.LoadString(IDS_MSGBOX912);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return;

	// ���X�g�A����
	CFileSpec fs_out(theApp.m_strDataFolder);	// user data folder
	CString strOrgFolder = fs_out.GetFolder();	// data folder
	fs_out.SetFullSpec(strOrgFolder);
	if (fs_out.Exist()) {
		if (!fs_out.FileDelete(TRUE, this->m_hWnd)) goto PROC_CANCEL;
	}
	if (!fs.FileCopy(strOrgFolder, TRUE, TRUE, this->m_hWnd)) goto PROC_CANCEL;

	// �f�[�^�x�[�X�̏�������
	BOOL bWorking = finder.FindFile(strOrgFolder + _T("\\*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory()) continue;
		str = finder.GetFileName();
		if (str == _T(".") || str == _T("..")) continue;

		CString strTbl = finder.GetFilePath() + _T("\\") + _T(DBFILE_NAME);
		CTblRdfl tbl(strTbl);
		CString strWhere(_T("lngSystemFlag>=0"));
		long lngCnt = tbl.CountRec(strWhere);	// all
		if (lngCnt == 0) continue;

		CString strSet = _T("strRemarks1='") + finder.GetFilePath() + _T("'");
		if (!tbl.UpdateRec(strSet, strWhere)) goto PROC_CANCEL;
	} 

	str.LoadString(IDS_MSGBOX918);
	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
	return;
PROC_CANCEL:
	str.LoadString(IDS_MSGBOX920);
	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
}


void CBackupDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function

