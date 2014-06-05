#if !defined(AFX_SETTINGVRECTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETTINGVRECTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingVRecTab.h : �w�b�_�[ �t�@�C��
//
#include <imm.h> 
#include "TabPageSSL.h"
#include "Label.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingVRecTab �_�C�A���O

class CSettingVRecTab : public CTabPageSSL
{
// �R���X�g���N�V����
public:
	CSettingVRecTab(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSettingVRecTab)
	enum { IDD = IDD_TAB_VREC };
	int		m_idxVRecordLimit;
	int		m_idxVCmpName;
	int		m_idxSepSize;
	CButton	m_btnVRecWarning;
	CButton	m_btnVRecStopWarning;
	BOOL	m_blVRecWarning;
	BOOL	m_blVRecStopWarning;
	BOOL	m_blVSetTop;
	BOOL	m_blNoOverlay;
	BOOL	m_blSep;
	BOOL	m_blAudio;
	CString m_strFps;
	CLabel	m_LinkLine;
	//}}AFX_DATA

private:
	/// ���W�X�g��
	CRegDWORD m_rgVRecordLimit;
	CRegDWORD m_rgEnableVRecWarning;
	CRegDWORD m_rgEnableVRecStopWarning;
	CRegDWORD m_rgVSetTop;
	CRegDWORD m_rgOverlay;
	CRegDWORD m_rgSep;
	CRegDWORD m_rgSepSize;
	CRegDWORD m_rgAudio;
	CRegDWORD m_rgVCmp;
	CRegString m_rgVCmpName;
	CRegDWORD m_rgFps;

	CStringArray m_aryVRecordLimit;	// �^�掞�ԃ��X�g
	CStringArray m_arySepSize;		// �^��T�C�Y���X�g
	CStringArray m_aryVCmpName;		// �^���`��
	CStringArray m_aryFps;			// FPS

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSettingVRecTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSettingVRecTab)
	afx_msg void OnBtnVRecWarning();
	afx_msg void OnBtnVRecStopWarning();
	afx_msg void OnChgFps();
	afx_msg void OnChgForm();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//void AssignListToComboBox(CComboBox *Dest, CString Source, long Indx, long MaxIndxLimit=-1);
	void AssignListToComboBox(CComboBox *Dest, CStringList *Source);

public:
	BOOL Check() { return TRUE; }	// ����
	void Save();	// �ۑ�
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SETTINGVRECTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
