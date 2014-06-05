/*
ACM変換クラス
現在はWAV<->MP3のみ対応
*/

#include "stdafx.h"
#include "AcmMgr.h"
#include "FileSpec.h"
#include "ProgressWnd.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

// ドライバ（Codec）リスト取得用コールバック
// dwInstance:CAcmMgrのオブジェクトアドレス参照
BOOL CALLBACK acmDriverEnumCallback(HACMDRIVERID hdriverId, DWORD dwInstance, DWORD dwSupport)
{
	CAcmMgr* pObj = (CAcmMgr*)dwInstance;

    if(dwSupport & ACMDRIVERDETAILS_SUPPORTF_CODEC) {
        MMRESULT mmr;
        ACMDRIVERDETAILS details;
        HACMDRIVER driver;
        UINT i;
        details.cbStruct = sizeof(ACMDRIVERDETAILS);
		// Driver詳細情報の取得
        mmr = acmDriverDetails(hdriverId,&details,0);
        mmr = acmDriverOpen(&driver,hdriverId,0);
        for(i=0;i<details.cFormatTags;i++) {
            ACMFORMATTAGDETAILS fmtDetails;
            ZeroMemory(&fmtDetails,sizeof(fmtDetails));
            fmtDetails.cbStruct = sizeof(ACMFORMATTAGDETAILS);
            fmtDetails.dwFormatTagIndex = i;
            mmr = acmFormatTagDetails(driver,&fmtDetails,
                                      ACM_FORMATTAGDETAILSF_INDEX);
            if(fmtDetails.dwFormatTag == WAVE_FORMAT_MPEGLAYER3){
				// Driver IDとDRIVERDETAILSのコレクション
				pObj->m_dicDriver->SetAt(hdriverId, details);
            }
        }
        mmr = acmDriverClose(driver,0);
    }

	return TRUE;
}

// フォーマットリスト取得用コールバック
// dwInstance:CAcmMgrのオブジェクトアドレス参照
BOOL CALLBACK acmFormatEnumCallback(HACMDRIVERID hdriverId, LPACMFORMATDETAILS lpFormatDetails, DWORD dwInstance, DWORD dwSupport)
{
	CAcmMgr* pObj = (CAcmMgr*)dwInstance;

	if (pObj->m_dicFormat->GetCount() > 0) {
		LPWAVEFORMATEX pwf;
		if (pObj->m_dicFormat->Lookup(CString(lpFormatDetails->szFormat), pwf)) return TRUE;	// フォーマット名の重複がある
	}

	// フォーマット文字列とWAVEFORMATEXのコレクション
	LPWAVEFORMATEX pwf = new WAVEFORMATEX;
	memcpy(pwf, lpFormatDetails->pwfx, sizeof(WAVEFORMATEX));
	pObj->m_dicFormat->SetAt(CString(lpFormatDetails->szFormat), pwf);

	return TRUE;
}


// MP3ファイル書き込みスレッド
UINT SaveMp3WorkerThread( LPVOID pParam )
{
	CAcmMgr* pAcmMgr = (CAcmMgr*)pParam;

	CFile *pfout = NULL;
	CFileException e;
	HACMDRIVER had = NULL;
	HACMSTREAM has = NULL;
	ACMSTREAMHEADER ash;
	MMRESULT res;
	LPBYTE pSrcBuf = NULL;	// 変換元バッファ
	LPBYTE pDstBuf = NULL;	// 変換先バッファ
	DWORD cbSrcBuf = 0; 
	DWORD cbDstBuf = 0;
	long lngCnt = 0;
	int leftover = 0;
	POSITION pos = NULL;
	CWaveBuffer *pBuf = NULL;

	/// Validation
	if (pAcmMgr->m_strSaveFile.GetLength() == 0) goto THREAD_END;
	
	/// ACM driver handleのオープン
	if (acmDriverOpen(&had, pAcmMgr->m_idDriver, 0) != 0) goto THREAD_END;

	/// ストリームオープン
	res = acmStreamOpen(&has, 
					had, 
					&pAcmMgr->m_FromWf, 
					(LPWAVEFORMATEX)&pAcmMgr->m_ToMf,
					NULL, 
					0, 
					0, 
					ACM_STREAMOPENF_NONREALTIME);
	if(res != S_OK) {
		goto THREAD_END;
	}

	TRY {
		/// データの書き込み
		while(1) {
			if (pAcmMgr->m_nBufferSize == 0) {
			//// 入力バッファが未生成
				if (pAcmMgr->m_bSaveReset) break;
				Sleep(10);  //CPU負荷減少
				continue;
			}

			//// バッファの確保(入力バッファの生成後、１回だけ実行される)
			if (pSrcBuf == NULL) {
				cbSrcBuf = pAcmMgr->m_nBufferSize; 
				///// 出力バッファサイズの推奨サイズを取得（実際は変動する）
				res = acmStreamSize(has, cbSrcBuf, &cbDstBuf, ACM_STREAMSIZEF_SOURCE);
				if(res != S_OK) {
					goto THREAD_END;
				}
				pSrcBuf = new BYTE[cbSrcBuf * 2];
				pDstBuf = new BYTE[cbDstBuf * 2];

				///// Streamヘッダのセット
				memset (&ash, 0, sizeof (ACMSTREAMHEADER));
				ash.cbStruct    = sizeof (ACMSTREAMHEADER);
				ash.pbSrc       = pSrcBuf;
				ash.cbSrcLength = cbSrcBuf;
				ash.pbDst       = pDstBuf;
				ash.cbDstLength = cbDstBuf;
				res = acmStreamPrepareHeader(has, &ash, 0);
				if(res != S_OK) {
					goto THREAD_END;
				}
				ash.cbDstLengthUsed = 0;
				ash.cbSrcLengthUsed = 0;
			}

			//// 入力バッファ状態チェック
			lngCnt = (long)pAcmMgr->m_listOfBuffer.GetCount();
			if (pAcmMgr->m_bSaveReset) {
			//// 録音終了
				if (lngCnt == 0) break;	// 終了
				// lngCnt > 0：残りのバッファを処理
			} else {
			//// 競合を防ぐため、バッファを１つ残して処理(バッファの生成待ち)
				if (lngCnt <= 1) {
					Sleep(10);  //CPU負荷減少
					continue;
				}
			}
			if (pAcmMgr->m_listOfBuffer.IsEmpty()) continue;
			pos = pAcmMgr->m_listOfBuffer.GetHeadPosition();
			if (pos == NULL)  continue;
			pBuf = (CWaveBuffer *)pAcmMgr->m_listOfBuffer.GetAt(pos);
			if (!pBuf) continue;
			if (pBuf->m_blActive) continue;		// バッファ書き込み中

			/// 出力ファイルオープン
			if (pfout == NULL) {
				pfout = new CFile();
				if( !pfout->Open(pAcmMgr->m_strSaveFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e ) ) {
					_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), pAcmMgr->m_strSaveFile, e.m_cause);
					pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
					goto THREAD_END;
				}
			}

			//// 変換処理
			if (!pBuf->m_blDead) {
				SHORT *pDat = (SHORT *)pBuf->GetBuffer();
				int sz = pBuf->GetNumSamples() * pBuf->GetSampleSize();		// ファイルの末尾では小さくなる
				memcpy(pSrcBuf + leftover, (BYTE *)pBuf->GetBuffer(), sz);
				ash.cbSrcLength = sz + leftover;	// 変換元バッファサイズの変動
				res = acmStreamConvert(has, &ash, ACM_STREAMCONVERTF_BLOCKALIGN);
				if(res != S_OK) {
					leftover = 0;
					break;
				}
				//// データの書き込み
				pfout->Write(pDstBuf, ash.cbDstLengthUsed);
				//// 使い残しのデータが発生する場合がある
				leftover = ash.cbSrcLength - ash.cbSrcLengthUsed;
				memcpy(pSrcBuf, pSrcBuf+ash.cbSrcLengthUsed, leftover);
			}
						
			//// バッファの解放
			if (pBuf) {
				delete pBuf;
				pBuf = NULL;
			}
			pAcmMgr->m_listOfBuffer.RemoveHead();
		} //while

		/// 最後の使い残しデータを処理
		while (leftover > 0) {
			ash.cbSrcLength = leftover;	// 変換元バッファサイズ
			res = acmStreamConvert(has, &ash, ACM_STREAMCONVERTF_BLOCKALIGN);
			if(res != S_OK) {
				break;
			}
			//// データの書き込み
			pfout->Write(pDstBuf, ash.cbDstLengthUsed);
			//// 更なる使い残しデータ
			leftover = ash.cbSrcLength - ash.cbSrcLengthUsed;
			memcpy(pSrcBuf, pSrcBuf+ash.cbSrcLengthUsed, leftover);
		}

	}
    CATCH(CFileException, e)
    {
		switch(e->m_cause) {
		case CFileException::diskFull:
			AfxMessageBox(_T("HardDisk is full! finish this application."));
		default:
			_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File write error:%s, %s\n"), pAcmMgr->m_strSaveFile, e->m_cause);
			pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
		}
	}
    AND_CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("Record error:%s, %s\n"), pAcmMgr->m_strSaveFile, szCause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
    }
	END_CATCH_ALL

THREAD_END:
	// 出力ファイルのClose
	if (pfout) {
		pfout->Flush();
		pfout->Close();
		delete pfout;
	}

	// ストリームのClose
	if (has != NULL) {
		ash.cbSrcLength = cbSrcBuf;
		ash.cbDstLength = cbDstBuf;
		acmStreamUnprepareHeader(has, &ash, 0);
		acmStreamClose(has, 0);
	}

	// ACM driver handleのクローズ
	if (had != NULL) acmDriverClose(had, 0);

	// 出力バッファの解放
	if (pSrcBuf) delete [] pSrcBuf;
	if (pDstBuf) delete [] pDstBuf;		

	pAcmMgr->m_bSaveThread = FALSE;
	return 0;
};

// WAVファイル書き込みスレッド
UINT SaveWavWorkerThread( LPVOID pParam )
{
	CAcmMgr* pAcmMgr = (CAcmMgr*)pParam;

	CFile fout;
	CFileException e;

	/// Validation
	if (pAcmMgr->m_strSaveFile.GetLength() == 0) goto THREAD_END;
	
	/// 出力ファイルオープン
	if( !fout.Open(pAcmMgr->m_strSaveFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e ) ) {
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), pAcmMgr->m_strSaveFile, e.m_cause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
		goto THREAD_END;
	}

	TRY {
		//// WAVEヘッダの書き込み
		fout.Write("RIFF", 4) ;
		DWORD dwFileSize = 0;	// dummy
		fout.Write(&dwFileSize, sizeof(dwFileSize)) ;
		fout.Write("WAVEfmt ", 8) ;	// 大文字小文字は識別されるので注意！
		DWORD dwFmtSize = 16L;
		fout.Write(&dwFmtSize, sizeof(dwFmtSize)) ;
		WORD wFormatTag = WAVE_FORMAT_PCM;
		fout.Write(&pAcmMgr->m_ToWf.wFormatTag, sizeof(pAcmMgr->m_ToWf.wFormatTag)) ;
		fout.Write(&pAcmMgr->m_ToWf.nChannels, sizeof(pAcmMgr->m_ToWf.nChannels)) ;
		fout.Write(&pAcmMgr->m_ToWf.nSamplesPerSec, sizeof(pAcmMgr->m_ToWf.nSamplesPerSec)) ;
		fout.Write(&pAcmMgr->m_ToWf.nAvgBytesPerSec, sizeof(pAcmMgr->m_ToWf.nAvgBytesPerSec)) ;
		fout.Write(&pAcmMgr->m_ToWf.nBlockAlign, sizeof(pAcmMgr->m_ToWf.nBlockAlign)) ;
		fout.Write(&pAcmMgr->m_ToWf.wBitsPerSample, sizeof(pAcmMgr->m_ToWf.wBitsPerSample)) ;
		fout.Write("data", 4) ;
		DWORD dwNum = 0;	// dummy
		fout.Write(&dwNum, sizeof(dwNum)) ;

		/// データの書き込み
		long lngCnt = 0;
		POSITION pos = NULL;
		CWaveBuffer *pBuf;
		while(1) {
			if (pAcmMgr->m_nBufferSize == 0) {
			//// 入力バッファが未生成
				if (pAcmMgr->m_bSaveReset) break;
				Sleep(10);  //CPU負荷減少
				continue;
			}

			//// 入力バッファ状態チェック
			lngCnt = (long)pAcmMgr->m_listOfBuffer.GetCount();
			if (pAcmMgr->m_bSaveReset) {
			//// 録音終了
				if (lngCnt == 0) break;	// 終了
				// lngCnt > 0：残りのバッファを処理
			} else {
			//// 競合を防ぐため、バッファを１つ残して処理(バッファの生成待ち)
				if (lngCnt <= 1) {
					Sleep(10);  //CPU負荷減少
					continue;
				}
			}
			if (pAcmMgr->m_listOfBuffer.IsEmpty()) continue;
			pos = pAcmMgr->m_listOfBuffer.GetHeadPosition();
			if (pos == NULL)  continue;
			pBuf = (CWaveBuffer *)pAcmMgr->m_listOfBuffer.GetAt(pos);
			if (!pBuf) continue;
			if (pBuf->m_blActive) continue;		// バッファ書き込み中

			//// 変換処理
			if (!pBuf->m_blDead) {
				//// データの書き込み
				void *pDat = pBuf->GetBuffer();
				int sz = pBuf->GetNumSamples() * pBuf->GetSampleSize();	
				fout.Write(pDat, sz);
			}
						
			//// バッファの解放
			if (pBuf) {
				delete pBuf;
				pBuf = NULL;
			}
			pAcmMgr->m_listOfBuffer.RemoveHead();
		} //while

	}
    CATCH(CFileException, e)
    {
		switch(e->m_cause) {
		case CFileException::diskFull:
			AfxMessageBox(_T("HardDisk is full! finish this application."));
		default:
			fout.Close();
			_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File write error:%s, %s\n"), pAcmMgr->m_strSaveFile, e->m_cause);
			pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
			goto THREAD_END;
		}
	}
    AND_CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("Record error:%s, %s\n"), pAcmMgr->m_strSaveFile, szCause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
    }
	END_CATCH_ALL

	TRY {
		//// WAVEヘッダの修正
		DWORD dwFileSize = (DWORD)(fout.GetLength() - 8);
		DWORD dwNum = dwFileSize - 36;
		fout.Seek(4, CFile::begin);
		fout.Write(&dwFileSize, sizeof(dwFileSize));
		fout.Seek(40, CFile::begin);
		fout.Write(&dwNum, sizeof(dwNum));

		//// 出力ファイルのClose
		fout.Flush();
		fout.Close();
	}
    CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File header write error:%s, %s\n"), pAcmMgr->m_strSaveFile, szCause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
		fout.Close();
    }
	END_CATCH_ALL

THREAD_END:
	pAcmMgr->m_bSaveThread = FALSE;
	return 0;
};

// WAVファイル読み込みスレッド
UINT ReadWavWorkerThread( LPVOID pParam )
{
	CAcmMgr* pAcmMgr = (CAcmMgr*)pParam;

	CFile fin;
	CFileException e;
	LPBYTE pSrcBuf = NULL;	// 変換元バッファ
	DWORD cbSrcBuf = WAVE_BUFFER_SIZE;	// バッファサイズ

	/// Validation
	if (pAcmMgr->m_strReadFile.GetLength() == 0) goto THREAD_END;
	
	/// バッファの確保
	pSrcBuf = new BYTE[cbSrcBuf];

	/// 入力ファイルオープン
	if( !fin.Open(pAcmMgr->m_strReadFile, CFile::modeRead | CFile::shareDenyWrite, &e ) ) {
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), pAcmMgr->m_strReadFile, e.m_cause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
		goto THREAD_END;
	}

	TRY {
		/// ヘッダのスキップ
		if (fin.Read(pSrcBuf, 44) <= 0) {
			goto THREAD_END;
		}

		/// データの読み込み
		long lngCnt = 0;
		int intRead = 0;
		pAcmMgr->m_lngRdSz = 0;
		while(1) {
			if (pAcmMgr->m_bReadReset) {
				break;	// 読み込み終了
			}

			// 読み込みが早すぎるとメモリを圧迫するため、処理待ちする
			lngCnt = (long)pAcmMgr->m_listOfBuffer.GetCount();
			if (lngCnt > 10) {
				Sleep(10);  //CPU負荷減少
				continue;
			}

			intRead = fin.Read(pSrcBuf, cbSrcBuf);
			if (intRead == 0) {
				break;
			} 
			pAcmMgr->m_lngRdSz += intRead;	// プログレス処理用

			pAcmMgr->AddNewBuffer(pSrcBuf, intRead);
			if (intRead < (int)cbSrcBuf) {
				break;
			} 
		} //while

		/// ファイルのClose
		fin.Close();
	}
    CATCH(CFileException, e)
    {
		fin.Close();
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File read error:%s, %s\n"), pAcmMgr->m_strReadFile, e->m_cause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
		goto THREAD_END;
	}
    AND_CATCH_ALL(e)
    {
		fin.Close();
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("Read error:%s, %s\n"), pAcmMgr->m_strReadFile, szCause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
		goto THREAD_END;
    }
	END_CATCH_ALL

THREAD_END:
	// 出力バッファの解放
	if (pSrcBuf) delete [] pSrcBuf;

	pAcmMgr->m_bReadThread = FALSE;
	return 0;
};

// MP3ファイル読み込みスレッド
UINT ReadMp3WorkerThread( LPVOID pParam )
{
	CAcmMgr* pAcmMgr = (CAcmMgr*)pParam;

	CFile fin;
	CFileException e;
	CFileSpec fs;
	HACMSTREAM has = NULL;
	ACMSTREAMHEADER ash;
	MMRESULT res;
	LPBYTE pSrcBuf = NULL;	// 変換元バッファ
	LPBYTE pDstBuf = NULL;	// 変換先バッファ
	DWORD cbSrcBuf = WAVE_BUFFER_SIZE / 8;	// 出力（圧縮展開）バッファのサイズを抑えるため、小さめに 
	DWORD cbDstBuf = 0;

	/// Validation
	if (pAcmMgr->m_strReadFile.GetLength() == 0) goto THREAD_END;
	
	/// ストリームオープン
	res = acmStreamOpen(&has, 
					NULL, 
					(LPWAVEFORMATEX)&pAcmMgr->m_FromMf, 
					&pAcmMgr->m_ToWf,
					NULL, 
					0, 
					0, 
					ACM_STREAMOPENF_NONREALTIME);
	if(res != S_OK) {
		goto THREAD_END;
	}

	/// 出力バッファサイズの推奨サイズを取得（実際は変動する）
	res = acmStreamSize(has, cbSrcBuf, &cbDstBuf, ACM_STREAMSIZEF_SOURCE);
	if(res != S_OK) {
		goto THREAD_END;
	}
	/// バッファの確保（変動する可能性があるため２倍）
	pSrcBuf = new BYTE[cbSrcBuf * 2];
	pDstBuf = new BYTE[cbDstBuf * 2];

	/// Streamヘッダのセット
	memset (&ash, 0, sizeof (ACMSTREAMHEADER));
	ash.cbStruct    = sizeof (ACMSTREAMHEADER);
	ash.pbSrc       = pSrcBuf;
	ash.cbSrcLength = cbSrcBuf;
	ash.pbDst       = pDstBuf;
	ash.cbDstLength = cbDstBuf;
	res = acmStreamPrepareHeader(has, &ash, 0);
	if(res != S_OK) {
		goto THREAD_END;
	}
	ash.cbDstLengthUsed = 0;
	ash.cbSrcLengthUsed = 0;

	/// ファイルサイズ(ProgressBar用)
	fs.SetFullSpec(pAcmMgr->m_strReadFile);
	long lngSz = (long)fs.FileSize();
	long lngRdSz = 0;
	int intPercent = 0;
	int intPercentBak = 0;

	/// 入力ファイルオープン
	if( !fin.Open(pAcmMgr->m_strReadFile, CFile::modeRead | CFile::shareDenyWrite, &e ) ) {
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), pAcmMgr->m_strReadFile, e.m_cause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
		goto THREAD_END;
	}

	TRY {
		/// データの読み込み
		long lngCnt = 0;
		int intRead = 0;
		int leftover = 0;
		pAcmMgr->m_lngRdSz = 0;
		while(1) {
			if (pAcmMgr->m_bReadReset) {
				break;	// 読み込み終了
			}

			// 読み込みが早すぎるとメモリを圧迫するため、処理待ちする
			lngCnt = (long)pAcmMgr->m_listOfBuffer.GetCount();
			if (lngCnt > 10) {
				Sleep(10);  //CPU負荷減少
				continue;
			}

			//// ファイル読み込み
			intRead = fin.Read(pSrcBuf + leftover, cbSrcBuf);
			if (intRead == 0) {
				break;
			}
			pAcmMgr->m_lngRdSz += intRead;	// プログレス処理用

			//// 変換処理
			ash.cbSrcLength = intRead + leftover;	// 変換元バッファサイズの変動
			res = acmStreamConvert(has, &ash, ACM_STREAMCONVERTF_BLOCKALIGN);
			if(res != S_OK) {
				break;
			}
			//// 使い残しのデータが発生する場合がある
			leftover = ash.cbSrcLength - ash.cbSrcLengthUsed;
			if (leftover > 0) {
				memcpy(pSrcBuf, pSrcBuf+ash.cbSrcLengthUsed, leftover);
			}

			//// バッファ書き込み
			pAcmMgr->AddNewBuffer(pDstBuf, ash.cbDstLengthUsed);

			if (intRead < (int)cbSrcBuf) {
				break;
			} 
		} //while

		/// ファイルのClose
		fin.Close();
	}
    CATCH(CFileException, e)
    {
		fin.Close();
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File read error:%s, %s\n"), pAcmMgr->m_strReadFile, e->m_cause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
		goto THREAD_END;
	}
    AND_CATCH_ALL(e)
    {
		fin.Close();
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("Read error:%s, %s"), pAcmMgr->m_strReadFile, szCause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
		goto THREAD_END;
    }
	END_CATCH_ALL

THREAD_END:
	// ストリームのClose
	if (has != NULL) {
		ash.cbSrcLength = cbSrcBuf;
		ash.cbDstLength = cbDstBuf;
		acmStreamUnprepareHeader(has, &ash, 0);
		acmStreamClose(has, 0);
	}

	// 出力バッファの解放
	if (pSrcBuf) delete [] pSrcBuf;
	if (pDstBuf) delete [] pDstBuf;		

	pAcmMgr->m_bReadThread = FALSE;
	return 0;
};


// コンストラクタ
CAcmMgr::CAcmMgr()
{
	m_idDriver = 0;
	m_bReadReset = TRUE;

	// ドライバリストのメモリ確保
	m_dicDriver = new CMap<HACMDRIVERID, HACMDRIVERID, ACMDRIVERDETAILS, ACMDRIVERDETAILS>(10);

	// フォーマットリストのメモリ確保
	m_dicFormat = new CTypedPtrMap<CMapStringToPtr, CString, LPWAVEFORMATEX>; 

	FreeListOfBuffer();
	m_bSaveThread = FALSE;
	m_bSaveReset = TRUE;
}

// デストラクタ
CAcmMgr::~CAcmMgr(void)
{
	// フォーマットリストの解放
	ResetFormats();
	delete(m_dicFormat);

	// ドライバリストの解放
	m_dicDriver->RemoveAll();
	delete(m_dicDriver);

	ReadStop();
	RecordStop();
	FreeListOfBuffer();
}

//////////////////////////////////////////////////////////////////////
// for ACM
//////////////////////////////////////////////////////////////////////

//
//  機能     : ドライバ（Codec）リスト取得
//  
//  機能説明 : 
//  
//  返り値   : ドライバリスト件数
//  
//  備考     : 
//  
int CAcmMgr::GetDrivers(void)
{
	// ドライバ（Codec）リストを空にする
	m_dicDriver->RemoveAll();

	// dwInstance:CAcmMgrのオブジェクトアドレス参照を渡す
	MMRESULT res;
	res = acmDriverEnum((ACMDRIVERENUMCB)acmDriverEnumCallback, (DWORD)(void*)this, 0);
	if (res) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("acmDriverEnum error:%d"),  res);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
		return 0;
	}
	
	return (int)m_dicDriver->GetCount();
}

//
//  機能     : ドライバリスト取得
//  
//  機能説明 : ドライバリストを更新する
//  
//  返り値   : ドライバリスト件数
//  
//  備考     : 
//  
int CAcmMgr::GetDriverList(
CStringArray &aryStr)		// 取得したドライバリスト
{
	// フォーマットリスト更新
	int intCnt = GetDrivers();
	if (intCnt <= 0) return 0;

	HACMDRIVERID rId;
	ACMDRIVERDETAILS rDriver;
	POSITION pos = m_dicDriver->GetStartPosition();
	aryStr.RemoveAll();
	while (pos != NULL) {
		m_dicDriver->GetNextAssoc(pos, rId, rDriver);
		aryStr.Add(CString(rDriver.szShortName));
	}

	return intCnt;
}

//
//  機能     : カレントドライバのフォーマットリスト取得
//  
//  機能説明 : 
//  
//  返り値   : フォーマットリスト件数
//  
//  備考     : 
//  
int CAcmMgr::GetFormats(
CStringArray &aryStr,	// 取得したフォーマットリスト
LPWAVEFORMATEX pwf,		// 変換元フォーマット
BOOL blSvr)				// TRUE:リストを厳密化（チャンネル・レートが一致）
{
	int i;

	// フォーマットリストを空にする
	ResetFormats();

	// driver handleのオープン
	HACMDRIVER had;
	if (acmDriverOpen(&had, m_idDriver, 0) != 0) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Audio codec open error:%d"),  m_idDriver);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
		return 0;
	}

	// フォーマットリスト取得
	// 一時フォーマット領域が規定より大きくなる場合がある
	MMRESULT res;
	ACMFORMATDETAILS formatDetails;
	memset(&formatDetails, 0, sizeof(ACMFORMATDETAILS));
	LPWAVEFORMATEX ptmp;
	TRY {
		res = acmMetrics(NULL, ACM_METRIC_MAX_SIZE_FORMAT, &formatDetails.cbwfx);
		if (res) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("acmMetrics error:%d"),  res);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
			acmDriverClose(had, 0);		// driver handleのクローズ
			return 0;
		}
		ptmp = (LPWAVEFORMATEX)new BYTE[formatDetails.cbwfx];
		memset(ptmp, 0, formatDetails.cbwfx);
		if (m_CnvType == MP3_WAV) {
			formatDetails.dwFormatTag = WAVE_FORMAT_PCM;
			//ptmp = (LPWAVEFORMATEX)new char[sizeof(WAVEFORMATEX)];
			//memset(ptmp, 0, sizeof(WAVEFORMATEX));
		} else {
			formatDetails.dwFormatTag = WAVE_FORMAT_MPEGLAYER3;
			//ptmp = (LPWAVEFORMATEX)new char[sizeof(MPEGLAYER3WAVEFORMAT)];
			//memset(ptmp, 0, sizeof(MPEGLAYER3WAVEFORMAT));
		}
		memcpy(ptmp, pwf, sizeof(WAVEFORMATEX));
		ptmp->cbSize = 0;
		formatDetails.pwfx          = ptmp;		// 一時フォーマット領域（コールバック中のみ使用）
		formatDetails.cbStruct      = sizeof(ACMFORMATDETAILS);
		formatDetails.fdwSupport    = 0;
		res = acmFormatEnum(had, &formatDetails, (ACMFORMATENUMCB)acmFormatEnumCallback, (DWORD)(void*)this, ACM_FORMATENUMF_CONVERT);
		if (res) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("acmFormatEnum error:%d"),  res);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
			acmDriverClose(had, 0);		// driver handleのクローズ
			return 0;
		}
		// 待機
		for (i = 0; i <= 5; i++) {
			AfxGetThread()->PumpMessage();
			::Sleep(10);
			if (m_dicFormat->GetCount() > 0) break;
		}
	}
    AND_CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Audio codec format list error:%s"),  szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
		acmDriverClose(had, 0);		// driver handleのクローズ
		return 0;
    }
	END_CATCH_ALL	
	delete ptmp;	// 一時フォーマット領域解放

	acmDriverClose(had, 0);		// driver handleのクローズ

	// フォーマットリストを更新
_stprintf_s(te.errstr, MAX_ERRSTR, _T("Audio codec format:%d,%d"), m_idDriver, m_dicFormat->GetCount());
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
	CString key;
	POSITION pos = m_dicFormat->GetStartPosition();
	aryStr.RemoveAll();
	while (pos != NULL) {
		ptmp = NULL;
		m_dicFormat->GetNextAssoc(pos, key, ptmp);
//_stprintf_s(te.errstr, MAX_ERRSTR, _T("Audio codec format:%s,ch=%d,sps=%d"), key, ptmp->nChannels, ptmp->nSamplesPerSec);
//te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
		if (blSvr) {
			if (ptmp->nChannels == pwf->nChannels && ptmp->nSamplesPerSec == pwf->nSamplesPerSec) {
				aryStr.Add(key);
			}
		} else {
			aryStr.Add(key);
		}
	}

	return (int)aryStr.GetCount();
}

//
//  機能     : カレントドライバのフォーマットリスト取得
//  
//  機能説明 : 選択ドライバのフォーマットリストを更新する
//  
//  返り値   : フォーマットリスト件数
//  
//  備考     : 
//  
int CAcmMgr::GetFormatList(
CStringArray &aryStr,		// 取得したフォーマットリスト
LPWAVEFORMATEX pFromWf,		// 変換元フォーマット
BOOL blSvr)					// TRUE:リストを厳密化（チャンネル・レートが一致）
{
	int i;

	// 現在の変換元フォーマットをクリア
	memset(&m_FromWf, 0, sizeof(WAVEFORMATEX));
	memset(&m_FromMf, 0, sizeof(MPEGLAYER3WAVEFORMAT));

	// フォーマットリスト更新
	// 失敗する場合があるので、何度かリトライ
	int intCnt;
	for (i = 0; i < 10; i++) {
		intCnt =  GetFormats(aryStr, pFromWf, blSvr);
		if (intCnt > 0) break;
	}
	if (intCnt <= 0) return 0;

	// 現在の変換元フォーマットを設定
	if (m_CnvType == MP3_WAV) {
		m_FromMf.wfx = *pFromWf;
		m_FromMf.wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
		m_FromMf.wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;
		m_FromMf.wID = MPEGLAYER3_ID_MPEG;
		m_FromMf.fdwFlags = 0;
		m_FromMf.nBlockSize = 1;
		m_FromMf.nCodecDelay = 0;
		m_FromMf.nFramesPerBlock = 1;
	} else {
		m_FromWf = *pFromWf;
	    m_FromWf.wFormatTag = WAVE_FORMAT_PCM;
	}

	return intCnt;
}

//
//  機能     : フォーマット名から変換先フォーマットを取得
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CAcmMgr::FindFormat(
CString strFormat) 		// フォーマット名
{
	// 現在の変換先フォーマットをクリア
	memset(&m_ToWf, 0, sizeof(WAVEFORMATEX));
	memset(&m_ToMf, 0, sizeof(MPEGLAYER3WAVEFORMAT));

	// フォーマット名から変換先フォーマットを取得
	LPWAVEFORMATEX pwf;
	if (!m_dicFormat->Lookup(strFormat, pwf)) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Audio codec FindFormat error:%s"), strFormat);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
		return FALSE;
	}

	// 現在の変換先フォーマットを設定
	if (m_CnvType == WAV_MP3 || m_CnvType == WAV_MP3_LAME) {
		m_ToMf.wfx = *pwf;
		m_ToMf.wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
		m_ToMf.wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;
		m_ToMf.wID = MPEGLAYER3_ID_MPEG;
		m_ToMf.fdwFlags = MPEGLAYER3_FLAG_PADDING_OFF;
		//m_ToMf.nBlockSize = 417;	//MP3_BLOCK_SIZE; 
		m_ToMf.nBlockSize = (1152/8) * (WORD)(m_ToMf.wfx.nAvgBytesPerSec * 8 / m_ToMf.wfx.nSamplesPerSec);
		m_ToMf.nFramesPerBlock = 1;
		if (m_CnvType == WAV_MP3_LAME) {
			m_ToMf.nCodecDelay = 0;
		} else {
			m_ToMf.nCodecDelay = 1393;	// fraunh
		}
	} else {
		m_ToWf = *pwf;
	    m_ToWf.wFormatTag = WAVE_FORMAT_PCM;
	}

	return TRUE;
}
// 最適なフォーマットの検索（MP3->WAV only）
BOOL CAcmMgr::FindFormat(
LPMPEGLAYER3WAVEFORMAT pFromMf, 		// 変換元MP3フォーマット
LPWAVEFORMATEX pToMf)					// 変換先WAVフォーマット
{
	memset(&m_FromMf, 0, sizeof(MPEGLAYER3WAVEFORMAT));
	memset(&m_ToWf, 0, sizeof(WAVEFORMATEX));

	// Format指定
	memcpy(&m_FromMf, pFromMf, sizeof(MPEGLAYER3WAVEFORMAT));

	// 最適なフォーマットの検索
	m_ToWf.wFormatTag = WAVE_FORMAT_PCM;
	if(acmFormatSuggest(
                NULL, 
				(LPWAVEFORMATEX)&m_FromMf, 
				&m_ToWf, 
				sizeof(WAVEFORMATEX),
				(DWORD) (
				ACM_FORMATSUGGESTF_WFORMATTAG)) != S_OK) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Audio codec FindFormat error"));
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
		return FALSE;
	}

	if (pToMf != NULL) {
		memcpy(pToMf, &m_ToWf, sizeof(WAVEFORMATEX));
	}

	return TRUE;
}

//
//  機能     : フォーマットリストの解放
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
void CAcmMgr::ResetFormats()		
{
	CString key;
	LPWAVEFORMATEX pwf;
	POSITION pos = m_dicFormat->GetStartPosition();
	while (pos != NULL) {
		m_dicFormat->GetNextAssoc(pos, key, pwf);
		delete pwf;
	}
	m_dicFormat->RemoveAll();
}

//
//  機能     : ドライバ（Codec）選択
//  
//  機能説明 : 
//  
//  返り値   : 選択ドライバ名称、NULL:該当ドライバなし
//  
//  備考     : 
//  
CString CAcmMgr::FindDriver(
CnvType type,		// 変換タイプ
CString strCodec)	// ドライバ（Codec）名称（部分一致可）
{
	CString strDriver;

	// 変換タイプ
	m_CnvType = type;

	// ドライバリストがない場合は作成する
	int intCnt = (int)m_dicDriver->GetCount();
	if (intCnt == 0) {
		if (GetDrivers() <= 0) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("FindDriver error"));
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
			return _T("");
		}
	}

	// ドライバ（Codec）選択
	HACMDRIVERID rId;
	ACMDRIVERDETAILS rDriver;
	POSITION pos = m_dicDriver->GetStartPosition();
	strCodec.MakeUpper();
	while (pos != NULL) {
		m_dicDriver->GetNextAssoc(pos, rId, rDriver);
		strDriver = rDriver.szShortName;
		strDriver.MakeUpper();
		if (strDriver.Find(strCodec) >= 0) {
			m_idDriver = rId;
			break;
		}
	}
	if (m_idDriver <= 0) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("FindDriver error"));
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
		return _T("");
	}

	return strDriver;
}

// ファイルヘッダからWAVフォーマット取得
BOOL CAcmMgr::GetFileFormat(
CString strFile, 
LPWAVEFORMATEX pFromWf)
{
	HMMIO hmmio = NULL;
	MMCKINFO ckRIFF,ckfmt,ckdata,ckfact;
	WAVEFORMATEX wfex;
	_TCHAR pFileName[MAX_PATH];

	ckRIFF.ckid=FOURCC_RIFF;
	ckRIFF.fccType=mmioFOURCC('W','A','V','E');
	ckfmt.ckid=mmioFOURCC('f','m','t',' ');
	ckfact.ckid=mmioFOURCC('f','a','c','t');
	ckdata.ckid=mmioFOURCC('d','a','t','a');

	_tcscpy_s(pFileName, MAX_PATH, strFile);
	if((hmmio=mmioOpen((LPWSTR)pFileName, NULL,MMIO_READ | MMIO_ALLOCBUF))==NULL)
		return FALSE;

	if(mmioDescend(hmmio, &ckRIFF, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR)
		goto closeANDreturn;
	if(mmioDescend(hmmio, &ckfmt, &ckRIFF, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
		goto closeANDreturn;
	if(ckfmt.cksize < sizeof(PCMWAVEFORMAT))
		goto closeANDreturn;
	if(mmioRead(hmmio, (HPSTR)&wfex, sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT))
		goto closeANDreturn;
	wfex.wFormatTag=WAVE_FORMAT_PCM;
	wfex.cbSize=0;

	*pFromWf = wfex;
	mmioClose(hmmio,MMIO_FHOPEN);
	return TRUE;
closeANDreturn:
	mmioClose(hmmio,MMIO_FHOPEN);
	return FALSE;
}

// ファイルヘッダからMP3フォーマット取得
BOOL CAcmMgr::GetFileFormat(
CString strFile, 
LPMPEGLAYER3WAVEFORMAT pFromMf)
{
	CMP3Info mp3;
	if (!mp3.Open(strFile)) return FALSE;

	if (!(mp3.GetValidity() & MP3VLD_DATA_VALID)) return FALSE;
	//if (!(mp3.GetValidity() & MP3VLD_ID3V1_VALID)) return FALSE;

	memset(pFromMf, 0, sizeof(MPEGLAYER3WAVEFORMAT));
	pFromMf->wID = MPEGLAYER3_ID_MPEG;
	pFromMf->fdwFlags = 0;
	pFromMf->nBlockSize = 1;
	pFromMf->nCodecDelay = 0;
	pFromMf->nFramesPerBlock = 1;
	pFromMf->wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
	ENCHANNELMODE ch = mp3.GetChannelMode();
	if (ch == MP3CM_SINGLE_CHANNEL) {
		pFromMf->wfx.nChannels = 1;
	} else {
		pFromMf->wfx.nChannels = 2;
	}
	pFromMf->wfx.nSamplesPerSec = mp3.GetSampleRate();
	pFromMf->wfx.wBitsPerSample = 0;
    pFromMf->wfx.nBlockAlign = 0;
    pFromMf->wfx.nAvgBytesPerSec = mp3.GetBitRate() * 1000 / 8;
    pFromMf->wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;

	return TRUE;
}	

//////////////////////////////////////////////////////////////////////
// for 変換処理
//////////////////////////////////////////////////////////////////////

// バッファからファイル保存
BOOL CAcmMgr::Record(
CString strSaveFile)		// 保存ファイル名
{
	if ( !RecordStop() ) {
		return FALSE;
	}

	m_bSaveReset = FALSE;

	m_strSaveFile = strSaveFile;
	if (m_strSaveFile.GetLength() <= 0) return FALSE;

	// メモリからファイルへの書き込みをスレッド化
	Sleep(500);		// 前回のスレッドが残っている場合があるので
	CWinThread* pThread = NULL;
	if (m_CnvType == MP3_WAV) {
		pThread = AfxBeginThread(SaveWavWorkerThread, this);
	} else {
		pThread = AfxBeginThread(SaveMp3WorkerThread, this);
	}
	m_bSaveThread = (pThread == NULL) ? FALSE : TRUE;

	return m_bSaveThread;
}

// ファイル保存中止
BOOL CAcmMgr::RecordStop()
{
	m_bSaveReset = TRUE;

/* スレッドが終了するまで有効にする必要があるため、ここでは解放しない（次回Record時やクラスの消滅時に行われる）
*/

	return TRUE;
}

// ファイル保存中か判別
BOOL CAcmMgr::IsRecording()
{
	BOOL bResult = FALSE;

	// 録音時のスレッドが動作している
	if (m_bSaveThread) {
		bResult = TRUE;
	} else {
		bResult = FALSE;
	}

	return bResult;
}

// バッファへファイル読み込み
BOOL CAcmMgr::Read(
CString strReadFile)		// 読み込みファイル名
{
	if ( !ReadStop() ) {
		return FALSE;
	}

	m_bReadReset = FALSE;
	FreeListOfBuffer();		// バッファクリア

	m_strReadFile = strReadFile;
	if (m_strReadFile.GetLength() <= 0) return FALSE;

	// ファイルからバッファへの書き込みをスレッド化
	CWinThread* pThread = NULL;
	if (m_CnvType == MP3_WAV) {
		pThread = AfxBeginThread(ReadMp3WorkerThread, this);
	} else {
		pThread = AfxBeginThread(ReadWavWorkerThread, this);
	}
	m_bReadThread = (pThread == NULL) ? FALSE : TRUE;

	return m_bReadThread;
}

// ファイル読み込み中止
BOOL CAcmMgr::ReadStop()
{
	m_bReadReset = TRUE;

	return TRUE;
}

// ファイル読み込み中か判別
BOOL CAcmMgr::IsReading()
{
	BOOL bResult = FALSE;

	if (m_bReadThread) {
		// 読み込み時のスレッドが未だ動作している
		bResult = TRUE;
	} else {
		bResult = FALSE;
	}

	return bResult;
}

// 変換元データをバッファ化
BOOL CAcmMgr::AddNewBuffer(void *pData, long lngSize)
{
	CWaveBuffer *pBuf = new CWaveBuffer;

	pBuf->m_blActive = TRUE;	// スレッドによるアクセスを制限するフラグ

	m_listOfBuffer.AddTail(pBuf);
	( (CWaveBuffer*)m_listOfBuffer.GetTail() )->CopyBuffer(pData, lngSize / 4, 4);	// 4:16bit stereo
	if (m_nBufferSize == 0) {
		m_nBufferSize = (lngSize / 4) * 4;		// バッファサイズ
	}

	pBuf->m_blActive = FALSE;

	return TRUE;
}

// データバッファを解放
void CAcmMgr::FreeListOfBuffer()
{
	CWaveBuffer* pBuf = NULL;
	POSITION pos = NULL;

	pos = m_listOfBuffer.GetHeadPosition();
	while (pos) {
		pBuf = (CWaveBuffer*)m_listOfBuffer.GetNext(pos);
		if (pBuf) {
			delete pBuf;
			pBuf = NULL;
		}
	}
	m_listOfBuffer.RemoveAll();
	
	m_nBufferSize = 0;		// バッファサイズ
}

// 先頭データバッファの参照
CWaveBuffer* CAcmMgr::GetBuffer() 
{
	TRY {
		if (m_listOfBuffer.IsEmpty()) return NULL;

		POSITION pos = m_listOfBuffer.GetHeadPosition();
		if (pos == NULL)  return NULL;
		CWaveBuffer* pBuf = (CWaveBuffer*)m_listOfBuffer.GetAt(pos);
		return pBuf;
	}
    CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("GetBuffer error:%s"),  szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
		return NULL;
    }
	END_CATCH_ALL
}

// 先頭データバッファの削除
bool CAcmMgr::DelBuffer()
{
	TRY {
		if (m_listOfBuffer.IsEmpty()) return true;

		POSITION pos = m_listOfBuffer.GetHeadPosition();
		if (pos == NULL)  return true;
		CWaveBuffer* pBuf = (CWaveBuffer*)m_listOfBuffer.GetAt(pos);
		if (pBuf) {
			if (pBuf->m_blActive == FALSE) {
				delete pBuf;
			}
			m_listOfBuffer.RemoveHead();
		}
	}
    CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("DelBuffer error:%s"),  szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
		return false;
    }
	END_CATCH_ALL

	return true;
}


//////////////////////////////////////////////////////////////////////
// for 上位ツール
//////////////////////////////////////////////////////////////////////

//
//  機能     : WAV->MP3変換
//  
//  機能説明 : 指定した変換タイプで変換
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
BOOL CAcmMgr::ConvertWavToMp3(
CWnd *hWin,				// メッセージ表示用ハンドル
CnvType type,			// 変換タイプ
CString strReadFile,	// 入力ファイル
CString strSaveFile,	// 出力ファイル
CString strCodec,		// CODEC名称
CString strFormat,		// 出力先Format名称（GetFormatListの結果から選択）
BOOL blWait)			// 変換が終了するまで待つ
{
	BOOL blRet = TRUE;
	CProgressWnd wndProgress;

	// プログレスメッセージ用
	CFileSpec fs(strReadFile);
	long lngSz = (long)fs.FileSize();	
	int intPercent = 0;
	int intPercentBak = 0;

	/// CODEC/Format指定
	CString strDriver = FindDriver(type, strCodec);
	if (strDriver.GetLength() <= 0) return FALSE;
	if (!FindFormat(strFormat)) return FALSE;

	/// ファイル読込みスレッド開始
	if (blWait) {
		// プログレスメッセージ作成
		wndProgress.Create(hWin);
		//wndProgress.GoModal();
		CString strMsg;
		strMsg.LoadString(IDS_MSGBOX91);
		strMsg = _T("MP3") + strMsg;
		wndProgress.SetText(strMsg);
	}
	if (!Read(strReadFile)) {
		blRet = FALSE;
		goto PROC_END;
	}

	Sleep(100);  //バッファの生成待ち

	/// 録音スレッド開始
	if (!Record(strSaveFile)) {
		ReadStop();
		blRet = FALSE;
		goto PROC_END;
	}

	/// 処理待ち
	if (blWait) {
		while(IsReading()) {
			if (!IsRecording()) {
				// 読み込み中なのに書き込みスレッドが終了している場合はエラー
				ReadStop();
				blRet = FALSE;
				break;
			}

			// Report Progress
			intPercentBak = intPercent;
			intPercent = (int)((double)m_lngRdSz/(double)lngSz * 100.0);
			if (intPercentBak != intPercent) {
				wndProgress.SetPos(intPercent);
				wndProgress.PeekAndPump();
			}
			if (wndProgress.Cancelled()) {
				ReadStop();
				blRet = FALSE;
				break;
			}
		}
		RecordStop();
	}

PROC_END:
	if (!wndProgress.Cancelled()) {
		wndProgress.Hide();
	}
	return blRet;
}
// Codecの選択をOSに任せる場合
//
//  機能     : MP3->WAV変換
//  
//  機能説明 : Codec/Formatの選択をOSに任せる
//  
//  返り値   : BOOL
//  
//  備考     : 特定のWAVフォーマットを指定することはできない
//  
BOOL CAcmMgr::ConvertMp3ToWav(
CWnd *hWin,				// メッセージ表示用ハンドル
CString strReadFile,	// 入力ファイル
CString strSaveFile,	// 出力ファイル
BOOL blWait)			// 変換が終了するまで待つ
{
	BOOL blRet = TRUE;
	CProgressWnd wndProgress;

	// プログレスメッセージ用
	CFileSpec fs(strReadFile);
	long lngSz = (long)fs.FileSize();	
	int intPercent = 0;
	int intPercentBak = 0;

	// 変換タイプ
	m_CnvType = MP3_WAV;

	// 現在のCodec/Formatをクリア
	m_idDriver = NULL;
	memset(&m_FromWf, 0, sizeof(WAVEFORMATEX));
	memset(&m_FromMf, 0, sizeof(MPEGLAYER3WAVEFORMAT));
	memset(&m_ToWf, 0, sizeof(WAVEFORMATEX));
	memset(&m_ToMf, 0, sizeof(MPEGLAYER3WAVEFORMAT));

	// CODEC/Format指定
	MPEGLAYER3WAVEFORMAT FromMf;
	GetFileFormat(strReadFile, &FromMf);
	if (!FindFormat(&FromMf)) return FALSE;

	// ファイル読込みスレッド開始
	if (blWait) {
		// プログレスメッセージ作成
		wndProgress.Create(hWin);
		//wndProgress.GoModal();
		CString strMsg;
		strMsg.LoadString(IDS_MSGBOX91);
		strMsg = _T("WAVE") + strMsg;
		wndProgress.SetText(strMsg);
	}
	if (!Read(strReadFile)) {
		blRet = FALSE;
		goto PROC_END;
	}

	Sleep(100);  //バッファの生成待ち

	// 録音スレッド開始
	if (!Record(strSaveFile)) {
		ReadStop();
		blRet = FALSE;
		goto PROC_END;
	}

	// 処理待ち
	if (blWait) {
		while(IsReading()) {
			if (!IsRecording()) {
				// 読み込み中なのに書き込みスレッドが終了している場合はエラー
				ReadStop();
				blRet = FALSE;
				break;
			}

			// Report Progress
			intPercentBak = intPercent;
			intPercent = (int)((double)m_lngRdSz/(double)lngSz * 100.0);
			if (intPercentBak != intPercent) {
				wndProgress.SetPos(intPercent);
				wndProgress.PeekAndPump();
			}
			if (wndProgress.Cancelled()) {
				ReadStop();
				blRet = FALSE;
				break;
			}
		}
		RecordStop();
		Sleep(100);		// スレッド完了待ち
	}

PROC_END:
	if (!wndProgress.Cancelled()) {
		wndProgress.Hide();
	}
	wndProgress.DestroyWindow();

	return blRet;
}
