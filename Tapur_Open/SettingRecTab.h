#if !defined(AFX_SETTINGRECTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETTINGRECTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingRecTab.h : ヘッダー ファイル
//
#include <imm.h> 
#include "TabPageSSL.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingRecTab ダイアログ

class CSettingRecTab : public CTabPageSSL
{
// コンストラクション
public:
	CSettingRecTab(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSettingRecTab)
	enum { IDD = IDD_TAB_REC };
	int		m_idxRecordLimit;
	CButton	m_btnRecWarning;
	CButton	m_btnRecStopWarning;
	BOOL	m_blRecWarning;
	BOOL	m_blRecStopWarning;
	BOOL	m_blEnableAuto;
	int		m_idxCodecName;
	int		m_idxFormatName;
	CString	m_strVolSnd;
	CString	m_strVolRcv;
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

private:
	/// レジストリ
	CRegDWORD m_rgRecordLimit;
	CRegDWORD m_rgEnableRecWarning;
	CRegDWORD m_rgEnableRecStopWarning;
	CRegDWORD m_rgStereo;
	CRegDWORD m_rgFormatType;		// WAV or MP3
	CRegString m_rgCodecName;		// MP3コーデック名称
	CRegString m_rgFormatName;		// MP3フォーマット名称
	CRegDWORD m_rgEnableAuto;		// 自動録音
	CRegDWORD m_rgVolSnd;			// 音量調整
	CRegDWORD m_rgVolRcv;			// 音量調整

	CStringArray m_aryRecordLimit;	// 録音時間リスト
	CStringArray m_aryCodecName;	// MP3コーデック形式
	CStringArray m_aryFormatName;	// MP3フォーマット形式

	CAcmMgr m_oAcmMgr;				// ACMマネージャ
	WAVEFORMATEX m_wfx;				// 現在選択中のフォーマットに対応したWAVEFORMATEX

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSettingRecTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSettingRecTab)
	afx_msg void OnBtnRecWarning();
	afx_msg void OnBtnRecStopWarning();
	afx_msg void OnRdoStereo();
	afx_msg void OnCmbCodec();
	afx_msg void OnRdoMp3();
	afx_msg void OnChgVolSnd();
	afx_msg void OnChgVolRcv();
	afx_msg void OnChgForm();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL Check() { return TRUE; }	// 検査
	void Save();	// 保存
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SETTINGRECTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
