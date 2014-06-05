 // WaveBuffer.cpp: implementation of the CWaveBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveBuffer.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
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
CWaveBuffer::CWaveBuffer() : m_dwNum(0), m_pBuffer(NULL), m_nSampleSize(0)
{
	m_blActive = FALSE;
	m_blDead = FALSE;
}

//////////////////////////////////////////////////////////////////////
CWaveBuffer::~CWaveBuffer()
{
	m_blActive = FALSE;
	m_blDead = FALSE;
	m_dwNum = 0L;
	if (m_pBuffer) {
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
void* CWaveBuffer::GetBuffer() const
{
	return m_pBuffer;
}

//////////////////////////////////////////////////////////////////////
DWORD CWaveBuffer::GetNumSamples() const
{
	return m_dwNum;
}

//////////////////////////////////////////////////////////////////////
void CWaveBuffer::CopyBuffer(void* pBuffer, DWORD dwNumSamples, int nSize)
{
	ASSERT(dwNumSamples >= 0);
	ASSERT(nSize);

	if (!m_pBuffer)
		SetNumSamples(dwNumSamples, nSize);

	if (__min(m_dwNum, dwNumSamples) * nSize > 0) {
		ZeroMemory(m_pBuffer, m_dwNum * m_nSampleSize);
		CopyMemory(m_pBuffer, pBuffer, __min(m_dwNum, dwNumSamples) * nSize);
	}
}

//////////////////////////////////////////////////////////////////////
// バッファの確保
void CWaveBuffer::SetNumSamples(DWORD dwNumSamples, int nSize)
{
	ASSERT(dwNumSamples >= 0);
	ASSERT(nSize > 0);

	void* pBuffer = NULL;

	pBuffer = new char[nSize * dwNumSamples];
	SetBuffer(pBuffer, dwNumSamples, nSize);
}

//////////////////////////////////////////////////////////////////////
void CWaveBuffer::SetBuffer(void *pBuffer, DWORD dwNumSamples, int nSize)
{
//	ASSERT(dwNumSamples >= 0);
//	ASSERT(nSize);

	delete[] m_pBuffer;
	m_pBuffer = pBuffer;
	m_dwNum = dwNumSamples;
	m_nSampleSize = nSize;
}

//////////////////////////////////////////////////////////////////////
int CWaveBuffer::GetSampleSize() const
{
	return m_nSampleSize;
}
