// WaveOut.h: interface for the CWaveOut class.
//
//////////////////////////////////////////////////////////////////////

/*/
	written for E-MED INNOVATIONS INC. by E-MED INNOVATIONS INC.
						08/2001
				http://www.e-medsounds.com
				mailto://pcanthelou@e-medsounds.com
/*/

#if !defined(AFX_WAVEOUT_H__FA0341BE_5A5D_40C5_80CD_ADCEDC734DEB__INCLUDED_)
#define AFX_WAVEOUT_H__FA0341BE_5A5D_40C5_80CD_ADCEDC734DEB__INCLUDED_

#include "Wave.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
#include "Wave.h"
#include "WaveDevice.h"
#include "PlaySound.h"
#include "AcmMgr.h"

//////////////////////////////////////////////////////////////////////
void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

//////////////////////////////////////////////////////////////////////
#ifdef WAVE_OUT_BUFFER_SIZE
#undef WAVE_OUT_BUFFER_SIZE
#endif
#define WAVEOUT_BUFFER_SIZE 4096

#define NUMWAVEOUTHDR 5
//#define NUMWAVEOUTHDR 3
#define INFINITE_LOOP INT_MAX

//////////////////////////////////////////////////////////////////////
class CWaveOut
{
	friend void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

public:
	static enum PlayType { WAV, MP3 };	// 再生タイプ

	CString GetError() const;
	DWORD GetPosition();
	bool IsPlaying();
	
	bool Close();
	bool Continue();
	bool FullPlay(int nLoop = -1, DWORD dwStart = -1, DWORD dwEnd = -1);
	bool Open(PlayType type);
	bool Pause();
	bool Play(DWORD dwStart = -1, DWORD dwEnd = -1);
	bool Stop();
	CWave m_wave;
	CAcmMgr m_oAcmMgr;

	void SetDevice(const CWaveDevice& aDevice);
	void SetWaveFormat(WAVEFORMATEX tagFormat);
	void SetPlayFormat(WORD nChannels, DWORD nSamplesPerSec, WORD wBitsPerSample);	// リアルタイム再生用フォーマット指定
	BOOL m_blVac;			// VACを使用するか
	CString m_strPlayDev1;	// リアルタイム再生用デバイス名1
	BOOL m_blPlayDev1;		// アルタイム再生用デバイス1を使用するか

	CWaveOut();
//	CWaveOut(const CWave& aWave, const CWaveDevice& aDevice);
	~CWaveOut();

private:
//	bool AddFullHeader(HWAVEOUT hwo, int nLoop);
	bool AddNewHeader(HWAVEOUT hwo);
	DWORD GetBufferLength();
	void FreeListOfHeader();
	void InitListOfHeader();
	bool IsError(MMRESULT nResult);
	//bool ResetRequired(CWaveOut* pWaveOut);

private:
	PlayType m_PlayType;	// 再生タイプ
	bool m_blStatus;
	bool m_bPlayReset;				// 再生中止フラグ
	HWAVEOUT m_hWaveOut;
	UINT m_nError;
	int m_nIndexWaveHdr;
	WAVEHDR m_tagWaveHdr[NUMWAVEOUTHDR];
	CWaveDevice m_waveDevice;
	CPlaySound* m_pPlaySound1;	// リアルタイム再生用デバイス1
	WAVEFORMATEX m_PlayFmt;		// リアルタイム再生用フォーマット
};

#endif // !defined(AFX_WAVEOUT_H__FA0341BE_5A5D_40C5_80CD_ADCEDC734DEB__INCLUDED_)
