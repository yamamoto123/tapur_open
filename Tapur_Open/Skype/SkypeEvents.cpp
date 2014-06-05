// SkypeEvents.cpp

#include "stdafx.h"
#include <shlobj.h>		// SHGetSpecialFolderPath()
#include "Tapur.h"
#include "SkypeEvents.h"
#include "MsgTree.h"
#include "MsgList.h"
#include "FileSpec.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "TPutil.h"
#include "MainFrm.h"
#include "ProgressWnd.h"
#include "Tokenizer.h"
#include "BalloonHelp.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


CAccessEvents::CAccessEvents()
{
	SetMyParent(NULL);
	
	m_pCall == NULL;
	m_ProcCode.Empty();
	blSkypeVideoVersion = FALSE;
//	theApp.m_blSenderCall = FALSE;
	theApp.m_lngAppStatus = 0;

	///// ビデオ+音声結合クラスの初期化
	//for (int i = 0; i < sizeof(m_pAviManager)/sizeof(m_pAviManager[0]); i++) {
	//	m_pAviManager[i] = new CAviManager(AfxGetMainWnd());
	//}

	/// 音声操作クラスのインスタンス生成
	m_pAudio = CAudio::Instance();
	/// ビデオ操作クラスのインスタンス生成
	m_pVideo = CVideo::Instance();
}

CAccessEvents::~CAccessEvents()
{
	SetMyParent(NULL);	
}

// 通話時の応答処理
BOOL CAccessEvents::Response()
{
	CString str;
	_bstr_t bStr;
	CString strTmp;	
	CString strPlayFile;	// 再生メッセージファイル
	UST_RSF_RESP st_resp;
	UST_RSF_RESP *pst_resp = &st_resp;
	CFileSpec fs;
	long lngPlaySize = 0;	// 最大PLAY時間
	BOOL blRet;

	try {
		if (IsResponsing()) return TRUE;		// 現在応答中の場合
		SetResponsing(TRUE);					// 応答中フラグをON
		/// 応答処理が可能かチェック
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		if (pFrame->m_flgEnable == 0) goto Response_END;	// 応答無効の場合
		if (m_pCall == NULL) goto Response_END;
		if (m_CallStatus.blSenderCall) goto Response_END;		// ユーザ発信の場合
		//if (m_pCall->Status != prgInProgress) goto Response_END;
		if (m_ProcCode.IsEmpty()) goto Response_END;
TRACE(_T("m_ProcCode=%s\n"), m_ProcCode);
		theApp.m_lngAppStatus = 2;		// APP状態フラグを留守録にする
		//// 応答コードに対応した応答
		m_Setup.GetResponse(m_ProcCode, pst_resp);
		switch (pst_resp->type) {
		case RESP_TYPE_PLAY:	// 再生フェーズ
			if (pFrame->m_stPlayChk.blOnTimer) break;
			///// BEEP
			if (pst_resp->beep > 0) {
				if (!PlayBeep(pst_resp->beep)) {
					str.LoadString(IDS_MSGBOX61);
					te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
				}
			}
			if (pst_resp->play_type & RESP_PLAY_TYPE_CHAT) {
			///// チャットメッセージ
				strPlayFile = m_Setup.RsfChatFileName(m_ProcCode);
				fs.SetFullSpec(strPlayFile);
				if (fs.Exist()) {
					if (!fs.LoadString(&str)) {
						str.LoadString(IDS_MSGBOX102);
						te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
					}
					if (!str.IsEmpty()) SendChat(str, m_CallStatus.strPartnerId);
				}
			}
			/// ビデオが有効な場合
			if (IsSkypeVideo(m_CallStatus.strPartnerId)) {
				if (pst_resp->play_type & RESP_PLAY_TYPE_VIDEO) {
					///// ビデオ再生
					strPlayFile = m_Setup.RsfVideoFileName(m_ProcCode);
					fs.SetFullSpec(strPlayFile);
					if (fs.Exist()) {
						////// 仮想カメラへ指示(レジストリ経由)
						CRegDWORD rgMode(_T("Software\\Tapur\\VirtualCamera\\mode"), 0, TRUE);
						rgMode = 1;
						strTmp = m_Setup.RsfTmpFolder();
						if (strTmp.Right(1) == _T("\\")) strTmp.Delete(strTmp.GetLength() - 1);
						CRegString rgPath(_T("Software\\Tapur\\VirtualCamera\\frmpath"), _T(""), TRUE);
						rgPath = strTmp;
						///// 再生ファイルのfpsを取得
						m_pVideo->SetPlayFile(strPlayFile);
						int intFps = (int)m_pVideo->GetFps();
						if (intFps <= 0) {
							// 不明の場合デフォルト値
							CRegDWORD rgFps(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordFps"), 5, TRUE);	// 初期値:5(fps)
							intFps = (int)rgFps;	
						}
						CRegDWORD rgFps(_T("Software\\Tapur\\VirtualCamera\\fps"), 0, TRUE);
						rgFps = intFps;		// fps
						///// フレーム生成開始
						long lngTime = (long)(m_pVideo->GetPlaySize() + 1.0);		// 1sec余裕を持たせる
						if (lngTime > lngPlaySize) lngPlaySize = lngTime;
						m_pVideo->PlayOpenSkype(strTmp);
						if (!m_pVideo->PlaySkype(strPlayFile)) {
							str.LoadString(IDS_MSGBOX64);
							te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
						}
						////// SKYPEビデオデバイス変更
// イベント処理内ではフレームファイルが生成されないため、ここでコールしても無効
//						SetSkypeVDevice(TVC_IN_DEV);
//						RefreshSkypeVideo(m_CallStatus.lngCallId);
					}
				}
				if (pst_resp->play_type & RESP_PLAY_TYPE_IMAGE) {
					///// 静止画再生
					strPlayFile = m_Setup.FindRsfImgFile(m_ProcCode);
					fs.SetFullSpec(strPlayFile);
					if (fs.Exist()) {
						////// 仮想カメラへ指示(レジストリ経由)
						CRegDWORD rgMode(_T("Software\\Tapur\\VirtualCamera\\mode"), 0, TRUE);
						rgMode = 0;
						CRegString rgPath = CRegString(_T("Software\\Tapur\\VirtualCamera\\imgfile"), _T(""), TRUE);
						rgPath = strPlayFile;
						CRegDWORD rgFps(_T("Software\\Tapur\\VirtualCamera\\fps"), 0, TRUE);
						rgFps = 0;		// fps
						////// SKYPEビデオデバイス変更
						SetSkypeVDevice(TVC_IN_DEV);
						RefreshSkypeVideo(m_CallStatus.lngCallId);
					}
				}
			}
			if (pst_resp->play_type & RESP_PLAY_TYPE_AUDIO) {
			///// 音声再生
				strPlayFile = m_Setup.RsfAudioFileName(m_ProcCode);
				fs.SetFullSpec(strPlayFile);
				if (fs.Exist()) {
					m_pAudio->m_PlayFile = strPlayFile;
					long lngTime = (long)m_pAudio->GetPlaySize();
					if (lngTime > lngPlaySize) lngPlaySize = lngTime;
					m_pAudio->PlayOpen();				// モニター再生
					m_pAudio->Play();

					m_pAudio->PlayOpenSkype(m_pCall);	// SkypeAPI再生
					blRet = m_pAudio->PlaySkype();
					if (!blRet) {
						str.LoadString(IDS_MSGBOX61);
						te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
					}
				}
			}

			/// 応答コードを更新
			m_ProcCode += _T("$");		

			/// 応答メッセージ再生終了待ちタイマの起動
			pFrame->StartPlayChkTimer(lngPlaySize);
			break;
		case RESP_TYPE_RECORD:
		case RESP_TYPE_VRECORD:
			//// 既に録音中の場合何もしない
			if (pFrame->m_stRecordMax.blOnTimer) break;
			if (pst_resp->type == RESP_TYPE_VRECORD) {
				//// 4.0の場合、明示的にコマンドを送らないと表示されない場合がある(以下でも動作しない)
				if (m_pCall != NULL) {
					bStr = m_pCall->GetVideoStatus();
					str.Format(_T("%s"), (LPCTSTR)bStr);
					str.MakeUpper();
					if (str.Find(_T("VIDEO_RECEIVE_ENABLED")) >= 0 || str.Find(_T("VIDEO_BOTH_ENABLED")) >= 0) {
						m_pCall->StartVideoRecieve();
					}	
				}
				//// ビデオ録画がある場合はSkypeを表示する（最小化されている場合があるので）
				ShowVideoWin(m_CallStatus.strPartnerName, FALSE);
			}
			//// 再生中の音声・ビデオを停止
			if (m_pAudio->IsPlaying()) {
				m_pAudio->PlayStop();
			}
			if (m_pAudio->IsPlayingSkype()) {
				m_pAudio->PlayStopSkype();
			}
			if (m_pVideo->IsPlaying()) {
				m_pVideo->PlayStop();
			}
			if (m_pVideo->IsPlayingSkype()) {
				m_pVideo->PlayStopSkype();
			} 
			//// BEEP
			if (pst_resp->beep > 0) {
				if (!PlayBeep(pst_resp->beep)) {
					CString str;
					str.LoadString(IDS_MSGBOX61);
					te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
				}
			}
			// マイクをMUTE
			m_pAccess->Mute = -1;	// TRUE
			//// 録音/録画開始
			if (pst_resp->type == RESP_TYPE_VRECORD) {
				// 廃止（Ver1.00）：留守録中は自分のビデオはオフ
				// Skype3.5以降では自分のビデオは別画面のため。また、自分のビデオオフで画像サイズが変動するため
				//RefreshSkypeVideo(m_CallStatus.lngCallId, TRUE);// 留守録中は自分のビデオはオフ

				int intRet = StartRecord(TRUE);
			} else {
				StartRecord(FALSE);
			}
			break;
		case RESP_TYPE_DISCONNECT:
			///// BEEP
			if (pst_resp->beep > 0) {
				if (!PlayBeep(pst_resp->beep)) {
					str.LoadString(IDS_MSGBOX61);
					te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
				}
			}

			m_pCall->Status = prgFinished;		// 切断

			////// SKYPEビデオデバイスを戻す
			if (IsSkypeVideo()) SetSkypeVDevice(m_defVDevice);
			break;
		case RESP_TYPE_EXEC:
			////// 連携プログラムを実行
			ExecResp(pst_resp->exec_id);
			break;
		default:
			break;
		}
	}
    catch(CException *e)
    {
		m_ProcCode.Empty();		// 応答コードを初期化
		/// SKYPEビデオデバイスを戻す
		if (IsSkypeVideo()) SetSkypeVDevice(m_defVDevice);
		/// 切断
		//str.Format(_T("SET CALL %d STATUS FINISHED"), m_CallStatus.lngCallId);
		//m_pAccess->SendCommand(_bstr_t((LPCTSTR)str));
		m_pCall->Status = prgFinished;
		SetResponsing(FALSE);					// 応答中フラグをOFF
		// error message
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("unknown error:%s\n"), szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		e->Delete();
		return FALSE;
	}

	goto Response_END;

Response_END:
	SetResponsing(FALSE);					// 応答中フラグをOFF
	return TRUE;
}

// Skype API を実行するとネストしてResultイベントが発生しやすいので、なるべく実行しない方が良い
HRESULT CAccessEvents::Result(BSTR Description)
{
	HRESULT ret = S_OK;

	/// prgInProgressイベントが発生しない場合があるので、最初の応答処理の場合のみコール
	//if (m_ProcCode == _T("$")) {
	//	ret = Response() ? S_OK : S_FALSE;
	//	return ret;
	//}

	return ret;
}

HRESULT CAccessEvents::CallStatusChanged(ICallPtr pChangedCall, SkypeCallProgress Status)
{
	CString str;
	_bstr_t bStr;
	CRegDWORD rgWaitAnswer;
	DWORD dw;
	long id;
	BOOL blRet;

	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	//if (pFrame->m_flgEnable == 0) return S_OK;	// 応答無効の場合

	try {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("CallStatusChanged:%d"), Status);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
TRACE(_T("SkypeCallProgress:%d\n"), Status);
		switch (Status)
		{
		case prgUnknown:	// Unknown call status
			break;
		case prgUnplaced:	// Call was never placed
			break;
		case prgRouting:	// Call is currently being routed
			break;
		case prgEarlyMedia:	// Early media is being played
			break;
		case prgFailed:		// Call Failed
			break;
		case prgRinging:	// Ringing
			if (pFrame->m_flgEnable == 0) return S_OK;	// 応答無効の場合
			if (pChangedCall == NULL) break;
			/// 会話中or応答待ちの場合何もしない
			if (m_pCall != NULL) {
				if (IsTalking() || pFrame->m_stAnswerWait.blOnTimer) {
					break;
				} else {
					//// 何故かCallオブジェクトが残っている場合クリアする
					m_pCall == NULL;
				}
			}

			/// 現在のSkype Callクラス
			m_pCall = pChangedCall;
			m_CallStatus.SetCall(m_pCall);
			m_Setup.SetCallerId(m_CallStatus.strPartnerId);	// 送信者ID
			m_ProcCode.Empty();						// 応答コードを初期化
			m_pAudio->Reset();						// 音声オブジェクトの初期化
			m_pVideo->Reset();						// ビデオオブジェクトの初期化
			if (m_CallStatus.blSenderCall) break;	// ユーザ発信の場合
			SetResponsing(FALSE);					// 応答中フラグをOFF
			SetCallFinish(FALSE);					// 通話終了中フラグをOFF
			/// 送信者に対応した応答設定をロード
			if (LoadSetup()) {
				/// レジストリの初期値チェック
				rgWaitAnswer = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\WaitAnswerTime"), 10, TRUE);
				dw = rgWaitAnswer;
				if (dw == 0) {
					if (m_pAudio->IsPlaying()) {
						m_pAudio->PlayStop();
					} 
					if (m_pAudio->IsPlayingSkype()) {
						m_pAudio->PlayStopSkype();
					} 
					/// 電話に出る
					m_ProcCode = _T("$");		// 最初の応答
					pChangedCall->Status = prgInProgress;
				} else {
					/// 留守番応答待ちタイマの起動
					pFrame->StartAnswerWaitTimer();
				}
			}
			break;
		case prgInProgress:	// Call is in progress
			if (pFrame->m_flgEnable == 1 && !m_ProcCode.IsEmpty()) {
			// 留守番処理
				/// 留守番応答待ちタイマ処理の終了
				pFrame->StopAnswerWaitTimer();
				// 応答処理をコール
				if (!Response()) return S_FALSE;
			} else if (pFrame->m_flgAutoEnable == 1 && m_ProcCode.IsEmpty()) {
			// 自動録音処理
				pFrame->OnRecordStart();
			}
			break;
		case prgOnHold:		// Call is placed on hold
			if (pFrame->m_flgEnable == 0) return S_OK;	// 応答無効の場合
			// 留守番対応中の保留は終了扱いとする
			if (theApp.m_lngAppStatus == 2) {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("Abort hold call"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
				id = pChangedCall->GetID();
				if (id > 0) {
					pChangedCall->Status = prgFinished;
				}
			}
			break;
	/// 通話が終了している場合
		case prgFinished:	// Call is finished
		case prgMissed:		// Call was missed
			// Ring中にも発生する場合がある→コールが失われているので仕方ない（Ver1.2）
		case prgRefused:	// Call was refused
		case prgCancelled:	// Call was cancelled
			if (IsCallFinishing()) break;		// 現在通話終了中の場合
			SetCallFinish(TRUE);				// 通話終了中フラグをON
			if (pChangedCall != NULL) {		// NULL:強制終了
				/// 関係のないコール（処理中にかかってきた別のコール）の場合終了処理しない
				if ((m_pCall == NULL) ||
					(m_pCall != NULL && pFrame->m_stAnswerWait.blOnTimer)) {
					bStr = pChangedCall->PartnerHandle;
					str.Format(_T("%s"), (LPCTSTR)bStr);
					if (m_CallStatus.strPartnerId != str)  {
						SetCallFinish(FALSE);					// 通話終了中フラグをOFF
						break;
					}
				}
				/// 未だ会話中の場合何もしない（会議通話・通話時の着信などの場合）
				if (IsTalking()) {
					SetCallFinish(FALSE);					// 通話終了中フラグをOFF
					break;
				}
			}
			/// 留守番録音用タイマーの終了
			pFrame->StopAnswerWaitTimer();
			pFrame->StopRecordMaxTimer();
			pFrame->StopPlayChkTimer();

			if (m_pVideo->IsPlaying()) {
				m_pVideo->PlayStop();
			} 
			if (m_pVideo->IsPlayingSkype()) {
				m_pVideo->PlayStopSkype();
			} 
			if (m_pAudio->IsPlaying()) {
				m_pAudio->PlayStop();
			} 
			if (m_pAudio->IsPlayingSkype()) {
				m_pAudio->PlayStopSkype();
			} 
			//Ver1.00 ビデオ終了後の画面が残るので削除
			// Sleep(1000);	// イベントが早すぎる場合があるので、少し待つ
			if (theApp.m_lngAppStatus == 1) {
			/// 会話録音
				blRet = m_pAudio->IsRecordingSkype();
				if (blRet) {
					if (pFrame->m_wndToolBar.VRecordStop(TRUE)) {
						pFrame->m_wndToolBar.RecordStop(FALSE);
					} else {
						pFrame->m_wndToolBar.RecordStop(TRUE);
					}
				}
			} else if (theApp.m_lngAppStatus == 2 && m_ProcCode == _T("$$")) {
			/// 留守録音（録音してなくても通話記録は残す）
			// 制限時間の録音終了はタイマで行うので、ここでは実行しない
				/// 録音録画完了
				FinishRecord();
				//// マイクのMUTEを戻す（Skypeが自動で戻すが念のため）
				//m_pAccess->Mute = 0;	//FALSE
				/// 応答コードを更新
				m_ProcCode += _T("$");		
			}
			if (IsSkypeVideo()) SetSkypeVDevice(m_defVDevice);	// SKYPEビデオデバイスを戻す
			/// 留守番終了時に連携プログラムを実行
			if (theApp.m_lngAppStatus == 2) {
				UST_RSF_RESP st_resp;
				UST_RSF_RESP *pst_resp = &st_resp;
				//CString strCode = _T("$$$$");
				// 切断のタイミングで応答コードが変わってしまう（暫定的に両方チェック）
				m_Setup.GetResponse(m_ProcCode, pst_resp);
				if (pst_resp->exec_id < 1) {
					m_Setup.GetResponse(m_ProcCode + _T("$"), pst_resp);
				}
				ExecResp(pst_resp->exec_id);
			}
			m_pCall = NULL;			// 通話が終了（FinishRecord()が使用するので、この位置で）
			m_ProcCode.Empty();		// 応答コードを初期化
			theApp.m_lngAppStatus = 0;		// APP状態フラグをリセット
			SetResponsing(FALSE);			// 応答中フラグをOFF
			SetCallFinish(FALSE);			// 通話終了中フラグをOFF
			pFrame->SetFocus();				// TaskTrayの表示更新のためにフォーカスを移す
			break;
		case prgBusy:		// Destination is busy
			break;
		default:
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Undefined call status:%d"), Status);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			break;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		goto PROC_ERR;
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, err);
		goto PROC_ERR;
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("unknown error"));
		goto PROC_ERR;
	}

	return S_OK;

PROC_ERR:
	// 通話のエラー終了時処理（例外が発生するものは避ける）
	m_pCall = NULL;
	m_ProcCode.Empty();		// 応答コードを初期化
	theApp.m_lngAppStatus = 0;		// APP状態フラグをリセット
	SetResponsing(FALSE);			// 応答中フラグをOFF
	SetCallFinish(FALSE);			// 通話終了中フラグをOFF
	/// SKYPEビデオデバイスを戻す
	if (IsSkypeVideo()) SetSkypeVDevice(m_defVDevice);
	/// Skypeと接続切れしている可能性が高い→再接続
	pFrame->ConnectSkype();

	return S_FALSE;
}

HRESULT CAccessEvents::Command(BSTR SkypeCommand)
{

	return S_OK;
}

HRESULT CAccessEvents::UserStatusChanged(SkypeOnlineStatus Status)
{
	try {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("UserStatusChanged:%d"), Status);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		switch (Status)
		{
		case olsUnknown:	
			break;
		case olsOffline:	
			break;
		case olsOnline:
			/// （070218）オンラインなのにAPI接続通知が来ていなかった場合
			if (theApp.m_blSkypeConnect == FALSE) {
				APIStatusChanged(apiAttachSuccess);
			}
			break;
		case olsAway:	
			break;
		case olsNotAvail:	
			break;
		case olsDoNotDisturb:	
			break;
		case olsInvisible:	
			break;
		case olsSkypeMe:	
			break;
		case olsLoggedOut:	
			break;
		default:
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Undefined Online status:%d"), Status);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			break;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return S_FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, err);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CAccessEvents::MuteChanged(VARIANT_BOOL Mute)
{
	return S_OK;
}

HRESULT CAccessEvents::ConnectionStatusChanged(SkypeConnectionStatus Status)
{
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();

	try {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("ConnectionStatusChanged:%d"), Status);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		switch (Status)
		{
		case conUnknown:	
			break;
		case conOnline:		// Skype is online
			break;
		case conConnecting:	// Skype is connecting
			break;
		case conPausing:	// Skype's connectivity has been suspended  
			break;
		case conOffline:	// Skype is offline  
			break;
		case conLoggedOut:	// Skype's user has logged out
			/// 前回使用したユーザを使用
			InitDummyUser();
			theApp.m_blSkypeConnect = FALSE;		// 接続フラグ
			pFrame->m_flgEnable = 0;		// 応答処理フラグを無効
			pFrame->m_flgAutoEnable = 0;	// 自動録音処理フラグを無効
			/// 画面の更新（表示されている場合）
			pFrame->UpdateForm();
			break;
		default:
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Undefined Connection status:%d"), Status);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			break;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return S_FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, err);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CAccessEvents::MessageReceived(IMessagePtr pSkypeMessage)
{
	return S_OK;
}

HRESULT CAccessEvents::MessageSent(IMessagePtr pSkypeMessage)
{
	return S_OK;
}

HRESULT CAccessEvents::APIStatusChanged(SkypeAPIAttachmentStatus Status)
{
	int ret = 0;
	_bstr_t bStr;
	CString str, str1;
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRegDWORD rgDisable;
	DWORD dw;

	try {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("APIStatusChanged:%d"), Status);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		switch (Status)
		{
		case apiAttachUnknown:	
			break;
		case apiAttachSuccess:	
			if (theApp.m_blSkypeConnect == TRUE) break;		// 接続済み
			theApp.m_blSkypeConnect = TRUE;		// 接続フラグ
			/// スカイプ接続待ちタイマーの終了
			pFrame->StopConnectTimer();
			/// 古いログファイルの削除
			if (!te.DelOldLog()) {
				throw _T("Can't clean log files");
			}	
			/// Skypeデバイスの初期化
			if (!InitDevice()) {
				//// エラーの場合システム終了
				pFrame->DestroyWindow();
				return S_FALSE;
			}
			/// DBファイルの存在チェック→存在しない場合新規作成する
			if (!InitUserData()) {
				throw _T("Can't create user database");
			}
			/// データベースのバージョンアップ
			if (!VerUpUserData()) {
				throw _T("Can't version up user database");
			}	
			/// 応答処理フラグを設定
			//// レジストリの初期値チェック
			rgDisable = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\InitDisable"), 0, TRUE);
			dw = rgDisable;
			pFrame->m_flgEnable = (dw == 1) ? 0 : 1;
			/// 自動録音処理フラグを設定
			//// レジストリの初期値チェック
			rgDisable = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableAutoVoiceRecord"), 0, TRUE);
			dw = rgDisable;
			pFrame->m_flgAutoEnable = dw;
			/// バルーンメッセージの表示
			str1.LoadString(IDS_MSGBOX5);
			pFrame->m_TrayIcon.ShowBalloon(str1, str, NIIF_INFO, 10);	// ? 10 sec 以上必須
			/// ツールチップ更新
			pFrame->InitTooltip();	// ツールチップの初期化
			/// 画面の更新（表示されている場合）
			pFrame->UpdateForm();
			break;
		case apiAttachPendingAuth:	
			break;
		case apiAttachRefused:	
			/// 前回使用したユーザを使用
			InitDummyUser();
			theApp.m_blSkypeConnect = FALSE;		// 接続フラグ
			pFrame->m_flgEnable = 0;		// 応答処理フラグを無効
			pFrame->m_flgAutoEnable = 0;	// 自動録音処理フラグを無効
			/// バルーンメッセージの表示
			if (!pFrame->m_stConnect.blOnTimer) {
				//// スカイプ監視タイマー起動中は接続不可メッセージは出さない
				str.LoadString(IDS_MSGBOX0);
				str = _T("Tapur - ") + str;
				str1.LoadString(IDS_MSGBOX1);
				if (!pFrame->m_TrayIcon.ShowBalloon(str1, str, NIIF_INFO, 20)) {
					//// 旧Windows Shellでは表示されないため、メッセージボックスで代用
					str += _T("\n\n") + str1;
					AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
				}
				/// ツールチップ更新
				pFrame->m_TrayIcon.SetTooltipText(IDS_TOOLTIP_DISCON);
			}
			/// 画面の更新（表示されている場合）
			pFrame->UpdateForm();
			/// スカイプ接続待ちタイマの起動
			if (pFrame->m_stConnect.blOnTimer == FALSE) {
				pFrame->StartConnectTimer();
			}
			break;
		case apiAttachNotAvailable:	
			break;
		case apiAttachAPIAvail:	
			break;
		default:
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Undefined API status:%d"), Status);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			break;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return S_FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, err);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CAccessEvents::CallHistoryChanged(void)
{
	return S_OK;
}

HRESULT CAccessEvents::IMHistoryChanged(void)
{
	return S_OK;
}

HRESULT CAccessEvents::Error(LONG Number, BSTR Description)
{
	return S_OK;
}

HRESULT CAccessEvents::OnlineStatusChanged(IUserPtr pSkypeUser, SkypeOnlineStatus Status)
{

	try {
_stprintf_s(te.errstr, MAX_ERRSTR, _T("OnlineStatusChanged:%d"), Status);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		switch (Status)
		{
		case olsUnknown:	
			break;
		case olsOffline:	// PSTN
			break;
		case olsOnline:	
			break;
		case olsAway:	
			break;
		case olsNotAvail:
			break;
		case olsDoNotDisturb:	
			break;
		case olsInvisible:	
			break;
		case olsSkypeMe:	
			break;
		case olsLoggedOut:	
			break;
		default:
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Undefined Online status:%d"), Status);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			break;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return S_FALSE;   
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, err);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CAccessEvents::ChatMessageReceived(IChatMessagePtr pSkypeChatMessage)
{
	return S_OK;
}

HRESULT CAccessEvents::ChatMessageSent(IChatMessagePtr pSkypeChatMessage)
{
	return S_OK;
}

//
//  機能     : AccessEvents処理
//  
//  機能説明 : 
//  
//  返り値   : STDMETHODIMP
//  
//  備考     : Invokeにより渡されるイベントの引数DISPPARAMSの配列は逆向きに使用される（スタック形式？）
//  
STDMETHODIMP CAccessEvents::Invoke(DISPID dispid, REFIID riid, LCID lcid, 
	WORD wFlags, DISPPARAMS* pdispParams, VARIANT* pvarResult, 
	EXCEPINFO* pexcepinfo, UINT* puArgErr)
{
	if (riid != IID_NULL) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_UNKNOWNINTERFACE"));
		return DISP_E_UNKNOWNINTERFACE;
	}

	HRESULT hr;
	CComVariant varArg1;
	CComVariant varArg2;
	USES_CONVERSION;

	_bstr_t bstr1;
	_bstr_t bstr2;

	if (!(wFlags & DISPATCH_METHOD)) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_MEMBERNOTFOUND"));
		return DISP_E_MEMBERNOTFOUND;
	}

#ifdef _DEBUG
//_stprintf_s(te.errstr, MAX_ERRSTR, _T("Skype AccessEvents:%d"), dispid);
//te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
#endif
	switch (dispid)
	{
	case 1:	// Result
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_BSTR, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		bstr1 = _bstr_t(varArg1.bstrVal, true);
		varArg1.Clear();
		return Result(bstr1);
	case 2:	// CallStatusChanged
		if (pdispParams->cArgs < 2) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_I4, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		hr = varArg2.ChangeType(VT_DISPATCH, (&pdispParams->rgvarg[1]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return CallStatusChanged((ICall *)varArg2.pdispVal, (SkypeCallProgress)varArg1.lVal);
	case 3:	// Command
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_BSTR, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		bstr1 = _bstr_t(varArg1.bstrVal, true);
		varArg1.Clear();
		return Command(bstr1);
	case 4:	// UserStatusChanged
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_I4, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return UserStatusChanged((SkypeOnlineStatus)varArg1.lVal);
	case 5:	// MuteChanged
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_BOOL, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return MuteChanged(varArg1.boolVal);
	case 6:	// ConnectionStatusChanged
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_I4, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return ConnectionStatusChanged((SkypeConnectionStatus)varArg1.lVal);
	case 7:	// MessageReceived
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_DISPATCH, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return MessageReceived((IMessage *)varArg1.pdispVal);
	case 8:	// MessageSent
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_DISPATCH, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return MessageSent((IMessage *)varArg1.pdispVal);
	case 9:	// APIStatusChanged
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_I4, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return APIStatusChanged((SkypeAPIAttachmentStatus)varArg1.lVal);
	case 10:	// CallHistoryChanged
		return CallHistoryChanged();
	case 11:	// IMHistoryChanged
		return IMHistoryChanged();
	case 12:	// Error
		if (pdispParams->cArgs < 2) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_BSTR, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		hr = varArg2.ChangeType(VT_I4, (&pdispParams->rgvarg[1]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		bstr1 = _bstr_t(varArg1.bstrVal, true);
		varArg1.Clear();
		return Error(varArg2.lVal, bstr1);
	case 13:	// OnlineStatusChanged
		if (pdispParams->cArgs < 2) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_I4, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		hr = varArg2.ChangeType(VT_DISPATCH, (&pdispParams->rgvarg[1]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return OnlineStatusChanged((IUser *)varArg2.pdispVal, (SkypeOnlineStatus)varArg1.lVal);
	case 14:	// ChatMessageReceived
/* エラーが出る場合があるので無視
		if (pdispParams->cArgs < 2) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_BSTR, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		hr = varArg2.ChangeType(VT_I4, (&pdispParams->rgvarg[1]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return ChatMessageReceived((IChatMessage *)NULL);
*/
		return S_OK;
	case 15:	// ChatMessageSent
/* 使わないので無視
		if (pdispParams->cArgs < 1) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_BADPARAMCOUNT"));
			return DISP_E_BADPARAMCOUNT;
		}
		hr = varArg1.ChangeType(VT_DISPATCH, (&pdispParams->rgvarg[0]));
		if (FAILED(hr)) {
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("DISP_E_TYPEMISMATCH"));
			return DISP_E_TYPEMISMATCH;
		}
		return ChatMessageSent((IChatMessage *)varArg1.pdispVal);
*/
		return S_OK;
	default:
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Skype AccessEvents:DISP_E_MEMBERNOTFOUND:%d"), dispid);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return DISP_E_MEMBERNOTFOUND;
	}

	return S_OK;
}


//
//  機能     : BEEP音出力
//  
//  機能説明 : スピーカ・ライン同時再生
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAccessEvents::PlayBeep(
long lngSecond)			// 再生時間(未使用)
{
	BOOL blRet = FALSE;

	CTPsetup tps(theApp.m_strDataFolder);
	CString strFile = tps.RsfBeepFileName();
	CFileSpec fs(strFile);
	if (fs.Exist()) {
		m_pAudio->m_PlayFile = fs.GetFullSpec();
		m_pAudio->PlayOpen();				// モニター再生
		m_pAudio->Play();

		m_pAudio->PlayOpenSkype(m_pCall);	// SkypeAPI再生
		if (m_pAudio->PlaySkype(TRUE)) {	// full play now
			m_pAudio->PlayStopSkype();
			m_pAudio->PlayStop();
			return TRUE;
		}
		m_pAudio->PlayStopSkype();
		m_pAudio->PlayStop();
	}

	// エラー発生時にバックアップファイルでリトライ
	strFile = tps.RsfBeepFileName(_T("1"));
	fs.SetFullSpec(strFile);
	if (fs.Exist()) {
		m_pAudio->m_PlayFile = fs.GetFullSpec();
		m_pAudio->PlayOpen();				// モニター再生
		m_pAudio->Play();

		m_pAudio->PlayOpenSkype(m_pCall);	// SkypeAPI再生
		if (m_pAudio->PlaySkype(TRUE)) {	// full play now
			m_pAudio->PlayStopSkype();
			m_pAudio->PlayStop();
			return TRUE;
		}
		m_pAudio->PlayStopSkype();
		m_pAudio->PlayStop();
	}

	// エラー発生時にバックアップファイルでリトライ
	strFile = tps.RsfBeepFileName(_T("1"));
	fs.SetFullSpec(strFile);
	if (fs.Exist()) {
		m_pAudio->m_PlayFile = fs.GetFullSpec();
		m_pAudio->PlayOpen();				// モニター再生
		m_pAudio->Play();

		m_pAudio->PlayOpenSkype(m_pCall);	// SkypeAPI再生
		if (m_pAudio->PlaySkype(TRUE)) {	// full play now
			m_pAudio->PlayStopSkype();
			m_pAudio->PlayStop();
			return TRUE;
		}
		m_pAudio->PlayStopSkype();
		m_pAudio->PlayStop();
	}

	return blRet;
}

//
//  機能     : スカイプコール
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAccessEvents::SendCall(
CString strSender)		// SKYPE名
{
	CString str;

	/// Skypeの状態確認
	if (!theApp.m_blSkypeConnect) {
		str.LoadString(IDS_MSGBOX2);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}
	int intStatus = m_pAccess->GetConnectionStatus();
	if (intStatus != conOnline) {
		str.LoadString(IDS_MSGBOX15);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}
	ICallCollectionPtr pCallCol = m_pAccess->GetActiveCallList();
	if (pCallCol->GetCount() > 0) {
		str.LoadString(IDS_MSGBOX16);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	/// 確認
	str.LoadString(IDS_MSGBOX14);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;

	/// Skypeに発信
	_TCHAR _str[MAX_LEN];
	_tcscpy_s(_str, MAX_LEN, strSender);
//	theApp.m_blSenderCall = TRUE;	// ユーザ発信フラグをセット
// SkypeAPI 1.0.1.06 ではエラーとなるため
	_bstr_t bStr;
	//str.Empty();
	//str = str + _T("CALL ") + strSender;
	//bStr = _bstr_t((LPCTSTR)str);
	//m_pAccess->SendCommand(bStr);
	bStr = _bstr_t((LPCTSTR)strSender);
	ICallPtr pCall = m_pAccess->PlaceCall(bStr);
	
	return TRUE;
}

//
//  機能     : 通話中のユーザにチャット送信
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAccessEvents::SendChat(
CString strMsg,		// 送信メッセージ
CString strUserId)	// 送信先スカイプID、空の場合アクティブな全ユーザ対象
{
	long i = 0;
	long lngCnt = 0;
	_bstr_t bStrMsg;
	_bstr_t bStrHandle;
	ICallPtr pCall;
	IMessagePtr pMsg;
	_variant_t vt;

	bStrMsg = _bstr_t((LPCTSTR)strMsg);

	try {
		if (strUserId.IsEmpty()) {
			ICallCollectionPtr pCallCol = m_pAccess->GetActiveCallList();
			if (pCallCol == NULL) goto PROC_END;
			lngCnt = pCallCol->GetCount();
			if (lngCnt <= 0) goto PROC_END;
			for(i = 1; i <= lngCnt; i++) {
				vt = i;
				pCall = pCallCol->GetItem(vt);
				/// P2Pコールの場合のみ送信化
				if (!(pCall->Type == ctypIncomingP2P || pCall->Type == ctypOutgoingP2P)) continue;
				bStrHandle = pCall->PartnerHandle;
				pMsg = m_pAccess->SendMessageA(pCall->PartnerHandle, bStrMsg);
//				m_pAccess->SendMessage(pCall->PartnerHandle, bStrMsg);
			}
		} else {
			pMsg = m_pAccess->SendMessageA((LPCTSTR)strUserId, bStrMsg);
//			m_pAccess->SendMessage((LPCTSTR)strUserId, bStrMsg);
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("SendChat error:%d/%d"), i, lngCnt);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
PROC_END:
	return FALSE;
}

//
//  機能     : 通話中か否か
//  
//  機能説明 : 会議も考慮。１回線でも会話が継続していれば有効とする。
//  
//  返り値   : BOOL
//  
//  備考     : m_pAccess->GetActiveCallList()がエラーになりやすい？
//  
BOOL CAccessEvents::IsTalking(
BOOL blUpdate)	// 通話中のコールを見つけた場合、m_pCallに反映する
{
	long i = 0;
	long lngCnt = 0;
	_bstr_t bStr;
	CString strTmp;
	ICallPtr pCall;
	_variant_t vt;

//	if (m_pCall == NULL) return FALSE;

	try {
		ICallCollectionPtr pCallCol = m_pAccess->GetActiveCallList();
		if (pCallCol == NULL) goto PROC_END;
		lngCnt = pCallCol->GetCount();
		if (lngCnt <= 0) goto PROC_END;
		for(i = 1; i <= lngCnt; i++) {
			vt = i;
			pCall = pCallCol->GetItem(vt);
			if (pCall->Status == prgInProgress) {
				if (blUpdate) {
					m_pCall = pCall;
					m_CallStatus.SetCall(m_pCall);
					m_Setup.SetCallerId(m_CallStatus.strPartnerId);	// 送信者ID
				}
				return TRUE;
			}
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("IsTalking call error:%d/%d"), i, lngCnt);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

PROC_END:
	return FALSE;
}

//
//  機能     : ビデオ通話中か否か
//  
//  機能説明 : 受信映像がなければFALSEとする。
//  
//  返り値   : BOOL
//  
//  備考     : APIが不安定なため、ビデオウィンドウの存在で判定
//  
BOOL CAccessEvents::IsVTalking()
{
	CString str;
	_bstr_t bStr;
	_bstr_t bStrRet;

	/// 会話状態か確認
	if (!IsTalking(TRUE)) return FALSE;

	/// ビデオ受信状態か確認
	try {
		RECT rct;
		LPRECT pRect = &rct;
		if (m_pVideo->GetVideoWinLarge(pRect)) return TRUE;
		if (m_pVideo->GetVideoWinLocal(m_CallStatus.strPartnerName, pRect)) return TRUE;
		//str.Format(_T("GET CALL %d VIDEO_RECEIVE_STATUS"), m_CallStatus.lngCallId);
		//bStr = _bstr_t((LPCTSTR)str);
		//bStrRet = m_pAccess->SendBlockingCommand(bStr);
		//// ex) "CALL <id> VIDEO_RECEIVE_STATUS RUNNING"
		//str.Format(_T("%s"), (LPCTSTR)bStrRet);
		//if (str.Find(_T("RUNNING")) >= 0 || str.Find(_T("STARTING")) >= 0) {
		//	return TRUE;
		//}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("IsVTalking call error:%d"), m_CallStatus.lngCallId);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return FALSE;
}

//
//  機能     : 送信者に対応した応答設定をロード
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAccessEvents::LoadSetup()
{
	_bstr_t bStr;
	CString strTmp;

	if (m_pCall == NULL) return FALSE;

	/// 送信者に対応した応答設定をロード
	bStr = m_CallStatus.strPartnerId;			// 送信者名（スカイプ名）
	strTmp.Format(_T("%s"), (LPCTSTR)bStr);
	m_Setup.SetDataDir(theApp.m_strDataFolder);		// データフォルダ
	m_Setup.SetCallerId(strTmp);					// 送信者ID
	
	/// RSF設定のロード
	if (m_Setup.LoadRSF() == NULL) return FALSE;

	return TRUE;
}

//
//  機能     : 送信者の名称orID
//  
//  機能説明 : 名称がない場合ID
//  
//  返り値   : 
//  
//  備考     : 
//  
CString CAccessEvents::GetSender()
{
	_bstr_t bStr;
	CString strRet;

	strRet.Empty();
	if (m_pCall == NULL) return strRet;

	bStr = m_CallStatus.strPartnerName;			// 送信者名（表示名）
	strRet.Format(_T("%s"), (LPCTSTR)bStr);	
	if (strRet.IsEmpty()) {
		bStr = m_CallStatus.strPartnerId;			// 送信者名（スカイプ名）
		strRet.Format(_T("%s"), (LPCTSTR)bStr);
	}

	return strRet;
}

//
//  機能     : 録音・録画開始時の処理
//  
//  機能説明 : 
//  
//  返り値   : 1-audio only, 2-video only, 3-audio & video, 0-error
//  
//  備考     : 
//  
int CAccessEvents::StartRecord(BOOL blVideo)
{
	int intRet = 0;
	CString strRecordFile;
	CString strDate = m_Setup.GetDateStr();

	try {
		CString strSender = GetSender();
		if (strSender.IsEmpty()) return 0;

		m_Setup.SetCallerId(m_CallStatus.strPartnerId);

		if (blVideo) {
		//// ビデオ録画開始
			///// ビデオ受信状態か確認
			if (IsVTalking()) {
				///// 圧縮形式の指定
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
					//// 録音音質の指定（mono）
					m_pAudio->SetRecordSyncStereo(0);
					WAVEFORMATEX Fmt = m_pAudio->pSocketIn->GetWaveFormat();		// 保存用WAVEフォーマット
					CPtrList *pList = m_pAudio->pSocketIn->GetOutListBuffer();		// 音声ストリーム用バッファ
					m_pVideo->SetAudio(&Fmt, pList);
				} else {
					m_pVideo->SetAudio(NULL, NULL);
				}
				///// ビデオ録画開始
				if (m_pVideo->RecordOpen(strSender)) {
					strRecordFile = m_Setup.RdfVideoFileName(strDate, m_ProcCode);	// 録画ファイル
					if (m_pVideo->Record(strRecordFile)) {
						intRet = 2;
					}
				}
			}
		}

		/// SkypeAPIで音声録音開始
		if (m_pAudio->RecordOpenSkype(m_pCall)) {
			//// 留守電の時はStream error チェックは不要（短時間録音と混同しやすい）
			m_pAudio->EnableStreamErrSkype(FALSE);
			//// 録音音質の指定（mono）
			m_pAudio->SetRecordSyncStereo(0);
			//// 録音対象の指定（受信音声）
			m_pAudio->SetRecordSyncSw(1);
			//// 保存形式の指定（WAV/MP3）
			CRegDWORD rgFormatType(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordType"), 0, TRUE);		// 初期値:0(WAV)
			int intFormatType = rgFormatType;
			CRegString rgCodecName(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordMP3Codec"), _T(""), TRUE);
			CString strCodec = rgCodecName;
			CRegString rgFormatName(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordMP3Format"), _T(""), TRUE);
			CString strFormat = rgFormatName;
			if (m_pVideo->IsRecording()) {
				// 録画時はWAVに固定
				intFormatType = 0;
				// 録画用音声バッファの生成を指示
				m_pAudio->pSocketIn->EnableOutBuffer(TRUE);
			} 
			m_pAudio->SetRecordFormat(intFormatType, strCodec, strFormat);
			//// 録音ファイル名
			CString strRecordFile;
			if (intFormatType > 0) {
				strRecordFile = m_Setup.RdfMp3FileName(strDate, m_ProcCode);
			} else {
				strRecordFile = m_Setup.RdfWavFileName(strDate, m_ProcCode);
			}
			//// ポートの指定（使用しないMICポートも必要）
			CRegDWORD rgSkypePortOut = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortOut"), 42805, TRUE);
			CRegDWORD rgSkypePortMic = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortMic"), 42806, TRUE);
			CString strPortOut, strPortMic;
			strPortOut.Format(_T("%d"), (DWORD)rgSkypePortOut);
			strPortMic.Format(_T("%d"), (DWORD)rgSkypePortMic);
			if (m_pAudio->RecordSkype(strRecordFile, strPortOut, strPortMic, m_defOutDevice)) {
			//if (m_pAudio->RecordSkype(strRecordFile, m_defOutDevice)) {
				intRet += 1;
				/// 留守番録音MAX時間チェックタイマの起動
				CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
				pFrame->StartRecordMaxTimer();
			} else {
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Start Record error"));
				return FALSE;
			}
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Start Record error"));
		return FALSE;
	}

	return intRet;
}
//
//  機能     : 録音・録画完了時のテーブル保存
//  
//  機能説明 : 受信箱の更新も行なう
//  
//  返り値   : 
//  
//  備考     : サルベージされたデータの登録にも使用される
//  
BOOL CAccessEvents::FinishRecordTbl(
CString strAudioFile,	// 録音ファイル
CString strVideoFile,	// 録画ファイル
BOOL blRepair)			// サルベージの場合
{
	_bstr_t bStr;
	CString str;
	CString strTmp;
	CString strWhere;
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();

	try {
		/// ファイル名からレコード開始日時を算出
		CString strDate = m_Setup.GetAudioFileDate(strAudioFile);
		if (strDate.IsEmpty()) strDate = m_Setup.GetDateStr();
		/// RDFLテーブルと照合
		strWhere.Empty();
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		strWhere = strWhere + _T(" and ") + _T("lngSystemNew") + _T("=") + strDate;
		CTblRdfl tbl(theApp.m_strDBPath);
		if (tbl.CountRec(strWhere) > 0) return FALSE;	// 既に登録済み

		/// RDFLテーブルへ保存
		tbl.lngMsgType = (strVideoFile.IsEmpty()) ? RDF_TYPE_AUDIO : RDF_TYPE_VIDEO;
		//bStr = m_CallStatus.strPartnerId;		// m_Setup.CallerIdはデフォルト値の場合があるので使用不可
		//m_Setup.SetDataDir(theApp.m_strDataFolder);	// データフォルダ
		//m_Setup.SetCallerId(m_CallStatus.strPartnerId);	// 送信者ID
		if (strAudioFile.IsEmpty()) {
			// 録音されなかった場合
			tbl.strSenderId = m_CallStatus.strPartnerId;	// 送信者名（スカイプ名）
		} else {
 			tbl.strSenderId = m_Setup.GetAudioFileCallerId(strAudioFile);	// 送信者名（スカイプ名）
		}
		if (m_pCall != NULL) {		// サルベージでない場合
			bStr = m_CallStatus.strPartnerName;
			tbl.strSenderName = m_CallStatus.strPartnerName;// 送信者名（表示名）
			bStr = m_pCall->Subject;
			strTmp.Empty();
			if (bStr.length() > 0) {
				strTmp.Format(_T("%s"), (LPCTSTR)bStr);
			}
			tbl.strTitle = strTmp;						// CALL SUBJECT
			tbl.lngRespBaseId = m_Setup.GetRespId();
			tbl.lngClsCode1 = (long)m_pCall->Type + 1;		// コールタイプ
		}
		tbl.strReceiverId = theApp.m_strUsrId;		// 受信者名（スカイプ名）
		tbl.strReceiverName = theApp.m_strUsrName;	// 受信者名（表示名）
		if (theApp.m_lngAppStatus == 1) {
			tbl.lngRespType = RESPBASE_TYPE_NOTHING;	// 直接録音・録画
		} else if (theApp.m_lngAppStatus == 2) {
			tbl.lngRespType = RESPBASE_TYPE_SIMPLE;		// 留守番
		} else {
			if (blRepair) {
				tbl.lngRespType = RESPBASE_TYPE_NOTHING;	// サルベージされた場合
				tbl.strTitle.LoadString(IDS_REPAIR_NAME);	// CALL SUBJECT
			} else {
				return FALSE;	// イベント重複でコールされる場合がある（11.04.24）
			}
		}
		tbl.strRemarks1 = m_Setup.GetDataFolder();		// データパス
		tbl.InsertRec(strDate);

		/// ユーザテーブルへ保存
		CTblPerson ptbl(theApp.m_strDBPath);
		if (ptbl.ExistTbl()) {
			// テーブルが存在する場合のみ
			CString strWhere = _T("strId='") + m_CallStatus.strPartnerId + _T("'");
			long lngCnt = ptbl.SelectRec(strWhere);
			ptbl.strId = m_CallStatus.strPartnerId;			// 通話相手(スカイプID);
			ptbl.strName = m_CallStatus.strPartnerFullName;	// 通話相手(フル名);
			ptbl.strAbbName = m_CallStatus.strPartnerName;	// 通話相手(スカイプ名);
			ptbl.lngBirthday = m_CallStatus.lngBirthday;	// 誕生日;
			ptbl.lngSex = m_CallStatus.lngSex;	// 性別（usexUnknown = -1, usexNotSpecified = 0, usexMale = 1, usexFemale = 2)
			ptbl.strLanguage = m_CallStatus.strLanguage;	// 言語
			ptbl.strLanguageCode = m_CallStatus.strLanguageCode;// 言語コード
			ptbl.strCountry = m_CallStatus.strCountry;		// 住所（国）
			ptbl.strCountryCode = m_CallStatus.strCountryCode;	// 住所（国コード）
			ptbl.strProvince = m_CallStatus.strProvince;	// 住所（州/県）
			ptbl.strCity = m_CallStatus.strCity;			// 住所（市）
			ptbl.strTel = m_CallStatus.strPhoneHome;		// 電話（自宅）
			ptbl.strWkTel = m_CallStatus.strPhoneOffice;	// 電話（オフィス）
			ptbl.strMbTel = m_CallStatus.strPhoneMobile;	// 電話（モバイル）
			ptbl.strWeb = m_CallStatus.strHomepage;			// HP
			ptbl.strAbout = m_CallStatus.strAbout;			// 自己紹介
			ptbl.strMood = m_CallStatus.strMoodText;		// MoodText
			ptbl.lngTimeZone = m_CallStatus.lngTimezone;	// Timezone
			ptbl.lngSystemFlag = 0;		// 念のため削除フラグを設定
			if (lngCnt > 0) {
				// 更新
				ptbl.UpdateRec(ptbl.ID);
			} else {
				// 追加
				ptbl.InsertRec();
			}
		}

		/// 受信箱が表示されている場合、メッセージリストを更新
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CMsgTree *pfv = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
		if (pfv->m_hActive == pfv->m_hFolderDef) {
			CMsgList *pmv = (CMsgList *)pFrame->m_pMsgList->GetWnd();
			pmv->UpdateForm();
			//// 新規アイテムを選択
			pmv->m_wndList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		}

		/// 新着メッセージ数更新
		pFrame->IncNewMsgCnt(1);
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Record Table error"));
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 録音・録画完了時の処理
//  
//  機能説明 : テーブル保存
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAccessEvents::FinishRecord()
{
	int n = 0;
	_bstr_t bStr;
	CString str;
	CString strTmp;
	CString strAudioFile = _T("");
	CString strVideoFile = _T("");
	CString strRecordFile;
	CFileSpec fs;
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CWaitCursor();		// 砂時計

	if (m_pCall == NULL) return FALSE;

	try {
		/// 録画終了
		if (m_pVideo->IsRecording()) {
			strVideoFile = m_pVideo->GetRecFile();
			m_pVideo->RecStop();
		}
		//// ビデオファイルの生成待ちの場合があるため、リトライループする
		str.LoadString(IDS_MSGBOX79);
		str += " ";
		strTmp = str;
		n = 180;	// max 1.5min
		while(n--) {
			if(!m_pVideo->IsRecording()) break;
			if (n % 3 == 1) {
				pFrame->SetStatusText(str + _T("+"));
			} else if (n % 3 == 2) {
				pFrame->SetStatusText(str + _T("-"));
			} else {
				pFrame->SetStatusText(str + _T("*"));
				str += "*";
				if (str.Find(_T("********************")) > 0) {
					str = strTmp;	// メッセージを戻す
				}
			}
			Sleep(500); 
			AfxGetThread()->PumpMessage();
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't finish recording video."));
	}

	try {
		/// 録音終了
		if (m_pAudio->IsRecordingSkype()) {
			strAudioFile = m_pAudio->m_RecSkypeFile;
			m_pAudio->RecStopSkype();
		}
		//// 音声ファイルの生成待ちの場合があるため、リトライループする
		str.LoadString(IDS_MSGBOX78);
		str += " ";
		strTmp = str;
		n = 180;	// max 1.5min
		while(n--) {
			if(!m_pAudio->IsRecordingSkype()) break;
			if (n % 3 == 1) {
				pFrame->SetStatusText(str + _T("+"));
			} else if (n % 3 == 2) {
				pFrame->SetStatusText(str + _T("-"));
			} else {
				pFrame->SetStatusText(str + _T("*"));
				str += "*";
				if (str.Find(_T("********************")) > 0) {
					str = strTmp;	// メッセージを戻す
				}
			}
			Sleep(500);
			AfxGetThread()->PumpMessage();
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't finish recording voice."));
	}

	// 録音・録画完了時のテーブル保存
	FinishRecordTbl(strAudioFile, strVideoFile);

	//// ビデオ＋音声ファイル結合
	//ULONGLONG ullSz = 0;
	///// ファイルチェック
	//if (strVideoFile.IsEmpty() || strAudioFile.IsEmpty()) return TRUE;
	//fs.SetFullSpec(strVideoFile);
	//if (!fs.Exist()) return TRUE;
	//ullSz += fs.FileSize();
	//fs.SetFullSpec(strAudioFile);
	//if (!fs.Exist()) return TRUE;
	//CRegDWORD rgAudio(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordWithAudio"), 1, TRUE);	// 初期値:1(音声付録画)
	//if ((DWORD)rgAudio == 0) return TRUE;
	//ullSz += fs.FileSize();
	///// 一時ファイルチェック
	//if (fs.FreeDisk() < ullSz) {
	//	//// バルーンメッセージ表示
	//	///// STOPボタン上に表示
	//	CToolBarCtrl &tbc = pFrame->m_wndToolBar.GetToolBarCtrl();
	//	UINT idx = tbc.CommandToIndex(ID_RECORD_STOP);
	//	RECT rc;
	//	tbc.GetItemRect(idx, &rc);
	//	CPoint pnt((rc.left+rc.right)/2, (rc.top+rc.bottom)/2);
	//	CString strTitle;
	//	CString strMsg;
	//	strTitle.LoadString(IDS_MSGBOX131);
	//	strMsg.LoadString(IDS_MSGBOX95);
	//	CBalloonHelp::LaunchBalloon(strTitle, strMsg, pnt, IDI_INFORMATION, 
	//		CBalloonHelp::unSHOW_CLOSE_BUTTON|CBalloonHelp::unSHOW_TOPMOST, 
	//		&pFrame->m_wndToolBar, _T(""), 0);
	//	return TRUE;
	//}
	//CTPsetup tps(theApp.m_strDataFolder);
	//CString strTmpFile = tps.RdfTmpVideoFileName();
	//fs.SetFullSpec(strTmpFile);
	//fs.MkFolder();
	//// 結合処理(アンモーダルで実行)
	///// 未使用のビデオ+音声結合クラスを使用
	//for (int i = 0; i < sizeof(m_pAviManager)/sizeof(m_pAviManager[0]); i++) {
	//	if (m_pAviManager[i]->m_stApp.blOnTimer) continue;
	//	m_pAviManager[i]->SetMove(TRUE);
	//	m_pAviManager[i]->CopyAviFileDs(strTmpFile, strVideoFile, strAudioFile, FALSE);
	//	break;
	//}

	return TRUE;
}

//
//  機能     : 連携プログラムの実行
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAccessEvents::ExecResp(int intPrgId)
{
	if (intPrgId < 1) return FALSE;

	CString str, strPrgId;
	strPrgId.Format(_T("%d"), intPrgId);
	CString strFile;	// 連携プログラムCSVファイル
	if (m_Setup.GetRespId() == 1) {
		// デフォルト設定時
		m_Setup.SetCallerId(_T("0"));
		strFile = m_Setup.RsfPrgFileName(_T("$$$$"), strPrgId);
	} else {
		strFile = m_Setup.RsfPrgFileName(_T("$$$$"), strPrgId);
	}
	CFileSpec fs(strFile);
	if (!fs.Exist()) {
		AfxTrace(_T("%s\n"), strFile);
		str.LoadString(IDS_MSGBOX107);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
		return FALSE;
	}
	// 連携プログラムCSVの内容を取得
	CString strCsv;
	if (!fs.LoadString(&strCsv)) {
		AfxTrace(_T("%s\n"), strCsv);
		str.LoadString(IDS_MSGBOX107);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
		return FALSE;
	}

	CTokenizer tok(strCsv, _T(","));
	CString cs, strExeFile;
	CStringArray aryArg;
	int i = 0;
	BOOL blExe = TRUE;
	while(tok.Next(cs)) {
		//if (i == 0) 予約
		if (i == 1) {
			// 実行ファイル取得
			strExeFile = cs;
			if (strExeFile.Find(_T('\\')) < 0) {
				// パス未指定→テンプレートCSVフォルダを追加
				fs.SetFullSpec(FS_APPDIR);
				strExeFile = fs.GetFullSpec() + _T("bin\\") + strExeFile;
			}
			fs.SetFullSpec(strExeFile);
			if (!fs.Exist()) {
				str.LoadString(IDS_MSGBOX107);
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
				blExe = FALSE;
				return FALSE;
			}
		} else if (2 <= i) {
			// 引数取得
			if (cs.CompareNoCase(_T("$AudioFile")) == 0) {				// 録音ファイル
				strFile = m_pAudio->m_RecSkypeFile;
				fs.SetFullSpec(strFile);
				if (fs.Exist()) {
					aryArg.Add(strFile);
				} else {
					aryArg.Add(_T(""));
				}
			} else if (cs.CompareNoCase(_T("$VideoFile")) == 0) {			// 録画ファイル
				strFile = m_pVideo->GetRecFile();
				fs.SetFullSpec(strFile);
				if (fs.Exist()) {
					aryArg.Add(strFile);
				} else {
					aryArg.Add(_T(""));
				}
			} else if (cs.CompareNoCase(_T("$PartnerId")) == 0) {			// 通話相手(スカイプID)
					aryArg.Add(m_CallStatus.strPartnerId);
			} else if (cs.CompareNoCase(_T("$PartnerName")) == 0) {		// 通話相手(スカイプ名)
					aryArg.Add(m_CallStatus.strPartnerName);
			} else if (cs.CompareNoCase(_T("$UsrId")) == 0) {				// ユーザ(スカイプID)
					aryArg.Add(m_CallStatus.strUsrId);
			} else if (cs.CompareNoCase(_T("$UsrName")) == 0) {			// ユーザ(スカイプ名)
					aryArg.Add(m_CallStatus.strUsrName);
			} else if (cs.CompareNoCase(_T("$PartnerAddress")) == 0) {		// 通話相手住所
					str = m_CallStatus.strCountry;
					str += m_CallStatus.strProvince;
					str += m_CallStatus.strCity;
					aryArg.Add(str);
			} else if (cs.CompareNoCase(_T("$PartnerPhoneHome")) == 0) {	// 通話相手電話（自宅）
					aryArg.Add(m_CallStatus.strPhoneHome);
			} else if (cs.CompareNoCase(_T("$PartnerPhoneOffice")) == 0) {	// 通話相手電話（オフィス）
					aryArg.Add(m_CallStatus.strPhoneOffice);
			} else if (cs.CompareNoCase(_T("$PartnerPhoneMobile")) == 0) {	// 通話相手電話（モバイル）
					aryArg.Add(m_CallStatus.strPhoneMobile);
			} else {
				if (cs.Find(_T("(")) >= 0 && cs.Find(_T(")")) >= 0) {
					// 説明表示のままの場合
					aryArg.Add(_T(""));
				} else {
					aryArg.Add(cs);
				}
			}
		}
		i++;
	}

	if (blExe) {
		// 実行
		fs.SetFullSpec(strExeFile);
		strExeFile = _T("\"") + strExeFile + _T("\"");
		CString strPrm = _T("");
		for (i = 0; i < aryArg.GetCount(); i++) {
			if (i > 0) strPrm = strPrm + _T(" ");
			strPrm = strPrm + _T("\"") + aryArg.GetAt(i) + _T("\"");
		}
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, strExeFile + _T(" ") + strPrm);
		HINSTANCE ret;
		SetCurrentDirectory(fs.GetFolder());
		if (strExeFile.Right(5).CompareNoCase(_T(".bat\"")) == 0) {
			// batの実行
			ret = ShellExecute(NULL, _T("open"), strExeFile,  strPrm, fs.GetFolder(), SW_HIDE);
		} else {
			// exeの実行
			ret = ShellExecute(NULL, NULL, strExeFile,  strPrm, fs.GetFolder(), SW_SHOWNORMAL);
		}
		if ((int)ret <= 32) {
			str.LoadString(IDS_MSGBOX107);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, str);
			return FALSE;
		}
	}

	return TRUE;
}

//
//  機能     : Skypeデバイスの設定
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : Ver1.0以降はデバイス変更は不要
//  
//BOOL CAccessEvents::SetSkypeDeviceTry(
//CString strInDev,	// 入力デバイス
//CString strOutDev)	// 出力デバイス
//{
//	long i;
//	CString str;
//	_bstr_t bStr;
//	CString strTmpIn;
//	CString strTmpOut;
//	// GetAudioIn(), GetAudioOut()は結果のコピーを渡さないため、専用の変数で取得する
//	_bstr_t bStrInDev;
//	_bstr_t bStrOutDev;
//
//	try {
//		/// Skype入出力WAVEデバイスの確認
//		// Windows標準の場合は空文字列が返る
//		bStrInDev = m_pAccess->GetAudioIn();
//		strTmpIn.Format(_T("%s"), (LPCTSTR)bStrInDev);
//		bStrOutDev = m_pAccess->GetAudioOut();
//		strTmpOut.Format(_T("%s"), (LPCTSTR)bStrOutDev);
//
//		/// 既存のデバイス名と異なれば変更
//		if (strTmpIn.CompareNoCase(strInDev) != 0) {
//			bStr = _bstr_t((LPCTSTR)strInDev);
//			//// 設定が反映されない場合リトライする
//			for(i = 0; i < 10; i++) {
//				m_pAccess->PutAudioIn(bStr);
//				bStrInDev = m_pAccess->GetAudioIn();
//				strTmpIn.Format(_T("%s"), (LPCTSTR)bStrInDev);
//				if (strTmpIn.CompareNoCase(strInDev) == 0) break;
//			}
//		}
//		if (strTmpOut.CompareNoCase(strOutDev) != 0) {
//			bStr = _bstr_t((LPCTSTR)strOutDev);
//			//// 設定が反映されない場合リトライする
//			for(i = 0; i < 10; i++) {
//				m_pAccess->PutAudioOut(bStr);
//				bStrOutDev = m_pAccess->GetAudioOut();
//				strTmpOut.Format(_T("%s"), (LPCTSTR)bStrOutDev);
//				if (strTmpOut.CompareNoCase(strOutDev) == 0) break;
//			}
//		}
//	} catch(_com_error& e) {
//		if (e.ErrorInfo()) {
//			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
//		} else {
//			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
//		}
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
//		return FALSE;   
//	} catch(...) {
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't change Skype devices."));
//		return FALSE;
//	}
//
//	return TRUE;
//}

//
//  機能     : Skypeデバイスの設定
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : Ver1.0以降はデバイス変更は不要
//  
//BOOL CAccessEvents::SetSkypeDevice(
//CString strInDev,	// 入力デバイス
//CString strOutDev,	// 出力デバイス
//BOOL blSilence)		// エラーメッセージ抑制モード
//{
//	if (theApp.m_lngAppStatus == 0) return TRUE;
//
//	if (SetSkypeDeviceTry(strInDev, strOutDev)) return TRUE;
//
//	if (!blSilence) {
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't change Skype devices."));
//	}
//	return FALSE;
//}

//
//  機能     : Skypeビデオが有効なバージョンか？
//  
//  機能説明 : 有効なバージョンの場合のみTRUE
//  
//  返り値   : BOOL
//  
//  備考     : "GET SKYPEVERSION"メソッドは例外を起こしやすいので、コールは最小限にする
//  
BOOL CAccessEvents::IsSkypeVideoVersion()
{
	CString str;
	_bstr_t bStr;
	CString strRet;
	_bstr_t bStrRet;
	CString strCmp;

	try {
	/* Skype2.5からWin2000でもVIDEOが有効な場合がある
		//// Windows VERSION チェック
		if (!WinVersion.IsXPorLater()) return FALSE;
	*/
		//// SKYPE VERSION チェック
		//bStr = _bstr_t(_T("GET SKYPEVERSION"));
		//bStrRet = m_pAccess->SendBlockingCommand(bStr);
		//strRet.Format(_T("%s"), (LPCTSTR)bStrRet);
		//strCmp = _T("SKYPEVERSION ");
		//str = strRet.Mid(strCmp.GetLength(), 1);
		bStrRet = m_pAccess->GetVersion();
		strRet.Format(_T("%s"), (LPCTSTR)bStrRet);
		str = strRet.Left(1);
		int intVer = _ttoi((LPCTSTR)str);
		if (intVer < 2) {
			return FALSE;
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("IsSkypeVideoVersion error:%s"), strRet);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : Skypeビデオが使用可能か？
//  
//  機能説明 : 送受信の両者が使用可能な場合のみTRUE
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAccessEvents::IsSkypeVideo(
CString strUsr)		// 通話先ユーザ（Emptyの場合、自環境のみチェック）
{
	//CString str;
	//_bstr_t bStr;
	//CString strRet;
	//_bstr_t bStrRet;
	//CString strCmp;

	if (!blSkypeVideoVersion) return FALSE;

// 以下のチェックは自分については動作しない
	//str.Format(_T("GET USER %s IS_VIDEO_CAPABLE"), theApp.m_strUsrId);
	//bStr = _bstr_t((LPCTSTR)str);
	//bStrRet = m_pAccess->SendBlockingCommand(bStr);
	//strRet.Format(_T("%s"), (LPCTSTR)bStrRet);
	//str = strRet.Right(4);
	//str.MakeUpper();
	//if (str != _T("TRUE")) {
	//	return FALSE;
	//}

// コンタクトリストにない相手はチェックできない
	/// 相手の環境チェック
	//if (!strUsr.IsEmpty()) {
	//	str.Format(_T("GET USER %s IS_VIDEO_CAPABLE"), strUsr);
	//	bStr = _bstr_t((LPCTSTR)str);
	//	bStrRet = m_pAccess->SendBlockingCommand(bStr);
	//	strRet.Format(_T("%s"), (LPCTSTR)bStrRet);
	//	str = strRet.Right(4);
	//	str.MakeUpper();
	//	if (str != _T("TRUE")) {
	//		return FALSE;
	//	}
	//}

	return TRUE;
}

//
//  機能     : ビデオ表示をリフレッシュ
//  
//  機能説明 : ビデオ通話中の場合、変更を反映させるためにON/OFFが必要
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAccessEvents::RefreshSkypeVideo(
long lngCall,		// CALL ID
BOOL blStop)		// ビデオ送信を停止
{
	CString str;
	_bstr_t bStr;
	CString strRet;
	_bstr_t bStrRet;

	try {
		if (m_pCall == NULL) return FALSE;

		/// ビデオ送信が可能か？
		bStrRet = m_pCall->GetVideoStatus();
		strRet.Format(_T("%s"), (LPCTSTR)bStrRet);
		strRet.MakeUpper();
		if (strRet.Find(_T("VIDEO_SEND_ENABLED")) < 0 && strRet.Find(_T("VIDEO_BOTH_ENABLED")) < 0) {
			return FALSE;
		}	

		/// 一旦ビデオ送信を停止
		m_pCall->StopVideoSend();

		/// ビデオ送信を開始
		if (!blStop) {
			m_pCall->StartVideoSend();
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("RefreshSkypeVideo error:%s"), str);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : Skypeビデオデバイスの取得
//  
//  機能説明 : 
//  
//  返り値   : CString
//  
//  備考     : 
//
//CString CAccessEvents::GetSkypeVDevice()
//{
//	CString str;
//	_bstr_t bStr;
//	CString strTmpIn;
//	_bstr_t bStrInDev;
//
//	try {
//		// Windows標準の場合は空文字列が返る
//		bStr = _bstr_t(_T("GET VIDEO_IN"));
//		bStrInDev = m_pAccess->SendBlockingCommand(bStr);
//		if (bStrInDev.length() > 9) {		// more "VIDEO_IN "
//			strTmpIn.Format(_T("%s"), (LPCTSTR)bStrInDev);
//			strTmpIn.Delete(0, 9);	// cut "VIDEO_IN "
//		} else {
//			strTmpIn.Empty();
//		}
//	} catch(...) {
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't change Skype video devices."));
//		return _T("");
//	}
//
//	return strTmpIn;
//}

//
//  機能     : Skypeビデオデバイスの設定
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAccessEvents::SetSkypeVDeviceTry(
CString strDev)		// デバイス
{
	long i;
	CString str;
	_bstr_t bStr;
	CString strTmpIn;
	_bstr_t bStrInDev;

	try {
//		::Sleep(500);	// WAITが必要?

		/// Skype入力VIDEOデバイスの確認
		// Windows標準の場合は空文字列が返る
		bStrInDev = m_pAccess->GetVideoIn();
		strTmpIn.Format(_T("%s"), (LPCTSTR)bStrInDev);

		/// 既存のデバイス名と異なれば変更
		if (strTmpIn.CompareNoCase(strDev) != 0) {
			bStr = _bstr_t((LPCTSTR)strDev);
			//// 設定が反映されない場合リトライする
			for(i = 0; i < 10; i++) {
				m_pAccess->PutVideoIn(bStr);
				bStrInDev = m_pAccess->GetVideoIn();
				strTmpIn.Format(_T("%s"), (LPCTSTR)bStrInDev);
				if (strTmpIn.CompareNoCase(strDev) == 0) break;
			}
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("SetSkypeVDeviceTry error:%s"), str);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : Skypeビデオデバイスの設定
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAccessEvents::SetSkypeVDevice(
CString strDev,		// 入力デバイス
BOOL blSilence)		// エラーメッセージ抑制モード
{
	BOOL blRet = FALSE;

	/// リトライ付きでデバイス変更
//	int i;
//	for(i = 0; i < 10; i++) {
		if (SetSkypeVDeviceTry(strDev)) {
			blRet = TRUE;
//			break;
		}
//	}

	if (!blSilence && !blRet) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't change Skype video devices."));
		return blRet;
	}

	return blRet;
}

//
//  機能     : Tapurデバイスの設定
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAccessEvents::SetAppDevice(
CString strInDev,	// 入力デバイス
CString strOutDev)	// 出力デバイス
{
	m_pAudio->SetInDevice(strInDev);
	m_pAudio->SetOutDevice(strOutDev);

	return TRUE;
}

//
//  機能     : Skypeデバイスの初期化
//  
//  機能説明 : 既存デバイスのバックアップ
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAccessEvents::InitDevice(void)
{
	CString str;
	_bstr_t bStr;

	try {
		// 13/07/14 オフライン時にConnectionStatusが取得できないため廃止（APIStatusChanged:0で接続確認とする）
		/// Skypeとの接続状態チェック
		//int intStatus = m_pAccess->GetConnectionStatus();
		//if (!(intStatus == conConnecting || intStatus == conOnline)) {
		//	str.LoadString(IDS_MSGBOX7);
		//	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		//	AfxThrowUserException();
		//}

		/// PROTOCOL 指定
		m_pAccess->Protocol = 6;	// skype3.0～
//		m_pAccess->Protocol = 5;	// skype1.4～

		/// 実際に使用可能なPROTOCOLの取得
		m_lngProtocol = m_pAccess->Protocol;
		bStr = m_pAccess->Version;
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Skype Ver:%s Protocol:%d"), (LPCTSTR)bStr, m_lngProtocol);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

		//// ユーザ名（スカイプ名）の取得
		IUserPtr pUser;
		bStr = m_pAccess->CurrentUserHandle;
		if ( bStr.length() == 0 ) {
			str.LoadString(IDS_MSGBOX8);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			AfxThrowUserException( );
		}
		theApp.m_strUsrId.Format(_T("%s"), (LPCTSTR)bStr);
		pUser = m_pAccess->GetUser(bStr);
		bStr = pUser->FullName;
		if ( bStr.length() > 0 ) {
			theApp.m_strUsrName.Format(_T("%s"), (LPCTSTR)bStr);
		}

		/// Skype入出力WAVEデバイスの特定
		// Windows標準の場合は空文字列が返る
		bStr = m_pAccess->GetAudioIn();
		m_defInDevice.Format(_T("%s"), (LPCTSTR)bStr);
		bStr = m_pAccess->GetAudioOut();
		m_defOutDevice.Format(_T("%s"), (LPCTSTR)bStr);

		/// Skype入出力デバイスが"Tapur Virtual Cable"の場合、デフォルトに変更
		//if (m_defInDevice.Left(5).CompareNoCase(_T("Tapur")) == 0) {
		//	bStr = _bstr_t("");
		//	m_pAccess->PutAudioIn(bStr);
		//	m_defInDevice.Empty();
		//}
		//if (m_defOutDevice.Left(5).CompareNoCase(_T("Tapur")) == 0) {
		//	bStr = _bstr_t("");
		//	m_pAccess->PutAudioOut(bStr);
		//	m_defOutDevice.Empty();
		//}

		// アプリのデバイスを設定
		m_pAudio->SetInDevice(m_defInDevice);
		m_pAudio->SetOutDevice(m_defOutDevice);

		/// サウンドデバイスのフォーマット特定
		m_pAudio->FindFormat(FALSE);

		/// ビデオが有効な場合
		if (IsSkypeVideoVersion()) {
			blSkypeVideoVersion = TRUE;

			/// Skypeビデオデバイスの特定
			// Windows標準の場合は空文字列が返る
			bStr = m_pAccess->GetVideoIn();
			m_defVDevice.Format(_T("%s"), (LPCTSTR)bStr);
			//m_defVDevice = GetSkypeVDevice();

			/// Skypeビデオデバイスが"Tapur Virtual Camera"の場合、デフォルトに変更
			if (m_defVDevice.GetLength() < 5) return TRUE;
			if (m_defVDevice.Left(5).CompareNoCase(_T("Tapur")) == 0) {
				bStr = _bstr_t("");
				m_pAccess->PutVideoIn(bStr);
				m_defVDevice.Empty();
			} else {
// 廃止（Ver1.05）:起動時はデバイス変更がエラーになりやすいため
				///// SKYPEビデオデバイス変更テスト
				//// インストール直後はSkypeがデバイスを認識しない場合があるため
				//if (!SetSkypeVDevice(TVC_IN_DEV, TRUE)) {
				//	CString strErr;
				//	strErr.LoadString(IDS_MSGBOX7);
				//	AfxMessageBox(strErr, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
				//	AfxThrowUserException();
				//}
				//SetSkypeVDevice(m_defVDevice);
			}
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
//		return FALSE;   
		return TRUE;	// Skype接続がOKなら、API関連エラーは見逃す（Ver1.2） 
    } catch( CUserException* e ) {
		e->Delete();
        return FALSE;   
	} catch( CException* e ) {
		CString strErr;
		_TCHAR   szCause[MAX_LEN];
 		e->GetErrorMessage(szCause, MAX_LEN);
		strErr.Format(_T("Init Skype device error:%s\n"), szCause);
		AfxMessageBox(strErr);
		e->Delete();
	}

	return TRUE;
}


//
//  機能     : ユーザ単位のデータベース初期生成
//  
//  機能説明 : フォルダ生成、初期データコピー
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAccessEvents::InitUserData(void)
{
	int ret = 0;
	_bstr_t bStr;
	CFileSpec fs;
	CFileSpec fstmp;
	_TCHAR strFolder[MAX_PATH];	
	CString strPath;	
	CString str;

	try {
	/// 現在のユーザフォルダとデータの検出
		//// Application Dataフォルダの取得（ex, C:\Documents and Settings\paak\Application Data)
		ret = ::SHGetSpecialFolderPath(NULL, strFolder, CSIDL_APPDATA, FALSE);
		if ( !ret ) {
			throw _T("can't find data folder.");
		}
		theApp.m_strDataFolder = strFolder;
		theApp.m_strDataFolder += _T(TAPUR_DATA_PATH) + ChkFolder(theApp.m_strUsrId);
		//theApp.m_strDataFolder += _T(TAPUR_DATA_PATH) + theApp.m_strUsrId;
		theApp.m_strDBPath = theApp.m_strDataFolder + _T("\\") + _T(DBFILE_NAME);	// DBファイル名の特定
		m_Setup.SetDataDir(theApp.m_strDataFolder);	// データフォルダ

	/// DBファイルの存在チェック→存在しない場合新規作成する
		fs.SetFullSpec(theApp.m_strDBPath);
		if (!fs.Exist()) {
			//// ユーザ名が存在しない場合、新規にデータベースを作成（コピー）する
			///// ユーザフォルダの作成
			if (!fs.MkFolder()) {
				throw _T("can't create user data folder.");
			}
			///// DBファイルのコピー
			fs.SetFullSpec(FS_APPDIR);
			fs.SetFileName(_T("init.db"));
			if (!fs.FileCopy(theApp.m_strDBPath, FALSE)) {
				throw _T("can't create user data.");
			}
		}

	/// 初期応答データの存在チェック→存在しない場合作成（コピー）する
		CTPsetup tps(theApp.m_strDataFolder, _T("0"));
		//// 初期応答音声ファイル作成
		strPath = tps.RsfAudioFileName(_T("$"));
		fs.SetFullSpec(strPath);
		if (!fs.Exist()) {
			///// 初期応答音声ファイルのコピー
			fs.SetFullSpec(FS_APPDIR);
			fs.SetFileName(_T("greeting.wav"));
			if (!fs.FileCopy(strPath, TRUE)) {
				throw _T("can't create initial response audio file.");
			}
		}
		//// 初期BEEP音声ファイルのコピー
		strPath = tps.RsfBeepFileName();
		fs.SetFullSpec(strPath);
		if (!fs.Exist()) {
			fs.SetFullSpec(FS_APPDIR);
			fs.SetFileName(_T("beep.wav"));
			if (!fs.FileCopy(strPath, TRUE)) {
				throw _T("can't create initial beep audio file.");
			}
		}
		strPath = tps.RsfBeepFileName(_T("1"));
		fstmp.SetFullSpec(strPath);
		if (!fstmp.Exist()) {
			if (!fs.FileCopy(strPath, TRUE)) {
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, 
					_T("can't create initial beep audio file1."));
			}
		}
		strPath = tps.RsfBeepFileName(_T("2"));
		fstmp.SetFullSpec(strPath);
		if (!fstmp.Exist()) {
			if (!fs.FileCopy(strPath, TRUE)) {
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, 
					_T("can't create initial beep audio file2."));
			}
		}
		//// 初期応答ファイル作成
		strPath = tps.RsfFileName();
		fs.SetFullSpec(strPath);
		if (!fs.Exist()) {
			if (!tps.SaveRsfFile()){
				throw _T("can't create initial response file.");
			}
		}
		//// 初期応答設定
		CString strWhere;
		CTblRsfl tbl(theApp.m_strDBPath);
		strWhere.Empty();
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		strWhere = strWhere + _T(" and ") + _T("lngDefaultFlag=1");
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
		if (tbl.CountRec(strWhere) == 0) {
			//// 入力値
			tbl.strReceiverId = theApp.m_strUsrId;	// 受信者名（スカイプ名）
			tbl.strReceiverName = theApp.m_strUsrName;	// 受信者名（表示名）
			tbl.lngRespType = RESPBASE_TYPE_SIMPLE;
			tbl.lngDefaultFlag = 1;
			//// insert
			tbl.InsertRec();
		}
	} catch(LPTSTR err) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, err);
		return FALSE;
	}
		
	return TRUE;
}

//
//  機能     : SKYPEに接続できない時の表示ユーザの特定
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAccessEvents::InitDummyUser(void)
{
	_TCHAR tstrFolder[MAX_PATH];	
	CString str, strtmp;

	/// Application Dataフォルダ内の最新のユーザを使用
	int ret = ::SHGetSpecialFolderPath(NULL, tstrFolder, CSIDL_APPDATA, FALSE);
	if ( !ret ) return FALSE;
	CString strFolder = tstrFolder;
	strFolder += _T(TAPUR_DATA_PATH);
	//// 最も日付の新しい下位フォルダの検索
	if (!SetCurrentDirectory(strFolder)) return FALSE;
	CFileFind finder;
	CTime tm, btm(2000, 1, 1, 0, 0, 0);
	BOOL bWorking = finder.FindFile(_T("*"));
	if (!bWorking) return FALSE;
	str.Empty();
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory()) {
			strtmp = finder.GetFileName();
			if (strtmp.Left(1) == _T(".")) continue;
			finder.GetLastWriteTime(tm);
			if (tm > btm) {
				str = strtmp;
				btm = tm;
			}
		}
	}
	if (str.IsEmpty()) return FALSE;
	theApp.m_strUsrId = str;
	theApp.m_strUsrName.Empty();

	theApp.m_strDataFolder = strFolder;
	theApp.m_strDataFolder += ChkFolder(theApp.m_strUsrId);
	//theApp.m_strDataFolder += theApp.m_strUsrId;
	theApp.m_strDBPath = theApp.m_strDataFolder + _T("\\") + _T(DBFILE_NAME);	// DBファイル名の特定
	m_Setup.SetDataDir(theApp.m_strDataFolder);	// データフォルダ
		
	return TRUE;
}

//
//  機能     : TAB内のビデオ画面をウィンドウ表示にする
//  
//  機能説明 : 
//  
//  返り値   : 1:TAB->ウィンドウ表示成功、2:既にウィンドウ表示
//           : 0:TAB画面（ビデオ画面）なし、-1:TAB->ウィンドウ表示失敗
//  
//  備考     : 
//
int CAccessEvents::ShowVideoWin(CString strSkypeId, BOOL blLarge)
{
	RECT rct;
	LPRECT pRect = &rct;
	CPoint point;

	/// 現在ウィンドウ表示か確認
	if (m_pVideo->GetVideoWinLarge(pRect)) return 2;

	/// SkypeWindowを表示する
	_bstr_t bStr = _bstr_t(_T("FOCUS"));
	m_pAccess->SendCommand(bStr);

	/// 通話TABを表示する
	if (!FocusCallTab(strSkypeId)) return 0;

	if (!blLarge) return 0;

	/// TAB内のビデオ画面位置を取得
	if (!m_pVideo->GetVideoWinLocal(strSkypeId, pRect)) return 0;

	/// 前面表示
	HWND hwnd = ::FindWindow(_T("tSkMainForm.UnicodeClass"), NULL); if (hwnd == NULL) return 0;
	::SetForegroundWindow(hwnd);
	Sleep(100);
	/// 擬似的にマウスを押下してウィンドウ表示にする
//	point.x = pRect->right - 40;
//	point.y = pRect->top + 24;
	point.x = pRect->right - 110;
	point.y = pRect->top + 30;
	::SetCursorPos(point.x, point.y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	/// ウィンドウサイズ（録画サイズ）を規定
	// ウィンドウが大きすぎるとノイズが入りやすい
	Sleep(400);		// ウィンドウの表示待ち
	pRect->top = VECHO_POSY;	// エラーメッセージ（画面中央）の表示位置を避ける
	pRect->left = VECHO_POSX;
	pRect->right = pRect->left + VECHO_SZX;
	pRect->bottom = pRect->top + VECHO_SZY;
	if (!m_pVideo->SetVideoWinLarge(pRect)) {
		// マウスイベントが無視される場合があるので、リトライ
		::SetCursorPos(point.x, point.y);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		Sleep(400);		// ウィンドウの表示待ち
		if (!m_pVideo->SetVideoWinLarge(pRect)) return -1;
	}

	return 1;
}

//
//  機能     : 通話TABを表示する
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAccessEvents::FocusCallTab(
CString strSkypeId)		// ユーザ毎のTABの見分けが付かないため、現在は無効
{
	RECT rct;
	LPRECT pRect = &rct;
	CPoint point;
	HWND hwnd;
	HWND hVwnd;

	/// Call Tab
	hVwnd = FindTab(_T("tMultiCallPanel"));
	if (hVwnd == NULL) return FALSE;
	if (::IsWindowVisible(hVwnd)) return TRUE;	// 既に表示されている

	/// 現在の表示TAB
	int intNext = 0;
	hwnd = FindTab(_T("TMainUserList")); if (hwnd == NULL) return FALSE;
	if (::IsWindowVisible(hwnd)) intNext = 2;
	hwnd = FindTab(_T("THistoryList")); if (hwnd == NULL) return FALSE;
	if (::IsWindowVisible(hwnd)) intNext = 1;

	/// 前面表示
	hwnd = ::FindWindow(_T("tSkMainForm.UnicodeClass"), NULL); if (hwnd == NULL) return FALSE;
	::SetForegroundWindow(hwnd);
	Sleep(100);
	//// Ctrl+Tabを回数分実行し、Call Tabを表示する
	// 着信中は動作しないらしい・・・
	for (int i = 0 ; i < intNext; i++) {
		KeyCtrlTab();
	}

	// 再確認
	if (!::IsWindowVisible(hVwnd)) return FALSE;

	return TRUE;
}

//
//  機能     : Skype TABのWindowハンドル取得
//  
//  機能説明 : 
//  
//  返り値   : HWND
//  
//  備考     : 
//  
HWND CAccessEvents::FindTab(CString strTab)
{
	HWND hwnd = ::FindWindow(_T("tSkMainForm.UnicodeClass"), NULL); if (hwnd == NULL) return NULL;
	hwnd = ::FindWindowEx(hwnd, 0, _T("TPanel"), NULL); if (hwnd == NULL) return NULL;
	hwnd = ::FindWindowEx(hwnd, 0, _T("TPanel"), NULL); if (hwnd == NULL) return NULL;
	hwnd = ::FindWindowEx(hwnd, 0, _T("TSkypeTabControl"), NULL); if (hwnd == NULL) return NULL;
	// タブの検索
	hwnd = ::FindWindowEx(hwnd, 0, strTab, NULL); if (hwnd == NULL) return NULL;

	return hwnd;
}

//
//  機能     : Ctrl+Tabキーを発行してSkypeのTABを切り替える
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
void CAccessEvents::KeyCtrlTab()
{
/*
	INPUT inputKey[4];

	ZeroMemory(inputKey, sizeof(INPUT)*4);
	inputKey[0].type = INPUT_KEYBOARD;
	inputKey[0].ki.wVk = VK_CONTROL;
	inputKey[0].ki.wScan = MapVirtualKey(VK_CONTROL, 0);
	inputKey[1].type = INPUT_KEYBOARD;
	inputKey[1].ki.wVk = VK_TAB;
	inputKey[1].ki.wScan = MapVirtualKey(VK_TAB, 0);
	inputKey[2] = inputKey[0];
	inputKey[2].ki.dwFlags = KEYEVENTF_KEYUP;
	inputKey[3] = inputKey[1];
	inputKey[3].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(4, inputKey, sizeof(INPUT));
*/

	keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), 0, 0);
	keybd_event(VK_TAB, MapVirtualKey(VK_TAB, 0), 0, 0);
	keybd_event(VK_TAB, MapVirtualKey(VK_TAB, 0), KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), KEYEVENTF_KEYUP, 0);
}

//
//  機能     : ユーザ単位のデータベースのバージョンアップ
//  
//  機能説明 : 旧バージョンのデータを修正
//  
//  返り値   : BOOL
//  
//  備考     : Ver0.90～ : 応答WAVEファイルのフォーマット修正（16Khz, 16bit, 1channel）
//           : Ver0.92～ : 応答WAVEファイルのフォーマット修正（48Khz, 16bit, 1channel）
//           : Ver1.00～ : 応答WAVEファイルのフォーマット修正（16Khz, 16bit, 1channel）
//  
BOOL CAccessEvents::VerUpUserData(void)
{
	long i;
	CString str;
	CString strWhere;
	CString strPath;	
	CString strPathTmp;	
	CFileSpec fs;
	BOOL blFlag = FALSE;
	AUDIO_FORMAT fmt;
	fmt.nBits = 16;
	fmt.nChannels = 1;
	fmt.nFrequency = 16000;

	try {
		CTPsetup tps(theApp.m_strDataFolder);
		CTblRsfl tbl(theApp.m_strDBPath);
		strWhere.Empty();
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		long lngCnt = tbl.SelectRec(strWhere);
		if (lngCnt == 0) return TRUE;

		for (i = 0; i < lngCnt; i++) {
			/// Ver.92-0.99用処理
			if (tbl.strSystemVersion == _T(POTM_VERSION_OLD2) || tbl.strSystemVersion == _T(POTM_VERSION_OLD3) || 
			    tbl.strSystemVersion == _T(POTM_VERSION_OLD4) || tbl.strSystemVersion == _T(POTM_VERSION_OLD5)) {
				if (!blFlag) {
					/// BEEPファイルを16KHzフォーマットへ変換（１回だけ実行）
					blFlag = TRUE;
					strPath = tps.RsfBeepFileName();
					fs.SetFullSpec(strPath);
					if (fs.Exist()) {
						strPathTmp = strPath + _T("_tmp");
						//// 一時ファイルとして生成
						if (!m_pAudio->ChgWaveFormat(strPath, strPathTmp, fmt, 0)) {
							str.LoadString(IDS_MSGBOX901);
							AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
						} else {
							//// 一時ファイルを応答ファイルにコピー
							fs.SetFullSpec(strPathTmp);
							if (fs.Exist()) {
								fs.FileMove(strPath, TRUE);
							}
						}
					}
					strPath = tps.RsfBeepFileName(_T("1"));
					fs.SetFullSpec(strPath);
					if (fs.Exist()) {
						strPathTmp = strPath + _T("_tmp");
						//// 一時ファイルとして生成
						if (!m_pAudio->ChgWaveFormat(strPath, strPathTmp, fmt, 0)) {
							str.LoadString(IDS_MSGBOX901);
							AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
						} else {
							//// 一時ファイルを応答ファイルにコピー
							fs.SetFullSpec(strPathTmp);
							if (fs.Exist()) {
								fs.FileMove(strPath, TRUE);
							}
						}
					}
					strPath = tps.RsfBeepFileName(_T("2"));
					fs.SetFullSpec(strPath);
					if (fs.Exist()) {
						strPathTmp = strPath + _T("_tmp");
						//// 一時ファイルとして生成
						if (!m_pAudio->ChgWaveFormat(strPath, strPathTmp, fmt, 0)) {
							str.LoadString(IDS_MSGBOX901);
							AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
						} else {
							//// 一時ファイルを応答ファイルにコピー
							fs.SetFullSpec(strPathTmp);
							if (fs.Exist()) {
								fs.FileMove(strPath, TRUE);
							}
						}
					}
				}
				/// 応答音声ファイル
				if (tbl.lngDefaultFlag == 1) {
					tps.SetCallerId(_T("0"));
				} else {
					tps.SetCallerId(tbl.strSenderId);
				}
				strPath = tps.RsfAudioFileName(_T("$"));
				/// 応答音声ファイルを16KHzフォーマットへ変換
				fs.SetFullSpec(strPath);
				if (!fs.Exist()) goto NEXT_LOOP;
				strPathTmp = strPath + _T("_tmp");
				//// 一時ファイルとして生成
				if (!m_pAudio->ChgWaveFormat(strPath, strPathTmp, fmt, 0)) {
					str.LoadString(IDS_MSGBOX900);
					AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
					goto NEXT_LOOP;
				}
				//// 一時ファイルを応答ファイルにコピー
				fs.SetFullSpec(strPathTmp);
				if (!fs.Exist()) goto NEXT_LOOP;
				fs.FileMove(strPath, TRUE);
			}
NEXT_LOOP:
			if (!tbl.NextRec()) break;
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("version up data error"));
		return FALSE;
	}
		
	return TRUE;
}
