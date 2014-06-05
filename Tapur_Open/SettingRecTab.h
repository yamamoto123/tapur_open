#if !defined(AFX_SETTINGRECTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETTINGRECTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingRecTab.h : �w�b�_�[ �t�@�C��
//
#include <imm.h> 
#include "TabPageSSL.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingRecTab �_�C�A���O

class CSettingRecTab : public CTabPageSSL
{
// �R���X�g���N�V����
public:
	CSettingRecTab(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSettingRecTab)
	enum { IDD = IDD_TAB_REC };
	int		m_idxRecordLimit;
	CButton	m_btnRecWarning;
	CButton	m_btnRecStopWarning;
	BOOL	m_blRecWarning;
	BOOL	m_blRecStopWarning;
	BOOL	m_blEnableAuto;
	int		m_idxCodecName;
	int		m_idxFormatName;
	CString	m_strVolSnd;
	CString	m_strVolRcv;
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA

private:
	/// ���W�X�g��
	CRegDWORD m_rgRecordLimit;
	CRegDWORD m_rgEnableRecWarning;
	CRegDWORD m_rgEnableRecStopWarning;
	CRegDWORD m_rgStereo;
	CRegDWORD m_rgFormatType;		// WAV or MP3
	CRegString m_rgCodecName;		// MP3�R�[�f�b�N����
	CRegString m_rgFormatName;		// MP3�t�H�[�}�b�g����
	CRegDWORD m_rgEnableAuto;		// �����^��
	CRegDWORD m_rgVolSnd;			// ���ʒ���
	CRegDWORD m_rgVolRcv;			// ���ʒ���

	CStringArray m_aryRecordLimit;	// �^�����ԃ��X�g
	CStringArray m_aryCodecName;	// MP3�R�[�f�b�N�`��
	CStringArray m_aryFormatName;	// MP3�t�H�[�}�b�g�`��

	CAcmMgr m_oAcmMgr;				// ACM�}�l�[�W��
	WAVEFORMATEX m_wfx;				// ���ݑI�𒆂̃t�H�[�}�b�g�ɑΉ�����WAVEFORMATEX

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSettingRecTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSettingRecTab)
	afx_msg void OnBtnRecWarning();
	afx_msg void OnBtnRecStopWarning();
	afx_msg void OnRdoStereo();
	afx_msg void OnCmbCodec();
	afx_msg void OnRdoMp3();
	afx_msg void OnChgVolSnd();
	afx_msg void OnChgVolRcv();
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

#endif // !defined(AFX_SETTINGRECTAB_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
