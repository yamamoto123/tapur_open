// RsfNewDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfNewDialog.h"
#include "RsfTree.h"
#include "RsfList.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "TPutil.h"
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
// CRsfNewDialog ダイアログ


CRsfNewDialog::CRsfNewDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRsfNewDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRsfNewDialog)
	m_idxId = 0;
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_strId = _T("");
}

void CRsfNewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfNewDialog)
	DDX_CBIndex(pDX, IDC_CMB_ID, m_idxId);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRsfNewDialog, CDialog)
	//{{AFX_MSG_MAP(CRsfNewDialog)
	ON_CBN_SELCHANGE(IDC_CMB_ID, OnSelchangeId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfNewDialog メッセージ ハンドラ


BOOL CRsfNewDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_aryHandle.RemoveAll();
	m_aryFullName.RemoveAll();
    m_pcmb = (CComboBox*)GetDlgItem(IDC_CMB_ID);
	m_pcmb->LimitText(32);
	
	/// コンタクトリストよりコンボボックスリスト作成
	if (theApp.m_blSkypeConnect) {	// Skypeの状態確認
		long i, lngCnt;
		IUserPtr pUsr;
		_variant_t vt;
		_bstr_t bStr;
		CString str;
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CRsfList *pfview = (CRsfList *)pFrame->m_pRsfList->GetWnd();
		IUserCollectionPtr pUsrCol = theApp.m_pAccess->GetFriendList();
		if (pUsrCol != NULL) {
			lngCnt = pUsrCol->GetCount();
			if (lngCnt > 0) {
				for(i = 1; i <= lngCnt; i++) {
					vt = i;
					pUsr = pUsrCol->GetItem(vt);
					if (pUsr->GetIsSkypeOut()) continue;
					bStr = pUsr->GetHandle();
					str.Format(_T("%s"), (LPCTSTR)bStr);
					/// 既存設定と重複するものは除外
					if (!pfview->ChkSetup(str)) continue;
					m_pcmb->AddString(str);
					m_aryHandle.Add(str);
					bStr = pUsr->GetFullName();
					str.Format(_T("%s"), (LPCTSTR)bStr);
					m_aryFullName.Add(str);
				}
			}
		}
	}

	/// 初期IDが指定された場合
	if (!m_strId.IsEmpty()) m_pcmb->SetWindowText(m_strId);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// コンボボックスの入力制限
BOOL CRsfNewDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	if(pMsg->message== WM_CHAR && (GetFocus()->GetParent())->GetDlgCtrlID()==IDC_CMB_ID) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		if (!(ChkSkypeId(CString((TCHAR)pMsg->wParam)) || 
			(nVirtKey >= VK_LBUTTON && nVirtKey <= VK_NUMLOCK))) return(TRUE); 
	} 	

	return CDialog::PreTranslateMessage(pMsg);
}

void CRsfNewDialog::OnSelchangeId()
{
	int i;
	CString strId;

	UpdateData(TRUE);	// DDX更新

	m_strName.Empty();
	m_pcmb->GetLBText(m_idxId, strId);
	for(i = 0; i < m_aryHandle.GetSize(); i++) {
		if (m_aryHandle.GetAt(i) == strId) {
			m_strName = m_aryFullName.GetAt(i);
			break;
		}
	}

	UpdateData(FALSE);	// DDX更新
}

void CRsfNewDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CString str;
	CString strId;

	if (!UpdateData(TRUE)) return;	// DDX更新

	/// スカイプ名のチェック
	m_pcmb->GetWindowText(strId);
	if (strId.IsEmpty()) {
		str.LoadString(IDS_MSGBOX13);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return;
	}
	if (!ChkSkypeId(strId)) {
		str.LoadString(IDS_MSGBOX12);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return;
	}
	// 重複チェック
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfList *pmv = (CRsfList *)pFrame->m_pRsfList->GetWnd();
	if (!pmv->ChkSetup(strId)) {
		str.LoadString(IDS_MSGBOX41);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return;
	}

	/// 応答設定作成（音声ファイル、設定ファイル、テーブル）
	CFileSpec fs;
	CString strPath, strOrgPath;	
	CTPsetup tps(theApp.m_strDataFolder, strId);
	//// 初期応答ファイル作成
	strPath = tps.RsfFileName();
	fs.SetFullSpec(strPath);
	if (!tps.SaveRsfFile()){
		TRACE(_T("can't create response file.\n"));
		return;
	}
	//// ダミーとして初期応答音声ファイル使用
	strPath = tps.RsfAudioFileName(_T("$"));
	///// 初期応答音声ファイルのコピー
	tps.SetCallerId(_T("0"));
	strOrgPath = tps.RsfAudioFileName(_T("$"));
	fs.SetFullSpec(strOrgPath);
	if (!fs.FileCopy(strPath, TRUE)) {
		TRACE(_T("can't create response audio file:%s\n"), strPath);
		return;
	}
	//// rsflテーブルに追加
	///// 選択フォルダ情報
	CRsfTree *pfv = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	CTblRsfl tbl(theApp.m_strDBPath);
	tbl.strSenderId = strId;		// 送信者名（スカイプ名）
	tbl.strSenderName = m_strName;	// 送信者名（表示名）
	tbl.strReceiverId = theApp.m_strUsrId;	// 受信者名（スカイプ名）
	tbl.strReceiverName = theApp.m_strUsrName;	// 受信者名（表示名）
	tbl.lngRespType = RESPBASE_TYPE_SIMPLE;
	if (pfv->m_dwFolderType == FD_NORMAL) {
		///// カレントフォルダが（デフォルト以外の）設定フォルダの場合のみフォルダ指定
		tbl.lngFolderId = pfv->m_dwFolderId;
	}
// (?) コンテキストからの呼び出しだとトランザクションが機能しない
	tbl.m_blTransaction = TRUE;
	BOOL ret = tbl.InsertRec();
	tbl.m_blTransaction = FALSE;
	if (ret) {
		/// 生成した設定を表示
		pmv->SelSetup(strId);
	}

	CDialog::OnOK();
}

void CRsfNewDialog::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function


