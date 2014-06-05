// SettingVRecTab.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "SettingVRecTab.h"
#include "SettingDialog.h"
#include "SetChatDialog.h"
#include "TabCtrlSSL.h"
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
// CSettingVRecTab ダイアログ


CSettingVRecTab::CSettingVRecTab(CWnd* pParent /*=NULL*/)
	: CTabPageSSL (CSettingVRecTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingVRecTab)
	m_idxVRecordLimit = 0;
	m_idxSepSize = 0;
	m_idxVCmpName = 0;
	m_strFps = _T("");
	m_blVRecWarning = TRUE;
	m_blVRecStopWarning = TRUE;
	m_blVSetTop = TRUE;
	m_blNoOverlay = TRUE;
	m_blSep = TRUE;
	m_blAudio = TRUE;
	//}}AFX_DATA_INIT
}


void CSettingVRecTab::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingVRecTab)
	DDX_CBIndex(pDX, IDC_CMB_VRECORD_LIMIT, m_idxVRecordLimit);
	DDX_Check(pDX, IDC_CHK_VREC_WARNING, m_blVRecWarning);
	DDX_Control(pDX, IDC_BTN_VREC_WARNING, m_btnVRecWarning);
	DDX_Check(pDX, IDC_CHK_VRECSTOP_WARNING, m_blVRecStopWarning);
	DDX_Control(pDX, IDC_BTN_VRECSTOP_WARNING, m_btnVRecStopWarning);
	DDX_Check(pDX, IDC_CHK_VSETTOP, m_blVSetTop);
	DDX_Check(pDX, IDC_CHK_OVERLAY, m_blNoOverlay);
	DDX_Check(pDX, IDC_CHK_SEP, m_blSep);
	DDX_CBIndex(pDX, IDC_CMB_SEP, m_idxSepSize);
	DDX_Check(pDX, IDC_CHK_AUDIO, m_blAudio);
	DDX_CBIndex(pDX, IDC_CMB_CMP, m_idxVCmpName);
	DDX_CBString(pDX, IDC_CMB_FPS, m_strFps);
	DDX_Control(pDX, IDC_STATIC_LINK_LICENSE, m_LinkLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingVRecTab, CDialog)
	//{{AFX_MSG_MAP(CSettingVRecTab)
	ON_BN_CLICKED(IDC_CHK_VREC_WARNING, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_VRECSTOP_WARNING, OnChgForm)
	ON_BN_CLICKED(IDC_BTN_VREC_WARNING, OnBtnVRecWarning)
	ON_BN_CLICKED(IDC_BTN_VRECSTOP_WARNING, OnBtnVRecStopWarning)
	ON_CBN_SELCHANGE(IDC_CMB_VRECORD_LIMIT, OnChgForm)
	ON_CBN_SELCHANGE(IDC_CMB_SEP, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_VSETTOP, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_OVERLAY, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_SEP, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_AUDIO, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_NOCMP, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_CMP, OnChgForm)
	ON_CBN_SELCHANGE(IDC_CMB_CMP, OnChgForm)
	ON_CBN_SELCHANGE(IDC_CMB_FPS, OnChgForm)
	ON_CBN_EDITCHANGE(IDC_CMB_FPS, OnChgFps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingVRecTab メッセージ ハンドラ

BOOL CSettingVRecTab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	int i;
	CString str;
	DWORD dw;

	/// コンボボックス初期化
	CComboBox *pcmb;
	//// 録画時間
	m_aryVRecordLimit.Add(_T("1"));
    m_aryVRecordLimit.Add(_T("2"));
    m_aryVRecordLimit.Add(_T("3"));
    m_aryVRecordLimit.Add(_T("5"));
    m_aryVRecordLimit.Add(_T("10"));
    m_aryVRecordLimit.Add(_T("15"));
    m_aryVRecordLimit.Add(_T("20"));
    m_aryVRecordLimit.Add(_T("30"));
    m_aryVRecordLimit.Add(_T("45"));
    m_aryVRecordLimit.Add(_T("60"));
    m_aryVRecordLimit.Add(_T("90"));
    m_aryVRecordLimit.Add(_T("120"));
    m_aryVRecordLimit.Add(_T("180"));
    m_aryVRecordLimit.Add(_T("240"));
    m_aryVRecordLimit.Add(_T("360"));
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_VRECORD_LIMIT);
	for(i = 0; i <  m_aryVRecordLimit.GetSize(); i++) {
		pcmb->AddString(m_aryVRecordLimit.GetAt(i));
	}
	//// 録画サイズ
	m_arySepSize.Add(_T("1"));
    m_arySepSize.Add(_T("2"));
    m_arySepSize.Add(_T("4"));
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_SEP);
	for(i = 0; i <  m_arySepSize.GetSize(); i++) {
		pcmb->AddString(m_arySepSize.GetAt(i));
	}
	//// FPS
	m_aryFps.Add(_T("5"));
    m_aryFps.Add(_T("10"));
    m_aryFps.Add(_T("15"));
    m_aryFps.Add(_T("25"));
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_FPS);
	for(i = 0; i <  m_aryFps.GetSize(); i++) {
		pcmb->AddString(m_aryFps.GetAt(i));
	}
	//// 圧縮形式
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_CMP);
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CVideo *pVideo = pFrame->m_pVideo;
	//AssignListToComboBox(pcmb, pVideo->GetCompressors(), pVideo->CountCompressor());
	CStringList oStrList;
	pVideo->GetCompressors(&oStrList);
	AssignListToComboBox(pcmb, &oStrList);
	int intCnt = pcmb->GetCount();
	m_aryVCmpName.SetSize(intCnt);
	for(i = 0; i < pcmb->GetCount(); i++) {
		pcmb->GetLBText(i, str);
		m_aryVCmpName.SetAt(i, str);
	}

	/// レジストリ読み込み
	//// 文字列レジストリでは空白入力が無視される場合があるのでforce=TRUEに
	m_rgEnableVRecWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVideoRecordWarning"), 1, TRUE);	// 初期値:1(True)
	m_rgEnableVRecStopWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVideoRecordStopWarning"), 1, TRUE);	// 初期値:1(True)
 	//// 録画時警告チャット
	dw = m_rgEnableVRecWarning;
	m_blVRecWarning = (dw == 1) ? TRUE : FALSE;
	dw = m_rgEnableVRecStopWarning;
	m_blVRecStopWarning = (dw == 1) ? TRUE : FALSE;
	//// ビデオ画面を前面表示
	m_rgVSetTop = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordSetTop"), 1, TRUE);	// 初期値:1(True)
	dw = m_rgVSetTop;
	m_blVSetTop = (dw == 1) ? TRUE : FALSE;
	//// オーバーレイ録画
	m_rgOverlay = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordOverlay"), 1, TRUE);	// 初期値:1(オーバーレイする)
	dw = m_rgOverlay;
	m_blNoOverlay = (dw == 1) ? FALSE : TRUE;
	//// ファイル分割保存
	m_rgSep = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordSep"), 1, TRUE);	// 初期値:1(分割する)
	dw = m_rgSep;
	m_blSep = (dw == 1) ? TRUE : FALSE;
	//// 音声結合
	m_rgAudio = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordWithAudio"), 1, TRUE);	// 初期値:1(音声付録画)
	dw = m_rgAudio;
	m_blAudio = (dw == 1) ? TRUE : FALSE;
 	//// 非圧縮 or 圧縮
	m_rgVCmp = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordCompress"), 1, TRUE);		// 初期値:1(圧縮)
	m_rgVCmpName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordCompressName"), _T("Lagarith lossless codec"), TRUE);		// 初期値:"Lagarith"
 	//// FPS
	m_rgFps = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordFps"), 5, TRUE);			// 初期値:5(fps)

	/// レジストリ値によりコンボボックスを選択
 	//// 録画時間時間コンボボックスを選択
	m_rgVRecordLimit = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VRecordLimitTime"), 600, TRUE);	// 初期値:10min
	dw = m_rgVRecordLimit;
	dw /= 60;	// sec -> min
	for(i = 0; i <  m_aryVRecordLimit.GetSize(); i++) {
		str = m_aryVRecordLimit.GetAt(i);
		if (dw == (DWORD)_ttoi(str)) {
			m_idxVRecordLimit = i;
			break;
		}
	}
 	//// 録画サイズコンボボックスを選択
	m_rgSepSize = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordSepSize"), 2000, TRUE);	// 初期値:2000Mbyte
	dw = m_rgSepSize;
	dw /= 1000;	// Mbyte -> Gbyte
	for(i = 0; i <  m_arySepSize.GetSize(); i++) {
		str = m_arySepSize.GetAt(i);
		if (dw == (DWORD)_ttoi(str)) {
			m_idxSepSize = i;
			break;
		}
	}
 	//// 圧縮形式コンボボックスを選択
	str = m_rgVCmpName;
	str.MakeUpper();
	for(i = 0; i <  m_aryVCmpName.GetSize(); i++) {
		CString strName = m_aryVCmpName.GetAt(i);
		strName.MakeUpper();
		if (strName.Find(str) >= 0) {
			m_idxVCmpName = i;
			break;
		}
	}
 	//// FPSコンボボックスを選択
	dw = m_rgFps;
	m_strFps.Format(_T("%d"), dw);

 	//// 非圧縮 or 圧縮を選択
	dw = m_rgVCmp;
	// ラジオボタンの参照定義（DDXは扱いにくいので）
	// （クラスメンバーで宣言すると正常に動作しない）
	CButton *rd_nocmp = (CButton *)GetDlgItem(IDC_RDO_NOCMP);
	CButton *rd_cmp = (CButton *)GetDlgItem(IDC_RDO_CMP);
	if (dw > 0) {
		rd_nocmp->SetCheck(0);
		rd_cmp->SetCheck(1);
	} else {
		rd_nocmp->SetCheck(1);
		rd_cmp->SetCheck(0);
	}

	//// ライセンスリンク
	m_LinkLine.SetTextColor(RGB(0,0,255));
	m_LinkLine.SetFontUnderline(TRUE);
	str.LoadString(IDS_URL_LICENSE);
	m_LinkLine.SetHyperLink(str);
	HCURSOR cursor = AfxGetApp()->LoadCursor(IDC_LINK);
	m_LinkLine.SetLinkCursor(cursor);
	m_LinkLine.SetLink(TRUE, FALSE);

	UpdateData(FALSE);	// DDX更新
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// 録画時警告メッセージの設定
void CSettingVRecTab::OnBtnVRecWarning() 
{
	UpdateData(TRUE);	// DDX更新

	CSetChatDialog dlg;
	dlg.SetRegKey(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordWarning"));
	if (dlg.DoModal() != IDOK) return;
}
void CSettingVRecTab::OnBtnVRecStopWarning() 
{
	UpdateData(TRUE);	// DDX更新

	CSetChatDialog dlg;
	dlg.SetRegKey(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordStopWarning"));
	if (dlg.DoModal() != IDOK) return;
}

// FPS変更チェックイベント
void CSettingVRecTab::OnChgFps() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString strOld = m_strFps;
	UpdateData(TRUE);	// DDX更新

	// 整数のみ有効
	if (!ChkNaturalNumber(m_strFps)) {
		m_strFps = strOld;
		UpdateData(FALSE);	// DDX更新
		return;
	}

	UpdateData(FALSE);	// DDX更新

	/// 適用ボタンの有効化
	OnChgForm();
}

// フォーム入力イベント
void CSettingVRecTab::OnChgForm() 
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
//  機能     : 改行付き文字列のComboBoxへのリスト登録
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
//void CSettingVRecTab::AssignListToComboBox(CComboBox *Dest, CString Source, long Indx, long MaxIndxLimit) 
//{
//    Dest->ResetContent();
//
//	long MaxIndxCount = (MaxIndxLimit>0) ? MaxIndxLimit : 99999;
//	long Startpos = 0;
//	long Endpos = Source.Find(_T("\r"), Startpos);
//
//	while ((Endpos >= 0) && (MaxIndxCount > 0))
//	{
//		Dest->AddString(Source.Mid(Startpos, Endpos - Startpos));
//		Startpos = Endpos + 2;	// skip CR/LF
//		Endpos = Source.Find(_T("\r"), Startpos);	// InStr(Startpos, Source, vbCr)
//		MaxIndxCount--;
//	}
//
//	if (Indx < Dest->GetCount())
//		Dest->SetCurSel(Indx);
//}
void CSettingVRecTab::AssignListToComboBox(CComboBox *Dest, CStringList *Source) 
{
    Dest->ResetContent();

	while (Source->GetCount() > 0) {
		POSITION pos = Source->GetHeadPosition();
		if (pos == NULL)  break;
		Dest->AddString(Source->GetAt(pos));
		Source->RemoveAt(pos);
	}

	if (Dest->GetCount() > 0)
		Dest->SetCurSel(0);
}

//
//  機能     : レジストリへ保存
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
void CSettingVRecTab::Save() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CString str;
	DWORD dw;

	UpdateData(TRUE);	// DDX更新

	/// レジストリ更新
 	//// 録画時警告メッセージ
	m_rgEnableVRecWarning = (m_blVRecWarning) ? 1 : 0;
	m_rgEnableVRecStopWarning = (m_blVRecStopWarning) ? 1 : 0;
	//// ビデオ画面を前面表示
	m_rgVSetTop = (m_blVSetTop) ? 1 : 0;
	//// オーバーレイ録画
	m_rgOverlay = (m_blNoOverlay) ? 0 : 1;
	//// ファイル分割保存
	m_rgSep = (m_blSep) ? 1 : 0;
	str = m_arySepSize.GetAt(m_idxSepSize);
	dw = (DWORD)_ttoi(str);
	dw *= 1000;	// Gbyte -> Mbyte
	m_rgSepSize = dw;
	//// 音声結合
	m_rgAudio = (m_blAudio) ? 1 : 0;
 	//// 録画制限時間
	str = m_aryVRecordLimit.GetAt(m_idxVRecordLimit);
	dw = (DWORD)_ttoi(str);
	dw *= 60;	// min -> sec
	m_rgVRecordLimit = dw;
 	//// 非圧縮 or 圧縮
	// ラジオボタンの参照定義（DDXは扱いにくいので）
	// （クラスメンバーで宣言すると正常に動作しない）
	CButton *rd_cmp = (CButton *)GetDlgItem(IDC_RDO_CMP);
	if (rd_cmp->GetCheck()) {
		m_rgVCmp = 1;
	} else {
		m_rgVCmp = 0;
	}
 	//// 圧縮形式
	str = m_aryVCmpName.GetAt(m_idxVCmpName);
	m_rgVCmpName = str;
 	//// FPS
	m_rgFps = (DWORD)_ttoi(m_strFps);
}


BOOL CSettingVRecTab::PreTranslateMessage(MSG* pMsg) 
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
				if (cWnd->m_hWnd == GetDlgItem(IDC_CMB_CMP)->m_hWnd) {
					cWnd = cWnd->GetParentOwner();
					cWnd->SetFocus();
					return TRUE;
				}
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
