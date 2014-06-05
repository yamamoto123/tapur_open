// XWinVer.cpp  Version 1.1
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// Description:
//     XWinVer implements CXWinVersion, a singleton class to retrieve
//     the Windows OS version.  Please see article at 
//     www.codeproject.com.
//
// History
//     Version 1.1 - 2005 March 14
//     - Initial public release
//
// Public APIs:
//             NAME                            DESCRIPTION
//     ---------------------  -------------------------------------------------
//     GetMajorVersion()      Get major version number
//     GetMinorVersion()      Get minor version number
//     GetBuildNumber()       Get build number (ANDed with 0xFFFF for Win9x)
//     GetServicePackNT()     Get service pack number
//     GetServicePackString() Get service pack string
//     GetWinVersion()        Get windows version (CXWinVersion code)
//     GetWinVersionString()  Get windows version as string
//     IsWin2KorLater()       TRUE = Win2000 or later
//     IsWin2003()            TRUE = Win2003
//     IsXP()                 TRUE = XP
//     IsXPorLater()          TRUE = XP or later
//     IsXPHome()             TRUE = XP Home
//     IsXPPro()              TRUE = XP Pro
//     IsXPSP2()              TRUE = XP SP2
//
// License:
//     This software is released into the public domain.  You are free to use
//     it in any way you like, except that you may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#define XWINVER_CPP
#include "XWinVer.h"

// from winbase.h
#ifndef VER_PLATFORM_WIN32_WINDOWS
#define VER_PLATFORM_WIN32_WINDOWS      1
#endif
#ifndef VER_PLATFORM_WIN32_NT
#define VER_PLATFORM_WIN32_NT           2
#endif
#ifndef VER_PLATFORM_WIN32_CE
#define VER_PLATFORM_WIN32_CE           3
#endif
// from winnt.h
#ifndef VER_NT_WORKSTATION
#define VER_NT_WORKSTATION              0x0000001
#endif
#ifndef VER_SUITE_PERSONAL
#define VER_SUITE_PERSONAL              0x00000200
#endif


///////////////////////////////////////////////////////////////////////////////
/*
    This table has been assembled from Usenet postings, personal
    observations, and reading other people's code.  Please feel
    free to add to it or correct it.


         dwPlatFormID  dwMajorVersion  dwMinorVersion  dwBuildNumber
95             1              4               0             950
95 SP1         1              4               0        >950 && <=1080
95 OSR2        1              4             <10           >1080
98             1              4              10            1998
98 SP1         1              4              10       >1998 && <2183
98 SE          1              4              10          >=2183
ME             1              4              90            3000

NT 3.51        2              3              51            1057
NT 4           2              4               0            1381
2000           2              5               0            2195
XP             2              5               1            2600
2003           2              5               2            3790

CE 1.0         3              1               0
CE 2.0         3              2               0
CE 2.1         3              2               1
CE 3.0         3              3               0
*/

///////////////////////////////////////////////////////////////////////////////
//  per process data
BOOL CXWinVersion::m_bInitialized = FALSE;
CXWinVersion::OSVERSIONINFOEXX CXWinVersion::m_osinfo = { 0 };

///////////////////////////////////////////////////////////////////////////////
// Init
void CXWinVersion::Init()
{
	TRACE(_T("in CXWinVersion::Init\n"));

	ZeroMemory(&m_osinfo, sizeof(m_osinfo));

	m_osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (GetVersionEx((LPOSVERSIONINFO) &m_osinfo))
	{
		m_bInitialized = TRUE;

		if ((m_osinfo.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
			(m_osinfo.dwMajorVersion >= 5))
		{
			// get extended version info for 2000 and later

			ZeroMemory(&m_osinfo, sizeof(m_osinfo));

			m_osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

			GetVersionEx((LPOSVERSIONINFO) &m_osinfo);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// GetWinVersionString
CString CXWinVersion::GetWinVersionString()
{
	CString strVersion = WUNKNOWNSTR;

	int nVersion = GetWinVersion();

	switch (nVersion)
	{
		default:
		case WUNKNOWN:								break;
		case W95:		strVersion = W95STR;		break;
		case W95SP1:	strVersion = W95SP1STR;		break;
		case W95OSR2:	strVersion = W95OSR2STR;	break;
		case W98:		strVersion = W98STR;		break;
		case W98SP1:	strVersion = W98SP1STR;		break;
		case W98SE:		strVersion = W98SESTR;		break;
		case WME:		strVersion = WMESTR;		break;
		case WNT351:	strVersion = WNT351STR;		break;
		case WNT4:		strVersion = WNT4STR;		break;
		case W2K:		strVersion = W2KSTR;		break;
		case WXP:		strVersion = WXPSTR;		break;
		case W2003:		strVersion = W2003STR;		break;
		case WCE:		strVersion = WCESTR;		break;
	}

	return strVersion;
}

///////////////////////////////////////////////////////////////////////////////
// GetWinVersion
int CXWinVersion::GetWinVersion()
{
	int nVersion = WUNKNOWN;

	DWORD dwPlatformId   = m_osinfo.dwPlatformId;
	DWORD dwMinorVersion = m_osinfo.dwMinorVersion;
	DWORD dwMajorVersion = m_osinfo.dwMajorVersion;
	DWORD dwBuildNumber  = m_osinfo.dwBuildNumber & 0xFFFF;	// Win 9x needs this

	if ((dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (dwMajorVersion == 4))
	{
		if ((dwMinorVersion < 10) && (dwBuildNumber == 950))
		{
			nVersion = W95;
		}
		else if ((dwMinorVersion < 10) &&
				((dwBuildNumber > 950) && (dwBuildNumber <= 1080)))
		{
			nVersion = W95SP1;
		}
		else if ((dwMinorVersion < 10) && (dwBuildNumber > 1080))
		{
			nVersion = W95OSR2;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber == 1998))
		{
			nVersion = W98;
		}
		else if ((dwMinorVersion == 10) &&
				((dwBuildNumber > 1998) && (dwBuildNumber < 2183)))
		{
			nVersion = W98SP1;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber >= 2183))
		{
			nVersion = W98SE;
		}
		else if (dwMinorVersion == 90)
		{
			nVersion = WME;
		}
	}
	else if (dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if ((dwMajorVersion == 3) && (dwMinorVersion == 51))
		{
			nVersion = WNT351;
		}
		else if ((dwMajorVersion == 4) && (dwMinorVersion == 0))
		{
			nVersion = WNT4;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 0))
		{
			nVersion = W2K;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 1))
		{
			nVersion = WXP;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 2))
		{
			nVersion = W2003;
		}
	}
	else if (dwPlatformId == VER_PLATFORM_WIN32_CE)
	{
		nVersion = WCE;
	}

	return nVersion;
}

///////////////////////////////////////////////////////////////////////////////
// GetServicePackNT - returns a valid service pack number only for NT platform
int CXWinVersion::GetServicePackNT()
{
	int nServicePack = 0;

	for (int i = 0;
		 (m_osinfo.szCSDVersion[i] != _T('\0')) &&
			 (i < (sizeof(m_osinfo.szCSDVersion)/sizeof(TCHAR)));
		 i++)
	{
		if (_istdigit(m_osinfo.szCSDVersion[i]))
		{
			nServicePack = _ttoi(&m_osinfo.szCSDVersion[i]);
			break;
		}
	}

	return nServicePack;
}

///////////////////////////////////////////////////////////////////////////////
// IsXP
BOOL CXWinVersion::IsXP()
{
	if (GetWinVersion() == WXP)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// IsXPHome
BOOL CXWinVersion::IsXPHome()
{
	if (GetWinVersion() == WXP)
	{
		if (m_osinfo.wSuiteMask & VER_SUITE_PERSONAL)
			return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// IsXPPro
BOOL CXWinVersion::IsXPPro()
{
	if (GetWinVersion() == WXP)
	{
		if ((m_osinfo.wProductType == VER_NT_WORKSTATION) && !IsXPHome())
			return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// IsXPSP2
BOOL CXWinVersion::IsXPSP2()
{
	if (GetWinVersion() == WXP)
	{
		if (GetServicePackNT() == 2)
			return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// IsWin2003
BOOL CXWinVersion::IsWin2003()
{
	if ((m_osinfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && 
		(m_osinfo.dwMajorVersion == 5) &&
		(m_osinfo.dwMinorVersion == 2))
		return TRUE;

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// IsWin2KorLater
BOOL CXWinVersion::IsWin2KorLater()
{
	if ((m_osinfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && 
		(m_osinfo.dwMajorVersion >= 5))
		return TRUE;

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// IsXPorLater
BOOL CXWinVersion::IsXPorLater()
{
	if ((m_osinfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && 
		(((m_osinfo.dwMajorVersion == 5) && (m_osinfo.dwMinorVersion > 0)) || 
		(m_osinfo.dwMajorVersion > 5)))
		return TRUE;

	return FALSE;
}
