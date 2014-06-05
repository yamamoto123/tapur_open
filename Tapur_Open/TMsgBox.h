#if !defined(AFX_TMSGBOX_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_TMSGBOX_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TMsgBox.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CTMsgBox �_�C�A���O

class CTMsgBox : public CDialog
{
// �R���X�g���N�V����
public:
	CTMsgBox(CWnd* pParent, CString strCaption, CString strMsg, BOOL blTop = FALSE);
	CTMsgBox(CWnd* pParent, CString strCaption, CString strMsg, BOOL blTop, CStringArray &aryName);

protected:
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTMsgBox)
	enum { IDD = IDD_TMSGBOX };
	BOOL m_blChk;
	CComboBox m_pCmb;
	//}}AFX_DATA
	CString	m_strCaption;	// window�^�C�g��
	CString	m_strMsg;		// �\�����b�Z�[�W
	BOOL m_blTop;			// �O�ʕ\��
	CStringArray m_aryName;	// ���X�g�\���̏ꍇ

public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTMsgBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTMsgBox)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_blReturn;
	CString m_strReturn;
	//BOOL EnableCheck(CString strMsg);
	void EnableCombo(CStringArray &aryName);

public:
	void Show(BOOL blModal = TRUE);
	BOOL GetResult() { return m_blReturn; }
	BOOL GetCheck() { return m_blChk; }
	CString GetCombo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TMSGBOX_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
