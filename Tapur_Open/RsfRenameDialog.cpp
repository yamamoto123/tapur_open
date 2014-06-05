// RsfRenameDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfRenameDialog.h"
#include "RsfList.h"
#include "TPtable.h"
#include "TPutil.h"
#include "TPsetup.h"
#include "FileSpec.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CRsfRenameDialog �_�C�A���O


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
// CRsfRenameDialog ���b�Z�[�W �n���h��

BOOL CRsfRenameDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	m_aryHandle.RemoveAll();
	m_aryFullName.RemoveAll();
    m_pcmb = (CComboBox*)GetDlgItem(IDC_CMB_ID);
	m_pcmb->LimitText(32);

	/// �I�����
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfList *pfview = (CRsfList *)pFrame->m_pRsfList->GetWnd();
	m_lngId = pfview->m_wndList.GetItemData(pfview->m_hActive);

	/// �R���^�N�g���X�g���R���{�{�b�N�X���X�g�쐬
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
				/// �����ݒ�Əd��������̂͏��O
				if (!pfview->ChkSetup(str)) continue;
				m_pcmb->AddString(str);
				m_aryHandle.Add(str);
				bStr = pUsr->GetFullName();
				str.Format(_T("%s"), (LPCTSTR)bStr);
				m_aryFullName.Add(str);
			}
		}
	}

	/// �����̑��M�Җ���\��
	CTblRsfl tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(m_lngId) != 1) return TRUE;
	m_strOrgId = tbl.strSenderId;		// ���M�Җ��i�X�J�C�v���j�̃o�b�N�A�b�v
	m_strName = tbl.strSenderName;		// ���M�Җ��i�\�����j
	UpdateData(FALSE);	// DDX�X�V(�R���{�{�b�N�XTOP�̃X�J�C�v�������f����Ă��܂��̂ŁA���̈ʒu�Ŏ��s)
	m_pcmb->SetWindowText(m_strOrgId);	// ���M�Җ��i�X�J�C�v���j

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// �R���{�{�b�N�X�̓��͐���
BOOL CRsfRenameDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

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

	UpdateData(TRUE);	// DDX�X�V

	m_strName.Empty();
	m_pcmb->GetLBText(m_idxId, strId);
	for(i = 0; i < m_aryHandle.GetSize(); i++) {
		if (m_aryHandle.GetAt(i) == strId) {
			m_strName = m_aryFullName.GetAt(i);
			break;
		}
	}

	UpdateData(FALSE);	// DDX�X�V
}

void CRsfRenameDialog::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	CString str;
	CString strId;

	if (!UpdateData(TRUE)) return;	// DDX�X�V

	/// �X�J�C�v���̃`�F�b�N
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
	// �d���`�F�b�N
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfList *pfview = (CRsfList *)pFrame->m_pRsfList->GetWnd();
	if (m_strOrgId != strId) {
		if (!pfview->ChkSetup(strId)) {
			str.LoadString(IDS_MSGBOX11);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return;
		}
	}

	/// �����ݒ�쐬�i�����t�@�C���A�ݒ�t�@�C���A�e�[�u���j
	if (m_strOrgId != strId) {
		CTPsetup tps(theApp.m_strDataFolder);
		//// ���������t�@�C����RENAME
		tps.SetCallerId(m_strOrgId);
		CString strOrgPath = tps.RsfAudioFileName(_T("$"));
		tps.SetCallerId(strId);
		CString strPath = tps.RsfAudioFileName(_T("$"));
		CFileSpec fs(strOrgPath);
		if (!fs.FileMove(strPath, TRUE)) {
			return;
		}
		//// �����ݒ�t�@�C���č쐬
		tps.SetCallerId(m_strOrgId);
//		strOrgPath = tps.RsfFileName();
//		fs.SetFullSpec(strOrgPath);
//		fs.FileDelete();
		tps.SetCallerId(strId);
		if (!tps.SaveRsfFile()){
			return;
		}
	}
	//// rsfl�e�[�u���X�V
	CTblRsfl tbl(theApp.m_strDBPath);
	tbl.SelectRec(m_lngId);
	tbl.strSenderId = strId;		// ���M�Җ��i�X�J�C�v���j
	tbl.strSenderName = m_strName;	// ���M�Җ��i�\�����j
	tbl.UpdateRec(m_lngId);

	/// ���b�Z�[�W���X�g�̍X�V
	pfview->UpdateForm();

	CDialog::OnOK();
}

void CRsfRenameDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function


