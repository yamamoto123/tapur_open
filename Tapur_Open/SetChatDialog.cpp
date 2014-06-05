// SetChatDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Registry.h"
#include "SetChatDialog.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CSetChatDialog ダイアログ


CSetChatDialog::CSetChatDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSetChatDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetChatDialog)
	m_strChat = _T("");
	//}}AFX_DATA_INIT
}

CSetChatDialog::~CSetChatDialog()
{
}

void CSetChatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetChatDialog)
	DDX_Text(pDX, IDC_EDIT_CHAT, m_strChat);
	DDV_MaxChars(pDX, m_strChat, 400);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetChatDialog, CDialog)
	//{{AFX_MSG_MAP(CSetChatDialog)
	ON_BN_CLICKED(IDC_BTN_INIT, OnButtonInit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetChatDialog メッセージ ハンドラ

BOOL CSetChatDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	// 画面更新
	UpdateForm();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/// 対応レジストリの初期値を表示
void CSetChatDialog::OnButtonInit() 
{
	if (m_strRegKey.IsEmpty()) return;

	if (m_strRegKey.Find(_T("VoiceRecordWarning")) > 0) {
		m_strChat.LoadString(IDS_TAB_REC_WARNING);
	} else if (m_strRegKey.Find(_T("VoiceRecordStopWarning")) > 0) {
		m_strChat.LoadString(IDS_TAB_RECSTOP_WARNING);
	} else if (m_strRegKey.Find(_T("VideoRecordWarning")) > 0) {
		m_strChat.LoadString(IDS_TAB_VREC_WARNING);
	} else if (m_strRegKey.Find(_T("VideoRecordStopWarning")) > 0) {
		m_strChat.LoadString(IDS_TAB_VRECSTOP_WARNING);
	} else {
		return;
	}

	UpdateData(FALSE);	// DDX更新
}

void CSetChatDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CString str;

	UpdateData(TRUE);	// DDX更新

	/// レジストリ保存
	//// 文字列レジストリでは空白入力が無視される場合があるのでforce=TRUEに
	if (!m_strRegKey.IsEmpty()) {
		CRegString rgWarning = CRegString(m_strRegKey, _T(""), TRUE);
		rgWarning = m_strChat;
	}

	CDialog::OnOK();
}

void CSetChatDialog::OnCancel() 
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
BOOL CSetChatDialog::UpdateForm() 
{
	CString str;

	/// 対応レジストリの内容を画面に表示
	if (m_strRegKey.IsEmpty()) return FALSE;

	if (m_strRegKey.Find(_T("VoiceRecordWarning")) > 0) {
		str.LoadString(IDS_TAB_REC_WARNING);
	} else if (m_strRegKey.Find(_T("VoiceRecordStopWarning")) > 0) {
		str.LoadString(IDS_TAB_RECSTOP_WARNING);
	} else if (m_strRegKey.Find(_T("VideoRecordWarning")) > 0) {
		str.LoadString(IDS_TAB_VREC_WARNING);
	} else if (m_strRegKey.Find(_T("VideoRecordStopWarning")) > 0) {
		str.LoadString(IDS_TAB_VRECSTOP_WARNING);
	} else {
		return FALSE;
	}

	m_strChat = CRegString(m_strRegKey, str, TRUE);	

	UpdateData(FALSE);	// DDX更新

	return TRUE;
}

