// VideoDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "VideoDialog.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

// ��ʂ̍ŏ��l
#define MIN_WIDTH	100
#define MIN_HEIGHT	100

// �E�B���h�E�T�C�Y���W�X�g���ݒ�
// Section name
#define SEC_SETTING "Settings"
// Entry name
#define ENT_WINPOS "VideoWinPos"

/////////////////////////////////////////////////////////////////////////////
// CVideoDialog �_�C�A���O


CVideoDialog::CVideoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVideoDialog)
	//}}AFX_DATA_INIT

	m_intRecordingStatus = 0;
}

CVideoDialog::~CVideoDialog()
{
}

void CVideoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideoDialog)
	DDX_Control(pDX, IDC_VIDEOGRAB, m_VideoGrab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVideoDialog, CDialog)
	//{{AFX_MSG_MAP(CVideoDialog)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CVideoDialog, CDialog)
    //{{AFX_EVENTSINK_MAP(CMainForm)
	ON_EVENT(CVideoDialog, IDC_VIDEOGRAB, 201 /* OnResize */, OnResizeVideograbber, VTS_NONE)
	ON_EVENT(CVideoDialog, IDC_VIDEOGRAB, 244 /* OnRecordingStarted */, OnRecordingStartedVideograbber, VTS_BSTR)
	ON_EVENT(CVideoDialog, IDC_VIDEOGRAB, 240 /* OnRecordingCompleted */, OnRecordingCompletedVideograbber, VTS_BSTR VTS_BOOL)
	ON_EVENT(CVideoDialog, IDC_VIDEOGRAB, 222 /* OnFrameCaptureCompleted */, OnFrameCaptureCompletedVideograbber, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_R8 VTS_I4 VTS_BSTR VTS_BOOL VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoDialog ���b�Z�[�W �n���h��

// �r�f�I�̈�ύX�C�x���g
// �r�f�I�̈�ύX����WINDOW�T�C�Y���A�����ĕύX
void CVideoDialog::OnResizeVideograbber() 
{
	try {
		if (!m_VideoGrab.m_hWnd) return;

		RECT rect;
		m_VideoGrab.GetClientRect(&rect);

		long NewWidth, NewHeight;
		NewWidth  = rect.right - rect.left + 35;
		NewHeight = rect.bottom - rect.top + 50;
	//	NewWidth  = rect.right + 5;	//? m_VideoGrabber.Left + m_VideoGrabber.Width  + 30;
	//	NewHeight = rect.bottom+ 5;	//? m_VideoGrabber.Top  + m_VideoGrabber.Height + 60;

		if (NewWidth  < MIN_WIDTH)	NewWidth  = MIN_WIDTH;
		if (NewHeight < MIN_HEIGHT)	NewHeight = MIN_HEIGHT;

		/// ���W�X�g������WINDOW�ʒu�擾
		memset(&rect, NULL, sizeof(RECT));
		UINT nl;
		WINDOWPLACEMENT *lwp;
		if(AfxGetApp()->GetProfileBinary(_T(SEC_SETTING), _T(ENT_WINPOS), (LPBYTE*)&lwp, &nl))
		{
			rect = lwp->rcNormalPosition; 
			delete [] lwp;
		}

	//	this->GetWindowRect(&rect);
		rect.right = rect.left	+ NewWidth;
		rect.bottom= rect.top	+ NewHeight;
		this->MoveWindow(&rect);
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("OnResizeVideograbber error"));
	}
}

// Recording�J�n�C�x���g
void CVideoDialog::OnRecordingStartedVideograbber(LPCTSTR FileName) 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	m_intRecordingStatus = VIDEO_RECORD_RUNNING;
}

// Recording�I���i�t�@�C���������݊����j�C�x���g
void CVideoDialog::OnRecordingCompletedVideograbber(LPCTSTR FileName, BOOL Success) 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if (Success) {
		m_intRecordingStatus = VIDEO_RECORD_SUCCESS;
	} else {
		m_intRecordingStatus = VIDEO_RECORD_FAILED;
	}
}

// �X�g���[���Đ��p�t���[�������C�x���g�i�t���[�����Đ�����閈�ɃR�[���j
void CVideoDialog::OnFrameCaptureCompletedVideograbber(long FrameBitmapHandle, long BitmapWidth, long BitmapHeight, long FrameNumber, double FrameTime, long DestType, LPCTSTR FileName, BOOL Success, long FrameId) 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
//TRACE(_T("OnFrameCaptureCompletedVideograbber:Success=%d, FrameNumber=%d, FrameTime=%f, FileName=%s\n"), Success, FrameNumber, FrameTime, FileName);

	try {
		// ��ɂR�t���[����ێ�����
		if (Success) {
			// ���݂̃t���[���ԍ����擾
			// FrameNumber�ł͂Ȃ�FileName����擾�����삪�x���ꍇ�A�t���[���̓X�L�b�v����邽��
			TCHAR buf[MAX_PATH];
			int len = _tcsclen(FileName);
			_tcsncpy_s(buf, MAX_PATH, FileName+(len-10), 6);	// get "00000n"
			long now = _ttol(buf);

			// �R�O�̃t���[�����폜
			TCHAR fname[MAX_PATH];
			_stprintf_s(fname, MAX_PATH, _T("%s\\vg%06d.jpg"), m_strStreamPath, now-3);
	//TRACE(_T("OnFrameCaptureCompletedVideograbber:fname=%s\n"), fname);
			if (_taccess_s(fname, 0) == 0) {
				::DeleteFile(fname);
			}
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("OnFrameCaptureCompletedVideograbber error"));
	}
}

/*
void CVideoDialog::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������

	CDialog::OnOK();
}
*/

// [�]]�{�^������
void CVideoDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B

	try {
		/// �Đ����̏ꍇ�A��~����
		//if (IsPlayStatus(ps_Playing)) {
		if (m_VideoGrab.GetIsPlayerPlaying()) {
			m_VideoGrab.ClosePlayer();
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("OnCancel error"));
	}

	CDialog::OnCancel();
}

void CVideoDialog::OnClose() 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	try {
		// �E�B���h�E�ʒu�E�T�C�Y�̕ۑ�
		WINDOWPLACEMENT wp;
		GetWindowPlacement(&wp);
		AfxGetApp()->WriteProfileBinary(_T(SEC_SETTING), _T(ENT_WINPOS), (LPBYTE)&wp, sizeof(wp));
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("OnClose error"));
	}
	
	CDialog::OnClose();
}

void CVideoDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	try {
		/// �r�f�I�^�撆����
		/// Skype�r�f�I�̈悪�ϓ������ꍇ�A�L���v�`���̈��ZOOM�𒲐�����
		if (nIDEvent == ID_TIMER_VIDEO) {
			//// ���݂̃L���v�`���̈�����擾
			long lngX = m_VideoGrab.GetCropping_X();
			long lngY = m_VideoGrab.GetCropping_Y();
			long lngWidth = m_VideoGrab.GetCropping_Width();
			long lngHeight = m_VideoGrab.GetCropping_Height();
			long lngDevWidth = GetSystemMetrics(SM_CXSCREEN);
			long lngDevHeight = GetSystemMetrics(SM_CYSCREEN);

			//// Skype�r�f�I�̈�����擾
			double dblRateW, dblRateH, dblRate;
			double dblZoom = m_VideoGrab.GetCropping_Zoom();
			CRect rect;
			if (GetVideoWndLarge(rect)) {
			//// Skype�r�f�I����Window�ŋN��
				if (m_blVSetTop) {
					SetTopVideoWndLocal(FALSE);
					SetTopVideoWndLarge(TRUE);
				}
			} else if (GetVideoWndLocal(rect)) {
			//// Skype�r�f�I���N��
				if (m_blVSetTop) {
					SetTopVideoWndLarge(FALSE);
					SetTopVideoWndLocal(TRUE);
				}
			} else {
			//// Skype�r�f�I���N�����Ă��Ȃ�
				SetTopVideoWndLocal(FALSE);
				SetTopVideoWndLarge(FALSE);
				goto ONTIMER_ERR;
			}

			//// Capture�ʒu����
			if (rect.left != lngX || rect.top != lngY) {
				m_VideoGrab.SetCropping_X((long)rect.left);
				m_VideoGrab.SetCropping_Y((long)rect.top);
				goto ONTIMER_ERR;
			}

			//// Capture Zoom����
			// Skype�g�ݍ��݉�ʂƕʃE�B���h�E��ʂł͏c���䂪���ƂȂ邽�߁APC��ʂ��኱�����Ă��܂�
			// ����ł͂�ނ����Ȃ�
			if (rect.Width() == (int)lngWidth && rect.Height() == (int)lngHeight) {
				if (dblZoom != 1.0) 
					m_VideoGrab.SetCropping_Zoom(1.0);
			} else {
				dblRateW = (double)rect.Width()/(double)lngDevWidth;
				dblRateH = (double)rect.Height()/(double)lngDevHeight;
				dblRate = (double)((long)(max(dblRateW, dblRateH) * 100.0) / 100.0);
				dblRate = 1.0 / dblRate / 10.0;	// 1/n -> 0.n
				if (dblZoom != dblRate) {
					m_VideoGrab.SetCropping_Zoom(dblRate);
				}
			}
		}
	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, te.errstr);
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("OnTimer error"));
	}
	
ONTIMER_ERR:
	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// function

//
//  �@�\     : Skype�̃r�f�I��ʗ̈�̎擾�i�g�ݍ��݉�ʁj
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : HWND
//  
//  ���l     : Skype Ver�ɂ��Window�\�����قȂ�B2.5�ł͖��m�F�B
//  
HWND CVideoDialog::GetVideoWndLocal(LPRECT pRect)
{
	HWND hwnd = NULL;

	try {
		HWND hwndTmp = NULL;
		hwnd = ::FindWindow(_T("TConversationForm"), NULL);		// 5.1-
		if (hwnd == NULL) hwnd = ::FindWindow(_T("TConversationForm.UnicodeClass"), NULL);	// 5.0
		if (hwnd == NULL) {
			hwnd = ::FindWindow(_T("TZapMainForm.UnicodeClass"), NULL);		// 4.00215�܂�
			if (hwnd == NULL) {
				hwnd = ::FindWindow(_T("tSkMainForm.UnicodeClass"), NULL);	// 4.00226-
				if (hwnd != NULL) {
					hwndTmp = hwnd;
					hwnd = ::FindWindowEx(hwnd, 0, _T("TConversationForm.UnicodeClass"), (LPCTSTR)m_strSkypeId);	// skype name or ID
					if (hwnd == NULL) {
						// �r�f�I��c�ł͕������[�U�̃E�B���h�E���ɂȂ�
						hwnd = ::FindWindowEx(hwndTmp, 0, _T("TConversationForm.UnicodeClass"), NULL);
					}
				}
			}
			if (hwnd == NULL) {
				hwnd = ::FindWindow(_T("tSkMainForm"), NULL);	// 5.1-
				if (hwnd != NULL) {
					hwndTmp = hwnd;
					hwnd = ::FindWindowEx(hwnd, 0, _T("TConversationForm"), (LPCTSTR)m_strSkypeId);	// skype name or ID
					if (hwnd == NULL) {
						// �r�f�I��c�ł͕������[�U�̃E�B���h�E���ɂȂ�
						hwnd = ::FindWindowEx(hwndTmp, 0, _T("TConversationForm"), NULL);
					}
				}
			}
		}

		if (hwnd != NULL) {
			// skype 4.0-
			HWND hwndTmp1 = NULL;
			HWND hwndTmp2 = NULL;
			CRect rect1;
			CRect rect2;
			hwndTmp1 = ::FindWindowEx(hwnd, 0, _T("TConversationVideoPanel"), NULL); 
			if (hwndTmp1 == NULL) {
				// skype 5.0-
				hwnd = ::FindWindowEx(hwnd, 0, _T("TLiveConversationPanel"), NULL); if (hwnd == NULL) return FALSE;
			} else {
				// skype 4.0-
				hwnd = ::FindWindowEx(hwndTmp1, 0, _T("TConversationVideo"), NULL); if (hwnd == NULL) return FALSE;
				hwndTmp1 = ::FindWindowEx(hwnd, 0, _T("TCustomVideoWindow"), NULL); if (hwndTmp1 == NULL) return FALSE;
				hwndTmp2 = ::FindWindowEx(hwnd, hwndTmp1, _T("TCustomVideoWindow"), NULL);
				if (hwndTmp2 == NULL) {
					hwnd = hwndTmp1;
				} else {
					::GetWindowRect(hwndTmp1, rect1);
					::GetWindowRect(hwndTmp2, rect2);
					hwnd = (rect1.Width() > rect2.Width()) ? hwndTmp1 : hwndTmp2;
				}
			}
			::GetWindowRect(hwnd, pRect);
		} else {
			hwnd = ::FindWindow(_T("tSkMainForm.UnicodeClass"), NULL); if (hwnd == NULL) return FALSE;
			hwnd = ::FindWindowEx(hwnd, 0, _T("TPanel"), NULL); if (hwnd == NULL) return FALSE;
			hwnd = ::FindWindowEx(hwnd, 0, _T("TPanel"), NULL); if (hwnd == NULL) return FALSE;
			hwndTmp = ::FindWindowEx(hwnd, 0, _T("TPanel"), NULL);
			if (hwndTmp != NULL) {
				// skype 2.0-2.6
				hwnd = hwndTmp;
				hwnd = ::FindWindowEx(hwnd, 0, _T("TPanel"), NULL); if (hwnd == NULL) return FALSE;
				hwnd = ::FindWindowEx(hwnd, 0, _T("TPanel"), NULL); if (hwnd == NULL) return FALSE;
				hwnd = ::FindWindowEx(hwnd, 0, _T("TPanel"), NULL); if (hwnd == NULL) return FALSE;
				hwndTmp = ::FindWindowEx(hwnd, 0, _T("TTntPageControl.UnicodeClass"), NULL);
				if (hwndTmp != NULL) {
					// skype 2.0-2.5(maybe)
					hwnd = hwndTmp;
					hwnd = ::FindWindowEx(hwnd, 0, _T("TTntTabSheet.UnicodeClass"), (LPCTSTR)m_strSkypeId);	// skype name or ID
					if (hwnd == NULL) return FALSE;
					hwnd = ::FindWindowEx(hwnd, 0, _T("tCallPanel"), NULL); if (hwnd == NULL) return FALSE;
				} else {
					// skype 2.6
					// 2.5�̐��m�ȏ�Ԃ��s���̂��߁A���̐ݒ���c��
					hwnd = ::FindWindowEx(hwnd, 0, _T("TSkPageControl.UnicodeClass"), NULL); if (hwnd == NULL) return FALSE;
					hwnd = ::FindWindowEx(hwnd, 0, _T("TSkTabSheet.UnicodeClass"), (LPCTSTR)m_strSkypeId);	// skype name or ID
					if (hwnd == NULL) return FALSE;
					hwnd = ::FindWindowEx(hwnd, 0, _T("tMultiCallPanel"), NULL); if (hwnd == NULL) return FALSE;
					hwnd = ::FindWindowEx(hwnd, 0, _T("tCallView"), NULL); if (hwnd == NULL) return FALSE;
				}
			} else {
				// skype 3.0-
				hwnd = ::FindWindowEx(hwnd, 0, _T("TSkypeTabControl"), NULL); if (hwnd == NULL) return FALSE;
				hwnd = ::FindWindowEx(hwnd, 0, _T("tMultiCallPanel"), NULL); if (hwnd == NULL) return FALSE;
				hwnd = ::FindWindowEx(hwnd, 0, _T("tCallView"), NULL); if (hwnd == NULL) return FALSE;
			}
			hwnd = ::FindWindowEx(hwnd, 0, _T("tSkVideoAvatar"), NULL); if (hwnd == NULL) return FALSE;
			
			::GetWindowRect(hwnd, pRect);

			hwnd = ::FindWindowEx(hwnd, 0, _T("tSkLocalVideoControl"), NULL); 
			if (hwnd == NULL) {
				pRect->top += 12;
				pRect->bottom -= 21;	// ?? �����ڂƍ����ĂȂ����A���̒l��������
				pRect->left += 12;
				pRect->right -= 12;
			} else {
				::GetWindowRect(hwnd, pRect);
			}
		}

		// �T�C�Y���������ꍇ�͎����̃r�f�I�̂ݕ\���Ƃ݂Ȃ�
		if ((pRect->right - pRect->left) < 80) {
			return NULL;	
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("GetVideoWndLocal error"));
		return NULL;	
	}

	return hwnd;
}

//
//  �@�\     : Skype�̃r�f�I��ʗ̈�̎擾�i�ʃE�B���h�E�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : HWND
//  
//  ���l     : 
//  
HWND CVideoDialog::GetVideoWndLarge(LPRECT pRect)
{
	HWND hwnd = NULL;

	try {
		hwnd = ::FindWindow(_T("TLiveConversationWindow"), (LPCTSTR)m_strSkypeId);	// 5.1
		if (hwnd == NULL) {
			// �r�f�I��c�ł͕������[�U�̃E�B���h�E���ɂȂ�
			hwnd = ::FindWindow(_T("TLiveConversationWindow"), NULL);	// 5.1
		}
		if (hwnd == NULL) {
			hwnd = ::FindWindow(_T("TLiveConversationWindow.UnicodeClass"), (LPCTSTR)m_strSkypeId);	// 5.0
		}
		if (hwnd == NULL) {
			// �r�f�I��c�ł͕������[�U�̃E�B���h�E���ɂȂ�
			hwnd = ::FindWindow(_T("TLiveConversationWindow.UnicodeClass"), NULL);	// 5,0
		}
		if (hwnd != NULL) {
			// skype 5.0-
			hwnd = ::FindWindowEx(hwnd, 0, _T("TLiveConversation"), NULL); if (hwnd == NULL) return FALSE;
		} else {
			// skype 4.0-
			hwnd = ::FindWindow(_T("TVideoWindowForm.UnicodeClass"), NULL); 
			if (hwnd != NULL) {
				HWND hwndTmp1 = NULL;
				HWND hwndTmp2 = NULL;
				CRect rect1;
				CRect rect2;
				hwnd = ::FindWindowEx(hwnd, 0, _T("TConversationVideo"), NULL); if (hwnd == NULL) return FALSE;
				hwndTmp1 = ::FindWindowEx(hwnd, 0, _T("TCustomVideoWindow"), NULL); if (hwndTmp1 == NULL) return FALSE;
				hwndTmp2 = ::FindWindowEx(hwnd, hwndTmp1, _T("TCustomVideoWindow"), NULL);
				if (hwndTmp2 == NULL) {
					hwnd = hwndTmp1;
				} else {
					::GetWindowRect(hwndTmp1, rect1);
					::GetWindowRect(hwndTmp2, rect2);
					hwnd = (rect1.Width() > rect2.Width()) ? hwndTmp1 : hwndTmp2;
				}
			} else {
				hwnd = ::FindWindow(_T("TSkVideoWindow2"), NULL); 
				if (hwnd == NULL) {
					// skype3.1-
					hwnd = ::FindWindow(_T("TSkVideoWindow2.UnicodeClass"), NULL); if (hwnd == NULL) return FALSE;
				}
				::GetWindowRect(hwnd, pRect);

				hwnd = ::FindWindowEx(hwnd, 0, _T("tSkWindowedVideoControl"), NULL); if (hwnd == NULL) return FALSE;
			}
		}

		::GetWindowRect(hwnd, pRect);

	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("GetVideoWndLarge error"));
		return NULL;	
	}

	return hwnd;
}

//
//  �@�\     : Skype�̃r�f�I��ʗ̈�̐ݒ�i�ʃE�B���h�E�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CVideoDialog::SetVideoWndLarge(LPRECT pRect, BOOL blSizeOnly)
{
	try {
		long szx = pRect->right - pRect->left;
		long szy = pRect->bottom - pRect->top;
		HWND hwnd = ::FindWindow(_T("TSkVideoWindow2"), NULL); 
		if (hwnd == NULL) {
			// skype3.1-
			hwnd = ::FindWindow(_T("TSkVideoWindow2.UnicodeClass"), NULL); if (hwnd == NULL) return FALSE;
		}
		
		if (blSizeOnly) {
			::SetWindowPos(hwnd, 0, 0,0, szx, szy, SWP_DRAWFRAME | SWP_NOZORDER | SWP_NOMOVE);
		} else {
			::SetWindowPos(hwnd, 0, pRect->left, pRect->top, szx, szy, SWP_DRAWFRAME | SWP_NOZORDER);
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("SetVideoWndLarge error"));
		return FALSE;	
	}

	return TRUE;
}

//
//  �@�\     : Skype�̃r�f�I��ʂ�O�ʕ\���i�g�ݍ��݉�ʁj
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CVideoDialog::SetTopVideoWndLocal(BOOL blSetTop)
{
	try {
		HWND hwnd = ::FindWindow(_T("TConversationForm"), NULL);		// 5.1- ����window
		if (hwnd == NULL) {
			hwnd = ::FindWindow(_T("TConversationForm.UnicodeClass"), NULL);	// skype4.x ����window
		}
		if (hwnd == NULL) {
			hwnd = ::FindWindow(_T("TZapMainForm.UnicodeClass"), NULL); 	// skype4.00215�܂�
		}
		if (hwnd == NULL) {
			// skype 3.x. 4.0026-
			hwnd = ::FindWindow(_T("tSkMainForm.UnicodeClass"), NULL);
		}
		if (hwnd == NULL) {
			// 5.1-
			hwnd = ::FindWindow(_T("tSkMainForm"), NULL);	
		}
		if (hwnd == NULL) return FALSE;

		long style;
		if (blSetTop) {
			/// �O�ʕ\��
			style = ::GetWindowLong(hwnd, GWL_EXSTYLE);
			if (!(style & WS_EX_TOPMOST)) {
				::SetWindowPos(hwnd, HWND_TOPMOST, 0,0,0,0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE);
			}
			/// �ŏ����̋֎~
			style = ::GetWindowLong(hwnd, GWL_STYLE);
			if (style & (WS_MINIMIZE)) {	// �ŏ�����Ԃ��擾�ł��Ȃ��i�HSkype���C�x���g���I�[�o�[���[�h���Ă�j
	//		if (style & (WS_MINIMIZE | WS_MAXIMIZE)) {
				::ShowWindow(hwnd, SW_NORMAL);
			}
		} else {
			/// �O�ʕ\������
			style = ::GetWindowLong(hwnd, GWL_EXSTYLE);
			if (style & WS_EX_TOPMOST) {
				::SetWindowPos(hwnd, HWND_NOTOPMOST, 0,0,0,0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE);
			}
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("SetTopVideoWndLocal error"));
		return FALSE;	
	}

	return TRUE;
}

//
//  �@�\     : Skype�̃r�f�I��ʂ�O�ʕ\���i�ʃE�B���h�E�j
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CVideoDialog::SetTopVideoWndLarge(BOOL blSetTop)
{
	try {
		HWND hwnd = ::FindWindow(_T("TLiveConversationWindow.UnicodeClass"), (LPCTSTR)m_strSkypeId);
		if (hwnd == NULL) {
			// �r�f�I��c�ł͕������[�U�̃E�B���h�E���ɂȂ�
			hwnd = ::FindWindow(_T("TLiveConversationWindow.UnicodeClass"), NULL);
		}
		if (hwnd == NULL) {
			hwnd = ::FindWindow(_T("TVideoWindowForm.UnicodeClass"), NULL); // skype 4.0-
			if (hwnd == NULL) {
				hwnd = ::FindWindow(_T("TSkVideoWindow2"), NULL); 
				if (hwnd == NULL) {
					// skype3.1-
					hwnd = ::FindWindow(_T("TSkVideoWindow2.UnicodeClass"), NULL);
				}
			}
		}
		if (hwnd == NULL) return FALSE;

		long style;
		if (blSetTop) {
			/// �O�ʕ\��
			style = ::GetWindowLong(hwnd, GWL_EXSTYLE);
			if (!(style & WS_EX_TOPMOST)) {
				::SetWindowPos(hwnd, HWND_TOPMOST, 0,0,0,0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE);
			}
			/// �ŏ����̋֎~
			style = ::GetWindowLong(hwnd, GWL_STYLE);
			if (style & (WS_MINIMIZE)) {
	//		if (style & (WS_MINIMIZE | WS_MAXIMIZE)) {
				::ShowWindow(hwnd, SW_NORMAL);
			}
		} else {
			/// �O�ʕ\������
			style = ::GetWindowLong(hwnd, GWL_EXSTYLE);
			if (style & WS_EX_TOPMOST) {
				::SetWindowPos(hwnd, HWND_NOTOPMOST, 0,0,0,0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE);
			}
		}
	} catch(...) {
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_AVI, _T("SetTopVideoWndLarge error"));
		return FALSE;	
	}

	return TRUE;
}

//
//  �@�\     : VideoPlayer�̍Đ����
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
//BOOL CVideoDialog::IsPlayStatus(TxPlayerState status) 
//{
//	BOOL blRet = FALSE;
//
//	try {
//		long lngStatus = m_VideoGrab.GetPlayerState();
//		BOOL blRet = (lngStatus == status) ? TRUE:FALSE;
//	} catch(...) {
//		return FALSE;
//	}
//
//	return blRet;
//}
//
