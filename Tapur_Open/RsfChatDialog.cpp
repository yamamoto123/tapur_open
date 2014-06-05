// RsfChatDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfChatDialog.h"
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
// CRsfChatDialog ダイアログ


CRsfChatDialog::CRsfChatDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRsfChatDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRsfChatDialog)
	m_strChat = _T("");
	//}}AFX_DATA_INIT
}

CRsfChatDialog::~CRsfChatDialog()
{
}

void CRsfChatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfChatDialog)
	DDX_Text(pDX, IDC_EDIT_CHAT, m_strChat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRsfChatDialog, CDialog)
	//{{AFX_MSG_MAP(CRsfChatDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfChatDialog メッセージ ハンドラ

BOOL CRsfChatDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	// 画面更新
	UpdateForm();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CRsfChatDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CString str;

	UpdateData(TRUE);	// DDX更新

	/// テキストファイル保存
	CFileSpec fs(m_strPlayFile);
	if (!fs.SaveString(m_strChat)) {
		str.LoadString(IDS_MSGBOX101);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}

	CDialog::OnOK();
}

void CRsfChatDialog::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function

//
//  機能     : 画面の更新
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfChatDialog::UpdateForm() 
{
	/// 応答チャットファイルがある場合、画面に表示
	CFileSpec fs(m_strPlayFile);
	if (!fs.Exist()) return TRUE;
	if (!fs.LoadString(&m_strChat)) return FALSE;

	UpdateData(FALSE);	// DDX更新

	return TRUE;
}

