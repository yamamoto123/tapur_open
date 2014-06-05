// SettingRecTab.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "SettingRecTab.h"
#include "SettingDialog.h"
#include "SetChatDialog.h"
#include "TabCtrlSSL.h"
#include "Audio.h"
#include "TPutil.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingRecTab ダイアログ


CSettingRecTab::CSettingRecTab(CWnd* pParent /*=NULL*/)
	: CTabPageSSL (CSettingRecTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingRecTab)
	m_idxCodecName = 0;
	m_idxFormatName = 0;
	m_idxRecordLimit = 0;
	m_blRecWarning = TRUE;
	m_blRecStopWarning = TRUE;
	m_blEnableAuto = TRUE;
	//}}AFX_DATA_INIT
}


void CSettingRecTab::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingRecTab)
	DDX_CBIndex(pDX, IDC_CMB_CODEC, m_idxCodecName);
	DDX_CBIndex(pDX, IDC_CMB_FORMAT, m_idxFormatName);
	DDX_CBIndex(pDX, IDC_CMB_RECORD_LIMIT, m_idxRecordLimit);
	DDX_Check(pDX, IDC_CHK_REC_WARNING, m_blRecWarning);
	DDX_Control(pDX, IDC_BTN_REC_WARNING, m_btnRecWarning);
	DDX_Check(pDX, IDC_CHK_RECSTOP_WARNING, m_blRecStopWarning);
	DDX_Control(pDX, IDC_BTN_RECSTOP_WARNING, m_btnRecStopWarning);
	DDX_Check(pDX, IDC_CHK_AUTO_ENABLE, m_blEnableAuto);
    DDX_Text(pDX, IDC_EDIT_VOL_SND, m_strVolSnd);
    DDX_Text(pDX, IDC_EDIT_VOL_RCV, m_strVolRcv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingRecTab, CDialog)
	//{{AFX_MSG_MAP(CSettingRecTab)
	ON_BN_CLICKED(IDC_CHK_REC_WARNING, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_RECSTOP_WARNING, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_AUTO_ENABLE, OnChgForm)
	ON_BN_CLICKED(IDC_BTN_REC_WARNING, OnBtnRecWarning)
	ON_BN_CLICKED(IDC_BTN_RECSTOP_WARNING, OnBtnRecStopWarning)
	ON_BN_CLICKED(IDC_RDO_MONORAL, OnRdoStereo)
	ON_BN_CLICKED(IDC_RDO_STEREO, OnRdoStereo)
	ON_BN_CLICKED(IDC_RDO_STEREO2, OnRdoStereo)
	ON_CBN_SELCHANGE(IDC_CMB_RECORD_LIMIT, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_WAV, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_MP3, OnRdoMp3)
	ON_CBN_SELCHANGE(IDC_CMB_CODEC, OnCmbCodec)
	ON_CBN_SELCHANGE(IDC_CMB_FORMAT, OnChgForm)
	ON_EN_CHANGE(IDC_EDIT_VOL_SND, OnChgVolSnd)
	ON_EN_CHANGE(IDC_EDIT_VOL_RCV, OnChgVolRcv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingRecTab メッセージ ハンドラ

BOOL CSettingRecTab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	int i;
	int intCnt;
	CString str;
	DWORD dw;

	/// レジストリ読み込み
	//// 文字列レジストリでは空白入力が無視される場合があるのでforce=TRUEに
	m_rgEnableRecWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVoiceRecordWarning"), 1, TRUE);	// 初期値:1(True)
	m_rgEnableRecStopWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVoiceRecordStopWarning"), 1, TRUE);	// 初期値:1(True)
	m_rgEnableAuto = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableAutoVoiceRecord"), 0, TRUE);	// 初期値:0(False)
	m_rgVolSnd = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordVolSnd"), 15, TRUE);	// 初期値:15%
	m_rgVolRcv = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordVolRcv"), 0, TRUE);		// 初期値:0%

	/// 録音時警告チャット
	dw = m_rgEnableRecWarning;
	m_blRecWarning = (dw == 1) ? TRUE : FALSE;
	dw = m_rgEnableRecStopWarning;
	m_blRecStopWarning = (dw == 1) ? TRUE : FALSE;
 	/// 録音制限時間
	m_rgRecordLimit = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\RecordLimitTime"), 3600);		// 初期値:60min
 	/// monoral or stereo（完全パン）or  stereo（調整パン）
	m_rgStereo = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordStereo"), 1, TRUE);		// 初期値:stereo ??force=TRUEでないとレジストリが生成されない
 	/// WAV or MP3
	m_rgFormatType = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordType"), 0, TRUE);		// 初期値:0(WAV)
	m_rgCodecName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordMP3Codec"), _T(""), TRUE);
	m_rgFormatName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordMP3Format"), _T(""), TRUE);
 	/// 自動録音
	dw = m_rgEnableAuto;
	m_blEnableAuto = (dw == 1) ? TRUE : FALSE;
	/// 音量調節
	dw = m_rgVolSnd;
	m_strVolSnd.Format(_T("%d"), dw);
	dw = m_rgVolRcv;
	m_strVolRcv.Format(_T("%d"), dw);

	/// コンボボックス初期化
	CComboBox *pcmb;
 	//// 録音時間
    m_aryRecordLimit.Add(_T("5"));
    m_aryRecordLimit.Add(_T("10"));
    m_aryRecordLimit.Add(_T("15"));
    m_aryRecordLimit.Add(_T("20"));
    m_aryRecordLimit.Add(_T("30"));
    m_aryRecordLimit.Add(_T("45"));
    m_aryRecordLimit.Add(_T("60"));
    m_aryRecordLimit.Add(_T("90"));
    m_aryRecordLimit.Add(_T("120"));
    m_aryRecordLimit.Add(_T("180"));
    m_aryRecordLimit.Add(_T("240"));
    m_aryRecordLimit.Add(_T("360"));
    m_aryRecordLimit.Add(_T("480"));
    m_aryRecordLimit.Add(_T("720"));
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_RECORD_LIMIT);
	for(i = 0; i < m_aryRecordLimit.GetSize(); i++) {
		pcmb->AddString(m_aryRecordLimit.GetAt(i));
	}
 	//// 録音コンボボックスを選択
	dw = m_rgRecordLimit;
	dw /= 60;	// sec -> min
	for(i = 0; i <  m_aryRecordLimit.GetSize(); i++) {
		str = m_aryRecordLimit.GetAt(i);
		if (dw == (DWORD)_ttoi(str)) {
			m_idxRecordLimit = i;
			break;
		}
	}
 	//// MP3コーデック
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_CODEC);
	intCnt = m_oAcmMgr.GetDriverList(m_aryCodecName);
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
		if (m_rgStereo == 0) {
			wv.BuildFormat(1, 16000, 16);	//monoral
		} else {
			wv.BuildFormat(2, 16000, 16);	//stereo
		}
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

 	//// monoral or stereoを選択
	dw = m_rgStereo;
	// ラジオボタンの参照定義（DDXは扱いにくいので）
	// （クラスメンバーで宣言すると正常に動作しない）
	CButton *rd_monoral = (CButton *)GetDlgItem(IDC_RDO_MONORAL);
	CButton *rd_stereo1 = (CButton *)GetDlgItem(IDC_RDO_STEREO1);
	CButton *rd_stereo2 = (CButton *)GetDlgItem(IDC_RDO_STEREO2);
	if (dw == 1) {
		rd_monoral->SetCheck(0);
		rd_stereo1->SetCheck(1);
		rd_stereo2->SetCheck(0);
	} else if (dw == 2) {
		rd_monoral->SetCheck(0);
		rd_stereo1->SetCheck(0);
		rd_stereo2->SetCheck(1);
	} else {
		rd_monoral->SetCheck(1);
		rd_stereo1->SetCheck(0);
		rd_stereo2->SetCheck(0);
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
	
	UpdateData(FALSE);	// DDX更新
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// 録音時警告メッセージの設定
void CSettingRecTab::OnBtnRecWarning() 
{
	UpdateData(TRUE);	// DDX更新

	CSetChatDialog dlg;
	dlg.SetRegKey(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordWarning"));
	if (dlg.DoModal() != IDOK) return;
}
void CSettingRecTab::OnBtnRecStopWarning() 
{
	UpdateData(TRUE);	// DDX更新

	CSetChatDialog dlg;
	dlg.SetRegKey(_T("Software\\Tapur\\Tapur\\Settings\\VoiceRecordStopWarning"));
	if (dlg.DoModal() != IDOK) return;
}

// 録音形式の変更→MP3フォーマットリスト更新
void CSettingRecTab::OnRdoStereo() 
{
	UpdateData(TRUE);	// DDX更新

	int i;

	/// MP3フォーマットリストを更新
	CWave wv;
	CButton *rd_monoral = (CButton *)GetDlgItem(IDC_RDO_MONORAL);
	if (rd_monoral->GetCheck()) {
		if (m_wfx.nChannels == 1) goto PROC_END;	// フォーマットの変更が不要
		wv.BuildFormat(1, 16000, 16);	//monoral
	} else {
		if (m_wfx.nChannels == 2) goto PROC_END;	// フォーマットの変更が不要
		wv.BuildFormat(2, 16000, 16);	//stereo
	}
	m_wfx = wv.GetFormat();
	int intCnt = m_oAcmMgr.GetFormatList(m_aryFormatName, &m_wfx);
	CComboBox *pcmb = (CComboBox*)GetDlgItem(IDC_CMB_FORMAT);
	pcmb->ResetContent();
	for(i = 0; i < intCnt; i++) {
		pcmb->AddString(m_aryFormatName.GetAt(i));
	}
 	/// MP3フォーマットコンボボックスを選択
	m_idxFormatName = 0;

	UpdateData(FALSE);	// DDX更新

PROC_END:
	/// 適用ボタンの有効化
	OnChgForm();
}

// MP3コーデックの変更→フォーマットリスト更新
void CSettingRecTab::OnCmbCodec() 
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
void CSettingRecTab::OnRdoMp3() 
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

// 音量調節変更チェックイベント
void CSettingRecTab::OnChgVolSnd() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString strOld = m_strVolSnd;
	UpdateData(TRUE);	// DDX更新

	// 整数のみ有効
	if (!ChkNumber(m_strVolSnd)) {
		m_strVolSnd = strOld;
		UpdateData(FALSE);	// DDX更新
		return;
	}
	// -400 <-> 400
	int intVal = _ttoi(m_strVolSnd);
	if (intVal < -400 || intVal > 400) {
		m_strVolSnd = strOld;
		UpdateData(FALSE);	// DDX更新
		return;
	}

	UpdateData(FALSE);	// DDX更新

	/// 適用ボタンの有効化
	OnChgForm();
}
void CSettingRecTab::OnChgVolRcv() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString strOld = m_strVolRcv;
	UpdateData(TRUE);	// DDX更新

	// 整数のみ有効
	if (!ChkNumber(m_strVolRcv)) {
		m_strVolRcv = strOld;
		UpdateData(FALSE);	// DDX更新
		return;
	}
	// -400 <-> 400
	int intVal = _ttoi(m_strVolRcv);
	if (intVal < -400 || intVal > 400) {
		m_strVolRcv = strOld;
		UpdateData(FALSE);	// DDX更新
		return;
	}

	UpdateData(FALSE);	// DDX更新

	/// 適用ボタンの有効化
	OnChgForm();
}

// フォーム入力イベント
void CSettingRecTab::OnChgForm() 
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
void CSettingRecTab::Save() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CString str;
	DWORD dw;

	UpdateData(TRUE);	// DDX更新

	/// レジストリ更新
 	//// 録音時警告メッセージ
	m_rgEnableRecWarning = (m_blRecWarning) ? 1 : 0;
	m_rgEnableRecStopWarning = (m_blRecStopWarning) ? 1 : 0;
 	//// 録音制限時間
	str = m_aryRecordLimit.GetAt(m_idxRecordLimit);
	dw = (DWORD)_ttoi(str);
	dw *= 60;	// min -> sec
	m_rgRecordLimit = dw;
 	//// monoral or stereo設定
	// ラジオボタンの参照定義（DDXは扱いにくいので）
	// （クラスメンバーで宣言すると正常に動作しない）
	CButton *rd_monoral = (CButton *)GetDlgItem(IDC_RDO_MONORAL);
	CButton *rd_stereo2 = (CButton *)GetDlgItem(IDC_RDO_STEREO2);
	if (rd_monoral->GetCheck()) {
		m_rgStereo = 0;
	} else if (rd_stereo2->GetCheck()) {
		m_rgStereo = 2;
	} else {
		m_rgStereo = 1;
	}
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
 	//// 自動録音
	m_rgEnableAuto = (m_blEnableAuto) ? 1 : 0;
	//// 音量調整
	m_rgVolSnd = (DWORD)_ttoi(m_strVolSnd);
	m_rgVolRcv = (DWORD)_ttoi(m_strVolRcv);
}


BOOL CSettingRecTab::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	SHORT rc;
	CWnd *cWnd;

    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		/// TAB KEY : 上位フォームへフォーカス遷移
		/// TAB+Ctrl KEY : コントロールタブ遷移
		if (nVirtKey == VK_TAB) {
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
