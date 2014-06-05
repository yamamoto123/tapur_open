#if !defined(AFX_SETTINGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETTINGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDialog.h : ヘッダー ファイル
//
#include "TabCtrlSSL.h"
//#include "ButtonSSL.h"
#include "SettingPubTab.h"	
#include "SettingRecTab.h"	
#include "SettingVRecTab.h"	
#include "SettingAdminTab.h"	

/////////////////////////////////////////////////////////////////////////////
// CSettingDialog ダイアログ

class CSettingDialog : public CDialog
{
// コンストラクション
public:
	CSettingDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSettingDialog)
	enum { IDD = IDD_SETTING };
	CButton	m_Apply;
	CTabCtrlSSL	m_tabCtrl;
	CString	m_strId;
	CString	m_strName;
	//}}AFX_DATA

private:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSettingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	CSettingPubTab m_tabPub;
	CSettingRecTab m_tabRec;
	CSettingVRecTab m_tabVRec;
	CSettingAdminTab m_tabAdmin;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSettingDialog)
	afx_msg void OnApply();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void EnableApply() { m_Apply.EnableWindow(TRUE); }	// 適用ボタンを有効化

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SETTINGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
