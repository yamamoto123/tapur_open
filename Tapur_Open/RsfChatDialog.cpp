// RsfChatDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfChatDialog.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CRsfChatDialog �_�C�A���O


CRsfChatDialog::CRsfChatDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRsfChatDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRsfChatDialog)
	m_strChat = _T("");
	//}}AFX_DATA_INIT
}

CRsfChatDialog::~CRsfChatDialog()
{
}

void CRsfChatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfChatDialog)
	DDX_Text(pDX, IDC_EDIT_CHAT, m_strChat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRsfChatDialog, CDialog)
	//{{AFX_MSG_MAP(CRsfChatDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfChatDialog ���b�Z�[�W �n���h��

BOOL CRsfChatDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	// ��ʍX�V
	UpdateForm();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CRsfChatDialog::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	CString str;

	UpdateData(TRUE);	// DDX�X�V

	/// �e�L�X�g�t�@�C���ۑ�
	CFileSpec fs(m_strPlayFile);
	if (!fs.SaveString(m_strChat)) {
		str.LoadString(IDS_MSGBOX101);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}

	CDialog::OnOK();
}

void CRsfChatDialog::OnCancel() 
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
BOOL CRsfChatDialog::UpdateForm() 
{
	/// �����`���b�g�t�@�C��������ꍇ�A��ʂɕ\��
	CFileSpec fs(m_strPlayFile);
	if (!fs.Exist()) return TRUE;
	if (!fs.LoadString(&m_strChat)) return FALSE;

	UpdateData(FALSE);	// DDX�X�V

	return TRUE;
}

