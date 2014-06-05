#if !defined(AFX_SETTINGADMINTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETTINGADMINTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingAdminTab.h : ヘッダー ファイル
//
#include "TabPageSSL.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingAdminTab ダイアログ

class CSettingAdminTab : public CTabPageSSL
{
// コンストラクション
public:
	CSettingAdminTab(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSettingAdminTab)
	enum { IDD = IDD_TAB_PUB };
	BOOL	m_blHotKey;
	BOOL	m_blEnableLog;
	BOOL	m_blDataPath;
	CEdit	m_edtDataPath;
	CButton m_btnDataPath;
	CString	m_strSkypePortOut;
	CString	m_strSkypePortMic;
	//}}AFX_DATA

private:

	/// レジストリ
	CRegDWORD m_rgEnableHotKey;
	CRegDWORD m_rgEnableLog;
	CRegDWORD m_rgEnableDataPath;
	CRegString m_rgDataPath;
	CRegDWORD m_rgSkypePortOut;
	CRegDWORD m_rgSkypePortMic;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSettingAdminTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSettingAdminTab)
	afx_msg void OnBtnHotkey();
	afx_msg void OnBrowseDataPath();
	afx_msg void OnChgDataPath();
	afx_msg void OnChgSkypePortOut();
	afx_msg void OnChgSkypePortMic();
	afx_msg void OnChgForm();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL Check();	// 検査
	void Save();	// 保存
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SETTINGADMINTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
