// FolderForm.cpp : implementation file
//

#include "stdafx.h"
#include "Tapur.h"
#include "FolderForm.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CFolderForm

IMPLEMENT_DYNCREATE(CFolderForm, TVisualFormView)

CFolderForm::CFolderForm()
	: TVisualFormView(CFolderForm::IDD)
{
	//{{AFX_DATA_INIT(CFolderForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// �w�i�F�u���V������
	m_brush.CreateSolidBrush(RGB(255,255,255));		// white
}

CFolderForm::~CFolderForm()
{
}

void CFolderForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFolderForm)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFolderForm, TVisualFormView)
	//{{AFX_MSG_MAP(CFolderForm)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderForm diagnostics

#ifdef _DEBUG
void CFolderForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CFolderForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFolderForm message handlers

void CFolderForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

}

HBRUSH CFolderForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = TVisualFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������
	
	// TODO: �f�t�H���g�̃u���V���]�݂̂��̂łȂ��ꍇ�ɂ́A�Ⴄ�u���V��Ԃ��Ă�������
	// �w�i�F�𔒂ɕύX
	if (nCtlColor == CTLCOLOR_DLG) 
		return (HBRUSH)m_brush.GetSafeHandle();

	return hbr;
}
