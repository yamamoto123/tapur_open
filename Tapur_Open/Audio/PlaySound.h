#if !defined(AFX_PLAYSOUND_H__5260C01C_03B2_11D2_A421_FC4B2C882A60__INCLUDED_)
#define AFX_PLAYSOUND_H__5260C01C_03B2_11D2_A421_FC4B2C882A60__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PlaySound.h : header file
#include <afxmt.h>
#include "Wave.h"
//

#ifndef __COMPLEX__
#define __COMPLEX__
typedef struct 
{
    float real; 
	float imag;
} 
COMPLEXNUMBER;
#endif

#define SOUNDSAMPLES 1000


/////////////////////////////////////////////////////////////////////////////
// CPlaySound thread
#define WM_PLAYSOUND_STARTPLAYING WM_USER+600
#define WM_PLAYSOUND_STOPPLAYING WM_USER+601
#define WM_PLAYSOUND_PLAYBLOCK WM_USER+602
#define WM_PLAYSOUND_ENDTHREAD  WM_USER+603


class CRecordDlg;

class CPlaySound : public CWinThread
{
	DECLARE_DYNCREATE(CPlaySound)
public:
	CPlaySound();// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlaySound)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	BOOL SetDevice(CString strDev);		// デバイスの設定
	void SetFormat(WORD nChannels, DWORD nFrequency, WORD nBits, BOOL blVac);
//	virtual void ProcessSoundData(BYTE *sound, DWORD dwSamples);

// Implementation
protected:
	virtual ~CPlaySound();

	// Generated message map functions
	//{{AFX_MSG(CPlaySound)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	afx_msg void OnStartPlaying(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStopPlaying(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEndPlaySoundData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnWriteSoundData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEndThread(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	long m_nWaitBuffers;	// 出力処理待ちバッファ数
	//long m_nOutputBuffers;
	//long m_nEndBuffers;
	VACFORMAT m_vacWaveFormat;
	//WAVEFORMATEX m_WaveFormatEx;
	BOOL m_bPlay;
	HWAVEOUT m_hPlay;
	UINT m_nDevice;		// 使用デバイス

public:
	CSemaphore* m_pSemaphore;
	
protected:

LPWAVEHDR CreateWaveHeader();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYSOUND_H__5260C01C_03B2_11D2_A421_FC4B2C882A60__INCLUDED_)
