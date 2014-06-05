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
// MessageCollection.h : Declaration of the MessageCollection

#pragma once
#include "resource.h"       // main symbols
#include <list>

typedef std::list<long> MessageIDList;

namespace SkypeAPILib 
{

// Forward declarations 
[object]
__interface IMessage; 

// IMessageCollection
[
	object,
	uuid("20E24A12-DCB4-47af-A59C-37D7BB13BCC0"),
	dual,	helpstring("IMessageCollection Interface"),
	helpcontext(7500),
	pointer_default(unique)
]
__interface IMessageCollection : IDispatch
{
	[propget, helpcontext(7503), restricted, id(DISPID_NEWENUM)] HRESULT _NewEnum([out, retval] IUnknown ** ppUnk);
	[propget, helpcontext(7501), id(1)] HRESULT Count([out, retval] long * pVal);
	[propget, helpcontext(7502), id(DISPID_VALUE)] HRESULT Item([in] VARIANT index, [out, retval] IMessage ** pVal);
};



// MessageCollection

[
	coclass,
	noncreatable,	
	default(IMessageCollection),
	threading(apartment),
	support_error_info("IMessageCollection"),
	vi_progid("ActiveST.MessageCollection"),
	progid("ActiveST.MessageCollection.1"),
	version(1.0),
	uuid("D3C0355D-B186-48bc-9F3A-61EC0537D0D4"),
	helpstring("MessageCollection Class")
]
class ATL_NO_VTABLE MessageCollection : 
	public IMessageCollection
{
public:
	MessageCollection()
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

public:

	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown ** ppUnk);
	STDMETHOD(get_Count)(/*[out, retval]*/ long * pVal);
	STDMETHOD(get_Item)(/*[in]*/ VARIANT index, /*[out, retval]*/ IMessage ** pVal);

	void SetItemList(LPCWSTR ItemList);

private:
	MessageIDList m_itemList;
};

}