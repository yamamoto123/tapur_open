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
// VoiceMailCollection.h : Declaration of the VoiceMailCollection

#pragma once
#include "resource.h"       // main symbols
#include <list>

namespace SkypeAPILib 
{

// Forward declarations 
[object]
__interface IVoiceMail; 

typedef std::list<long> VoiceMailList;

// IVoiceMailCollection
[
	object,
	uuid("84228251-93C4-4132-A262-ECEA129FA6A8"),
	dual,	helpstring("IVoiceMailCollection Interface"),
	helpcontext(8500),
	pointer_default(unique)
]
__interface IVoiceMailCollection : IDispatch
{
	[propget, helpcontext(8503), restricted, id(DISPID_NEWENUM)] HRESULT _NewEnum([out, retval] IUnknown ** ppUnk);
	[propget, helpcontext(8501), id(1)] HRESULT Count([out, retval] long * pVal);
	[propget, helpcontext(8502), id(DISPID_VALUE)] HRESULT Item([in] VARIANT index, [out, retval] IVoiceMail ** pVal);
};



// VoiceMailCollection

[
	coclass,
	noncreatable,	
	default(IVoiceMailCollection),
	threading(apartment),
	support_error_info("IVoiceMailCollection"),
	vi_progid("ActiveST.VoiceMailCollection"),
	progid("ActiveST.VoiceMailCollection.1"),
	version(1.0),
	uuid("6D49D4A5-22A1-471e-A670-88BDFC2A533A"),
	helpstring("VoiceMailCollection Class")
]
class ATL_NO_VTABLE VoiceMailCollection : 
	public IVoiceMailCollection
{
private:
	VoiceMailList m_itemList;

public:
	VoiceMailCollection()
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
	STDMETHOD(get_Item)(/*[in]*/ VARIANT index, /*[out, retval]*/ IVoiceMail ** pVal);

	void SetItemList(LPCWSTR ItemList);
#ifndef _NO_USE_LIST
	void SetItemList(VoiceMailList ItemList);
#endif
};

}