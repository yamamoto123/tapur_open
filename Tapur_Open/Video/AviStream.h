// AviStream.h : ヘッダー ファイル
//

#if !defined(AVISTREAM_H__2A51C317_11C8_4D0E_ACDE_8DBA3F4FFEF8__INCLUDED_)
#define AVISTREAM_H__2A51C317_11C8_4D0E_ACDE_8DBA3F4FFEF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vfw.h>

/////////////////////////////////////////////////////////////////////////////
// CAviStream 

class CAviStream
{
protected:
	PAVIFILE m_pAviFile;
	PAVISTREAM m_pAviStream;
	PAVISTREAM m_pCompressedStream;
	bool m_writeCompressed;

public:
	CAviStream() { m_writeCompressed = false; }
    /// <summary>Close the stream</summary>
    virtual void Close(){
		if(m_writeCompressed){
			AVIStreamRelease(m_pCompressedStream);
		}
		AVIStreamRelease(m_pAviStream);
	}

    PAVIFILE FilePointer() { return m_pAviFile; }
    PAVISTREAM StreamPointer() { return m_pAviStream; }
    bool WriteCompressed() { return m_writeCompressed; }

	virtual void ExportStream(CString fileName) { return;}	
};


#endif // !defined(AVISTREAM_H__2A51C317_11C8_4D0E_ACDE_8DBA3F4FFEF8__INCLUDED_)
