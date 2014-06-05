// MsgTree.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "MsgTree.h"
#include "MsgList.h"
#include "MsgFolderNewDialog.h"
#include "MsgFolderRenameDialog.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgTree

IMPLEMENT_DYNCREATE(CMsgTree, CTreeView)

CMsgTree::CMsgTree()
{
}

CMsgTree::~CMsgTree()
{
}


BEGIN_MESSAGE_MAP(CMsgTree, CTreeView)
	//{{AFX_MSG_MAP(CMsgTree)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_POPUP_FOLDER_NEW, OnFolderNew)
	ON_COMMAND(ID_POPUP_FOLDER_RENAME, OnFolderRename)
	ON_COMMAND(ID_POPUP_FOLDER_DEL, OnFolderDel)
	ON_COMMAND(ID_POPUP_TRASHCAN_CLEAR, OnTrashCanClear)
	ON_COMMAND(ID_POPUP_TRASH_RECOVER, OnTrashRecover)
	ON_COMMAND(ID_POPUP_TRASH_CLEAR, OnTrashClear)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgTree 描画

void CMsgTree::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: この位置に描画用のコードを追加してください
}

/////////////////////////////////////////////////////////////////////////////
// CMsgTree 診断

#ifdef _DEBUG
void CMsgTree::AssertValid() const
{
	CTreeView::AssertValid();
}

void CMsgTree::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMsgTree メッセージ ハンドラ

int CMsgTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください

	// Set tree control styles
	CTreeCtrl &tc = GetTreeCtrl();
	long style = GetWindowLong(tc.m_hWnd,GWL_STYLE);
	style |= (TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | 
			TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP);
	SetWindowLong(tc.m_hWnd,GWL_STYLE,style);

	// アイコン設定
	HICON h;
	m_imgl.Create(16, 16, ILC_COLORDDB, 0, 0);
	h = AfxGetApp()->LoadIcon(IDI_F_ROOT);
	m_imgl.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_F_MSG_CLOSE);
	m_imgl.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_F_MSG_OPEN);
	m_imgl.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_F_MSG_TRASH);
	m_imgl.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_F_RESP_CLOSE);
	m_imgl.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_F_RESP_OPEN);
	m_imgl.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_F_RESP_TRASH);
	m_imgl.Add(h);
	tc.SetImageList(&m_imgl, TVSIL_NORMAL);

	return 0;
}

void CMsgTree::OnDestroy() 
{
	CTreeView::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
/*
	/// ツリーの展開状態をレジストリに保存
	DOWRD dw;
	CTreeCtrl &tc = GetTreeCtrl();
	if (dw == 1) tc.Expand(m_hRoot, TVE_EXPAND);

	m_rgRootStatus = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\RootFolderStatus", 1);
	m_rgMsgDefStatus = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\MsgFolderDefStatus", 1);
	m_rgMsgTrashDefStatus = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\MsgFolderTrashDefStatus", 1);
	m_rgSetupDefStatus = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\SetupFolderDefStatus", 1);
	m_rgSetupTrashDefStatus = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\SetupFolderTrashDefStatus", 1);
	dw = m_rgRootStatus;
	if (dw == 1) tc.Expand(m_hRoot, TVE_EXPAND);
	dw = m_rgMsgDefStatus;
	if (dw == 1) tc.Expand(m_hFolderDef, TVE_EXPAND);
	dw = m_rgMsgTrashDefStatus;
	if (dw == 1) tc.Expand(m_hTrashDef, TVE_EXPAND);
	dw = m_rgSetupDefStatus;
	if (dw == 1) tc.Expand(m_hSetupDef, TVE_EXPAND);
	dw = m_rgSetupTrashDefStatus;
	if (dw == 1) tc.Expand(m_hSetupTrashDef, TVE_EXPAND);
*/
}

// ツリーアイテムの選択変更時イベント
void CMsgTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    switch( pNMTreeView->action ) {
	case TVC_UNKNOWN:	 // プログラム
	case TVC_BYKEYBOARD: // キーボード操作
    case TVC_BYMOUSE:    // マウス操作
		FolderChange();
        break;
    }
	
	*pResult = 0;
}

// ツリーの開閉時イベント
void CMsgTree::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    switch( pNMTreeView->action ) {
    case TVE_COLLAPSE:  // 折りたたみ時
		FolderChange();
		break;
    }

	*pResult = 0;
}

// OnContextMenu(), OnRButtonUp()は認識しないため、このハンドルを使用
void CMsgTree::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	/// マウスY座標が正常に取得されないため、再計算する
	POINT pnt;
//	RECT rct;
	GetCursorPos(&pnt);
	ScreenToClient(&pnt);
//	GetWindowRect(&rct);
//	pNMTreeView->ptDrag.y = pnt.y - rct.top;
	/// コンテキストメニューの表示
	ContextMenu(pnt);
//	ContextMenu(pNMTreeView->ptDrag);

	*pResult = 0;
}

// キーイベント
void CMsgTree::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	RECT rct;
	CPoint point, point2;
	/// TAB KEY : リストフォームへフォーカス移動
	/// リストが空の場合はフォーカス移動しない
	if (pTVKeyDown->wVKey == VK_TAB) {
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CMsgList *pfview = (CMsgList *)pFrame->m_pMsgList->GetWnd();
		CReportCtrl &rc = pfview->m_wndList;
		if (rc.GetItemCount() > 0) {
			//// リストの先頭行を選択
			// (?)以下の方法ではメッセージリストのコンテキストメニューのコマンドが実行されない
			// 擬似的にマウスを押下して対応
//			pfview->SetFocus();
//			rc.SetAllItemStates(RC_ITEM_ALL, RC_ITEM_UNSELECTED);
//			rc.SetItemStates(0, RC_ITEM_SELECTED | RC_ITEM_FOCUSED);
			rc.GetItemRect(0, &rct, 0);
			point.x = (rct.left + rct.right) / 2 + 150;		// (?) Xだけずれる
			point.y = (rct.top + rct.bottom) / 2;
			ClientToScreen(&point);
			SetCursorPos(point.x, point.y);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
	} else if (pTVKeyDown->wVKey == VK_RETURN) {
	/// RETURN KEY : コンテキストメニューの表示
		/// 現在選択されているアイテム位置で表示
		CTreeCtrl &tc = GetTreeCtrl();
		m_hContext = tc.GetSelectedItem();
		if (m_hContext > 0) {
			tc.GetItemRect(m_hContext, &rct, 0);
			point.x = (rct.left + rct.right) / 2;
			point.y = (rct.top + rct.bottom) / 2;
			ContextMenu(point);
		}
	}
	
	*pResult = 0;
}

// フォルダ追加ダイアログ
void CMsgTree::OnFolderNew()
{
	CMsgFolderNewDialog dlg;
	dlg.DoModal();
}

// フォルダ名称変更ダイアログ
void CMsgTree::OnFolderRename()
{
	CMsgFolderRenameDialog dlg;
	dlg.DoModal();
}

// フォルダ削除
void CMsgTree::OnFolderDel()
{
	if (m_dwFolderType == FD_NORMAL) {
		FolderDel();
	}
}

// ごみ箱クリア
void CMsgTree::OnTrashCanClear()
{
	if (m_dwFolderType == FD_TRASH_DEF) {
		TrashClear();
	}
}

// ごみ箱内のフォルダ削除
void CMsgTree::OnTrashClear()
{
	if (m_dwFolderType == FD_TRASH) {
		TrashDel();
	}
}

// ごみ箱内のフォルダを元に戻す
void CMsgTree::OnTrashRecover()
{
	if (m_dwFolderType == FD_TRASH) {
		TrashRecover();
	}
}

/////////////////////////////////////////////////////////////////////////////

//
//  機能     : コンテキストメニューの表示
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgTree::ContextMenu(
CPoint point)	// マウス位置
{
	/// マウス上のフォルダを選択
	CTreeCtrl &tc = GetTreeCtrl();
	UINT intFlag;
	m_hContext = tc.HitTest(point, &intFlag);
	if (m_hContext <= 0) {
		/// マウス上にない場合現在選択されているアイテム
		m_hContext = tc.GetSelectedItem();
		if (m_hContext <= 0) return FALSE;
	}
	m_dwFolderType = tc.GetItemData(m_hContext) & FD_MASK_TYPE;
	m_dwFolderId = tc.GetItemData(m_hContext) & FD_MASK_ID;
	m_strFolderName = tc.GetItemText(m_hContext);

	/// POPUPメニュー表示
	CMenu menu;
	CMenu* pPopup;
	if (m_dwFolderType == FD_NORMAL || m_dwFolderType == FD_NORMAL_DEF) { 
		VERIFY(menu.LoadMenu(IDR_POPUP_FOLDER));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		if (m_dwFolderType == FD_NORMAL_DEF) {
			pPopup->EnableMenuItem(ID_POPUP_FOLDER_RENAME, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_POPUP_FOLDER_DEL, MF_BYCOMMAND | MF_GRAYED);
		}
	} else if (m_dwFolderType == FD_TRASH_DEF) {
		VERIFY(menu.LoadMenu(IDR_POPUP_TRASHCAN));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
	} else if (m_dwFolderType == FD_TRASH) {
		VERIFY(menu.LoadMenu(IDR_POPUP_TRASH));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
	} else {
		return FALSE;
	}
	
	ClientToScreen(&point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		 point.x, point.y, AfxGetMainWnd());

	return TRUE;
}

//
//  機能     : ツリー項目の追加
//  
//  機能説明 : 
//  
//  返り値   : 生成ツリー項目のハンドル
//  
//  備考     : 
//  
HTREEITEM CMsgTree::AddItem(
HTREEITEM hParent,	// 親項目ハンドル
CString strTitle,	// タイトル
DWORD type,			// フォルダタイプ
DWORD recId = 0)	// レコードID
{
	CString str;
	HTREEITEM h;
	CTreeCtrl &tc = GetTreeCtrl();

	if (type == FD_ROOT) {
		h = tc.InsertItem(strTitle);
		tc.SetItemImage(h, 0, 0);
	} else if (type == FD_NORMAL) {
		h = tc.InsertItem(strTitle, hParent, TVI_FIRST);
		tc.SetItemImage(h, 1, 2);
	} else if (type == FD_NORMAL_DEF) {
		if (strTitle.GetLength() > 0) {
			str.LoadString(IDS_FOLDER_MSG);
			strTitle = str + strTitle;
		} else {
			strTitle.LoadString(IDS_FOLDER_MSG);
		}
		h = tc.InsertItem(strTitle, hParent);
		tc.SetItemImage(h, 1, 2);
	} else if (type == FD_TRASH) {
		h = tc.InsertItem(strTitle, hParent);
		tc.SetItemImage(h, 1, 2);
	} else if (type == FD_TRASH_DEF) {
		if (strTitle.GetLength() > 0) {
			str.LoadString(IDS_FOLDER_MSG_TRASH);
			strTitle = str + strTitle;
		} else {
			strTitle.LoadString(IDS_FOLDER_MSG_TRASH);	
		}
		h = tc.InsertItem(strTitle, hParent);
		tc.SetItemImage(h, 3, 3);
	}
	tc.SetItemData(h, (type | recId));

	return h;
}

//
//  機能     : ツリーのフォルダ一覧生成
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgTree::MakeSubFolders(
DWORD type)			// フォルダタイプ
{
	HTREEITEM hParent;
	CString strFolderType;
	CString strSystemFlag;

	if (type == FD_NORMAL) {
		hParent = m_hFolderDef;
		strFolderType = _T("0");
		strSystemFlag = _T("0");
	} else if (type == FD_TRASH) {
		hParent = m_hTrashDef;
		strFolderType = _T("0");
		strSystemFlag = _T("1");
	} else {
		return FALSE;
	}

	/// フォルダ一覧取得
	CString strWhere;
	strWhere.Empty();
	strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
	strWhere = strWhere + _T(" and ") + _T("lngFolderType") + _T("=") + strFolderType;
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag") + _T("=") + strSystemFlag;
	strWhere = strWhere + _T(" order by strTitle");

	//// データベースからfolderテーブル参照
	CString str;
	long lngCnt, lngMsgCnt;
	CTblFolder tbl(theApp.m_strDBPath);
	lngCnt = tbl.SelectRec(strWhere);
	while (lngCnt > 0)
	{
		str = tbl.strTitle;
		if (type == FD_NORMAL || type == FD_TRASH) {
			lngMsgCnt = GetUnreadMsgCnt(type, tbl.ID);
			if (lngMsgCnt > 0) {
				str.Format(_T("%s [%d]"), tbl.strTitle, lngMsgCnt);
			}
		}
		AddItem(hParent, str, type, tbl.ID);
		if (!tbl.NextRec()) break;
		--lngCnt;
	}
		
	return TRUE;
}

//
//  機能     : ツリーの生成・更新
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgTree::UpdateForm(
BOOL blForce)	// 強制更新
{
	CString str;
	long lngMsgCnt;

	/// フォルダが存在する場合はそのまま
	CTreeCtrl &tc = GetTreeCtrl();
	if (!blForce) {
		if (!(tc.GetCount() == 0 && _tcsclen(theApp.m_strUsrId) > 0)) return TRUE;
	}

	/// フォルダをクリア
	tc.DeleteAllItems();

	/// 受信者（root）
	if (theApp.m_strUsrName.IsEmpty()) {
		m_hRoot = AddItem(NULL, theApp.m_strUsrId, FD_ROOT);
	} else {
		m_hRoot = AddItem(NULL, theApp.m_strUsrName, FD_ROOT);
	}
	/// "受信箱"
	str.Empty();
	lngMsgCnt = GetUnreadMsgCnt(FD_NORMAL_DEF);
	if (lngMsgCnt > 0) str.Format(_T(" [%d]"), lngMsgCnt);
	m_hFolderDef = AddItem(m_hRoot, str, FD_NORMAL_DEF);
	/// "ごみ箱"
	str.Empty();
	lngMsgCnt = GetUnreadMsgCnt(FD_TRASH_DEF);
	if (lngMsgCnt > 0) str.Format(_T(" [%d]"), lngMsgCnt);
	m_hTrashDef = AddItem(m_hRoot, str, FD_TRASH_DEF);
	/// 受信データ用フォルダ一覧
	MakeSubFolders(FD_NORMAL);
	/// 受信データ用ごみ箱フォルダ一覧
	MakeSubFolders(FD_TRASH);

	/// ツリー展開(レジストリより取得)
	DWORD dw;
	m_rgRootStatus = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\RootFolderStatus"), 1, TRUE);
	m_rgFolderDefStatus = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\MsgFolderDefStatus"), 1, TRUE);
	m_rgTrashDefStatus = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\MsgFolderTrashDefStatus"), 1, TRUE);
	dw = m_rgRootStatus;
	if (dw == 1) tc.Expand(m_hRoot, TVE_EXPAND);
	dw = m_rgFolderDefStatus;
	if (dw == 1) tc.Expand(m_hFolderDef, TVE_EXPAND);
	dw = m_rgTrashDefStatus;
	if (dw == 1) tc.Expand(m_hTrashDef, TVE_EXPAND);

	/// 受信箱をアクティブにする
	tc.SelectItem(m_hFolderDef);
		
	return TRUE;
}

//
//  機能     : 指定フォルダの未読メッセージ数
//  
//  機能説明 : 
//  
//  返り値   : 未読メッセージ数、-1:error
//  
//  備考     : 
//  
long CMsgTree::GetUnreadMsgCnt(
DWORD lngFolderType,	// フォルダタイプ
long lngFolderId)		// フォルダID
{
	_TCHAR buf[32];

// 開発途中
return 0;

	CTblRdfl tbl(theApp.m_strDBPath);

	/// データベースから未読メッセージ数検索
	CString strWhere;
	strWhere.Empty();
	switch(lngFolderType) {
	case FD_NORMAL:
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T(" and ") + _T("lngFolderId") + _T("=") + buf;
		strWhere = strWhere + _T(" and ") + _T("lngViewFlag=0");
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
		break;
	case FD_NORMAL_DEF:
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		strWhere = strWhere + _T(" and ") + _T("lngFolderId=0");
		strWhere = strWhere + _T(" and ") + _T("lngViewFlag=0");
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
		break;
	case FD_TRASH:
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		 _ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T(" and ") + _T("lngFolderId") + _T("=") + buf;
		strWhere = strWhere + _T(" and ") + _T("lngViewFlag=0");
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
		break;
	case FD_TRASH_DEF:
		//// 該当するフォルダがごみ箱にないか、削除されていない場合
		tbl.OpenTbl(_T("rdfl left outer join folder on rdfl.lngFolderId=folder.ID"));
		strWhere = strWhere + _T("rdfl.strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		strWhere = strWhere + _T(" and ") + _T("(folder.ID is NULL or folder.lngSystemFlag=0)");
		strWhere = strWhere + _T(" and ") + _T("rdfl.lngViewFlag=0");
		strWhere = strWhere + _T(" and ") + _T("rdfl.lngSystemFlag=1");
		break;
	default:
		return -1;
	}

	long lngCnt = tbl.CountRec(strWhere);

	return lngCnt;
}

//
//  機能     : フォルダ変更時処理
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgTree::FolderChange(void) 
{
	CString str;

	/// カレントフォルダの取得
	CTreeCtrl &tc = GetTreeCtrl();
	m_hActive = tc.GetSelectedItem();
	str = tc.GetItemText(m_hActive);
	theApp.m_strActiveFolder = str;
	m_dwFolderType = tc.GetItemData(m_hActive) & FD_MASK_TYPE;
	m_dwFolderId = tc.GetItemData(m_hActive) & FD_MASK_ID;

	/// メッセージリストの更新
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgList *pfview = (CMsgList *)pFrame->m_pMsgList->GetWnd();
	pfview->UpdateForm();

	/// メッセージリストが空の場合、フォルダタブ表示
	CReportCtrl &rc = pfview->m_wndList;
	if (rc.GetItemCount() <= 0) {
		//// タブラベルをフォルダ名にする
		HTREEITEM h = m_hActive;
		while (h != m_hRoot) {
			h = tc.GetParentItem(h);
			if(h == m_hRoot) break;
			str.Insert(0, _T(" - "));
			str.Insert(0, tc.GetItemText(h));
		}
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		pFrame->m_Framework.ShowTab(pFrame->m_pMsgFolderForm, TRUE);
		pFrame->m_Framework.SetTabLabel(pFrame->m_pMsgFolderForm, str);
		pFrame->m_Framework.SetActiveTab(pFrame->m_pMsgFolderForm);
		pFrame->m_Framework.ShowTab(pFrame->m_pMsgForm, FALSE);
	}

	/// フォーカス再設定（メッセージリストの更新で一旦はずれるため）
	this->SetFocus();

	return TRUE;
}

//
//  機能     : 受信フォルダ削除
//  
//  機能説明 : 削除フラグを付加
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgTree::FolderDel()
{
	CString str;

	/// 仮選択フォルダ情報
	CString strTitle = m_strFolderName;
	long lngFolderId = m_dwFolderId;

	/// 確認メッセージ
	str.LoadString(IDS_MSGBOX23);
	str = strTitle + _T(" : ") + str;
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;

	/// folderテーブルを更新
	CTblFolder ftbl(theApp.m_strDBPath);
	if (ftbl.SelectRec(lngFolderId) != 1) {
		return FALSE;
	}
	ftbl.lngSystemFlag = 1;				// 削除フラグ付加
	ftbl.UpdateRec(lngFolderId);

	/// フォルダ内メッセージを更新
	_TCHAR buf[32];
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	CString strFolderId = buf;
	CTblRdfl dtbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T("lngFolderId") + _T("=") + buf;
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
	dtbl.UpdateRec(_T("lngSystemFlag=1"), strWhere);	// 削除フラグ付加

	/// 画面更新
	//// ツリー項目の削除
	CTreeCtrl &tc = GetTreeCtrl();
	tc.DeleteItem(m_hContext);
	//// ごみ箱フォルダに追加
	AddItem(m_hTrashDef, strTitle, FD_TRASH, lngFolderId);
	//// 選択フォルダを削除した場合、デフォルトフォルダを再選択
	if (m_hContext == m_hActive) {
		tc.SelectItem(m_hFolderDef);
	}

	return TRUE;
}

//
//  機能     : ごみ箱(受信)クリア
//  
//  機能説明 : テーブルから削除
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgTree::TrashClear()
{
	CString str;

	/// 確認メッセージ
	str.LoadString(IDS_MSGBOX31);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;

	/// フォルダを削除
	CTblFolder ftbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	strWhere = strWhere + _T("lngFolderType=0");
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
	ftbl.DeleteRec(strWhere);

	/// メッセージを削除
	strWhere = _T("lngSystemFlag=1");
	SqlClear(strWhere);

	/// 画面更新
	//// ツリー項目の削除
	BOOL blFlag = FALSE;
	HTREEITEM h, hTrashDef;
	hTrashDef = m_hTrashDef;
	CTreeCtrl &tc = GetTreeCtrl();
	h = tc.GetChildItem(hTrashDef);
	while (h != NULL) {
		if (h == m_hActive) blFlag = TRUE;
		tc.DeleteItem(h);
		h = tc.GetChildItem(hTrashDef);
	}
	if (blFlag) {
		//// 選択フォルダを削除した場合、ごみ箱フォルダを再選択
		tc.SelectItem(hTrashDef);
	} else {
		//// ごみ箱フォルダを選択していた場合メッセージリスト更新
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CMsgList *pfview = (CMsgList *)pFrame->m_pMsgList->GetWnd();
		pfview->UpdateForm();
	}

	return TRUE;
}

//
//  機能     : ごみ箱(受信)内のフォルダ削除
//  
//  機能説明 : テーブルから削除
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgTree::TrashDel()
{
	CString str;
	_TCHAR buf[32];

	/// 仮選択フォルダ情報
	CString strTitle = m_strFolderName;
	long lngFolderId = m_dwFolderId;

	/// 確認メッセージ
	str.LoadString(IDS_MSGBOX32);
	str = strTitle + _T(" : ") + str;
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;

	/// フォルダを削除
	CTblFolder ftbl(theApp.m_strDBPath);
	ftbl.DeleteRec(lngFolderId);

	/// フォルダ内メッセージを削除
	CString strWhere;
	strWhere.Empty();
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T("lngFolderId") + _T("=") + buf;
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
	SqlClear(strWhere);

	/// 画面更新
	//// ツリー項目の削除
	CTreeCtrl &tc = GetTreeCtrl();
	tc.DeleteItem(m_hContext);
	//// 選択フォルダを削除した場合、ごみ箱フォルダを再選択
	if (m_hContext == m_hActive) {
		tc.SelectItem(m_hTrashDef);
	}

	return TRUE;
}

//
//  機能     : 受信データの完全削除
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgTree::SqlClear(
CString strWhere)	// 削除対象データ選択用WHERE句
{
	int i;
	CString str;
	CFileSpec fs;
	int intCnt;

	CTblRdfl dtbl(theApp.m_strDBPath);

	//// 音声・ビデオファイルの削除
	CTPsetup tps(theApp.m_strDataFolder);
	intCnt = dtbl.SelectRec(strWhere);
	for (i = 0; i < intCnt; i++) {
		tps.SetCallerId(dtbl.strSenderId);
		///// 音声ファイルの削除
		str = tps.FindRdfWavFileName(dtbl.strRemarks1, dtbl.dlngSystemNew, _T("$$"));
		if (!str.IsEmpty()) {
			fs.SetFullSpec(str);
			fs.FileDelete();
		}
		str = tps.FindRdfMp3FileName(dtbl.strRemarks1, dtbl.dlngSystemNew, _T("$$"));
		if (!str.IsEmpty()) {
			fs.SetFullSpec(str);
			fs.FileDelete();
		}
		///// ビデオファイルの削除
		str = tps.FindRdfVideoFileName(dtbl.strRemarks1, dtbl.dlngSystemNew, _T("$$"));
		if (!str.IsEmpty()) {
			fs.SetFullSpec(str);
			fs.FileDelete();
		}
		dtbl.NextRec();
	}

	//// レコードの削除
	dtbl.DeleteRec(strWhere);

	return TRUE;
}

//
//  機能     : ごみ箱(受信)内のフォルダを元に戻す
//  
//  機能説明 : 削除フラグを解除
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgTree::TrashRecover()
{
	CString str;

	/// 仮選択フォルダ情報
	CString strTitle = m_strFolderName;
	long lngFolderId = m_dwFolderId;

	/// 確認メッセージ
	str.LoadString(IDS_MSGBOX33);
	str = strTitle + _T(" : ") + str;
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;

	/// folderテーブルを更新
	CTblFolder ftbl(theApp.m_strDBPath);
	if (ftbl.SelectRec(lngFolderId) != 1) {
		return FALSE;
	}
	ftbl.lngSystemFlag = 0;				// 削除フラグ解除
	ftbl.UpdateRec(lngFolderId);

	/// フォルダ内メッセージを更新
	_TCHAR buf[32];
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	CString strFolderId = buf;
	CTblRdfl dtbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T("lngFolderId") + _T("=") + buf;
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
	dtbl.UpdateRec(_T("lngSystemFlag=0"), strWhere);	// 削除フラグ解除

	/// 画面更新
	//// ツリー項目の削除
	CTreeCtrl &tc = GetTreeCtrl();
	tc.DeleteItem(m_hContext);
	//// 元のフォルダに追加
	AddItem(m_hFolderDef, strTitle, FD_NORMAL, lngFolderId);
	//// 選択フォルダを削除した場合、ごみ箱フォルダを再選択
	if (m_hContext == m_hActive) {
		tc.SelectItem(m_hTrashDef);
	}

	return TRUE;
}

//
//  機能     : フォルダ名チェック
//  
//  機能説明 : ごみ箱内のフォルダも対象
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgTree::ChkFolder(
CString strFolder,	// フォルダ名
DWORD type)			// フォルダタイプ
{
	CString str;

	if (strFolder.IsEmpty()) {
		str.LoadString(IDS_MSGBOX22);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	/// 既存フォルダとの重複チェック
	//// "受信箱"、"ごみ箱(受信)"、"応答設定"、"ごみ箱(応答)"は禁止
	CString str1, str2, str3, str4;
	str1.LoadString(IDS_FOLDER_MSG);
	str2.LoadString(IDS_FOLDER_MSG_TRASH);	
	str3.LoadString(IDS_FOLDER_SETUP);
	str4.LoadString(IDS_FOLDER_SETUP_TRASH);	
	if (str1 == strFolder || str2 == strFolder || str3 == strFolder || str4 == strFolder) {
//	if (theApp.m_strUsrId == strFolder || str1 == strFolder || str2 == strFolder) {
		str.LoadString(IDS_MSGBOX21);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}
	//// その他のフォルダ名
	CTblFolder tbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
	strWhere = strWhere + _T(" and ") + _T("strTitle") + _T("='") + strFolder + _T("'");
	if (type == FD_NORMAL || type == FD_NORMAL_DEF) {
		strWhere = strWhere + _T(" and ") + _T("lngFolderType=0");
	}
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
	long lngCnt = tbl.CountRec(strWhere);
	if (lngCnt > 0) {
		str.LoadString(IDS_MSGBOX21);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : IDによるフォルダ検索
//  
//  機能説明 : ごみ箱内のフォルダも対象
//  
//  返り値   : HTREEITEM
//  
//  備考     : 
//  
HTREEITEM CMsgTree::FindItemId(HTREEITEM hti, long lngId)
{
	CTreeCtrl &tc = GetTreeCtrl();

	if(hti == NULL) return NULL;
	if((tc.GetItemData(hti) & FD_MASK_ID) == lngId) return hti;

	hti = tc.GetChildItem( hti );
	do
	{
		HTREEITEM hti_res;
		if((hti_res = FindItemId( hti, lngId)) != NULL ) return hti_res;
	} while( (hti = tc.GetNextSiblingItem( hti )) != NULL );

	return NULL;
}

//
//  機能     : アイテムデータにによるフォルダ検索
//  
//  機能説明 : 
//  
//  返り値   : HTREEITEM
//  
//  備考     : 
//  
HTREEITEM CMsgTree::FindItemData(HTREEITEM hti, DWORD dwData)
{
	CTreeCtrl &tc = GetTreeCtrl();

	if(hti == NULL) return NULL;
	if(tc.GetItemData( hti ) == dwData) return hti;

	hti = tc.GetChildItem( hti );
	do
	{
		HTREEITEM hti_res;
		if((hti_res = FindItemData( hti, dwData)) != NULL ) return hti_res;
	} while( (hti = tc.GetNextSiblingItem( hti )) != NULL );

	return NULL;
}

