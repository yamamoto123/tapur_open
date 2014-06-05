#if !defined(AFX_MSGLIST_H__64AA0262_2D7C_4E02_A9B1_E1C68488FAA8__INCLUDED_)
#define AFX_MSGLIST_H__64AA0262_2D7C_4E02_A9B1_E1C68488FAA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgList.h : ヘッダー ファイル
//
//#include "XListCtrl.h"
#include "ReportCtrl.h"
#include "CppSQLite3.h"
#include "TPerror.h"

/////////////////////////////////////////////////////////////////////////////
// CMsgList フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMsgList : public TVisualFormView
{
protected:
	CMsgList();           // 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CMsgList)

// フォーム データ
public:
	//{{AFX_DATA(CMsgList)
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

	COLORREF m_clrUnview;	// 未再生リストカラー

// オペレーション
public:

private:
	CTPerror te;
	void InitListCtrl(void);
	void AdustColumns(void);	// リストのカラムサイズ調整
	BOOL MakeList(DWORD lngFolderType, long lngFolderId);	// メッセージリスト生成
	BOOL Move(long lngFolderId);		// 選択されたメッセージを移動
	BOOL TrashDel();		// ごみ箱(受信)内のアイテム削除
	BOOL TrashRecover();	// ごみ箱(受信)内のアイテムを元に戻す
	long SelectedList(CUIntArray *pAryIdx, BOOL blData);						// メッセージ選択リストの取得
	CPoint CnvPntM_F(const CPoint point);	// クライアント座標変換（MsgList->FolderTree）
	BOOL Play();	// 音声再生
	BOOL ExecPrg(int intPrgId);		// 連携プログラム実行
	BOOL ChkPlayIcon(const CPoint point);	// 指定座標が音声再生アイコン上にあるか
	BOOL ExeProcess(LPTSTR cmdstr, LPTSTR dirstr, int nShowCmd, DWORD dwTimeout);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMsgList)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CMsgList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMsgList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMsgCall();
	afx_msg void OnLinkRebtel();
	afx_msg void OnMsgSetup();
	afx_msg void OnMsgDel();
	afx_msg void OnPrg1();
	afx_msg void OnPrg11();
	afx_msg void OnPrg901();
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
	BOOL ChgMsgMark(BOOL blUpdate = TRUE);	// 未再生リストを再生済みに変更
	long SelectedId();			// 選択リストIDの取得
	BOOL SelectId(int idx);	// リスト選択
  };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MSGLIST_H__64AA0262_2D7C_4E02_A9B1_E1C68488FAA8__INCLUDED_)
