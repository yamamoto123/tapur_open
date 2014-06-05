#if !defined(AFX_RSFTREE_H__F4F2B8E9_082C_475A_845A_60D4562FF840__INCLUDED_)
#define AFX_RSFTREE_H__F4F2B8E9_082C_475A_845A_60D4562FF840__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfTree.h : ヘッダー ファイル
//
#include <afxcview.h>
#include "TPerror.h"

/////////////////////////////////////////////////////////////////////////////
// CRsfTree ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

// フォルダ種別(DWORD上位１６ビット排他)
#define	FD_MASK_ID			0x0000ffff
#define	FD_MASK_TYPE		0xffff0000
#define	FD_ROOT				0x00010000
#define	FD_NORMAL			0x00020000					
#define	FD_NORMAL_DEF		0x00040000					
#define	FD_TRASH			0x00080000	
#define	FD_TRASH_DEF		0x00100000	

class CRsfTree : public CTreeView
{
protected:
	CRsfTree();           // 動的生成に使用されるプロテクト コンストラクタ
	DECLARE_DYNCREATE(CRsfTree)

// アトリビュート
public:
	HTREEITEM m_hRoot;			// ルートアイテム
	HTREEITEM m_hFolderDef;		// 応答設定アイテム
	HTREEITEM m_hTrashDef;		// ごみ箱（設定）アイテム
	HTREEITEM m_hActive;		// 選択アイテム
	HTREEITEM m_hContext;		// コンテキストメニュー時の（仮）選択アイテム
	DWORD m_dwFolderType;	// コンテキストメニュー時のフォルダタイプ
	DWORD m_dwFolderId;		// コンテキストメニュー時のフォルダID
	CString m_strFolderName;// コンテキストメニュー時のフォルダ名
	// ツリーの展開状態
	CRegDWORD m_rgRootStatus;
	CRegDWORD m_rgFolderDefStatus;
	CRegDWORD m_rgTrashDefStatus;

private:
	CTPerror te;
	CImageList m_imgl;

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CRsfTree)
	protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	//}}AFX_VIRTUAL

private:
	BOOL FolderChange(void);			// フォルダ変更時処理
	BOOL MakeSubFolders(DWORD type);	// ツリーのフォルダ一覧生成
	BOOL SqlClear(CString strWhere);	// WHERE句による応答データの削除

// インプリメンテーション
protected:
	virtual ~CRsfTree();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CRsfTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetupNew();
	afx_msg void OnFolderNew();
	afx_msg void OnFolderRename();
	afx_msg void OnFolderDel();
	afx_msg void OnTrashCanClear();
	afx_msg void OnTrashRecover();
	afx_msg void OnTrashClear();
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL ContextMenu(CPoint point);
	BOOL FolderDel();	// (応答)フォルダ削除
	BOOL TrashClear();	// ごみ箱(応答)クリア
	BOOL TrashDel();	// ごみ箱(応答)内のフォルダ削除
	BOOL TrashRecover();	// ごみ箱(応答)内のフォルダを元に戻す

	BOOL UpdateForm(BOOL blForce = FALSE);		// ツリーの生成・更新
	HTREEITEM AddItem(HTREEITEM hParent, CString strTitle, DWORD type, DWORD recId);	// ツリー項目の追加
	BOOL ChkFolder(CString strFolder, DWORD type);		// フォルダチェック
	HTREEITEM FindItemId(HTREEITEM hti, long lngId);		// IDによるフォルダ検索
	HTREEITEM FindItemData(HTREEITEM hti, DWORD dwData);	// データによるフォルダ検索
	BOOL IsTrash(DWORD type) {
		return (type == FD_TRASH_DEF || type == FD_TRASH) ? TRUE : FALSE; };
	BOOL IsFolder(DWORD type) {
		return (type == FD_NORMAL_DEF || type == FD_NORMAL) ? TRUE : FALSE; };
	BOOL IsFolderWT(DWORD type) {
		return (type == FD_NORMAL_DEF || type == FD_NORMAL ||
			type == FD_TRASH_DEF || type == FD_TRASH) ? TRUE : FALSE; };
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RSFTREE_H__F4F2B8E9_082C_475A_845A_60D4562FF840__INCLUDED_)
