#if !defined(AFX_MSGLIST_H__64AA0262_2D7C_4E02_A9B1_E1C68488FAA8__INCLUDED_)
#define AFX_MSGLIST_H__64AA0262_2D7C_4E02_A9B1_E1C68488FAA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgList.h : �w�b�_�[ �t�@�C��
//
//#include "XListCtrl.h"
#include "ReportCtrl.h"
#include "CppSQLite3.h"
#include "TPerror.h"

/////////////////////////////////////////////////////////////////////////////
// CMsgList �t�H�[�� �r���[

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMsgList : public TVisualFormView
{
protected:
	CMsgList();           // ���I�����Ɏg�p�����v���e�N�g �R���X�g���N�^�B
	DECLARE_DYNCREATE(CMsgList)

// �t�H�[�� �f�[�^
public:
	//{{AFX_DATA(CMsgList)
	enum { IDD = IDD_MSGLISTFORM };
	CReportCtrl m_wndList;	// ���X�g����N���X
	//}}AFX_DATA

// �A�g���r���[�g
public:
	CImageList  m_ListImage;	// ���X�g�A�C�R�����i�[����|�C���^
	CImageList  m_DragImage;	// �h���b�O�C���[�W���i�[����|�C���^
	BOOL        m_bDrag;		// �h���b�O���Ȃ�TRUE�A�h���b�O���łȂ����FALSE
	int			m_hItemDrag;	// �h���b�O�A�C�e���i�R�s�[���A�C�e���j�̃n���h��
	HCURSOR		m_DragCursor;	// �h���b�O���̃o�b�N�A�b�v�J�[�\��

	int m_hActive;			// �I���A�C�e��
//	int m_hContext;			// �R���e�L�X�g���j���[���́i���j�I���A�C�e��
	DWORD m_dwFolderType;	// �R���e�L�X�g���j���[���̃t�H���_�^�C�v
	DWORD m_dwFolderId;		// �R���e�L�X�g���j���[���̃t�H���_ID
	CUIntArray m_aryClip;	// �R�s�[�E�\��t������ID�ۑ��̈�

	COLORREF m_clrUnview;	// ���Đ����X�g�J���[

// �I�y���[�V����
public:

private:
	CTPerror te;
	void InitListCtrl(void);
	void AdustColumns(void);	// ���X�g�̃J�����T�C�Y����
	BOOL MakeList(DWORD lngFolderType, long lngFolderId);	// ���b�Z�[�W���X�g����
	BOOL Move(long lngFolderId);		// �I�����ꂽ���b�Z�[�W���ړ�
	BOOL TrashDel();		// ���ݔ�(��M)���̃A�C�e���폜
	BOOL TrashRecover();	// ���ݔ�(��M)���̃A�C�e�������ɖ߂�
	long SelectedList(CUIntArray *pAryIdx, BOOL blData);						// ���b�Z�[�W�I�����X�g�̎擾
	CPoint CnvPntM_F(const CPoint point);	// �N���C�A���g���W�ϊ��iMsgList->FolderTree�j
	BOOL Play();	// �����Đ�
	BOOL ExecPrg(int intPrgId);		// �A�g�v���O�������s
	BOOL ChkPlayIcon(const CPoint point);	// �w����W�������Đ��A�C�R����ɂ��邩
	BOOL ExeProcess(LPTSTR cmdstr, LPTSTR dirstr, int nShowCmd, DWORD dwTimeout);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMsgList)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	virtual ~CMsgList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CMsgList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMsgCall();
	afx_msg void OnLinkRebtel();
	afx_msg void OnMsgSetup();
	afx_msg void OnMsgDel();
	afx_msg void OnPrg1();
	afx_msg void OnPrg11();
	afx_msg void OnPrg901();
	afx_msg void OnTrashRecover();
	afx_msg void OnTrashClear();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL ContextMenu(CPoint point);
	BOOL UpdateForm();	// ���X�g�̐����E�X�V
	BOOL ChgMsgMark(BOOL blUpdate = TRUE);	// ���Đ����X�g���Đ��ς݂ɕύX
	long SelectedId();			// �I�����X�gID�̎擾
	BOOL SelectId(int idx);	// ���X�g�I��
  };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MSGLIST_H__64AA0262_2D7C_4E02_A9B1_E1C68488FAA8__INCLUDED_)
