/*
ACM�ϊ��N���X
���݂�WAV<->MP3�̂ݑΉ�
*/

#include "stdafx.h"
#include "AcmMgr.h"
#include "FileSpec.h"
#include "ProgressWnd.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

// �h���C�o�iCodec�j���X�g�擾�p�R�[���o�b�N
// dwInstance:CAcmMgr�̃I�u�W�F�N�g�A�h���X�Q��
BOOL CALLBACK acmDriverEnumCallback(HACMDRIVERID hdriverId, DWORD dwInstance, DWORD dwSupport)
{
	CAcmMgr* pObj = (CAcmMgr*)dwInstance;

    if(dwSupport & ACMDRIVERDETAILS_SUPPORTF_CODEC) {
        MMRESULT mmr;
        ACMDRIVERDETAILS details;
        HACMDRIVER driver;
        UINT i;
        details.cbStruct = sizeof(ACMDRIVERDETAILS);
		// Driver�ڍ׏��̎擾
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
				// Driver ID��DRIVERDETAILS�̃R���N�V����
				pObj->m_dicDriver->SetAt(hdriverId, details);
            }
        }
        mmr = acmDriverClose(driver,0);
    }

	return TRUE;
}

// �t�H�[�}�b�g���X�g�擾�p�R�[���o�b�N
// dwInstance:CAcmMgr�̃I�u�W�F�N�g�A�h���X�Q��
BOOL CALLBACK acmFormatEnumCallback(HACMDRIVERID hdriverId, LPACMFORMATDETAILS lpFormatDetails, DWORD dwInstance, DWORD dwSupport)
{
	CAcmMgr* pObj = (CAcmMgr*)dwInstance;

	if (pObj->m_dicFormat->GetCount() > 0) {
		LPWAVEFORMATEX pwf;
		if (pObj->m_dicFormat->Lookup(CString(lpFormatDetails->szFormat), pwf)) return TRUE;	// �t�H�[�}�b�g���̏d��������
	}

	// �t�H�[�}�b�g�������WAVEFORMATEX�̃R���N�V����
	LPWAVEFORMATEX pwf = new WAVEFORMATEX;
	memcpy(pwf, lpFormatDetails->pwfx, sizeof(WAVEFORMATEX));
	pObj->m_dicFormat->SetAt(CString(lpFormatDetails->szFormat), pwf);

	return TRUE;
}


// MP3�t�@�C���������݃X���b�h
UINT SaveMp3WorkerThread( LPVOID pParam )
{
	CAcmMgr* pAcmMgr = (CAcmMgr*)pParam;

	CFile *pfout = NULL;
	CFileException e;
	HACMDRIVER had = NULL;
	HACMSTREAM has = NULL;
	ACMSTREAMHEADER ash;
	MMRESULT res;
	LPBYTE pSrcBuf = NULL;	// �ϊ����o�b�t�@
	LPBYTE pDstBuf = NULL;	// �ϊ���o�b�t�@
	DWORD cbSrcBuf = 0; 
	DWORD cbDstBuf = 0;
	long lngCnt = 0;
	int leftover = 0;
	POSITION pos = NULL;
	CWaveBuffer *pBuf = NULL;

	/// Validation
	if (pAcmMgr->m_strSaveFile.GetLength() == 0) goto THREAD_END;
	
	/// ACM driver handle�̃I�[�v��
	if (acmDriverOpen(&had, pAcmMgr->m_idDriver, 0) != 0) goto THREAD_END;

	/// �X�g���[���I�[�v��
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
		/// �f�[�^�̏�������
		while(1) {
			if (pAcmMgr->m_nBufferSize == 0) {
			//// ���̓o�b�t�@��������
				if (pAcmMgr->m_bSaveReset) break;
				Sleep(10);  //CPU���׌���
				continue;
			}

			//// �o�b�t�@�̊m��(���̓o�b�t�@�̐�����A�P�񂾂����s�����)
			if (pSrcBuf == NULL) {
				cbSrcBuf = pAcmMgr->m_nBufferSize; 
				///// �o�̓o�b�t�@�T�C�Y�̐����T�C�Y���擾�i���ۂ͕ϓ�����j
				res = acmStreamSize(has, cbSrcBuf, &cbDstBuf, ACM_STREAMSIZEF_SOURCE);
				if(res != S_OK) {
					goto THREAD_END;
				}
				pSrcBuf = new BYTE[cbSrcBuf * 2];
				pDstBuf = new BYTE[cbDstBuf * 2];

				///// Stream�w�b�_�̃Z�b�g
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

			//// ���̓o�b�t�@��ԃ`�F�b�N
			lngCnt = (long)pAcmMgr->m_listOfBuffer.GetCount();
			if (pAcmMgr->m_bSaveReset) {
			//// �^���I��
				if (lngCnt == 0) break;	// �I��
				// lngCnt > 0�F�c��̃o�b�t�@������
			} else {
			//// ������h�����߁A�o�b�t�@���P�c���ď���(�o�b�t�@�̐����҂�)
				if (lngCnt <= 1) {
					Sleep(10);  //CPU���׌���
					continue;
				}
			}
			if (pAcmMgr->m_listOfBuffer.IsEmpty()) continue;
			pos = pAcmMgr->m_listOfBuffer.GetHeadPosition();
			if (pos == NULL)  continue;
			pBuf = (CWaveBuffer *)pAcmMgr->m_listOfBuffer.GetAt(pos);
			if (!pBuf) continue;
			if (pBuf->m_blActive) continue;		// �o�b�t�@�������ݒ�

			/// �o�̓t�@�C���I�[�v��
			if (pfout == NULL) {
				pfout = new CFile();
				if( !pfout->Open(pAcmMgr->m_strSaveFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e ) ) {
					_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), pAcmMgr->m_strSaveFile, e.m_cause);
					pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
					goto THREAD_END;
				}
			}

			//// �ϊ�����
			if (!pBuf->m_blDead) {
				SHORT *pDat = (SHORT *)pBuf->GetBuffer();
				int sz = pBuf->GetNumSamples() * pBuf->GetSampleSize();		// �t�@�C���̖����ł͏������Ȃ�
				memcpy(pSrcBuf + leftover, (BYTE *)pBuf->GetBuffer(), sz);
				ash.cbSrcLength = sz + leftover;	// �ϊ����o�b�t�@�T�C�Y�̕ϓ�
				res = acmStreamConvert(has, &ash, ACM_STREAMCONVERTF_BLOCKALIGN);
				if(res != S_OK) {
					leftover = 0;
					break;
				}
				//// �f�[�^�̏�������
				pfout->Write(pDstBuf, ash.cbDstLengthUsed);
				//// �g���c���̃f�[�^����������ꍇ������
				leftover = ash.cbSrcLength - ash.cbSrcLengthUsed;
				memcpy(pSrcBuf, pSrcBuf+ash.cbSrcLengthUsed, leftover);
			}
						
			//// �o�b�t�@�̉��
			if (pBuf) {
				delete pBuf;
				pBuf = NULL;
			}
			pAcmMgr->m_listOfBuffer.RemoveHead();
		} //while

		/// �Ō�̎g���c���f�[�^������
		while (leftover > 0) {
			ash.cbSrcLength = leftover;	// �ϊ����o�b�t�@�T�C�Y
			res = acmStreamConvert(has, &ash, ACM_STREAMCONVERTF_BLOCKALIGN);
			if(res != S_OK) {
				break;
			}
			//// �f�[�^�̏�������
			pfout->Write(pDstBuf, ash.cbDstLengthUsed);
			//// �X�Ȃ�g���c���f�[�^
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
	// �o�̓t�@�C����Close
	if (pfout) {
		pfout->Flush();
		pfout->Close();
		delete pfout;
	}

	// �X�g���[����Close
	if (has != NULL) {
		ash.cbSrcLength = cbSrcBuf;
		ash.cbDstLength = cbDstBuf;
		acmStreamUnprepareHeader(has, &ash, 0);
		acmStreamClose(has, 0);
	}

	// ACM driver handle�̃N���[�Y
	if (had != NULL) acmDriverClose(had, 0);

	// �o�̓o�b�t�@�̉��
	if (pSrcBuf) delete [] pSrcBuf;
	if (pDstBuf) delete [] pDstBuf;		

	pAcmMgr->m_bSaveThread = FALSE;
	return 0;
};

// WAV�t�@�C���������݃X���b�h
UINT SaveWavWorkerThread( LPVOID pParam )
{
	CAcmMgr* pAcmMgr = (CAcmMgr*)pParam;

	CFile fout;
	CFileException e;

	/// Validation
	if (pAcmMgr->m_strSaveFile.GetLength() == 0) goto THREAD_END;
	
	/// �o�̓t�@�C���I�[�v��
	if( !fout.Open(pAcmMgr->m_strSaveFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e ) ) {
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), pAcmMgr->m_strSaveFile, e.m_cause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
		goto THREAD_END;
	}

	TRY {
		//// WAVE�w�b�_�̏�������
		fout.Write("RIFF", 4) ;
		DWORD dwFileSize = 0;	// dummy
		fout.Write(&dwFileSize, sizeof(dwFileSize)) ;
		fout.Write("WAVEfmt ", 8) ;	// �啶���������͎��ʂ����̂Œ��ӁI
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

		/// �f�[�^�̏�������
		long lngCnt = 0;
		POSITION pos = NULL;
		CWaveBuffer *pBuf;
		while(1) {
			if (pAcmMgr->m_nBufferSize == 0) {
			//// ���̓o�b�t�@��������
				if (pAcmMgr->m_bSaveReset) break;
				Sleep(10);  //CPU���׌���
				continue;
			}

			//// ���̓o�b�t�@��ԃ`�F�b�N
			lngCnt = (long)pAcmMgr->m_listOfBuffer.GetCount();
			if (pAcmMgr->m_bSaveReset) {
			//// �^���I��
				if (lngCnt == 0) break;	// �I��
				// lngCnt > 0�F�c��̃o�b�t�@������
			} else {
			//// ������h�����߁A�o�b�t�@���P�c���ď���(�o�b�t�@�̐����҂�)
				if (lngCnt <= 1) {
					Sleep(10);  //CPU���׌���
					continue;
				}
			}
			if (pAcmMgr->m_listOfBuffer.IsEmpty()) continue;
			pos = pAcmMgr->m_listOfBuffer.GetHeadPosition();
			if (pos == NULL)  continue;
			pBuf = (CWaveBuffer *)pAcmMgr->m_listOfBuffer.GetAt(pos);
			if (!pBuf) continue;
			if (pBuf->m_blActive) continue;		// �o�b�t�@�������ݒ�

			//// �ϊ�����
			if (!pBuf->m_blDead) {
				//// �f�[�^�̏�������
				void *pDat = pBuf->GetBuffer();
				int sz = pBuf->GetNumSamples() * pBuf->GetSampleSize();	
				fout.Write(pDat, sz);
			}
						
			//// �o�b�t�@�̉��
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
		//// WAVE�w�b�_�̏C��
		DWORD dwFileSize = (DWORD)(fout.GetLength() - 8);
		DWORD dwNum = dwFileSize - 36;
		fout.Seek(4, CFile::begin);
		fout.Write(&dwFileSize, sizeof(dwFileSize));
		fout.Seek(40, CFile::begin);
		fout.Write(&dwNum, sizeof(dwNum));

		//// �o�̓t�@�C����Close
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

// WAV�t�@�C���ǂݍ��݃X���b�h
UINT ReadWavWorkerThread( LPVOID pParam )
{
	CAcmMgr* pAcmMgr = (CAcmMgr*)pParam;

	CFile fin;
	CFileException e;
	LPBYTE pSrcBuf = NULL;	// �ϊ����o�b�t�@
	DWORD cbSrcBuf = WAVE_BUFFER_SIZE;	// �o�b�t�@�T�C�Y

	/// Validation
	if (pAcmMgr->m_strReadFile.GetLength() == 0) goto THREAD_END;
	
	/// �o�b�t�@�̊m��
	pSrcBuf = new BYTE[cbSrcBuf];

	/// ���̓t�@�C���I�[�v��
	if( !fin.Open(pAcmMgr->m_strReadFile, CFile::modeRead | CFile::shareDenyWrite, &e ) ) {
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), pAcmMgr->m_strReadFile, e.m_cause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
		goto THREAD_END;
	}

	TRY {
		/// �w�b�_�̃X�L�b�v
		if (fin.Read(pSrcBuf, 44) <= 0) {
			goto THREAD_END;
		}

		/// �f�[�^�̓ǂݍ���
		long lngCnt = 0;
		int intRead = 0;
		pAcmMgr->m_lngRdSz = 0;
		while(1) {
			if (pAcmMgr->m_bReadReset) {
				break;	// �ǂݍ��ݏI��
			}

			// �ǂݍ��݂���������ƃ��������������邽�߁A�����҂�����
			lngCnt = (long)pAcmMgr->m_listOfBuffer.GetCount();
			if (lngCnt > 10) {
				Sleep(10);  //CPU���׌���
				continue;
			}

			intRead = fin.Read(pSrcBuf, cbSrcBuf);
			if (intRead == 0) {
				break;
			} 
			pAcmMgr->m_lngRdSz += intRead;	// �v���O���X�����p

			pAcmMgr->AddNewBuffer(pSrcBuf, intRead);
			if (intRead < (int)cbSrcBuf) {
				break;
			} 
		} //while

		/// �t�@�C����Close
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
	// �o�̓o�b�t�@�̉��
	if (pSrcBuf) delete [] pSrcBuf;

	pAcmMgr->m_bReadThread = FALSE;
	return 0;
};

// MP3�t�@�C���ǂݍ��݃X���b�h
UINT ReadMp3WorkerThread( LPVOID pParam )
{
	CAcmMgr* pAcmMgr = (CAcmMgr*)pParam;

	CFile fin;
	CFileException e;
	CFileSpec fs;
	HACMSTREAM has = NULL;
	ACMSTREAMHEADER ash;
	MMRESULT res;
	LPBYTE pSrcBuf = NULL;	// �ϊ����o�b�t�@
	LPBYTE pDstBuf = NULL;	// �ϊ���o�b�t�@
	DWORD cbSrcBuf = WAVE_BUFFER_SIZE / 8;	// �o�́i���k�W�J�j�o�b�t�@�̃T�C�Y��}���邽�߁A�����߂� 
	DWORD cbDstBuf = 0;

	/// Validation
	if (pAcmMgr->m_strReadFile.GetLength() == 0) goto THREAD_END;
	
	/// �X�g���[���I�[�v��
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

	/// �o�̓o�b�t�@�T�C�Y�̐����T�C�Y���擾�i���ۂ͕ϓ�����j
	res = acmStreamSize(has, cbSrcBuf, &cbDstBuf, ACM_STREAMSIZEF_SOURCE);
	if(res != S_OK) {
		goto THREAD_END;
	}
	/// �o�b�t�@�̊m�ہi�ϓ�����\�������邽�߂Q�{�j
	pSrcBuf = new BYTE[cbSrcBuf * 2];
	pDstBuf = new BYTE[cbDstBuf * 2];

	/// Stream�w�b�_�̃Z�b�g
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

	/// �t�@�C���T�C�Y(ProgressBar�p)
	fs.SetFullSpec(pAcmMgr->m_strReadFile);
	long lngSz = (long)fs.FileSize();
	long lngRdSz = 0;
	int intPercent = 0;
	int intPercentBak = 0;

	/// ���̓t�@�C���I�[�v��
	if( !fin.Open(pAcmMgr->m_strReadFile, CFile::modeRead | CFile::shareDenyWrite, &e ) ) {
		_stprintf_s(pAcmMgr->te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), pAcmMgr->m_strReadFile, e.m_cause);
		pAcmMgr->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, pAcmMgr->te.errstr);
		goto THREAD_END;
	}

	TRY {
		/// �f�[�^�̓ǂݍ���
		long lngCnt = 0;
		int intRead = 0;
		int leftover = 0;
		pAcmMgr->m_lngRdSz = 0;
		while(1) {
			if (pAcmMgr->m_bReadReset) {
				break;	// �ǂݍ��ݏI��
			}

			// �ǂݍ��݂���������ƃ��������������邽�߁A�����҂�����
			lngCnt = (long)pAcmMgr->m_listOfBuffer.GetCount();
			if (lngCnt > 10) {
				Sleep(10);  //CPU���׌���
				continue;
			}

			//// �t�@�C���ǂݍ���
			intRead = fin.Read(pSrcBuf + leftover, cbSrcBuf);
			if (intRead == 0) {
				break;
			}
			pAcmMgr->m_lngRdSz += intRead;	// �v���O���X�����p

			//// �ϊ�����
			ash.cbSrcLength = intRead + leftover;	// �ϊ����o�b�t�@�T�C�Y�̕ϓ�
			res = acmStreamConvert(has, &ash, ACM_STREAMCONVERTF_BLOCKALIGN);
			if(res != S_OK) {
				break;
			}
			//// �g���c���̃f�[�^����������ꍇ������
			leftover = ash.cbSrcLength - ash.cbSrcLengthUsed;
			if (leftover > 0) {
				memcpy(pSrcBuf, pSrcBuf+ash.cbSrcLengthUsed, leftover);
			}

			//// �o�b�t�@��������
			pAcmMgr->AddNewBuffer(pDstBuf, ash.cbDstLengthUsed);

			if (intRead < (int)cbSrcBuf) {
				break;
			} 
		} //while

		/// �t�@�C����Close
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
	// �X�g���[����Close
	if (has != NULL) {
		ash.cbSrcLength = cbSrcBuf;
		ash.cbDstLength = cbDstBuf;
		acmStreamUnprepareHeader(has, &ash, 0);
		acmStreamClose(has, 0);
	}

	// �o�̓o�b�t�@�̉��
	if (pSrcBuf) delete [] pSrcBuf;
	if (pDstBuf) delete [] pDstBuf;		

	pAcmMgr->m_bReadThread = FALSE;
	return 0;
};


// �R���X�g���N�^
CAcmMgr::CAcmMgr()
{
	m_idDriver = 0;
	m_bReadReset = TRUE;

	// �h���C�o���X�g�̃������m��
	m_dicDriver = new CMap<HACMDRIVERID, HACMDRIVERID, ACMDRIVERDETAILS, ACMDRIVERDETAILS>(10);

	// �t�H�[�}�b�g���X�g�̃������m��
	m_dicFormat = new CTypedPtrMap<CMapStringToPtr, CString, LPWAVEFORMATEX>; 

	FreeListOfBuffer();
	m_bSaveThread = FALSE;
	m_bSaveReset = TRUE;
}

// �f�X�g���N�^
CAcmMgr::~CAcmMgr(void)
{
	// �t�H�[�}�b�g���X�g�̉��
	ResetFormats();
	delete(m_dicFormat);

	// �h���C�o���X�g�̉��
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
//  �@�\     : �h���C�o�iCodec�j���X�g�擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : �h���C�o���X�g����
//  
//  ���l     : 
//  
int CAcmMgr::GetDrivers(void)
{
	// �h���C�o�iCodec�j���X�g����ɂ���
	m_dicDriver->RemoveAll();

	// dwInstance:CAcmMgr�̃I�u�W�F�N�g�A�h���X�Q�Ƃ�n��
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
//  �@�\     : �h���C�o���X�g�擾
//  
//  �@�\���� : �h���C�o���X�g���X�V����
//  
//  �Ԃ�l   : �h���C�o���X�g����
//  
//  ���l     : 
//  
int CAcmMgr::GetDriverList(
CStringArray &aryStr)		// �擾�����h���C�o���X�g
{
	// �t�H�[�}�b�g���X�g�X�V
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
//  �@�\     : �J�����g�h���C�o�̃t�H�[�}�b�g���X�g�擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : �t�H�[�}�b�g���X�g����
//  
//  ���l     : 
//  
int CAcmMgr::GetFormats(
CStringArray &aryStr,	// �擾�����t�H�[�}�b�g���X�g
LPWAVEFORMATEX pwf,		// �ϊ����t�H�[�}�b�g
BOOL blSvr)				// TRUE:���X�g���������i�`�����l���E���[�g����v�j
{
	int i;

	// �t�H�[�}�b�g���X�g����ɂ���
	ResetFormats();

	// driver handle�̃I�[�v��
	HACMDRIVER had;
	if (acmDriverOpen(&had, m_idDriver, 0) != 0) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Audio codec open error:%d"),  m_idDriver);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
		return 0;
	}

	// �t�H�[�}�b�g���X�g�擾
	// �ꎞ�t�H�[�}�b�g�̈悪�K����傫���Ȃ�ꍇ������
	MMRESULT res;
	ACMFORMATDETAILS formatDetails;
	memset(&formatDetails, 0, sizeof(ACMFORMATDETAILS));
	LPWAVEFORMATEX ptmp;
	TRY {
		res = acmMetrics(NULL, ACM_METRIC_MAX_SIZE_FORMAT, &formatDetails.cbwfx);
		if (res) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("acmMetrics error:%d"),  res);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
			acmDriverClose(had, 0);		// driver handle�̃N���[�Y
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
		formatDetails.pwfx          = ptmp;		// �ꎞ�t�H�[�}�b�g�̈�i�R�[���o�b�N���̂ݎg�p�j
		formatDetails.cbStruct      = sizeof(ACMFORMATDETAILS);
		formatDetails.fdwSupport    = 0;
		res = acmFormatEnum(had, &formatDetails, (ACMFORMATENUMCB)acmFormatEnumCallback, (DWORD)(void*)this, ACM_FORMATENUMF_CONVERT);
		if (res) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("acmFormatEnum error:%d"),  res);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
			acmDriverClose(had, 0);		// driver handle�̃N���[�Y
			return 0;
		}
		// �ҋ@
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
		acmDriverClose(had, 0);		// driver handle�̃N���[�Y
		return 0;
    }
	END_CATCH_ALL	
	delete ptmp;	// �ꎞ�t�H�[�}�b�g�̈���

	acmDriverClose(had, 0);		// driver handle�̃N���[�Y

	// �t�H�[�}�b�g���X�g���X�V
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
//  �@�\     : �J�����g�h���C�o�̃t�H�[�}�b�g���X�g�擾
//  
//  �@�\���� : �I���h���C�o�̃t�H�[�}�b�g���X�g���X�V����
//  
//  �Ԃ�l   : �t�H�[�}�b�g���X�g����
//  
//  ���l     : 
//  
int CAcmMgr::GetFormatList(
CStringArray &aryStr,		// �擾�����t�H�[�}�b�g���X�g
LPWAVEFORMATEX pFromWf,		// �ϊ����t�H�[�}�b�g
BOOL blSvr)					// TRUE:���X�g���������i�`�����l���E���[�g����v�j
{
	int i;

	// ���݂̕ϊ����t�H�[�}�b�g���N���A
	memset(&m_FromWf, 0, sizeof(WAVEFORMATEX));
	memset(&m_FromMf, 0, sizeof(MPEGLAYER3WAVEFORMAT));

	// �t�H�[�}�b�g���X�g�X�V
	// ���s����ꍇ������̂ŁA���x�����g���C
	int intCnt;
	for (i = 0; i < 10; i++) {
		intCnt =  GetFormats(aryStr, pFromWf, blSvr);
		if (intCnt > 0) break;
	}
	if (intCnt <= 0) return 0;

	// ���݂̕ϊ����t�H�[�}�b�g��ݒ�
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
//  �@�\     : �t�H�[�}�b�g������ϊ���t�H�[�}�b�g���擾
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
BOOL CAcmMgr::FindFormat(
CString strFormat) 		// �t�H�[�}�b�g��
{
	// ���݂̕ϊ���t�H�[�}�b�g���N���A
	memset(&m_ToWf, 0, sizeof(WAVEFORMATEX));
	memset(&m_ToMf, 0, sizeof(MPEGLAYER3WAVEFORMAT));

	// �t�H�[�}�b�g������ϊ���t�H�[�}�b�g���擾
	LPWAVEFORMATEX pwf;
	if (!m_dicFormat->Lookup(strFormat, pwf)) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Audio codec FindFormat error:%s"), strFormat);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
		return FALSE;
	}

	// ���݂̕ϊ���t�H�[�}�b�g��ݒ�
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
// �œK�ȃt�H�[�}�b�g�̌����iMP3->WAV only�j
BOOL CAcmMgr::FindFormat(
LPMPEGLAYER3WAVEFORMAT pFromMf, 		// �ϊ���MP3�t�H�[�}�b�g
LPWAVEFORMATEX pToMf)					// �ϊ���WAV�t�H�[�}�b�g
{
	memset(&m_FromMf, 0, sizeof(MPEGLAYER3WAVEFORMAT));
	memset(&m_ToWf, 0, sizeof(WAVEFORMATEX));

	// Format�w��
	memcpy(&m_FromMf, pFromMf, sizeof(MPEGLAYER3WAVEFORMAT));

	// �œK�ȃt�H�[�}�b�g�̌���
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
//  �@�\     : �t�H�[�}�b�g���X�g�̉��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
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
//  �@�\     : �h���C�o�iCodec�j�I��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : �I���h���C�o���́ANULL:�Y���h���C�o�Ȃ�
//  
//  ���l     : 
//  
CString CAcmMgr::FindDriver(
CnvType type,		// �ϊ��^�C�v
CString strCodec)	// �h���C�o�iCodec�j���́i������v�j
{
	CString strDriver;

	// �ϊ��^�C�v
	m_CnvType = type;

	// �h���C�o���X�g���Ȃ��ꍇ�͍쐬����
	int intCnt = (int)m_dicDriver->GetCount();
	if (intCnt == 0) {
		if (GetDrivers() <= 0) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("FindDriver error"));
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_ACM, te.errstr);
			return _T("");
		}
	}

	// �h���C�o�iCodec�j�I��
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

// �t�@�C���w�b�_����WAV�t�H�[�}�b�g�擾
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

// �t�@�C���w�b�_����MP3�t�H�[�}�b�g�擾
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
// for �ϊ�����
//////////////////////////////////////////////////////////////////////

// �o�b�t�@����t�@�C���ۑ�
BOOL CAcmMgr::Record(
CString strSaveFile)		// �ۑ��t�@�C����
{
	if ( !RecordStop() ) {
		return FALSE;
	}

	m_bSaveReset = FALSE;

	m_strSaveFile = strSaveFile;
	if (m_strSaveFile.GetLength() <= 0) return FALSE;

	// ����������t�@�C���ւ̏������݂��X���b�h��
	Sleep(500);		// �O��̃X���b�h���c���Ă���ꍇ������̂�
	CWinThread* pThread = NULL;
	if (m_CnvType == MP3_WAV) {
		pThread = AfxBeginThread(SaveWavWorkerThread, this);
	} else {
		pThread = AfxBeginThread(SaveMp3WorkerThread, this);
	}
	m_bSaveThread = (pThread == NULL) ? FALSE : TRUE;

	return m_bSaveThread;
}

// �t�@�C���ۑ����~
BOOL CAcmMgr::RecordStop()
{
	m_bSaveReset = TRUE;

/* �X���b�h���I������܂ŗL���ɂ���K�v�����邽�߁A�����ł͉�����Ȃ��i����Record����N���X�̏��Ŏ��ɍs����j
*/

	return TRUE;
}

// �t�@�C���ۑ���������
BOOL CAcmMgr::IsRecording()
{
	BOOL bResult = FALSE;

	// �^�����̃X���b�h�����삵�Ă���
	if (m_bSaveThread) {
		bResult = TRUE;
	} else {
		bResult = FALSE;
	}

	return bResult;
}

// �o�b�t�@�փt�@�C���ǂݍ���
BOOL CAcmMgr::Read(
CString strReadFile)		// �ǂݍ��݃t�@�C����
{
	if ( !ReadStop() ) {
		return FALSE;
	}

	m_bReadReset = FALSE;
	FreeListOfBuffer();		// �o�b�t�@�N���A

	m_strReadFile = strReadFile;
	if (m_strReadFile.GetLength() <= 0) return FALSE;

	// �t�@�C������o�b�t�@�ւ̏������݂��X���b�h��
	CWinThread* pThread = NULL;
	if (m_CnvType == MP3_WAV) {
		pThread = AfxBeginThread(ReadMp3WorkerThread, this);
	} else {
		pThread = AfxBeginThread(ReadWavWorkerThread, this);
	}
	m_bReadThread = (pThread == NULL) ? FALSE : TRUE;

	return m_bReadThread;
}

// �t�@�C���ǂݍ��ݒ��~
BOOL CAcmMgr::ReadStop()
{
	m_bReadReset = TRUE;

	return TRUE;
}

// �t�@�C���ǂݍ��ݒ�������
BOOL CAcmMgr::IsReading()
{
	BOOL bResult = FALSE;

	if (m_bReadThread) {
		// �ǂݍ��ݎ��̃X���b�h���������삵�Ă���
		bResult = TRUE;
	} else {
		bResult = FALSE;
	}

	return bResult;
}

// �ϊ����f�[�^���o�b�t�@��
BOOL CAcmMgr::AddNewBuffer(void *pData, long lngSize)
{
	CWaveBuffer *pBuf = new CWaveBuffer;

	pBuf->m_blActive = TRUE;	// �X���b�h�ɂ��A�N�Z�X�𐧌�����t���O

	m_listOfBuffer.AddTail(pBuf);
	( (CWaveBuffer*)m_listOfBuffer.GetTail() )->CopyBuffer(pData, lngSize / 4, 4);	// 4:16bit stereo
	if (m_nBufferSize == 0) {
		m_nBufferSize = (lngSize / 4) * 4;		// �o�b�t�@�T�C�Y
	}

	pBuf->m_blActive = FALSE;

	return TRUE;
}

// �f�[�^�o�b�t�@�����
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
	
	m_nBufferSize = 0;		// �o�b�t�@�T�C�Y
}

// �擪�f�[�^�o�b�t�@�̎Q��
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

// �擪�f�[�^�o�b�t�@�̍폜
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
// for ��ʃc�[��
//////////////////////////////////////////////////////////////////////

//
//  �@�\     : WAV->MP3�ϊ�
//  
//  �@�\���� : �w�肵���ϊ��^�C�v�ŕϊ�
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL CAcmMgr::ConvertWavToMp3(
CWnd *hWin,				// ���b�Z�[�W�\���p�n���h��
CnvType type,			// �ϊ��^�C�v
CString strReadFile,	// ���̓t�@�C��
CString strSaveFile,	// �o�̓t�@�C��
CString strCodec,		// CODEC����
CString strFormat,		// �o�͐�Format���́iGetFormatList�̌��ʂ���I���j
BOOL blWait)			// �ϊ����I������܂ő҂�
{
	BOOL blRet = TRUE;
	CProgressWnd wndProgress;

	// �v���O���X���b�Z�[�W�p
	CFileSpec fs(strReadFile);
	long lngSz = (long)fs.FileSize();	
	int intPercent = 0;
	int intPercentBak = 0;

	/// CODEC/Format�w��
	CString strDriver = FindDriver(type, strCodec);
	if (strDriver.GetLength() <= 0) return FALSE;
	if (!FindFormat(strFormat)) return FALSE;

	/// �t�@�C���Ǎ��݃X���b�h�J�n
	if (blWait) {
		// �v���O���X���b�Z�[�W�쐬
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

	Sleep(100);  //�o�b�t�@�̐����҂�

	/// �^���X���b�h�J�n
	if (!Record(strSaveFile)) {
		ReadStop();
		blRet = FALSE;
		goto PROC_END;
	}

	/// �����҂�
	if (blWait) {
		while(IsReading()) {
			if (!IsRecording()) {
				// �ǂݍ��ݒ��Ȃ̂ɏ������݃X���b�h���I�����Ă���ꍇ�̓G���[
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
// Codec�̑I����OS�ɔC����ꍇ
//
//  �@�\     : MP3->WAV�ϊ�
//  
//  �@�\���� : Codec/Format�̑I����OS�ɔC����
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : �����WAV�t�H�[�}�b�g���w�肷�邱�Ƃ͂ł��Ȃ�
//  
BOOL CAcmMgr::ConvertMp3ToWav(
CWnd *hWin,				// ���b�Z�[�W�\���p�n���h��
CString strReadFile,	// ���̓t�@�C��
CString strSaveFile,	// �o�̓t�@�C��
BOOL blWait)			// �ϊ����I������܂ő҂�
{
	BOOL blRet = TRUE;
	CProgressWnd wndProgress;

	// �v���O���X���b�Z�[�W�p
	CFileSpec fs(strReadFile);
	long lngSz = (long)fs.FileSize();	
	int intPercent = 0;
	int intPercentBak = 0;

	// �ϊ��^�C�v
	m_CnvType = MP3_WAV;

	// ���݂�Codec/Format���N���A
	m_idDriver = NULL;
	memset(&m_FromWf, 0, sizeof(WAVEFORMATEX));
	memset(&m_FromMf, 0, sizeof(MPEGLAYER3WAVEFORMAT));
	memset(&m_ToWf, 0, sizeof(WAVEFORMATEX));
	memset(&m_ToMf, 0, sizeof(MPEGLAYER3WAVEFORMAT));

	// CODEC/Format�w��
	MPEGLAYER3WAVEFORMAT FromMf;
	GetFileFormat(strReadFile, &FromMf);
	if (!FindFormat(&FromMf)) return FALSE;

	// �t�@�C���Ǎ��݃X���b�h�J�n
	if (blWait) {
		// �v���O���X���b�Z�[�W�쐬
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

	Sleep(100);  //�o�b�t�@�̐����҂�

	// �^���X���b�h�J�n
	if (!Record(strSaveFile)) {
		ReadStop();
		blRet = FALSE;
		goto PROC_END;
	}

	// �����҂�
	if (blWait) {
		while(IsReading()) {
			if (!IsRecording()) {
				// �ǂݍ��ݒ��Ȃ̂ɏ������݃X���b�h���I�����Ă���ꍇ�̓G���[
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
		Sleep(100);		// �X���b�h�����҂�
	}

PROC_END:
	if (!wndProgress.Cancelled()) {
		wndProgress.Hide();
	}
	wndProgress.DestroyWindow();

	return blRet;
}
