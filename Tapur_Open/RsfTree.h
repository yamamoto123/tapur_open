#if !defined(AFX_RSFTREE_H__F4F2B8E9_082C_475A_845A_60D4562FF840__INCLUDED_)
#define AFX_RSFTREE_H__F4F2B8E9_082C_475A_845A_60D4562FF840__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfTree.h : �w�b�_�[ �t�@�C��
//
#include <afxcview.h>
#include "TPerror.h"

/////////////////////////////////////////////////////////////////////////////
// CRsfTree �r���[

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

// �t�H���_���(DWORD��ʂP�U�r�b�g�r��)
#define	FD_MASK_ID			0x0000ffff
#define	FD_MASK_TYPE		0xffff0000
#define	FD_ROOT				0x00010000
#define	FD_NORMAL			0x00020000					
#define	FD_NORMAL_DEF		0x00040000					
#define	FD_TRASH			0x00080000	
#define	FD_TRASH_DEF		0x00100000	

class CRsfTree : public CTreeView
{
protected:
	CRsfTree();           // ���I�����Ɏg�p�����v���e�N�g �R���X�g���N�^
	DECLARE_DYNCREATE(CRsfTree)

// �A�g���r���[�g
public:
	HTREEITEM m_hRoot;			// ���[�g�A�C�e��
	HTREEITEM m_hFolderDef;		// �����ݒ�A�C�e��
	HTREEITEM m_hTrashDef;		// ���ݔ��i�ݒ�j�A�C�e��
	HTREEITEM m_hActive;		// �I���A�C�e��
	HTREEITEM m_hContext;		// �R���e�L�X�g���j���[���́i���j�I���A�C�e��
	DWORD m_dwFolderType;	// �R���e�L�X�g���j���[���̃t�H���_�^�C�v
	DWORD m_dwFolderId;		// �R���e�L�X�g���j���[���̃t�H���_ID
	CString m_strFolderName;// �R���e�L�X�g���j���[���̃t�H���_��
	// �c���[�̓W�J���
	CRegDWORD m_rgRootStatus;
	CRegDWORD m_rgFolderDefStatus;
	CRegDWORD m_rgTrashDefStatus;

private:
	CTPerror te;
	CImageList m_imgl;

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CRsfTree)
	protected:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h���܂����B
	//}}AFX_VIRTUAL

private:
	BOOL FolderChange(void);			// �t�H���_�ύX������
	BOOL MakeSubFolders(DWORD type);	// �c���[�̃t�H���_�ꗗ����
	BOOL SqlClear(CString strWhere);	// WHERE��ɂ�鉞���f�[�^�̍폜

// �C���v�������e�[�V����
protected:
	virtual ~CRsfTree();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CRsfTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetupNew();
	afx_msg void OnFolderNew();
	afx_msg void OnFolderRename();
	afx_msg void OnFolderDel();
	afx_msg void OnTrashCanClear();
	afx_msg void OnTrashRecover();
	afx_msg void OnTrashClear();
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL ContextMenu(CPoint point);
	BOOL FolderDel();	// (����)�t�H���_�폜
	BOOL TrashClear();	// ���ݔ�(����)�N���A
	BOOL TrashDel();	// ���ݔ�(����)���̃t�H���_�폜
	BOOL TrashRecover();	// ���ݔ�(����)���̃t�H���_�����ɖ߂�

	BOOL UpdateForm(BOOL blForce = FALSE);		// �c���[�̐����E�X�V
	HTREEITEM AddItem(HTREEITEM hParent, CString strTitle, DWORD type, DWORD recId);	// �c���[���ڂ̒ǉ�
	BOOL ChkFolder(CString strFolder, DWORD type);		// �t�H���_�`�F�b�N
	HTREEITEM FindItemId(HTREEITEM hti, long lngId);		// ID�ɂ��t�H���_����
	HTREEITEM FindItemData(HTREEITEM hti, DWORD dwData);	// �f�[�^�ɂ��t�H���_����
	BOOL IsTrash(DWORD type) {
		return (type == FD_TRASH_DEF || type == FD_TRASH) ? TRUE : FALSE; };
	BOOL IsFolder(DWORD type) {
		return (type == FD_NORMAL_DEF || type == FD_NORMAL) ? TRUE : FALSE; };
	BOOL IsFolderWT(DWORD type) {
		return (type == FD_NORMAL_DEF || type == FD_NORMAL ||
			type == FD_TRASH_DEF || type == FD_TRASH) ? TRUE : FALSE; };
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_RSFTREE_H__F4F2B8E9_082C_475A_845A_60D4562FF840__INCLUDED_)
