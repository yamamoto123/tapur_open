/*
ACM�ϊ��N���X
���݂�WAV<->MP3�̂ݑΉ�
*/
#pragma once

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <afxtempl.h>
#include <Mmsystem.h>
#include <mmreg.h>
#include <Msacm.h>
#include "TPerror.h"
#include "WaveBuffer.h"
#include "MP3Info.h"

// ���ݎg�p�\��CODEC����
#define ACM_CODEC_MP3		_T("MPEG LAYER-3")
#define ACM_CODEC_MP3_LAME	_T("LAME MP3")


class CAcmMgr
{
	friend BOOL CALLBACK acmDriverEnumCallback(HACMDRIVERID hdriverId, DWORD dwInstance, DWORD dwSupport);
	friend BOOL CALLBACK acmFormatEnumCallback(HACMDRIVERID hdriverId, LPACMFORMATDETAILS lpFormatDetails, DWORD dwInstance, DWORD dwSupport);

	friend UINT ReadWavWorkerThread( LPVOID pParam );
	friend UINT SaveMp3WorkerThread( LPVOID pParam );
	friend UINT ReadMp3WorkerThread( LPVOID pParam );
	friend UINT SaveWavWorkerThread( LPVOID pParam );

public:
	static enum CnvType { WAV_MP3, WAV_MP3_LAME, MP3_WAV };	// �ϊ��^�C�v

private:
	CTPerror te;

	// for ACM
	CnvType m_CnvType;																	// �ϊ��^�C�v
	WAVEFORMATEX m_FromWf;																// �J�����g�ϊ����t�H�[�}�b�g
	WAVEFORMATEX m_ToWf;																// �J�����g�ϊ���t�H�[�}�b�g
	MPEGLAYER3WAVEFORMAT m_FromMf;														// �J�����g�ϊ����t�H�[�}�b�g
	MPEGLAYER3WAVEFORMAT m_ToMf;														// �J�����g�ϊ���t�H�[�}�b�g
	HACMDRIVERID m_idDriver;															// �J�����g�h���C�oID
	CMap<HACMDRIVERID, HACMDRIVERID, ACMDRIVERDETAILS, ACMDRIVERDETAILS> *m_dicDriver;	// �J�����g�h���C�o���X�g
	CTypedPtrMap<CMapStringToPtr, CString, LPWAVEFORMATEX> *m_dicFormat;					// �J�����g�t�H�[�}�b�g���X�g
	int GetDrivers(void);														// �J�����g�h���C�o���X�g�̎擾
	int GetFormats(CStringArray &aryStr, LPWAVEFORMATEX pFromWf, BOOL blSvr);	// �J�����g�t�H�[�}�b�g���X�g�̎擾
	void ResetFormats();														// �t�H�[�}�b�g���X�g�̉��

	// for �ϊ�����
	CString m_strSaveFile;			// �ۑ��t�@�C����
	CString m_strReadFile;			// �Ǎ��t�@�C����
	BOOL m_bSaveThread;				// �f�[�^�����X���b�h�̐������
	BOOL m_bReadThread;				// �f�[�^�Ǎ��X���b�h�̐������
	BOOL m_bSaveReset;				// �f�[�^�����X���b�h�̒��~�t���O
	BOOL m_bReadReset;				// �f�[�^�Ǎ��X���b�h�̒��~�t���O
	CPtrList m_listOfBuffer;		// ���͉����o�b�t�@
	UINT m_nBufferSize;				// ���͉����o�b�t�@�T�C�Y
	long m_lngRdSz;					// �ǂݍ��ݍς݃t�@�C���T�C�Y

public:
	CAcmMgr();
	~CAcmMgr(void);

	// for ACM
	BOOL GetFileFormat(CString strFile, LPWAVEFORMATEX pFromWf);			// �t�@�C���w�b�_����WAV�t�H�[�}�b�g�擾
	BOOL GetFileFormat(CString strFile, LPMPEGLAYER3WAVEFORMAT pFromMf);	// �t�@�C���w�b�_����MP3�t�H�[�}�b�g�擾
	int GetDriverList(CStringArray &aryStr);								// �h���C�o�iCodec�j���X�g�擾
	CString FindDriver(CnvType type, CString strCodec);						// �h���C�o�iCodec�j�I��
	int GetFormatList(CStringArray &aryStr, LPWAVEFORMATEX pFromWf, BOOL blSvr = TRUE);	// �t�H�[�}�b�g���X�g�擾
	BOOL FindFormat(CString strFormat);													// �t�H�[�}�b�g������t�H�[�}�b�g���擾
	BOOL FindFormat(LPMPEGLAYER3WAVEFORMAT pFromMf, LPWAVEFORMATEX pToMf = NULL);				// �ϊ����t�H�[�}�b�g����ϊ���t�H�[�}�b�g���擾

	// for �ϊ�����
	void SetCnvType(CnvType type) {m_CnvType = type;};
	CString GetSaveFile() {return m_strSaveFile;};	// �ۑ��t�@�C����
	CString GetReadFile() {return m_strReadFile;};	// �Ǎ��t�@�C����
	BOOL Record(CString strSaveFile);				// �o�b�t�@����t�@�C���ۑ�
	BOOL RecordStop();								// �t�@�C���ۑ����~
	BOOL IsRecording();								// �t�@�C���ۑ���������
	BOOL AddNewBuffer(void *pData, long lngSize);	// �o�b�t�@�փf�[�^�ǉ�
	void FreeListOfBuffer();						// ���͉����o�b�t�@�̉��
	CWaveBuffer* GetBuffer();						// �擪�f�[�^�o�b�t�@�̎Q��
	bool DelBuffer();								// �擪�f�[�^�o�b�t�@�̍폜
	BOOL Read(CString strReadFile);					// �o�b�t�@�փt�@�C���Ǎ�
	BOOL ReadStop();								// �t�@�C���Ǎ����~
	BOOL IsReading();								// �t�@�C���Ǎ���������

	// ��ʃc�[��
	BOOL ConvertWavToMp3(CWnd *hWin, CnvType type, CString strReadFile, CString strSaveFile, CString strCodec, CString strFormat, BOOL blWait);
	BOOL ConvertMp3ToWav(CWnd *hWin, CString strReadFile, CString strSaveFile, BOOL blWait);
};

