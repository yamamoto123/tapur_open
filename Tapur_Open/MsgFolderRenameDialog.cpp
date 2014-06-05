// MsgFolderRenameDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "MsgFolderRenameDialog.h"
#include "MsgTree.h"
#include "TPtable.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgFolderRenameDialog �_�C�A���O


CMsgFolderRenameDialog::CMsgFolderRenameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgFolderRenameDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgFolderRenameDialog)
	m_strFolder = _T("");
	//}}AFX_DATA_INIT
}


void CMsgFolderRenameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgFolderRenameDialog)
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_strFolder);
	DDV_MaxChars(pDX, m_strFolder, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgFolderRenameDialog, CDialog)
	//{{AFX_MSG_MAP(CMsgFolderRenameDialog)
	ON_EN_SETFOCUS(IDC_EDIT_FOLDER, OnSetfocusEditId)
	ON_EN_KILLFOCUS(IDC_EDIT_FOLDER, OnKillfocusEditId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgFolderRenameDialog ���b�Z�[�W �n���h��

void CMsgFolderRenameDialog::OnSetfocusEditId()
{
/*
	/// IME�𔼊p�p�����ɕύX
	m_hImc = ImmGetContext(m_hWnd); 
	ImmGetConversionStatus(m_hImc, &m_dwImeConv, &m_dwImeSent); 
	ImmSetConversionStatus(m_hImc, m_dwImeConv, IME_SMODE_NONE);
*/
}

void CMsgFolderRenameDialog::OnKillfocusEditId()
{
/*
	/// IME�̏�ԕ��A
	ImmSetConversionStatus(m_hImc, m_dwImeConv, m_dwImeSent); 
	ImmReleaseContext(m_hWnd, m_hImc);
*/
}

void CMsgFolderRenameDialog::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������

	if (!UpdateData(TRUE)) return;	// DDX�X�V

	/// �I���t�H���_���
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	DWORD type = tc.GetItemData(pfview->m_hContext) & FD_MASK_TYPE;
	if (!(type == FD_NORMAL)) return;
	long lngFolderId = tc.GetItemData(pfview->m_hContext) & FD_MASK_ID;
	CString strTitle = tc.GetItemText(pfview->m_hContext);

	/// �t�H���_���̃`�F�b�N
	if (!pfview->ChkFolder(m_strFolder, type)) return;

	/// folder�e�[�u�����X�V
	CTblFolder tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(lngFolderId) != 1) {
		return;
	}
	tbl.strTitle = m_strFolder;				// �t�H���_��
	tbl.UpdateRec(lngFolderId);

	/// �t�H���_���X�g���X�V
	tc.SetItemText(pfview->m_hContext, m_strFolder);
	tc.EnsureVisible(pfview->m_hContext);

	CDialog::OnOK();
}

void CMsgFolderRenameDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function


