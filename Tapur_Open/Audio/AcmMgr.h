/*
ACM変換クラス
現在はWAV<->MP3のみ対応
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

// 現在使用可能なCODEC名称
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
	static enum CnvType { WAV_MP3, WAV_MP3_LAME, MP3_WAV };	// 変換タイプ

private:
	CTPerror te;

	// for ACM
	CnvType m_CnvType;																	// 変換タイプ
	WAVEFORMATEX m_FromWf;																// カレント変換元フォーマット
	WAVEFORMATEX m_ToWf;																// カレント変換先フォーマット
	MPEGLAYER3WAVEFORMAT m_FromMf;														// カレント変換元フォーマット
	MPEGLAYER3WAVEFORMAT m_ToMf;														// カレント変換先フォーマット
	HACMDRIVERID m_idDriver;															// カレントドライバID
	CMap<HACMDRIVERID, HACMDRIVERID, ACMDRIVERDETAILS, ACMDRIVERDETAILS> *m_dicDriver;	// カレントドライバリスト
	CTypedPtrMap<CMapStringToPtr, CString, LPWAVEFORMATEX> *m_dicFormat;					// カレントフォーマットリスト
	int GetDrivers(void);														// カレントドライバリストの取得
	int GetFormats(CStringArray &aryStr, LPWAVEFORMATEX pFromWf, BOOL blSvr);	// カレントフォーマットリストの取得
	void ResetFormats();														// フォーマットリストの解放

	// for 変換処理
	CString m_strSaveFile;			// 保存ファイル名
	CString m_strReadFile;			// 読込ファイル名
	BOOL m_bSaveThread;				// データ書込スレッドの生存状態
	BOOL m_bReadThread;				// データ読込スレッドの生存状態
	BOOL m_bSaveReset;				// データ書込スレッドの中止フラグ
	BOOL m_bReadReset;				// データ読込スレッドの中止フラグ
	CPtrList m_listOfBuffer;		// 入力音声バッファ
	UINT m_nBufferSize;				// 入力音声バッファサイズ
	long m_lngRdSz;					// 読み込み済みファイルサイズ

public:
	CAcmMgr();
	~CAcmMgr(void);

	// for ACM
	BOOL GetFileFormat(CString strFile, LPWAVEFORMATEX pFromWf);			// ファイルヘッダからWAVフォーマット取得
	BOOL GetFileFormat(CString strFile, LPMPEGLAYER3WAVEFORMAT pFromMf);	// ファイルヘッダからMP3フォーマット取得
	int GetDriverList(CStringArray &aryStr);								// ドライバ（Codec）リスト取得
	CString FindDriver(CnvType type, CString strCodec);						// ドライバ（Codec）選択
	int GetFormatList(CStringArray &aryStr, LPWAVEFORMATEX pFromWf, BOOL blSvr = TRUE);	// フォーマットリスト取得
	BOOL FindFormat(CString strFormat);													// フォーマット名からフォーマットを取得
	BOOL FindFormat(LPMPEGLAYER3WAVEFORMAT pFromMf, LPWAVEFORMATEX pToMf = NULL);				// 変換元フォーマットから変換先フォーマットを取得

	// for 変換処理
	void SetCnvType(CnvType type) {m_CnvType = type;};
	CString GetSaveFile() {return m_strSaveFile;};	// 保存ファイル名
	CString GetReadFile() {return m_strReadFile;};	// 読込ファイル名
	BOOL Record(CString strSaveFile);				// バッファからファイル保存
	BOOL RecordStop();								// ファイル保存中止
	BOOL IsRecording();								// ファイル保存中か判別
	BOOL AddNewBuffer(void *pData, long lngSize);	// バッファへデータ追加
	void FreeListOfBuffer();						// 入力音声バッファの解放
	CWaveBuffer* GetBuffer();						// 先頭データバッファの参照
	bool DelBuffer();								// 先頭データバッファの削除
	BOOL Read(CString strReadFile);					// バッファへファイル読込
	BOOL ReadStop();								// ファイル読込中止
	BOOL IsReading();								// ファイル読込中か判別

	// 上位ツール
	BOOL ConvertWavToMp3(CWnd *hWin, CnvType type, CString strReadFile, CString strSaveFile, CString strCodec, CString strFormat, BOOL blWait);
	BOOL ConvertMp3ToWav(CWnd *hWin, CString strReadFile, CString strSaveFile, BOOL blWait);
};

