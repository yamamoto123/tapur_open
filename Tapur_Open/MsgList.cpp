// MsgList.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "TPutil.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"
#include "MainFrm.h"
#include "MsgList.h"
#include "RsfList.h"
#include "MsgTree.h"
#include "MsgForm.h"
#include "RsfNewDialog.h"
#include "SkypeEvents.h"
#include "Tokenizer.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CMsgList

IMPLEMENT_DYNCREATE(CMsgList, TVisualFormView)

CMsgList::CMsgList()
	: TVisualFormView(CMsgList::IDD)
{
	//{{AFX_DATA_INIT(CMsgList)
		// メモ: ClassWizard はこの位置にメンバの初期化処理を追加します
	//}}AFX_DATA_INIT

	m_bDrag = FALSE;
	m_hItemDrag = 0;
}

CMsgList::~CMsgList()
{
}

void CMsgList::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgList)
		// メモ: ClassWizard はこの位置に DDX および DDV の呼び出しを追加します
	DDX_Control(pDX, IDC_LIST, m_wndList);	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMsgList, TVisualFormView)
	//{{AFX_MSG_MAP(CMsgList)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchanged)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST, OnBegindrag)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_MSGLIST_CALL, OnMsgCall)
	ON_COMMAND(ID_POPUP_REBTEL, OnLinkRebtel)
	ON_COMMAND(ID_POPUP_MSGLIST_SETUP, OnMsgSetup)
	ON_COMMAND(ID_POPUP_MSGLIST_DEL, OnMsgDel)
	ON_COMMAND(ID_POPUP_PRG1, OnPrg1)
	ON_COMMAND(ID_POPUP_PRG11, OnPrg11)
	ON_COMMAND(ID_POPUP_PRG901, OnPrg901)
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
// CMsgList 診断

#ifdef _DEBUG
void CMsgList::AssertValid() const
{
	CFormView::AssertValid();
}

void CMsgList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMsgList メッセージ ハンドラ

int CMsgList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (TVisualFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください
	
	m_aryClip.RemoveAll();

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
	m_clrUnview = RGB(210, 255, 210);	// light green

	return 0;
}

void CMsgList::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit();
	
//	srand((unsigned)time(NULL));

	InitListCtrl();
}

void CMsgList::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	// リストビューをウィンドウにフィットさせる
    m_wndList.ResizeToFitParent();
	
	// リストのカラムサイズ調整
	AdustColumns();
}

// リスト選択時イベント
void CMsgList::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int iItem = -1;
	int iSubItem = -1;
	UINT state;
	UINT intCnt = 0;
	CMsgForm *pmv;
	*pResult = 0;
	long lngId;

	if (!pNMLV) goto PROC_END;
	iItem = pNMLV->iItem;
	iSubItem = pNMLV->iSubItem;
	if (iItem < 0 || iSubItem < 0) goto PROC_END;
	intCnt = m_wndList.GetSelectedCount();
	//if (intCnt <= 0) return;
	if (intCnt != 1) goto PROC_END;	// 複数選択の場合メッセージフォームは更新しない（大量選択の場合、時間がかかるので）

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
		strHead = m_wndList.GetItemText(iItem, 3);	// 件名
		//strHead = m_wndList.GetItemText(iItem, 1);	// 送信者名
		//strHead = m_wndList.GetItemText(iItem, 2);	// 日時
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
		switch (pfview->m_dwFolderType) {
		case FD_NORMAL:
		case FD_NORMAL_DEF:
		case FD_TRASH:
		case FD_TRASH_DEF:
			theApp.m_lngRsflId = -1;
			//// カレント受信レコードIDの保持
			lngId = m_wndList.GetItemData(iItem);
			//if (lngId >= 0 && theApp.m_lngRdflId >= 0 && (lngId == theApp.m_lngRdflId)) {
			//	///// 同じデータを選択した場合は、再表示しない
			//	break;
			//}
			theApp.m_lngRdflId = lngId;
			//// メッセージタブの表示
			pFrame->m_Framework.ShowTab(pFrame->m_pMsgForm, TRUE);
			pFrame->m_Framework.SetTabLabel(pFrame->m_pMsgForm, strHead);
			pFrame->m_Framework.SetActiveTab(pFrame->m_pMsgForm);
			pFrame->m_Framework.ShowTab(pFrame->m_pMsgFolderForm, FALSE);
			//// メッセージフォームを更新
			pmv = (CMsgForm *)pFrame->m_pMsgForm->GetWnd();
			pmv->UpdateForm(FALSE);	// フォーカスを移動しない
			break;
		default:
			goto PROC_END;
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

PROC_END:
	*pResult = 0;
}

// Tabキーを押した場合、OnKeyDown関数で取得することができないため、ここで取得する。
BOOL CMsgList::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		/// TAB KEY : リストフォームへフォーカス移動（リストが空の場合はフォーカス移動しない）
		/// RETRUN KEY : メニュー表示
		if (nVirtKey == VK_TAB) {
			CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
			CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
			CMsgForm *pmv;
			if (pfview->IsFolderWT(pfview->m_dwFolderType)) {
				pmv = (CMsgForm *)pFrame->m_pMsgForm->GetWnd();
				pmv->SetFocus();
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
					OnMsgDel();
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
void CMsgList::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	/// 選択項目がない時は無効
	if (m_wndList.GetSelectedCount() == 0) return;

	/// 所属フォルダ情報
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
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
		m_DragImage.BeginDrag(0, CPoint(0, 0));
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
void CMsgList::OnMouseMove(UINT nFlags, CPoint point) 
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

		/// フォルダの上かチェック
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
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
void CMsgList::OnLButtonUp(UINT nFlags, CPoint point) 
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
			CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
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
						/// メッセージをごみ箱へ
						Move(-1);
					} else if (type == FD_TRASH) {
						return;
					} else {
						/// メッセージを移動
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
BOOL CMsgList::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
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
void CMsgList::OnLButtonDblClk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	Play();

	*pResult = 0;
}

// 音声ファイル再生
void CMsgList::OnLButtonDown(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CMsgList::OnContextMenu(CWnd* pWnd, CPoint point) 
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

// 送信者へコール
void CMsgList::OnMsgCall()
{
	CUIntArray aryIdx;

	/// 選択リスト情報
	long lngCnt = SelectedList(&aryIdx, TRUE);
	if (lngCnt != 1) return;
	long lngId = aryIdx.GetAt(0);

	/// 送信者名（スカイプ名）取得
	CTblRdfl tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(lngId) != 1) return;
	CString strId = tbl.strSenderId;		
	if (strId == _T("0")) {
		//// 不明な発信者(SkypeIn)
		CString str;
		str.LoadString(IDS_MSGBOX17);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return;
		
	}

	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	pFrame->m_pAccessEvents->SendCall(strId);
}

// 広告（Rebtel）
void CMsgList::OnLinkRebtel()
{
	CString str;
	str.LoadString(IDS_URL_REBTEL);
	theApp.OpenUrl(str);
}

// 対応する応答設定の表示
void CMsgList::OnMsgSetup()
{
	CString str;
	CUIntArray aryIdx;

	/// 選択リスト情報
	long lngCnt = SelectedList(&aryIdx, TRUE);
	if (lngCnt != 1) return;
	long lngId = aryIdx.GetAt(0);

	/// 送信者名（スカイプ名）取得
	CTblRdfl tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(lngId) != 1) return;
	//// SelSetup()で再選択されるため、ここで値を確保
	CString strId = tbl.strSenderId;		
	CString strName = tbl.strSenderName;		

	/// 応答設定の表示
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfList *pfview = (CRsfList *)pFrame->m_pRsfList->GetWnd();
	if (!pfview->SelSetup(strId)) {
		//// 該当がないため新規応答設定の作成
		str.LoadString(IDS_MSGBOX52);
		if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return;

		CRsfNewDialog dlg;
		dlg.m_strId = strId;
//		dlg.m_pcmb->SetWindowText(strId);
		dlg.m_strName = strName;
		dlg.DoModal();
	}
	//// 応答設定TABの表示
	pFrame->m_pTabSetup->SetActiveTab();
}

// 受信データ削除
void CMsgList::OnMsgDel()
{
	Move(-1);
}

// ごみ箱内のアイテム削除
void CMsgList::OnTrashClear()
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
void CMsgList::OnTrashRecover()
{
	if (m_dwFolderType == FD_TRASH || m_dwFolderType == FD_TRASH_DEF) {
		TrashRecover();
	}
}

// 連携プログラムの実行
void CMsgList::OnPrg1()
{
	ExecPrg(1);
}
void CMsgList::OnPrg11()
{
	ExecPrg(11);
}
void CMsgList::OnPrg901()
{
	ExecPrg(901);
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
BOOL CMsgList::ContextMenu(
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
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	m_dwFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
	m_dwFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;

	/// POPUPメニュー表示
	CMenu menu;
	CMenu* pPopup;
	if (pfview->IsFolder(m_dwFolderType)) {
		VERIFY(menu.LoadMenu(IDR_POPUP_MSGLIST));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		if (lngSelCnt != 1) {
			pPopup->EnableMenuItem(ID_POPUP_MSGLIST_CALL, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_POPUP_MSGLIST_SETUP, MF_BYCOMMAND | MF_GRAYED);
			if (lngSelCnt == 0) {
				pPopup->EnableMenuItem(ID_POPUP_MSGLIST_DEL, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_POPUP_PRG1, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_POPUP_PRG11, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_POPUP_PRG901, MF_BYCOMMAND | MF_GRAYED);
			}
		}
	} else if (pfview->IsTrash(m_dwFolderType)) {
		VERIFY(menu.LoadMenu(IDR_POPUP_TRASH));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		if (lngSelCnt == 0) {
			pPopup->EnableMenuItem(ID_POPUP_TRASH_RECOVER, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_POPUP_TRASH_CLEAR, MF_BYCOMMAND | MF_GRAYED);
		}
	} else {
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
void CMsgList::InitListCtrl(void)
{
	CString str;

	// set column width according to window rect
//	m_wndList.Create(this, 0, NULL, LVS_REPORT | LVS_SHOWSELALWAYS);
	m_wndList.SetGridLines(FALSE);					// hide grid lines
	m_wndList.SetCheckboxeStyle(RC_CHKBOX_NONE);	// No checkboxes
	m_wndList.SetEditable(FALSE);					// disallow sub-text edit
	//// ヘッダー定義(title,size,aligne) 
	// ex) _T(" , 20, 0; 送信者, 100, 0; 日時, 110, 0")
	str.LoadString(IDS_MSGLIST_HEAD);
	m_wndList.SetColumnHeader(str);
	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT);// 行選択

	// リストのカラムサイズ調整
	AdustColumns();

//	m_wndList.SetHeaderImageList(IDB_BITMAP2);
//	m_wndList.SetHeaderImage(0, 0);
	
	m_wndList.SetImageList(&m_ListImage);
/*
	CRect rect;
	list.GetWindowRect(&rect);

	int w = rect.Width() - 2;
	int colwidths[6] = { 20, 150, 150 };

	TCHAR *	lpszHeaders[] = { _T(""),
							  _T("送信者"),
							  _T("日時"),
							  NULL };
	int i;
	int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// add columns
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = (i == 1 || i == 5) ? LVCFMT_LEFT : LVCFMT_CENTER;
		lvcolumn.pszText = lpszHeaders[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = (lpszHeaders[i+1] == NULL) ? w - total_cx - 2 : (w * colwidths[i]) / 64;
		total_cx += lvcolumn.cx;
		list.InsertColumn(i, &lvcolumn);
	}


	// create the image list from bitmap resource
	VERIFY(list.m_cImageList.Create(IDB_BITMAP1, 16, 3, RGB(255, 0, 255)));
	list.m_HeaderCtrl.SetImageList(&list.m_cImageList);

	// iterate through header items and attach the image list
	HDITEM hditem;

	for (i = 0; i < list.m_HeaderCtrl.GetItemCount(); i++)
	{
		hditem.mask = HDI_IMAGE | HDI_FORMAT;
		list.m_HeaderCtrl.GetItem(i, &hditem);
		hditem.fmt |=  HDF_IMAGE;
		if (i == 0 || i == 4)
			hditem.iImage = XHEADERCTRL_UNCHECKED_IMAGE;
		else
			hditem.iImage = XHEADERCTRL_NO_IMAGE;

		list.m_HeaderCtrl.SetItem(i, &hditem);
	}

	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// set the format again - must do this twice or first column does not get set
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
		lvcolumn.fmt = (i == 1 || i == 5) ? LVCFMT_LEFT : LVCFMT_CENTER;
		lvcolumn.iSubItem = i;
		list.SetColumn(i, &lvcolumn);
	}
*/
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
void CMsgList::AdustColumns(void) 
{
	if (m_wndList.GetSafeHwnd() != NULL)
	{
		CListCtrl *clc;
		CRect rect;
		int intFixSize;
		int intMargin;
		int intWidth;

		clc = &m_wndList;

		/// 固定値
		intFixSize = clc->GetColumnWidth(0) + clc->GetColumnWidth(2);	// アイコンと日時の幅
		intMargin = 80;	// マージン

		/// 変動項目の幅設定
		clc->GetParent()->GetClientRect(&rect);
		intWidth = rect.Width();
		if (intWidth > (intFixSize+intMargin)) {	
			intWidth = (intWidth - intFixSize - 20) / 2;	// 20:スクロールバーの幅
			clc->SetColumnWidth(1, intWidth);				// 送信者名
			clc->SetColumnWidth(3, intWidth);				// タイトル
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
BOOL CMsgList::UpdateForm() 
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
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	m_dwFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
	m_dwFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;
//	DWORD lngFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
//	long lngFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;

	/// リストの生成
	if (pfview->IsFolderWT(m_dwFolderType)) {
		//// メッセージリストの生成
		BeginWaitCursor();
		MakeList(m_dwFolderType, m_dwFolderId);
		EndWaitCursor();
	}
	
	/// リストの先頭行を選択
	if (m_wndList.GetItemCount() > 0) {
		m_wndList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

	return TRUE;
}

//
//  機能     : カレントフォルダに対応したメッセージリストの生成
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgList::MakeList(
DWORD lngFolderType,	// フォルダタイプ
long lngFolderId)		// フォルダID
{
	_TCHAR buf[32];

	CTblRdfl tbl(theApp.m_strDBPath);

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
		strWhere = strWhere + _T(" order by lngSystemNew desc");
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
		tbl.OpenTbl(_T("rdfl left outer join folder on rdfl.lngFolderId=folder.ID"));
		strWhere = strWhere + _T("rdfl.strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		strWhere = strWhere + _T(" and ") + _T("(folder.ID is NULL or folder.lngSystemFlag=0)");
		strWhere = strWhere + _T(" and ") + _T("rdfl.lngSystemFlag=1");
		strWhere = strWhere + _T(" order by rdfl.lngSystemNew desc");
		break;
	default:
		return FALSE;
	}

	CString strDef;
	strDef.LoadString(IDS_MSG_DEF);
	CString strSender;
	CString strTitle;
	CString strPlayFile;
	_TCHAR strDate[64];
	int intIdx;
	CTPsetup tps(theApp.m_strDataFolder);
	long lngCnt = tbl.SelectRec(strWhere);
	// 01.May.08 高速化（ファイル存在チェックの方が重いので効果は少ない）
	m_wndList.SetItemCount(lngCnt);	
	m_wndList.SetRedraw( FALSE );
	m_wndList.LockWindowUpdate();
	while (lngCnt > 0)
	{
		//// [送信者名]取得
		if (tbl.strSenderId == _T("0")) {
			//// 不明な発信者(SkypeIn)
			strSender = strDef;
		} else {
			if (tbl.strSenderName.IsEmpty()) {
				strSender = tbl.strSenderId;
			} else {
				strSender = tbl.strSenderName + _T(" <") + tbl.strSenderId + _T(">");
			}
		}
		//// [日時]取得
		CnvDateString((LPCTSTR)tbl.dlngSystemNew, strDate);
		//// [件名]取得
		strTitle = tbl.strTitle;
		//// リストにデータ設定
		intIdx = m_wndList.InsertItemEx(INT_MAX, _T(""), strSender, strDate, strTitle);
		//// ビデオファイルがあればビデオアイコン表示
		CFileSpec fs;
		tps.SetCallerId(tbl.strSenderId);
		strPlayFile = tps.FindRdfVideoFileName(tbl.strRemarks1,tbl.dlngSystemNew, _T("$$"));
		if (!strPlayFile.IsEmpty()) {
			if (tbl.lngRespType == 1) {
			///// シンプル応答
				m_wndList.SetItemImage(intIdx, 0, 3);	// videoアイコン
			} else if (tbl.lngRespType == 2) {
			///// 複雑な応答
			} else {
			///// 直接録画
				if (tbl.lngClsCode1 == 1 || tbl.lngClsCode1 == 3) {
					m_wndList.SetItemImage(intIdx, 0, 7);	// incoming videoアイコン
				} else if (tbl.lngClsCode1 == 2 || tbl.lngClsCode1 == 4) {
					m_wndList.SetItemImage(intIdx, 0, 8);	// outgoing videoアイコン
				} else {
					m_wndList.SetItemImage(intIdx, 0, 1);	// videoアイコン
				}
			}
		} else {
			//// 音声ファイルがあれば音声アイコン表示
			strPlayFile = tps.FindRdfWavFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
			if (strPlayFile.IsEmpty()) {
				strPlayFile = tps.FindRdfMp3FileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
			}
			if (!strPlayFile.IsEmpty()) {
				if (tbl.lngRespType == 1) {
				///// シンプル応答
					m_wndList.SetItemImage(intIdx, 0, 2);	// soundアイコン
				} else if (tbl.lngRespType == 2) {
				///// 複雑な応答
				} else {
				///// 直接録音
					if (tbl.lngClsCode1 == 1 || tbl.lngClsCode1 == 3) {
						m_wndList.SetItemImage(intIdx, 0, 5);	// incoming soundアイコン
					} else if (tbl.lngClsCode1 == 2 || tbl.lngClsCode1 == 4) {
						m_wndList.SetItemImage(intIdx, 0, 6);	// outgoing soundアイコン
					} else {
						m_wndList.SetItemImage(intIdx, 0, 0);	// soundアイコン
					}
				}
			} else {
				m_wndList.SetItemImage(intIdx, 0, -1);	// no icon
			}
		}
		//// 未再生レコードのみ強調表示
		if (!tbl.lngViewFlag) {
			m_wndList.SetItemBkColor(intIdx, -1, m_clrUnview, FALSE);
			m_wndList.SetItemTextBold(intIdx, -1, TRUE, FALSE);
		}
		//// レコードIDの保存
		m_wndList.SetItemData(intIdx, tbl.ID);
		if (!tbl.NextRec()) break;
		--lngCnt;
	}
	// 01.May.08 高速化
	m_wndList.UnlockWindowUpdate();
	m_wndList.SetRedraw( TRUE );

	return TRUE;
}

//
//  機能     : メッセージの移動
//  
//  機能説明 : ごみ箱対応
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgList::Move(
long lngFolderId)		// -1:ごみ箱へ移動
{
	CString str;
	UINT intCnt, i = 0;
	int idx;
	CUIntArray aryIdx;

	/// 選択リスト情報
	intCnt = SelectedList(&aryIdx, FALSE);
	if (intCnt < 1) return FALSE;

	/// テーブルを更新
	long lngId;
	_TCHAR buf[32];
	CTblRdfl tbl(theApp.m_strDBPath);
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
//  機能     : ごみ箱(受信)内のリスト削除
//  
//  機能説明 : 完全削除
//  
//  返り値   : BOOL
//  
//  備考     : トランザクションが必要なので、１関数で実装
//  
BOOL CMsgList::TrashDel()
{
	_TCHAR buf[32];
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

	/// メッセージテーブルを更新
	long lngId;
	CTblRdfl tbl(theApp.m_strDBPath);
	//// 音声・ビデオファイルの削除
	CFileSpec fs;
	CTPsetup tps(theApp.m_strDataFolder);
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		if (tbl.SelectRec(lngId) <= 0) continue;
		tps.SetCallerId(tbl.strSenderId);
		///// wav file
		str = tps.FindRdfWavFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
		if (!str.IsEmpty()) {
			fs.SetFullSpec(str);
			fs.FileDelete();
		}
		///// mp3 file
		str = tps.FindRdfMp3FileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
		if (!str.IsEmpty()) {
			fs.SetFullSpec(str);
			fs.FileDelete();
		}
		///// avi file
		str = tps.FindRdfVideoFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
		if (!str.IsEmpty()) {
			fs.SetFullSpec(str);
			fs.FileDelete();
		}
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
	if (!tbl.TransactionEnd()) return FALSE;

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
//  機能     : ごみ箱（受信）内のリストを元に戻す
//  
//  機能説明 : 削除フラグを解除
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgList::TrashRecover()
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
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();

	/// メッセージテーブルを更新
	//// 選択リスト情報
	intCnt = SelectedList(&aryIdx, FALSE);
	if (intCnt < 1) return FALSE;
	//// リカバリ先フォルダの存在チェック
	CTblRdfl tbl(theApp.m_strDBPath);
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
//  機能     : リスト選択リスト
//  
//  機能説明 : 複数選択されている場合は最初のID
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgList::SelectId(int idx)		// リストのID			
{
	UINT intCnt;

	/// 選択済みリスト解除
	//intCnt = m_wndList.GetSelectedCount();
	//if (intCnt > 1) {
	//	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	//	if (pos != NULL) {
	//		for (i = 0; i < intCnt; i++) {
	//			id = m_wndList.GetNextSelectedItem(pos);
	//			m_wndList.SetItemState(id, 0, LVIS_SELECTED);
	//			if (pos == NULL) break;
	//		}
	//	}
	//}
	m_wndList.SetAllItemStates(RC_ITEM_SELECTED, RC_ITEM_UNSELECTED);
	m_wndList.SetAllItemStates(RC_ITEM_FOCUSED, RC_ITEM_UNFOCUSED);

	intCnt = m_wndList.GetItemCount();
	if (idx < 0) {
		idx = 0;
	} else if (intCnt-1 < (UINT)idx) {
		idx = intCnt-1;
	}

	m_wndList.SetItemState(idx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	m_wndList.SetSelectionMark(idx);

	return idx;
}

//
//  機能     : 選択リストIDの取得
//  
//  機能説明 : 複数選択されている場合は最初のID
//  
//  返り値   : リストのID、-1:非選択
//  
//  備考     : 
//  
long CMsgList::SelectedId()			
{
	UINT intCnt;
	int idx;

	/// 選択リスト情報
	intCnt = m_wndList.GetSelectedCount();
	if (intCnt < 1) return -1;

	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	if (pos == NULL) return -1;

	idx = m_wndList.GetNextSelectedItem(pos);

	return idx;
}

//
//  機能     : メッセージ選択リストの取得
//  
//  機能説明 : 
//  
//  返り値   : 取得リスト件数
//  
//  備考     : 
//  
long CMsgList::SelectedList(
CUIntArray *pAryIdx,	// リスト保存配列
BOOL blData)			// TRUE:リストインデックスの代わりにアイテムデータを保存
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
		if (blData) {
			pAryIdx->Add(intId);
		} else {
			pAryIdx->Add(idx);
		}
		if (pos == NULL) break;
	}

	return pAryIdx->GetSize();
}

//
//  機能     : クライアント座標変換（MsgList->MsgTree）
//  
//  機能説明 : 
//  
//  返り値   : MsgTreeクライアント座標
//  
//  備考     : 
//  
CPoint CMsgList::CnvPntM_F(
const CPoint point)	// MsgListクライアント座標
{
	CPoint pnt = point;
	ClientToScreen(&pnt);
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
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
BOOL CMsgList::Play() 
{
	/// カレントフォルダの情報取得
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
	if (pfview->IsFolderWT(pfview->m_dwFolderType)) {
		CMsgForm *pform = (CMsgForm *)pFrame->m_pMsgForm->GetWnd();
		pform->Play();
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
BOOL CMsgList::ChkPlayIcon(
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

//
//  機能     : 未再生リストを再生済みに変更
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgList::ChgMsgMark(
BOOL blUpdate)	// テーブルUPDATEを含むか
{
	CReportCtrl &rc = m_wndList;
	COLORREF clr = rc.GetItemBkColor(m_hActive, 2);
	if (clr == m_clrUnview) {	// 未再生の場合のみ
		/// テーブルを更新
		_TCHAR buf[32];
		CTblRdfl tbl(theApp.m_strDBPath);
		CString strWhere;
		long lngId = rc.GetItemData(m_hActive);
		strWhere.Empty();
		_ltot_s(lngId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T("ID") + _T("=") + buf;
		tbl.UpdateRec(_T("lngViewFlag=1"), strWhere);	
		//// リストを更新
		rc.SetItemBkColor(m_hActive, -1);	// 背景色
		rc.SetItemTextBold(m_hActive, -1);	// BOLD
	}

	return TRUE;
}

//
//  機能     : 連携プログラムの実行
//  
//  機能説明 : デフォルト設定を使用（ユーザ対応ではない）
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgList::ExecPrg(
int intPrgId)		// プログラムID
{
	CString str;
	UINT intCnt, i = 0;
	int idx;

	if (intPrgId < 1) return FALSE;
	CString strPrgId;
	strPrgId.Format(_T("%d"), intPrgId);

	// 連携プログラムCSVファイル
	CTPsetup tps(theApp.m_strDataFolder, _T("0"));	// デフォルト設定
	CString strFile = tps.RsfPrgFileName(_T("$$$$"), strPrgId);
	CFileSpec fs(strFile);
	if (!fs.Exist()) {
		AfxTrace(_T("%s\n"), strFile);
		str.LoadString(IDS_MSGBOX107);
		te.ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_TAPUR, str);
		return FALSE;
	}
	// 連携プログラムCSVの内容を取得
	CString strCsv;
	if (!fs.LoadString(&strCsv)) {
		AfxTrace(_T("%s\n"), strCsv);
		str.LoadString(IDS_MSGBOX107);
		te.ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_TAPUR, str);
		return FALSE;
	}


	/// 選択リスト情報
	CUIntArray aryIdx;
	intCnt = SelectedList(&aryIdx, FALSE);
	if (intCnt < 1) return FALSE;

	/// 各データについて実行
	long lngId;
	CTblRdfl tbl(theApp.m_strDBPath);
	CTblPerson tblPerson(theApp.m_strDBPath);
	CString strWhere;
	for (i = 0; i < intCnt; i++) {
		// 受信データ
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		if (tbl.SelectRec(lngId) != 1) continue;

		// 発信者情報
		strWhere.Format(_T("strId='%s'"), tbl.strSenderId);
		if (tblPerson.SelectRec(strWhere) != 1) continue;

		// 連携プログラムのパラメータ生成
		CTokenizer tok(strCsv, _T(","));
		CString cs, strExeFile;
		CStringArray aryArg;
		int k = 0;
		BOOL blExe = TRUE;
		BOOL blWait = FALSE;
		while(tok.Next(cs)) {
			if (k == 0) {
				// プログラムの処理待ち（並列実行しない）
				blWait = (cs.CompareNoCase(_T("1")) == 0) ? TRUE : FALSE;
			} else if (k == 1) {
				// 実行ファイル取得
				strExeFile = cs;
				if (strExeFile.Find(_T('\\')) < 0) {
					// パス未指定→テンプレートCSVフォルダを追加
					fs.SetFullSpec(FS_APPDIR);
					strExeFile = fs.GetFullSpec() + _T("bin\\") + strExeFile;
				}
				fs.SetFullSpec(strExeFile);
				if (!fs.Exist()) {
					str.LoadString(IDS_MSGBOX107);
					te.ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_TAPUR, str);
					blExe = FALSE;
					return FALSE;
				}
			} else if (2 <= k) {
				// 引数取得
				if (cs.CompareNoCase(_T("$AudioFile")) == 0) {				// 録音ファイル
					tps.SetCallerId(tbl.strSenderId);
					///// mp3 file
					strFile = tps.FindRdfMp3FileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
					if (strFile.IsEmpty()) {
						///// wav file
						strFile = tps.FindRdfWavFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
					}
					aryArg.Add(strFile);
				} else if (cs.CompareNoCase(_T("$VideoFile")) == 0) {			// 録画ファイル
					tps.SetCallerId(tbl.strSenderId);
					///// avi file
					strFile = tps.FindRdfVideoFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
					aryArg.Add(strFile);
				} else if (cs.CompareNoCase(_T("$PartnerId")) == 0) {			// 通話相手(スカイプID)
						aryArg.Add(tbl.strSenderId);
				} else if (cs.CompareNoCase(_T("$PartnerName")) == 0) {		// 通話相手(スカイプ名)
						aryArg.Add(tbl.strSenderName);
				} else if (cs.CompareNoCase(_T("$UsrId")) == 0) {				// ユーザ(スカイプID)
						aryArg.Add(tbl.strReceiverId);
				} else if (cs.CompareNoCase(_T("$UsrName")) == 0) {			// ユーザ(スカイプ名)
						aryArg.Add(tbl.strReceiverName);
				} else if (cs.CompareNoCase(_T("$PartnerAddress")) == 0) {		// 通話相手住所
						str = tblPerson.strCountry;
						str += tblPerson.strProvince;
						str += tblPerson.strCity;
						aryArg.Add(str);
				} else if (cs.CompareNoCase(_T("$PartnerPhoneHome")) == 0) {	// 通話相手電話（自宅）
						aryArg.Add(tblPerson.strTel);
				} else if (cs.CompareNoCase(_T("$PartnerPhoneOffice")) == 0) {	// 通話相手電話（オフィス）
						aryArg.Add(tblPerson.strWkTel);
				} else if (cs.CompareNoCase(_T("$PartnerPhoneMobile")) == 0) {	// 通話相手電話（モバイル）
						aryArg.Add(tblPerson.strMbTel);
				} else {
					if (cs.Find(_T("(")) >= 0 && cs.Find(_T(")")) >= 0) {
						// 説明表示のままの場合
						aryArg.Add(_T(""));
					} else {
						aryArg.Add(cs);
					}
				}
			}
			k++;
		}

		if (blExe) {
			// 実行
			fs.SetFullSpec(strExeFile);
			strExeFile = _T("\"") + strExeFile + _T("\"");
			CString strPrm = _T("");
			for (k = 0; k < aryArg.GetCount(); k++) {
				if (k > 0) strPrm = strPrm + _T(" ");
				strPrm = strPrm + _T("\"") + aryArg.GetAt(k) + _T("\"");
			}
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, strExeFile + _T(" ") + strPrm);
			if (blWait) {
				BOOL blRet;
				if (strExeFile.Right(5).CompareNoCase(_T(".bat\"")) == 0) {
					// batの実行
					strExeFile += _T(" ") + strPrm;
					blRet = ExeProcess((LPTSTR)(LPCTSTR)strExeFile, (LPTSTR)(LPCTSTR)fs.GetFolder(), SW_HIDE, 1000*10);	// timeout:10s
				} else {
					// exeの実行
					strExeFile += _T(" ") + strPrm;
					blRet = ExeProcess((LPTSTR)(LPCTSTR)strExeFile, (LPTSTR)(LPCTSTR)fs.GetFolder(), SW_SHOWNORMAL, 1000*10);
				}
				if (!blRet) {
					str.LoadString(IDS_MSGBOX107);
					te.ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_TAPUR, str);
					return FALSE;
				}
			} else {
				HINSTANCE ret;
				if (strExeFile.Right(5).CompareNoCase(_T(".bat\"")) == 0) {
					// batの実行
					ret = ShellExecute(NULL, _T("open"), strExeFile,  strPrm, fs.GetFolder(), SW_HIDE);
				} else {
					// exeの実行
					ret = ShellExecute(NULL, NULL, strExeFile,  strPrm, fs.GetFolder(), SW_SHOWNORMAL);
				}
				if ((int)ret <= 32) {
					str.LoadString(IDS_MSGBOX107);
					te.ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_TAPUR, str);
					return FALSE;
				}
			}
		}
	}

	str.LoadString(IDS_MSGBOX108);
	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
	return TRUE;
}

// CreateProcess()を使って外部アプリケーションを起動する
BOOL CMsgList::ExeProcess(
LPTSTR cmdstr,		// 実行コマンド
LPTSTR dirstr,		// 実行フォルダ
int nShowCmd,		// 実行時のウィンドウ状態
DWORD dwTimeout)	// タイムアウト（ms）
{
	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;

	ZeroMemory( &sinfo, sizeof( STARTUPINFO ) );
	ZeroMemory( &pinfo, sizeof( PROCESS_INFORMATION ) );
	sinfo.cb = sizeof( STARTUPINFO );
	sinfo.dwFlags = STARTF_USESHOWWINDOW;
	sinfo.wShowWindow = nShowCmd;

	if ( !CreateProcess( NULL,
						 cmdstr,
						 NULL,
						 NULL,
						 FALSE,
						 0,
						 NULL,
						 dirstr,
						 &sinfo,
						 &pinfo ) ) { 
		// 起動失敗
		return FALSE;
	}

	// 処理待ち
    WaitForSingleObject( pinfo.hProcess, dwTimeout );

    CloseHandle( pinfo.hProcess );
    CloseHandle( pinfo.hThread );

	return TRUE;
}
