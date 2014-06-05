#if !defined(AFX_SETTINGVRECTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETTINGVRECTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingVRecTab.h : ヘッダー ファイル
//
#include <imm.h> 
#include "TabPageSSL.h"
#include "Label.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingVRecTab ダイアログ

class CSettingVRecTab : public CTabPageSSL
{
// コンストラクション
public:
	CSettingVRecTab(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSettingVRecTab)
	enum { IDD = IDD_TAB_VREC };
	int		m_idxVRecordLimit;
	int		m_idxVCmpName;
	int		m_idxSepSize;
	CButton	m_btnVRecWarning;
	CButton	m_btnVRecStopWarning;
	BOOL	m_blVRecWarning;
	BOOL	m_blVRecStopWarning;
	BOOL	m_blVSetTop;
	BOOL	m_blNoOverlay;
	BOOL	m_blSep;
	BOOL	m_blAudio;
	CString m_strFps;
	CLabel	m_LinkLine;
	//}}AFX_DATA

private:
	/// レジストリ
	CRegDWORD m_rgVRecordLimit;
	CRegDWORD m_rgEnableVRecWarning;
	CRegDWORD m_rgEnableVRecStopWarning;
	CRegDWORD m_rgVSetTop;
	CRegDWORD m_rgOverlay;
	CRegDWORD m_rgSep;
	CRegDWORD m_rgSepSize;
	CRegDWORD m_rgAudio;
	CRegDWORD m_rgVCmp;
	CRegString m_rgVCmpName;
	CRegDWORD m_rgFps;

	CStringArray m_aryVRecordLimit;	// 録画時間リスト
	CStringArray m_arySepSize;		// 録画サイズリスト
	CStringArray m_aryVCmpName;		// 録音形式
	CStringArray m_aryFps;			// FPS

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSettingVRecTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSettingVRecTab)
	afx_msg void OnBtnVRecWarning();
	afx_msg void OnBtnVRecStopWarning();
	afx_msg void OnChgFps();
	afx_msg void OnChgForm();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//void AssignListToComboBox(CComboBox *Dest, CString Source, long Indx, long MaxIndxLimit=-1);
	void AssignListToComboBox(CComboBox *Dest, CStringList *Source);

public:
	BOOL Check() { return TRUE; }	// 検査
	void Save();	// 保存
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SETTINGVRECTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
