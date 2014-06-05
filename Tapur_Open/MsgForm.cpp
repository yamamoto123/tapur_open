// MsgForm.cpp : implementation file
//

#include "stdafx.h"
#include "Tapur.h"
#include "MainFrm.h"
#include "MsgForm.h"
#include "MsgTree.h"
#include "TPtable.h"
#include "TPsetup.h"
#include "TPutil.h"
#include "FileSpec.h"
#include "MsgList.h"
#include "HttpAccess.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CMsgForm

IMPLEMENT_DYNCREATE(CMsgForm, TVisualFormView)

CMsgForm::CMsgForm()
	: TVisualFormView(CMsgForm::IDD)
{
	//{{AFX_DATA_INIT(CMsgForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	/// �^�C�}�[������
	m_stPlayChk.blOnTimer = FALSE;
	m_stPlayChk.lngPassTime = 0;
	m_stPlayChk.lngLimitTime = 0;

/*
	// ��������C���X�^���X�̐���
	m_pAudio = CAudio::Instance();
	/// �r�f�I����N���X�̃C���X�^���X����
	m_pVideo = CVideo::Instance();
*/
	// �w�i�F�u���V������
	m_brush.CreateSolidBrush(RGB(255,255,255));		// white
}

CMsgForm::~CMsgForm()
{
}

void CMsgForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgForm)
	DDX_Text(pDX, IDC_EDIT_TITLE, m_EditTitle);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_Title);
	DDX_Control(pDX, IDC_STATIC_TITLE1, m_Title1);
	DDX_Control(pDX, IDC_STATIC_TITLE2, m_Title2);
	DDX_Control(pDX, IDC_STATIC_TITLE3, m_Title3);
	DDX_Control(pDX, IDC_STATIC_TITLE4, m_Title4);
	DDX_Control(pDX, IDC_STATIC_DATE, m_Date);
	DDX_Control(pDX, IDC_STATIC_SENDER_NAME, m_SenderName);
	DDX_Control(pDX, IDC_STATIC_SENDER_ID, m_SenderId);
	DDX_Control(pDX, IDC_STATIC_TIME, m_Time);
	DDX_Control(pDX, IDC_PROGRESS, m_Prog);
	DDX_Control(pDX, IDC_BTN_FILE, m_File);
	DDX_Control(pDX, IDC_BTN_VFILE, m_VFile);
	DDX_Control(pDX, IDC_BTN_PODCAST, m_PodCast);
	DDX_Control(pDX, IDC_APP_PLAY, m_AppPlay);
	DDX_Control(pDX, IDC_APP_VPLAY, m_AppVPlay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgForm, TVisualFormView)
	//{{AFX_MSG_MAP(CMsgForm)
	ON_EN_KILLFOCUS(IDC_EDIT_TITLE, OnKillfocusEditTitle)
	ON_BN_CLICKED(IDC_PLAY, OnBtnPlay)
	ON_BN_CLICKED(IDC_PAUSE, OnBtnPause)
	ON_BN_CLICKED(IDC_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_FILE, OnBtnFile)
	ON_BN_CLICKED(IDC_BTN_VFILE, OnBtnVFile)
	ON_BN_CLICKED(IDC_APP_PLAY, OnBtnAppPlay)
	ON_BN_CLICKED(IDC_APP_VPLAY, OnBtnAppVPlay)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgForm diagnostics

#ifdef _DEBUG
void CMsgForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CMsgForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMsgForm message handlers

int CMsgForm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (TVisualFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	
	m_lngPlayTime = 0;

	// ���t���ԊǗ��p�^�C�}�[�J�n
//	SetTimer(ID_TIMER_MSGFORM, 1000, NULL);	// 1 sec each
	
	return 0;
}

void CMsgForm::OnDestroy() 
{
	TVisualFormView::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	m_lngPlayTime = 0;

	// ���t���ԊǗ��p�^�C�}�[�j��
	StopPlayChkTimer();
//	KillTimer(ID_TIMER_MSGFORM);
}

void CMsgForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	// ��������C���X�^���X�̐���
	m_pAudio = CAudio::Instance();
	/// �r�f�I����N���X�̃C���X�^���X����
	m_pVideo = CVideo::Instance();
 
	/// �{�^��ID��CBitmapButton�I�u�W�F�N�g�Ɋ֘A�t����
	// "Failed to load bitmap for normal image."�G���[���o�邪�������Ă悢
	m_Play.AutoLoad(IDC_PLAY, this);
	m_Pause.AutoLoad(IDC_PAUSE, this);
	m_Stop.AutoLoad(IDC_STOP, this);
	//// �{�^���̃L���v�V�����Ɋ֘A�����r�b�g�}�b�v�����[�h����Ȃ��̂ŁA���\�[�XID�ōĎw�肷��B
	m_Play.LoadBitmaps(PLAYU, PLAYD, PLAYF, PLAYX);
	m_Pause.LoadBitmaps(PAUSEU, PAUSED, PAUSEF, PAUSEX);
	m_Stop.LoadBitmaps(STOPU, STOPD, STOPF, STOPX);
	/// ���s���Ƀ{�^�����ʒu���킹����(Play�{�^���ɍ��킹�Ĕz�u�����)
	//// Play�{�^���̍��W���_�C�A���O�{�b�N�X����̑��Έʒu�Ƃ��Ď擾����
	CRect rect;
	m_Play.GetWindowRect(rect);
	ScreenToClient(rect);
	int nTopPos = rect.top;
	int nWidth = rect.right - rect.left;    
	//// Pause�{�^���̈ʒu��ݒ肷��
	m_Pause.SetWindowPos(NULL, rect.right, nTopPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//// Stop�{�^���̈ʒu��ݒ肷��
	m_Stop.SetWindowPos(NULL, rect.right + nWidth, nTopPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	/// �A�C�R����CButton�Ɋ��蓖��
	HICON hIcon;
	hIcon = AfxGetApp()->LoadIcon(IDI_APP_PLAY);
	m_AppPlay.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_APP_VPLAY);
	m_AppVPlay.SetIcon(hIcon);

	/// ���x���̑����ύX
	//// �t�H�[���̃f�t�H���g�ݒ�̎擾
	CFont *ft = CFormView::GetFont();
	LOGFONT lft;
	ft->GetLogFont(&lft);
	//// �X�V
	m_Title.SetFontBold(TRUE);
	m_Title.SetFontUnderline(TRUE);
	m_Title.SetBkColor(RGB(255,255,255));	// white
	m_Time.SetBkColor(RGB(255,255,255));	// white

	UpdateData(FALSE);	// DDX�X�V
}

HBRUSH CMsgForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = TVisualFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������
	
	// TODO: �f�t�H���g�̃u���V���]�݂̂��̂łȂ��ꍇ�ɂ́A�Ⴄ�u���V��Ԃ��Ă�������

	/// �t�H�[���w�i�F�𔒂ɕύX
	if (nCtlColor == CTLCOLOR_DLG) {
		return (HBRUSH)m_brush.GetSafeHandle();
	}

	/// STATIC�R���g���[���F�ύX
	if (nCtlColor == CTLCOLOR_STATIC) {
		int id = pWnd->GetDlgCtrlID();
		if (id == IDC_STATIC_TITLE || id == IDC_STATIC_TIME) {
			pDC->SetBkColor(RGB(255,255,255));
			pDC->SetBkMode(TRANSPARENT);//�w�i����
			return (HBRUSH)m_brush.GetSafeHandle();
		}
	}

	return hbr;
}

// Window�̕\���E��\�����C�x���g
void CMsgForm::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	TVisualFormView::OnShowWindow(bShow, nStatus);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	
	///// �Đ����̃t�@�C�����~����
	//PlayStop();
}

// [����]�e�L�X�g�{�b�N�X����̃t�H�[�J�XOUT
void CMsgForm::OnKillfocusEditTitle()
{
	CString strBak = m_EditTitle;
	UpdateData(TRUE);	// DDX�X�V
	if (m_EditTitle == strBak) return;	// �ύX���Ȃ��ꍇ

	/// RDFL�e�[�u���̍X�V
	CTblRdfl tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(theApp.m_lngRdflId) <= 0) return;
	tbl.strTitle = m_EditTitle;		// ����
	tbl.UpdateRec(theApp.m_lngRdflId);

	/// ���X�g���[����]�\���̍X�V
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgList *pfview = (CMsgList *)pFrame->m_pMsgList->GetWnd();
	CReportCtrl &rc = pfview->m_wndList;
	rc.SetItemText(pfview->m_hActive, 3, m_EditTitle);
}

void CMsgForm::OnBtnPlay()
{
	Play();
}

void CMsgForm::OnBtnPause()
{
	PlayPause();
}

void CMsgForm::OnBtnStop()
{
	PlayStop();
}

// �����t�@�C���̃G�N�X�|�[�g
void CMsgForm::OnBtnFile()
{
	ExportFile();
}

// �r�f�I�t�@�C���̃G�N�X�|�[�g
void CMsgForm::OnBtnVFile()
{
	ExportVFile();
}

// �����t�@�C���̊O���A�v���Đ�
void CMsgForm::OnBtnAppPlay()
{
	HINSTANCE ret = ShellExecute(NULL, NULL, (LPCTSTR)m_strAudioFile,  NULL, NULL, SW_SHOWNORMAL);
}

// �r�f�I�t�@�C���̊O���A�v���Đ�
void CMsgForm::OnBtnAppVPlay()
{
	HINSTANCE ret = ShellExecute(NULL, NULL, (LPCTSTR)m_strVideoFile,  NULL, NULL, SW_SHOWNORMAL);
}

// �����t�@�C����PODCAST���J
//void CMsgForm::OnBtnPodCast()
//{
//	CFileSpec fs;
//	CString strFile;
//
////m_strPlayFile = _T("C:\\Documents and Settings\\paak\\My Documents\\�e�X�g.wav");
//
//	/// Cnv WAV to MP3
//	fs.SetFullSpec(m_strAudioFile);
//	if(!fs.Exist()) {
//		AfxMessageBox(_T("no file!"), MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
//		return;
//	}
//	strFile = fs.FullPathNoExtension() + _T(".mp3");
//	CCnvAudioDlg cnvDlg;
//	cnvDlg.m_strSrcFile = m_strAudioFile;
//	cnvDlg.m_strDstFile = strFile;
//	if (!cnvDlg.CnvWavMp3()) return;
//
//	/// Upload to WEB Server
//	CString str;
//	CString strUrlHost = _T("218.216.71.189");
//	CString strUrlPath = _T("/media/Japanese/test/yamamoto_tatsuya/");
//	CString strUsr = _T("paak");
//	CString strPass = _T("paak1085");
//	CString strSrcFile = strFile;
//
//	char url_host[MAX_LEN];
//	char url_path[MAX_LEN];
//	char username[MAX_LEN];
//	char password[MAX_LEN];
////	char src_filename[MAX_LEN];
//	char dst_filename[MAX_LEN];
//
//	wcstombs(url_host, strUrlHost, MAX_LEN);
//	wcstombs(url_path, strUrlPath, MAX_LEN);
//	wcstombs(username, strUsr, MAX_LEN);
//	wcstombs(password, strPass, MAX_LEN);
////	CnvCString_Bytes(strFile, src_filename, MAX_LEN);
//	fs.SetFullSpec(strFile);
//	strFile = strUrlPath + fs.GetFileName();
//	CnvCString_Bytes(strFile, dst_filename, MAX_LEN);
//
//	CHttpAccess http;
//	int upSock = http.HttpConnect(url_host, url_path, HTTP_PORT, username, password);
//	if (upSock < 0) {
//		str.Format(_T("can't connect to URL:%s%s\n"),strUrlHost,strUrlPath);
//		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
//		return;
//	}
//	if (http.HttpUpLoad(upSock, url_host, dst_filename, (LPCTSTR)strSrcFile) < 0) {
//		str.Format(_T("can't upload file:%s to HOST:%s\n"),m_strAudioFile, strUrlHost);
//		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
//		http.HttpDisConnect(upSock);
//		return;
//	}
//
//	http.HttpDisConnect(upSock);
//
//	AfxMessageBox(_T("SUCCESS"), MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
//}

// �{�^���̃w���v���X�e�[�^�X�o�[�ɕ\��
BOOL CMsgForm::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	DWORD pos;
	CPoint mouse;
	CRect area;
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();

	pos = ::GetMessagePos();
	mouse.x = LOWORD(pos);
	mouse.y = HIWORD(pos);
	m_AppPlay.GetWindowRect(&area);
	if ( area.PtInRect(mouse) != 0 ) {
		/// �}�E�X���W���R���g���[�����������ꍇ�̏���
		pFrame->SetMessageText(ID_APP_PLAY);
		goto OnSetCursor_END;
	}
	m_AppVPlay.GetWindowRect(&area);
	if ( area.PtInRect(mouse) != 0 ) {
		/// �}�E�X���W���R���g���[�����������ꍇ�̏���
		pFrame->SetMessageText(ID_APP_VPLAY);
		goto OnSetCursor_END;
	}

	pFrame->SetMessageText(_T(""));

OnSetCursor_END:
	return TVisualFormView::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CMsgForm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		/// RETURN KEY 
		if (nVirtKey == VK_RETURN) {
			//// �t�H�[�J�X�R���g���[����Bitmap�{�^���̏ꍇ
			CWnd *cWnd = GetFocus();
			if (cWnd->m_hWnd == m_Play.m_hWnd) {
				OnBtnPlay();
				return TRUE;
			} else if (cWnd->m_hWnd == m_Pause.m_hWnd) {
				OnBtnPause();
				return TRUE;
			} else if (cWnd->m_hWnd == m_Stop.m_hWnd) {
				OnBtnStop();
				return TRUE;
			} else if (cWnd->m_hWnd == GetDlgItem(IDC_EDIT_TITLE)->m_hWnd) {
				//// �������͂̊m��
				OnKillfocusEditTitle();
				return TRUE;
			}
		} else if (nVirtKey == VK_TAB) {
		/// TAB KEY : MsgTree�t�H�[���֑J��
			//// �t�H�[�J�X�Ō�̏ꍇ�̂ݗL��
			CWnd *cWnd = GetFocus();
			if ((cWnd->m_hWnd == m_AppVPlay.m_hWnd) || 
				(cWnd->m_hWnd == m_AppPlay.m_hWnd && m_AppVPlay.IsWindowEnabled() == FALSE) ||
				(cWnd->m_hWnd == m_Title.m_hWnd && m_AppPlay.IsWindowEnabled() == FALSE)) {
				CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
				CMsgTree *pfview = (CMsgTree *)pFrame->m_pMsgTree->GetWnd();
				pfview->SetFocus();
				return TRUE;
			}
		}

	}
	
	return TVisualFormView::PreTranslateMessage(pMsg);
}

void CMsgForm::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	if (nIDEvent == ID_TIMER_MSGFORM) {
		m_stPlayChk.lngPassTime++;	// �^�C�}�[�o�ߎ��Ԃ��X�V
		/// �Đ����̃v���O���X�o�[�X�V
		int intPos, intLow, intUpper;
		if (m_pAudio->IsPlaying()) {
			if (!m_pAudio->IsPlayPause()) {
				intPos = m_Prog.GetPos();
				m_Prog.GetRange(intLow, intUpper);
				if (intPos < intUpper) {
					m_Prog.StepIt();
				} else {
					/// �Đ��I��
					PlayStop();
				}
			}
		} else {
			/// �Đ��I��
			PlayStop();
			/// �v���O���X�o�[���t���Ɂi�����ځj
			m_Prog.GetRange(intLow, intUpper);
			m_Prog.SetPos(intUpper);
		}
	}
	
	TVisualFormView::OnTimer(nIDEvent);
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
BOOL CMsgForm::UpdateForm(BOOL blPlayStop) 
//BOOL CMsgForm::UpdateForm() 
{
	CFileSpec fs;
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();

	/// �J�����g���R�[�h
	if (theApp.m_lngRdflId <= 0) return FALSE;

	/// �Đ����̃t�@�C�����~����(�t�H�[�J�X���ړ�����̂Œ��ӁI)
//	PlayStop();
	if (blPlayStop) PlayStop();

	//// �v���O���X�o�[�N���A
	if (m_Prog) m_Prog.SetPos(0);	


	/// �����ݒ�
	m_lngPlayTime = 0;
	m_strAudioFile.Empty();
	m_strVideoFile.Empty();
	m_Play.EnableWindow(FALSE);
	m_Stop.EnableWindow(FALSE);
	m_Pause.EnableWindow(FALSE);
	m_File.EnableWindow(FALSE);
	m_VFile.EnableWindow(FALSE);
	m_AppPlay.EnableWindow(FALSE);
	m_AppVPlay.EnableWindow(FALSE);
	m_Time.SetWindowText(_T("[ 00:00 ]"));

	/// RDFL�e�[�u������
	CTblRdfl tbl(theApp.m_strDBPath);
	if (tbl.SelectRec(theApp.m_lngRdflId) <= 0) {
		return FALSE;
	}

	/// [����]�擾
	m_EditTitle = tbl.strTitle;
	//// [���M�Җ�]�擾
	CString strSender;
	if (tbl.strSenderId == _T("0")) {
		//// �s���Ȕ��M��(SkypeIn)
		strSender.LoadString(IDS_MSG_DEF);
		m_SenderName.SetWindowText(strSender);
		m_SenderId.SetWindowText(_T(""));
	} else {
		m_SenderName.SetWindowText(tbl.strSenderName);
		m_SenderId.SetWindowText(tbl.strSenderId);
	}
	//// [����]�擾
	_TCHAR strDate[64];
	CnvDateString((LPCTSTR)tbl.dlngSystemNew, strDate);
	m_Date.SetWindowText(strDate);
	/// ����/�r�f�I�t�@�C�����擾
	int intFormatType = 0;
	CTPsetup tps(theApp.m_strDataFolder);
	tps.SetCallerId(tbl.strSenderId);
	m_strVideoFile = tps.FindRdfVideoFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
	m_strAudioFile = tps.FindRdfWavFileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
	if (m_strAudioFile.IsEmpty()) {
		m_strAudioFile = tps.FindRdfMp3FileName(tbl.strRemarks1, tbl.dlngSystemNew, _T("$$"));
		intFormatType = 1;
	}

	CString strBar;
	if (!m_strAudioFile.IsEmpty()) {
	/// ��M�����t�@�C��������ꍇ
		CString strTmp;
		//// STatusBar�p������
		fs.SetFullSpec(m_strAudioFile);
		strBar = fs.GetFileName();
		/// ��M�����t�@�C���̎��ԕ\��
		float fTime = m_pAudio->GetPlaySize(m_strAudioFile);
		if (fTime < 0) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Audio file may be broken:%s"), m_strAudioFile);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, te.errstr);
			return FALSE;
		}
		m_lngPlayTime = (fTime > 0) ? (long)fTime : 1;	// �P�b�ȉ��͂P�b�Ƃ���
		strTmp.Format(_T("[ %02i:%02i ]"), (m_lngPlayTime/60), (m_lngPlayTime%60));
		m_Time.SetWindowText(strTmp);
		//// �Đ��E�G�N�X�|�[�g�{�^����L��
		m_Play.EnableWindow(TRUE);
		m_File.EnableWindow(TRUE);
		m_AppPlay.EnableWindow(TRUE);

	}
	if (!m_strVideoFile.IsEmpty()) {
	/// ��M�r�f�I�t�@�C��������ꍇ
		//// STatusBar�p������
		if (strBar.GetLength() > 0) {
			strBar += _T("/avi");
		} else {
			fs.SetFullSpec(m_strVideoFile);
			strBar = fs.GetFileName();
		}
		//// �G�N�X�|�[�g�{�^����L��
		m_VFile.EnableWindow(TRUE);
		m_AppVPlay.EnableWindow(TRUE);
	}

	/// StatusBar�Ƀt�@�C�����A�f�[�^�^�C�v�\��
	pFrame->SetStatusText(strBar);
	if (tbl.lngRespType == 0) {
		strBar = _T("REC");
		if (tbl.lngClsCode1 == 1 || tbl.lngClsCode1 == 3) {
			strBar += _T("/IN");
		} else if (tbl.lngClsCode1 == 2 || tbl.lngClsCode1 == 4) {
			strBar += _T("/OUT");
		}
	} else {
		strBar = _T("ANSWER");
	}
	pFrame->SetStatusText(strBar, 1);

	/// �Đ��t�@�C�������݂��Ȃ��ꍇ�A�\���݂̂Ŗ��Đ����X�g���Đ��ς݂ɕύX
	if (m_strAudioFile.IsEmpty() && m_strVideoFile.IsEmpty() && tbl.lngViewFlag != 1) {
		// �i�H�H�H�jChgMsgMark()�Ńe�[�u���X�V����ƁA�e�[�u�����b�N����������Ă��Ȃ����߁A�����ōX�V����
		tbl.lngViewFlag = 1;
		tbl.UpdateRec(theApp.m_lngRdflId);
		CMsgList *pfview = (CMsgList *)pFrame->m_pMsgList->GetWnd();
		pfview->ChgMsgMark(FALSE);
	}

	UpdateData(FALSE);	// DDX�X�V

	return TRUE;
}

//
//  �@�\     : �����E�r�f�I�Đ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMsgForm::Play()
{
	if (_tcsclen(m_strAudioFile) <= 0) return FALSE;

	/// �Đ����ł���ΐ�ɒ�~
	PlayStop();

	/// �r�f�I�Đ��J�n
	if (!m_strVideoFile.IsEmpty()) {
		if (!m_pVideo->PlayOpen()) {
			CString str;
			str.LoadString(IDS_MSGBOX64);
			AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return FALSE;
		}
		if (!m_pVideo->Play(m_strVideoFile)) {
			// Win7+DivX�̊���VideoGrabber�����삵�Ȃ����G���[�łȂ�WMP�Đ��ő�p
			//CString str;
			//str.LoadString(IDS_MSGBOX64);
			//AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			//return FALSE;
			m_pVideo->PlayStop();
			OnBtnAppVPlay();
			goto SECTION_AUDIO_SKIP;
		}
	}
	
	/// �����Đ��J�n
	m_pAudio->m_PlayFile = m_strAudioFile;
	if (!m_pAudio->PlayOpen()) {
		CString str;
		str.LoadString(IDS_MSGBOX61);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (!m_pAudio->Play(FALSE)) {
		CString str;
		str.LoadString(IDS_MSGBOX61);
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return FALSE;
	}

	/// ���t���ԊǗ��p�^�C�}�[�J�n
	StartPlayChkTimer(m_lngPlayTime);

	/// ��~�{�^����L��
	m_Stop.EnableWindow(TRUE);
	/// �ꎞ��~�{�^����L��
	m_Pause.EnableWindow(TRUE);
	/// �v���O���X�o�[������
	if (m_Prog) m_Prog.SetPos(0);	

SECTION_AUDIO_SKIP:

	/// ���Đ����X�g���Đ��ς݂ɕύX
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CMsgList *pfview = (CMsgList *)pFrame->m_pMsgList->GetWnd();
	pfview->ChgMsgMark();

	/// �V�����b�Z�[�W�����N���A(TOOLTIP�̍X�V�̂���)
	pFrame->IncNewMsgCnt(0);

	return TRUE;
}

//
//  �@�\     : �����E�r�f�I�Đ��ꎞ��~
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMsgForm::PlayPause()
{
	if (!m_pVideo->IsPlaying() && !m_pAudio->IsPlaying()) return TRUE;

	if (m_pAudio->IsPlayPause()) {
		/// �Đ��ꎞ��~�̕��A
		if (m_pAudio->IsPlaying()) {
			m_pAudio->PlayContinue();
		}
		if (m_pVideo->IsPlaying()) {
			m_pVideo->PlayContinue();
		}
	} else {
		/// �Đ��ꎞ��~
		if (m_pVideo->IsPlaying()) {
			m_pVideo->PlayPause();
		}
		if (m_pAudio->IsPlaying()) {
			m_pAudio->PlayPause();
		}
	}

	///// �Đ��{�^���Ƀt�H�[�J�X
	//m_Play.SetFocus();
	///// �ꎞ��~�{�^���𖳌�
	//m_Pause.EnableWindow(FALSE);
	/// �ꎞ��~�{�^���Ƀt�H�[�J�X
	m_Pause.SetFocus();

	return TRUE;
}

//
//  �@�\     : �����E�r�f�I�Đ���~
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMsgForm::PlayStop()
{
	/// �r�f�I�Đ��I��
	if (m_pVideo->IsPlaying()) {
		m_pVideo->PlayStop();
	}

	/// �����Đ��I��
	if (m_pAudio->IsPlaying()) {
		m_pAudio->PlayStop();
	}

	//// ���t���ԊǗ��p�^�C�}�[�I��
	StopPlayChkTimer();
	//// �v���O���X�o�[������
//	if (m_Prog) m_Prog.SetPos(0);	

	/// �Đ��{�^���Ƀt�H�[�J�X
	if (m_Play) m_Play.SetFocus();
	/// ��~�{�^���𖳌�
	if (m_Stop) m_Stop.EnableWindow(FALSE);
	/// �ꎞ��~�{�^���𖳌�
	if (m_Pause) m_Pause.EnableWindow(FALSE);

	return TRUE;
}

// �����t�@�C���̃G�N�X�|�[�g
BOOL CMsgForm::ExportFile()
{
	if (!m_File.IsWindowEnabled()) return FALSE;

	if (m_cnvADlg.m_hWnd == NULL) {
		m_cnvADlg.Create(IDD_CONVERT_AUDIO, this);
	} else {
		if (m_cnvADlg.IsWindowVisible()) {
			m_cnvADlg.SetFocus();
			return TRUE;
		}
	}

	m_cnvADlg.SetSrcFile(m_strAudioFile);
	m_cnvADlg.ShowWindow(SW_SHOW);

	return TRUE;
}

// �r�f�I�t�@�C���̃G�N�X�|�[�g
BOOL CMsgForm::ExportVFile()
{
	if (!m_VFile.IsWindowEnabled()) return FALSE;

	if (m_cnvVDlg.m_hWnd == NULL) {
		m_cnvVDlg.Create(IDD_CONVERT_VIDEO,this);
	} else {
		if (m_cnvVDlg.IsWindowVisible()) {
			m_cnvVDlg.SetFocus();
			return TRUE;
		}
	}

	m_cnvVDlg.SetSrcFile(m_strVideoFile, m_strAudioFile);
	m_cnvVDlg.ShowWindow(SW_SHOW);

	return TRUE;
}

//
//  �@�\     : ���t���ԊǗ��p�^�C�}�[�J�n
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMsgForm::StartPlayChkTimer(
long lngPlayTime)	// PLAY����
{
	if (!m_stPlayChk.blOnTimer) {
		SetTimer(ID_TIMER_MSGFORM, 1000, NULL);	// 1 sec each(must)
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StartPlayChkTimer"));
	}
	m_stPlayChk.blOnTimer = TRUE;		// �^�C�}�[���s���t���O
	m_stPlayChk.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A
	m_stPlayChk.lngLimitTime = lngPlayTime;		// �^�C�}�[�������Ԃ��Z�b�g

	//// �v���O���X�o�[�J�n
	if (m_Prog) {
		m_Prog.SetStep(1);
		m_Prog.SetRange32(0, m_lngPlayTime);
		m_Prog.SetPos(0);
	}

	return TRUE;
}

//
//  �@�\     : ���t���ԊǗ��p�^�C�}�[�I��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CMsgForm::StopPlayChkTimer()
{
	if (m_stPlayChk.blOnTimer) {
		KillTimer(ID_TIMER_MSGFORM);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_TAPUR, _T("StopPlayChkTimer"));
	}
	m_stPlayChk.blOnTimer = FALSE;		// �^�C�}�[���s���t���O
	m_stPlayChk.lngPassTime = 0;		// �^�C�}�[�o�ߎ��Ԃ��N���A
	m_stPlayChk.lngLimitTime = 0;		// �^�C�}�[�������Ԃ��N���A

	return TRUE;
}

