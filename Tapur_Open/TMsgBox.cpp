// TMsgBox.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "TMsgBox.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CTMsgBox �_�C�A���O

// �`�F�b�N��ʗp�R���X�g���N�^
CTMsgBox::CTMsgBox(CWnd* pParent /*=NULL*/, CString strCaption, CString strMsg, BOOL blTop)
	: CDialog(CTMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTMsgBox)
	m_blChk = FALSE;
	//}}AFX_DATA_INIT

	m_strCaption = strCaption;
	m_strMsg = strMsg;
	m_blTop = blTop;

	m_blReturn = FALSE;
	m_strReturn = _T("");
}

// ���X�g��ʗp�R���X�g���N�^
CTMsgBox::CTMsgBox(CWnd* pParent /*=NULL*/, CString strCaption, CString strMsg, BOOL blTop, CStringArray &aryName)
	: CDialog(CTMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTMsgBox)
	m_blChk = FALSE;
	//}}AFX_DATA_INIT

	m_strCaption = strCaption;
	m_strMsg = strMsg;
	m_blTop = blTop;

	m_blReturn = FALSE;
	m_strReturn = _T("");

	m_aryName.RemoveAll();
	for(int i = 0; i < aryName.GetSize(); i++) {
		m_aryName.Add(aryName.GetAt(i));
	}

}

void CTMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTMsgBox)
	DDX_Check(pDX, IDC_CHK_TMSGBOX, m_blChk);
	DDX_Control(pDX, IDC_CMB_TMSGBOX, m_pCmb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTMsgBox, CDialog)
	//{{AFX_MSG_MAP(CTMsgBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTMsgBox ���b�Z�[�W �n���h��


BOOL CTMsgBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	/// �^�C�g���ƃ��b�Z�[�W�\��
	this->SetWindowText(m_strCaption);
	CStatic *ptxt;
	ptxt = (CStatic*)GetDlgItem(IDC_STATIC_TMSGBOX);
	ptxt->SetWindowText(m_strMsg);

	/// �\���v�f�̗L��/����
	if (m_aryName.GetSize() > 0) {
		// ���X�g�I�����
		EnableCombo(m_aryName);
	} else {
		// �`�F�b�N���
		m_pCmb.ShowWindow(SW_HIDE);
	}

	/// Window�O�ʕ\��
	if (m_blTop) {
		this->SetWindowPos(&wndTopMost, 0,0,0,0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE);
	}


	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}


void CTMsgBox::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	UpdateData(TRUE);	// DDX�X�V

	m_blReturn = TRUE;
	m_pCmb.GetWindowText(m_strReturn);

	CDialog::OnOK();
}

void CTMsgBox::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	m_blReturn = FALSE;

	CDialog::OnCancel();
}

void CTMsgBox::Show(BOOL blModal) 
{
	if (blModal) {
		this->DoModal();
	} else {
		this->ShowWindow(SW_SHOWNORMAL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// local function

void CTMsgBox::EnableCombo(CStringArray &aryName)
{
	/// �\���v�f�̗L��/����
	CWnd *pChk = NULL;
	pChk = GetDlgItem(IDC_CHK_TMSGBOX);
	pChk->ShowWindow(SW_HIDE);
	m_pCmb.ShowWindow(SW_SHOW);

	/// ���X�g�̐ݒ�
	for(int i = 0; i < aryName.GetSize(); i++) {
		m_pCmb.AddString(aryName.GetAt(i));
	}
	m_pCmb.SetCurSel(0);
}

CString CTMsgBox::GetCombo()
{
	return m_strReturn;
}

