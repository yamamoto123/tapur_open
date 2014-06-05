/*
Copyright (c) 2005, Jason Terando
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the parties distributing the library nor the names of its contributors may be
      used to endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
// App2AppStream.cpp : Implementation of App2AppStream

#include "stdafx.h"
#include ".\skypemessagequeue.h"
#include ".\usercollection.h"
#include ".\user.h"
#include ".\app2app.h"
#include ".\app2appstream.h"
#include ".\md5.h"

namespace SkypeAPILib {

#define STREAM_ID_CHECK \
	if(m_strAppName.empty() || m_strStreamID.empty()) \
		return AtlReportError(GetObjectCLSID(), _T("Stream Information Not Set"), GUID_NULL, E_FAIL);

// App2AppStream
STDMETHODIMP App2AppStream::Disconnect(void)
{
	STREAM_ID_CHECK
	CHECK_PROTOCOL(5)

	wstring wstrCommand = L"ALTER APPLICATION ";
	wstrCommand += m_strAppName;
	wstrCommand += L" DISCONNECT ";
	wstrCommand += m_strStreamID;

	try {
		gSkypeQueue.ExecuteBlockingCommand(wstrCommand.c_str(), wstrCommand.c_str());
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

	return S_OK;
}

STDMETHODIMP App2AppStream::WriteData(BSTR TextData)
{
	STREAM_ID_CHECK
	CHECK_PROTOCOL(5)

	if(wcslen(TextData) > 32767) {
		return AtlReportError(GetObjectCLSID(), L"Datagrams can be a maximum of 65,535 bytes (32,767 unicode)",
			GUID_NULL, E_INVALIDARG);
	}

	wstring wstrPrefix = L"ALTER APPLICATION ";
	wstrPrefix += m_strAppName;
	wstrPrefix += L" WRITE ";
	wstrPrefix += m_strStreamID;

	wstring wstrCommand = wstrPrefix;
	wstrCommand += L" ";
	wstrCommand += TextData;

	try {
		gSkypeQueue.ExecuteBlockingCommand(wstrCommand.c_str(), wstrPrefix.c_str());
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

	return S_OK;
}

STDMETHODIMP App2AppStream::WriteDatagram(BSTR TextData)
{
	STREAM_ID_CHECK
	CHECK_PROTOCOL(5)

	if(wcslen(TextData) > 700) {
		return AtlReportError(GetObjectCLSID(), L"Datagrams can be a maximum of 1,400 bytes (700 unicode)",
			GUID_NULL, E_INVALIDARG);
	}

	wstring wstrPrefix = L"ALTER APPLICATION ";
	wstrPrefix += m_strAppName;
	wstrPrefix += L" DATAGRAM ";
	wstrPrefix += m_strStreamID;

	wstring wstrCommand = wstrPrefix;
	wstrCommand += L" ";
	wstrCommand += TextData;

	try {
		gSkypeQueue.ExecuteBlockingCommand(wstrCommand.c_str(), wstrPrefix.c_str());
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

	return S_OK;
}


void App2AppStream::Initialize(LPCWSTR AppName, LPCWSTR StreamID)
{
	m_strAppName = AppName;
	m_strStreamID = StreamID;
}

HRESULT App2AppStream::get_StreamID(BSTR * pVal)
{
	STREAM_ID_CHECK
	CHECK_PROTOCOL(5)

	*pVal = SysAllocString(m_strStreamID.c_str());
	return S_OK;
}

HRESULT App2AppStream::get_User(IUser** pVal)
{
	STREAM_ID_CHECK
	CHECK_PROTOCOL(5)

	CComObject<SkypeAPILib::User> * ptrUser;
	HRESULT hr = CComObject<SkypeAPILib::User>::CreateInstance(& ptrUser);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create user object",
			GUID_NULL, hr);
	}

	size_t iPos = m_strStreamID.find(':');
	if(iPos == string.npos) {
		ptrUser->SetUserName(m_strStreamID.c_str());
	} else {
		ptrUser->SetUserName(m_strStreamID.substr(0, iPos).c_str());
	}

	return ptrUser->QueryInterface(pVal);
}

HRESULT App2AppStream::WriteBinaryData(BSTR ID, SAFEARRAY ** Array)
{
	HRESULT hr;
	VARTYPE vt;
	ULONG ulCount, ulRead;

	wstring wstrPrefix = L"ALTER APPLICATION ";
	wstrPrefix += m_strAppName;
	wstrPrefix += L" WRITE ";
	wstrPrefix += m_strStreamID;
	wstrPrefix += L" #XFERSTREAM# ";
	wstrPrefix += ID;

	bool bOk = false;
	UINT uiDims = SafeArrayGetDim(* Array);
	SafeArrayGetVartype(* Array, & vt);

	if((uiDims != 1) || (vt != VT_UI1)) {
		return AtlReportError(GetObjectCLSID(), L"Array must be a one dimensional array of bytes", GUID_NULL, E_INVALIDARG);
	}

	ulCount = (*Array)->rgsabound[0].cElements;
	if(ulCount < 1) {
		return AtlReportError(GetObjectCLSID(), L"Array is empty", GUID_NULL, E_INVALIDARG);
	}
	
	hr = SafeArrayLock(* Array);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to lock array", GUID_NULL, E_FAIL);
	}

	// Calculate the MD5 checksum
	MD5 objMD5;
	char * ptrMD5 = objMD5.CalcMD5FromBuffer((LPBYTE) (*Array)->pvData, ulCount);
	{
		WCHAR wstrSize[32];
		wstring wstrCommand = wstrPrefix;
		wstrCommand += L" START ";
		wstrCommand += CA2W(ptrMD5);
		swprintf_s(wstrSize, 32, L" %d", ulCount);
		wstrCommand += wstrSize;
		try {
			WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(wstrCommand.c_str(), 
				wstrPrefix.c_str());
			free(pResult);
		} catch(const WCHAR * ex)  {
			return AtlReportError(GetObjectCLSID(), ex,	GUID_NULL, E_FAIL);
		}
	}
	
	ULONG ulIndex = 0;
	const ULONG BUFFER_SIZE = 8192;
	BYTE bBuffer[BUFFER_SIZE * 2 + 2];

	while(ulIndex < ulCount) {
		// Default to the total bytes in the buffer
		ulRead = ulCount - ulIndex;
		if(ulRead > BUFFER_SIZE / 2) {
			ulRead = BUFFER_SIZE / 2;
		}

		memset(bBuffer, 0, BUFFER_SIZE * 2 + 2);
		memcpy(& bBuffer[0], ((LPBYTE) (*Array)->pvData) + ulIndex, ulRead);

		ULONG i = 0;
		ULONG ulBufferLength = ulRead;
		while(i < ulBufferLength) {

			// Replace FF's with FF FF
			if(bBuffer[i] == 0xFF) {
				ulBufferLength++;
				memmove(& bBuffer[i + 1], & bBuffer[i], ulBufferLength - i + 1);
				bBuffer[i + 1] = 0xFF;
				i++;
			}

			// Replace FF's with FF EE
			if(bBuffer[i] == 0x00) {
				ulBufferLength++;
				memmove(& bBuffer[i + 1], & bBuffer[i], ulBufferLength - i + 1);
				bBuffer[i] = 0xFF;
				bBuffer[i + 1] = 0xFE;
				i++;
			}

			i++;
		}

		// Now, write the data
		try {
			LPWSTR lpswData = (LPWSTR) bBuffer;
			wstring wstrCommand = wstrPrefix;
			wstrCommand += L" DATA ";
			// Switch the lo/hi bytes of each word so we don't get NULL'd on the last byte (if odd number)
			for(ULONG i = 0; i < wcslen(lpswData); i++) {
				lpswData[i] = WordSwap(lpswData[i]);
			}
			
			wstrCommand += lpswData;
			WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(wstrCommand.c_str(), 
				wstrPrefix.c_str());
			free(pResult);
			Sleep(500);
		} catch (const WCHAR * err) {
			return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
		}

		ulIndex += ulRead;
	} 

	wstring wstrCommand = wstrPrefix;
	wstrCommand += L" END";
	try {
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(wstrCommand.c_str(), 
			wstrPrefix.c_str());
		free(pResult);
	} catch(const WCHAR * ex)  {
		return AtlReportError(GetObjectCLSID(), ex,	GUID_NULL, E_FAIL);
	}

	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to read from stream", 
			GUID_NULL, hr);
	}

	return S_OK;
}

}