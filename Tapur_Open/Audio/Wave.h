// Wave.h: interface for the CWave class.
//
//////////////////////////////////////////////////////////////////////

/*/
	written for E-MED INNOVATIONS INC. by E-MED INNOVATIONS INC.
						08/2001
				http://www.e-medsounds.com
				mailto://pcanthelou@e-medsounds.com
/*/

#if !defined(AFX_WAVE_H__54B7993A_3F57_4AB0_9CCD_A55C2A1B2871__INCLUDED_)
#define AFX_WAVE_H__54B7993A_3F57_4AB0_9CCD_A55C2A1B2871__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>
#include <mmreg.h>
#include "WaveBuffer.h"	// Added by ClassView
#include "TPerror.h"

/// for VAC
#define NativeOpenGuidStr "00000000-0000-0000-0000-000000000000"
#define WAVE_FORMAT_NATIVE	0x1235
#pragma pack (push,1)
typedef struct tagVACFORMAT {
	WAVEFORMATEX Fmt;
	GUID Key;
} VACFORMAT, *LPVACFORMAT;
#pragma pack (pop)

#define VAC_IN_DEV1 _T("Tapur Virtual Cable 1 In")
#define VAC_OUT_DEV1 _T("Tapur Virtual Cable 1 Out")
#define VAC_IN_DEV2 _T("Tapur Virtual Cable 2 In")
#define VAC_OUT_DEV2 _T("Tapur Virtual Cable 2 Out")

struct __declspec (uuid (NativeOpenGuidStr)) OpenSigGuid;
/// for VAC

/*
// ファイルロードスレッド制御関数の引数として使用するクラス
class CUKWorkerParam
{
public:
	CUKWorkerParam() {};
	void SetParam(CWaveBuffer *wb, CString strFile, DWORD dwNum, DWORD dwPos)
	{
		m_pBuffer = wb;
		m_strFile = strFile;
		m_dwNum = dwNum;
		m_dwPos = dwPos;
	}
	CWaveBuffer* GetBuff() { return m_pBuffer; }
	CString GetFile() { return m_strFile; }
	DWORD GetNum() { return m_dwNum; }
	DWORD GetPos() { return m_dwPos; }
private:
	CWaveBuffer* m_pBuffer;
	CString m_strFile;
	DWORD m_dwNum;	
	DWORD m_dwPos;	
};
*/

class CWave
{
	friend UINT WaveWorkerThread( LPVOID pParam );

private:
	CTPerror te;
	CString m_strLoadFile;			// ロードファイル名
	DWORD m_dwHeaderSize;			// WAVE HEADER サイズ
	DWORD m_dwDataSize;				// WAVE DATA サイズ
	BOOL m_blLoadSw;				// ファイルLOADスレッドの制御変数
	bool Load(CFile* f, BOOL blVac);
	bool LoadFormat(CFile* f, BOOL blVac);

public:
	bool Load(CString strFile, BOOL blVac);
	void StopLoad() { m_blLoadSw = FALSE; }
	bool LoadFormat(const CString& strFile, BOOL blVac);
	DWORD GetDataLength() { return m_dwDataSize; }
	BOOL AddNewBuffer(void *pData, long lngSize);	// バッファへデータ追加
	void FreeListOfBuffer();		// 入力音声バッファの解放
	CWaveBuffer* GetBuffer();
	bool DelBuffer();
	WAVEFORMATEX GetFormat() const;
	void BuildFormat(WORD nChannels, DWORD nFrequency, WORD nBits, BOOL blVac = FALSE);
	CWave();
	CWave(const CWave& copy);
	CWave& operator=(const CWave& wave);
	~CWave();

public:
	CPtrList m_listOfBuffer;
//	CWaveBuffer m_buffer;
	VACFORMAT m_vacWaveFormat;
//	WAVEFORMATEX m_pcmWaveFormat;
};

#endif // !defined(AFX_WAVE_H__54B7993A_3F57_4AB0_9CCD_A55C2A1B2871__INCLUDED_)
