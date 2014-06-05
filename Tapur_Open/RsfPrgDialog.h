#if !defined(AFX_RSFPRGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFPRGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfPrgDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRsfPrgDialog ダイアログ

class CRsfPrgDialog : public CDialog
{
// コンストラクション
public:
	CRsfPrgDialog(CWnd* pParent = NULL, CString strCaption = _T(""));   // 標準のコンストラクタ
	~CRsfPrgDialog();      

// ダイアログ データ
	//{{AFX_DATA(CRsfPrgDialog)
	enum { IDD = IDD_RSF_PRG };
	CString	m_strPrg;
	CComboBox m_pcmb1;
	CComboBox m_pcmb2;
	CComboBox m_pcmb3;
	CComboBox m_pcmb4;
	CComboBox m_pcmb5;
	CComboBox m_pcmb6;
	CComboBox m_pcmb7;
	CComboBox m_pcmb8;
	CComboBox m_pcmb9;
	CComboBox m_pcmb10;
	//}}AFX_DATA

private:
	CString m_strPlayFile;	// 連携CSVファイル名
	CStringArray m_aryName;	// 引数リスト（変数名）
	CStringArray m_aryVar;	// 引数リスト（変数）

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRsfPrgDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	CString	m_strCaption;	// windowタイトル

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRsfPrgDialog)
	afx_msg void OnBtnFile();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetFile(CString strFile) { m_strPlayFile = strFile; }
	BOOL UpdateForm();	// 画面の更新
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RSFPRGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
