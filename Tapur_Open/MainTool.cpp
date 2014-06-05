// MainTool.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "NewMenu.h"
#include "Tapur.h"
#include "MainFrm.h"
#include "SkypeEvents.h"
#include "MainTool.h"
#include "TPutil.h"
#include "BalloonHelp.h"
#include "TMsgBox.h"
#include ".\maintool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// CMainFrame等のクラス定義が再帰してしまうので、メンバー変数としては定義できない。
CMainFrame *m_pFrame;
CAccessEvents *m_pAccess;

/////////////////////////////////////////////////////////////////////////////
// CMainTool

BEGIN_MESSAGE_MAP(CMainTool, CToolBar)
	//{{AFX_MSG_MAP(CMainTool)
	ON_UPDATE_COMMAND_UI(ID_VRECORD_START, OnUpdateVRecordStart)
	ON_UPDATE_COMMAND_UI(ID_RECORD_START, OnUpdateRecordStart)
	ON_UPDATE_COMMAND_UI(ID_RECORD_PAUSE, OnUpdateRecordPause)
	ON_UPDATE_COMMAND_UI(ID_RECORD_STOP, OnUpdateRecordStop)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()


CMainTool::CMainTool()
{
	m_lngRecordTime = 0;
	//// 音声制御インスタンスの生成
	//m_pAudio = CAudio::Instance();
	//// ビデオ操作クラスのインスタンス生成
	//m_pVideo = CVideo::Instance();
	m_pAudio = NULL;
	m_pVideo = NULL;
}

CMainTool::~CMainTool()
{
	m_lngRecordTime = 0;
	m_pAudio = NULL;
	m_pVideo = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CMainTool メッセージ ハンドラ

int CMainTool::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO :  ここに特定な作成コードを追加してください。

	return 0;
}

void CMainTool::OnDestroy() 
{
	CToolBar::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	/// プロパティの解放
	m_pFrame = NULL;
	m_pAccess = NULL;

	// 録音時間管理用タイマー破棄
	m_lngRecordTime = 0;
	KillTimer(ID_TIMER_MAINTOOL);
}

void CMainTool::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	if (nIDEvent == ID_TIMER_MAINTOOL) {	// (?)なぜかマウスがツールバー上にあるとイベントが発生してしまうため
		/// 録音時間表示
		_TCHAR strTmp[64];
		BOOL blRet = m_pAudio->IsRecordPauseSkype();
		if (!blRet) {	// 停止中はカウントしない
			++m_lngRecordTime;
			_stprintf_s(strTmp, sizeof(strTmp)/sizeof(strTmp[0]), _T("%02i:%02i"), (m_lngRecordTime/60), (m_lngRecordTime%60));
			m_wndStatic.SetText(strTmp);
		}
		/// 録音・録画MAX時間が経過した場合、STOPボタンを押す
		DWORD dwRecordLimit = 0;
		if (m_pVideo->IsRecording()) {
			CRegDWORD rgVRecordLimit(_T("Software\\Tapur\\Tapur\\Settings\\VRecordLimitTime"), 600, TRUE);	// 初期値:10min
			dwRecordLimit = rgVRecordLimit;
		} else {
			blRet = m_pAudio->IsRecordingSkype();
			if (blRet) {
				CRegDWORD rgRecordLimit(_T("Software\\Tapur\\Tapur\\Settings\\RecordLimitTime"), 3600, TRUE);		// 初期値:60min
				dwRecordLimit = rgRecordLimit;
			}
		}
		/// 通話が終了しているのにprgFinishedイベントが発生しない場合、終了処理コール
		if(m_pFrame->m_pAccessEvents->m_pCall != NULL) {
			if (m_pFrame->m_pAccessEvents->m_pCall->Status == prgFinished) {
				CTPerror te;
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("Force event:prgFinished"));
				//m_pFrame->m_pAccessEvents->CallStatusChanged(NULL, prgFinished);
				KillTimer(ID_TIMER_MAINTOOL);	// タイマー終了(重要！：これがないとタイマ処理がループしてしまう)
				m_pFrame->PushRecordStop();		// STOPボタンを押す
			}
		}

		if (dwRecordLimit > 0 && (long)dwRecordLimit <= m_lngRecordTime) {
			KillTimer(ID_TIMER_MAINTOOL);	// タイマー終了
			//// メッセージ表示
			///// STOPボタン上に表示
			CToolBarCtrl &tbc = GetToolBarCtrl();
			UINT idx = tbc.CommandToIndex(ID_RECORD_STOP);
			RECT rc;
			tbc.GetItemRect(idx, &rc);
			CPoint pnt((rc.left+rc.right)/2, (rc.top+rc.bottom)/2);
			CString strTitle;
			CString strMsg;
			if (m_pVideo->IsRecording()) {
				strTitle.LoadString(IDS_MSGBOX75);
				strMsg.LoadString(IDS_MSGBOX77);
			} else {
				strTitle.LoadString(IDS_MSGBOX74);
				strMsg.LoadString(IDS_MSGBOX76);
			}
			CBalloonHelp::LaunchBalloon(strTitle, strMsg, pnt, IDI_INFORMATION, 
				CBalloonHelp::unSHOW_CLOSE_BUTTON|CBalloonHelp::unSHOW_TOPMOST, 
				this, _T(""), 0);
			//// STOPボタンを押す
			m_pFrame->PushRecordStop();		
		}
	}

	CToolBar::OnTimer(nIDEvent);
}

// 録画ボタンの状態遷移
void CMainTool::OnUpdateVRecordStart(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください

	CToolBarCtrl &tbc = GetToolBarCtrl();
	if (m_pAudio == NULL || m_pVideo == NULL) {
		pCmdUI->Enable(FALSE);
		return;
	}
	
	/// 録音中（かつ留守録中ではない）は無効
	BOOL blRet = m_pAudio->IsRecordingSkype();
	if (blRet && !m_pVideo->IsRecording() && theApp.m_lngAppStatus == 1) {
		if (tbc.IsButtonEnabled(ID_VRECORD_START)) {
			pCmdUI->Enable(FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_VRECORD_START, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_VRECORD_START)) {
			pCmdUI->Enable(TRUE); 
			m_pFrame->GetMenu()->EnableMenuItem(ID_VRECORD_START, MF_ENABLED | MF_BYCOMMAND);
		}
	}
}

// 録音ボタンの状態遷移
void CMainTool::OnUpdateRecordStart(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください

	CToolBarCtrl &tbc = GetToolBarCtrl();
	if (m_pAudio == NULL || m_pVideo == NULL) {
		pCmdUI->Enable(FALSE);
		return;
	}
	
	/// 留守録中/録画中は無効
	if (m_pVideo->IsRecording() && theApp.m_lngAppStatus == 1) {
		if (tbc.IsButtonEnabled(ID_RECORD_START)) {
			pCmdUI->Enable(FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_START, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_RECORD_START)) {
			pCmdUI->Enable(TRUE); 
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_START, MF_ENABLED | MF_BYCOMMAND);
		}
	}
}

// 録音一時停止ボタンの状態遷移
void CMainTool::OnUpdateRecordPause(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください

	CToolBarCtrl &tbc = GetToolBarCtrl();
	if (m_pAudio == NULL || m_pVideo == NULL) {
		pCmdUI->Enable(FALSE);
		return;
	}
	
	/// 録音中のみ録音一時停止可能
	BOOL blRet = m_pAudio->IsRecordingSkype();
	if (!blRet || !theApp.m_lngAppStatus == 1) {
		if (tbc.IsButtonEnabled(ID_RECORD_PAUSE)) {
			pCmdUI->Enable(FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_PAUSE, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_RECORD_PAUSE)) {
			/// ボタンのアイコン設定
			if (m_pVideo->IsRecording()) {
				ReplaceIcon(7, IDI_T_VRECORD_PAUSE);	// ツールバーアイコンの変更
			} else {
				ReplaceIcon(7, IDI_T_RECORD_PAUSE);	// ツールバーアイコンの変更
			}
			/// ボタンの有効化
			pCmdUI->Enable(TRUE); 
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_PAUSE, MF_ENABLED | MF_BYCOMMAND);
		}
	}
}

// 録音終了ボタンの状態遷移
void CMainTool::OnUpdateRecordStop(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください

	CToolBarCtrl &tbc = GetToolBarCtrl();
	if (m_pAudio == NULL || m_pVideo == NULL) {
		pCmdUI->Enable(FALSE);
		return;
	}
	
	/// 録音中のみ録音終了可能
	BOOL blRet = m_pAudio->IsRecordingSkype();
	if (!blRet || !theApp.m_lngAppStatus == 1) {
		if (tbc.IsButtonEnabled(ID_RECORD_STOP)) {
			pCmdUI->Enable(FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_STOP, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_RECORD_STOP)) {
			/// ボタンのアイコン設定
			if (m_pVideo->IsRecording()) {
				ReplaceIcon(8, IDI_T_VRECORD_STOP);	// ツールバーアイコンの変更
			} else {
				ReplaceIcon(8, IDI_T_RECORD_STOP);	// ツールバーアイコンの変更
			}
			/// ボタンの有効化
			pCmdUI->Enable(TRUE); 
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_STOP, MF_ENABLED | MF_BYCOMMAND);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainTool 外部メソッド

//
//  機能     : 初期設定
//  
//  機能説明 : 先にCMainFrameが生成されている必要があるため、イベントでなく、CMainFrameからコール
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainTool::Init(
CWnd *pWnd)		// CMainFrameハンドルを指定
{
	// 音声制御インスタンスの生成
	m_pAudio = CAudio::Instance();
	/// ビデオ操作クラスのインスタンス生成
	m_pVideo = CVideo::Instance();

	/// プロパティの設定
	m_pFrame = (CMainFrame*)pWnd;
	m_pAccess = m_pFrame->m_pAccessEvents;

	/// ボタンアイコンの差し替え
	// 16bitアイコンを拡大表示するため、動的にロードする（ツールバーのビットマップは無視）
	ReplaceIcon(5, IDI_T_VRECORD);
	ReplaceIcon(6, IDI_T_RECORD);
	ReplaceIcon(7, IDI_T_RECORD_PAUSE);
	ReplaceIcon(8, IDI_T_RECORD_STOP);

	/// 録音対象選択ボタンの更新
	UpadateRecordMod();

	return TRUE;
}

/* EnableButton() はOnUpdateイベントでしか使用できない
BOOL CMainTool::UpdateToolBar() 
{
	CToolBarCtrl &tbc = GetToolBarCtrl();

	/// 録画ボタンの状態遷移
	//// 録画中は無効
	if (m_pAudio->IsRecording() && !m_pVideo->IsRecording()) {
		if (tbc.IsButtonEnabled(ID_VRECORD_START)) {
			tbc.EnableButton(ID_VRECORD_START, FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_VRECORD_START, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_VRECORD_START)) {
			tbc.EnableButton(ID_VRECORD_START, TRUE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_VRECORD_START, MF_ENABLED | MF_BYCOMMAND);
		}
	}

	/// 録音ボタンの状態遷移
	//// 録画中は無効
	if (m_pVideo->IsRecording()) {
		if (tbc.IsButtonEnabled(ID_RECORD_START)) {
			tbc.EnableButton(ID_RECORD_START, FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_START, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_RECORD_START)) {
			tbc.EnableButton(ID_RECORD_START, TRUE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_START, MF_ENABLED | MF_BYCOMMAND);
		}
	}

	/// 録音一時停止ボタンの状態遷移
	//// 録音中のみ録音一時停止可能
	if (!m_pAudio->IsRecordingVac() || !theApp.m_lngAppStatus == 1) {
		if (tbc.IsButtonEnabled(ID_RECORD_PAUSE)) {
			tbc.EnableButton(ID_RECORD_PAUSE, FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_PAUSE, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_RECORD_PAUSE)) {
			/// ボタンのアイコン設定
			if (m_pVideo->IsRecording()) {
				ReplaceIcon(7, IDI_T_VRECORD_PAUSE);	// ツールバーアイコンの変更
			} else {
				ReplaceIcon(7, IDI_T_RECORD_PAUSE);		// ツールバーアイコンの変更
			}
			/// ボタンの有効化
			tbc.EnableButton(ID_RECORD_PAUSE, TRUE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_PAUSE, MF_ENABLED | MF_BYCOMMAND);
		}
	}

	/// 録音終了ボタンの状態遷移
	//// 録音中のみ録音終了可能
	if (!m_pAudio->IsRecordingVac() || !theApp.m_lngAppStatus == 1) {
		if (tbc.IsButtonEnabled(ID_RECORD_STOP)) {
			tbc.EnableButton(ID_RECORD_STOP, FALSE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_STOP, MF_GRAYED | MF_BYCOMMAND);
		}
	} else {
		if (!tbc.IsButtonEnabled(ID_RECORD_STOP)) {
			/// ボタンのアイコン設定
			if (m_pVideo->IsRecording()) {
				ReplaceIcon(8, IDI_T_VRECORD_STOP);	// ツールバーアイコンの変更
			} else {
				ReplaceIcon(8, IDI_T_RECORD_STOP);	// ツールバーアイコンの変更
			}
			/// ボタンの有効化
			tbc.EnableButton(ID_RECORD_STOP, TRUE);
			m_pFrame->GetMenu()->EnableMenuItem(ID_RECORD_STOP, MF_ENABLED | MF_BYCOMMAND);
		}
	}

//	tbc.RedrawWindow();
//	tbc.InvalidateRect(NULL);					// ツールバーの表示をリフレッシュ

	return TRUE;
}
*/

//
//  機能     : 録音対象選択ボタンの更新
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainTool::UpadateRecordMod()
{
	/// 録音対象状態のレジストリ取得
	CRegDWORD rgRecMod(_T("Software\\Tapur\\Tapur\\Settings\\RecordMod"), 0, TRUE);
	DWORD dwRecMod = rgRecMod;

	/// Audioインスタンスへ設定反映
	m_pAudio->SetRecordSyncSw(dwRecMod);

 	/// 録音対象選択ボタンの更新
	switch (dwRecMod) {
    case 1:		// 受信音声のみ
		ReplaceIcon(10, IDI_T_RECORD_RCV);	// ツールバーアイコンの変更
		break;
    case 2:		// 送信音声のみ
		ReplaceIcon(10, IDI_T_RECORD_SND);	// ツールバーアイコンの変更
		break;
    case 0:		// 送受信音声
	default:
		ReplaceIcon(10, IDI_T_RECORD_BOTH);	// ツールバーアイコンの変更
		break;
	}

	return TRUE;
}

//
//  機能     : ビデオ録画開始イベント
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : CMainFrameでしかイベントが取得できないため、CMainFrameでこのメソッドをコール
//  
BOOL CMainTool::VRecordStart(
CString strDate,// 録音開始日時
BOOL blChat,	// チャットメッセージを送信するか
BOOL blSilent)	// サイレントモード
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString str;

	/// 留守番応答中は無効
	if (theApp.m_lngAppStatus == 2) {
		if (!blSilent) {
			str.LoadString(IDS_MSGBOX73);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		}
		return FALSE;
	}

	/// ビデオ受信中のみ有効
	if (!m_pAccess->IsVTalking()) {
		if (!blSilent) {
			str.LoadString(IDS_MSGBOX72);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		}
		return FALSE;
	}

	if (m_pVideo->IsRecording()) {
		if (m_pVideo->IsRecordPause()) {
			/// 録画一時停止の復帰
			m_pVideo->RecContinue();
		}
	} else {
	/// 録画開始
		CRegDWORD rgEnableRecWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVideoRecordWarning"), 1, TRUE);	// 初期値:1(True)
		CRegDWORD rgEnableRecStopWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVideoRecordStopWarning"), 1, TRUE);	// 初期値:1(True)
		//// 警告チャットの確認画面
		if (blChat && !blSilent) {
			if ((DWORD)rgEnableRecWarning == 1 || (DWORD)rgEnableRecStopWarning == 1) {
				CRegDWORD rgChk(_T("Software\\Tapur\\Tapur\\Settings\\EnableVideoRecordConfirm"), 1, TRUE);		// 初期値:1
				BOOL bl = (rgChk > 0) ? TRUE : FALSE;
				if (bl) {
					CString strTitle;
					CString strMsg;
					strTitle.LoadString(IDS_TMSGBOX_TITLE2);
					strMsg.LoadString(IDS_TMSGBOX_MSG2);
					CTMsgBox msgbox(NULL, strTitle, strMsg, TRUE);
					msgbox.Show(TRUE);
					if (!msgbox.GetResult()) {
						return FALSE;
					}
					if (msgbox.GetCheck()) {
						rgChk = 0;
					}
				}
			}
		}

		CString strSender = m_pAccess->GetSender();
		if (strSender.IsEmpty()) return FALSE;

		// 録音の開始（録画結合時にSocketInオブジェクトが必要なので、ここで開始してしまう）
		if (!RecordStart(strDate, FALSE, TRUE)) return FALSE;

		/// 圧縮形式の指定
		CRegDWORD rgVCmp(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordCompress"), 1, TRUE);		// 初期値:1(圧縮)
		BOOL bl = (rgVCmp > 0) ? TRUE : FALSE;
		m_pVideo->SetCompress(bl);
		CRegString rgVCmpName(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordCompressName"), _T("Lagarith lossless codec"), TRUE);		// 初期値:"Lagarith"
		CString strCmp = rgVCmpName;
		m_pVideo->SetCompressName(strCmp);
		/// 音声結合の指定
		CRegDWORD rgAudio(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordWithAudio"), 1, TRUE);	// 初期値:1(音声付録画)
		bl = (rgAudio > 0) ? TRUE : FALSE;
		if (bl) {
			///// 録音音質の指定（mono/stereo/natural stereo）
			CRegDWORD rgStereo(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordStereo"), 1, TRUE);		// 初期値:stereo(完全パン)
			int intVal = rgStereo;
			m_pAudio->SetRecordSyncStereo(intVal);
			WAVEFORMATEX Fmt = m_pAudio->pSocketIn->GetWaveFormat();		// 保存用WAVEフォーマット
			CPtrList *pList = m_pAudio->pSocketIn->GetOutListBuffer();		// 音声ストリーム用バッファ
			m_pVideo->SetAudio(&Fmt, pList);
		} else {
			m_pVideo->SetAudio(NULL, NULL);
		}
		/// 通話切断に備えて、応答コードを「録音中」に設定
		m_pAccess->m_ProcCode = _T("$$");	
		/// TMPファイルで録画開始
		if (m_pVideo->RecordOpen(strSender)) {
			CString strRecordFile = m_pAccess->m_Setup.RdfVideoFileName(strDate, m_pAccess->m_ProcCode);
			m_pVideo->Record(strRecordFile);
		}
		/// 相手先に警告チャット送付
		if (blChat) {
			if ((DWORD)rgEnableRecWarning == 1) {
				str.LoadString(IDS_TAB_VREC_WARNING);
				CRegString rgRecWarning = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordWarning"), str, TRUE);
				str = rgRecWarning;
				if (!str.IsEmpty()) m_pAccess->SendChat(str);
			}
		}
	}

	return TRUE;
}

//
//  機能     : 録音開始イベント
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : CMainFrameでしかイベントが取得できないため、CMainFrameでこのメソッドをコール
//  
BOOL CMainTool::RecordStart(
CString strDate,// 録音開始日時
BOOL blChat,	// チャットメッセージを送信するか
BOOL blSilent)	// サイレントモード
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString str;

	CToolBarCtrl &tbc = GetToolBarCtrl();

	/// 留守番応答中は無効
	if (theApp.m_lngAppStatus == 2) {
		if (!blSilent) {
			str.LoadString(IDS_MSGBOX73);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		}
		return FALSE;
	}

	/// 通話中のみ有効
	if (!m_pAccess->IsTalking(TRUE)) {
		if (!blSilent) {
			str.LoadString(IDS_MSGBOX70);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		}
		return FALSE;
	}

	BOOL blRet = m_pAudio->IsRecordingSkype();
	if (blRet) {
		if (m_pAudio->IsRecordPauseSkype()) {
			/// 録音一時停止の復帰
			m_pAudio->RecContinueSkype();
		}
	} else {
	/// 録音開始
		CRegDWORD rgEnableRecWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVoiceRecordWarning"), 1, TRUE);	// 初期値:1(True)
		CRegDWORD rgEnableRecStopWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVoiceRecordStopWarning"), 1, TRUE);	// 初期値:1(True)
		//// 警告チャットの確認画面
		if (blChat && !blSilent) {
			if ((DWORD)rgEnableRecWarning == 1 || (DWORD)rgEnableRecStopWarning == 1) {
				CRegDWORD rgChk(_T("Software\\Tapur\\Tapur\\Settings\\EnableVoiceRecordConfirm"), 1, TRUE);		// 初期値:1
				BOOL bl = (rgChk > 0) ? TRUE : FALSE;
				if (bl) {
					CString strTitle;
					CString strMsg;
					strTitle.LoadString(IDS_TMSGBOX_TITLE1);
					strMsg.LoadString(IDS_TMSGBOX_MSG1);
					CTMsgBox msgbox(NULL, strTitle, strMsg, TRUE);
					msgbox.Show(TRUE);
					if (!msgbox.GetResult()) {
						return FALSE;
					}
					if (msgbox.GetCheck()) {
						rgChk = 0;
					}
				}
			}
		}

		/// カウンタクリア
		m_lngRecordTime = 0;
		m_wndStatic.SetText(_T("00:00"));
		/// 通話切断に備えて、応答コードを「録音中」に設定
		m_pAccess->m_ProcCode = _T("$$");

		/// 受信音声をSkypeAPIで音声録音開始
		if (m_pAudio->RecordOpenSkype(m_pAccess->m_pCall)) {
			///// 録音音質の指定（mono/stereo/natural stereo）
			CRegDWORD rgStereo(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordStereo"), 1, TRUE);		// 初期値:stereo(完全パン)
			int intVal = rgStereo;
			m_pAudio->SetRecordSyncStereo(intVal);
			///// 録音対象の指定（受信音声/MIC音声/両方）
			CRegDWORD rgRecMod(_T("Software\\Tapur\\Tapur\\Settings\\RecordMod"), 0, TRUE);
			DWORD dwVal = rgRecMod;
			m_pAudio->SetRecordSyncSw(dwVal);
			///// 保存形式の指定（WAV/MP3）
			CRegDWORD rgFormatType(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordType"), 0, TRUE);		// 初期値:0(WAV)
			int intFormatType = rgFormatType;
			CRegString rgCodecName(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordMP3Codec"), _T(""), TRUE);
			CString strCodec = rgCodecName;
			CRegString rgFormatName(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordMP3Format"), _T(""), TRUE);
			CString strFormat = rgFormatName;
			if (m_pVideo->IsRecording()) {
				// 録画時はWAVに固定
				intFormatType = 0;
				// 録画用音声バッファの生成を指示
				 m_pAudio->pSocketIn->EnableOutBuffer(TRUE);
			} 
			m_pAudio->SetRecordFormat(intFormatType, strCodec, strFormat);
			///// 録音ファイル名
			CString strRecordFile;
			if (intFormatType > 0) {
				strRecordFile = m_pAccess->m_Setup.RdfMp3FileName(strDate, m_pAccess->m_ProcCode);
			} else {
				strRecordFile = m_pAccess->m_Setup.RdfWavFileName(strDate, m_pAccess->m_ProcCode);
			}
			///// 録音開始
			CRegDWORD rgSkypePortOut = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortOut"), 42805, TRUE);
			CRegDWORD rgSkypePortMic = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortMic"), 42806, TRUE);
			CString strPortOut, strPortMic;
			strPortOut.Format(_T("%d"), (DWORD)rgSkypePortOut);
			strPortMic.Format(_T("%d"), (DWORD)rgSkypePortMic);
			if (m_pAudio->RecordSkype(strRecordFile, strPortOut, strPortMic, m_pAccess->m_defOutDevice)) {
				//// 録音時間管理用タイマー開始
				SetTimer(ID_TIMER_MAINTOOL, 1000, NULL);	// 1 sec each
			} else {
				CTPerror te;
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("RecordStart error"));
				return FALSE;
			}
		}

		theApp.m_lngAppStatus = 1;		// 通話録音フラグ
		m_wndStatic.SetTextColor(RGB(0,0,0));	//black
		/// 相手先に警告チャット送付
		if (blChat) {
			if ((DWORD)rgEnableRecWarning == 1) {
				str.LoadString(IDS_TAB_REC_WARNING);
				CRegString rgRecWarning = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordWarning"), str, TRUE);
				str = rgRecWarning;
				if (!str.IsEmpty()) m_pAccess->SendChat(str);
			}
		}
	}

	return TRUE;
}

//
//  機能     : 録画一時開始・再開イベント
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : CMainFrameでしかイベントが取得できないため、CMainFrameでこのメソッドをコール
//  
BOOL CMainTool::VRecordPause() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	if (!m_pVideo->IsRecording()) return FALSE;

	if (m_pVideo->IsRecordPause()) {
		/// 録画一時停止の復帰
		m_pVideo->RecContinue();
	} else {
		/// 録画一時停止
		m_pVideo->RecPause();
	}

	return TRUE;
}

//
//  機能     : 録音一時開始・再開イベント
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : CMainFrameでしかイベントが取得できないため、CMainFrameでこのメソッドをコール
//  
BOOL CMainTool::RecordPause() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	if (!m_pAudio->IsRecordingSkype()) return FALSE;

	if (m_pAudio->IsRecordPauseSkype()) {
		/// 録音一時停止の復帰
		m_pAudio->RecContinueSkype();
	} else {
		/// 録音一時停止
		m_pAudio->RecPauseSkype();
	}

	return TRUE;
}

//
//  機能     : 録画終了イベント
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : CMainFrameでしかイベントが取得できないため、CMainFrameでこのメソッドをコール
//  
BOOL CMainTool::VRecordStop(
BOOL blChat)	// チャットメッセージを送信するか
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString str;

	if (!m_pVideo->IsRecording()) return FALSE;

	/// 録画完了
	// RecordStop()で録画保存も行う

	/// 相手先に警告チャット送付
	if (blChat) {
		CRegDWORD rgEnableRecWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVideoRecordStopWarning"), 1, TRUE);	// 初期値:1(True)
		if ((DWORD)rgEnableRecWarning == 1) {
			str.LoadString(IDS_TAB_VRECSTOP_WARNING);
			CRegString rgRecStopWarning = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordStopWarning"), str, TRUE);
			str = rgRecStopWarning;
			if (!str.IsEmpty()) m_pAccess->SendChat(str);
		}
	}

	return TRUE;
}

//
//  機能     : 録音終了イベント
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : CMainFrameでしかイベントが取得できないため、CMainFrameでこのメソッドをコール
//  
BOOL CMainTool::RecordStop(
BOOL blChat)	// チャットメッセージを送信するか
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString str;

	/// タイマー終了
	KillTimer(ID_TIMER_MAINTOOL);

	/// カウンタクリア
	m_lngRecordTime = 0;
	m_wndStatic.SetText(_T("00:00"));
	m_wndStatic.SetTextColor(RGB(153,153,153));	//gray

	BOOL blRet = m_pAudio->IsRecordingSkype();
	if (!blRet) return FALSE;

	/// 録音録画完了
	m_pAccess->FinishRecord();
	theApp.m_lngAppStatus = 0;	// APP状態フラグをリセット（FinishRecordで使用するので、その後）

	/// 応答コードをクリア
	m_pAccess->m_ProcCode.Empty();	
	/// 相手先に警告チャット送付
	if (blChat) {
		CRegDWORD rgEnableRecWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVoiceRecordStopWarning"), 1, TRUE);	// 初期値:1(True)
		if ((DWORD)rgEnableRecWarning == 1) {
			str.LoadString(IDS_TAB_RECSTOP_WARNING);
			CRegString rgRecStopWarning = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordStopWarning"), str, TRUE);
			str = rgRecStopWarning;
			if (!str.IsEmpty()) m_pAccess->SendChat(str);
		}
	}

	return TRUE;
}

//
//  機能     : 録音対象選択ボタンイベント
//  
//  機能説明 : ドロップダウンリストの表示
//  
//  返り値   : BOOL
//  
//  備考     : CMainFrameでしかイベントが取得できないため、CMainFrameでこのメソッドをコール
//  
void CMainTool::RecordMod() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	CWnd *pWnd = &(m_pFrame->m_wndToolBar);
	UINT nID = IDR_DROP_RECMOD;

	// load and display popup menu
	CNewMenu menu;
	menu.SetMenuDrawMode(CNewMenu::STYLE_XP);
	menu.LoadMenu(nID);
	menu.LoadToolBar(IDR_RECMOD_TOOL);
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup);

	CRect rc;
	pWnd->SendMessage(TB_GETRECT, ID_RECORD_MOD, (LPARAM)&rc);
	pWnd->ClientToScreen(&rc);

	pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		rc.left, rc.bottom, AfxGetMainWnd(), &rc);
}

//
//  機能     : TOOLBAR上にStaticTextを生成
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainTool::CreateStatic(
CLabel& txtStatic,	// CStatic参照
CString strText,	// 表示文字列
UINT nIndex,		// ツールバー上のindex
UINT nID,			// ツールバー上のボタンID
int nWidth)			// 文字列の表示幅
{
	// Create the static text
	SetButtonInfo(nIndex, nID, TBBS_SEPARATOR, nWidth);

	CRect rect;
	GetItemRect(nIndex, &rect);
	rect.top += 1;
	rect.bottom -= 1;

	if (!txtStatic.Create(strText, WS_VISIBLE, rect, this, nID))
	{
		TRACE(_T("Failed to create static window...\n"));
		return FALSE;
	}

	txtStatic.SetFontName(_T("System"));	// 他のフォントではサイズ指定できない
	txtStatic.SetTextColor(RGB(153,153,153));	//gray
	txtStatic.SetFontSize(18);
	txtStatic.SetSunken(TRUE);
	
	return TRUE;
} 

//
//  機能     : ツールバーアイコンの変更
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CMainTool::ReplaceIcon(
int intPos,		// 対象ボタン位置（0～）
WORD iconId)	// ICON ID
{
	CToolBarCtrl &tbc = GetToolBarCtrl();
	CImageList *pImageList = tbc.GetImageList();// 現在のツールバーのイメージ
	HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(iconId)); 
	pImageList->Replace(intPos, hIcon);			// ボタンイメージを変更
	tbc.SetImageList(pImageList);				// ツールバーのイメージを変更
	tbc.InvalidateRect(NULL);					// ツールバーの表示をリフレッシュ

	return TRUE;
}

