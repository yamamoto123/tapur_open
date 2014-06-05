#if !defined(AFX_SETTINGPUBTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETTINGPUBTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingPubTab.h : �w�b�_�[ �t�@�C��
//
#include <imm.h> 
#include "TabPageSSL.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingPubTab �_�C�A���O

class CSettingPubTab : public CTabPageSSL
{
// �R���X�g���N�V����
public:
	CSettingPubTab(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSettingPubTab)
	enum { IDD = IDD_TAB_PUB };
	BOOL	m_blStartup;
	BOOL	m_blDisable;
	int		m_idxWaitAnswer;
	int		m_idxMaxRecord;
	BOOL	m_blShortCutTapur;
	BOOL	m_blShortCutSkype;
	CButton	m_File;
	int		m_idxCodecName;
	int		m_idxFormatName;
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA

private:
	HIMC m_hImc;		// IME�n���h��(���͎��̑ޔ�p)
	DWORD m_dwImeConv;	// IME���̓��[�h(���͎��̑ޔ�p)
	DWORD m_dwImeSent; 	// IME�ϊ����[�h(���͎��̑ޔ�p)

	/// ���W�X�g��
	CRegString m_rgStartup;
	CRegDWORD m_rgDisable;
	CRegDWORD m_rgWaitAnswer;
	CRegDWORD m_rgMaxRecord;
	CRegDWORD m_rgShortCutTapur;
	CRegDWORD m_rgShortCutSkype;
	CRegDWORD m_rgFormatType;		// WAV or MP3
	CRegString m_rgCodecName;		// MP3�R�[�f�b�N����
	CRegString m_rgFormatName;		// MP3�t�H�[�}�b�g����

	CString m_strBeepFile;			// �ݒ�BEEP�t�@�C��
	CStringArray m_aryWaitAnswer;	// �ҋ@���ԃ��X�g
	CStringArray m_aryMaxRecord;	// �^�����ԃ��X�g
	CStringArray m_aryCodecName;	// MP3�R�[�f�b�N�`��
	CStringArray m_aryFormatName;	// MP3�t�H�[�}�b�g�`��

	CAcmMgr m_oAcmMgr;				// ACM�}�l�[�W��
	WAVEFORMATEX m_wfx;				// ���ݑI�𒆂̃t�H�[�}�b�g�ɑΉ�����WAVEFORMATEX

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSettingPubTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSettingPubTab)
//	afx_msg void OnSetfocusWaitAnswer();
//	afx_msg void OnKillfocusWaitAnswer();
//	afx_msg void OnSetfocusMaxRecord();
//	afx_msg void OnKillfocusMaxRecord();
//	afx_msg void OnChkDisable();
//	afx_msg void OnChkContact();
//	afx_msg void OnChkMute();
	afx_msg void OnBtnBeepFile();
	afx_msg void OnCmbCodec();
	afx_msg void OnRdoMp3();
	afx_msg void OnChgForm();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL Check() { return TRUE; }	// ����
	void Save();	// �ۑ�
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SETTINGPUBTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
