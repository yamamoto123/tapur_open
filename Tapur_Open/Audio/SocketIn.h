// SocketIn.h: interface for the CSocketIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETIN_H__2473839B_76B0_45EB_9F9A_386D27903BB1__INCLUDED_)
#define AFX_SOCKETIN_H__2473839B_76B0_45EB_9F9A_386D27903BB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
#include "ServerSocket.h"
#include "Wave.h"
#include "WaveDevice.h"
#include "AcmMgr.h"

//////////////////////////////////////////////////////////////////////
class CSocketIn
{
	friend UINT SocketInWorkerThread( LPVOID pParam );

private:
	CTPerror te;
	BOOL m_bResetRequired;			// WorkerThread�̒ʏ�I���t���O
	BOOL m_bResetForce;				// WorkerThread�̋����I���t���O
	CPtrList m_listOfBufferIn;		// ���͉�����M�p�o�b�t�@
	CPtrList m_listOfBufferMic;		// �}�C�N������M�p�o�b�t�@
	CPtrList m_listOfBufferOut;		// �O���i�^��p�X�g���[���j�]���p�o�b�t�@
	int m_SyncSw;					// 0:���͉����o�b�t�@�ƃ}�C�N�����o�b�t�@�̘^���A1:���͉����o�b�t�@�̂݁A2:���������o�b�t�@�̂�
	int m_SyncStereo;				// 0:���͉����o�b�t�@�ƃ}�C�N�����o�b�t�@�̍����^��(monoral)�A1:�X�e���I�^��(���S�p��)�A2:�X�e���I�^��(�����p��)
	double m_dblVolSnd;				// ���ʒ����i���M�j
	double m_dblVolRcv;				// ���ʒ����i��M�j
	BOOL m_blStreamErrMsg;				// Stream err�̃`�F�b�N�L��/����
	BOOL m_blOutBuffer;				// �O���i�^��p�X�g���[���j�]���L��/����

	CAcmMgr m_oAcmMgr;				// ACM�ϊ��}�l�[�W��
	int m_intFormatType;			// �^���t�H�[�}�b�g�A0:WAV, 1:MP3
	CString m_strCodec;				// �^���t�H�[�}�b�g�ACodec����
	CString m_strFormat;			// �^���t�H�[�}�b�g�AFormat����

	UINT m_nError;
	UINT m_nBufferSize;
	CServerSocket m_sockServerIn;	// ���͉�����M�p�\�P�b�g
	CServerSocket m_sockServerMic;	// �}�C�N������M�p�\�P�b�g
	CWave m_wave;
	CString m_strSaveFile;		// �ۑ��t�@�C����
	BOOL m_blWorkThread;		// �f�[�^�������݃X���b�h�̐������

	void FreeListOfBuffer();
	BOOL ResetRequired();
	BOOL Record();		// �t�@�C���ۑ����ʊ֐�

public:
	CSocketIn();
	CSocketIn(WAVEFORMATEX tagFormat);
	~CSocketIn();

	void SetWaveFormat(WAVEFORMATEX tagFormat);
	WAVEFORMATEX GetWaveFormat() { return m_wave.GetFormat(); }
	BOOL SetSyncSw(int sw);
	BOOL SetSyncStereo(int val);
	BOOL SetVolAdjust(int volSnd, int volRcv);	// ���ʒ���
	BOOL SetFormat(int intFormatType, CString strCodec, CString strFormat);	// �^�����̃t�H�[�}�b�g
	void EnableStreamErr(BOOL bl) { m_blStreamErrMsg = bl; }
	void EnableOutBuffer(BOOL bl) { m_blOutBuffer = bl; }

	BOOL IsRecording();

	BOOL AddOutBuffer(void *pData, long lngSize);	// �����f�[�^���O���i�^��p�X�g���[���j�֓]��
	CPtrList *GetOutListBuffer();					// �O���i�^��p�X�g���[���j�]���p�o�b�t�@�Q��

	BOOL Open(UINT uiPortIn, UINT uiPortMic);
	BOOL Close();
	BOOL Continue();	
	BOOL Pause();		
	BOOL Record(CString strSaveFile);	// �t�@�C���ۑ�
	BOOL Stop();
};

#endif // !defined(AFX_SOCKETIN_H__2473839B_76B0_45EB_9F9A_386D27903BB1__INCLUDED_)
