#if !defined(AFX_RSFFOLDERNEWDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFFOLDERNEWDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfFolderNewDialog.h : ヘッダー ファイル
//
#include <imm.h> 

/////////////////////////////////////////////////////////////////////////////
// CRsfFolderNewDialog ダイアログ

class CRsfFolderNewDialog : public CDialog
{
// コンストラクション
public:
	CRsfFolderNewDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CRsfFolderNewDialog)
	enum { IDD = IDD_FOLDER_NEW };
	CString	m_strFolder;
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

private:
	HIMC m_hImc;		// IMEハンドル(入力時の退避用)
	DWORD m_dwImeConv;	// IME入力モード(入力時の退避用)
	DWORD m_dwImeSent; 	// IME変換モード(入力時の退避用)

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRsfFolderNewDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRsfFolderNewDialog)
	afx_msg void OnSetfocusEditId();
	afx_msg void OnKillfocusEditId();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RSFFOLDERNEWDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
