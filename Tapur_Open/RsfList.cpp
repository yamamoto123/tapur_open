// RsfList.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "TPutil.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"
#include "MainFrm.h"
#include "RsfList.h"
#include "RsfTree.h"
#include "RsfSimpleForm.h"
#include "RsfNewDialog.h"
#include "RsfRenameDialog.h"
#include "SkypeEvents.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CRsfList

IMPLEMENT_DYNCREATE(CRsfList, TVisualFormView)

CRsfList::CRsfList()
	: TVisualFormView(CRsfList::IDD)
{
	//{{AFX_DATA_INIT(CRsfList)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�̏�����������ǉ����܂�
	//}}AFX_DATA_INIT

	m_bDrag = FALSE;
	m_hItemDrag = 0;
}

CRsfList::~CRsfList()
{
}

void CRsfList::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRsfList)
		// ����: ClassWizard �͂��̈ʒu�� DDX ����� DDV �̌Ăяo����ǉ����܂�
	DDX_Control(pDX, IDC_LIST, m_wndList);	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRsfList, TVisualFormView)
	//{{AFX_MSG_MAP(CRsfList)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchanged)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST, OnBegindrag)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_SETLIST_NEW, OnSetupNew)
	ON_COMMAND(ID_POPUP_SETLIST_RENAME, OnSetupRename)
	ON_COMMAND(ID_POPUP_SETLIST_COPY, OnSetupCopy)
	ON_COMMAND(ID_POPUP_SETLIST_PASTE, OnSetupPaste)
	ON_COMMAND(ID_POPUP_SETLIST_CALL, OnSetupCall)
	ON_COMMAND(ID_POPUP_SETLIST_DEL, OnSetupDel)
	ON_COMMAND(ID_POPUP_TRASH_RECOVER, OnTrashRecover)
	ON_COMMAND(ID_POPUP_TRASH_CLEAR, OnTrashClear)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnLButtonDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnLButtonDblClk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsfList �f�f

#ifdef _DEBUG
void CRsfList::AssertValid() const
{
	CFormView::AssertValid();
}

void CRsfList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRsfList ���b�Z�[�W �n���h��

int CRsfList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (TVisualFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	
	m_aryClip.RemoveAll();
	//m_arySelect.RemoveAll();

	/// Set control styles
/* ���\�[�X�̐ݒ肪�D�悳���̂Ŗ���
	CListCtrl *lc = &m_wndList;
	long style = GetWindowLong(lc->m_hWnd,GWL_STYLE);
	style |= (LVS_SHOWSELALWAYS | LVS_REPORT);
	SetWindowLong(lc->m_hWnd,GWL_STYLE,style);
*/	
	/// �h���b�O�A�C�R���ݒ�
	HICON h;
	m_DragImage.Create(16, 16, ILC_COLORDDB, 0, 0);
	h = AfxGetApp()->LoadIcon(IDI_F_MSG_DRAG);
	m_DragImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_F_RESP_DRAG);
	m_DragImage.Add(h);

	/// ���X�g�A�C�R���ݒ�
	m_ListImage.Create(16, 16, ILC_COLORDDB, 0, 0);
	h = AfxGetApp()->LoadIcon(IDI_MSG_SIMPLE);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_VMSG_SIMPLE);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_MSG_RECORD);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_VMSG_RECORD);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_RESP_SIMPLE);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_MSG_IN);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_MSG_OUT);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_VMSG_IN);
	m_ListImage.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_VMSG_OUT);
	m_ListImage.Add(h);

	/// �F�ݒ�
//	m_clrUnview = RGB(210, 255, 210);	// light green

	return 0;
}

void CRsfList::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit();
	
//	srand((unsigned)time(NULL));

	InitListCtrl();
}

void CRsfList::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	// ���X�g�r���[���E�B���h�E�Ƀt�B�b�g������
    m_wndList.ResizeToFitParent();
	
	// ���X�g�̃J�����T�C�Y����
	AdustColumns();
}

// ���X�g�I�����C�x���g
void CRsfList::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int iItem = -1;
	int iSubItem = -1;
	UINT state;
	CRsfSimpleForm *prsv;
	*pResult = 0;

	if (!pNMLV) return;
	iItem = pNMLV->iItem;
	iSubItem = pNMLV->iSubItem;
	if (iItem < 0 || iSubItem < 0) return;
	if (m_wndList.GetSelectedCount() <= 0) return;

/*
	/// ���{�^���ł��I�������̂ŁA�����ŉ�������B
	int iFocus;
	short sht;
	sht = GetAsyncKeyState(VK_RBUTTON);
	if (sht & 0x80000000) {
		iFocus = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
		if( iFocus != -1 ) {
			m_wndList.SetItemState(iFocus, ~LVIS_SELECTED, LVIS_SELECTED);
			goto PROC_END;
		}
	}
*/
	state = pNMLV->uNewState;
	/// ���X�g�I����
	if ((state & LVIS_SELECTED) != (pNMLV->uOldState & LVIS_SELECTED)) {
		m_hActive = iItem;	// �A�N�e�B�u�A�C�e��
		//// �^�u�\���p������̍쐬
		CString strHead;
		strHead = m_wndList.GetItemText(iItem, 1);	// ���M�Җ�
//		strHead = strHead + _T(" - ") + m_wndList.GetItemText(iItem, 2);	// ����
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
		switch (pfview->m_dwFolderType) {
		case FD_NORMAL:
		case FD_NORMAL_DEF:
		case FD_TRASH:
		case FD_TRASH_DEF:
			//// �J�����g�ݒ背�R�[�hID�̕ێ�
			theApp.m_lngRsflId = m_wndList.GetItemData(iItem);					
			theApp.m_lngRdflId = -1;
			//// �ݒ�^�u�̕\��
			pFrame->m_Framework.ShowTab(pFrame->m_pRsfSimpleForm, TRUE);
			pFrame->m_Framework.SetTabLabel(pFrame->m_pRsfSimpleForm, strHead);
			pFrame->m_Framework.SetActiveTab(pFrame->m_pRsfSimpleForm);
			pFrame->m_Framework.ShowTab(pFrame->m_pRsfFolderForm, FALSE);
			//// �ݒ�t�H�[�����X�V
			prsv = (CRsfSimpleForm *)pFrame->m_pRsfSimpleForm->GetWnd();
			prsv->UpdateForm();
			break;
		default:
			return;
		}
	} else if ((state & LVIS_STATEIMAGEMASK) != (pNMLV->uOldState & LVIS_STATEIMAGEMASK)) {
	}
/*
	if ((newimage = pnmlist->uNewState & LVIS_STATEIMAGEMASK)
                    !=(pnmlist->uOldState & LVIS_STATEIMAGEMASK))
                {
                    if (newimage == INDEXTOSTATEIMAGEMASK(2))
                        TRACE("%d�̃`�F�b�N���ꂽ�_n", pnmlist->iItem);
                    else if (newimage == INDEXTOSTATEIMAGEMASK(1))
                        TRACE("%d�̃`�F�b�N���O���ꂽ�_n", pnmlist->iItem);
                }
*/

	*pResult = 0;
}

// Tab�L�[���������ꍇ�AOnKeyDown�֐��Ŏ擾���邱�Ƃ��ł��Ȃ����߁A�����Ŏ擾����B
BOOL CRsfList::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		/// TAB KEY : ���X�g�t�H�[���փt�H�[�J�X�ړ��i���X�g����̏ꍇ�̓t�H�[�J�X�ړ����Ȃ��j
		/// RETRUN KEY : ���j���[�\��
		if (nVirtKey == VK_TAB) {
			CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
			CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
			CRsfSimpleForm *prsv;
			switch (pfview->m_dwFolderType) {
			case FD_NORMAL:
			case FD_NORMAL_DEF:
			case FD_TRASH:
			case FD_TRASH_DEF:
				prsv = (CRsfSimpleForm *)pFrame->m_pRsfSimpleForm->GetWnd();
				prsv->SetFocus();
				break;
			}
			return TRUE;
		} else if (nVirtKey == VK_RETURN) {
			/// �R���e�L�X�g���j���[�̕\��
			/// ���ݑI������Ă���A�C�e���ʒu�ŕ\��
			POSITION pos = m_wndList.GetFirstSelectedItemPosition();
			if (pos != NULL) {
				int idx = m_wndList.GetNextSelectedItem(pos);
				RECT rct;
				m_wndList.GetItemRect(idx, &rct, 0);
				CPoint point;
				point.x = (rct.left + rct.right) / 2;
				point.y = (rct.top + rct.bottom) / 2;
				ContextMenu(point);
				return TRUE;
			}
		} else if (nVirtKey == VK_DELETE) {
			/// ���ݑI������Ă���A�C�e�����폜
			POSITION pos = m_wndList.GetFirstSelectedItemPosition();
			if (pos != NULL) {
				if (m_dwFolderType == FD_NORMAL || m_dwFolderType == FD_NORMAL_DEF) {
					OnSetupDel();
				} else {
					OnTrashClear();
				}
				return TRUE;
			}
		}
	}
	
	return TVisualFormView::PreTranslateMessage(pMsg);
}

// �h���b�O�J�n�n���h��
void CRsfList::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	/// �I�����ڂ��Ȃ����͖���
	if (m_wndList.GetSelectedCount() == 0) return;

	/// �����t�H���_���
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	m_dwFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
	if (pfview->IsTrash(m_dwFolderType)) {
		m_dwFolderType = 0;
		return;
	}
	m_dwFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;

	/// �h���b�O���t���O��ON���ACImageList::BeginDrag�֐������s
	m_bDrag = TRUE;
	if (pfview->IsFolder(m_dwFolderType)) {
		m_DragImage.BeginDrag(1, CPoint(0, 0));
	} else {
		return;
	}
	/// �J�[�\�����o�b�N�A�b�v
	m_DragCursor = GetCursor();

	/// CImageList::DragEnter�֐������s�i���W�̓X�N���[�����W�ɂ���K�v����j
	POINT pt = pNMLV->ptAction;
	ClientToScreen( &pt );
	m_DragImage.DragEnter(NULL, pt);

	/// �}�E�X�L���v�`���[
	SetCapture();

	*pResult = 0;
}

// �h���b�O���n���h��
void CRsfList::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	if (m_bDrag)
	{
		/// �J�[�\�����֎~�J�[�\���ɂ���
		HCURSOR cursor = AfxGetApp()->LoadStandardCursor(IDC_NO);
		SetCursor(cursor);

		/// CImageList::DragMove�֐������s�i���W�̓X�N���[�����W�ɂ���K�v����j
		POINT pt = point;
		ClientToScreen( &pt );
		CImageList::DragMove(pt);

		/// �I�����ڂ��������́u�K��̉����v�݂̂̏ꍇ�͖���
		if (m_dwFolderType == FD_NORMAL_DEF) {
			CUIntArray aryIdx;
			UINT intCnt1 = SelectedList(&aryIdx, FALSE, FALSE);
			UINT intCnt2 = SelectedList(&aryIdx, FALSE, TRUE);
			//// �f�t�H���g�ݒ���܂�łP������
			if (intCnt1 == 0 && intCnt2 == 1) {
				return;
			}
		}

		/// �t�H���_�̏ォ�`�F�b�N
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
		CTreeCtrl &tc = pfview->GetTreeCtrl();
		CPoint pnt = CnvPntM_F(point);
		HTREEITEM hItem = tc.HitTest(pnt, NULL);
		if ( hItem && (hItem != pfview->m_hActive))
		{
			DWORD type = tc.GetItemData(hItem) & FD_MASK_TYPE;
			/// �t�H���_�̎�ʂ���v���邩�`�F�b�N
			if (((pfview->IsFolder(type) || type == FD_TRASH_DEF) && pfview->IsFolder(m_dwFolderType))) 
			{
				/// �J�[�\����߂�
				SetCursor(m_DragCursor);
				/// �h���b�v��������\������
				CImageList::DragShowNolock(FALSE);
				tc.SelectDropTarget(hItem);
				CImageList::DragShowNolock(TRUE);
			}
		}
	}

	TVisualFormView::OnMouseMove(nFlags, point);
}

// �h���b�v�n���h��
void CRsfList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
		if (m_bDrag)
		{
			/// �h���b�O���t���O��OFF���A�}�E�X�L���v�`���[�����
			m_bDrag = FALSE;
			ReleaseCapture();

			/// CImageList::DragLeave�֐���CImageList::EndDrag�֐������s
			CImageList::DragLeave(this);
			CImageList::EndDrag();

			/// �h���b�v��̋����\��������
			CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
			CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
			CTreeCtrl &tc = pfview->GetTreeCtrl();
			tc.SelectDropTarget(NULL);

			/// �J�[�\����߂�
			SetCursor(m_DragCursor);

			/// �h���b�v��c���[�A�C�e���̃n���h�����擾
			CPoint pnt = CnvPntM_F(point);
			HTREEITEM hItemDrop = tc.HitTest(pnt, NULL);
			if (!hItemDrop) return;
			long lngFolderId = tc.GetItemData(hItemDrop) & FD_MASK_ID;

			/// �h���b�v�悪���݂��Ȃ��ꍇ�A�h���b�O�A�C�e���ƃh���b�v��A�C�e�����������ꍇ�͉������Ȃ�
			if( hItemDrop == NULL || pfview->m_hActive == hItemDrop )
			{
				return;
			}

			/// �h���b�v��t�H���_���̏���
			DWORD type = tc.GetItemData(hItemDrop) & FD_MASK_TYPE;
			if (pfview->IsFolder(m_dwFolderType)) {
				if (pfview->IsFolderWT(type)) {
					if (type == FD_TRASH_DEF) {
						/// �����ݒ�����ݔ���
						Move(-1);
					} else if (type == FD_TRASH) {
						return;
					} else {
						/// �����ݒ���ړ�
						Move(lngFolderId);
					}
				} else {
					/// �t�H���_�̎�ʂ���v���Ȃ��ꍇ�͉������Ȃ�
					return;
				}
			}
		}
	
	TVisualFormView::OnLButtonUp(nFlags, point);
}

// �J�[�\���̕ύX
BOOL CRsfList::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	/// ���X�g�A�C�R����ł̓J�[�\����ύX����
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);
	if (ChkPlayIcon(point)) {
		HCURSOR cursor = AfxGetApp()->LoadCursor(IDC_LINK);
		SetCursor(cursor);
		return TRUE;
	}
	
	return TVisualFormView::OnSetCursor(pWnd, nHitTest, message);
}


// �����t�@�C���Đ�
void CRsfList::OnLButtonDblClk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	Play();

	*pResult = 0;
}

// �����t�@�C���Đ�
void CRsfList::OnLButtonDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CPoint pt = pNMLV->ptAction;

	if (ChkPlayIcon(pt)) {
		Play();
	}

	*pResult = 0;
}

// �R���e�L�X�g���j���[�̕\��
void CRsfList::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	/// �X�N���[�����W���擾����邽�߁A�N���C�A���g���W�ɕϊ�
	RECT rct;
	GetWindowRect(&rct);
	point.x -= rct.left;
	point.y -= rct.top;
	/// �R���e�L�X�g���j���[�̕\��
	ContextMenu(point);
}

// �����ݒ�V�K�쐬
void CRsfList::OnSetupNew()
{
	CRsfNewDialog dlg;
	dlg.DoModal();

	/// �t�H���_�̈ړ����Ȃ����߁A�����I�ɍĕ`��
	UpdateForm();
}

// �����ݒ著�M�ҕύX
void CRsfList::OnSetupRename()
{
	CRsfRenameDialog dlg;
	dlg.DoModal();
}

// �����ݒ���R�s�[
void CRsfList::OnSetupCopy()
{
	/// �I�����X�g�擾
	SelectedList(&m_aryClip, TRUE, TRUE);
}

// �����ݒ��\��t��
void CRsfList::OnSetupPaste()
{
	Copy();
}

// �����ݒ著�M�҂փR�[��
void CRsfList::OnSetupCall()
{
	CUIntArray aryIdx;

	/// �I�����X�g���
	long lngCnt = SelectedList(&aryIdx, TRUE, FALSE);
	if (lngCnt != 1) return;
	long lngId = aryIdx.GetAt(0);

	/// ���M�Җ��i�X�J�C�v���j�擾
	CTblRsfl tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(lngId) != 1) return;
	CString strId = tbl.strSenderId;		

	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	pFrame->m_pAccessEvents->SendCall(strId);
}

// �����ݒ�폜
void CRsfList::OnSetupDel()
{
	CString str;

	/// �m�F���b�Z�[�W
	str.LoadString(IDS_MSGBOX53);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return;

	Move(-1);
}

// ���ݔ����̃A�C�e���폜
void CRsfList::OnTrashClear()
{
	/// �m�F���b�Z�[�W
	CString str;
	str.LoadString(IDS_MSGBOX34);
	if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return;

	if (m_dwFolderType == FD_TRASH || m_dwFolderType == FD_TRASH_DEF) {
		TrashDel();
	}
}

// ���ݔ����̃A�C�e�������ɖ߂�
void CRsfList::OnTrashRecover()
{
	if (m_dwFolderType == FD_TRASH || m_dwFolderType == FD_TRASH_DEF) {
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
BOOL CRsfList::ContextMenu(
CPoint point)	// �}�E�X�ʒu
{
/*
	/// �}�E�X��̃��X�g��I��
	UINT intFlag;
	m_hContext = m_wndList.HitTest(point, &intFlag);
	if (m_hContext < 0) {
		m_hContext = -1;
	}
*/
	long lngSelCnt = m_wndList.GetSelectedCount();

	/// �����t�H���_���
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	m_dwFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
	m_dwFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;

	/// POPUP���j���[�\��
	CMenu menu;
	CMenu* pPopup;
	switch(m_dwFolderType) {
	case FD_NORMAL:
	case FD_NORMAL_DEF:
		VERIFY(menu.LoadMenu(IDR_POPUP_SETLIST));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		if (lngSelCnt != 1) {
			pPopup->EnableMenuItem(ID_POPUP_SETLIST_RENAME, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_POPUP_SETLIST_CALL, MF_BYCOMMAND | MF_GRAYED);
			if (lngSelCnt == 0) {
				pPopup->EnableMenuItem(ID_POPUP_SETLIST_COPY, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_POPUP_SETLIST_DEL, MF_BYCOMMAND | MF_GRAYED);
			}
		} else {
			//// �f�t�H���g�ݒ莞
			CUIntArray aryIdx;
			if (SelectedList(&aryIdx, TRUE, FALSE) == 0) {
				pPopup->EnableMenuItem(ID_POPUP_SETLIST_RENAME, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_POPUP_SETLIST_CALL, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_POPUP_SETLIST_DEL, MF_BYCOMMAND | MF_GRAYED);
			}
		}
		if (m_aryClip.GetSize() < 1) {
			pPopup->EnableMenuItem(ID_POPUP_SETLIST_PASTE, MF_BYCOMMAND | MF_GRAYED);
		}
		break;
	case FD_TRASH_DEF:
	case FD_TRASH:
		VERIFY(menu.LoadMenu(IDR_POPUP_TRASH));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		if (lngSelCnt == 0) {
			pPopup->EnableMenuItem(ID_POPUP_TRASH_RECOVER, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_POPUP_TRASH_CLEAR, MF_BYCOMMAND | MF_GRAYED);
		}
		break;
	default:
		return FALSE;
	}
	
	ClientToScreen(&point);
	CWnd *pWnd = AfxGetMainWnd();
	if (!pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		point.x, point.y, pWnd)) return FALSE;

	return TRUE;
}

//
//  �@�\     : ���X�g�̏����ݒ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
void CRsfList::InitListCtrl(void)
{
	CString str;

	// set column width according to window rect
//	m_wndList.Create(this, 0, NULL, LVS_REPORT | LVS_SHOWSELALWAYS);
	m_wndList.SetGridLines(FALSE); // hide grid lines
	m_wndList.SetCheckboxeStyle(RC_CHKBOX_NONE); // No checkboxes
	m_wndList.SetEditable(FALSE); // disallow sub-text edit
	//// �w�b�_�[��`(title,size,aligne) 
	// ex) _T(" , 20, 0; ���M��, 100, 0; ����, 110, 0")
	str.LoadString(IDS_RSFLIST_HEAD);
	m_wndList.SetColumnHeader(str);
	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT);// �s�I��

	// ���X�g�̃J�����T�C�Y����
	AdustColumns();

	m_wndList.SetImageList(&m_ListImage);
}

//
//  �@�\     : ���X�g�̃J�����T�C�Y����
//  
//  �@�\���� : ���M�Җ���ϓ�
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
void CRsfList::AdustColumns(void) 
{
	if (m_wndList.GetSafeHwnd() != NULL)
	{
		CListCtrl *clc;
		CRect rect;
		int intFixSize;
		int intMargin;
		int intWidth;

		clc = &m_wndList;

		// �Œ�l
		intFixSize = clc->GetColumnWidth(0) + clc->GetColumnWidth(2);
		intMargin = 80;	// �}�[�W��

		clc->GetParent()->GetClientRect(&rect);
		intWidth = rect.Width();
		if (intWidth > (intFixSize+intMargin)) {
			clc->SetColumnWidth(1, intWidth - intFixSize - 20);		// 20:�X�N���[���o�[�̕�
		}
	}
}

//
//  �@�\     : ���X�g�̐����E�X�V
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfList::UpdateForm() 
{
	/// ���X�g�̃N���A
	m_wndList.DeleteAllItems();

	/// ���X�g�r���[���E�B���h�E�Ƀt�B�b�g������
    m_wndList.ResizeToFitParent();

	if (_tcsclen(theApp.m_strUsrId) == 0 || _tcsclen(theApp.m_strActiveFolder) == 0) {
		return FALSE;
	}

	/// �J�����g�t�H���_�̏��擾
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	m_dwFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
	m_dwFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;
//	DWORD lngFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
//	long lngFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;

	/// ���X�g�̐���
	switch(m_dwFolderType) {
	case FD_NORMAL:
	case FD_NORMAL_DEF:
	case FD_TRASH:
	case FD_TRASH_DEF:
		//// �����ݒ胊�X�g�̐���
		MakeList(m_dwFolderType, m_dwFolderId);
		break;
	}
	
	/// ���X�g�̐擪�s��I��
	if (m_wndList.GetItemCount() > 0) {
		m_wndList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

	return TRUE;
}

//
//  �@�\     : �J�����g�t�H���_�ɑΉ����������ݒ胊�X�g�̐���
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfList::MakeList(
DWORD lngFolderType,	// �t�H���_�^�C�v
long lngFolderId)		// �t�H���_ID
{
	_TCHAR buf[32];
	//int i;

	CTblRsfl tbl(theApp.m_strDBPath);

	/// �f�[�^�x�[�X���烊�X�g�����E�X�V
	CString strWhere;
	strWhere.Empty();
	switch(lngFolderType) {
	case FD_NORMAL:
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T(" and ") + _T("lngFolderId") + _T("=") + buf;
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
		strWhere = strWhere + _T(" order by lngSystemNew desc");
		break;
	case FD_NORMAL_DEF:
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		strWhere = strWhere + _T(" and ") + _T("lngFolderId=0");
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
		strWhere = strWhere + _T(" order by lngDefaultFlag desc, lngSystemNew desc");
		break;
	case FD_TRASH:
		strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T(" and ") + _T("lngFolderId") + _T("=") + buf;
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
		strWhere = strWhere + _T(" order by lngSystemNew desc");
		break;
	case FD_TRASH_DEF:
		//// �Y������t�H���_�����ݔ��ɂȂ����A�폜����Ă��Ȃ��ꍇ
		tbl.OpenTbl(_T("rsfl left outer join folder on rsfl.lngFolderId=folder.ID"));
		strWhere = strWhere + _T("rsfl.strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		strWhere = strWhere + _T(" and ") + _T("(folder.ID is NULL or folder.lngSystemFlag=0)");
		strWhere = strWhere + _T(" and ") + _T("rsfl.lngSystemFlag=1");
		strWhere = strWhere + _T(" order by rsfl.lngSystemNew desc");
		break;
	default:
		return FALSE;
	}

	/// ���X�g����
	CString strPlayFile;
	CTPsetup tps(theApp.m_strDataFolder);
	CString strSender;
	_TCHAR strDate[64];
	int intIdx;
	long lngCnt = tbl.SelectRec(strWhere);
	// 01.May.08 �������i�����j
	m_wndList.SetItemCount(lngCnt);	
	m_wndList.SetRedraw( FALSE );
	m_wndList.LockWindowUpdate();
	while (lngCnt > 0)
	{
		if (tbl.lngDefaultFlag == 1) {
			strSender.LoadString(IDS_RSF_DEF);
		} else if (tbl.strSenderName.IsEmpty()) {
			strSender = tbl.strSenderId;
		} else {
			strSender = tbl.strSenderName + _T(" <") + tbl.strSenderId + _T(">");
		}
		CnvDateString((LPCTSTR)tbl.dlngSystemNew, strDate);
		intIdx = m_wndList.InsertItemEx(INT_MAX, _T(""), strSender, strDate);
		//// �����t�@�C��������΃A�C�R���\��
		if (tbl.lngDefaultFlag == 1) {
			tps.SetCallerId(_T("0"));
		} else {
			tps.SetCallerId(tbl.strSenderId);
		}
		strPlayFile = tps.RsfAudioFileName(_T("$"));
		CFileSpec fs(strPlayFile);
		if (fs.Exist()) {
			m_wndList.SetItemImage(intIdx, 0, 4);	// setup icon
		} else {
			m_wndList.SetItemImage(intIdx, 0, -1);	// no icon
		}
		//// ���R�[�hID�̕ۑ�
		m_wndList.SetItemData(intIdx, tbl.ID);
		//// �I��\��
		//for (i = 0; i < m_arySelect.GetSize(); i++) {
		//	if (m_arySelect.GetAt(i) == tbl.ID) {
		//		m_wndList.SetItemState(intIdx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		//	}
		//}
		if (!tbl.NextRec()) break;
		--lngCnt;
	}
	//m_arySelect.RemoveAll();
	// 01.May.08 �������i�����j
	m_wndList.UnlockWindowUpdate();
	m_wndList.SetRedraw( TRUE );

	return TRUE;
}

//
//  �@�\     : �I�����ꂽ�����ݒ���R�s�[
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfList::Copy() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	long i, j;
	_TCHAR buf[32];
	long lngId;
	CString str;
	CString strId, strName, strOrgId;
	CString strPath, strOrgPath;
	BOOL ret = TRUE;

	long lngCnt = m_aryClip.GetSize();
	if (lngCnt < 0) return FALSE;

	CFileSpec fs;
	CTblRsfl tbl(theApp.m_strDBPath);
	CTPsetup tps(theApp.m_strDataFolder);
	for (i = 0; i < lngCnt; i++) {
		/// �����̑��M�ҏ��
		lngId = m_aryClip.GetAt(i);
		if (tbl.SelectRec(lngId) != 1) {
			ret = FALSE;
			break;
		}
		strOrgId = tbl.strSenderId;		// ���M�Җ��i�X�J�C�v���j
		if (tbl.lngDefaultFlag == 1) strOrgId = _T("0");	// �f�t�H���g����
		strName = tbl.strSenderName;	// ���M�Җ��i�\�����j

		/// �R�s�[���M�Җ��̐���
		for (j = 1; j< 100; j++) {
			if (tbl.lngDefaultFlag == 1) {
				str.Empty();
				_ltot_s(j, buf, sizeof(buf)/sizeof(buf[0]), 10);
				str = str + _T("default_") + buf;
			} else {
				_ltot_s(j, buf, sizeof(buf)/sizeof(buf[0]), 10);
				str = strOrgId + _T("_") + buf;
			}
			//// �d���`�F�b�N
			if (ChkSetup(str)) break;
		}
		if (j == 100) {
			ret = FALSE;
			break;
		}
		strId = str;
		if (!strName.IsEmpty()) {
			_ltot_s(j, buf, sizeof(buf)/sizeof(buf[0]), 10);
			strName = strName + _T("_") + buf;
		}

		/// �����ݒ�쐬�i����/�`���b�g/�r�f�I/�摜�t�@�C���A�ݒ�t�@�C���A�e�[�u���j
		//// ���������t�@�C���̃R�s�[
		tps.SetCallerId(strOrgId);
		strOrgPath = tps.RsfAudioFileName(_T("$"));
		tps.SetCallerId(strId);
		strPath = tps.RsfAudioFileName(_T("$"));
		fs.SetFullSpec(strOrgPath);
		if (!fs.FileCopy(strPath, TRUE)) {
			ret = FALSE;
			break;
		}
		//// �����`���b�g�t�@�C���̃R�s�[
		tps.SetCallerId(strOrgId);
		strOrgPath = tps.RsfChatFileName(_T("$"));
		tps.SetCallerId(strId);
		strPath = tps.RsfChatFileName(_T("$"));
		fs.SetFullSpec(strOrgPath);
		if (fs.Exist()) {
			if (!fs.FileCopy(strPath, TRUE)) {
				ret = FALSE;
				break;
			}
		}
		//// �����r�f�I�t�@�C���̃R�s�[
		tps.SetCallerId(strOrgId);
		strOrgPath = tps.RsfVideoFileName(_T("$"));
		tps.SetCallerId(strId);
		strPath = tps.RsfVideoFileName(_T("$"));
		fs.SetFullSpec(strOrgPath);
		if (fs.Exist()) {
			if (!fs.FileCopy(strPath, TRUE)) {
				ret = FALSE;
				break;
			}
		}
		//// �����摜�t�@�C���̃R�s�[
		tps.SetCallerId(strOrgId);
		strOrgPath = tps.FindRsfImgFile(_T("$"));
		fs.SetFullSpec(strOrgPath);
		if (fs.Exist()) {
			tps.SetCallerId(strId);
			strPath = tps.RsfImgFileName(_T("$"), strOrgPath.Right(3));
			if (!fs.FileCopy(strPath, TRUE)) {
				ret = FALSE;
				break;
			}
		}

		/// �A�g�v���O����CSV�t�@�C���̃R�s�[
		CString strPrgId, strBinDir, strFile;
		for(int i = 0; i < 999; i++) {
			strPrgId.Format(_T("%d"), i);
			tps.SetCallerId(strOrgId);
			strOrgPath = tps.RsfPrgFileName(_T("$$$$"), strPrgId);
			fs.SetFullSpec(strOrgPath);
			if (fs.Exist()) {
				tps.SetCallerId(strId);
				strPath = tps.RsfPrgFileName(_T("$$$$"), strPrgId);
				if (!fs.FileCopy(strPath, TRUE)) {
					ret = FALSE;
					break;
				}
			}
		}

		/// �����ݒ�t�@�C���̃R�s�[
		tps.SetCallerId(strOrgId);
		UST_RSF *pst = tps.LoadRSF();
		if (pst == NULL) {
			ret = FALSE;
			break;
		}
		pst->telno = strId;
		tps.SetCallerId(strId);
		if (!tps.SaveRsfFile(pst)) {
			ret = FALSE;
			break;
		}

		/// rsfl�e�[�u���ɒǉ�
		tbl.strSenderId = strId;		// ���M�Җ��i�X�J�C�v���j
		tbl.strSenderName = strName;	// ���M�Җ��i�\�����j
		tbl.strReceiverId = theApp.m_strUsrId;	// ��M�Җ��i�X�J�C�v���j
		tbl.strReceiverName = theApp.m_strUsrName;	// ��M�Җ��i�\�����j
		tbl.lngRespType = RESPBASE_TYPE_SIMPLE;
		tbl.lngFolderId = m_dwFolderId;
		tbl.lngDefaultFlag = 0;
		tbl.InsertRec();
	}

	if (!ret) {
		str.LoadString(IDS_MSGBOX54);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return ret;
	}

	/// ���b�Z�[�W���X�g�̍X�V
	UpdateForm();
	//// �f�t�H���g�t�H���_�̏ꍇ�A�i�K��̐ݒ�j�̎��̍s��I��
	if(m_dwFolderType == FD_NORMAL_DEF) {
		if (m_wndList.GetItemCount() > 1) {
			m_wndList.SetAllItemStates(RC_ITEM_SELECTED, RC_ITEM_UNSELECTED);
			m_wndList.SetItemStates(1, RC_ITEM_FOCUSED | RC_ITEM_SELECTED);
		}
	}

	return ret;
}

//
//  �@�\     : �����ݒ�̈ړ�
//  
//  �@�\���� : ���ݔ��Ή�
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfList::Move(
long lngFolderId)		// -1:���ݔ��ֈړ�
{
	CString str;
	UINT intCnt, i = 0;
	int idx;
	CUIntArray aryIdx;

	/// �I�����X�g���
	intCnt = SelectedList(&aryIdx, FALSE, FALSE);
	if (intCnt < 1) return FALSE;

	/// �e�[�u�����X�V
	long lngId;
	_TCHAR buf[32];
	CTblRsfl tbl(theApp.m_strDBPath);
	CString strWhere;
	CString strSet;
	//// �g�����U�N�V�����J�n
	tbl.m_blTransaction = TRUE;
	if (!tbl.TransactionStart()) return FALSE;
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		strWhere.Empty();
		_ltot_s(lngId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T("ID") + _T("=") + buf;
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=0");
		strSet.Empty();
		if (lngFolderId < 0) {
			///// �폜�t���O�t��
			strSet = strSet + _T("lngSystemFlag=1");
		} else {
			///// �t�H���_�ύX
			_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
			strSet = strSet + _T("lngFolderId=") + buf;
		}
		tbl.UpdateRec(strSet, strWhere);	
	}
	//// �g�����U�N�V�����I��
	tbl.TransactionEnd();

	/// ���X�g����폜�i���X�g�̌�납��j
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(intCnt-i-1);
		m_wndList.DeleteItem(idx);
	}
	//// ���̍��ڂ��đI��
	if (m_wndList.GetItemCount() < 1) return TRUE;
	if (idx > 0) idx--;
	m_wndList.SetItemState(idx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

	return TRUE;
}

//
//  �@�\     : ���ݔ�(����)���̃��X�g�폜
//  
//  �@�\���� : ���S�폜
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : �g�����U�N�V�������K�v�Ȃ̂ŁA�P�֐��Ŏ���
//  
BOOL CRsfList::TrashDel()
{
	CString str;
	UINT intCnt, i = 0;
	int idx;
	CUIntArray aryIdx;

	/// �I�����X�g���
	intCnt = m_wndList.GetSelectedCount();
	if (intCnt < 1) return FALSE;
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	if (pos == NULL) return FALSE;
	for (i = 0; i < intCnt; i++) {
		idx = m_wndList.GetNextSelectedItem(pos);
		aryIdx.Add(idx);
		if (pos == NULL) break;
	}
	intCnt = aryIdx.GetSize();
	if (intCnt < 1) return FALSE;

	long lngId;
	_TCHAR buf[32];
	/// �ݒ�e�[�u�����X�V
	CTblRsfl tbl(theApp.m_strDBPath);
	//// �ݒ�E�����t�@�C���̍폜
	CFileSpec fs;
	CTPsetup tps(theApp.m_strDataFolder);
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		if (tbl.SelectRec(lngId) <= 0) continue;
		tps.SetCallerId(tbl.strSenderId);
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
	}
	//// �g�����U�N�V�����J�n
	CString strWhere;
	tbl.m_blTransaction = TRUE;
	if (!tbl.TransactionStart()) return FALSE;
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		strWhere.Empty();
		_ltot_s(lngId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T("ID") + _T("=") + buf;
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
		tbl.DeleteRec(strWhere);	// �폜
	}
	//// �g�����U�N�V�����I��
	tbl.TransactionEnd();

	/// ���X�g����폜�i���X�g�̌�납��j
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(intCnt-i-1);
		m_wndList.DeleteItem(idx);
	}
	//// ���̍��ڂ��đI��
	if (m_wndList.GetItemCount() < 1) return TRUE;
	if (idx > 0) idx--;
	m_wndList.SetItemState(idx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

	return TRUE;
}

//
//  �@�\     : ���ݔ��i�����j���̃��X�g�����ɖ߂�
//  
//  �@�\���� : �폜�t���O������
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfList::TrashRecover()
{
	CString str;
	UINT intCnt, i = 0;
	_TCHAR buf[32];
	int idx, lngId, lngFolderId;
	CUIntArray aryIdx;
	CUIntArray aryFolderId;
	CString strWhere;
	CString strSet;

	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();

	/// �ݒ�e�[�u�����X�V
	//// �I�����X�g���
	intCnt = SelectedList(&aryIdx, FALSE, TRUE);
	if (intCnt < 1) return FALSE;
	//// ���J�o����t�H���_�̑��݃`�F�b�N
	CTblRsfl tbl(theApp.m_strDBPath);
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		tbl.SelectRec(lngId);
		if (pfview->FindItemId(pfview->m_hFolderDef, tbl.lngFolderId)) {
			aryFolderId.Add(tbl.lngFolderId);
		} else {
			aryFolderId.Add(0);
		}
	}
	//// �g�����U�N�V�����J�n
	tbl.m_blTransaction = TRUE;
	if (!tbl.TransactionStart()) return FALSE;
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		lngFolderId = aryFolderId.GetAt(i);
		strWhere.Empty();
		_ltot_s(lngId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T("ID") + _T("=") + buf;
		strWhere = strWhere + _T(" and ") + _T("lngSystemFlag=1");
		strSet.Empty();
		_ltot_s(lngFolderId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strSet = strSet + _T("lngSystemFlag=0,lngFolderId=") + buf;
		tbl.UpdateRec(strSet , strWhere);
	}
	//// �g�����U�N�V�����I��
	tbl.TransactionEnd();

	/// ���X�g����폜�i���X�g�̌�납��j
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(intCnt-i-1);
		m_wndList.DeleteItem(idx);
	}
	//// ���̍��ڂ��đI��
	if (m_wndList.GetItemCount() < 1) return TRUE;
	if (idx > 0) idx--;
	m_wndList.SetItemState(idx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

	return TRUE;
}

//
//  �@�\     : �Ή����鉞���ݒ�̕\��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfList::SelSetup(
CString strSender)	// ���M�Җ�
{
	CString str;
	HTREEITEM h;

	//m_arySelect.RemoveAll();

	/// �����ݒ�̌��o
	CTblRsfl tbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
	strWhere = strWhere + _T(" and ") + _T("strSenderId") + _T("='") + strSender + _T("'");
	long lngCnt = tbl.SelectRec(strWhere);
	if (lngCnt == 0) return FALSE;

	long lngId = tbl.ID;
	long lngFolderId = tbl.lngFolderId;
	long lngSystemFlag = tbl.lngSystemFlag;

	/// �����ݒ�t�H���_�̑I��\��
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	//// �t�H���_����
	if (lngFolderId == 0) {
		if (lngSystemFlag == 0) {
			h = pfview->m_hFolderDef;
		} else {
			h = pfview->m_hTrashDef;
		}
	} else {
		if (lngSystemFlag == 0) {
			h = pfview->FindItemId(pfview->m_hFolderDef, lngFolderId);
		} else {
			h = pfview->FindItemId(pfview->m_hTrashDef, lngFolderId);
		}
	}
	//// ���X�g�I������ID�̎w��
	//m_arySelect.Add(lngId);
	//// �t�H���_�I���E���X�g�X�V
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	tc.SelectItem(h);
	tc.EnsureVisible(h);

	/// �����ݒ�̑I��\��
	long idx = FindIdx(lngId);
	if (idx < 0) {
		//// �Y���A�C�e�����Ȃ��ꍇ�A�Đ������ă��g���C
		UpdateForm();
		idx = FindIdx(lngId);
	}
	if (idx >= 0) {
		m_wndList.SetAllItemStates(RC_ITEM_SELECTED, RC_ITEM_UNSELECTED);
		m_wndList.SetItemStates(idx, RC_ITEM_FOCUSED | RC_ITEM_SELECTED);
//		m_wndList.SetItemState(idx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	}

	return TRUE;
}


//
//  �@�\     : �����ݒ著�M�Җ��`�F�b�N
//  
//  �@�\���� : ���ݔ������Ώ�
//  
//  �Ԃ�l   : TRUE:�d���Ȃ��AFALSE�F�d������
//  
//  ���l     : 
//  
BOOL CRsfList::ChkSetup(
CString strSender)	// ���M�Җ�
{
	CString str;

	if (strSender.IsEmpty()) {
		str.LoadString(IDS_MSGBOX42);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	/// �������M�Җ��Ƃ̏d���`�F�b�N
	CTblRsfl tbl(theApp.m_strDBPath);
	CString strWhere;
	strWhere.Empty();
	strWhere = strWhere + _T("strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
	strWhere = strWhere + _T(" and ") + _T("strSenderId") + _T("='") + strSender + _T("'");
	long lngCnt = tbl.CountRec(strWhere);
	if (lngCnt > 0) {
		return FALSE;
	}

	return TRUE;
}


//
//  �@�\     : �����ݒ�I�����X�g�̎擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : �擾���X�g����
//  
//  ���l     : 
//  
long CRsfList::SelectedList(
CUIntArray *pAryIdx,	// ���X�g�ۑ��z��
BOOL blData,			// TRUE:���X�g�C���f�b�N�X�̑���ɃA�C�e���f�[�^��ۑ�
BOOL blDefault)			// TRUE:�f�t�H���g�ݒ���܂�
{
	CString str;
	UINT intCnt, i = 0;
	int idx, intId;

	pAryIdx->RemoveAll();

	/// �I�����X�g���
	intCnt = m_wndList.GetSelectedCount();
	if (intCnt < 1) return 0;

	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	if (pos == NULL) return 0;

	for (i = 0; i < intCnt; i++) {
		idx = m_wndList.GetNextSelectedItem(pos);
		intId = m_wndList.GetItemData(idx);
		if (!blDefault && (intId == 1)) {
			//// �f�t�H���g�ݒ�͏��O
		} else {
			if (blData) {
				pAryIdx->Add(intId);
			} else {
				pAryIdx->Add(idx);
			}
		}
		if (pos == NULL) break;
	}

	return pAryIdx->GetSize();
}

//
//  �@�\     : �����ݒ胊�X�g�C���f�b�N�X�̌���
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : �C���f�b�N�X�A-1:not found
//  
//  ���l     : 
//  
long CRsfList::FindIdx(
long lngId)			// �����ݒ�e�[�u��ID
{
	CString str;
	UINT intCnt, i = 0;
	int idx, intId;

	/// �I�����X�g���
	intCnt = m_wndList.GetSelectedCount();
	if (intCnt < 1) return -1;

	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	if (pos == NULL) return -1;

	for (i = 0; i < intCnt; i++) {
		idx = m_wndList.GetNextSelectedItem(pos);
		intId = m_wndList.GetItemData(idx);
		if (lngId == (long)intId) {
			return idx;
		}
		if (pos == NULL) break;
	}

	return -1;
}

//
//  �@�\     : �N���C�A���g���W�ϊ��iRsfList->RsfTree�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : RsfTree�N���C�A���g���W
//  
//  ���l     : 
//  
CPoint CRsfList::CnvPntM_F(
const CPoint point)	// RsfList�N���C�A���g���W
{
	CPoint pnt = point;
	ClientToScreen(&pnt);
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	pfview->ScreenToClient(&pnt);

	return pnt;
}

//
//  �@�\     : �I���s�̍Đ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CRsfList::Play() 
{
	/// �J�����g�t�H���_�̏��擾
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfTree *pfview = (CRsfTree *)pFrame->m_pRsfTree->GetWnd();
	if (pfview->IsFolderWT(pfview->m_dwFolderType)) {
//		CRsfSimpleForm *pform = (CRsfSimpleForm *)theApp.m_pRsfSimpleForm->GetWnd();
//		pform->Play();
	}

	return TRUE;
}

//
//  �@�\     : �w����W�������Đ��A�C�R����ɂ��邩
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CRsfList::ChkPlayIcon(
const CPoint point)	// �N���C�A���g���W
{
	LVHITTESTINFO hitTestInfo;
	hitTestInfo.pt = point;
	hitTestInfo.flags = LVHT_ONITEM;
	if (m_wndList.SubItemHitTest(&hitTestInfo) >= 0) {
		if (hitTestInfo.iItem >= 0 && hitTestInfo.iSubItem == 0) {
			if (m_wndList.GetItemImage(hitTestInfo.iItem, 0) >= 0) {
				return TRUE;
			}
		}
	}
	
	return FALSE;
}


