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
// App2App.h : Declaration of the App2App

#pragma once
#include "resource.h"       // main symbols

namespace SkypeAPILib
{
// Forward declarations 
[object]
__interface IApp2AppStream; 

// IApp2App
[
	object,
	uuid("E8EF86A8-27ED-4c9b-9096-E0B32005B9DB"),
	dual,	helpstring("IApp2App Interface"),
	helpcontext(9000),
	pointer_default(unique)
]
__interface IApp2App : IDispatch
{
	[id(1), helpcontext(9001), helpstring("method CreateApplication")] HRESULT CreateApplication(void);
	[id(2), helpcontext(9002), helpstring("method DeleteApplication")] HRESULT DeleteApplication(void);
	[id(3), helpcontext(9003), helpstring("method InitiateConnection")] HRESULT InitiateConnection([in] BSTR UserHandle);
	[propget, id(4), helpcontext(9004), helpstring("property ConnectableUsers")] HRESULT ConnectableUsers([out, retval] IUserCollection ** pVal);
};



// App2App

[
	coclass,
	noncreatable,	
	threading(apartment),
	default(IApp2App),
	support_error_info("IApp2App"),
	vi_progid("ActiveST.App2App"),
	progid("ActiveST.App2App.1"),
	version(1.0),
	uuid("C5155C83-C87A-4620-B63B-89584498DFA1"),
	helpstring("App2App Class")
]
class ATL_NO_VTABLE App2App : 
	public IApp2App
{
public:
	App2App()
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

	void SetAppName(LPCWSTR AppName);

private:
	wstring m_strAppName;

public:

	STDMETHOD(CreateApplication)(void);
	STDMETHOD(DeleteApplication)(void);
	STDMETHOD(InitiateConnection)(BSTR UserHandle);
	STDMETHOD(get_ConnectableUsers)(IUserCollection ** pVal);
};

}