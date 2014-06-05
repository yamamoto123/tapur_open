// WaveInterface.cpp: implementation of the CWaveInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <mmsystem.h>
#include "WaveInterface.h"

// memory leak åüèo
#ifdef	__AFX_H__            // MFCÇÃÇ›
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
UINT CWaveInterface::GetWaveInCount()
{
	return waveInGetNumDevs();	
}

//////////////////////////////////////////////////////////////////////
CString CWaveInterface::GetWaveInName(UINT nIndex)
{
	ASSERT(nIndex < GetWaveInCount());
	WAVEINCAPS tagCaps;
	switch (waveInGetDevCaps(nIndex, &tagCaps, sizeof(tagCaps))) {
	case MMSYSERR_NOERROR:
		return tagCaps.szPname;
		break;
	default:
		return _T("");
	}
}

//////////////////////////////////////////////////////////////////////
UINT CWaveInterface::GetWaveOutCount()
{
	return waveOutGetNumDevs();	
}

//////////////////////////////////////////////////////////////////////
CString CWaveInterface::GetWaveOutName(UINT nIndex)
{
	ASSERT(nIndex < GetWaveOutCount());
	WAVEOUTCAPS tagCaps;
	switch (waveOutGetDevCaps(nIndex, &tagCaps, sizeof(tagCaps))) {
	case MMSYSERR_NOERROR:
		return tagCaps.szPname;
		break;
	default:
		return _T("");
	}
}
