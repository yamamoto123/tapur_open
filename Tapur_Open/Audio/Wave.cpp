// Wave.cpp: implementation of the CWave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Wave.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

// VAC
static GUID const &OpenSig = __uuidof (OpenSigGuid);
// VAC

//////////////////////////////////////////////////////////////////////

// �t�@�C�����[�h�X���b�h����֐�
UINT WaveWorkerThread( LPVOID pParam )
{
	CWave* pWave = (CWave*)pParam;

	CFile fin;
	CFileException e;
	LPBYTE pSrcBuf = NULL;	// �ϊ����o�b�t�@
	DWORD cbSrcBuf = WAVE_BUFFER_SIZE;	// �o�b�t�@�T�C�Y

	/// Validation
	if (pWave->m_strLoadFile.GetLength() == 0) goto THREAD_END;
	
	/// �o�b�t�@�̊m��
	pSrcBuf = new BYTE[cbSrcBuf];

	/// ���̓t�@�C���I�[�v��
	if( !fin.Open(pWave->m_strLoadFile, CFile::modeRead | CFile::shareDenyWrite, &e ) ) {
		_stprintf_s(pWave->te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s\n"), pWave->m_strLoadFile, e.m_cause);
		pWave->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, pWave->te.errstr);
		goto THREAD_END;
	}

	TRY {
		/// �w�b�_�̃X�L�b�v
		if (fin.Read(pSrcBuf, 44) <= 0) {
			goto THREAD_END;
		}

		/// �f�[�^�̓ǂݍ���
		long lngCnt = 0;
		int intRead = 0;
		while(pWave->m_blLoadSw) {
			// �ǂݍ��݂���������ƃ��������������邽�߁A�����҂�����
			lngCnt = (long)pWave->m_listOfBuffer.GetCount();
			if (lngCnt > 10) {
				Sleep(10);  //CPU���׌���
				continue;
			}

			intRead = fin.Read(pSrcBuf, cbSrcBuf);
			if (intRead == 0) {
				break;
			} else {
				pWave->AddNewBuffer(pSrcBuf, intRead);
				if (intRead < (int)cbSrcBuf) {
					break;
				} 
			}
		} //while

		/// �t�@�C����Close
		fin.Close();
	}
    CATCH(CFileException, e)
    {
		fin.Close();
		_stprintf_s(pWave->te.errstr, MAX_ERRSTR, _T("File read error:%s, %s\n"), pWave->m_strLoadFile, e->m_cause);
		pWave->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, pWave->te.errstr);
		goto THREAD_END;
	}
    AND_CATCH_ALL(e)
    {
		fin.Close();
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(pWave->te.errstr, MAX_ERRSTR, _T("Read error:%s, %s\n"), pWave->m_strLoadFile, szCause);
		pWave->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, pWave->te.errstr);
		goto THREAD_END;
    }
	END_CATCH_ALL

THREAD_END:
	// �o�̓o�b�t�@�̉��
	if (pSrcBuf) delete [] pSrcBuf;

	return 0;
};
//UINT WaveWorkerThread( LPVOID pParam )
//{
//	CWave* pWave = (CWave*)pParam;
//	CTPerror te;
//
//	_TCHAR *pStrFile = pWave->m_strLoadFile;
//	if (_tcslen(pStrFile) == 0) {
//		return 0;
//	}
//
//	/// �w�b�_��SEEK
//	CFile f;
//	CFileException e;
//	if( !f.Open(pStrFile, CFile::modeRead | CFile::shareDenyNone, &e ) ) {
//		_stprintf_s(te.errstr, MAX_ERRSTR, _T("File could not be opened:%s, %s"), pStrFile, e.m_cause);
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
//		return 0;
//	}
//
//	TRY {
//		DWORD dwPos = pWave->m_dwHeaderSize;	// seek �J�n�ʒu
//		f.Seek(dwPos, CFile::begin);	// �w�b�_��
//
//		/// �g�`�f�[�^�T�C�Y
//		DWORD dwNum;
//		f.Read(&dwNum, sizeof(dwNum)) ;
//		if (dwNum == 0) {
//			f.Close();
//			return 0;
//		}
//
//		/// �g�`�f�[�^���[�h
//		pWave->m_dwDataSize = dwNum / pWave->GetFormat().nBlockAlign * pWave->GetFormat().nBlockAlign;
//		while(pWave->m_blLoadSw) {
//			if (pWave->m_listOfBuffer.GetCount() > 100) {
//				Sleep(10);  //CPU���׌���
//				continue;
//			}
//			CWaveBuffer *pBuf = new CWaveBuffer;
//			pBuf->SetNumSamples(1, WAVE_BUFFER_SIZE);
//			pWave->m_listOfBuffer.AddTail(pBuf);
//			if (dwNum <= WAVE_BUFFER_SIZE) {
//				f.Read((char*)pBuf->GetBuffer(), dwNum);
//				break;
//			}
//			f.Read((char*)pBuf->GetBuffer(), WAVE_BUFFER_SIZE);
//
//			dwNum -= WAVE_BUFFER_SIZE;
//			if (dwNum <= 0) break;
//		}
//	}
//    CATCH(CFileException, e)
//    {
//		f.Close();
//		_stprintf_s(te.errstr, MAX_ERRSTR, _T("File read error:%s, %s"), pStrFile, e->m_cause);
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
//		return 0;
//	}
//    AND_CATCH_ALL(e)
//    {
//		f.Close();
//		_TCHAR   szCause[1024];
// 		e->GetErrorMessage(szCause, 1024);
//		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Record error:%s, %s"), pStrFile, szCause);
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
//		return 0;
//    }
//	END_CATCH_ALL
//
//	f.Close();
//	return 0;
//};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//IMPLEMENT_SERIAL(CWave, CObject, 1)


//////////////////////////////////////////////////////////////////////
// �R���X�g���N�^
CWave::CWave()
{
	m_blLoadSw = FALSE; 
	FreeListOfBuffer();
	ZeroMemory((void*)&m_vacWaveFormat, sizeof(m_vacWaveFormat));
//	m_vacWaveFormat.Fmt.wFormatTag = WAVE_FORMAT_NATIVE;
}

//////////////////////////////////////////////////////////////////////
// �R���X�g���N�^
CWave::CWave(const CWave &copy)
{
	m_blLoadSw = FALSE; 
	FreeListOfBuffer();
	m_vacWaveFormat.Fmt = copy.GetFormat();
//	m_buffer.SetNumSamples( copy.GetNumSamples(), copy.GetFormat().nBlockAlign ) ;
//	m_buffer.CopyBuffer( copy.GetBuffer(), copy.GetNumSamples(), copy.GetFormat().nBlockAlign );
}

//////////////////////////////////////////////////////////////////////
// �f�X�g���N�^
CWave::~CWave()
{
	m_blLoadSw = FALSE; 
	FreeListOfBuffer();
}

//////////////////////////////////////////////////////////////////////
// �R�s�[�I�y���[�^
CWave& CWave::operator =(const CWave& wave)
{
	if (&wave != this) {
		m_vacWaveFormat.Fmt = wave.GetFormat();
//		m_buffer.SetNumSamples( wave.GetNumSamples(), wave.GetFormat().nBlockAlign );
//		m_buffer.CopyBuffer( wave.GetBuffer(), wave.GetNumSamples(), wave.GetFormat().nBlockAlign );
	}
	return *this;
}


//////////////////////////////////////////////////////////////////////
//
//  �@�\     : �o�͗pWAVE�t�H�[�}�b�g����
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
void CWave::BuildFormat(
WORD nChannels,		// 1:monoral, 2:stereo
DWORD nFrequency,	// �T���v�����O���[�g
WORD nBits,			// �o�̓r�b�g
BOOL blVac)			// TRUE:VAC���[�h, FALSE:�m�[�}���f�o�C�X
{
	m_vacWaveFormat.Fmt.nChannels = nChannels;
	m_vacWaveFormat.Fmt.nSamplesPerSec = nFrequency;
	m_vacWaveFormat.Fmt.nAvgBytesPerSec = nFrequency * nChannels * nBits / 8;
	m_vacWaveFormat.Fmt.nBlockAlign = nChannels * nBits / 8;
	m_vacWaveFormat.Fmt.wBitsPerSample = nBits;
	if (blVac) {
		m_vacWaveFormat.Fmt.wFormatTag = WAVE_FORMAT_NATIVE;
		m_vacWaveFormat.Fmt.cbSize = sizeof(GUID);
		m_vacWaveFormat.Key = OpenSig;
	} else {
		m_vacWaveFormat.Fmt.wFormatTag = WAVE_FORMAT_PCM;
		m_vacWaveFormat.Fmt.cbSize = 0;
	}

//	m_buffer.SetNumSamples(0L, m_vacWaveFormat.Fmt.nBlockAlign);
}	

//////////////////////////////////////////////////////////////////////
inline WAVEFORMATEX CWave::GetFormat() const
{
	return m_vacWaveFormat.Fmt;
}

//////////////////////////////////////////////////////////////////////
/*
void CWave::Serialize( CArchive& archive )
{
	CFile* f = archive.GetFile();
	if (archive.IsLoading())
		Load(f, FALSE);
	else
		Save(f);
}
*/
//////////////////////////////////////////////////////////////////////
bool CWave::Load(CString strFile, BOOL blVac)
{
	if (LoadFormat(strFile, blVac) == false) return false;

	// ����ȃt�@�C���ǂݍ��݂͎��Ԃ������邽�߁A�X���b�h���i�Đ��̕����x�����ߖ��Ȃ��j
	m_strLoadFile = strFile;
	m_blLoadSw = TRUE;	// �t�@�C�����[�h�X���b�h�̊J�n���w��
	Sleep(500);		// �O��̃X���b�h���c���Ă���ꍇ������̂�
	FreeListOfBuffer();	// �o�b�t�@���N���A
	CWinThread* pThread = AfxBeginThread(WaveWorkerThread, this);

	return true;
}

bool CWave::LoadFormat(const CString &strFile, BOOL blVac)
{
	CFile f(strFile, CFile::modeRead | CFile::shareDenyNone);   
//	CFile f(strFile, CFile::modeRead); 
	bool ret = LoadFormat(&f, blVac);
	m_dwHeaderSize = (DWORD)f.GetPosition();
	f.Close();

	return ret;
}

//////////////////////////////////////////////////////////////////////
bool CWave::LoadFormat(CFile *f, BOOL blVac)
{
	char szTmp[64];

	VACFORMAT vacWaveFormat;
	ZeroMemory(szTmp, 10 * sizeof(char));
	f->Read(szTmp, 4 * sizeof(char)) ;
	if (strncmp(szTmp, "RIFF", 4) != 0) return false;
	DWORD dwFileSize/* = m_buffer.GetNumSamples() * m_vacWaveFormat.Fmt.nBlockAlign + 36*/ ;
	f->Read(&dwFileSize, sizeof(dwFileSize)) ;
	ZeroMemory(szTmp, 10 * sizeof(char));
	f->Read(szTmp, 8 * sizeof(char)) ;
	if (strncmp(szTmp, "WAVEfmt ", 8) != 0)  return false;
	DWORD dwFmtSize /*= 16L*/;
	f->Read(&dwFmtSize, sizeof(dwFmtSize)) ;
	ZeroMemory(szTmp, 32 * sizeof(char));
	f->Read(szTmp, dwFmtSize);

	char *pos;
	pos = szTmp;
	if (blVac) {
		vacWaveFormat.Fmt.wFormatTag = WAVE_FORMAT_NATIVE;
	} else {
		memcpy(&vacWaveFormat.Fmt.wFormatTag, pos, sizeof(vacWaveFormat.Fmt.wFormatTag)) ;
	}
	pos += sizeof(vacWaveFormat.Fmt.wFormatTag);
	memcpy(&vacWaveFormat.Fmt.nChannels, pos, sizeof(vacWaveFormat.Fmt.nChannels)) ;
	pos += sizeof(vacWaveFormat.Fmt.nChannels);
	memcpy(&vacWaveFormat.Fmt.nSamplesPerSec, pos, sizeof(vacWaveFormat.Fmt.nSamplesPerSec)) ;
	pos += sizeof(vacWaveFormat.Fmt.nSamplesPerSec);
	memcpy(&vacWaveFormat.Fmt.nAvgBytesPerSec, pos, sizeof(vacWaveFormat.Fmt.nAvgBytesPerSec)) ;
	pos += sizeof(vacWaveFormat.Fmt.nAvgBytesPerSec);
	memcpy(&vacWaveFormat.Fmt.nBlockAlign, pos, sizeof(vacWaveFormat.Fmt.nBlockAlign)) ;
	pos += sizeof(vacWaveFormat.Fmt.nBlockAlign);
	memcpy(&vacWaveFormat.Fmt.wBitsPerSample, pos, sizeof(vacWaveFormat.Fmt.wBitsPerSample)) ;

	ZeroMemory(szTmp, 64 * sizeof(char));
	f->Read(szTmp, 4 * sizeof(char)) ;
	if (strncmp(szTmp, "data", 4) == 0) {
		//// "data"�i���j�APCM�̏ꍇ�j
	} else if (strncmp(szTmp, "fact", 4) == 0) {
		//// "fact"�i���j�APCM�łȂ��ꍇ�j
		ZeroMemory(szTmp, 64 * sizeof(char));
		f->Read(szTmp, 4 * sizeof(char)) ;	//// �o�C�g��
		int sz = (int)*szTmp;
		ZeroMemory(szTmp, 64 * sizeof(char));
		f->Read(szTmp, sz * sizeof(char)) ;	//// �T���v����
		ZeroMemory(szTmp, 64 * sizeof(char));
		f->Read(szTmp, 4 * sizeof(char)) ;
		if (strncmp(szTmp, "data", 4) != 0) return false;
	} else {
		return false;
	}

	if (blVac) {
		vacWaveFormat.Fmt.cbSize = sizeof(GUID);
		vacWaveFormat.Key = OpenSig;
	} else {
		vacWaveFormat.Fmt.cbSize = 0;
	}
	m_vacWaveFormat = vacWaveFormat;

	return true;
}

/*
bool CWave::Load(CFile *f, BOOL blVac)
{
	if (LoadFormat(f, blVac) == false) return false;

	// ����ȃt�@�C���ǂݍ��݂͎��Ԃ������邽�߁A�X���b�h���i�Đ��̕����x�����ߖ��Ȃ��j
	m_buffer.m_blActive = TRUE;	// �t�@�C�����[�h�X���b�h�̊J�n���w��
	Sleep(500);		// �O��̃X���b�h���c���Ă���ꍇ������̂�
	AfxBeginThread(WaveWorkerThread, this);

//	f->Read(m_buffer.GetBuffer(), dwNum) ;

	return true;
}
*/
//////////////////////////////////////////////////////////////////////
/*
void CWave::Save(const CString &strFile)
{
	CFile f(strFile, CFile::modeCreate | CFile::modeWrite);
	Save(&f);
	f.Close();
}

//////////////////////////////////////////////////////////////////////
void CWave::Save(CFile *f)
{
	ASSERT( m_buffer.GetNumSamples() > 0 );

	f->Write("RIFF", 4) ;
	DWORD dwFileSize = m_buffer.GetNumSamples() * m_vacWaveFormat.Fmt.nBlockAlign + 36 ;
	f->Write(&dwFileSize, sizeof(dwFileSize)) ;
	f->Write("WAVEfmt ", 8) ;
	DWORD dwFmtSize = 16L;
	f->Write(&dwFmtSize, sizeof(dwFmtSize)) ;
	WORD wFormatTag = WAVE_FORMAT_PCM;
	f->Write(&wFormatTag, sizeof(wFormatTag)) ;
	f->Write(&m_vacWaveFormat.Fmt.nChannels, sizeof(m_vacWaveFormat.Fmt.nChannels)) ;
	f->Write(&m_vacWaveFormat.Fmt.nSamplesPerSec, sizeof(m_vacWaveFormat.Fmt.nSamplesPerSec)) ;
	f->Write(&m_vacWaveFormat.Fmt.nAvgBytesPerSec, sizeof(m_vacWaveFormat.Fmt.nAvgBytesPerSec)) ;
	f->Write(&m_vacWaveFormat.Fmt.nBlockAlign, sizeof(m_vacWaveFormat.Fmt.nBlockAlign)) ;
	f->Write(&m_vacWaveFormat.Fmt.wBitsPerSample, sizeof(m_vacWaveFormat.Fmt.wBitsPerSample)) ;
	f->Write("data", 4) ;
	DWORD dwNum = m_buffer.GetNumSamples() * m_vacWaveFormat.Fmt.nBlockAlign;
	f->Write(&dwNum, sizeof(dwNum)) ;
	f->Write(m_buffer.GetBuffer(), dwNum) ;
}
*/

//////////////////////////////////////////////////////////////////////
CWaveBuffer* CWave::GetBuffer()
{
	TRY {
		if (m_listOfBuffer.IsEmpty()) return NULL;

		POSITION pos = m_listOfBuffer.GetHeadPosition();
		if (pos == NULL)  return NULL;
		CWaveBuffer* pBuf = (CWaveBuffer*)m_listOfBuffer.GetAt(pos);
		return pBuf;
	}
    CATCH_ALL(e)
    {
		CTPerror te;
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("GetBuffer error:%s"),  szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
		return NULL;
    }
	END_CATCH_ALL
}

bool CWave::DelBuffer()
{
	TRY {
		if (m_listOfBuffer.IsEmpty()) return true;

		POSITION pos = m_listOfBuffer.GetHeadPosition();
		if (pos == NULL)  return true;
		CWaveBuffer* pBuf = (CWaveBuffer*)m_listOfBuffer.GetAt(pos);
		if (pBuf) {
			// buffer�����Ă���ꍇ������i�X���b�h����ɃI�u�W�F�N�g���j������Ă���H�j
			if (pBuf->m_blActive == FALSE) {
				delete pBuf;
			}
			m_listOfBuffer.RemoveHead();
		}
	}
    CATCH_ALL(e)
    {
		CTPerror te;
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("DelBuffer error:%s"),  szCause);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_WAVE, te.errstr);
		return false;
    }
	END_CATCH_ALL

	return true;
}

// �ϊ����f�[�^���o�b�t�@��
BOOL CWave::AddNewBuffer(void *pData, long lngSize)
{
	CWaveBuffer *pBuf = new CWaveBuffer;

	pBuf->m_blActive = TRUE;	// �X���b�h�ɂ��A�N�Z�X�𐧌�����t���O

	m_listOfBuffer.AddTail(pBuf);
	( (CWaveBuffer*)m_listOfBuffer.GetTail() )->CopyBuffer(pData, lngSize / 4, 4);	// 4:16bit stereo

	pBuf->m_blActive = FALSE;

	return TRUE;
}

// �f�[�^�o�b�t�@�����
void CWave::FreeListOfBuffer()
{
	CWaveBuffer* pBuf = NULL;
	POSITION pos = NULL;

	pos = m_listOfBuffer.GetHeadPosition();
	while (pos) {
		pBuf = (CWaveBuffer*)m_listOfBuffer.GetNext(pos);
		if (pBuf) {
			delete pBuf;
			pBuf = NULL;
		}
	}
	m_listOfBuffer.RemoveAll();
}

