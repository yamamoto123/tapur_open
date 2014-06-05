// DSEncoder.h: interface for the CDSEncoder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSENCODER_H__598FE0B2_9419_4E96_A5A9_FDF6E6480EBF__INCLUDED_)
#define AFX_DSENCODER_H__598FE0B2_9419_4E96_A5A9_FDF6E6480EBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dshow.h"
#include "DSCodec.h"

class CDSEncoder : public CArray<CDSCodec*, CDSCodec*>
{
public:
	int BuildGraph(CWnd *hWin, CString szSrcFileName, CString szDestFileName, int nCodec, int nFormat);
	CDSEncoder();
	virtual ~CDSEncoder();

protected:
	void BuildCodecArray();
	HRESULT AddFilterByClsid(IGraphBuilder *pGraph, LPCWSTR wszName, const GUID& clsid, IBaseFilter **ppF);
	BOOL SetFilterFormat(AM_MEDIA_TYPE* pStreamFormat, IBaseFilter* pBaseFilter);

	IGraphBuilder *m_pGraphBuilder;
};

#endif // !defined(AFX_DSENCODER_H__598FE0B2_9419_4E96_A5A9_FDF6E6480EBF__INCLUDED_)
