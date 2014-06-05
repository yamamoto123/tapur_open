#if !defined(AFX_SetChatDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SetChatDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetChatDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSetChatDialog ダイアログ

class CSetChatDialog : public CDialog
{
// コンストラクション
public:
	CSetChatDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CSetChatDialog();      

// ダイアログ データ
	//{{AFX_DATA(CSetChatDialog)
	enum { IDD = IDD_SET_CHAT };
	CString	m_strChat;
	//}}AFX_DATA

private:
	CString m_strRegKey;	// 対応レジストリキー

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSetChatDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSetChatDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void OnButtonInit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetRegKey(CString strRegKey) { m_strRegKey = strRegKey; }
	BOOL UpdateForm();	// 画面の更新
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SetChatDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
