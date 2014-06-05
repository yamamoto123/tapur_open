#if !defined(AFX_SETTINGADMINTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETTINGADMINTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingAdminTab.h : �w�b�_�[ �t�@�C��
//
#include "TabPageSSL.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingAdminTab �_�C�A���O

class CSettingAdminTab : public CTabPageSSL
{
// �R���X�g���N�V����
public:
	CSettingAdminTab(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSettingAdminTab)
	enum { IDD = IDD_TAB_PUB };
	BOOL	m_blHotKey;
	BOOL	m_blEnableLog;
	BOOL	m_blDataPath;
	CEdit	m_edtDataPath;
	CButton m_btnDataPath;
	CString	m_strSkypePortOut;
	CString	m_strSkypePortMic;
	//}}AFX_DATA

private:

	/// ���W�X�g��
	CRegDWORD m_rgEnableHotKey;
	CRegDWORD m_rgEnableLog;
	CRegDWORD m_rgEnableDataPath;
	CRegString m_rgDataPath;
	CRegDWORD m_rgSkypePortOut;
	CRegDWORD m_rgSkypePortMic;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSettingAdminTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSettingAdminTab)
	afx_msg void OnBtnHotkey();
	afx_msg void OnBrowseDataPath();
	afx_msg void OnChgDataPath();
	afx_msg void OnChgSkypePortOut();
	afx_msg void OnChgSkypePortMic();
	afx_msg void OnChgForm();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL Check();	// ����
	void Save();	// �ۑ�
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SETTINGADMINTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
