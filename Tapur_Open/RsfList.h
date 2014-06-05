#if !defined(AFX_RSFLIST_H__64AA0262_2D7C_4E02_A9B1_E1C68488FAA8__INCLUDED_)
#define AFX_RSFLIST_H__64AA0262_2D7C_4E02_A9B1_E1C68488FAA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfList.h : ヘッダー ファイル
//
//#include "XListCtrl.h"
#include "ReportCtrl.h"
#include "CppSQLite3.h"
#include "TPerror.h"

/////////////////////////////////////////////////////////////////////////////
// CRsfList フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CRsfList :  public TVisualFormView
{
protected:
	CRsfList();           // 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CRsfList)

// フォーム データ
public:
	//{{AFX_DATA(CRsfList)
	enum { IDD = IDD_MSGLISTFORM };
	CReportCtrl m_wndList;	// リスト操作クラス
	//}}AFX_DATA

// アトリビュート
public:
	CImageList  m_ListImage;	// リストアイコンを格納するポインタ
	CImageList  m_DragImage;	// ドラッグイメージを格納するポインタ
	BOOL        m_bDrag;		// ドラッグ中ならTRUE、ドラッグ中でなければFALSE
	int			m_hItemDrag;	// ドラッグアイテム（コピー元アイテム）のハンドル
	HCURSOR		m_DragCursor;	// ドラッグ中のバックアップカーソル

	int m_hActive;			// 選択アイテム
//	int m_hContext;			// コンテキストメニュー時の（仮）選択アイテム
	DWORD m_dwFolderType;	// コンテキストメニュー時のフォルダタイプ
	DWORD m_dwFolderId;		// コンテキストメニュー時のフォルダID
	CUIntArray m_aryClip;	// コピー・貼り付け時のID保存領域
	CUIntArray m_arySelect;	// リスト表示時に選択状態にするID保存領域

// オペレーション
public:

private:
	CTPerror te;
	void InitListCtrl(void);
	void AdustColumns(void);	// リストのカラムサイズ調整
	BOOL MakeList(DWORD lngFolderType, long lngFolderId);	// 設定リスト生成
	BOOL Move(long lngFolderId);	// 選択された応答設定を移動
	BOOL Copy();	// 選択された応答設定をコピー
	BOOL TrashDel();	// ごみ箱(応答)内のアイテム削除
	BOOL TrashRecover();// ごみ箱(応答)内のアイテムを元に戻す
	BOOL Call(CString strSender);	// 送信者にスカイプコール
	long SelectedList(CUIntArray *pAryIdx, BOOL blData, BOOL blDefault);	// 応答設定選択リストの取得
	long FindIdx(long lngId);				// 応答設定リストインデックスの検索
	CPoint CnvPntM_F(const CPoint point);	// クライアント座標変換（RsfList->FolderTree）
	BOOL Play();	// 音声再生
	BOOL ChkPlayIcon(const CPoint point);	// 指定座標が音声再生アイコン上にあるか

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRsfList)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CRsfList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRsfList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSetupNew();
	afx_msg void OnSetupRename();
	afx_msg void OnSetupCopy();
	afx_msg void OnSetupPaste();
	afx_msg void OnSetupCall();
	afx_msg void OnSetupDel();
	afx_msg void OnTrashRecover();
	afx_msg void OnTrashClear();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL ContextMenu(CPoint point);
	BOOL UpdateForm();	// リストの生成・更新
	BOOL ChkSetup(CString strSender);	// 応答設定送信者名チェック
	BOOL SelSetup(CString strSender);	// 対応する応答設定の表示
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RSFLIST_H__64AA0262_2D7C_4E02_A9B1_E1C68488FAA8__INCLUDED_)
