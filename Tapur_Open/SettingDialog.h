#if !defined(AFX_SETTINGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETTINGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDialog.h : �w�b�_�[ �t�@�C��
//
#include "TabCtrlSSL.h"
//#include "ButtonSSL.h"
#include "SettingPubTab.h"	
#include "SettingRecTab.h"	
#include "SettingVRecTab.h"	
#include "SettingAdminTab.h"	

/////////////////////////////////////////////////////////////////////////////
// CSettingDialog �_�C�A���O

class CSettingDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CSettingDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSettingDialog)
	enum { IDD = IDD_SETTING };
	CButton	m_Apply;
	CTabCtrlSSL	m_tabCtrl;
	CString	m_strId;
	CString	m_strName;
	//}}AFX_DATA

private:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSettingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	CSettingPubTab m_tabPub;
	CSettingRecTab m_tabRec;
	CSettingVRecTab m_tabVRec;
	CSettingAdminTab m_tabAdmin;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSettingDialog)
	afx_msg void OnApply();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void EnableApply() { m_Apply.EnableWindow(TRUE); }	// �K�p�{�^����L����

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SETTINGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
