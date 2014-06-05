// "winmm.lib" and "msacm32.lib" must be imported

#ifndef	WAVEFILEACM_H
#define	WAVEFILEACM_H

#include <mmsystem.h>
#include <mmreg.h>
#include <msacm.h>
#include <WinDef.h>		// paak

class CWaveFileACM
{
public:
	CWaveFileACM();
	virtual ~CWaveFileACM();
	BOOL Open(const TCHAR *);
	BOOL Create(const TCHAR *);
	BOOL FormatChooseDlg(HWND=NULL);
	BOOL FormatChoose(int,int,int);
	void FormatSet(const WAVEFORMATEX *);
	BOOL FormatSet(int,int,int);
	void Close(void);
	int Read(void *,int);
	int Write(const void *,int);
	int Seek(unsigned int);
// paak
	TCHAR FileName[MAX_PATH];
	BOOL FlagW;
	unsigned int dataCP,dataEP;
	WAVEFORMATEX wfex;
protected:
	HMMIO hmmio;
	MMCKINFO ckRIFF,ckfmt,ckdata,ckfact;
	LPWAVEFORMATEX pwfex1,pwfex2;
	HACMSTREAM hacms1,hacms2;
	ACMSTREAMHEADER acmsh1,acmsh2;
};

#endif	// WAVEFILEACM_H
