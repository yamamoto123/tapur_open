// Japanese.h : JAPANESE アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_JAPANESE_H__999A7E0F_7D1D_4644_B7D6_167AAA697032__INCLUDED_)
#define AFX_JAPANESE_H__999A7E0F_7D1D_4644_B7D6_167AAA697032__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CJapaneseApp
// このクラスの動作の定義に関しては Japanese.cpp ファイルを参照してください。
//

class CJapaneseApp : public CWinApp
{
public:
	CJapaneseApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CJapaneseApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CJapaneseApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_JAPANESE_H__999A7E0F_7D1D_4644_B7D6_167AAA697032__INCLUDED_)
