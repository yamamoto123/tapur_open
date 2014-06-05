#if !defined(AFX_SETHOTKEY_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SETHOTKEY_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetHotkey.h : �w�b�_�[ �t�@�C��
//
#include "ReportCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSetHotkey �_�C�A���O

class CSetHotkey : public CDialog
{
// �R���X�g���N�V����
public:
	CSetHotkey(CWnd* pParent = NULL);

protected:
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSetHotkey)
	enum { IDD = IDD_HOTKEY };
	CReportCtrl m_lstTool;	// ���X�g����N���X
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA

public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSetHotkey)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSetHotkey)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnHotkeyEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
//���W�X�g���ϐ�
	// ���C����ʕ\��
	CRegDWORD m_rgEnableShow;
	CRegDWORD m_rgModShow;
	CRegDWORD m_rgKeyShow;
	// ���C����ʔ�\��
	CRegDWORD m_rgEnableHide;
	CRegDWORD m_rgModHide;
	CRegDWORD m_rgKeyHide;
	// �^��
	CRegDWORD m_rgEnableRecord;
	CRegDWORD m_rgModRecord;
	CRegDWORD m_rgKeyRecord;
	// �^��
	CRegDWORD m_rgEnableVRecord;
	CRegDWORD m_rgModVRecord;
	CRegDWORD m_rgKeyVRecord;
	// �^���E�^��ꎞ��~
	CRegDWORD m_rgEnableRecPause;
	CRegDWORD m_rgModRecPause;
	CRegDWORD m_rgKeyRecPause;
	// �^���E�^��I��
	CRegDWORD m_rgEnableRecStop;
	CRegDWORD m_rgModRecStop;
	CRegDWORD m_rgKeyRecStop;
	// �Đ��F���݃��X�g�őI������Ă��郌�R�[�h
	CRegDWORD m_rgEnablePlay;
	CRegDWORD m_rgModPlay;
	CRegDWORD m_rgKeyPlay;
	// �Đ��ꎞ��~�E�ĊJ
	CRegDWORD m_rgEnablePlayPause;
	CRegDWORD m_rgModPlayPause;
	CRegDWORD m_rgKeyPlayPause;
	// �Đ��I��
	CRegDWORD m_rgEnablePlayStop;
	CRegDWORD m_rgModPlayStop;
	CRegDWORD m_rgKeyPlayStop;
	// ���̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̎��̃��R�[�h
	CRegDWORD m_rgEnablePlayNext;
	CRegDWORD m_rgModPlayNext;
	CRegDWORD m_rgKeyPlayNext;
	// �O�̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̑O�̃��R�[�h
	CRegDWORD m_rgEnablePlayPrev;
	CRegDWORD m_rgModPlayPrev;
	CRegDWORD m_rgKeyPlayPrev;

	// ���W�X�g���Q��
	BOOL LoadRegistry();

private:
	BOOL AddRecord(UINT RESID, BOOL blChk, UINT uiMod, int iKey);		// ���R�[�h�ǉ�
	BOOL UpdateRecord(int idx, UINT uiMod, int iKey);		// ���R�[�h�X�V
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SETHOTKEY_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
