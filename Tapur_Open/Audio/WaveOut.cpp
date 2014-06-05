// WaveOut.cpp: implementation of the CWaveOut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveOut.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

#ifdef _DEBUG
extern int inew1;
#endif

//////////////////////////////////////////////////////////////////////
void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	TRY {
		switch(uMsg) {
		case MM_WOM_DONE:
			WAVEHDR* pWaveHdr = ( (WAVEHDR*)dwParam1 );
			CWaveOut* pWaveOut = (CWaveOut*)(pWaveHdr->dwUser);

			if (pWaveHdr && hwo && pWaveOut) {
				if ((pWaveHdr->dwFlags & WHDR_DONE) == WHDR_DONE) {
					pWaveHdr->dwFlags = 0;
					if ( pWaveOut->IsError(waveOutUnprepareHeader(hwo, pWaveHdr, sizeof(WAVEHDR))) ) {
						break;
					}
					/// �����ς݂̃��[�v�o�b�t�@���N���A�iAddNewHeader�Ŋm�ۂ������́j
					if (pWaveHdr->lpData) {
						delete[] pWaveHdr->lpData;
						pWaveHdr->lpData = NULL;
					}
				}

				if (!pWaveOut->m_bPlayReset) {	// �K�{
					if ( !pWaveOut->AddNewHeader(hwo) ) {
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
		strErr.Format(_T("waveOutProc error:%s\n"), szCause);
		AfxMessageBox(strErr);
    }
	END_CATCH_ALL
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWaveOut::CWaveOut() : m_hWaveOut(0)
{
	InitListOfHeader();
	m_PlayType = CWaveOut::WAV;
	m_blVac = FALSE;
	m_pPlaySound1 = NULL;
	m_blPlayDev1 = FALSE;
	m_blStatus = false;
	m_bPlayReset = false;
}

//////////////////////////////////////////////////////////////////////
CWaveOut::~CWaveOut()
{
	Close();
	FreeListOfHeader();
}

//////////////////////////////////////////////////////////////////////
// Initialisation
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
void CWaveOut::SetDevice(const CWaveDevice &aDevice)
{
	m_waveDevice = aDevice;
}

//////////////////////////////////////////////////////////////////////
void CWaveOut::SetWaveFormat(WAVEFORMATEX tagFormat)
{
	m_wave.BuildFormat(tagFormat.nChannels, tagFormat.nSamplesPerSec, tagFormat.wBitsPerSample, m_blVac);
}

// ���A���^�C���Đ��p�t�H�[�}�b�g�w��
void CWaveOut::SetPlayFormat(WORD nChannels, DWORD nSamplesPerSec, WORD wBitsPerSample)
{
	m_PlayFmt = m_wave.GetFormat();
	m_PlayFmt.nChannels = nChannels; 
	m_PlayFmt.nSamplesPerSec = nSamplesPerSec; 
	m_PlayFmt.wBitsPerSample = wBitsPerSample;
}

//////////////////////////////////////////////////////////////////////
void CWaveOut::InitListOfHeader()
{
	for (int i = 0; i < NUMWAVEOUTHDR; i++) {
//		delete[] m_tagWaveHdr[i].lpData;
		m_tagWaveHdr[i].lpData = NULL;
		ZeroMemory(m_tagWaveHdr + i, sizeof(WAVEHDR));
	}
}

//////////////////////////////////////////////////////////////////////
// Son
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
bool CWaveOut::Close()
{
	bool ret = true;

	if (m_hWaveOut != NULL) {
		if (Stop()) {
			if (IsError( waveOutClose(m_hWaveOut))) {
				ret = false;
			}
		}
		m_hWaveOut = 0;
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

	return ret;
}

//////////////////////////////////////////////////////////////////////
bool CWaveOut::Continue()
{
	if (m_hWaveOut) {
		return !IsError( waveOutRestart(m_hWaveOut) );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// �o�̓f�o�C�X�����A���̏ꍇ�v���C�o�b�N��VAC
// �o�̓f�o�C�X��VAC�̏ꍇ�v���C�o�b�N�̓��A��
bool CWaveOut::Open(
PlayType type)		// �Đ��^�C�v
{
	/// �Đ��^�C�v
	m_PlayType = type;

	/// ���A���^�C���Đ��̊J�n
	if (m_blPlayDev1) {
		m_pPlaySound1 = new CPlaySound();
		m_pPlaySound1->CreateThread();
		m_pPlaySound1->SetDevice(m_strPlayDev1);
		m_pPlaySound1->SetFormat(m_PlayFmt.nChannels, m_PlayFmt.nSamplesPerSec, m_PlayFmt.wBitsPerSample, !m_blVac);
		m_pPlaySound1->PostThreadMessage(WM_PLAYSOUND_STARTPLAYING, 0, 0);				
	}

	/// �o�̓f�o�C�X�I�[�v��
	MMRESULT r = waveOutOpen(&m_hWaveOut, m_waveDevice.GetDevice(), &(m_wave.m_vacWaveFormat.Fmt), (DWORD)waveOutProc, NULL, CALLBACK_FUNCTION);
	if (IsError(r)) {
		m_wave.FreeListOfBuffer();
		m_oAcmMgr.FreeListOfBuffer();	
		return false;
	} else {
		return true;
	}
}

//////////////////////////////////////////////////////////////////////
bool CWaveOut::Pause()
{
	if (m_hWaveOut) {
		return !IsError( waveOutPause(m_hWaveOut) );
	}
	return true;
}

bool CWaveOut::Play(DWORD dwStart/*=-1*/, DWORD dwEnd/*=-1*/)
{
	/// �t�@�C����ǂ݂̂��߂̑ҋ@
	// �ŏ��̃o�b�t�@���m�ۂ����̂�҂�(max:30sec)
	int i = 300;
	while(i--) {
		Sleep(100);
		if (m_PlayType == CWaveOut::MP3) {
			if (m_oAcmMgr.GetBuffer() != NULL) break;
		} else {
			if (m_wave.GetBuffer() != NULL) break;
		}
	}

	/// ���[�v�o�b�t�@�̐ݒ�
	m_nIndexWaveHdr = NUMWAVEOUTHDR - 1;	// AddNewHeader�Ń��[�v����iNUMWAVEOUTHDR - 1�j���O
	m_blStatus = false;
	m_bPlayReset = false;
	for (i = 0; i < NUMWAVEOUTHDR - 2; i++) {	// ��s���čĐ��J�n�i�����̓R�[���o�b�N�Łj
		if ( !AddNewHeader(m_hWaveOut) ) {
			return false;
		}
	}

	return true;
}

bool CWaveOut::Stop()
{
	// �t�@�C�����[�h�X���b�h�̒�~���w��
	if (m_PlayType == CWaveOut::MP3) {
		m_oAcmMgr.ReadStop();		
	} else {
		m_wave.StopLoad();		
	}
	if (m_hWaveOut != NULL) {
		m_bPlayReset = true;
		Sleep(500);		
		if ( IsError(waveOutReset(m_hWaveOut)) ) {
			return false;
		}
	}
	return true;
}

bool CWaveOut::AddNewHeader(HWAVEOUT hwo)
{
	bool blRet = false;
	DWORD dwBufLen = 0;
	CWaveBuffer *pWavBuf = NULL;

	if (m_blStatus) return true;
	m_blStatus = true;

	TRY {
		m_nIndexWaveHdr = (m_nIndexWaveHdr == NUMWAVEOUTHDR - 1) ? 0 : m_nIndexWaveHdr + 1;

		if (m_PlayType == CWaveOut::MP3) {
			pWavBuf = m_oAcmMgr.GetBuffer();
		} else {
			pWavBuf = m_wave.GetBuffer();
		}
		if (!pWavBuf) goto AddNewHeader_END;

		char *pBuf = (char *)pWavBuf->GetBuffer();
		dwBufLen = pWavBuf->GetNumSamples() * pWavBuf->GetSampleSize();		// �t�@�C���̖����ł͏������Ȃ�
		if (m_tagWaveHdr[m_nIndexWaveHdr].lpData == NULL) {
			m_tagWaveHdr[m_nIndexWaveHdr].lpData = new char[dwBufLen];
		}
		memcpy(m_tagWaveHdr[m_nIndexWaveHdr].lpData, (char*)pBuf, dwBufLen);

		if (m_PlayType == CWaveOut::MP3) {
			blRet = m_oAcmMgr.DelBuffer();
		} else {
			blRet = m_wave.DelBuffer();
		}
		if (!blRet) {
			delete[] m_tagWaveHdr[m_nIndexWaveHdr].lpData;
			m_tagWaveHdr[m_nIndexWaveHdr].lpData = NULL;
			goto AddNewHeader_END;
		}
		m_tagWaveHdr[m_nIndexWaveHdr].dwBufferLength = dwBufLen;
		m_tagWaveHdr[m_nIndexWaveHdr].dwFlags = 0;
		m_tagWaveHdr[m_nIndexWaveHdr].dwUser = (DWORD)(void*)this;
		if ( IsError(waveOutPrepareHeader(hwo, &m_tagWaveHdr[m_nIndexWaveHdr], sizeof(WAVEHDR))) ) {
			goto AddNewHeader_END;
		}

		/// ���A���^�C���Đ��p�Ƀo�b�t�@���R�s�[
		// �o�b�t�@�̊J����PlaySound�Ŕ񓯊��ɍs���邽�߁A
		// pWriteHdr�̓N���X�����o�̂悤�ȌŒ�A�h���X�������Ă͂����Ȃ�
		WAVEHDR* pWaveHdr = NULL;
		WAVEHDR* pWriteHdr1 = NULL;
		if (m_blPlayDev1 && m_pPlaySound1) {
			pWriteHdr1 = new WAVEHDR;
			if(pWriteHdr1) {
				pWaveHdr = &m_tagWaveHdr[m_nIndexWaveHdr];
				memcpy(pWriteHdr1, pWaveHdr, sizeof(WAVEHDR));
				BYTE * pSound1 = new BYTE[pWaveHdr->dwBufferLength];
				if(pSound1) {
					memcpy(pSound1, pWaveHdr->lpData, pWaveHdr->dwBufferLength);
					pWriteHdr1->lpData = (char*)pSound1;
				} else {
					if(pWriteHdr1) delete pWriteHdr1;
					pWriteHdr1 = NULL;
				}
			}
	//TRACE(_T("new1=%d dev=%s\n"), ++inew1, m_strPlayDev1);
		}

		/// �ʏ�̍Đ�
		if ( IsError(waveOutWrite(hwo, &m_tagWaveHdr[m_nIndexWaveHdr], sizeof(WAVEHDR))) ) { 
			waveOutUnprepareHeader( hwo, &m_tagWaveHdr[m_nIndexWaveHdr], sizeof(WAVEHDR) );
			if (m_tagWaveHdr[m_nIndexWaveHdr].lpData) {
				delete[] m_tagWaveHdr[m_nIndexWaveHdr].lpData;
				m_tagWaveHdr[m_nIndexWaveHdr].lpData = NULL;
			}
			m_tagWaveHdr[m_nIndexWaveHdr].dwBufferLength = 0;
			m_tagWaveHdr[m_nIndexWaveHdr].dwFlags = 0;
			m_tagWaveHdr[m_nIndexWaveHdr].dwUser = NULL;
			m_nIndexWaveHdr--;
			if (m_blPlayDev1 && m_pPlaySound1 && pWriteHdr1) {
				if (pWriteHdr1->lpData) {
					delete[] pWriteHdr1->lpData;
				}
				if (pWriteHdr1) {
					delete pWriteHdr1;
					pWriteHdr1 = NULL;
				}
			} 
			goto AddNewHeader_END;
		}

		/// ���A���^�C���Đ��p�X���b�h�ɍĐ��w��
		// �o�b�t�@�̓��A���^�C���Đ��p�X���b�h�ŏ���
		if (m_blPlayDev1 && m_pPlaySound1 && pWriteHdr1) {
			if (!m_pPlaySound1->PostThreadMessage(WM_PLAYSOUND_PLAYBLOCK, GetCurrentThreadId(),(LPARAM)pWriteHdr1)) {
				if (pWriteHdr1->lpData) {
					delete[] pWriteHdr1->lpData;
					pWriteHdr1->lpData = NULL;
				}
				if (pWriteHdr1) {
					delete pWriteHdr1;
					pWriteHdr1 = NULL;
				}
			}
		} 

	}
    CATCH_ALL(e)
    {
		CString strErr;
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		strErr.Format(_T("AddNewHeader error:%s\n"), szCause);
		AfxMessageBox(strErr);
		goto AddNewHeader_END;
    }
	END_CATCH_ALL

	blRet = true;
AddNewHeader_END:
	m_blStatus = false;
	return blRet;
}
//////////////////////////////////////////////////////////////////////
void CWaveOut::FreeListOfHeader()
{
	for (int i = 0; i < NUMWAVEOUTHDR; i++) {
		if (m_tagWaveHdr[i].lpData) {
			delete[] m_tagWaveHdr[i].lpData;
			m_tagWaveHdr[i].lpData = NULL;
		}
		ZeroMemory(m_tagWaveHdr + i, sizeof(WAVEHDR));
	}
}

//////////////////////////////////////////////////////////////////////
// GET
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
CString CWaveOut::GetError() const
{
	if (m_nError != MMSYSERR_NOERROR) {
		TCHAR szText[MAXERRORLENGTH + 1];
		if ( waveOutGetErrorText(m_nError, szText, MAXERRORLENGTH) == MMSYSERR_NOERROR ) {
			return szText;
		}
	}
	return _T("");
}

//////////////////////////////////////////////////////////////////////
DWORD CWaveOut::GetPosition()
{
	if (m_hWaveOut) {
		MMTIME mmt;
		mmt.wType = TIME_SAMPLES;
		if ( IsError(waveOutGetPosition(m_hWaveOut, &mmt, sizeof(MMTIME))) ) {
		//	return -1;
			return 0;
		}
		else {
			return mmt.u.sample;
		}
	}
//	return -1;
	return 0;
}

//////////////////////////////////////////////////////////////////////
bool CWaveOut::IsError(MMRESULT nResult)
{
	m_nError = nResult;
	return (m_nError != MMSYSERR_NOERROR);
}

//////////////////////////////////////////////////////////////////////
bool CWaveOut::IsPlaying()
{
	bool bResult = false;
	DWORD dw;

	if (m_nIndexWaveHdr > -1) {
		dw = m_tagWaveHdr[m_nIndexWaveHdr].dwFlags;
		if (dw != 0) {
			bResult = !((dw & WHDR_DONE) == WHDR_DONE);
		}
	}
	return bResult;
}

