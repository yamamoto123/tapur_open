// OggViorbis.h 
//
// Ogg Vorbis encoder

#if !defined(_OGGVORBIS_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
#define _OGGVORBIS_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include "vorbis/vorbisenc.h"


class COggVorbis
{
// Construction
public:
	COggVorbis();	
	~COggVorbis();	

// Implementation
private:

public:
	int Encode(CWnd *hWin, CString strWaveFile, CString strOggFile, double dblQualityBase = 0.4);
};

#endif // !defined(_OGGVORBIS_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
