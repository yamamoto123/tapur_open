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
// Call.cpp : Implementation of Call

#include "stdafx.h"
#include "Call.h"
#include ".\Conversion.h"
#include ".\DateTime.h"
#include ".\SkypeMessageQueue.h"

namespace SkypeAPILib {

#define CALL_ID_CHECK \
	if(m_lCallID < 1) \
		return AtlReportError(GetObjectCLSID(), _T("Call Object's ID Not Set"), GUID_NULL, E_FAIL);

// Call

void Call::SetCallID(long ID)
{
	m_lCallID = ID;
}

STDMETHODIMP Call::get_Timestamp(DATE *pVal)
{
	CALL_ID_CHECK

	try {
		WCHAR * pStrTimestamp = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"TIMESTAMP");
		*pVal = UnixTimestampToDATE(_wtol(pStrTimestamp));
		free(pStrTimestamp);
		return S_OK;
	} catch(const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	} 
}

STDMETHODIMP Call::get_ID(LONG *pVal)
{
	*pVal = m_lCallID;
	return S_OK;
}

STDMETHODIMP Call::get_PartnerHandle(BSTR *pVal)
{
	CALL_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"PARTNER_HANDLE");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}


STDMETHODIMP Call::get_PartnerDisplayName(BSTR *pVal)
{
	CALL_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"PARTNER_DISPNAME");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::get_Type(SkypeCallType *pVal)
{
	CALL_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"TYPE");
		*pVal = TextToSkypeCallType(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}


STDMETHODIMP Call::get_ConferenceID(LONG *pVal)
{
	CALL_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"CONF_ID");
		*pVal = _wtol(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::get_Status(SkypeCallProgress *pVal)
{
	CALL_ID_CHECK

	try {
#ifdef _NO_USE_LIST
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"STATUS");
		*pVal = TextToSkypeCallProgress(pResult);
		free(pResult);
#else
		*pVal = gSkypeQueue.GetCallProgress(m_lCallID);
#endif
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::put_Status(/*[in]*/ SkypeCallProgress newVal)
{
	WCHAR * strStatus;
	WCHAR * pResult = NULL;

	switch(newVal) {
		case prgInProgress:
			strStatus = L"INPROGRESS";
			break;
		case prgOnHold:
			strStatus = L"ONHOLD";
			break;
		case prgFinished:
			strStatus = L"FINISHED";
			pResult = L"FINISHED|REFUSED";
			break;
		default:
			return AtlReportError(GetObjectCLSID(), L"Call status may only be set to in progres, on hold or finished", GUID_NULL, E_FAIL);
	}

	try {
		gSkypeQueue.SetProperty(L"CALL", m_lCallID, L"STATUS", strStatus, pResult);
#ifndef _NO_USE_LIST
		gSkypeQueue.UpdateCallProgress(m_lCallID, TextToSkypeCallProgress(strStatus));
#endif
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::get_FailureReason(BSTR *pVal)
{
	CALL_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"FAILUREREASON");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::get_Subject(BSTR *pVal)
{
	CALL_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"SUBJECT");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::get_PSTN_Number(BSTR *pVal)
{
	CALL_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"PSTN_NUMBER");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::get_Duration(LONG *pVal)
{
	CALL_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"DURATION");
		*pVal = _wtol(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::get_PSTN_Status(BSTR *pVal)
{
	CALL_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"PSTN_STATUS");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}


STDMETHODIMP Call::get_ParticipantCount(long *pVal)
{
	CALL_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"CONF_PARTICIPANTS_COUNT");
		*pVal = _wtol(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}


STDMETHODIMP Call::GetParticipantInformation(LONG Number, BSTR * Handle, SkypeCallType * Type, SkypeCallProgress * Status, BSTR * DisplayName)
{
	CALL_ID_CHECK

	try {
		_bstr_t bstrHandle(L"");
		_bstr_t bstrDisplayName(L"");
		*Type = ctypUnknown;
		*Status = prgUnknown;

		WCHAR strInfo[32];
		swprintf_s(strInfo, 32, L"CONF_PARTICIPANT %d", Number);
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, strInfo);
		if(pResult) {
			WCHAR *next_token = NULL;
			WCHAR * pToken = wcstok_s(pResult, L" ", &next_token);
			int ctr = 0;
			while(pToken) {
				switch(++ctr) {
				case 1:
					bstrHandle = pToken;
					break;
				case 2:
					*Type = TextToSkypeCallType(pToken);
					break;
				case 3:
					*Status = TextToSkypeCallProgress(pToken);
					break;
				case 4:
					bstrDisplayName = pToken;
					break;
				}

				if(ctr == 4) {
					break;
				} else {
					pToken = wcstok_s(NULL, L" ", &next_token);
				}
			}
		}

		*Handle = SysAllocString(bstrHandle);
		*DisplayName = SysAllocString(bstrDisplayName);
		
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}
STDMETHODIMP Call::JoinConference(/*[in]*/ LONG CallID)
{
	CALL_ID_CHECK

	try {
		WCHAR strCmd[64];
		swprintf_s(strCmd, 64, L"CALL %d JOIN_CONFERENCE %d", m_lCallID, CallID);
		gSkypeQueue.SendCommand(strCmd);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::get_VoiceMailDuration(/*[out, retval]*/ LONG *pVal)
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(5)

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"VM_DURATION");
		*pVal = _wtol(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::get_VoiceMailAllowedDuration(/*[out, retval]*/ LONG *pVal)
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(5)
	
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"VM_ALLOWED_DURATION");
		*pVal = _wtol(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::SendDTMF(/*[in]*/ BSTR Codes)
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(5)

	try {
		WCHAR wstrCallID[32];
		swprintf_s(wstrCallID, 32, L"%d", m_lCallID);

		wstring wstrCmd = L"SET CALL ";
		wstrCmd += wstrCallID;
		wstrCmd += L" DTMF ";
		wstrCmd += Codes;

		gSkypeQueue.SendCommand(wstrCmd.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::GetInput(BSTR *Soundcard, BSTR *Port, BSTR *File)
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(6)

	try {
		_bstr_t bstrSoundcard(L"");
		_bstr_t bstrPort(L"");
		_bstr_t bstrFile(L"");

		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"INPUT");
		if(pResult) {
			WCHAR *next_token = NULL;
			WCHAR * pToken = wcstok_s(pResult, L" ", &next_token);
			int ctr = 0;
			size_t szProp = 0;
			int result = 0;
			while(pToken) {
				++ctr;
				pToken = trim(pToken);
				if (wcslen(pToken) > 0) {
					szProp = wcslen(L"SOUNDCARD=");
					result = _wcsnicmp(pToken, L"SOUNDCARD=", szProp);
					if( result == 0 ) {
						bstrSoundcard = pToken;
					}
					szProp = wcslen(L"PORT=");
					result = _wcsnicmp(pToken, L"PORT=", szProp);
					if( result == 0 ) {
						bstrPort = pToken;
					}
					szProp = wcslen(L"FILE=");
					result = _wcsnicmp(pToken, L"FILE=", szProp);
					if( result == 0 ) {
						bstrFile = pToken;
					}
				}
				if(ctr >= 3) {
					break;
				}

                pToken = wcstok_s(NULL, L" ", &next_token);
			}
		}

		*Soundcard = SysAllocString(bstrSoundcard);
		*Port = SysAllocString(bstrPort);
		*File = SysAllocString(bstrFile);

		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::GetOutput(BSTR *Soundcard, BSTR *Port, BSTR *File)
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(6)

	try {
		_bstr_t bstrSoundcard(L"");
		_bstr_t bstrPort(L"");
		_bstr_t bstrFile(L"");

		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"OUTPUT");
		if(pResult) {
			WCHAR *next_token = NULL;
			WCHAR * pToken = wcstok_s(pResult, L" ", &next_token);
			int ctr = 0;
			size_t szProp = 0;
			int result = 0;
			while(pToken) {
				++ctr;
				pToken = trim(pToken);
				if (wcslen(pToken) > 0) {
					szProp = wcslen(L"SOUNDCARD=");
					result = _wcsnicmp(pToken, L"SOUNDCARD=", szProp);
					if( result == 0 ) {
						bstrSoundcard = pToken;
					}
					szProp = wcslen(L"PORT=");
					result = _wcsnicmp(pToken, L"PORT=", szProp);
					if( result == 0 ) {
						bstrPort = pToken;
					}
					szProp = wcslen(L"FILE=");
					result = _wcsnicmp(pToken, L"FILE=", szProp);
					if( result == 0 ) {
						bstrFile = pToken;
					}
				}
				if(ctr >= 3) {
					break;
				}

				pToken = wcstok_s(NULL, L" ", &next_token);
			}
		}

		*Soundcard = SysAllocString(bstrSoundcard);
		*Port = SysAllocString(bstrPort);
		*File = SysAllocString(bstrFile);

		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::GetCaptureMic(BSTR *Port, BSTR *File)
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(6)

	try {
		_bstr_t bstrPort(L"");
		_bstr_t bstrFile(L"");

		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"CAPTURE_MIC");
		if(pResult) {
			WCHAR *next_token = NULL;
			WCHAR * pToken = wcstok_s(pResult, L" ", &next_token);
			int ctr = 0;
			size_t szProp = 0;
			int result = 0;
			while(pToken) {
				++ctr;
				pToken = trim(pToken);
				if (wcslen(pToken) > 0) {
					szProp = wcslen(L"PORT=");
					result = _wcsnicmp(pToken, L"PORT=", szProp);
					if( result == 0 ) {
						bstrPort = pToken;
					}
					szProp = wcslen(L"FILE=");
					result = _wcsnicmp(pToken, L"FILE=", szProp);
					if( result == 0 ) {
						bstrFile = pToken;
					}
				}
				if(ctr >= 2) {
					break;
				}

				pToken = wcstok_s(NULL, L" ", &next_token);
			}
		}

		*Port = SysAllocString(bstrPort);
		*File = SysAllocString(bstrFile);

		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::SetInput(BSTR Soundcard, BSTR Port, BSTR File)
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(6)

	try {
		WCHAR wstrVal[_MAX_LINE];

		wcscpy_s(wstrVal, _MAX_LINE, L"");
		if (wcslen(Soundcard) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L"SOUNDCARD=\"");
			wcscat_s(wstrVal, _MAX_LINE, Soundcard);
			wcscat_s(wstrVal, _MAX_LINE, L"\"");
		}
		if (wcslen(wstrVal) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L" ");
		}
		if (wcslen(Port) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L"PORT=\"");
			wcscat_s(wstrVal, _MAX_LINE, Port);
			wcscat_s(wstrVal, _MAX_LINE, L"\"");
		}
		if (wcslen(wstrVal) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L" ");
		}
		if (wcslen(File) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L"FILE=\"");
			wcscat_s(wstrVal, _MAX_LINE, File);
			wcscat_s(wstrVal, _MAX_LINE, L"\"");
		}

		gSkypeQueue.AlterMethodNoWait(L"CALL", m_lCallID, L"SET_INPUT", wstrVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

	return S_OK;
}

STDMETHODIMP Call::SetOutput(BSTR Soundcard, BSTR Port, BSTR File)
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(6)

	try {
		WCHAR wstrVal[_MAX_LINE];

		wcscpy_s(wstrVal, _MAX_LINE, L"");
		if (wcslen(Soundcard) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L"SOUNDCARD=\"");
			wcscat_s(wstrVal, _MAX_LINE, Soundcard);
			wcscat_s(wstrVal, _MAX_LINE, L"\"");
		}
		if (wcslen(wstrVal) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L" ");
		}
		if (wcslen(Port) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L"PORT=\"");
			wcscat_s(wstrVal, _MAX_LINE, Port);
			wcscat_s(wstrVal, _MAX_LINE, L"\"");
		}
		if (wcslen(wstrVal) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L" ");
		}
		if (wcslen(File) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L"FILE=\"");
			wcscat_s(wstrVal, _MAX_LINE, File);
			wcscat_s(wstrVal, _MAX_LINE, L"\"");
		}

		gSkypeQueue.AlterMethodNoWait(L"CALL", m_lCallID, L"SET_OUTPUT", wstrVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

	return S_OK;
}

STDMETHODIMP Call::SetCaptureMic(BSTR Port, BSTR File)
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(6)

	try {
		WCHAR wstrVal[_MAX_LINE];

		wcscpy_s(wstrVal, _MAX_LINE, L"");
		if (wcslen(Port) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L"PORT=\"");
			wcscat_s(wstrVal, _MAX_LINE, Port);
			wcscat_s(wstrVal, _MAX_LINE, L"\"");
		}
		if (wcslen(wstrVal) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L" ");
		}
		if (wcslen(File) > 0) {
			wcscat_s(wstrVal, _MAX_LINE, L"FILE=\"");
			wcscat_s(wstrVal, _MAX_LINE, File);
			wcscat_s(wstrVal, _MAX_LINE, L"\"");
		}

		gSkypeQueue.AlterMethodNoWait(L"CALL", m_lCallID, L"SET_CAPTURE_MIC", wstrVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

	return S_OK;
}

STDMETHODIMP Call::get_VideoStatus(BSTR *pVal)
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(5)

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CALL", m_lCallID, L"VIDEO_STATUS");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Call::StartVideoSend()
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(5)

	try {
		gSkypeQueue.AlterMethodNoWait(L"CALL", m_lCallID, L"START_VIDEO_SEND", L"");
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

	return S_OK;
}

STDMETHODIMP Call::StopVideoSend()
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(5)

	try {
		gSkypeQueue.AlterMethodNoWait(L"CALL", m_lCallID, L"STOP_VIDEO_SEND", L"");
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

	return S_OK;
}

STDMETHODIMP Call::StartVideoRecieve()
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(5)

	try {
		gSkypeQueue.AlterMethodNoWait(L"CALL", m_lCallID, L"START_VIDEO_RECEIVE", L"");
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

	return S_OK;
}

STDMETHODIMP Call::StopVideoRecieve()
{
	CALL_ID_CHECK
	CHECK_PROTOCOL(5)

	try {
		gSkypeQueue.AlterMethodNoWait(L"CALL", m_lCallID, L"STOP_VIDEO_RECEIVE", L"");
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

	return S_OK;
}

}