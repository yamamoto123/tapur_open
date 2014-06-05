#if !defined(AFX_RSFIMGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFIMGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfImgDialog.h : �w�b�_�[ �t�@�C��
//
//#include <imm.h> 
#include "Picture.h"

/////////////////////////////////////////////////////////////////////////////
// CRsfImgDialog �_�C�A���O

class CRsfImgDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CRsfImgDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CRsfImgDialog();      

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CRsfImgDialog)
	enum { IDD = IDD_RSF_IMG };
	CButton	m_Ok;
	CButton	m_Cancel;
	CStatic	m_Pict;
	CButton	m_File;
	//}}AFX_DATA

private:
	CString m_strPlayFile;	// �����摜�t�@�C����
	CString m_strTmpFile;	// �o�b�N�A�b�v�摜�t�@�C����
	CString m_strOutFile;	// �o�͉摜�t�@�C�����i�g���q�Ȃ��j
	CPicture m_Cpict;	// �摜���[�h�N���X
	long m_width;		// �摜�T�C�Y
	long m_height;		// �摜�T�C�Y
	double m_wrate;		// �摜�k����
	double m_hrate;		// �摜�k����

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CRsfImgDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CRsfImgDialog)
	afx_msg void OnBtnFile();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetFile(CString strFile) { m_strPlayFile = strFile; }
	void SetFileNoExt(CString strFile) { m_strOutFile = strFile; }
	BOOL UpdateForm();	// ��ʂ̍X�V
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_RSFIMGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
