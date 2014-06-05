// FolderDialog.cpp: CFolderDialog �N���X�̃C���v�������e�[�V����
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
// �\�z/����
//////////////////////////////////////////////////////////////////////
CFolderDialog::~CFolderDialog()
{

}

BOOL CFolderDialog::DoModal()
{
	//�錾
	BOOL fRes=FALSE;
	BROWSEINFO bi;
	_TCHAR *lpBuffer;
	LPITEMIDLIST pidlRootFolders;
	LPITEMIDLIST pidlBrowse;
	LPITEMIDLIST pidlDefault=0;
	IMalloc* g_pMalloc;
	SHGetMalloc(&g_pMalloc);

	//�����Ȓl�Ȃ�֐����o��
	if((lpBuffer=(_TCHAR *)g_pMalloc->Alloc(MAX_PATH * sizeof(_TCHAR))) == NULL)
		return fRes;
	if (!SUCCEEDED(SHGetSpecialFolderLocation(m_parentHWnd,CSIDL_DESKTOP,&pidlRootFolders)))
	{
		g_pMalloc->Free(lpBuffer);
		return fRes;
	}

	//�f�t�H���g�̃t�H���_���w�肳��Ă��邩�ǂ���������
	if(m_defaulFolder!=_T(""))
		pidlDefault=GetItemIDList(m_defaulFolder);

	//�\�z
	bi.hwndOwner=m_parentHWnd;
	bi.pidlRoot=pidlRootFolders;
	bi.pszDisplayName=lpBuffer;
	bi.lpszTitle=m_display;
	bi.ulFlags=0;
	bi.lpfn=(BFFCALLBACK)BrowseCallbackProc;
	bi.lParam=(LPARAM)pidlDefault;

	//���ۂɕ\��
	pidlBrowse=SHBrowseForFolder(&bi);

	//�f�[�^���擾
	if(pidlBrowse!=NULL)
	{
		if(SHGetPathFromIDList(pidlBrowse,lpBuffer))
		{
			m_selectedFolder.Format(_T("%s"),lpBuffer);
			fRes=TRUE;
		}
		g_pMalloc->Free(pidlBrowse);
	}

	//��n��
	g_pMalloc->Free(pidlRootFolders);
	g_pMalloc->Free(lpBuffer);
	g_pMalloc->Free(pidlDefault);

	return fRes;
}

LPITEMIDLIST CFolderDialog::GetItemIDList(CString p_cFileStr)
{
	//�����Ȓl�Ȃ�֐����o��
	if( p_cFileStr.IsEmpty())
		return NULL;

	//�錾
	LPITEMIDLIST pIDL;
	LPSHELLFOLDER pDesktopFolder; 

	//�����Ȃ�֐����o��
	if( ::SHGetDesktopFolder( &pDesktopFolder ) != NOERROR )
		return NULL;

	//�錾
	OLECHAR ochPath[MAX_PATH];
	ULONG chEaten;	//������̃T�C�Y���󂯎��܂��B
	ULONG dwAttributes;	//�������󂯎��܂��B
	HRESULT hRes;

	//������̕ϊ�
	::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, (LPCSTR)(LPCTSTR)p_cFileStr, -1, ochPath, MAX_PATH );
	//���ۂ�ITEMIDLIST���擾
	hRes = pDesktopFolder->ParseDisplayName( NULL, NULL, ochPath, &chEaten, &pIDL, &dwAttributes);
	
	//�����G���[���o����֐����o��
	if( hRes != NOERROR )
		pIDL = NULL;
	
	//��n��
	pDesktopFolder->Release();

	return pIDL;

}

int AFXAPI CFolderDialog::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	//�@���������Ƀt�H���_��I�������܂��B
	if( uMsg == BFFM_INITIALIZED )	
	{
		::SetWindowText(hwnd,_T("�t�H���_�̎Q��"));
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
