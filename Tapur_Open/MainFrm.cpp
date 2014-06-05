// MainFrm.cpp : CMainFrame �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "Tapur.h"
#include "SkypeEvents.h"
#include "MainFrm.h"
#include "MsgTree.h"
#include "RsfTree.h"
#include "MsgList.h"
#include "RsfList.h"
#include "FolderForm.h"
#include "MsgForm.h"
#include "RsfSimpleForm.h"
#include "SetHotkey.h"
#include "TPtable.h"
#include "TPutil.h"
#include "FileSpec.h"
#include "ScreenCap.h"
#include "BalloonHelp.h"
// UNICODE �ł͓��삵�Ȃ��@�\
#ifdef _UNICODE
#undef _UNICODE
#undef UNICODE
#include "KillProcess.h"
//#include ".\mainfrm.h"
#define _UNICODE
#define UNICODE
#endif
#define SKYPE_NAME	"Skype.exe"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

// HotKey���ʃR�[�h
static int ID_HK_SHOW = WM_USER + 411;			// ���C����ʕ\��
static int ID_HK_HIDE = WM_USER + 412;			// ���C����ʔ�\��
static int ID_HK_RECORD = WM_USER + 413;		// �^��
static int ID_HK_VRECORD = WM_USER + 414;		// �^��
static int ID_HK_REC_PAUSE = WM_USER + 415;		// �^���E�^��ꎞ��~
static int ID_HK_REC_STOP = WM_USER + 416;		// �^��I��
static int ID_HK_PLAY = WM_USER + 417;			// �Đ��F���݃��X�g�őI������Ă��郌�R�[�h
static int ID_HK_PLAY_PAUSE = WM_USER + 418;	// �Đ��ꎞ��~�E�ĊJ
static int ID_HK_PLAY_STOP = WM_USER + 419;		// �Đ��I��
static int ID_HK_PLAY_NEXT = WM_USER + 420;		// ���̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̎��̃��R�[�h
static int ID_HK_PLAY_PREV = WM_USER + 421;		// �O�̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̑O�̃��R�[�h
// �V���[�g�J�b�g���ʃL�[�R�[�h
static int ID_SK_EXPORT = 'X';		// �^���t�@�C���G�N�X�|�[�g��ʕ\��
static int ID_SK_VEXPORT = 'Y';		// �^��t�@�C���G�N�X�|�[�g��ʕ\��


// �^�X�N�g���C�̃A�C�R���ɑ����郁�b�Z�[�W
#define	WM_ICON_NOTIFY	(WM_APP + 10)

// �E�B���h�E�����T�C�Y
#define INIT_WINSZX		600
#define INIT_WINSZY		600
// �E�B���h�E�ŏ��T�C�Y
#define MIN_WINSZX		400
#define MIN_WINSZY		400

// �E�B���h�E�T�C�Y���W�X�g���ݒ�
// Section name
#define SEC_SETTING "Settings"
// Entry name
#define ENT_WINPOS "WinPos"


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_POPUP_OPEN, OnPopupOpen)
	ON_COMMAND(ID_POPUP_STOP, OnPopupStop)
	ON_COMMAND(ID_POPUP_ENABLE, OnPopupEnable)
	ON_COMMAND(ID_POPUP_AUTO_ENABLE, OnPopupAutoEnable)
	ON_COMMAND(ID_POPUP_HOTKEY, OnPopupHotkey)
	ON_COMMAND(ID_APP_HELP, OnPopupHelp)
	ON_COMMAND(ID_APP_SKYPE, OnPopupSkype)
	ON_COMMAND(ID_POPUP_CLOSE, OnPopupClose)
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_POPUP_STOP, OnUpdatePopupStop)
	ON_UPDATE_COMMAND_UI(ID_POPUP_ENABLE, OnUpdatePopupEnable)
	ON_UPDATE_COMMAND_UI(ID_POPUP_AUTO_ENABLE, OnUpdatePopupAutoEnable)
	ON_WM_TIMER()
	ON_WM_SIZING()
	ON_BN_CLICKED(ID_VRECORD_START, OnVRecordStart)
	ON_BN_CLICKED(ID_RECORD_START, OnRecordStart)
	ON_BN_CLICKED(ID_RECORD_PAUSE, OnRecordPause)
	ON_BN_CLICKED(ID_RECORD_STOP, OnRecordStop)
	ON_BN_CLICKED(ID_RECORD_MOD, OnRecordMod)
	ON_COMMAND(ID_RECMOD_BOTH, OnRecordModBoth)
	ON_COMMAND(ID_RECMOD_RCV, OnRecordModRcv)
	ON_COMMAND(ID_RECMOD_SND, OnRecordModSnd)
    ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_MESSAGE(WM_SCREENCAP_NOTIFY, OnGraphNotify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
	ID_SEPARATOR,           // �ǉ�
	ID_SEPARATOR,           // �ǉ�
	//ID_INDICATOR_KANA,
	//ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �N���X�̍\�z/����

CMainFrame::CMainFrame()
{
	// TODO: ���̈ʒu�Ƀ����o�̏����������R�[�h��ǉ����Ă��������B

	/// ���j���[��Ԃ��ςƂ���
	m_bAutoMenuEnable = FALSE;

	m_flgEnable = 0;		// ���������t���O�����l
	m_flgAutoEnable = 0;	// �����^�������t���O�����l
	m_lngNewMsg = 0;		// �V�����b�Z�[�W�Ǘ��ϐ�

	/// �^�C�}�[������
	m_stApp.blOnTimer = FALSE;
	m_stApp.lngPassTime = 0;
	m_stConnect.blOnTimer = FALSE;
	m_stConnect.lngPassTime = 0;
	m_stAnswerWait.blOnTimer = FALSE;
	m_stAnswerWait.lngPassTime = 0;
	m_stRecordMax.blOnTimer = FALSE;
	m_stRecordMax.lngPassTime = 0;
	m_stPlayChk.blOnTimer = FALSE;
	m_stPlayChk.lngPassTime = 0;
	m_stPlayChk.lngLimitTime = 0;
}

CMainFrame::~CMainFrame()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �N���X�̐f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/// ToolBar�̐���
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAIN_TOOL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // �쐬�Ɏ��s
	}
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // �쐬�Ɏ��s
	}

	// �c�[���o�[���CStatic����
    int index = 0;  
    while (m_wndToolBar.GetItemID(index) != ID_PLACEHOLDER) index++;     
    if (!m_wndToolBar.CreateStatic(m_wndToolBar.m_wndStatic, _T("00:00"), index, ID_PLACEHOLDER, 80))
    {
        TRACE0("Failed to create text on toolbar\n");
        return -1;
    }
    m_wndToolBar.m_wndStatic.ShowWindow(SW_SHOW);

	/// StatusBar�̐���
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // �쐬�Ɏ��s
	}
	//// �G���[���b�Z�[�WPane�̃X�^�C���ύX�i�{�[�_�[��L��������style(SBPS_NOBORDERS)���O���j
	//m_wndStatusBar.SetPaneStyle(0, SBPS_STRETCH);
    m_wndStatusBar.SetPaneInfo(0,0,SBPS_STRETCH,0);	// 1�Ԗڋ��̃y�C����L������ꍇ
    m_wndStatusBar.SetPaneInfo(1,0,SBPS_NORMAL,60);	// 2�Ԗڂ̋��ɘg��\���A�T�C�Y���w�肷��ꍇ
    m_wndStatusBar.SetPaneInfo(2,0,SBPS_NORMAL,10);	// 3�Ԗڂ̋��ɘg��\���A�T�C�Y���w�肷��ꍇ

	/// [��]�t�c�[���o�[�̋���
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	/// �u�^���Ώۃc�[���v��[��]�t�^
	DWORD dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_RECORD_MOD));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_RECORD_MOD), dwStyle);

	// TODO: �����c�[�� �`�b�v�X���K�v�Ȃ��ꍇ�A�������폜���Ă��������B
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	/// ��������C���X�^���X�̐���
	m_pAudio = CAudio::Instance();
	/// �r�f�I����N���X�̃C���X�^���X����
	m_pVideo = CVideo::Instance();

	/// IAccess�C���^�[�t�F�[�X�I�u�W�F�N�g�̍쐬
	if (!InitSkypeAccess()) {
		TRACE0("Failed to create Skype interface.\n");
		return -1;      // �쐬�Ɏ��s
		//PostMessage(WM_CLOSE, 0, 0L);
		//return 0;
	}

	/// Skype�ʐM�|�[�g�̃`�F�b�N
	// �iFW�̃��b�Z�[�W��\�������A���[�U�̊m�F�𑣂��j
	CRegDWORD rgSkypePortOut = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortOut"), 42805, TRUE);
	CRegDWORD rgSkypePortMic = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\SkypePortMic"), 42806, TRUE);
	UINT uiPortOut = (UINT)rgSkypePortOut;
	UINT uiPortMic = (UINT)rgSkypePortMic;
	CSocketIn oSock;
	if (oSock.Open(uiPortOut, uiPortMic) ) {
		oSock.Close();
	}

	/// Skype�ɐڑ�
	//// �����l�Ƃ��ă_�~�[���[�U��`
	m_pAccessEvents->InitDummyUser();
	if (!ConnectSkype()) {
		//// �ڑ��ł��Ȃ��ꍇ
		///// �X�J�C�v�ڑ��҂��^�C�}���N��
		StartConnectTimer();
	}

	///�@�V�X�e���Ŏg���A�C�R�����܂Ƃ߂ă��[�h
	m_hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_MAIN)); 
	m_hIconNew = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_NEW)); 
	m_hIconDis = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_DISCON));

	///�@�A�C�R�����^�X�N�g���C�ɕ\��
	CString str;
 	str.LoadString(IDS_TOOLTIP);
	if (!m_TrayIcon.Create(
                        this,               // Let icon deal with its own messages
                        WM_ICON_NOTIFY,     // Icon notify message to use
                        str,				// tooltip
                        m_hIcon,			// ID of tray icon
                        IDR_POPUP_MAIN))    // ID of popup menu
    {
		return -1;
    }
	//// �f�t�H���g���j���[�̐ݒ�
    m_TrayIcon.SetMenuDefaultItem(ID_POPUP_OPEN, FALSE);
	//// �A�j���[�V�����A�C�R���̐ݒ�(resource.h��ID�ԍ��͘A�����Ă���K�v������)
    m_TrayIcon.SetIconList(IDI_RCV1, IDI_RCV2); 

	/// �c�[���o�[�̏����ݒ�
	m_wndToolBar.Init(this);

	/// APP����m�F�^�C�}�J�n
	StartAppTimer();		

	return 0;
}

void CMainFrame::OnDestroy() 
{
	// �E�B���h�E����u�ŏ��̃C���X�^���X�^�O�v���폜����
	//::RemoveProp(GetSafeHwnd(), AfxGetApp()->m_pszExeName);

	CFrameWnd::OnDestroy();

	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	/// APP����m�F�^�C�}�I��
	StopAppTimer();		

	// HotKey�̉���
	UnregHotKey();

	/*
	// KEY HOOK������Uninstall
	UninstallKeyHook(); 
	*/

	// �^�X�N�g���C�̃A�C�R�����폜���܂��B
	::Shell_NotifyIcon( NIM_DELETE, &m_stNtfyIcon );

	// Skype API �̃C�x���g����
	TRY {
		if (theApp.m_pAccess) {
			if (m_pAccessEvents) {
				// AtlUnadvise�ŉ�������̂ŁARelease�͕s�v
				AtlUnadvise(theApp.m_pAccess, DIID__IAccessEvents, m_dwCookie);
				m_pAccessEvents = NULL;
			}
			theApp.m_pAccess.Release();
			theApp.m_pAccess = NULL;
		}
	}
    CATCH_ALL(e)
    {
		// Nothing
	}
	END_CATCH_ALL

	/// TMP�t�H���_�̃N���A
	ClearTmpData();

	// VisualFX Framework �̔j��
	m_Framework.Destroy();

	/// ��������N���X�̃C���X�^���X�j��
	CAudio::Instance_Delete();
	/// �r�f�I����N���X�̃C���X�^���X�j��
	CVideo::Instance_Delete();
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���b�Z�[�W �n���h��


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	CString str;

	/// �E�B���h�E�T�C�Y�̎Q��
    WINDOWPLACEMENT *lwp;
    UINT nl;
	long szx = INIT_WINSZX;
	long szy = INIT_WINSZY;
    if(AfxGetApp()->GetProfileBinary(_T(SEC_SETTING), _T(ENT_WINPOS), (LPBYTE*)&lwp, &nl)) {
		szx = abs(lwp->rcNormalPosition.right - lwp->rcNormalPosition.left);
		szy = abs(lwp->rcNormalPosition.top - lwp->rcNormalPosition.bottom);
		delete [] lwp;	// �K�{�i������new����邽�߁j
	} 

	/// �t���[���T�C�Y�̎Q��
	long szMsgTreeX = 150;
	long szRsfTreeX = 150;
	long szMsgListY = szy * 1/3;
	long szRsfListY = szy * 1/4;
	CRegRect rgRsfRect(_T("Software\\Tapur\\Tapur\\Settings\\RsfFrmPos"), CRect(0,0,0,0), TRUE);
	CRect rct = rgRsfRect;
	if (rct.left > 30 && rct.left < szx - 30)	szRsfTreeX = rct.left;
	if (rct.top > 30 && rct.top < szy - 30)	szRsfListY = rct.top;
	CRegRect rgMsgRect(_T("Software\\Tapur\\Tapur\\Settings\\MsgFrmPos"), CRect(0,0,0,0), TRUE);
	rct = rgMsgRect;
	if (rct.left > 30 && rct.left < szx - 30)	szMsgTreeX = rct.left;
	if (rct.top > 30 && rct.top < szy - 30)	szMsgListY = rct.top;

	// �e�t���[����ʂ̐���
	m_pMainTab = new TVisualObject(1,_T("MainTab"),pContext,RUNTIME_CLASS(TTabWnd),TVisualObject::TOS_TABBOTTOM);
	/// �@�\TAB
	str.LoadString(IDS_PANE_MSG);
	m_pTabInbox = new TVisualObject(2,str,1,2,pContext);
	str.LoadString(IDS_PANE_SETUP);
	m_pTabSetup = new TVisualObject(3,str,1,2,pContext);
	/// Left:Tree�\��Pane
	m_pMsgTree = new TVisualObject(4,0,0,pContext, RUNTIME_CLASS(CMsgTree),CSize(szMsgTreeX,0));
	m_pRsfTree = new TVisualObject(5,0,0,pContext, RUNTIME_CLASS(CRsfTree),CSize(szRsfTreeX,0));
	/// Right:Nest
	m_pNestInbox = new TVisualObject(6,0,1,2,1,pContext);
	m_pNestSetup = new TVisualObject(7,0,1,2,1,pContext);
	/// Right:���X�g�\��
	m_pMsgList = new TVisualObject(8,0,0,pContext, RUNTIME_CLASS(CMsgList), CSize(0,szMsgListY));
	m_pRsfList = new TVisualObject(9,0,0,pContext, RUNTIME_CLASS(CRsfList), CSize(0,szRsfListY));
	/// Right:���X�g�ڍׁi���b�Z�[�W�ARSF�ݒ�A���j
	m_pTabMsg = new TVisualObject(10,1,0,pContext,RUNTIME_CLASS(TTabWnd),CSize(0,0),TVisualObject::TOS_TABTOP);
	m_pTabRsf = new TVisualObject(11,1,0,pContext,RUNTIME_CLASS(TTabWnd),CSize(0,0),TVisualObject::TOS_TABTOP);
	m_pMsgForm = new TVisualObject(12,_T(" "),pContext, RUNTIME_CLASS(CMsgForm));
	m_pRsfSimpleForm = new TVisualObject(13,_T(" "),pContext, RUNTIME_CLASS(CRsfSimpleForm));
	m_pMsgFolderForm = new TVisualObject(14,_T(" "),pContext, RUNTIME_CLASS(CFolderForm));
	m_pRsfFolderForm = new TVisualObject(15,_T(" "),pContext, RUNTIME_CLASS(CFolderForm));

	m_Framework.Add(m_pMainTab);
	// Inbox Tab
	m_Framework.Add(m_pMainTab, m_pTabInbox);
	m_Framework.Add(m_pTabInbox, m_pMsgTree);
	m_Framework.Add(m_pTabInbox, m_pNestInbox);
	m_Framework.Add(m_pNestInbox, m_pMsgList);
	m_Framework.Add(m_pNestInbox, m_pTabMsg);
	m_Framework.Add(m_pTabMsg, m_pMsgFolderForm);
	m_Framework.Add(m_pTabMsg, m_pMsgForm);
	// Setup Tab
	m_Framework.Add(m_pMainTab, m_pTabSetup);
	m_Framework.Add(m_pTabSetup, m_pRsfTree);
	m_Framework.Add(m_pTabSetup, m_pNestSetup);
	m_Framework.Add(m_pNestSetup, m_pRsfList);
	m_Framework.Add(m_pNestSetup, m_pTabRsf);
	m_Framework.Add(m_pTabRsf, m_pRsfFolderForm);
	m_Framework.Add(m_pTabRsf, m_pRsfSimpleForm);

	m_Framework.Create(this);

	TVisualFrameworkIterator it(m_Framework);
	while (!it.End()) {
		TVisualObject *pObject = it.Get();
		it++;
	}

	// HotKey�̐ݒ�
	RegHotKey();

	return TRUE;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	if(m_Framework.ProcessMessage(pMsg))
		return TRUE;


	// ���[�J���V���[�g�J�b�g�L�[����
    if (pMsg->message == WM_KEYDOWN) {
		if (IsWindowVisible() && GetKeyState(VK_CONTROL) < 0) {
			CMsgForm *pMsgForm = (CMsgForm *)m_pMsgForm->GetWnd();
			int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
			if(nVirtKey == (int)ID_SK_EXPORT) {
				// �^���t�@�C���G�N�X�|�[�g��ʕ\��
				pMsgForm->ExportFile();
			} else if(nVirtKey == (int)ID_SK_VEXPORT) {
				// �^��t�@�C���G�N�X�|�[�g��ʕ\��
				pMsgForm->ExportVFile();
			}
		}
	}

	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
    if(bShow) {
		if(!IsWindowVisible()) {
			/// ���W�X�g������WINDOW�ʒu�擾
			WINDOWPLACEMENT *lwp;
			UINT nl;
			if(AfxGetApp()->GetProfileBinary(_T(SEC_SETTING), _T(ENT_WINPOS), (LPBYTE*)&lwp, &nl))
			{
				SetWindowPlacement(lwp);
				delete [] lwp;
			} else {
				/// ���W�X�g���ɑ��݂��Ȃ��ꍇ�A�����l��ݒ�
				WINDOWPLACEMENT wp;
				GetWindowPlacement(&wp);
				wp.rcNormalPosition.right = wp.rcNormalPosition.left + INIT_WINSZX;
				wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + INIT_WINSZY;
				SetWindowPlacement(&wp);
			}
		} else {
			/// �ŏ��̕\���̎��̂݁A��ʂ̍X�V
			CString str;
			this->GetWindowText(str);
			if (str.GetLength() > 0 && str.GetLength() < 8) {
				UpdateForm();
			}
		}
	}
}

// ���C����ʃI�[�v��
void CMainFrame::OnPopupOpen() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString str;

	/// �X�J�C�v���[�U�`�F�b�N�i�_�~�[���Ȃ��ꍇ�j
	if (theApp.m_strUsrId.IsEmpty()) {
		str.LoadString(IDS_MSGBOX3);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return;
	}

	/// WINDOW�I�[�v��
    CSystemTray::MaximiseFromTray(this);

	/// ��ʂ̍X�V
	UpdateForm();
}

// ����ԉ����̒��~���j���[
void CMainFrame::OnPopupStop() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString str;

	/// ����ԉ�����
	if (theApp.m_lngAppStatus == 2) {
		/// �����I��
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("Force event:prgFinished"));
		m_pAccessEvents->CallStatusChanged(NULL, prgFinished);
	}
}

// ����������L��/�������j���[
void CMainFrame::OnPopupEnable() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString str;

	if (m_flgEnable == 1) {
		/// ���������𖳌��ɂ���
		m_flgEnable = 0;
	} else if (m_flgEnable == 0) {
		/// ����������L���ɂ���
		//// Skype�̏�Ԋm�F
		if (!theApp.m_blSkypeConnect) {
			str.LoadString(IDS_MSGBOX3);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return;
		}
		m_flgEnable = 1;
	}

	// ���W�X�g���X�V
	CRegDWORD rgDisable = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\InitDisable"), 0, TRUE);
	rgDisable = (m_flgEnable == 1) ? 0 : 1;

	InitTooltip();	// �c�[���`�b�v�̏�����
}

// �����^��������L��/�������j���[
void CMainFrame::OnPopupAutoEnable() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString str;

	if (m_flgAutoEnable == 1) {
		/// �����^�������𖳌��ɂ���
		m_flgAutoEnable = 0;
	} else if (m_flgAutoEnable == 0) {
		/// �����^��������L���ɂ���
		//// Skype�̏�Ԋm�F
		if (!theApp.m_blSkypeConnect) {
			str.LoadString(IDS_MSGBOX3);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return;
		}
		m_flgAutoEnable = 1;
	}

	// ���W�X�g���X�V
	CRegDWORD rgEnableAuto = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableAutoVoiceRecord"), 0, TRUE);	// �����l:0(False)
	rgEnableAuto = m_flgAutoEnable;

	InitTooltip();	// �c�[���`�b�v�̏�����
}

// �V���[�g�J�b�g�L�[�̎Q��
void CMainFrame::OnPopupHotkey() 
{
	static CSetHotkey dlg;

	if (dlg.m_hWnd == NULL) {
		dlg.Create(IDD_HOTKEY, this);
		//CButton *btn = (CButton *)dlg.GetDlgItem(IDOK);
		//btn->EnableWindow(FALSE);
	} else {
		if (dlg.IsWindowVisible()) {
			dlg.SetFocus();
			return;
		}
	}

	dlg.ShowWindow(SW_SHOW);
}

// �w���v�\��
void CMainFrame::OnPopupHelp() 
{
	CString str;
	str.LoadString(IDS_URL_HELP);
	theApp.OpenUrl(str);
}

// SkypeIn/Out�w���y�[�W�\��
void CMainFrame::OnPopupSkype() 
{
	CString str;
	str.LoadString(IDS_URL_SKYPE);
	theApp.OpenUrl(str);
}

// �I�����j���[
void CMainFrame::OnPopupClose() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	/// �E�B���h�E�ʒu�E�T�C�Y�̕ۑ�
    if(IsWindowVisible()) {
		OnClose();
	}

	/// Tapur�Ď��v���O�����̏I��
	TRY {
		CKillProcess kp;
		kp.KillProcess("tapmgr.exe", 500);
	}
    CATCH_ALL(e)
    {
		// Nothing
	}
	END_CATCH_ALL

	/// window���I��
	// �n���O����ꍇ������̂ōŌ�Ɏ��s�H
	DestroyWindow();
}

// ���C����ʂ����i��\���j
void CMainFrame::OnClose() 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	// �E�B���h�E�ʒu�E�T�C�Y�̕ۑ�
    WINDOWPLACEMENT wp;
    GetWindowPlacement(&wp);
    AfxGetApp()->WriteProfileBinary(_T(SEC_SETTING), _T(ENT_WINPOS), (LPBYTE)&wp, sizeof(wp));

	// �t���[���T�C�Y�̕ۑ�
	CRect rct;
	int szx, szy;
	CRegRect rgRsfRect(_T("Software\\Tapur\\Tapur\\Settings\\RsfFrmPos"), CRect(0,0,0,0), TRUE);
	m_pRsfTree->GetWnd()->GetClientRect(&rct);
	szx = rct.Width();
	m_pRsfList->GetWnd()->GetClientRect(&rct);
	szy = rct.Height();
	rgRsfRect = CRect(szx, szy, 0, 0);
	CRegRect rgMsgRect(_T("Software\\Tapur\\Tapur\\Settings\\MsgFrmPos"), CRect(0,0,0,0), TRUE);
	m_pMsgTree->GetWnd()->GetClientRect(&rct);
	szx = rct.Width();
	m_pMsgList->GetWnd()->GetClientRect(&rct);
	szy = rct.Height();
	rgMsgRect = CRect(szx, szy, 0, 0);

	// window����邾���ŁA�I���͂��Ȃ��B
	//windows���폜�����̂ŁACSystemTray::MinimiseToTray(this); �͎g�p�s��
	ShowWindow(SW_HIDE);
	
//	CFrameWnd::OnClose();
}

// ����ԉ����`�F�b�N�X�V
void CMainFrame::OnUpdatePopupEnable(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(m_flgEnable);
}

// �����^���`�F�b�N�X�V
void CMainFrame::OnUpdatePopupAutoEnable(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(m_flgAutoEnable);
}

// ����ԉ������~���j���[�X�V
void CMainFrame::OnUpdatePopupStop(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	/// ����ԉ�����
	if (theApp.m_lngAppStatus == 2) {
		pCmdUI->Enable(TRUE);
	} else {
		pCmdUI->Enable(FALSE);
	}
}

// �^�X�N�g���C�A�C�R���̃��b�Z�[�W����
LRESULT CMainFrame::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	// ���{�^���N���b�N���E�{�^���N���b�N�ɍ����ւ���i�����@�\�����蓖�āj
	if (LOWORD(lParam) == WM_LBUTTONUP) {
		lParam &= ~WM_LBUTTONUP;
		lParam |= WM_RBUTTONUP;
	}

	// Delegate all the work back to the default 
    // implementation in CSystemTray.
    return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

// �^��C�x���g�̃��b�Z�[�W����
LRESULT CMainFrame::OnGraphNotify(WPARAM wParam, LPARAM lParam)
{
	long evCode = lParam;	// �C�x���g�R�[�h
	UINT uiStatus = wParam;	// �I�����R
	CString str = _T("");

	switch (uiStatus) {
        case 1:	// no window
			// �^����~���A�^���̂ݍĊJ
			if (m_pAudio->IsRecordingSkype()) {
				OnRecordStop();
				// �c�[���o�[�A�C�R���̋����ύX�i�^�恨�^���Ɉڍs���邽�߁j
				m_wndToolBar.ReplaceIcon(7, IDI_T_RECORD_PAUSE);
				m_wndToolBar.ReplaceIcon(8, IDI_T_RECORD_STOP);
				CString strDate = m_pAccessEvents->m_Setup.GetDateStr();
				if (!m_wndToolBar.RecordStart(strDate, TRUE, TRUE)) return NOERROR;
				str.LoadString(IDS_MSGBOX141);
			}
			break;
        case 2:	// capture size changed
			// �^����~���A�ĊJ
			if (m_pAudio->IsRecordingSkype()) {
				OnRecordStop();
				CString strDate = m_pAccessEvents->m_Setup.GetDateStr();
				if (!m_wndToolBar.VRecordStart(strDate, TRUE, TRUE)) return NOERROR;
				str.LoadString(IDS_MSGBOX142);
			}
			break;
        //case 3:	// window is invisible
		default:	// normal
			if (evCode == EC_USERABORT) {	// �t�@�C���T�C�Y����
				// �^����~���A�ĊJ
				if (m_pAudio->IsRecordingSkype()) {
					OnRecordStop();
					CString strDate = m_pAccessEvents->m_Setup.GetDateStr();
					if (!m_wndToolBar.VRecordStart(strDate, TRUE, TRUE)) return NOERROR;
					str.LoadString(IDS_MSGBOX143);
				}
			}
			break;
	}

	// �o���[�����b�Z�[�W�\��
	if (!str.IsEmpty()) {
		///// VRECORD�{�^����ɕ\��
		CToolBarCtrl &tbc = m_wndToolBar.GetToolBarCtrl();
		UINT idx = tbc.CommandToIndex(ID_VRECORD_START);
		RECT rc;
		tbc.GetItemRect(idx, &rc);
		CPoint pnt((rc.left+rc.right)/2, (rc.top+rc.bottom)/2);
		CBalloonHelp::LaunchBalloon(str, _T(""), pnt, IDI_INFORMATION, 
			CBalloonHelp::unSHOW_CLOSE_BUTTON|CBalloonHelp::unSHOW_TOPMOST, 
			&m_wndToolBar, _T(""), 10000);	// 10 sec
	}

	return NOERROR;
}

//  �X�J�C�v�Ď��C�x���g
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	_bstr_t bStr;
	CString str;
	CRegDWORD rgMaxRecord;
	CRegDWORD rgWaitAnswer;
	DWORD dw;
	CTimeSpan tms;

	switch (nIDEvent) {
	case ID_TIMER_APP:
	/// APP����m�F�^�C�}����
		m_stApp.lngPassTime++;	// �^�C�}�[�o�ߎ��Ԃ��X�V
		if (m_stApp.lngPassTime == 60) {	// 10 min each
			m_stApp.lngPassTime = 0;
			//// ����m�F�p��PING���b�Z�[�W�����O�ɐ���
			if (m_pAccessEvents->m_pCall == NULL) {		// �ʘb���͂��̑��̃��O����������̂ŕs�v
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("PING"));
				/// Skype�̐ڑ���Ԃ̊m�F
				if (theApp.m_blSkypeConnect) {
					// �ڑ����̂͂��Ȃ̂ɐڑ�����Ă��Ȃ��ꍇ�͍Đڑ�
					CString str;
					_bstr_t bStr;
					_bstr_t bStrRet;
					try {
						bStr = _bstr_t(_T("PING"));
						bStrRet = m_pAccessEvents->m_pAccess->SendBlockingCommand(bStr);
						str.Format(_T("%s"), (LPCTSTR)bStrRet);
						if (str.Find(_T("PONG")) < 0) {
							ConnectSkype();
						}
					} catch(_com_error& e) {
						ConnectSkype();
					} catch(...) {
						ConnectSkype();
					}
				}
			}
		}
		/// Skype�ڑ���ԃ`�F�b�N
		if (!theApp.m_blSkypeConnect && m_stConnect.blOnTimer == FALSE) {
			//// Skype�ɐڑ�����Ă��炸�A�X�J�C�v�ڑ��҂��^�C�}�����삵�Ă��Ȃ��ꍇ
			//// �X�J�C�v�ڑ��҂��^�C�}���N��
			StartConnectTimer();
		}
		goto OnTimer_END;
	case ID_TIMER_CONNECT:
	/// �X�J�C�v�ڑ��҂��^�C�}����
		m_stConnect.lngPassTime++;	// �^�C�}�[�o�ߎ��Ԃ��X�V
		if (!theApp.m_blSkypeConnect) {		// �ڑ��t���O
			/// Skype�ɐڑ�����Ă��Ȃ��ꍇ�A�ڑ�����
			if (ConnectSkype()) {
				//// �ڑ��ł����ꍇ�A�X�J�C�v�ڑ��҂��^�C�}���~
				StopConnectTimer();
			}
		}
		goto OnTimer_END;
	case ID_TIMER_RECORD_MAX:
	/// ����Ԙ^��MAX���ԃ`�F�b�N�^�C�}����
		m_stRecordMax.lngPassTime++;	// �^�C�}�[���s�񐔂��X�V
		m_stRecordMax.tmPass = CTime::GetCurrentTime();	// �^�C�}�[�o�ߎ��Ԃ��X�V
		tms = m_stRecordMax.tmPass - m_stRecordMax.tmStart;
		m_stRecordMax.lngSpanTime = (long)tms.GetTotalSeconds();
		/// �^��MAX���Ԃ��o�߂����ꍇ�A�ؒf�i�ۑ��j����
		rgMaxRecord = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\MaxRecordTime"), 180, TRUE);
		dw = rgMaxRecord;
		if ((long)dw < m_stRecordMax.lngSpanTime) {
			StopRecordMaxTimer();
			/// �^���^��im_ProcCode�̍X�V�O�ɌĂяo���j
			m_pAccessEvents->FinishRecord();
			// ���̉���
			m_pAccessEvents->m_ProcCode += _T("$");		
			m_pAccessEvents->Response();
		}

		/// �ʘb���I�����Ă���̂�prgFinished�C�x���g���������Ȃ��ꍇ�A�I�������R�[��
		if(m_pAccessEvents->m_pCall != NULL) {
			if (m_pAccessEvents->m_pCall->Status == prgFinished) {
				te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("Force event:prgFinished"));
				m_pAccessEvents->CallStatusChanged(NULL, prgFinished);
			}
		}

		break;
	case ID_TIMER_WAIT_ANSWER:
	/// ����ԉ����҂��^�C�}����
		m_stAnswerWait.lngPassTime++;	// �^�C�}�[�o�ߎ��Ԃ��X�V
		if(m_pAccessEvents->m_pCall != NULL && m_pAccessEvents->m_pCall->Status != prgInProgress) {
			/// �����ҋ@���Ԃ��o�߂����ꍇ�A��������
			rgWaitAnswer = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\WaitAnswerTime"), 10, TRUE);
			dw = rgWaitAnswer;
			if (dw > 0 && (long)dw <= m_stAnswerWait.lngPassTime) {
				StopAnswerWaitTimer();
				if (m_pAudio->IsPlaying()) {
					m_pAudio->PlayStop();
				} 
				if (m_pAudio->IsPlayingSkype()) {
					m_pAudio->PlayStopSkype();
				} 
				m_pAccessEvents->m_ProcCode = _T("$");		// �ŏ��̉���
				//str.Format(_T("SET CALL %d STATUS INPROGRESS"), m_pAccessEvents->m_CallStatus.lngCallId);
				//m_pAccessEvents->m_pAccess->SendCommand(_bstr_t((LPCTSTR)str));	
				m_pAccessEvents->m_pCall->Status = prgInProgress;
			}
		}
		goto OnTimer_END;
	case ID_TIMER_PLAY_CHK:
	/// �������b�Z�[�W�Đ��I���҂��^�C�}����
		m_stPlayChk.lngPassTime++;	// �^�C�}�[���s�񐔂��X�V
		m_stPlayChk.tmPass = CTime::GetCurrentTime();	// �^�C�}�[�o�ߎ��Ԃ��X�V
		tms = m_stPlayChk.tmPass - m_stPlayChk.tmStart;
		m_stPlayChk.lngSpanTime = (long)tms.GetTotalSeconds();
		/// SKYPE�r�f�I�f�o�C�X�ύX
		// �C�x���g�������ł̓t���[���t�@�C������������Ȃ����߁A�����łP�񂾂��R�[��
		if (m_stPlayChk.lngPassTime == 1 && m_pVideo->IsPlayingSkype()) {
			m_pAccessEvents->SetSkypeVDevice(TVC_IN_DEV);
			m_pAccessEvents->RefreshSkypeVideo(m_pAccessEvents->m_CallStatus.lngCallId);
		}
		/// �Đ��������ԂɒB�����ꍇ�A���邢�͍Đ����b�Z�[�W���I�������ꍇ�A���̏����ֈڍs����
		//if ((m_stPlayChk.lngLimitTime <= m_stPlayChk.lngPassTime) ||
		//   (!m_pAudio->IsPlayingSkype() && !m_pVideo->IsPlayingSkype())) {
		if (m_stPlayChk.lngLimitTime <= m_stPlayChk.lngSpanTime) {
			if (m_pAudio->IsPlaying()) {
				m_pAudio->PlayStop();
			} 
			if (m_pAudio->IsPlayingSkype()) {
				m_pAudio->PlayStopSkype();
			} 
			if (m_pVideo->IsPlayingSkype()) {
				m_pVideo->PlayStopSkype();
			} 
			StopPlayChkTimer();
			// ���̉����������R�[��
			//m_pAccessEvents->m_ProcCode += _T("$");		�`���b�g�݂̂̏ꍇ������̂ł����Őݒ�s��
			m_pAccessEvents->Response();
		}
		break;
	default:
		goto OnTimer_END;
	}
	
OnTimer_END:
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CFrameWnd::OnSizing(fwSide, pRect);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	/// �E�B���h�E�̃T�C�Y����
	if ((pRect->right - pRect->left) < MIN_WINSZX) pRect->right = pRect->left + MIN_WINSZX;
	if ((pRect->bottom - pRect->top) < MIN_WINSZY) pRect->bottom = pRect->top + MIN_WINSZY;
}

/*
// KEY HOOK�����C�x���g
LRESULT CMainFrame::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
    KEYRESULT kr; // The struct to receive information

    // Information that we need to extract are event type and printable character
    UINT nMask = KH_MASK_EVENTTYPE | KH_MASK_PRINTABLECHAR;
 
    // This function extracts event details
    LRESULT lRes = GetKeyEventResult(wParam, lParam, &kr, nMask);
    ASSERT(lRes == KH_OK);

    // We only display key-strokes that produce printable characters
    if (kr.chPrintableChar != 0) {
        if (kr.iKeyEvent == KH_KEY_DOWN) {
			if (kr.chPrintableChar == 't' || kr.chPrintableChar == 'T') {
				/// ���W�X�g���̃`�F�b�N
				CRegDWORD rgShortCut = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\ShortCutTapur", 1);
				DWORD dw = rgShortCut;
				if (dw == 1) {
					/// Tapur��ʕ\��
					OnPopupOpen();
				}
			} else if (kr.chPrintableChar == 's' || kr.chPrintableChar == 'S') {
				/// ���W�X�g���̃`�F�b�N
				CRegDWORD rgShortCut = CRegDWORD("Software\\Tapur\\Tapur\\Settings\\ShortCutSkype", 1);
				DWORD dw = rgShortCut;
				if (dw == 1) {
					/// SKype�N��or��ʕ\��
					CRegString rgShortCut = CRegString("Software\\Skype\\Phone\\SkypePath");
					CString str = rgShortCut;
					if (str.GetLength() > 0) {
						HINSTANCE ret = ShellExecute(NULL, NULL, str,  NULL, NULL, SW_SHOWNORMAL);
	//					if ((int)ret <= 32) {
	//					}
					}
				}
			}
		}
    }

    return (LRESULT)0;
}
*/

// MainTool�ւ̉�b�^���֘A�C�x���g�]��
void CMainFrame::OnVRecordStart() 
{
	CString strDate = m_pAccessEvents->m_Setup.GetDateStr();
	m_wndToolBar.VRecordStart(strDate, TRUE);
}
void CMainFrame::OnRecordStart() 
{
	CString strDate = m_pAccessEvents->m_Setup.GetDateStr();
	m_wndToolBar.RecordStart(strDate, TRUE);
}
void CMainFrame::OnRecordPause() 
{
	m_wndToolBar.VRecordPause();
	m_wndToolBar.RecordPause();
}
void CMainFrame::OnRecordStop() 
{
	if (m_wndToolBar.VRecordStop(TRUE)) {
		m_wndToolBar.RecordStop(FALSE);
	} else {
		m_wndToolBar.RecordStop(TRUE);
	}
}
void CMainFrame::OnRecordMod() 
{
	m_wndToolBar.RecordMod();
}
/*
void CMainFrame::OnRecordModDropDown(NMTOOLBAR* pnmtb, LRESULT *plr)
{
	if (pnmtb->iItem == ID_VIEW_DETAILS) {
		m_wndToolBar.RecordMod();
	}
}
*/
BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

#define lpnm   ((LPNMHDR)lParam)
#define lpnmTB ((LPNMTOOLBAR)lParam)

	switch(lpnm->code)
	{
	case TBN_DROPDOWN:
		m_wndToolBar.RecordMod();
		return FALSE; //indicates the TBN_DROPDOWN
					  //notification was handled.
	}
	
	return CFrameWnd::OnNotify(wParam, lParam, pResult);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	// �E�B���h�E�N���X���̎w��
    LPCTSTR   lpzsNewName = _T(MAIN_CLASS_NAME);
    WNDCLASS wndcls;
    BOOL bRes = CFrameWnd::PreCreateWindow( cs );   // MDI �ł� CMDIFrameWnd
    HINSTANCE hInst = AfxGetInstanceHandle();
    // see if the class already exists
    if ( !::GetClassInfo( hInst, lpzsNewName, &wndcls ) )
    {
        // get default stuff
        ::GetClassInfo( hInst, cs.lpszClass, &wndcls );
        // register a new class
        wndcls.lpszClassName = lpzsNewName;
        wndcls.hIcon = ::LoadIcon(hInst,MAKEINTRESOURCE(IDI_MAIN));
        ::RegisterClass( &wndcls );
    }
    cs.lpszClass = lpzsNewName;
    return bRes;

	//return __super::PreCreateWindow(cs);
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

	/// HotKey�̏���
	UINT uiMod = MOD_CONTROL | MOD_SHIFT;
	CMsgForm *pMsgForm = NULL;
	CMsgList *pMsgList = NULL;
	int idx;

	if(message == WM_HOTKEY) {
		CSetHotkey dlg;
		dlg.LoadRegistry();

		if(LOWORD(wParam) == ID_HK_SHOW) {
			OnPopupOpen();		// ���C����ʕ\��
		} else if(LOWORD(wParam) == ID_HK_HIDE) {
			OnClose();			// ���C����ʔ�\��
		} else if(LOWORD(wParam) == ID_HK_RECORD) {
			OnRecordStart();	// �^��
		} else if(LOWORD(wParam) == ID_HK_VRECORD) {
			OnVRecordStart();	// �^��
		} else if(LOWORD(wParam) == ID_HK_REC_PAUSE) {
			OnRecordPause();	// �^���E�^��ꎞ��~
		} else if(LOWORD(wParam) == ID_HK_REC_STOP) {
			OnRecordStop();		// �^��I��
		} else if(LOWORD(wParam) == ID_HK_PLAY) {
			// �Đ�
			if(!IsWindowVisible()) return 0;	// �N�����x���\������Ă��Ȃ��ꍇ�́A���X�g��ʂ���������Ă��Ȃ��i���Ԃ�������̂Łj
			pMsgForm = (CMsgForm *)m_pMsgForm->GetWnd();
			pMsgList = (CMsgList *)m_pMsgList->GetWnd();
			// ���݂̑I�����X�g
			idx = pMsgList->SelectedId();
			// �I�����ꂽ���X�g���Ȃ��ꍇ�ŏ��̃A�C�e����I��
			if (idx < 0) {
				idx = 0;
				pMsgList->SelectId(idx);
			}
			pMsgForm->Play();
		} else if(LOWORD(wParam) == ID_HK_PLAY_PAUSE) {
			// �Đ��ꎞ��~
			if(!IsWindowVisible()) return 0;	// �N�����x���\������Ă��Ȃ��ꍇ�́A���X�g��ʂ���������Ă��Ȃ��i���Ԃ�������̂Łj
			pMsgForm = (CMsgForm *)m_pMsgForm->GetWnd();
			pMsgForm->PlayPause();
		} else if(LOWORD(wParam) == ID_HK_PLAY_STOP) {
			// �Đ��I��
			if(!IsWindowVisible()) return 0;	// �N�����x���\������Ă��Ȃ��ꍇ�́A���X�g��ʂ���������Ă��Ȃ��i���Ԃ�������̂Łj
			pMsgForm = (CMsgForm *)m_pMsgForm->GetWnd();
			pMsgForm->PlayStop();
		} else if(LOWORD(wParam) == ID_HK_PLAY_NEXT) {
			// ���̍Đ�
			if(!IsWindowVisible()) return 0;	// �N�����x���\������Ă��Ȃ��ꍇ�́A���X�g��ʂ���������Ă��Ȃ��i���Ԃ�������̂Łj
			pMsgForm = (CMsgForm *)m_pMsgForm->GetWnd();
			pMsgList = (CMsgList *)m_pMsgList->GetWnd();
			// ���݂̑I�����X�g
			idx = pMsgList->SelectedId();
			// �����łȂ���Ύ��̃��X�g��I��
			idx++;
			pMsgList->SelectId(idx);
			pMsgForm->Play();
		} else if(LOWORD(wParam) == ID_HK_PLAY_PREV) {
			// �O�̍Đ�
			if(!IsWindowVisible()) return 0;	// �N�����x���\������Ă��Ȃ��ꍇ�́A���X�g��ʂ���������Ă��Ȃ��i���Ԃ�������̂Łj
			pMsgForm = (CMsgForm *)m_pMsgForm->GetWnd();
			pMsgList = (CMsgList *)m_pMsgList->GetWnd();
			// ���݂̑I�����X�g
			idx = pMsgList->SelectedId();
			// �擪�łȂ���ΑO�̃��X�g��I��
			idx--;
			pMsgList->SelectId(idx);
			pMsgForm->Play();
		}
		return 0;
	}


	/// �^�X�N�g���C�A�C�R���ƃc�[���`�b�v�̍X�V
	CString str;
	CString strTip;
	HICON hIcon = m_TrayIcon.GetIcon();
	if (theApp.m_blSkypeConnect) {
		/// Skype�ڑ���
		if (theApp.m_lngAppStatus == 1) {
			/// �^���E�^�撆�A�j���̕\��
			if (!m_TrayIcon.IsAnimating()) {
				m_TrayIcon.Animate(1000, -1);
				//// tooltip�̕ύX
				str.LoadString(IDS_TOOLTIP_REC);
				m_TrayIcon.SetTooltipText(str);
				//// balloon�\��
				m_TrayIcon.ShowBalloon(str, NULL, NIIF_INFO, 10);	// ? 10 sec �ȏ�K�{
			}
		} else if (theApp.m_lngAppStatus == 2) {
			/// ����ԉ������A�j���̕\��
			if (!m_TrayIcon.IsAnimating()) {
				m_TrayIcon.Animate(1000, -1);
				//// tooltip�̕ύX
				str.LoadString(IDS_TOOLTIP_RCV);
				m_TrayIcon.SetTooltipText(str);
				//// balloon�\��
				m_TrayIcon.ShowBalloon(str, NULL, NIIF_INFO, 10);	// ? 10 sec �ȏ�K�{
			}
		} else {
			/// ����ԉ���/�^���^�撆�łȂ�
			if (m_TrayIcon.IsAnimating()) {
				//// �A�j���̒��~
				m_TrayIcon.StopAnimation();
			}
			if (m_lngNewMsg > 0) {
				/// �V�����b�Z�[�W������ꍇ
				if (hIcon != m_hIconNew) {
					m_TrayIcon.SetIcon(m_hIconNew);
					str.LoadString(IDS_TOOLTIP_NEW);
					strTip.Format(_T("%s(%d)"), str, m_lngNewMsg);
					m_TrayIcon.SetTooltipText(strTip);
				}
			} else {
				/// �V�����b�Z�[�W���Ȃ��ꍇ
				if (hIcon != m_hIcon) {
					m_TrayIcon.SetIcon(m_hIcon);
					InitTooltip();	// �c�[���`�b�v�̏�����
				}
			}
		}
	} else {
		/// Skype�ڑ��s��
		if (hIcon != m_hIconDis) {
			m_TrayIcon.SetIcon(m_hIconDis);
			m_TrayIcon.SetTooltipText(IDS_TOOLTIP_DISCON);
		}
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

// �^���ΏۑI���|�b�v�A�b�v�C�x���g
/// �^���ΏۑI���{�^���̍X�V
void CMainFrame::OnRecordModBoth()
{
	/// �^���Ώۏ�ԃ��W�X�g���̍X�V
	CRegDWORD rgRecMod(_T("Software\\Tapur\\Tapur\\Settings\\RecordMod"), 0, TRUE);
	rgRecMod = 0;

	/// �^���ΏۑI���{�^���̍X�V
	m_wndToolBar.UpadateRecordMod();
}
void CMainFrame::OnRecordModRcv()
{
	/// �^���Ώۏ�ԃ��W�X�g���̍X�V
	CRegDWORD rgRecMod(_T("Software\\Tapur\\Tapur\\Settings\\RecordMod"), 0, TRUE);
	rgRecMod = 1;

	/// �^���ΏۑI���{�^���̍X�V
	m_wndToolBar.UpadateRecordMod();
}
void CMainFrame::OnRecordModSnd()
{
	/// �^���Ώۏ�ԃ��W�X�g���̍X�V
	CRegDWORD rgRecMod(_T("Software\\Tapur\\Tapur\\Settings\\RecordMod"), 0, TRUE);
	rgRecMod = 2;

	/// �^���ΏۑI���{�^���̍X�V
	m_wndToolBar.UpadateRecordMod();
}

/////////////////////////////////////////////////////////////////////////////

//
//  �@�\     : ��ʂ̍X�V
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
void CMainFrame::UpdateForm() 
{
	/// ��ʂ��\������Ă��鎞�̂ݗL��
	//if (!(this->IsWindowVisible())) return;

	/// WINDOW�^�C�g��
	CString str;
	if (theApp.m_blSkypeConnect) {
		str.LoadString(IDS_CAPTION_CON);
	} else {
		str.LoadString(IDS_CAPTION_DISCON);
	}
//	if (theApp.m_strUsrName.IsEmpty()) {
		str += _T(" - ") + theApp.m_strUsrId;
//	} else {
//		str += _T(" - ") + theApp.m_strUsrName;
//	}
	this->SetWindowText(str);

	/// �t�H���_�c���[�\��
	CMsgTree *pMtree = (CMsgTree *)m_pMsgTree->GetWnd();
	pMtree->UpdateForm(TRUE);
	CRsfTree *pRtree = (CRsfTree *)m_pRsfTree->GetWnd();
	pRtree->UpdateForm(TRUE);
}

//
//  �@�\     : �c�[���`�b�v�̏�����
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
void CMainFrame::InitTooltip() 
{
	CString str;
	CString strTip;

	strTip.LoadString(IDS_TOOLTIP);
	str.Empty();
	if (m_flgAutoEnable == 1) {
		str.LoadString(IDS_TOOLTIP_AUTO_ENABLE);
		strTip += _T("(") + str;
	}
	if (m_flgEnable == 0) {
		if (str.GetLength() > 0) {
			strTip += _T("/");
		} else {
			strTip += _T("(");
		}
		str.LoadString(IDS_TOOLTIP_DISABLE);
		strTip += str;
	}
	if (str.GetLength() > 0) {
		strTip += _T(")");
	}

	m_TrayIcon.SetTooltipText(strTip);
}

//
//  �@�\     : �X�J�C�v�A�N�Z�X�I�u�W�F�N�g�̐���
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::InitSkypeAccess(void)
{
	/// IAccess�C���^�[�t�F�[�X�I�u�W�F�N�g�̍쐬
	HRESULT hr = CoCreateInstance(CLSID_Access, NULL, CLSCTX_INPROC_SERVER, IID_IAccess, 
		(void**)&theApp.m_pAccess);
	if (FAILED(hr))
	{
		return FALSE;
	}

	/// �V���N�I�u�W�F�N�g�̍쐬
	CComObject<CAccessEvents>::CreateInstance(&m_pAccessEvents);
	if (!m_pAccessEvents)
	{
		theApp.m_pAccess = NULL;
		return FALSE;
	}
	
	/// IUnknown�C���^�[�t�F�[�X�̎擾
	CComPtr<IUnknown> pEventUnk = m_pAccessEvents;
	
	/// �V���N�I�u�W�F�N�g�̒ʒm
	hr = AtlAdvise(theApp.m_pAccess, pEventUnk, DIID__IAccessEvents, &m_dwCookie);
	if (FAILED(hr))
	{
		theApp.m_pAccess = NULL;
		pEventUnk = NULL;
		return FALSE;
	}
	
	/// IAccessEvent�C���^�[�t�F�[�X�֐ݒ�R�s�[
	m_pAccessEvents->SetMyParent(m_hWnd);
	m_pAccessEvents->SetAccess(theApp.m_pAccess);

	return TRUE;
}

//
//  �@�\     : Skype�ɐڑ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::ConnectSkype(void)
{
	try {
		/// Skype process �̑��݊m�F
		DWORD dwId;	// return value
		CKillProcess kp;
		HANDLE hProcess = kp.FindProcess(SKYPE_NAME, dwId);
		if(!hProcess) return FALSE;

		/// �ڑ��m�F��ʂ��\�����̏ꍇ�AConnect���Ȃ��i�ڑ���ʂ̏d���������j(3.x�̂�)
		HWND hwnd = ::FindWindow(_T("TskACLForm.UnicodeClass"), NULL);	//3.x
		if (hwnd) return FALSE;
		/// ���O�C����ʂ��\�����̏ꍇ�AConnect���Ȃ��i4.x�̂݁j
		hwnd = ::FindWindow(_T("TLoginForm.UnicodeClass"), NULL);	//4.x
		if (hwnd) return FALSE;

		/// Skype API �ɐڑ�
//te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Connect1"));
		HRESULT hr = theApp.m_pAccess->Connect();
		if (FAILED(hr)) {
//te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Connect2"));
			return FALSE;
		}
//te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Connect3"));

// �ȉ��̂悤�ȏ�����Connect��̃C�x���g�������ς܂Ȃ��Ǝ��s�ł��Ȃ�
		/// PROTOCOL �w��
//		theApp.m_pAccess->Protocol = 6;	// skype3.0�`
		/// ���ۂɎg�p�\��PROTOCOL�̎擾
//		m_pAccessEvents->m_lngProtocol = theApp.m_pAccess->Protocol;
//_stprintf_s(te.errstr, MAX_ERRSTR, _T("Skype Protocol:%d"), m_pAccessEvents->m_lngProtocol);
//te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
// ���x���Đڑ��������ꍇ������̂ŁA���b�Z�[�W�E�B���h�E�͓K���Ȃ��B
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, _T("Tapur can't connect to Skype. Please check Skype is online."));
		//AfxMessageBox(_T("Tapur can't connect to Skype. Please check Skype is online."), 
		//	MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
		return FALSE;
	}

	return TRUE;
}

//
//  �@�\     : �X�J�C�v�ڑ��҂��^�C�}�J�n
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::StartConnectTimer()
{
	if (!m_stConnect.blOnTimer) {
		SetTimer(ID_TIMER_CONNECT, 5000, NULL);	// 5 sec each
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StartConnectTimer"));
	}
	m_stConnect.blOnTimer = TRUE;		// �^�C�}�[���s���t���O
	m_stConnect.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A

	return TRUE;
}

//
//  �@�\     : �X�J�C�v�ڑ��҂��^�C�}�I��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::StopConnectTimer()
{
	try {
		if (m_stConnect.blOnTimer) {
			KillTimer(ID_TIMER_CONNECT);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopConnectTimer"));
		}
		m_stConnect.blOnTimer = FALSE;	// �^�C�}�[���s���t���O
		m_stConnect.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A
	} catch(...) {
		// CMainFrame�̐�������������O�ɌĂ΂��ꍇ�����邽��
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("Skype connect timer error."));
		return FALSE;
	}

	return TRUE;
}

//
//  �@�\     : APP����m�F�^�C�}�J�n
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::StartAppTimer()
{
	if (!m_stApp.blOnTimer) {
		SetTimer(ID_TIMER_APP, 10000, NULL);	// 10 sec each
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("StartAppTimer:%s %s"), _T(POTM_APPNAME), _T(POTM_VERSION));
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, te.errstr);
	}
	m_stApp.blOnTimer = TRUE;		// �^�C�}�[���s���t���O
	m_stApp.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A

	return TRUE;
}

//
//  �@�\     : APP����m�F�^�C�}�I��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::StopAppTimer()
{
	if (m_stApp.blOnTimer) {
		KillTimer(ID_TIMER_APP);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopAppTimer"));
	}
	m_stApp.blOnTimer = FALSE;	// �^�C�}�[���s���t���O
	m_stApp.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A

	return TRUE;
}

//
//  �@�\     : ����ԉ����҂��^�C�}�J�n
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::StartAnswerWaitTimer()
{
	if (!m_stAnswerWait.blOnTimer) {
		SetTimer(ID_TIMER_WAIT_ANSWER, 1000, NULL);	// 1 sec each
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StartAnswerWaitTimer"));
	}
	m_stAnswerWait.blOnTimer = TRUE;		// �^�C�}�[���s���t���O
	m_stAnswerWait.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A

	return TRUE;
}

//
//  �@�\     : ����ԉ����҂��^�C�}�I��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::StopAnswerWaitTimer()
{
	if (m_stAnswerWait.blOnTimer) {
		KillTimer(ID_TIMER_WAIT_ANSWER);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopAnswerWaitTimer"));
	}
	m_stAnswerWait.blOnTimer = FALSE;	// �^�C�}�[���s���t���O
	m_stAnswerWait.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A

	return TRUE;
}

//
//  �@�\     : ����Ԙ^��MAX���ԃ`�F�b�N�^�C�}�J�n
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::StartRecordMaxTimer()
{
	if (!m_stRecordMax.blOnTimer) {
		SetTimer(ID_TIMER_RECORD_MAX, 1000, NULL);	// 1 sec each
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StartRecordMaxTimer"));
	}
	m_stRecordMax.blOnTimer = TRUE;		// �^�C�}�[���s���t���O
	m_stRecordMax.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A
	m_stRecordMax.tmStart = CTime::GetCurrentTime();		// �^�C�}�[�J�n�������Z�b�g

	return TRUE;
}

//
//  �@�\     : ����Ԙ^��MAX���ԃ`�F�b�N�^�C�}�I��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::StopRecordMaxTimer()
{
	if (m_stRecordMax.blOnTimer) {
		KillTimer(ID_TIMER_RECORD_MAX);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopRecordMaxTimer"));
	}
	m_stRecordMax.blOnTimer = FALSE;	// �^�C�}�[���s���t���O
	m_stRecordMax.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A

	return TRUE;
}

//
//  �@�\     : �������b�Z�[�W�Đ��I���҂��^�C�}�J�n
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::StartPlayChkTimer(
long lngPlayTime)	// PLAY����
{
	if (!m_stPlayChk.blOnTimer) {
		SetTimer(ID_TIMER_PLAY_CHK, 1000, NULL);	// 1 sec each(must)
CString str;
str.Format(_T("StartPlayChkTimer:%d"), lngPlayTime);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, str);
	}
	m_stPlayChk.blOnTimer = TRUE;		// �^�C�}�[���s���t���O
	m_stPlayChk.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A
	m_stPlayChk.lngLimitTime = lngPlayTime;		// �^�C�}�[�������Ԃ��Z�b�g
	m_stPlayChk.tmStart = CTime::GetCurrentTime();		// �^�C�}�[�J�n�������Z�b�g

	return TRUE;
}

//
//  �@�\     : �������b�Z�[�W�Đ��I���҂��^�C�}�I��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::StopPlayChkTimer()
{
	if (m_stPlayChk.blOnTimer) {
		KillTimer(ID_TIMER_PLAY_CHK);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopPlayChkTimer"));
	}
	m_stPlayChk.blOnTimer = FALSE;		// �^�C�}�[���s���t���O
	m_stPlayChk.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A
	m_stPlayChk.lngLimitTime = 0;		// �^�C�}�[�������Ԃ��N���A

	return TRUE;
}

//
//  �@�\     : TMP�t�H���_�̃N���A
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::ClearTmpData(void)
{
	if (theApp.m_strDataFolder.IsEmpty()) return TRUE;

	CString strFolder = theApp.m_strDataFolder + _T(RDF_TMP_PATH);
	CFileSpec fs(strFolder);
	fs.ClearFolder();

	strFolder = theApp.m_strDataFolder + _T(RSF_TMP_PATH);
	fs.SetFullSpec(strFolder);
	fs.ClearFolder();

	return TRUE;
}

//
//  �@�\     : ���o�^�̈ꎞ�f�[�^�̏C��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : �C���f�[�^��, -1:error
//  
//  ���l     : 
//  
long CMainFrame::RepairTmpData(
long lngChkHour)	// �`�F�b�N����f�[�^�̊��ԁi�w�莞��(h)�O�܂Łj
{
	long lngRet = 0;
	CString strAudioFile;
	CString strVideoFile;
	CFileFind finder;
	CTime ntm;
	CTime tm;
	CTimeSpan tms;
	CFileSpec fs;

	if (theApp.m_strDataFolder.IsEmpty()) return TRUE;

	CTblRdfl tbl(theApp.m_strDBPath);
	CString strVideoFolder = m_pAccessEvents->m_Setup.RdfVideoFolder();

	TRY {
		// WAV�����t�@�C��
		CString strAudioFolder = m_pAccessEvents->m_Setup.RdfWavFolder();
		if (!SetCurrentDirectory(strAudioFolder)) return 0;
		ntm = CTime::GetCurrentTime();
		BOOL bWorking = finder.FindFile(_T("rdf-*.wav"));
		if (!bWorking) return 0;

		while (bWorking)
		{
			BOOL blRepair = FALSE;
			bWorking = finder.FindNextFile();
			if (!finder.IsDirectory()) {
				strAudioFile = finder.GetFileName();
				/// WAV�t�@�C���쐬���Ԃƌ��݂Ƃ̍���
				finder.GetCreationTime(tm);
				tms = ntm - tm;
				if (tms.GetTotalHours() <= lngChkHour) {
					fs.SetFullSpec(strAudioFile);
					if (!fs.Exist()) continue;
					strVideoFile = fs.FileName() + _T(".avi");
					if (!fs.Exist()) strVideoFile.Empty();
					/// �����t�@�C���̏C��
					if (m_pAudio->RepairWaveFile(strAudioFile) > 0) {
						blRepair = TRUE;
					}
					AfxGetThread()->PumpMessage();
					/// DB�ɓo�^
					if (m_pAccessEvents->FinishRecordTbl(strAudioFile, strVideoFile, TRUE)) {
						// �o�^���ꂽ�ꍇ�i�����̃f�[�^���Ȃ������ꍇ�j
						blRepair = TRUE;
					}
					if (blRepair) lngRet++;
				}
			}
		}
		finder.Close();

		// MP3�����t�@�C��
		strAudioFolder = m_pAccessEvents->m_Setup.RdfMp3Folder();
		if (!SetCurrentDirectory(strAudioFolder)) return 0;
		ntm = CTime::GetCurrentTime();
		bWorking = finder.FindFile(_T("rdf-*.mp3"));
		if (!bWorking) return 0;

		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (!finder.IsDirectory()) {
				strAudioFile = finder.GetFileName();
				/// MP3�t�@�C���쐬���Ԃƌ��݂Ƃ̍���
				finder.GetCreationTime(tm);
				tms = ntm - tm;
				if (tms.GetTotalHours() <= lngChkHour) {
					fs.SetFullSpec(strAudioFile);
					if (!fs.Exist()) continue;
					strVideoFile = fs.FileName() + _T(".avi");
					if (!fs.Exist()) strVideoFile.Empty();
					/// DB�ɓo�^
					if (m_pAccessEvents->FinishRecordTbl(strAudioFile, strVideoFile)) {
					// �o�^���ꂽ�ꍇ�i�����̃f�[�^���Ȃ������ꍇ�j
						AfxGetThread()->PumpMessage();
						lngRet++;
					}
				}
			}
		}
		finder.Close();
	}
    CATCH_ALL(e)
    {
		finder.Close();
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("RepairTmpData error"));
		return -1;
    }
	END_CATCH_ALL

	return lngRet;
}

//
//  �@�\     : �V�����b�Z�[�W���X�V
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMainFrame::IncNewMsgCnt(
long lngCnt)		// �ǉ����郁�b�Z�[�W���A0�ȉ��F���Z�b�g
{
	if (lngCnt <= 0) {
		m_lngNewMsg = 0;
	} else {
		m_lngNewMsg += lngCnt;
		/// �c�[���`�b�v�̍X�V
		CString str;
		CString strTip;
		str.LoadString(IDS_TOOLTIP_NEW);
		strTip.Format(_T("%s(%d)"), str, m_lngNewMsg);
		m_TrayIcon.SetTooltipText(strTip);
	}

	return TRUE;
}

//
//  �@�\     : HotKey�̐ݒ�^����
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
void CMainFrame::RegHotKey()
{
	m_blHotKey = FALSE;

	/// HotKey�̎g�p��
	CRegDWORD rgDW = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableHotKey"), 1, TRUE);
	DWORD dwFlg = (long)rgDW;
	if (dwFlg == 0) return;

	m_blHotKey = TRUE;

	/// �ڍאݒ�̎擾
	CSetHotkey dlg;
	dlg.LoadRegistry();
	// ���C����ʕ\��
	if (dlg.m_rgEnableShow == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_SHOW, dlg.m_rgModShow, dlg.m_rgKeyShow);
	}
	// ���C����ʔ�\��
	if (dlg.m_rgEnableHide == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_HIDE, dlg.m_rgModHide, dlg.m_rgKeyHide);
	}
	//// �^��
	if (dlg.m_rgEnableRecord == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_RECORD, dlg.m_rgModRecord, dlg.m_rgKeyRecord);
	}
	//// �^��
	if (dlg.m_rgEnableVRecord == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_VRECORD, dlg.m_rgModVRecord, dlg.m_rgKeyVRecord);
	}
	//// �^���E�^��ꎞ��~
	if (dlg.m_rgEnableRecPause == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_REC_PAUSE, dlg.m_rgModRecPause, dlg.m_rgKeyRecPause);
	}
	//// �^���E�^��I��
	if (dlg.m_rgEnableRecStop == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_REC_STOP, dlg.m_rgModRecStop, dlg.m_rgKeyRecStop);
	}
	//// �Đ��F���݃��X�g�őI������Ă��郌�R�[�h
	if (dlg.m_rgEnablePlay == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_PLAY, dlg.m_rgModPlay, dlg.m_rgKeyPlay);
	}
	//// �Đ��ꎞ��~�E�ĊJ
	if (dlg.m_rgEnablePlayPause == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_PLAY_PAUSE, dlg.m_rgModPlayPause, dlg.m_rgKeyPlayPause);
	}
	//// �Đ��I��
	if (dlg.m_rgEnablePlayStop == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_PLAY_STOP, dlg.m_rgModPlayStop, dlg.m_rgKeyPlayStop);
	}
	//// ���̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̎��̃��R�[�h
	if (dlg.m_rgEnablePlayNext == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_PLAY_NEXT, dlg.m_rgModPlayNext, dlg.m_rgKeyPlayNext);
	}
	//// �O�̍Đ��F���݃��X�g�őI������Ă��郌�R�[�h�̑O�̃��R�[�h
	if (dlg.m_rgEnablePlayPrev == 1) {
		::RegisterHotKey(m_hWnd, ID_HK_PLAY_PREV, dlg.m_rgModPlayPrev, dlg.m_rgKeyPlayPrev);
	}
}
void CMainFrame::UnregHotKey()
{
	if (m_blHotKey == FALSE) return;

	CSetHotkey dlg;
	dlg.LoadRegistry();

	::UnregisterHotKey(m_hWnd, ID_HK_SHOW);
	::UnregisterHotKey(m_hWnd, ID_HK_HIDE);
	::UnregisterHotKey(m_hWnd, ID_HK_RECORD);
	::UnregisterHotKey(m_hWnd, ID_HK_VRECORD);
	::UnregisterHotKey(m_hWnd, ID_HK_REC_PAUSE);
	::UnregisterHotKey(m_hWnd, ID_HK_REC_STOP);
	::UnregisterHotKey(m_hWnd, ID_HK_PLAY);
	::UnregisterHotKey(m_hWnd, ID_HK_PLAY_PAUSE);
	::UnregisterHotKey(m_hWnd, ID_HK_PLAY_STOP);
	::UnregisterHotKey(m_hWnd, ID_HK_PLAY_NEXT);
	::UnregisterHotKey(m_hWnd, ID_HK_PLAY_PREV);
}
