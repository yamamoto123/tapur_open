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
// UI.cpp : Implementation of UI

#include "stdafx.h"
#include ".\enums.h"
#include ".\skypemessagequeue.h"
#include ".\ui.h"

// UI
HRESULT UI::Execute(LPCWSTR Dialog)
{
	try {
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(Dialog, Dialog);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP UI::OpenProfile(void)
{
	CHECK_PROTOCOL(5)
	return Execute(L"OPEN PROFILE");
}

STDMETHODIMP UI::OpenUserInfo(BSTR UserName)
{
	CHECK_PROTOCOL(5)

	if(wcslen(UserName) < 1) {
		return AtlReportError(GetObjectCLSID(), L"This function requires a value to be passed", 
			GUID_NULL, E_INVALIDARG);
	}

	wstring wstrCmd = L"OPEN USERINFO ";
	wstrCmd += UserName;
	return Execute(wstrCmd.c_str());
}

STDMETHODIMP UI::OpenConference(void)
{
	CHECK_PROTOCOL(5)
	return Execute(L"OPEN CONFERENCE");
}

STDMETHODIMP UI::OpenSearch(void)
{
	CHECK_PROTOCOL(5)
	return Execute(L"OPEN SEARCH");
}

STDMETHODIMP UI::OpenOptions(SkypeOptionsPage Page)
{
	CHECK_PROTOCOL(5)
	wstring wstrCmd = L"OPEN OPTIONS ";
	switch(Page) {
		case pgGeneral:
			wstrCmd += L"GENERAL";
			break;
		case pgPrivacy:
			wstrCmd += L"PRIVACY";
			break;
		case pgNotifications:
			wstrCmd += L"NOTIFICATIONS";
			break;
		case pgSoundAlerts:
			wstrCmd += L"SOUNDALERTS";
			break;
		case pgSoundDevices:
			wstrCmd += L"SOUNDDEVICES";
			break;
		case pgHotKeys:
			wstrCmd += L"HOTKEYS";
			break;
		case pgConnection:
			wstrCmd += L"CONNECTION";
			break;
		case pgVoiceMail:
			wstrCmd += L"VOICEMAIL";
			break;
		case pgCallForward:
			wstrCmd += L"CALLFORWARD";
			break;
		case pgVideo:
			wstrCmd += L"VIDEO";
			break;
		case pgAdvanced:
			wstrCmd += L"ADVANCED";
			break;
		default:
			return AtlReportError(GetObjectCLSID(), L"Invalid options page specified", 
				GUID_NULL, E_INVALIDARG);
	}
	return Execute(wstrCmd.c_str());
}

STDMETHODIMP UI::OpenCallHistory(void)
{
	CHECK_PROTOCOL(5)
	return Execute(L"OPEN CALLHISTORY");
}

STDMETHODIMP UI::OpenContacts(void)
{
	CHECK_PROTOCOL(5)
	return Execute(L"OPEN CONTACTS");
}

STDMETHODIMP UI::OpenDialpad(void)
{
	CHECK_PROTOCOL(5)
	return Execute(L"OPEN DIALPAD");
}

STDMETHODIMP UI::OpenSendContacts(void)
{
	CHECK_PROTOCOL(5)
	return Execute(L"OPEN SENDCONTACTS");
}

STDMETHODIMP UI::OpenBlockedUsers(void)
{
	CHECK_PROTOCOL(5)
	return Execute(L"OPEN BLOCKEDUSERS");
}

STDMETHODIMP UI::OpenImportContacts(void)
{
	CHECK_PROTOCOL(5)
	return Execute(L"OPEN IMPORTCONTACTS");
}

STDMETHODIMP UI::OpenGettingStarted(void)
{
	CHECK_PROTOCOL(5)
	return Execute(L"OPEN GETTINGSTARTED");
}

STDMETHODIMP UI::OpenAuthorization(BSTR UserName)
{
	CHECK_PROTOCOL(5)

	if(wcslen(UserName) < 1) {
		return AtlReportError(GetObjectCLSID(), L"This function requires a value to be passed", 
			GUID_NULL, E_INVALIDARG);
	}
	wstring wstrCmd = L"OPEN AUTHORIZATION ";
	wstrCmd += UserName;
	return Execute(wstrCmd.c_str());
}
