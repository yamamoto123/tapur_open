// Tapur.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include <locale.h>
//#ifdef _DEBUG
//#include "vld.h"
//#endif
#include "Tapur.h"
#include "MainFrm.h"
#include "TapurDoc.h"
#include "TapurView.h"
#include <initguid.h>
#include "Tapur_i.c"
#include "Tokenizer.h"
#include "FileSpec.h"
#include "TPsetup.h"
#include "TMsgBox.h"
#include "BackupDialog.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CTapurApp

BEGIN_MESSAGE_MAP(CTapurApp, CWinApp)
	//{{AFX_MSG_MAP(CTapurApp)
	ON_COMMAND(ID_APP_HELP, OnAppHelp)
	ON_COMMAND(ID_LINK_LAME, OnLinkLame)
	ON_COMMAND(ID_LINK_LICENSE, OnLinkLicense)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_TOOL_SETTING, OnToolSetting)
	ON_COMMAND(ID_TOOL_BACKUP, OnToolBackup)
	ON_COMMAND(ID_WAV_FOLDER, OnWavFolder)
	ON_COMMAND(ID_MP3_FOLDER, OnMp3Folder)
	ON_COMMAND(ID_VIDEO_FOLDER, OnVideoFolder)
	ON_COMMAND(ID_REPAIR, OnRepair)
	ON_COMMAND(ID_SOUND_RECORDER, OnSoundRecoder)
	ON_COMMAND(ID_AUDACITY, OnAudacity)
// 以下の設定はMainFrmで処理される（IDが一致するため）
/*
	ON_COMMAND(ID_VRECORD_START, OnToolVRecord)
	ON_COMMAND(ID_RECORD_START, OnToolRecord)
	ON_COMMAND(ID_RECORD_PAUSE, OnToolRecPause)
	ON_COMMAND(ID_RECORD_STOP, OnToolRecStop)
	ON_COMMAND(ID_RECORD_MOD, OnToolRecMod)
*/
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTapurApp クラスの構築

CTapurApp::CTapurApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。

#ifdef _DEBUG
//   _CrtSetBreakAlloc(171);	// memory leak 検出
#endif

	// TRACEマクロにマルチバイト文字列を有効にする（VSのバグ対応）
	(void)::_tsetlocale( LC_ALL, _T("japanese") );

	m_intEnableLog = -1;
	m_lngRdflId = -1;
	m_lngRsflId = -1;
	m_strUsrId.Empty();
	m_strUsrName.Empty();
	m_strDataFolder.Empty();
	m_strDBPath.Empty();

	m_blSkypeConnect = FALSE;
}

CTapurApp::~CTapurApp()
{
#ifdef _DEBUG
//	_CrtDumpMemoryLeaks();	// memory leak 検出
#endif
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CTapurApp オブジェクト

CTapurApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTapurApp クラスの初期化

BOOL CTapurApp::InitInstance()
{
	// memory leak 検出
#ifdef _DEBUG
	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	if (!InitATL())
		return FALSE;

	AfxEnableControlContainer();

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	// したければ以下の特定の初期化ルーチンの中から不必要なもの
	// を削除してください。

	/// 二重起動の防止
	// ミューテックスオブジェクトを作る。ミューテックスオブジェクトが
	// すでに存在している場合は、アプリケーションの2つ目のインスタンス
	// である。ミューテックスハンドルは、プロセスが終了すると自動的に
	// 閉じられることに注意
	::CreateMutex(NULL, TRUE, m_pszExeName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// 前のアプリケーションのメイン画面を表示
		HWND hwnd = ::FindWindow(_T(MAIN_CLASS_NAME), NULL); 
		if (hwnd) {
			::SetParent(hwnd,NULL);
			::ShowWindow(hwnd, SW_SHOWNORMAL);
			::SetWindowLong(hwnd,GWL_EXSTYLE,WS_VISIBLE);
			::RedrawWindow(hwnd, NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_FRAME |
                                   RDW_INVALIDATE | RDW_ERASE);
			::SetActiveWindow(hwnd);
			::SetForegroundWindow(hwnd);
		}
		// (注意)タスクトレイからの既存APPの画面表示がうまくいかない→エラーメッセージで回避
		//AfxMessageBox(_T("Tapur is already running!"), MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		//TRACE(_T("Could not find previous instance main window!\n"));
		return FALSE;
	}

/* VC7では不要？
#ifdef _AFXDLL
	Enable3dControls();		// 共有 DLL の中で MFC を使用する場合にはここを呼び出してください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクしている場合にはここを呼び出してください。
#endif
*/
	// Socketの初期化
	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("SOCKETS_INIT_FAILED"));
		return FALSE;
	}

	// 言語リソースDLLのロード
	_TCHAR strDll[MAX_PATH];	
	CFileSpec fs(FS_APPDIR);	// application folder
	_stprintf_s(strDll, MAX_PATH, _T("%slang.dll"), (LPCTSTR)(fs.GetFolder()));
	m_hResDLL = LoadLibrary(strDll);
	if (!m_hResDLL) {
		AfxMessageBox(_T("Can't load resource DLL."));
		return FALSE;
	}
	// リソースの参照先を指定
	AfxSetResourceHandle(m_hResDLL);

	// 設定が保存される下のレジストリ キーを変更します。
	// TODO: この文字列を、会社名または所属など適切なものに
	// 変更してください。
	SetRegistryKey(_T("Tapur"));

	LoadStdProfileSettings();  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)

	/// Windowsオーディオデバイスのチェック
	if (!InitWinDev()) return FALSE;

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTapurDoc),
		RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS(CTapurView));
	if (pDocTemplate == NULL)	return FALSE;
	AddDocTemplate(pDocTemplate);
	if (GetFirstDocTemplatePosition() == NULL)	return FALSE;

	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		return TRUE;
	}
	// コマンドラインでディスパッチ コマンドを指定します。
	if (!ProcessShellCommand(cmdInfo))		// MainFrameの生成もここで行われる
		return FALSE;

	// メイン ウィンドウが初期化されたので表示と更新を行います。
	// 初期状態ではウィンドウは閉じる
	m_pMainWnd->SetWindowText(m_pszExeName);
	m_pMainWnd->ShowWindow(SW_HIDE);
//	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// タグ とウィンドウを関連付け、後で探せるようにする
	//::SetProp(m_pMainWnd->GetSafeHwnd(), m_pszExeName, (HANDLE)1);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard 仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush m_brush;		// 背景色のブラシ
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT

	// 背景色ブラシ初期化
	m_brush.CreateSolidBrush(RGB(255,255,255));		// white
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: この位置で DC のアトリビュートを変更してください

	// フォーム背景色を白に変更
	if (nCtlColor == CTLCOLOR_DLG) {
		pDC->SetBkColor(RGB(255,255,255));		// 背景を白に
		return (HBRUSH)m_brush.GetSafeHandle();
	}
	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);//背景透過
		int id = pWnd->GetDlgCtrlID();
		if (id == IDC_STATIC_COPY || 
			id == IDC_STATIC_COPY2 ||
			id == IDC_STATIC_COPY3 ||
			id == IDC_STATIC_COPY4)
		{
			pDC->SetTextColor(RGB(153,153,153));// テキスト色灰色設定
		}
		return (HBRUSH)m_brush.GetSafeHandle();
	}
	
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTapurApp メッセージ ハンドラ

// オンラインマニュアル表示
void CTapurApp::OnAppHelp()
{
	CString str;
	str.LoadString(IDS_URL_HELP);
	OpenUrl(str);
}

// LAME ダウンロードサイト
void CTapurApp::OnLinkLame()
{
	CString str;
	str.LoadString(IDS_URL_LAME);
	OpenUrl(str);
}

// FFDSHOW ダウンロードサイト
//void CTapurApp::OnLinkFFDSHOW()
//{
//	CString str;
//	str.LoadString(IDS_URL_FFDSHOW);
//	OpenUrl(str);
//}

// SkypeIn/Out購入ページ表示
//void CTapurApp::OnAppSkype()
//{
//	CString str;
//	str.LoadString(IDS_URL_SKYPE);
//	OpenUrl(str);
//}

// 寄付募集メニュー表示
//void CTapurApp::OnDonation()
//{
//	CString str;
//	str.LoadString(IDS_URL_DONATION);
//	OpenUrl(str);
//}

// ライセンス購入サイト
void CTapurApp::OnLinkLicense()
{
	CString str;
	str.LoadString(IDS_URL_LICENSE);
	OpenUrl(str);
}

// バージョン情報表示
void CTapurApp::OnAppAbout()
{
	static CAboutDlg m_aboutDlg;
	//aboutDlg.DoModal();

	if (m_aboutDlg.m_hWnd == NULL) {
		m_aboutDlg.Create(IDD_ABOUTBOX, this->m_pMainWnd);
	} else {
		if (m_aboutDlg.IsWindowVisible()) {
			m_aboutDlg.SetFocus();
			return;
		}
	}

	m_aboutDlg.ShowWindow(SW_SHOW);
}

// 設定画面表示
void CTapurApp::OnToolSetting()
{
	static CSettingDialog m_setDlg;	
	//setDlg.DoModal();

	if (m_setDlg.m_hWnd == NULL) {
		m_setDlg.Create(IDD_SETTING, this->m_pMainWnd);
	} else {
		//if (m_setDlg.IsWindowVisible()) {
		//	m_setDlg.SetFocus();
		//	return;
		//}
		m_setDlg.DestroyWindow();
		m_setDlg.Create(IDD_SETTING, this->m_pMainWnd);
	}

	m_setDlg.ShowWindow(SW_SHOW);
}

// PC移行ツール表示
void CTapurApp::OnToolBackup()
{
	static CBackupDialog m_backupDlg;	

	if (m_backupDlg.m_hWnd == NULL) {
		m_backupDlg.Create(IDD_DATA_BACKUP, this->m_pMainWnd);
	} else {
		if (m_backupDlg.IsWindowVisible()) {
			m_backupDlg.SetFocus();
			return;
		}
	}

	m_backupDlg.ShowWindow(SW_SHOW);
}

// WAV録音データが保存されているフォルダを開きます
void CTapurApp::OnWavFolder()
{	 
	CTPsetup tps(theApp.m_strDataFolder);
	CString str = tps.RdfWavFolder();
	HINSTANCE ret = ShellExecute(NULL, NULL, str,  NULL, NULL, SW_SHOWNORMAL);
}

// MP3録音データが保存されているフォルダを開きます
void CTapurApp::OnMp3Folder()
{	 
	CTPsetup tps(theApp.m_strDataFolder);
	CString str = tps.RdfMp3Folder();
	HINSTANCE ret = ShellExecute(NULL, NULL, str,  NULL, NULL, SW_SHOWNORMAL);
}

// 録画データが保存されているフォルダを開きます
void CTapurApp::OnVideoFolder()
{
	CTPsetup tps(theApp.m_strDataFolder);
	CString str = tps.RdfVideoFolder();
	HINSTANCE ret = ShellExecute(NULL, NULL, str,  NULL, NULL, SW_SHOWNORMAL);
}

// 障害時に残った一時データからデータを復旧する
void CTapurApp::OnRepair()
{
	int i = 0;
	CString str;

	/// 確認ダイアログ
	CWordArray aryWd;
	aryWd.RemoveAll();
	aryWd.Add(24);	// 1day
	aryWd.Add(48);	// 2days
	aryWd.Add(168);	// 1week
	aryWd.Add(336);	// 2weeks
	aryWd.Add(720);	// 30days

	CString strTitle;
	CString strMsg;
	CString strData;
	CStringArray aryName;
	strTitle.LoadString(IDS_REPAIR_TITLE);
	strMsg.LoadString(IDS_REPAIR_MSG);
	strData.LoadString(IDS_REPAIR_DATA);
	CTokenizer tok(strData, _T("|"));
	CString cs;
	aryName.RemoveAll();
	while(tok.Next(cs)) {
		aryName.Add(cs);
	}
	str.LoadString(IDS_RSF_ARG_AUDIOFILE);
	CTMsgBox msgbox(NULL, strTitle, strMsg, TRUE, aryName);
	msgbox.Show(TRUE);
	if (!msgbox.GetResult()) return;
	//// 選択結果
	long lngTime = (long)aryWd.GetAt(0);
	CString strSel;
	strSel = msgbox.GetCombo();
	if (strSel.GetLength() <= 0) return;
	for(i = 0; i < aryName.GetSize(); i++) {
		str = aryName.GetAt(i);
		if (str == strSel) {
			lngTime = (long)aryWd.GetAt(i);
			break;
		}
	}

	/// 未登録の一時データの修復
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	long lngCnt = pFrame->RepairTmpData(lngTime);
	if (lngCnt > 0) {
		strMsg.LoadString(IDS_MSGBOX111);
		str.Format(_T("%d%s"), lngCnt, strMsg);
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
	} else {
		str.LoadString(IDS_MSGBOX112);
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
	}
}

// Windows標準のサウンドレコーダを起動
void CTapurApp::OnSoundRecoder()
{
	CString str;

	HINSTANCE ret = ShellExecute(NULL, NULL, _T("sndrec32.exe"),  NULL, NULL, SW_SHOWNORMAL);
	if ((int)ret <= 32) {
		// サウンドレコーダが使えない場合（Vista）、Audacityを推奨する
		str.LoadString(IDS_MSGBOX115);
		if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONINFORMATION ) == IDYES) {
			OnAudacity();
		}
	}
}

// Audacityを起動
void CTapurApp::OnAudacity()
{
	CString str;
	CFileFind finder;
	CFileSpec fs(FS_PROGRAM_FILES);

	SetCurrentDirectory(fs.GetFullSpec());
	BOOL bWorking = finder.FindFile(_T("Audacity*"));
	if (bWorking) {
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDirectory()) {
				str = finder.GetFileName();
				bWorking = TRUE;
				break;
			}
		}
	}
	finder.Close();

	if (bWorking) {
		str = fs.GetFullSpec() + _T("\\") + str + _T("\\audacity.exe");
		HINSTANCE ret = ShellExecute(NULL, NULL, str,  NULL, NULL, SW_SHOWNORMAL);
		if ((int)ret <= 32) bWorking = FALSE;
	}

	if (!bWorking) {
		// Audacityが使えない場合、ダウンロードリンクを表示
		str.LoadString(IDS_URL_AUDACITY);
		OpenUrl(str);
	}
}

BOOL CTapurApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
/*
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	if (pFrame) {
		if(pFrame->m_Framework.ProcessMessage(pMsg))
			return TRUE;
	}
*/

	return CWinApp::PreTranslateMessage(pMsg);
}

	
CTapurModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

LONG CTapurModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CTapurModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR CTapurModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}


int CTapurApp::ExitInstance()
{
	if (m_bATLInited)
	{
		_Module.RevokeClassObjects();
		_Module.Term();
		CoUninitialize();
	}

	// リソースDLLを解除
	if (m_hResDLL) {
		FreeLibrary(m_hResDLL);
	}

	return CWinApp::ExitInstance();

}

BOOL CTapurApp::InitATL()
{
	m_bATLInited = TRUE;

#if _WIN32_WINNT >= 0x0400
// （注意！）MFCのCOMはSTAでしか起動できない？
	HRESULT hRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
//	HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	HRESULT hRes = CoInitialize(NULL);
#endif

	if (FAILED(hRes))
	{
		m_bATLInited = FALSE;
		return FALSE;
	}

	_Module.Init(ObjectMap, AfxGetInstanceHandle());
	_Module.dwThreadID = GetCurrentThreadId();

	LPTSTR lpCmdLine = GetCommandLine(); // この行は _ATL_MIN_CRT で必要です
	TCHAR szTokens[] = _T("-/");

	BOOL bRun = TRUE;
	LPCTSTR lpszToken = _Module.FindOneOf(lpCmdLine, szTokens);
	while (lpszToken != NULL)
	{
		if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_TAPUR, FALSE);
			_Module.UnregisterServer(TRUE); // TRUE は typelib が登録されないことを示します
			bRun = FALSE;
			break;
		}
		if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_TAPUR, TRUE);
			_Module.RegisterServer(TRUE);
			bRun = FALSE;
			break;
		}
		lpszToken = _Module.FindOneOf(lpszToken, szTokens);
	}

	if (!bRun)
	{
		m_bATLInited = FALSE;
		_Module.Term();
		CoUninitialize();
		return FALSE;
	}

	hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
		REGCLS_MULTIPLEUSE);
	if (FAILED(hRes))
	{
		m_bATLInited = FALSE;
		CoUninitialize();
		return FALSE;
	}	

	return TRUE;

}

//
//  機能     : Windowsオーディオデバイスのチェック
//  
//  機能説明 : 標準デバイス名は特定できないので、ユーザに警告するのみ
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTapurApp::InitWinDev()
{
	CString str;
	CMixer mx;

	/// レジストリ読み込み
	CRegString rgDevIn = CRegString(_T("Software\\Microsoft\\Multimedia\\Sound Mapper\\Record"), _T(""));
	CRegString rgDevOut = CRegString(_T("Software\\Microsoft\\Multimedia\\Sound Mapper\\Playback"), _T(""));
	CString strDevIn = rgDevIn;
	CString strDevOut = rgDevOut;

	/// 標準デバイスチェック
	strDevIn.TrimLeft();
	strDevIn.TrimRight();
	strDevOut.TrimLeft();
	strDevOut.TrimRight();
	if (strDevIn.IsEmpty() || strDevOut.IsEmpty()) {
		//// 標準デバイスが規定されていない場合
		//// ミキサーからWAVE_MAPPERデバイス名のチェック
		strDevOut = mx.GetDeviceName();
		strDevOut.MakeUpper();
		if (strDevOut.Find(_T("TAPUR")) >= 0) {
			goto InitWinDev_ERR;
		}
	} else {
		//// 標準デバイスが規定されている場合
		strDevIn.MakeUpper();
		strDevOut.MakeUpper();
		if (strDevIn.Find(_T("TAPUR")) >= 0 || strDevOut.Find(_T("TAPUR")) >= 0) {
			goto InitWinDev_ERR;
		}
	}

	return TRUE;

InitWinDev_ERR:
	str.LoadString(IDS_MSGBOX6);
	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
	return FALSE;
}

//
//  機能     : URLのブラウザ表示
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CTapurApp::OpenUrl(CString strUrl)
{
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.fMask = SEE_MASK_INVOKEIDLIST;
	sei.lpVerb = _T("open");
	sei.lpFile = (LPCTSTR)strUrl;
	sei.nShow = SW_SHOWNORMAL;
	ShellExecuteEx(&sei);

	return TRUE;
}
