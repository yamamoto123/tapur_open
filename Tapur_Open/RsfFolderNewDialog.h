#if !defined(AFX_RSFFOLDERNEWDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFFOLDERNEWDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfFolderNewDialog.h : �w�b�_�[ �t�@�C��
//
#include <imm.h> 

/////////////////////////////////////////////////////////////////////////////
// CRsfFolderNewDialog �_�C�A���O

class CRsfFolderNewDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CRsfFolderNewDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CRsfFolderNewDialog)
	enum { IDD = IDD_FOLDER_NEW };
	CString	m_strFolder;
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA

private:
	HIMC m_hImc;		// IME�n���h��(���͎��̑ޔ�p)
	DWORD m_dwImeConv;	// IME���̓��[�h(���͎��̑ޔ�p)
	DWORD m_dwImeSent; 	// IME�ϊ����[�h(���͎��̑ޔ�p)

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CRsfFolderNewDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CRsfFolderNewDialog)
	afx_msg void OnSetfocusEditId();
	afx_msg void OnKillfocusEditId();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_RSFFOLDERNEWDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
