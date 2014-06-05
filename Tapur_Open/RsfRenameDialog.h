#if !defined(AFX_RSFRENAMEDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFRENAMEDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfRenameDialog.h : �w�b�_�[ �t�@�C��
//
#include <imm.h> 

/////////////////////////////////////////////////////////////////////////////
// CRsfRenameDialog �_�C�A���O

class CRsfRenameDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CRsfRenameDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CRsfRenameDialog)
	enum { IDD = IDD_RSF_RENAME };
	CComboBox *m_pcmb;
	int m_idxId;
	CString	m_strName;
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA

private:
	HIMC m_hImc;		// IME�n���h��(���͎��̑ޔ�p)
	DWORD m_dwImeConv;	// IME���̓��[�h(���͎��̑ޔ�p)
	DWORD m_dwImeSent; 	// IME�ϊ����[�h(���͎��̑ޔ�p)
	long m_lngId;		// �Ώۃe�[�u��ID
	CString m_strOrgId;	// ���M�Җ��i�X�J�C�v���j�̃o�b�N�A�b�v
	CStringArray m_aryHandle;	// �R���^�N�g���X�g�iID�j
	CStringArray m_aryFullName;	// �R���^�N�g���X�g�i�\�����j

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CRsfRenameDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CRsfRenameDialog)
	afx_msg void OnSelchangeId();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_RSFRENAMEDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
