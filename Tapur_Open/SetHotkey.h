#if !defined(AFX_SETHOTKEY_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETHOTKEY_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetHotkey.h : ヘッダー ファイル
//
#include "ReportCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSetHotkey ダイアログ

class CSetHotkey : public CDialog
{
// コンストラクション
public:
	CSetHotkey(CWnd* pParent = NULL);

protected:
// ダイアログ データ
	//{{AFX_DATA(CSetHotkey)
	enum { IDD = IDD_HOTKEY };
	CReportCtrl m_lstTool;	// リスト操作クラス
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSetHotkey)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSetHotkey)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnHotkeyEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
//レジストリ変数
	// メイン画面表示
	CRegDWORD m_rgEnableShow;
	CRegDWORD m_rgModShow;
	CRegDWORD m_rgKeyShow;
	// メイン画面非表示
	CRegDWORD m_rgEnableHide;
	CRegDWORD m_rgModHide;
	CRegDWORD m_rgKeyHide;
	// 録音
	CRegDWORD m_rgEnableRecord;
	CRegDWORD m_rgModRecord;
	CRegDWORD m_rgKeyRecord;
	// 録画
	CRegDWORD m_rgEnableVRecord;
	CRegDWORD m_rgModVRecord;
	CRegDWORD m_rgKeyVRecord;
	// 録音・録画一時停止
	CRegDWORD m_rgEnableRecPause;
	CRegDWORD m_rgModRecPause;
	CRegDWORD m_rgKeyRecPause;
	// 録音・録画終了
	CRegDWORD m_rgEnableRecStop;
	CRegDWORD m_rgModRecStop;
	CRegDWORD m_rgKeyRecStop;
	// 再生：現在リストで選択されているレコード
	CRegDWORD m_rgEnablePlay;
	CRegDWORD m_rgModPlay;
	CRegDWORD m_rgKeyPlay;
	// 再生一時停止・再開
	CRegDWORD m_rgEnablePlayPause;
	CRegDWORD m_rgModPlayPause;
	CRegDWORD m_rgKeyPlayPause;
	// 再生終了
	CRegDWORD m_rgEnablePlayStop;
	CRegDWORD m_rgModPlayStop;
	CRegDWORD m_rgKeyPlayStop;
	// 次の再生：現在リストで選択されているレコードの次のレコード
	CRegDWORD m_rgEnablePlayNext;
	CRegDWORD m_rgModPlayNext;
	CRegDWORD m_rgKeyPlayNext;
	// 前の再生：現在リストで選択されているレコードの前のレコード
	CRegDWORD m_rgEnablePlayPrev;
	CRegDWORD m_rgModPlayPrev;
	CRegDWORD m_rgKeyPlayPrev;

	// レジストリ参照
	BOOL LoadRegistry();

private:
	BOOL AddRecord(UINT RESID, BOOL blChk, UINT uiMod, int iKey);		// レコード追加
	BOOL UpdateRecord(int idx, UINT uiMod, int iKey);		// レコード更新
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SETHOTKEY_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
