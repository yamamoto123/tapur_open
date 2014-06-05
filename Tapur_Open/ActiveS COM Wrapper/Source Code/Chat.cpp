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
// Chat.cpp : Implementation of Chat

#include "stdafx.h"
#include ".\skypemessagequeue.h"
#include ".\conversion.h"
#include ".\usercollection.h"
#include ".\user.h"
#include ".\chatmessagecollection.h"
#include ".\chatmessage.h"
#include ".\chat.h"

#define CHAT_ID_CHECK \
	if(m_strName.empty()) \
		return AtlReportError(GetObjectCLSID(), _T("Chat's Name Not Set"), GUID_NULL, E_FAIL);

namespace SkypeAPILib 
{

// Chat

STDMETHODIMP Chat::get_Name(BSTR *pVal)
{
	CHAT_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CHAT", m_strName.c_str(), L"NAME");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Chat::get_Timestamp(DATE *pVal)
{
	CHAT_ID_CHECK

	try {
		WCHAR * pStrTimestamp = gSkypeQueue.RetrieveProperty(L"CHAT", m_strName.c_str(), L"TIMESTAMP");
		*pVal = UnixTimestampToDATE(_wtol(pStrTimestamp));
		free(pStrTimestamp);
	} catch(const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
	return S_OK;
}

STDMETHODIMP Chat::get_Adder(IUser **pVal)
{
	CHAT_ID_CHECK

	CComObject<User> * ptrUser;

	HRESULT hr = CComObject<User>::CreateInstance(& ptrUser);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create user", GUID_NULL, hr);
	}

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CHAT", m_strName.c_str(), L"ADDER");
		if(wcslen(pResult) == 0) {
			free(pResult);
			return AtlReportError(GetObjectCLSID(), L"No adder for this call", GUID_NULL, E_FAIL);
		}
		ptrUser->SetUserName(pResult);
		hr = ptrUser->QueryInterface(pVal);
		free(pResult);
		return hr;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Chat::get_Status(SkypeChatStatus *pVal)
{
	CHAT_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CHAT", m_strName.c_str(), L"STATUS");
		*pVal = TextToSkypeChatStatus(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Chat::get_Posters(IUserCollection **pVal)
{
	CHAT_ID_CHECK

	CComObject<UserCollection> * ptrUsers;

	HRESULT hr = CComObject<UserCollection>::CreateInstance(& ptrUsers);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create user list", GUID_NULL, hr);
	}

	try {
		WCHAR cmd[_MAX_LINE];
		wcscpy_s(cmd, _MAX_LINE, L"GET CHAT ");
		wcsncat_s(cmd, _MAX_LINE, m_strName.c_str(), _MAX_LINE);
		wcsncat_s(cmd, _MAX_LINE, L" POSTERS", _MAX_LINE);
		WCHAR * pStrUsers = gSkypeQueue.ExecuteBlockingCommand(cmd, L"POSTERS");
		ptrUsers->SetItemList(& pStrUsers[wcslen(cmd) - 4]);
		free(pStrUsers);
		return ptrUsers->QueryInterface(pVal);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Chat::get_Members(IUserCollection **pVal)
{
	CHAT_ID_CHECK

	CComObject<UserCollection> * ptrUsers;

	HRESULT hr = CComObject<UserCollection>::CreateInstance(& ptrUsers);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create user list", GUID_NULL, hr);
	}

	try {
		WCHAR cmd[_MAX_LINE];
		wcscpy_s(cmd, _MAX_LINE, L"GET CHAT ");
		wcsncat_s(cmd, _MAX_LINE, m_strName.c_str(), _MAX_LINE);
		wcsncat_s(cmd, _MAX_LINE, L" MEMBERS", _MAX_LINE);
		WCHAR * pStrUsers = gSkypeQueue.ExecuteBlockingCommand(cmd, L"MEMBERS");
		ptrUsers->SetItemList(& pStrUsers[wcslen(cmd) - 4]);
		free(pStrUsers);
		return ptrUsers->QueryInterface(pVal);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Chat::get_Topic(BSTR *pVal)
{
	CHAT_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CHAT", m_strName.c_str(), L"TOPIC");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Chat::get_ActiveMembers(IUserCollection **pVal)
{
	CHAT_ID_CHECK

	CComObject<UserCollection> * ptrUsers;

	HRESULT hr = CComObject<UserCollection>::CreateInstance(& ptrUsers);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create user list", GUID_NULL, hr);
	}

	try {
		WCHAR cmd[_MAX_LINE];
		wcscpy_s(cmd, _MAX_LINE, L"GET CHAT ");
		wcsncat_s(cmd, _MAX_LINE, m_strName.c_str(), _MAX_LINE);
		wcsncat_s(cmd, _MAX_LINE, L" ACTIVEMEMBERS", _MAX_LINE);
		WCHAR * pStrUsers = gSkypeQueue.ExecuteBlockingCommand(cmd, L"ACTIVEMEMBERS");
		ptrUsers->SetItemList(& pStrUsers[wcslen(cmd) - 4]);
		free(pStrUsers);
		return ptrUsers->QueryInterface(pVal);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Chat::get_FriendlyName(BSTR *pVal)
{
	CHAT_ID_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"CHAT", m_strName.c_str(), L"FRIENDLYNAME");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Chat::SendMessage(/*[in]*/ BSTR Message)
{
	CHAT_ID_CHECK
	CHECK_PROTOCOL(5)

	try {
		wstring wstrCmd = L"CHATMESSAGE ";
		wstrCmd += m_strName.c_str();
		wstrCmd += L" ";
		wstrCmd += Message;
		gSkypeQueue.SendCommand(wstrCmd.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Chat::put_Topic(BSTR Topic)
{
	CHAT_ID_CHECK
	CHECK_PROTOCOL(5)

	try {
		wstring wstrCmd = L"CHAT ";
		wstrCmd += m_strName.c_str();
		wstrCmd += L" SETTOPIC ";
		wstrCmd += Topic;

		gSkypeQueue.SendCommand(wstrCmd.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Chat::Leave()
{
	CHAT_ID_CHECK
	CHECK_PROTOCOL(4)

	try {
		wstring wstrCmd = L"CHAT ";
		wstrCmd += m_strName.c_str();
		wstrCmd += L" LEAVE";
		gSkypeQueue.SendCommand(wstrCmd.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Chat::AddMembers(/*[in]*/ SAFEARRAY ** UserHandles)
{
	CHAT_ID_CHECK
	CHECK_PROTOCOL(5)

	VARTYPE vt = VT_UNKNOWN;
	SafeArrayGetVartype(* UserHandles, & vt);
	UINT uiDim = SafeArrayGetDim(* UserHandles);
	if((uiDim != 1) || (vt != VT_BSTR)) {
		return AtlReportError(GetObjectCLSID(), L"Array of users must be single dimension array of strings", GUID_NULL, E_INVALIDARG);
	}

	wstring wstrCommand;
	wstrCommand = L"CHAT ";
	wstrCommand += m_strName.c_str();
	wstrCommand += L" ";
	HRESULT hr = SafeArrayLock(* UserHandles);
	if(SUCCEEDED(hr)) {
		BSTR * pHandles = (BSTR *) (* UserHandles)->pvData;
		for(ULONG l = 0; l < (* UserHandles)->rgsabound[0].cElements; l++) {
			BSTR bstrHandle = pHandles[l];
			if(l > 0) {
				wstrCommand += L", ";
			}
			wstrCommand += bstrHandle;
		}
		SafeArrayUnlock(* UserHandles);
	}

	if(SUCCEEDED(hr)) {
		try { 
			gSkypeQueue.SendCommand(wstrCommand.c_str());
			return S_OK;
		} catch (const WCHAR * err) {
			return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
		}
	} else {
		return AtlReportError(GetObjectCLSID(), L"Unable to Add Members to Chat", GUID_NULL, hr);
	}
}

STDMETHODIMP Chat::LaunchChatWindow()
{
	CHAT_ID_CHECK
	CHECK_PROTOCOL(5)

	try {
		wstring wstrCommand = L"OPEN CHAT ";
		wstrCommand += m_strName.c_str();
		gSkypeQueue.SendCommand(wstrCommand.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Chat::GetChatMessageList(IChatMessageCollection **List)
{
	CComObject<ChatMessageCollection> * ptrChatMessages;

	CHECK_PROTOCOL(5)

	HRESULT hr = CComObject<ChatMessageCollection>::CreateInstance(& ptrChatMessages);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create chat message list", GUID_NULL, hr);
	}

	try {
		wstring wstrCmd = L"GET CHAT ";
		wstrCmd += m_strName.c_str();
		wstrCmd += L" CHATMESSAGES ";
		WCHAR * pStrMessages = gSkypeQueue.ExecuteBlockingCommand(wstrCmd.c_str(), L"CHATMESSAGES");
		ptrChatMessages->SetItemList(& pStrMessages[9]);
		free(pStrMessages);
		return ptrChatMessages->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}
STDMETHODIMP Chat::GetRecentChatMessageList(/*[out, retval]*/ IChatMessageCollection ** List)
{
	CComObject<ChatMessageCollection> * ptrChatMessages;

	CHECK_PROTOCOL(5)

	HRESULT hr = CComObject<ChatMessageCollection>::CreateInstance(& ptrChatMessages);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create chat message list", GUID_NULL, hr);
	}

	try {
		wstring wstrCmd = L"GET CHAT ";
		wstrCmd += m_strName.c_str();
		wstrCmd += L" RECENTCHATMESSAGES ";
		WCHAR * pStrMessages = gSkypeQueue.ExecuteBlockingCommand(wstrCmd.c_str(), L"CHATMESSAGES");
		ptrChatMessages->SetItemList(& pStrMessages[9]);
		free(pStrMessages);
		return ptrChatMessages->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

}