/*
 *	$Header: /common/FileSpec.h 2     12/04/01 11:40p Admin $
 *
 *	$History: FileSpec.h $
 * 
 * *****************  Version 4  *****************
 * User: PAAK        Date: 12/04/05
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

	enum FS_BUILTINS
	{
		FS_EMPTY,		//	Nothing
		FS_APP,			//  Full application path and name
		FS_APPDIR,		//	Application folder
		FS_WINDIR,		//	Windows folder
		FS_SYSDIR,		//	System folder
		FS_TMPDIR,		//	Temporary folder
		FS_DESKTOP,		//	Desktop folder
		FS_FAVOURITES,	//	Favourites folder
		FS_PROGRAMS,	//	プログラムメニュー
		FS_PERSONAL,	//	User Folder
		FS_MYDOC,		//	My Document
		FS_PROGRAM_FILES,	//	Program Files
		FS_STARTUP,		//  Startup
		FS_MEDIA,		//	Default media folder
		FS_CURRDIR,		//	Current folder
		FS_TEMPNAME		//	Create a temporary name
	};

class CFileSpec
{
public:

	CFileSpec(FS_BUILTINS eSpec = FS_EMPTY);
	CFileSpec(FS_BUILTINS eSpec, LPCTSTR szFileame);
	CFileSpec(LPCTSTR szSpec, LPCTSTR szFilename);
	CFileSpec(LPCTSTR szFilename);

//	Operations
	BOOL			IsFolder();
	BOOL			Exist();
	BOOL			IsUNCPath();
	BOOL			LoadArchive(CObject *pObj);
	BOOL			SaveArchive(CObject *pObj);
	BOOL			LoadString(CString *pStr);
	BOOL			SaveString(CString strTxt, BOOL blAdd = FALSE, BOOL blUnicode = FALSE);
	BOOL			MkFolder();
	BOOL			ClearFolder();
	BOOL			FileCopy(LPCTSTR newFilename, BOOL blOverwrite, BOOL blCancelEnable = FALSE, HWND hwnd = NULL);
//	BOOL			FastFileCopy(LPCTSTR newFilename, BOOL blOverwrite);
	BOOL			FileMove(LPCTSTR newFilename, BOOL blOverwrite = TRUE, BOOL blWait = TRUE);
	BOOL			FileDelete(BOOL blCancelEnable = FALSE, HWND hwnd = NULL);
	ULONGLONG		FileSize();
	ULONGLONG		FreeDisk();

//	Access functions
	CString&		Drive()				{ return m_csDrive; }
	CString&		Path()				{ return m_csPath; }
	CString&		FileName()			{ return m_csFilename; }
	CString&		Extension()			{ return m_csExtension; }
	CString	FullPathNoExtension();
	CString	GetFolder();
	CString	GetFullSpec();
	CString	GetFileName();
	CString	ConvertToUNCPath();

	void			SetFullSpec(LPCTSTR szSpec);
	void			SetFullSpec(FS_BUILTINS eSpec = FS_EMPTY);
	void			SetFileName(LPCTSTR szSpec);

	void			Initialise(FS_BUILTINS eSpec);

private:
	BOOL			IsUNCPath(LPCTSTR szPath);
	void			WriteAble();
	void			ReadOnly();
	void			GetShellFolder(int iFolder);

	CString			m_csDrive,
					m_csPath,
					m_csFilename,
					m_csExtension;
};
