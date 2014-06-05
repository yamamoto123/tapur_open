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
// VoiceMail.cpp : Implementation of VoiceMail

#include "stdafx.h"
#include ".\conversion.h"
#include ".\skypemessagequeue.h"
#include ".\voicemail.h"

#define VOICEMAIL_ID_CHECK \
	if(m_lVoiceMailID < 1) \
		return AtlReportError(GetObjectCLSID(), "VoiceMail Object's ID Not Set", GUID_NULL, E_FAIL);

namespace SkypeAPILib 
{

// VoiceMail

void VoiceMail::SetID(LONG ID) 
{
	m_lVoiceMailID = ID;
}

STDMETHODIMP VoiceMail::get_ID(LONG* pVal) 
{
	*pVal = m_lVoiceMailID;
	return S_OK;
}

STDMETHODIMP VoiceMail::get_Type(SkypeVoiceMailType* pVal)
{
	VOICEMAIL_ID_CHECK
	
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"VOICEMAIL", m_lVoiceMailID, L"TYPE");
		*pVal = TextToSkypeVoiceMailType(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP VoiceMail::get_PartnerHandle(BSTR* pVal)
{
	VOICEMAIL_ID_CHECK
	
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"VOICEMAIL", m_lVoiceMailID, L"PARTNER_HANDLE");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP VoiceMail::get_PartnerDisplayName(BSTR* pVal)
{
	VOICEMAIL_ID_CHECK
	
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"VOICEMAIL", m_lVoiceMailID, L"PARTNER_DISPNAME");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP VoiceMail::get_Status(SkypeVoiceMailStatus* pVal)
{
	VOICEMAIL_ID_CHECK
	
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"VOICEMAIL", m_lVoiceMailID, L"STATUS");
		*pVal = TextToSkypeVoiceMailStatus(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP VoiceMail::get_FailureReason(SkypeVoiceMailFailure* pVal)
{
	VOICEMAIL_ID_CHECK
	
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"VOICEMAIL", m_lVoiceMailID, L"FAILUREREASON");
		*pVal = TextToSkypeVoiceMailFailure(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP VoiceMail::get_Subject(BSTR* pVal)
{
	VOICEMAIL_ID_CHECK
	
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"VOICEMAIL", m_lVoiceMailID, L"SUBJECT");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP VoiceMail::get_Timestamp(DATE* pVal)
{
	VOICEMAIL_ID_CHECK
	
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"VOICEMAIL", m_lVoiceMailID, L"TIMESTAMP");
		*pVal = UnixTimestampToDATE(_wtol(pResult));
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP VoiceMail::get_Duration(LONG* pVal)
{
	VOICEMAIL_ID_CHECK
	
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"VOICEMAIL", m_lVoiceMailID, L"DURATION");
		*pVal = _wtol(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP VoiceMail::get_AllowedDuration(LONG* pVal)
{
	VOICEMAIL_ID_CHECK
	
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"VOICEMAIL", m_lVoiceMailID, L"ALLOWED_DURATION");
		*pVal = _wtol(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP VoiceMail::OpenAndPlay()
{
	VOICEMAIL_ID_CHECK

	try {
		WCHAR wstrID[16];
		swprintf_s(wstrID, sizeof(wstrID)/sizeof(wstrID[0]), L"%d", m_lVoiceMailID);
		wstring wstrCmd = L"OPEN VOICEMAIL ";
		wstrCmd += wstrID;
		gSkypeQueue.SendCommand(wstrCmd.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

}