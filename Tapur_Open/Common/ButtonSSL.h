#if !defined(AFX_CBUTTONSSL_H__29DF048F_8196_11D5_9EFE_000102FB9990__INCLUDED_)
#define AFX_CBUTTONSSL_H__29DF048F_8196_11D5_9EFE_000102FB9990__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CButtonSSL.h : header file
//
// SSL Button Styles
#define SSL_BS_FLAT			0x00000001	// Flat button.
#define SSL_BS_MENU_BTN		0x00000002	// Button with drop-dwon arrow for menu
#define SSL_BS_AUTOSIZE		0x00000004	// Button sizes to fit contents
// SSL Text Alignment Styles (Same as CDC::DrawText)
#define SSL_TEXT_TOP		0x00000000
#define SSL_TEXT_LEFT       0x00000000
#define SSL_TEXT_CENTER     0x00000001
#define SSL_TEXT_RIGHT      0x00000002
#define SSL_TEXT_VCENTER    0x00000004
#define SSL_TEXT_BOTTOM     0x00000008
// SSL Image Alignment Styles
#define SSL_IMAGE_TOP		0x00000000
#define SSL_IMAGE_LEFT		0x00000000
#define SSL_IMAGE_CENTER	0x00000001
#define SSL_IMAGE_RIGHT		0x00000002
#define SSL_IMAGE_VCENTER	0x00000004
#define SSL_IMAGE_BOTTOM	0x00000008
// SSL Default Mask
#define SSL_MASK	RGB (255, 0, 255)
// Menu Item does not exist
#define MF_DOES_NOT_EXIST	0xFFFFFFFF

/////////////////////////////////////////////////////////////////////////////
// CButtonSSL window
#include "OddButton.h"

class CButtonSSL : public COddButton {
public:
// Construction
	CButtonSSL();
// Destruction
	virtual ~CButtonSSL();

// Operations
public:
// Initialisation
	void SetSSLButtonStyle (UINT nStyle);
	UINT GetSSLButtonStyle ();
// Image Functions
	BOOL SetSSLButtonBitmap (UINT nResourceID, COLORREF crMask = SSL_MASK);
	BOOL SetSSLButtonBitmaps (UINT nUpID, COLORREF crUpMask = SSL_MASK, UINT nOverID = 0, COLORREF crOverMask = SSL_MASK,
						      UINT nDownID = 0, COLORREF crDownMask = SSL_MASK, 
						      UINT nDisabledID = 0, COLORREF crDisabledMask = SSL_MASK);
	BOOL SetSSLButtonIcon (UINT nResourceID);
	BOOL SetSSLButtonIcons (UINT nUpID, UINT nOverID = 0, UINT nDownID = 0, UINT nDisabledID = 0);
	void SetSSLImageAlign (UINT nImageAlign);
	UINT GetSSLImageAlign ();
// Text Functions
	void SetSSLTextAlign (UINT nTextAlign);
	UINT GetSSLTextAlign ();
	BOOL SetSSLButtonFont (LPCTSTR lpszFaceName, int nSizePoints = 8, BOOL bUnderline = FALSE,
						   BOOL bBold = FALSE, BOOL bStrikeOut = FALSE, BOOL bItalic = FALSE);
	BOOL SetSSLButtonFont (CFont& font);
// Colour Functions
	void SetSSLDefaultColors (BOOL bRedraw = TRUE);
	BOOL SetSSLColor (BYTE byColorIndex, COLORREF crColor, BOOL bRedraw = TRUE);
	BOOL GetSSLColor (BYTE byColorIndex, COLORREF* pcrColor);
// Check Box Functions
	void SetSSLCheck (int nCheck, BOOL bRedraw = TRUE);
	int	 GetSSLCheck ();
// Menu Functions
	BOOL SetSSLButtonMenu (UINT nResourceID);
	BOOL SetSSLButtonMenu (LPCTSTR lpszResourceName);
	UINT CheckSSLMenuItem (UINT nIDCheckItem, UINT nCheck = MF_CHECKED);
    BOOL CheckSSLMenuRadioItem (UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags);
    UINT EnableSSLMenuItem (UINT nIDEnableItem, UINT nEnable);
    UINT GetSSLMenuItemCount () const;
    UINT GetSSLMenuItemID (int nPos) const;
	UINT GetSSLMenuState (UINT nID, UINT nFlags);
    int  GetSSLMenuString (UINT nIDItem, LPTSTR lpString, int nMaxCount, UINT nFlags);
    int  GetSSLMenuString (UINT nIDItem, CString& rString, UINT nFlags);
    BOOL GetSSLMenuItemInfo (UINT nIDItem, LPMENUITEMINFO lpMenuItemInfo, BOOL ByPos = FALSE);
    BOOL ModifySSLMenu (UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
    BOOL ModifySSLMenu (UINT nPosition, UINT nFlags, UINT nIDNewItem, const CBitmap* pBmp);
    BOOL RemoveSSLMenu (UINT nPosition, UINT nFlags);
    BOOL SetSSLMenuItemBitmaps (UINT nPosition, UINT nFlags, const CBitmap* pBmpUnchecked,
                                const CBitmap* pBmpChecked);
// Miscellaneous Functions
	BOOL SetSSLButtonCursor (UINT nResourceID);
	void SetSSLButtonToolTip (LPCTSTR lpszTipText, BOOL bActivate = TRUE);
	void SetSSLButtonToolTip (UINT nResourceID, BOOL bActivate = TRUE);
	void GetSSLButtonToolTip (CString& strTipText);
	void SetSSLButtonURL (LPCTSTR lpszURL);

// Colour Enumeration for button and text colours
	enum {
		SSL_UP_BK_COLOR = 0,	// Background colour for SSL_STATE_UP
		SSL_UP_TEXT_COLOR,		// Text colour for SSL_STATE_UP
		SSL_OVER_BK_COLOR,		// Background colour for SSL_STATE_OVER
		SSL_OVER_TEXT_COLOR,	// Text colour for SSL_STATE_OVER
		SSL_DOWN_BK_COLOR,		// Background colour for SSL_STATE_DOWN
		SSL_DOWN_TEXT_COLOR,	// Text colour for SSL_STATE_DOWN
		SSL_MAX_COLORS			// Maximum number of colours
	};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonSSL)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	// SSL Enumeration for button states (also acts as index into image list)
	enum ButtonState {
		SSL_STATE_UP,
		SSL_STATE_OVER,
		SSL_STATE_DOWN,
		SSL_STATE_DISABLED
	};
	void DrawArrow (CDC* pDC, CPoint ptTip);
	BOOL HitMenuButton (CPoint pt);
	void InitToolTipCtrl ();
	void AddBitmap (UINT nResourceID, COLORREF crMask);
	void AddIcon (UINT nResourceID);
	CPoint GetImagePoint (CRect btnRect);
	void SetSize ();

	CMenu			m_menu;
	CToolTipCtrl	m_ToolTip;
	TCHAR			m_szURL[MAX_PATH];
	HCURSOR			m_hCursor;
	COLORREF		m_crColors[SSL_MAX_COLORS];
	CFont			m_font;

	CBitmap		m_bmpImage;
	BITMAP		m_bmp;
	CImageList	m_imageList;
	HICON		m_hIcon;
	int			m_nImageWidth;
	int			m_nImageHeight;

	BOOL		m_bImageLoaded;
	BOOL		m_bMenuLoaded;
	BOOL		m_bMenuPushed;
	BOOL		m_bBtnPushed;
	BOOL		m_bMouseOnBtn;
	BOOL		m_bDisabled;

	UINT		m_nStyle;
	UINT		m_nTextAlign;
	UINT		m_nImageAlign;

	int			m_nCheck;
	ButtonState	m_bsState;
	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonSSL)
	afx_msg BOOL OnClicked();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg LRESULT OnGetCheck ();
	afx_msg LRESULT OnSetCheck (WPARAM fCheck);
	afx_msg HBRUSH CtlColor (CDC* pDC, UINT nCtlColor);
	virtual void DrawBackground (CDC* pDC, LPCRECT pRect);
	virtual void DrawBorder (CDC* pDC, LPRECT pRect);

	DECLARE_MESSAGE_MAP()
private:
	void TraceState (ButtonState state);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBUTTONSSL_H__29DF048F_8196_11D5_9EFE_000102FB9990__INCLUDED_)
