// RsfPrgDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfPrgDialog.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"
#include "Tokenizer.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CRsfPrgDialog ダイアログ


CRsfPrgDialog::CRsfPrgDialog(CWnd* pParent /*=NULL*/, CString strCaption)
	: CDialog(CRsfPrgDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRsfPrgDialog)
	m_strPrg = _T("");
	//}}AFX_DATA_INIT
	m_strCaption = strCaption;
}

CRsfPrgDialog::~CRsfPrgDialog()
{
}

void CRsfPrgDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfPrgDialog)
	DDX_Text(pDX, IDC_EDIT_PRGPATH, m_strPrg);
	DDX_Control(pDX, IDC_CMB_PRGARG1, m_pcmb1);
	DDX_Control(pDX, IDC_CMB_PRGARG2, m_pcmb2);
	DDX_Control(pDX, IDC_CMB_PRGARG3, m_pcmb3);
	DDX_Control(pDX, IDC_CMB_PRGARG4, m_pcmb4);
	DDX_Control(pDX, IDC_CMB_PRGARG5, m_pcmb5);
	DDX_Control(pDX, IDC_CMB_PRGARG6, m_pcmb6);
	DDX_Control(pDX, IDC_CMB_PRGARG7, m_pcmb7);
	DDX_Control(pDX, IDC_CMB_PRGARG8, m_pcmb8);
	DDX_Control(pDX, IDC_CMB_PRGARG9, m_pcmb9);
	DDX_Control(pDX, IDC_CMB_PRGARG10, m_pcmb10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRsfPrgDialog, CDialog)
	//{{AFX_MSG_MAP(CRsfPrgDialog)
	ON_BN_CLICKED(IDC_BTN_FILE, OnBtnFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfPrgDialog メッセージ ハンドラ

BOOL CRsfPrgDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CString str;

	/// タイトル表示
	this->SetWindowText(m_strCaption);

	/// コンボボックス入力長
	m_pcmb1.LimitText(1024);
	m_pcmb2.LimitText(1024);
	m_pcmb3.LimitText(1024);
	m_pcmb4.LimitText(1024);
	m_pcmb5.LimitText(1024);
	m_pcmb6.LimitText(1024);
	m_pcmb7.LimitText(1024);
	m_pcmb8.LimitText(1024);
	m_pcmb9.LimitText(1024);
	m_pcmb10.LimitText(1024);

	/// プログラム変数名リスト作成
	m_aryName.RemoveAll();
	m_aryName.Add(_T(""));
	str.LoadString(IDS_RSF_ARG_AUDIOFILE);
	m_aryName.Add(str);
	str.LoadString(IDS_RSF_ARG_VIDEOFILE);
	m_aryName.Add(str);
	str.LoadString(IDS_RSF_ARG_CALLERID);
	m_aryName.Add(str);
	str.LoadString(IDS_RSF_ARG_CALLERNAME);
	m_aryName.Add(str);
	str.LoadString(IDS_RSF_ARG_MYID);
	m_aryName.Add(str);
	str.LoadString(IDS_RSF_ARG_MYNAME);
	m_aryName.Add(str);
	str.LoadString(IDS_RSF_ARG_CALLERADDRESS);
	m_aryName.Add(str);
	str.LoadString(IDS_RSF_ARG_CALLERPHONEHOME);
	m_aryName.Add(str);
	str.LoadString(IDS_RSF_ARG_CALLERPHONEOFFICE);
	m_aryName.Add(str);
	str.LoadString(IDS_RSF_ARG_CALLERPHONEMOBILE);
	m_aryName.Add(str);

	/// プログラム変数リスト作成
	m_aryVar.RemoveAll();
	m_aryVar.Add(_T(""));
	m_aryVar.Add(_T("$AudioFile"));			// 録音ファイル
	m_aryVar.Add(_T("$VideoFile"));			// 録画ファイル
	m_aryVar.Add(_T("$PartnerId"));			// 通話相手(スカイプID)
	m_aryVar.Add(_T("$PartnerName"));		// 通話相手(スカイプ名)
	m_aryVar.Add(_T("$UsrId"));				// ユーザ(スカイプID)
	m_aryVar.Add(_T("$UsrName"));			// ユーザ(スカイプ名)
	m_aryVar.Add(_T("$PartnerAddress"));		// 通話相手住所
	m_aryVar.Add(_T("$PartnerPhoneHome"));	// 通話相手電話（自宅）
	m_aryVar.Add(_T("$PartnerPhoneOffice"));	// 通話相手電話（オフィス）
	m_aryVar.Add(_T("$PartnerPhoneMobile"));	// 通話相手電話（モバイル）

	/// プログラム変数コンボボックス初期化
	for(int i = 0; i < m_aryName.GetSize(); i++) {
		m_pcmb1.AddString(m_aryName.GetAt(i));
		m_pcmb2.AddString(m_aryName.GetAt(i));
		m_pcmb3.AddString(m_aryName.GetAt(i));
		m_pcmb4.AddString(m_aryName.GetAt(i));
		m_pcmb5.AddString(m_aryName.GetAt(i));
		m_pcmb6.AddString(m_aryName.GetAt(i));
		m_pcmb7.AddString(m_aryName.GetAt(i));
		m_pcmb8.AddString(m_aryName.GetAt(i));
		m_pcmb9.AddString(m_aryName.GetAt(i));
		m_pcmb10.AddString(m_aryName.GetAt(i));
	}

	// 画面更新
	UpdateForm();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// EXEファイル参照
void CRsfPrgDialog::OnBtnFile() 
{
	CString str;
	CString strFile;

	/// 連携EXEファイル参照
	CFileDialog fileDlg(TRUE, _T("EXE"), _T("*.exe"), OFN_FILEMUSTEXIST, 
		_T("Exe files (*.exe; *.bat)|*.exe; *.bat|all(*.*)|*.*||"));
	_TCHAR lpstrPath[MAX_PATH];                
	CFileSpec fs(FS_APPDIR);
	str = fs.GetFullSpec() + _T("bin");	// テンプレートCSVフォルダ
	_tcscpy_s(lpstrPath, MAX_PATH, str);
	fileDlg.m_ofn.lpstrInitialDir = lpstrPath;	// CStringは受け取れない
	if (fileDlg.DoModal() != IDOK) return;

	strFile = fileDlg.GetPathName();
	fs.SetFullSpec(strFile);
	if (!fs.Exist()) return;
	m_strPrg = strFile;

	UpdateData(FALSE);	// DDX更新（変数→画面）
}

void CRsfPrgDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CString str;

	UpdateData(TRUE);	// DDX更新

	m_strPrg = m_strPrg.Trim();
	if (m_strPrg.GetLength() == 0) {
		str.LoadString(IDS_MSGBOX106);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}

	/// 連携プログラムCSV保存
	int i, k;
	CComboBox *pcmb;
	CString strLine;
	strLine.Empty();
	strLine = strLine + _T("0") + _T(",") + m_strPrg;
	for(i = 0; i < 10; i++) {
		if (i == 0) {
			pcmb = &m_pcmb1;
		} else if (i == 1) {
			pcmb = &m_pcmb2;
		} else if (i == 2) {
			pcmb = &m_pcmb3;
		} else if (i == 3) {
			pcmb = &m_pcmb4;
		} else if (i == 4) {
			pcmb = &m_pcmb5;
		} else if (i == 5) {
			pcmb = &m_pcmb6;
		} else if (i == 6) {
			pcmb = &m_pcmb7;
		} else if (i == 7) {
			pcmb = &m_pcmb8;
		} else if (i == 8) {
			pcmb = &m_pcmb9;
		} else if (i == 9) {
			pcmb = &m_pcmb10;
		}
		pcmb->GetWindowText(str);
		for(k = 0; k < m_aryName.GetSize(); k++) {
			if (str == m_aryName.GetAt(k)) {
				// リストの名称と一致する場合、変数を保存
				str = m_aryVar.GetAt(k);
				break;
			}
		}
		if (str.GetLength() > 0) strLine += _T(",") + str;
	}

	CFileSpec fs(m_strPlayFile);
	if (!fs.SaveString(strLine)) {
		str.LoadString(IDS_MSGBOX106);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}

	CDialog::OnOK();
}

void CRsfPrgDialog::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function

//
//  機能     : 画面の更新
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CRsfPrgDialog::UpdateForm() 
{
	/// 応答CSVの内容を画面に表示
	CString str;
	CString strCsv;
	CFileSpec fs(m_strPlayFile);
	if (!fs.Exist()) return FALSE;
	if (!fs.LoadString(&strCsv)) return FALSE;

	CTokenizer tok(strCsv, _T(","));
	CString cs;
	int i = 0, k;
	BOOL blFlag;
	CComboBox *pcmb;
	while(tok.Next(cs)) {
		//if (i == 0) 予約
		if (i == 1) {
			m_strPrg = cs;
		} else if (i == 2) {
			pcmb = &m_pcmb1;
		} else if (i == 3) {
			pcmb = &m_pcmb2;
		} else if (i == 4) {
			pcmb = &m_pcmb3;
		} else if (i == 5) {
			pcmb = &m_pcmb4;
		} else if (i == 6) {
			pcmb = &m_pcmb5;
		} else if (i == 7) {
			pcmb = &m_pcmb6;
		} else if (i == 8) {
			pcmb = &m_pcmb7;
		} else if (i == 9) {
			pcmb = &m_pcmb8;
		} else if (i == 10) {
			pcmb = &m_pcmb9;
		} else if (i == 11) {
			pcmb = &m_pcmb10;
		}
		if (2 <= i && i <= 11) {
			blFlag = FALSE;
			for(k = 0; k < m_aryVar.GetSize(); k++) {
				str = m_aryVar.GetAt(k);
				if (str == cs) {
					// リストの変数と一致する場合、変数名称を選択
					pcmb->SetCurSel(k);
					blFlag = TRUE;
					break;
				}
			}
			if (!blFlag) {
				// リストの変数と一致しない場合、ユーザ文字列として表示
				pcmb->SetCurSel(0);
				pcmb->SetWindowText(cs);
			}
		}

		i++;
	}

	UpdateData(FALSE);	// DDX更新（変数→画面）

	return TRUE;
}

