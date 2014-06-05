// SkypeEvents.h

#ifndef __SKYPEEVENTS_H__
#define __SKYPEEVENTS_H__

#include "TPerror.h"
#include "TPsetup.h"
#include "Audio.h"
#include "Video.h"
#include "CallStatus.h"
#include "AviManager.h"

// Smart Pointer for SkypeAPI Interface
/*
typedef CComPtr<IAccess> IAccessPtr;
typedef CComPtr<ICall> ICallPtr;
typedef CComPtr<IMessage> IMessagePtr;
typedef CComPtr<IUser> IUserPtr;
typedef CComPtr<IChatMessage> IChatMessagePtr;
typedef CComPtr<ICallCollection> ICallCollectionPtr;
*/

class CAccessEvents :
  public CComObjectRoot,
  public IDispatchImpl<_IAccessEvents, &DIID__IAccessEvents, &LIBID_SKYPEAPILib>
{
public:
	CAccessEvents();
	~CAccessEvents();

BEGIN_COM_MAP(CAccessEvents)
  COM_INTERFACE_ENTRY(IDispatch)
  COM_INTERFACE_ENTRY_IID(DIID__IAccessEvents, IDispatch)
END_COM_MAP()

// IDispatch
	STDMETHOD(Invoke)(DISPID dispid, REFIID riid, LCID lcid, 
		WORD wFlags, DISPPARAMS* pdispParams, VARIANT* pvarResult, 
		EXCEPINFO* pexcepinfo, UINT* puArgErr);

	void SetMyParent(HWND hWnd) { m_hParent = hWnd; };

private:
	CTPerror te;
	HWND m_hParent;

// �C�x���g�̎���
private:
	HRESULT Result(BSTR Description);
	HRESULT Command(BSTR SkypeCommand);
	HRESULT UserStatusChanged(SkypeOnlineStatus Status);
	HRESULT MuteChanged(VARIANT_BOOL Mute);
	HRESULT ConnectionStatusChanged(SkypeConnectionStatus Status);
	HRESULT MessageReceived(IMessagePtr pSkypeMessage);
	HRESULT MessageSent(IMessagePtr pSkypeMessage);
	HRESULT APIStatusChanged(SkypeAPIAttachmentStatus Status);
	HRESULT CallHistoryChanged();
	HRESULT IMHistoryChanged();
	HRESULT Error(LONG Number, BSTR Description);
	HRESULT OnlineStatusChanged(IUserPtr pSkypeUser, SkypeOnlineStatus Status);
	HRESULT ChatMessageReceived(IChatMessagePtr pSkypeChatMessage);
	HRESULT ChatMessageSent(IChatMessagePtr pSkypeChatMessage);
public:
	HRESULT CallStatusChanged(ICallPtr pChangedCall, SkypeCallProgress Status);
	BOOL Response();
	
private:
	BOOL SetAppDevice(CString strInDev, CString strOutDev);		// Tapur�f�o�C�X�̐ݒ�

public:
	CTPsetup m_Setup;		// �����ݒ�N���X
	IAccessPtr m_pAccess;	// SkypeAPI Access�N���X
	ICallPtr m_pCall;		// ���݂�Skype Call�N���X
	CCallStatus m_CallStatus;	// ���݂�Skype Call���
	long m_lngProtocol;		// SKYPE�v���g�R��
	CString m_defInDevice;	// SKYPE�̃f�t�H���gIN�f�o�C�X
	CString m_defOutDevice;	// SKYPE�̃f�t�H���gOUT�f�o�C�X
	CString m_defVDevice;	// SKYPE�̃f�t�H���g�r�f�I�f�o�C�X
	CAudio *m_pAudio;		// ��������N���X
	CVideo *m_pVideo;		// �r�f�I����N���X
	//CAviManager *m_pAviManager[16];		// �r�f�I�{���������N���X
	CString m_ProcCode;		// ���݂̉����R�[�h
	BOOL blSkypeVideoVersion;	// �X�J�C�v�r�f�I�L���o�[�W�����t���O
	BOOL blResponse;		// �����������t���O
	BOOL blCallFinish;		// �ʘb�I���������t���O

	void SetAccess(IAccessPtr pAccess) { m_pAccess = pAccess; }
	BOOL PlayBeep(long lngSecond);		// BEEP��
	BOOL SendCall(CString strSender);	// Skype�R�[���J�n
	BOOL SendChat(CString strMsg, CString strUserId = _T(""));	// �`���b�g���b�Z�[�W���M
	BOOL IsTalking(BOOL blUpdate = FALSE);		// �ʘb�����ۂ�
	BOOL IsVTalking();			// �r�f�I��M�����ۂ�
	void SetResponsing(BOOL blSet) { blResponse = blSet; }		// �����������t���O�ݒ�
	void SetCallFinish(BOOL blSet) { blCallFinish = blSet; }	// �ʘb�I���������t���O�ݒ�
	BOOL IsResponsing() { return blResponse; }					// �����������t���O�Q��
	BOOL IsCallFinishing() { return blCallFinish; }				// �ʘb�I���������t���O�Q��
	CString GetSender();		// ���M�҂̖���orID
	BOOL LoadSetup();			// ���M�҂ɑΉ����������ݒ�����[�h
	int StartRecord(BOOL blVideo);	// �^���E�^��J�n���̏���
	BOOL FinishRecordTbl(CString strAudioFile, CString strVideoFile, BOOL blRepair = FALSE);	// �^���E�^�抮�����̃e�[�u������
	BOOL FinishRecord();		// �^���E�^�抮�����̏���
	BOOL ExecResp(int intPrgId);// �A�g�v���O�����̎��s
	BOOL InitUserData(void);	// ���[�U�f�[�^�̏�����
	BOOL InitDummyUser(void);	// SKYPE�ɐڑ��ł��Ȃ����̕\�����[�U�̓���
	BOOL InitDevice(void);		// Skype�f�o�C�X�̏�����
	BOOL VerUpUserData(void);	// ���[�U�P�ʂ̃f�[�^�x�[�X�̃o�[�W�����A�b�v
	//BOOL SetSkypeDevice(CString strInDev, CString strOutDev, BOOL blSilence = FALSE);	// Skype�f�o�C�X�̐ݒ�
	//BOOL SetSkypeDeviceTry(CString strInDev, CString strOutDev);	// Skype�f�o�C�X�̐ݒ�
	BOOL IsSkypeVideoVersion();		// Skype�r�f�I���L���ȃo�[�W�������H
	BOOL IsSkypeVideo(CString strUsr = _T(""));		// Skype�r�f�I���g�p�\���H
	//CString GetSkypeVDevice();	// Skype�r�f�I�f�o�C�X�̎擾
	BOOL SetSkypeVDevice(CString strInDev, BOOL blSilence = FALSE);	// Skype�r�f�I�f�o�C�X�̐ݒ�
	BOOL SetSkypeVDeviceTry(CString strInDev);	// Skype�r�f�I�f�o�C�X�̐ݒ�
	BOOL RefreshSkypeVideo(long lngCall, BOOL blStop = FALSE);		// �r�f�I�\�������t���b�V��
	int ShowVideoWin(CString strSkypeId, BOOL blLarge = TRUE);		// �r�f�I��ʂ�TAB���E�B���h�E�ɂ���
	BOOL FocusCallTab(CString strSkypeId);
	HWND FindTab(CString strTab);
	void KeyCtrlTab();
};


#endif	// __SKYPEEVENTS_H__

