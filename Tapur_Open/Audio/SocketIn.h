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
	BOOL m_bResetRequired;			// WorkerThreadの通常終了フラグ
	BOOL m_bResetForce;				// WorkerThreadの強制終了フラグ
	CPtrList m_listOfBufferIn;		// 入力音声受信用バッファ
	CPtrList m_listOfBufferMic;		// マイク音声受信用バッファ
	CPtrList m_listOfBufferOut;		// 外部（録画用ストリーム）転送用バッファ
	int m_SyncSw;					// 0:入力音声バッファとマイク音声バッファの録音、1:入力音声バッファのみ、2:同期音声バッファのみ
	int m_SyncStereo;				// 0:入力音声バッファとマイク音声バッファの合成録音(monoral)、1:ステレオ録音(完全パン)、2:ステレオ録音(調整パン)
	double m_dblVolSnd;				// 音量調整（送信）
	double m_dblVolRcv;				// 音量調整（受信）
	BOOL m_blStreamErrMsg;				// Stream errのチェック有効/無効
	BOOL m_blOutBuffer;				// 外部（録画用ストリーム）転送有効/無効

	CAcmMgr m_oAcmMgr;				// ACM変換マネージャ
	int m_intFormatType;			// 録音フォーマット、0:WAV, 1:MP3
	CString m_strCodec;				// 録音フォーマット、Codec名称
	CString m_strFormat;			// 録音フォーマット、Format名称

	UINT m_nError;
	UINT m_nBufferSize;
	CServerSocket m_sockServerIn;	// 入力音声受信用ソケット
	CServerSocket m_sockServerMic;	// マイク音声受信用ソケット
	CWave m_wave;
	CString m_strSaveFile;		// 保存ファイル名
	BOOL m_blWorkThread;		// データ書き込みスレッドの生存状態

	void FreeListOfBuffer();
	BOOL ResetRequired();
	BOOL Record();		// ファイル保存共通関数

public:
	CSocketIn();
	CSocketIn(WAVEFORMATEX tagFormat);
	~CSocketIn();

	void SetWaveFormat(WAVEFORMATEX tagFormat);
	WAVEFORMATEX GetWaveFormat() { return m_wave.GetFormat(); }
	BOOL SetSyncSw(int sw);
	BOOL SetSyncStereo(int val);
	BOOL SetVolAdjust(int volSnd, int volRcv);	// 音量調整
	BOOL SetFormat(int intFormatType, CString strCodec, CString strFormat);	// 録音時のフォーマット
	void EnableStreamErr(BOOL bl) { m_blStreamErrMsg = bl; }
	void EnableOutBuffer(BOOL bl) { m_blOutBuffer = bl; }

	BOOL IsRecording();

	BOOL AddOutBuffer(void *pData, long lngSize);	// 音声データを外部（録画用ストリーム）へ転送
	CPtrList *GetOutListBuffer();					// 外部（録画用ストリーム）転送用バッファ参照

	BOOL Open(UINT uiPortIn, UINT uiPortMic);
	BOOL Close();
	BOOL Continue();	
	BOOL Pause();		
	BOOL Record(CString strSaveFile);	// ファイル保存
	BOOL Stop();
};

#endif // !defined(AFX_SOCKETIN_H__2473839B_76B0_45EB_9F9A_386D27903BB1__INCLUDED_)
