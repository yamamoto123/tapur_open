// DSCodecFormat.h: interface for the CDSCodecFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSCODECFORMAT_H__562E126F_C51A_402B_8F43_EDCEF9B526D8__INCLUDED_)
#define AFX_DSCODECFORMAT_H__562E126F_C51A_402B_8F43_EDCEF9B526D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dshow.h"

class CDSCodecFormat  
{
public:
	WORD BitsPerSample();
	DWORD BytesPerSec();
	DWORD SamplesPerSecond();
	WORD NumberOfChannels();
	CDSCodecFormat();
	virtual ~CDSCodecFormat();

public:
	AM_MEDIA_TYPE* m_pMediaType;
};

#endif // !defined(AFX_DSCODECFORMAT_H__562E126F_C51A_402B_8F43_EDCEF9B526D8__INCLUDED_)
