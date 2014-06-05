#if !defined(AFX_RSFCHATDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFCHATDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfChatDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRsfChatDialog ダイアログ

class CRsfChatDialog : public CDialog
{
// コンストラクション
public:
	CRsfChatDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CRsfChatDialog();      

// ダイアログ データ
	//{{AFX_DATA(CRsfChatDialog)
	enum { IDD = IDD_RSF_CHAT };
	CString	m_strChat;
	//}}AFX_DATA

private:
	CString m_strPlayFile;	// 応答チャットファイル名

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRsfChatDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRsfChatDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetFile(CString strFile) { m_strPlayFile = strFile; }
	BOOL UpdateForm();	// 画面の更新
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RSFCHATDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
