#if !defined(AFX_BACKUPDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_BACKUPDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackupDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CBackupDialog �_�C�A���O

class CBackupDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CBackupDialog(CWnd* pParent = NULL);

protected:
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CBackupDialog)
	enum { IDD = IDD_DATA_BACKUP };
	CEdit	m_edtBackupPath;
	CEdit	m_edtRestorePath;
	//}}AFX_DATA

public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CBackupDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CBackupDialog)
	afx_msg void OnBrowseBackup();
	afx_msg void OnBrowseRestore();
	afx_msg void OnBtnBackup();
	afx_msg void OnBtnRestore();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_BACKUPDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
