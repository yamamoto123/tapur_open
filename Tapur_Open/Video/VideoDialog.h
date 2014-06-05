#if !defined(AFX_VIDEODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_VIDEODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VideoDialog.h : �w�b�_�[ �t�@�C��
//
#include <imm.h> 
#include "TPerror.h"
#include "videograbbervc6.h"
#include "videograbberConst.h"

// Recording Status
#define VIDEO_RECORD_RUNNING	1
#define VIDEO_RECORD_SUCCESS	2
#define VIDEO_RECORD_FAILED		3

// for videoecho123
#define VECHO_POSX 200
#define VECHO_POSY 10
#define VECHO_SZX  280
#define VECHO_SZY  240


/////////////////////////////////////////////////////////////////////////////
// CVideoDialog �_�C�A���O

class CVideoDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CVideoDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CVideoDialog();      

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CVideoDialog)
	enum { IDD = IDD_VIDEO };
	CVideoGrabberVC6	m_VideoGrab;
	//}}AFX_DATA

private:
	CTPerror te;
	int m_intRecordingStatus;	// �^�����
	CString m_strPlayFile;		// �Đ��t�@�C����
	CString m_strStreamPath;	// �X�g���[���Đ���ƃt�H���_
	CString m_strSkypeId;		// Skype Name or ID

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CVideoDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CVideoDialog)
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnResizeVideograbber();
	afx_msg void OnRecordingStartedVideograbber(LPCTSTR FileName);
	afx_msg void OnRecordingCompletedVideograbber(LPCTSTR FileName, BOOL Success);
	afx_msg void OnFrameCaptureCompletedVideograbber(long FrameBitmapHandle, long BitmapWidth, long BitmapHeight, long FrameNumber, double FrameTime, long DestType, LPCTSTR FileName, BOOL Success, long FrameId);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_blVSetTop;	// �r�f�I��ʂ�O�ʕ\���t���O

	void SetSkypeId(CString strSkypeId) {m_strSkypeId = strSkypeId; }
	void SetStreamPath(CString strPath) {m_strStreamPath = strPath; }
	int GetRecordingStatus() { return m_intRecordingStatus; }
	HWND GetVideoWndLocal(LPRECT pRect);
	HWND GetVideoWndLarge(LPRECT pRect);
	BOOL SetVideoWndLarge(LPRECT pRect, BOOL blSizeOnly = TRUE);
	BOOL SetTopVideoWndLocal(BOOL blSetTop);
	BOOL SetTopVideoWndLarge(BOOL blSetTop);
	//BOOL IsPlayStatus(TxPlayerState status);	// VideoPlayer�̍Đ����
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_VIDEODIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
