/*
 * Copyright (c) 2001 Paolo Messina and Jerzy Kaczorowski
 * 
 * The contents of this file are subject to the Artistic License (the "License").
 * You may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at:
 * http://www.opensource.org/licenses/artistic-license.html
 * 
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF 
 * MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * 
 */

#if !defined(AFX_ODDBUTTON_H__4CA1E42E_E3C3_4FEA_99A1_E865DEB500DA__INCLUDED_)
#define AFX_ODDBUTTON_H__4CA1E42E_E3C3_4FEA_99A1_E865DEB500DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OddButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COddButton window

class COddButton : public CButton
{
// Construction
public:
	COddButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COddButton)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COddButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(COddButton)
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	afx_msg LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bCanBeDefault;
	BOOL m_bDefault;
	UINT m_nTypeStyle;

protected:
	// used to enable default state handling
	void EnableDefault(BOOL bEnable);

	// use to know when the control has default state
	BOOL IsDefault();

	// use to know the type of control to draw
	UINT GetControlType();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODDBUTTON_H__4CA1E42E_E3C3_4FEA_99A1_E865DEB500DA__INCLUDED_)
