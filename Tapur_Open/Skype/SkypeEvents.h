// SkypeEvents.h

#ifndef __SKYPEEVENTS_H__
#define __SKYPEEVENTS_H__

#include "TPerror.h"
#include "TPsetup.h"
#include "Audio.h"
#include "Video.h"
#include "CallStatus.h"
#include "AviManager.h"

// Smart Pointer for SkypeAPI Interface
/*
typedef CComPtr<IAccess> IAccessPtr;
typedef CComPtr<ICall> ICallPtr;
typedef CComPtr<IMessage> IMessagePtr;
typedef CComPtr<IUser> IUserPtr;
typedef CComPtr<IChatMessage> IChatMessagePtr;
typedef CComPtr<ICallCollection> ICallCollectionPtr;
*/

class CAccessEvents :
  public CComObjectRoot,
  public IDispatchImpl<_IAccessEvents, &DIID__IAccessEvents, &LIBID_SKYPEAPILib>
{
public:
	CAccessEvents();
	~CAccessEvents();

BEGIN_COM_MAP(CAccessEvents)
  COM_INTERFACE_ENTRY(IDispatch)
  COM_INTERFACE_ENTRY_IID(DIID__IAccessEvents, IDispatch)
END_COM_MAP()

// IDispatch
	STDMETHOD(Invoke)(DISPID dispid, REFIID riid, LCID lcid, 
		WORD wFlags, DISPPARAMS* pdispParams, VARIANT* pvarResult, 
		EXCEPINFO* pexcepinfo, UINT* puArgErr);

	void SetMyParent(HWND hWnd) { m_hParent = hWnd; };

private:
	CTPerror te;
	HWND m_hParent;

// イベントの実装
private:
	HRESULT Result(BSTR Description);
	HRESULT Command(BSTR SkypeCommand);
	HRESULT UserStatusChanged(SkypeOnlineStatus Status);
	HRESULT MuteChanged(VARIANT_BOOL Mute);
	HRESULT ConnectionStatusChanged(SkypeConnectionStatus Status);
	HRESULT MessageReceived(IMessagePtr pSkypeMessage);
	HRESULT MessageSent(IMessagePtr pSkypeMessage);
	HRESULT APIStatusChanged(SkypeAPIAttachmentStatus Status);
	HRESULT CallHistoryChanged();
	HRESULT IMHistoryChanged();
	HRESULT Error(LONG Number, BSTR Description);
	HRESULT OnlineStatusChanged(IUserPtr pSkypeUser, SkypeOnlineStatus Status);
	HRESULT ChatMessageReceived(IChatMessagePtr pSkypeChatMessage);
	HRESULT ChatMessageSent(IChatMessagePtr pSkypeChatMessage);
public:
	HRESULT CallStatusChanged(ICallPtr pChangedCall, SkypeCallProgress Status);
	BOOL Response();
	
private:
	BOOL SetAppDevice(CString strInDev, CString strOutDev);		// Tapurデバイスの設定

public:
	CTPsetup m_Setup;		// 応答設定クラス
	IAccessPtr m_pAccess;	// SkypeAPI Accessクラス
	ICallPtr m_pCall;		// 現在のSkype Callクラス
	CCallStatus m_CallStatus;	// 現在のSkype Call状態
	long m_lngProtocol;		// SKYPEプロトコル
	CString m_defInDevice;	// SKYPEのデフォルトINデバイス
	CString m_defOutDevice;	// SKYPEのデフォルトOUTデバイス
	CString m_defVDevice;	// SKYPEのデフォルトビデオデバイス
	CAudio *m_pAudio;		// 音声操作クラス
	CVideo *m_pVideo;		// ビデオ操作クラス
	//CAviManager *m_pAviManager[16];		// ビデオ＋音声結合クラス
	CString m_ProcCode;		// 現在の応答コード
	BOOL blSkypeVideoVersion;	// スカイプビデオ有効バージョンフラグ
	BOOL blResponse;		// 応答処理中フラグ
	BOOL blCallFinish;		// 通話終了処理中フラグ

	void SetAccess(IAccessPtr pAccess) { m_pAccess = pAccess; }
	BOOL PlayBeep(long lngSecond);		// BEEP音
	BOOL SendCall(CString strSender);	// Skypeコール開始
	BOOL SendChat(CString strMsg, CString strUserId = _T(""));	// チャットメッセージ送信
	BOOL IsTalking(BOOL blUpdate = FALSE);		// 通話中か否か
	BOOL IsVTalking();			// ビデオ受信中か否か
	void SetResponsing(BOOL blSet) { blResponse = blSet; }		// 応答処理中フラグ設定
	void SetCallFinish(BOOL blSet) { blCallFinish = blSet; }	// 通話終了処理中フラグ設定
	BOOL IsResponsing() { return blResponse; }					// 応答処理中フラグ参照
	BOOL IsCallFinishing() { return blCallFinish; }				// 通話終了処理中フラグ参照
	CString GetSender();		// 送信者の名称orID
	BOOL LoadSetup();			// 送信者に対応した応答設定をロード
	int StartRecord(BOOL blVideo);	// 録音・録画開始時の処理
	BOOL FinishRecordTbl(CString strAudioFile, CString strVideoFile, BOOL blRepair = FALSE);	// 録音・録画完了時のテーブル処理
	BOOL FinishRecord();		// 録音・録画完了時の処理
	BOOL ExecResp(int intPrgId);// 連携プログラムの実行
	BOOL InitUserData(void);	// ユーザデータの初期化
	BOOL InitDummyUser(void);	// SKYPEに接続できない時の表示ユーザの特定
	BOOL InitDevice(void);		// Skypeデバイスの初期化
	BOOL VerUpUserData(void);	// ユーザ単位のデータベースのバージョンアップ
	//BOOL SetSkypeDevice(CString strInDev, CString strOutDev, BOOL blSilence = FALSE);	// Skypeデバイスの設定
	//BOOL SetSkypeDeviceTry(CString strInDev, CString strOutDev);	// Skypeデバイスの設定
	BOOL IsSkypeVideoVersion();		// Skypeビデオが有効なバージョンか？
	BOOL IsSkypeVideo(CString strUsr = _T(""));		// Skypeビデオが使用可能か？
	//CString GetSkypeVDevice();	// Skypeビデオデバイスの取得
	BOOL SetSkypeVDevice(CString strInDev, BOOL blSilence = FALSE);	// Skypeビデオデバイスの設定
	BOOL SetSkypeVDeviceTry(CString strInDev);	// Skypeビデオデバイスの設定
	BOOL RefreshSkypeVideo(long lngCall, BOOL blStop = FALSE);		// ビデオ表示をリフレッシュ
	int ShowVideoWin(CString strSkypeId, BOOL blLarge = TRUE);		// ビデオ画面をTAB→ウィンドウにする
	BOOL FocusCallTab(CString strSkypeId);
	HWND FindTab(CString strTab);
	void KeyCtrlTab();
};


#endif	// __SKYPEEVENTS_H__

