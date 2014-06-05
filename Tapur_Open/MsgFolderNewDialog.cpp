// MsgFolderNewDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "MsgFolderNewDialog.h"
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
// CMsgFolderNewDialog �_�C�A���O


CMsgFolderNewDialog::CMsgFolderNewDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgFolderNewDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgFolderNewDialog)
	m_strFolder = _T("");
	//}}AFX_DATA_INIT
}


void CMsgFolderNewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgFolderNewDialog)
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_strFolder);
	DDV_MaxChars(pDX, m_strFolder, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgFolderNewDialog, CDialog)
	//{{AFX_MSG_MAP(CMsgFolderNewDialog)
	ON_EN_SETFOCUS(IDC_EDIT_FOLDER, OnSetfocusEditId)
	ON_EN_KILLFOCUS(IDC_EDIT_FOLDER, OnKillfocusEditId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgFolderNewDialog ���b�Z�[�W �n���h��

void CMsgFolderNewDialog::OnSetfocusEditId()
{
/*
	/// IME�𔼊p�p�����ɕύX
	m_hImc = ImmGetContext(m_hWnd); 
	ImmGetConversionStatus(m_hImc, &m_dwImeConv, &m_dwImeSent); 
	ImmSetConversionStatus(m_hImc, m_dwImeConv, IME_SMODE_NONE);
*/
}

void CMsgFolderNewDialog::OnKillfocusEditId()
{
/*
	/// IME�̏�ԕ��A
	ImmSetConversionStatus(m_hImc, m_dwImeConv, m_dwImeSent); 
	ImmReleaseContext(m_hWnd, m_hImc);
*/
}

void CMsgFolderNewDialog::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������

	if (!UpdateData(TRUE)) return;	// DDX�X�V

	/// �I���t�H���_���
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	DWORD type = tc.GetItemData(pfview->m_hContext) & FD_MASK_TYPE;
	if (!(type == FD_NORMAL || type == FD_NORMAL_DEF)) return;

	/// �t�H���_���̃`�F�b�N
	if (!pfview->ChkFolder(m_strFolder, type)) return;

	/// folder�e�[�u���ɒǉ�
	CTblFolder tbl(theApp.m_strDBPath);
	tbl.strReceiverId = theApp.m_strUsrId;	// ��M�Җ��i�X�J�C�v���j
	tbl.strTitle = m_strFolder;				// �t�H���_��
	tbl.InsertRec();
	//// �ǉ����R�[�h��ID�擾
	CString strWhere;
	strWhere.Empty();
	long lngId = (long)tbl.MaxVal(_T("ID"), strWhere);

	/// �t�H���_���X�g�֒ǉ�
	HTREEITEM h;
	h = pfview->AddItem(pfview->m_hFolderDef, m_strFolder, FD_NORMAL, lngId);
	tc.EnsureVisible(h);

	CDialog::OnOK();
}

void CMsgFolderNewDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function

