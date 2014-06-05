#if !defined(AFX_FolderForm_H__1400D4B6_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_)
#define AFX_FolderForm_H__1400D4B6_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderForm.h : header file
//

#include "VisualFx.h"

/////////////////////////////////////////////////////////////////////////////
// CFolderForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFolderForm : public TVisualFormView
{
protected:
	CFolderForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFolderForm)

// Form Data
public:
	//{{AFX_DATA(CFolderForm)
	enum { IDD = IDD_FOLDERFORM };
	//}}AFX_DATA

// Attributes
private:
	CBrush m_brush;		// îwåiêFÇÃÉuÉâÉV

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFolderForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFolderForm)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FolderForm_H__1400D4B6_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_)
