// RsfNewDialog.cpp : �C���v�������e�[�V���� �t�@�C��
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

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CRsfNewDialog �_�C�A���O


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
// CRsfNewDialog ���b�Z�[�W �n���h��


BOOL CRsfNewDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	m_aryHandle.RemoveAll();
	m_aryFullName.RemoveAll();
    m_pcmb = (CComboBox*)GetDlgItem(IDC_CMB_ID);
	m_pcmb->LimitText(32);
	
	/// �R���^�N�g���X�g���R���{�{�b�N�X���X�g�쐬
	if (theApp.m_blSkypeConnect) {	// Skype�̏�Ԋm�F
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
	}

	/// ����ID���w�肳�ꂽ�ꍇ
	if (!m_strId.IsEmpty()) m_pcmb->SetWindowText(m_strId);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// �R���{�{�b�N�X�̓��͐���
BOOL CRsfNewDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

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

void CRsfNewDialog::OnOK() 
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
	CRsfList *pmv = (CRsfList *)pFrame->m_pRsfList->GetWnd();
	if (!pmv->ChkSetup(strId)) {
		str.LoadString(IDS_MSGBOX41);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return;
	}

	/// �����ݒ�쐬�i�����t�@�C���A�ݒ�t�@�C���A�e�[�u���j
	CFileSpec fs;
	CString strPath, strOrgPath;	
	CTPsetup tps(theApp.m_strDataFolder, strId);
	//// ���������t�@�C���쐬
	strPath = tps.RsfFileName();
	fs.SetFullSpec(strPath);
	if (!tps.SaveRsfFile()){
		TRACE(_T("can't create response file.\n"));
		return;
	}
	//// �_�~�[�Ƃ��ď������������t�@�C���g�p
	strPath = tps.RsfAudioFileName(_T("$"));
	///// �������������t�@�C���̃R�s�[
	tps.SetCallerId(_T("0"));
	strOrgPath = tps.RsfAudioFileName(_T("$"));
	fs.SetFullSpec(strOrgPath);
	if (!fs.FileCopy(strPath, TRUE)) {
		TRACE(_T("can't create response audio file:%s\n"), strPath);
		return;
	}
	//// rsfl�e�[�u���ɒǉ�
	///// �I���t�H���_���
	CRsfTree *pfv = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	CTblRsfl tbl(theApp.m_strDBPath);
	tbl.strSenderId = strId;		// ���M�Җ��i�X�J�C�v���j
	tbl.strSenderName = m_strName;	// ���M�Җ��i�\�����j
	tbl.strReceiverId = theApp.m_strUsrId;	// ��M�Җ��i�X�J�C�v���j
	tbl.strReceiverName = theApp.m_strUsrName;	// ��M�Җ��i�\�����j
	tbl.lngRespType = RESPBASE_TYPE_SIMPLE;
	if (pfv->m_dwFolderType == FD_NORMAL) {
		///// �J�����g�t�H���_���i�f�t�H���g�ȊO�́j�ݒ�t�H���_�̏ꍇ�̂݃t�H���_�w��
		tbl.lngFolderId = pfv->m_dwFolderId;
	}
// (?) �R���e�L�X�g����̌Ăяo�����ƃg�����U�N�V�������@�\���Ȃ�
	tbl.m_blTransaction = TRUE;
	BOOL ret = tbl.InsertRec();
	tbl.m_blTransaction = FALSE;
	if (ret) {
		/// ���������ݒ��\��
		pmv->SelSetup(strId);
	}

	CDialog::OnOK();
}

void CRsfNewDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function


