// RsfSimpleForm.cpp : implementation file
//

#include "stdafx.h"
#include "Tapur.h"
#include "MainFrm.h"
#include "RsfTree.h"
#include "RsfSimpleForm.h"
#include "RsfAudioDialog.h"
#include "RsfChatDialog.h"
#include "RsfVideoDialog.h"
#include "RsfImgDialog.h"
#include "RsfPrgDialog.h"
#include "Audio.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"
#include "file_access.h"
#include ".\rsfsimpleform.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CRsfSimpleForm

IMPLEMENT_DYNCREATE(CRsfSimpleForm, TVisualFormView)

CRsfSimpleForm::CRsfSimpleForm()
	: TVisualFormView(CRsfSimpleForm::IDD)
{
	//{{AFX_DATA_INIT(CRsfSimpleForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_blAudio = FALSE;
    m_blChat = FALSE;
    m_blVideo = FALSE;
    m_blImg = FALSE;
	m_blBeep = FALSE;
	m_blPrg = FALSE;
	m_idxPrg = 0;

	// 背景色ブラシ初期化
	m_brush.CreateSolidBrush(RGB(255,255,255));		// white
}

CRsfSimpleForm::~CRsfSimpleForm()
{
}

void CRsfSimpleForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfSimpleForm)
    DDX_Check(pDX, IDC_CHK_AUDIO, m_blAudio);
    DDX_Check(pDX, IDC_CHK_CHAT, m_blChat);
    DDX_Check(pDX, IDC_CHK_VIDEO, m_blVideo);
    DDX_Check(pDX, IDC_CHK_IMG, m_blImg);
    DDX_Check(pDX, IDC_CHK_BEEP, m_blBeep);
	DDX_Control(pDX, IDC_ST_SIZE_AUDIO, m_lblAudioSize);
	DDX_Control(pDX, IDC_ST_SIZE_CHAT, m_lblChatSize);
	DDX_Control(pDX, IDC_ST_SIZE_VIDEO, m_lblVideoSize);
	DDX_Control(pDX, IDC_ST_SIZE_IMG, m_lblImgSize);
    DDX_Check(pDX, IDC_CHK_PRG, m_blPrg);
	DDX_CBIndex(pDX, IDC_CMB_PRG, m_idxPrg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRsfSimpleForm, TVisualFormView)
	//{{AFX_MSG_MAP(CRsfSimpleForm)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHK_AUDIO, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_CHAT, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_VIDEO, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_IMG, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_BEEP, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_REC, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_VREC, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_DISCONNECT, OnChgForm)
	ON_BN_CLICKED(IDC_BTN_AUDIO, OnButtonAudio)
	ON_BN_CLICKED(IDC_BTN_CHAT, OnButtonChat)
	ON_BN_CLICKED(IDC_BTN_VIDEO, OnButtonVideo)
	ON_BN_CLICKED(IDC_BTN_IMG, OnButtonImage)
	ON_BN_CLICKED(IDC_CHK_PRG, OnChgForm)
	ON_CBN_SELCHANGE(IDC_CMB_PRG, OnChgForm)
	ON_BN_CLICKED(IDC_BTN_PRG, OnButtonPrg)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfSimpleForm diagnostics

#ifdef _DEBUG
void CRsfSimpleForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CRsfSimpleForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRsfSimpleForm message handlers

void CRsfSimpleForm::OnDestroy()
{
	TVisualFormView::OnDestroy();

	// TODO : ここにメッセージ ハンドラ コードを追加します。
}

void CRsfSimpleForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	/// ラベルの属性変更
	m_lblAudioSize.SetBkColor(RGB(255,255,255));	// white
	m_lblChatSize.SetBkColor(RGB(255,255,255));		// white
	m_lblVideoSize.SetBkColor(RGB(255,255,255));	// white
	m_lblImgSize.SetBkColor(RGB(255,255,255));		// white
/*
	//// フォームのデフォルト設定の取得
	CFont *ft = CFormView::GetFont();
	LOGFONT lft;
	ft->GetLogFont(&lft);
	//// 更新
	m_Title.SetFontStatic(lft.lfFaceName, abs(lft.lfHeight), RGB(0,0,0), FS_VCENTER | FS_UNDERLINED | FS_BOLD);
*/

	/// 連携プログラム名コンボボックス初期化
	CString strItm;
	CComboBox *pcmb;
    pcmb = (CComboBox*)GetDlgItem(IDC_CMB_PRG);
	strItm.LoadString(IDS_RSF_PRG1);
	pcmb->AddString(strItm);
	strItm.LoadString(IDS_RSF_PRG11);
	pcmb->AddString(strItm);
	strItm.LoadString(IDS_RSF_PRG901);
	pcmb->AddString(strItm);
	m_idxPrg = 0;	// 先頭を選択
	/// 連携プログラムIDリスト初期化
	m_aryPrgId.Add(_T("1"));	
	m_aryPrgId.Add(_T("11"));	
	m_aryPrgId.Add(_T("901"));

	UpdateData(FALSE);	// DDX更新
}

HBRUSH CRsfSimpleForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = TVisualFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: この位置で DC のアトリビュートを変更してください
	
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください

	// フォーム背景色を白に変更
	if (nCtlColor == CTLCOLOR_DLG) {
		return (HBRUSH)m_brush.GetSafeHandle();
	}

	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkColor(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);//背景透過
//			pDC->SetTextColor(RGB(0,0,255));// テキスト色青設定
		return (HBRUSH)m_brush.GetSafeHandle();
	}

	return hbr;
}

BOOL CRsfSimpleForm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		if (nVirtKey == VK_TAB) {
		/// TAB KEY : RsfTreeフォームへ遷移
			//// フォーカス最後の場合のみ有効
			CWnd *cWnd = GetFocus();
			if (cWnd->m_hWnd == GetDlgItem(IDC_RDO_REC)->m_hWnd ||
				cWnd->m_hWnd == GetDlgItem(IDC_RDO_VREC)->m_hWnd ||
				cWnd->m_hWnd == GetDlgItem(IDC_RDO_DISCONNECT)->m_hWnd) {
				CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
				CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
				pfview->SetFocus();
				return TRUE;
			}
		}
	}
	
	return TVisualFormView::PreTranslateMessage(pMsg);
}

// 音声入力ボタン
void CRsfSimpleForm::OnButtonAudio() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	/// 音声ファイルの特定
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(m_strSenderId);
	CString strFile = tps.RsfAudioFileName(_T("$"));

	/// 画面の表示
	CRsfAudioDialog dlg;
	dlg.SetFile(strFile);
	if (dlg.DoModal() == IDOK) {
		/// RSFファイルの更新
		if (!UpdateFile()) return;
		/// 画面の更新
		UpdateForm();
	}
}

// チャットテキスト入力ボタン
void CRsfSimpleForm::OnButtonChat() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	/// チャットファイルの特定
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(m_strSenderId);
	CString strFile = tps.RsfChatFileName(_T("$"));

	/// 画面の表示
	CRsfChatDialog dlg;
	dlg.SetFile(strFile);
	if (dlg.DoModal() == IDOK) {
		/// RSFファイルの更新
		if (!UpdateFile()) return;
		/// 画面の更新
		UpdateForm();
	}
}

// ビデオ入力ボタン
void CRsfSimpleForm::OnButtonVideo() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	/// ビデオファイルの特定
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(m_strSenderId);
	CString strFile = tps.RsfVideoFileName(_T("$"));

	/// 画面の表示
	CRsfVideoDialog dlg;
	dlg.SetFile(strFile);
	if (dlg.DoModal() == IDOK) {
		/// RSFファイルの更新
		if (!UpdateFile()) return;
		/// 画面の更新
		UpdateForm();
	}
}

// 画像入力ボタン
void CRsfSimpleForm::OnButtonImage() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	/// 画像ファイルの特定
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(m_strSenderId);
	CString strFile = tps.FindRsfImgFile(_T("$"));
	CString strTmp = tps.RsfImgFileName(_T("$"), _T(""));	// 拡張子なしファイル

	/// 画面の表示
	CRsfImgDialog dlg;
	dlg.SetFile(strFile);
	dlg.SetFileNoExt(strTmp);
	if (dlg.DoModal() == IDOK) {
		/// RSFファイルの更新
		if (!UpdateFile()) return;
		/// 画面の更新
		UpdateForm();
	}
}

// 連携プログラム設定ボタン
void CRsfSimpleForm::OnButtonPrg() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString str;

	UpdateData(TRUE);	// DDX更新

	// 連携プログラムID
	CString strPrgId = m_aryPrgId.GetAt(m_idxPrg);

	/// プログラムCSVファイルの特定
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(m_strSenderId);
	CString strFile = tps.RsfPrgFileName(_T("$$$$"), strPrgId);

	/// 画面の表示
	CComboBox *pcmb;
    pcmb = (CComboBox*)GetDlgItem(IDC_CMB_PRG);
	pcmb->GetLBText(m_idxPrg, str);
	CRsfPrgDialog dlg(this, str);
	dlg.SetFile(strFile);
	if (dlg.DoModal() == IDOK) {
		/// RSFファイルの更新
		if (!UpdateFile()) return;
		/// 画面の更新
		UpdateForm();
	}
}

// フォーム入力イベント
// checkbox, radio button 押下時
void CRsfSimpleForm::OnChgForm() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	// checkboxの併用チェック
	CButton *chk_audio = (CButton *)GetDlgItem(IDC_CHK_AUDIO);
	CButton *chk_chat = (CButton *)GetDlgItem(IDC_CHK_CHAT);
	CButton *chk_img = (CButton *)GetDlgItem(IDC_CHK_IMG);
	CButton *chk_video = (CButton *)GetDlgItem(IDC_CHK_VIDEO);
	if (chk_img->GetCheck() && chk_video->GetCheck()) {
		UpdateData(FALSE);	// ボタン状態を戻す
		CString str;
		str.LoadString(IDS_MSGBOX58);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}
	if (!chk_audio->GetCheck() && !chk_chat->GetCheck() && 
		!chk_img->GetCheck() && !chk_video->GetCheck()) {
			UpdateData(FALSE);	// ボタン状態を戻す
			CString str;
			str.LoadString(IDS_MSGBOX59);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
			return;
	}

	/// RSFファイルの更新
	UpdateFile();
}

/////////////////////////////////////////////////////////////////////////////
// local function

//
//  機能     : RSFファイルの更新
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfSimpleForm::UpdateFile() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString str;

	UpdateData(TRUE);	// DDX更新

	// ラジオボタンの参照定義（DDXは扱いにくいので）
	// （クラスメンバーで宣言すると正常に動作しない）
	CButton *rd_rec = (CButton *)GetDlgItem(IDC_RDO_REC);
	CButton *rd_vrec = (CButton *)GetDlgItem(IDC_RDO_VREC);
	CButton *rd_discon = (CButton *)GetDlgItem(IDC_RDO_DISCONNECT);

	/// 応答設定パラメータ作成
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(m_strSenderId);
	CRsfPrm prm;
	CArray<CRsfPrm, CRsfPrm> prmArray;
	//// 応答メッセージ
	if (m_blAudio) {
		prm.type = RESP_TYPE_PLAY;
		prm.play_type |= RESP_PLAY_TYPE_AUDIO;
	}
	if (m_blChat) {
		prm.type = RESP_TYPE_PLAY;
		prm.play_type |= RESP_PLAY_TYPE_CHAT;
	}
	if (m_blVideo) {
		prm.type = RESP_TYPE_PLAY;
		prm.play_type |= RESP_PLAY_TYPE_VIDEO;
	}
	if (m_blImg) {
		prm.type = RESP_TYPE_PLAY;
		prm.play_type |= RESP_PLAY_TYPE_IMAGE;
		str = tps.FindRsfImgFile(_T("$"));
		str = str.Right(3);
		str.MakeLower();
		if (str == _T("bmp")) {
			prm.file_type = RESP_FILE_TYPE_BMP;
		} else if (str == _T("jpg")) {
			prm.file_type = RESP_FILE_TYPE_JPEG;
		} else if (str == _T("gif")) {
			prm.file_type = RESP_FILE_TYPE_GIF;
		}
	}
	if (prm.type == RESP_TYPE_PLAY) {
		prmArray.Add(prm);
	}
	//// 録音タイプ
	prm.Init();
	if (m_blBeep) {
		prm.beep = 1;
	}
	if (rd_rec->GetCheck()) {
		prm.type = RESP_TYPE_RECORD;
	} else if (rd_vrec->GetCheck()) {
		prm.type = RESP_TYPE_VRECORD;
	} else if (rd_discon->GetCheck()) {
		prm.type = RESP_TYPE_DISCONNECT;
	}
	prmArray.Add(prm);
	//// 終了
	if (prm.type != RESP_TYPE_DISCONNECT) {
		prm.Init();
		prm.type = RESP_TYPE_DISCONNECT;
		prmArray.Add(prm);
	}
	//// 連携プログラム
	if (m_blPrg) {
		prm.Init();
		prm.type = RESP_TYPE_EXEC;
		CString strPrgId = m_aryPrgId.GetAt(m_idxPrg);
		prm.exec_id = _ttoi(strPrgId);
		prmArray.Add(prm);
	}

	/// 応答設定ファイル再作成
	if (!tps.SaveRsfFile(prmArray)){
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 画面の更新
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfSimpleForm::UpdateForm() 
{
	/// カレントレコード
	if (theApp.m_lngRsflId <= 0) return FALSE;

	/// 再生中のファイルを停止する
//	PlayStop();

	// StatusBarのクリア
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	pFrame->SetStatusText(_T(""), 0);
	pFrame->SetStatusText(_T(""), 1);

	// ラジオボタンの参照定義（DDXは扱いにくいので）
	// （クラスメンバーで宣言すると正常に動作しない）
	CButton *rd_rec = (CButton *)GetDlgItem(IDC_RDO_REC);
	CButton *rd_vrec = (CButton *)GetDlgItem(IDC_RDO_VREC);
	CButton *rd_discon = (CButton *)GetDlgItem(IDC_RDO_DISCONNECT);

	/// 初期設定
	m_blAudio = FALSE;
    m_blChat = FALSE;
    m_blVideo = FALSE;
    m_blImg = FALSE;
	m_blBeep = FALSE;
	m_blPrg = FALSE;
	rd_rec->SetCheck(0);
	rd_vrec->SetCheck(0);
	rd_discon->SetCheck(1);

	/// 応答設定テーブルを選択
	CTblRsfl tbl(theApp.m_strDBPath);
	CString strWhere;
	_TCHAR buf[64];
	strWhere.Empty();
	_ltot_s(theApp.m_lngRsflId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T("ID") + _T("=") + buf;
	_ltot_s(RESPBASE_TYPE_SIMPLE, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T(" and ") + _T("lngRespType") + _T("=") + buf;
	if (tbl.SelectRec(strWhere) <= 0) {
		return FALSE;
	}

	/// 選択された応答設定RSFをロード
	CTPsetup tps(theApp.m_strDataFolder);
	if (tbl.lngDefaultFlag == 1) {
		m_strSenderId = _T("0");	// default setting
	} else {
		m_strSenderId = tbl.strSenderId;
	}
	tps.SetCallerId(m_strSenderId);
	UST_RSF *pst = tps.LoadRSF();
	if (pst == NULL) return FALSE;

	/// 選択された応答設定RSFを参照
	UST_RSF_RESP *pst_resp;
	for (int i = 0; i < pst->resp_cnt; i++) {
		pst_resp = pst->resp + i;
		//// BEEPのON/OFF
		if (pst_resp->beep > 0) {
			m_blBeep = TRUE;
		}
		switch(pst_resp->type) {
		case RESP_TYPE_PLAY:
			//// 再生内容のON/OFF
			if (pst_resp->play_type & RESP_PLAY_TYPE_AUDIO) {
				m_blAudio = TRUE;
			}
			if (pst_resp->play_type & RESP_PLAY_TYPE_VIDEO) {
				m_blVideo = TRUE;
			}
			if (pst_resp->play_type & RESP_PLAY_TYPE_IMAGE) {
				m_blImg = TRUE;
			}
			if (pst_resp->play_type & RESP_PLAY_TYPE_CHAT) {
				m_blChat = TRUE;
			}
			break;
		//// 録音内容のON/OFF
		case RESP_TYPE_RECORD:
			rd_rec->SetCheck(1);
			rd_vrec->SetCheck(0);
			rd_discon->SetCheck(0);
			break;
		case RESP_TYPE_VRECORD:
			rd_rec->SetCheck(0);
			rd_vrec->SetCheck(1);
			rd_discon->SetCheck(0);
			break;
		case RESP_TYPE_DISCONNECT:
			// 全ての設定で使用されるので、設定すべきことはない
			break;
		//// プログラム連携のON/OFF
		case RESP_TYPE_EXEC:
			m_blPrg = TRUE;
			break;
		default:
			break;
		}
	}

	/// 応答音声ファイルの時間表示
	CString strPlayFile;
	CString strSize;
	CFileSpec fs;
	strSize = _T("[ 00:00 ]");
	strPlayFile = tps.RsfAudioFileName(_T("$"));
	fs.SetFullSpec(strPlayFile);
	if (fs.Exist()) {
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CAudio *pAudio = pFrame->m_pAudio;
		pAudio->m_PlayFile = strPlayFile;
		float fTime = pAudio->GetPlaySize();
		if (fTime > 0.0) {
			long lngTime = (long)(fTime+1);
			strSize.Format(_T("[ %02i:%02i ]"), (lngTime/60), (lngTime%60));
		}
	}
	m_lblAudioSize.SetText((LPCTSTR)strSize);

	/// 応答ビデオファイルの時間表示
	strSize = _T("[ 00:00 ]");
	strPlayFile = tps.RsfVideoFileName(_T("$"));
	fs.SetFullSpec(strPlayFile);
	if (fs.Exist()) {
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CVideo *pVideo = pFrame->m_pVideo;
		pVideo->SetPlayFile(strPlayFile);
		float fTime = pVideo->GetPlaySize();
		if (fTime > 0.0) {
			long lngTime = (long)(fTime+1);
			strSize.Format(_T("[ %02i:%02i ]"), (lngTime/60), (lngTime%60));
		}
	}
	m_lblVideoSize.SetText((LPCTSTR)strSize);

	/// 画像ファイルのサイズ表示
	strSize = _T("[ 0 Kbytes ]");
	strPlayFile = tps.FindRsfImgFile(_T("$"));
	fs.SetFullSpec(strPlayFile);
	if (fs.Exist()) {
		strSize.Format(_T("[ %d Kbytes ]"), fs.FileSize()/1024);
	}
	m_lblImgSize.SetText((LPCTSTR)strSize);

	/// チャットテキストファイルのサイズ表示
	strSize = _T("[ 0 bytes ]");
	strPlayFile = tps.RsfChatFileName(_T("$"));
	fs.SetFullSpec(strPlayFile);
	if (fs.Exist()) {
		strSize.Format(_T("[ %d bytes ]"), fs.FileSize());
	}
	m_lblChatSize.SetText((LPCTSTR)strSize);

 	/// 連携プログラムコンボボックスを選択
	CString str, strPrgId;
	strPrgId.Format(_T("%d"), pst_resp->exec_id);
	int idx = 0;
	for(int i = 0; i <  m_aryPrgId.GetSize(); i++) {
		str = m_aryPrgId.GetAt(i);
		if (strPrgId == str) {
			idx = i;
			break;
		}
	}
	m_idxPrg = idx;

	/// 連携プログラムCSVファイルが存在しない場合、テンプレートCSVフォルダからコピー
	CString strBinDir, strFile;
	fs.SetFullSpec(FS_APPDIR);
	strBinDir = fs.GetFullSpec() + _T("bin");	// テンプレートCSVフォルダ
	for(int i = 0; i <  m_aryPrgId.GetSize(); i++) {
		strPrgId = m_aryPrgId.GetAt(i);
		strFile = tps.RsfPrgFileName(_T("$$$$"), strPrgId);	// 連携プログラムCSVファイル
		fs.SetFullSpec(strFile);
		if (!fs.Exist()) {
			fs.SetFullSpec(strBinDir + "\\" + strPrgId + ".csv");
			if (fs.Exist()) {
				fs.FileCopy(strFile, FALSE);	// 上書きはしない
			}
		}
	}

	UpdateData(FALSE);	// DDX更新

	return TRUE;
}


