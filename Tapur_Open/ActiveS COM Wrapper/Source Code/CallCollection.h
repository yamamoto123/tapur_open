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
// CallCollection.h : Declaration of the CallCollection

#pragma once
#include "resource.h"       // main symbols
#include <list>

typedef std::list<long> CallIDList;

namespace SkypeAPILib
{

// Forward declarations 
[object]
__interface ICall; 

// ICallCollection
[
	object,
	uuid("D84C375F-43B9-446a-ADFB-F504FE049052"),
	dual,	helpstring("ICallCollection Interface"),
	helpcontext(3500),
	pointer_default(unique)
]
__interface ICallCollection : IDispatch
{
	[propget, restricted, id(DISPID_NEWENUM), helpcontext(3503)] HRESULT _NewEnum([out, retval] IUnknown ** ppUnk);
	[propget, id(1), helpcontext(3501)] HRESULT Count([out, retval] long * pVal);
	[propget, id(DISPID_VALUE), helpcontext(3502)] HRESULT Item([in] VARIANT index, [out, retval] ICall ** pVal);
};


// CallCollection

[
	coclass,
	noncreatable,	
	default(ICallCollection),
	threading(apartment),
	support_error_info("ICallCollection"),
	vi_progid("ActiveST.CallCollection"),
	progid("ActiveST.CallCollection.1"),
	version(1.0),
	uuid("1703F3AD-0353-45b2-8077-6833B8252B90"),
	helpstring("CallCollection Class")
]
class ATL_NO_VTABLE CallCollection : 
	public ICallCollection
{
public:
	CallCollection()
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
	CallIDList m_itemList;

// ICallCollection
public:
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown ** ppUnk);
	STDMETHOD(get_Count)(/*[out, retval]*/ long * pVal);
	STDMETHOD(get_Item)(/*[in]*/ VARIANT index, /*[out, retval]*/ ICall ** pVal);

	void SetItemList(LPCWSTR ItemList);
#ifndef _NO_USE_LIST
	void SetItemList(CallList ItemList);
#endif
};

}