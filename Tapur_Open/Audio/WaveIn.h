// WaveIn.h: interface for the CWaveIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEIN_H__2473839B_76B0_45EB_9F9A_386D27903BB1__INCLUDED_)
#define AFX_WAVEIN_H__2473839B_76B0_45EB_9F9A_386D27903BB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
#include "Wave.h"
#include "WaveDevice.h"
#include "PlaySound.h"

//////////////////////////////////////////////////////////////////////
void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

//////////////////////////////////////////////////////////////////////
#ifdef WAVE_IN_BUFFER_SIZE
#undef WAVE_IN_BUFFER_SIZE
#endif
#define WAVEIN_BUFFER_SIZE 4096

#define NUMWAVEINHDR 2

//////////////////////////////////////////////////////////////////////
class CWaveIn
{
	friend void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	friend UINT WaveInWorkerThread( LPVOID pParam );

private:
	CTPerror te;
	bool m_bResetRequired;
	HWAVEIN	m_hWaveIn;
	CPtrList m_listOfBuffer;
	CPtrList *m_plistOfSyncBuffer;	// ����MIX�̂��߂̓����o�b�t�@(����WaveIn�C���X�^���X����Q�Ɓj
	int m_SyncSw;					// 0:���o�b�t�@�Ɠ����o�b�t�@�̘^���A1:���o�b�t�@�̂݁A2:�����o�b�t�@�̂�
	int m_SyncStereo;				// 0:���o�b�t�@�Ɠ����o�b�t�@�̍����^��(monoral)�A1:�X�e���I�^��(���S�p��)�A2:�X�e���I�^��(�����p��)
	UINT m_nError;
	int	m_nIndexWaveHdr;
	UINT m_nBufferSize;
	WAVEHDR	m_tagWaveHdr[NUMWAVEINHDR];
	CWave m_wave;
	CWaveDevice m_waveDevice;
	CPlaySound* m_pPlaySound1;	// ���A���^�C���Đ��p�f�o�C�X1
	CPlaySound* m_pPlaySound2;	// ���A���^�C���Đ��p�f�o�C�X2
	CString m_strSaveFile;		// �ۑ��t�@�C����
	BOOL m_blWorkThread;		// �f�[�^�������݃X���b�h�̐������

	bool AddNewBuffer(WAVEHDR* pWaveHdr);
	bool AddNewHeader(HWAVEIN hwi);
	void FreeListOfBuffer();
	void FreeListOfHeader();
	void InitListOfHeader();
	bool IsError(MMRESULT nResult);
	bool ResetRequired();
	bool Record();		// �t�@�C���ۑ����ʊ֐�

public:
	CWaveIn();
	CWaveIn(WAVEFORMATEX tagFormat, const CWaveDevice& aDevice);
	~CWaveIn();

	BOOL m_blVac;			// VAC���g�p���邩
	BOOL m_blVirtualPause;	// ���z��~�@�\
	CString m_strPlayDev1;	// ���A���^�C���Đ��p�f�o�C�X��1
	CString m_strPlayDev2;	// ���A���^�C���Đ��p�f�o�C�X��2
	BOOL m_blPlayDev1;		// ���A���^�C���Đ��p�f�o�C�X1���g�p���邩
	BOOL m_blPlayDev2;		// ���A���^�C���Đ��p�f�o�C�X1���g�p���邩

	void SetDevice(const CWaveDevice& aDevice);
	void SetWaveFormat(WAVEFORMATEX tagFormat);
	void SetSyncSw(int sw) { m_SyncSw = sw; }
	void SetSyncStereo(int val) { m_SyncStereo = val; }

	CString GetError() const;
	DWORD GetPosition();
	bool IsRecording();
	CWave MakeWave();

	bool Close();
	bool Continue();
	bool Open();
	bool Pause();
	bool Record(CString strSaveFile);	// �t�@�C���ۑ�
	bool Record(CWaveIn *pWaveIn);		// ���̃C���X�^���X�ւ̃}�[�W�ۑ�
	bool Stop();
};

#endif // !defined(AFX_WAVEIN_H__2473839B_76B0_45EB_9F9A_386D27903BB1__INCLUDED_)
