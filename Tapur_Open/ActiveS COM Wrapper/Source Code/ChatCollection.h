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
// ChatCollection.h : Declaration of the ChatCollection

#pragma once
#include "resource.h"       // main symbols
#include <list>
typedef std::list<wstring> ChatList;

namespace SkypeAPILib 
{

// Forward declarations 
[object]
__interface IChat; 

// IChatCollection
[
	object,
	uuid("046D1967-46D1-4aad-A395-1AB1ADB8A15E"),
	dual,	helpstring("IChatCollection Interface"),
	helpcontext(5500),
	pointer_default(unique)
]
__interface IChatCollection : IDispatch
{
	[propget, helpcontext(5503), restricted, id(DISPID_NEWENUM)] HRESULT _NewEnum([out, retval] IUnknown ** ppUnk);
	[propget, helpcontext(5501), id(1)] HRESULT Count([out, retval] long * pVal);
	[propget, helpcontext(5502), id(DISPID_VALUE)] HRESULT Item([in] VARIANT index, [out, retval] IChat ** pVal);
};

// ChatCollection

[
	coclass,
	noncreatable,	
	default(IChatCollection),
	threading(apartment),
	support_error_info("IChatCollection"),
	vi_progid("ActiveST.ChatCollection"),
	progid("ActiveST.ChatCollection.1"),
	version(1.0),
	uuid("B8B7C427-DF53-4807-808A-1D7866A3A185"),
	helpstring("ChatCollection Class")
]
class ATL_NO_VTABLE ChatCollection : 
	public IChatCollection
{
public:
	ChatCollection()
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
	ChatList m_itemList;

// IChatCollection
public:

public:
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown ** ppUnk);
	STDMETHOD(get_Count)(/*[out, retval]*/ long * pVal);
	STDMETHOD(get_Item)(/*[in]*/ VARIANT index, /*[out, retval]*/ IChat ** pVal);

	void SetItemList(LPCWSTR ItemList);
#ifndef _NO_USE_LIST
	void SetItemList(ChatList ItemList);
#endif
};

}