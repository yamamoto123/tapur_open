// SetHotkeyEdit.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "SetHotkeyEdit.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetHotkeyEdit �_�C�A���O


CSetHotkeyEdit::CSetHotkeyEdit(CWnd* pParent, CString strTitle, UINT uiMod, int intKey)
	: CDialog(CSetHotkeyEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetHotkeyEdit)
	m_blShift = TRUE;
	m_blCtrl = TRUE;
	m_blAlt = TRUE;
	m_idxKey = 0;
	//}}AFX_DATA_INIT

	// �@�\��
	m_strTitle = strTitle;

	// �L�[�̏����l
	m_uiMod = uiMod;
	m_intKey = intKey;
}

void CSetHotkeyEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetHotkeyEdit)
	DDX_Check(pDX, IDC_CHK_SHIFT, m_blShift);
	DDX_Check(pDX, IDC_CHK_CTRL, m_blCtrl);
	DDX_Check(pDX, IDC_CHK_ALT, m_blAlt);
	DDX_CBIndex(pDX, IDC_CMB_HOTKEY, m_idxKey);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetHotkeyEdit, CDialog)
	//{{AFX_MSG_MAP(CSetHotkeyEdit)
	ON_BN_CLICKED(IDC_CHK_SHIFT, OnChkBox)
	ON_BN_CLICKED(IDC_CHK_CTRL, OnChkBox)
	ON_BN_CLICKED(IDC_CHK_ALT, OnChkBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetHotkeyEdit ���b�Z�[�W �n���h��


BOOL CSetHotkeyEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	/// �R���{�{�b�N�X������
	CComboBox *pcmb;
	for (_TCHAR ch = _T('A'); ch <= _T('Z'); ch++) {
		m_aryKey.Add(CString(ch));
	}
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_HOTKEY);
	for(int i = 0; i < m_aryKey.GetSize(); i++) {
		pcmb->AddString(m_aryKey.GetAt(i));
	}

	// �@�\����
	CStatic *ptxt = (CStatic*)GetDlgItem(IDC_STATIC_HOTKEY);
	ptxt->SetWindowText(m_strTitle);

	// �L�[�̏����ݒ�
	SetKey(m_uiMod, m_intKey);

	UpdateData(FALSE);	// DDX�X�V

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// �`�F�b�N�{�b�N�X���̓C�x���g
void CSetHotkeyEdit::OnChkBox() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	UpdateData(TRUE);	// DDX�X�V

	// �S���ڔ�I���͋����Ȃ�
	if (!m_blShift && !m_blCtrl && !m_blAlt) {
		m_blCtrl = true;
	}

	UpdateData(FALSE);	// DDX�X�V
}

void CSetHotkeyEdit::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	UpdateData(TRUE);	// DDX�X�V

	m_uiMod = 0;
	m_intKey = 0;

	if (m_blShift) m_uiMod |= MOD_SHIFT;
	if (m_blCtrl) m_uiMod |= MOD_CONTROL;
	if (m_blAlt) m_uiMod |= MOD_ALT;

	CStringA strA = (CStringA)m_aryKey.GetAt(m_idxKey);
	m_intKey = (int)strA.GetAt(0);

	CDialog::OnOK();
}

void CSetHotkeyEdit::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B

	CDialog::OnCancel();
}

// �L�[�̏����ݒ�
void CSetHotkeyEdit::SetKey(UINT uiMod, int intKey)
{
	m_blShift = (uiMod & MOD_SHIFT) ? TRUE : FALSE;
	m_blCtrl = (uiMod & MOD_CONTROL) ? TRUE : FALSE;
	m_blAlt = (uiMod & MOD_ALT) ? TRUE : FALSE;

	for(int i = 0; i <  m_aryKey.GetSize(); i++) {
		if (CString((TCHAR)intKey) == m_aryKey.GetAt(i)) {
			m_idxKey = i;
			break;
		}
	}

	UpdateData(FALSE);	// DDX�X�V
}
