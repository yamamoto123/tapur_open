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
// UI.h : Declaration of the UI

#pragma once
#include "resource.h"       // main symbols


// IUI
[
	object,
	uuid("BE2A8840-5A65-49da-8DDF-F6A7C6D781E5"),
	dual,	helpstring("IUI Interface"),
	pointer_default(unique)
]
__interface IUI : IDispatch
{
	[id(1), helpstring("method OpenProfile"), helpcontext(11001)] HRESULT OpenProfile(void);
	[id(2), helpstring("method OpenUserInfo"), helpcontext(11002)] HRESULT OpenUserInfo([in] BSTR UserName);
	[id(3), helpstring("method OpenConference"), helpcontext(11003)] HRESULT OpenConference(void);
	[id(4), helpstring("method OpenSearch"), helpcontext(11004)] HRESULT OpenSearch(void);
	[id(5), helpstring("method OpenOptions"), helpcontext(11005)] HRESULT OpenOptions([in] SkypeOptionsPage Page);
	[id(6), helpstring("method OpenCallHistory"), helpcontext(11006)] HRESULT OpenCallHistory(void);
	[id(7), helpstring("method OpenContacts"), helpcontext(11007)] HRESULT OpenContacts(void);
	[id(8), helpstring("method OpenDialpad"), helpcontext(11008)] HRESULT OpenDialpad(void);
	[id(9), helpstring("method OpenSendContacts"), helpcontext(11009)] HRESULT OpenSendContacts(void);
	[id(10), helpstring("method OpenBlockedUsers"), helpcontext(11010)] HRESULT OpenBlockedUsers(void);
	[id(11), helpstring("method OpenImportContacts"), helpcontext(11011)] HRESULT OpenImportContacts(void);
	[id(12), helpstring("method OpenGettingStarted"), helpcontext(11012)] HRESULT OpenGettingStarted(void);
	[id(13), helpstring("method OpenAuthorization"), helpcontext(11013)] HRESULT OpenAuthorization([in] BSTR UserName);
};



// UI

[
	coclass,
	threading(apartment),
	vi_progid("ActiveST.UI"),
	progid("ActiveST.UI.1"),
	version(1.0),
	uuid("D9770D9B-BB30-4a82-BC01-83B110408D5C"),
	helpcontext(11000), 
	helpstring("UI Class")
]
class ATL_NO_VTABLE UI : 
	public IUI
{
public:
	UI()
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

	inline HRESULT Execute(LPCWSTR Dialog);

public:

	STDMETHOD(OpenProfile)(void);
	STDMETHOD(OpenUserInfo)(BSTR UserName);
	STDMETHOD(OpenConference)(void);
	STDMETHOD(OpenSearch)(void);
	STDMETHOD(OpenOptions)(SkypeOptionsPage Page);
	STDMETHOD(OpenCallHistory)(void);
	STDMETHOD(OpenContacts)(void);
	STDMETHOD(OpenDialpad)(void);
	STDMETHOD(OpenSendContacts)(void);
	STDMETHOD(OpenBlockedUsers)(void);
	STDMETHOD(OpenImportContacts)(void);
	STDMETHOD(OpenGettingStarted)(void);
	STDMETHOD(OpenAuthorization)(BSTR UserName);
};

