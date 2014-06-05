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
// Profile.cpp : Implementation of Profile

#include "stdafx.h"
#include ".\skypemessagequeue.h"
#include ".\conversion.h"
#include ".\Profile.h"
#include ".\datetime.h"

// Profile
STDMETHODIMP Profile::get_FullName(BSTR* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"FULLNAME");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_FullName(BSTR newVal)
{
	try {
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"FULLNAME", newVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_Birthday(DATE* pVal)
{
	LONG yy, mm, dd;
	BOOL bOk = FALSE;

	try {
		WCHAR * pStrBirthdate = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"BIRTHDAY");
		if(swscanf_s(pStrBirthdate, L"%04d%02d%02d", & yy, & mm, &dd) == 3) {
			OleDateFromTm((WORD) yy, (WORD) mm, (WORD) dd, 0, 0, 0, * pVal);
			bOk = TRUE;
		}
		free(pStrBirthdate);
		return bOk;		// ñ¢ì¸óÕÇÃèÍçáÇ‡ÉGÉâÅ[Ç∆ÇµÇ»Ç¢ÅB
		//return bOk ? S_OK : AtlReportError(GetObjectCLSID(), L"Invalid birthday in user Profile", GUID_NULL, E_FAIL);
	} catch(const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_Birthday(DATE newVal)
{
	try {
		CDateTime dt = newVal;
		_bstr_t bstrDate = dt.Format("%Y%m%d");

		gSkypeQueue.SetProperty(L"PROFILE", L"", L"BIRTHDAY", bstrDate);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_Sex(SkypeUserSex* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"SEX");
		*pVal = TextToSkypeUserSex(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_Sex(SkypeUserSex newVal)
{
	try {
		CComObject<Conversion> * objConvert;
		HRESULT hr = CComObject<Conversion>::CreateInstance(& objConvert);
		if(SUCCEEDED(hr)) {
			BSTR bstrText;
			hr = objConvert->SkypeUserSexAsText(newVal, & bstrText);
			if(SUCCEEDED(hr)) {
				gSkypeQueue.SetProperty(L"PROFILE", L"", L"SEX", bstrText);
				SysFreeString(bstrText);
			}
		}
		return hr;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_Languages(SAFEARRAY ** pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"LANGUAGES");

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

STDMETHODIMP Profile::put_Languages(SAFEARRAY * newVal)
{
	try {
		VARTYPE vt = VT_UNKNOWN;
		SafeArrayGetVartype(newVal, & vt);
		UINT uiDim = SafeArrayGetDim(newVal);
		if((uiDim != 1) || (vt != VT_BSTR)) {
			return AtlReportError(GetObjectCLSID(), L"Array of languages must be single dimension array of strings", GUID_NULL, E_INVALIDARG);
		}

		HRESULT hr = SafeArrayLock(newVal);
		if(FAILED(hr)) {
			return AtlReportError(GetObjectCLSID(), L"Unable to lock array", GUID_NULL, E_FAIL);
		}

		BSTR * pItems = (BSTR *) newVal->pvData;
		wstring wstrItems;
		for(ULONG l = 0; l < newVal->rgsabound[0].cElements; l++) {
			if(l > 0) wstrItems += L" ";
			wstrItems += pItems[l];
		}

		SafeArrayUnlock(newVal);
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"LANGUAGES", wstrItems.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_Country(BSTR * pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"COUNTRY");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_Country(BSTR newVal)
{
	try {
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"COUNTRY", newVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_Province(BSTR* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"PROVINCE");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_Province(BSTR newVal)
{
	try {
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"PROVINCE", newVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_City(BSTR* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"CITY");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_City(BSTR newVal)
{
	try {
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"CITY", newVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_HomePhone(BSTR* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"PHONE_HOME");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_HomePhone(BSTR newVal)
{
	try {
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"PHONE_HOME", newVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_OfficePhone(BSTR* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"PHONE_OFFICE");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_OfficePhone(BSTR newVal)
{
	try {
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"PHONE_OFFICE", newVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_MobilePhone(BSTR* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"PHONE_MOBILE");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_MobilePhone(BSTR newVal)
{
	try {
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"PHONE_MOBILE", newVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_Homepage(BSTR* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"HOMEPAGE");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_Homepage(BSTR newVal)
{
	try {
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"HOMEPAGE", newVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_About(BSTR* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"ABOUT");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_About(BSTR newVal)
{
	try {
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"ABOUT", newVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_MoodText(BSTR* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"MOOD_TEXT");
		*pVal = SysAllocString(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_MoodText(BSTR newVal)
{
	try {
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"MOOD_TEXT", newVal);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_Timezone(LONG* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"TIMEZONE");
		*pVal = _wtol(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_Timezone(LONG newVal)
{
	try {
		WCHAR wstrNumber[64];
		_ltow_s(newVal, wstrNumber, sizeof(wstrNumber)/sizeof(wstrNumber[0]), 10);
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"TIMEZONE", wstrNumber);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}


STDMETHODIMP Profile::get_ForwardingRules(SAFEARRAY ** pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"CALL_FORWARD_RULES");
		size_t sz = wcslen(pResult) + 1;
		WCHAR * pCopy = new WCHAR[sz];

		long lTotal=0, lCtr=0;
		HRESULT hr;
		ForwardingRule * pItems;

		for(int i = 0; i < 2; i++) {
			wcscpy_s(pCopy, sz, pResult);
			WCHAR *next_token = NULL;
			WCHAR * pItem = wcstok_s(pCopy, L" ", &next_token);
			while(pItem != NULL) {
				size_t iLen = wcslen(pItem);
				if(iLen > 0) {
					LONG start;
					LONG end;
					WCHAR * name = new WCHAR[iLen];

					bool bValid = 
						swscanf_s(pItem, L"%d,%d,%s", & start, & end, name, iLen) == 3;

					if(bValid) {
						if(i == 0) {
							lTotal++;
						} else {
							pItems[lCtr].StartTime = start;
							pItems[lCtr].EndTime = end;
							pItems[lCtr].Handle = SysAllocString(name);
							lCtr++;
						}
					}

					delete(name);
				}
				pItem = wcstok_s(NULL, L" ", &next_token);
			}

			if(i == 0) {
				SAFEARRAYBOUND sba;
				sba.cElements = lTotal;
				sba.lLbound = 0;

				CComPtr<IRecordInfo> *ptrRecInfo;

				hr = GetRecordInfoFromGuids(CAtlModule::m_libid, 1, 0, 0x409,
					__uuidof(ForwardingRule), (IRecordInfo**) & ptrRecInfo);

				if(FAILED(hr)) {
					return AtlReportError(GetObjectCLSID(), L"Unable to retrieve structure info", GUID_NULL, E_FAIL);				
				}
					
				*pVal = SafeArrayCreateEx(VT_RECORD, 1, & sba, ptrRecInfo);
				if((*pVal) == NULL) {
					return AtlReportError(GetObjectCLSID(), L"Unable to create array", GUID_NULL, E_FAIL);
				}

				hr = SafeArrayLock(*pVal);
				if(FAILED(hr)) {
					return AtlReportError(GetObjectCLSID(), L"Unable to lock array", GUID_NULL, E_FAIL);
				}

				pItems = (ForwardingRule *) (*pVal)->pvData;

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

	return S_OK;
}

STDMETHODIMP Profile::put_ForwardingRules(SAFEARRAY * newVal)
{
	try {
		VARTYPE vt = VT_UNKNOWN;
		SafeArrayGetVartype(newVal, & vt);
		UINT uiDim = SafeArrayGetDim(newVal);
		if((uiDim != 1) || (vt != VT_RECORD)) {
			return AtlReportError(GetObjectCLSID(), L"Array of rules must be single dimension array of strings", GUID_NULL, E_INVALIDARG);
		}

		HRESULT hr = SafeArrayLock(newVal);
		if(FAILED(hr)) {
			return AtlReportError(GetObjectCLSID(), L"Unable to lock array", GUID_NULL, E_FAIL);
		}

		wstring wstrItems;
		ForwardingRule * pItems = (ForwardingRule *) newVal->pvData;
		for(ULONG l = 0; l < newVal->rgsabound[0].cElements; l++) {
			WCHAR start[32], end[32];
			_ltow_s(pItems[l].StartTime, start, sizeof(start)/sizeof(start[0]), 10);
			_ltow_s(pItems[l].EndTime, end, sizeof(end)/sizeof(end[0]), 10);
			if(l > 0) {
				wstrItems += L" ";
			}
			wstrItems += start;
			wstrItems += L",";
			wstrItems += end;
			wstrItems += L",";
			wstrItems += pItems[l].Handle;
		}

		SafeArrayUnlock(newVal);
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"CALL_FORWARD_RULES", wstrItems.c_str());
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_NoAnswerTimeout(LONG* pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"CALL_NOANSWER_TIMEOUT");
		*pVal = _wtol(pResult);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_NoAnswerTimeout(LONG newVal)
{
	try {
		WCHAR wstrNumber[64];
		_ltow_s(newVal, wstrNumber, sizeof(wstrNumber)/sizeof(wstrNumber[0]), 10);
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"CALL_NOANSWER_TIMEOUT", wstrNumber);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_CallForwardingEnabled(VARIANT_BOOL * pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"CALL_APPLY_CF");
		*pVal = (_wcsicmp(pResult, L"TRUE") == 0);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_CallForwardingEnabled(VARIANT_BOOL newVal)
{
	try {
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"CALL_APPLY_CF", newVal ? L"TRUE" : L"FALSE");
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_VoicemailEnabled(VARIANT_BOOL * pVal)
{
	try {
		WCHAR * pResult = gSkypeQueue.RetrieveProperty(L"PROFILE", L"", L"CALL_SEND_TO_VM");
		*pVal = (_wcsicmp(pResult, L"TRUE") == 0);
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_VoicemailEnabled(VARIANT_BOOL newVal)
{
	try {
		gSkypeQueue.SetProperty(L"PROFILE", L"", L"CALL_SEND_TO_VM", newVal ? L"TRUE" : L"FALSE");
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

#define CHECK_RINGTONE(x) if(((x < rngRingtone) || (x > rngConnecting)) && ((x < rngResume) || (x > rngOnlineAlert))) \
	return AtlReportError(GetObjectCLSID(), L"Invalid tone specifed", GUID_NULL, E_INVALIDARG);

STDMETHODIMP Profile::get_RingtoneStatus(/*[in]*/ SkypeRingtone Tone, /*[out, retval]*/ VARIANT_BOOL *pVal)
{
	CHECK_PROTOCOL(5)
	CHECK_RINGTONE(Tone)

	WCHAR wstrCmd[32], wstrResponse[32];
	try {
		swprintf_s(wstrCmd, sizeof(wstrCmd)/sizeof(wstrCmd[0]), L"GET RINGTONE %d STATUS", Tone);
		swprintf_s(wstrResponse, sizeof(wstrResponse)/sizeof(wstrResponse[0]), L"RINGTONE %d ", Tone);

		WCHAR * pResponse = gSkypeQueue.ExecuteBlockingCommand(wstrCmd, wstrResponse);
		*pVal = (wcsstr(pResponse, L" ON") != NULL);
		free(pResponse);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::get_RingtoneFileName(/*[in]*/ SkypeRingtone Tone, /*[in, out]*/ BSTR * FileName)
{
	CHECK_PROTOCOL(5)
	CHECK_RINGTONE(Tone)

	WCHAR wstrCmd[32], wstrResponse[32];
	try {
		swprintf_s(wstrCmd, sizeof(wstrCmd)/sizeof(wstrCmd[0]), L"GET RINGTONE %d", Tone);
		swprintf_s(wstrResponse, sizeof(wstrResponse)/sizeof(wstrResponse[0]), L"RINGTONE %d ", Tone);
		WCHAR * pResponse = gSkypeQueue.ExecuteBlockingCommand(wstrCmd, wstrResponse);
		*FileName = SysAllocString(pResponse);
		free(pResponse);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

STDMETHODIMP Profile::put_RingtoneFileName(/*[in]*/ SkypeRingtone Tone, /*[in]*/ BSTR FileName)
{
	CHECK_PROTOCOL(5)
	CHECK_RINGTONE(Tone)

	WCHAR wstrBuffer[32];

	try {
		wstring wstrCmd = L"SET RINGTONE ";
		wstrCmd += _ltow_s(Tone, wstrBuffer, sizeof(wstrBuffer)/sizeof(wstrBuffer[0]), 10);
		wstrCmd += L" ";
		wstrCmd += FileName;
		
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(wstrCmd.c_str(), L"RINGTONE");
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}

// STDMETHODIMP SkypeAPILib::Profile::get_AvatarFileName(/*[in]*/ SkypeRingtone Tone, /*[in, out]*/ BSTR * FileName)
/*
{
	CHECK_PROTOCOL(5)
	CHECK_RINGTONE(Tone)

	WCHAR wstrCmd[32], wstrResponse[32];
	try {
		swprintf(wstrCmd, L"GET AVATAR %d", Tone);
		swprintf(wstrResponse, L"AVATAR %d ", Tone);
		WCHAR * pResponse = gSkypeQueue.ExecuteBlockingCommand(wstrCmd, wstrResponse);
		*FileName = SysAllocString(pResponse);
		free(pResponse);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}
*/

STDMETHODIMP Profile::put_AvatarFileName(/*[in]*/ LONG Index, /*[in]*/ BSTR FileName)
{
	CHECK_PROTOCOL(5)

	WCHAR wstrBuffer[32];

	try {
		wstring wstrCmd = L"SET AVATAR ";
		wstrCmd += _ltow_s(Index, wstrBuffer, sizeof(wstrBuffer)/sizeof(wstrBuffer[0]), 10);
		wstrCmd += L" ";
		wstrCmd += FileName;
		
		WCHAR * pResult = gSkypeQueue.ExecuteBlockingCommand(wstrCmd.c_str(), L"AVATAR");
		free(pResult);
		return S_OK;
	} catch (const WCHAR * err) {
		return AtlReportError(GetObjectCLSID(), err, GUID_NULL, E_FAIL);
	}
}
