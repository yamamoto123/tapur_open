// SettingDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "SettingDialog.h"
#include "MsgList.h"
#include "TPtable.h"
#include "TPutil.h"
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
// CSettingDialog ダイアログ


CSettingDialog::CSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingDialog)
	//}}AFX_DATA_INIT
}


void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingDialog)
	DDX_Control(pDX, IDC_TAB, m_tabCtrl);
	DDX_Control(pDX, IDC_APPLY, m_Apply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialog)
	//{{AFX_MSG_MAP(CSettingDialog)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDialog メッセージ ハンドラ

BOOL CSettingDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	// Setup the tab control
	CString str;
	int nPageID = 0;
	str.LoadString(IDS_TAB_PUB);
	m_tabPub.Create (IDD_TAB_PUB, this);
	m_tabCtrl.AddSSLPage (str, nPageID++, &m_tabPub);
	str.LoadString(IDS_TAB_REC);
	m_tabRec.Create (IDD_TAB_REC, this);
	m_tabCtrl.AddSSLPage (str, nPageID++, &m_tabRec);
	str.LoadString(IDS_TAB_VREC);
	m_tabVRec.Create (IDD_TAB_VREC, this);
	m_tabCtrl.AddSSLPage (str, nPageID++, &m_tabVRec);
	str.LoadString(IDS_TAB_ADMIN);
	m_tabAdmin.Create (IDD_TAB_ADMIN, this);
	m_tabCtrl.AddSSLPage (str, nPageID++, &m_tabAdmin);
	
	/// 適用ボタンを無効
	m_Apply.EnableWindow(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/// 各TABの値の保存
void CSettingDialog::OnApply() 
{
	// 設定チェック
	if (!m_tabPub.Check())	return;
	if (!m_tabRec.Check())	return;
	if (!m_tabVRec.Check())	return;
	if (!m_tabAdmin.Check())	return;

	// 保存
	m_tabPub.Save();
	m_tabRec.Save();
	m_tabVRec.Save();
	m_tabAdmin.Save();

	/// 適用ボタンを無効
	m_Apply.EnableWindow(FALSE);
}

void CSettingDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

	/// 各TABの値の保存
	OnApply();

	CDialog::OnOK();
}

void CSettingDialog::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function


