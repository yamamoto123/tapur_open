// SocketIn.cpp: implementation of the CSocketIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SocketIn.h"
#include "FileSpec.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


//////////////////////////////////////////////////////////////////////
// ファイル書き込みスレッド
UINT SocketInWorkerThread( LPVOID pParam )
{
	CSocketIn* pSocketIn = (CSocketIn*)pParam;

	CFileException e;
	DWORD dwFileSize;
	DWORD dwNum;
	CFile f;
	CString strErr;
	SHORT tmpDat[SOCKET_IN_BUFFER_SIZE * 4];	// ステレオ録音時の一時バッファ
	WAVEFORMATEX Fmt = pSocketIn->m_wave.GetFormat();		// 保存用WAVEフォーマット

	CString strFile = pSocketIn->m_strSaveFile;
	if (strFile.GetLength() == 0) {
		goto THREAD_END;
	}

	TRY {
		/// 波形データの書き込み
		dwFileSize = 0;
		dwNum = 0;
		DWORD dwFmtSize = 0L;
		WORD wFormatTag = 0;
		int intCntStreamErr = 0;
		long i, cnt1 = 0, cnt2 = 0;
		long lngCnt, lngCnt2;
		long lngBufCnt1, lngBufCnt2;
		POSITION pos = NULL;
		CWaveBuffer *pBuf;
		CWaveBuffer *pSyncBuf;
		while(1) {
			if (pSocketIn->m_bResetForce) break;	// 強制終了

			// 初回だけ実行
			if (dwFmtSize == 0L) {
				/// 6秒以内にソケットにデータ来なければSkypeのストリームエラー（BUG）と考えられる。
				// (３者通話で最初のCallerが抜けた場合)
				if (!pSocketIn->m_sockServerIn.IsWorking() && 
						pSocketIn->m_listOfBufferIn.GetCount() == 0 && pSocketIn->m_listOfBufferMic.GetCount() == 0) {
					Sleep(10);  //CPU負荷減少
					++intCntStreamErr;
					if (intCntStreamErr > 600) {
						if (pSocketIn->m_blStreamErrMsg) {
							strErr.LoadString(IDS_MSGBOX811);
							_stprintf_s(pSocketIn->te.errstr, MAX_ERRSTR, strErr);
							pSocketIn->te.ErrorMessage(ERRTYPE_ERROR, _T(__FILE__), __LINE__, ERR_SOCK, pSocketIn->te.errstr);
						}
						goto THREAD_END;
					}
					continue;
				}

				dwFmtSize = 16L;
				wFormatTag = WAVE_FORMAT_PCM;
				if (pSocketIn->m_intFormatType == 0) {		// WAV
					/// ファイルオープン
					if( !f.Open(strFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e ) ) {
						_stprintf_s(pSocketIn->te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), strFile, e.m_cause);
						pSocketIn->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, pSocketIn->te.errstr);
						goto THREAD_END;
					}
					/// 最初の書き込み（ヘッダも書き込み）
					f.Write("RIFF", 4) ;
					f.Write(&dwFileSize, sizeof(dwFileSize)) ;
					f.Write("WAVEfmt ", 8) ;	// 大文字小文字は識別されるので注意！
					f.Write(&dwFmtSize, sizeof(dwFmtSize)) ;
					f.Write(&wFormatTag, sizeof(wFormatTag)) ;
					f.Write(&Fmt.nChannels, sizeof(Fmt.nChannels)) ;
					f.Write(&Fmt.nSamplesPerSec, sizeof(Fmt.nSamplesPerSec)) ;
					f.Write(&Fmt.nAvgBytesPerSec, sizeof(Fmt.nAvgBytesPerSec)) ;
					f.Write(&Fmt.nBlockAlign, sizeof(Fmt.nBlockAlign)) ;
					f.Write(&Fmt.wBitsPerSample, sizeof(Fmt.wBitsPerSample)) ;
					f.Write("data", 4) ;
					f.Write(&dwNum, sizeof(dwNum)) ;
				}
			}

			if (pSocketIn->m_bResetRequired) {
			//// 残りのバッファを処理して録音終了
				cnt1 = pSocketIn->m_listOfBufferIn.GetCount();
				cnt2 = pSocketIn->m_listOfBufferMic.GetCount();
				if (cnt1 > 0 && cnt2 > 0) {
					// 残りのバッファを処理
				} else {
					// 残りのバッファを破棄
					if (cnt1 == 0) {
						for(i = 0; i < cnt2; i++) {
							pos = pSocketIn->m_listOfBufferMic.GetHeadPosition();
							if (pos == NULL)  break;
							pSyncBuf = (CWaveBuffer *)pSocketIn->m_listOfBufferMic.GetAt(pos);
							if (!pSyncBuf) break;
							if (pSyncBuf->m_blActive) break;		// バッファ書き込み中
							if (pSyncBuf) {
								delete pSyncBuf;
								pSyncBuf = NULL;
							}
							pSocketIn->m_listOfBufferMic.RemoveHead();
						}
						cnt2 = pSocketIn->m_listOfBufferMic.GetCount();
					} else if (cnt2 == 0) {
						for(i = 0; i < cnt1; i++) {
							pos = pSocketIn->m_listOfBufferIn.GetHeadPosition();
							if (pos == NULL)  break;
							pBuf = (CWaveBuffer *)pSocketIn->m_listOfBufferIn.GetAt(pos);
							if (!pBuf) break;
							if (pBuf->m_blActive) break;		// バッファ書き込み中
							if (pBuf) {
								delete pBuf;
								pBuf = NULL;
							}
							pSocketIn->m_listOfBufferIn.RemoveHead();
						}
						cnt1 = pSocketIn->m_listOfBufferIn.GetCount();
					}
					if (cnt1 == 0 && cnt2 == 0) break;
				}
			} else {
				Sleep(10);  //CPU負荷減少
			//// 競合を防ぐため、バッファを１つ残して処理(バッファの生成待ち)
				if (pSocketIn->m_listOfBufferIn.GetCount() <= 1 || pSocketIn->m_listOfBufferIn.IsEmpty()) continue;
				if (pSocketIn->m_listOfBufferMic.GetCount() <= 1 || pSocketIn->m_listOfBufferMic.IsEmpty()) continue;
			}
			if (pSocketIn->m_listOfBufferIn.IsEmpty()) continue;
			pos = pSocketIn->m_listOfBufferIn.GetHeadPosition();
			if (pos == NULL)  continue;
			pBuf = (CWaveBuffer *)pSocketIn->m_listOfBufferIn.GetAt(pos);
			if (!pBuf) continue;
			if (pBuf->m_blActive) continue;		// バッファ書き込み中

			/// 波形データの書き込み
			//// 合成して保存
			pSyncBuf = (CWaveBuffer *)pSocketIn->m_listOfBufferMic.GetHead();
			if (!pSyncBuf) continue;
			if (pSyncBuf->m_blActive) continue;		// バッファ書き込み中
			///// 同期バッファのサンプリング数が超過する場合がある→同期バッファを一部削除して調整する
			lngBufCnt1 = pSocketIn->m_listOfBufferIn.GetCount();
			lngBufCnt2 = pSocketIn->m_listOfBufferMic.GetCount();
			if ((lngBufCnt1 + 1) < lngBufCnt2) {
				if (pSyncBuf) {
					delete pSyncBuf;
					pSyncBuf = NULL;
				}
				pSocketIn->m_listOfBufferMic.RemoveHead();
				continue;
			} else if ((lngBufCnt2 + 1) < lngBufCnt1) {
				if (pBuf) {
					delete pBuf;
					pBuf = NULL;
				}
				pSocketIn->m_listOfBufferIn.RemoveHead();
				continue;
			}

			if (!pBuf->m_blDead || !pSyncBuf->m_blDead) {
				///// 合成して保存
				// 両方のバッファが無効の場合は保存しない
				// 一方のバッファが無効な場合は、バッファ自体が無音なので問題はない
				SHORT *pDat = (SHORT *)pBuf->GetBuffer();
				SHORT *pSyncDat = (SHORT *)pSyncBuf->GetBuffer();
				long tmp = 0;
				long tmp1 = 0;
				long tmp2 = 0;
				lngCnt = pBuf->GetSampleSize() * pBuf->GetNumSamples() / sizeof(SHORT);
				lngCnt2 = pSyncBuf->GetSampleSize() * pSyncBuf->GetNumSamples() / sizeof(SHORT);
				if (lngCnt > lngCnt2) {
					lngCnt = lngCnt2;
				}
				for(i = 0; i < lngCnt ;i++) {
					if (pSocketIn->m_SyncStereo == 1) {
					////// ステレオ録音(完全パン)
						// WAVE合成
						tmpDat[i*2] = *(pSyncDat+i) + (SHORT)((double)*(pSyncDat+i) * pSocketIn->m_dblVolSnd);
						if (tmpDat[i*2] > 32767) {
							tmpDat[i*2] = 32767;
						} else if (tmpDat[i*2] < -32767) {
							tmpDat[i*2] = -32767;
						}
						tmpDat[i*2+1] = *(pDat+i) + (SHORT)((double)*(pDat+i) * pSocketIn->m_dblVolRcv);
						if (tmpDat[i*2+1] > 32767) {
							tmpDat[i*2+1] = 32767;
						} else if (tmpDat[i*2+1] < -32767) {
							tmpDat[i*2+1] = -32767;
						}
					} else if (pSocketIn->m_SyncStereo == 2) {
					////// ステレオ録音(調整パン)
						tmp1 = *(pSyncDat+i) + (SHORT)((double)*(pSyncDat+i) * pSocketIn->m_dblVolSnd);
						tmp2 = *(pDat+i) + (SHORT)((double)*(pDat+i) * pSocketIn->m_dblVolRcv);
						// WAVE合成
						tmp = (long)(tmp1 * 1.0) + (long)(tmp2 * 0.2);
						if (tmp > 32767) {
							tmpDat[i*2] = 32767;
						} else if (tmp < -32767) {
							tmpDat[i*2] = -32767;
						} else {
							tmpDat[i*2] = (SHORT)tmp;
						}
						tmp = (long)(tmp2 * 1.0) + (long)(tmp1 * 0.2);
						if (tmp > 32767) {
							tmpDat[i*2+1] = 32767;
						} else if (tmp < -32767) {
							tmpDat[i*2+1] = -32767;
						} else {
							tmpDat[i*2+1] = (SHORT)tmp;
						}
					} else {
					////// monoral
						// WAVE合成
						tmp1 = *(pSyncDat+i) + (SHORT)((double)*(pSyncDat+i) * pSocketIn->m_dblVolSnd);
						tmp2 = *(pDat+i) + (SHORT)((double)*(pDat+i) * pSocketIn->m_dblVolRcv);
						tmp = tmp1 + tmp2;
						if (tmp > 32767) {
							*(pDat+i) = 32767;
						} else if (tmp < -32767) {
							*(pDat+i) = -32767;
						} else {
							*(pDat+i) = (SHORT)tmp;
						}
					}
				}

				///// ファイルにデータ書き込み
				void *pData = NULL;
				long lngSize = 0;
				if (pSocketIn->m_SyncStereo > 0) {
					// stereoのため、バッファサイズは倍になる
					pData = tmpDat;
					lngSize = pBuf->GetSampleSize() * pBuf->GetNumSamples() * 2;
				} else {
					pData = pBuf->GetBuffer();
					lngSize = pBuf->GetSampleSize() * pBuf->GetNumSamples();
				}
				pSocketIn->AddOutBuffer(pData, lngSize);	// 録画用に転送
				if (pSocketIn->m_intFormatType == 0) {		// WAV
					f.Write(pData, lngSize);
				} else {	// MP3
					// ACMマネージャにデータを送る
					pSocketIn->m_oAcmMgr.AddNewBuffer(pData, lngSize);
				}
			}
						
			//// バッファの解放
			if (pBuf) {
				delete pBuf;
				pBuf = NULL;
			}
			pSocketIn->m_listOfBufferIn.RemoveHead();
			if (pSyncBuf) {
				delete pSyncBuf;
				pSyncBuf = NULL;
			}
			pSocketIn->m_listOfBufferMic.RemoveHead();
//TRACE(_T("list I:%ld Y:%ld\n"), pSocketIn->m_listOfBufferIn.GetCount(), pSocketIn->m_listOfBufferMic.GetCount());
		} //while

	}
    CATCH(CFileException, e)
    {
		switch(e->m_cause) {
		case CFileException::diskFull:
			AfxMessageBox(_T("HardDisk is full! finish this application."));
		default:
			if (pSocketIn->m_intFormatType == 0) f.Close();
			_stprintf_s(pSocketIn->te.errstr, MAX_ERRSTR, _T("File write error:%s, %s\n"), strFile, e->m_cause);
			pSocketIn->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, pSocketIn->te.errstr);
			goto THREAD_END;
		}
	}
    AND_CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(pSocketIn->te.errstr, MAX_ERRSTR, _T("Record error:%s, %s\n"), strFile, szCause);
		pSocketIn->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, pSocketIn->te.errstr);
    }
	END_CATCH_ALL

	//// ヘッダの修正
	TRY {
		if (pSocketIn->m_intFormatType == 0) {		// WAV
			dwFileSize = (DWORD)(f.GetLength() - 8);
			dwNum = dwFileSize - 36;
			f.Seek(4, CFile::begin);
			f.Write(&dwFileSize, sizeof(dwFileSize));
			f.Seek(40, CFile::begin);
			f.Write(&dwNum, sizeof(dwNum));
			f.Flush();
			f.Close();
		}
	}
    CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(pSocketIn->te.errstr, MAX_ERRSTR, _T("File header write error:%s, %s\n"), strFile, szCause);
		pSocketIn->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, pSocketIn->te.errstr);
		if (pSocketIn->m_intFormatType == 0) f.Close();
    }
	END_CATCH_ALL

THREAD_END:
	pSocketIn->m_blWorkThread = FALSE;
	return 0;
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
CSocketIn::CSocketIn(WAVEFORMATEX tagFormat) 
{
	m_intFormatType = 0;	
	m_strCodec.Empty();				
	m_strFormat.Empty();			
	m_dblVolSnd = m_dblVolRcv = 0.0;

	FreeListOfBuffer();
	m_blWorkThread = FALSE;
	m_bResetRequired = TRUE;
	m_bResetForce = FALSE;
	m_blStreamErrMsg = TRUE;
	m_blOutBuffer = FALSE;

	SetWaveFormat(tagFormat);
}

//////////////////////////////////////////////////////////////////////
CSocketIn::CSocketIn()
{
	m_intFormatType = 0;	
	m_strCodec.Empty();				
	m_strFormat.Empty();			
	m_dblVolSnd = m_dblVolRcv = 0.0;

	FreeListOfBuffer();
	m_blWorkThread = FALSE;
	m_bResetRequired = TRUE;
	m_bResetForce = FALSE;
	m_blStreamErrMsg = TRUE;
	m_blOutBuffer = FALSE;
}

//////////////////////////////////////////////////////////////////////
CSocketIn::~CSocketIn()
{
	Close();
	m_bResetForce = TRUE;	// WorkerThreadがまだ生きている場合は強制終了
	FreeListOfBuffer();
}

//////////////////////////////////////////////////////////////////////
void CSocketIn::SetWaveFormat(WAVEFORMATEX tagFormat)
{
	m_wave.BuildFormat(tagFormat.nChannels, tagFormat.nSamplesPerSec, tagFormat.wBitsPerSample, FALSE);
}

//////////////////////////////////////////////////////////////////////
// Son
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
BOOL CSocketIn::Open(UINT uiPortIn, UINT uiPortMic)
{
	TRY {
		// ソケット受信用バッファの設定
		m_sockServerIn.SetListBuffer(&m_listOfBufferIn);
		m_sockServerMic.SetListBuffer(&m_listOfBufferMic);

		/// 入力音声用ソケットを作成する
		if(m_sockServerIn.Create(uiPortIn) == FALSE)
		{
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket Create error In:%d:%d"), m_sockServerIn.GetLastError(), uiPortIn);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
			return FALSE;
		}

		/// マイク音声用ソケットを作成する
		if(m_sockServerMic.Create(uiPortMic) == FALSE)
		{
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket Create error Mic:%d:%d"), m_sockServerMic.GetLastError(), uiPortMic);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
			return FALSE;
		}

		/// 入力音声用ソケットを受信待ちにする
		if( m_sockServerIn.Listen() == FALSE)
		{
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket Listen error In:%d"), m_sockServerIn.GetLastError());
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
			return FALSE;
		}

		/// マイク音声用ソケットを受信待ちにする
		if( m_sockServerMic.Listen() == FALSE)
		{
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket Listen error Mic:%d"), m_sockServerMic.GetLastError());
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
			return FALSE;
		}
	}
    CATCH_ALL(e)
    {
		return FALSE;
    }
	END_CATCH_ALL

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
BOOL CSocketIn::Close()
{
	return Stop();
}

//////////////////////////////////////////////////////////////////////
BOOL CSocketIn::Continue()
{
	if (m_SyncSw == 0) {
		m_sockServerIn.Continue();
		m_sockServerMic.Continue();
	} else if (m_SyncSw == 1) {
		m_sockServerIn.Continue();
		m_sockServerMic.Pause();
	} else if (m_SyncSw == 2) {
		m_sockServerIn.Pause();
		m_sockServerMic.Continue();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
BOOL CSocketIn::Pause()
{
	m_sockServerIn.Pause();
	m_sockServerMic.Pause();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// 録音対象の制御
// 0:両方、1:Skype音声のみ、2:MIC音声のみ
BOOL CSocketIn::SetSyncSw(int sw)
{
	m_SyncSw = sw;

	// PAUSE中の場合、ソケットクラスの変更はなし
	if (m_sockServerIn.IsPause() && m_sockServerMic.IsPause()) return TRUE;

	// ソケットクラスに受信停止指示
	return Continue();
}

// 録音音質の制御
// 0:mono、1:stereo、2:natural stereo
BOOL CSocketIn::SetSyncStereo(int val)
{
	m_SyncStereo = val;

	// 保存用WAVEフォーマット
	if (m_SyncStereo > 0) {
		m_wave.BuildFormat(2, 16000, 16, FALSE);
	} else {
		m_wave.BuildFormat(1, 16000, 16, FALSE);
	}

	return TRUE;
}

// 音量調整
// % -> 実数
BOOL CSocketIn::SetVolAdjust(int volSnd, int volRcv)
{
	m_dblVolSnd = (double)volSnd / 100.0;				// 音量調整（送信）
	m_dblVolRcv = (double)volRcv / 100.0;				// 音量調整（受信）

	return TRUE;
}

//
//  機能     : 録音時のフォーマット
//  
//  機能説明 : 現在はMP3録音時のみ有効、WAV録音時はフォーマット固定
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CSocketIn::SetFormat(
int intFormatType,		// 0:WAV, 1:MP3
CString strCodec,		// Codec名称
CString strFormat)		// Format名称
{
	m_intFormatType = intFormatType;	// 録音フォーマット、0:WAV, 1:MP3
	m_strCodec = strCodec;				// 録音フォーマット、Codec名称
	m_strFormat = strFormat;			// 録音フォーマット、Format名称

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
BOOL CSocketIn::Record(CString strSaveFile)
{
	if ( !Stop() ) {
		return FALSE;
	}

	m_bResetRequired = FALSE;
	FreeListOfBuffer();
	m_nBufferSize = SOCKET_IN_BUFFER_SIZE;

	if (m_intFormatType > 0) {		// MP3
		/// CODEC/Format指定
		CString strDriver; 
		m_strCodec.MakeUpper();
		if (m_strCodec.Find(ACM_CODEC_MP3_LAME)) {
			strDriver = m_oAcmMgr.FindDriver(CAcmMgr::WAV_MP3_LAME, m_strCodec);
		} else {
			strDriver = m_oAcmMgr.FindDriver(CAcmMgr::WAV_MP3, m_strCodec);
		}
		if (strDriver.GetLength() <= 0) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Record error:%s"), strDriver);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
			return FALSE;
		}
		//// フォーマットリストを更新
		WAVEFORMATEX Fmt = m_wave.GetFormat();
		CStringArray aryStr;
		if (m_oAcmMgr.GetFormatList(aryStr, &Fmt) <= 0) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Record error:%d"), aryStr.GetSize());
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
			return FALSE;
		}
		if (!m_oAcmMgr.FindFormat(m_strFormat)) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Record error:%s"), m_strFormat);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
			return FALSE;
		}
	}

	m_strSaveFile = strSaveFile;
	if (m_strSaveFile.GetLength() <= 0) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Record error:%s"), m_strSaveFile);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
		return FALSE;
	}

	Sleep(500);		// 前回のスレッドが残っている場合があるので

	// メモリからファイルへの書き込みをスレッド化（巨大メモリを避けるため）
	if (m_intFormatType > 0) m_oAcmMgr.FreeListOfBuffer();	//MP3の時のACMバッファをクリア
	CWinThread* pThread = AfxBeginThread(SocketInWorkerThread, this);
	m_blWorkThread = (pThread == NULL) ? FALSE : TRUE;

	if (m_intFormatType > 0) {		// MP3
		// SocketInWorkerThreadから転送されたデータをMP3書き込み
		Sleep(100);  //バッファの生成待ち

		// 録音スレッド開始
		if (!m_oAcmMgr.Record(strSaveFile)) {
			m_oAcmMgr.RecordStop();
			return FALSE;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
BOOL CSocketIn::Stop()
{
	m_sockServerIn.Stop();
	m_sockServerMic.Stop();
	m_bResetRequired = TRUE;

	if (m_intFormatType > 0) {		// MP3
		// MP3録音の中止
		m_oAcmMgr.RecordStop();
	}

/* スレッドが終了するまで有効にする必要があるため、ここでは解放しない（次回Record時やクラスの消滅時に行われる）
		FreeListOfBuffer();
		FreeListOfHeader();
*/

	return TRUE;
}

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
void CSocketIn::FreeListOfBuffer()
{
	CWaveBuffer* pBuf = NULL;
	POSITION pos = NULL;

	pos = m_listOfBufferIn.GetHeadPosition();
	while (pos) {
		pBuf = (CWaveBuffer*)m_listOfBufferIn.GetNext(pos);
		if (pBuf) {
			delete pBuf;
			pBuf = NULL;
		}
	}
	m_listOfBufferIn.RemoveAll();

	pos = m_listOfBufferMic.GetHeadPosition();
	while (pos) {
		pBuf = (CWaveBuffer*)m_listOfBufferMic.GetNext(pos);
		if (pBuf) {
			delete pBuf;
			pBuf = NULL;
		}
	}
	m_listOfBufferMic.RemoveAll();

	pos = m_listOfBufferOut.GetHeadPosition();
	while (pos) {
		pBuf = (CWaveBuffer*)m_listOfBufferOut.GetNext(pos);
		if (pBuf) {
			delete pBuf;
			pBuf = NULL;
		}
	}
	m_listOfBufferOut.RemoveAll();
}


//////////////////////////////////////////////////////////////////////
// GET
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
BOOL CSocketIn::IsRecording()
{
	BOOL bResult = FALSE;

	if (m_intFormatType > 0) {		// MP3
		// MP3録音中
		if (m_oAcmMgr.IsRecording()) return TRUE;
	}

	if (m_bResetRequired) {
		// 録音中でない
		if (m_blWorkThread) {
			// 録音中でないが、録音時のスレッドが未だ動作している
			bResult = TRUE;
		} else {
			bResult = FALSE;
		}
	} else {
		// 録音中
		bResult = TRUE;
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////
BOOL CSocketIn::ResetRequired()
{
	return m_bResetRequired;
}

// 音声データを外部（録画用ストリーム）へ転送
BOOL CSocketIn::AddOutBuffer(void *pData, long lngSize)
{
	// バッファが消費されていない場合、データを追加しない
	if (m_listOfBufferOut.GetCount() > 100) return FALSE;

	CWaveBuffer *pBuf = new CWaveBuffer;

	pBuf->m_blActive = TRUE;	// スレッドによるアクセスを制限するフラグ

	m_listOfBufferOut.AddTail(pBuf);
	( (CWaveBuffer*)m_listOfBufferOut.GetTail() )->CopyBuffer(pData, lngSize / 4, 4);	// 4:16bit stereo

	pBuf->m_blActive = FALSE;

	return TRUE;
}

// 外部（録画用ストリーム）転送用バッファ参照
CPtrList * CSocketIn::GetOutListBuffer()
{
	return &m_listOfBufferOut;
}
