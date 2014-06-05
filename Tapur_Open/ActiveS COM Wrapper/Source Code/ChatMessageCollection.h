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
// ChatMessageCollection.h : Declaration of the ChatMessageCollection

#pragma once
#include "resource.h"       // main symbols
#include <list>

typedef std::list<long> ChatMessageList;

namespace SkypeAPILib 
{

// Forward declarations 
[object]
__interface IChatMessage; 

// IChatMessageCollection
[
	object,
	uuid("FBA3551A-6AF4-4812-9660-CBB6DF041EB7"),
	dual,	helpstring("IChatMessageCollection Interface"),
	helpcontext(6500),
	pointer_default(unique)
]
__interface IChatMessageCollection : IDispatch
{
	[propget, helpcontext(6503), restricted, id(DISPID_NEWENUM)] HRESULT _NewEnum([out, retval] IUnknown ** ppUnk);
	[propget, helpcontext(6501), id(1)] HRESULT Count([out, retval] long * pVal);
	[propget, helpcontext(6502), id(DISPID_VALUE)] HRESULT Item([in] VARIANT index, [out, retval] IChatMessage ** pVal);
};



// ChatMessageCollection

[
	coclass,
	noncreatable,	
	default(IChatMessageCollection),
	threading(apartment),
	support_error_info("IChatMessageCollection"),
	vi_progid("ActiveST.ChatMessageCollection"),
	progid("ActiveST.ChatMessageCollection.1"),
	version(1.0),
	uuid("6EBD5624-D4DE-441d-ABED-4CF8D487ECF9"),
	helpstring("ChatMessageCollection Class")
]
class ATL_NO_VTABLE ChatMessageCollection : 
	public IChatMessageCollection
{
public:
	ChatMessageCollection()
	{
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

private:
	ChatMessageList m_itemList;

// IChatMessageCollection
public:
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown ** ppUnk);
	STDMETHOD(get_Count)(/*[out, retval]*/ long * pVal);
	STDMETHOD(get_Item)(/*[in]*/ VARIANT index, /*[out, retval]*/ IChatMessage ** pVal);

	void SetItemList(LPCWSTR ItemList);
#ifndef _NO_USE_LIST
	void SetItemList(ChatMessageList ItemList);
#endif
};

}