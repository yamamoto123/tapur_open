// RsfFolderRenameDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfFolderRenameDialog.h"
#include "RsfTree.h"
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
// CRsfFolderRenameDialog ダイアログ


CRsfFolderRenameDialog::CRsfFolderRenameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRsfFolderRenameDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRsfFolderRenameDialog)
	m_strFolder = _T("");
	//}}AFX_DATA_INIT
}


void CRsfFolderRenameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfFolderRenameDialog)
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_strFolder);
	DDV_MaxChars(pDX, m_strFolder, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRsfFolderRenameDialog, CDialog)
	//{{AFX_MSG_MAP(CRsfFolderRenameDialog)
	ON_EN_SETFOCUS(IDC_EDIT_FOLDER, OnSetfocusEditId)
	ON_EN_KILLFOCUS(IDC_EDIT_FOLDER, OnKillfocusEditId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfFolderRenameDialog メッセージ ハンドラ

void CRsfFolderRenameDialog::OnSetfocusEditId()
{
/*
	/// IMEを半角英数字に変更
	m_hImc = ImmGetContext(m_hWnd); 
	ImmGetConversionStatus(m_hImc, &m_dwImeConv, &m_dwImeSent); 
	ImmSetConversionStatus(m_hImc, m_dwImeConv, IME_SMODE_NONE);
*/
}

void CRsfFolderRenameDialog::OnKillfocusEditId()
{
/*
	/// IMEの状態復帰
	ImmSetConversionStatus(m_hImc, m_dwImeConv, m_dwImeSent); 
	ImmReleaseContext(m_hWnd, m_hImc);
*/
}

void CRsfFolderRenameDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

	if (!UpdateData(TRUE)) return;	// DDX更新

	/// 選択フォルダ情報
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	DWORD type = tc.GetItemData(pfview->m_hContext) & FD_MASK_TYPE;
	if (!(type == FD_NORMAL)) return;
	long lngFolderId = tc.GetItemData(pfview->m_hContext) & FD_MASK_ID;
	CString strTitle = tc.GetItemText(pfview->m_hContext);

	/// フォルダ名のチェック
	if (!pfview->ChkFolder(m_strFolder, type)) return;

	/// folderテーブルを更新
	CTblFolder tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(lngFolderId) != 1) {
		return;
	}
	tbl.strTitle = m_strFolder;				// フォルダ名
	tbl.UpdateRec(lngFolderId);

	/// フォルダリストを更新
	tc.SetItemText(pfview->m_hContext, m_strFolder);
	tc.EnsureVisible(pfview->m_hContext);

	CDialog::OnOK();
}

void CRsfFolderRenameDialog::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function


