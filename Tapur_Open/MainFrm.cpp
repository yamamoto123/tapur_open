// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Tapur.h"
#include "SkypeEvents.h"
#include "MainFrm.h"
#include "MsgTree.h"
#include "RsfTree.h"
#include "MsgList.h"
#include "RsfList.h"
#include "FolderForm.h"
#include "MsgForm.h"
#include "RsfSimpleForm.h"
#include "SetHotkey.h"
#include "TPtable.h"
#include "TPutil.h"
#include "FileSpec.h"
#include "ScreenCap.h"
#include "BalloonHelp.h"
// UNICODE では動作しない機能
#ifdef _UNICODE
#undef _UNICODE
#undef UNICODE
#include "KillProcess.h"
//#include ".\mainfrm.h"
#define _UNICODE
#define UNICODE
#endif
#define SKYPE_NAME	"Skype.exe"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

// HotKey識別コード
static int ID_HK_SHOW = WM_USER + 411;			// メイン画面表示
static int ID_HK_HIDE = WM_USER + 412;			// メイン画面非表示
static int ID_HK_RECORD = WM_USER + 413;		// 録音
static int ID_HK_VRECORD = WM_USER + 414;		// 録画
static int ID_HK_REC_PAUSE = WM_USER + 415;		// 録音・録画一時停止
static int ID_HK_REC_STOP = WM_USER + 416;		// 録画終了
static int ID_HK_PLAY = WM_USER + 417;			// 再生：現在リストで選択されているレコード
static int ID_HK_PLAY_PAUSE = WM_USER + 418;	// 再生一時停止・再開
static int ID_HK_PLAY_STOP = WM_USER + 419;		// 再生終了
static int ID_HK_PLAY_NEXT = WM_USER + 420;		// 次の再生：現在リストで選択されているレコードの次のレコード
static int ID_HK_PLAY_PREV = WM_USER + 421;		// 前の再生：現在リストで選択されているレコードの前のレコード
// ショートカット識別キーコード
static int ID_SK_EXPORT = 'X';		// 録音ファイルエクスポート画面表示
static int ID_SK_VEXPORT = 'Y';		// 録画ファイルエクスポート画面表示


// タスクトレイのアイコンに送られるメッセージ
#define	WM_ICON_NOTIFY	(WM_APP + 10)

// ウィンドウ初期サイズ
#define INIT_WINSZX		600
#define INIT_WINSZY		600
// ウィンドウ最小サイズ
#define MIN_WINSZX		400
#define MIN_WINSZY		400

// ウィンドウサイズレジストリ設定
// Section name
#define SEC_SETTING "Settings"
// Entry name
#define ENT_WINPOS "WinPos"


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_POPUP_OPEN, OnPopupOpen)
	ON_COMMAND(ID_POPUP_STOP, OnPopupStop)
	ON_COMMAND(ID_POPUP_ENABLE, OnPopupEnable)
	ON_COMMAND(ID_POPUP_AUTO_ENABLE, OnPopupAutoEnable)
	ON_COMMAND(ID_POPUP_HOTKEY, OnPopupHotkey)
	ON_COMMAND(ID_APP_HELP, OnPopupHelp)
	ON_COMMAND(ID_APP_SKYPE, OnPopupSkype)
	ON_COMMAND(ID_POPUP_CLOSE, OnPopupClose)
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_POPUP_STOP, OnUpdatePopupStop)
	ON_UPDATE_COMMAND_UI(ID_POPUP_ENABLE, OnUpdatePopupEnable)
	ON_UPDATE_COMMAND_UI(ID_POPUP_AUTO_ENABLE, OnUpdatePopupAutoEnable)
	ON_WM_TIMER()
	ON_WM_SIZING()
	ON_BN_CLICKED(ID_VRECORD_START, OnVRecordStart)
	ON_BN_CLICKED(ID_RECORD_START, OnRecordStart)
	ON_BN_CLICKED(ID_RECORD_PAUSE, OnRecordPause)
	ON_BN_CLICKED(ID_RECORD_STOP, OnRecordStop)
	ON_BN_CLICKED(ID_RECORD_MOD, OnRecordMod)
	ON_COMMAND(ID_RECMOD_BOTH, OnRecordModBoth)
	ON_COMMAND(ID_RECMOD_RCV, OnRecordModRcv)
	ON_COMMAND(ID_RECMOD_SND, OnRecordModSnd)
    ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_MESSAGE(WM_SCREENCAP_NOTIFY, OnGraphNotify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_SEPARATOR,           // 追加
	ID_SEPARATOR,           // 追加
	//ID_INDICATOR_KANA,
	//ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

CMainFrame::CMainFrame()
{
	// TODO: この位置にメンバの初期化処理コードを追加してください。

	/// メニュー状態を可変とする
	m_bAutoMenuEnable = FALSE;

	m_flgEnable = 0;		// 応答処理フラグ初期値
	m_flgAutoEnable = 0;	// 自動録音処理フラグ初期値
	m_lngNewMsg = 0;		// 新着メッセージ管理変数

	/// タイマー初期化
	m_stApp.blOnTimer = FALSE;
	m_stApp.lngPassTime = 0;
	m_stConnect.blOnTimer = FALSE;
	m_stConnect.lngPassTime = 0;
	m_stAnswerWait.blOnTimer = FALSE;
	m_stAnswerWait.lngPassTime = 0;
	m_stRecordMax.blOnTimer = FALSE;
	m_stRecordMax.lngPassTime = 0;
	m_stPlayChk.blOnTimer = FALSE;
	m_stPlayChk.lngPassTime = 0;
	m_stPlayChk.lngLimitTime = 0;
}

CMainFrame::~CMainFrame()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/// ToolBarの生成
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAIN_TOOL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // 作成に失敗
	}
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // 作成に失敗
	}

	// ツールバー上のCStatic生成
    int index = 0;  
    while (m_wndToolBar.GetItemID(index) != ID_PLACEHOLDER) index++;     
    if (!m_wndToolBar.CreateStatic(m_wndToolBar.m_wndStatic, _T("00:00"), index, ID_PLACEHOLDER, 80))
    {
        TRACE0("Failed to create text on toolbar\n");
        return -1;
    }
    m_wndToolBar.m_wndStatic.ShowWindow(SW_SHOW);

	/// StatusBarの生成
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // 作成に失敗
	}
	//// エラーメッセージPaneのスタイル変更（ボーダーを有効←無効style(SBPS_NOBORDERS)を外す）
	//m_wndStatusBar.SetPaneStyle(0, SBPS_STRETCH);
    m_wndStatusBar.SetPaneInfo(0,0,SBPS_STRETCH,0);	// 1番目区画のペインを伸張する場合
    m_wndStatusBar.SetPaneInfo(1,0,SBPS_NORMAL,60);	// 2番目の区画に枠を表示、サイズを指定する場合
    m_wndStatusBar.SetPaneInfo(2,0,SBPS_NORMAL,10);	// 3番目の区画に枠を表示、サイズを指定する場合

	/// [↓]付ツールバーの許可
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	/// 「録音対象ツール」に[↓]付与
	DWORD dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_RECORD_MOD));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_RECORD_MOD), dwStyle);

	// TODO: もしツール チップスが必要ない場合、ここを削除してください。
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	/// 音声制御インスタンスの生成
	m_pAudio = CAudio::Instance();
	/// ビデオ操作クラスのインスタンス生成
	m_pVideo = CVideo::Instance();

	/// IAccessインターフェースオブジェクトの作成
	if (!InitSkypeAccess()) {
		TRACE0("Failed to create Skype interface.\n");
		return -1;      // 作成に失敗
		//PostMessage(WM_CLOSE, 0, 0L);
		//return 0;
	}

	/// Skype通信ポートのチェック
	// （FWのメッセージを表示させ、ユーザの確認を促す）
	CRegDWORD rgSkypePortOut = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortOut"), 42805, TRUE);
	CRegDWORD rgSkypePortMic = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortMic"), 42806, TRUE);
	UINT uiPortOut = (UINT)rgSkypePortOut;
	UINT uiPortMic = (UINT)rgSkypePortMic;
	CSocketIn oSock;
	if (oSock.Open(uiPortOut, uiPortMic) ) {
		oSock.Close();
	}

	/// Skypeに接続
	//// 初期値としてダミーユーザ定義
	m_pAccessEvents->InitDummyUser();
	if (!ConnectSkype()) {
		//// 接続できない場合
		///// スカイプ接続待ちタイマを起動
		StartConnectTimer();
	}

	///　システムで使うアイコンをまとめてロード
	m_hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_MAIN)); 
	m_hIconNew = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_NEW)); 
	m_hIconDis = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_DISCON));

	///　アイコンをタスクトレイに表示
	CString str;
 	str.LoadString(IDS_TOOLTIP);
	if (!m_TrayIcon.Create(
                        this,               // Let icon deal with its own messages
                        WM_ICON_NOTIFY,     // Icon notify message to use
                        str,				// tooltip
                        m_hIcon,			// ID of tray icon
                        IDR_POPUP_MAIN))    // ID of popup menu
    {
		return -1;
    }
	//// デフォルトメニューの設定
    m_TrayIcon.SetMenuDefaultItem(ID_POPUP_OPEN, FALSE);
	//// アニメーションアイコンの設定(resource.hのID番号は連続している必要がある)
    m_TrayIcon.SetIconList(IDI_RCV1, IDI_RCV2); 

	/// ツールバーの初期設定
	m_wndToolBar.Init(this);

	/// APP動作確認タイマ開始
	StartAppTimer();		

	return 0;
}

void CMainFrame::OnDestroy() 
{
	// ウィンドウから「最初のインスタンスタグ」を削除する
	//::RemoveProp(GetSafeHwnd(), AfxGetApp()->m_pszExeName);

	CFrameWnd::OnDestroy();

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	/// APP動作確認タイマ終了
	StopAppTimer();		

	// HotKeyの解除
	UnregHotKey();

	/*
	// KEY HOOK処理のUninstall
	UninstallKeyHook(); 
	*/

	// タスクトレイのアイコンを削除します。
	::Shell_NotifyIcon( NIM_DELETE, &m_stNtfyIcon );

	// Skype API のイベント解除
	TRY {
		if (theApp.m_pAccess) {
			if (m_pAccessEvents) {
				// AtlUnadviseで解放されるので、Releaseは不要
				AtlUnadvise(theApp.m_pAccess, DIID__IAccessEvents, m_dwCookie);
				m_pAccessEvents = NULL;
			}
			theApp.m_pAccess.Release();
			theApp.m_pAccess = NULL;
		}
	}
    CATCH_ALL(e)
    {
		// Nothing
	}
	END_CATCH_ALL

	/// TMPフォルダのクリア
	ClearTmpData();

	// VisualFX Framework の破棄
	m_Framework.Destroy();

	/// 音声操作クラスのインスタンス破棄
	CAudio::Instance_Delete();
	/// ビデオ操作クラスのインスタンス破棄
	CVideo::Instance_Delete();
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	CString str;

	/// ウィンドウサイズの参照
    WINDOWPLACEMENT *lwp;
    UINT nl;
	long szx = INIT_WINSZX;
	long szy = INIT_WINSZY;
    if(AfxGetApp()->GetProfileBinary(_T(SEC_SETTING), _T(ENT_WINPOS), (LPBYTE*)&lwp, &nl)) {
		szx = abs(lwp->rcNormalPosition.right - lwp->rcNormalPosition.left);
		szy = abs(lwp->rcNormalPosition.top - lwp->rcNormalPosition.bottom);
		delete [] lwp;	// 必須（内部でnewされるため）
	} 

	/// フレームサイズの参照
	long szMsgTreeX = 150;
	long szRsfTreeX = 150;
	long szMsgListY = szy * 1/3;
	long szRsfListY = szy * 1/4;
	CRegRect rgRsfRect(_T("Software\\Tapur\\Tapur\\Settings\\RsfFrmPos"), CRect(0,0,0,0), TRUE);
	CRect rct = rgRsfRect;
	if (rct.left > 30 && rct.left < szx - 30)	szRsfTreeX = rct.left;
	if (rct.top > 30 && rct.top < szy - 30)	szRsfListY = rct.top;
	CRegRect rgMsgRect(_T("Software\\Tapur\\Tapur\\Settings\\MsgFrmPos"), CRect(0,0,0,0), TRUE);
	rct = rgMsgRect;
	if (rct.left > 30 && rct.left < szx - 30)	szMsgTreeX = rct.left;
	if (rct.top > 30 && rct.top < szy - 30)	szMsgListY = rct.top;

	// 各フレーム画面の生成
	m_pMainTab = new TVisualObject(1,_T("MainTab"),pContext,RUNTIME_CLASS(TTabWnd),TVisualObject::TOS_TABBOTTOM);
	/// 機能TAB
	str.LoadString(IDS_PANE_MSG);
	m_pTabInbox = new TVisualObject(2,str,1,2,pContext);
	str.LoadString(IDS_PANE_SETUP);
	m_pTabSetup = new TVisualObject(3,str,1,2,pContext);
	/// Left:Tree表示Pane
	m_pMsgTree = new TVisualObject(4,0,0,pContext, RUNTIME_CLASS(CMsgTree),CSize(szMsgTreeX,0));
	m_pRsfTree = new TVisualObject(5,0,0,pContext, RUNTIME_CLASS(CRsfTree),CSize(szRsfTreeX,0));
	/// Right:Nest
	m_pNestInbox = new TVisualObject(6,0,1,2,1,pContext);
	m_pNestSetup = new TVisualObject(7,0,1,2,1,pContext);
	/// Right:リスト表示
	m_pMsgList = new TVisualObject(8,0,0,pContext, RUNTIME_CLASS(CMsgList), CSize(0,szMsgListY));
	m_pRsfList = new TVisualObject(9,0,0,pContext, RUNTIME_CLASS(CRsfList), CSize(0,szRsfListY));
	/// Right:リスト詳細（メッセージ、RSF設定、等）
	m_pTabMsg = new TVisualObject(10,1,0,pContext,RUNTIME_CLASS(TTabWnd),CSize(0,0),TVisualObject::TOS_TABTOP);
	m_pTabRsf = new TVisualObject(11,1,0,pContext,RUNTIME_CLASS(TTabWnd),CSize(0,0),TVisualObject::TOS_TABTOP);
	m_pMsgForm = new TVisualObject(12,_T(" "),pContext, RUNTIME_CLASS(CMsgForm));
	m_pRsfSimpleForm = new TVisualObject(13,_T(" "),pContext, RUNTIME_CLASS(CRsfSimpleForm));
	m_pMsgFolderForm = new TVisualObject(14,_T(" "),pContext, RUNTIME_CLASS(CFolderForm));
	m_pRsfFolderForm = new TVisualObject(15,_T(" "),pContext, RUNTIME_CLASS(CFolderForm));

	m_Framework.Add(m_pMainTab);
	// Inbox Tab
	m_Framework.Add(m_pMainTab, m_pTabInbox);
	m_Framework.Add(m_pTabInbox, m_pMsgTree);
	m_Framework.Add(m_pTabInbox, m_pNestInbox);
	m_Framework.Add(m_pNestInbox, m_pMsgList);
	m_Framework.Add(m_pNestInbox, m_pTabMsg);
	m_Framework.Add(m_pTabMsg, m_pMsgFolderForm);
	m_Framework.Add(m_pTabMsg, m_pMsgForm);
	// Setup Tab
	m_Framework.Add(m_pMainTab, m_pTabSetup);
	m_Framework.Add(m_pTabSetup, m_pRsfTree);
	m_Framework.Add(m_pTabSetup, m_pNestSetup);
	m_Framework.Add(m_pNestSetup, m_pRsfList);
	m_Framework.Add(m_pNestSetup, m_pTabRsf);
	m_Framework.Add(m_pTabRsf, m_pRsfFolderForm);
	m_Framework.Add(m_pTabRsf, m_pRsfSimpleForm);

	m_Framework.Create(this);

	TVisualFrameworkIterator it(m_Framework);
	while (!it.End()) {
		TVisualObject *pObject = it.Get();
		it++;
	}

	// HotKeyの設定
	RegHotKey();

	return TRUE;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if(m_Framework.ProcessMessage(pMsg))
		return TRUE;


	// ローカルショートカットキー処理
    if (pMsg->message == WM_KEYDOWN) {
		if (IsWindowVisible() && GetKeyState(VK_CONTROL) < 0) {
			CMsgForm *pMsgForm = (CMsgForm *)m_pMsgForm->GetWnd();
			int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
			if(nVirtKey == (int)ID_SK_EXPORT) {
				// 録音ファイルエクスポート画面表示
				pMsgForm->ExportFile();
			} else if(nVirtKey == (int)ID_SK_VEXPORT) {
				// 録画ファイルエクスポート画面表示
				pMsgForm->ExportVFile();
			}
		}
	}

	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
    if(bShow) {
		if(!IsWindowVisible()) {
			/// レジストリからWINDOW位置取得
			WINDOWPLACEMENT *lwp;
			UINT nl;
			if(AfxGetApp()->GetProfileBinary(_T(SEC_SETTING), _T(ENT_WINPOS), (LPBYTE*)&lwp, &nl))
			{
				SetWindowPlacement(lwp);
				delete [] lwp;
			} else {
				/// レジストリに存在しない場合、初期値を設定
				WINDOWPLACEMENT wp;
				GetWindowPlacement(&wp);
				wp.rcNormalPosition.right = wp.rcNormalPosition.left + INIT_WINSZX;
				wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + INIT_WINSZY;
				SetWindowPlacement(&wp);
			}
		} else {
			/// 最初の表示の時のみ、画面の更新
			CString str;
			this->GetWindowText(str);
			if (str.GetLength() > 0 && str.GetLength() < 8) {
				UpdateForm();
			}
		}
	}
}

// メイン画面オープン
void CMainFrame::OnPopupOpen() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString str;

	/// スカイプユーザチェック（ダミーもない場合）
	if (theApp.m_strUsrId.IsEmpty()) {
		str.LoadString(IDS_MSGBOX3);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return;
	}

	/// WINDOWオープン
    CSystemTray::MaximiseFromTray(this);

	/// 画面の更新
	UpdateForm();
}

// 留守番応答の中止メニュー
void CMainFrame::OnPopupStop() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString str;

	/// 留守番応答中
	if (theApp.m_lngAppStatus == 2) {
		/// 強制終了
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("Force event:prgFinished"));
		m_pAccessEvents->CallStatusChanged(NULL, prgFinished);
	}
}

// 応答処理を有効/無効メニュー
void CMainFrame::OnPopupEnable() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString str;

	if (m_flgEnable == 1) {
		/// 応答処理を無効にする
		m_flgEnable = 0;
	} else if (m_flgEnable == 0) {
		/// 応答処理を有効にする
		//// Skypeの状態確認
		if (!theApp.m_blSkypeConnect) {
			str.LoadString(IDS_MSGBOX3);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return;
		}
		m_flgEnable = 1;
	}

	// レジストリ更新
	CRegDWORD rgDisable = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\InitDisable"), 0, TRUE);
	rgDisable = (m_flgEnable == 1) ? 0 : 1;

	InitTooltip();	// ツールチップの初期化
}

// 自動録音処理を有効/無効メニュー
void CMainFrame::OnPopupAutoEnable() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString str;

	if (m_flgAutoEnable == 1) {
		/// 自動録音処理を無効にする
		m_flgAutoEnable = 0;
	} else if (m_flgAutoEnable == 0) {
		/// 自動録音処理を有効にする
		//// Skypeの状態確認
		if (!theApp.m_blSkypeConnect) {
			str.LoadString(IDS_MSGBOX3);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return;
		}
		m_flgAutoEnable = 1;
	}

	// レジストリ更新
	CRegDWORD rgEnableAuto = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableAutoVoiceRecord"), 0, TRUE);	// 初期値:0(False)
	rgEnableAuto = m_flgAutoEnable;

	InitTooltip();	// ツールチップの初期化
}

// ショートカットキーの参照
void CMainFrame::OnPopupHotkey() 
{
	static CSetHotkey dlg;

	if (dlg.m_hWnd == NULL) {
		dlg.Create(IDD_HOTKEY, this);
		//CButton *btn = (CButton *)dlg.GetDlgItem(IDOK);
		//btn->EnableWindow(FALSE);
	} else {
		if (dlg.IsWindowVisible()) {
			dlg.SetFocus();
			return;
		}
	}

	dlg.ShowWindow(SW_SHOW);
}

// ヘルプ表示
void CMainFrame::OnPopupHelp() 
{
	CString str;
	str.LoadString(IDS_URL_HELP);
	theApp.OpenUrl(str);
}

// SkypeIn/Out購入ページ表示
void CMainFrame::OnPopupSkype() 
{
	CString str;
	str.LoadString(IDS_URL_SKYPE);
	theApp.OpenUrl(str);
}

// 終了メニュー
void CMainFrame::OnPopupClose() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	/// ウィンドウ位置・サイズの保存
    if(IsWindowVisible()) {
		OnClose();
	}

	/// Tapur監視プログラムの終了
	TRY {
		CKillProcess kp;
		kp.KillProcess("tapmgr.exe", 500);
	}
    CATCH_ALL(e)
    {
		// Nothing
	}
	END_CATCH_ALL

	/// windowを終了
	// ハングする場合があるので最後に実行？
	DestroyWindow();
}

// メイン画面を閉じる（非表示）
void CMainFrame::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	// ウィンドウ位置・サイズの保存
    WINDOWPLACEMENT wp;
    GetWindowPlacement(&wp);
    AfxGetApp()->WriteProfileBinary(_T(SEC_SETTING), _T(ENT_WINPOS), (LPBYTE)&wp, sizeof(wp));

	// フレームサイズの保存
	CRect rct;
	int szx, szy;
	CRegRect rgRsfRect(_T("Software\\Tapur\\Tapur\\Settings\\RsfFrmPos"), CRect(0,0,0,0), TRUE);
	m_pRsfTree->GetWnd()->GetClientRect(&rct);
	szx = rct.Width();
	m_pRsfList->GetWnd()->GetClientRect(&rct);
	szy = rct.Height();
	rgRsfRect = CRect(szx, szy, 0, 0);
	CRegRect rgMsgRect(_T("Software\\Tapur\\Tapur\\Settings\\MsgFrmPos"), CRect(0,0,0,0), TRUE);
	m_pMsgTree->GetWnd()->GetClientRect(&rct);
	szx = rct.Width();
	m_pMsgList->GetWnd()->GetClientRect(&rct);
	szy = rct.Height();
	rgMsgRect = CRect(szx, szy, 0, 0);

	// windowを閉じるだけで、終了はしない。
	//windowsが削除されるので、CSystemTray::MinimiseToTray(this); は使用不可
	ShowWindow(SW_HIDE);
	
//	CFrameWnd::OnClose();
}

// 留守番応答チェック更新
void CMainFrame::OnUpdatePopupEnable(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_flgEnable);
}

// 自動録音チェック更新
void CMainFrame::OnUpdatePopupAutoEnable(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck(m_flgAutoEnable);
}

// 留守番応答中止メニュー更新
void CMainFrame::OnUpdatePopupStop(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	/// 留守番応答中
	if (theApp.m_lngAppStatus == 2) {
		pCmdUI->Enable(TRUE);
	} else {
		pCmdUI->Enable(FALSE);
	}
}

// タスクトレイアイコンのメッセージ処理
LRESULT CMainFrame::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	// 左ボタンクリックを右ボタンクリックに差し替える（同じ機能を割り当て）
	if (LOWORD(lParam) == WM_LBUTTONUP) {
		lParam &= ~WM_LBUTTONUP;
		lParam |= WM_RBUTTONUP;
	}

	// Delegate all the work back to the default 
    // implementation in CSystemTray.
    return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

// 録画イベントのメッセージ処理
LRESULT CMainFrame::OnGraphNotify(WPARAM wParam, LPARAM lParam)
{
	long evCode = lParam;	// イベントコード
	UINT uiStatus = wParam;	// 終了理由
	CString str = _T("");

	switch (uiStatus) {
        case 1:	// no window
			// 録画を停止し、録音のみ再開
			if (m_pAudio->IsRecordingSkype()) {
				OnRecordStop();
				// ツールバーアイコンの強制変更（録画→録音に移行するため）
				m_wndToolBar.ReplaceIcon(7, IDI_T_RECORD_PAUSE);
				m_wndToolBar.ReplaceIcon(8, IDI_T_RECORD_STOP);
				CString strDate = m_pAccessEvents->m_Setup.GetDateStr();
				if (!m_wndToolBar.RecordStart(strDate, TRUE, TRUE)) return NOERROR;
				str.LoadString(IDS_MSGBOX141);
			}
			break;
        case 2:	// capture size changed
			// 録画を停止し、再開
			if (m_pAudio->IsRecordingSkype()) {
				OnRecordStop();
				CString strDate = m_pAccessEvents->m_Setup.GetDateStr();
				if (!m_wndToolBar.VRecordStart(strDate, TRUE, TRUE)) return NOERROR;
				str.LoadString(IDS_MSGBOX142);
			}
			break;
        //case 3:	// window is invisible
		default:	// normal
			if (evCode == EC_USERABORT) {	// ファイルサイズ制限
				// 録画を停止し、再開
				if (m_pAudio->IsRecordingSkype()) {
					OnRecordStop();
					CString strDate = m_pAccessEvents->m_Setup.GetDateStr();
					if (!m_wndToolBar.VRecordStart(strDate, TRUE, TRUE)) return NOERROR;
					str.LoadString(IDS_MSGBOX143);
				}
			}
			break;
	}

	// バルーンメッセージ表示
	if (!str.IsEmpty()) {
		///// VRECORDボタン上に表示
		CToolBarCtrl &tbc = m_wndToolBar.GetToolBarCtrl();
		UINT idx = tbc.CommandToIndex(ID_VRECORD_START);
		RECT rc;
		tbc.GetItemRect(idx, &rc);
		CPoint pnt((rc.left+rc.right)/2, (rc.top+rc.bottom)/2);
		CBalloonHelp::LaunchBalloon(str, _T(""), pnt, IDI_INFORMATION, 
			CBalloonHelp::unSHOW_CLOSE_BUTTON|CBalloonHelp::unSHOW_TOPMOST, 
			&m_wndToolBar, _T(""), 10000);	// 10 sec
	}

	return NOERROR;
}

//  スカイプ監視イベント
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	_bstr_t bStr;
	CString str;
	CRegDWORD rgMaxRecord;
	CRegDWORD rgWaitAnswer;
	DWORD dw;
	CTimeSpan tms;

	switch (nIDEvent) {
	case ID_TIMER_APP:
	/// APP動作確認タイマ処理
		m_stApp.lngPassTime++;	// タイマー経過時間を更新
		if (m_stApp.lngPassTime == 60) {	// 10 min each
			m_stApp.lngPassTime = 0;
			//// 動作確認用のPINGメッセージをログに生成
			if (m_pAccessEvents->m_pCall == NULL) {		// 通話中はその他のログが発生するので不要
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("PING"));
				/// Skypeの接続状態の確認
				if (theApp.m_blSkypeConnect) {
					// 接続中のはずなのに接続されていない場合は再接続
					CString str;
					_bstr_t bStr;
					_bstr_t bStrRet;
					try {
						bStr = _bstr_t(_T("PING"));
						bStrRet = m_pAccessEvents->m_pAccess->SendBlockingCommand(bStr);
						str.Format(_T("%s"), (LPCTSTR)bStrRet);
						if (str.Find(_T("PONG")) < 0) {
							ConnectSkype();
						}
					} catch(_com_error& e) {
						ConnectSkype();
					} catch(...) {
						ConnectSkype();
					}
				}
			}
		}
		/// Skype接続状態チェック
		if (!theApp.m_blSkypeConnect && m_stConnect.blOnTimer == FALSE) {
			//// Skypeに接続されておらず、スカイプ接続待ちタイマも動作していない場合
			//// スカイプ接続待ちタイマを起動
			StartConnectTimer();
		}
		goto OnTimer_END;
	case ID_TIMER_CONNECT:
	/// スカイプ接続待ちタイマ処理
		m_stConnect.lngPassTime++;	// タイマー経過時間を更新
		if (!theApp.m_blSkypeConnect) {		// 接続フラグ
			/// Skypeに接続されていない場合、接続する
			if (ConnectSkype()) {
				//// 接続できた場合、スカイプ接続待ちタイマを停止
				StopConnectTimer();
			}
		}
		goto OnTimer_END;
	case ID_TIMER_RECORD_MAX:
	/// 留守番録音MAX時間チェックタイマ処理
		m_stRecordMax.lngPassTime++;	// タイマー実行回数を更新
		m_stRecordMax.tmPass = CTime::GetCurrentTime();	// タイマー経過時間を更新
		tms = m_stRecordMax.tmPass - m_stRecordMax.tmStart;
		m_stRecordMax.lngSpanTime = (long)tms.GetTotalSeconds();
		/// 録音MAX時間が経過した場合、切断（保存）する
		rgMaxRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\MaxRecordTime"), 180, TRUE);
		dw = rgMaxRecord;
		if ((long)dw < m_stRecordMax.lngSpanTime) {
			StopRecordMaxTimer();
			/// 録音録画（m_ProcCodeの更新前に呼び出す）
			m_pAccessEvents->FinishRecord();
			// 次の応答
			m_pAccessEvents->m_ProcCode += _T("$");		
			m_pAccessEvents->Response();
		}

		/// 通話が終了しているのにprgFinishedイベントが発生しない場合、終了処理コール
		if(m_pAccessEvents->m_pCall != NULL) {
			if (m_pAccessEvents->m_pCall->Status == prgFinished) {
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("Force event:prgFinished"));
				m_pAccessEvents->CallStatusChanged(NULL, prgFinished);
			}
		}

		break;
	case ID_TIMER_WAIT_ANSWER:
	/// 留守番応答待ちタイマ処理
		m_stAnswerWait.lngPassTime++;	// タイマー経過時間を更新
		if(m_pAccessEvents->m_pCall != NULL && m_pAccessEvents->m_pCall->Status != prgInProgress) {
			/// 応答待機時間が経過した場合、応答する
			rgWaitAnswer = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\WaitAnswerTime"), 10, TRUE);
			dw = rgWaitAnswer;
			if (dw > 0 && (long)dw <= m_stAnswerWait.lngPassTime) {
				StopAnswerWaitTimer();
				if (m_pAudio->IsPlaying()) {
					m_pAudio->PlayStop();
				} 
				if (m_pAudio->IsPlayingSkype()) {
					m_pAudio->PlayStopSkype();
				} 
				m_pAccessEvents->m_ProcCode = _T("$");		// 最初の応答
				//str.Format(_T("SET CALL %d STATUS INPROGRESS"), m_pAccessEvents->m_CallStatus.lngCallId);
				//m_pAccessEvents->m_pAccess->SendCommand(_bstr_t((LPCTSTR)str));	
				m_pAccessEvents->m_pCall->Status = prgInProgress;
			}
		}
		goto OnTimer_END;
	case ID_TIMER_PLAY_CHK:
	/// 応答メッセージ再生終了待ちタイマ処理
		m_stPlayChk.lngPassTime++;	// タイマー実行回数を更新
		m_stPlayChk.tmPass = CTime::GetCurrentTime();	// タイマー経過時間を更新
		tms = m_stPlayChk.tmPass - m_stPlayChk.tmStart;
		m_stPlayChk.lngSpanTime = (long)tms.GetTotalSeconds();
		/// SKYPEビデオデバイス変更
		// イベント処理内ではフレームファイルが生成されないため、ここで１回だけコール
		if (m_stPlayChk.lngPassTime == 1 && m_pVideo->IsPlayingSkype()) {
			m_pAccessEvents->SetSkypeVDevice(TVC_IN_DEV);
			m_pAccessEvents->RefreshSkypeVideo(m_pAccessEvents->m_CallStatus.lngCallId);
		}
		/// 再生制限時間に達した場合、あるいは再生メッセージが終了した場合、次の処理へ移行する
		//if ((m_stPlayChk.lngLimitTime <= m_stPlayChk.lngPassTime) ||
		//   (!m_pAudio->IsPlayingSkype() && !m_pVideo->IsPlayingSkype())) {
		if (m_stPlayChk.lngLimitTime <= m_stPlayChk.lngSpanTime) {
			if (m_pAudio->IsPlaying()) {
				m_pAudio->PlayStop();
			} 
			if (m_pAudio->IsPlayingSkype()) {
				m_pAudio->PlayStopSkype();
			} 
			if (m_pVideo->IsPlayingSkype()) {
				m_pVideo->PlayStopSkype();
			} 
			StopPlayChkTimer();
			// 次の応答処理をコール
			//m_pAccessEvents->m_ProcCode += _T("$");		チャットのみの場合もあるのでここで設定不可
			m_pAccessEvents->Response();
		}
		break;
	default:
		goto OnTimer_END;
	}
	
OnTimer_END:
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CFrameWnd::OnSizing(fwSide, pRect);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	/// ウィンドウのサイズ制限
	if ((pRect->right - pRect->left) < MIN_WINSZX) pRect->right = pRect->left + MIN_WINSZX;
	if ((pRect->bottom - pRect->top) < MIN_WINSZY) pRect->bottom = pRect->top + MIN_WINSZY;
}

/*
// KEY HOOK処理イベント
LRESULT CMainFrame::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
    KEYRESULT kr; // The struct to receive information

    // Information that we need to extract are event type and printable character
    UINT nMask = KH_MASK_EVENTTYPE | KH_MASK_PRINTABLECHAR;
 
    // This function extracts event details
    LRESULT lRes = GetKeyEventResult(wParam, lParam, &kr, nMask);
    ASSERT(lRes == KH_OK);

    // We only display key-strokes that produce printable characters
    if (kr.chPrintableChar != 0) {
        if (kr.iKeyEvent == KH_KEY_DOWN) {
			if (kr.chPrintableChar == 't' || kr.chPrintableChar == 'T') {
				/// レジストリのチェック
				CRegDWORD rgShortCut = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\ShortCutTapur", 1);
				DWORD dw = rgShortCut;
				if (dw == 1) {
					/// Tapur画面表示
					OnPopupOpen();
				}
			} else if (kr.chPrintableChar == 's' || kr.chPrintableChar == 'S') {
				/// レジストリのチェック
				CRegDWORD rgShortCut = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\ShortCutSkype", 1);
				DWORD dw = rgShortCut;
				if (dw == 1) {
					/// SKype起動or画面表示
					CRegString rgShortCut = CRegString("Software\\Skype\\Phone\\SkypePath");
					CString str = rgShortCut;
					if (str.GetLength() > 0) {
						HINSTANCE ret = ShellExecute(NULL, NULL, str,  NULL, NULL, SW_SHOWNORMAL);
	//					if ((int)ret <= 32) {
	//					}
					}
				}
			}
		}
    }

    return (LRESULT)0;
}
*/

// MainToolへの会話録音関連イベント転送
void CMainFrame::OnVRecordStart() 
{
	CString strDate = m_pAccessEvents->m_Setup.GetDateStr();
	m_wndToolBar.VRecordStart(strDate, TRUE);
}
void CMainFrame::OnRecordStart() 
{
	CString strDate = m_pAccessEvents->m_Setup.GetDateStr();
	m_wndToolBar.RecordStart(strDate, TRUE);
}
void CMainFrame::OnRecordPause() 
{
	m_wndToolBar.VRecordPause();
	m_wndToolBar.RecordPause();
}
void CMainFrame::OnRecordStop() 
{
	if (m_wndToolBar.VRecordStop(TRUE)) {
		m_wndToolBar.RecordStop(FALSE);
	} else {
		m_wndToolBar.RecordStop(TRUE);
	}
}
void CMainFrame::OnRecordMod() 
{
	m_wndToolBar.RecordMod();
}
/*
void CMainFrame::OnRecordModDropDown(NMTOOLBAR* pnmtb, LRESULT *plr)
{
	if (pnmtb->iItem == ID_VIEW_DETAILS) {
		m_wndToolBar.RecordMod();
	}
}
*/
BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

#define lpnm   ((LPNMHDR)lParam)
#define lpnmTB ((LPNMTOOLBAR)lParam)

	switch(lpnm->code)
	{
	case TBN_DROPDOWN:
		m_wndToolBar.RecordMod();
		return FALSE; //indicates the TBN_DROPDOWN
					  //notification was handled.
	}
	
	return CFrameWnd::OnNotify(wParam, lParam, pResult);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	// ウィンドウクラス名の指定
    LPCTSTR   lpzsNewName = _T(MAIN_CLASS_NAME);
    WNDCLASS wndcls;
    BOOL bRes = CFrameWnd::PreCreateWindow( cs );   // MDI では CMDIFrameWnd
    HINSTANCE hInst = AfxGetInstanceHandle();
    // see if the class already exists
    if ( !::GetClassInfo( hInst, lpzsNewName, &wndcls ) )
    {
        // get default stuff
        ::GetClassInfo( hInst, cs.lpszClass, &wndcls );
        // register a new class
        wndcls.lpszClassName = lpzsNewName;
        wndcls.hIcon = ::LoadIcon(hInst,MAKEINTRESOURCE(IDI_MAIN));
        ::RegisterClass( &wndcls );
    }
    cs.lpszClass = lpzsNewName;
    return bRes;

	//return __super::PreCreateWindow(cs);
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	/// HotKeyの処理
	UINT uiMod = MOD_CONTROL | MOD_SHIFT;
	CMsgForm *pMsgForm = NULL;
	CMsgList *pMsgList = NULL;
	int idx;

	if(message == WM_HOTKEY) {
		CSetHotkey dlg;
		dlg.LoadRegistry();

		if(LOWORD(wParam) == ID_HK_SHOW) {
			OnPopupOpen();		// メイン画面表示
		} else if(LOWORD(wParam) == ID_HK_HIDE) {
			OnClose();			// メイン画面非表示
		} else if(LOWORD(wParam) == ID_HK_RECORD) {
			OnRecordStart();	// 録音
		} else if(LOWORD(wParam) == ID_HK_VRECORD) {
			OnVRecordStart();	// 録画
		} else if(LOWORD(wParam) == ID_HK_REC_PAUSE) {
			OnRecordPause();	// 録音・録画一時停止
		} else if(LOWORD(wParam) == ID_HK_REC_STOP) {
			OnRecordStop();		// 録画終了
		} else if(LOWORD(wParam) == ID_HK_PLAY) {
			// 再生
			if(!IsWindowVisible()) return 0;	// 起動後一度も表示されていない場合は、リスト画面が生成されていない（時間がかかるので）
			pMsgForm = (CMsgForm *)m_pMsgForm->GetWnd();
			pMsgList = (CMsgList *)m_pMsgList->GetWnd();
			// 現在の選択リスト
			idx = pMsgList->SelectedId();
			// 選択されたリストがない場合最初のアイテムを選択
			if (idx < 0) {
				idx = 0;
				pMsgList->SelectId(idx);
			}
			pMsgForm->Play();
		} else if(LOWORD(wParam) == ID_HK_PLAY_PAUSE) {
			// 再生一時停止
			if(!IsWindowVisible()) return 0;	// 起動後一度も表示されていない場合は、リスト画面が生成されていない（時間がかかるので）
			pMsgForm = (CMsgForm *)m_pMsgForm->GetWnd();
			pMsgForm->PlayPause();
		} else if(LOWORD(wParam) == ID_HK_PLAY_STOP) {
			// 再生終了
			if(!IsWindowVisible()) return 0;	// 起動後一度も表示されていない場合は、リスト画面が生成されていない（時間がかかるので）
			pMsgForm = (CMsgForm *)m_pMsgForm->GetWnd();
			pMsgForm->PlayStop();
		} else if(LOWORD(wParam) == ID_HK_PLAY_NEXT) {
			// 次の再生
			if(!IsWindowVisible()) return 0;	// 起動後一度も表示されていない場合は、リスト画面が生成されていない（時間がかかるので）
			pMsgForm = (CMsgForm *)m_pMsgForm->GetWnd();
			pMsgList = (CMsgList *)m_pMsgList->GetWnd();
			// 現在の選択リスト
			idx = pMsgList->SelectedId();
			// 末尾でなければ次のリストを選択
			idx++;
			pMsgList->SelectId(idx);
			pMsgForm->Play();
		} else if(LOWORD(wParam) == ID_HK_PLAY_PREV) {
			// 前の再生
			if(!IsWindowVisible()) return 0;	// 起動後一度も表示されていない場合は、リスト画面が生成されていない（時間がかかるので）
			pMsgForm = (CMsgForm *)m_pMsgForm->GetWnd();
			pMsgList = (CMsgList *)m_pMsgList->GetWnd();
			// 現在の選択リスト
			idx = pMsgList->SelectedId();
			// 先頭でなければ前のリストを選択
			idx--;
			pMsgList->SelectId(idx);
			pMsgForm->Play();
		}
		return 0;
	}


	/// タスクトレイアイコンとツールチップの更新
	CString str;
	CString strTip;
	HICON hIcon = m_TrayIcon.GetIcon();
	if (theApp.m_blSkypeConnect) {
		/// Skype接続中
		if (theApp.m_lngAppStatus == 1) {
			/// 録音・録画中アニメの表示
			if (!m_TrayIcon.IsAnimating()) {
				m_TrayIcon.Animate(1000, -1);
				//// tooltipの変更
				str.LoadString(IDS_TOOLTIP_REC);
				m_TrayIcon.SetTooltipText(str);
				//// balloon表示
				m_TrayIcon.ShowBalloon(str, NULL, NIIF_INFO, 10);	// ? 10 sec 以上必須
			}
		} else if (theApp.m_lngAppStatus == 2) {
			/// 留守番応答中アニメの表示
			if (!m_TrayIcon.IsAnimating()) {
				m_TrayIcon.Animate(1000, -1);
				//// tooltipの変更
				str.LoadString(IDS_TOOLTIP_RCV);
				m_TrayIcon.SetTooltipText(str);
				//// balloon表示
				m_TrayIcon.ShowBalloon(str, NULL, NIIF_INFO, 10);	// ? 10 sec 以上必須
			}
		} else {
			/// 留守番応答/録音録画中でない
			if (m_TrayIcon.IsAnimating()) {
				//// アニメの中止
				m_TrayIcon.StopAnimation();
			}
			if (m_lngNewMsg > 0) {
				/// 新着メッセージがある場合
				if (hIcon != m_hIconNew) {
					m_TrayIcon.SetIcon(m_hIconNew);
					str.LoadString(IDS_TOOLTIP_NEW);
					strTip.Format(_T("%s(%d)"), str, m_lngNewMsg);
					m_TrayIcon.SetTooltipText(strTip);
				}
			} else {
				/// 新着メッセージがない場合
				if (hIcon != m_hIcon) {
					m_TrayIcon.SetIcon(m_hIcon);
					InitTooltip();	// ツールチップの初期化
				}
			}
		}
	} else {
		/// Skype接続不可
		if (hIcon != m_hIconDis) {
			m_TrayIcon.SetIcon(m_hIconDis);
			m_TrayIcon.SetTooltipText(IDS_TOOLTIP_DISCON);
		}
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

// 録音対象選択ポップアップイベント
/// 録音対象選択ボタンの更新
void CMainFrame::OnRecordModBoth()
{
	/// 録音対象状態レジストリの更新
	CRegDWORD rgRecMod(_T("Software\\Tapur\\Tapur\\Settings\\RecordMod"), 0, TRUE);
	rgRecMod = 0;

	/// 録音対象選択ボタンの更新
	m_wndToolBar.UpadateRecordMod();
}
void CMainFrame::OnRecordModRcv()
{
	/// 録音対象状態レジストリの更新
	CRegDWORD rgRecMod(_T("Software\\Tapur\\Tapur\\Settings\\RecordMod"), 0, TRUE);
	rgRecMod = 1;

	/// 録音対象選択ボタンの更新
	m_wndToolBar.UpadateRecordMod();
}
void CMainFrame::OnRecordModSnd()
{
	/// 録音対象状態レジストリの更新
	CRegDWORD rgRecMod(_T("Software\\Tapur\\Tapur\\Settings\\RecordMod"), 0, TRUE);
	rgRecMod = 2;

	/// 録音対象選択ボタンの更新
	m_wndToolBar.UpadateRecordMod();
}

/////////////////////////////////////////////////////////////////////////////

//
//  機能     : 画面の更新
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
void CMainFrame::UpdateForm() 
{
	/// 画面が表示されている時のみ有効
	//if (!(this->IsWindowVisible())) return;

	/// WINDOWタイトル
	CString str;
	if (theApp.m_blSkypeConnect) {
		str.LoadString(IDS_CAPTION_CON);
	} else {
		str.LoadString(IDS_CAPTION_DISCON);
	}
//	if (theApp.m_strUsrName.IsEmpty()) {
		str += _T(" - ") + theApp.m_strUsrId;
//	} else {
//		str += _T(" - ") + theApp.m_strUsrName;
//	}
	this->SetWindowText(str);

	/// フォルダツリー表示
	CMsgTree *pMtree = (CMsgTree *)m_pMsgTree->GetWnd();
	pMtree->UpdateForm(TRUE);
	CRsfTree *pRtree = (CRsfTree *)m_pRsfTree->GetWnd();
	pRtree->UpdateForm(TRUE);
}

//
//  機能     : ツールチップの初期化
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
void CMainFrame::InitTooltip() 
{
	CString str;
	CString strTip;

	strTip.LoadString(IDS_TOOLTIP);
	str.Empty();
	if (m_flgAutoEnable == 1) {
		str.LoadString(IDS_TOOLTIP_AUTO_ENABLE);
		strTip += _T("(") + str;
	}
	if (m_flgEnable == 0) {
		if (str.GetLength() > 0) {
			strTip += _T("/");
		} else {
			strTip += _T("(");
		}
		str.LoadString(IDS_TOOLTIP_DISABLE);
		strTip += str;
	}
	if (str.GetLength() > 0) {
		strTip += _T(")");
	}

	m_TrayIcon.SetTooltipText(strTip);
}

//
//  機能     : スカイプアクセスオブジェクトの生成
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::InitSkypeAccess(void)
{
	/// IAccessインターフェースオブジェクトの作成
	HRESULT hr = CoCreateInstance(CLSID_Access, NULL, CLSCTX_INPROC_SERVER, IID_IAccess, 
		(void**)&theApp.m_pAccess);
	if (FAILED(hr))
	{
		return FALSE;
	}

	/// シンクオブジェクトの作成
	CComObject<CAccessEvents>::CreateInstance(&m_pAccessEvents);
	if (!m_pAccessEvents)
	{
		theApp.m_pAccess = NULL;
		return FALSE;
	}
	
	/// IUnknownインターフェースの取得
	CComPtr<IUnknown> pEventUnk = m_pAccessEvents;
	
	/// シンクオブジェクトの通知
	hr = AtlAdvise(theApp.m_pAccess, pEventUnk, DIID__IAccessEvents, &m_dwCookie);
	if (FAILED(hr))
	{
		theApp.m_pAccess = NULL;
		pEventUnk = NULL;
		return FALSE;
	}
	
	/// IAccessEventインターフェースへ設定コピー
	m_pAccessEvents->SetMyParent(m_hWnd);
	m_pAccessEvents->SetAccess(theApp.m_pAccess);

	return TRUE;
}

//
//  機能     : Skypeに接続
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::ConnectSkype(void)
{
	try {
		/// Skype process の存在確認
		DWORD dwId;	// return value
		CKillProcess kp;
		HANDLE hProcess = kp.FindProcess(SKYPE_NAME, dwId);
		if(!hProcess) return FALSE;

		/// 接続確認画面が表示中の場合、Connectしない（接続画面の重複を避ける）(3.xのみ)
		HWND hwnd = ::FindWindow(_T("TskACLForm.UnicodeClass"), NULL);	//3.x
		if (hwnd) return FALSE;
		/// ログイン画面が表示中の場合、Connectしない（4.xのみ）
		hwnd = ::FindWindow(_T("TLoginForm.UnicodeClass"), NULL);	//4.x
		if (hwnd) return FALSE;

		/// Skype API に接続
//te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Connect1"));
		HRESULT hr = theApp.m_pAccess->Connect();
		if (FAILED(hr)) {
//te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Connect2"));
			return FALSE;
		}
//te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Connect3"));

// 以下のような処理はConnect後のイベント処理が済まないと実行できない
		/// PROTOCOL 指定
//		theApp.m_pAccess->Protocol = 6;	// skype3.0～
		/// 実際に使用可能なPROTOCOLの取得
//		m_pAccessEvents->m_lngProtocol = theApp.m_pAccess->Protocol;
//_stprintf_s(te.errstr, MAX_ERRSTR, _T("Skype Protocol:%d"), m_pAccessEvents->m_lngProtocol);
//te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
// 何度か再接続を試す場合があるので、メッセージウィンドウは適さない。
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't connect to Skype. Please check Skype is online."));
		//AfxMessageBox(_T("Tapur can't connect to Skype. Please check Skype is online."), 
		//	MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : スカイプ接続待ちタイマ開始
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::StartConnectTimer()
{
	if (!m_stConnect.blOnTimer) {
		SetTimer(ID_TIMER_CONNECT, 5000, NULL);	// 5 sec each
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StartConnectTimer"));
	}
	m_stConnect.blOnTimer = TRUE;		// タイマー実行中フラグ
	m_stConnect.lngPassTime = 0;		// タイマー経過時間をクリア

	return TRUE;
}

//
//  機能     : スカイプ接続待ちタイマ終了
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::StopConnectTimer()
{
	try {
		if (m_stConnect.blOnTimer) {
			KillTimer(ID_TIMER_CONNECT);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopConnectTimer"));
		}
		m_stConnect.blOnTimer = FALSE;	// タイマー実行中フラグ
		m_stConnect.lngPassTime = 0;		// タイマー経過時間をクリア
	} catch(...) {
		// CMainFrameの生成が完了する前に呼ばれる場合があるため
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("Skype connect timer error."));
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : APP動作確認タイマ開始
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::StartAppTimer()
{
	if (!m_stApp.blOnTimer) {
		SetTimer(ID_TIMER_APP, 10000, NULL);	// 10 sec each
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("StartAppTimer:%s %s"), _T(POTM_APPNAME), _T(POTM_VERSION));
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, te.errstr);
	}
	m_stApp.blOnTimer = TRUE;		// タイマー実行中フラグ
	m_stApp.lngPassTime = 0;		// タイマー経過時間をクリア

	return TRUE;
}

//
//  機能     : APP動作確認タイマ終了
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::StopAppTimer()
{
	if (m_stApp.blOnTimer) {
		KillTimer(ID_TIMER_APP);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopAppTimer"));
	}
	m_stApp.blOnTimer = FALSE;	// タイマー実行中フラグ
	m_stApp.lngPassTime = 0;		// タイマー経過時間をクリア

	return TRUE;
}

//
//  機能     : 留守番応答待ちタイマ開始
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::StartAnswerWaitTimer()
{
	if (!m_stAnswerWait.blOnTimer) {
		SetTimer(ID_TIMER_WAIT_ANSWER, 1000, NULL);	// 1 sec each
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StartAnswerWaitTimer"));
	}
	m_stAnswerWait.blOnTimer = TRUE;		// タイマー実行中フラグ
	m_stAnswerWait.lngPassTime = 0;		// タイマー経過時間をクリア

	return TRUE;
}

//
//  機能     : 留守番応答待ちタイマ終了
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::StopAnswerWaitTimer()
{
	if (m_stAnswerWait.blOnTimer) {
		KillTimer(ID_TIMER_WAIT_ANSWER);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopAnswerWaitTimer"));
	}
	m_stAnswerWait.blOnTimer = FALSE;	// タイマー実行中フラグ
	m_stAnswerWait.lngPassTime = 0;		// タイマー経過時間をクリア

	return TRUE;
}

//
//  機能     : 留守番録音MAX時間チェックタイマ開始
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::StartRecordMaxTimer()
{
	if (!m_stRecordMax.blOnTimer) {
		SetTimer(ID_TIMER_RECORD_MAX, 1000, NULL);	// 1 sec each
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StartRecordMaxTimer"));
	}
	m_stRecordMax.blOnTimer = TRUE;		// タイマー実行中フラグ
	m_stRecordMax.lngPassTime = 0;		// タイマー経過時間をクリア
	m_stRecordMax.tmStart = CTime::GetCurrentTime();		// タイマー開始時刻をセット

	return TRUE;
}

//
//  機能     : 留守番録音MAX時間チェックタイマ終了
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::StopRecordMaxTimer()
{
	if (m_stRecordMax.blOnTimer) {
		KillTimer(ID_TIMER_RECORD_MAX);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopRecordMaxTimer"));
	}
	m_stRecordMax.blOnTimer = FALSE;	// タイマー実行中フラグ
	m_stRecordMax.lngPassTime = 0;		// タイマー経過時間をクリア

	return TRUE;
}

//
//  機能     : 応答メッセージ再生終了待ちタイマ開始
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::StartPlayChkTimer(
long lngPlayTime)	// PLAY時間
{
	if (!m_stPlayChk.blOnTimer) {
		SetTimer(ID_TIMER_PLAY_CHK, 1000, NULL);	// 1 sec each(must)
CString str;
str.Format(_T("StartPlayChkTimer:%d"), lngPlayTime);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, str);
	}
	m_stPlayChk.blOnTimer = TRUE;		// タイマー実行中フラグ
	m_stPlayChk.lngPassTime = 0;		// タイマー経過時間をクリア
	m_stPlayChk.lngLimitTime = lngPlayTime;		// タイマー制限時間をセット
	m_stPlayChk.tmStart = CTime::GetCurrentTime();		// タイマー開始時刻をセット

	return TRUE;
}

//
//  機能     : 応答メッセージ再生終了待ちタイマ終了
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::StopPlayChkTimer()
{
	if (m_stPlayChk.blOnTimer) {
		KillTimer(ID_TIMER_PLAY_CHK);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopPlayChkTimer"));
	}
	m_stPlayChk.blOnTimer = FALSE;		// タイマー実行中フラグ
	m_stPlayChk.lngPassTime = 0;		// タイマー経過時間をクリア
	m_stPlayChk.lngLimitTime = 0;		// タイマー制限時間をクリア

	return TRUE;
}

//
//  機能     : TMPフォルダのクリア
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::ClearTmpData(void)
{
	if (theApp.m_strDataFolder.IsEmpty()) return TRUE;

	CString strFolder = theApp.m_strDataFolder + _T(RDF_TMP_PATH);
	CFileSpec fs(strFolder);
	fs.ClearFolder();

	strFolder = theApp.m_strDataFolder + _T(RSF_TMP_PATH);
	fs.SetFullSpec(strFolder);
	fs.ClearFolder();

	return TRUE;
}

//
//  機能     : 未登録の一時データの修復
//  
//  機能説明 : 
//  
//  返り値   : 修復データ数, -1:error
//  
//  備考     : 
//  
long CMainFrame::RepairTmpData(
long lngChkHour)	// チェックするデータの期間（指定時間(h)前まで）
{
	long lngRet = 0;
	CString strAudioFile;
	CString strVideoFile;
	CFileFind finder;
	CTime ntm;
	CTime tm;
	CTimeSpan tms;
	CFileSpec fs;

	if (theApp.m_strDataFolder.IsEmpty()) return TRUE;

	CTblRdfl tbl(theApp.m_strDBPath);
	CString strVideoFolder = m_pAccessEvents->m_Setup.RdfVideoFolder();

	TRY {
		// WAV音声ファイル
		CString strAudioFolder = m_pAccessEvents->m_Setup.RdfWavFolder();
		if (!SetCurrentDirectory(strAudioFolder)) return 0;
		ntm = CTime::GetCurrentTime();
		BOOL bWorking = finder.FindFile(_T("rdf-*.wav"));
		if (!bWorking) return 0;

		while (bWorking)
		{
			BOOL blRepair = FALSE;
			bWorking = finder.FindNextFile();
			if (!finder.IsDirectory()) {
				strAudioFile = finder.GetFileName();
				/// WAVファイル作成時間と現在との差分
				finder.GetCreationTime(tm);
				tms = ntm - tm;
				if (tms.GetTotalHours() <= lngChkHour) {
					fs.SetFullSpec(strAudioFile);
					if (!fs.Exist()) continue;
					strVideoFile = fs.FileName() + _T(".avi");
					if (!fs.Exist()) strVideoFile.Empty();
					/// 音声ファイルの修正
					if (m_pAudio->RepairWaveFile(strAudioFile) > 0) {
						blRepair = TRUE;
					}
					AfxGetThread()->PumpMessage();
					/// DBに登録
					if (m_pAccessEvents->FinishRecordTbl(strAudioFile, strVideoFile, TRUE)) {
						// 登録された場合（既存のデータがなかった場合）
						blRepair = TRUE;
					}
					if (blRepair) lngRet++;
				}
			}
		}
		finder.Close();

		// MP3音声ファイル
		strAudioFolder = m_pAccessEvents->m_Setup.RdfMp3Folder();
		if (!SetCurrentDirectory(strAudioFolder)) return 0;
		ntm = CTime::GetCurrentTime();
		bWorking = finder.FindFile(_T("rdf-*.mp3"));
		if (!bWorking) return 0;

		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (!finder.IsDirectory()) {
				strAudioFile = finder.GetFileName();
				/// MP3ファイル作成時間と現在との差分
				finder.GetCreationTime(tm);
				tms = ntm - tm;
				if (tms.GetTotalHours() <= lngChkHour) {
					fs.SetFullSpec(strAudioFile);
					if (!fs.Exist()) continue;
					strVideoFile = fs.FileName() + _T(".avi");
					if (!fs.Exist()) strVideoFile.Empty();
					/// DBに登録
					if (m_pAccessEvents->FinishRecordTbl(strAudioFile, strVideoFile)) {
					// 登録された場合（既存のデータがなかった場合）
						AfxGetThread()->PumpMessage();
						lngRet++;
					}
				}
			}
		}
		finder.Close();
	}
    CATCH_ALL(e)
    {
		finder.Close();
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("RepairTmpData error"));
		return -1;
    }
	END_CATCH_ALL

	return lngRet;
}

//
//  機能     : 新着メッセージ数更新
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainFrame::IncNewMsgCnt(
long lngCnt)		// 追加するメッセージ数、0以下：リセット
{
	if (lngCnt <= 0) {
		m_lngNewMsg = 0;
	} else {
		m_lngNewMsg += lngCnt;
		/// ツールチップの更新
		CString str;
		CString strTip;
		str.LoadString(IDS_TOOLTIP_NEW);
		strTip.Format(_T("%s(%d)"), str, m_lngNewMsg);
		m_TrayIcon.SetTooltipText(strTip);
	}

	return TRUE;
}

//
//  機能     : HotKeyの設定／解除
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
void CMainFrame::RegHotKey()
{
	m_blHotKey = FALSE;

	/// HotKeyの使用可否
	CRegDWORD rgDW = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableHotKey"), 1, TRUE);
	DWORD dwFlg = (long)rgDW;
	if (dwFlg == 0) return;

	m_blHotKey = TRUE;

	/// 詳細設定の取得
	CSetHotkey dlg;
	dlg.LoadRegistry();
	// メイン画面表示
	if (dlg.m_rgEnableShow == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_SHOW, dlg.m_rgModShow, dlg.m_rgKeyShow);
	}
	// メイン画面非表示
	if (dlg.m_rgEnableHide == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_HIDE, dlg.m_rgModHide, dlg.m_rgKeyHide);
	}
	//// 録音
	if (dlg.m_rgEnableRecord == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_RECORD, dlg.m_rgModRecord, dlg.m_rgKeyRecord);
	}
	//// 録画
	if (dlg.m_rgEnableVRecord == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_VRECORD, dlg.m_rgModVRecord, dlg.m_rgKeyVRecord);
	}
	//// 録音・録画一時停止
	if (dlg.m_rgEnableRecPause == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_REC_PAUSE, dlg.m_rgModRecPause, dlg.m_rgKeyRecPause);
	}
	//// 録音・録画終了
	if (dlg.m_rgEnableRecStop == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_REC_STOP, dlg.m_rgModRecStop, dlg.m_rgKeyRecStop);
	}
	//// 再生：現在リストで選択されているレコード
	if (dlg.m_rgEnablePlay == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_PLAY, dlg.m_rgModPlay, dlg.m_rgKeyPlay);
	}
	//// 再生一時停止・再開
	if (dlg.m_rgEnablePlayPause == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_PLAY_PAUSE, dlg.m_rgModPlayPause, dlg.m_rgKeyPlayPause);
	}
	//// 再生終了
	if (dlg.m_rgEnablePlayStop == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_PLAY_STOP, dlg.m_rgModPlayStop, dlg.m_rgKeyPlayStop);
	}
	//// 次の再生：現在リストで選択されているレコードの次のレコード
	if (dlg.m_rgEnablePlayNext == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_PLAY_NEXT, dlg.m_rgModPlayNext, dlg.m_rgKeyPlayNext);
	}
	//// 前の再生：現在リストで選択されているレコードの前のレコード
	if (dlg.m_rgEnablePlayPrev == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_PLAY_PREV, dlg.m_rgModPlayPrev, dlg.m_rgKeyPlayPrev);
	}
}
void CMainFrame::UnregHotKey()
{
	if (m_blHotKey == FALSE) return;

	CSetHotkey dlg;
	dlg.LoadRegistry();

	::UnregisterHotKey(m_hWnd, ID_HK_SHOW);
	::UnregisterHotKey(m_hWnd, ID_HK_HIDE);
	::UnregisterHotKey(m_hWnd, ID_HK_RECORD);
	::UnregisterHotKey(m_hWnd, ID_HK_VRECORD);
	::UnregisterHotKey(m_hWnd, ID_HK_REC_PAUSE);
	::UnregisterHotKey(m_hWnd, ID_HK_REC_STOP);
	::UnregisterHotKey(m_hWnd, ID_HK_PLAY);
	::UnregisterHotKey(m_hWnd, ID_HK_PLAY_PAUSE);
	::UnregisterHotKey(m_hWnd, ID_HK_PLAY_STOP);
	::UnregisterHotKey(m_hWnd, ID_HK_PLAY_NEXT);
	::UnregisterHotKey(m_hWnd, ID_HK_PLAY_PREV);
}
