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
// ChatMessage.cpp : Implementation of ChatMessage

#include "stdafx.h"
#include ".\skypemessagequeue.h"
#include ".\conversion.h"
#include ".\usercollection.h"
#include ".\user.h"
#include ".\chatmessagecollection.h"
#include ".\chatmessage.h"
#include ".\chat.h"
#include ".\datetime.h"

#define CHAT_MESSAGE_ID_CHECK \
	if(m_lID == -1) \
		return AtlReportError(GetObjectCLSID(), _T("Chat Message's ID Not Set"), GUID_NULL, E_FAIL);

namespace SkypeAPILib 
{

// ChatMessage

void ChatMessage::SetID(long ID)
{
	m_lID = ID;
}

STDMETHODIMP ChatMessage::get_ID(long *pVal)
{
	CHAT_MESSAGE_ID_CHECK
	
	*pVal = m_lID;
	return S_OK;
}

STDMETHODIMP ChatMessage::get_Chat(IChat **pVal)
{
	CHAT_MESSAGE_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CHATMESSAGE", m_lID, L"CHATNAME");
		CComObject<Chat> * ptrChat;
		CComObject<Chat>::CreateInstance(& ptrChat);
		ptrChat->SetName(pResult);
		free(pResult);
		return ptrChat->QueryInterface(pVal);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP ChatMessage::get_Timestamp(DATE *pVal)
{
	CHAT_MESSAGE_ID_CHECK

	try {
		WCHAR * pStrTimestamp = gSkypeQueue.RetrieveProperty(L"CHATMESSAGE", m_lID, L"TIMESTAMP");
		*pVal = UnixTimestampToDATE(_wtol(pStrTimestamp));
		free(pStrTimestamp);
		return S_OK;
	} catch(WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP ChatMessage::get_MessageFrom(IUser **pVal)
{
	CHAT_MESSAGE_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CHATMESSAGE", m_lID, L"FROM_HANDLE");
		CComObject<User> * ptrUser;
		CComObject<User>::CreateInstance(& ptrUser);
		ptrUser->SetUserName(pResult);
		free(pResult);
		ptrUser->AddRef();
		return ptrUser->QueryInterface(pVal);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP ChatMessage::get_MessageType(SkypeChatMessageType *pVal)
{
	CHAT_MESSAGE_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CHATMESSAGE", m_lID, L"TYPE");
		*pVal = TextToSkypeChatMessageType(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP ChatMessage::get_Users(IUserCollection **pVal)
{
	CComObject<UserCollection> * ptrUsers;

	HRESULT hr = CComObject<UserCollection>::CreateInstance(& ptrUsers);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create user list", GUID_NULL, hr);
	}

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CHATMESSAGE", m_lID, L"USERS");
		ptrUsers->SetItemList(pResult);
		free(pResult);
		return ptrUsers->QueryInterface(pVal);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP ChatMessage::get_LeftReason(SkypeChatMessageLeftReason *pVal)
{
	CHAT_MESSAGE_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CHATMESSAGE", m_lID, L"LEAVEREASON");
		*pVal = TextToSkypeChatMessageLeftReason(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP ChatMessage::get_Body(BSTR *pVal)
{
	CHAT_MESSAGE_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CHATMESSAGE", m_lID, L"BODY");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP ChatMessage::get_Status(SkypeChatMessageStatus *pVal)
{
	CHAT_MESSAGE_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CHATMESSAGE", m_lID, L"STATUS");
		*pVal = TextToSkypeChatMessageStatus(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP ChatMessage::put_Status(SkypeChatMessageStatus /*newVal*/)
{
	// TODO: Add your implementation code here

	return S_OK;
}

}