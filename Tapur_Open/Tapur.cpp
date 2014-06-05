// Tapur.cpp : �A�v���P�[�V�����p�N���X�̋@�\��`���s���܂��B
//

#include "stdafx.h"
#include <locale.h>
//#ifdef _DEBUG
//#include "vld.h"
//#endif
#include "Tapur.h"
#include "MainFrm.h"
#include "TapurDoc.h"
#include "TapurView.h"
#include <initguid.h>
#include "Tapur_i.c"
#include "Tokenizer.h"
#include "FileSpec.h"
#include "TPsetup.h"
#include "TMsgBox.h"
#include "BackupDialog.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CTapurApp

BEGIN_MESSAGE_MAP(CTapurApp, CWinApp)
	//{{AFX_MSG_MAP(CTapurApp)
	ON_COMMAND(ID_APP_HELP, OnAppHelp)
	ON_COMMAND(ID_LINK_LAME, OnLinkLame)
	ON_COMMAND(ID_LINK_LICENSE, OnLinkLicense)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_TOOL_SETTING, OnToolSetting)
	ON_COMMAND(ID_TOOL_BACKUP, OnToolBackup)
	ON_COMMAND(ID_WAV_FOLDER, OnWavFolder)
	ON_COMMAND(ID_MP3_FOLDER, OnMp3Folder)
	ON_COMMAND(ID_VIDEO_FOLDER, OnVideoFolder)
	ON_COMMAND(ID_REPAIR, OnRepair)
	ON_COMMAND(ID_SOUND_RECORDER, OnSoundRecoder)
	ON_COMMAND(ID_AUDACITY, OnAudacity)
// �ȉ��̐ݒ��MainFrm�ŏ��������iID����v���邽�߁j
/*
	ON_COMMAND(ID_VRECORD_START, OnToolVRecord)
	ON_COMMAND(ID_RECORD_START, OnToolRecord)
	ON_COMMAND(ID_RECORD_PAUSE, OnToolRecPause)
	ON_COMMAND(ID_RECORD_STOP, OnToolRecStop)
	ON_COMMAND(ID_RECORD_MOD, OnToolRecMod)
*/
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
	// �W���̃t�@�C����{�h�L�������g �R�}���h
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// �W���̈���Z�b�g�A�b�v �R�}���h
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTapurApp �N���X�̍\�z

CTapurApp::CTapurApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B

#ifdef _DEBUG
//   _CrtSetBreakAlloc(171);	// memory leak ���o
#endif

	// TRACE�}�N���Ƀ}���`�o�C�g�������L���ɂ���iVS�̃o�O�Ή��j
	(void)::_tsetlocale( LC_ALL, _T("japanese") );

	m_intEnableLog = -1;
	m_lngRdflId = -1;
	m_lngRsflId = -1;
	m_strUsrId.Empty();
	m_strUsrName.Empty();
	m_strDataFolder.Empty();
	m_strDBPath.Empty();

	m_blSkypeConnect = FALSE;
}

CTapurApp::~CTapurApp()
{
#ifdef _DEBUG
//	_CrtDumpMemoryLeaks();	// memory leak ���o
#endif
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CTapurApp �I�u�W�F�N�g

CTapurApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTapurApp �N���X�̏�����

BOOL CTapurApp::InitInstance()
{
	// memory leak ���o
#ifdef _DEBUG
	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	if (!InitATL())
		return FALSE;

	AfxEnableControlContainer();

	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������
	// ��������Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ���
	// ���폜���Ă��������B

	/// ��d�N���̖h�~
	// �~���[�e�b�N�X�I�u�W�F�N�g�����B�~���[�e�b�N�X�I�u�W�F�N�g��
	// ���łɑ��݂��Ă���ꍇ�́A�A�v���P�[�V������2�ڂ̃C���X�^���X
	// �ł���B�~���[�e�b�N�X�n���h���́A�v���Z�X���I������Ǝ����I��
	// �����邱�Ƃɒ���
	::CreateMutex(NULL, TRUE, m_pszExeName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// �O�̃A�v���P�[�V�����̃��C����ʂ�\��
		HWND hwnd = ::FindWindow(_T(MAIN_CLASS_NAME), NULL); 
		if (hwnd) {
			::SetParent(hwnd,NULL);
			::ShowWindow(hwnd, SW_SHOWNORMAL);
			::SetWindowLong(hwnd,GWL_EXSTYLE,WS_VISIBLE);
			::RedrawWindow(hwnd, NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_FRAME |
                                   RDW_INVALIDATE | RDW_ERASE);
			::SetActiveWindow(hwnd);
			::SetForegroundWindow(hwnd);
		}
		// (����)�^�X�N�g���C����̊���APP�̉�ʕ\�������܂������Ȃ����G���[���b�Z�[�W�ŉ��
		//AfxMessageBox(_T("Tapur is already running!"), MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		//TRACE(_T("Could not find previous instance main window!\n"));
		return FALSE;
	}

/* VC7�ł͕s�v�H
#ifdef _AFXDLL
	Enable3dControls();		// ���L DLL �̒��� MFC ���g�p����ꍇ�ɂ͂������Ăяo���Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N���Ă���ꍇ�ɂ͂������Ăяo���Ă��������B
#endif
*/
	// Socket�̏�����
	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("SOCKETS_INIT_FAILED"));
		return FALSE;
	}

	// ���ꃊ�\�[�XDLL�̃��[�h
	_TCHAR strDll[MAX_PATH];	
	CFileSpec fs(FS_APPDIR);	// application folder
	_stprintf_s(strDll, MAX_PATH, _T("%slang.dll"), (LPCTSTR)(fs.GetFolder()));
	m_hResDLL = LoadLibrary(strDll);
	if (!m_hResDLL) {
		AfxMessageBox(_T("Can't load resource DLL."));
		return FALSE;
	}
	// ���\�[�X�̎Q�Ɛ���w��
	AfxSetResourceHandle(m_hResDLL);

	// �ݒ肪�ۑ�����鉺�̃��W�X�g�� �L�[��ύX���܂��B
	// TODO: ���̕�������A��Ж��܂��͏����ȂǓK�؂Ȃ��̂�
	// �ύX���Ă��������B
	SetRegistryKey(_T("Tapur"));

	LoadStdProfileSettings();  // �W���� INI �t�@�C���̃I�v�V���������[�ނ��܂� (MRU ���܂�)

	/// Windows�I�[�f�B�I�f�o�C�X�̃`�F�b�N
	if (!InitWinDev()) return FALSE;

	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTapurDoc),
		RUNTIME_CLASS(CMainFrame),       // ���C�� SDI �t���[�� �E�B���h�E
		RUNTIME_CLASS(CTapurView));
	if (pDocTemplate == NULL)	return FALSE;
	AddDocTemplate(pDocTemplate);
	if (GetFirstDocTemplatePosition() == NULL)	return FALSE;

	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		return TRUE;
	}
	// �R�}���h���C���Ńf�B�X�p�b�` �R�}���h���w�肵�܂��B
	if (!ProcessShellCommand(cmdInfo))		// MainFrame�̐����������ōs����
		return FALSE;

	// ���C�� �E�B���h�E�����������ꂽ�̂ŕ\���ƍX�V���s���܂��B
	// ������Ԃł̓E�B���h�E�͕���
	m_pMainWnd->SetWindowText(m_pszExeName);
	m_pMainWnd->ShowWindow(SW_HIDE);
//	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// �^�O �ƃE�B���h�E���֘A�t���A��ŒT����悤�ɂ���
	//::SetProp(m_pMainWnd->GetSafeHwnd(), m_pszExeName, (HANDLE)1);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard ���z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush m_brush;		// �w�i�F�̃u���V
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT

	// �w�i�F�u���V������
	m_brush.CreateSolidBrush(RGB(255,255,255));		// white
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������

	// �t�H�[���w�i�F�𔒂ɕύX
	if (nCtlColor == CTLCOLOR_DLG) {
		pDC->SetBkColor(RGB(255,255,255));		// �w�i�𔒂�
		return (HBRUSH)m_brush.GetSafeHandle();
	}
	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);//�w�i����
		int id = pWnd->GetDlgCtrlID();
		if (id == IDC_STATIC_COPY || 
			id == IDC_STATIC_COPY2 ||
			id == IDC_STATIC_COPY3 ||
			id == IDC_STATIC_COPY4)
		{
			pDC->SetTextColor(RGB(153,153,153));// �e�L�X�g�F�D�F�ݒ�
		}
		return (HBRUSH)m_brush.GetSafeHandle();
	}
	
	// TODO: �f�t�H���g�̃u���V���]�݂̂��̂łȂ��ꍇ�ɂ́A�Ⴄ�u���V��Ԃ��Ă�������
	return hbr;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTapurApp ���b�Z�[�W �n���h��

// �I�����C���}�j���A���\��
void CTapurApp::OnAppHelp()
{
	CString str;
	str.LoadString(IDS_URL_HELP);
	OpenUrl(str);
}

// LAME �_�E�����[�h�T�C�g
void CTapurApp::OnLinkLame()
{
	CString str;
	str.LoadString(IDS_URL_LAME);
	OpenUrl(str);
}

// FFDSHOW �_�E�����[�h�T�C�g
//void CTapurApp::OnLinkFFDSHOW()
//{
//	CString str;
//	str.LoadString(IDS_URL_FFDSHOW);
//	OpenUrl(str);
//}

// SkypeIn/Out�w���y�[�W�\��
//void CTapurApp::OnAppSkype()
//{
//	CString str;
//	str.LoadString(IDS_URL_SKYPE);
//	OpenUrl(str);
//}

// ��t��W���j���[�\��
//void CTapurApp::OnDonation()
//{
//	CString str;
//	str.LoadString(IDS_URL_DONATION);
//	OpenUrl(str);
//}

// ���C�Z���X�w���T�C�g
void CTapurApp::OnLinkLicense()
{
	CString str;
	str.LoadString(IDS_URL_LICENSE);
	OpenUrl(str);
}

// �o�[�W�������\��
void CTapurApp::OnAppAbout()
{
	static CAboutDlg m_aboutDlg;
	//aboutDlg.DoModal();

	if (m_aboutDlg.m_hWnd == NULL) {
		m_aboutDlg.Create(IDD_ABOUTBOX, this->m_pMainWnd);
	} else {
		if (m_aboutDlg.IsWindowVisible()) {
			m_aboutDlg.SetFocus();
			return;
		}
	}

	m_aboutDlg.ShowWindow(SW_SHOW);
}

// �ݒ��ʕ\��
void CTapurApp::OnToolSetting()
{
	static CSettingDialog m_setDlg;	
	//setDlg.DoModal();

	if (m_setDlg.m_hWnd == NULL) {
		m_setDlg.Create(IDD_SETTING, this->m_pMainWnd);
	} else {
		//if (m_setDlg.IsWindowVisible()) {
		//	m_setDlg.SetFocus();
		//	return;
		//}
		m_setDlg.DestroyWindow();
		m_setDlg.Create(IDD_SETTING, this->m_pMainWnd);
	}

	m_setDlg.ShowWindow(SW_SHOW);
}

// PC�ڍs�c�[���\��
void CTapurApp::OnToolBackup()
{
	static CBackupDialog m_backupDlg;	

	if (m_backupDlg.m_hWnd == NULL) {
		m_backupDlg.Create(IDD_DATA_BACKUP, this->m_pMainWnd);
	} else {
		if (m_backupDlg.IsWindowVisible()) {
			m_backupDlg.SetFocus();
			return;
		}
	}

	m_backupDlg.ShowWindow(SW_SHOW);
}

// WAV�^���f�[�^���ۑ�����Ă���t�H���_���J���܂�
void CTapurApp::OnWavFolder()
{	 
	CTPsetup tps(theApp.m_strDataFolder);
	CString str = tps.RdfWavFolder();
	HINSTANCE ret = ShellExecute(NULL, NULL, str,  NULL, NULL, SW_SHOWNORMAL);
}

// MP3�^���f�[�^���ۑ�����Ă���t�H���_���J���܂�
void CTapurApp::OnMp3Folder()
{	 
	CTPsetup tps(theApp.m_strDataFolder);
	CString str = tps.RdfMp3Folder();
	HINSTANCE ret = ShellExecute(NULL, NULL, str,  NULL, NULL, SW_SHOWNORMAL);
}

// �^��f�[�^���ۑ�����Ă���t�H���_���J���܂�
void CTapurApp::OnVideoFolder()
{
	CTPsetup tps(theApp.m_strDataFolder);
	CString str = tps.RdfVideoFolder();
	HINSTANCE ret = ShellExecute(NULL, NULL, str,  NULL, NULL, SW_SHOWNORMAL);
}

// ��Q���Ɏc�����ꎞ�f�[�^����f�[�^�𕜋�����
void CTapurApp::OnRepair()
{
	int i = 0;
	CString str;

	/// �m�F�_�C�A���O
	CWordArray aryWd;
	aryWd.RemoveAll();
	aryWd.Add(24);	// 1day
	aryWd.Add(48);	// 2days
	aryWd.Add(168);	// 1week
	aryWd.Add(336);	// 2weeks
	aryWd.Add(720);	// 30days

	CString strTitle;
	CString strMsg;
	CString strData;
	CStringArray aryName;
	strTitle.LoadString(IDS_REPAIR_TITLE);
	strMsg.LoadString(IDS_REPAIR_MSG);
	strData.LoadString(IDS_REPAIR_DATA);
	CTokenizer tok(strData, _T("|"));
	CString cs;
	aryName.RemoveAll();
	while(tok.Next(cs)) {
		aryName.Add(cs);
	}
	str.LoadString(IDS_RSF_ARG_AUDIOFILE);
	CTMsgBox msgbox(NULL, strTitle, strMsg, TRUE, aryName);
	msgbox.Show(TRUE);
	if (!msgbox.GetResult()) return;
	//// �I������
	long lngTime = (long)aryWd.GetAt(0);
	CString strSel;
	strSel = msgbox.GetCombo();
	if (strSel.GetLength() <= 0) return;
	for(i = 0; i < aryName.GetSize(); i++) {
		str = aryName.GetAt(i);
		if (str == strSel) {
			lngTime = (long)aryWd.GetAt(i);
			break;
		}
	}

	/// ���o�^�̈ꎞ�f�[�^�̏C��
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	long lngCnt = pFrame->RepairTmpData(lngTime);
	if (lngCnt > 0) {
		strMsg.LoadString(IDS_MSGBOX111);
		str.Format(_T("%d%s"), lngCnt, strMsg);
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
	} else {
		str.LoadString(IDS_MSGBOX112);
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
	}
}

// Windows�W���̃T�E���h���R�[�_���N��
void CTapurApp::OnSoundRecoder()
{
	CString str;

	HINSTANCE ret = ShellExecute(NULL, NULL, _T("sndrec32.exe"),  NULL, NULL, SW_SHOWNORMAL);
	if ((int)ret <= 32) {
		// �T�E���h���R�[�_���g���Ȃ��ꍇ�iVista�j�AAudacity�𐄏�����
		str.LoadString(IDS_MSGBOX115);
		if (AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONINFORMATION ) == IDYES) {
			OnAudacity();
		}
	}
}

// Audacity���N��
void CTapurApp::OnAudacity()
{
	CString str;
	CFileFind finder;
	CFileSpec fs(FS_PROGRAM_FILES);

	SetCurrentDirectory(fs.GetFullSpec());
	BOOL bWorking = finder.FindFile(_T("Audacity*"));
	if (bWorking) {
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDirectory()) {
				str = finder.GetFileName();
				bWorking = TRUE;
				break;
			}
		}
	}
	finder.Close();

	if (bWorking) {
		str = fs.GetFullSpec() + _T("\\") + str + _T("\\audacity.exe");
		HINSTANCE ret = ShellExecute(NULL, NULL, str,  NULL, NULL, SW_SHOWNORMAL);
		if ((int)ret <= 32) bWorking = FALSE;
	}

	if (!bWorking) {
		// Audacity���g���Ȃ��ꍇ�A�_�E�����[�h�����N��\��
		str.LoadString(IDS_URL_AUDACITY);
		OpenUrl(str);
	}
}

BOOL CTapurApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
/*
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	if (pFrame) {
		if(pFrame->m_Framework.ProcessMessage(pMsg))
			return TRUE;
	}
*/

	return CWinApp::PreTranslateMessage(pMsg);
}

	
CTapurModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

LONG CTapurModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CTapurModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR CTapurModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}


int CTapurApp::ExitInstance()
{
	if (m_bATLInited)
	{
		_Module.RevokeClassObjects();
		_Module.Term();
		CoUninitialize();
	}

	// ���\�[�XDLL������
	if (m_hResDLL) {
		FreeLibrary(m_hResDLL);
	}

	return CWinApp::ExitInstance();

}

BOOL CTapurApp::InitATL()
{
	m_bATLInited = TRUE;

#if _WIN32_WINNT >= 0x0400
// �i���ӁI�jMFC��COM��STA�ł����N���ł��Ȃ��H
	HRESULT hRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
//	HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	HRESULT hRes = CoInitialize(NULL);
#endif

	if (FAILED(hRes))
	{
		m_bATLInited = FALSE;
		return FALSE;
	}

	_Module.Init(ObjectMap, AfxGetInstanceHandle());
	_Module.dwThreadID = GetCurrentThreadId();

	LPTSTR lpCmdLine = GetCommandLine(); // ���̍s�� _ATL_MIN_CRT �ŕK�v�ł�
	TCHAR szTokens[] = _T("-/");

	BOOL bRun = TRUE;
	LPCTSTR lpszToken = _Module.FindOneOf(lpCmdLine, szTokens);
	while (lpszToken != NULL)
	{
		if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_TAPUR, FALSE);
			_Module.UnregisterServer(TRUE); // TRUE �� typelib ���o�^����Ȃ����Ƃ������܂�
			bRun = FALSE;
			break;
		}
		if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_TAPUR, TRUE);
			_Module.RegisterServer(TRUE);
			bRun = FALSE;
			break;
		}
		lpszToken = _Module.FindOneOf(lpszToken, szTokens);
	}

	if (!bRun)
	{
		m_bATLInited = FALSE;
		_Module.Term();
		CoUninitialize();
		return FALSE;
	}

	hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
		REGCLS_MULTIPLEUSE);
	if (FAILED(hRes))
	{
		m_bATLInited = FALSE;
		CoUninitialize();
		return FALSE;
	}	

	return TRUE;

}

//
//  �@�\     : Windows�I�[�f�B�I�f�o�C�X�̃`�F�b�N
//  
//  �@�\���� : �W���f�o�C�X���͓���ł��Ȃ��̂ŁA���[�U�Ɍx������̂�
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CTapurApp::InitWinDev()
{
	CString str;
	CMixer mx;

	/// ���W�X�g���ǂݍ���
	CRegString rgDevIn = CRegString(_T("Software\\Microsoft\\Multimedia\\Sound Mapper\\Record"), _T(""));
	CRegString rgDevOut = CRegString(_T("Software\\Microsoft\\Multimedia\\Sound Mapper\\Playback"), _T(""));
	CString strDevIn = rgDevIn;
	CString strDevOut = rgDevOut;

	/// �W���f�o�C�X�`�F�b�N
	strDevIn.TrimLeft();
	strDevIn.TrimRight();
	strDevOut.TrimLeft();
	strDevOut.TrimRight();
	if (strDevIn.IsEmpty() || strDevOut.IsEmpty()) {
		//// �W���f�o�C�X���K�肳��Ă��Ȃ��ꍇ
		//// �~�L�T�[����WAVE_MAPPER�f�o�C�X���̃`�F�b�N
		strDevOut = mx.GetDeviceName();
		strDevOut.MakeUpper();
		if (strDevOut.Find(_T("TAPUR")) >= 0) {
			goto InitWinDev_ERR;
		}
	} else {
		//// �W���f�o�C�X���K�肳��Ă���ꍇ
		strDevIn.MakeUpper();
		strDevOut.MakeUpper();
		if (strDevIn.Find(_T("TAPUR")) >= 0 || strDevOut.Find(_T("TAPUR")) >= 0) {
			goto InitWinDev_ERR;
		}
	}

	return TRUE;

InitWinDev_ERR:
	str.LoadString(IDS_MSGBOX6);
	AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
	return FALSE;
}

//
//  �@�\     : URL�̃u���E�U�\��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CTapurApp::OpenUrl(CString strUrl)
{
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.fMask = SEE_MASK_INVOKEIDLIST;
	sei.lpVerb = _T("open");
	sei.lpFile = (LPCTSTR)strUrl;
	sei.nShow = SW_SHOWNORMAL;
	ShellExecuteEx(&sei);

	return TRUE;
}
