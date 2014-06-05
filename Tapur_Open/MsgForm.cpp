// MsgForm.cpp : implementation file
//

#include "stdafx.h"
#include "Tapur.h"
#include "MainFrm.h"
#include "MsgForm.h"
#include "MsgTree.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "TPutil.h"
#include "FileSpec.h"
#include "MsgList.h"
#include "HttpAccess.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CMsgForm

IMPLEMENT_DYNCREATE(CMsgForm, TVisualFormView)

CMsgForm::CMsgForm()
	: TVisualFormView(CMsgForm::IDD)
{
	//{{AFX_DATA_INIT(CMsgForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	/// タイマー初期化
	m_stPlayChk.blOnTimer = FALSE;
	m_stPlayChk.lngPassTime = 0;
	m_stPlayChk.lngLimitTime = 0;

/*
	// 音声制御インスタンスの生成
	m_pAudio = CAudio::Instance();
	/// ビデオ操作クラスのインスタンス生成
	m_pVideo = CVideo::Instance();
*/
	// 背景色ブラシ初期化
	m_brush.CreateSolidBrush(RGB(255,255,255));		// white
}

CMsgForm::~CMsgForm()
{
}

void CMsgForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgForm)
	DDX_Text(pDX, IDC_EDIT_TITLE, m_EditTitle);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_Title);
	DDX_Control(pDX, IDC_STATIC_TITLE1, m_Title1);
	DDX_Control(pDX, IDC_STATIC_TITLE2, m_Title2);
	DDX_Control(pDX, IDC_STATIC_TITLE3, m_Title3);
	DDX_Control(pDX, IDC_STATIC_TITLE4, m_Title4);
	DDX_Control(pDX, IDC_STATIC_DATE, m_Date);
	DDX_Control(pDX, IDC_STATIC_SENDER_NAME, m_SenderName);
	DDX_Control(pDX, IDC_STATIC_SENDER_ID, m_SenderId);
	DDX_Control(pDX, IDC_STATIC_TIME, m_Time);
	DDX_Control(pDX, IDC_PROGRESS, m_Prog);
	DDX_Control(pDX, IDC_BTN_FILE, m_File);
	DDX_Control(pDX, IDC_BTN_VFILE, m_VFile);
	DDX_Control(pDX, IDC_BTN_PODCAST, m_PodCast);
	DDX_Control(pDX, IDC_APP_PLAY, m_AppPlay);
	DDX_Control(pDX, IDC_APP_VPLAY, m_AppVPlay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgForm, TVisualFormView)
	//{{AFX_MSG_MAP(CMsgForm)
	ON_EN_KILLFOCUS(IDC_EDIT_TITLE, OnKillfocusEditTitle)
	ON_BN_CLICKED(IDC_PLAY, OnBtnPlay)
	ON_BN_CLICKED(IDC_PAUSE, OnBtnPause)
	ON_BN_CLICKED(IDC_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_FILE, OnBtnFile)
	ON_BN_CLICKED(IDC_BTN_VFILE, OnBtnVFile)
	ON_BN_CLICKED(IDC_APP_PLAY, OnBtnAppPlay)
	ON_BN_CLICKED(IDC_APP_VPLAY, OnBtnAppVPlay)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgForm diagnostics

#ifdef _DEBUG
void CMsgForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CMsgForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMsgForm message handlers

int CMsgForm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (TVisualFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください
	
	m_lngPlayTime = 0;

	// 演奏時間管理用タイマー開始
//	SetTimer(ID_TIMER_MSGFORM, 1000, NULL);	// 1 sec each
	
	return 0;
}

void CMsgForm::OnDestroy() 
{
	TVisualFormView::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	m_lngPlayTime = 0;

	// 演奏時間管理用タイマー破棄
	StopPlayChkTimer();
//	KillTimer(ID_TIMER_MSGFORM);
}

void CMsgForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	// 音声制御インスタンスの生成
	m_pAudio = CAudio::Instance();
	/// ビデオ操作クラスのインスタンス生成
	m_pVideo = CVideo::Instance();
 
	/// ボタンIDをCBitmapButtonオブジェクトに関連付ける
	// "Failed to load bitmap for normal image."エラーが出るが無視してよい
	m_Play.AutoLoad(IDC_PLAY, this);
	m_Pause.AutoLoad(IDC_PAUSE, this);
	m_Stop.AutoLoad(IDC_STOP, this);
	//// ボタンのキャプションに関連したビットマップがロードされないので、リソースIDで再指定する。
	m_Play.LoadBitmaps(PLAYU, PLAYD, PLAYF, PLAYX);
	m_Pause.LoadBitmaps(PAUSEU, PAUSED, PAUSEF, PAUSEX);
	m_Stop.LoadBitmaps(STOPU, STOPD, STOPF, STOPX);
	/// 実行時にボタンを位置合わせする(Playボタンに合わせて配置される)
	//// Playボタンの座標をダイアログボックスからの相対位置として取得する
	CRect rect;
	m_Play.GetWindowRect(rect);
	ScreenToClient(rect);
	int nTopPos = rect.top;
	int nWidth = rect.right - rect.left;    
	//// Pauseボタンの位置を設定する
	m_Pause.SetWindowPos(NULL, rect.right, nTopPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//// Stopボタンの位置を設定する
	m_Stop.SetWindowPos(NULL, rect.right + nWidth, nTopPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	/// アイコンをCButtonに割り当て
	HICON hIcon;
	hIcon = AfxGetApp()->LoadIcon(IDI_APP_PLAY);
	m_AppPlay.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_APP_VPLAY);
	m_AppVPlay.SetIcon(hIcon);

	/// ラベルの属性変更
	//// フォームのデフォルト設定の取得
	CFont *ft = CFormView::GetFont();
	LOGFONT lft;
	ft->GetLogFont(&lft);
	//// 更新
	m_Title.SetFontBold(TRUE);
	m_Title.SetFontUnderline(TRUE);
	m_Title.SetBkColor(RGB(255,255,255));	// white
	m_Time.SetBkColor(RGB(255,255,255));	// white

	UpdateData(FALSE);	// DDX更新
}

HBRUSH CMsgForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = TVisualFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: この位置で DC のアトリビュートを変更してください
	
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください

	/// フォーム背景色を白に変更
	if (nCtlColor == CTLCOLOR_DLG) {
		return (HBRUSH)m_brush.GetSafeHandle();
	}

	/// STATICコントロール色変更
	if (nCtlColor == CTLCOLOR_STATIC) {
		int id = pWnd->GetDlgCtrlID();
		if (id == IDC_STATIC_TITLE || id == IDC_STATIC_TIME) {
			pDC->SetBkColor(RGB(255,255,255));
			pDC->SetBkMode(TRANSPARENT);//背景透過
			return (HBRUSH)m_brush.GetSafeHandle();
		}
	}

	return hbr;
}

// Windowの表示・非表示時イベント
void CMsgForm::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	TVisualFormView::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
	///// 再生中のファイルを停止する
	//PlayStop();
}

// [件名]テキストボックスからのフォーカスOUT
void CMsgForm::OnKillfocusEditTitle()
{
	CString strBak = m_EditTitle;
	UpdateData(TRUE);	// DDX更新
	if (m_EditTitle == strBak) return;	// 変更がない場合

	/// RDFLテーブルの更新
	CTblRdfl tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(theApp.m_lngRdflId) <= 0) return;
	tbl.strTitle = m_EditTitle;		// 件名
	tbl.UpdateRec(theApp.m_lngRdflId);

	/// リスト上の[件名]表示の更新
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgList *pfview = (CMsgList *)pFrame->m_pMsgList->GetWnd();
	CReportCtrl &rc = pfview->m_wndList;
	rc.SetItemText(pfview->m_hActive, 3, m_EditTitle);
}

void CMsgForm::OnBtnPlay()
{
	Play();
}

void CMsgForm::OnBtnPause()
{
	PlayPause();
}

void CMsgForm::OnBtnStop()
{
	PlayStop();
}

// 音声ファイルのエクスポート
void CMsgForm::OnBtnFile()
{
	ExportFile();
}

// ビデオファイルのエクスポート
void CMsgForm::OnBtnVFile()
{
	ExportVFile();
}

// 音声ファイルの外部アプリ再生
void CMsgForm::OnBtnAppPlay()
{
	HINSTANCE ret = ShellExecute(NULL, NULL, (LPCTSTR)m_strAudioFile,  NULL, NULL, SW_SHOWNORMAL);
}

// ビデオファイルの外部アプリ再生
void CMsgForm::OnBtnAppVPlay()
{
	HINSTANCE ret = ShellExecute(NULL, NULL, (LPCTSTR)m_strVideoFile,  NULL, NULL, SW_SHOWNORMAL);
}

// 音声ファイルのPODCAST公開
//void CMsgForm::OnBtnPodCast()
//{
//	CFileSpec fs;
//	CString strFile;
//
////m_strPlayFile = _T("C:\\Documents and Settings\\paak\\My Documents\\テスト.wav");
//
//	/// Cnv WAV to MP3
//	fs.SetFullSpec(m_strAudioFile);
//	if(!fs.Exist()) {
//		AfxMessageBox(_T("no file!"), MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
//		return;
//	}
//	strFile = fs.FullPathNoExtension() + _T(".mp3");
//	CCnvAudioDlg cnvDlg;
//	cnvDlg.m_strSrcFile = m_strAudioFile;
//	cnvDlg.m_strDstFile = strFile;
//	if (!cnvDlg.CnvWavMp3()) return;
//
//	/// Upload to WEB Server
//	CString str;
//	CString strUrlHost = _T("218.216.71.189");
//	CString strUrlPath = _T("/media/Japanese/test/yamamoto_tatsuya/");
//	CString strUsr = _T("paak");
//	CString strPass = _T("paak1085");
//	CString strSrcFile = strFile;
//
//	char url_host[MAX_LEN];
//	char url_path[MAX_LEN];
//	char username[MAX_LEN];
//	char password[MAX_LEN];
////	char src_filename[MAX_LEN];
//	char dst_filename[MAX_LEN];
//
//	wcstombs(url_host, strUrlHost, MAX_LEN);
//	wcstombs(url_path, strUrlPath, MAX_LEN);
//	wcstombs(username, strUsr, MAX_LEN);
//	wcstombs(password, strPass, MAX_LEN);
////	CnvCString_Bytes(strFile, src_filename, MAX_LEN);
//	fs.SetFullSpec(strFile);
//	strFile = strUrlPath + fs.GetFileName();
//	CnvCString_Bytes(strFile, dst_filename, MAX_LEN);
//
//	CHttpAccess http;
//	int upSock = http.HttpConnect(url_host, url_path, HTTP_PORT, username, password);
//	if (upSock < 0) {
//		str.Format(_T("can't connect to URL:%s%s\n"),strUrlHost,strUrlPath);
//		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
//		return;
//	}
//	if (http.HttpUpLoad(upSock, url_host, dst_filename, (LPCTSTR)strSrcFile) < 0) {
//		str.Format(_T("can't upload file:%s to HOST:%s\n"),m_strAudioFile, strUrlHost);
//		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
//		http.HttpDisConnect(upSock);
//		return;
//	}
//
//	http.HttpDisConnect(upSock);
//
//	AfxMessageBox(_T("SUCCESS"), MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
//}

// ボタンのヘルプをステータスバーに表示
BOOL CMsgForm::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	DWORD pos;
	CPoint mouse;
	CRect area;
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();

	pos = ::GetMessagePos();
	mouse.x = LOWORD(pos);
	mouse.y = HIWORD(pos);
	m_AppPlay.GetWindowRect(&area);
	if ( area.PtInRect(mouse) != 0 ) {
		/// マウス座標がコントロール内だった場合の処理
		pFrame->SetMessageText(ID_APP_PLAY);
		goto OnSetCursor_END;
	}
	m_AppVPlay.GetWindowRect(&area);
	if ( area.PtInRect(mouse) != 0 ) {
		/// マウス座標がコントロール内だった場合の処理
		pFrame->SetMessageText(ID_APP_VPLAY);
		goto OnSetCursor_END;
	}

	pFrame->SetMessageText(_T(""));

OnSetCursor_END:
	return TVisualFormView::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CMsgForm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		/// RETURN KEY 
		if (nVirtKey == VK_RETURN) {
			//// フォーカスコントロールがBitmapボタンの場合
			CWnd *cWnd = GetFocus();
			if (cWnd->m_hWnd == m_Play.m_hWnd) {
				OnBtnPlay();
				return TRUE;
			} else if (cWnd->m_hWnd == m_Pause.m_hWnd) {
				OnBtnPause();
				return TRUE;
			} else if (cWnd->m_hWnd == m_Stop.m_hWnd) {
				OnBtnStop();
				return TRUE;
			} else if (cWnd->m_hWnd == GetDlgItem(IDC_EDIT_TITLE)->m_hWnd) {
				//// 件名入力の確定
				OnKillfocusEditTitle();
				return TRUE;
			}
		} else if (nVirtKey == VK_TAB) {
		/// TAB KEY : MsgTreeフォームへ遷移
			//// フォーカス最後の場合のみ有効
			CWnd *cWnd = GetFocus();
			if ((cWnd->m_hWnd == m_AppVPlay.m_hWnd) || 
				(cWnd->m_hWnd == m_AppPlay.m_hWnd && m_AppVPlay.IsWindowEnabled() == FALSE) ||
				(cWnd->m_hWnd == m_Title.m_hWnd && m_AppPlay.IsWindowEnabled() == FALSE)) {
				CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
				CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
				pfview->SetFocus();
				return TRUE;
			}
		}

	}
	
	return TVisualFormView::PreTranslateMessage(pMsg);
}

void CMsgForm::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	if (nIDEvent == ID_TIMER_MSGFORM) {
		m_stPlayChk.lngPassTime++;	// タイマー経過時間を更新
		/// 再生時のプログレスバー更新
		int intPos, intLow, intUpper;
		if (m_pAudio->IsPlaying()) {
			if (!m_pAudio->IsPlayPause()) {
				intPos = m_Prog.GetPos();
				m_Prog.GetRange(intLow, intUpper);
				if (intPos < intUpper) {
					m_Prog.StepIt();
				} else {
					/// 再生終了
					PlayStop();
				}
			}
		} else {
			/// 再生終了
			PlayStop();
			/// プログレスバーをフルに（見た目）
			m_Prog.GetRange(intLow, intUpper);
			m_Prog.SetPos(intUpper);
		}
	}
	
	TVisualFormView::OnTimer(nIDEvent);
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
BOOL CMsgForm::UpdateForm(BOOL blPlayStop) 
//BOOL CMsgForm::UpdateForm() 
{
	CFileSpec fs;
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();

	/// カレントレコード
	if (theApp.m_lngRdflId <= 0) return FALSE;

	/// 再生中のファイルを停止する(フォーカスが移動するので注意！)
//	PlayStop();
	if (blPlayStop) PlayStop();

	//// プログレスバークリア
	if (m_Prog) m_Prog.SetPos(0);	


	/// 初期設定
	m_lngPlayTime = 0;
	m_strAudioFile.Empty();
	m_strVideoFile.Empty();
	m_Play.EnableWindow(FALSE);
	m_Stop.EnableWindow(FALSE);
	m_Pause.EnableWindow(FALSE);
	m_File.EnableWindow(FALSE);
	m_VFile.EnableWindow(FALSE);
	m_AppPlay.EnableWindow(FALSE);
	m_AppVPlay.EnableWindow(FALSE);
	m_Time.SetWindowText(_T("[ 00:00 ]"));

	/// RDFLテーブル検索
	CTblRdfl tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(theApp.m_lngRdflId) <= 0) {
		return FALSE;
	}

	/// [件名]取得
	m_EditTitle = tbl.strTitle;
	//// [送信者名]取得
	CString strSender;
	if (tbl.strSenderId == _T("0")) {
		//// 不明な発信者(SkypeIn)
		strSender.LoadString(IDS_MSG_DEF);
		m_SenderName.SetWindowText(strSender);
		m_SenderId.SetWindowText(_T(""));
	} else {
		m_SenderName.SetWindowText(tbl.strSenderName);
		m_SenderId.SetWindowText(tbl.strSenderId);
	}
	//// [日時]取得
	_TCHAR strDate[64];
	CnvDateString((LPCTSTR)tbl.dlngSystemNew, strDate);
	m_Date.SetWindowText(strDate);
	/// 音声/ビデオファイル名取得
	int intFormatType = 0;
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(tbl.strSenderId);
	m_strVideoFile = tps.FindRdfVideoFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
	m_strAudioFile = tps.FindRdfWavFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
	if (m_strAudioFile.IsEmpty()) {
		m_strAudioFile = tps.FindRdfMp3FileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
		intFormatType = 1;
	}

	CString strBar;
	if (!m_strAudioFile.IsEmpty()) {
	/// 受信音声ファイルがある場合
		CString strTmp;
		//// STatusBar用文字列
		fs.SetFullSpec(m_strAudioFile);
		strBar = fs.GetFileName();
		/// 受信音声ファイルの時間表示
		float fTime = m_pAudio->GetPlaySize(m_strAudioFile);
		if (fTime < 0) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Audio file may be broken:%s"), m_strAudioFile);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, te.errstr);
			return FALSE;
		}
		m_lngPlayTime = (fTime > 0) ? (long)fTime : 1;	// １秒以下は１秒とする
		strTmp.Format(_T("[ %02i:%02i ]"), (m_lngPlayTime/60), (m_lngPlayTime%60));
		m_Time.SetWindowText(strTmp);
		//// 再生・エクスポートボタンを有効
		m_Play.EnableWindow(TRUE);
		m_File.EnableWindow(TRUE);
		m_AppPlay.EnableWindow(TRUE);

	}
	if (!m_strVideoFile.IsEmpty()) {
	/// 受信ビデオファイルがある場合
		//// STatusBar用文字列
		if (strBar.GetLength() > 0) {
			strBar += _T("/avi");
		} else {
			fs.SetFullSpec(m_strVideoFile);
			strBar = fs.GetFileName();
		}
		//// エクスポートボタンを有効
		m_VFile.EnableWindow(TRUE);
		m_AppVPlay.EnableWindow(TRUE);
	}

	/// StatusBarにファイル名、データタイプ表示
	pFrame->SetStatusText(strBar);
	if (tbl.lngRespType == 0) {
		strBar = _T("REC");
		if (tbl.lngClsCode1 == 1 || tbl.lngClsCode1 == 3) {
			strBar += _T("/IN");
		} else if (tbl.lngClsCode1 == 2 || tbl.lngClsCode1 == 4) {
			strBar += _T("/OUT");
		}
	} else {
		strBar = _T("ANSWER");
	}
	pFrame->SetStatusText(strBar, 1);

	/// 再生ファイルが存在しない場合、表示のみで未再生リストを再生済みに変更
	if (m_strAudioFile.IsEmpty() && m_strVideoFile.IsEmpty() && tbl.lngViewFlag != 1) {
		// （？？？）ChgMsgMark()でテーブル更新すると、テーブルロックが解除されていないため、ここで更新する
		tbl.lngViewFlag = 1;
		tbl.UpdateRec(theApp.m_lngRdflId);
		CMsgList *pfview = (CMsgList *)pFrame->m_pMsgList->GetWnd();
		pfview->ChgMsgMark(FALSE);
	}

	UpdateData(FALSE);	// DDX更新

	return TRUE;
}

//
//  機能     : 音声・ビデオ再生
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgForm::Play()
{
	if (_tcsclen(m_strAudioFile) <= 0) return FALSE;

	/// 再生中であれば先に停止
	PlayStop();

	/// ビデオ再生開始
	if (!m_strVideoFile.IsEmpty()) {
		if (!m_pVideo->PlayOpen()) {
			CString str;
			str.LoadString(IDS_MSGBOX64);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return FALSE;
		}
		if (!m_pVideo->Play(m_strVideoFile)) {
			// Win7+DivXの環境でVideoGrabberが動作しない→エラーでなくWMP再生で代用
			//CString str;
			//str.LoadString(IDS_MSGBOX64);
			//AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			//return FALSE;
			m_pVideo->PlayStop();
			OnBtnAppVPlay();
			goto SECTION_AUDIO_SKIP;
		}
	}
	
	/// 音声再生開始
	m_pAudio->m_PlayFile = m_strAudioFile;
	if (!m_pAudio->PlayOpen()) {
		CString str;
		str.LoadString(IDS_MSGBOX61);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (!m_pAudio->Play(FALSE)) {
		CString str;
		str.LoadString(IDS_MSGBOX61);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	/// 演奏時間管理用タイマー開始
	StartPlayChkTimer(m_lngPlayTime);

	/// 停止ボタンを有効
	m_Stop.EnableWindow(TRUE);
	/// 一時停止ボタンを有効
	m_Pause.EnableWindow(TRUE);
	/// プログレスバー初期化
	if (m_Prog) m_Prog.SetPos(0);	

SECTION_AUDIO_SKIP:

	/// 未再生リストを再生済みに変更
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgList *pfview = (CMsgList *)pFrame->m_pMsgList->GetWnd();
	pfview->ChgMsgMark();

	/// 新着メッセージ数をクリア(TOOLTIPの更新のため)
	pFrame->IncNewMsgCnt(0);

	return TRUE;
}

//
//  機能     : 音声・ビデオ再生一時停止
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgForm::PlayPause()
{
	if (!m_pVideo->IsPlaying() && !m_pAudio->IsPlaying()) return TRUE;

	if (m_pAudio->IsPlayPause()) {
		/// 再生一時停止の復帰
		if (m_pAudio->IsPlaying()) {
			m_pAudio->PlayContinue();
		}
		if (m_pVideo->IsPlaying()) {
			m_pVideo->PlayContinue();
		}
	} else {
		/// 再生一時停止
		if (m_pVideo->IsPlaying()) {
			m_pVideo->PlayPause();
		}
		if (m_pAudio->IsPlaying()) {
			m_pAudio->PlayPause();
		}
	}

	///// 再生ボタンにフォーカス
	//m_Play.SetFocus();
	///// 一時停止ボタンを無効
	//m_Pause.EnableWindow(FALSE);
	/// 一時停止ボタンにフォーカス
	m_Pause.SetFocus();

	return TRUE;
}

//
//  機能     : 音声・ビデオ再生停止
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgForm::PlayStop()
{
	/// ビデオ再生終了
	if (m_pVideo->IsPlaying()) {
		m_pVideo->PlayStop();
	}

	/// 音声再生終了
	if (m_pAudio->IsPlaying()) {
		m_pAudio->PlayStop();
	}

	//// 演奏時間管理用タイマー終了
	StopPlayChkTimer();
	//// プログレスバー初期化
//	if (m_Prog) m_Prog.SetPos(0);	

	/// 再生ボタンにフォーカス
	if (m_Play) m_Play.SetFocus();
	/// 停止ボタンを無効
	if (m_Stop) m_Stop.EnableWindow(FALSE);
	/// 一時停止ボタンを無効
	if (m_Pause) m_Pause.EnableWindow(FALSE);

	return TRUE;
}

// 音声ファイルのエクスポート
BOOL CMsgForm::ExportFile()
{
	if (!m_File.IsWindowEnabled()) return FALSE;

	if (m_cnvADlg.m_hWnd == NULL) {
		m_cnvADlg.Create(IDD_CONVERT_AUDIO, this);
	} else {
		if (m_cnvADlg.IsWindowVisible()) {
			m_cnvADlg.SetFocus();
			return TRUE;
		}
	}

	m_cnvADlg.SetSrcFile(m_strAudioFile);
	m_cnvADlg.ShowWindow(SW_SHOW);

	return TRUE;
}

// ビデオファイルのエクスポート
BOOL CMsgForm::ExportVFile()
{
	if (!m_VFile.IsWindowEnabled()) return FALSE;

	if (m_cnvVDlg.m_hWnd == NULL) {
		m_cnvVDlg.Create(IDD_CONVERT_VIDEO,this);
	} else {
		if (m_cnvVDlg.IsWindowVisible()) {
			m_cnvVDlg.SetFocus();
			return TRUE;
		}
	}

	m_cnvVDlg.SetSrcFile(m_strVideoFile, m_strAudioFile);
	m_cnvVDlg.ShowWindow(SW_SHOW);

	return TRUE;
}

//
//  機能     : 演奏時間管理用タイマー開始
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgForm::StartPlayChkTimer(
long lngPlayTime)	// PLAY時間
{
	if (!m_stPlayChk.blOnTimer) {
		SetTimer(ID_TIMER_MSGFORM, 1000, NULL);	// 1 sec each(must)
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StartPlayChkTimer"));
	}
	m_stPlayChk.blOnTimer = TRUE;		// タイマー実行中フラグ
	m_stPlayChk.lngPassTime = 0;		// タイマー経過時間をクリア
	m_stPlayChk.lngLimitTime = lngPlayTime;		// タイマー制限時間をセット

	//// プログレスバー開始
	if (m_Prog) {
		m_Prog.SetStep(1);
		m_Prog.SetRange32(0, m_lngPlayTime);
		m_Prog.SetPos(0);
	}

	return TRUE;
}

//
//  機能     : 演奏時間管理用タイマー終了
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMsgForm::StopPlayChkTimer()
{
	if (m_stPlayChk.blOnTimer) {
		KillTimer(ID_TIMER_MSGFORM);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopPlayChkTimer"));
	}
	m_stPlayChk.blOnTimer = FALSE;		// タイマー実行中フラグ
	m_stPlayChk.lngPassTime = 0;		// タイマー経過時間をクリア
	m_stPlayChk.lngLimitTime = 0;		// タイマー制限時間をクリア

	return TRUE;
}

