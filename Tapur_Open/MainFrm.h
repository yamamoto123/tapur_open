// MainFrm.h : CMainFrame �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CD11421F_56D8_4EB4_9104_11F0891E2316__INCLUDED_)
#define AFX_MAINFRM_H__CD11421F_56D8_4EB4_9104_11F0891E2316__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TPerror.h"
#include "VisualFx.h"
#include "SystemTray.h"
#include "ExtStatusControlBar.h"
#include "MainTool.h"
#include "Audio.h"
#include "TPerror.h"

//interface IAccess;
class CAccessEvents;

class CMainFrame : public CFrameWnd
{
	
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// �A�g���r���[�g
public:
	NOTIFYICONDATA m_stNtfyIcon;
	CSystemTray m_TrayIcon;

private:
	CTPerror te;
	BOOL InitSkypeAccess(void);	// �X�J�C�v�A�N�Z�X�I�u�W�F�N�g�̐���

// �I�y���[�V����
public:
	BOOL ConnectSkype(void);	// �X�J�C�v�ڑ�

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMainFrame)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnGraphNotify(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �R���g���[�� �o�[�p�����o
	CExtStatusControlBar  m_wndStatusBar;
	CReBar      m_wndReBar;
public:  // �R���g���[�� �o�[�p�����o
	CMainTool   m_wndToolBar;
	CDialogBar  m_wndDlgBar;

public:
	TVisualFramework m_Framework;
	TVisualObject *m_pMainTab;
	TVisualObject *m_pTabInbox;
	TVisualObject *m_pTabSetup;
	TVisualObject *m_pMsgTree;
	TVisualObject *m_pRsfTree;
	TVisualObject *m_pNestInbox;
	TVisualObject *m_pNestSetup;
	TVisualObject *m_pMsgList;
	TVisualObject *m_pRsfList;
	TVisualObject *m_pTabMsg;
	TVisualObject *m_pTabRsf;
	TVisualObject *m_pMsgForm;
	TVisualObject *m_pRsfSimpleForm;
	TVisualObject *m_pMsgFolderForm;
	TVisualObject *m_pRsfFolderForm;

	HICON m_hIcon;			// �ʏ�̃^�X�N�g���C�A�C�R��
	HICON m_hIconDis;		// �ڑ��s�̃^�X�N�g���C�A�C�R��
	HICON m_hIconNew;		// �V������̃^�X�N�g���C�A�C�R��

	int m_flgEnable;		// ���������̗L��/�����t���O
	int m_flgAutoEnable;	// �����^�������̗L��/�����t���O
	ST_TIMER m_stApp;			// APP����m�F�^�C�}
	ST_TIMER m_stConnect;		// �X�J�C�v�ڑ��҂��^�C�}
	ST_TIMER m_stAnswerWait;	// ����ԉ����҂��^�C�} 
	ST_TIMER m_stRecordMax;		// ����Ԙ^��MAX���ԃ`�F�b�N�^�C�} 
	ST_TIMER m_stPlayChk;		// �������b�Z�[�W�Đ��I���҂��^�C�} 

// �������ꂽ���b�Z�[�W �}�b�v�֐�
public:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPopupOpen();
	afx_msg void OnPopupStop();
	afx_msg void OnPopupEnable();
	afx_msg void OnPopupAutoEnable();
	afx_msg void OnPopupHotkey();
	afx_msg void OnPopupHelp();
	afx_msg void OnPopupSkype();
	afx_msg void OnPopupClose();
	afx_msg void OnClose();
	afx_msg void OnUpdatePopupStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupEnable(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupAutoEnable(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnVRecordStart();
	afx_msg void OnRecordStart();
	afx_msg void OnRecordPause();
	afx_msg void OnRecordStop();
	afx_msg void OnRecordMod();
	afx_msg void OnRecordModDropDown(NMTOOLBAR* pnmh, LRESULT* plRes);
	afx_msg void OnRecordModBoth();
	afx_msg void OnRecordModRcv();
	afx_msg void OnRecordModSnd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	DWORD m_dwCookie;

	BOOL m_blHotKey;			// HotKey�̗L������
	long m_lngNewMsg;			// �V�����b�Z�[�W�Ǘ��ϐ�
	BOOL ClearTmpData();		// TMP�t�H���_�N���A

public:
	CComObject<CAccessEvents>* m_pAccessEvents;
	CAudio *m_pAudio;	// ��������N���X
	CVideo *m_pVideo;	// �r�f�I����N���X

	void PushRecordStop() { OnRecordStop(); }

	void UpdateForm();			// ��ʂ̍X�V
	void InitTooltip();			// �c�[���`�b�v�̏�����
	BOOL StartAppTimer();		// APP����m�F�^�C�}�J�n
	BOOL StopAppTimer();		// APP����m�F�^�C�}�I��
	BOOL StartConnectTimer();	// �X�J�C�v�ڑ��҂��^�C�}�J�n
	BOOL StopConnectTimer();	// �X�J�C�v�ڑ��҂��^�C�}�I��
	BOOL StartAnswerWaitTimer();// ����ԉ����҂��^�C�}�J�n
	BOOL StopAnswerWaitTimer();	// ����ԉ����҂��^�C�}�I��
	BOOL StartRecordMaxTimer();	// ����Ԙ^��MAX���ԃ`�F�b�N�^�C�}�J�n
	BOOL StopRecordMaxTimer();	// ����Ԙ^��MAX���ԃ`�F�b�N�^�C�}�I��
	BOOL StartPlayChkTimer(long lngPlayTime);	// �������b�Z�[�W�Đ��I���҂��^�C�}�J�n
	BOOL StopPlayChkTimer();	// �������b�Z�[�W�Đ��I���҂��^�C�}�I��
	long RepairTmpData(long lngChkHour);	// ���o�^�̈ꎞ�f�[�^�̏C��
	BOOL IncNewMsgCnt(long lngCnt);		// �V�����b�Z�[�W���X�V
	BOOL SetStatusText(CString str, int idx = 0) { return m_wndStatusBar.SetPaneText(idx, str, TRUE); }
	void RegHotKey();			// HotKey�̓o�^
	void UnregHotKey();			// HotKey�̉���
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MAINFRM_H__CD11421F_56D8_4EB4_9104_11F0891E2316__INCLUDED_)
