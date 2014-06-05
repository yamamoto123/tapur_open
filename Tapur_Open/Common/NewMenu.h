//------------------------------------------------------------------------------
// File    : NewMenu.h 
// Version : 1.25
// Date    : 25. June 2005
// Author  : Bruno Podetti
// Email   : podetti@gmx.net
// Web     : www.podetti.com/NewMenu 
// Systems : VC6.0/7.0 and VC7.1 (Run under (Window 98/ME), Windows Nt 2000/XP)
//           for all systems it will be the best when you install the latest IE
//           it is recommended for CNewToolBar
//
// You are free to use/modify this code but leave this header intact.
// This class is public domain so you are free to use it any of your 
// applications (Freeware, Shareware, Commercial). 
// All I ask is that you let me know so that if you have a real winner I can
// brag to my buddies that some of my code is in your app. I also wouldn't 
// mind if you sent me a copy of your application since I like to play with
// new stuff.
//------------------------------------------------------------------------------
#ifndef __CNewMenu_H_
#define __CNewMenu_H_

// new constant for having the same color in the toolbar, you can define this
// also in the projekt definition
//#define USE_NEW_DOCK_BAR

// for using my CNewMenu in the GuiToolkit see
// http://www.codeproject.com/library/guitoolkit.asp
// you need to #define _GUILIB_   in the ExtLib.h  and include this file
// the constant for exporting importing these classes can be changed in the 
// next release GUILIBDLLEXPORT!!!

#ifdef NEW_MENU_DLL
  #ifdef NEW_MENU_DLL_EXPORT
    #define GUILIBDLLEXPORT __declspec(dllexport)
  #else
    #define GUILIBDLLEXPORT __declspec(dllimport)

#if _MSC_VER >= 1300
    // older compiler have a bug. static template member were not exported corectly
    // so we need this
    #define _GUILIB_
#endif

  #endif
#endif

#ifndef GUILIBDLLEXPORT
#define GUILIBDLLEXPORT
#endif

#pragma warning(push)
#pragma warning(disable : 4211)

#include <afxtempl.h>
#include <afxpriv.h>

// Flagdefinitions 
#define MFT_TITLE       0x0001
#define MFT_TOP_TITLE   0x0000
#define MFT_SIDE_TITLE  0x0002
#define MFT_GRADIENT    0x0004
#define MFT_SUNKEN      0x0008
#define MFT_LINE        0x0010
#define MFT_ROUND       0x0020
#define MFT_CENTER      0x0040

// Typedefinition for compatibility to MFC 7.0
#ifndef DWORD_PTR
typedef DWORD DWORD_PTR, *PDWORD_PTR;
#endif

#ifndef ULONG_PTR
typedef unsigned long ULONG_PTR, *PULONG_PTR;
#endif

#ifndef LONG_PTR
typedef long LONG_PTR, *PLONG_PTR;
#endif

// Additional flagdefinition for highlighting
#ifndef ODS_HOTLIGHT
#define ODS_HOTLIGHT        0x0040
#endif

#ifndef ODS_INACTIVE
#define ODS_INACTIVE        0x0080
#endif

// Typedefinition for old compatibility lesser than MFC 8.0
#if _MFC_VER < 0x0800  
#define _tcscpy_s(strDestination,sizeInTchar,strSource )  _tcscpy(strDestination,strSource)
#define _tcsncpy_s(strDestination,sizeInTchar,strSource,sizeCopyInTchar) _tcsncpy(strDestination,strSource,sizeCopyInTchar)
#endif


// new define menustyles
#define ODS_SELECTED_OPEN   0x0800
#define ODS_RIGHTJUSTIFY    0x1000
#define ODS_WRAP            0x2000
#define ODS_HIDDEN          0x4000
#define ODS_DRAW_VERTICAL   0x8000


/////////////////////////////////////////////////////////////////////////////
// Constans for detecting OS-Type
enum Win32Type
{
  Win32s,
  WinNT3,
  Win95,
  Win98,
  WinME,
  WinNT4,
  Win2000,
  WinXP
};

Win32Type GUILIBDLLEXPORT IsShellType();

extern const Win32Type GUILIBDLLEXPORT g_Shell;
extern BOOL GUILIBDLLEXPORT bRemoteSession;
BOOL GUILIBDLLEXPORT IsMenuThemeActive();

/////////////////////////////////////////////////////////////////////////////
// Support for getting menuinfo without runtime-error

#if(WINVER < 0x0500)

#define MNS_NOCHECK         0x80000000
#define MNS_MODELESS        0x40000000
#define MNS_DRAGDROP        0x20000000
#define MNS_AUTODISMISS     0x10000000
#define MNS_NOTIFYBYPOS     0x08000000
#define MNS_CHECKORBMP      0x04000000

#define MIM_MAXHEIGHT               0x00000001
#define MIM_BACKGROUND              0x00000002
#define MIM_HELPID                  0x00000004
#define MIM_MENUDATA                0x00000008
#define MIM_STYLE                   0x00000010
#define MIM_APPLYTOSUBMENUS         0x80000000


typedef struct tagMENUINFO
{
  DWORD   cbSize;
  DWORD   fMask;
  DWORD   dwStyle;
  UINT    cyMax;
  HBRUSH  hbrBack;
  DWORD   dwContextHelpID;
  ULONG_PTR dwMenuData;
}   MENUINFO, FAR *LPMENUINFO;
typedef MENUINFO CONST FAR *LPCMENUINFO;

BOOL GUILIBDLLEXPORT GetMenuInfo( HMENU hMenu, LPMENUINFO pInfo);
BOOL GUILIBDLLEXPORT SetMenuInfo( HMENU hMenu, LPCMENUINFO pInfo);

#define WS_EX_LAYOUTRTL         0x00400000L // Right to left mirroring

#define LAYOUT_RTL              0x00000001  // Right to left

DWORD GUILIBDLLEXPORT GetLayout(HDC hDC);
DWORD GUILIBDLLEXPORT SetLayout(HDC hDC, DWORD dwLayout);

#endif

inline bool IsMirroredHdc(HDC hDC)
{
  return (GetLayout(hDC)&LAYOUT_RTL)?true:false;
}

inline bool IsMirroredWnd(HWND hWnd)
{
  return (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL)?true:false;
}

CRect GUILIBDLLEXPORT GetScreenRectFromWnd(HWND hWnd);
CRect GUILIBDLLEXPORT GetScreenRectFromRect(LPCRECT pRect);
BOOL GUILIBDLLEXPORT TrackPopupMenuSpecial(CMenu *pMenu, UINT uFlags, CRect rcTBItem, CWnd* pWndCommand, BOOL bOnSide);

/////////////////////////////////////////////////////////////////////////////
// Forwarddeclaration and global function for menu drawing
void GUILIBDLLEXPORT DrawGradient(CDC* pDC,CRect& Rect,
                                  COLORREF StartColor,COLORREF EndColor, 
                                  BOOL bHorizontal,BOOL bUseSolid=FALSE);

void GUILIBDLLEXPORT MenuDrawText(HDC hDC ,LPCTSTR lpString,int nCount,LPRECT lpRect,UINT uFormat);


COLORREF GUILIBDLLEXPORT DarkenColorXP(COLORREF color);
COLORREF GUILIBDLLEXPORT DarkenColor( long lScale, COLORREF lColor);

COLORREF GUILIBDLLEXPORT MixedColor(COLORREF colorA,COLORREF colorB);
COLORREF GUILIBDLLEXPORT MidColor(COLORREF colorA,COLORREF colorB);

COLORREF GUILIBDLLEXPORT LightenColor( long lScale, COLORREF lColor);

COLORREF GUILIBDLLEXPORT BleachColor(int Add, COLORREF color);
COLORREF GUILIBDLLEXPORT GetAlphaBlendColor(COLORREF blendColor, COLORREF pixelColor,int weighting);

COLORREF GUILIBDLLEXPORT GetXpHighlightColor();
COLORREF GUILIBDLLEXPORT GrayColor(COLORREF crColor); 

/////////////////////////////////////////////////////////////////////////////
// Global helperfunctions
UINT GUILIBDLLEXPORT GetSafeTimerID(HWND hWnd, const UINT uiElapse);

BOOL GUILIBDLLEXPORT DrawMenubarItem(CWnd* pWnd,CMenu* pMenu, UINT nItemIndex,UINT nState);
void GUILIBDLLEXPORT UpdateMenuBarColor(HMENU hMenu=NULL);
CBrush GUILIBDLLEXPORT *GetMenuBarBrush();

int GUILIBDLLEXPORT NumScreenColors();
WORD GUILIBDLLEXPORT NumBitmapColors(LPBITMAPINFOHEADER lpBitmap);
HBITMAP GUILIBDLLEXPORT LoadColorBitmap(LPCTSTR lpszResourceName, HMODULE hInst, int* pNumcol=NULL);

COLORREF GUILIBDLLEXPORT MakeGrayAlphablend(CBitmap* pBitmap, int weighting, COLORREF blendcolor);
HBITMAP GUILIBDLLEXPORT CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent);

/////////////////////////////////////////////////////////////////////////////
// Forwarddeclaration for drawing purpose
class CNewMenu;
class CMenuTheme;
class CNewDockBar;

/////////////////////////////////////////////////////////////////////////////
// CMenuTheme for drawing border and the rest

typedef void (CNewMenu::*pItemMeasureFkt) (LPMEASUREITEMSTRUCT lpMIS, BOOL bIsMenubar);
typedef void (CNewMenu::*pItemDrawFkt) (LPDRAWITEMSTRUCT lpDIS, BOOL bIsMenubar);
typedef BOOL (CNewMenu::*pEraseBkgndFkt) (HWND hWnd, HDC hDC);

typedef void (CMenuTheme::*pDrawMenuBorder)( HWND hWnd, HDC hDC, CPoint screen);

class GUILIBDLLEXPORT CMenuTheme
{
public:
  CMenuTheme();

  CMenuTheme(DWORD dwThemeId,
    pItemMeasureFkt pMeasureItem,
    pItemDrawFkt pDrawItem,
    pItemDrawFkt pDrawTitle,
    DWORD dwFlags=0);

  virtual ~CMenuTheme();
  virtual BOOL OnInitWnd(HWND hWnd);
  virtual BOOL OnUnInitWnd(HWND hWnd);

  virtual BOOL DoDrawBorder();

  virtual void UpdateSysColors();
  virtual void UpdateSysMetrics();

  virtual BOOL OnDrawBorder(HWND hWnd, HDC hDC, BOOL bOnlyBorder=FALSE);
  virtual BOOL OnEraseBkgnd(HWND hWnd, HDC hDC);
  virtual BOOL OnNcCalcSize(HWND hWnd, NCCALCSIZE_PARAMS* pCalc);
  virtual BOOL OnWindowPosChanging(HWND hWnd, LPWINDOWPOS pPos);
  virtual BOOL OnCalcFrameRect(HWND hWnd,LPRECT pRect);

  virtual void DrawGripper( CDC* pDC, const CRect& rect, DWORD dwStyle, 	
                            int m_cxLeftBorder, int m_cxRightBorder,
                            int m_cyTopBorder, int m_cyBottomBorder);

  virtual void DrawCorner ( CDC* pDC, LPCRECT pRect, DWORD dwStyle);

  void PaintCorner(CDC *pDC, LPCRECT pRect, COLORREF color);

  virtual void GetBarColor(COLORREF &clrUpperColor, COLORREF &clrMediumColor, COLORREF &clrBottomColor, COLORREF &clrDarkLine);

  void DrawShade( HWND hWnd, HDC hDC, CPoint screen);
  void DrawSmalBorder( HWND hWnd, HDC hDC);
  void DrawFrame(CDC* pDC, CRect rectOuter, CRect rectInner, COLORREF crBorder);

  void* SetScreenBitmap(HWND hWnd, HDC hDC);

public:
  DWORD m_dwThemeId;
  DWORD m_dwFlags;

  pItemMeasureFkt m_pMeasureItem;
  pItemDrawFkt m_pDrawItem;
  pItemDrawFkt m_pDrawTitle;

  CSize m_BorderTopLeft;
  CSize m_BorderBottomRight;

	// color values of system colors used for CToolBar
	COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite;
	COLORREF clrBtnText, clrWindowFrame;
};

/////////////////////////////////////////////////////////////////////////////
// CMenuThemeXP for drawing border and the rest

class GUILIBDLLEXPORT CMenuThemeXP :public CMenuTheme
{
public:
  CMenuThemeXP(DWORD dwThemeId,
    pItemMeasureFkt pMeasureItem,
    pItemDrawFkt pDrawItem,
    pItemDrawFkt pDrawTitle,
    DWORD dwFlags=0);

  virtual BOOL OnDrawBorder(HWND hWnd, HDC hDC, BOOL bOnlyBorder=FALSE);
  virtual BOOL OnEraseBkgnd(HWND hWnd, HDC hDC);
};

/////////////////////////////////////////////////////////////////////////////
// CMenuTheme2003 for drawing border and the rest
class GUILIBDLLEXPORT CMenuTheme2003 :public CMenuThemeXP
{
public:
  CMenuTheme2003(DWORD dwThemeId,
    pItemMeasureFkt pMeasureItem,
    pItemDrawFkt pDrawItem,
    pItemDrawFkt pDrawTitle,
    DWORD dwFlags=0);
};

/////////////////////////////////////////////////////////////////////////////
// CNewMenuIcons menu icons for drawing

class GUILIBDLLEXPORT CNewMenuIcons : public CObject
{
  DECLARE_DYNCREATE(CNewMenuIcons)

public:
  CNewMenuIcons();
  virtual ~CNewMenuIcons();

public:
  BOOL GetIconSize(int* cx, int* cy);
  CSize GetIconSize();

  virtual int FindIndex(UINT nID);
  virtual void OnSysColorChange();

  virtual BOOL LoadToolBar(LPCTSTR lpszResourceName, HMODULE hInst);
  virtual BOOL LoadToolBar(WORD* pToolInfo, COLORREF crTransparent=CLR_NONE);
  virtual BOOL LoadToolBar(HBITMAP hBitmap, CSize size, UINT* pID, COLORREF crTransparent=CLR_NONE);

  virtual BOOL DoMatch(LPCTSTR lpszResourceName, HMODULE hInst);
  virtual BOOL DoMatch(WORD* pToolInfo, COLORREF crTransparent=CLR_NONE);
  virtual BOOL DoMatch(HBITMAP hBitmap, CSize size, UINT* pID);

  virtual BOOL LoadBitmap(int nWidth, int nHeight, LPCTSTR lpszResourceName, HMODULE hInst=NULL);

  //  virtual BOOL SetBlendImage();
  virtual int AddGloomIcon(HICON hIcon, int nIndex=-1);
  virtual int AddGrayIcon(HICON hIcon, int nIndex=-1);
  virtual BOOL MakeImages();

  void SetResourceName(LPCTSTR lpszResourceName);

  int AddRef();
  int Release();

#if defined(_DEBUG) || defined(_AFXDLL)
  // Diagnostic Support
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

public:
  LPCTSTR m_lpszResourceName;
  HMODULE m_hInst;
  HBITMAP m_hBitmap;
  int m_nColors;
  COLORREF m_crTransparent;

  CImageList m_IconsList;

  CArray<UINT,UINT&> m_IDs;
  DWORD m_dwRefCount;
};

/////////////////////////////////////////////////////////////////////////////
// CNewMenuBitmaps menu icons for drawing
class GUILIBDLLEXPORT CNewMenuBitmaps : public CNewMenuIcons
{
  DECLARE_DYNCREATE(CNewMenuBitmaps)

public:
  CNewMenuBitmaps();
  virtual ~CNewMenuBitmaps();

public:
  int Add(UINT nID, COLORREF crTransparent=CLR_NONE);
  int Add(HICON hIcon, UINT nID=0);
  int Add(CBitmap* pBitmap, COLORREF crTransparent=CLR_NONE);

  virtual void OnSysColorChange();

  CArray<COLORREF,COLORREF&> m_TranspColors;
};

/////////////////////////////////////////////////////////////////////////////
// CNewMenuItemData menu item data for drawing

class GUILIBDLLEXPORT CNewMenuItemData : public CObject
{
  DECLARE_DYNCREATE(CNewMenuItemData)

public:
  CNewMenuItemData();
  virtual ~CNewMenuItemData();

public:
  // get the item text, 
  // hAccel=INVALID_HANDLE_VALUE = gets the default from the frame
  // hAccel=NULL disable accelerator support
  virtual CString GetString(HACCEL hAccel=NULL);
  virtual void SetString(LPCTSTR szMenuText);

#if defined(_DEBUG) || defined(_AFXDLL)
  // Diagnostic Support
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

public:
  CString m_szMenuText;

  UINT m_nTitleFlags;
  UINT m_nFlags;
  UINT m_nID;
  UINT m_nSyncFlag;

  CNewMenuIcons* m_pMenuIcon;
  int m_nMenuIconOffset;

  void* m_pData;
};

/////////////////////////////////////////////////////////////////////////////
// CNewMenuItemDataTitle menu item data for drawing menu Title

class GUILIBDLLEXPORT CNewMenuItemDataTitle : public CNewMenuItemData
{
  DECLARE_DYNCREATE(CNewMenuItemDataTitle)

public:
  CNewMenuItemDataTitle();
  virtual ~CNewMenuItemDataTitle();

public:
  COLORREF m_clrTitle;
  COLORREF m_clrLeft;
  COLORREF m_clrRight;

  CFont m_Font;
};

/////////////////////////////////////////////////////////////////////////////
// CNewMenu the new menu class

class GUILIBDLLEXPORT CNewMenu : public CMenu
{
  friend class CNewMenuHook;
  friend class CNewMenuIcons;

  DECLARE_DYNCREATE(CNewMenu)

public:
  // how the menu's are drawn, either original or XP style
  typedef enum 
  { 
    STYLE_ORIGINAL=0,
    STYLE_ORIGINAL_NOBORDER=1,

    STYLE_XP=2,
    STYLE_XP_NOBORDER=3,

    STYLE_SPECIAL=4,
    STYLE_SPECIAL_NOBORDER=5,

    STYLE_ICY=6,
    STYLE_ICY_NOBORDER=7,

    STYLE_XP_2003=8,
    STYLE_XP_2003_NOBORDER=9,

    STYLE_COLORFUL=10,
    STYLE_COLORFUL_NOBORDER=11,

    STYLE_LAST = 11,

    STYLE_UNDEFINED = -1

  } EDrawStyle;

  // how seperators are handled when removing a menu (Tongzhe Cui)
  typedef enum {NONE, HEAD, TAIL, BOTH} ESeperator;

public:
  CNewMenu(HMENU hParent=0); 
  virtual ~CNewMenu();

  // Functions for loading and applying bitmaps to menus (see example application)
  virtual BOOL LoadMenu(HMENU hMenu);
  virtual BOOL LoadMenu(LPCTSTR lpszResourceName);
  virtual BOOL LoadMenu(int nResource);

  BOOL LoadToolBar(HBITMAP hBitmap, CSize size, UINT* pID, COLORREF crTransparent=CLR_NONE);
  BOOL LoadToolBar(WORD* pIconInfo, COLORREF crTransparent=CLR_NONE);
  BOOL LoadToolBar(LPCTSTR lpszResourceName, HMODULE hInst = NULL);
  BOOL LoadToolBar(UINT nToolBar, HMODULE hInst = NULL);
  BOOL LoadToolBars(const UINT *arID,int n, HMODULE hInst = NULL);
  // Jan-12-2005 - Mark P. Peterson - mpp@rhinosoft.com - http://www.RhinoSoft.com/
  BOOL LoadToolBar(UINT n16ToolBarID, UINT n256BitmapID, COLORREF transparentColor, HMODULE hInst = NULL);

  BOOL LoadFromToolBar(UINT nID,UINT nToolBar,int& xoffset);
  BOOL AddBitmapToImageList(CImageList *list,UINT nResourceID);

  static HBITMAP LoadSysColorBitmap(int nResourceId);
  // custom check mark bitmaps
  void LoadCheckmarkBitmap(int unselect, int select); 

  // functions for appending a menu option, use the AppendMenu call
  BOOL AppendMenu(UINT nFlags,UINT nIDNewItem=0,LPCTSTR lpszNewItem=NULL,int nIconNormal=-1);
  BOOL AppendMenu(UINT nFlags,UINT nIDNewItem,LPCTSTR lpszNewItem,CImageList *il,int xoffset);
  BOOL AppendMenu(UINT nFlags,UINT nIDNewItem,LPCTSTR lpszNewItem,CBitmap *bmp);

  BOOL AppendODMenu(LPCTSTR lpstrText, UINT nFlags = MF_OWNERDRAW, UINT nID = 0, int nIconNormal = -1);  
  BOOL AppendODMenu(LPCTSTR lpstrText, UINT nFlags, UINT nID, CBitmap* pbmp);
  BOOL AppendODMenu(LPCTSTR lpstrText, UINT nFlags, UINT nID, CImageList *il, int xoffset);
  BOOL AppendODMenu(LPCTSTR lpstrText, UINT nFlags, UINT nID, CNewMenuIcons* pIcons, int nIndex);

  // for appending a popup menu (see example application)
  CNewMenu* AppendODPopupMenu(LPCTSTR lpstrText);

  // functions for inserting a menu option, use the InsertMenu call (see above define)
  BOOL InsertMenu(UINT nPosition,UINT nFlags,UINT nIDNewItem=0,LPCTSTR lpszNewItem=NULL,int nIconNormal=-1);
  BOOL InsertMenu(UINT nPosition,UINT nFlags,UINT nIDNewItem,LPCTSTR lpszNewItem,CImageList *il,int xoffset);
  BOOL InsertMenu(UINT nPosition,UINT nFlags,UINT nIDNewItem,LPCTSTR lpszNewItem,CBitmap *bmp);

  BOOL InsertODMenu(UINT nPosition,LPCTSTR lpstrText, UINT nFlags = MF_OWNERDRAW,UINT nID = 0,int nIconNormal = -1);  
  BOOL InsertODMenu(UINT nPosition,LPCTSTR lpstrText, UINT nFlags, UINT nID, CBitmap* pBmp);
  BOOL InsertODMenu(UINT nPosition,LPCTSTR lpstrText, UINT nFlags, UINT nID, CImageList *il,int xoffset);
  BOOL InsertODMenu(UINT nPosition,LPCTSTR lpstrText, UINT nFlags, UINT nID, CNewMenuIcons* pIcons, int nIndex);

  // Same as ModifyMenu but replacement for CNewMenu
  BOOL ModifyODMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0,LPCTSTR lpszNewItem = NULL);
  BOOL ModifyODMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem, const CBitmap* pBmp);

  // functions for modifying a menu option, use the ModifyODMenu call (see above define)
  BOOL ModifyODMenu(LPCTSTR lpstrText, UINT nID=0, int nIconNormal=-1);
  BOOL ModifyODMenu(LPCTSTR lpstrText, UINT nID, CImageList *il, int xoffset);
  BOOL ModifyODMenu(LPCTSTR lpstrText, UINT nID, CNewMenuIcons* pIcons, int nIndex);

  BOOL ModifyODMenu(LPCTSTR lpstrText,UINT nID,CBitmap *bmp);
  BOOL ModifyODMenu(LPCTSTR lpstrText,LPCTSTR OptionText,int nIconNormal);
  // use this method for adding a solid/hatched colored square beside a menu option
  // courtesy of Warren Stevens
  BOOL ModifyODMenu(LPCTSTR lpstrText,UINT nID,COLORREF fill,COLORREF border,int hatchstyle=-1);

  // for deleting and removing menu options
  BOOL  DeleteMenu(UINT uiId,UINT nFlags);
  BOOL  RemoveMenu(UINT uiId,UINT nFlags);
  int RemoveMenu(LPCTSTR pText, ESeperator sPos=CNewMenu::NONE);

  // function for retrieving and setting a menu options text (use this function
  // because it is ownerdrawn)
  BOOL GetMenuText(UINT id,CString &string,UINT nFlags = MF_BYPOSITION);
  BOOL SetMenuText(UINT id,CString string, UINT nFlags = MF_BYPOSITION);

  // Getting a submenu from it's name or position
  CMenu* GetSubMenu (LPCTSTR lpszSubMenuName) const;
  CMenu* GetSubMenu (int nPos) const;
  int GetMenuPosition(LPCTSTR pText);

  // Destoying
  virtual BOOL DestroyMenu();

  // Drawing: 
  // Draw an item
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
  // Measure an item  
  virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
  // Draw title of the menu
  virtual void DrawTitle(LPDRAWITEMSTRUCT lpDIS, BOOL bIsMenuBar);
  // Erase the Background of the menu
  virtual BOOL EraseBkgnd(HWND hWnd, HDC hDC);

  static COLORREF GetMenuBarColor2003();
  static void GetMenuBarColor2003(COLORREF& color1, COLORREF& color2, BOOL bBackgroundColor = TRUE);
  static COLORREF GetMenuBarColorXP();
  static COLORREF GetMenuBarColor(HMENU hMenu=NULL);
  static COLORREF GetMenuColor(HMENU hMenu=NULL);

  static void SetMenuTitleFont(CFont* pFont);
  static void SetMenuTitleFont(LOGFONT* pLogFont);
  static LOGFONT GetMenuTitleFont();

  // Menutitle function
  BOOL SetMenuTitle(LPCTSTR pTitle, UINT nTitleFlags=MFT_TOP_TITLE, int nIconNormal = -1);
  BOOL SetMenuTitle(LPCTSTR pTitle, UINT nTitleFlags, CBitmap* pBmp);
  BOOL SetMenuTitle(LPCTSTR pTitle, UINT nTitleFlags, CImageList *pil, int xoffset);
  BOOL SetMenuTitle(LPCTSTR pTitle, UINT nTitleFlags, CNewMenuIcons* pIcons, int nIndex);

  BOOL SetMenuTitleColor(COLORREF clrTitle=CLR_DEFAULT, COLORREF clrLeft=CLR_DEFAULT, COLORREF clrRight=CLR_DEFAULT);

  BOOL RemoveMenuTitle();

  // Function to set how menu is drawn, either original or XP style
  static UINT GetMenuDrawMode();
  static UINT SetMenuDrawMode(UINT mode);

  static CMenuTheme* GetActualMenuTheme(){ return m_pActMenuDrawing;}

  // Function to set how disabled items are drawn 
  //(mode=FALSE means they are not drawn selected)
  static BOOL SetSelectDisableMode(BOOL mode);
  static BOOL GetSelectDisableMode();

  // Function to set how icons were drawn in normal mode 
  //(enable=TRUE means they are drawn blended)
  static BOOL SetXpBlending(BOOL bEnable=TRUE);
  static BOOL GetXpBlending();

  // Jan-12-2005 - Mark P. Peterson - mpp@rhinosoft.com - http://www.RhinoSoft.com/
  // added SetGloomFactor() and GetGloomFactor() so that the glooming can be done in a more or less subtle way
  static int SetGloomFactor(int nGloomFactor);
  static int GetGloomFactor();

  // Function to set how default menu border were drawn
  //(enable=TRUE means that all menu in the application has the same border)
  static BOOL SetNewMenuBorderAllMenu(BOOL bEnable=TRUE);
  static BOOL GetNewMenuBorderAllMenu();

  static void OnSysColorChange();

  // Static functions used for handling menu's in the mainframe
  static LRESULT FindKeyboardShortcut(UINT nChar,UINT nFlags,CMenu *pMenu);
  static BOOL OnMeasureItem(const MSG* pMsg);
  static void OnInitMenuPopup(HWND hWnd, CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);

  // Helperfunction to find the menu to the item
  static CMenu* FindPopupMenuFromID(CMenu* pMenu, UINT nID);
  static CMenu* FindPopupMenuFromID(HMENU hMenu, UINT nID);

  static CMenu* FindPopupMenuFromIDData(CMenu* pMenu, UINT nID, ULONG_PTR pData);
  static CMenu* FindPopupMenuFromIDData(HMENU hMenu, UINT nID, ULONG_PTR pData);

  virtual void OnInitMenuPopup();
  virtual BOOL OnUnInitPopupMenu();

  // Customizing:
  // Set icon size
  void SetIconSize(int width, int height);
  CSize GetIconSize();

  // set the color in the bitmaps that is the background transparent color
  COLORREF SetBitmapBackground(COLORREF newColor);
  COLORREF GetBitmapBackground(); 

  // Return the last itemrect from a menubaritem.
  CRect GetLastActiveMenuRect();
  void SetLastMenuRect(HDC hDC, LPRECT pRect);
  void SetLastMenuRect(LPRECT pRect);

  HMENU GetParent();
  BOOL IsPopup();
  BOOL SetPopup(BOOL bIsPopup=TRUE);

  BOOL SetItemData(UINT uiId, DWORD_PTR dwItemData, BOOL fByPos = FALSE);
  BOOL SetItemDataPtr(UINT uiId, void* pItemData, BOOL fByPos = FALSE);

  DWORD_PTR GetItemData(UINT uiId, BOOL fByPos = FALSE) const;
  void* GetItemDataPtr(UINT uiId, BOOL fByPos = FALSE) const;

  BOOL SetMenuData(DWORD_PTR dwMenuData);
  BOOL SetMenuDataPtr(void* pMenuData);

  DWORD_PTR GetMenuData() const;
  void* GetMenuDataPtr() const;

  // enable or disable the global accelerator drawing
  static BOOL SetAcceleratorsDraw (BOOL bDraw);
  static BOOL GetAcceleratorsDraw ();

  // Set Alphablending of the menu only win2000 /XP/2003
  // When bAlpha is 255, the window is opaque
  static BYTE SetAlpha(BYTE bAlpha);
  static BYTE GetAlpha();

  // INVALID_HANDLE_VALUE = Draw default frame's accel. NULL = Off
  HACCEL SetAccelerator (HACCEL hAccel);
  HACCEL GetAccelerator ();

  // can set icons from a global saved icons-list
  DWORD SetMenuIcons(CNewMenuIcons* pMenuIcons);

  // Miscellaneous Protected Member functions
protected:
  CNewMenuIcons* GetMenuIcon(int &nIndex, UINT nID, CImageList *pil, int xoffset);
  CNewMenuIcons* GetMenuIcon(int &nIndex, int nID);
  CNewMenuIcons* GetMenuIcon(int &nIndex, CBitmap* pBmp);

  CNewMenuIcons* GetToolbarIcons(UINT nToolBar, HMODULE hInst=NULL);

  BOOL Replace(UINT nID, UINT nNewID);

  static BOOL IsNewShell();
  BOOL IsMenuBar(HMENU hMenu=NULL);

  CNewMenuItemData* FindMenuItem(UINT nID);
  CNewMenu* FindMenuOption(int nId, int& nLoc);
  CNewMenu* FindMenuOption(LPCTSTR lpstrText, int& nLoc);

  CNewMenu* FindAnotherMenuOption(int nId, int& nLoc, CArray<CNewMenu*,CNewMenu*>&newSubs, CArray<int,int&>&newLocs);

  CNewMenuItemData* NewODMenu(UINT pos, UINT nFlags, UINT nID, LPCTSTR string);
  CNewMenuItemDataTitle* GetMemuItemDataTitle();

  void SynchronizeMenu();
  void InitializeMenuList(int value);
  void DeleteMenuList();

  CNewMenuItemData* FindMenuList(UINT nID);
  CNewMenuItemData* CheckMenuItemData(ULONG_PTR nItemData) const;

  void DrawSpecial_OldStyle(CDC* pDC, LPCRECT pRect, UINT nID, DWORD dwStyle);
  void DrawSpecial_WinXP(CDC* pDC, LPCRECT pRect, UINT nID, DWORD dwStyle);

  void DrawSpecialCharStyle(CDC* pDC, LPCRECT pRect, TCHAR Sign, DWORD dwStyle);
  void DrawSpecialChar(CDC* pDC, LPCRECT pRect, TCHAR Sign, BOOL bBold);

  void DrawMenuTitle(LPDRAWITEMSTRUCT lpDIS, BOOL bIsMenuBar);

  // Measure an item
  void MeasureItem_OldStyle( LPMEASUREITEMSTRUCT lpMIS, BOOL bIsMenuBar); 
  void DrawItem_OldStyle (LPDRAWITEMSTRUCT lpDIS, BOOL bIsMenubar);

  void MeasureItem_Icy( LPMEASUREITEMSTRUCT lpMIS, BOOL bIsMenuBar); 
  void DrawItem_Icy (LPDRAWITEMSTRUCT lpDIS, BOOL bIsMenubar);
  BOOL Draw3DCheckmark(CDC* dc, CRect rc, HBITMAP hbmpChecked, HBITMAP hbmpUnchecked, DWORD dwState);

  void MeasureItem_WinXP( LPMEASUREITEMSTRUCT lpMIS, BOOL bIsMenuBar); 
  void DrawItem_WinXP (LPDRAWITEMSTRUCT lpDIS, BOOL bIsMenuBar);

  void DrawItem_XP_2003 (LPDRAWITEMSTRUCT lpDIS, BOOL bIsMenuBar);

  void DrawItem_SpecialStyle (LPDRAWITEMSTRUCT lpDIS, BOOL bIsMenubar);

  //  BOOL ImageListDuplicate(CImageList* il,int xoffset,CImageList* newlist);
  void ColorBitmap(CDC* pDC, CBitmap& bmp, CSize size, COLORREF fill, COLORREF border, int hatchstyle=-1);


  // Member Variables
public:
  static DWORD m_dwLastActiveItem;

protected: 
  // Stores list of menu items
  CTypedPtrArray<CPtrArray, CNewMenuItemData*> m_MenuItemList;   
  // When loading an owner-drawn menu using a Resource, CNewMenu must keep track of
  // the popup menu's that it creates. Warning, this list *MUST* be destroyed
  // last item first :)
  // Stores list of sub-menus
  CTypedPtrArray<CPtrArray, HMENU>  m_SubMenus;

  static BOOL m_bEnableXpBlending;
  static BOOL m_bNewMenuBorderAllMenu;
  static BOOL m_bSelectDisable;
  // Jan-12-2005 - Mark P. Peterson - mpp@rhinosoft.com - http://www.RhinoSoft.com/
  // added gloom factor
  static int m_nGloomFactor;
  static CMenuTheme* m_pActMenuDrawing;
  static LOGFONT m_MenuTitleFont;
  static CTypedPtrList<CPtrList, CNewMenuIcons*>* m_pSharedMenuIcons;
  static BOOL m_bDrawAccelerators;
  static BYTE m_bAlpha;

  int m_iconX;
  int m_iconY;

  HWND m_hTempOwner;

  COLORREF m_bitmapBackground;

  CImageList* m_checkmaps;
  BOOL m_checkmapsshare;

  int m_selectcheck;
  int m_unselectcheck;

  BOOL m_bDynIcons;

  HMENU m_hParentMenu;

  BOOL m_bIsPopupMenu;

  CRect m_LastActiveMenuRect;

  DWORD m_dwOpenMenu;

  void* m_pData;

  HACCEL  m_hAccelToDraw;
};

/////////////////////////////////////////////////////////////////////////////
// CNewMenuThreadHook class for hooking in a new thread
// Jan-17-2005 - Mark P. Peterson - mpp@rhinosoft.com - http://www.RhinoSoft.com/

class GUILIBDLLEXPORT CNewMenuThreadHook
{
// Initialization
public:
  CNewMenuThreadHook();
  virtual ~CNewMenuThreadHook();

// Attributes
private:
  HHOOK m_hHookOldMenuCbtFilter;
};

/////////////////////////////////////////////////////////////////////////////
// CNewFrame<> template for easy using of the new menu

template<class baseClass>
class GUILIBDLLEXPORT CNewFrame : public baseClass
{
  typedef CNewFrame<baseClass> MyNewFrame;
public:
  CNewMenu m_DefaultNewMenu;
  CNewMenu m_SystemNewMenu;

public:

  CNewFrame()
  {
    m_bInMenuLoop = FALSE;
    m_TimerID = NULL;
    m_menubarItemIndex = UINT(-1);
  }

  // control bar docking
  void EnableDocking(DWORD dwDockStyle)
  {
    baseClass::EnableDocking(dwDockStyle);
    // Owerite registering for floating frame
    m_pFloatingFrameClass = RUNTIME_CLASS(CNewMiniDockFrameWnd);
  }

private:
  static const AFX_MSGMAP_ENTRY _messageEntries[];

protected: 
  static const AFX_MSGMAP messageMap;

  BOOL m_bInMenuLoop;
  UINT m_TimerID;
  UINT m_menubarItemIndex;


#if _MFC_VER < 0x0700 
  static const AFX_MSGMAP* PASCAL _GetBaseMessageMap()
  { 
    return &baseClass::messageMap; 
  };
#else
  static const AFX_MSGMAP* PASCAL GetThisMessageMap()
  {
    return &CNewFrame<baseClass>::messageMap; 
  }
#endif

  virtual const AFX_MSGMAP* GetMessageMap() const 
  {
    return &CNewFrame<baseClass>::messageMap; 
  }
	//{{AFX_MSG(CNewFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG

  static const AFX_MSGMAP_ENTRY* GetMessageEntries()
  {
#if _MFC_VER >= 0x0800  
    typedef MyNewFrame ThisClass;
#endif
    static const AFX_MSGMAP_ENTRY Entries[] =
    {
        ON_WM_MEASUREITEM()
        ON_WM_MENUCHAR()
        ON_WM_INITMENUPOPUP()
        ON_WM_ENTERMENULOOP()
        ON_WM_EXITMENULOOP() 
        ON_WM_TIMER()
        ON_WM_CREATE()
        ON_WM_NCHITTEST()
        ON_WM_DESTROY()
        ON_WM_SYSCOLORCHANGE()
#ifdef USE_NEW_DOCK_BAR
        ON_WM_NCPAINT()
        ON_WM_PAINT()
        ON_WM_ACTIVATEAPP()
        ON_WM_ACTIVATE()
#endif //USE_NEW_DOCK_BAR
      {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
    }; 
    return Entries;
  }

  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
  {
    if (baseClass::OnCreate(lpCreateStruct) == -1)
      return -1;

    if(m_SystemNewMenu.m_hMenu)
    {
      ::DestroyMenu(m_SystemNewMenu.Detach());
    }
    m_SystemNewMenu.Attach(::GetSystemMenu(m_hWnd,FALSE));
    return 0;
  }

  afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
  {
    baseClass::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
    CNewMenu::OnInitMenuPopup(m_hWnd,pPopupMenu, nIndex, bSysMenu);
  }

  afx_msg void OnSysColorChange() 
  {
    baseClass::OnSysColorChange();
    CNewMenu::OnSysColorChange();
    UpdateMenuBarColor(m_DefaultNewMenu);
  }

  afx_msg void OnEnterMenuLoop(BOOL bIsTrackPopupMenu)
  {
    m_bInMenuLoop = TRUE;
    if(m_TimerID!=NULL)
    {
      KillTimer(m_TimerID);
      m_TimerID = NULL;
    }
    if (m_menubarItemIndex!=UINT(-1))
    {
      DrawMenubarItem(this,GetMenu(),m_menubarItemIndex,NULL);
      m_menubarItemIndex=UINT(-1);
    }
    baseClass::OnEnterMenuLoop(bIsTrackPopupMenu);
  }

  afx_msg void OnExitMenuLoop(BOOL bIsTrackPopupMenu)
  {
    m_bInMenuLoop = FALSE;
    baseClass::OnExitMenuLoop(bIsTrackPopupMenu);
  }

  afx_msg void OnTimer(UINT_PTR nIDEvent)
  {
    baseClass::OnTimer(nIDEvent);
    if(m_TimerID==nIDEvent)
    {   
      CPoint pt;
      GetCursorPos(&pt);
      SendMessage(WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y));
    }
  }

  afx_msg void OnDestroy()
  {
    if(m_TimerID!=NULL)
    {
      KillTimer(m_TimerID);
      m_TimerID = NULL;
    }
    baseClass::OnDestroy();
  }

 #if _MFC_VER < 0x0800  
  afx_msg UINT OnNcHitTest(CPoint point)
  {
    UINT nHitCode = baseClass::OnNcHitTest(point);
#else
  afx_msg LRESULT OnNcHitTest(CPoint point)
  {
    UINT nHitCode = (UINT)baseClass::OnNcHitTest(point);
#endif
    // Test Win95/98/me and Win NT 4.0
    if(g_Shell<Win2000 || bRemoteSession)
    {
      UINT nStatus;
      UINT nHotlightStatus;

      if( IsChild(GetFocus()) )
      { 
        nStatus = 0; 
        nHotlightStatus = ODS_HOTLIGHT;
      }
      else if(g_Shell==Win95 || g_Shell==WinNT4)
      { // Win95 or winNt 4.0 do not have an inactive menubar
        nStatus = 0; 
        nHotlightStatus = 0;
      }
      else
      { 
        nStatus = ODS_INACTIVE;
        nHotlightStatus = ODS_HOTLIGHT|ODS_INACTIVE;
      }

      CNewMenu* pNewMenu = DYNAMIC_DOWNCAST(CNewMenu,GetMenu());
      if (!m_bInMenuLoop && nHitCode == HTMENU)
      {
        // I support only CNewMenu ownerdrawings menu!!
        if(pNewMenu)
        {
          UINT nItemIndex = MenuItemFromPoint(m_hWnd, pNewMenu->m_hMenu, point);

          if ( nItemIndex!=(UINT)-1 )
          {
            if(m_menubarItemIndex!=nItemIndex)
            { 
              // Clear the old Item
              DrawMenubarItem(this,pNewMenu,m_menubarItemIndex,nStatus);

              // Draw the hotlight item.
              if(DrawMenubarItem(this,pNewMenu,nItemIndex,nHotlightStatus))
              {
                // Set a new Timer
                if(m_TimerID==NULL)
                {
                  m_TimerID=GetSafeTimerID(m_hWnd,100);
                }
                m_menubarItemIndex = nItemIndex;
              }
              else
              {
                m_menubarItemIndex = UINT(-1);
              }
            }
            else
            {
              // Draw the hotlight item again.
              if(CNewMenu::m_dwLastActiveItem==NULL && 
                DrawMenubarItem(this,pNewMenu,nItemIndex,nHotlightStatus))
              {
                // Set a new Timer
                if(m_TimerID==NULL)
                {
                  m_TimerID=GetSafeTimerID(m_hWnd,100);
                }
                m_menubarItemIndex = nItemIndex;
              }
            }
            return nHitCode;
          }
        }
      }

      if (m_menubarItemIndex!=UINT(-1))
      {
        DrawMenubarItem(this,pNewMenu,m_menubarItemIndex,nStatus);
        m_menubarItemIndex=UINT(-1);
      }
      if(m_TimerID!=NULL)
      {
        KillTimer(m_TimerID);
        m_TimerID=NULL;
      }
    }
    return nHitCode;
  }

  afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMIS) 
  {
    if(!CNewMenu::OnMeasureItem(GetCurrentMessage()))
    {
      CMenu* pMenu;
      _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
      if (pThreadState && pThreadState->m_hTrackingWindow == m_hWnd)
      {
        // start from popup
        pMenu = CMenu::FromHandle(pThreadState->m_hTrackingMenu);
      }
      else
      {
        // start from menubar
        pMenu = GetMenu();
      }
      if(pMenu)
      {
        baseClass::OnMeasureItem(nIDCtl, lpMIS);
      }
    } 
  }

  afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
  {
    LRESULT lresult;
    if( DYNAMIC_DOWNCAST(CNewMenu,pMenu) )
      lresult=CNewMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
    else
      lresult=baseClass::OnMenuChar(nChar, nFlags, pMenu);

    return lresult;
  }

#ifdef USE_NEW_DOCK_BAR
  afx_msg void OnNcPaint()
  {
    baseClass::OnNcPaint();
    DrawSmallBorder();
  }

  afx_msg void OnPaint()
  {
    baseClass::OnPaint();
    DrawSmallBorder();
  }

#if _MFC_VER < 0x0700 
  afx_msg void OnActivateApp(BOOL bActive, HTASK hTask)
  {
    baseClass::OnActivateApp(bActive, hTask);
    DrawSmallBorder();
  }
#else
  afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID)
  {
    baseClass::OnActivateApp(bActive, dwThreadID);
    DrawSmallBorder();
  }
#endif

  afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized )
  {
    baseClass::OnActivate(nState,pWndOther, bMinimized );
    DrawSmallBorder();
  }

  void DrawSmallBorder()
  {
    // To eliminate small border between menu and client rect
    MENUINFO menuInfo = {0};
    menuInfo.cbSize = sizeof(menuInfo);
    menuInfo.fMask = MIM_BACKGROUND;
    if(::GetMenuInfo(::GetMenu(m_hWnd),&menuInfo) && menuInfo.hbrBack)
    {
      CDC* pDC = GetWindowDC(); 
      CRect clientRect;
      GetClientRect(clientRect);
      ClientToScreen(clientRect);
      CRect windowRect;
      GetWindowRect(windowRect);
      CRect rect(clientRect.left-windowRect.left,clientRect.top-windowRect.top-1,clientRect.right-windowRect.left,clientRect.top-windowRect.top);

      CBrush *pBrush = CBrush::FromHandle(menuInfo.hbrBack);
      // need for win95/98/me
      VERIFY(pBrush->UnrealizeObject());
      CPoint oldOrg = pDC->SetBrushOrg(0,0);
      pDC->FillRect(rect,pBrush);
      pDC->SetBrushOrg(oldOrg);
      // must be called
      ReleaseDC(pDC);
    }
  }
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CNewMiniDockFrameWnd for docking toolbars with new menu

class GUILIBDLLEXPORT CNewMiniDockFrameWnd: public CNewFrame<CMiniDockFrameWnd> 
{
  DECLARE_DYNCREATE(CNewMiniDockFrameWnd) 
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewDialog for dialog implementation

class GUILIBDLLEXPORT CNewDialog : public CNewFrame<CDialog>
{
  DECLARE_DYNAMIC(CNewDialog);

public:
  CNewDialog();
  CNewDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
  CNewDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);

  // Overridables (special message map entries)
  virtual BOOL OnInitDialog();

protected:
  // Generated message map functions
  //{{AFX_MSG(CNewDialog)
  afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewMiniFrameWnd for menu to documents

class GUILIBDLLEXPORT CNewMiniFrameWnd : public CNewFrame<CMiniFrameWnd>
{
  DECLARE_DYNCREATE(CNewMiniFrameWnd)
	//{{AFX_MSG(CNewMiniFrameWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewMDIChildWnd for menu to documents

class GUILIBDLLEXPORT CNewMDIChildWnd : public CNewFrame<CMDIChildWnd>
{
  DECLARE_DYNCREATE(CNewMDIChildWnd)
	//{{AFX_MSG(CNewMDIChildWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewFrameWnd for menu to documents

class GUILIBDLLEXPORT CNewFrameWnd : public CNewFrame<CFrameWnd>
{
  DECLARE_DYNCREATE(CNewFrameWnd)

public:
#if _MFC_VER < 0x0700 
  // dynamic creation - load frame and associated resources
  virtual BOOL LoadFrame(UINT nIDResource,
    DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
    CWnd* pParentWnd = NULL,
    CCreateContext* pContext = NULL);
#endif

  // under MFC 7.0 the next function is virtual so we don't neet to owerwrite
  // loadframe
  BOOL Create(LPCTSTR lpszClassName,
    LPCTSTR lpszWindowName,
    DWORD dwStyle = WS_OVERLAPPEDWINDOW,
    const RECT& rect = rectDefault,
    CWnd* pParentWnd = NULL,        // != NULL for popups
    LPCTSTR lpszMenuName = NULL,
    DWORD dwExStyle = 0,
    CCreateContext* pContext = NULL);

#ifdef USE_NEW_DOCK_BAR
  // control bar docking
  void EnableDocking(DWORD dwDockStyle);
#endif
	//{{AFX_MSG(CNewFrameWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewMDIFrameWnd for menu to documents

class GUILIBDLLEXPORT CNewMDIFrameWnd : public CNewFrame<CMDIFrameWnd>
{
  DECLARE_DYNCREATE(CNewMDIFrameWnd);
  HMENU m_hShowMenu;

public: 
  CNewMDIFrameWnd():m_hShowMenu(NULL){};

  BOOL ShowMenu(BOOL bShow);

#if _MFC_VER < 0x0700 
  // dynamic creation - load frame and associated resources
  virtual BOOL LoadFrame(UINT nIDResource,
    DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
    CWnd* pParentWnd = NULL,
    CCreateContext* pContext = NULL);
#endif
  // under MFC 7.0 the next function is virtual so we don't neet to owerwrite
  // loadframe
  BOOL Create(LPCTSTR lpszClassName,
    LPCTSTR lpszWindowName,
    DWORD dwStyle = WS_OVERLAPPEDWINDOW,
    const RECT& rect = rectDefault,
    CWnd* pParentWnd = NULL,        // != NULL for popups
    LPCTSTR lpszMenuName = NULL,
    DWORD dwExStyle = 0,
    CCreateContext* pContext = NULL);

#ifdef USE_NEW_DOCK_BAR
  // control bar docking
  void EnableDocking(DWORD dwDockStyle);
#endif

	//{{AFX_MSG(CNewMDIFrameWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CNewMultiDocTemplate for menu to documents

class GUILIBDLLEXPORT CNewMultiDocTemplate: public CMultiDocTemplate
{
  DECLARE_DYNAMIC(CNewMultiDocTemplate)

public:
  CNewMenu m_NewMenuShared;

  // Constructors
public:
  CNewMultiDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
    CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);

  ~CNewMultiDocTemplate();
};


/////////////////////////////////////////////////////////////////////////////
// CNewMemDC helperclass for drawing off screen 

class GUILIBDLLEXPORT CNewMemDC: public CDC
{
  CRect m_rect;
  CBitmap* m_pOldBitmap;
  CBitmap  m_memBitmap;
  BOOL m_bCancel;

  HDC m_hDcOriginal;

public:
  CNewMemDC(LPCRECT pRect, HDC hdc):m_rect(pRect),m_bCancel(FALSE),m_hDcOriginal(hdc)
  {
    CDC *pOrgDC = CDC::FromHandle(m_hDcOriginal);
    CreateCompatibleDC(pOrgDC);

    m_memBitmap.CreateCompatibleBitmap (pOrgDC,m_rect.Width (),m_rect.Height());
    m_pOldBitmap = SelectObject (&m_memBitmap);
    SetWindowOrg(m_rect.left, m_rect.top);
  }

  // Abborting to copy image from memory dc to client
  void DoCancel(){m_bCancel=TRUE;}

  ~CNewMemDC()
  {
    if(!m_bCancel)
    {
      CDC *pOrgDC = CDC::FromHandle(m_hDcOriginal);
      pOrgDC->BitBlt (m_rect.left,m_rect.top,m_rect.Width (),m_rect.Height (),this,m_rect.left,m_rect.top,SRCCOPY);
    }
    SelectObject (m_pOldBitmap);
  }
};

/////////////////////////////////////////////////////////////////////////////
// CNewMenuHelper for enabling / disabling menu border, replacing system menu.

#define  NEW_MENU_DIALOG_SUBCLASS     1
#define  NEW_MENU_DIALOG_SYSTEM_MENU  2
#define  NEW_MENU_DEFAULT_BORDER      4
#define  NEW_MENU_CHANGE_MENU_STYLE   8

class GUILIBDLLEXPORT CNewMenuHelper
{
private:
  DWORD m_dwOldFlags;
  int m_OldMenuDrawStyle;

public: 
  CNewMenuHelper(DWORD dwFlags);
  CNewMenuHelper(CNewMenu::EDrawStyle setTempStyle);

  ~CNewMenuHelper();
};

/////////////////////////////////////////////////////////////////////////////
// CNewDockBar support for office2003 colors.
class GUILIBDLLEXPORT CNewDockBar : public CDockBar
{
  DECLARE_DYNCREATE(CNewDockBar)
public:
  CNewDockBar(BOOL bFloating = FALSE);   // TRUE if attached to CMiniDockFrameWnd
  virtual ~CNewDockBar();

#if defined(_DEBUG) || defined(_AFXDLL)
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

public:

  virtual DWORD RecalcDelayShow(AFX_SIZEPARENTPARAMS* lpLayout);
  void EraseNonClient();

  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnNcPaint();

protected:
	//{{AFX_MSG(CNewDockBar)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#ifndef _GUILIB_
  #ifdef _AFXDLL
    #if _MFC_VER < 0x0700 
      template<class baseClass>
      const AFX_MSGMAP CNewFrame<baseClass>::messageMap = { &CNewFrame<baseClass>::_GetBaseMessageMap, GetMessageEntries()};
    #else
      template<class baseClass>
      const AFX_MSGMAP CNewFrame<baseClass>::messageMap = { &baseClass::GetThisMessageMap, GetMessageEntries()};
    #endif
  #else
    #if _MFC_VER < 0x0800 
      template<class baseClass>
      const AFX_MSGMAP CNewFrame<baseClass>::messageMap = { &baseClass::messageMap, GetMessageEntries()};
    #else
      template<class baseClass>
      const AFX_MSGMAP CNewFrame<baseClass>::messageMap = { &baseClass::GetThisMessageMap, GetMessageEntries()};
    #endif
  #endif // _AFXDLL
#endif // _GUILIB_


#pragma warning(pop)

#endif // __CNewMenu_H_
