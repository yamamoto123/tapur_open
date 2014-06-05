#if !defined(AFX_RSFRENAMEDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFRENAMEDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfRenameDialog.h : ヘッダー ファイル
//
#include <imm.h> 

/////////////////////////////////////////////////////////////////////////////
// CRsfRenameDialog ダイアログ

class CRsfRenameDialog : public CDialog
{
// コンストラクション
public:
	CRsfRenameDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CRsfRenameDialog)
	enum { IDD = IDD_RSF_RENAME };
	CComboBox *m_pcmb;
	int m_idxId;
	CString	m_strName;
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

private:
	HIMC m_hImc;		// IMEハンドル(入力時の退避用)
	DWORD m_dwImeConv;	// IME入力モード(入力時の退避用)
	DWORD m_dwImeSent; 	// IME変換モード(入力時の退避用)
	long m_lngId;		// 対象テーブルID
	CString m_strOrgId;	// 送信者名（スカイプ名）のバックアップ
	CStringArray m_aryHandle;	// コンタクトリスト（ID）
	CStringArray m_aryFullName;	// コンタクトリスト（表示名）

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRsfRenameDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRsfRenameDialog)
	afx_msg void OnSelchangeId();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RSFRENAMEDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
