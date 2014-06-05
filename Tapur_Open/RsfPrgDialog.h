#if !defined(AFX_RSFPRGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFPRGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfPrgDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CRsfPrgDialog �_�C�A���O

class CRsfPrgDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CRsfPrgDialog(CWnd* pParent = NULL, CString strCaption = _T(""));   // �W���̃R���X�g���N�^
	~CRsfPrgDialog();      

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CRsfPrgDialog)
	enum { IDD = IDD_RSF_PRG };
	CString	m_strPrg;
	CComboBox m_pcmb1;
	CComboBox m_pcmb2;
	CComboBox m_pcmb3;
	CComboBox m_pcmb4;
	CComboBox m_pcmb5;
	CComboBox m_pcmb6;
	CComboBox m_pcmb7;
	CComboBox m_pcmb8;
	CComboBox m_pcmb9;
	CComboBox m_pcmb10;
	//}}AFX_DATA

private:
	CString m_strPlayFile;	// �A�gCSV�t�@�C����
	CStringArray m_aryName;	// �������X�g�i�ϐ����j
	CStringArray m_aryVar;	// �������X�g�i�ϐ��j

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CRsfPrgDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	CString	m_strCaption;	// window�^�C�g��

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CRsfPrgDialog)
	afx_msg void OnBtnFile();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetFile(CString strFile) { m_strPlayFile = strFile; }
	BOOL UpdateForm();	// ��ʂ̍X�V
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_RSFPRGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
