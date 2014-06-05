// Tapur.h : TAPUR �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_TAPUR_H__55A782FF_7AB5_41FF_BA17_12E16C5027BB__INCLUDED_)
#define AFX_TAPUR_H__55A782FF_7AB5_41FF_BA17_12E16C5027BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Tapur_i.h"
#include "registry.h"
#include "potm.h"
#include "MainFrm.h"
#include "SettingDialog.h"

// �E�B���h�E�N���X��
#define MAIN_CLASS_NAME	"tTapurMain"

class CTapurApp;
extern CTapurApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTapurApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� Tapur.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CTapurApp : public CWinApp
{
public:
	CTapurApp();
	~CTapurApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTapurApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CTapurApp)
	afx_msg void OnAppHelp();
	afx_msg void OnLinkLame();
	afx_msg void OnLinkLicense();
	afx_msg void OnAppAbout();
	afx_msg void OnToolSetting();
	afx_msg void OnToolBackup();
	afx_msg void OnWavFolder();
	afx_msg void OnMp3Folder();
	afx_msg void OnVideoFolder();
	afx_msg void OnRepair();
	afx_msg void OnSoundRecoder();
	afx_msg void OnAudacity();
/*
	afx_msg void OnToolVRecord();
	afx_msg void OnToolRecord();
	afx_msg void OnToolRecPause();
	afx_msg void OnToolRecStop();
	afx_msg void OnToolRecMod();
*/
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bATLInited;
private:
	BOOL InitATL();
	BOOL LoadRegistry();	// ���W�X�g���̃��[�h
	BOOL InitWinDev();		// Windows�f�o�C�X�̏����ݒ�

private:
	HINSTANCE m_hResDLL;	// resource DLL handle

public:
// �O���[�o���ϐ�
	int m_intEnableLog;			// ���O�̗L��/�����t���O

	BOOL m_blSkypeConnect;		// �ڑ��t���O
	IAccessPtr m_pAccess;		// Skype����N���X
//	BOOL m_blSenderCall;		// ���[�U���M�t���O
	long m_lngAppStatus;		// Tapur��ԃt���O�i1:��b�^���A2:����^�A0:�ʏ�j

	CString m_strUsrId;			// ���[�U(�X�J�C�vID)
	CString m_strUsrName;		// ���[�U(�X�J�C�v��)
//	long m_lngCallId;			// �ʘbID
//	CString m_strPartnerId;		// �ʘb����(�X�J�C�vID)
//	CString m_strPartnerName;	// �ʘb����(�X�J�C�v��)
	CString m_strDataFolder;	// �f�[�^�x�[�X�t�H���_
	CString m_strDBPath;		// �f�[�^�x�[�X�t�@�C��
	CString m_strActiveFolder;	// �J�����g�t�H���_
	long m_lngRdflId;			// �J�����g��M���R�[�hID
	long m_lngRsflId;			// �J�����g�ݒ背�R�[�hID

	BOOL OpenUrl(CString strUrl);	// URL�̃u���E�U�\��
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TAPUR_H__55A782FF_7AB5_41FF_BA17_12E16C5027BB__INCLUDED_)
