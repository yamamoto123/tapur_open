// RsfAudioDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfAudioDialog.h"
#include "RsfTree.h"
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
// CRsfAudioDialog ダイアログ


CRsfAudioDialog::CRsfAudioDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRsfAudioDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRsfAudioDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	/// タイマー初期化
	m_stPlayChk.blOnTimer = FALSE;
	m_stPlayChk.lngPassTime = 0;
	m_stPlayChk.lngLimitTime = 0;

	//// 音声制御インスタンスの生成
	//m_pAudio = CAudio::Instance();
}

CRsfAudioDialog::~CRsfAudioDialog()
{
}

void CRsfAudioDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfAudioDialog)
	DDX_Control(pDX, IDC_STATIC_TIME, m_Time);
	DDX_Control(pDX, IDC_STATIC_LINKBUY, m_LinkBuy);
	DDX_Control(pDX, IDC_PROGRESS, m_Prog);
	DDX_Control(pDX, IDC_BTN_FILE, m_File);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRsfAudioDialog, CDialog)
	//{{AFX_MSG_MAP(CRsfAudioDialog)
	ON_BN_CLICKED(IDC_PLAY, OnBtnPlay)
	ON_BN_CLICKED(IDC_PAUSE, OnBtnPause)
	ON_BN_CLICKED(IDC_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_FILE, OnBtnFile)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfAudioDialog メッセージ ハンドラ

// （注意）このメソッド内でコントロールのフォーカス設定は行わない
// →呼び出しフォームにフォーカスが戻らなくなるため
void CRsfAudioDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
	//// 演奏時間管理用タイマー終了
	StopPlayChkTimer();

	if (_tcsclen(m_strPlayFile) > 0) {
		/// 再生終了
		if (m_pAudio->IsPlaying()) {
			m_pAudio->PlayStop();
		}
	}
}

BOOL CRsfAudioDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	/// 音声制御インスタンスの生成
	m_pAudio = CAudio::Instance();

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
	//// 音声ファイル購入リンク
	m_LinkBuy.SetTextColor(RGB(0,0,255));
	m_LinkBuy.SetFontUnderline(TRUE);
	CString str;
	str.LoadString(IDS_URL_LINKBUY);
	m_LinkBuy.SetHyperLink(str);
	HCURSOR cursor = AfxGetApp()->LoadCursor(IDC_LINK);
	m_LinkBuy.SetLinkCursor(cursor);
	m_LinkBuy.SetLink(TRUE, FALSE);

	// 画面更新
	UpdateForm();

	// 演奏時間管理用タイマー開始
//	SetTimer(ID_TIMER_RSF_AUDIO, 1000, NULL);	// 1 sec each

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CRsfAudioDialog::OnBtnPlay()
{
	Play();
}

void CRsfAudioDialog::OnBtnPause()
{
	if (_tcsclen(m_strPlayFile) <= 0) return;

	/// 再生一時停止
	if (!m_pAudio->IsPlaying()) return;
	m_pAudio->PlayPause();

	/// 再生ボタンにフォーカス
	m_Play.SetFocus();
	/// 一時停止ボタンを無効
	m_Pause.EnableWindow(FALSE);
}

void CRsfAudioDialog::OnBtnStop()
{
	PlayStop();
}

void CRsfAudioDialog::OnBtnFile()
{
	CString str;
	CString strFile;

	/// 応答音声ファイル参照
	CFileDialog fileDlg(TRUE, _T("wave"), _T("*.wav"), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 
		_T("wave(*.wav)|*.wav|all(*.*)|*.*||"));
	if (fileDlg.DoModal() != IDOK) return;
	strFile = fileDlg.GetPathName();
	CFileSpec fs(strFile);
	if (!fs.Exist()) return;

	/// 再生中のファイルを停止する
	PlayStop();

	/// ファイルのフォーマットチェック
	m_pAudio->m_PlayFile = strFile;
	if (!m_pAudio->ChkPlayFile()) {
		str.LoadString(IDS_MSGBOX63);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}

	/// 現在の応答音声ファイルのバックアップ
	fs.SetFullSpec(m_strPlayFile);
	if (fs.Exist()) {
		CTPsetup tps(theApp.m_strDataFolder);
		m_strTmpFile = tps.RsfTmpAudioFileName();
		fs.FileCopy(m_strTmpFile, TRUE);
	}

	/// 応答音声ファイルのコピー(16KHzフォーマットへ変換)
	AUDIO_FORMAT fmt;
	fmt.nBits = 16;
	fmt.nChannels = 1;
	fmt.nFrequency = 16000;
	if (!m_pAudio->ChgWaveFormat(strFile, m_strPlayFile, fmt, 1)) {
		str.LoadString(IDS_MSGBOX69);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		//// バックアップファイルを戻す
		fs.SetFullSpec(m_strTmpFile);
		if (fs.Exist()) {
			fs.FileCopy(m_strPlayFile, TRUE);
		}
		return;
	}

	/// 画面の更新
	UpdateForm();
}

// キーボード操作の調整
BOOL CRsfAudioDialog::PreTranslateMessage(MSG* pMsg) 
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

void CRsfAudioDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	if (nIDEvent == ID_TIMER_RSF_AUDIO) {
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

	CDialog::OnTimer(nIDEvent);
}

void CRsfAudioDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

	///　登録確認
//	str.LoadString(IDS_MSGBOX60);
//	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return;

	CDialog::OnOK();
}

void CRsfAudioDialog::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。

	//// バックアップファイルを戻す
	CFileSpec fs(m_strTmpFile);
	if (fs.Exist()) {
		fs.FileCopy(m_strPlayFile, TRUE);
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
BOOL CRsfAudioDialog::UpdateForm() 
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
	/// 応答音声ファイルがある場合
		/// 応答音声ファイルの時間表示
		m_pAudio->m_PlayFile = m_strPlayFile;
		float fTime = m_pAudio->GetPlaySize();
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
//  機能     : 音声再生
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfAudioDialog::Play()
{
	if (_tcsclen(m_strPlayFile) <= 0) return FALSE;

	if (m_pAudio->IsPlaying() && m_pAudio->IsPlayPause()) {
		/// 再生一時停止の復帰
		m_pAudio->PlayContinue();
	} else {
		/// 再生開始
		m_pAudio->m_PlayFile = m_strPlayFile;
		m_pAudio->PlayOpen();
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
	}
	/// 一時停止ボタンを有効
	m_Pause.EnableWindow(TRUE);

	return TRUE;
}

//
//  機能     : 音声再生停止
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfAudioDialog::PlayStop()
{
	if (_tcsclen(m_strPlayFile) <= 0) return FALSE;

	/// 再生終了
	if (m_pAudio->IsPlaying()) {
		m_pAudio->PlayStop();
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
BOOL CRsfAudioDialog::StartPlayChkTimer(
long lngPlayTime)	// PLAY時間
{
	if (!m_stPlayChk.blOnTimer) {
		SetTimer(ID_TIMER_RSF_AUDIO, 1000, NULL);	// 1 sec each(must)
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
BOOL CRsfAudioDialog::StopPlayChkTimer()
{
	if (m_stPlayChk.blOnTimer) {
		KillTimer(ID_TIMER_RSF_AUDIO);
	}
	m_stPlayChk.blOnTimer = FALSE;		// タイマー実行中フラグ
	m_stPlayChk.lngPassTime = 0;		// タイマー経過時間をクリア
	m_stPlayChk.lngLimitTime = 0;		// タイマー制限時間をクリア

	return TRUE;
}
