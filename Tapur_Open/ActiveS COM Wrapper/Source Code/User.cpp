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
// User.cpp : Implementation of User

#include "stdafx.h"
#include "datetime.h"
#include "conversion.h"
#include "User.h"
#include "skypemessagequeue.h"

#define USER_INFO_CHECK \
	if(m_strUserName.empty()) \
		return AtlReportError(GetObjectCLSID(), _T("User ID Not Set"), GUID_NULL, E_FAIL);
#define USER_SKYPEOUT_CHECK \
	if(m_strUserName[0] == '+') \
		return AtlReportError(GetObjectCLSID(), _T("This property is unavailable for SkypeOut contact entries"), GUID_NULL, E_FAIL);

namespace SkypeAPILib {

// User

STDMETHODIMP User::get_Handle(BSTR *pVal)
{
	USER_INFO_CHECK

	try {
		/*
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"HANDLE");
		*pVal = SysAllocString(pResult);
		free(pResult);
		*/
		*pVal = SysAllocString(m_strUserName.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_FullName(BSTR *pVal)
{
	USER_INFO_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"FULLNAME");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}


STDMETHODIMP User::get_Birthdate(DATE *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK
	
	LONG yy, mm, dd;
	BOOL bOk = FALSE;

	try {
		WCHAR * pStrBirthdate = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"BIRTHDAY");
		if(swscanf_s(pStrBirthdate, L"%04d%02d%02d", &yy, &mm, &dd) == 3) {
			OleDateFromTm((WORD)yy, (WORD)mm, (WORD)dd, 0, 0, 0, *pVal);
			bOk = TRUE;
		}
		free(pStrBirthdate);
		return bOk;		// ñ¢ì¸óÕÇÃèÍçáÇ‡ÉGÉâÅ[Ç∆ÇµÇ»Ç¢ÅB
		// return bOk ? S_OK : AtlReportError(GetObjectCLSID(), L"Invalid birthday in user StartTime", GUID_NULL, E_FAIL);
	} catch(const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_Sex(SkypeUserSex *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * ptrResponse = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"SEX");
		*pVal = TextToSkypeUserSex(ptrResponse);
		free(ptrResponse);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_Country(BSTR *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"COUNTRY");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_Province(BSTR *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"PROVINCE");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_City(BSTR *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"CITY");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_Phone_Home(BSTR *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"PHONE_HOME");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_Phone_Office(BSTR *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"PHONE_OFFICE");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_Phone_Mobile(BSTR *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"PHONE_MOBILE");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_Homepage(BSTR *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"HOMEPAGE");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_About(BSTR *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"ABOUT");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_HasCallEquipment(VARIANT_BOOL *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"HASCALLEQUIPMENT");
		*pVal = (_wtol(pResult) != 0);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_BuddyStatus(SkypeBuddyStatus *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * ptrResponse = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"BUDDYSTATUS");
		*pVal = TextToSkypeBuddyStatus(ptrResponse);
		free(ptrResponse);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::put_BuddyStatus(SkypeBuddyStatus Val)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	CHECK_PROTOCOL(4)

	try {
		wstring wstrCmd = L"SET USER ";
		wstrCmd += m_strUserName.c_str();
		switch(Val) {
			case budDeleted:
				wstrCmd += L" BUDDYSTATUS 1";
				break;
			case budPending:
				wstrCmd += L" BUDDYSTATUS 2";
				break;
			default:
				return AtlReportError(GetObjectCLSID(), L"Status must be Deleted or Pending", GUID_NULL, E_FAIL);
		}
		gSkypeQueue.SendCommand(wstrCmd.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_Authorized(VARIANT_BOOL *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"ISAUTHORIZED");
		*pVal = (_wcsicmp(pResult, L"TRUE") == 0);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_Blocked(VARIANT_BOOL *pVal)
{
	USER_INFO_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"ISBLOCKED");
		*pVal = (_wcsicmp(pResult, L"TRUE") == 0);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_DisplayName(BSTR *pVal)
{
	USER_INFO_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"DISPLAYNAME");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_OnlineStatus(SkypeOnlineStatus *pVal)
{
	USER_INFO_CHECK

	try {
#ifdef _NO_USE_LIST
		WCHAR * ptrResponse = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"ONLINESTATUS");
		*pVal = TextToSkypeOnlineStatus(ptrResponse);
		free(ptrResponse);
#else
		*pVal = gSkypeQueue.GetUserOnlineStatus(m_strUserName.c_str());
#endif
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_LastOnline(DATE *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * pStrLastOnline = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"LASTONLINETIMESTAMP");
		*pVal = UnixTimestampToDATE(_wtol(pStrLastOnline));
		free(pStrLastOnline);
		return S_OK;
	} catch(WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_IsSkypeOut(VARIANT_BOOL *pVal)
{
	USER_INFO_CHECK
	*pVal = (m_strUserName[0] == '+');
	return S_OK;
}

STDMETHODIMP User::get_ReceivedAuthorizationRequest(BSTR *pVal) 
{
	USER_INFO_CHECK

	CHECK_PROTOCOL(5)

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"RECEIVEDAUTHREQUEST");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_IsAuthorized(VARIANT_BOOL *pVal)
{
	USER_INFO_CHECK

	CHECK_PROTOCOL(5)

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"ISAUTHORIZED");
		*pVal = (_wtol(pResult) != 0);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::put_IsAuthorized(VARIANT_BOOL Val)
{
	USER_INFO_CHECK

	CHECK_PROTOCOL(5)

	try {
		wstring wstrCmd = L"SET USER ";
		wstrCmd += m_strUserName.c_str();
		wstrCmd += L" ISAUTHORIZED ";
		wstrCmd += Val ? L"1" : L"0";
		gSkypeQueue.SendCommand(wstrCmd.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}


STDMETHODIMP User::get_IsBlocked(VARIANT_BOOL *pVal)
{
	USER_INFO_CHECK

	CHECK_PROTOCOL(5)

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"ISBLOCKED");
		*pVal = (_wtol(pResult) != 0);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::put_IsBlocked(VARIANT_BOOL Val)
{
	USER_INFO_CHECK

	CHECK_PROTOCOL(5)

	try {
		wstring wstrCmd = L"SET USER ";
		wstrCmd += m_strUserName.c_str();
		wstrCmd += L" ISBLOCKED ";
		wstrCmd += Val ? L"1" : L"0";
		gSkypeQueue.SendCommand(wstrCmd.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_SpeedDial(BSTR *pVal)
{
	USER_INFO_CHECK

	CHECK_PROTOCOL(5)

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"SPEEDDIAL");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::put_SpeedDial(BSTR Val)
{
	USER_INFO_CHECK

	CHECK_PROTOCOL(5)

	try {
		wstring wstrCmd = L"SET USER ";
		wstrCmd += m_strUserName.c_str();
		wstrCmd += L" SPEEDIAL ";
		wstrCmd += Val;
		gSkypeQueue.SendCommand(wstrCmd.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_MoodText(BSTR* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"MOOD_TEXT");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_Aliases(SAFEARRAY ** pVal)
{
	USER_INFO_CHECK
	CHECK_PROTOCOL(5)

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"ALIASES");

		long lTotal=0, lCtr=0;
		HRESULT hr;
		BSTR * pItems;

		for(int i = 0; i < 2; i++) {
			WCHAR *next_token = NULL;
			WCHAR * pItem = wcstok_s(pResult, L" ", &next_token);
			while(pItem != NULL) {
				if(wcslen(pItem) > 0) {
					if(i == 0) {
						lTotal++;
					} else {
						pItems[lCtr++] = SysAllocString(pItem);
					}
				}
				pItem = wcstok_s(NULL, L" ", &next_token);
			}

			if(i == 0) {
				SAFEARRAYBOUND sba;
				sba.cElements = lTotal;
				sba.lLbound = 0;
				*pVal = SafeArrayCreateEx(VT_BSTR, 1, & sba, NULL);
				if((*pVal) == NULL) {
					return AtlReportError(GetObjectCLSID(), L"Unable to create array", GUID_NULL, E_FAIL);
				}
				hr = SafeArrayLock(*pVal);
				if(FAILED(hr)) {
					return AtlReportError(GetObjectCLSID(), L"Unable to lock array", GUID_NULL, E_FAIL);
				}

				pItems = (BSTR *) (*pVal)->pvData;
			} else {
				hr = SafeArrayUnlock(*pVal);
				if(FAILED(hr)) {
					return AtlReportError(GetObjectCLSID(), L"Unable to unlock array", GUID_NULL, E_FAIL);
				}
			}
		}

		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_Timezone(LONG* pVal)
{
	USER_INFO_CHECK
	CHECK_PROTOCOL(5)

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"TIMEZONE");
		*pVal = _wtol(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP User::get_Language(BSTR *pVal)
{
	USER_INFO_CHECK
	USER_SKYPEOUT_CHECK

	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"USER", m_strUserName.c_str(), L"LANGUAGE");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

}