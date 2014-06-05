// SetHotkeyEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SetHotkeyEdit.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetHotkeyEdit ダイアログ


CSetHotkeyEdit::CSetHotkeyEdit(CWnd* pParent, CString strTitle, UINT uiMod, int intKey)
	: CDialog(CSetHotkeyEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetHotkeyEdit)
	m_blShift = TRUE;
	m_blCtrl = TRUE;
	m_blAlt = TRUE;
	m_idxKey = 0;
	//}}AFX_DATA_INIT

	// 機能名
	m_strTitle = strTitle;

	// キーの初期値
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
// CSetHotkeyEdit メッセージ ハンドラ


BOOL CSetHotkeyEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	/// コンボボックス初期化
	CComboBox *pcmb;
	for (_TCHAR ch = _T('A'); ch <= _T('Z'); ch++) {
		m_aryKey.Add(CString(ch));
	}
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_HOTKEY);
	for(int i = 0; i < m_aryKey.GetSize(); i++) {
		pcmb->AddString(m_aryKey.GetAt(i));
	}

	// 機能名称
	CStatic *ptxt = (CStatic*)GetDlgItem(IDC_STATIC_HOTKEY);
	ptxt->SetWindowText(m_strTitle);

	// キーの初期設定
	SetKey(m_uiMod, m_intKey);

	UpdateData(FALSE);	// DDX更新

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// チェックボックス入力イベント
void CSetHotkeyEdit::OnChkBox() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(TRUE);	// DDX更新

	// 全項目非選択は許可しない
	if (!m_blShift && !m_blCtrl && !m_blAlt) {
		m_blCtrl = true;
	}

	UpdateData(FALSE);	// DDX更新
}

void CSetHotkeyEdit::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	UpdateData(TRUE);	// DDX更新

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
	// TODO: この位置に特別な後処理を追加してください。

	CDialog::OnCancel();
}

// キーの初期設定
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

	UpdateData(FALSE);	// DDX更新
}
