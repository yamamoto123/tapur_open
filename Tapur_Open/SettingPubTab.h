#if !defined(AFX_SETTINGPUBTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETTINGPUBTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingPubTab.h : ヘッダー ファイル
//
#include <imm.h> 
#include "TabPageSSL.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingPubTab ダイアログ

class CSettingPubTab : public CTabPageSSL
{
// コンストラクション
public:
	CSettingPubTab(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSettingPubTab)
	enum { IDD = IDD_TAB_PUB };
	BOOL	m_blStartup;
	BOOL	m_blDisable;
	int		m_idxWaitAnswer;
	int		m_idxMaxRecord;
	BOOL	m_blShortCutTapur;
	BOOL	m_blShortCutSkype;
	CButton	m_File;
	int		m_idxCodecName;
	int		m_idxFormatName;
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

private:
	HIMC m_hImc;		// IMEハンドル(入力時の退避用)
	DWORD m_dwImeConv;	// IME入力モード(入力時の退避用)
	DWORD m_dwImeSent; 	// IME変換モード(入力時の退避用)

	/// レジストリ
	CRegString m_rgStartup;
	CRegDWORD m_rgDisable;
	CRegDWORD m_rgWaitAnswer;
	CRegDWORD m_rgMaxRecord;
	CRegDWORD m_rgShortCutTapur;
	CRegDWORD m_rgShortCutSkype;
	CRegDWORD m_rgFormatType;		// WAV or MP3
	CRegString m_rgCodecName;		// MP3コーデック名称
	CRegString m_rgFormatName;		// MP3フォーマット名称

	CString m_strBeepFile;			// 設定BEEPファイル
	CStringArray m_aryWaitAnswer;	// 待機時間リスト
	CStringArray m_aryMaxRecord;	// 録音時間リスト
	CStringArray m_aryCodecName;	// MP3コーデック形式
	CStringArray m_aryFormatName;	// MP3フォーマット形式

	CAcmMgr m_oAcmMgr;				// ACMマネージャ
	WAVEFORMATEX m_wfx;				// 現在選択中のフォーマットに対応したWAVEFORMATEX

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSettingPubTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSettingPubTab)
//	afx_msg void OnSetfocusWaitAnswer();
//	afx_msg void OnKillfocusWaitAnswer();
//	afx_msg void OnSetfocusMaxRecord();
//	afx_msg void OnKillfocusMaxRecord();
//	afx_msg void OnChkDisable();
//	afx_msg void OnChkContact();
//	afx_msg void OnChkMute();
	afx_msg void OnBtnBeepFile();
	afx_msg void OnCmbCodec();
	afx_msg void OnRdoMp3();
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

#endif // !defined(AFX_SETTINGPUBTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
