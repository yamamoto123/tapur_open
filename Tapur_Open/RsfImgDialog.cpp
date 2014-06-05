// RsfImgDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfImgDialog.h"
#include "RsfTree.h"
#include "TPtable.h"
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

// �摜�̍ő�t�@�C���T�C�Y(Kbyte)
#define MAX_JPG_SIZE	300
#define MAX_BMP_SIZE	1024

// �摜�̍ő�\���T�C�Y
#define MAX_IMG_WIDTH	640
#define MAX_IMG_HIGHT	400

/////////////////////////////////////////////////////////////////////////////
// CRsfImgDialog �_�C�A���O


CRsfImgDialog::CRsfImgDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRsfImgDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRsfImgDialog)
	//}}AFX_DATA_INIT

	m_width = 0;
	m_height = 0;
	m_wrate = 0;
	m_hrate = 0;
}

CRsfImgDialog::~CRsfImgDialog()
{
}

void CRsfImgDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfImgDialog)
	DDX_Control(pDX, IDC_PICT, m_Pict);
	DDX_Control(pDX, IDC_BTN_FILE, m_File);
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRsfImgDialog, CDialog)
	//{{AFX_MSG_MAP(CRsfImgDialog)
	ON_BN_CLICKED(IDC_BTN_FILE, OnBtnFile)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfImgDialog ���b�Z�[�W �n���h��

BOOL CRsfImgDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	// ��ʍX�V
	UpdateForm();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CRsfImgDialog::OnBtnFile()
{
	CString str;
	CString strFile;

	/// �����摜�t�@�C���Q��
	CFileDialog fileDlg(TRUE, _T("JPEG"), _T("*.jpg"), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 
		_T("Image files (*.jpg; *.gif; *.bmp)|*.jpg; *.gif; *.bmp|all(*.*)|*.*||"));
	if (fileDlg.DoModal() != IDOK) return;
	strFile = fileDlg.GetPathName();
	CFileSpec fs(strFile);
	if (!fs.Exist()) return;

	/// �摜�T�C�Y�̃`�F�b�N
	long fsz = (long)fs.FileSize();
	CString strExt = fs.Extension();
	strExt.MakeLower();
	if (strExt == _T(".bmp")) {
		if (fsz > MAX_BMP_SIZE*1024) {
			str.LoadString(IDS_MSGBOX68);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
			return;
		}
	} else if (strExt == _T(".jpg") || strExt == _T(".gif")) {
		if (fsz > MAX_JPG_SIZE*1024) {
			str.LoadString(IDS_MSGBOX68);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
			return;
		}
	} else {
		str.LoadString(IDS_MSGBOX67);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return;
	}
	

	/// �摜�t�@�C���̃��[�h
	if (!m_Cpict.Load(strFile)) return;

	/// ���݂̉����摜�t�@�C���̃o�b�N�A�b�v
	CTPsetup tps(theApp.m_strDataFolder);
	fs.SetFullSpec(m_strPlayFile);
	if (fs.Exist()) {
		m_strTmpFile = tps.RsfTmpImgFileName(strExt.Right(3));
		fs.FileMove(m_strTmpFile);
	}

	/// �����摜�t�@�C���̃R�s�[
	m_strPlayFile = m_strOutFile + strExt;
	fs.SetFullSpec(strFile);
	fs.FileCopy(m_strPlayFile, TRUE);

	/// ��ʂ̍X�V
	UpdateForm();
}

void CRsfImgDialog::OnPaint() 
{
	if (!IsIconic())
	{
		CDialog::OnPaint();

		if (m_Cpict.m_IPicture == NULL) return;

		CDC *pDC = m_Pict.GetDC();
		m_Cpict.UpdateSizeOnDC(pDC);
		m_Cpict.Show(pDC, CPoint(0,0), CPoint(m_width, m_height), 0, 0);
	}
}

void CRsfImgDialog::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������

	CDialog::OnOK();
}

void CRsfImgDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	//// �o�b�N�A�b�v�t�@�C����߂�
	CTPsetup tps(theApp.m_strDataFolder);
	CFileSpec fs(m_strTmpFile);
	if (fs.Exist()) {
		m_strPlayFile = m_strOutFile + m_strTmpFile.Right(3);
		fs.FileMove(m_strPlayFile);
	}

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
BOOL CRsfImgDialog::UpdateForm() 
{
	/// �����摜�t�@�C��������ꍇ�A��ʂɕ\��
	CFileSpec fs(m_strPlayFile);
	if (!fs.Exist()) return TRUE;

	/// �摜�̃��[�h
	m_Cpict.Load(m_strPlayFile);

	/// �摜�T�C�Y
	m_width = m_Cpict.m_Width;
	m_height = m_Cpict.m_Height;

	/// �摜�T�C�Y���傫���ꍇ�k������
	if (m_width > MAX_IMG_WIDTH || m_height > MAX_IMG_HIGHT) {
		m_wrate = (double)MAX_IMG_WIDTH / (double)m_width;
		m_hrate = (double)MAX_IMG_HIGHT / (double)m_height;
		if (m_wrate > m_hrate) {
			m_width = (long)((double)m_width * m_hrate);
			m_height = MAX_IMG_HIGHT;
		} else {
			m_width = MAX_IMG_WIDTH;
			m_height = (long)((double)m_height * m_wrate);
		}
	}

	/// PictureBox���g��
	m_Pict.SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);

	/// �t�H�[���T�C�Y���g��
    WINDOWPLACEMENT wp;
    GetWindowPlacement(&wp);
	this->MoveWindow(wp.rcNormalPosition.left, wp.rcNormalPosition.top, 
		m_width+28, m_height+100);

	/// �{�^���̈ʒu��ݒ肷��
	m_Ok.SetWindowPos(NULL, m_width-172, m_height+48, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Cancel.SetWindowPos(NULL, m_width-76, m_height+48, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	UpdateData(FALSE);	// DDX�X�V

	return TRUE;
}

