// SettingDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "SettingDialog.h"
#include "MsgList.h"
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
// CSettingDialog �_�C�A���O


CSettingDialog::CSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingDialog)
	//}}AFX_DATA_INIT
}


void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingDialog)
	DDX_Control(pDX, IDC_TAB, m_tabCtrl);
	DDX_Control(pDX, IDC_APPLY, m_Apply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialog)
	//{{AFX_MSG_MAP(CSettingDialog)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDialog ���b�Z�[�W �n���h��

BOOL CSettingDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	// Setup the tab control
	CString str;
	int nPageID = 0;
	str.LoadString(IDS_TAB_PUB);
	m_tabPub.Create (IDD_TAB_PUB, this);
	m_tabCtrl.AddSSLPage (str, nPageID++, &m_tabPub);
	str.LoadString(IDS_TAB_REC);
	m_tabRec.Create (IDD_TAB_REC, this);
	m_tabCtrl.AddSSLPage (str, nPageID++, &m_tabRec);
	str.LoadString(IDS_TAB_VREC);
	m_tabVRec.Create (IDD_TAB_VREC, this);
	m_tabCtrl.AddSSLPage (str, nPageID++, &m_tabVRec);
	str.LoadString(IDS_TAB_ADMIN);
	m_tabAdmin.Create (IDD_TAB_ADMIN, this);
	m_tabCtrl.AddSSLPage (str, nPageID++, &m_tabAdmin);
	
	/// �K�p�{�^���𖳌�
	m_Apply.EnableWindow(FALSE);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/// �eTAB�̒l�̕ۑ�
void CSettingDialog::OnApply() 
{
	// �ݒ�`�F�b�N
	if (!m_tabPub.Check())	return;
	if (!m_tabRec.Check())	return;
	if (!m_tabVRec.Check())	return;
	if (!m_tabAdmin.Check())	return;

	// �ۑ�
	m_tabPub.Save();
	m_tabRec.Save();
	m_tabVRec.Save();
	m_tabAdmin.Save();

	/// �K�p�{�^���𖳌�
	m_Apply.EnableWindow(FALSE);
}

void CSettingDialog::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������

	/// �eTAB�̒l�̕ۑ�
	OnApply();

	CDialog::OnOK();
}

void CSettingDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// local function


