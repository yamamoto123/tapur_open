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
// UserCollection.cpp : Implementation of UserCollection

#include "stdafx.h"
#include ".\skypemessagequeue.h"
#include ".\usercollection.h"
#include ".\user.h"

// UserCollection

namespace SkypeAPILib {

void UserCollection::SetItemList(LPCWSTR ItemList)
{
	try {
		m_itemList.clear();

		WCHAR * list = _wcsdup(ItemList);
		WCHAR *next_token = NULL;
		WCHAR * pItem = wcstok_s(list, L", ", &next_token);
		while(pItem) {
			if(wcslen(pItem) > 0) {
				m_itemList.push_back(std::wstring(pItem));
			}
			pItem = wcstok_s(NULL, L", ", &next_token);
		}
		free(list);
	} catch(...) {
	}
}

#ifndef _NO_USE_LIST
void UserCollection::SetItemList(OnlineUserList ItemList)
{
	m_itemList.clear();
	for(OnlineUserList::iterator it = ItemList.begin(); it != ItemList.end(); it++) {
		std::wstring strName = it->first;
		if(strName.length() > 0) {
			m_itemList.push_back(strName);
		}
	}
}
#endif


STDMETHODIMP UserCollection::get__NewEnum(IUnknown** pRetVal)
{
	CComObject<User> * ptrUser;

	if (pRetVal == NULL)
		return E_POINTER;
	*pRetVal = NULL;

	size_t size = m_itemList.size();

	CComVariant* varArray = new CComVariant[size];
	long nLoop = 0;
	for(UserIDList::iterator it = m_itemList.begin(); it != m_itemList.end(); it++) {

		LPDISPATCH lpDisp;

		if(SUCCEEDED(CComObject<User>::CreateInstance(& ptrUser))) {
			ptrUser->SetUserName(it->c_str());
			ptrUser->QueryInterface(& lpDisp);
			_ASSERTE(lpDisp);

			varArray[nLoop].vt = VT_DISPATCH;
			varArray[nLoop].pdispVal = lpDisp;
			nLoop++;
		}
	}

	CEnumVARIANT* pEnum = new CEnumVARIANT;
	HRESULT hRes = pEnum->Init(&varArray[0], &varArray[size], NULL, AtlFlagCopy);
	if (SUCCEEDED(hRes))
		hRes = pEnum->QueryInterface(/*IID_IEnumVARIANT*/IID_IUnknown, (void**)pRetVal);
	if (FAILED(hRes))
		delete pEnum;

	delete [] varArray;
	return S_OK;
}

STDMETHODIMP UserCollection::get_Item(VARIANT index, IUser ** pRetVal)
{
	if (pRetVal == NULL)
		return E_POINTER;

	VARIANT vIndex;
	VariantInit(& vIndex);
	VariantChangeType(& vIndex, & index, 0, VT_I4);

	long lCtr = 0;
	for(UserIDList::iterator it = m_itemList.begin(); it != m_itemList.end(); it++) {
		if(++lCtr == vIndex.lVal) {
			CComObject<User> * ptrUser;
			if(SUCCEEDED(CComObject<User>::CreateInstance(& ptrUser))) {
				ptrUser->SetUserName(it->c_str());
				return ptrUser->QueryInterface(pRetVal);
			}
		}
	}

	return AtlReportError(GetObjectCLSID(), "Index not within collection bounds", GUID_NULL, E_INVALIDARG);
}

STDMETHODIMP UserCollection::get_Count(long * pVal)
{
	if (pVal == NULL)
		return E_POINTER;

	*pVal = (long) m_itemList.size();

	return S_OK;
}

}