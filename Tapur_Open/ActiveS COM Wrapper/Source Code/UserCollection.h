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
// UserCollection.h : Declaration of the UserCollection

#pragma once
#include "resource.h"       // main symbols
#include <list>

namespace SkypeAPILib {

// Forward declarations 
[object]
__interface IUser; 

// IUserCollection
[
	object,
	uuid("5B0EB0B9-BF7C-4119-9886-568177FF1FF7"),
	dual,	helpstring("IUserCollection Interface"),
	pointer_default(unique)
]
__interface IUserCollection : IDispatch
{
	[propget, restricted, id(DISPID_NEWENUM)] HRESULT _NewEnum([out, retval] IUnknown ** ppUnk);
	[propget, id(1)] HRESULT Count([out, retval] long * pVal);
	[propget, id(DISPID_VALUE)] HRESULT Item([in] VARIANT index, [out, retval] IUser ** pVal);
};

typedef std::list<std::wstring> UserIDList;

// UserCollection

[
	coclass,
	noncreatable,	
	default(IUserCollection),
	threading(apartment),
	support_error_info("IUserCollection"),
	vi_progid("ActiveST.UserCollection"),
	progid("ActiveST.UserCollection.1"),
	version(1.0),
	uuid("C2FA1611-6E4E-454d-8F21-BB149115F34C"),
	helpstring("UserCollection Class")
]
class ATL_NO_VTABLE UserCollection : 
	public IUserCollection
{
public:
	UserCollection()
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
	UserIDList m_itemList;
#ifndef _NO_USE_LIST
	void SetItemList(OnlineUserList ItemList);
#endif

public:

	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown ** ppUnk);
	STDMETHOD(get_Count)(/*[out, retval]*/ long * pVal);
	STDMETHOD(get_Item)(/*[in]*/ VARIANT index, /*[out, retval]*/ IUser ** pVal);

	void SetItemList(LPCWSTR ItemList);
};

}