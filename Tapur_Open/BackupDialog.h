#if !defined(AFX_BACKUPDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_BACKUPDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackupDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CBackupDialog ダイアログ

class CBackupDialog : public CDialog
{
// コンストラクション
public:
	CBackupDialog(CWnd* pParent = NULL);

protected:
// ダイアログ データ
	//{{AFX_DATA(CBackupDialog)
	enum { IDD = IDD_DATA_BACKUP };
	CEdit	m_edtBackupPath;
	CEdit	m_edtRestorePath;
	//}}AFX_DATA

public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CBackupDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CBackupDialog)
	afx_msg void OnBrowseBackup();
	afx_msg void OnBrowseRestore();
	afx_msg void OnBtnBackup();
	afx_msg void OnBtnRestore();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_BACKUPDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
