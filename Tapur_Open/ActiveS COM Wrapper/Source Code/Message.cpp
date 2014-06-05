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
// Message.cpp : Implementation of Message

#include "stdafx.h"
#include "Message.h"
#include ".\conversion.h"
#include ".\skypemessagequeue.h"


#define MESSAGE_ID_CHECK \
	if(m_lMessageID < 1) \
		return AtlReportError(GetObjectCLSID(), _T("Message Object's ID Not Set"), GUID_NULL, E_FAIL);

namespace SkypeAPILib 
{

// Message

void Message::SetMessageID(LONG ID)
{
	m_lMessageID = ID;
}

STDMETHODIMP Message::get_ID(LONG *pVal)
{
	*pVal = m_lMessageID;
	return S_OK;
}

STDMETHODIMP Message::get_Status(SkypeMessageStatus *pVal)
{
	MESSAGE_ID_CHECK;

	try {
		WCHAR * ptrResponse = gSkypeQueue.RetrieveProperty(L"MESSAGE", m_lMessageID, L"STATUS");
		*pVal = TextToSkypeMesasgeStatus(ptrResponse);
		free(ptrResponse);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Message::get_Timestamp(DATE *pVal)
{
	MESSAGE_ID_CHECK;

	try {
		WCHAR * pStrTimestamp = gSkypeQueue.RetrieveProperty(L"MESSAGE", m_lMessageID, L"TIMESTAMP");
		*pVal = UnixTimestampToDATE(_wtol(pStrTimestamp));
		free(pStrTimestamp);
		return S_OK;
	} catch(WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Message::get_PartnerHandle(BSTR *pVal)
{
	MESSAGE_ID_CHECK;

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"MESSAGE", m_lMessageID, L"PARTNER_HANDLE");
		*pVal = SysAllocString(CComBSTR(pResult));
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Message::get_PartnerDisplayName(BSTR *pVal)
{
	MESSAGE_ID_CHECK;

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"MESSAGE", m_lMessageID, L"PARTNER_DISPNAME");
		*pVal = SysAllocString(CComBSTR(pResult));
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Message::get_ConferenceID(LONG *pVal)
{
	MESSAGE_ID_CHECK;

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"MESSAGE", m_lMessageID, L"CONF_ID");
		*pVal = _wtol(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Message::get_Type(SkypeMessageType *pVal)
{
	MESSAGE_ID_CHECK;

	try {
		WCHAR * ptrResponse = gSkypeQueue.RetrieveProperty(L"MESSAGE", m_lMessageID, L"TYPE");
		*pVal = TextToSkypeMessageType(ptrResponse);
		free(ptrResponse);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Message::get_TypeAsText(BSTR *pVal)
{
	MESSAGE_ID_CHECK;

	try {
		WCHAR * ptrResponse = gSkypeQueue.RetrieveProperty(L"MESSAGE", m_lMessageID, L"TYPE");
		*pVal = SysAllocString(CComBSTR(ptrResponse));
		free(ptrResponse);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Message::get_Body(BSTR *pVal)
{
	MESSAGE_ID_CHECK;

	try {
		WCHAR * ptrResponse = gSkypeQueue.RetrieveProperty(L"MESSAGE", m_lMessageID, L"BODY");
		*pVal = SysAllocString(CComBSTR(ptrResponse));
		free(ptrResponse);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Message::get_Seen(VARIANT_BOOL *pVal)
{
	MESSAGE_ID_CHECK;

	try {
		WCHAR * ptrResponse = gSkypeQueue.RetrieveProperty(L"MESSAGE", m_lMessageID, L"SEEN");
		bool bSeen = false;
		if(ptrResponse) {
			bSeen = (_wcsicmp(ptrResponse, L"TRUE") == 0);
		}
		*pVal = bSeen;
		free(ptrResponse);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Message::put_Seen(VARIANT_BOOL newVal)
{
	MESSAGE_ID_CHECK;

	try {
		gSkypeQueue.SetProperty(L"CALL", m_lMessageID, L"SEEN", 
			newVal ? L"TRUE" : L"FALSE");
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

}