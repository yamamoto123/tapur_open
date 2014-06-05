// MsgList.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "TPutil.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "FileSpec.h"
#include "MainFrm.h"
#include "MsgList.h"
#include "RsfList.h"
#include "MsgTree.h"
#include "MsgForm.h"
#include "RsfNewDialog.h"
#include "SkypeEvents.h"
#include "Tokenizer.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CMsgList

IMPLEMENT_DYNCREATE(CMsgList, TVisualFormView)

CMsgList::CMsgList()
	: TVisualFormView(CMsgList::IDD)
{
	//{{AFX_DATA_INIT(CMsgList)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�̏�����������ǉ����܂�
	//}}AFX_DATA_INIT

	m_bDrag = FALSE;
	m_hItemDrag = 0;
}

CMsgList::~CMsgList()
{
}

void CMsgList::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgList)
		// ����: ClassWizard �͂��̈ʒu�� DDX ����� DDV �̌Ăяo����ǉ����܂�
	DDX_Control(pDX, IDC_LIST, m_wndList);	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMsgList, TVisualFormView)
	//{{AFX_MSG_MAP(CMsgList)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchanged)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST, OnBegindrag)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_MSGLIST_CALL, OnMsgCall)
	ON_COMMAND(ID_POPUP_REBTEL, OnLinkRebtel)
	ON_COMMAND(ID_POPUP_MSGLIST_SETUP, OnMsgSetup)
	ON_COMMAND(ID_POPUP_MSGLIST_DEL, OnMsgDel)
	ON_COMMAND(ID_POPUP_PRG1, OnPrg1)
	ON_COMMAND(ID_POPUP_PRG11, OnPrg11)
	ON_COMMAND(ID_POPUP_PRG901, OnPrg901)
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
// CMsgList �f�f

#ifdef _DEBUG
void CMsgList::AssertValid() const
{
	CFormView::AssertValid();
}

void CMsgList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMsgList ���b�Z�[�W �n���h��

int CMsgList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (TVisualFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	
	m_aryClip.RemoveAll();

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
	m_clrUnview = RGB(210, 255, 210);	// light green

	return 0;
}

void CMsgList::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit();
	
//	srand((unsigned)time(NULL));

	InitListCtrl();
}

void CMsgList::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	// ���X�g�r���[���E�B���h�E�Ƀt�B�b�g������
    m_wndList.ResizeToFitParent();
	
	// ���X�g�̃J�����T�C�Y����
	AdustColumns();
}

// ���X�g�I�����C�x���g
void CMsgList::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int iItem = -1;
	int iSubItem = -1;
	UINT state;
	UINT intCnt = 0;
	CMsgForm *pmv;
	*pResult = 0;
	long lngId;

	if (!pNMLV) goto PROC_END;
	iItem = pNMLV->iItem;
	iSubItem = pNMLV->iSubItem;
	if (iItem < 0 || iSubItem < 0) goto PROC_END;
	intCnt = m_wndList.GetSelectedCount();
	//if (intCnt <= 0) return;
	if (intCnt != 1) goto PROC_END;	// �����I���̏ꍇ���b�Z�[�W�t�H�[���͍X�V���Ȃ��i��ʑI���̏ꍇ�A���Ԃ�������̂Łj

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
		strHead = m_wndList.GetItemText(iItem, 3);	// ����
		//strHead = m_wndList.GetItemText(iItem, 1);	// ���M�Җ�
		//strHead = m_wndList.GetItemText(iItem, 2);	// ����
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
		switch (pfview->m_dwFolderType) {
		case FD_NORMAL:
		case FD_NORMAL_DEF:
		case FD_TRASH:
		case FD_TRASH_DEF:
			theApp.m_lngRsflId = -1;
			//// �J�����g��M���R�[�hID�̕ێ�
			lngId = m_wndList.GetItemData(iItem);
			//if (lngId >= 0 && theApp.m_lngRdflId >= 0 && (lngId == theApp.m_lngRdflId)) {
			//	///// �����f�[�^��I�������ꍇ�́A�ĕ\�����Ȃ�
			//	break;
			//}
			theApp.m_lngRdflId = lngId;
			//// ���b�Z�[�W�^�u�̕\��
			pFrame->m_Framework.ShowTab(pFrame->m_pMsgForm, TRUE);
			pFrame->m_Framework.SetTabLabel(pFrame->m_pMsgForm, strHead);
			pFrame->m_Framework.SetActiveTab(pFrame->m_pMsgForm);
			pFrame->m_Framework.ShowTab(pFrame->m_pMsgFolderForm, FALSE);
			//// ���b�Z�[�W�t�H�[�����X�V
			pmv = (CMsgForm *)pFrame->m_pMsgForm->GetWnd();
			pmv->UpdateForm(FALSE);	// �t�H�[�J�X���ړ����Ȃ�
			break;
		default:
			goto PROC_END;
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

PROC_END:
	*pResult = 0;
}

// Tab�L�[���������ꍇ�AOnKeyDown�֐��Ŏ擾���邱�Ƃ��ł��Ȃ����߁A�����Ŏ擾����B
BOOL CMsgList::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		/// TAB KEY : ���X�g�t�H�[���փt�H�[�J�X�ړ��i���X�g����̏ꍇ�̓t�H�[�J�X�ړ����Ȃ��j
		/// RETRUN KEY : ���j���[�\��
		if (nVirtKey == VK_TAB) {
			CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
			CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
			CMsgForm *pmv;
			if (pfview->IsFolderWT(pfview->m_dwFolderType)) {
				pmv = (CMsgForm *)pFrame->m_pMsgForm->GetWnd();
				pmv->SetFocus();
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
					OnMsgDel();
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
void CMsgList::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	/// �I�����ڂ��Ȃ����͖���
	if (m_wndList.GetSelectedCount() == 0) return;

	/// �����t�H���_���
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
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
		m_DragImage.BeginDrag(0, CPoint(0, 0));
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
void CMsgList::OnMouseMove(UINT nFlags, CPoint point) 
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

		/// �t�H���_�̏ォ�`�F�b�N
		CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
		CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
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
void CMsgList::OnLButtonUp(UINT nFlags, CPoint point) 
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
			CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
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
						/// ���b�Z�[�W�����ݔ���
						Move(-1);
					} else if (type == FD_TRASH) {
						return;
					} else {
						/// ���b�Z�[�W���ړ�
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
BOOL CMsgList::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
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
void CMsgList::OnLButtonDblClk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	Play();

	*pResult = 0;
}

// �����t�@�C���Đ�
void CMsgList::OnLButtonDown(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CMsgList::OnContextMenu(CWnd* pWnd, CPoint point) 
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

// ���M�҂փR�[��
void CMsgList::OnMsgCall()
{
	CUIntArray aryIdx;

	/// �I�����X�g���
	long lngCnt = SelectedList(&aryIdx, TRUE);
	if (lngCnt != 1) return;
	long lngId = aryIdx.GetAt(0);

	/// ���M�Җ��i�X�J�C�v���j�擾
	CTblRdfl tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(lngId) != 1) return;
	CString strId = tbl.strSenderId;		
	if (strId == _T("0")) {
		//// �s���Ȕ��M��(SkypeIn)
		CString str;
		str.LoadString(IDS_MSGBOX17);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return;
		
	}

	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	pFrame->m_pAccessEvents->SendCall(strId);
}

// �L���iRebtel�j
void CMsgList::OnLinkRebtel()
{
	CString str;
	str.LoadString(IDS_URL_REBTEL);
	theApp.OpenUrl(str);
}

// �Ή����鉞���ݒ�̕\��
void CMsgList::OnMsgSetup()
{
	CString str;
	CUIntArray aryIdx;

	/// �I�����X�g���
	long lngCnt = SelectedList(&aryIdx, TRUE);
	if (lngCnt != 1) return;
	long lngId = aryIdx.GetAt(0);

	/// ���M�Җ��i�X�J�C�v���j�擾
	CTblRdfl tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(lngId) != 1) return;
	//// SelSetup()�ōđI������邽�߁A�����Œl���m��
	CString strId = tbl.strSenderId;		
	CString strName = tbl.strSenderName;		

	/// �����ݒ�̕\��
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CRsfList *pfview = (CRsfList *)pFrame->m_pRsfList->GetWnd();
	if (!pfview->SelSetup(strId)) {
		//// �Y�����Ȃ����ߐV�K�����ݒ�̍쐬
		str.LoadString(IDS_MSGBOX52);
		if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION ) != IDYES) return;

		CRsfNewDialog dlg;
		dlg.m_strId = strId;
//		dlg.m_pcmb->SetWindowText(strId);
		dlg.m_strName = strName;
		dlg.DoModal();
	}
	//// �����ݒ�TAB�̕\��
	pFrame->m_pTabSetup->SetActiveTab();
}

// ��M�f�[�^�폜
void CMsgList::OnMsgDel()
{
	Move(-1);
}

// ���ݔ����̃A�C�e���폜
void CMsgList::OnTrashClear()
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
void CMsgList::OnTrashRecover()
{
	if (m_dwFolderType == FD_TRASH || m_dwFolderType == FD_TRASH_DEF) {
		TrashRecover();
	}
}

// �A�g�v���O�����̎��s
void CMsgList::OnPrg1()
{
	ExecPrg(1);
}
void CMsgList::OnPrg11()
{
	ExecPrg(11);
}
void CMsgList::OnPrg901()
{
	ExecPrg(901);
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
BOOL CMsgList::ContextMenu(
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
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	m_dwFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
	m_dwFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;

	/// POPUP���j���[�\��
	CMenu menu;
	CMenu* pPopup;
	if (pfview->IsFolder(m_dwFolderType)) {
		VERIFY(menu.LoadMenu(IDR_POPUP_MSGLIST));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		if (lngSelCnt != 1) {
			pPopup->EnableMenuItem(ID_POPUP_MSGLIST_CALL, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_POPUP_MSGLIST_SETUP, MF_BYCOMMAND | MF_GRAYED);
			if (lngSelCnt == 0) {
				pPopup->EnableMenuItem(ID_POPUP_MSGLIST_DEL, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_POPUP_PRG1, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_POPUP_PRG11, MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_POPUP_PRG901, MF_BYCOMMAND | MF_GRAYED);
			}
		}
	} else if (pfview->IsTrash(m_dwFolderType)) {
		VERIFY(menu.LoadMenu(IDR_POPUP_TRASH));
		pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		if (lngSelCnt == 0) {
			pPopup->EnableMenuItem(ID_POPUP_TRASH_RECOVER, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_POPUP_TRASH_CLEAR, MF_BYCOMMAND | MF_GRAYED);
		}
	} else {
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
void CMsgList::InitListCtrl(void)
{
	CString str;

	// set column width according to window rect
//	m_wndList.Create(this, 0, NULL, LVS_REPORT | LVS_SHOWSELALWAYS);
	m_wndList.SetGridLines(FALSE);					// hide grid lines
	m_wndList.SetCheckboxeStyle(RC_CHKBOX_NONE);	// No checkboxes
	m_wndList.SetEditable(FALSE);					// disallow sub-text edit
	//// �w�b�_�[��`(title,size,aligne) 
	// ex) _T(" , 20, 0; ���M��, 100, 0; ����, 110, 0")
	str.LoadString(IDS_MSGLIST_HEAD);
	m_wndList.SetColumnHeader(str);
	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT);// �s�I��

	// ���X�g�̃J�����T�C�Y����
	AdustColumns();

//	m_wndList.SetHeaderImageList(IDB_BITMAP2);
//	m_wndList.SetHeaderImage(0, 0);
	
	m_wndList.SetImageList(&m_ListImage);
/*
	CRect rect;
	list.GetWindowRect(&rect);

	int w = rect.Width() - 2;
	int colwidths[6] = { 20, 150, 150 };

	TCHAR *	lpszHeaders[] = { _T(""),
							  _T("���M��"),
							  _T("����"),
							  NULL };
	int i;
	int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// add columns
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = (i == 1 || i == 5) ? LVCFMT_LEFT : LVCFMT_CENTER;
		lvcolumn.pszText = lpszHeaders[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = (lpszHeaders[i+1] == NULL) ? w - total_cx - 2 : (w * colwidths[i]) / 64;
		total_cx += lvcolumn.cx;
		list.InsertColumn(i, &lvcolumn);
	}


	// create the image list from bitmap resource
	VERIFY(list.m_cImageList.Create(IDB_BITMAP1, 16, 3, RGB(255, 0, 255)));
	list.m_HeaderCtrl.SetImageList(&list.m_cImageList);

	// iterate through header items and attach the image list
	HDITEM hditem;

	for (i = 0; i < list.m_HeaderCtrl.GetItemCount(); i++)
	{
		hditem.mask = HDI_IMAGE | HDI_FORMAT;
		list.m_HeaderCtrl.GetItem(i, &hditem);
		hditem.fmt |=  HDF_IMAGE;
		if (i == 0 || i == 4)
			hditem.iImage = XHEADERCTRL_UNCHECKED_IMAGE;
		else
			hditem.iImage = XHEADERCTRL_NO_IMAGE;

		list.m_HeaderCtrl.SetItem(i, &hditem);
	}

	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// set the format again - must do this twice or first column does not get set
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
		lvcolumn.fmt = (i == 1 || i == 5) ? LVCFMT_LEFT : LVCFMT_CENTER;
		lvcolumn.iSubItem = i;
		list.SetColumn(i, &lvcolumn);
	}
*/
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
void CMsgList::AdustColumns(void) 
{
	if (m_wndList.GetSafeHwnd() != NULL)
	{
		CListCtrl *clc;
		CRect rect;
		int intFixSize;
		int intMargin;
		int intWidth;

		clc = &m_wndList;

		/// �Œ�l
		intFixSize = clc->GetColumnWidth(0) + clc->GetColumnWidth(2);	// �A�C�R���Ɠ����̕�
		intMargin = 80;	// �}�[�W��

		/// �ϓ����ڂ̕��ݒ�
		clc->GetParent()->GetClientRect(&rect);
		intWidth = rect.Width();
		if (intWidth > (intFixSize+intMargin)) {	
			intWidth = (intWidth - intFixSize - 20) / 2;	// 20:�X�N���[���o�[�̕�
			clc->SetColumnWidth(1, intWidth);				// ���M�Җ�
			clc->SetColumnWidth(3, intWidth);				// �^�C�g��
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
BOOL CMsgList::UpdateForm() 
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
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
	CTreeCtrl &tc = pfview->GetTreeCtrl();
	m_dwFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
	m_dwFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;
//	DWORD lngFolderType = tc.GetItemData(pfview->m_hActive) & FD_MASK_TYPE;
//	long lngFolderId = tc.GetItemData(pfview->m_hActive) & FD_MASK_ID;

	/// ���X�g�̐���
	if (pfview->IsFolderWT(m_dwFolderType)) {
		//// ���b�Z�[�W���X�g�̐���
		BeginWaitCursor();
		MakeList(m_dwFolderType, m_dwFolderId);
		EndWaitCursor();
	}
	
	/// ���X�g�̐擪�s��I��
	if (m_wndList.GetItemCount() > 0) {
		m_wndList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

	return TRUE;
}

//
//  �@�\     : �J�����g�t�H���_�ɑΉ��������b�Z�[�W���X�g�̐���
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMsgList::MakeList(
DWORD lngFolderType,	// �t�H���_�^�C�v
long lngFolderId)		// �t�H���_ID
{
	_TCHAR buf[32];

	CTblRdfl tbl(theApp.m_strDBPath);

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
		strWhere = strWhere + _T(" order by lngSystemNew desc");
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
		tbl.OpenTbl(_T("rdfl left outer join folder on rdfl.lngFolderId=folder.ID"));
		strWhere = strWhere + _T("rdfl.strReceiverId") + _T("='") + theApp.m_strUsrId + _T("'");
		strWhere = strWhere + _T(" and ") + _T("(folder.ID is NULL or folder.lngSystemFlag=0)");
		strWhere = strWhere + _T(" and ") + _T("rdfl.lngSystemFlag=1");
		strWhere = strWhere + _T(" order by rdfl.lngSystemNew desc");
		break;
	default:
		return FALSE;
	}

	CString strDef;
	strDef.LoadString(IDS_MSG_DEF);
	CString strSender;
	CString strTitle;
	CString strPlayFile;
	_TCHAR strDate[64];
	int intIdx;
	CTPsetup tps(theApp.m_strDataFolder);
	long lngCnt = tbl.SelectRec(strWhere);
	// 01.May.08 �������i�t�@�C�����݃`�F�b�N�̕����d���̂Ō��ʂ͏��Ȃ��j
	m_wndList.SetItemCount(lngCnt);	
	m_wndList.SetRedraw( FALSE );
	m_wndList.LockWindowUpdate();
	while (lngCnt > 0)
	{
		//// [���M�Җ�]�擾
		if (tbl.strSenderId == _T("0")) {
			//// �s���Ȕ��M��(SkypeIn)
			strSender = strDef;
		} else {
			if (tbl.strSenderName.IsEmpty()) {
				strSender = tbl.strSenderId;
			} else {
				strSender = tbl.strSenderName + _T(" <") + tbl.strSenderId + _T(">");
			}
		}
		//// [����]�擾
		CnvDateString((LPCTSTR)tbl.dlngSystemNew, strDate);
		//// [����]�擾
		strTitle = tbl.strTitle;
		//// ���X�g�Ƀf�[�^�ݒ�
		intIdx = m_wndList.InsertItemEx(INT_MAX, _T(""), strSender, strDate, strTitle);
		//// �r�f�I�t�@�C��������΃r�f�I�A�C�R���\��
		CFileSpec fs;
		tps.SetCallerId(tbl.strSenderId);
		strPlayFile = tps.FindRdfVideoFileName(tbl.strRemarks1,tbl.dlngSystemNew, _T("$$"));
		if (!strPlayFile.IsEmpty()) {
			if (tbl.lngRespType == 1) {
			///// �V���v������
				m_wndList.SetItemImage(intIdx, 0, 3);	// video�A�C�R��
			} else if (tbl.lngRespType == 2) {
			///// ���G�ȉ���
			} else {
			///// ���ژ^��
				if (tbl.lngClsCode1 == 1 || tbl.lngClsCode1 == 3) {
					m_wndList.SetItemImage(intIdx, 0, 7);	// incoming video�A�C�R��
				} else if (tbl.lngClsCode1 == 2 || tbl.lngClsCode1 == 4) {
					m_wndList.SetItemImage(intIdx, 0, 8);	// outgoing video�A�C�R��
				} else {
					m_wndList.SetItemImage(intIdx, 0, 1);	// video�A�C�R��
				}
			}
		} else {
			//// �����t�@�C��������Ή����A�C�R���\��
			strPlayFile = tps.FindRdfWavFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
			if (strPlayFile.IsEmpty()) {
				strPlayFile = tps.FindRdfMp3FileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
			}
			if (!strPlayFile.IsEmpty()) {
				if (tbl.lngRespType == 1) {
				///// �V���v������
					m_wndList.SetItemImage(intIdx, 0, 2);	// sound�A�C�R��
				} else if (tbl.lngRespType == 2) {
				///// ���G�ȉ���
				} else {
				///// ���ژ^��
					if (tbl.lngClsCode1 == 1 || tbl.lngClsCode1 == 3) {
						m_wndList.SetItemImage(intIdx, 0, 5);	// incoming sound�A�C�R��
					} else if (tbl.lngClsCode1 == 2 || tbl.lngClsCode1 == 4) {
						m_wndList.SetItemImage(intIdx, 0, 6);	// outgoing sound�A�C�R��
					} else {
						m_wndList.SetItemImage(intIdx, 0, 0);	// sound�A�C�R��
					}
				}
			} else {
				m_wndList.SetItemImage(intIdx, 0, -1);	// no icon
			}
		}
		//// ���Đ����R�[�h�̂݋����\��
		if (!tbl.lngViewFlag) {
			m_wndList.SetItemBkColor(intIdx, -1, m_clrUnview, FALSE);
			m_wndList.SetItemTextBold(intIdx, -1, TRUE, FALSE);
		}
		//// ���R�[�hID�̕ۑ�
		m_wndList.SetItemData(intIdx, tbl.ID);
		if (!tbl.NextRec()) break;
		--lngCnt;
	}
	// 01.May.08 ������
	m_wndList.UnlockWindowUpdate();
	m_wndList.SetRedraw( TRUE );

	return TRUE;
}

//
//  �@�\     : ���b�Z�[�W�̈ړ�
//  
//  �@�\���� : ���ݔ��Ή�
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMsgList::Move(
long lngFolderId)		// -1:���ݔ��ֈړ�
{
	CString str;
	UINT intCnt, i = 0;
	int idx;
	CUIntArray aryIdx;

	/// �I�����X�g���
	intCnt = SelectedList(&aryIdx, FALSE);
	if (intCnt < 1) return FALSE;

	/// �e�[�u�����X�V
	long lngId;
	_TCHAR buf[32];
	CTblRdfl tbl(theApp.m_strDBPath);
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
//  �@�\     : ���ݔ�(��M)���̃��X�g�폜
//  
//  �@�\���� : ���S�폜
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : �g�����U�N�V�������K�v�Ȃ̂ŁA�P�֐��Ŏ���
//  
BOOL CMsgList::TrashDel()
{
	_TCHAR buf[32];
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

	/// ���b�Z�[�W�e�[�u�����X�V
	long lngId;
	CTblRdfl tbl(theApp.m_strDBPath);
	//// �����E�r�f�I�t�@�C���̍폜
	CFileSpec fs;
	CTPsetup tps(theApp.m_strDataFolder);
	for (i = 0; i < intCnt; i++) {
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		if (tbl.SelectRec(lngId) <= 0) continue;
		tps.SetCallerId(tbl.strSenderId);
		///// wav file
		str = tps.FindRdfWavFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
		if (!str.IsEmpty()) {
			fs.SetFullSpec(str);
			fs.FileDelete();
		}
		///// mp3 file
		str = tps.FindRdfMp3FileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
		if (!str.IsEmpty()) {
			fs.SetFullSpec(str);
			fs.FileDelete();
		}
		///// avi file
		str = tps.FindRdfVideoFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
		if (!str.IsEmpty()) {
			fs.SetFullSpec(str);
			fs.FileDelete();
		}
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
	if (!tbl.TransactionEnd()) return FALSE;

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
//  �@�\     : ���ݔ��i��M�j���̃��X�g�����ɖ߂�
//  
//  �@�\���� : �폜�t���O������
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMsgList::TrashRecover()
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
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();

	/// ���b�Z�[�W�e�[�u�����X�V
	//// �I�����X�g���
	intCnt = SelectedList(&aryIdx, FALSE);
	if (intCnt < 1) return FALSE;
	//// ���J�o����t�H���_�̑��݃`�F�b�N
	CTblRdfl tbl(theApp.m_strDBPath);
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
//  �@�\     : ���X�g�I�����X�g
//  
//  �@�\���� : �����I������Ă���ꍇ�͍ŏ���ID
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMsgList::SelectId(int idx)		// ���X�g��ID			
{
	UINT intCnt;

	/// �I���ς݃��X�g����
	//intCnt = m_wndList.GetSelectedCount();
	//if (intCnt > 1) {
	//	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	//	if (pos != NULL) {
	//		for (i = 0; i < intCnt; i++) {
	//			id = m_wndList.GetNextSelectedItem(pos);
	//			m_wndList.SetItemState(id, 0, LVIS_SELECTED);
	//			if (pos == NULL) break;
	//		}
	//	}
	//}
	m_wndList.SetAllItemStates(RC_ITEM_SELECTED, RC_ITEM_UNSELECTED);
	m_wndList.SetAllItemStates(RC_ITEM_FOCUSED, RC_ITEM_UNFOCUSED);

	intCnt = m_wndList.GetItemCount();
	if (idx < 0) {
		idx = 0;
	} else if (intCnt-1 < (UINT)idx) {
		idx = intCnt-1;
	}

	m_wndList.SetItemState(idx, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	m_wndList.SetSelectionMark(idx);

	return idx;
}

//
//  �@�\     : �I�����X�gID�̎擾
//  
//  �@�\���� : �����I������Ă���ꍇ�͍ŏ���ID
//  
//  �Ԃ�l   : ���X�g��ID�A-1:��I��
//  
//  ���l     : 
//  
long CMsgList::SelectedId()			
{
	UINT intCnt;
	int idx;

	/// �I�����X�g���
	intCnt = m_wndList.GetSelectedCount();
	if (intCnt < 1) return -1;

	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	if (pos == NULL) return -1;

	idx = m_wndList.GetNextSelectedItem(pos);

	return idx;
}

//
//  �@�\     : ���b�Z�[�W�I�����X�g�̎擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : �擾���X�g����
//  
//  ���l     : 
//  
long CMsgList::SelectedList(
CUIntArray *pAryIdx,	// ���X�g�ۑ��z��
BOOL blData)			// TRUE:���X�g�C���f�b�N�X�̑���ɃA�C�e���f�[�^��ۑ�
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
		if (blData) {
			pAryIdx->Add(intId);
		} else {
			pAryIdx->Add(idx);
		}
		if (pos == NULL) break;
	}

	return pAryIdx->GetSize();
}

//
//  �@�\     : �N���C�A���g���W�ϊ��iMsgList->MsgTree�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : MsgTree�N���C�A���g���W
//  
//  ���l     : 
//  
CPoint CMsgList::CnvPntM_F(
const CPoint point)	// MsgList�N���C�A���g���W
{
	CPoint pnt = point;
	ClientToScreen(&pnt);
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
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
BOOL CMsgList::Play() 
{
	/// �J�����g�t�H���_�̏��擾
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
	if (pfview->IsFolderWT(pfview->m_dwFolderType)) {
		CMsgForm *pform = (CMsgForm *)pFrame->m_pMsgForm->GetWnd();
		pform->Play();
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
BOOL CMsgList::ChkPlayIcon(
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

//
//  �@�\     : ���Đ����X�g���Đ��ς݂ɕύX
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMsgList::ChgMsgMark(
BOOL blUpdate)	// �e�[�u��UPDATE���܂ނ�
{
	CReportCtrl &rc = m_wndList;
	COLORREF clr = rc.GetItemBkColor(m_hActive, 2);
	if (clr == m_clrUnview) {	// ���Đ��̏ꍇ�̂�
		/// �e�[�u�����X�V
		_TCHAR buf[32];
		CTblRdfl tbl(theApp.m_strDBPath);
		CString strWhere;
		long lngId = rc.GetItemData(m_hActive);
		strWhere.Empty();
		_ltot_s(lngId, buf, sizeof(buf)/sizeof(buf[0]), 10);
		strWhere = strWhere + _T("ID") + _T("=") + buf;
		tbl.UpdateRec(_T("lngViewFlag=1"), strWhere);	
		//// ���X�g���X�V
		rc.SetItemBkColor(m_hActive, -1);	// �w�i�F
		rc.SetItemTextBold(m_hActive, -1);	// BOLD
	}

	return TRUE;
}

//
//  �@�\     : �A�g�v���O�����̎��s
//  
//  �@�\���� : �f�t�H���g�ݒ���g�p�i���[�U�Ή��ł͂Ȃ��j
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMsgList::ExecPrg(
int intPrgId)		// �v���O����ID
{
	CString str;
	UINT intCnt, i = 0;
	int idx;

	if (intPrgId < 1) return FALSE;
	CString strPrgId;
	strPrgId.Format(_T("%d"), intPrgId);

	// �A�g�v���O����CSV�t�@�C��
	CTPsetup tps(theApp.m_strDataFolder, _T("0"));	// �f�t�H���g�ݒ�
	CString strFile = tps.RsfPrgFileName(_T("$$$$"), strPrgId);
	CFileSpec fs(strFile);
	if (!fs.Exist()) {
		AfxTrace(_T("%s\n"), strFile);
		str.LoadString(IDS_MSGBOX107);
		te.ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_TAPUR, str);
		return FALSE;
	}
	// �A�g�v���O����CSV�̓��e���擾
	CString strCsv;
	if (!fs.LoadString(&strCsv)) {
		AfxTrace(_T("%s\n"), strCsv);
		str.LoadString(IDS_MSGBOX107);
		te.ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_TAPUR, str);
		return FALSE;
	}


	/// �I�����X�g���
	CUIntArray aryIdx;
	intCnt = SelectedList(&aryIdx, FALSE);
	if (intCnt < 1) return FALSE;

	/// �e�f�[�^�ɂ��Ď��s
	long lngId;
	CTblRdfl tbl(theApp.m_strDBPath);
	CTblPerson tblPerson(theApp.m_strDBPath);
	CString strWhere;
	for (i = 0; i < intCnt; i++) {
		// ��M�f�[�^
		idx = aryIdx.GetAt(i);
		lngId = m_wndList.GetItemData(idx);
		if (tbl.SelectRec(lngId) != 1) continue;

		// ���M�ҏ��
		strWhere.Format(_T("strId='%s'"), tbl.strSenderId);
		if (tblPerson.SelectRec(strWhere) != 1) continue;

		// �A�g�v���O�����̃p�����[�^����
		CTokenizer tok(strCsv, _T(","));
		CString cs, strExeFile;
		CStringArray aryArg;
		int k = 0;
		BOOL blExe = TRUE;
		BOOL blWait = FALSE;
		while(tok.Next(cs)) {
			if (k == 0) {
				// �v���O�����̏����҂��i������s���Ȃ��j
				blWait = (cs.CompareNoCase(_T("1")) == 0) ? TRUE : FALSE;
			} else if (k == 1) {
				// ���s�t�@�C���擾
				strExeFile = cs;
				if (strExeFile.Find(_T('\\')) < 0) {
					// �p�X���w�聨�e���v���[�gCSV�t�H���_��ǉ�
					fs.SetFullSpec(FS_APPDIR);
					strExeFile = fs.GetFullSpec() + _T("bin\\") + strExeFile;
				}
				fs.SetFullSpec(strExeFile);
				if (!fs.Exist()) {
					str.LoadString(IDS_MSGBOX107);
					te.ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_TAPUR, str);
					blExe = FALSE;
					return FALSE;
				}
			} else if (2 <= k) {
				// �����擾
				if (cs.CompareNoCase(_T("$AudioFile")) == 0) {				// �^���t�@�C��
					tps.SetCallerId(tbl.strSenderId);
					///// mp3 file
					strFile = tps.FindRdfMp3FileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
					if (strFile.IsEmpty()) {
						///// wav file
						strFile = tps.FindRdfWavFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
					}
					aryArg.Add(strFile);
				} else if (cs.CompareNoCase(_T("$VideoFile")) == 0) {			// �^��t�@�C��
					tps.SetCallerId(tbl.strSenderId);
					///// avi file
					strFile = tps.FindRdfVideoFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
					aryArg.Add(strFile);
				} else if (cs.CompareNoCase(_T("$PartnerId")) == 0) {			// �ʘb����(�X�J�C�vID)
						aryArg.Add(tbl.strSenderId);
				} else if (cs.CompareNoCase(_T("$PartnerName")) == 0) {		// �ʘb����(�X�J�C�v��)
						aryArg.Add(tbl.strSenderName);
				} else if (cs.CompareNoCase(_T("$UsrId")) == 0) {				// ���[�U(�X�J�C�vID)
						aryArg.Add(tbl.strReceiverId);
				} else if (cs.CompareNoCase(_T("$UsrName")) == 0) {			// ���[�U(�X�J�C�v��)
						aryArg.Add(tbl.strReceiverName);
				} else if (cs.CompareNoCase(_T("$PartnerAddress")) == 0) {		// �ʘb����Z��
						str = tblPerson.strCountry;
						str += tblPerson.strProvince;
						str += tblPerson.strCity;
						aryArg.Add(str);
				} else if (cs.CompareNoCase(_T("$PartnerPhoneHome")) == 0) {	// �ʘb����d�b�i����j
						aryArg.Add(tblPerson.strTel);
				} else if (cs.CompareNoCase(_T("$PartnerPhoneOffice")) == 0) {	// �ʘb����d�b�i�I�t�B�X�j
						aryArg.Add(tblPerson.strWkTel);
				} else if (cs.CompareNoCase(_T("$PartnerPhoneMobile")) == 0) {	// �ʘb����d�b�i���o�C���j
						aryArg.Add(tblPerson.strMbTel);
				} else {
					if (cs.Find(_T("(")) >= 0 && cs.Find(_T(")")) >= 0) {
						// �����\���̂܂܂̏ꍇ
						aryArg.Add(_T(""));
					} else {
						aryArg.Add(cs);
					}
				}
			}
			k++;
		}

		if (blExe) {
			// ���s
			fs.SetFullSpec(strExeFile);
			strExeFile = _T("\"") + strExeFile + _T("\"");
			CString strPrm = _T("");
			for (k = 0; k < aryArg.GetCount(); k++) {
				if (k > 0) strPrm = strPrm + _T(" ");
				strPrm = strPrm + _T("\"") + aryArg.GetAt(k) + _T("\"");
			}
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, strExeFile + _T(" ") + strPrm);
			if (blWait) {
				BOOL blRet;
				if (strExeFile.Right(5).CompareNoCase(_T(".bat\"")) == 0) {
					// bat�̎��s
					strExeFile += _T(" ") + strPrm;
					blRet = ExeProcess((LPTSTR)(LPCTSTR)strExeFile, (LPTSTR)(LPCTSTR)fs.GetFolder(), SW_HIDE, 1000*10);	// timeout:10s
				} else {
					// exe�̎��s
					strExeFile += _T(" ") + strPrm;
					blRet = ExeProcess((LPTSTR)(LPCTSTR)strExeFile, (LPTSTR)(LPCTSTR)fs.GetFolder(), SW_SHOWNORMAL, 1000*10);
				}
				if (!blRet) {
					str.LoadString(IDS_MSGBOX107);
					te.ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_TAPUR, str);
					return FALSE;
				}
			} else {
				HINSTANCE ret;
				if (strExeFile.Right(5).CompareNoCase(_T(".bat\"")) == 0) {
					// bat�̎��s
					ret = ShellExecute(NULL, _T("open"), strExeFile,  strPrm, fs.GetFolder(), SW_HIDE);
				} else {
					// exe�̎��s
					ret = ShellExecute(NULL, NULL, strExeFile,  strPrm, fs.GetFolder(), SW_SHOWNORMAL);
				}
				if ((int)ret <= 32) {
					str.LoadString(IDS_MSGBOX107);
					te.ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_TAPUR, str);
					return FALSE;
				}
			}
		}
	}

	str.LoadString(IDS_MSGBOX108);
	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
	return TRUE;
}

// CreateProcess()���g���ĊO���A�v���P�[�V�������N������
BOOL CMsgList::ExeProcess(
LPTSTR cmdstr,		// ���s�R�}���h
LPTSTR dirstr,		// ���s�t�H���_
int nShowCmd,		// ���s���̃E�B���h�E���
DWORD dwTimeout)	// �^�C���A�E�g�ims�j
{
	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;

	ZeroMemory( &sinfo, sizeof( STARTUPINFO ) );
	ZeroMemory( &pinfo, sizeof( PROCESS_INFORMATION ) );
	sinfo.cb = sizeof( STARTUPINFO );
	sinfo.dwFlags = STARTF_USESHOWWINDOW;
	sinfo.wShowWindow = nShowCmd;

	if ( !CreateProcess( NULL,
						 cmdstr,
						 NULL,
						 NULL,
						 FALSE,
						 0,
						 NULL,
						 dirstr,
						 &sinfo,
						 &pinfo ) ) { 
		// �N�����s
		return FALSE;
	}

	// �����҂�
    WaitForSingleObject( pinfo.hProcess, dwTimeout );

    CloseHandle( pinfo.hProcess );
    CloseHandle( pinfo.hThread );

	return TRUE;
}
