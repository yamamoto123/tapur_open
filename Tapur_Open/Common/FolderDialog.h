// FolderDialog.h: CFolderDialog クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOLDERDIALOG_H__129B7AFB_DCF6_11D1_9880_88D807C10000__INCLUDED_)
#define AFX_FOLDERDIALOG_H__129B7AFB_DCF6_11D1_9880_88D807C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <shlobj.h>

class CFolderDialog  
{
private: //メンバ変数

	CString m_display;
	HWND m_parentHWnd;
	CString m_selectedFolder;
	CString m_defaulFolder;

private:
	static int AFXAPI BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData );
	LPITEMIDLIST GetItemIDList( CString p_cFileStr );

public:	
	virtual ~CFolderDialog();
	CFolderDialog(CString display=_T("フォルダを選択してください"),CString defaultc=_T(""),CWnd* pWnd=AfxGetMainWnd())
	{
		m_defaulFolder=defaultc;
		m_display=display;

		if(pWnd!=NULL)
			m_parentHWnd=pWnd->GetSafeHwnd();
		else
			m_parentHWnd=NULL;
	}
	BOOL DoModal();
	CString GetFolderPath(){return m_selectedFolder;}
	void SetDefaultFolder(CString def){m_defaulFolder=def;}
	void SetDisplay(CString display){m_display=display;}

};

#endif // !defined(AFX_FOLDERDIALOG_H__129B7AFB_DCF6_11D1_9880_88D807C10000__INCLUDED_)
