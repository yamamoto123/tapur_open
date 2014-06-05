// RsfTree.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "RsfTree.h"
#include "RsfList.h"
#include "RsfNewDialog.h"
#include "RsfFolderNewDialog.h"
#include "RsfFolderRenameDialog.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CRsfTree

IMPLEMENT_DYNCREATE(CRsfTree, CTreeView)

CRsfTree::CRsfTree()
{
}

CRsfTree::~CRsfTree()
{
}


BEGIN_MESSAGE_MAP(CRsfTree, CTreeView)
	//{{AFX_MSG_MAP(CRsfTree)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_POPUP_SETUP_NEW, OnSetupNew)
	ON_COMMAND(ID_POPUP_FOLDER_NEW, OnFolderNew)
	ON_COMMAND(ID_POPUP_FOLDER_RENAME, OnFolderRename)
	ON_COMMAND(ID_POPUP_FOLDER_DEL, OnFolderDel)
	ON_COMMAND(ID_POPUP_TRASHCAN_CLEAR, OnTrashCanClear)
	ON_COMMAND(ID_POPUP_TRASH_RECOVER, OnTrashRecover)
	ON_COMMAND(ID_POPUP_TRASH_CLEAR, OnTrashClear)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfTree �`��

void CRsfTree::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���̈ʒu�ɕ`��p�̃R�[�h��ǉ����Ă�������
}

/////////////////////////////////////////////////////////////////////////////
// CRsfTree �f�f

#ifdef _DEBUG
void CRsfTree::AssertValid() const
{
	CTreeView::AssertValid();
}

void CRsfTree::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRsfTree ���b�Z�[�W �n���h��

int CRsfTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������

	// Set tree control styles
	CTreeCtrl &tc = GetTreeCtrl();
	long style = GetWindowLong(tc.m_hWnd,GWL_STYLE);
	style |= (TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | 
			TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP);
	SetWindowLong(tc.m_hWnd,GWL_STYLE,style);

	// �A�C�R���ݒ�
	HICON h;
	m_imgl.Create(16, 16, ILC_COLORDDB, 0, 0);
	h = AfxGetApp()->LoadIcon(IDI_F_ROOT);
	m_imgl.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_F_RESP_CLOSE);
	m_imgl.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_F_RESP_OPEN);
	m_imgl.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_F_RESP_TRASH);
	m_imgl.Add(h);
	tc.SetImageList(&m_imgl, TVSIL_NORMAL);

	return 0;
}

void CRsfTree::OnDestroy() 
{
	CTreeView::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
/*
	/// �c���[�̓W�J��Ԃ����W�X�g���ɕۑ�
	DOWRD dw;
	CTreeCtrl &tc = GetTreeCtrl();
	if (dw == 1) tc.Expand(m_hRoot, TVE_EXPAND);

	m_rgRootStatus = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\RootFolderStatus", 1);
	m_rgMsgDefStatus = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\MsgFolderDefStatus", 1);
	m_rgMsgTrashDefStatus = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\MsgFolderTrashDefStatus", 1);
	m_rgSetupDefStatus = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\SetupFolderDefStatus", 1);
	m_rgSetupTrashDefStatus = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\SetupFolderTrashDefStatus", 1);
	dw = m_rgRootStatus;
	if (dw == 1) tc.Expand(m_hRoot, TVE_EXPAND);
	dw = m_rgMsgDefStatus;
	if (dw == 1) tc.Expand(m_hFolderDef, TVE_EXPAND);
	dw = m_rgMsgTrashDefStatus;
	if (dw == 1) tc.Expand(m_hFolderTrashDef, TVE_EXPAND);
	dw = m_rgSetupDefStatus;
	if (dw == 1) tc.Expand(m_hFolderDef, TVE_EXPAND);
	dw = m_rgSetupTrashDefStatus;
	if (dw == 1) tc.Expand(m_hTrashDef, TVE_EXPAND);
*/
}

// �c���[�A�C�e���̑I��ύX���C�x���g
void CRsfTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    switch( pNMTreeView->action ) {
	case TVC_UNKNOWN:	 // �v���O����
	case TVC_BYKEYBOARD: // �L�[�{�[�h����
    case TVC_BYMOUSE:    // �}�E�X����
		FolderChange();
        break;
    }
	
	*pResult = 0;
}

// �c���[�̊J���C�x���g
void CRsfTree::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    switch( pNMTreeView->action ) {
    case TVE_COLLAPSE:  // �܂肽���ݎ�
		FolderChange();
		break;
    }

	*pResult = 0;
}

// OnContextMenu(), OnRButtonUp()�͔F�����Ȃ����߁A���̃n���h�����g�p
void CRsfTree::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	/// �}�E�XY���W������Ɏ擾����Ȃ����߁A�Čv�Z����
	POINT pnt;
//	RECT rct;
	GetCursorPos(&pnt);
	ScreenToClient(&pnt);
//	GetWindowRect(&rct);
//	pNMTreeView->ptDrag.y = pnt.y - rct.top;
	/// �R���e�L�X�g���j���[�̕\��
	ContextMenu(pnt);
//	ContextMenu(pNMTreeView->ptDrag);

	*pResult = 0;
}

// �L�[�C�x���g
void CRsfTree::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	RECT rct;
	CPoint point, point2;
	/// TAB KEY : ���X�g�t�H�[���փt�H�[�J�X�ړ�
	/// ���X�g����̏ꍇ�̓t�H�[�J�X�ړ����Ȃ�
	if (pTVKeyDown->wVKey == VK_TAB) {
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CRsfList *pfview = (CRsfList *)pFrame->m_pRsfList->GetWnd();
		CReportCtrl &rc = pfview->m_wndList;
		if (rc.GetItemCount() > 0) {
			//// ���X�g�̐擪�s��I��
			// (?)�ȉ��̕��@�ł̓��b�Z�[�W���X�g�̃R���e�L�X�g���j���[�̃R�}���h�����s����Ȃ�
			// �[���I�Ƀ}�E�X���������đΉ�
//			pfview->SetFocus();
//			rc.SetAllItemStates(RC_ITEM_ALL, RC_ITEM_UNSELECTED);
//			rc.SetItemStates(0, RC_ITEM_SELECTED | RC_ITEM_FOCUSED);
			rc.GetItemRect(0, &rct, 0);
			point.x = (rct.left + rct.right) / 2 + 150;		// (?) X���������
			point.y = (rct.top + rct.bottom) / 2;
			ClientToScreen(&point);
			SetCursorPos(point.x, point.y);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
	} else if (pTVKeyDown->wVKey == VK_RETURN) {
	/// RETURN KEY : �R���e�L�X�g���j���[�̕\��
		/// ���ݑI������Ă���A�C�e���ʒu�ŕ\��
		CTreeCtrl &tc = GetTreeCtrl();
		m_hContext = tc.GetSelectedItem();
		if (m_hContext > 0) {
			tc.GetItemRect(m_hContext, &rct, 0);
			point.x = (rct.left + rct.right) / 2;
			point.y = (rct.top + rct.bottom) / 2;
			ContextMenu(point);
		}
	}
	
	*pResult = 0;
}

// �����ݒ�ǉ��_�C�A���O
void CRsfTree::OnSetupNew()
{
	CRsfNewDialog dlg;
	dlg.DoModal();
}

// �t�H���_�ǉ��_�C�A���O
void CRsfTree::OnFolderNew()
{
	CRsfFolderNewDialog dlg;
	dlg.DoModal();
}

// �t�H���_���̕ύX�_�C�A���O
void CRsfTree::OnFolderRename()
{
	CRsfFolderRenameDialog dlg;
	dlg.DoModal();
}

// �t�H���_�폜
void CRsfTree::OnFolderDel()
{
	if (m_dwFolderType == FD_NORMAL) {
		FolderDel();
	}
}

// ���ݔ��N���A
void CRsfTree::OnTrashCanClear()
{
	if (m_dwFolderType == FD_TRASH_DEF) {
		TrashClear();
	}
}

// ���ݔ����̃t�H���_�폜
void CRsfTree::OnTrashClear()
{
	if (m_dwFolderType == FD_TRASH) {
		TrashDel();
	}
}

// ���ݔ����̃t�H���_�����ɖ߂�
void CRsfTree::OnTrashRecover()
{
	if (m_dwFolderType == FD_TRASH) {
		TrashRecover();
	}
}

/////////////////////////////////////////////////////////////////////////////

//
//  �@�\     : �R���e�L�X�g���j���[�̕\��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfTree::ContextMenu(
CPoint point)	// �}�E�X�ʒu
{
	/// �}�E�X��̃t�H���_��I��
	CTreeCtrl &tc = GetTreeCtrl();
	UINT intFlag;
	m_hContext = tc.HitTest(point, &intFlag);
	if (m_hContext <= 0) {
		/// �}�E�X��ɂȂ��ꍇ���ݑI������Ă���A�C�e��
		m_hContext = tc.GetSelectedItem();
		if (m_hContext <= 0) return FALSE;
	}
	m_dwFolderType = tc.GetItemData(m_hContext) & FD_MASK_TYPE;
	m_dwFolderId = tc.GetItemData(m_hContext) & FD_MASK_ID;
	m_strFolderName = tc.GetItemText(m_hContext);

	/// POPUP���j���[�\��
	CMenu menu;
	CMenu* pPopup;
	if (m_dwFolderType == FD_NORMAL || m_dwFolderType == FD_NORMAL_DEF) {
		VERIFY(menu.LoadMenu(IDR_POPUP_SETUP));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		if (m_dwFolderType == FD_NORMAL_DEF) {
			pPopup->EnableMenuItem(ID_POPUP_FOLDER_RENAME, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_POPUP_FOLDER_DEL, MF_BYCOMMAND | MF_GRAYED);
		}
	} else if (m_dwFolderType == FD_TRASH_DEF) {
		VERIFY(menu.LoadMenu(IDR_POPUP_TRASHCAN));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
	} else if (m_dwFolderType == FD_TRASH) {
		VERIFY(menu.LoadMenu(IDR_POPUP_TRASH));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
	} else {
		return FALSE;
	}
	
	ClientToScreen(&point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		 point.x, point.y, AfxGetMainWnd());

	return TRUE;
}

//
//  �@�\     : �c���[���ڂ̒ǉ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : �����c���[���ڂ̃n���h��
//  
//  ���l     : 
//  
HTREEITEM CRsfTree::AddItem(
HTREEITEM hParent,	// �e���ڃn���h��
CString strTitle,	// �^�C�g��
DWORD type,			// �t�H���_�^�C�v
DWORD recId = 0)	// ���R�[�hID
{
	CString str;
	HTREEITEM h;
	CTreeCtrl &tc = GetTreeCtrl();

	if (type == FD_ROOT) {
		h = tc.InsertItem(strTitle);
		tc.SetItemImage(h, 0, 0);
	} else if (type == FD_NORMAL) {
		h = tc.InsertItem(strTitle, hParent, TVI_FIRST);
		tc.SetItemImage(h, 1, 2);
	} else if (type == FD_NORMAL_DEF) {
		strTitle.LoadString(IDS_FOLDER_SETUP);	
		h = tc.InsertItem(strTitle, hParent);
		tc.SetItemImage(h, 1, 2);
	} else if (type == FD_TRASH) {
		h = tc.InsertItem(strTitle, hParent);
		tc.SetItemImage(h, 1, 2);
	} else if (type == FD_TRASH_DEF) {
		strTitle.LoadString(IDS_FOLDER_SETUP_TRASH);	
		h = tc.InsertItem(strTitle, hParent);
		tc.SetItemImage(h, 3, 3);
	}
	tc.SetItemData(h, (type | recId));

	return h;
}

//
//  �@�\     : �c���[�̃t�H���_�ꗗ����
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfTree::MakeSubFolders(
DWORD type)			// �t�H���_�^�C�v
{
	HTREEITEM hParent;
	CString strFolderType;
	CString strSystemFlag;

	if (type == FD_NORMAL) {
		hParent = m_hFolderDef;
		strFolderType = _T("1");
		strSystemFlag = _T("0");
	} else if (type == FD_TRASH) {
		hParent = m_hTrashDef;
		strFolderType = _T("1");
		strSystemFlag = _T("1");
	} else {
		return FALSE;
	}

	/// �t�H���_�ꗗ�擾
	CString strWhere;
	strWhere.Empty();
	strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
	strWhere = strWhere + _T(" and ") + _T("lngFolderType") + _T("=") + strFolderType;
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag") + _T("=") + strSystemFlag;
	strWhere = strWhere + _T(" order by strTitle");

	//// �f�[�^�x�[�X����folder�e�[�u���Q��
	CString str;
	long lngCnt;
	CTblFolder tbl(theApp.m_strDBPath);
	lngCnt = tbl.SelectRec(strWhere);
	while (lngCnt > 0)
	{
		str = tbl.strTitle;
		AddItem(hParent, str, type, tbl.ID);
		if (!tbl.NextRec()) break;
		--lngCnt;
	}
		
	return TRUE;
}

//
//  �@�\     : �c���[�̐����E�X�V
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfTree::UpdateForm(
BOOL blForce)	// �����X�V
{
	CString str;

	/// �t�H���_�����݂���ꍇ�͂��̂܂�
	CTreeCtrl &tc = GetTreeCtrl();
	if (!blForce) {
		if (!(tc.GetCount() == 0 && _tcsclen(theApp.m_strUsrId) > 0)) return TRUE;
	}

	/// �t�H���_���N���A
	tc.DeleteAllItems();

	/// ���[�U�iroot�j
	if (theApp.m_strUsrName.IsEmpty()) {
		m_hRoot = AddItem(NULL, theApp.m_strUsrId, FD_ROOT);
	} else {
		m_hRoot = AddItem(NULL, theApp.m_strUsrName, FD_ROOT);
	}
	/// "�����ݒ�"
	m_hFolderDef = AddItem(m_hRoot, str, FD_NORMAL_DEF);
	/// "���ݔ�"(�����ݒ�)
	m_hTrashDef = AddItem(m_hRoot, str, FD_TRASH_DEF);
	/// �����ݒ�p�t�H���_�ꗗ
	MakeSubFolders(FD_NORMAL);
	/// �����ݒ�p���ݔ��t�H���_�ꗗ
	MakeSubFolders(FD_TRASH);

	/// �c���[�W�J(���W�X�g�����擾)
	DWORD dw;
	m_rgRootStatus = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\RootFolderStatus"), 1, TRUE);
	m_rgFolderDefStatus = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SetupFolderDefStatus"), 1, TRUE);
	m_rgTrashDefStatus = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SetupFolderTrashDefStatus"), 1, TRUE);
	dw = m_rgRootStatus;
	if (dw == 1) tc.Expand(m_hRoot, TVE_EXPAND);
	dw = m_rgFolderDefStatus;
	if (dw == 1) tc.Expand(m_hFolderDef, TVE_EXPAND);
	dw = m_rgTrashDefStatus;
	if (dw == 1) tc.Expand(m_hTrashDef, TVE_EXPAND);

	/// �����ݒ���A�N�e�B�u�ɂ���
	tc.SelectItem(m_hFolderDef);
		
	return TRUE;
}

//
//  �@�\     : �t�H���_�ύX������
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfTree::FolderChange(void) 
{
	CString str;

	/// �J�����g�t�H���_�̎擾
	CTreeCtrl &tc = GetTreeCtrl();
	m_hActive = tc.GetSelectedItem();
	str = tc.GetItemText(m_hActive);
	theApp.m_strActiveFolder = str;
	m_dwFolderType = tc.GetItemData(m_hActive) & FD_MASK_TYPE;
	m_dwFolderId = tc.GetItemData(m_hActive) & FD_MASK_ID;

	/// ���b�Z�[�W���X�g�̍X�V
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfList *pfview = (CRsfList *)pFrame->m_pRsfList->GetWnd();
	pfview->UpdateForm();

	/// ���b�Z�[�W���X�g����̏ꍇ�A�t�H���_�^�u�\��
	CReportCtrl &rc = pfview->m_wndList;
	if (rc.GetItemCount() <= 0) {
		//// �^�u���x�����t�H���_���ɂ���
		HTREEITEM h = m_hActive;
		while (h != m_hRoot) {
			h = tc.GetParentItem(h);
			if(h == m_hRoot) break;
			str.Insert(0, _T(" - "));
			str.Insert(0, tc.GetItemText(h));
		}
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		pFrame->m_Framework.ShowTab(pFrame->m_pRsfFolderForm, TRUE);
		pFrame->m_Framework.SetTabLabel(pFrame->m_pRsfFolderForm, str);
		pFrame->m_Framework.SetActiveTab(pFrame->m_pRsfFolderForm);
		pFrame->m_Framework.ShowTab(pFrame->m_pRsfSimpleForm, FALSE);
	}

	/// �t�H�[�J�X�Đݒ�i���b�Z�[�W���X�g�̍X�V�ň�U�͂���邽�߁j
	this->SetFocus();

	return TRUE;
}


//
//  �@�\     : �����t�H���_�폜
//  
//  �@�\���� : �폜�t���O��t��
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfTree::FolderDel()
{
	CString str;

	/// ���I���t�H���_���
	CString strTitle = m_strFolderName;
	long lngFolderId = m_dwFolderId;

	/// �m�F���b�Z�[�W
	str.LoadString(IDS_MSGBOX23);
	str = strTitle + _T(" : ") + str;
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;

	/// folder�e�[�u�����X�V
	CTblFolder ftbl(theApp.m_strDBPath);
	if (ftbl.SelectRec(lngFolderId) != 1) {
		return FALSE;
	}
	ftbl.lngSystemFlag = 1;				// �폜�t���O�t��
	ftbl.UpdateRec(lngFolderId);

	/// �t�H���_�����b�Z�[�W���X�V
	_TCHAR buf[32];
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	CString strFolderId = buf;
	CTblRsfl stbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T("lngFolderId") + _T("=") + buf;
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
	stbl.UpdateRec(_T("lngSystemFlag=1"), strWhere);	// �폜�t���O�t��

	/// ��ʍX�V
	//// �c���[���ڂ̍폜
	CTreeCtrl &tc = GetTreeCtrl();
	tc.DeleteItem(m_hContext);
	//// ���ݔ��t�H���_�ɒǉ�
	AddItem(m_hTrashDef, strTitle, FD_TRASH, lngFolderId);
	//// �I���t�H���_���폜�����ꍇ�A�f�t�H���g�t�H���_���đI��
	if (m_hContext == m_hActive) {
		tc.SelectItem(m_hFolderDef);
	}

	return TRUE;
}

//
//  �@�\     : ���ݔ�(����)�N���A
//  
//  �@�\���� : �e�[�u������폜
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfTree::TrashClear()
{
	CString str;

	/// �m�F���b�Z�[�W
	str.LoadString(IDS_MSGBOX31);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;

	/// �t�H���_���폜
	CTblFolder ftbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	strWhere = strWhere + _T("lngFolderType=1");
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
	ftbl.DeleteRec(strWhere);

	//// �����ݒ���폜
	strWhere = _T("lngSystemFlag=1");
	SqlClear(strWhere);

	/// ��ʍX�V
	//// �c���[���ڂ̍폜
	BOOL blFlag = FALSE;
	HTREEITEM h, hTrashDef;
	hTrashDef = m_hTrashDef;
	CTreeCtrl &tc = GetTreeCtrl();
	h = tc.GetChildItem(hTrashDef);
	while (h != NULL) {
		if (h == m_hActive) blFlag = TRUE;
		tc.DeleteItem(h);
		h = tc.GetChildItem(hTrashDef);
	}
	if (blFlag) {
		//// �I���t�H���_���폜�����ꍇ�A���ݔ��t�H���_���đI��
		tc.SelectItem(hTrashDef);
	} else {
		//// ���ݔ��t�H���_��I�����Ă����ꍇ���b�Z�[�W���X�g�X�V
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CRsfList *pfview = (CRsfList *)pFrame->m_pRsfList->GetWnd();
		pfview->UpdateForm();
	}

	return TRUE;
}

//
//  �@�\     : ���ݔ�(����)���̃t�H���_�폜
//  
//  �@�\���� : �e�[�u������폜
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfTree::TrashDel()
{
	CString str;
	_TCHAR buf[32];

	/// ���I���t�H���_���
	CString strTitle = m_strFolderName;
	long lngFolderId = m_dwFolderId;

	/// �m�F���b�Z�[�W
	str.LoadString(IDS_MSGBOX32);
	str = strTitle + _T(" : ") + str;
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;

	/// �t�H���_���폜
	CTblFolder ftbl(theApp.m_strDBPath);
	ftbl.DeleteRec(lngFolderId);

	/// �t�H���_�����b�Z�[�W���폜
	CString strWhere;
	strWhere.Empty();
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T("lngFolderId") + _T("=") + buf;
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
	SqlClear(strWhere);

	/// ��ʍX�V
	//// �c���[���ڂ̍폜
	CTreeCtrl &tc = GetTreeCtrl();
	tc.DeleteItem(m_hContext);
	//// �I���t�H���_���폜�����ꍇ�A���ݔ��t�H���_���đI��
	if (m_hContext == m_hActive) {
		tc.SelectItem(m_hTrashDef);
	}

	return TRUE;
}

//
//  �@�\     : �����f�[�^�̊��S�폜
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfTree::SqlClear(
CString strWhere)	// �폜�Ώۃf�[�^�I��pWHERE��
{
	int i;
	CString str;
	CFileSpec fs;
	int intCnt;

	CTblRsfl stbl(theApp.m_strDBPath);

	//// �ݒ�E�����E�`���b�g�t�@�C���̍폜
	CTPsetup tps(theApp.m_strDataFolder);
	intCnt = stbl.SelectRec(strWhere);
	for (i = 0; i < intCnt; i++) {
		tps.SetCallerId(stbl.strSenderId);
		///// �ݒ�t�@�C��
		str = tps.RsfFileName();
		fs.SetFullSpec(str);
		if (fs.Exist()) fs.FileDelete();
		///// �����t�@�C��
		str = tps.RsfAudioFileName(_T("$"));
		fs.SetFullSpec(str);
		if (fs.Exist()) fs.FileDelete();
		///// �r�f�I�t�@�C��
		str = tps.RsfVideoFileName(_T("$"));
		fs.SetFullSpec(str);
		if (fs.Exist()) fs.FileDelete();
		///// �摜�t�@�C��
		str = tps.FindRsfImgFile(_T("$"));
		fs.SetFullSpec(str);
		if (fs.Exist()) fs.FileDelete();
		///// �`���b�g�t�@�C��
		str = tps.RsfChatFileName(_T("$"));
		fs.SetFullSpec(str);
		if (fs.Exist()) fs.FileDelete();
		stbl.NextRec();
	}

	//// ���R�[�h�̍폜
	stbl.DeleteRec(strWhere);

	return TRUE;
}


//
//  �@�\     : ���ݔ�(����)���̃t�H���_�����ɖ߂�
//  
//  �@�\���� : �폜�t���O������
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfTree::TrashRecover()
{
	CString str;

	/// ���I���t�H���_���
	CString strTitle = m_strFolderName;
	long lngFolderId = m_dwFolderId;

	/// �m�F���b�Z�[�W
	str.LoadString(IDS_MSGBOX33);
	str = strTitle + _T(" : ") + str;
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return FALSE;

	/// folder�e�[�u�����X�V
	CTblFolder ftbl(theApp.m_strDBPath);
	if (ftbl.SelectRec(lngFolderId) != 1) {
		return FALSE;
	}
	ftbl.lngSystemFlag = 0;				// �폜�t���O����
	ftbl.UpdateRec(lngFolderId);

	/// �t�H���_�����b�Z�[�W���X�V
	_TCHAR buf[32];
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	CString strFolderId = buf;
	CTblRsfl stbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
	strWhere = strWhere + _T("lngFolderId") + _T("=") + buf;
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
	stbl.UpdateRec(_T("lngSystemFlag=0"), strWhere);	// �폜�t���O����

	/// ��ʍX�V
	//// �c���[���ڂ̍폜
	CTreeCtrl &tc = GetTreeCtrl();
	tc.DeleteItem(m_hContext);
	//// ���̃t�H���_�ɒǉ�
	AddItem(m_hFolderDef, strTitle, FD_NORMAL, lngFolderId);
	//// �I���t�H���_���폜�����ꍇ�A���ݔ��t�H���_���đI��
	if (m_hContext == m_hActive) {
		tc.SelectItem(m_hTrashDef);
	}

	return TRUE;
}

//
//  �@�\     : �t�H���_���`�F�b�N
//  
//  �@�\���� : ���ݔ����̃t�H���_���Ώ�
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfTree::ChkFolder(
CString strFolder,	// �t�H���_��
DWORD type)			// �t�H���_�^�C�v
{
	CString str;

	if (strFolder.IsEmpty()) {
		str.LoadString(IDS_MSGBOX22);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	/// �����t�H���_�Ƃ̏d���`�F�b�N
	//// "��M��"�A"���ݔ�(��M)"�A"�����ݒ�"�A"���ݔ�(����)"�͋֎~
	CString str1, str2, str3, str4;
	str1.LoadString(IDS_FOLDER_MSG);
	str2.LoadString(IDS_FOLDER_MSG_TRASH);	
	str3.LoadString(IDS_FOLDER_SETUP);
	str4.LoadString(IDS_FOLDER_SETUP_TRASH);	
	if (str1 == strFolder || str2 == strFolder || str3 == strFolder || str4 == strFolder) {
//	if (theApp.m_strUsrId == strFolder || str1 == strFolder || str2 == strFolder) {
		str.LoadString(IDS_MSGBOX21);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}
	//// ���̑��̃t�H���_��
	CTblFolder tbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
	strWhere = strWhere + _T(" and ") + _T("strTitle") + _T("='") + strFolder + _T("'");
	if (type == FD_NORMAL || type == FD_NORMAL_DEF) {
		strWhere = strWhere + _T(" and ") + _T("lngFolderType=1");
	}
	strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
	long lngCnt = tbl.CountRec(strWhere);
	if (lngCnt > 0) {
		str.LoadString(IDS_MSGBOX21);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	return TRUE;
}

//
//  �@�\     : ID�ɂ��t�H���_����
//  
//  �@�\���� : ���ݔ����̃t�H���_���Ώ�
//  
//  �Ԃ�l   : HTREEITEM
//  
//  ���l     : 
//  
HTREEITEM CRsfTree::FindItemId(HTREEITEM hti, long lngId)
{
	CTreeCtrl &tc = GetTreeCtrl();

	if(hti == NULL) return NULL;
	if((tc.GetItemData(hti) & FD_MASK_ID) == lngId) return hti;

	hti = tc.GetChildItem( hti );
	do
	{
		HTREEITEM hti_res;
		if((hti_res = FindItemId( hti, lngId)) != NULL ) return hti_res;
	} while( (hti = tc.GetNextSiblingItem( hti )) != NULL );

	return NULL;
}

//
//  �@�\     : �A�C�e���f�[�^�ɂɂ��t�H���_����
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : HTREEITEM
//  
//  ���l     : 
//  
HTREEITEM CRsfTree::FindItemData(HTREEITEM hti, DWORD dwData)
{
	CTreeCtrl &tc = GetTreeCtrl();

	if(hti == NULL) return NULL;
	if(tc.GetItemData( hti ) == dwData) return hti;

	hti = tc.GetChildItem( hti );
	do
	{
		HTREEITEM hti_res;
		if((hti_res = FindItemData( hti, dwData)) != NULL ) return hti_res;
	} while( (hti = tc.GetNextSiblingItem( hti )) != NULL );

	return NULL;
}

