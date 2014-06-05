#if !defined(AFX_MAINTOOL_H__D4101CEA_8F09_498A_ABBE_81F0B717582C__INCLUDED_)
#define AFX_MAINTOOL_H__D4101CEA_8F09_498A_ABBE_81F0B717582C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainTool.h : �w�b�_�[ �t�@�C��
#include "Label.h"
#include "Audio.h"
#include "Video.h"

/////////////////////////////////////////////////////////////////////////////
// CMainTool �E�B���h�E

class CMainTool : public CToolBar
{
// �R���X�g���N�V����
public:
	CMainTool();

// �A�g���r���[�g
private:
	CAudio *m_pAudio;	// ��������N���X
	CVideo *m_pVideo;	// �r�f�I����N���X
	long m_lngRecordTime;	// �^���E�^�掞��(sec)
	//DWORD m_dwRecMod;	// �^���Ώۏ��

public:
	CLabel m_wndStatic;

// �I�y���[�V����
private:
	BOOL UpdateToolBar();		// �c�[���o�[�̍X�V

public:
	BOOL CreateStatic(CLabel& txtStatic,CString strText,UINT nIndex,UINT nID,int nWidth);
	BOOL Init(CWnd *pWnd);			// ������
	BOOL ReplaceIcon(int intPos, WORD iconId);	// �c�[���o�[�A�C�R���̕ύX
	BOOL UpadateRecordMod();		// �^���ΏۑI���{�^���̍X�V
	BOOL VRecordStart(CString strDate, BOOL blChat, BOOL blSilent = FALSE);	// �^��J�n�C�x���g
	BOOL RecordStart(CString strDate, BOOL blChat, BOOL blSilent = FALSE);	// �^���J�n�C�x���g
	BOOL VRecordPause();			// �^��ꎞ��~�C�x���g
	BOOL RecordPause();				// �^���ꎞ��~�C�x���g
	BOOL VRecordStop(BOOL blChat);	// �^��I���C�x���g
	BOOL RecordStop(BOOL blChat);	// �^���I���C�x���g
	void RecordMod();				// �^���ΏۑI���C�x���g

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMainTool)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMainTool();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMainTool)
	afx_msg void OnUpdateVRecordStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRecordStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRecordPause(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRecordStop(CCmdUI* pCmdUI);
//	afx_msg void OnRecordModBoth();
//	afx_msg void OnRecordModRcv();
//	afx_msg void OnRecordModSnd();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MAINTOOL_H__D4101CEA_8F09_498A_ABBE_81F0B717582C__INCLUDED_)
