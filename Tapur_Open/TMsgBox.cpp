// TMsgBox.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "TMsgBox.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CTMsgBox ダイアログ

// チェック画面用コンストラクタ
CTMsgBox::CTMsgBox(CWnd* pParent /*=NULL*/, CString strCaption, CString strMsg, BOOL blTop)
	: CDialog(CTMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTMsgBox)
	m_blChk = FALSE;
	//}}AFX_DATA_INIT

	m_strCaption = strCaption;
	m_strMsg = strMsg;
	m_blTop = blTop;

	m_blReturn = FALSE;
	m_strReturn = _T("");
}

// リスト画面用コンストラクタ
CTMsgBox::CTMsgBox(CWnd* pParent /*=NULL*/, CString strCaption, CString strMsg, BOOL blTop, CStringArray &aryName)
	: CDialog(CTMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTMsgBox)
	m_blChk = FALSE;
	//}}AFX_DATA_INIT

	m_strCaption = strCaption;
	m_strMsg = strMsg;
	m_blTop = blTop;

	m_blReturn = FALSE;
	m_strReturn = _T("");

	m_aryName.RemoveAll();
	for(int i = 0; i < aryName.GetSize(); i++) {
		m_aryName.Add(aryName.GetAt(i));
	}

}

void CTMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTMsgBox)
	DDX_Check(pDX, IDC_CHK_TMSGBOX, m_blChk);
	DDX_Control(pDX, IDC_CMB_TMSGBOX, m_pCmb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTMsgBox, CDialog)
	//{{AFX_MSG_MAP(CTMsgBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTMsgBox メッセージ ハンドラ


BOOL CTMsgBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	/// タイトルとメッセージ表示
	this->SetWindowText(m_strCaption);
	CStatic *ptxt;
	ptxt = (CStatic*)GetDlgItem(IDC_STATIC_TMSGBOX);
	ptxt->SetWindowText(m_strMsg);

	/// 構成要素の有効/無効
	if (m_aryName.GetSize() > 0) {
		// リスト選択画面
		EnableCombo(m_aryName);
	} else {
		// チェック画面
		m_pCmb.ShowWindow(SW_HIDE);
	}

	/// Window前面表示
	if (m_blTop) {
		this->SetWindowPos(&wndTopMost, 0,0,0,0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE);
	}


	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


void CTMsgBox::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	UpdateData(TRUE);	// DDX更新

	m_blReturn = TRUE;
	m_pCmb.GetWindowText(m_strReturn);

	CDialog::OnOK();
}

void CTMsgBox::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	m_blReturn = FALSE;

	CDialog::OnCancel();
}

void CTMsgBox::Show(BOOL blModal) 
{
	if (blModal) {
		this->DoModal();
	} else {
		this->ShowWindow(SW_SHOWNORMAL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// local function

void CTMsgBox::EnableCombo(CStringArray &aryName)
{
	/// 構成要素の有効/無効
	CWnd *pChk = NULL;
	pChk = GetDlgItem(IDC_CHK_TMSGBOX);
	pChk->ShowWindow(SW_HIDE);
	m_pCmb.ShowWindow(SW_SHOW);

	/// リストの設定
	for(int i = 0; i < aryName.GetSize(); i++) {
		m_pCmb.AddString(aryName.GetAt(i));
	}
	m_pCmb.SetCurSel(0);
}

CString CTMsgBox::GetCombo()
{
	return m_strReturn;
}

