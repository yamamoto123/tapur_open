// XWinVer.h  Version 1.1
//
// Author: Hans Dietrich
//         hdietrich@gmail.com
//
// This software is released into the public domain.  You are free to use
// it in any way you like, except that you may not sell this source code.
//
// This software is provided "as is" with no expressed or implied warranty.
// I accept no liability for any damage or loss of business that this
// software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XWINVER_H
#define XWINVER_H

///////////////////////////////////////////////////////////////////////////////
//
// version strings
//
#define WUNKNOWNSTR	_T("unknown Windows version")

#define W95STR		_T("Windows 95")
#define W95SP1STR	_T("Windows 95 SP1")
#define W95OSR2STR	_T("Windows 95 OSR2")
#define W98STR		_T("Windows 98")
#define W98SP1STR	_T("Windows 98 SP1")
#define W98SESTR	_T("Windows 98 SE")
#define WMESTR		_T("Windows ME")

#define WNT351STR	_T("Windows NT 3.51")
#define WNT4STR		_T("Windows NT 4")
#define W2KSTR		_T("Windows 2000")
#define WXPSTR		_T("Windows XP")
#define W2003STR	_T("Windows Server 2003")

#define WCESTR		_T("Windows CE")


///////////////////////////////////////////////////////////////////////////////
//
// version integer values
//
#define WUNKNOWN	0

#define W9XFIRST	1
#define W95			1
#define W95SP1		2
#define W95OSR2		3
#define W98			4
#define W98SP1		5
#define W98SE		6
#define WME			7
#define W9XLAST		99

#define WNTFIRST	101
#define WNT351		101
#define WNT4		102
#define W2K			103
#define WXP			104
#define W2003		105
#define WNTLAST		199

#define WCEFIRST	201
#define WCE			201
#define WCELAST		299


///////////////////////////////////////////////////////////////////////////////
//
// CXWinVersion singleton class
//
class CXWinVersion
{
// Construction
public:
	CXWinVersion()
	{
		TRACE(_T("in CXWinVersion\n"));
		if (!m_bInitialized)
		{
			Init();
		}
		else
		{
			TRACE(_T("already initialized\n"));
		}
	}

// Attributes
public:
	int		GetMajorVersion()		{ return m_osinfo.dwMajorVersion; }
	int		GetMinorVersion()		{ return m_osinfo.dwMinorVersion; }
	int		GetBuildNumber()		{ return (m_osinfo.dwBuildNumber & 0xFFFF); }	// needed for 9x
	int		GetServicePackNT();
	CString	GetServicePackString()	{ return m_osinfo.szCSDVersion; }
	int		GetWinVersion();
	CString	GetWinVersionString();
	BOOL	IsWin2KorLater();
	BOOL	IsWin2003();
	BOOL	IsXP();
	BOOL	IsXPorLater();
	BOOL	IsXPHome();
	BOOL	IsXPPro();
	BOOL	IsXPSP2();


// Implementation
private:
	void Init();

	struct OSVERSIONINFOEXX
	{  
		DWORD dwOSVersionInfoSize;  
		DWORD dwMajorVersion;  
		DWORD dwMinorVersion;  
		DWORD dwBuildNumber;  
		DWORD dwPlatformId;  
		TCHAR szCSDVersion[128];  
		WORD  wServicePackMajor;  
		WORD  wServicePackMinor;  
		WORD  wSuiteMask;  
		BYTE  wProductType;  
		BYTE  wReserved;
	};

	static BOOL m_bInitialized;
	static OSVERSIONINFOEXX m_osinfo;
};


///////////////////////////////////////////////////////////////////////////////
//
// CXWinVersion instance
//
#ifndef XWINVER_CPP
// include an instance in each file;  the namespace insures uniqueness
namespace { CXWinVersion WinVersion; }
#endif


#endif //XWINVER_H
