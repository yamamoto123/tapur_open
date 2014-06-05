// RsfList.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "TPutil.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"
#include "MainFrm.h"
#include "RsfList.h"
#include "RsfTree.h"
#include "RsfSimpleForm.h"
#include "RsfNewDialog.h"
#include "RsfRenameDialog.h"
#include "SkypeEvents.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CRsfList

IMPLEMENT_DYNCREATE(CRsfList, TVisualFormView)

CRsfList::CRsfList()
	: TVisualFormView(CRsfList::IDD)
{
	//{{AFX_DATA_INIT(CRsfList)
		// メモ: ClassWizard はこの位置にメンバの初期化処理を追加します
	//}}AFX_DATA_INIT

	m_bDrag = FALSE;
	m_hItemDrag = 0;
}

CRsfList::~CRsfList()
{
}

void CRsfList::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfList)
		// メモ: ClassWizard はこの位置に DDX および DDV の呼び出しを追加します
	DDX_Control(pDX, IDC_LIST, m_wndList);	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRsfList, TVisualFormView)
	//{{AFX_MSG_MAP(CRsfList)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchanged)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST, OnBegindrag)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_SETLIST_NEW, OnSetupNew)
	ON_COMMAND(ID_POPUP_SETLIST_RENAME, OnSetupRename)
	ON_COMMAND(ID_POPUP_SETLIST_COPY, OnSetupCopy)
	ON_COMMAND(ID_POPUP_SETLIST_PASTE, OnSetupPaste)
	ON_COMMAND(ID_POPUP_SETLIST_CALL, OnSetupCall)
	ON_COMMAND(ID_POPUP_SETLIST_DEL, OnSetupDel)
	ON_COMMAND(ID_POPUP_TRASH_RECOVER, OnTrashRecover)
	ON_COMMAND(ID_POPUP_TRASH_CLEAR, OnTrashClear)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnLButtonDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnLButtonDblClk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfList 診断

#ifdef _DEBUG
void CRsfList::AssertValid() const
{
	CFormView::AssertValid();
}

void CRsfList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRsfList メッセージ ハンドラ

int CRsfList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (TVisualFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください
	
	m_aryClip.RemoveAll();
	//m_arySelect.RemoveAll();

	/// Set control styles
/* リソースの設定が優先されるので無効
	CListCtrl *lc = &m_wndList;
	long style = GetWindowLong(lc->m_hWnd,GWL_STYLE);
	style |= (LVS_SHOWSELALWAYS | LVS_REPORT);
	SetWindowLong(lc->m_hWnd,GWL_STYLE,style);
*/	
	/// ドラッグアイコン設定
	HICON h;
	m_DragImage.Create(16, 16, ILC_COLORDDB, 0, 0);
	h = AfxGetApp()->LoadIcon(IDI_F_MSG_DRAG);
	m_DragImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_F_RESP_DRAG);
	m_DragImage.Add(h);

	/// リストアイコン設定
	m_ListImage.Create(16, 16, ILC_COLORDDB, 0, 0);
	h = AfxGetApp()->LoadIcon(IDI_MSG_SIMPLE);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_VMSG_SIMPLE);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_MSG_RECORD);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_VMSG_RECORD);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_RESP_SIMPLE);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_MSG_IN);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_MSG_OUT);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_VMSG_IN);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_VMSG_OUT);
	m_ListImage.Add(h);

	/// 色設定
//	m_clrUnview = RGB(210, 255, 210);	// light green

	return 0;
}

void CRsfList::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit();
	
//	srand((unsigned)time(NULL));

	InitListCtrl();
}

void CRsfList::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	// リストビューをウィンドウにフィットさせる
    m_wndList.ResizeToFitParent();
	
	// リストのカラムサイズ調整
	AdustColumns();
}

// リスト選択時イベント
void CRsfList::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int iItem = -1;
	int iSubItem = -1;
	UINT state;
	CRsfSimpleForm *prsv;
	*pResult = 0;

	if (!pNMLV) return;
	iItem = pNMLV->iItem;
	iSubItem = pNMLV->iSubItem;
	if (iItem < 0 || iSubItem < 0) return;
	if (m_wndList.GetSelectedCount() <= 0) return;

/*
	/// 左ボタンでも選択されるので、ここで解除する。
	int iFocus;
	short sht;
	sht = GetAsyncKeyState(VK_RBUTTON);
	if (sht & 0x80000000) {
		iFocus = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
		if( iFocus != -1 ) {
			m_wndList.SetItemState(iFocus, ~LVIS_SELECTED, LVIS_SELECTED);
			goto PROC_END;
		}
	}
*/
	state = pNMLV->uNewState;
	/// リスト選択時
	if ((state & LVIS_SELECTED) != (pNMLV->uOldState & LVIS_SELECTED)) {
		m_hActive = iItem;	// アクティブアイテム
		//// タブ表示用文字列の作成
		CString strHead;
		strHead = m_wndList.GetItemText(iItem, 1);	// 送信者名
//		strHead = strHead + _T(" - ") + m_wndList.GetItemText(iItem, 2);	// 日時
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
		switch (pfview->m_dwFolderType) {
		case FD_NORMAL:
		case FD_NORMAL_DEF:
		case FD_TRASH:
		case FD_TRASH_DEF:
			//// カレント設定レコードIDの保持
			theApp.m_lngRsflId = m_wndList.GetItemData(iItem);					
			theApp.m_lngRdflId = -1;
			//// 設定タブの表示
			pFrame->m_Framework.ShowTab(pFrame->m_pRsfSimpleForm, TRUE);
			pFrame->m_Framework.SetTabLabel(pFrame->m_pRsfSimpleForm, strHead);
			pFrame->m_Framework.SetActiveTab(pFrame->m_pRsfSimpleForm);
			pFrame->m_Framework.ShowTab(pFrame->m_pRsfFolderForm, FALSE);
			//// 設定フォームを更新
			prsv = (CRsfSimpleForm *)pFrame->m_pRsfSimpleForm->GetWnd();
			prsv->UpdateForm();
			break;
		default:
			return;
		}
	} else if ((state & LVIS_STATEIMAGEMASK) != (pNMLV->uOldState & LVIS_STATEIMAGEMASK)) {
	}
/*
	if ((newimage = pnmlist->uNewState & LVIS_STATEIMAGEMASK)
                    !=(pnmlist->uOldState & LVIS_STATEIMAGEMASK))
                {
                    if (newimage == INDEXTOSTATEIMAGEMASK(2))
                        TRACE("%dのチェックされた＼n", pnmlist->iItem);
                    else if (newimage == INDEXTOSTATEIMAGEMASK(1))
                        TRACE("%dのチェックが外された＼n", pnmlist->iItem);
                }
*/

	*pResult = 0;
}

// Tabキーを押した場合、OnKeyDown関数で取得することができないため、ここで取得する。
BOOL CRsfList::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		/// TAB KEY : リストフォームへフォーカス移動（リストが空の場合はフォーカス移動しない）
		/// RETRUN KEY : メニュー表示
		if (nVirtKey == VK_TAB) {
			CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
			CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
			CRsfSimpleForm *prsv;
			switch (pfview->m_dwFolderType) {
			case FD_NORMAL:
			case FD_NORMAL_DEF:
			case FD_TRASH:
			case FD_TRASH_DEF:
				prsv = (CRsfSimpleForm *)pFrame->m_pRsfSimpleForm->GetWnd();
				prsv->SetFocus();
				break;
			}
			return TRUE;
		} else if (nVirtKey == VK_RETURN) {
			/// コンテキストメニューの表示
			/// 現在選択されているアイテム位置で表示
			POSITION pos = m_wndList.GetFirstSelectedItemPosition();
			if (pos != NULL) {
				int idx = m_wndList.GetNextSelectedItem(pos);
				RECT rct;
				m_wndList.GetItemRect(idx, &rct, 0);
				CPoint point;
				point.x = (rct.left + rct.right) / 2;
				point.y = (rct.top + rct.bottom) / 2;
				ContextMenu(point);
				return TRUE;
			}
		} else if (nVirtKey == VK_DELETE) {
			/// 現在選択されているアイテムを削除
			POSITION pos = m_wndList.GetFirstSelectedItemPosition();
			if (pos != NULL) {
				if (m_dwFolderType == FD_NORMAL || m_dwFolderType == FD_NORMAL_DEF) {
					OnSetupDel();
				} else {
					OnTrashClear();
				}
				return TRUE;
			}
		}
	}
	
	return TVisualFormView::PreTranslateMessage(pMsg);
}

// ドラッグ開始ハンドラ
void CRsfList::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	/// 選択項目がない時は無効
	if (m_wndList.GetSelectedCount() == 0) return;

	/// 所属フォルダ情報
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	m_dwFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
	if (pfview->IsTrash(m_dwFolderType)) {
		m_dwFolderType = 0;
		return;
	}
	m_dwFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;

	/// ドラッグ中フラグをONし、CImageList::BeginDrag関数を実行
	m_bDrag = TRUE;
	if (pfview->IsFolder(m_dwFolderType)) {
		m_DragImage.BeginDrag(1, CPoint(0, 0));
	} else {
		return;
	}
	/// カーソルをバックアップ
	m_DragCursor = GetCursor();

	/// CImageList::DragEnter関数を実行（座標はスクリーン座標にする必要あり）
	POINT pt = pNMLV->ptAction;
	ClientToScreen( &pt );
	m_DragImage.DragEnter(NULL, pt);

	/// マウスキャプチャー
	SetCapture();

	*pResult = 0;
}

// ドラッグ中ハンドラ
void CRsfList::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	if (m_bDrag)
	{
		/// カーソルを禁止カーソルにする
		HCURSOR cursor = AfxGetApp()->LoadStandardCursor(IDC_NO);
		SetCursor(cursor);

		/// CImageList::DragMove関数を実行（座標はスクリーン座標にする必要あり）
		POINT pt = point;
		ClientToScreen( &pt );
		CImageList::DragMove(pt);

		/// 選択項目が応答時の「規定の応答」のみの場合は無効
		if (m_dwFolderType == FD_NORMAL_DEF) {
			CUIntArray aryIdx;
			UINT intCnt1 = SelectedList(&aryIdx, FALSE, FALSE);
			UINT intCnt2 = SelectedList(&aryIdx, FALSE, TRUE);
			//// デフォルト設定を含んで１件だけ
			if (intCnt1 == 0 && intCnt2 == 1) {
				return;
			}
		}

		/// フォルダの上かチェック
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
		CTreeCtrl &tc = pfview->GetTreeCtrl();
		CPoint pnt = CnvPntM_F(point);
		HTREEITEM hItem = tc.HitTest(pnt, NULL);
		if ( hItem && (hItem != pfview->m_hActive))
		{
			DWORD type = tc.GetItemData(hItem) & FD_MASK_TYPE;
			/// フォルダの種別が一致するかチェック
			if (((pfview->IsFolder(type) || type == FD_TRASH_DEF) && pfview->IsFolder(m_dwFolderType))) 
			{
				/// カーソルを戻す
				SetCursor(m_DragCursor);
				/// ドロップ先を強調表示する
				CImageList::DragShowNolock(FALSE);
				tc.SelectDropTarget(hItem);
				CImageList::DragShowNolock(TRUE);
			}
		}
	}

	TVisualFormView::OnMouseMove(nFlags, point);
}

// ドロップハンドラ
void CRsfList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
		if (m_bDrag)
		{
			/// ドラッグ中フラグをOFFし、マウスキャプチャーを解放
			m_bDrag = FALSE;
			ReleaseCapture();

			/// CImageList::DragLeave関数とCImageList::EndDrag関数を実行
			CImageList::DragLeave(this);
			CImageList::EndDrag();

			/// ドロップ先の強調表示を解除
			CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
			CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
			CTreeCtrl &tc = pfview->GetTreeCtrl();
			tc.SelectDropTarget(NULL);

			/// カーソルを戻す
			SetCursor(m_DragCursor);

			/// ドロップ先ツリーアイテムのハンドルを取得
			CPoint pnt = CnvPntM_F(point);
			HTREEITEM hItemDrop = tc.HitTest(pnt, NULL);
			if (!hItemDrop) return;
			long lngFolderId = tc.GetItemData(hItemDrop) & FD_MASK_ID;

			/// ドロップ先が存在しない場合、ドラッグアイテムとドロップ先アイテムが等しい場合は何もしない
			if( hItemDrop == NULL || pfview->m_hActive == hItemDrop )
			{
				return;
			}

			/// ドロップ先フォルダ毎の処理
			DWORD type = tc.GetItemData(hItemDrop) & FD_MASK_TYPE;
			if (pfview->IsFolder(m_dwFolderType)) {
				if (pfview->IsFolderWT(type)) {
					if (type == FD_TRASH_DEF) {
						/// 応答設定をごみ箱へ
						Move(-1);
					} else if (type == FD_TRASH) {
						return;
					} else {
						/// 応答設定を移動
						Move(lngFolderId);
					}
				} else {
					/// フォルダの種別が一致しない場合は何もしない
					return;
				}
			}
		}
	
	TVisualFormView::OnLButtonUp(nFlags, point);
}

// カーソルの変更
BOOL CRsfList::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	/// リストアイコン上ではカーソルを変更する
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);
	if (ChkPlayIcon(point)) {
		HCURSOR cursor = AfxGetApp()->LoadCursor(IDC_LINK);
		SetCursor(cursor);
		return TRUE;
	}
	
	return TVisualFormView::OnSetCursor(pWnd, nHitTest, message);
}


// 音声ファイル再生
void CRsfList::OnLButtonDblClk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	Play();

	*pResult = 0;
}

// 音声ファイル再生
void CRsfList::OnLButtonDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CPoint pt = pNMLV->ptAction;

	if (ChkPlayIcon(pt)) {
		Play();
	}

	*pResult = 0;
}

// コンテキストメニューの表示
void CRsfList::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	/// スクリーン座標が取得されるため、クライアント座標に変換
	RECT rct;
	GetWindowRect(&rct);
	point.x -= rct.left;
	point.y -= rct.top;
	/// コンテキストメニューの表示
	ContextMenu(point);
}

// 応答設定新規作成
void CRsfList::OnSetupNew()
{
	CRsfNewDialog dlg;
	dlg.DoModal();

	/// フォルダの移動がないため、明示的に再描画
	UpdateForm();
}

// 応答設定送信者変更
void CRsfList::OnSetupRename()
{
	CRsfRenameDialog dlg;
	dlg.DoModal();
}

// 応答設定をコピー
void CRsfList::OnSetupCopy()
{
	/// 選択リスト取得
	SelectedList(&m_aryClip, TRUE, TRUE);
}

// 応答設定を貼り付け
void CRsfList::OnSetupPaste()
{
	Copy();
}

// 応答設定送信者へコール
void CRsfList::OnSetupCall()
{
	CUIntArray aryIdx;

	/// 選択リスト情報
	long lngCnt = SelectedList(&aryIdx, TRUE, FALSE);
	if (lngCnt != 1) return;
	long lngId = aryIdx.GetAt(0);

	/// 送信者名（スカイプ名）取得
	CTblRsfl tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(lngId) != 1) return;
	CString strId = tbl.strSenderId;		

	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	pFrame->m_pAccessEvents->SendCall(strId);
}

// 応答設定削除
void CRsfList::OnSetupDel()
{
	CString str;

	/// 確認メッセージ
	str.LoadString(IDS_MSGBOX53);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return;

	Move(-1);
}

// ごみ箱内のアイテム削除
void CRsfList::OnTrashClear()
{
	/// 確認メッセージ
	CString str;
	str.LoadString(IDS_MSGBOX34);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return;

	if (m_dwFolderType == FD_TRASH || m_dwFolderType == FD_TRASH_DEF) {
		TrashDel();
	}
}

// ごみ箱内のアイテムを元に戻す
void CRsfList::OnTrashRecover()
{
	if (m_dwFolderType == FD_TRASH || m_dwFolderType == FD_TRASH_DEF) {
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
BOOL CRsfList::ContextMenu(
CPoint point)	// マウス位置
{
/*
	/// マウス上のリストを選択
	UINT intFlag;
	m_hContext = m_wndList.HitTest(point, &intFlag);
	if (m_hContext < 0) {
		m_hContext = -1;
	}
*/
	long lngSelCnt = m_wndList.GetSelectedCount();

	/// 所属フォルダ情報
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	m_dwFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
	m_dwFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;

	/// POPUPメニュー表示
	CMenu menu;
	CMenu* pPopup;
	switch(m_dwFolderType) {
	case FD_NORMAL:
	case FD_NORMAL_DEF:
		VERIFY(menu.LoadMenu(IDR_POPUP_SETLIST));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		if (lngSelCnt != 1) {
			pPopup->EnableMenuItem(ID_POPUP_SETLIST_RENAME, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_POPUP_SETLIST_CALL, MF_BYCOMMAND | MF_GRAYED);
			if (lngSelCnt == 0) {
				pPopup->EnableMenuItem(ID_POPUP_SETLIST_COPY, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_POPUP_SETLIST_DEL, MF_BYCOMMAND | MF_GRAYED);
			}
		} else {
			//// デフォルト設定時
			CUIntArray aryIdx;
			if (SelectedList(&aryIdx, TRUE, FALSE) == 0) {
				pPopup->EnableMenuItem(ID_POPUP_SETLIST_RENAME, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_POPUP_SETLIST_CALL, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_POPUP_SETLIST_DEL, MF_BYCOMMAND | MF_GRAYED);
			}
		}
		if (m_aryClip.GetSize() < 1) {
			pPopup->EnableMenuItem(ID_POPUP_SETLIST_PASTE, MF_BYCOMMAND | MF_GRAYED);
		}
		break;
	case FD_TRASH_DEF:
	case FD_TRASH:
		VERIFY(menu.LoadMenu(IDR_POPUP_TRASH));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		if (lngSelCnt == 0) {
			pPopup->EnableMenuItem(ID_POPUP_TRASH_RECOVER, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_POPUP_TRASH_CLEAR, MF_BYCOMMAND | MF_GRAYED);
		}
		break;
	default:
		return FALSE;
	}
	
	ClientToScreen(&point);
	CWnd *pWnd = AfxGetMainWnd();
	if (!pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		point.x, point.y, pWnd)) return FALSE;

	return TRUE;
}

//
//  機能     : リストの初期設定
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
void CRsfList::InitListCtrl(void)
{
	CString str;

	// set column width according to window rect
//	m_wndList.Create(this, 0, NULL, LVS_REPORT | LVS_SHOWSELALWAYS);
	m_wndList.SetGridLines(FALSE); // hide grid lines
	m_wndList.SetCheckboxeStyle(RC_CHKBOX_NONE); // No checkboxes
	m_wndList.SetEditable(FALSE); // disallow sub-text edit
	//// ヘッダー定義(title,size,aligne) 
	// ex) _T(" , 20, 0; 送信者, 100, 0; 日時, 110, 0")
	str.LoadString(IDS_RSFLIST_HEAD);
	m_wndList.SetColumnHeader(str);
	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT);// 行選択

	// リストのカラムサイズ調整
	AdustColumns();

	m_wndList.SetImageList(&m_ListImage);
}

//
//  機能     : リストのカラムサイズ調整
//  
//  機能説明 : 送信者名を変動
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
void CRsfList::AdustColumns(void) 
{
	if (m_wndList.GetSafeHwnd() != NULL)
	{
		CListCtrl *clc;
		CRect rect;
		int intFixSize;
		int intMargin;
		int intWidth;

		clc = &m_wndList;

		// 固定値
		intFixSize = clc->GetColumnWidth(0) + clc->GetColumnWidth(2);
		intMargin = 80;	// マージン

		clc->GetParent()->GetClientRect(&rect);
		intWidth = rect.Width();
		if (intWidth > (intFixSize+intMargin)) {
			clc->SetColumnWidth(1, intWidth - intFixSize - 20);		// 20:スクロールバーの幅
		}
	}
}

//
//  機能     : リストの生成・更新
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfList::UpdateForm() 
{
	/// リストのクリア
	m_wndList.DeleteAllItems();

	/// リストビューをウィンドウにフィットさせる
    m_wndList.ResizeToFitParent();

	if (_tcsclen(theApp.m_strUsrId) == 0 || _tcsclen(theApp.m_strActiveFolder) == 0) {
		return FALSE;
	}

	/// カレントフォルダの情報取得
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	m_dwFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
	m_dwFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;
//	DWORD lngFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
//	long lngFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;

	/// リストの生成
	switch(m_dwFolderType) {
	case FD_NORMAL:
	case FD_NORMAL_DEF:
	case FD_TRASH:
	case FD_TRASH_DEF:
		//// 応答設定リストの生成
		MakeList(m_dwFolderType, m_dwFolderId);
		break;
	}
	
	/// リストの先頭行を選択
	if (m_wndList.GetItemCount() > 0) {
		m_wndList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

	return TRUE;
}

//
//  機能     : カレントフォルダに対応した応答設定リストの生成
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfList::MakeList(
DWORD lngFolderType,	// フォルダタイプ
long lngFolderId)		// フォルダID
{
	_TCHAR buf[32];
	//int i;

	CTblRsfl tbl(theApp.m_strDBPath);

	/// データベースからリスト生成・更新
	CString strWhere;
	strWhere.Empty();
	switch(lngFolderType) {
	case FD_NORMAL:
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T(" and ") + _T("lngFolderId") + _T("=") + buf;
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
		strWhere = strWhere + _T(" order by lngSystemNew desc");
		break;
	case FD_NORMAL_DEF:
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		strWhere = strWhere + _T(" and ") + _T("lngFolderId=0");
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
		strWhere = strWhere + _T(" order by lngDefaultFlag desc, lngSystemNew desc");
		break;
	case FD_TRASH:
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T(" and ") + _T("lngFolderId") + _T("=") + buf;
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
		strWhere = strWhere + _T(" order by lngSystemNew desc");
		break;
	case FD_TRASH_DEF:
		//// 該当するフォルダがごみ箱にないか、削除されていない場合
		tbl.OpenTbl(_T("rsfl left outer join folder on rsfl.lngFolderId=folder.ID"));
		strWhere = strWhere + _T("rsfl.strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		strWhere = strWhere + _T(" and ") + _T("(folder.ID is NULL or folder.lngSystemFlag=0)");
		strWhere = strWhere + _T(" and ") + _T("rsfl.lngSystemFlag=1");
		strWhere = strWhere + _T(" order by rsfl.lngSystemNew desc");
		break;
	default:
		return FALSE;
	}

	/// リスト生成
	CString strPlayFile;
	CTPsetup tps(theApp.m_strDataFolder);
	CString strSender;
	_TCHAR strDate[64];
	int intIdx;
	long lngCnt = tbl.SelectRec(strWhere);
	// 01.May.08 高速化（微妙）
	m_wndList.SetItemCount(lngCnt);	
	m_wndList.SetRedraw( FALSE );
	m_wndList.LockWindowUpdate();
	while (lngCnt > 0)
	{
		if (tbl.lngDefaultFlag == 1) {
			strSender.LoadString(IDS_RSF_DEF);
		} else if (tbl.strSenderName.IsEmpty()) {
			strSender = tbl.strSenderId;
		} else {
			strSender = tbl.strSenderName + _T(" <") + tbl.strSenderId + _T(">");
		}
		CnvDateString((LPCTSTR)tbl.dlngSystemNew, strDate);
		intIdx = m_wndList.InsertItemEx(INT_MAX, _T(""), strSender, strDate);
		//// 音声ファイルがあればアイコン表示
		if (tbl.lngDefaultFlag == 1) {
			tps.SetCallerId(_T("0"));
		} else {
			tps.SetCallerId(tbl.strSenderId);
		}
		strPlayFile = tps.RsfAudioFileName(_T("$"));
		CFileSpec fs(strPlayFile);
		if (fs.Exist()) {
			m_wndList.SetItemImage(intIdx, 0, 4);	// setup icon
		} else {
			m_wndList.SetItemImage(intIdx, 0, -1);	// no icon
		}
		//// レコードIDの保存
		m_wndList.SetItemData(intIdx, tbl.ID);
		//// 選択表示
		//for (i = 0; i < m_arySelect.GetSize(); i++) {
		//	if (m_arySelect.GetAt(i) == tbl.ID) {
		//		m_wndList.SetItemState(intIdx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		//	}
		//}
		if (!tbl.NextRec()) break;
		--lngCnt;
	}
	//m_arySelect.RemoveAll();
	// 01.May.08 高速化（微妙）
	m_wndList.UnlockWindowUpdate();
	m_wndList.SetRedraw( TRUE );

	return TRUE;
}

//
//  機能     : 選択された応答設定をコピー
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfList::Copy() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	long i, j;
	_TCHAR buf[32];
	long lngId;
	CString str;
	CString strId, strName, strOrgId;
	CString strPath, strOrgPath;
	BOOL ret = TRUE;

	long lngCnt = m_aryClip.GetSize();
	if (lngCnt < 0) return FALSE;

	CFileSpec fs;
	CTblRsfl tbl(theApp.m_strDBPath);
	CTPsetup tps(theApp.m_strDataFolder);
	for (i = 0; i < lngCnt; i++) {
		/// 既存の送信者情報
		lngId = m_aryClip.GetAt(i);
		if (tbl.SelectRec(lngId) != 1) {
			ret = FALSE;
			break;
		}
		strOrgId = tbl.strSenderId;		// 送信者名（スカイプ名）
		if (tbl.lngDefaultFlag == 1) strOrgId = _T("0");	// デフォルト応答
		strName = tbl.strSenderName;	// 送信者名（表示名）

		/// コピー送信者名の生成
		for (j = 1; j< 100; j++) {
			if (tbl.lngDefaultFlag == 1) {
				str.Empty();
				_ltot_s(j, buf, sizeof(buf)/sizeof(buf[0]), 10);
				str = str + _T("default_") + buf;
			} else {
				_ltot_s(j, buf, sizeof(buf)/sizeof(buf[0]), 10);
				str = strOrgId + _T("_") + buf;
			}
			//// 重複チェック
			if (ChkSetup(str)) break;
		}
		if (j == 100) {
			ret = FALSE;
			break;
		}
		strId = str;
		if (!strName.IsEmpty()) {
			_ltot_s(j, buf, sizeof(buf)/sizeof(buf[0]), 10);
			strName = strName + _T("_") + buf;
		}

		/// 応答設定作成（音声/チャット/ビデオ/画像ファイル、設定ファイル、テーブル）
		//// 応答音声ファイルのコピー
		tps.SetCallerId(strOrgId);
		strOrgPath = tps.RsfAudioFileName(_T("$"));
		tps.SetCallerId(strId);
		strPath = tps.RsfAudioFileName(_T("$"));
		fs.SetFullSpec(strOrgPath);
		if (!fs.FileCopy(strPath, TRUE)) {
			ret = FALSE;
			break;
		}
		//// 応答チャットファイルのコピー
		tps.SetCallerId(strOrgId);
		strOrgPath = tps.RsfChatFileName(_T("$"));
		tps.SetCallerId(strId);
		strPath = tps.RsfChatFileName(_T("$"));
		fs.SetFullSpec(strOrgPath);
		if (fs.Exist()) {
			if (!fs.FileCopy(strPath, TRUE)) {
				ret = FALSE;
				break;
			}
		}
		//// 応答ビデオファイルのコピー
		tps.SetCallerId(strOrgId);
		strOrgPath = tps.RsfVideoFileName(_T("$"));
		tps.SetCallerId(strId);
		strPath = tps.RsfVideoFileName(_T("$"));
		fs.SetFullSpec(strOrgPath);
		if (fs.Exist()) {
			if (!fs.FileCopy(strPath, TRUE)) {
				ret = FALSE;
				break;
			}
		}
		//// 応答画像ファイルのコピー
		tps.SetCallerId(strOrgId);
		strOrgPath = tps.FindRsfImgFile(_T("$"));
		fs.SetFullSpec(strOrgPath);
		if (fs.Exist()) {
			tps.SetCallerId(strId);
			strPath = tps.RsfImgFileName(_T("$"), strOrgPath.Right(3));
			if (!fs.FileCopy(strPath, TRUE)) {
				ret = FALSE;
				break;
			}
		}

		/// 連携プログラムCSVファイルのコピー
		CString strPrgId, strBinDir, strFile;
		for(int i = 0; i < 999; i++) {
			strPrgId.Format(_T("%d"), i);
			tps.SetCallerId(strOrgId);
			strOrgPath = tps.RsfPrgFileName(_T("$$$$"), strPrgId);
			fs.SetFullSpec(strOrgPath);
			if (fs.Exist()) {
				tps.SetCallerId(strId);
				strPath = tps.RsfPrgFileName(_T("$$$$"), strPrgId);
				if (!fs.FileCopy(strPath, TRUE)) {
					ret = FALSE;
					break;
				}
			}
		}

		/// 応答設定ファイルのコピー
		tps.SetCallerId(strOrgId);
		UST_RSF *pst = tps.LoadRSF();
		if (pst == NULL) {
			ret = FALSE;
			break;
		}
		pst->telno = strId;
		tps.SetCallerId(strId);
		if (!tps.SaveRsfFile(pst)) {
			ret = FALSE;
			break;
		}

		/// rsflテーブルに追加
		tbl.strSenderId = strId;		// 送信者名（スカイプ名）
		tbl.strSenderName = strName;	// 送信者名（表示名）
		tbl.strReceiverId = theApp.m_strUsrId;	// 受信者名（スカイプ名）
		tbl.strReceiverName = theApp.m_strUsrName;	// 受信者名（表示名）
		tbl.lngRespType = RESPBASE_TYPE_SIMPLE;
		tbl.lngFolderId = m_dwFolderId;
		tbl.lngDefaultFlag = 0;
		tbl.InsertRec();
	}

	if (!ret) {
		str.LoadString(IDS_MSGBOX54);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return ret;
	}

	/// メッセージリストの更新
	UpdateForm();
	//// デフォルトフォルダの場合、（規定の設定）の次の行を選択
	if(m_dwFolderType == FD_NORMAL_DEF) {
		if (m_wndList.GetItemCount() > 1) {
			m_wndList.SetAllItemStates(RC_ITEM_SELECTED, RC_ITEM_UNSELECTED);
			m_wndList.SetItemStates(1, RC_ITEM_FOCUSED | RC_ITEM_SELECTED);
		}
	}

	return ret;
}

//
//  機能     : 応答設定の移動
//  
//  機能説明 : ごみ箱対応
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfList::Move(
long lngFolderId)		// -1:ごみ箱へ移動
{
	CString str;
	UINT intCnt, i = 0;
	int idx;
	CUIntArray aryIdx;

	/// 選択リスト情報
	intCnt = SelectedList(&aryIdx, FALSE, FALSE);
	if (intCnt < 1) return FALSE;

	/// テーブルを更新
	long lngId;
	_TCHAR buf[32];
	CTblRsfl tbl(theApp.m_strDBPath);
	CString strWhere;
	CString strSet;
	//// トランザクション開始
	tbl.m_blTransaction = TRUE;
	if (!tbl.TransactionStart()) return FALSE;
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		strWhere.Empty();
		_ltot_s(lngId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T("ID") + _T("=") + buf;
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
		strSet.Empty();
		if (lngFolderId < 0) {
			///// 削除フラグ付加
			strSet = strSet + _T("lngSystemFlag=1");
		} else {
			///// フォルダ変更
			_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
			strSet = strSet + _T("lngFolderId=") + buf;
		}
		tbl.UpdateRec(strSet, strWhere);	
	}
	//// トランザクション終了
	tbl.TransactionEnd();

	/// リストから削除（リストの後ろから）
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(intCnt-i-1);
		m_wndList.DeleteItem(idx);
	}
	//// 一つ上の項目を再選択
	if (m_wndList.GetItemCount() < 1) return TRUE;
	if (idx > 0) idx--;
	m_wndList.SetItemState(idx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

	return TRUE;
}

//
//  機能     : ごみ箱(応答)内のリスト削除
//  
//  機能説明 : 完全削除
//  
//  返り値   : BOOL
//  
//  備考     : トランザクションが必要なので、１関数で実装
//  
BOOL CRsfList::TrashDel()
{
	CString str;
	UINT intCnt, i = 0;
	int idx;
	CUIntArray aryIdx;

	/// 選択リスト情報
	intCnt = m_wndList.GetSelectedCount();
	if (intCnt < 1) return FALSE;
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	if (pos == NULL) return FALSE;
	for (i = 0; i < intCnt; i++) {
		idx = m_wndList.GetNextSelectedItem(pos);
		aryIdx.Add(idx);
		if (pos == NULL) break;
	}
	intCnt = aryIdx.GetSize();
	if (intCnt < 1) return FALSE;

	long lngId;
	_TCHAR buf[32];
	/// 設定テーブルを更新
	CTblRsfl tbl(theApp.m_strDBPath);
	//// 設定・応答ファイルの削除
	CFileSpec fs;
	CTPsetup tps(theApp.m_strDataFolder);
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		if (tbl.SelectRec(lngId) <= 0) continue;
		tps.SetCallerId(tbl.strSenderId);
		///// 設定ファイル
		str = tps.RsfFileName();
		fs.SetFullSpec(str);
		if (fs.Exist()) fs.FileDelete();
		///// 音声ファイル
		str = tps.RsfAudioFileName(_T("$"));
		fs.SetFullSpec(str);
		if (fs.Exist()) fs.FileDelete();
		///// ビデオファイル
		str = tps.RsfVideoFileName(_T("$"));
		fs.SetFullSpec(str);
		if (fs.Exist()) fs.FileDelete();
		///// 画像ファイル
		str = tps.FindRsfImgFile(_T("$"));
		fs.SetFullSpec(str);
		if (fs.Exist()) fs.FileDelete();
		///// チャットファイル
		str = tps.RsfChatFileName(_T("$"));
		fs.SetFullSpec(str);
		if (fs.Exist()) fs.FileDelete();
	}
	//// トランザクション開始
	CString strWhere;
	tbl.m_blTransaction = TRUE;
	if (!tbl.TransactionStart()) return FALSE;
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		strWhere.Empty();
		_ltot_s(lngId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T("ID") + _T("=") + buf;
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
		tbl.DeleteRec(strWhere);	// 削除
	}
	//// トランザクション終了
	tbl.TransactionEnd();

	/// リストから削除（リストの後ろから）
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(intCnt-i-1);
		m_wndList.DeleteItem(idx);
	}
	//// 一つ上の項目を再選択
	if (m_wndList.GetItemCount() < 1) return TRUE;
	if (idx > 0) idx--;
	m_wndList.SetItemState(idx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

	return TRUE;
}

//
//  機能     : ごみ箱（応答）内のリストを元に戻す
//  
//  機能説明 : 削除フラグを解除
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfList::TrashRecover()
{
	CString str;
	UINT intCnt, i = 0;
	_TCHAR buf[32];
	int idx, lngId, lngFolderId;
	CUIntArray aryIdx;
	CUIntArray aryFolderId;
	CString strWhere;
	CString strSet;

	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();

	/// 設定テーブルを更新
	//// 選択リスト情報
	intCnt = SelectedList(&aryIdx, FALSE, TRUE);
	if (intCnt < 1) return FALSE;
	//// リカバリ先フォルダの存在チェック
	CTblRsfl tbl(theApp.m_strDBPath);
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		tbl.SelectRec(lngId);
		if (pfview->FindItemId(pfview->m_hFolderDef, tbl.lngFolderId)) {
			aryFolderId.Add(tbl.lngFolderId);
		} else {
			aryFolderId.Add(0);
		}
	}
	//// トランザクション開始
	tbl.m_blTransaction = TRUE;
	if (!tbl.TransactionStart()) return FALSE;
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		lngFolderId = aryFolderId.GetAt(i);
		strWhere.Empty();
		_ltot_s(lngId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T("ID") + _T("=") + buf;
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
		strSet.Empty();
		_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strSet = strSet + _T("lngSystemFlag=0,lngFolderId=") + buf;
		tbl.UpdateRec(strSet , strWhere);
	}
	//// トランザクション終了
	tbl.TransactionEnd();

	/// リストから削除（リストの後ろから）
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(intCnt-i-1);
		m_wndList.DeleteItem(idx);
	}
	//// 一つ上の項目を再選択
	if (m_wndList.GetItemCount() < 1) return TRUE;
	if (idx > 0) idx--;
	m_wndList.SetItemState(idx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

	return TRUE;
}

//
//  機能     : 対応する応答設定の表示
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfList::SelSetup(
CString strSender)	// 送信者名
{
	CString str;
	HTREEITEM h;

	//m_arySelect.RemoveAll();

	/// 応答設定の検出
	CTblRsfl tbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
	strWhere = strWhere + _T(" and ") + _T("strSenderId") + _T("='") + strSender + _T("'");
	long lngCnt = tbl.SelectRec(strWhere);
	if (lngCnt == 0) return FALSE;

	long lngId = tbl.ID;
	long lngFolderId = tbl.lngFolderId;
	long lngSystemFlag = tbl.lngSystemFlag;

	/// 応答設定フォルダの選択表示
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	//// フォルダ検索
	if (lngFolderId == 0) {
		if (lngSystemFlag == 0) {
			h = pfview->m_hFolderDef;
		} else {
			h = pfview->m_hTrashDef;
		}
	} else {
		if (lngSystemFlag == 0) {
			h = pfview->FindItemId(pfview->m_hFolderDef, lngFolderId);
		} else {
			h = pfview->FindItemId(pfview->m_hTrashDef, lngFolderId);
		}
	}
	//// リスト選択するIDの指定
	//m_arySelect.Add(lngId);
	//// フォルダ選択・リスト更新
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	tc.SelectItem(h);
	tc.EnsureVisible(h);

	/// 応答設定の選択表示
	long idx = FindIdx(lngId);
	if (idx < 0) {
		//// 該当アイテムがない場合、再生成してリトライ
		UpdateForm();
		idx = FindIdx(lngId);
	}
	if (idx >= 0) {
		m_wndList.SetAllItemStates(RC_ITEM_SELECTED, RC_ITEM_UNSELECTED);
		m_wndList.SetItemStates(idx, RC_ITEM_FOCUSED | RC_ITEM_SELECTED);
//		m_wndList.SetItemState(idx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	}

	return TRUE;
}


//
//  機能     : 応答設定送信者名チェック
//  
//  機能説明 : ごみ箱内も対象
//  
//  返り値   : TRUE:重複なし、FALSE：重複あり
//  
//  備考     : 
//  
BOOL CRsfList::ChkSetup(
CString strSender)	// 送信者名
{
	CString str;

	if (strSender.IsEmpty()) {
		str.LoadString(IDS_MSGBOX42);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	/// 既存送信者名との重複チェック
	CTblRsfl tbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
	strWhere = strWhere + _T(" and ") + _T("strSenderId") + _T("='") + strSender + _T("'");
	long lngCnt = tbl.CountRec(strWhere);
	if (lngCnt > 0) {
		return FALSE;
	}

	return TRUE;
}


//
//  機能     : 応答設定選択リストの取得
//  
//  機能説明 : 
//  
//  返り値   : 取得リスト件数
//  
//  備考     : 
//  
long CRsfList::SelectedList(
CUIntArray *pAryIdx,	// リスト保存配列
BOOL blData,			// TRUE:リストインデックスの代わりにアイテムデータを保存
BOOL blDefault)			// TRUE:デフォルト設定を含む
{
	CString str;
	UINT intCnt, i = 0;
	int idx, intId;

	pAryIdx->RemoveAll();

	/// 選択リスト情報
	intCnt = m_wndList.GetSelectedCount();
	if (intCnt < 1) return 0;

	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	if (pos == NULL) return 0;

	for (i = 0; i < intCnt; i++) {
		idx = m_wndList.GetNextSelectedItem(pos);
		intId = m_wndList.GetItemData(idx);
		if (!blDefault && (intId == 1)) {
			//// デフォルト設定は除外
		} else {
			if (blData) {
				pAryIdx->Add(intId);
			} else {
				pAryIdx->Add(idx);
			}
		}
		if (pos == NULL) break;
	}

	return pAryIdx->GetSize();
}

//
//  機能     : 応答設定リストインデックスの検索
//  
//  機能説明 : 
//  
//  返り値   : インデックス、-1:not found
//  
//  備考     : 
//  
long CRsfList::FindIdx(
long lngId)			// 応答設定テーブルID
{
	CString str;
	UINT intCnt, i = 0;
	int idx, intId;

	/// 選択リスト情報
	intCnt = m_wndList.GetSelectedCount();
	if (intCnt < 1) return -1;

	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	if (pos == NULL) return -1;

	for (i = 0; i < intCnt; i++) {
		idx = m_wndList.GetNextSelectedItem(pos);
		intId = m_wndList.GetItemData(idx);
		if (lngId == (long)intId) {
			return idx;
		}
		if (pos == NULL) break;
	}

	return -1;
}

//
//  機能     : クライアント座標変換（RsfList->RsfTree）
//  
//  機能説明 : 
//  
//  返り値   : RsfTreeクライアント座標
//  
//  備考     : 
//  
CPoint CRsfList::CnvPntM_F(
const CPoint point)	// RsfListクライアント座標
{
	CPoint pnt = point;
	ClientToScreen(&pnt);
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	pfview->ScreenToClient(&pnt);

	return pnt;
}

//
//  機能     : 選択行の再生
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CRsfList::Play() 
{
	/// カレントフォルダの情報取得
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	if (pfview->IsFolderWT(pfview->m_dwFolderType)) {
//		CRsfSimpleForm *pform = (CRsfSimpleForm *)theApp.m_pRsfSimpleForm->GetWnd();
//		pform->Play();
	}

	return TRUE;
}

//
//  機能     : 指定座標が音声再生アイコン上にあるか
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfList::ChkPlayIcon(
const CPoint point)	// クライアント座標
{
	LVHITTESTINFO hitTestInfo;
	hitTestInfo.pt = point;
	hitTestInfo.flags = LVHT_ONITEM;
	if (m_wndList.SubItemHitTest(&hitTestInfo) >= 0) {
		if (hitTestInfo.iItem >= 0 && hitTestInfo.iSubItem == 0) {
			if (m_wndList.GetItemImage(hitTestInfo.iItem, 0) >= 0) {
				return TRUE;
			}
		}
	}
	
	return FALSE;
}


