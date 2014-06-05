// SetHotkey.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "SetHotkey.h"
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
// CSetHotkey �_�C�A���O


CSetHotkey::CSetHotkey(CWnd* pParent /*=NULL*/)
	: CDialog(CSetHotkey::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetHotkey)
	//}}AFX_DATA_INIT

}

void CSetHotkey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetHotkey)
	DDX_Control(pDX, IDC_LIST_HOTKEY, m_lstTool);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetHotkey, CDialog)
	//{{AFX_MSG_MAP(CSetHotkey)
	ON_BN_CLICKED(IDC_BTN_HOTKEY_EDIT, OnBtnHotkeyEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetHotkey ���b�Z�[�W �n���h��


BOOL CSetHotkey::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	CString str;

	/// ���X�g�X�^�C���ݒ�
	DWORD dwStyle = m_lstTool.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_lstTool.SetExtendedStyle(dwStyle);
	/// �w�b�_�[��`
	str.LoadString(IDS_HOTKEYLIST_HEAD);
	m_lstTool.SetColumnHeader(str);

	/// �f�[�^�ݒ�
	// ���W�X�g���Q��
	LoadRegistry();
	// ���C����ʕ\��
	AddRecord(IDS_HK_SHOW, (BOOL)m_rgEnableShow, (UINT)m_rgModShow, (int)m_rgKeyShow);
	// ���C����ʔ�\��
	AddRecord(IDS_HK_HIDE, (BOOL)m_rgEnableHide, (UINT)m_rgModHide, (int)m_rgKeyHide);
	// �^��
	AddRecord(IDS_HK_RECORD, (BOOL)m_rgEnableRecord, (UINT)m_rgModRecord, (int)m_rgKeyRecord);
	// �^��
	AddRecord(IDS_HK_VRECORD, (BOOL)m_rgEnableVRecord, (UINT)m_rgModVRecord, (int)m_rgKeyVRecord);
	// �^���E�^��ꎞ��~
	AddRecord(IDS_HK_REC_PAUSE, (BOOL)m_rgEnableRecPause, (UINT)m_rgModRecPause, (int)m_rgKeyRecPause);
	// �^���E�^��I��
	AddRecord(IDS_HK_REC_STOP, (BOOL)m_rgEnableRecStop, (UINT)m_rgModRecStop, (int)m_rgKeyRecStop);
	// �Đ��F���݃��X�g�őI������Ă��郌�R�[�h
	AddRecord(IDS_HK_PLAY, (BOOL)m_rgEnablePlay, (UINT)m_rgModPlay, (int)m_rgKeyPlay);
	// �Đ��ꎞ��~�E�ĊJ
	AddRecord(IDS_HK_PLAY_PAUSE, (BOOL)m_rgEnablePlayPause, (UINT)m_rgModPlayPause, (int)m_rgKeyPlayPause);
	// �Đ��I��
	AddRecord(IDS_HK_PLAY_STOP, (BOOL)m_rgEnablePlayStop, (UINT)m_rgModPlayStop, (int)m_rgKeyPlayStop);
	// ���̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̎��̃��R�[�h
	AddRecord(IDS_HK_PLAY_NEXT, (BOOL)m_rgEnablePlayNext, (UINT)m_rgModPlayNext, (int)m_rgKeyPlayNext);
	// �O�̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̑O�̃��R�[�h
	AddRecord(IDS_HK_PLAY_PREV, (BOOL)m_rgEnablePlayPrev, (UINT)m_rgModPlayPrev, (int)m_rgKeyPlayPrev);

	UpdateData(FALSE);	// DDX�X�V

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// �V���[�g�J�b�g�ύX�{�^��
void CSetHotkey::OnBtnHotkeyEdit() 
{
	UpdateData(TRUE);	// DDX�X�V

	// �I�����X�g���
	if (m_lstTool.GetSelectedCount() < 1) return;
	POSITION pos = m_lstTool.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	int idx = m_lstTool.GetNextSelectedItem(pos);
	CString strTitle = m_lstTool.GetItemText(idx, 1);
	CString str = m_lstTool.GetItemText(idx, 2);
	UINT RESID = (UINT)m_lstTool.GetItemData(idx);
	//	�L�[���
	UINT uniMod = 0;
	if (str.Find(_T("Ctrl")) >= 0) uniMod |= MOD_CONTROL;
	if (str.Find(_T("Shift")) >= 0) uniMod |= MOD_SHIFT;
	if (str.Find(_T("Alt")) >= 0) uniMod |= MOD_ALT;
	CStringA strA = (CStringA)str.Right(1);
	int intKey = (int)strA.GetAt(0);

	// �X�V�_�C�A���O
	CSetHotkeyEdit dlg(this, strTitle, uniMod, intKey);
	if (dlg.DoModal() != IDOK) return;
	UINT uniModNew = dlg.GetMod();
	int intKeyNew = dlg.GetKey();
	if (uniMod == uniModNew && intKey == intKeyNew) return;

	// ���X�g�X�V
	UpdateRecord(idx, uniModNew, intKeyNew);

	// ���W�X�g���X�V
	// ���C����ʕ\��
	if (RESID == IDS_HK_SHOW) m_rgModShow = uniModNew, m_rgKeyShow = intKeyNew;
	// ���C����ʔ�\��
	if (RESID == IDS_HK_HIDE) m_rgModHide = uniModNew, m_rgKeyHide = intKeyNew;
	// �^��
	if (RESID == IDS_HK_RECORD) m_rgModRecord = uniModNew, m_rgKeyRecord = intKeyNew;
	// �^��
	if (RESID == IDS_HK_VRECORD) m_rgModVRecord = uniModNew, m_rgKeyVRecord = intKeyNew;
	// �^���E�^��ꎞ��~
	if (RESID == IDS_HK_REC_PAUSE) m_rgModRecPause = uniModNew, m_rgKeyRecPause = intKeyNew;
	// �^���E�^��I��
	if (RESID == IDS_HK_REC_STOP) m_rgModRecStop = uniModNew, m_rgKeyRecStop = intKeyNew;
	// �Đ��F���݃��X�g�őI������Ă��郌�R�[�h
	if (RESID == IDS_HK_PLAY) m_rgModPlay = uniModNew, m_rgKeyPlay = intKeyNew;
	// �Đ��ꎞ��~�E�ĊJ
	if (RESID == IDS_HK_PLAY_PAUSE) m_rgModPlayPause = uniModNew, m_rgKeyPlayPause = intKeyNew;
	// �Đ��I��
	if (RESID == IDS_HK_PLAY_STOP) m_rgModPlayStop = uniModNew, m_rgKeyPlayStop = intKeyNew;
	// ���̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̎��̃��R�[�h
	if (RESID == IDS_HK_PLAY_NEXT) m_rgModPlayNext = uniModNew, m_rgKeyPlayNext = intKeyNew;
	// �O�̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̑O�̃��R�[�h
	if (RESID == IDS_HK_PLAY_PREV) m_rgModPlayPrev = uniModNew, m_rgKeyPlayPrev = intKeyNew;
}

// ���ʂ͗L��/�����̐ݒ�̂ݕۑ�
// �L�[�̐ݒ�̓��W�X�g���̒��ڑ���݂̂Ƃ���
void CSetHotkey::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	UpdateData(TRUE);	// DDX�X�V

	/// ���W�X�g���X�V
	BOOL blChk;
	// ���C����ʕ\��
	int idx = 0; blChk = m_lstTool.GetCheck(idx);
	m_rgEnableShow = (DWORD)blChk;
	// ���C����ʔ�\��
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnableHide = (DWORD)blChk;
	// �^��
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnableRecord = (DWORD)blChk;
	// �^��
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnableVRecord = (DWORD)blChk;
	// �^���E�^��ꎞ��~
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnableRecPause = (DWORD)blChk;
	// �^���E�^��I��
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnableRecStop = (DWORD)blChk;
	// �Đ��F���݃��X�g�őI������Ă��郌�R�[�h
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnablePlay = (DWORD)blChk;
	// �Đ��ꎞ��~�E�ĊJ
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnablePlayPause = (DWORD)blChk;
	// �Đ��I��
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnablePlayStop = (DWORD)blChk;
	// ���̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̎��̃��R�[�h
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnablePlayNext = (DWORD)blChk;
	// �O�̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̑O�̃��R�[�h
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnablePlayPrev = (DWORD)blChk;

	// Hotkey�̓o�^�X�V
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	pFrame->UnregHotKey();
	pFrame->RegHotKey();

	CDialog::OnOK();
}

void CSetHotkey::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B

	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// global function

// ���W�X�g���Q��
BOOL CSetHotkey::LoadRegistry() 
{
	// ���C����ʕ\��
	m_rgEnableShow = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnableShow"), 1, TRUE);
	m_rgModShow = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModShow"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyShow = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyShow"), 'T', TRUE);
	// ���C����ʔ�\��
	m_rgEnableHide = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnableHide"), 1, TRUE);
	m_rgModHide = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModHide"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyHide = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyHide"), 'H', TRUE);
	// �^��
	m_rgEnableRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnableRecord"), 1, TRUE);
	m_rgModRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModRecord"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyRecord"), 'R', TRUE);
	// �^��
	m_rgEnableVRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnableVRecord"), 1, TRUE);
	m_rgModVRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModVRecord"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyVRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyVRecord"), 'V', TRUE);
	// �^���E�^��ꎞ��~
	m_rgEnableRecPause = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnableRecPause"), 1, TRUE);
	m_rgModRecPause = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModRecPause"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyRecPause = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyRecPause"), 'O', TRUE);
	// �^���E�^��I��
	m_rgEnableRecStop = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnableRecStop"), 1, TRUE);
	m_rgModRecStop = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModRecStop"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyRecStop = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyRecStop"), 'X', TRUE);
	// �Đ��F���݃��X�g�őI������Ă��郌�R�[�h
	m_rgEnablePlay = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnablePlay"), 1, TRUE);
	m_rgModPlay = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModPlay"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyPlay = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyPlay"), 'P', TRUE);
	// �Đ��ꎞ��~�E�ĊJ
	m_rgEnablePlayPause = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnablePlayPause"), 1, TRUE);
	m_rgModPlayPause = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModPlayPause"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyPlayPause = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyPlayPause"), 'I', TRUE);
	// �Đ��I��
	m_rgEnablePlayStop = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnablePlayStop"), 1, TRUE);
	m_rgModPlayStop = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModPlayStop"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyPlayStop = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyPlayStop"), 'E', TRUE);
	// ���̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̎��̃��R�[�h
	m_rgEnablePlayNext = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnablePlayNext"), 1, TRUE);
	m_rgModPlayNext = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModPlayNext"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyPlayNext = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyPlayNext"), 'F', TRUE);
	// �O�̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̑O�̃��R�[�h
	m_rgEnablePlayPrev = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnablePlayPrev"), 1, TRUE);
	m_rgModPlayPrev = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModPlayPrev"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyPlayPrev = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyPlayPrev"), 'B', TRUE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// local function

//
//  �@�\     : ���R�[�h�ǉ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CSetHotkey::AddRecord(
UINT RESID,		// ���\�[�XID
BOOL blChk,		// �L��/����
UINT uiMod,		// Modify key
int iKey)		// Virtual Key
{
	CString str;
	CString strExp;
	int idx;

	idx = m_lstTool.InsertItem(m_lstTool.GetItemCount(), _T(""));
	m_lstTool.SetCheck(idx, blChk);
	strExp.LoadString(RESID);
	m_lstTool.SetItemData(idx, (DWORD)RESID);
	m_lstTool.SetItemText(idx, 1, strExp);
	str.Empty();
	if (uiMod & MOD_CONTROL) {
		if (str.GetLength() > 0)	str += _T("+");
		str += _T("Ctrl");
	}
	if (uiMod & MOD_SHIFT) {
		if (str.GetLength() > 0)	str += _T("+");
		str += _T("Shift");
	}
	if (uiMod & MOD_ALT) {
		if (str.GetLength() > 0)	str += _T("+");
		str += _T("Alt");
	}

	if (str.GetLength() > 0)	str += _T("+");
	str += (TCHAR)iKey;
	m_lstTool.SetItemText(idx, 2, str);

	return TRUE;  
}

//
//  �@�\     : ���R�[�h�X�V
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CSetHotkey::UpdateRecord(
int idx,		// List index
UINT uiMod,		// Modify key
int iKey)		// Virtual Key
{
	CString str;

	str.Empty();
	if (uiMod & MOD_CONTROL) {
		if (str.GetLength() > 0)	str += _T("+");
		str += _T("Ctrl");
	}
	if (uiMod & MOD_SHIFT) {
		if (str.GetLength() > 0)	str += _T("+");
		str += _T("Shift");
	}
	if (uiMod & MOD_ALT) {
		if (str.GetLength() > 0)	str += _T("+");
		str += _T("Alt");
	}

	if (str.GetLength() > 0)	str += _T("+");
	str += (TCHAR)iKey;
	m_lstTool.SetItemText(idx, 2, str);

	return TRUE;  
}
