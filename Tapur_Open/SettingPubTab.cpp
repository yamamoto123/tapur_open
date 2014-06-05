// SettingPubTab.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "SettingPubTab.h"
#include "SettingDialog.h"
#include "TabCtrlSSL.h"
#include "FileSpec.h"
#include "Audio.h"
#include "TPsetup.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingPubTab ダイアログ


CSettingPubTab::CSettingPubTab(CWnd* pParent /*=NULL*/)
	: CTabPageSSL (CSettingPubTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingPubTab)
	m_idxCodecName = 0;
	m_idxFormatName = 0;
	m_blStartup = FALSE;
	m_blDisable = FALSE;
	m_idxWaitAnswer = 0;
	m_idxMaxRecord = 0;
//	m_blShortCutTapur = TRUE;
//	m_blShortCutSkype = TRUE;
	//}}AFX_DATA_INIT
}


void CSettingPubTab::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingPubTab)
	DDX_Check(pDX, IDC_CHK_STARTUP, m_blStartup);
	DDX_CBIndex(pDX, IDC_CMB_CODEC, m_idxCodecName);
	DDX_CBIndex(pDX, IDC_CMB_FORMAT, m_idxFormatName);
	DDX_Check(pDX, IDC_CHK_DISABLE, m_blDisable);
	DDX_CBIndex(pDX, IDC_CMB_WAIT_ANSWER, m_idxWaitAnswer);
	DDX_CBIndex(pDX, IDC_CMB_MAX_RECORD, m_idxMaxRecord);
	DDX_Control(pDX, IDC_BTN_BEEPFILE, m_File);
//	DDX_Check(pDX, IDC_CHK_SHORTCUT_TAPUR, m_blShortCutTapur);
//	DDX_Check(pDX, IDC_CHK_SHORTCUT_SKYPE, m_blShortCutSkype);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingPubTab, CDialog)
	//{{AFX_MSG_MAP(CSettingPubTab)
//	ON_EN_SETFOCUS(IDC_CMB_WAIT_ANSWER, OnSetfocusWaitAnswer)
//	ON_EN_KILLFOCUS(IDC_CMB_WAIT_ANSWER, OnKillfocusWaitAnswer)
//	ON_EN_SETFOCUS(IDC_CMB_MAX_RECORD, OnSetfocusMaxRecord)
//	ON_EN_KILLFOCUS(IDC_CMB_MAX_RECORD, OnKillfocusMaxRecord)
	ON_BN_CLICKED(IDC_CHK_STARTUP, OnChgForm)
	ON_BN_CLICKED(IDC_BTN_BEEPFILE, OnBtnBeepFile)
	ON_BN_CLICKED(IDC_CHK_DISABLE, OnChgForm)
	ON_CBN_SELCHANGE(IDC_CMB_WAIT_ANSWER, OnChgForm)
	ON_CBN_SELCHANGE(IDC_CMB_MAX_RECORD, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_WAV, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_MP3, OnRdoMp3)
	ON_CBN_SELCHANGE(IDC_CMB_CODEC, OnCmbCodec)
	ON_CBN_SELCHANGE(IDC_CMB_FORMAT, OnChgForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingPubTab メッセージ ハンドラ

BOOL CSettingPubTab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	int i;
	CString str;
	DWORD dw;

	/// レジストリ読み込み
	m_rgStartup = CRegString(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run\\Tapur"), _T(""), FALSE);
	m_rgDisable = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\InitDisable"), 0, TRUE);
	m_rgWaitAnswer = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\WaitAnswerTime"), 10, TRUE);
	m_rgMaxRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\MaxRecordTime"), 180, TRUE);
	m_rgShortCutTapur = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\ShortCutTapur"), 1, TRUE);
	m_rgShortCutSkype = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\ShortCutSkype"), 1, TRUE);
 	//// WAV or MP3
	m_rgFormatType = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordType"), 0, TRUE);		// 初期値:0(WAV)
	m_rgCodecName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordMP3Codec"), _T(""), TRUE);
	m_rgFormatName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\AnswerRecordMP3Format"), _T(""), TRUE);
	//// Startup
	str = m_rgStartup;
	m_blStartup = (str.IsEmpty()) ? FALSE : TRUE;
	//// 起動時無効
	// メインメニューの「応答の有効」は非接続時には無効となるので、この機能と完全な連動はできない
	dw = m_rgDisable;
	m_blDisable = (dw == 1) ? TRUE : FALSE;

	/// コンボボックス初期化
	CComboBox *pcmb;
	//// 待機時間
	m_aryWaitAnswer.Add(_T("0"));
    m_aryWaitAnswer.Add(_T("5"));
    m_aryWaitAnswer.Add(_T("10"));
    m_aryWaitAnswer.Add(_T("15"));
    m_aryWaitAnswer.Add(_T("20"));
    m_aryWaitAnswer.Add(_T("30"));
    m_aryWaitAnswer.Add(_T("45"));
    m_aryWaitAnswer.Add(_T("60"));
    pcmb = (CComboBox*)GetDlgItem(IDC_CMB_WAIT_ANSWER);
//	pcmb->ShowDropDown(TRUE);
//	pcmb->SetExtendedUI(TRUE);
	for(i = 0; i < m_aryWaitAnswer.GetSize(); i++) {
		pcmb->AddString(m_aryWaitAnswer.GetAt(i));
	}
 	//// 録音時間
	m_aryMaxRecord.Add(_T("1"));
    m_aryMaxRecord.Add(_T("2"));
    m_aryMaxRecord.Add(_T("3"));
    m_aryMaxRecord.Add(_T("5"));
    m_aryMaxRecord.Add(_T("10"));
    m_aryMaxRecord.Add(_T("15"));
    m_aryMaxRecord.Add(_T("20"));
    m_aryMaxRecord.Add(_T("30"));
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_MAX_RECORD);
//	pcmb->ShowDropDown(TRUE);
//	pcmb->SetExtendedUI(TRUE);
	for(i = 0; i <  m_aryMaxRecord.GetSize(); i++) {
		pcmb->AddString(m_aryMaxRecord.GetAt(i));
	}
 	//// MP3コーデック
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_CODEC);
	int intCnt = m_oAcmMgr.GetDriverList(m_aryCodecName);
	if (intCnt > 0) {
		for(i = 0; i < intCnt; i++) {
			pcmb->AddString(m_aryCodecName.GetAt(i));
		}
 		//// MP3コーデックコンボボックスを選択
		str = m_rgCodecName;
		for(i = 0; i <  m_aryCodecName.GetSize(); i++) {
			if (str == m_aryCodecName.GetAt(i)) {
				m_idxCodecName = i;
				break;
			}
		}
		str = m_aryCodecName.GetAt(m_idxCodecName);
		str.MakeUpper();
		if (str.Find(ACM_CODEC_MP3_LAME) >= 0) {
			m_oAcmMgr.FindDriver(CAcmMgr::WAV_MP3_LAME, str);
		} else {
			m_oAcmMgr.FindDriver(CAcmMgr::WAV_MP3, str);
		}
 		//// MP3フォーマット
		pcmb = (CComboBox*)GetDlgItem(IDC_CMB_FORMAT);
		CWave wv;
		wv.BuildFormat(1, 16000, 16);	//monoral
		m_wfx = wv.GetFormat();
		intCnt = m_oAcmMgr.GetFormatList(m_aryFormatName, &m_wfx);
		if (intCnt > 0) {
			for(i = 0; i < intCnt; i++) {
				pcmb->AddString(m_aryFormatName.GetAt(i));
			}
 			//// MP3フォーマットコンボボックスを選択
			str = m_rgFormatName;
			for(i = 0; i <  m_aryFormatName.GetSize(); i++) {
				if (str == m_aryFormatName.GetAt(i)) {
					m_idxFormatName = i;
					break;
				}
			}
		}
	} else {
		//// MP3コーデックが存在しない場合、強制的にWAVを選択
		m_rgFormatType = 0;	// WAV
		//// MP3を選択不可に
		CButton *rd = (CButton *)GetDlgItem(IDC_RDO_MP3);
		rd->EnableWindow(0);
	}

	//// 待機時間コンボボックスを選択
	dw = m_rgWaitAnswer;
	for(i = 0; i <  m_aryWaitAnswer.GetSize(); i++) {
		str = m_aryWaitAnswer.GetAt(i);
		if (dw == (DWORD)_ttoi(str)) {
			m_idxWaitAnswer = i;
			break;
		}
	}
 	//// 録音時間コンボボックスを選択
	dw = m_rgMaxRecord;
	dw /= 60;	// sec -> min
	for(i = 0; i <  m_aryMaxRecord.GetSize(); i++) {
		str = m_aryMaxRecord.GetAt(i);
		if (dw == (DWORD)_ttoi(str)) {
			m_idxMaxRecord = i;
			break;
		}
	}

 	//// WAV or MP3を選択
	dw = m_rgFormatType;
	CButton *rd_wav = (CButton *)GetDlgItem(IDC_RDO_WAV);
	CButton *rd_mp3 = (CButton *)GetDlgItem(IDC_RDO_MP3);
	if (dw == 1) {
		rd_wav->SetCheck(0);
		rd_mp3->SetCheck(1);
	} else {
		rd_wav->SetCheck(1);
		rd_mp3->SetCheck(0);
	}

/*
	//// ショートカット
	dw = m_rgShortCutTapur;
	m_blShortCutTapur = (dw == 1) ? TRUE : FALSE;
	dw = m_rgShortCutSkype;
	m_blShortCutSkype = (dw == 1) ? TRUE : FALSE;
*/

	UpdateData(FALSE);	// DDX更新
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*
void CSettingPubTab::OnSetfocusWaitAnswer()
{
	/// IMEを半角英数字に変更
	m_hImc = ImmGetContext(m_hWnd); 
	ImmGetConversionStatus(m_hImc, &m_dwImeConv, &m_dwImeSent); 
	ImmSetConversionStatus(m_hImc, m_dwImeConv, IME_SMODE_NONE);
}

void CSettingPubTab::OnKillfocusWaitAnswer()
{
	/// IMEの状態復帰
	ImmSetConversionStatus(m_hImc, m_dwImeConv, m_dwImeSent); 
	ImmReleaseContext(m_hWnd, m_hImc);
}

void CSettingPubTab::OnSetfocusMaxRecord()
{
	/// IMEを半角英数字に変更
	m_hImc = ImmGetContext(m_hWnd); 
	ImmGetConversionStatus(m_hImc, &m_dwImeConv, &m_dwImeSent); 
	ImmSetConversionStatus(m_hImc, m_dwImeConv, IME_SMODE_NONE);
}

void CSettingPubTab::OnKillfocusMaxRecord()
{
	/// IMEの状態復帰
	ImmSetConversionStatus(m_hImc, m_dwImeConv, m_dwImeSent); 
	ImmReleaseContext(m_hWnd, m_hImc);
}
*/

/*
void CSettingPubTab::OnChkDisable() 
{
	
}
*/

// BEEP音の更新
void CSettingPubTab::OnBtnBeepFile() 
{
	CString str;
	CString strFile;

	/// BEEP音声ファイル参照
	CFileDialog fileDlg(TRUE, _T("wave"), _T("*.wav"), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 
		_T("wave(*.wav)|*.wav|all(*.*)|*.*||"));
	if (fileDlg.DoModal() != IDOK) return;
	strFile = fileDlg.GetPathName();

	str.LoadString(IDS_MSGBOX60);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return;

	CFileSpec fs(strFile);
	if (!fs.Exist()) {
		str.LoadString(IDS_MSGBOX69);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}

	/// ファイルのフォーマットチェック
	CAudio *pAudio = CAudio::Instance();
	pAudio->m_PlayFile = strFile;
	BOOL blRet = pAudio->ChkPlayFile();
	if (!blRet) {
		str.LoadString(IDS_MSGBOX63);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}

	m_strBeepFile = strFile;

	/// 適用ボタンの有効化
	OnChgForm();
}

// MP3コーデックの変更→フォーマットリスト更新
void CSettingPubTab::OnCmbCodec() 
{
	UpdateData(TRUE);	// DDX更新

	int i;

	/// MP3コーデックを選択
	CString str = m_aryCodecName.GetAt(m_idxCodecName);
	str.MakeUpper();
	if (str.Find(ACM_CODEC_MP3_LAME) >= 0) {
		m_oAcmMgr.FindDriver(CAcmMgr::WAV_MP3_LAME, str);
	} else {
		m_oAcmMgr.FindDriver(CAcmMgr::WAV_MP3, str);
	}

	/// MP3フォーマットリストを更新
	int intCnt = m_oAcmMgr.GetFormatList(m_aryFormatName, &m_wfx);
	if (intCnt == 0 && str.Find(ACM_CODEC_MP3) >= 0) {
		//// FraunHoffer MP3コーデックが使用できない場合（Vista）
		CAudio *pAudio = CAudio::Instance();
		pAudio->MP3CodecRegistry();
	}

	/// MP3フォーマットコンボボックスを更新
	CComboBox *pcmb = (CComboBox*)GetDlgItem(IDC_CMB_FORMAT);
	pcmb->ResetContent();
	for(i = 0; i < intCnt; i++) {
		pcmb->AddString(m_aryFormatName.GetAt(i));
	}
 	/// MP3フォーマットコンボボックスを選択
	m_idxFormatName = 0;

	UpdateData(FALSE);	// DDX更新

	/// 適用ボタンの有効化
	OnChgForm();
}

// MP3の選択：MP3コーデックが使用できない場合（Vista）のチェック
void CSettingPubTab::OnRdoMp3() 
{
	UpdateData(TRUE);	// DDX更新

	/// MP3コーデックを選択
	CString str = m_aryCodecName.GetAt(m_idxCodecName);
	str.MakeUpper();

	/// MP3フォーマットリストを更新
	int intCnt = m_oAcmMgr.GetFormatList(m_aryFormatName, &m_wfx);
	if (intCnt == 0 && str.Find(ACM_CODEC_MP3) >= 0) {
		//// FraunHoffer MP3コーデックが使用できない場合（Vista）
		CAudio *pAudio = CAudio::Instance();
		pAudio->MP3CodecRegistry();
	}

	UpdateData(FALSE);	// DDX更新

	/// 適用ボタンの有効化
	OnChgForm();
}

// フォーム入力イベント
void CSettingPubTab::OnChgForm() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	/// 適用ボタンの有効化
	CWnd *cWnd;
	cWnd = GetParentOwner();
	CSettingDialog *pDlg = (CSettingDialog *)cWnd;
	pDlg->EnableApply();
}

/////////////////////////////////////////////////////////////////////////////
// local function

//
//  機能     : レジストリへ保存
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
void CSettingPubTab::Save() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CString str;

	UpdateData(TRUE);	// DDX更新

	/// BEEP音声ファイルのコピー
	if (!m_strBeepFile.IsEmpty()) {
		/// 現在のBEEP音声ファイルのバックアップ
		CTPsetup tps(theApp.m_strDataFolder);
		CString strFile = tps.RsfBeepFileName();
		CString strTmpFile = tps.RsfTmpFileName(_T("wav"));
		CFileSpec fs(strFile);
		fs.FileCopy(strTmpFile, TRUE);
		/// BEEP音声ファイルのコピー(16KHzフォーマットへ変換)
		AUDIO_FORMAT fmt;
		fmt.nBits = 16;
		fmt.nChannels = 1;
		fmt.nFrequency = 16000;
		CAudio *pAudio = CAudio::Instance();
		if (pAudio->ChgWaveFormat(m_strBeepFile, strFile, fmt, 1)) {
			str = tps.RsfBeepFileName(_T("1"));
			fs.FileCopy(str, TRUE);
			str = tps.RsfBeepFileName(_T("2"));
			fs.FileCopy(str, TRUE);
		} else {
			str.LoadString(IDS_MSGBOX69);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
			//// バックアップファイルを戻す
			fs.SetFullSpec(strTmpFile);
			if (fs.Exist()) {
				fs.FileCopy(strFile, TRUE);
			}
		}
	}

	// レジストリ更新
	//// Startup
	if (m_blStartup) {
		// tapmgr.exeをスタートアップレジストリに登録
		CFileSpec fs(FS_APPDIR);
		fs.SetFileName(_T("tapmgr.exe"));
		m_rgStartup = fs.GetFullSpec();
	} else {
		// スタートアップレジストリを削除
		m_rgStartup.removeValue();
	}
	//// 起動時無効
	m_rgDisable = (m_blDisable) ? 1 : 0;
	//// 待機時間
	str = m_aryWaitAnswer.GetAt(m_idxWaitAnswer);
	m_rgWaitAnswer = (DWORD)_ttoi(str);
 	//// 録音時間
	str = m_aryMaxRecord .GetAt(m_idxMaxRecord);
	DWORD dw = (DWORD)_ttoi(str);
	dw *= 60;	// min -> sec
	m_rgMaxRecord = dw;
 	//// WAV or MP3
	CButton *rd_mp3 = (CButton *)GetDlgItem(IDC_RDO_MP3);
	if (rd_mp3->GetCheck()) {
		m_rgFormatType = 1;
	} else {
		m_rgFormatType = 0;
	}
 	//// MP3コーデック
	if (m_aryCodecName.GetCount() > 0) {
		str = m_aryCodecName.GetAt(m_idxCodecName);
		m_rgCodecName = str;
	}
 	//// MP3フォーマット
	if (m_aryFormatName.GetCount() > 0) {
		str = m_aryFormatName.GetAt(m_idxFormatName);
		m_rgFormatName = str;
	}
/*
	//// ショートカット
	m_rgShortCutTapur = (m_blShortCutTapur) ? 1 : 0;
	m_rgShortCutSkype = (m_blShortCutSkype) ? 1 : 0;
*/
}


BOOL CSettingPubTab::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	SHORT rc;
	CWnd *cWnd;

    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		/// TAB KEY : 上位フォームへフォーカス遷移
		if (nVirtKey == VK_TAB) {
			//// フォーカス最後の場合のみ有効
			cWnd = GetFocus();
			rc = ::GetKeyState(VK_CONTROL);
			if( ( rc & 0x8000 ) != 0 ) {
				cWnd = cWnd->GetParentOwner();
				CSettingDialog *pDlg = (CSettingDialog *)cWnd;
				CTabCtrlSSL *pTabCtrl = &(pDlg->m_tabCtrl);
				int pageid = pTabCtrl->GetSSLActivePage();
				int idx = pTabCtrl->GetSSLPageIndex(pageid);
				int idxMax = pTabCtrl->GetSSLPageCount();
				idx = (idx == (idxMax-1)) ? 0 : idx+1;
				pTabCtrl->ActivateSSLPage(idx);
				return TRUE;
			} else {
				//// フォーカス最後のコントロールのみ有効
				if (cWnd->m_hWnd == GetDlgItem(IDC_CMB_FORMAT)->m_hWnd) {
					cWnd = cWnd->GetParentOwner();
					cWnd->SetFocus();
					return TRUE;
				}
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
