// SocketIn.cpp: implementation of the CSocketIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SocketIn.h"
#include "FileSpec.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


//////////////////////////////////////////////////////////////////////
// �t�@�C���������݃X���b�h
UINT SocketInWorkerThread( LPVOID pParam )
{
	CSocketIn* pSocketIn = (CSocketIn*)pParam;

	CFileException e;
	DWORD dwFileSize;
	DWORD dwNum;
	CFile f;
	CString strErr;
	SHORT tmpDat[SOCKET_IN_BUFFER_SIZE * 4];	// �X�e���I�^�����̈ꎞ�o�b�t�@
	WAVEFORMATEX Fmt = pSocketIn->m_wave.GetFormat();		// �ۑ��pWAVE�t�H�[�}�b�g

	CString strFile = pSocketIn->m_strSaveFile;
	if (strFile.GetLength() == 0) {
		goto THREAD_END;
	}

	TRY {
		/// �g�`�f�[�^�̏�������
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
			if (pSocketIn->m_bResetForce) break;	// �����I��

			// ���񂾂����s
			if (dwFmtSize == 0L) {
				/// 6�b�ȓ��Ƀ\�P�b�g�Ƀf�[�^���Ȃ����Skype�̃X�g���[���G���[�iBUG�j�ƍl������B
				// (�R�Ғʘb�ōŏ���Caller���������ꍇ)
				if (!pSocketIn->m_sockServerIn.IsWorking() && 
						pSocketIn->m_listOfBufferIn.GetCount() == 0 && pSocketIn->m_listOfBufferMic.GetCount() == 0) {
					Sleep(10);  //CPU���׌���
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
					/// �t�@�C���I�[�v��
					if( !f.Open(strFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e ) ) {
						_stprintf_s(pSocketIn->te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), strFile, e.m_cause);
						pSocketIn->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, pSocketIn->te.errstr);
						goto THREAD_END;
					}
					/// �ŏ��̏������݁i�w�b�_���������݁j
					f.Write("RIFF", 4) ;
					f.Write(&dwFileSize, sizeof(dwFileSize)) ;
					f.Write("WAVEfmt ", 8) ;	// �啶���������͎��ʂ����̂Œ��ӁI
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
			//// �c��̃o�b�t�@���������Ę^���I��
				cnt1 = pSocketIn->m_listOfBufferIn.GetCount();
				cnt2 = pSocketIn->m_listOfBufferMic.GetCount();
				if (cnt1 > 0 && cnt2 > 0) {
					// �c��̃o�b�t�@������
				} else {
					// �c��̃o�b�t�@��j��
					if (cnt1 == 0) {
						for(i = 0; i < cnt2; i++) {
							pos = pSocketIn->m_listOfBufferMic.GetHeadPosition();
							if (pos == NULL)  break;
							pSyncBuf = (CWaveBuffer *)pSocketIn->m_listOfBufferMic.GetAt(pos);
							if (!pSyncBuf) break;
							if (pSyncBuf->m_blActive) break;		// �o�b�t�@�������ݒ�
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
							if (pBuf->m_blActive) break;		// �o�b�t�@�������ݒ�
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
				Sleep(10);  //CPU���׌���
			//// ������h�����߁A�o�b�t�@���P�c���ď���(�o�b�t�@�̐����҂�)
				if (pSocketIn->m_listOfBufferIn.GetCount() <= 1 || pSocketIn->m_listOfBufferIn.IsEmpty()) continue;
				if (pSocketIn->m_listOfBufferMic.GetCount() <= 1 || pSocketIn->m_listOfBufferMic.IsEmpty()) continue;
			}
			if (pSocketIn->m_listOfBufferIn.IsEmpty()) continue;
			pos = pSocketIn->m_listOfBufferIn.GetHeadPosition();
			if (pos == NULL)  continue;
			pBuf = (CWaveBuffer *)pSocketIn->m_listOfBufferIn.GetAt(pos);
			if (!pBuf) continue;
			if (pBuf->m_blActive) continue;		// �o�b�t�@�������ݒ�

			/// �g�`�f�[�^�̏�������
			//// �������ĕۑ�
			pSyncBuf = (CWaveBuffer *)pSocketIn->m_listOfBufferMic.GetHead();
			if (!pSyncBuf) continue;
			if (pSyncBuf->m_blActive) continue;		// �o�b�t�@�������ݒ�
			///// �����o�b�t�@�̃T���v�����O�������߂���ꍇ�����遨�����o�b�t�@���ꕔ�폜���Ē�������
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
				///// �������ĕۑ�
				// �����̃o�b�t�@�������̏ꍇ�͕ۑ����Ȃ�
				// ����̃o�b�t�@�������ȏꍇ�́A�o�b�t�@���̂������Ȃ̂Ŗ��͂Ȃ�
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
					////// �X�e���I�^��(���S�p��)
						// WAVE����
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
					////// �X�e���I�^��(�����p��)
						tmp1 = *(pSyncDat+i) + (SHORT)((double)*(pSyncDat+i) * pSocketIn->m_dblVolSnd);
						tmp2 = *(pDat+i) + (SHORT)((double)*(pDat+i) * pSocketIn->m_dblVolRcv);
						// WAVE����
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
						// WAVE����
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

				///// �t�@�C���Ƀf�[�^��������
				void *pData = NULL;
				long lngSize = 0;
				if (pSocketIn->m_SyncStereo > 0) {
					// stereo�̂��߁A�o�b�t�@�T�C�Y�͔{�ɂȂ�
					pData = tmpDat;
					lngSize = pBuf->GetSampleSize() * pBuf->GetNumSamples() * 2;
				} else {
					pData = pBuf->GetBuffer();
					lngSize = pBuf->GetSampleSize() * pBuf->GetNumSamples();
				}
				pSocketIn->AddOutBuffer(pData, lngSize);	// �^��p�ɓ]��
				if (pSocketIn->m_intFormatType == 0) {		// WAV
					f.Write(pData, lngSize);
				} else {	// MP3
					// ACM�}�l�[�W���Ƀf�[�^�𑗂�
					pSocketIn->m_oAcmMgr.AddNewBuffer(pData, lngSize);
				}
			}
						
			//// �o�b�t�@�̉��
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

	//// �w�b�_�̏C��
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
	m_bResetForce = TRUE;	// WorkerThread���܂������Ă���ꍇ�͋����I��
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
		// �\�P�b�g��M�p�o�b�t�@�̐ݒ�
		m_sockServerIn.SetListBuffer(&m_listOfBufferIn);
		m_sockServerMic.SetListBuffer(&m_listOfBufferMic);

		/// ���͉����p�\�P�b�g���쐬����
		if(m_sockServerIn.Create(uiPortIn) == FALSE)
		{
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket Create error In:%d:%d"), m_sockServerIn.GetLastError(), uiPortIn);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
			return FALSE;
		}

		/// �}�C�N�����p�\�P�b�g���쐬����
		if(m_sockServerMic.Create(uiPortMic) == FALSE)
		{
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket Create error Mic:%d:%d"), m_sockServerMic.GetLastError(), uiPortMic);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
			return FALSE;
		}

		/// ���͉����p�\�P�b�g����M�҂��ɂ���
		if( m_sockServerIn.Listen() == FALSE)
		{
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket Listen error In:%d"), m_sockServerIn.GetLastError());
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
			return FALSE;
		}

		/// �}�C�N�����p�\�P�b�g����M�҂��ɂ���
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
// �^���Ώۂ̐���
// 0:�����A1:Skype�����̂݁A2:MIC�����̂�
BOOL CSocketIn::SetSyncSw(int sw)
{
	m_SyncSw = sw;

	// PAUSE���̏ꍇ�A�\�P�b�g�N���X�̕ύX�͂Ȃ�
	if (m_sockServerIn.IsPause() && m_sockServerMic.IsPause()) return TRUE;

	// �\�P�b�g�N���X�Ɏ�M��~�w��
	return Continue();
}

// �^�������̐���
// 0:mono�A1:stereo�A2:natural stereo
BOOL CSocketIn::SetSyncStereo(int val)
{
	m_SyncStereo = val;

	// �ۑ��pWAVE�t�H�[�}�b�g
	if (m_SyncStereo > 0) {
		m_wave.BuildFormat(2, 16000, 16, FALSE);
	} else {
		m_wave.BuildFormat(1, 16000, 16, FALSE);
	}

	return TRUE;
}

// ���ʒ���
// % -> ����
BOOL CSocketIn::SetVolAdjust(int volSnd, int volRcv)
{
	m_dblVolSnd = (double)volSnd / 100.0;				// ���ʒ����i���M�j
	m_dblVolRcv = (double)volRcv / 100.0;				// ���ʒ����i��M�j

	return TRUE;
}

//
//  �@�\     : �^�����̃t�H�[�}�b�g
//  
//  �@�\���� : ���݂�MP3�^�����̂ݗL���AWAV�^�����̓t�H�[�}�b�g�Œ�
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CSocketIn::SetFormat(
int intFormatType,		// 0:WAV, 1:MP3
CString strCodec,		// Codec����
CString strFormat)		// Format����
{
	m_intFormatType = intFormatType;	// �^���t�H�[�}�b�g�A0:WAV, 1:MP3
	m_strCodec = strCodec;				// �^���t�H�[�}�b�g�ACodec����
	m_strFormat = strFormat;			// �^���t�H�[�}�b�g�AFormat����

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
		/// CODEC/Format�w��
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
		//// �t�H�[�}�b�g���X�g���X�V
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

	Sleep(500);		// �O��̃X���b�h���c���Ă���ꍇ������̂�

	// ����������t�@�C���ւ̏������݂��X���b�h���i���僁����������邽�߁j
	if (m_intFormatType > 0) m_oAcmMgr.FreeListOfBuffer();	//MP3�̎���ACM�o�b�t�@���N���A
	CWinThread* pThread = AfxBeginThread(SocketInWorkerThread, this);
	m_blWorkThread = (pThread == NULL) ? FALSE : TRUE;

	if (m_intFormatType > 0) {		// MP3
		// SocketInWorkerThread����]�����ꂽ�f�[�^��MP3��������
		Sleep(100);  //�o�b�t�@�̐����҂�

		// �^���X���b�h�J�n
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
		// MP3�^���̒��~
		m_oAcmMgr.RecordStop();
	}

/* �X���b�h���I������܂ŗL���ɂ���K�v�����邽�߁A�����ł͉�����Ȃ��i����Record����N���X�̏��Ŏ��ɍs����j
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
		// MP3�^����
		if (m_oAcmMgr.IsRecording()) return TRUE;
	}

	if (m_bResetRequired) {
		// �^�����łȂ�
		if (m_blWorkThread) {
			// �^�����łȂ����A�^�����̃X���b�h���������삵�Ă���
			bResult = TRUE;
		} else {
			bResult = FALSE;
		}
	} else {
		// �^����
		bResult = TRUE;
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////
BOOL CSocketIn::ResetRequired()
{
	return m_bResetRequired;
}

// �����f�[�^���O���i�^��p�X�g���[���j�֓]��
BOOL CSocketIn::AddOutBuffer(void *pData, long lngSize)
{
	// �o�b�t�@�������Ă��Ȃ��ꍇ�A�f�[�^��ǉ����Ȃ�
	if (m_listOfBufferOut.GetCount() > 100) return FALSE;

	CWaveBuffer *pBuf = new CWaveBuffer;

	pBuf->m_blActive = TRUE;	// �X���b�h�ɂ��A�N�Z�X�𐧌�����t���O

	m_listOfBufferOut.AddTail(pBuf);
	( (CWaveBuffer*)m_listOfBufferOut.GetTail() )->CopyBuffer(pData, lngSize / 4, 4);	// 4:16bit stereo

	pBuf->m_blActive = FALSE;

	return TRUE;
}

// �O���i�^��p�X�g���[���j�]���p�o�b�t�@�Q��
CPtrList * CSocketIn::GetOutListBuffer()
{
	return &m_listOfBufferOut;
}
