#if !defined(AFX_SetChatDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_SetChatDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetChatDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CSetChatDialog �_�C�A���O

class CSetChatDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CSetChatDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CSetChatDialog();      

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSetChatDialog)
	enum { IDD = IDD_SET_CHAT };
	CString	m_strChat;
	//}}AFX_DATA

private:
	CString m_strRegKey;	// �Ή����W�X�g���L�[

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSetChatDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSetChatDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void OnButtonInit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetRegKey(CString strRegKey) { m_strRegKey = strRegKey; }
	BOOL UpdateForm();	// ��ʂ̍X�V
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SetChatDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
