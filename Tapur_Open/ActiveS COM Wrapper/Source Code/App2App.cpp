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
// App2App.cpp : Implementation of App2App

#include "stdafx.h"
#include ".\skypemessagequeue.h"
#include ".\usercollection.h"
#include ".\app2app.h"
#include ".\app2appstream.h"

namespace SkypeAPILib
{

#define APP_ID_CHECK \
	if(m_strAppName.empty()) \
		return AtlReportError(GetObjectCLSID(), _T("Application Name Not Set"), GUID_NULL, E_FAIL);

// App2App

void App2App::SetAppName(LPCWSTR AppName) {
	if(wcslen(AppName) > 32) {
		throw(L"Application names are limited to 32 characters");
	}
	m_strAppName = AppName;
}

STDMETHODIMP App2App::CreateApplication()
{
	APP_ID_CHECK
	CHECK_PROTOCOL(5)

	if(m_strAppName.length() > 32) {
		return AtlReportError(GetObjectCLSID(), L"Application name must be under 32 characters",
			GUID_NULL, E_INVALIDARG);
	}

	wstring wstrCommand = L"CREATE APPLICATION " + m_strAppName;
	try {
		gSkypeQueue.ExecuteBlockingCommand(wstrCommand.c_str(), wstrCommand.c_str());
		return S_OK;
	} catch(const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP App2App::DeleteApplication(void)
{
	APP_ID_CHECK
	CHECK_PROTOCOL(5)

	wstring wstrCommand = L"DELETE APPLICATION ";
	wstrCommand += m_strAppName;

	try {
		gSkypeQueue.ExecuteBlockingCommand(wstrCommand.c_str(), wstrCommand.c_str());
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}

	return S_OK;
}

STDMETHODIMP App2App::InitiateConnection(BSTR UserID)
{
	APP_ID_CHECK
	CHECK_PROTOCOL(5)

	wstring wstrCommand = L"ALTER APPLICATION ";
	wstrCommand += m_strAppName;
	wstrCommand += L" CONNECT ";
	wstrCommand += UserID;

	try {
		LPWSTR lpstrResult = gSkypeQueue.ExecuteBlockingCommand(wstrCommand.c_str(), wstrCommand.c_str());
		free(lpstrResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}


STDMETHODIMP App2App::get_ConnectableUsers(IUserCollection ** pVal)
{
	APP_ID_CHECK
	CHECK_PROTOCOL(5)

	CComObject<UserCollection> * pUsers = NULL;
	LPWSTR pstrUsers = NULL;

	try {
		pstrUsers = gSkypeQueue.RetrieveProperty(L"APPLICATION", m_strAppName.c_str(), L"CONNECTABLE");
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}


	try {
		HRESULT hr = CComObject<UserCollection>::CreateInstance(& pUsers);
		if(SUCCEEDED(hr)) {
			pUsers->SetItemList(pstrUsers);
		}
		free(pstrUsers);
		return pUsers->QueryInterface(pVal);
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

}