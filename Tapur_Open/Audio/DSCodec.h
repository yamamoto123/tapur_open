// DSCodec.h: interface for the CDSCodec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSCODEC_H__08AD6127_616C_4925_9BC6_61DB24964EF3__INCLUDED_)
#define AFX_DSCODEC_H__08AD6127_616C_4925_9BC6_61DB24964EF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "dshow.h"
#include "DSCodecFormat.h"

class CDSCodec : public CArray<CDSCodecFormat*, CDSCodecFormat*>
{
public:
	CDSCodec();
	virtual ~CDSCodec();

	CString m_szCodecName;
	IMoniker	*m_pMoniker;
	void BuildCodecFormatArray();
};

#endif // !defined(AFX_DSCODEC_H__08AD6127_616C_4925_9BC6_61DB24964EF3__INCLUDED_)
