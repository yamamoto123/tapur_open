#if !defined(AFX_TMSGBOX_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_TMSGBOX_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TMsgBox.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTMsgBox ダイアログ

class CTMsgBox : public CDialog
{
// コンストラクション
public:
	CTMsgBox(CWnd* pParent, CString strCaption, CString strMsg, BOOL blTop = FALSE);
	CTMsgBox(CWnd* pParent, CString strCaption, CString strMsg, BOOL blTop, CStringArray &aryName);

protected:
// ダイアログ データ
	//{{AFX_DATA(CTMsgBox)
	enum { IDD = IDD_TMSGBOX };
	BOOL m_blChk;
	CComboBox m_pCmb;
	//}}AFX_DATA
	CString	m_strCaption;	// windowタイトル
	CString	m_strMsg;		// 表示メッセージ
	BOOL m_blTop;			// 前面表示
	CStringArray m_aryName;	// リスト表示の場合

public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTMsgBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTMsgBox)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_blReturn;
	CString m_strReturn;
	//BOOL EnableCheck(CString strMsg);
	void EnableCombo(CStringArray &aryName);

public:
	void Show(BOOL blModal = TRUE);
	BOOL GetResult() { return m_blReturn; }
	BOOL GetCheck() { return m_blChk; }
	CString GetCombo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TMSGBOX_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
