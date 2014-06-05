// RsfRenameDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfRenameDialog.h"
#include "RsfList.h"
#include "TPtable.h"
#include "TPutil.h"
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
// CRsfRenameDialog ダイアログ


CRsfRenameDialog::CRsfRenameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRsfRenameDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRsfRenameDialog)
	m_idxId = 0;
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CRsfRenameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfRenameDialog)
	DDX_CBIndex(pDX, IDC_CMB_ID, m_idxId);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRsfRenameDialog, CDialog)
	//{{AFX_MSG_MAP(CRsfRenameDialog)
	ON_CBN_SELCHANGE(IDC_CMB_ID, OnSelchangeId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfRenameDialog メッセージ ハンドラ

BOOL CRsfRenameDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_aryHandle.RemoveAll();
	m_aryFullName.RemoveAll();
    m_pcmb = (CComboBox*)GetDlgItem(IDC_CMB_ID);
	m_pcmb->LimitText(32);

	/// 選択情報
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfList *pfview = (CRsfList *)pFrame->m_pRsfList->GetWnd();
	m_lngId = pfview->m_wndList.GetItemData(pfview->m_hActive);

	/// コンタクトリストよりコンボボックスリスト作成
	long i, lngCnt;
	IUserPtr pUsr;
	_variant_t vt;
	_bstr_t bStr;
	CString str;
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

	/// 既存の送信者名を表示
	CTblRsfl tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(m_lngId) != 1) return TRUE;
	m_strOrgId = tbl.strSenderId;		// 送信者名（スカイプ名）のバックアップ
	m_strName = tbl.strSenderName;		// 送信者名（表示名）
	UpdateData(FALSE);	// DDX更新(コンボボックスTOPのスカイプ名が反映されてしまうので、この位置で実行)
	m_pcmb->SetWindowText(m_strOrgId);	// 送信者名（スカイプ名）

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// コンボボックスの入力制限
BOOL CRsfRenameDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	if(pMsg->message== WM_CHAR && (GetFocus()->GetParent())->GetDlgCtrlID()==IDC_CMB_ID) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		if (!(ChkSkypeId(CString((TCHAR)pMsg->wParam)) || 
			(nVirtKey >= VK_LBUTTON && nVirtKey <= VK_NUMLOCK))) return(TRUE); 
	} 	

	return CDialog::PreTranslateMessage(pMsg);
}

void CRsfRenameDialog::OnSelchangeId()
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

void CRsfRenameDialog::OnOK() 
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
	CRsfList *pfview = (CRsfList *)pFrame->m_pRsfList->GetWnd();
	if (m_strOrgId != strId) {
		if (!pfview->ChkSetup(strId)) {
			str.LoadString(IDS_MSGBOX11);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return;
		}
	}

	/// 応答設定作成（音声ファイル、設定ファイル、テーブル）
	if (m_strOrgId != strId) {
		CTPsetup tps(theApp.m_strDataFolder);
		//// 応答音声ファイルのRENAME
		tps.SetCallerId(m_strOrgId);
		CString strOrgPath = tps.RsfAudioFileName(_T("$"));
		tps.SetCallerId(strId);
		CString strPath = tps.RsfAudioFileName(_T("$"));
		CFileSpec fs(strOrgPath);
		if (!fs.FileMove(strPath, TRUE)) {
			return;
		}
		//// 応答設定ファイル再作成
		tps.SetCallerId(m_strOrgId);
//		strOrgPath = tps.RsfFileName();
//		fs.SetFullSpec(strOrgPath);
//		fs.FileDelete();
		tps.SetCallerId(strId);
		if (!tps.SaveRsfFile()){
			return;
		}
	}
	//// rsflテーブル更新
	CTblRsfl tbl(theApp.m_strDBPath);
	tbl.SelectRec(m_lngId);
	tbl.strSenderId = strId;		// 送信者名（スカイプ名）
	tbl.strSenderName = m_strName;	// 送信者名（表示名）
	tbl.UpdateRec(m_lngId);

	/// メッセージリストの更新
	pfview->UpdateForm();

	CDialog::OnOK();
}

void CRsfRenameDialog::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function


