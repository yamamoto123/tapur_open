#if !defined(AFX_RSFNEWDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFNEWDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfNewDialog.h : ヘッダー ファイル
//
#include <imm.h> 

/////////////////////////////////////////////////////////////////////////////
// CRsfNewDialog ダイアログ

class CRsfNewDialog : public CDialog
{
// コンストラクション
public:
	CRsfNewDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CRsfNewDialog)
	enum { IDD = IDD_RSF_NEW };
	CComboBox *m_pcmb;
	int m_idxId;
	CString	m_strName;
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA
	CString	m_strId;	// 初期ID

private:
	HIMC m_hImc;		// IMEハンドル(入力時の退避用)
	DWORD m_dwImeConv;	// IME入力モード(入力時の退避用)
	DWORD m_dwImeSent; 	// IME変換モード(入力時の退避用)
	CStringArray m_aryHandle;	// コンタクトリスト（ID）
	CStringArray m_aryFullName;	// コンタクトリスト（表示名）

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRsfNewDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRsfNewDialog)
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

#endif // !defined(AFX_RSFNEWDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
