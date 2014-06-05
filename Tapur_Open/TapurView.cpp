// TapurView.cpp : CTapurView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Tapur.h"
#include "TapurDoc.h"
#include "TapurView.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CTapurView

IMPLEMENT_DYNCREATE(CTapurView, CView)

BEGIN_MESSAGE_MAP(CTapurView, CView)
	//{{AFX_MSG_MAP(CTapurView)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//    この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTapurView クラスの構築/消滅

CTapurView::CTapurView()
{
	// TODO: この場所に構築用のコードを追加してください。

}

CTapurView::~CTapurView()
{
}

BOOL CTapurView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATapurRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTapurView クラスの描画

void CTapurView::OnDraw(CDC* pDC)
{
	CTapurDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}

/////////////////////////////////////////////////////////////////////////////
// CTapurView クラスの印刷

BOOL CTapurView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
	return DoPreparePrinting(pInfo);
}

void CTapurView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CTapurView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

/////////////////////////////////////////////////////////////////////////////
// CTapurView クラスの診断

#ifdef _DEBUG
void CTapurView::AssertValid() const
{
	CView::AssertValid();
}

void CTapurView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTapurDoc* CTapurView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTapurDoc)));
	return (CTapurDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTapurView クラスのメッセージ ハンドラ
