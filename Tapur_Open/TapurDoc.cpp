// TapurDoc.cpp : CTapurDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "Tapur.h"
#include "TapurDoc.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CTapurDoc

IMPLEMENT_DYNCREATE(CTapurDoc, CDocument)

BEGIN_MESSAGE_MAP(CTapurDoc, CDocument)
	//{{AFX_MSG_MAP(CTapurDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTapurDoc �N���X�̍\�z/����

CTapurDoc::CTapurDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CTapurDoc::~CTapurDoc()
{
}

BOOL CTapurDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTapurDoc �V���A���C�[�[�V����

void CTapurDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���̈ʒu�ɕۑ��p�̃R�[�h��ǉ����Ă��������B
	}
	else
	{
		// TODO: ���̈ʒu�ɓǂݍ��ݗp�̃R�[�h��ǉ����Ă��������B
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTapurDoc �N���X�̐f�f

#ifdef _DEBUG
void CTapurDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTapurDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTapurDoc commands
