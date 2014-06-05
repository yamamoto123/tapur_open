// SetHotkey.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "SetHotkey.h"
#include "SetHotkeyEdit.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetHotkey ダイアログ


CSetHotkey::CSetHotkey(CWnd* pParent /*=NULL*/)
	: CDialog(CSetHotkey::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetHotkey)
	//}}AFX_DATA_INIT

}

void CSetHotkey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetHotkey)
	DDX_Control(pDX, IDC_LIST_HOTKEY, m_lstTool);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetHotkey, CDialog)
	//{{AFX_MSG_MAP(CSetHotkey)
	ON_BN_CLICKED(IDC_BTN_HOTKEY_EDIT, OnBtnHotkeyEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetHotkey メッセージ ハンドラ


BOOL CSetHotkey::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CString str;

	/// リストスタイル設定
	DWORD dwStyle = m_lstTool.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_lstTool.SetExtendedStyle(dwStyle);
	/// ヘッダー定義
	str.LoadString(IDS_HOTKEYLIST_HEAD);
	m_lstTool.SetColumnHeader(str);

	/// データ設定
	// レジストリ参照
	LoadRegistry();
	// メイン画面表示
	AddRecord(IDS_HK_SHOW, (BOOL)m_rgEnableShow, (UINT)m_rgModShow, (int)m_rgKeyShow);
	// メイン画面非表示
	AddRecord(IDS_HK_HIDE, (BOOL)m_rgEnableHide, (UINT)m_rgModHide, (int)m_rgKeyHide);
	// 録音
	AddRecord(IDS_HK_RECORD, (BOOL)m_rgEnableRecord, (UINT)m_rgModRecord, (int)m_rgKeyRecord);
	// 録画
	AddRecord(IDS_HK_VRECORD, (BOOL)m_rgEnableVRecord, (UINT)m_rgModVRecord, (int)m_rgKeyVRecord);
	// 録音・録画一時停止
	AddRecord(IDS_HK_REC_PAUSE, (BOOL)m_rgEnableRecPause, (UINT)m_rgModRecPause, (int)m_rgKeyRecPause);
	// 録音・録画終了
	AddRecord(IDS_HK_REC_STOP, (BOOL)m_rgEnableRecStop, (UINT)m_rgModRecStop, (int)m_rgKeyRecStop);
	// 再生：現在リストで選択されているレコード
	AddRecord(IDS_HK_PLAY, (BOOL)m_rgEnablePlay, (UINT)m_rgModPlay, (int)m_rgKeyPlay);
	// 再生一時停止・再開
	AddRecord(IDS_HK_PLAY_PAUSE, (BOOL)m_rgEnablePlayPause, (UINT)m_rgModPlayPause, (int)m_rgKeyPlayPause);
	// 再生終了
	AddRecord(IDS_HK_PLAY_STOP, (BOOL)m_rgEnablePlayStop, (UINT)m_rgModPlayStop, (int)m_rgKeyPlayStop);
	// 次の再生：現在リストで選択されているレコードの次のレコード
	AddRecord(IDS_HK_PLAY_NEXT, (BOOL)m_rgEnablePlayNext, (UINT)m_rgModPlayNext, (int)m_rgKeyPlayNext);
	// 前の再生：現在リストで選択されているレコードの前のレコード
	AddRecord(IDS_HK_PLAY_PREV, (BOOL)m_rgEnablePlayPrev, (UINT)m_rgModPlayPrev, (int)m_rgKeyPlayPrev);

	UpdateData(FALSE);	// DDX更新

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// ショートカット変更ボタン
void CSetHotkey::OnBtnHotkeyEdit() 
{
	UpdateData(TRUE);	// DDX更新

	// 選択リスト情報
	if (m_lstTool.GetSelectedCount() < 1) return;
	POSITION pos = m_lstTool.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	int idx = m_lstTool.GetNextSelectedItem(pos);
	CString strTitle = m_lstTool.GetItemText(idx, 1);
	CString str = m_lstTool.GetItemText(idx, 2);
	UINT RESID = (UINT)m_lstTool.GetItemData(idx);
	//	キー情報
	UINT uniMod = 0;
	if (str.Find(_T("Ctrl")) >= 0) uniMod |= MOD_CONTROL;
	if (str.Find(_T("Shift")) >= 0) uniMod |= MOD_SHIFT;
	if (str.Find(_T("Alt")) >= 0) uniMod |= MOD_ALT;
	CStringA strA = (CStringA)str.Right(1);
	int intKey = (int)strA.GetAt(0);

	// 更新ダイアログ
	CSetHotkeyEdit dlg(this, strTitle, uniMod, intKey);
	if (dlg.DoModal() != IDOK) return;
	UINT uniModNew = dlg.GetMod();
	int intKeyNew = dlg.GetKey();
	if (uniMod == uniModNew && intKey == intKeyNew) return;

	// リスト更新
	UpdateRecord(idx, uniModNew, intKeyNew);

	// レジストリ更新
	// メイン画面表示
	if (RESID == IDS_HK_SHOW) m_rgModShow = uniModNew, m_rgKeyShow = intKeyNew;
	// メイン画面非表示
	if (RESID == IDS_HK_HIDE) m_rgModHide = uniModNew, m_rgKeyHide = intKeyNew;
	// 録音
	if (RESID == IDS_HK_RECORD) m_rgModRecord = uniModNew, m_rgKeyRecord = intKeyNew;
	// 録画
	if (RESID == IDS_HK_VRECORD) m_rgModVRecord = uniModNew, m_rgKeyVRecord = intKeyNew;
	// 録音・録画一時停止
	if (RESID == IDS_HK_REC_PAUSE) m_rgModRecPause = uniModNew, m_rgKeyRecPause = intKeyNew;
	// 録音・録画終了
	if (RESID == IDS_HK_REC_STOP) m_rgModRecStop = uniModNew, m_rgKeyRecStop = intKeyNew;
	// 再生：現在リストで選択されているレコード
	if (RESID == IDS_HK_PLAY) m_rgModPlay = uniModNew, m_rgKeyPlay = intKeyNew;
	// 再生一時停止・再開
	if (RESID == IDS_HK_PLAY_PAUSE) m_rgModPlayPause = uniModNew, m_rgKeyPlayPause = intKeyNew;
	// 再生終了
	if (RESID == IDS_HK_PLAY_STOP) m_rgModPlayStop = uniModNew, m_rgKeyPlayStop = intKeyNew;
	// 次の再生：現在リストで選択されているレコードの次のレコード
	if (RESID == IDS_HK_PLAY_NEXT) m_rgModPlayNext = uniModNew, m_rgKeyPlayNext = intKeyNew;
	// 前の再生：現在リストで選択されているレコードの前のレコード
	if (RESID == IDS_HK_PLAY_PREV) m_rgModPlayPrev = uniModNew, m_rgKeyPlayPrev = intKeyNew;
}

// 当面は有効/無効の設定のみ保存
// キーの設定はレジストリの直接操作のみとする
void CSetHotkey::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	UpdateData(TRUE);	// DDX更新

	/// レジストリ更新
	BOOL blChk;
	// メイン画面表示
	int idx = 0; blChk = m_lstTool.GetCheck(idx);
	m_rgEnableShow = (DWORD)blChk;
	// メイン画面非表示
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnableHide = (DWORD)blChk;
	// 録音
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnableRecord = (DWORD)blChk;
	// 録画
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnableVRecord = (DWORD)blChk;
	// 録音・録画一時停止
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnableRecPause = (DWORD)blChk;
	// 録音・録画終了
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnableRecStop = (DWORD)blChk;
	// 再生：現在リストで選択されているレコード
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnablePlay = (DWORD)blChk;
	// 再生一時停止・再開
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnablePlayPause = (DWORD)blChk;
	// 再生終了
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnablePlayStop = (DWORD)blChk;
	// 次の再生：現在リストで選択されているレコードの次のレコード
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnablePlayNext = (DWORD)blChk;
	// 前の再生：現在リストで選択されているレコードの前のレコード
	idx++; blChk = m_lstTool.GetCheck(idx);
	m_rgEnablePlayPrev = (DWORD)blChk;

	// Hotkeyの登録更新
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	pFrame->UnregHotKey();
	pFrame->RegHotKey();

	CDialog::OnOK();
}

void CSetHotkey::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。

	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// global function

// レジストリ参照
BOOL CSetHotkey::LoadRegistry() 
{
	// メイン画面表示
	m_rgEnableShow = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnableShow"), 1, TRUE);
	m_rgModShow = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModShow"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyShow = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyShow"), 'T', TRUE);
	// メイン画面非表示
	m_rgEnableHide = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnableHide"), 1, TRUE);
	m_rgModHide = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModHide"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyHide = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyHide"), 'H', TRUE);
	// 録音
	m_rgEnableRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnableRecord"), 1, TRUE);
	m_rgModRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModRecord"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyRecord"), 'R', TRUE);
	// 録画
	m_rgEnableVRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnableVRecord"), 1, TRUE);
	m_rgModVRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModVRecord"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyVRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyVRecord"), 'V', TRUE);
	// 録音・録画一時停止
	m_rgEnableRecPause = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnableRecPause"), 1, TRUE);
	m_rgModRecPause = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModRecPause"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyRecPause = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyRecPause"), 'O', TRUE);
	// 録音・録画終了
	m_rgEnableRecStop = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnableRecStop"), 1, TRUE);
	m_rgModRecStop = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModRecStop"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyRecStop = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyRecStop"), 'X', TRUE);
	// 再生：現在リストで選択されているレコード
	m_rgEnablePlay = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnablePlay"), 1, TRUE);
	m_rgModPlay = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModPlay"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyPlay = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyPlay"), 'P', TRUE);
	// 再生一時停止・再開
	m_rgEnablePlayPause = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnablePlayPause"), 1, TRUE);
	m_rgModPlayPause = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModPlayPause"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyPlayPause = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyPlayPause"), 'I', TRUE);
	// 再生終了
	m_rgEnablePlayStop = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnablePlayStop"), 1, TRUE);
	m_rgModPlayStop = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModPlayStop"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyPlayStop = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyPlayStop"), 'E', TRUE);
	// 次の再生：現在リストで選択されているレコードの次のレコード
	m_rgEnablePlayNext = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnablePlayNext"), 1, TRUE);
	m_rgModPlayNext = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModPlayNext"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyPlayNext = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyPlayNext"), 'F', TRUE);
	// 前の再生：現在リストで選択されているレコードの前のレコード
	m_rgEnablePlayPrev = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\EnablePlayPrev"), 1, TRUE);
	m_rgModPlayPrev = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\ModPlayPrev"), MOD_CONTROL | MOD_SHIFT, TRUE);
	m_rgKeyPlayPrev = CRegDWORD(_T("Software\\Tapur\\Tapur\\HotKey\\KeyPlayPrev"), 'B', TRUE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// local function

//
//  機能     : レコード追加
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CSetHotkey::AddRecord(
UINT RESID,		// リソースID
BOOL blChk,		// 有効/無効
UINT uiMod,		// Modify key
int iKey)		// Virtual Key
{
	CString str;
	CString strExp;
	int idx;

	idx = m_lstTool.InsertItem(m_lstTool.GetItemCount(), _T(""));
	m_lstTool.SetCheck(idx, blChk);
	strExp.LoadString(RESID);
	m_lstTool.SetItemData(idx, (DWORD)RESID);
	m_lstTool.SetItemText(idx, 1, strExp);
	str.Empty();
	if (uiMod & MOD_CONTROL) {
		if (str.GetLength() > 0)	str += _T("+");
		str += _T("Ctrl");
	}
	if (uiMod & MOD_SHIFT) {
		if (str.GetLength() > 0)	str += _T("+");
		str += _T("Shift");
	}
	if (uiMod & MOD_ALT) {
		if (str.GetLength() > 0)	str += _T("+");
		str += _T("Alt");
	}

	if (str.GetLength() > 0)	str += _T("+");
	str += (TCHAR)iKey;
	m_lstTool.SetItemText(idx, 2, str);

	return TRUE;  
}

//
//  機能     : レコード更新
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CSetHotkey::UpdateRecord(
int idx,		// List index
UINT uiMod,		// Modify key
int iKey)		// Virtual Key
{
	CString str;

	str.Empty();
	if (uiMod & MOD_CONTROL) {
		if (str.GetLength() > 0)	str += _T("+");
		str += _T("Ctrl");
	}
	if (uiMod & MOD_SHIFT) {
		if (str.GetLength() > 0)	str += _T("+");
		str += _T("Shift");
	}
	if (uiMod & MOD_ALT) {
		if (str.GetLength() > 0)	str += _T("+");
		str += _T("Alt");
	}

	if (str.GetLength() > 0)	str += _T("+");
	str += (TCHAR)iKey;
	m_lstTool.SetItemText(idx, 2, str);

	return TRUE;  
}
