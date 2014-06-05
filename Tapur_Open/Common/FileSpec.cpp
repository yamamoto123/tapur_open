/*
 *	$Header: /common/FileSpec.cpp 3     8/05/01 10:38p Admin $
 *
 *	$History: FileSpec.cpp $
 * 
 * *****************  Version 4  *****************
 * User: PAAK        Date: 12/04/05
 * 
 * *****************  Version 3  *****************
 * User: Admin        Date: 8/05/01    Time: 10:38p
 * Updated in $/common
 * Fixes to the /msg crash bug.  Changed the trigger point for Autostrut
 * and auto scores.  Added auto scores.
 * 
 * *****************  Version 2  *****************
 * User: Admin        Date: 12/04/01   Time: 11:40p
 * Updated in $/common
 * Fixed some resource leaks.  Added code to update the server list via
 * Sir Prober.
 * 
 * *****************  Version 1  *****************
 * User: Admin        Date: 28/03/01   Time: 8:42p
 * Created in $/common
 */
#include "stdafx.h"
#include <stdlib.h>
#include <io.h>
#include <sys\types.h> 
#include <sys\stat.h> 
#include <shlobj.h>
#include <lm.h>
#include <atlconv.h>
#include <direct.h>
#include <shlobj.h>
#include <shellapi.h>	//paak
#include "Shlwapi.h"	//paak

#include "Filespec.h"

#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "mpr.lib")

#ifdef countof
#undef countof
#endif

#define countof(x) (sizeof(x) / sizeof((x)[0]))

/*
// CreateProcess()を使って外部アプリケーションを起動する
static int ExeProcess(LPTSTR cmdstr)
{

	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;

	ZeroMemory( &sinfo, sizeof( STARTUPINFO ) );
	ZeroMemory( &pinfo, sizeof( PROCESS_INFORMATION ) );
	sinfo.cb = sizeof( STARTUPINFO );
	sinfo.dwFlags = STARTF_USESHOWWINDOW;
	sinfo.wShowWindow = SW_SHOWNORMAL;

	if ( !CreateProcess( NULL,
						 cmdstr,
						 NULL,
						 NULL,
						 FALSE,
						 0,
						 NULL,
						 NULL,
						 &sinfo,
						 &pinfo ) ) { 
		// 起動失敗
		return -1;
	}

	// 処理が終わるまで制御を戻しません
	WaitForInputIdle( pinfo.hProcess, INFINITE );
	WaitForSingleObject( pinfo.hProcess, INFINITE );
	return 0;
}
*/
CFileSpec::CFileSpec(FS_BUILTINS eSpec)
{
	Initialise(eSpec);
}

CFileSpec::CFileSpec(FS_BUILTINS eSpec, LPCTSTR szFilename)
{
	ASSERT(szFilename);
	ASSERT(AfxIsValidString(szFilename));

	Initialise(eSpec);
	SetFileName(szFilename);
}

CFileSpec::CFileSpec(LPCTSTR szSpec, LPCTSTR szFilename)
{
	ASSERT(szFilename);
	ASSERT(AfxIsValidString(szFilename));

	SetFullSpec(szSpec);
	SetFileName(szFilename);
}

CFileSpec::CFileSpec(LPCTSTR szFilename)
{
	ASSERT(szFilename);
	ASSERT(AfxIsValidString(szFilename));

	SetFullSpec(szFilename);
}

void CFileSpec::Initialise(FS_BUILTINS eSpec)
{
	CString str;
	TCHAR tszPath[_MAX_PATH],
		  *ptsz;

	switch (eSpec)
	{
	case FS_EMPTY:								//	Nothing
		break;

	case FS_APP:								//	Full application path and name
		GetModuleFileName(NULL, tszPath, countof(tszPath));
		SetFullSpec(LPCTSTR(tszPath));
		break;

	case FS_APPDIR:								//	Application folder
		GetModuleFileName(NULL, tszPath, countof(tszPath));
		SetFullSpec(LPCTSTR(tszPath));
		m_csExtension = _T("");
		m_csFilename = _T("");
		break;

	case FS_WINDIR:								//	Windows folder
		GetWindowsDirectory(tszPath, countof(tszPath));
		PathAddBackslash(tszPath);
		SetFullSpec(LPCTSTR(tszPath));
		break;

	case FS_SYSDIR:								//	System folder
		GetSystemDirectory(tszPath, countof(tszPath));
		PathAddBackslash(tszPath);
		SetFullSpec(LPCTSTR(tszPath));
		break;

	case FS_TMPDIR:								//	Temporary folder
		GetTempPath(countof(tszPath), tszPath);
		SetFullSpec(LPCTSTR(tszPath));
		break;

	case FS_DESKTOP:							//	Desktop folder
		GetShellFolder(CSIDL_DESKTOP);
		break;

	case FS_PROGRAMS:							//	プログラムメニュー
		GetShellFolder(CSIDL_PROGRAMS);
		break;

	case FS_PERSONAL:							//	User Folder
		GetShellFolder(CSIDL_PERSONAL);
		break;

	case FS_MYDOC:								//	My Document
		//GetShellFolder(CSIDL_MYDOCUMENTS);
		GetShellFolder(CSIDL_PERSONAL);
		break;

	case FS_PROGRAM_FILES:						//	Program Files
		GetShellFolder(CSIDL_PROGRAM_FILES);
		break;

	case FS_STARTUP:							//	Startup
		GetShellFolder(CSIDL_STARTUP);
		break;

	case FS_FAVOURITES:							//	Favourites folder
		GetShellFolder(CSIDL_FAVORITES);
		break;

	case FS_TEMPNAME:
		GetTempPath(countof(tszPath), tszPath);
#ifdef _UNICODE
		_tcscpy_s(tszPath, _MAX_PATH, ptsz = _wtempnam(tszPath, _T("~")));
#else
		_tcscpy_s(tszPath, _MAX_PATH, ptsz = _tempnam(tszPath, _T("~")));
#endif
		SetFullSpec(LPCTSTR(tszPath));
		free(ptsz);
		break;

	case FS_MEDIA:
		GetWindowsDirectory(tszPath, countof(tszPath));
		PathAddBackslash(tszPath);
		_tcscat_s(tszPath, _MAX_PATH, _T("media\\"));
		SetFullSpec(LPCTSTR(tszPath));
		break;

	case FS_CURRDIR:
		GetCurrentDirectory(countof(tszPath), tszPath);
		PathAddBackslash(tszPath);
		SetFullSpec(LPCTSTR(tszPath));
		break;

	default:
#ifdef _DEBUG
		TRACE(_T("Invalid initialisation spec for CFileSpec, %d\n"), eSpec);
#endif
		ASSERT(NULL);
	}
}

void CFileSpec::GetShellFolder(int iFolder)
{
	LPITEMIDLIST pItemList;
	LPMALLOC	 pMalloc;

	SHGetMalloc(&pMalloc);

	if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, iFolder, &pItemList)))
	{
		TCHAR tszPath[_MAX_PATH];

		if (SHGetPathFromIDList(pItemList, tszPath))
			SetFullSpec(tszPath);

		pMalloc->Free(pItemList);
	}
}

//	Operations
BOOL CFileSpec::LoadArchive(CObject *pObj)
{
	CFile file;
	BOOL  bStatus = FALSE;

	ASSERT(pObj);
	ASSERT_VALID(pObj);
	ASSERT_KINDOF(CObject, pObj);
	ASSERT(pObj->IsSerializable());

	if (Exist())
	{
		try
		{
			if (file.Open(GetFullSpec(), CFile::modeRead | CFile::typeBinary | CFile::shareExclusive))
			{
				CArchive ar(&file, CArchive::load);

				pObj->Serialize(ar);
				ar.Close();
				file.Close();
				bStatus = TRUE;
			}
		}
		catch(CException *e)
		{
			e->Delete();
		}
	}

	return bStatus;
}

BOOL CFileSpec::SaveArchive(CObject *pObj)
{
	CFile file;
	BOOL  bStatus = FALSE;

	ASSERT(pObj);
	ASSERT_VALID(pObj);
	ASSERT_KINDOF(CObject, pObj);
	ASSERT(pObj->IsSerializable());

	WriteAble();

	try
	{
		if (file.Open(GetFullSpec(), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareExclusive))
		{
			CArchive ar(&file, CArchive::store);

			pObj->Serialize(ar);
			ar.Close();
			file.Close();
			bStatus = TRUE;
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}

	//ReadOnly();
	return bStatus;
}

// load text file
BOOL CFileSpec::LoadString(CString *pStr)
{
	CFile file;
	BOOL  bStatus = FALSE;
	char *buf;

	ASSERT(pStr);

	if (Exist())
	{
		try
		{
			if (file.Open(GetFullSpec(), CFile::modeRead | CFile::typeBinary | CFile::shareExclusive))
			{
				long size = (long)(file.GetLength() + 2);
				buf = new char[size];
				memset(buf, NULL, size);
				file.Read(buf, size-1);
				file.Close();
				*pStr = buf;
				bStatus = TRUE;
			}
		}
		catch(CException *e)
		{
			e->Delete();
		}
	}

	delete [] buf;
	return bStatus;
}

// save text file
// blUnicode:UNICODEのまま書き込み
BOOL CFileSpec::SaveString(CString strTxt, BOOL blAdd, BOOL blUnicode)
{
	CFile file;
	BOOL  bStatus = FALSE;
	DWORD dwLen;
	char *buf = NULL;
	wchar_t *wbuf = NULL;

	WriteAble();

	try
	{
		UINT nOpenFlags = CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyWrite;
		if (blAdd) nOpenFlags |= CFile::modeNoTruncate;
		if (file.Open(GetFullSpec(), nOpenFlags))
		{
			if (blAdd) file.SeekToEnd();
			if (blUnicode) {
				if (file.GetPosition() == 0) {
					// ファイル先頭にBOMを追加
					wchar_t wBom = 0xfeff;
					file.Write(&wBom,sizeof(wBom));
					file.SeekToEnd();
				}
				CStringW strW = strTxt;
				dwLen = strW.GetLength();
				wbuf = new wchar_t[dwLen+1];	// +1:終端NULL用
				wcscpy_s(wbuf, dwLen+1, strW);
				file.Write(wbuf, sizeof(wchar_t) * dwLen);	// byte数
			} else {
				//VS2005 error CStringA strA = strTxt;
				CStringA strA(strTxt);
				//// wcstombs()は日本語を正常に処理しない
				//dwLen = ::WideCharToMultiByte(CP_ACP,0,strTxt,-1,NULL,0,NULL,NULL);
				//buf = new char[dwLen];
				//::WideCharToMultiByte(CP_ACP, 0, strTxt, -1, buf, dwLen, NULL, NULL);
				//file.Write(buf, dwLen-1);	// 末尾のNULL文字は除外
				dwLen = strA.GetLength();
				buf = new char[dwLen+1];
				strcpy_s(buf, dwLen+1, strA);
				file.Write(buf, dwLen);	
			}
			file.Close();
			bStatus = TRUE;
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}

	if (buf) delete [] buf;
	if (wbuf) delete [] wbuf;
	return bStatus;
}

BOOL CFileSpec::IsUNCPath()
{
	return IsUNCPath(m_csDrive);
}

BOOL CFileSpec::IsUNCPath(LPCTSTR szPath)
{
	ASSERT(szPath);
	ASSERT(AfxIsValidString(szPath));

	if (_tcslen(szPath) < 2)
		return FALSE;
	
	return (szPath[0] == '\\' && szPath[1] == '\\');
}

BOOL CFileSpec::IsFolder()
{
	CString szPath = GetFullSpec();

	if (szPath.GetLength() < 2)
		return FALSE;
	
	DWORD dwAttr = GetFileAttributes(szPath);
	BOOL bFolder = (dwAttr & FILE_ATTRIBUTE_DIRECTORY);
	return bFolder;
}

BOOL CFileSpec::Exist()
{
/*　遅い
	CFileStatus status;
	return CFile::GetStatus(GetFullSpec(), status);
*/
#ifdef _UNICODE
	return _waccess(GetFullSpec(), 0) == 0;
#else
	return _access(GetFullSpec(), 0) == 0;
#endif
}

BOOL CFileSpec::MkFolder()
{
    TCHAR strTemp[MAX_PATH];
	TCHAR string[MAX_PATH];
	TCHAR seps[] = _T("\\");
	LPTSTR token;
	TCHAR *next_token = NULL;
    
	TRY {
		_tcscpy_s(string, MAX_PATH, GetFolder());
		if (_taccess_s(string, 0) == 0) return TRUE;
	    
		_tcscpy_s(strTemp, MAX_PATH, _T(""));
		token = _tcstok_s( string, seps, &next_token );
		while( token != NULL )
		{
			_tcscat_s(strTemp, MAX_PATH, token);
			_tcscat_s(strTemp, MAX_PATH, _T("\\"));
			if (_taccess_s(strTemp, 0) != 0) {
				if (_tmkdir(strTemp) != 0) return FALSE;
			}
			token = _tcstok_s( NULL, seps, &next_token );
		}
	}
    CATCH_ALL(e)
    {
		return FALSE;
    }
	END_CATCH_ALL


	return TRUE;
}

// フォルダ内のファイル削除
BOOL CFileSpec::ClearFolder()
{
    TCHAR strTemp[MAX_PATH];
	TCHAR string[MAX_PATH];
	TCHAR seps[] = _T("\\");
    
	_tcscpy_s(string, MAX_PATH, GetFolder());
	if (_taccess_s(string, 0) != 0) return TRUE;
    
	WIN32_FIND_DATA info;
	HANDLE hp; 
	_stprintf_s(strTemp, MAX_PATH, _T("%s\\*.*"), string);
	hp = FindFirstFile(strTemp, &info);
	TRY {
		do
		{
		   _stprintf_s(strTemp, MAX_PATH, _T("%s\\%s"), string, info.cFileName);
		   DeleteFile(strTemp);

		}while(FindNextFile(hp, &info)); 
		FindClose(hp);
	}
    CATCH_ALL(e)
    {
		FindClose(hp);
		return FALSE;
    }
	END_CATCH_ALL

	return TRUE;
}

BOOL CFileSpec::FileCopy(LPCTSTR newFilename, BOOL blOverwrite, BOOL blCancelEnable, HWND hwnd)
{
	BOOL blRet;

	if (blCancelEnable) {
	/// Cancel可の場合
		_TCHAR	from[MAX_PATH];
		_TCHAR	to[MAX_PATH];
		_TCHAR	msg[MAX_PATH];
		_tcscpy_s(from, MAX_PATH, GetFullSpec());
		_tcscpy_s(to, MAX_PATH, newFilename);
		// 末尾の\は削除
		if (from[_tcsnlen(from, MAX_PATH) - 1] == '\\') from[_tcsnlen(from, MAX_PATH) - 1] = NULL;
		if (to[_tcsnlen(to, MAX_PATH) - 1] == '\\') to[_tcsnlen(to, MAX_PATH) - 1] = NULL;
		// from, to:double NULL必須（でないと複数ファイルと認識される）
		from[_tcsnlen(from, MAX_PATH) + 1] = NULL;
		to[_tcsnlen(to, MAX_PATH) + 1] = NULL;
		_stprintf_s(msg, MAX_PATH, _T("Copy to %s"), newFilename);
		SHFILEOPSTRUCT fop;
		ZeroMemory(&fop, sizeof(fop));
		fop.hwnd = hwnd;
		fop.wFunc = FO_COPY;
		fop.pFrom = (LPTSTR)from;
		fop.pTo = (LPTSTR)to;
		fop.fFlags = FOF_FILESONLY | FOF_NOCONFIRMATION | FOF_SIMPLEPROGRESS;
		fop.lpszProgressTitle = (LPTSTR)msg;
		if (SHFileOperation(&fop) == 0) {	// 待機しない
			blRet = TRUE;
		} else {
			blRet = FALSE;
		}
	} else {
	/// Cancel不可の場合
		blRet = ::CopyFile(GetFullSpec(), newFilename, !blOverwrite);
	}

	return blRet;
}

/*
// Shellコマンドを使用して実行
BOOL CFileSpec::FastFileCopy(LPCTSTR newFilename, BOOL blOverwrite)
{
	_TCHAR buf[MAX_PATH];

	_stprintf_s(buf, MAX_PATH, _T("copy \"%s\" \"%s\""), (LPCTSTR)GetFullSpec(), newFilename);
	if (blOverwrite) {
		_tcscat(buf, _T(" /Y"));
	} else {
		_tcscat(buf, _T(" /-Y"));
	}
TRACE(_T("FastFileCopy:%s\n"), buf);
	int ret = ExeProcess(buf);
	
	return (ret < 0) ? FALSE : TRUE;
}
*/

BOOL CFileSpec::FileDelete(BOOL blCancelEnable, HWND hwnd)
{
	BOOL blRet;

	if (blCancelEnable) {
	/// Cancel可の場合
		_TCHAR	from[MAX_PATH];
		_TCHAR	msg[MAX_PATH];
		_tcscpy_s(from, MAX_PATH, GetFullSpec());
		// 末尾の\は削除
		if (from[_tcsnlen(from, MAX_PATH) - 1] == '\\') from[_tcsnlen(from, MAX_PATH) - 1] = NULL;
		// from, to:double NULL必須（でないと複数ファイルと認識される）
		from[_tcsnlen(from, MAX_PATH) + 1] = NULL;
		_stprintf_s(msg, MAX_PATH, _T("Delete %s"), from);
		SHFILEOPSTRUCT fop;
		ZeroMemory(&fop, sizeof(fop));
		fop.hwnd = hwnd;
		fop.wFunc = FO_DELETE;
		fop.pFrom = (LPTSTR)from;
		fop.pTo = NULL;
		fop.fFlags = FOF_FILESONLY | FOF_NOCONFIRMATION | FOF_SIMPLEPROGRESS;
		fop.lpszProgressTitle = (LPTSTR)msg;
		if (SHFileOperation(&fop) == 0) {	// 待機しない
			blRet = TRUE;
		} else {
			blRet = FALSE;
		}
	} else {
		/// Cancel不可の場合
		// 読み取り専用属性のあるファイルは属性を変えてから削除
		SetFileAttributes(GetFullSpec(), FILE_ATTRIBUTE_NORMAL);
		BOOL ret = ::DeleteFile(GetFullSpec());
		if (!ret) {
			_TCHAR ErrStr[256];
			::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, GetLastError(), NULL, ErrStr, sizeof(ErrStr), NULL);
TRACE(_T("FileDelete error:%s:%s\n"), GetFullSpec(), ErrStr);
		}
	}

	return blRet;
}

BOOL CFileSpec::FileMove(LPCTSTR newFilename, 
BOOL blOverwrite,	// 同名ファイルの上書き指定
BOOL blWait)		// 移動が完了するまで制御を返さない
{
	DWORD flag = MOVEFILE_COPY_ALLOWED; 
	if (blWait) flag |= MOVEFILE_WRITE_THROUGH;
	if (blOverwrite) flag |= MOVEFILE_REPLACE_EXISTING;
	return ::MoveFileEx(GetFullSpec(), newFilename, flag);
}

// ファイルサイズの取得
// -1:error
ULONGLONG CFileSpec::FileSize()
{
	// インデックスから取得するため、正確でない場合がある
	//struct _stat64 fileStat; 
	//int err = _tstat64( GetFullSpec(), &fileStat ); 
	//if (0 != err) return 0; 
	//return fileStat.st_size; 
	
    HANDLE hFile = CreateFile(GetFullSpec(), 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == INVALID_HANDLE_VALUE) goto TSTAT_SECTION;

    LARGE_INTEGER FileSize;
    SecureZeroMemory( &FileSize, sizeof(FileSize));
    if (!GetFileSizeEx(hFile, &FileSize)) 
    {
		CloseHandle(hFile);
        goto TSTAT_SECTION;
    }
    CloseHandle ( hFile ) ;

	return FileSize.QuadPart;

TSTAT_SECTION:
	// インデックスから取得するため、正確でない場合がある
	struct _stat64 fileStat; 
	int err = _tstat64( GetFullSpec(), &fileStat ); 
	if (0 != err) return 0; 
	return fileStat.st_size; 
}

// 空きディスク容量の取得（Byte）
// -1:error
ULONGLONG CFileSpec::FreeDisk()
{
  ULARGE_INTEGER i64Free;
  ULARGE_INTEGER i64Avail;
  ULARGE_INTEGER i64Total;
  
  // ディスク情報の取得
  if (!GetDiskFreeSpaceEx(m_csDrive, &i64Free, &i64Total, &i64Avail)) return -1;

  return i64Avail.QuadPart; 
}


void CFileSpec::WriteAble()
{
#ifdef _UNICODE
	if (Exist())
		_wchmod(GetFullSpec(), _S_IWRITE | _S_IREAD);
#else
	if (Exist())
		_chmod(GetFullSpec(), _S_IWRITE | _S_IREAD);
#endif
}

void CFileSpec::ReadOnly()
{
#ifdef _UNICODE
	if (Exist())
		_wchmod(GetFullSpec(), _S_IREAD);
#else
	if (Exist())
		_chmod(GetFullSpec(), _S_IREAD);
#endif
}

//	Access functions
CString CFileSpec::GetFileName()
{
	return m_csFilename + m_csExtension;
}

void CFileSpec::SetFileName(LPCTSTR szSpec)
{
	ASSERT(szSpec);
	ASSERT(AfxIsValidString(szSpec));

	TCHAR tszFilename[_MAX_FNAME],
		  tszExtension[_MAX_EXT];

	_tsplitpath_s(szSpec, NULL, 0, NULL, 0, tszFilename, _MAX_FNAME, tszExtension, _MAX_EXT);
	m_csFilename = tszFilename;
	m_csExtension = tszExtension;
}

CString CFileSpec::FullPathNoExtension()
{
	return m_csDrive + m_csPath + m_csFilename;
}

CString CFileSpec::GetFolder()
{
	CString str = m_csDrive + m_csPath;
	return str;
}

CString CFileSpec::GetFullSpec()
{
	return m_csDrive + m_csPath + m_csFilename + m_csExtension;
}

void CFileSpec::SetFullSpec(FS_BUILTINS eSpec)
{
	Initialise(eSpec);
}

void CFileSpec::SetFullSpec(LPCTSTR szSpec)
{
	ASSERT(szSpec);
	ASSERT(AfxIsValidString(szSpec));

	TCHAR	tszDrive[_MAX_DRIVE],
			tszPath[_MAX_PATH],
			tszFilename[_MAX_FNAME],
			tszExtension[_MAX_EXT];
	LPCTSTR ptsz = szSpec;
	int		i = 4;

	if (IsUNCPath(szSpec))
	{
		//	UNC Paths are of the form \\server\share\path\filename[.ext]
		//	For the purpose if this code we treat the server/share portion
		//	as being logically equivalent to a drive and the rest of the 
		//	path then works the same way as for the older style path.
		m_csDrive = _T("");

		//	Brute force copy of the server/share part of the string
		while (i && *ptsz)
		{
			if (*ptsz == '\\')
				i--;

			if (i)
			{
				m_csDrive += *ptsz;

				ptsz++;
			}
		}

		_tsplitpath_s(ptsz, tszDrive, _MAX_DRIVE, tszPath, _MAX_PATH, tszFilename, _MAX_FNAME, tszExtension, _MAX_EXT);
		m_csPath = tszPath;
		m_csFilename = tszFilename;
		m_csExtension = tszExtension;
	}
	else
	{
		_tsplitpath_s(ptsz, tszDrive, _MAX_DRIVE, tszPath, _MAX_PATH, tszFilename, _MAX_FNAME, tszExtension, _MAX_EXT);
		m_csDrive = tszDrive;
		m_csPath = tszPath;
		m_csFilename = tszFilename;
		m_csExtension = tszExtension;
	}
}

CString CFileSpec::ConvertToUNCPath()
{
	USES_CONVERSION;

	CString csPath = GetFullSpec();

	if (IsUNCPath(csPath))
		return csPath;

	if (csPath[1] == ':')
	{
		// Fully qualified pathname including a drive letter, check if it's a mapped drive
		UINT uiDriveType = GetDriveType(m_csDrive);
		
		if (uiDriveType & DRIVE_REMOTE)
		{
			//	Yup - it's mapped so convert to a UNC path...
			TCHAR				tszTemp[_MAX_PATH];
			UNIVERSAL_NAME_INFO *uncName = (UNIVERSAL_NAME_INFO *) tszTemp;
			DWORD				dwSize = _MAX_PATH;
			DWORD				dwRet = WNetGetUniversalName(m_csDrive, REMOTE_NAME_INFO_LEVEL, uncName, &dwSize);
			CString				csDBShare;
			
			if (dwRet == NO_ERROR)
				return uncName->lpUniversalName + m_csPath + m_csFilename + m_csExtension;
		}
		else
		{
			//	It's a local drive so search for a share to it...
			NET_API_STATUS	res;
			PSHARE_INFO_502 BufPtr,
							p;
			DWORD			er = 0,
							tr = 0,
							resume = 0,
							i;
			int				iBestMatch = 0;
			CString			csTemp,
							csTempDrive,
							csBestMatch;
			
			do
			{
				res = NetShareEnum(NULL, 502, (LPBYTE *) &BufPtr, DWORD(-1), &er, &tr, &resume);

				//
				// If the call succeeds,
				//
				if (res == ERROR_SUCCESS || res == ERROR_MORE_DATA)
				{
					csTempDrive = GetFolder();
					csTempDrive.MakeLower();
					p = BufPtr;

					//
					// Loop through the entries;
					//  print retrieved data.
					//
					for (i = 1; i <= er; i++)
					{
						if (p->shi502_type == STYPE_DISKTREE)
						{
							csTemp = W2A((LPWSTR) p->shi502_path);
							csTemp.MakeLower();

							if (csTempDrive.Find(csTemp) == 0)
							{
								//	We found a match
								if (iBestMatch < csTemp.GetLength())
								{
									iBestMatch = csTemp.GetLength();
									csBestMatch = W2A((LPWSTR) p->shi502_netname);
								}
							}
						}

						p++;
					}

					//
					// Free the allocated buffer.
					//
					NetApiBufferFree(BufPtr);

					if (iBestMatch)
					{
						TCHAR tszComputerName[MAX_COMPUTERNAME_LENGTH + 1];
						DWORD dwBufLen = countof(tszComputerName);

						csTemp = GetFolder();
						csTemp = csTemp.Right(csTemp.GetLength() - iBestMatch + 1);
						GetComputerName(tszComputerName, &dwBufLen);
						csPath.Format(_T("\\\\%s\\%s%s%s%s"), tszComputerName, csBestMatch, csTemp, m_csFilename, m_csExtension);
					}
				}
				else 
					TRACE(_T("Error: %ld\n"), res);

				// Continue to call NetShareEnum while 
				//  there are more entries. 
				// 
			} while (res == ERROR_MORE_DATA); // end do
		}
	}

	return csPath;
}
