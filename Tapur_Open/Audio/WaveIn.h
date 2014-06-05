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
	CPtrList *m_plistOfSyncBuffer;	// 音声MIXのための同期バッファ(他のWaveInインスタンスから参照）
	int m_SyncSw;					// 0:自バッファと同期バッファの録音、1:自バッファのみ、2:同期バッファのみ
	int m_SyncStereo;				// 0:自バッファと同期バッファの合成録音(monoral)、1:ステレオ録音(完全パン)、2:ステレオ録音(調整パン)
	UINT m_nError;
	int	m_nIndexWaveHdr;
	UINT m_nBufferSize;
	WAVEHDR	m_tagWaveHdr[NUMWAVEINHDR];
	CWave m_wave;
	CWaveDevice m_waveDevice;
	CPlaySound* m_pPlaySound1;	// リアルタイム再生用デバイス1
	CPlaySound* m_pPlaySound2;	// リアルタイム再生用デバイス2
	CString m_strSaveFile;		// 保存ファイル名
	BOOL m_blWorkThread;		// データ書き込みスレッドの生存状態

	bool AddNewBuffer(WAVEHDR* pWaveHdr);
	bool AddNewHeader(HWAVEIN hwi);
	void FreeListOfBuffer();
	void FreeListOfHeader();
	void InitListOfHeader();
	bool IsError(MMRESULT nResult);
	bool ResetRequired();
	bool Record();		// ファイル保存共通関数

public:
	CWaveIn();
	CWaveIn(WAVEFORMATEX tagFormat, const CWaveDevice& aDevice);
	~CWaveIn();

	BOOL m_blVac;			// VACを使用するか
	BOOL m_blVirtualPause;	// 仮想停止機能
	CString m_strPlayDev1;	// リアルタイム再生用デバイス名1
	CString m_strPlayDev2;	// リアルタイム再生用デバイス名2
	BOOL m_blPlayDev1;		// リアルタイム再生用デバイス1を使用するか
	BOOL m_blPlayDev2;		// リアルタイム再生用デバイス1を使用するか

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
	bool Record(CString strSaveFile);	// ファイル保存
	bool Record(CWaveIn *pWaveIn);		// 他のインスタンスへのマージ保存
	bool Stop();
};

#endif // !defined(AFX_WAVEIN_H__2473839B_76B0_45EB_9F9A_386D27903BB1__INCLUDED_)
