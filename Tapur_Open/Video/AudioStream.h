// AudioStream.h : ヘッダー ファイル
//

#if !defined(AUDIOSTREAM_H__2A51C317_11C8_4D0E_ACDE_8DBA3F4FFEF8__INCLUDED_)
#define AUDIOSTREAM_H__2A51C317_11C8_4D0E_ACDE_8DBA3F4FFEF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vfw.h>
#include "AviStream.h"

/////////////////////////////////////////////////////////////////////////////
// CAudioStream 

class CAudioStream : public CAviStream
{
private:
	PCMWAVEFORMAT waveFormat;
	AVISTREAMINFO streamInfo;

	AVISTREAMINFO GetStreamInfo(LPVOID aviStream);
	
public:
	CAudioStream(PAVIFILE pAviFile, PAVISTREAM pAviStream);

	int CountBitsPerSample() { return waveFormat.wBitsPerSample; }
	int CountSamplesPerSecond() { return waveFormat.wf.nSamplesPerSec; }
	int CountChannels() { return waveFormat.wf.nChannels; }
	AVISTREAMINFO GetStreamInfo(PAVISTREAM pAviStream);
	AVISTREAMINFO GetStreamInfo();
	PCMWAVEFORMAT GetFormat();
	LPVOID GetStreamData(AVISTREAMINFO &streamInfo, PCMWAVEFORMAT &format, int &streamLength);
//	void ExportStream(CString fileName);
};


#endif // !defined(AUDIOSTREAM_H__2A51C317_11C8_4D0E_ACDE_8DBA3F4FFEF8__INCLUDED_)
