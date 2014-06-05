// TapurDoc.cpp : CTapurDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Tapur.h"
#include "TapurDoc.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
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
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTapurDoc クラスの構築/消滅

CTapurDoc::CTapurDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CTapurDoc::~CTapurDoc()
{
}

BOOL CTapurDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTapurDoc シリアライゼーション

void CTapurDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: この位置に保存用のコードを追加してください。
	}
	else
	{
		// TODO: この位置に読み込み用のコードを追加してください。
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTapurDoc クラスの診断

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
