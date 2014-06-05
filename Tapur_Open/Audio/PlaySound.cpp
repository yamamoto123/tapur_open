// PlaySound.cpp : implementation file
//
#include "stdafx.h"
#include "winbase.h"
#include <mmsystem.h>
#include <math.h>
#include "PlaySound.h"
#include "WaveInterface.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

// VAC
static GUID const &OpenSig = __uuidof (OpenSigGuid);
// VAC

#ifdef _DEBUG
int idel = 0;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlaySound

IMPLEMENT_DYNCREATE(CPlaySound, CWinThread)

#define BLOCKSAHEAD 3

CPlaySound::CPlaySound()
{
	m_nWaitBuffers = 0;
	//m_nOutputBuffers = 0;
	//m_nEndBuffers = 0;
	m_pSemaphore = new CSemaphore(BLOCKSAHEAD,BLOCKSAHEAD);
	m_bPlay = FALSE;
}

CPlaySound::~CPlaySound()
{
	if(m_pSemaphore)
		delete m_pSemaphore;
}

BOOL CPlaySound::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CPlaySound::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPlaySound, CWinThread)
	//{{AFX_MSG_MAP(CPlaySound)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_PLAYSOUND_STARTPLAYING, OnStartPlaying)
	ON_THREAD_MESSAGE(WM_PLAYSOUND_STOPPLAYING, OnStopPlaying)
	ON_THREAD_MESSAGE(WM_PLAYSOUND_PLAYBLOCK, OnWriteSoundData)
	ON_THREAD_MESSAGE(MM_WOM_DONE, OnEndPlaySoundData)
	ON_THREAD_MESSAGE(WM_PLAYSOUND_ENDTHREAD,OnEndThread)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

//
//  機能     : デバイスの設定
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 該当がない場合は初期設定（WAVE_WRAPPER）のまま
//  
BOOL CPlaySound::SetDevice(
CString strDev)	// 入力デバイス名
{
	UINT i = 0;
	CString strWavDevice;

	try {
		m_nDevice = WAVE_MAPPER;
		if (strDev.IsEmpty()) return TRUE;

		for (i = 0; i < CWaveInterface::GetWaveOutCount(); i++) {
			strWavDevice = CWaveInterface::GetWaveOutName(i);
			if (!strWavDevice.IsEmpty()) {
				if (strWavDevice.CompareNoCase(strDev) == 0 ) {
					m_nDevice = i;
					break;
				}
			}
		}
	} catch(...) {
		return FALSE;
	}

	return TRUE;
}

void CPlaySound::SetFormat(
WORD nChannels,		// 1:monoral, 2:stereo
DWORD nFrequency,	// サンプリングレート
WORD nBits,			// 出力ビット
BOOL blVac)			// TRUE:VACモード, FALSE:ノーマルデバイス
{
	memset(&m_vacWaveFormat,0x00,sizeof(m_vacWaveFormat));
	m_vacWaveFormat.Fmt.nChannels = nChannels;
	m_vacWaveFormat.Fmt.nSamplesPerSec = nFrequency;
	m_vacWaveFormat.Fmt.nAvgBytesPerSec = nFrequency * nChannels * nBits / 8;
	m_vacWaveFormat.Fmt.nBlockAlign = nChannels * nBits / 8;
	m_vacWaveFormat.Fmt.wBitsPerSample = nBits;
	if (blVac) {
		m_vacWaveFormat.Fmt.wFormatTag = WAVE_FORMAT_NATIVE;
		m_vacWaveFormat.Fmt.cbSize = sizeof(GUID);
		m_vacWaveFormat.Key = OpenSig;
	} else {
		m_vacWaveFormat.Fmt.wFormatTag = WAVE_FORMAT_PCM;
		m_vacWaveFormat.Fmt.cbSize = 0;
	}
}

// CPlaySound message handlers
void CPlaySound::OnStartPlaying(WPARAM wParam, LPARAM lParam)
{
	if(m_bPlay) return;

	if(!m_bPlay)
	{
		// open wavein device
		MMRESULT mmReturn = ::waveOutOpen( &m_hPlay, m_nDevice,
			&(m_vacWaveFormat.Fmt), ::GetCurrentThreadId(), 0, CALLBACK_THREAD);
		if(mmReturn )
		{
			_TCHAR errorbuffer[MAX_PATH];
			_TCHAR errorbuffer1[MAX_PATH];
			waveOutGetErrorText( mmReturn, 
								errorbuffer,
								MAX_PATH);
			_stprintf_s(errorbuffer1, MAX_PATH, _T("WAVEOUT:%x:%s"),mmReturn,errorbuffer);
			AfxMessageBox(errorbuffer1);  

		}
			
		if(!mmReturn)
		{
			m_bPlay = TRUE;
		}

	}
	return;
}

void CPlaySound::OnStopPlaying(WPARAM wParam, LPARAM lParam)
{
	MMRESULT mmReturn = 0;
	if(!m_bPlay)
		return;

	if(m_bPlay)
	{
		MSG msg;
		mmReturn = ::waveOutReset(m_hPlay);
		Sleep(20);
		while(::PeekMessage(&msg,NULL,MM_WOM_OPEN,MM_WOM_DONE,
			PM_REMOVE))
		{
			if(msg.message == MM_WOM_OPEN ||
				msg.message == MM_WOM_CLOSE)
				continue;
			
			OnEndPlaySoundData(msg.wParam,msg.lParam);
			Sleep(5);
		}
		
		if(!mmReturn)
			m_bPlay = FALSE;
		Sleep(500);
		if(!mmReturn)
			mmReturn = ::waveOutClose(m_hPlay);
		return;
	}
	return;
}

void CPlaySound::OnEndPlaySoundData(WPARAM wParam, LPARAM lParam)
{
	MMRESULT mmResult = 0;
	LPWAVEHDR lpHdr = (LPWAVEHDR) lParam;

	if(lpHdr)
	{
		// 出力処理待ちバッファ数の更新
		m_nWaitBuffers--;

		//m_nEndBuffers++;
		//TRACE1("SOUND BUFFER end:%d\n", m_nEndBuffers);

		mmResult =::waveOutUnprepareHeader(m_hPlay, lpHdr, sizeof(WAVEHDR));
		if(mmResult) {
			TRACE(_T("error from waveOutUnprepareHeader\n"));
		}
	
		if(lpHdr->lpData) {
			delete ((BYTE*) lpHdr->lpData);
			lpHdr->lpData = NULL;
		}
		if(lpHdr) {
			delete lpHdr;
			lpHdr = NULL;
		}
//TRACE(_T("del=%d dev=%d\n"), ++idel, m_nDevice);
		m_pSemaphore->Unlock();
	}
	return;
}

void CPlaySound::OnWriteSoundData(WPARAM wParam, LPARAM lParam)
{
	LPWAVEHDR lpHdr = (LPWAVEHDR) lParam;
	MMRESULT mmResult = 0;

	if(lpHdr)
	{
		if(m_bPlay)
		{
			BYTE* lpInt = (BYTE*) lpHdr->lpData;
			DWORD dwSamples = lpHdr->dwBufferLength/sizeof(BYTE);

			// 出力処理待ちバッファが溜まっている場合、バッファを破棄する
			// 音声の遅延を避けるため。遅いPCではノイズになる可能性がある。
			//TRACE1("SOUND BUFFER wait:%d\n", m_nWaitBuffers);
			if (m_nWaitBuffers > 2) return;

			//ProcessSoundData(lpInt, dwSamples);

			mmResult = ::waveOutPrepareHeader(m_hPlay, lpHdr, sizeof(WAVEHDR));
			if(mmResult) {
				TRACE(_T("error from waveOutPrepareHeader\n"));
				return;
			}
			
			mmResult = ::waveOutWrite(m_hPlay, lpHdr, sizeof(WAVEHDR));
			if(mmResult) {
				::waveOutUnprepareHeader(m_hPlay, lpHdr, sizeof(WAVEHDR));
				TRACE(_T("error from waveoutwrite\n"));
			}

			// 出力処理待ちバッファ数の更新
			m_nWaitBuffers++;

			//m_nOutputBuffers++;
			//TRACE1("SOUND BUFFER written:%d\n", m_nOutputBuffers);
		}
	}

	return;
}

LPWAVEHDR CPlaySound::CreateWaveHeader()
{
	LPWAVEHDR lpHdr = new WAVEHDR;
	ZeroMemory(lpHdr, sizeof(WAVEHDR));
	BYTE* lpByte = new BYTE[(m_vacWaveFormat.Fmt.nBlockAlign*SOUNDSAMPLES)];

	lpHdr->lpData = (char *) lpByte;
	lpHdr->dwBufferLength = (m_vacWaveFormat.Fmt.nBlockAlign*SOUNDSAMPLES);
	return lpHdr;

}

//void CPlaySound::ProcessSoundData(BYTE *sound, DWORD dwSamples)
//{
//}

void CPlaySound::OnEndThread(WPARAM wParam, LPARAM lParam)
{
	if(m_bPlay)
		OnStopPlaying(0,0);
	::PostQuitMessage(0);
	return;
}