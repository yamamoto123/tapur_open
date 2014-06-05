
#include "stdafx.h"
//#include <mmsystem.h>
#include "AudioStream.h"


/// <summary>Initialize an AudioStream for an existing stream</summary>
/// <param name="aviFile">The file that contains the stream</param>
/// <param name="aviStream">An IAVISTREAM from [aviFile]</param>
CAudioStream::CAudioStream(PAVIFILE pAviFile, PAVISTREAM pAviStream){
	m_pAviFile = pAviFile;
	m_pAviStream = pAviStream;
	m_writeCompressed = false;

	long size = sizeof(PCMWAVEFORMAT);
	AVIStreamReadFormat(pAviStream, 0, &waveFormat, &size);
	streamInfo = GetStreamInfo(pAviStream);
}

/// <summary>Read the stream's header information</summary>
/// <param name="aviStream">The IAVISTREAM to read from</param>
/// <returns>AVISTREAMINFO</returns>
AVISTREAMINFO CAudioStream::GetStreamInfo(PAVISTREAM pAviStream){
	AVISTREAMINFO streamInfo;
	HRESULT result = AVIStreamInfo(pAviStream, &streamInfo, sizeof(AVISTREAMINFO));
	if(result) {
		AfxMessageBox(_T("Exception in AVIStreamInfo"));
	}
	return streamInfo;
}

/// <summary>Read the stream's header information</summary>
/// <returns>AVISTREAMINFO</returns>
AVISTREAMINFO CAudioStream::GetStreamInfo(){
	if(m_writeCompressed){
		return GetStreamInfo(m_pCompressedStream);
	}else{
		return GetStreamInfo(m_pAviStream);
	}
}

/// <summary>Read the stream's format information</summary>
/// <returns>PCMWAVEFORMAT</returns>
PCMWAVEFORMAT CAudioStream::GetFormat(){
	PCMWAVEFORMAT format;
	long size = sizeof(PCMWAVEFORMAT);
	AVIStreamReadFormat(m_pAviStream, 0, &format, &size);
//	HRESULT result = AVIStreamReadFormat(m_pAviStream, 0, &format, &size);
	return format;
}

/// <summary>Returns all data needed to copy the stream</summary>
/// <remarks>Do not forget to call Marshal.FreeHGlobal and release the raw data pointer</remarks>
/// <param name="streamInfo">Receives the header information</param>
/// <param name="format">Receives the format</param>
/// <param name="streamLength">Receives the length of the stream</param>
/// <returns>Pointer to the wave data</returns>
LPVOID CAudioStream::GetStreamData(AVISTREAMINFO &streamInfo, PCMWAVEFORMAT &format, int &streamLength)
{
	streamInfo = GetStreamInfo();
	
	format = GetFormat();
	//length in bytes = length in samples * length of a sample
	streamLength = AVIStreamLength(m_pAviStream) * streamInfo.dwSampleSize;
	LPVOID waveData = malloc(streamLength);
	
	HRESULT result = AVIStreamRead(m_pAviStream, 0, streamLength, waveData, streamLength, 0, 0);
	if(result){
		AfxMessageBox(_T("Exception in AVIStreamRead"));
	}
	
	return waveData;
}

/*
/// <summary>Copy the stream into a new file</summary>
/// <param name="fileName">Name of the new file</param>
void CAudioStream::ExportStream(CString fileName){
	AVICOMPRESSOPTIONS opts;
	LPAVICOMPRESSOPTIONS pOpts = &opts;

	opts.fccType         = (DWORD)mmioStringToFOURCC("auds", 0);
	opts.fccHandler      = (DWORD)mmioStringToFOURCC("CAUD", 0);
	opts.dwKeyFrameEvery = 0;
	opts.dwQuality       = 0;
	opts.dwFlags         = 0;
	opts.dwBytesPerSecond= 0;
	opts.lpFormat        = new LPVOID(0);
	opts.cbFormat        = 0;
	opts.lpParms         = new LPVOID(0);
	opts.cbParms         = 0;
	opts.dwInterleaveEvery = 0;
	
	AVISaveV(fileName, 0, 0, 1, &m_pAviStream, &pOpts);
}
*/
