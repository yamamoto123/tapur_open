#if !defined(AFX_RSFAUDIODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFAUDIODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfAudioDialog.h : �w�b�_�[ �t�@�C��
//
#include <imm.h> 
#include "Audio.h"
#include "Label.h"
#include "TPerror.h"

/////////////////////////////////////////////////////////////////////////////
// CRsfAudioDialog �_�C�A���O

class CRsfAudioDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CRsfAudioDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CRsfAudioDialog();      

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CRsfAudioDialog)
	enum { IDD = IDD_RSF_AUDIO };
	CLabel	m_Time;
	CLabel	m_LinkBuy;
	CBitmapButton m_Play;
	CBitmapButton m_Pause;
	CBitmapButton m_Stop;
	CButton	m_File;
	CProgressCtrl m_Prog;
	//}}AFX_DATA

private:
	CTPerror te;
	CAudio *m_pAudio;		// ��������N���X
	CString m_strPlayFile;	// ���������t�@�C����
	CString m_strTmpFile;	// �o�b�N�A�b�v�����t�@�C����
	long m_lngPlayTime;		// ���t����(sec)
	ST_TIMER m_stPlayChk;	// ���t���ԊǗ��^�C�} 

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CRsfAudioDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CRsfAudioDialog)
	afx_msg void OnBtnPlay();
	afx_msg void OnBtnPause();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnFile();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL StartPlayChkTimer(long lngPlayTime);	// ���t���ԊǗ��^�C�}�J�n
	BOOL StopPlayChkTimer();	// ���t���ԊǗ��^�C�}�I��

public:
	void SetFile(CString strFile) { m_strPlayFile = strFile; }
	BOOL UpdateForm();	// ��ʂ̍X�V
	BOOL Play();		// �����Đ�
	BOOL PlayStop();	// �����Đ���~
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_RSFAUDIODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
