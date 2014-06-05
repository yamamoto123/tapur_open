// SettingAdminTab.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "SettingAdminTab.h"
#include "SettingDialog.h"
#include "FileSpec.h"
#include "FolderDialog.h"
#include "SetHotkey.h"
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
// CSettingAdminTab �_�C�A���O


CSettingAdminTab::CSettingAdminTab(CWnd* pParent /*=NULL*/)
	: CTabPageSSL (CSettingAdminTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingAdminTab)
	m_blHotKey = TRUE;
	m_blEnableLog = TRUE;
	m_blDataPath = TRUE;
	//}}AFX_DATA_INIT
}


void CSettingAdminTab::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingAdminTab)
    DDX_Control(pDX, IDC_DATAPATH, m_edtDataPath);
    DDX_Control(pDX, IDC_BROWSE_DATAPATH, m_btnDataPath);
    DDX_Text(pDX, IDC_EDIT_SKYPE_PORT_OUT, m_strSkypePortOut);
    DDX_Text(pDX, IDC_EDIT_SKYPE_PORT_MIC, m_strSkypePortMic);
	DDX_Check(pDX, IDC_CHK_DATAPATH, m_blDataPath);
	DDX_Check(pDX, IDC_CHK_HOTKEY, m_blHotKey);
	DDX_Check(pDX, IDC_CHK_LOG, m_blEnableLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingAdminTab, CDialog)
	//{{AFX_MSG_MAP(CSettingAdminTab)
	ON_BN_CLICKED(IDC_BTN_HOTKEY, OnBtnHotkey)
	ON_BN_CLICKED(IDC_BROWSE_DATAPATH, OnBrowseDataPath)
	ON_BN_CLICKED(IDC_CHK_DATAPATH, OnChgDataPath)
	ON_BN_CLICKED(IDC_CHK_LOG, OnChgForm)
	ON_EN_CHANGE(IDC_EDIT_SKYPE_PORT_OUT, OnChgSkypePortOut)
	ON_EN_CHANGE(IDC_EDIT_SKYPE_PORT_MIC, OnChgSkypePortMic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingAdminTab ���b�Z�[�W �n���h��

BOOL CSettingAdminTab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	CString str;
	DWORD dw;

	/// ���W�X�g���ǂݍ���
	m_rgDataPath = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\DataPath"), _T(""), TRUE);
	m_rgEnableDataPath = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableDataPath"), 0, TRUE);
	m_rgEnableHotKey = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableHotKey"), 1, TRUE);
	m_rgEnableLog = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableLog"), 1, TRUE);
	m_rgSkypePortOut = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortOut"), 42805, TRUE);
	m_rgSkypePortMic = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortMic"), 42806, TRUE);

	/// Hotkey
	dw = m_rgEnableHotKey;
	m_blHotKey = (dw == 1) ? TRUE : FALSE;
	/// �f�[�^�p�X
	dw = m_rgEnableDataPath;
	m_blDataPath = (dw == 1) ? TRUE : FALSE;
	str = m_rgDataPath;
	m_edtDataPath.SetWindowText(str);
	m_btnDataPath.EnableWindow(m_blDataPath);
	m_edtDataPath.EnableWindow(m_blDataPath);
	/// ���O
	dw = m_rgEnableLog;
	m_blEnableLog = (dw == 1) ? TRUE : FALSE;
	/// SKYPE PORT
	dw = m_rgSkypePortOut;
	m_strSkypePortOut.Format(_T("%d"), dw);
	dw = m_rgSkypePortMic;
	m_strSkypePortMic.Format(_T("%d"), dw);

	UpdateData(FALSE);	// DDX�X�V
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// �t�H�[�����̓C�x���g
void CSettingAdminTab::OnChgForm() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	/// �K�p�{�^���̗L����
	CWnd *cWnd;
	cWnd = GetParentOwner();
	CSettingDialog *pDlg = (CSettingDialog *)cWnd;
	pDlg->EnableApply();
}

// �V���[�g�J�b�g�ڍאݒ�{�^��
void CSettingAdminTab::OnBtnHotkey() 
{
	UpdateData(TRUE);	// DDX�X�V

	static CSetHotkey dlg;

	if (dlg.m_hWnd == NULL) {
		dlg.Create(IDD_HOTKEY, this);
	} else {
		if (dlg.IsWindowVisible()) {
			dlg.SetFocus();
			return;
		}
	}

	dlg.ShowWindow(SW_SHOW);
}

// �f�[�^�t�H���_�ύX�`�F�b�N�C�x���g
void CSettingAdminTab::OnChgDataPath() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	UpdateData(TRUE);	// DDX�X�V

	// �f�[�^�t�H���_�ύX�{�^���ƃe�L�X�g�{�b�N�X�̗L���E����
	if (m_blDataPath) {
		m_btnDataPath.EnableWindow(TRUE);
		m_edtDataPath.EnableWindow(TRUE);
	} else {
		m_btnDataPath.EnableWindow(FALSE);
		m_edtDataPath.EnableWindow(FALSE);
	}

	UpdateData(FALSE);	// DDX�X�V

	/// �K�p�{�^���̗L����
	OnChgForm();
}

// SKYPE OUPUT PORT�ύX�`�F�b�N�C�x���g
void CSettingAdminTab::OnChgSkypePortOut() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CString strOld = m_strSkypePortOut;
	UpdateData(TRUE);	// DDX�X�V

	// �����̂ݗL��
	if (!ChkNaturalNumber(m_strSkypePortOut)) {
		m_strSkypePortOut = strOld;
		UpdateData(FALSE);	// DDX�X�V
		return;
	}

	UpdateData(FALSE);	// DDX�X�V

	/// �K�p�{�^���̗L����
	OnChgForm();
}
// SKYPE MIC PORT�ύX�`�F�b�N�C�x���g
void CSettingAdminTab::OnChgSkypePortMic() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CString strOld = m_strSkypePortMic;
	UpdateData(TRUE);	// DDX�X�V

	// �����̂ݗL��
	if (!ChkNaturalNumber(m_strSkypePortMic)) {
		m_strSkypePortMic = strOld;
		UpdateData(FALSE);	// DDX�X�V
		return;
	}

	UpdateData(FALSE);	// DDX�X�V

	/// �K�p�{�^���̗L����
	OnChgForm();
}

// �f�[�^�t�H���_�ύX�{�^��
void CSettingAdminTab::OnBrowseDataPath() 
{
	UpdateData(TRUE);	// DDX�X�V

	CString str;

	// �t�H���_�I��
	CFolderDialog dlg;
	CFileSpec fs(FS_MYDOC);	// �f�t�H���g�t�H���_:My Document
	CString strFolder = fs.GetFullSpec();
	dlg.SetDefaultFolder((LPCTSTR)strFolder);
	// ���R�[�h�ɕۑ����邽�߃T�C�Y�������K�v
	
	if(dlg.DoModal())
	{
		strFolder = dlg.GetFolderPath();
		if (strFolder.GetLength() > (MAX_PATH - MAX_FILE)) {
			str.LoadString(IDS_MSGBOX121);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return;
		}

		// �f�[�^�t�H���_�ύX
		m_edtDataPath.SetWindowText(strFolder);

		UpdateData(FALSE);	// DDX�X�V

		/// �K�p�{�^���̗L����
		OnChgForm();
	}
}

/////////////////////////////////////////////////////////////////////////////
// public function

//
//  �@�\     : ���W�X�g���ۑ��O�̃`�F�b�N
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CSettingAdminTab::Check() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	CString str;
	CString strMsg;
	BOOL blErr = FALSE;

	UpdateData(TRUE);	// DDX�X�V

	//// �f�[�^�t�H���_
	if (m_blDataPath) {
		m_edtDataPath.GetWindowText(str);
		if (str.GetLength() == 0)	blErr = TRUE;
		CFileSpec fs(str);
		if (!fs.Exist())	blErr = TRUE;
		if (!fs.IsFolder())	blErr = TRUE;
		if (blErr) {
			strMsg = _T("[");
			str.LoadString(IDS_TAB_ADMIN);
			strMsg += str + _T("]-[");
			GetDlgItemText(IDC_FRM_MSG_FOLDER, str);
			strMsg += str + _T("]");
			str.LoadString(IDS_TAB_CHECK);
			strMsg += str;
			AfxMessageBox(strMsg, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return FALSE;
		}
	}

	return TRUE;
}


//
//  �@�\     : ���W�X�g���֕ۑ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
void CSettingAdminTab::Save() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	CString str;

	UpdateData(TRUE);	// DDX�X�V

	/// ���W�X�g���X�V
	//// Hotkey�L��/����
	m_rgEnableHotKey = (m_blHotKey) ? 1 : 0;
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	// ��U�������čĐݒ�
	pFrame->UnregHotKey();
	if (m_blHotKey) {
		pFrame->RegHotKey();
	}
	//// �f�[�^�t�H���_�ύX
	m_rgEnableDataPath = (m_blDataPath) ? 1 : 0;
	m_edtDataPath.GetWindowText(str);
	m_rgDataPath = str;
	//// ���O�N��������
	m_rgEnableLog = (m_blEnableLog) ? 1 : 0;
	theApp.m_intEnableLog = (m_blEnableLog) ? 1 : 0;
	/// SKYPE PORT
	m_rgSkypePortOut = (DWORD)_ttoi(m_strSkypePortOut);
	m_rgSkypePortMic = (DWORD)_ttoi(m_strSkypePortMic);
}

