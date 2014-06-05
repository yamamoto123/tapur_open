// WaveIn.cpp: implementation of the CWaveIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveIn.h"
#include "FileSpec.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

#ifdef _DEBUG
int inew1 = 0;
int inew2 = 0;
#endif

//////////////////////////////////////////////////////////////////////
// �t�@�C���������݃X���b�h
UINT WaveInWorkerThread( LPVOID pParam )
{
	CWaveIn* pWaveIn = (CWaveIn*)pParam;
	CTPerror te;

	CFileException e;
	WAVEFORMATEX Fmt;
	DWORD dwFileSize;
	DWORD dwNum;
	CFile f;
	CString strErr;
	SHORT tmpDat[WAVEIN_BUFFER_SIZE * 4];	// �X�e���I�^�����̈ꎞ�o�b�t�@

	CString strFile = pWaveIn->m_strSaveFile;
	if (strFile.GetLength() == 0) {
		goto THREAD_END;
	}

	/// �t�@�C���I�[�v��
	Fmt = pWaveIn->m_wave.GetFormat();		// �ۑ��pWAVE�t�H�[�}�b�g
	if( !f.Open(strFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e ) ) {
		_stprintf(te.errstr, _T("File could not be opened:%s, %s\n"), strFile, e.m_cause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
		goto THREAD_END;
	}

	TRY {
		/// �����o�b�t�@�L�ŃX�e���I�w��̏ꍇ�A�w�b�_�[��񍷂��ւ�
		if (pWaveIn->m_SyncStereo > 0 && pWaveIn->m_plistOfSyncBuffer) {
			Fmt.nChannels = 2;
			Fmt.nBlockAlign = Fmt.nChannels * Fmt.wBitsPerSample/8;
			Fmt.nAvgBytesPerSec = Fmt.nSamplesPerSec * Fmt.nBlockAlign;
		}

		/// �ŏ��̏������݁i�w�b�_���������݁j
		f.Write("RIFF", 4) ;
		dwFileSize = 0;	// dummy
		f.Write(&dwFileSize, sizeof(dwFileSize)) ;
		f.Write("WAVEfmt ", 8) ;	// �啶���������͎��ʂ����̂Œ��ӁI
		DWORD dwFmtSize = 16L;
		f.Write(&dwFmtSize, sizeof(dwFmtSize)) ;
		WORD wFormatTag = WAVE_FORMAT_PCM;
		f.Write(&wFormatTag, sizeof(wFormatTag)) ;
		f.Write(&Fmt.nChannels, sizeof(Fmt.nChannels)) ;
		f.Write(&Fmt.nSamplesPerSec, sizeof(Fmt.nSamplesPerSec)) ;
		f.Write(&Fmt.nAvgBytesPerSec, sizeof(Fmt.nAvgBytesPerSec)) ;
		f.Write(&Fmt.nBlockAlign, sizeof(Fmt.nBlockAlign)) ;
		f.Write(&Fmt.wBitsPerSample, sizeof(Fmt.wBitsPerSample)) ;
		f.Write("data", 4) ;
		dwNum = 0;	// dummy
		f.Write(&dwNum, sizeof(dwNum)) ;

		/// �g�`�f�[�^�̏�������
		long i, cnt1 = 0, cnt2 = 0;
		long lngCnt;
		long lngBufCnt1, lngBufCnt2;
		POSITION pos = NULL;
		CWaveBuffer *pBuf;
		CWaveBuffer *pSyncBuf;
		while(1) {
			Sleep(10);  //CPU���׌���
			if (pWaveIn->m_bResetRequired) {
			//// �c��̃o�b�t�@���������Ę^���I��
				cnt1 = pWaveIn->m_listOfBuffer.GetCount();
				if (pWaveIn->m_plistOfSyncBuffer) {
					cnt2 = pWaveIn->m_plistOfSyncBuffer->GetCount();
					if (cnt1 > 0 && cnt2 > 0) {
						// �c��̃o�b�t�@������
					} else {
						// �c��̃o�b�t�@��j��
						if (cnt1 == 0) {
							for(i = 0; i < cnt2; i++) {
								pos = pWaveIn->m_plistOfSyncBuffer->GetHeadPosition();
								if (pos == NULL)  break;
								pSyncBuf = (CWaveBuffer *)pWaveIn->m_plistOfSyncBuffer->GetAt(pos);
								//pSyncBuf = (CWaveBuffer *)pWaveIn->m_plistOfSyncBuffer->GetHead();
								if (!pSyncBuf) break;
								if (pSyncBuf->m_blActive) break;		// �o�b�t�@�������ݒ�
								if (pSyncBuf) {
									delete pSyncBuf;
									pSyncBuf = NULL;
								}
								pWaveIn->m_plistOfSyncBuffer->RemoveHead();
							}
							cnt2 = pWaveIn->m_plistOfSyncBuffer->GetCount();
						} else if (cnt2 == 0) {
							for(i = 0; i < cnt1; i++) {
								pos = pWaveIn->m_listOfBuffer.GetHeadPosition();
								if (pos == NULL)  break;
								pBuf = (CWaveBuffer *)pWaveIn->m_listOfBuffer.GetAt(pos);
								//pBuf = (CWaveBuffer *)pWaveIn->m_listOfBuffer.GetHead();
								if (!pBuf) break;
								if (pBuf->m_blActive) break;		// �o�b�t�@�������ݒ�
								if (pBuf) {
									delete pBuf;
									pBuf = NULL;
								}
								pWaveIn->m_listOfBuffer.RemoveHead();
							}
							cnt1 = pWaveIn->m_listOfBuffer.GetCount();
						}
						if (cnt1 == 0 && cnt2 == 0) break;
					}
				} else {
					if (cnt1 == 0) break;
				}
			} else {
			//// ������h�����߁A�o�b�t�@���P�c���ď���(�o�b�t�@�̐����҂�)
				if (pWaveIn->m_listOfBuffer.GetCount() <= 1 || pWaveIn->m_listOfBuffer.IsEmpty()) continue;
				if (pWaveIn->m_plistOfSyncBuffer) {
					if (pWaveIn->m_plistOfSyncBuffer->GetCount() <= 1 || pWaveIn->m_plistOfSyncBuffer->IsEmpty()) continue;
				}
			}
			if (pWaveIn->m_listOfBuffer.IsEmpty()) continue;
			pos = pWaveIn->m_listOfBuffer.GetHeadPosition();
			if (pos == NULL)  continue;
			pBuf = (CWaveBuffer *)pWaveIn->m_listOfBuffer.GetAt(pos);
			//pBuf = (CWaveBuffer *)pWaveIn->m_listOfBuffer.GetHead();
			if (!pBuf) continue;
			if (pBuf->m_blActive) continue;		// �o�b�t�@�������ݒ�

			//// �g�`�f�[�^�̏�������
			lngCnt = pBuf->GetSampleSize() * pBuf->GetNumSamples() / sizeof(SHORT);
			//// �����o�b�t�@������ꍇ�͍������ĕۑ�
			if (pWaveIn->m_plistOfSyncBuffer) {
				pSyncBuf = (CWaveBuffer *)pWaveIn->m_plistOfSyncBuffer->GetHead();
				if (!pSyncBuf) continue;
				if (pSyncBuf->m_blActive) continue;		// �o�b�t�@�������ݒ�
				///// �����o�b�t�@(STDIN)�̃T���v�����O�������߂���ꍇ������iVAC�̏�Q�H�j�������o�b�t�@���ꕔ�폜���Ē�������
				lngBufCnt1 = pWaveIn->m_listOfBuffer.GetCount();
				lngBufCnt2 = pWaveIn->m_plistOfSyncBuffer->GetCount();
				if (lngBufCnt1 < lngBufCnt2) {
					if (pSyncBuf) {
						delete pSyncBuf;
						pSyncBuf = NULL;
					}
					pWaveIn->m_plistOfSyncBuffer->RemoveHead();
					continue;
				}
				///// �������ĕۑ�
				SHORT *pDat = (SHORT *)pBuf->GetBuffer();
				SHORT *pSyncDat = (SHORT *)pSyncBuf->GetBuffer();
				long tmp = 0;
				long tmp1 = 0;
				long tmp2 = 0;
				for(i = 0; i < lngCnt ;i++) {
					if (pWaveIn->m_SyncStereo == 1) {
					////// �X�e���I�^��(���S�p��)
						if (pWaveIn->m_SyncSw == 0) {
						// WAVE����
							tmpDat[(i*2)] = *(pSyncDat+i);
							tmpDat[i*2+1] = *(pDat+i);
						} else if (pWaveIn->m_SyncSw == 1) {
						// �������Ȃ�
							tmpDat[(i*2)] = 0;
							tmpDat[i*2+1] = *(pDat+i);
						} else if (pWaveIn->m_SyncSw == 2) {
						// ���������ւ�
							tmpDat[(i*2)] = *(pSyncDat+i);
							tmpDat[i*2+1] = 0;
						}
					} else if (pWaveIn->m_SyncStereo == 2) {
					////// �X�e���I�^��(�����p��)
						tmp1 = *(pSyncDat+i);
						tmp2 = *(pDat+i);
						if (pWaveIn->m_SyncSw == 0) {
						// WAVE����
							tmp = (long)(tmp1 * 1.0) + (long)(tmp2 * 0.2);
							if (tmp > 32767) {
								tmpDat[(i*2)] = 32767;
							} else if (tmp < -32767) {
								tmpDat[(i*2)] = -32767;
							} else {
								tmpDat[(i*2)] = (SHORT)tmp;
							}
							tmp = (long)(tmp2 * 1.0) + (long)(tmp1 * 0.2);
							if (tmp > 32767) {
								tmpDat[i*2+1] = 32767;
							} else if (tmp < -32767) {
								tmpDat[i*2+1] = -32767;
							} else {
								tmpDat[i*2+1] = (SHORT)tmp;
							}
						} else if (pWaveIn->m_SyncSw == 1) {
						// �������Ȃ�
							tmpDat[(i*2)] = (SHORT)(*(pDat+i) * 0.2);
							tmpDat[i*2+1] = (SHORT)(*(pDat+i) * 1.0);
						} else if (pWaveIn->m_SyncSw == 2) {
						// ���������ւ�
							tmpDat[(i*2)] = (SHORT)(*(pSyncDat+i) * 1.0);
							tmpDat[i*2+1] = (SHORT)(*(pSyncDat+i) * 0.2);
						}
					} else {
					////// monoral
						if (pWaveIn->m_SyncSw == 0) {
						// WAVE����
							tmp = *(pDat+i) + *(pSyncDat+i);
							if (tmp > 32767) {
								*(pDat+i) = 32767;
							} else if (tmp < -32767) {
								*(pDat+i) = -32767;
							} else {
								*(pDat+i) = (SHORT)tmp;
							}
						} else if (pWaveIn->m_SyncSw == 1) {
						// �������Ȃ�
						} else if (pWaveIn->m_SyncSw == 2) {
						// ���������ւ�
							*(pDat+i) = *(pSyncDat+i);
						}
					}
				}
			}

			if (pWaveIn->m_SyncStereo > 0 && pWaveIn->m_plistOfSyncBuffer) {
				// stereo�̂��߁A�o�b�t�@�T�C�Y�͔{�ɂȂ�
				f.Write(tmpDat, pBuf->GetSampleSize() * pBuf->GetNumSamples() * 2);
			} else {
				f.Write(pBuf->GetBuffer(), pBuf->GetSampleSize() * pBuf->GetNumSamples());
			}
						
			//// �o�b�t�@�̉��
			if (pBuf) {
				delete pBuf;
				pBuf = NULL;
			}
			pWaveIn->m_listOfBuffer.RemoveHead();
			if (pWaveIn->m_plistOfSyncBuffer) {
				if (pSyncBuf) {
					delete pSyncBuf;
					pSyncBuf = NULL;
				}
				pWaveIn->m_plistOfSyncBuffer->RemoveHead();
			}
//TRACE(_T("list I:%ld Y:%ld\n"), pWaveIn->m_listOfBuffer.GetCount(), pWaveIn->m_plistOfSyncBuffer->GetCount());
//TRACE(_T("pWaveIn->m_SyncSw=%d\n"), pWaveIn->m_SyncSw);
		} //while

	}
    CATCH(CFileException, e)
    {
		switch(e->m_cause) {
		case CFileException::diskFull:
			AfxMessageBox(_T("HardDisk is full! finish this application."));
		default:
			f.Close();
			_stprintf(te.errstr, _T("File write error:%s, %s\n"), strFile, e->m_cause);
			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
			goto THREAD_END;
		}
	}
    AND_CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf(te.errstr, _T("Record error:%s, %s\n"), strFile, szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
    }
	END_CATCH_ALL

	//// �w�b�_�̏C��
	TRY {
		dwFileSize = (DWORD)(f.GetLength() - 8);
		dwNum = dwFileSize - 36;
		f.Seek(4, CFile::begin);
		f.Write(&dwFileSize, sizeof(dwFileSize));
		f.Seek(40, CFile::begin);
		f.Write(&dwNum, sizeof(dwNum));
		f.Flush();
		f.Close();
	}
    CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf(te.errstr, _T("File header write error:%s, %s\n"), strFile, szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
		f.Close();
    }
	END_CATCH_ALL

THREAD_END:
	pWaveIn->m_blWorkThread = FALSE;
	AfxEndThread(0);
	return 0;
};

//////////////////////////////////////////////////////////////////////
void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	TRY {
		switch(uMsg) {
		case MM_WIM_DATA:
			WAVEHDR* pWaveHdr = ( (WAVEHDR*)dwParam1 );
			CWaveIn* pWaveIn = (CWaveIn*)(pWaveHdr->dwUser);

			if (pWaveHdr && hwi && pWaveIn) {
				if ((pWaveHdr->dwFlags & WHDR_DONE) == WHDR_DONE) {
					pWaveHdr->dwFlags = 0;
					if ( pWaveIn->IsError(waveInUnprepareHeader(hwi, pWaveHdr, sizeof(WAVEHDR))) ) {
						break;
					}
					
					if (!pWaveIn->IsRecording()) break;
					if (pWaveHdr->dwBufferLength != WAVEIN_BUFFER_SIZE) break;
					if (!pWaveHdr->lpData) break;

					/// ���A���^�C���Đ��p�Ƀo�b�t�@���R�s�[(1)
					// �o�b�t�@�̊J����PlaySound�Ŕ񓯊��ɍs���邽�߁A
					// pWriteHdr�̓N���X�����o�̂悤�ȌŒ�A�h���X�������Ă͂����Ȃ�
					WAVEHDR* pWriteHdr1;
					WAVEHDR* pWriteHdr2;
					if (pWaveIn->m_blPlayDev1) {
						pWriteHdr1 = new WAVEHDR;
						if(!pWriteHdr1) break;
						memcpy(pWriteHdr1, pWaveHdr, sizeof(WAVEHDR));
						BYTE * pSound1 = new BYTE[pWaveHdr->dwBufferLength];
						if(!pSound1) {
							if(pWriteHdr1) delete pWriteHdr1;
							break;
						}
						memcpy(pSound1, pWaveHdr->lpData, pWaveHdr->dwBufferLength);
						pWriteHdr1->lpData = (char*)pSound1;
	//TRACE(_T("new1=%d dev=%s\n"), ++inew1, pWaveIn->m_strPlayDev1);
						/// ���A���^�C���Đ��p�X���b�h�ɍĐ��w��
						// �o�b�t�@�̓��A���^�C���Đ��p�X���b�h�ŏ���
						if (pWaveIn->m_pPlaySound1) {
							if (!pWaveIn->m_pPlaySound1->PostThreadMessage(WM_PLAYSOUND_PLAYBLOCK, GetCurrentThreadId(),(LPARAM)pWriteHdr1)) {
	#ifdef _DEBUG
	inew1--;
	#endif
								if (pWriteHdr1->lpData) {
									delete[] pWriteHdr1->lpData;
								}
								if(pWriteHdr1) delete pWriteHdr1;
							}
						} 
					}

					/// ���A���^�C���Đ��p�Ƀo�b�t�@���R�s�[(2)
					// �o�b�t�@�̊J����PlaySound�Ŕ񓯊��ɍs���邽�߁A
					// pWriteHdr�̓N���X�����o�̂悤�ȌŒ�A�h���X�������Ă͂����Ȃ�
					if (pWaveIn->m_blPlayDev2) {
						pWriteHdr2 = new WAVEHDR;
						if(!pWriteHdr2) break;
						memcpy(pWriteHdr2, pWaveHdr, sizeof(WAVEHDR));
						BYTE * pSound2 = new BYTE[pWaveHdr->dwBufferLength];
						if(!pSound2) {
							if(pWriteHdr2) delete pWriteHdr2;
							break;
						}
						memcpy(pSound2, pWaveHdr->lpData, pWaveHdr->dwBufferLength);
						pWriteHdr2->lpData = (char*)pSound2;
	//TRACE(_T("new2=%d dev=%s\n"), ++inew2, pWaveIn->m_strPlayDev2);
						/// ���A���^�C���Đ��p�X���b�h�ɍĐ��w��
						// �o�b�t�@�̓��A���^�C���Đ��p�X���b�h�ŏ���
						if (pWaveIn->m_blPlayDev2 && pWaveIn->m_pPlaySound2) {
							if (!pWaveIn->m_pPlaySound2->PostThreadMessage(WM_PLAYSOUND_PLAYBLOCK, GetCurrentThreadId(),(LPARAM)pWriteHdr2)) {
	#ifdef _DEBUG
	inew2--;
	#endif
								if (pWriteHdr2->lpData) {
									delete[] pWriteHdr2->lpData;
								}
								if(pWriteHdr2) delete pWriteHdr2;
							}
						} 
					}

					// �ʏ�̘^���o�b�t�@����
					if (pWaveHdr->dwBytesRecorded > 0) {
						pWaveIn->AddNewBuffer(pWaveHdr);
					}
					if (pWaveHdr->lpData) {
						delete[] pWaveHdr->lpData;
						pWaveHdr->lpData = NULL;
					}
				}

				if ( !pWaveIn->ResetRequired(pWaveIn) ) {
					if ( !pWaveIn->AddNewHeader(hwi) ) {
						break;
					}
				}
			}
			break;
		}
	}
    CATCH_ALL(e)
    {
		CString strErr;
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		strErr.Format(_T("waveInProc error:%s\n"), szCause);
		AfxMessageBox(strErr);
		return;
    }
	END_CATCH_ALL
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
CWaveIn::CWaveIn(WAVEFORMATEX tagFormat, const CWaveDevice& aDevice) : m_waveDevice(aDevice), \
	m_hWaveIn(0), m_nIndexWaveHdr(NUMWAVEINHDR - 1), m_bResetRequired(true)
{
	InitListOfHeader();
	FreeListOfBuffer();
	m_plistOfSyncBuffer = NULL;
	m_blVac = FALSE;
	m_blVirtualPause = FALSE;
	m_pPlaySound1 = NULL;
	m_pPlaySound2 = NULL;
	m_blPlayDev1 = FALSE;
	m_blPlayDev2 = FALSE;
	m_blWorkThread = FALSE;
}

//////////////////////////////////////////////////////////////////////
CWaveIn::CWaveIn() : m_hWaveIn(0), m_bResetRequired(true)
{
	InitListOfHeader();
	FreeListOfBuffer();
	m_plistOfSyncBuffer = NULL;
	m_blVac = FALSE;
	m_blVirtualPause = FALSE;
	m_pPlaySound1 = NULL;
	m_pPlaySound2 = NULL;
	m_blPlayDev1 = FALSE;
	m_blPlayDev2 = FALSE;
	m_SyncSw = 0;
	m_SyncStereo = 0;
	m_blWorkThread = FALSE;
}

//////////////////////////////////////////////////////////////////////
CWaveIn::~CWaveIn()
{
	Close();
	FreeListOfBuffer();
	FreeListOfHeader();

	m_plistOfSyncBuffer = NULL;
	m_blVac = FALSE;
	m_blVirtualPause = FALSE;
	m_pPlaySound1 = NULL;
	m_pPlaySound2 = NULL;
	m_blPlayDev1 = FALSE;
	m_blPlayDev2 = FALSE;

}

//////////////////////////////////////////////////////////////////////
// Initialisation
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
void CWaveIn::SetDevice(const CWaveDevice &aDevice)
{
	m_waveDevice = aDevice;
}

//////////////////////////////////////////////////////////////////////
void CWaveIn::SetWaveFormat(WAVEFORMATEX tagFormat)
{
	m_wave.BuildFormat(tagFormat.nChannels, tagFormat.nSamplesPerSec, tagFormat.wBitsPerSample, m_blVac);
}

//////////////////////////////////////////////////////////////////////
void CWaveIn::InitListOfHeader()
{
	for (int i = 0; i < NUMWAVEINHDR; i++) {
		m_tagWaveHdr[i].lpData = NULL;
		ZeroMemory(m_tagWaveHdr + i, sizeof(WAVEHDR));
	}
}

//////////////////////////////////////////////////////////////////////
// Son
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
bool CWaveIn::Close()
{
	bool ret = true;

	if (m_hWaveIn != NULL) {
		if ( !Stop() ) {
			ret = false;
		}
		if ( IsError( waveInClose(m_hWaveIn)) ) {
			ret = false;
		}
		m_hWaveIn = 0;
	}

	/// ���A���^�C���Đ��̏I��
	if (m_blPlayDev1 && m_pPlaySound1) {
		//// �Đ��I��
		m_pPlaySound1->PostThreadMessage(WM_PLAYSOUND_STOPPLAYING,0,0);
		//// �o�b�t�@�J��
		m_pPlaySound1->PostThreadMessage(WM_PLAYSOUND_ENDTHREAD,0,0);
		::WaitForSingleObject(m_pPlaySound1->m_hThread, 5000);
		m_pPlaySound1 = NULL;
	}
	if (m_blPlayDev2 && m_pPlaySound2) {
		//// �Đ��I��
		m_pPlaySound2->PostThreadMessage(WM_PLAYSOUND_STOPPLAYING,0,0);
		//// �o�b�t�@�J��
		m_pPlaySound2->PostThreadMessage(WM_PLAYSOUND_ENDTHREAD,0,0);
		::WaitForSingleObject(m_pPlaySound2->m_hThread, 5000);
		m_pPlaySound2 = NULL;
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::Continue()
{
	/// ���z��~�̏ꍇ�͉������Ȃ�
	if (m_blVirtualPause) return true;

	if (m_hWaveIn) {
		return !IsError( waveInStart(m_hWaveIn) );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// ���̓f�o�C�X�����A���̏ꍇ�v���C�o�b�N��VAC
// ���̓f�o�C�X��VAC�̏ꍇ�v���C�o�b�N�̓��A��
bool CWaveIn::Open()
{
	m_plistOfSyncBuffer = NULL;

	/// ���A���^�C���Đ��̊J�n
	if (m_blPlayDev1) {
		m_pPlaySound1 = new CPlaySound();
		m_pPlaySound1->CreateThread();
		WAVEFORMATEX fmt = m_wave.GetFormat();
		m_pPlaySound1->SetDevice(m_strPlayDev1);
		m_pPlaySound1->SetFormat(fmt.nChannels, fmt.nSamplesPerSec, fmt.wBitsPerSample, !m_blVac);
		m_pPlaySound1->PostThreadMessage(WM_PLAYSOUND_STARTPLAYING, 0, 0);				
	}
	if (m_blPlayDev2) {
		m_pPlaySound2 = new CPlaySound();
		m_pPlaySound2->CreateThread();
		WAVEFORMATEX fmt = m_wave.GetFormat();
		m_pPlaySound2->SetDevice(m_strPlayDev2);
		m_pPlaySound2->SetFormat(fmt.nChannels, fmt.nSamplesPerSec, fmt.wBitsPerSample, !m_blVac);
		m_pPlaySound2->PostThreadMessage(WM_PLAYSOUND_STARTPLAYING, 0, 0);				
	}

	/// ���̓f�o�C�X�I�[�v��
	MMRESULT r = waveInOpen(&m_hWaveIn, m_waveDevice.GetDevice(), &(m_wave.m_vacWaveFormat.Fmt), (DWORD)waveInProc, NULL, CALLBACK_FUNCTION);	
	return !IsError(r);
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::Pause()
{
	/// ���z��~�̏ꍇ�͉������Ȃ�
	if (m_blVirtualPause) return true;

	if (m_hWaveIn) {
		return !IsError( waveInStop(m_hWaveIn) );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::Record(CString strSaveFile)
{
	if (!Record()) return false;
	m_strSaveFile = strSaveFile;
	if (m_strSaveFile.GetLength() <= 0) return false;

	// ����������t�@�C���ւ̏������݂��X���b�h���i���僁����������邽�߁j
	Sleep(500);		// �O��̃X���b�h���c���Ă���ꍇ������̂�
	CWinThread* pThread = AfxBeginThread(WaveInWorkerThread, this);
	m_blWorkThread = (pThread == NULL) ? FALSE : TRUE;
#ifdef _DEBUG
_stprintf(errstr, _T("CWinThread:%d"), pThread);
ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, errstr);
#endif

	return true;
}

bool CWaveIn::Record(CWaveIn *pWaveIn)
{
	if (!Record()) return false;
	if (pWaveIn == NULL) return false;

	// ���������o�b�t�@�𑼂�WaveIn�C���X�^���X�Ɋ֘A�t����
	pWaveIn->m_plistOfSyncBuffer = &m_listOfBuffer;

	return true;
}

bool CWaveIn::Record()
{
	ASSERT(m_hWaveIn);

	if ( !Stop() ) {
		return false;
	}

	m_bResetRequired = false;
	FreeListOfBuffer();
	FreeListOfHeader();
	m_nIndexWaveHdr = NUMWAVEINHDR - 1;
	m_nBufferSize = WAVEIN_BUFFER_SIZE;
	for (int i = 0; i < NUMWAVEINHDR; i++) {
		if ( !AddNewHeader(m_hWaveIn) ) {
			return false;
		}
	}
	if ( IsError(waveInStart(m_hWaveIn)) ) {
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::Stop()
{
	if (m_hWaveIn != NULL) {
		m_bResetRequired = true;
		::Sleep(10);
		if ( IsError(waveInReset(m_hWaveIn)) ) {
			return false;
		}
/* �X���b�h���I������܂ŗL���ɂ���K�v�����邽�߁A�����ł͉�����Ȃ��i����Record����N���X�̏��Ŏ��ɍs����j
		FreeListOfBuffer();
		FreeListOfHeader();
*/
	}

	return true;
}

//////////////////////////////////////////////////////////////////////

bool CWaveIn::AddNewBuffer(WAVEHDR *pWaveHdr)
{
	ASSERT(pWaveHdr);

	/// ���z��~�̏ꍇ�̓f�[�^��j������
	if (m_blVirtualPause) return true;

	CWaveBuffer *pBuf = new CWaveBuffer;
	pBuf->m_blActive = TRUE;	// �X���b�h�ɂ��A�N�Z�X�𐧌�����t���O
	m_listOfBuffer.AddTail(pBuf);
	( (CWaveBuffer*)m_listOfBuffer.GetTail() )->CopyBuffer( pWaveHdr->lpData, \
		pWaveHdr->dwBytesRecorded / m_wave.GetFormat().nBlockAlign, \
		m_wave.GetFormat().nBlockAlign );
	pBuf->m_blActive = FALSE;

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::AddNewHeader(HWAVEIN hwi)
{
	ASSERT(m_nBufferSize > 0);

	m_nIndexWaveHdr = (m_nIndexWaveHdr == NUMWAVEINHDR - 1) ? 0 : m_nIndexWaveHdr + 1;
	if (m_tagWaveHdr[m_nIndexWaveHdr].lpData == NULL) {
		m_tagWaveHdr[m_nIndexWaveHdr].lpData = new char[m_nBufferSize];
	}
	ZeroMemory(m_tagWaveHdr[m_nIndexWaveHdr].lpData, m_nBufferSize);
	m_tagWaveHdr[m_nIndexWaveHdr].dwBufferLength = m_nBufferSize;
	m_tagWaveHdr[m_nIndexWaveHdr].dwFlags = 0;
	m_tagWaveHdr[m_nIndexWaveHdr].dwUser = (DWORD)(void*)this;
	if ( IsError(waveInPrepareHeader(hwi, &m_tagWaveHdr[m_nIndexWaveHdr], sizeof(WAVEHDR))) ) {
		return false;
	}
	if ( IsError(waveInAddBuffer(hwi, &m_tagWaveHdr[m_nIndexWaveHdr], sizeof(WAVEHDR))) ) {
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
void CWaveIn::FreeListOfHeader()
{
	for (int i = 0; i < NUMWAVEINHDR; i++) {
		if (m_tagWaveHdr[i].lpData) {
			delete[] m_tagWaveHdr[i].lpData;
			m_tagWaveHdr[i].lpData = NULL;
		}
		ZeroMemory(m_tagWaveHdr + i, sizeof(WAVEHDR));
	}
}

//////////////////////////////////////////////////////////////////////
void CWaveIn::FreeListOfBuffer()
{
	POSITION pos = m_listOfBuffer.GetHeadPosition();
	while (pos) {
		CWaveBuffer* pBuf = (CWaveBuffer*)m_listOfBuffer.GetNext(pos);
		if (pBuf) {
			delete pBuf;
			pBuf = NULL;
		}
	}
	m_listOfBuffer.RemoveAll();
}
//////////////////////////////////////////////////////////////////////
// GET
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/*
DWORD CWaveIn::GetNumSamples()
{
	DWORD dwTotal = 0L;
	POSITION pos = m_listOfBuffer.GetHeadPosition();
	while (pos) {
		CWaveBuffer* p_waveBuffer = (CWaveBuffer*) m_listOfBuffer.GetNext(pos);
		dwTotal += p_waveBuffer->GetNumSamples();
	}
	return dwTotal;
}
*/
//////////////////////////////////////////////////////////////////////
CString CWaveIn::GetError() const
{
	if (m_nError != MMSYSERR_NOERROR) {
		TCHAR szText[MAXERRORLENGTH + 1];
		if ( waveInGetErrorText(m_nError, szText, MAXERRORLENGTH) == MMSYSERR_NOERROR ) {
			return szText;
		}
	}
	return "";
}

//////////////////////////////////////////////////////////////////////
DWORD CWaveIn::GetPosition()
{
	if (m_hWaveIn) {
		MMTIME mmt;
		mmt.wType = TIME_SAMPLES;
		if ( IsError(waveInGetPosition(m_hWaveIn, &mmt, sizeof(MMTIME))) ) {
			return 0;
//			return -1;
		}
		else {
			return mmt.u.sample;
		}
	}
	return 0;
//	return -1;
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::IsError(MMRESULT nResult)
{
	m_nError = nResult;
	return (m_nError != MMSYSERR_NOERROR);
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::IsRecording()
{
	bool bResult = false;

/* �ȉ��̃t���O�͋@�\���Ă��Ȃ��H
	if (m_nIndexWaveHdr > -1 && m_tagWaveHdr[m_nIndexWaveHdr].dwFlags != 0) {
		bResult |= !(m_tagWaveHdr[m_nIndexWaveHdr].dwFlags & WHDR_DONE == WHDR_DONE);
	}
*/
	if (m_bResetRequired) {
		// �^�����łȂ�
		if (m_blWorkThread) {
			// �^�����łȂ����A�^�����̃X���b�h���������삵�Ă���
			bResult = true;
		} else {
			bResult = false;
		}
	} else {
		// �^����
		bResult = true;
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////
/*
CWave CWaveIn::MakeWave()
{
	void* pBuffer = new char[GetNumSamples() * m_wave.GetFormat().nBlockAlign];
	DWORD dwPosInBuffer = 0L;
	POSITION pos = m_listOfBuffer.GetHeadPosition();
	while (pos) {
		CWaveBuffer* p_waveBuffer = (CWaveBuffer*) m_listOfBuffer.GetNext(pos);
		CopyMemory( (char*)pBuffer + dwPosInBuffer, p_waveBuffer->GetBuffer(), p_waveBuffer->GetNumSamples() * p_waveBuffer->GetSampleSize() );
		dwPosInBuffer += p_waveBuffer->GetNumSamples() * p_waveBuffer->GetSampleSize();
	}
	m_wave.SetBuffer( pBuffer, GetNumSamples() );
	return m_wave;
}
*/

//////////////////////////////////////////////////////////////////////
bool CWaveIn::ResetRequired(CWaveIn* pWaveIn)
{
	return m_bResetRequired;
}

