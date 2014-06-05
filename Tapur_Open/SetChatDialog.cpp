// SetChatDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Registry.h"
#include "SetChatDialog.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CSetChatDialog �_�C�A���O


CSetChatDialog::CSetChatDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSetChatDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetChatDialog)
	m_strChat = _T("");
	//}}AFX_DATA_INIT
}

CSetChatDialog::~CSetChatDialog()
{
}

void CSetChatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetChatDialog)
	DDX_Text(pDX, IDC_EDIT_CHAT, m_strChat);
	DDV_MaxChars(pDX, m_strChat, 400);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetChatDialog, CDialog)
	//{{AFX_MSG_MAP(CSetChatDialog)
	ON_BN_CLICKED(IDC_BTN_INIT, OnButtonInit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetChatDialog ���b�Z�[�W �n���h��

BOOL CSetChatDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	// ��ʍX�V
	UpdateForm();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/// �Ή����W�X�g���̏����l��\��
void CSetChatDialog::OnButtonInit() 
{
	if (m_strRegKey.IsEmpty()) return;

	if (m_strRegKey.Find(_T("VoiceRecordWarning")) > 0) {
		m_strChat.LoadString(IDS_TAB_REC_WARNING);
	} else if (m_strRegKey.Find(_T("VoiceRecordStopWarning")) > 0) {
		m_strChat.LoadString(IDS_TAB_RECSTOP_WARNING);
	} else if (m_strRegKey.Find(_T("VideoRecordWarning")) > 0) {
		m_strChat.LoadString(IDS_TAB_VREC_WARNING);
	} else if (m_strRegKey.Find(_T("VideoRecordStopWarning")) > 0) {
		m_strChat.LoadString(IDS_TAB_VRECSTOP_WARNING);
	} else {
		return;
	}

	UpdateData(FALSE);	// DDX�X�V
}

void CSetChatDialog::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	CString str;

	UpdateData(TRUE);	// DDX�X�V

	/// ���W�X�g���ۑ�
	//// �����񃌃W�X�g���ł͋󔒓��͂����������ꍇ������̂�force=TRUE��
	if (!m_strRegKey.IsEmpty()) {
		CRegString rgWarning = CRegString(m_strRegKey, _T(""), TRUE);
		rgWarning = m_strChat;
	}

	CDialog::OnOK();
}

void CSetChatDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
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
BOOL CSetChatDialog::UpdateForm() 
{
	CString str;

	/// �Ή����W�X�g���̓��e����ʂɕ\��
	if (m_strRegKey.IsEmpty()) return FALSE;

	if (m_strRegKey.Find(_T("VoiceRecordWarning")) > 0) {
		str.LoadString(IDS_TAB_REC_WARNING);
	} else if (m_strRegKey.Find(_T("VoiceRecordStopWarning")) > 0) {
		str.LoadString(IDS_TAB_RECSTOP_WARNING);
	} else if (m_strRegKey.Find(_T("VideoRecordWarning")) > 0) {
		str.LoadString(IDS_TAB_VREC_WARNING);
	} else if (m_strRegKey.Find(_T("VideoRecordStopWarning")) > 0) {
		str.LoadString(IDS_TAB_VRECSTOP_WARNING);
	} else {
		return FALSE;
	}

	m_strChat = CRegString(m_strRegKey, str, TRUE);	

	UpdateData(FALSE);	// DDX�X�V

	return TRUE;
}

