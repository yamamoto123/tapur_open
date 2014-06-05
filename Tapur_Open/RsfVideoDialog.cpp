// RsfVideoDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfVideoDialog.h"
#include "RsfTree.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"
#include ".\rsfvideodialog.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CRsfVideoDialog ダイアログ


CRsfVideoDialog::CRsfVideoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRsfVideoDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRsfVideoDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	/// タイマー初期化
	m_stPlayChk.blOnTimer = FALSE;
	m_stPlayChk.lngPassTime = 0;
	m_stPlayChk.lngLimitTime = 0;

/*
	// ビデオ制御インスタンスの生成
	m_pVideo = CVideo::Instance();
*/
}

CRsfVideoDialog::~CRsfVideoDialog()
{
}

void CRsfVideoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfVideoDialog)
	DDX_Control(pDX, IDC_STATIC_TIME, m_Time);
	DDX_Control(pDX, IDC_PROGRESS, m_Prog);
	DDX_Control(pDX, IDC_BTN_FILE, m_File);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRsfVideoDialog, CDialog)
	//{{AFX_MSG_MAP(CRsfVideoDialog)
	ON_BN_CLICKED(IDC_PLAY, OnBtnPlay)
	ON_BN_CLICKED(IDC_PAUSE, OnBtnPause)
	ON_BN_CLICKED(IDC_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_FILE, OnBtnFile)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfVideoDialog メッセージ ハンドラ

int CRsfVideoDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO :  ここに特定な作成コードを追加してください。

	return 0;
}

// （注意）このメソッド内でコントロールのフォーカス設定は行わない
// →呼び出しフォームにフォーカスが戻らなくなるため
void CRsfVideoDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
	//// 演奏時間管理用タイマー終了
	StopPlayChkTimer();

	if (_tcsclen(m_strPlayFile) > 0) {
		/// 再生終了
		if (m_pVideo->IsPlaying()) {
			m_pVideo->PlayStop();
		}
	}
}

BOOL CRsfVideoDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	// ビデオ制御インスタンスの生成
	m_pVideo = CVideo::Instance();

	/// ボタンIDをCBitmapButtonオブジェクトに関連付ける
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

	// 画面更新
	UpdateForm();

	// 演奏時間管理用タイマー開始
//	SetTimer(ID_TIMER_RSF_VIDEO, 1000, NULL);	// 1 sec each

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CRsfVideoDialog::OnBtnPlay()
{
	Play();
}

void CRsfVideoDialog::OnBtnPause()
{
	if (_tcsclen(m_strPlayFile) <= 0) return;

	/// 再生一時停止
	if (!m_pVideo->IsPlaying()) return;
	m_pVideo->PlayPause();

	/// 再生ボタンにフォーカス
	m_Play.SetFocus();
	/// 一時停止ボタンを無効
	m_Pause.EnableWindow(FALSE);
}

void CRsfVideoDialog::OnBtnStop()
{
	PlayStop();
}

void CRsfVideoDialog::OnBtnFile()
{
	CString str;
	CString strFile;

	/// 応答ビデオファイル参照
	CFileDialog fileDlg(TRUE, _T("AVI"), _T("*.avi"), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 
		_T("AVI(*.avi)|*.avi|all(*.*)|*.*||"));
	if (fileDlg.DoModal() != IDOK) return;
	strFile = fileDlg.GetPathName();
	CFileSpec fs(strFile);
	if (!fs.Exist()) return;

	/// 再生中のファイルを停止する
	PlayStop();

	/// ファイルのフォーマットチェック
	m_pVideo->SetPlayFile(strFile);
	if (!m_pVideo->ChkPlayFile()) {
		str.LoadString(IDS_MSGBOX66);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}

	/// 現在の応答ビデオファイルのバックアップ
	fs.SetFullSpec(m_strPlayFile);
	if (fs.Exist()) {
		CTPsetup tps(theApp.m_strDataFolder);
		m_strTmpFile = tps.RsfTmpVideoFileName();
		fs.FileMove(m_strTmpFile);
	}

	/// 応答ビデオファイルのコピー
	fs.SetFullSpec(strFile);
	fs.FileCopy(m_strPlayFile, TRUE, TRUE);

	/// 画面の更新
	UpdateForm();
}

// キーボード操作の調整
BOOL CRsfVideoDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		/// RETURN KEY : Bitmapボタンの実行
		// ビットマップボタンはキー押下でデフォルトハンドル（OnOk）を呼んでしまうため
		if (nVirtKey == VK_RETURN) {
			//// フォーカスコントロールがBitmapボタンの場合のみ有効
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
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CRsfVideoDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	if (nIDEvent == ID_TIMER_RSF_VIDEO) {
		m_stPlayChk.lngPassTime++;	// タイマー経過時間を更新
		/// 再生時のプログレスバー更新
		int intPos, intLow, intUpper;
		if (m_pVideo->IsPlaying()) {
			if (!m_pVideo->IsPlayPause()) {
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
/* IsPlaying()になるまでに時間がかかる場合があるため、以下の処理は削除
			/// 再生終了
			PlayStop();
			/// プログレスバーをフルに（見た目）
			m_Prog.GetRange(intLow, intUpper);
			m_Prog.SetPos(intUpper);
*/
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CRsfVideoDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

	///　登録確認
//	str.LoadString(IDS_MSGBOX60);
//	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return;

	CDialog::OnOK();
}

void CRsfVideoDialog::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。

	//// バックアップファイルを戻す
	CFileSpec fs(m_strTmpFile);
	if (fs.Exist()) {
		fs.FileMove(m_strPlayFile);
	}
	
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
BOOL CRsfVideoDialog::UpdateForm() 
{
	/// 再生中のファイルを停止する
	PlayStop();

	/// 初期設定
	m_lngPlayTime = 0;
	m_Play.EnableWindow(FALSE);
	m_Stop.EnableWindow(FALSE);
	m_Pause.EnableWindow(FALSE);
	m_Time.SetText(_T("[ 00:00 ]"));

	CString strSize;
	CFileSpec fs(m_strPlayFile);
	if (fs.Exist()) {
	/// 応答ビデオファイルがある場合
		/// 応答ビデオファイルの時間表示
		m_pVideo->SetPlayFile(m_strPlayFile);
		float fTime = m_pVideo->GetPlaySize();
		if (fTime < 0) return FALSE;
		m_lngPlayTime = (long)(fTime+1);
		strSize.Format(_T("[ %02i:%02i ]"), (m_lngPlayTime/60), (m_lngPlayTime%60));
		m_Time.SetText(strSize);
		//// 再生ボタンを有効
		m_Play.EnableWindow(TRUE);
	}

	UpdateData(FALSE);	// DDX更新

	return TRUE;
}

//
//  機能     : ビデオ再生
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfVideoDialog::Play()
{
	if (_tcsclen(m_strPlayFile) <= 0) return FALSE;

	if (m_pVideo->IsPlaying() && m_pVideo->IsPlayPause()) {
		/// 再生一時停止の復帰
		m_pVideo->PlayContinue();
	} else {
		/// 再生開始
		m_pVideo->PlayOpen();
		if (!m_pVideo->Play(m_strPlayFile)) {
			CString str;
			str.LoadString(IDS_MSGBOX64);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return FALSE;
		}

		/// 演奏時間管理用タイマー開始
		StartPlayChkTimer(m_lngPlayTime);

		/// 停止ボタンを有効
		m_Stop.EnableWindow(TRUE);
	}
	/// 一時停止ボタンを有効
	m_Pause.EnableWindow(TRUE);

	return TRUE;
}

//
//  機能     : ビデオ再生停止
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfVideoDialog::PlayStop()
{
	if (_tcsclen(m_strPlayFile) <= 0) return FALSE;

	/// 再生終了
	if (m_pVideo->IsPlaying()) {
		m_pVideo->PlayStop();
	}

	//// 演奏時間管理用タイマー終了
	StopPlayChkTimer();
	//// プログレスバー末尾へ
//	m_Prog.SetPos(m_lngPlayTime);	

	/// 再生ボタンにフォーカス
	m_Play.SetFocus();
	/// 停止ボタンを無効
	m_Stop.EnableWindow(FALSE);
	/// 一時停止ボタンを無効
	m_Pause.EnableWindow(FALSE);

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
BOOL CRsfVideoDialog::StartPlayChkTimer(
long lngPlayTime)	// PLAY時間
{
	if (!m_stPlayChk.blOnTimer) {
		SetTimer(ID_TIMER_RSF_VIDEO, 1000, NULL);	// 1 sec each(must)
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
BOOL CRsfVideoDialog::StopPlayChkTimer()
{
	if (m_stPlayChk.blOnTimer) {
		KillTimer(ID_TIMER_RSF_VIDEO);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopPlayChkTimer"));
	}
	m_stPlayChk.blOnTimer = FALSE;		// タイマー実行中フラグ
	m_stPlayChk.lngPassTime = 0;		// タイマー経過時間をクリア
	m_stPlayChk.lngLimitTime = 0;		// タイマー制限時間をクリア

	return TRUE;
}

