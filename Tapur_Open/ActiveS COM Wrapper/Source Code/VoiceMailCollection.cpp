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
// VoiceMailCollection.cpp : Implementation of VoiceMailCollection

#include "stdafx.h"
#include ".\skypemessagequeue.h"
#include ".\voicemailcollection.h"
#include ".\voicemail.h"

namespace SkypeAPILib 
{

// VoiceMailCollection
void VoiceMailCollection::SetItemList(LPCWSTR ItemList)
{
	try {
		m_itemList.clear();

		WCHAR * list = _wcsdup(ItemList);
		WCHAR *next_token = NULL;
		WCHAR * pItem = wcstok_s(list, L", ", &next_token);
		while(pItem) {
			long lID = _wtol(pItem);
			if(lID > 0) {
				m_itemList.push_back(lID);
			}
			pItem = wcstok_s(NULL, L", ", &next_token);
		}
		free(list);
	} catch(...) {
	}
}

#ifndef _NO_USE_LIST
void VoiceMailCollection::SetItemList(VoiceMailList ItemList)
{
	m_itemList.clear();
	for(VoiceMailList::iterator it = ItemList.begin(); it != ItemList.end(); it++) {
		m_itemList.push_back(*it);
	}
}
#endif

STDMETHODIMP VoiceMailCollection::get__NewEnum(IUnknown** pRetVal)
{
	CComObject<VoiceMail> * ptrVoiceMail;

	if (pRetVal == NULL)
		return E_POINTER;
	*pRetVal = NULL;

	size_t size = m_itemList.size();

	CComVariant* varArray = new CComVariant[size];
	long nLoop = 0;
	for(VoiceMailList::iterator it = m_itemList.begin(); it != m_itemList.end(); it++) {

		LPDISPATCH lpDisp;

		if(SUCCEEDED(CComObject<VoiceMail>::CreateInstance(& ptrVoiceMail))) {
			ptrVoiceMail->SetID(*it);
			ptrVoiceMail->QueryInterface(& lpDisp);
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

STDMETHODIMP VoiceMailCollection::get_Item(VARIANT index, IVoiceMail ** pRetVal)
{
	if (pRetVal == NULL)
		return E_POINTER;

	VARIANT vIndex;
	VariantInit(& vIndex);
	VariantChangeType(& vIndex, & index, 0, VT_I4);

	long lCtr = 0;
	for(VoiceMailList::iterator it = m_itemList.begin(); it != m_itemList.end(); it++) {
		if(++lCtr == vIndex.lVal) {
			CComObject<VoiceMail> * ptrVoiceMail;
			if(SUCCEEDED(CComObject<VoiceMail>::CreateInstance(& ptrVoiceMail))) {
				ptrVoiceMail->SetID(*it);
				return ptrVoiceMail->QueryInterface(pRetVal);
			}
		}
	}

	return AtlReportError(GetObjectCLSID(), "Index not within collection bounds", GUID_NULL, E_INVALIDARG);
}

STDMETHODIMP VoiceMailCollection::get_Count(long * pVal)
{
	if (pVal == NULL)
		return E_POINTER;

	*pVal = (long) m_itemList.size();

	return S_OK;
}

}