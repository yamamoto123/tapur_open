// SettingAdminTab.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "SettingAdminTab.h"
#include "SettingDialog.h"
#include "FileSpec.h"
#include "FolderDialog.h"
#include "SetHotkey.h"
#include "TPutil.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingAdminTab ダイアログ


CSettingAdminTab::CSettingAdminTab(CWnd* pParent /*=NULL*/)
	: CTabPageSSL (CSettingAdminTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingAdminTab)
	m_blHotKey = TRUE;
	m_blEnableLog = TRUE;
	m_blDataPath = TRUE;
	//}}AFX_DATA_INIT
}


void CSettingAdminTab::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingAdminTab)
    DDX_Control(pDX, IDC_DATAPATH, m_edtDataPath);
    DDX_Control(pDX, IDC_BROWSE_DATAPATH, m_btnDataPath);
    DDX_Text(pDX, IDC_EDIT_SKYPE_PORT_OUT, m_strSkypePortOut);
    DDX_Text(pDX, IDC_EDIT_SKYPE_PORT_MIC, m_strSkypePortMic);
	DDX_Check(pDX, IDC_CHK_DATAPATH, m_blDataPath);
	DDX_Check(pDX, IDC_CHK_HOTKEY, m_blHotKey);
	DDX_Check(pDX, IDC_CHK_LOG, m_blEnableLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingAdminTab, CDialog)
	//{{AFX_MSG_MAP(CSettingAdminTab)
	ON_BN_CLICKED(IDC_BTN_HOTKEY, OnBtnHotkey)
	ON_BN_CLICKED(IDC_BROWSE_DATAPATH, OnBrowseDataPath)
	ON_BN_CLICKED(IDC_CHK_DATAPATH, OnChgDataPath)
	ON_BN_CLICKED(IDC_CHK_LOG, OnChgForm)
	ON_EN_CHANGE(IDC_EDIT_SKYPE_PORT_OUT, OnChgSkypePortOut)
	ON_EN_CHANGE(IDC_EDIT_SKYPE_PORT_MIC, OnChgSkypePortMic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingAdminTab メッセージ ハンドラ

BOOL CSettingAdminTab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CString str;
	DWORD dw;

	/// レジストリ読み込み
	m_rgDataPath = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\DataPath"), _T(""), TRUE);
	m_rgEnableDataPath = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableDataPath"), 0, TRUE);
	m_rgEnableHotKey = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableHotKey"), 1, TRUE);
	m_rgEnableLog = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableLog"), 1, TRUE);
	m_rgSkypePortOut = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortOut"), 42805, TRUE);
	m_rgSkypePortMic = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortMic"), 42806, TRUE);

	/// Hotkey
	dw = m_rgEnableHotKey;
	m_blHotKey = (dw == 1) ? TRUE : FALSE;
	/// データパス
	dw = m_rgEnableDataPath;
	m_blDataPath = (dw == 1) ? TRUE : FALSE;
	str = m_rgDataPath;
	m_edtDataPath.SetWindowText(str);
	m_btnDataPath.EnableWindow(m_blDataPath);
	m_edtDataPath.EnableWindow(m_blDataPath);
	/// ログ
	dw = m_rgEnableLog;
	m_blEnableLog = (dw == 1) ? TRUE : FALSE;
	/// SKYPE PORT
	dw = m_rgSkypePortOut;
	m_strSkypePortOut.Format(_T("%d"), dw);
	dw = m_rgSkypePortMic;
	m_strSkypePortMic.Format(_T("%d"), dw);

	UpdateData(FALSE);	// DDX更新
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// フォーム入力イベント
void CSettingAdminTab::OnChgForm() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	/// 適用ボタンの有効化
	CWnd *cWnd;
	cWnd = GetParentOwner();
	CSettingDialog *pDlg = (CSettingDialog *)cWnd;
	pDlg->EnableApply();
}

// ショートカット詳細設定ボタン
void CSettingAdminTab::OnBtnHotkey() 
{
	UpdateData(TRUE);	// DDX更新

	static CSetHotkey dlg;

	if (dlg.m_hWnd == NULL) {
		dlg.Create(IDD_HOTKEY, this);
	} else {
		if (dlg.IsWindowVisible()) {
			dlg.SetFocus();
			return;
		}
	}

	dlg.ShowWindow(SW_SHOW);
}

// データフォルダ変更チェックイベント
void CSettingAdminTab::OnChgDataPath() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	UpdateData(TRUE);	// DDX更新

	// データフォルダ変更ボタンとテキストボックスの有効・無効
	if (m_blDataPath) {
		m_btnDataPath.EnableWindow(TRUE);
		m_edtDataPath.EnableWindow(TRUE);
	} else {
		m_btnDataPath.EnableWindow(FALSE);
		m_edtDataPath.EnableWindow(FALSE);
	}

	UpdateData(FALSE);	// DDX更新

	/// 適用ボタンの有効化
	OnChgForm();
}

// SKYPE OUPUT PORT変更チェックイベント
void CSettingAdminTab::OnChgSkypePortOut() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString strOld = m_strSkypePortOut;
	UpdateData(TRUE);	// DDX更新

	// 整数のみ有効
	if (!ChkNaturalNumber(m_strSkypePortOut)) {
		m_strSkypePortOut = strOld;
		UpdateData(FALSE);	// DDX更新
		return;
	}

	UpdateData(FALSE);	// DDX更新

	/// 適用ボタンの有効化
	OnChgForm();
}
// SKYPE MIC PORT変更チェックイベント
void CSettingAdminTab::OnChgSkypePortMic() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString strOld = m_strSkypePortMic;
	UpdateData(TRUE);	// DDX更新

	// 整数のみ有効
	if (!ChkNaturalNumber(m_strSkypePortMic)) {
		m_strSkypePortMic = strOld;
		UpdateData(FALSE);	// DDX更新
		return;
	}

	UpdateData(FALSE);	// DDX更新

	/// 適用ボタンの有効化
	OnChgForm();
}

// データフォルダ変更ボタン
void CSettingAdminTab::OnBrowseDataPath() 
{
	UpdateData(TRUE);	// DDX更新

	CString str;

	// フォルダ選択
	CFolderDialog dlg;
	CFileSpec fs(FS_MYDOC);	// デフォルトフォルダ:My Document
	CString strFolder = fs.GetFullSpec();
	dlg.SetDefaultFolder((LPCTSTR)strFolder);
	// レコードに保存するためサイズ制限が必要
	
	if(dlg.DoModal())
	{
		strFolder = dlg.GetFolderPath();
		if (strFolder.GetLength() > (MAX_PATH - MAX_FILE)) {
			str.LoadString(IDS_MSGBOX121);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return;
		}

		// データフォルダ変更
		m_edtDataPath.SetWindowText(strFolder);

		UpdateData(FALSE);	// DDX更新

		/// 適用ボタンの有効化
		OnChgForm();
	}
}

/////////////////////////////////////////////////////////////////////////////
// public function

//
//  機能     : レジストリ保存前のチェック
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CSettingAdminTab::Check() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CString str;
	CString strMsg;
	BOOL blErr = FALSE;

	UpdateData(TRUE);	// DDX更新

	//// データフォルダ
	if (m_blDataPath) {
		m_edtDataPath.GetWindowText(str);
		if (str.GetLength() == 0)	blErr = TRUE;
		CFileSpec fs(str);
		if (!fs.Exist())	blErr = TRUE;
		if (!fs.IsFolder())	blErr = TRUE;
		if (blErr) {
			strMsg = _T("[");
			str.LoadString(IDS_TAB_ADMIN);
			strMsg += str + _T("]-[");
			GetDlgItemText(IDC_FRM_MSG_FOLDER, str);
			strMsg += str + _T("]");
			str.LoadString(IDS_TAB_CHECK);
			strMsg += str;
			AfxMessageBox(strMsg, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return FALSE;
		}
	}

	return TRUE;
}


//
//  機能     : レジストリへ保存
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
void CSettingAdminTab::Save() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CString str;

	UpdateData(TRUE);	// DDX更新

	/// レジストリ更新
	//// Hotkey有効/無効
	m_rgEnableHotKey = (m_blHotKey) ? 1 : 0;
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	// 一旦解除して再設定
	pFrame->UnregHotKey();
	if (m_blHotKey) {
		pFrame->RegHotKey();
	}
	//// データフォルダ変更
	m_rgEnableDataPath = (m_blDataPath) ? 1 : 0;
	m_edtDataPath.GetWindowText(str);
	m_rgDataPath = str;
	//// ログ起動時無効
	m_rgEnableLog = (m_blEnableLog) ? 1 : 0;
	theApp.m_intEnableLog = (m_blEnableLog) ? 1 : 0;
	/// SKYPE PORT
	m_rgSkypePortOut = (DWORD)_ttoi(m_strSkypePortOut);
	m_rgSkypePortMic = (DWORD)_ttoi(m_strSkypePortMic);
}

