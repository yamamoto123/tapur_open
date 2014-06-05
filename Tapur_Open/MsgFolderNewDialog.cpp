// MsgFolderNewDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "MsgFolderNewDialog.h"
#include "MsgTree.h"
#include "TPtable.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgFolderNewDialog ダイアログ


CMsgFolderNewDialog::CMsgFolderNewDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgFolderNewDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgFolderNewDialog)
	m_strFolder = _T("");
	//}}AFX_DATA_INIT
}


void CMsgFolderNewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgFolderNewDialog)
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_strFolder);
	DDV_MaxChars(pDX, m_strFolder, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgFolderNewDialog, CDialog)
	//{{AFX_MSG_MAP(CMsgFolderNewDialog)
	ON_EN_SETFOCUS(IDC_EDIT_FOLDER, OnSetfocusEditId)
	ON_EN_KILLFOCUS(IDC_EDIT_FOLDER, OnKillfocusEditId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgFolderNewDialog メッセージ ハンドラ

void CMsgFolderNewDialog::OnSetfocusEditId()
{
/*
	/// IMEを半角英数字に変更
	m_hImc = ImmGetContext(m_hWnd); 
	ImmGetConversionStatus(m_hImc, &m_dwImeConv, &m_dwImeSent); 
	ImmSetConversionStatus(m_hImc, m_dwImeConv, IME_SMODE_NONE);
*/
}

void CMsgFolderNewDialog::OnKillfocusEditId()
{
/*
	/// IMEの状態復帰
	ImmSetConversionStatus(m_hImc, m_dwImeConv, m_dwImeSent); 
	ImmReleaseContext(m_hWnd, m_hImc);
*/
}

void CMsgFolderNewDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

	if (!UpdateData(TRUE)) return;	// DDX更新

	/// 選択フォルダ情報
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	DWORD type = tc.GetItemData(pfview->m_hContext) & FD_MASK_TYPE;
	if (!(type == FD_NORMAL || type == FD_NORMAL_DEF)) return;

	/// フォルダ名のチェック
	if (!pfview->ChkFolder(m_strFolder, type)) return;

	/// folderテーブルに追加
	CTblFolder tbl(theApp.m_strDBPath);
	tbl.strReceiverId = theApp.m_strUsrId;	// 受信者名（スカイプ名）
	tbl.strTitle = m_strFolder;				// フォルダ名
	tbl.InsertRec();
	//// 追加レコードのID取得
	CString strWhere;
	strWhere.Empty();
	long lngId = (long)tbl.MaxVal(_T("ID"), strWhere);

	/// フォルダリストへ追加
	HTREEITEM h;
	h = pfview->AddItem(pfview->m_hFolderDef, m_strFolder, FD_NORMAL, lngId);
	tc.EnsureVisible(h);

	CDialog::OnOK();
}

void CMsgFolderNewDialog::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function

