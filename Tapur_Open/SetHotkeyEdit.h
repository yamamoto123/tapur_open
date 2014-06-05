#if !defined(AFX_SETHOTKEYEDIT_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETHOTKEYEDIT_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetHotkeyEdit.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSetHotkeyEdit ダイアログ

class CSetHotkeyEdit : public CDialog
{
// コンストラクション
public:
	CSetHotkeyEdit(CWnd* pParent, CString strTitle, UINT uiMod, int intKey);

protected:
// ダイアログ データ
	//{{AFX_DATA(CSetHotkeyEdit)
	enum { IDD = IDD_HOTKEY_EDIT };
	BOOL	m_blShift;
	BOOL	m_blCtrl;
	BOOL	m_blAlt;
	int		m_idxKey;
	//}}AFX_DATA

private:
	CStringArray m_aryKey;
	CString m_strTitle;
	UINT m_uiMod;	// Modify key
	int m_intKey;	// Virtual Key
	void SetKey(UINT uiMod, int intKey);	// キーの初期設定

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSetHotkeyEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSetHotkeyEdit)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnChkBox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	UINT GetMod() { return m_uiMod; }
	int GetKey() { return m_intKey; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SETHOTKEYEDIT_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
