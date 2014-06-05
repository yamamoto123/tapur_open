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
// Access.cpp : Implementation of Access
#include "stdafx.h"
#include <io.h>
#include ".\skypemessagequeue.h"
#include ".\conversion.h"
#include ".\callcollection.h"
#include ".\call.h"
#include ".\usercollection.h"
#include ".\user.h"
#include ".\app2app.h"
#include ".\app2appstream.h"
#include ".\messagecollection.h"
#include ".\message.h"
#include ".\chatmessagecollection.h"
#include ".\chatmessage.h"
#include ".\chatcollection.h"
#include ".\chat.h"
#include ".\voicemailcollection.h"
#include ".\voicemail.h"
#include ".\eventwindow.h"
#include ".\ui.h"
#include ".\profile.h"
#include ".\access.h"

#ifdef _DEBUG
#include <time.h>
static void WriteLog(
	_TCHAR* str)		// ログメッセージ
{
	/// ログファイル名の決定
	time_t tnum;
	struct tm ts;
    time(&tnum);
	if (localtime_s(&ts, &tnum)) return;	// error
	_TCHAR filename[_MAX_PATH];
	_stprintf_s(filename, _MAX_PATH, _T("%s\\skype%02d%02d%02d.log"), 
		"C:\\documents and settings\\paak\\Application Data\\Tapur\\log", ts.tm_year, (ts.tm_mon + 1), ts.tm_mday);

	/// メッセージ書き込み
	_TCHAR strMsg[_MAX_PATH];
	_sntprintf_s(strMsg, _MAX_PATH, _MAX_PATH-1, _T("%02d:%02d:%02d:%s\n"), ts.tm_hour, ts.tm_min, ts.tm_sec, str);

	DWORD dwLen;
#ifdef _UNICODE
	char *buf = NULL;
#else
	char buf[_MAX_PATH];
#endif

	try
	{
		FILE *stream;
		if (fopen_s(&stream, filename, "a")) return;

#ifdef _UNICODE
		// wcstombs()は日本語を正常に処理しない
		dwLen = ::WideCharToMultiByte(CP_ACP,0,strTxt,-1,NULL,0,NULL,NULL);
		buf = new char[dwLen];
		::WideCharToMultiByte(CP_ACP, 0, strMsg, -1, buf, dwLen, NULL, NULL);
#else
		dwLen = (DWORD)_tcslen(strMsg);
		strncpy_s(buf, _MAX_PATH, (LPCTSTR)strMsg, _MAX_PATH-1);
#endif
		fwrite(buf, sizeof(char), dwLen, stream);	// 末尾のNULL文字は除外
		fclose(stream);
	}
	catch(...)
	{
	}

#ifdef _UNICODE
	if (buf) {
		delete [] buf;
		buf = NULL;
	}
#endif
}
#endif

// Access
STDMETHODIMP SkypeAPILib::Access::SendCommand(BSTR Command)
{
	try {
		gSkypeQueue.SendCommand(Command);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
	return S_OK;
}

STDMETHODIMP SkypeAPILib::Access::GetCall(LONG CallID, ICall **Call)
{
	CComObject<SkypeAPILib::Call> * ptrCall;
	
	HRESULT hr = CComObject<SkypeAPILib::Call>::CreateInstance(& ptrCall);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create call object", GUID_NULL, E_FAIL);
	}

	try {
		ptrCall->SetCallID(CallID);
		return ptrCall->QueryInterface(Call);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::PlaceCall(BSTR UserName, ICall ** Call)
{
	HRESULT hr = S_OK;
	bool bResult = false;

	try {
		wstring wstrCmd = L"CALL ";
		wstrCmd += UserName;
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(wstrCmd.c_str(), L"CALL", TRUE);
		long lCallID = 0;
		if(swscanf_s(pResult, L"CALL %d ", & lCallID) == 1) {
			try {
				if(lCallID > 0) {
					CComObject<SkypeAPILib::Call> * ptrCall;
					hr = CComObject<SkypeAPILib::Call>::CreateInstance(& ptrCall);
					if(SUCCEEDED(hr)) {
						ptrCall->SetCallID(lCallID);
						hr = ptrCall->QueryInterface(Call);
						bResult = SUCCEEDED(hr);
					}
				}
			} catch (...) {
			}
		}

		free(pResult);

		if(! bResult) {
			return AtlReportError(GetObjectCLSID(), L"No call response was returned by Skype", GUID_NULL, E_FAIL);
		} else {
			return hr;
		}
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}


STDMETHODIMP SkypeAPILib::Access::SearchForUser(BSTR UserName, IUserCollection **List)
{
	CComObject<UserCollection> * ptrUsers;

	HRESULT hr = CComObject<UserCollection>::CreateInstance(& ptrUsers);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create user list", GUID_NULL, hr);
	}

	try {
		wstring wstrCmd = L"SEARCH USERS ";
		wstrCmd += UserName;
		WCHAR * pStrUsers = gSkypeQueue.ExecuteBlockingCommand(wstrCmd.c_str(), L"USERS ");
		ptrUsers->SetItemList(& pStrUsers[6]);
		free(pStrUsers);
		return ptrUsers->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetCallList(ICallCollection ** List)
{
	CComObject<CallCollection> * ptrCalls;

	HRESULT hr = CComObject<CallCollection>::CreateInstance(& ptrCalls);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create call list", GUID_NULL, hr);
	}

	try {
#ifdef _NO_USE_LIST
		WCHAR * pStrCalls = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH CALLS", L"CALLS");
		ptrCalls->SetItemList(& pStrCalls[6]);
		free(pStrCalls);
#else
		ptrCalls->SetItemList(gSkypeQueue.GetCallList());
#endif
		return ptrCalls->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

}

STDMETHODIMP SkypeAPILib::Access::GetFriendList(IUserCollection **List)
{
	CComObject<UserCollection> * ptrUsers;

	HRESULT hr = CComObject<UserCollection>::CreateInstance(& ptrUsers);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create user list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrUsers = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH FRIENDS", L"USERS ");
		ptrUsers->SetItemList(& pStrUsers[6]);
		free(pStrUsers);
		HRESULT hr = ptrUsers->QueryInterface(List);
		return hr;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}



STDMETHODIMP SkypeAPILib::Access::GetMessage(LONG MessageID, IMessage **Message)
{
	CComObject<SkypeAPILib::Message> * ptrMessage;
	
	HRESULT hr = CComObject<SkypeAPILib::Message>::CreateInstance(& ptrMessage);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create message object", GUID_NULL, E_FAIL);
	}

	try {
		ptrMessage->SetMessageID(MessageID);
		return ptrMessage->QueryInterface(Message);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}
STDMETHODIMP SkypeAPILib::Access::GetActiveCallList(ICallCollection **List)
{
	CComObject<CallCollection> * ptrCalls;

	HRESULT hr = CComObject<CallCollection>::CreateInstance(& ptrCalls);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create call list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrCalls = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH ACTIVECALLS", L"CALLS");
		ptrCalls->SetItemList(& pStrCalls[6]);
		free(pStrCalls);
		return ptrCalls->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}


STDMETHODIMP SkypeAPILib::Access::get_ConnectionStatus(SkypeConnectionStatus *pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(L"GET CONNSTATUS", L"CONNSTATUS");
		*pVal = TextToSkypeConnectionStatus(& pResult[11]);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::get_AudioIn(BSTR *pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(L"GET AUDIO_IN", L"AUDIO_IN");
		*pVal = SysAllocString(& pResult[9]);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::put_AudioIn(/*[in]*/ BSTR Val)
{
	try {
		wstring wstrCmd = L"SET AUDIO_IN ";
		wstrCmd += Val;
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(wstrCmd.c_str(), L"AUDIO_IN");
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::get_VideoIn(BSTR *pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(L"GET VIDEO_IN", L"VIDEO_IN");
		*pVal = SysAllocString(& pResult[9]);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::put_VideoIn(/*[in]*/ BSTR Val)
{
	try {
		wstring wstrCmd = L"SET VIDEO_IN ";
		wstrCmd += Val;
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(wstrCmd.c_str(), L"VIDEO_IN");
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::get_AudioOut(BSTR *pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(L"GET AUDIO_OUT", L"AUDIO_OUT");
		*pVal = SysAllocString(& pResult[10]);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::put_AudioOut(/*[in]*/ BSTR Val)
{
	try {
		wstring wstrCmd = L"SET AUDIO_OUT ";
		wstrCmd += Val;
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(wstrCmd.c_str(), L"AUDIO_OUT");
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::get_CurrentUserHandle(BSTR *pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(L"GET CURRENTUSERHANDLE", L"CURRENTUSERHANDLE");
		*pVal = SysAllocString(& pResult[18]);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::get_CurrentUserStatus(SkypeOnlineStatus *pVal)
{
	try {
		gSkypeQueue.SupportedProtocol();
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(L"GET USERSTATUS", L"USERSTATUS");
		*pVal = TextToSkypeOnlineStatus(& pResult[11]);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::put_CurrentUserStatus(SkypeOnlineStatus newVal)
{
	WCHAR * cmd;
	switch(newVal) {
		case olsOffline:
			cmd = L"SET USERSTATUS OFFLINE";
			break;
		case olsOnline:
			cmd = L"SET USERSTATUS ONLINE";
			break;
		case olsAway:
			cmd = L"SET USERSTATUS AWAY";
			break;
		case olsNotAvail:
			cmd = L"SET USERSTATUS NA";
			break;
		case olsDoNotDisturb:
			cmd = L"SET USERSTATUS DND";
			break;
		case olsInvisible:
			cmd = L"SET USERSTATUS INVISIBLE";
			break;
		case olsSkypeMe:
			cmd = L"SET USERSTATUS SKYPEME";
			break;
		case olsLoggedOut:
			cmd = L"SET USERSTATUS LOGGEDOUT";
			break;
		default:
			return AtlReportError(GetObjectCLSID(), L"Invalid User Status Specified", GUID_NULL, E_INVALIDARG);
	}

	try {
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(cmd, L"USER");
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::get_Mute(VARIANT_BOOL *pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(L"GET MUTE", L"MUTE");
		*pVal = (_wcsicmp(& pResult[5], L"ON") == 0);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::put_Mute(VARIANT_BOOL newVal)
{
	WCHAR * cmd = newVal ? L"SET MUTE ON" : L"SET MUTE OFF";
	WCHAR * expectedResult = newVal ? L"MUTE ON" : L"MUTE OFF";

	try {
// 11.02.26 Return値が文字化けしている場合があるので結果は判別しない
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(cmd, L"MUTE");
		//bool bOk = _wcsicmp(pResult, expectedResult) == 0;
		free(pResult);
		//return bOk ? S_OK : AtlReportError(GetObjectCLSID(), L"Mute can only be set when a call is in progress", GUID_NULL, E_FAIL);
		return S_OK;
	} catch(const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::SendMessage(BSTR UserName, BSTR Text, IMessage ** Message)
{
	HRESULT hr = S_OK;
	bool bOk = true;

	try {
		wstring wstrCmd = L"MESSAGE ";
		wstrCmd += UserName;
		wstrCmd += L" ";
		wstrCmd += Text;
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(wstrCmd.c_str(), L"MESSAGE");
		long lMessageID = 0;
		if(swscanf_s(pResult, L"MESSAGE %d ", & lMessageID) != 1) {
			if(swscanf_s(pResult, L"CHATMESSAGE %d ", & lMessageID) != 1) {
				bOk = false;
			}
		}

		if(bOk) {
			if(lMessageID > 0) {
				CComObject<SkypeAPILib::Message> * ptrMessage;
				hr = CComObject<SkypeAPILib::Message>::CreateInstance(& ptrMessage);
				if(SUCCEEDED(hr)) {
					ptrMessage->SetMessageID(lMessageID);
					hr = ptrMessage->QueryInterface(Message);
				}
			}
		}

		free(pResult);
		
		if(! bOk) {
			return AtlReportError(GetObjectCLSID(), L"Unexpected results after sending message", GUID_NULL, E_FAIL);
		} else {
			return hr;
		}
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}



STDMETHODIMP SkypeAPILib::Access::Connect()
{
	gSkypeQueue.Initialize();
	return S_OK;
}

STDMETHODIMP SkypeAPILib::Access::GetMissedCallList(ICallCollection **List)
{
	CComObject<CallCollection> * ptrCalls;

	HRESULT hr = CComObject<CallCollection>::CreateInstance(& ptrCalls);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create call list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrCalls = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH MISSEDCALLS", L"CALLS");
		ptrCalls->SetItemList(& pStrCalls[6]);
		free(pStrCalls);
		return ptrCalls->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetMessageList(IMessageCollection **List)
{
	CComObject<MessageCollection> * ptrMessages;

	HRESULT hr = CComObject<MessageCollection>::CreateInstance(& ptrMessages);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create messages list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrMessages = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH MESSAGES", L"MESSAGES");
		ptrMessages->SetItemList(& pStrMessages[9]);
		free(pStrMessages);
		return ptrMessages->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetMissedMessageList(IMessageCollection **List)
{
	CComObject<MessageCollection> * ptrMessages;

	HRESULT hr = CComObject<MessageCollection>::CreateInstance(& ptrMessages);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create messages list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrMessages = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH MISSEDMESSAGES", L"MESSAGES");
		ptrMessages->SetItemList(& pStrMessages[9]);
		free(pStrMessages);
		return ptrMessages->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::SearchForMessages(BSTR UserName, IMessageCollection **List)
{
	CComObject<MessageCollection> * ptrMessages;

	HRESULT hr = CComObject<MessageCollection>::CreateInstance(& ptrMessages);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create message list", GUID_NULL, hr);
	}

	try {
		WCHAR cmd[_MAX_LINE];
		wcscpy_s(cmd, _MAX_LINE, L"SEARCH MESSAGES ");
		wcsncat_s(cmd, _MAX_LINE, UserName, 1000);
		WCHAR * pStrMessages = gSkypeQueue.ExecuteBlockingCommand(cmd, L"MESSAGES");
		ptrMessages->SetItemList(& pStrMessages[9]);
		free(pStrMessages);
		return ptrMessages->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::SearchForChatMessages(BSTR UserName, IChatMessageCollection **List)
{
	CComObject<ChatMessageCollection> * ptrChatMessages;

	HRESULT hr = CComObject<ChatMessageCollection>::CreateInstance(& ptrChatMessages);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create chat message list", GUID_NULL, hr);
	}

	try {
		WCHAR cmd[_MAX_LINE];
		wcscpy_s(cmd, _MAX_LINE, L"SEARCH CHATMESSAGES ");
		wcsncat_s(cmd, _MAX_LINE, UserName, 1000);
		WCHAR * pStrMessages = gSkypeQueue.ExecuteBlockingCommand(cmd, L"CHATMESSAGES");
		ptrChatMessages->SetItemList(& pStrMessages[9]);
		free(pStrMessages);
		return ptrChatMessages->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}


STDMETHODIMP SkypeAPILib::Access::get_Version(BSTR *pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(L"GET SKYPEVERSION", L"SKYPEVERSION");
		if(wcslen(pResult) < 14)
			*pVal = SysAllocString(L"");
		else
			*pVal = SysAllocString(& pResult[13]);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}	
}

STDMETHODIMP SkypeAPILib::Access::get_Privilege(BSTR Name, VARIANT_BOOL *pVal)
{
	bool bOk = false;
	if(Name != NULL) {
		bOk = wcslen(Name) > 0;
	}

	if(! bOk) {
		return AtlReportError(GetObjectCLSID(), L"No Privelege Name Specified", GUID_NULL, E_FAIL);
	}

	try {
		wstring wstrCmd = L"GET PRIVILEGE ";
		wstrCmd += Name;

		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(wstrCmd.c_str(), L"PRIVILEGE");
		*pVal = false;
		if(wcslen(pResult) > wstrCmd.length()) {
			if(wcsstr(pResult, L"TRUE") != NULL) {
				*pVal = true;
			}
		}
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetUser(BSTR Handle, IUser **User)
{
	CComObject<SkypeAPILib::User> * ptrUser;
	
	HRESULT hr = CComObject<SkypeAPILib::User>::CreateInstance(& ptrUser);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create user object", GUID_NULL, E_FAIL);
	}

	try {
		ptrUser->SetUserName(Handle);
		return ptrUser->QueryInterface(User);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::get_SkypeExecutablePath(BSTR *pVal)
{
	char strSkypePath[_MAX_PATH] = "";
	DWORD dwType, dwSize = _MAX_PATH;
	bool bFound = false;
	bool bInvalid = false;
	HKEY hKey;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Skype\\Phone"), 0, KEY_READ, & hKey) == ERROR_SUCCESS) {
		if((RegQueryValueEx(hKey, _T("SkypePath"), 0, & dwType, (LPBYTE) strSkypePath, & dwSize) == ERROR_SUCCESS) && (dwType == REG_SZ)) {
			_finddata_t data;
			intptr_t lHndl = _findfirst(strSkypePath, & data);
			if(lHndl != -1) {
				bFound = true;
				_findclose(lHndl);
			} else {
				bInvalid = true;
			}
		}
	}

	RegCloseKey(hKey);

	*pVal = SysAllocString(CT2W(strSkypePath));
	if(bInvalid) {
		return AtlReportError(GetObjectCLSID(), L"The specified executable file was not found", GUID_NULL, E_FAIL);
	} else if (! bFound) {
		return AtlReportError(GetObjectCLSID(), L"Skype does not appear to be installed", GUID_NULL, E_FAIL);
	} else {
		return S_OK;
	}
}

STDMETHODIMP SkypeAPILib::Access::get_Protocol(long *pVal)
{
	try {
		*pVal = gSkypeQueue.SupportedProtocol();
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}	
}

STDMETHODIMP SkypeAPILib::Access::put_Protocol(long lngVal)
{
	try {
		WCHAR strCmd[64];
		swprintf_s(strCmd, 64, L"PROTOCOL %d", lngVal);

		gSkypeQueue.SendCommand(strCmd);
		return S_OK;
	} catch(const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetChatList(IChatCollection **List)
{
	CComObject<ChatCollection> * ptrChats;

	CHECK_PROTOCOL(3)

	HRESULT hr = CComObject<ChatCollection>::CreateInstance(& ptrChats);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create chat list", GUID_NULL, hr);
	}

	try {
		WCHAR cmd[_MAX_LINE];
		wcscpy_s(cmd, _MAX_LINE, L"SEARCH CHATS");
		WCHAR * pStrChats = gSkypeQueue.ExecuteBlockingCommand(cmd, L"CHATS ");
		ptrChats->SetItemList(& pStrChats[6]);
		free(pStrChats);
		return ptrChats->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetChat(BSTR Name, IChat **Chat)
{
	CComObject<SkypeAPILib::Chat> * ptrChat;

	CHECK_PROTOCOL(3)

	HRESULT hr = CComObject<SkypeAPILib::Chat>::CreateInstance(& ptrChat);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create Chat object", GUID_NULL, E_FAIL);
	}

	try {
		ptrChat->SetName(Name);
		return ptrChat->QueryInterface(Chat);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetChatMessageList(IChatMessageCollection **List)
{
	CComObject<ChatMessageCollection> * ptrMessages;

	CHECK_PROTOCOL(3)

	HRESULT hr = CComObject<ChatMessageCollection>::CreateInstance(& ptrMessages);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create chat message list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrMessages = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH CHATMESSAGES", L"MESSAGES ");
		ptrMessages->SetItemList(& pStrMessages[13]);
		free(pStrMessages);
		return ptrMessages->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetMissedChatMessageList(IChatMessageCollection **List)
{
	CComObject<ChatMessageCollection> * ptrMessages;

	CHECK_PROTOCOL(3)

	HRESULT hr = CComObject<ChatMessageCollection>::CreateInstance(& ptrMessages);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create chat message list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrMessages = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH MISSEDCHATMESSAGES", L"MESSAGES ");
		ptrMessages->SetItemList(& pStrMessages[13]);
		free(pStrMessages);
		return ptrMessages->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetChatMessage(LONG ID, IChatMessage **Message)
{
	CComObject<ChatMessage> * ptrChatMessage;

	CHECK_PROTOCOL(3)

	HRESULT hr = CComObject<ChatMessage>::CreateInstance(& ptrChatMessage);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create Chat object", GUID_NULL, E_FAIL);
	}

	try {
		ptrChatMessage->SetID(ID);
		return ptrChatMessage->QueryInterface(Message);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::CloseSkype()
{
	gSkypeQueue.CloseSkype();
	return S_OK;
}

STDMETHODIMP SkypeAPILib::Access::MinimizeSkype()
{
	gSkypeQueue.MinimizeSkype();
	return S_OK;
}

STDMETHODIMP SkypeAPILib::Access::get_SkypeOutBalance(LONG *pVal)
{
	CHECK_PROTOCOL(3)

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"PSTN_BALANCE");
		*pVal = _wtol(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::get_SkypeOutCurrency(SkypeCurrencyType *pVal)
{
	CHECK_PROTOCOL(3)

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"PSTN_BALANCE_CURRENCY");
		*pVal = TextToSkypeCurrencyType(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::get_APIAttachmentStatus(/*[out, retval]*/SkypeAPIAttachmentStatus * APIStatus)
{
	*APIStatus = gSkypeQueue.GetAPIStatus();
	return S_OK;
}

STDMETHODIMP SkypeAPILib::Access::LaunchChat(/*[in]*/ SAFEARRAY ** UserHandles, /*[out, retval]*/ IChat ** Chat)
{
	CHECK_PROTOCOL(3)

	VARTYPE vt = VT_UNKNOWN;
	SafeArrayGetVartype(* UserHandles, & vt);
	UINT uiDim = SafeArrayGetDim(* UserHandles);
	if((uiDim != 1) || (vt != VT_BSTR)) {
		return AtlReportError(GetObjectCLSID(), L"Array of users must be single dimension array of strings", GUID_NULL, E_INVALIDARG);
	}

	wstring wstrCommand;
	// wstrCommand = L"OPEN CHAT ";
	wstrCommand = L"CHAT CREATE ";
	HRESULT hr = SafeArrayLock(* UserHandles);
	if(SUCCEEDED(hr)) {
		BSTR * pHandles = (BSTR *) (*UserHandles)->pvData;
		for(ULONG l = 0; l < (*UserHandles)->rgsabound[0].cElements; l++) {
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
			CComObject<SkypeAPILib::Chat> * ptrChat;
			WCHAR * wstrResult = gSkypeQueue.ExecuteBlockingCommand(wstrCommand.c_str(), L"CHAT");
			hr = CComObject<SkypeAPILib::Chat>::CreateInstance(& ptrChat);
			if(SUCCEEDED(hr)) {
				if(wcslen(wstrResult) < 10) {
					hr = AtlReportError(GetObjectCLSID(), L"Error parsing launch chat result", GUID_NULL, E_FAIL);
				} else {
					WCHAR * wstrEnd = wcschr(& wstrResult[5], L' ');
					if(wstrEnd == NULL) {
						hr = AtlReportError(GetObjectCLSID(), L"Error parsing launch chat result", GUID_NULL, E_FAIL);
					} else {
						*wstrEnd = 0;
						ptrChat->SetName(& wstrResult[5]);
						hr = ptrChat->QueryInterface(Chat);
					}
				}
			}
			
			free(wstrResult);
		} catch (const WCHAR * err) {
			return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
		}

		if(SUCCEEDED(hr)) {
			return S_OK;
		} else {
			return AtlReportError(GetObjectCLSID(), L"Unable to Start Chat", GUID_NULL, hr);
		}
	} else {
		return AtlReportError(GetObjectCLSID(), L"Unable to Start Chat", GUID_NULL, hr);
	}
}

STDMETHODIMP SkypeAPILib::Access::PlaceConferenceCall(/*[in]*/ SAFEARRAY ** UserHandles, /*[out, retval]*/ ICall ** Call)
{
	CHECK_PROTOCOL(3)

	VARTYPE vt = VT_UNKNOWN;
	SafeArrayGetVartype(* UserHandles, & vt);
	UINT uiDim = SafeArrayGetDim(* UserHandles);
	if((uiDim != 1) || (vt != VT_BSTR)) {
		return AtlReportError(GetObjectCLSID(), L"Array of users must be single dimension array of strings", GUID_NULL, E_INVALIDARG);
	}

	wstring wstrCommand;
	wstrCommand = L"CALL ";
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

	WCHAR * pResult;
	if(SUCCEEDED(hr)) {
		try {
			pResult = gSkypeQueue.ExecuteBlockingCommand(wstrCommand.c_str(), L"CALL", TRUE);
		} catch (const WCHAR * err) {
			return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
		}
		if(pResult == NULL) {
			return AtlReportError(GetObjectCLSID(), L"No response returned", GUID_NULL, E_FAIL);
		} else {
			long lCallID = 0;
			if(swscanf_s(pResult, L"CALL %d ", & lCallID) == 1) {
				if(lCallID > 0) {
					try {
						CComObject<SkypeAPILib::Call> * ptrCall;
						hr = CComObject<SkypeAPILib::Call>::CreateInstance(& ptrCall);
						if(SUCCEEDED(hr)) {
							ptrCall->SetCallID(lCallID);
							hr = ptrCall->QueryInterface(Call);
						}
					} catch(...) {
						return AtlReportError(GetObjectCLSID(), L"Unable to create call object", GUID_NULL, hr);
					}
				} else {
					return AtlReportError(GetObjectCLSID(), L"Error parsing call results", GUID_NULL, E_FAIL);
				}
			} else {
				return AtlReportError(GetObjectCLSID(), L"Error parsing call results", GUID_NULL, E_FAIL);
			}
				try {
					free(pResult);
				} catch (const WCHAR * err) {
					return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
				}
		}
	}

	if(SUCCEEDED(hr)) {
		return S_OK;
	} else {
		return AtlReportError(GetObjectCLSID(), L"Unable to Place Conference Call", GUID_NULL, hr);
	}
}

STDMETHODIMP SkypeAPILib::Access::OpenIMDialog(/*[in]*/ BSTR UserHandle, /*[in]*/ BSTR Message)
{
	try {
		wstring wstrCommand;
		wstrCommand = L"OPEN IM ";
		wstrCommand += UserHandle;
		wstrCommand += L" ";
		wstrCommand += Message;

		gSkypeQueue.SendCommand(wstrCommand.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::FocusSkypeWindow()
{
	try {
		gSkypeQueue.SendCommand(L"FOCUS");
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::LaunchAddAFriend(/*[in]*/ BSTR UserHandle)
{
	try {
		wstring wstrCommand;
		wstrCommand = L"OPEN ADDAFRIEND ";
		wstrCommand += UserHandle;
		gSkypeQueue.SendCommand(wstrCommand.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::get_Conversion(/*[out, retval]*/IConversion ** pVal)
{
	HRESULT hr;

	if(m_objConversion == NULL) {
		hr = CComObject<Conversion>::CreateInstance(& m_objConversion);
		if(FAILED(hr)) {
			return AtlReportError(GetObjectCLSID(), L"Unable to create conversion object", GUID_NULL, hr);
		}
		m_objConversion->AddRef();
	}

	hr = m_objConversion->QueryInterface(pVal);
	if(SUCCEEDED(hr)) {
		return S_OK;
	} else {
		return AtlReportError(GetObjectCLSID(), L"Unable to return conversion object", GUID_NULL, hr);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetActiveChatList(/*[out, retval]*/ IChatCollection ** List)
{
	CComObject<SkypeAPILib::ChatCollection> * ptrChats;

	CHECK_PROTOCOL(5)

	HRESULT hr = CComObject<SkypeAPILib::ChatCollection>::CreateInstance(& ptrChats);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create chat list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrChats = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH ACTIVECHATS", L"CHATS ");
		ptrChats->SetItemList(& pStrChats[6]);
		free(pStrChats);
		return ptrChats->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetMissedChatList(/*[out, retval]*/ IChatCollection ** List)
{
	CComObject<SkypeAPILib::ChatCollection> * ptrChats;

	CHECK_PROTOCOL(5)

	HRESULT hr = CComObject<SkypeAPILib::ChatCollection>::CreateInstance(& ptrChats);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create chat list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrChats = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH MISSEDCHATS", L"CHATS ");
		ptrChats->SetItemList(& pStrChats[6]);
		free(pStrChats);
		return ptrChats->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetRecentChatList(/*[out, retval]*/ IChatCollection ** List)
{
	CComObject<SkypeAPILib::ChatCollection> * ptrChats;

	CHECK_PROTOCOL(5)

	HRESULT hr = CComObject<SkypeAPILib::ChatCollection>::CreateInstance(& ptrChats);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create chat list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrChats = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH RECENTCHATS", L"CHATS ");
		ptrChats->SetItemList(& pStrChats[6]);
		free(pStrChats);
		return ptrChats->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetBookmarkedChatList(/*[out, retval]*/ IChatCollection ** List)
{
	CComObject<SkypeAPILib::ChatCollection> * ptrChats;

	CHECK_PROTOCOL(5)

	HRESULT hr = CComObject<SkypeAPILib::ChatCollection>::CreateInstance(& ptrChats);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create chat list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrChats = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH BOOKMARKEDCHATS", L"CHATS ");
		ptrChats->SetItemList(& pStrChats[6]);
		free(pStrChats);
		return ptrChats->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetVoiceMailList(/*[out, retval]*/ IVoiceMailCollection ** List)
{
	CComObject<SkypeAPILib::VoiceMailCollection> * ptrVoiceMails;

	CHECK_PROTOCOL(5)

	HRESULT hr = CComObject<SkypeAPILib::VoiceMailCollection>::CreateInstance(& ptrVoiceMails);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create voice mail list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrVoiceMails = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH VOICEMAILS", L"VOICEMAILS ");
		ptrVoiceMails->SetItemList(& pStrVoiceMails[11]);
		free(pStrVoiceMails);
		return ptrVoiceMails->QueryInterface(List);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::SendVoiceMail(/*[in]*/ BSTR UserHandle, /*[out, retval]*/ IVoiceMail ** pVal)
{
	CComObject<SkypeAPILib::VoiceMail> * ptrCall;
	
	CHECK_PROTOCOL(5)

	HRESULT hr = CComObject<SkypeAPILib::VoiceMail>::CreateInstance(& ptrCall);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create call object", GUID_NULL, E_FAIL);
	}

	try {
		long lVoiceMailID = 0;
		wstring wstrCmd = L"VOICEMAIL ";
		wstrCmd += UserHandle;
		WCHAR * ptrResponse = gSkypeQueue.ExecuteBlockingCommand(wstrCmd.c_str(), L"VOICEMAIL ");
		bool bOk = (swscanf_s(ptrResponse, L"VOICEMAIL %d ", & lVoiceMailID) == 1);
		free(ptrResponse);
		if(! bOk) {
			return AtlReportError(GetObjectCLSID(), L"Unable to extract voicemail ID", GUID_NULL, E_FAIL);
		}

		ptrCall->SetID(lVoiceMailID);
		return ptrCall->QueryInterface(pVal);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::GetUsersWaitingForAuthorization(/*[out, retval]*/ IUserCollection ** pVal)
{
	CComObject<SkypeAPILib::UserCollection> * ptrUsers;

	CHECK_PROTOCOL(5)

	HRESULT hr = CComObject<SkypeAPILib::UserCollection>::CreateInstance(& ptrUsers);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create user list", GUID_NULL, hr);
	}

	try {
		WCHAR * pStrUsers = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH USERSWAITINGMYAUTHORIZATION", L"USERS ");
		ptrUsers->SetItemList(& pStrUsers[6]);
		free(pStrUsers);
		return ptrUsers->QueryInterface(pVal);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::ClearChatHistory()
{
	CHECK_PROTOCOL(5)

	try {
		gSkypeQueue.SendCommand(L"CLEAR CHATHISTORY");
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::ClearVoiceMailHistory()
{
	CHECK_PROTOCOL(5)

	try {
		gSkypeQueue.SendCommand(L"CLEAR VOICEMAILHISTORY");
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::ClearCallHistory(/*[in]*/ SkypeClearCallHistory CallType)
{
	CHECK_PROTOCOL(5)

	wstring wstrCmd = L"CLEAR CALLHISTORY ";
	switch(CallType) {
		case clrAllCalls:
			wstrCmd += L"ALL";
			break;
		case clrMissedCalls:
			wstrCmd += L"MISSED";
			break;
		case clrIncomingCalls:
			wstrCmd += L"INCOMING";
			break;
		case clrOutgoingCalls:
			wstrCmd += L"OUTGOING";
			break;
		default:
			return AtlReportError(GetObjectCLSID(), L"Invalid call type specified", GUID_NULL, E_FAIL);
	}
	
	try {
		gSkypeQueue.SendCommand(wstrCmd.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::get_Ringer(/*[out, retval]*/ BSTR* pVal)
{
	CHECK_PROTOCOL(5)

	try {
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(L"GET RINGER", L"RINGER ");
		*pVal = SysAllocString(& pResult[7]);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::put_Ringer(/*[in]*/ BSTR Val)
{
	CHECK_PROTOCOL(5)

	try {
		wstring wstrCmd = L"SET RINGER ";
		wstrCmd += Val;
		gSkypeQueue.SendCommand(wstrCmd.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP SkypeAPILib::Access::ConnectAndWait(/*[in]*/ LONG TimeoutInSeconds)
{
	gSkypeQueue.Initialize();
	// Wait for either accept, refused or not available
	DWORD dwStart = GetTickCount();
	for(;;) {
		SkypeAPIAttachmentStatus status = gSkypeQueue.GetAPIStatus();
		switch(status) {
			case apiAttachSuccess:
				return S_OK;
			case apiAttachRefused:
				return AtlReportError(GetObjectCLSID(), L"Access to the application was refused", GUID_NULL, E_ACCESSDENIED);
			case apiAttachNotAvailable:
				return AtlReportError(GetObjectCLSID(), L"Application access to Skype unavailable", GUID_NULL, E_ACCESSDENIED);
		}
		if(TimeoutInSeconds > 0) {
			DWORD dwDiff = (GetTickCount() - dwStart) / 1000;
			if(dwDiff >= (DWORD) TimeoutInSeconds) {
				return AtlReportError(GetObjectCLSID(), L"Timeout waiting for Skype to Respond", GUID_NULL, E_FAIL);
			}
		}
		Sleep(500);
	};
}

STDMETHODIMP SkypeAPILib::Access::OpenFileTransferDialog(/*[in]*/ SAFEARRAY ** UserHandles, /*[in]*/ BSTR Folder)
{
	VARTYPE vt = VT_UNKNOWN;
	SafeArrayGetVartype(* UserHandles, & vt);
	UINT uiDim = SafeArrayGetDim(* UserHandles);
	if((uiDim != 1) || (vt != VT_BSTR)) {
		return AtlReportError(GetObjectCLSID(), L"Array of users must be single dimension array of strings", GUID_NULL, E_INVALIDARG);
	}

	wstring wstrCommand;
	wstrCommand = L"OPEN FILETRANSFER ";
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

		if(Folder != NULL) {
			if(wcslen(Folder) > 0) {
				_wfinddata_t data;
				intptr_t h = _wfindfirst(Folder, & data);
				if(h == -1) {
					return AtlReportError(GetObjectCLSID(), L"The directory specified is invalid", GUID_NULL, hr);
				}
				_findclose(h);

				wstrCommand += L" IN ";
				wstrCommand += Folder;
			}
		}
		gSkypeQueue.SendCommand(wstrCommand.c_str());
		return S_OK;
	} else {
		return AtlReportError(GetObjectCLSID(), L"Unable to open file transfer dialog", GUID_NULL, hr);
	}
}

STDMETHODIMP SkypeAPILib::Access::SendBlockingCommand(BSTR Command, BSTR * pVal)
{
	CHECK_PROTOCOL(4)

	try {
		LPWSTR wstrResponse = gSkypeQueue.ExecuteBlockingCommand(Command, L"");
		*pVal = SysAllocString(wstrResponse);
		free(wstrResponse);

		if(_wcsnicmp(*pVal, L"ERROR ", 6) == 0) {
			return InError(*pVal, GUID_NULL, E_FAIL);
		}
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
	return S_OK;
}

STDMETHODIMP SkypeAPILib::Access::get_CommandTimeout(/*[out, retval]*/ LONG *pVal) 
{
	*pVal = gSkypeQueue.GetTimeout();
	return S_OK;
}

STDMETHODIMP SkypeAPILib::Access::put_CommandTimeout(/*[in]*/ LONG Val) 
{
	if(Val <= 0) {
		return AtlReportError(GetObjectCLSID(), L"Timeout must be greater than 0", GUID_NULL, E_INVALIDARG);
	}
	gSkypeQueue.SetTimeout(Val);
	return S_OK;
}

void SkypeAPILib::Access::ProcessCommand(LPCWSTR Command)
{
	m_wndEvent.PostMessage(WM_FIRE_COMMAND, 0, (LPARAM) _wcsdup(Command));
}

void SkypeAPILib::Access::ProcessResponse(LPCWSTR Response, BOOL ProcessCallbacks)
{

#ifdef _DEBUG
	_TCHAR str[_MAX_LINE];
	_stprintf_s(str, _MAX_LINE, _T("<< %s\n"), CW2CT(Response));
	WriteLog(str);
	ATLTRACE("<< %s\n", CW2CT(Response));
#endif

	m_wndEvent.PostMessage(WM_FIRE_RESULT, 0, (LPARAM) _wcsdup(Response));

	if(! ProcessCallbacks) {
		return;
	}

	WCHAR * ptrOnlineStatus = NULL;
	WCHAR * ptrOnlineStatusInd = wcsstr((LPWSTR)Response, L" ONLINESTATUS ");
	if(ptrOnlineStatusInd != NULL) {
		ptrOnlineStatus = & ptrOnlineStatusInd[14];
	}

	WCHAR * ptrBuffer1 = new WCHAR[wcslen(Response)];
	WCHAR * ptrBuffer2 = new WCHAR[wcslen(Response)];

	if(_wcsnicmp(Response, L"CALL ", 5) == 0) {

		long lCallID = 0;
		if(swscanf_s(Response, L"CALL %d STATUS", & lCallID) == 1) {

			if(lCallID > 0) {
			
				WCHAR * ptrStatus = NULL;
				WCHAR * ptrStatusInd = wcsstr((LPWSTR)Response, L" STATUS ");
				if(ptrStatusInd != NULL) {
					ptrStatus = & ptrStatusInd[8];
				}

				WCHAR * ptrDTMF = NULL;
				WCHAR * ptrDTMFInd = wcsstr((LPWSTR)Response, L" DTMF ");
				if(ptrDTMFInd != NULL) {
					ptrDTMF = & ptrDTMFInd[6];
				}
			
				if((ptrStatusInd != NULL) && (ptrStatus != NULL)) {
					SkypeCallProgress progress = TextToSkypeCallProgress(ptrStatus);
					m_wndEvent.PostMessage(WM_FIRE_CALLSTATUSCHANGED, progress, lCallID);
				} else if((ptrDTMFInd != NULL) && (ptrDTMF != NULL)) {
					m_wndEvent.PostMessage(WM_FIRE_INCOMINGDTMF, ((LPARAM) _wcsdup(ptrDTMF)), lCallID);
				}
			}
		}
	} else if((_wcsnicmp(Response, L"USER", 4) == 0) && (ptrOnlineStatusInd != NULL) && (ptrOnlineStatus != NULL)) {
		if(swscanf_s(Response, L"USER %s ONLINESTATUS", ptrBuffer1, wcslen(Response)) == 1) {
			if(wcslen(ptrBuffer1) > 0) {
				SkypeOnlineStatus status = TextToSkypeOnlineStatus(ptrOnlineStatus);
				m_wndEvent.PostMessage(WM_FIRE_ONLINESTATUSCHANGED, status, 
					(LPARAM) _wcsdup(ptrBuffer1));
			}
		}
	} else if(_wcsnicmp(Response, L"CONTACTS FOCUSED ", 17) == 0) {
		wstring wstrUser = & Response[17];
		if(wstrUser.length() > 0) {
			m_wndEvent.PostMessage(WM_FIRE_CONTACT_FOCUSED, (WPARAM) _wcsdup(wstrUser.c_str()), 0);
		}
	} else if(_wcsnicmp(Response, L"CALLHISTORYCHANGED ", 19) == 0) {
		m_wndEvent.PostMessage(WM_FIRE_CALLHISTORYCHANGED);
	} else if(_wcsnicmp(Response, L"IMHISTORYCHANGED ", 17) == 0) {
		m_wndEvent.PostMessage(WM_FIRE_IMHISTORYCHANGED);
	} else if(_wcsnicmp(Response, L"USERSTATUS ", 11) == 0) {
		m_wndEvent.PostMessage(WM_FIRE_USERSTATUSCHANGED, TextToSkypeOnlineStatus(& Response[11]));
	} else if(_wcsnicmp(Response, L"CONNSTATUS ", 11) == 0) {
		SkypeConnectionStatus status = TextToSkypeConnectionStatus(& Response[11]);
		if(status != conOnline) {
			gSkypeQueue.DeleteAllBinaryTransfers();
		}
		m_wndEvent.PostMessage(WM_FIRE_CONNECTIONSTATUSCHANGED, status);
	} else if(_wcsnicmp(Response, L"MUTE ", 5) == 0) {
		m_wndEvent.PostMessage(WM_FIRE_MUTECHANGED, _wcsicmp(& Response[5], L"ON") == 0);
	} else if((_wcsnicmp(Response, L"MESSAGE", 7) == 0) && (wcsstr(Response, L"STATUS RECEIVED") != NULL)) {
		long lMessageID = 0;
		if(swscanf_s(Response, L"MESSAGE %d ", & lMessageID) == 1) {
			m_wndEvent.PostMessage(WM_FIRE_MESSAGERECEIVED, 0, lMessageID);
		}
	} else if((_wcsnicmp(Response, L"MESSAGE", 7) == 0) && (wcsstr(Response, L"STATUS SENT") != NULL)) {
		long lMessageID = 0;
		if(swscanf_s(Response, L"MESSAGE %d ", & lMessageID) == 1) {
			m_wndEvent.PostMessage(WM_FIRE_MESSAGESENT, 0, lMessageID);
		}
	} else if((_wcsnicmp(Response, L"CHATMESSAGE", 11) == 0) && (wcsstr(Response, L"STATUS RECEIVED") != NULL)) {
		long lMessageID = 0;
		if(swscanf_s(Response, L"CHATMESSAGE %d ", & lMessageID) == 1) {
			m_wndEvent.PostMessage(WM_FIRE_CHATMESSAGERECEIVED, 0, lMessageID);
		}
	} else if((_wcsnicmp(Response, L"CHATMESSAGE", 11) == 0) && (wcsstr(Response, L"STATUS SENT") != NULL)) {
		long lMessageID = 0;
		if(swscanf_s(Response, L"CHATMESSAGE %d ", & lMessageID) == 1) {
			m_wndEvent.PostMessage(WM_FIRE_CHATMESSAGESENT, 0, lMessageID);
		}
	} else if(swscanf_s(Response, L"APPLICATION %s %s", ptrBuffer1, wcslen(Response), ptrBuffer2, wcslen(Response)) == 2) {
		if((wcslen(ptrBuffer1) > 0) && (wcslen(ptrBuffer2) > 0)){
			WCHAR * ptrData = wcsstr((LPWSTR)Response, ptrBuffer2) + wcslen(ptrBuffer2);
			if(*ptrData == L' ') {
				ptrData++;
			}

			if(_wcsnicmp(ptrBuffer2, L"CONNECTING", 10) == 0) {
				WCHAR *next_token = NULL;
				WCHAR * ptrToken = wcstok_s(ptrData, L" ", &next_token);
				while(ptrToken != NULL) {
					m_wndEvent.PostMessage(WM_FIRE_APP_CONNECTING, 
						(WPARAM) _wcsdup(ptrBuffer1), (LPARAM) _wcsdup(ptrToken));
					ptrToken = wcstok_s(NULL, L" ", &next_token);
				}
			} else if(_wcsnicmp(ptrBuffer2, L"DATAGRAM", 8) == 0) {
				wstring strInfo;
				WCHAR * ptrSpace = wcschr(ptrData, L' ');
				if(ptrSpace != NULL) {
					*ptrSpace = 0;
					if(wcsstr(ptrData, L":") != NULL) {
						strInfo = ptrBuffer1;
						strInfo += L"\t";
						strInfo += ptrData;

						m_wndEvent.PostMessage(WM_FIRE_APP_DATAGRAM_RECEIVED,
							(WPARAM) _wcsdup(strInfo.c_str()), 
							(LPARAM) _wcsdup(ptrSpace + 1));
					}
				}
			} else if(_wcsnicmp(ptrBuffer2, L"STREAMS", 7) == 0) {
				StreamList strStreams = gSkypeQueue.GetStreamList();
				StreamList strNewStreams;

				// Build new list
				WCHAR *next_token = NULL;
				WCHAR * ptrToken = wcstok_s(ptrData, L" ", &next_token);
				while(ptrToken != NULL) {
					strNewStreams.push_back(ptrToken);
					ptrToken = wcstok_s(NULL, L" ", &next_token);
				}

				gSkypeQueue.SetStreamList(strNewStreams);

				// Look for newly opened streams
				for(StreamList::iterator it = strNewStreams.begin(); it != strNewStreams.end(); it++) {
					bool bIsNew = true;
					for(StreamList::iterator it2 = strStreams.begin(); it2 != strStreams.end(); it2++) {
						wstring str = it->c_str();
						str += L", ";
						str += it2->c_str();
						// ::MessageBox(NULL, CW2CT(str.c_str()), "Checking for New Stream", MB_OK);
						if(_wcsicmp(it->c_str(), it2->c_str()) == 0) {
							bIsNew = false;
							break;
						}
					}
				
					// Send out notifications for new strings
					if(bIsNew) {
						// ::MessageBox(NULL, CW2CT(it->c_str()), "Adding New Stream", MB_OK);
						m_wndEvent.PostMessage(WM_FIRE_APP_STREAM_OPENED, 
							(WPARAM) _wcsdup(ptrBuffer1), (LPARAM) _wcsdup(it->c_str()));
					}
				}

				// Look for newly closed streams
				for(StreamList::iterator it2 = strStreams.begin(); it2 != strStreams.end(); it2++) {
					bool bIsClosed = true;
					for(StreamList::iterator it = strNewStreams.begin(); it != strNewStreams.end(); it++) {
						wstring str = it->c_str();
						str += L", ";
						str += it2->c_str();
						if(_wcsicmp(it->c_str(), it2->c_str()) == 0) {
							bIsClosed = false;
							break;
						}
					}
				
					// Send out notifications 
					if(bIsClosed) {
						m_wndEvent.PostMessage(WM_FIRE_APP_STREAM_CLOSED, 
							(WPARAM) _wcsdup(ptrBuffer1), (LPARAM) _wcsdup(it2->c_str()));
						gSkypeQueue.DeleteStreamBinaryTransfers(it2->c_str());
					}
				}
			} else if (_wcsnicmp(ptrBuffer2, L"RECEIVED", 8) == 0) {
				WCHAR *next_token = NULL;
				WCHAR * ptrToken = wcstok_s(ptrData, L" ", &next_token);
				while(ptrToken != NULL) {
					WCHAR * ptrToken2 = wcschr(ptrToken, L'=');
					if(ptrToken2 != NULL) {
						WCHAR chHold = *ptrToken2;
						*ptrToken2 = 0;
						wstring wstrCmd = L"ALTER APPLICATION ";
						wstrCmd += ptrBuffer1;
						wstrCmd += L" READ ";
						wstrCmd += ptrToken;
						gSkypeQueue.SendCommand(wstrCmd.c_str());
						*ptrToken2 = chHold;
					}
					ptrToken = wcstok_s(NULL, L" ", &next_token);
				}
			}
		}
	} else if(swscanf_s(Response, L"ALTER APPLICATION %s READ %s", ptrBuffer1, wcslen(Response), ptrBuffer2, wcslen(Response)) == 2) {
		if((wcslen(ptrBuffer1) > 0) && (wcslen(ptrBuffer2) > 0)) {
			WCHAR * pstrData = wcsstr((LPWSTR)Response, ptrBuffer2);
			if(pstrData != NULL) {
				pstrData += (wcslen(ptrBuffer2) + 1);
				if(*pstrData != 0) {
					wstring strInfo;	
					strInfo = ptrBuffer1;
					strInfo += L"\t";
					strInfo += ptrBuffer2;
					
					// Figure out if we have a binary transfer or not
					if(_wcsnicmp(pstrData, L"#XFERSTREAM# ", 12) == 0) {
						wstring wstrCmd;
						wstring wstrTransferID;
						wstring wstrMD5;
						wstring wstrCopy = pstrData;
						long lSize;
						SAFEARRAY * psa;
						BinaryTransferPTR ptrTransfer;

						int iCtr = 0;
						bool bKeepGoing = true;
						bool bStart = false;
						bool bData = false;
						bool bEnd = false;

						pstrData += 12;
						WCHAR *next_token = NULL;
						WCHAR * ptrItem = wcstok_s(pstrData, L" ", &next_token);
						while (bKeepGoing && (ptrItem != NULL)) {
							switch(iCtr) {
								case 0:
									wstrTransferID = ptrItem;
									break;
								case 1:
									wstrCmd = ptrItem;
									if(_wcsicmp(wstrCmd.c_str(), L"START") == 0) {
										bStart = true;
									} else if (_wcsicmp(wstrCmd.c_str(), L"DATA") == 0) {
										bData = true;
									} else if (_wcsicmp(wstrCmd.c_str(), L"END") == 0) {
										bEnd = true;
									} else {
										wstrCmd = L"Unexpected command: " + wstrCmd;
										m_wndEvent.PostMessage(WM_FIRE_BINARY_TRANSFER_ERROR,
											(WPARAM) _wcsdup(wstrTransferID.c_str()), 
											(LPARAM) _wcsdup(wstrCmd.c_str()));
										return;
									}

									if(! bStart) {
										ptrTransfer = gSkypeQueue.GetBinaryTransfer(wstrTransferID.c_str());
										if(ptrTransfer == NULL) {
											wstrCmd = L"Unexpected transfer item: " + wstrCmd;
											m_wndEvent.PostMessage(WM_FIRE_BINARY_TRANSFER_ERROR,
												(WPARAM) _wcsdup(L""), 
												(LPARAM) _wcsdup(wstrCmd.c_str()));
											return;
										}
									}
											
									if(bEnd) {
										if(! ptrTransfer->MD5Match()) {
											m_wndEvent.PostMessage(WM_FIRE_BINARY_TRANSFER_ERROR,
												(WPARAM) _wcsdup(wstrTransferID.c_str()), 
												((LPARAM) _wcsdup(L"MD5 checksum does not match")));
										} else {
											psa = SafeArrayCreateVector(VT_UI1, 0, 
												ptrTransfer->Length());
											SafeArrayLock(psa);
											memcpy(psa->pvData, ptrTransfer->Buffer(), 
												ptrTransfer->Length());
											SafeArrayUnlock(psa);
											m_wndEvent.PostMessage(WM_FIRE_BINARY_TRANSFER_END, 
												(WPARAM) _wcsdup(wstrTransferID.c_str()), (LPARAM) psa);
										}
										gSkypeQueue.EndBinaryTransfer(ptrTransfer->ID(), false);
										return;
									}

									break;
								case 2:
									if(bStart) {
										wstrMD5 = ptrItem;
									} else if (bData) {
										// Anything after the command is data
										size_t idx = wstrCopy.find(L" DATA ");
										// Swap the bytes back
										WCHAR * ptrHold = new WCHAR[wstrCopy.length() + 1];
										wcscpy_s(ptrHold, wstrCopy.length() + 1, & ((wstrCopy.c_str())[idx + 6]));
										for(ULONG i = 0; i < wcslen(ptrHold); i++) {
											ptrHold[i] = WordSwap(ptrHold[i]);
										}

										char * ptrHold2 = (char *) ptrHold;
										BinaryTransferStatus * ptrStatus = gSkypeQueue.AppendBinaryTransfer(wstrTransferID.c_str(), 
											(BYTE *) ptrHold2, (ULONG) strlen(ptrHold2));
										delete ptrHold;
										m_wndEvent.PostMessage(WM_FIRE_BINARY_TRANSFER_DATA,
											(WPARAM) _wcsdup(wstrTransferID.c_str()), 
											(LPARAM) ptrStatus);
										return;
									}
									break;
								case 3:
									if(bStart) {
										lSize = _wtol(ptrItem);
										try {
											gSkypeQueue.StartBinaryTransfer(wstrTransferID.c_str(),
												ptrBuffer1, CW2A(wstrMD5.c_str()), lSize);
											m_wndEvent.PostMessage(WM_FIRE_BINARY_TRANSFER_START,
												(WPARAM) _wcsdup(wstrTransferID.c_str()), 
												(LPARAM) lSize);
										} catch(const WCHAR * err) {
											m_wndEvent.PostMessage(WM_FIRE_BINARY_TRANSFER_ERROR,
												(WPARAM) _wcsdup(wstrTransferID.c_str()), 
												((LPARAM) _wcsdup(err)));
										}
									}
							}
							iCtr++;
							ptrItem = wcstok_s(NULL, L" ", &next_token);
						}
					} else {
						m_wndEvent.PostMessage(WM_FIRE_APP_DATA_RECEIVED,
							(WPARAM) _wcsdup(strInfo.c_str()), (LPARAM) _wcsdup(pstrData));
					}
				}
			}
		}
	} else if(_wcsnicmp(Response, L"ERROR", 5) == 0)  {
		try {
			long lErrorID = 0;
			if(swscanf_s(Response, L"ERROR %d", & lErrorID) == 1) {
				WCHAR * pStrDesc = wcsstr((LPWSTR)Response, L" ");
				pStrDesc = wcsstr(& pStrDesc[1], L" ");
				m_wndEvent.PostMessage(WM_FIRE_ERROR, lErrorID, ((LPARAM) _wcsdup(pStrDesc)));
			}
		} catch(...) {
		}
	}

	delete [] ptrBuffer1;
	delete [] ptrBuffer2;
}

void SkypeAPILib::Access::ProcessAPIStatusChange(SkypeAPIAttachmentStatus Status)
{
	m_wndEvent.PostMessage(WM_FIRE_APISTATUSCHANGED, Status);
}

HRESULT SkypeAPILib::Access::InError(LPCOLESTR lpszDesc, const IID &iid,  HRESULT hres)
{
	return CComCoClass<Access>::Error(lpszDesc, iid, hres);
}

HRESULT SkypeAPILib::Access::InError(LPCTSTR lpszDesc, const IID &iid,  HRESULT hres)
{
	return CComCoClass<Access>::Error(lpszDesc, iid, hres);
}


HRESULT SkypeAPILib::Access::OpenApplication(BSTR ApplicationName, IApp2App ** App)
{
	CComObject<App2App> * ptrApp;
	
	HRESULT hr = CComObject<App2App>::CreateInstance(& ptrApp);
	if(FAILED(hr)) {
		return AtlReportError(GetObjectCLSID(), L"Unable to create app2app object", GUID_NULL, E_FAIL);
	}

	try {
		ptrApp->SetAppName(ApplicationName);
	} catch(const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

	try {
		// If we get an error when trying to get "CONNECTABLE", then
		// the application needs to get created
		CComPtr<IUserCollection> ptrUsers;
		hr = ptrApp->get_ConnectableUsers(& ptrUsers);
	} catch(...) {
		hr = E_FAIL;
	}

	if(FAILED(hr)) {

		try {
			ptrApp->CreateApplication();
		} catch(const WCHAR * err) {
			return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
		}
	}

	return ptrApp->QueryInterface(App);
}

STDMETHODIMP SkypeAPILib::Access::get_Profile(/*[out, retval]*/IProfile ** pVal)
{
	HRESULT hr;

	if(m_objProfile == NULL) {
		hr = CComObject<Profile>::CreateInstance(& m_objProfile);
		if(FAILED(hr)) {
			return AtlReportError(GetObjectCLSID(), L"Unable to create profile object", GUID_NULL, hr);
		}
		m_objProfile->AddRef();
	}

	hr = m_objProfile->QueryInterface(pVal);
	if(SUCCEEDED(hr)) {
		return S_OK;
	} else {
		return AtlReportError(GetObjectCLSID(), L"Unable to return profile object", GUID_NULL, hr);
	}
}

STDMETHODIMP SkypeAPILib::Access::get_UI(/*[out, retval]*/IUI ** pVal)
{
	HRESULT hr;

	if(m_objUI == NULL) {
		hr = CComObject<UI>::CreateInstance(& m_objUI);
		if(FAILED(hr)) {
			return AtlReportError(GetObjectCLSID(), L"Unable to create UI object", GUID_NULL, hr);
		}
		m_objUI->AddRef();
	}

	hr = m_objUI->QueryInterface(pVal);
	if(SUCCEEDED(hr)) {
		return S_OK;
	} else {
		return AtlReportError(GetObjectCLSID(), L"Unable to return UI object", GUID_NULL, hr);
	}
}
