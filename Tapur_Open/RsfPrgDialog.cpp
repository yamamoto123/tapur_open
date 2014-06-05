// RsfPrgDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfPrgDialog.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"
#include "Tokenizer.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CRsfPrgDialog �_�C�A���O


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
// CRsfPrgDialog ���b�Z�[�W �n���h��

BOOL CRsfPrgDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	CString str;

	/// �^�C�g���\��
	this->SetWindowText(m_strCaption);

	/// �R���{�{�b�N�X���͒�
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

	/// �v���O�����ϐ������X�g�쐬
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

	/// �v���O�����ϐ����X�g�쐬
	m_aryVar.RemoveAll();
	m_aryVar.Add(_T(""));
	m_aryVar.Add(_T("$AudioFile"));			// �^���t�@�C��
	m_aryVar.Add(_T("$VideoFile"));			// �^��t�@�C��
	m_aryVar.Add(_T("$PartnerId"));			// �ʘb����(�X�J�C�vID)
	m_aryVar.Add(_T("$PartnerName"));		// �ʘb����(�X�J�C�v��)
	m_aryVar.Add(_T("$UsrId"));				// ���[�U(�X�J�C�vID)
	m_aryVar.Add(_T("$UsrName"));			// ���[�U(�X�J�C�v��)
	m_aryVar.Add(_T("$PartnerAddress"));		// �ʘb����Z��
	m_aryVar.Add(_T("$PartnerPhoneHome"));	// �ʘb����d�b�i����j
	m_aryVar.Add(_T("$PartnerPhoneOffice"));	// �ʘb����d�b�i�I�t�B�X�j
	m_aryVar.Add(_T("$PartnerPhoneMobile"));	// �ʘb����d�b�i���o�C���j

	/// �v���O�����ϐ��R���{�{�b�N�X������
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

	// ��ʍX�V
	UpdateForm();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// EXE�t�@�C���Q��
void CRsfPrgDialog::OnBtnFile() 
{
	CString str;
	CString strFile;

	/// �A�gEXE�t�@�C���Q��
	CFileDialog fileDlg(TRUE, _T("EXE"), _T("*.exe"), OFN_FILEMUSTEXIST, 
		_T("Exe files (*.exe; *.bat)|*.exe; *.bat|all(*.*)|*.*||"));
	_TCHAR lpstrPath[MAX_PATH];                
	CFileSpec fs(FS_APPDIR);
	str = fs.GetFullSpec() + _T("bin");	// �e���v���[�gCSV�t�H���_
	_tcscpy_s(lpstrPath, MAX_PATH, str);
	fileDlg.m_ofn.lpstrInitialDir = lpstrPath;	// CString�͎󂯎��Ȃ�
	if (fileDlg.DoModal() != IDOK) return;

	strFile = fileDlg.GetPathName();
	fs.SetFullSpec(strFile);
	if (!fs.Exist()) return;
	m_strPrg = strFile;

	UpdateData(FALSE);	// DDX�X�V�i�ϐ�����ʁj
}

void CRsfPrgDialog::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	CString str;

	UpdateData(TRUE);	// DDX�X�V

	m_strPrg = m_strPrg.Trim();
	if (m_strPrg.GetLength() == 0) {
		str.LoadString(IDS_MSGBOX106);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}

	/// �A�g�v���O����CSV�ۑ�
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
				// ���X�g�̖��̂ƈ�v����ꍇ�A�ϐ���ۑ�
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
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function

//
//  �@�\     : ��ʂ̍X�V
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfPrgDialog::UpdateForm() 
{
	/// ����CSV�̓��e����ʂɕ\��
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
		//if (i == 0) �\��
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
					// ���X�g�̕ϐ��ƈ�v����ꍇ�A�ϐ����̂�I��
					pcmb->SetCurSel(k);
					blFlag = TRUE;
					break;
				}
			}
			if (!blFlag) {
				// ���X�g�̕ϐ��ƈ�v���Ȃ��ꍇ�A���[�U������Ƃ��ĕ\��
				pcmb->SetCurSel(0);
				pcmb->SetWindowText(cs);
			}
		}

		i++;
	}

	UpdateData(FALSE);	// DDX�X�V�i�ϐ�����ʁj

	return TRUE;
}

