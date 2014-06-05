#if !defined(AFX_RsfSimpleForm_H__1400D4B6_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_)
#define AFX_RsfSimpleForm_H__1400D4B6_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfSimpleForm.h : header file
//

#include "VisualFx.h"
#include "Label.h"

/////////////////////////////////////////////////////////////////////////////
// CRsfSimpleForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CRsfSimpleForm : 
  public TVisualFormView
{
protected:
	CRsfSimpleForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRsfSimpleForm)

// Form Data
public:
	//{{AFX_DATA(CRsfSimpleForm)
	enum { IDD = IDD_RSF_SIMPLEFORM };
	BOOL	m_blAudio;
    BOOL	m_blChat;
    BOOL	m_blVideo;
    BOOL	m_blImg;
    BOOL	m_blBeep;
    BOOL	m_blPrg;
	CLabel	m_lblAudioSize;
	CLabel	m_lblChatSize;
	CLabel	m_lblVideoSize;
	CLabel	m_lblImgSize;
	int		m_idxPrg;
	//}}AFX_DATA

// Attributes
private:
	CBrush m_brush;			// 背景色のブラシ
	CString m_strSenderId;	// 選択中の応答設定送信者
	CStringArray m_aryPrgId;	// 連携プログラムIDリスト

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRsfSimpleForm)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRsfSimpleForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CRsfSimpleForm)
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChgForm();
	afx_msg void OnButtonAudio();
	afx_msg void OnButtonChat();
	afx_msg void OnButtonVideo();
	afx_msg void OnButtonImage();
	afx_msg void OnButtonPrg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL UpdateFile();	// RSFファイルの更新
	BOOL UpdateForm();	// 画面の更新
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RsfSimpleForm_H__1400D4B6_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_)
