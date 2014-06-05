/////////////////////////////////////////////////////////////////////////////
//
//   Filename: FontStatic.h
//  Classname: CFontStatic
//
// Written by: Patrik Svensson (patrik.svensson@home.se)
//
//             This class can be used by anyone for any purpose, 
//             but if you like it, then send me a mail and tell me so ;)
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONTSTATIC_H__2125167D_A024_4688_89DC_47BD19919DAA__INCLUDED_)
#define AFX_FONTSTATIC_H__2125167D_A024_4688_89DC_47BD19919DAA__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif

// Defines for the font style
#define FS_NORMAL			0x00
#define FS_BOLD				0x01
#define FS_ITALIC			0x02
#define FS_UNDERLINED		0x04
#define FS_STRIKETHROUGH	0x08
#define FS_ANTIALIAS		0x10

// Defines for horizontal alignment
#define FS_CENTER			0x20
#define FS_LEFT				0x40
#define FS_RIGHT			0x80

// PAAK
// Defines for vertical alignment
#define FS_VCENTER			0x100
#define FS_TOP				0x200
#define FS_BOTTOM			0x400

/////////////////////////////////////////////////////////////////////////////

class CFontStatic : public CStatic
{

// Construction
public:
	CFontStatic();

// Attributes
public:

	bool m_bBold;
	bool m_bItalic;
	bool m_bUnderlined;
	bool m_bStrikethrough;
	bool m_bAntialias;
	bool m_bBgColor;

	bool m_bCenter;
	bool m_bLeft;
	bool m_bRight;

// PAAK
	bool m_bVCenter;
	bool m_bTop;
	bool m_bBottom;

	CString m_szFont;
	DWORD m_dwColor;
	DWORD m_dwBgColor;
	int m_nSize;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetFontStyle(DWORD dwStyle);
	void SetBackground(DWORD dwBgColor);
	void SetFontStatic(CString szFont = _T("MS Sans Serif"), 
		int nSize = 8, DWORD dwColor = RGB(0,0,0), DWORD dwStyle = FS_LEFT | FS_TOP);
	virtual ~CFontStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFontStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTSTATIC_H__2125167D_A024_4688_89DC_47BD19919DAA__INCLUDED_)
