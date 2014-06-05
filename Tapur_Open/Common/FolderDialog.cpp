// FolderDialog.cpp: CFolderDialog クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FolderDialog.h"
#include <shlobj.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
CFolderDialog::~CFolderDialog()
{

}

BOOL CFolderDialog::DoModal()
{
	//宣言
	BOOL fRes=FALSE;
	BROWSEINFO bi;
	_TCHAR *lpBuffer;
	LPITEMIDLIST pidlRootFolders;
	LPITEMIDLIST pidlBrowse;
	LPITEMIDLIST pidlDefault=0;
	IMalloc* g_pMalloc;
	SHGetMalloc(&g_pMalloc);

	//無効な値なら関数を出る
	if((lpBuffer=(_TCHAR *)g_pMalloc->Alloc(MAX_PATH * sizeof(_TCHAR))) == NULL)
		return fRes;
	if (!SUCCEEDED(SHGetSpecialFolderLocation(m_parentHWnd,CSIDL_DESKTOP,&pidlRootFolders)))
	{
		g_pMalloc->Free(lpBuffer);
		return fRes;
	}

	//デフォルトのフォルダが指定されているかどうかを見る
	if(m_defaulFolder!=_T(""))
		pidlDefault=GetItemIDList(m_defaulFolder);

	//構築
	bi.hwndOwner=m_parentHWnd;
	bi.pidlRoot=pidlRootFolders;
	bi.pszDisplayName=lpBuffer;
	bi.lpszTitle=m_display;
	bi.ulFlags=0;
	bi.lpfn=(BFFCALLBACK)BrowseCallbackProc;
	bi.lParam=(LPARAM)pidlDefault;

	//実際に表示
	pidlBrowse=SHBrowseForFolder(&bi);

	//データを取得
	if(pidlBrowse!=NULL)
	{
		if(SHGetPathFromIDList(pidlBrowse,lpBuffer))
		{
			m_selectedFolder.Format(_T("%s"),lpBuffer);
			fRes=TRUE;
		}
		g_pMalloc->Free(pidlBrowse);
	}

	//後始末
	g_pMalloc->Free(pidlRootFolders);
	g_pMalloc->Free(lpBuffer);
	g_pMalloc->Free(pidlDefault);

	return fRes;
}

LPITEMIDLIST CFolderDialog::GetItemIDList(CString p_cFileStr)
{
	//無効な値なら関数を出る
	if( p_cFileStr.IsEmpty())
		return NULL;

	//宣言
	LPITEMIDLIST pIDL;
	LPSHELLFOLDER pDesktopFolder; 

	//無効なら関数を出る
	if( ::SHGetDesktopFolder( &pDesktopFolder ) != NOERROR )
		return NULL;

	//宣言
	OLECHAR ochPath[MAX_PATH];
	ULONG chEaten;	//文字列のサイズを受け取ります。
	ULONG dwAttributes;	//属性を受け取ります。
	HRESULT hRes;

	//文字列の変換
	::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, (LPCSTR)(LPCTSTR)p_cFileStr, -1, ochPath, MAX_PATH );
	//実際にITEMIDLISTを取得
	hRes = pDesktopFolder->ParseDisplayName( NULL, NULL, ochPath, &chEaten, &pIDL, &dwAttributes);
	
	//もしエラーが出たら関数を出る
	if( hRes != NOERROR )
		pIDL = NULL;
	
	//後始末
	pDesktopFolder->Release();

	return pIDL;

}

int AFXAPI CFolderDialog::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	//　初期化時にフォルダを選択させます。
	if( uMsg == BFFM_INITIALIZED )	
	{
		::SetWindowText(hwnd,_T("フォルダの参照"));
		::SendMessage(hwnd,BFFM_SETSELECTION,FALSE,lpData);
	}
	else
	{
		_TCHAR chText[MAX_PATH];
		if(::SHGetPathFromIDList((LPITEMIDLIST)lParam,chText))
		::SendMessage(hwnd,BFFM_SETSTATUSTEXT,TRUE,(LPARAM)chText);
	}
	return 0;
}
