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
// App2AppStream.h : Declaration of the App2AppStream

#pragma once
#include "resource.h"       // main symbols

namespace SkypeAPILib
{

// IApp2AppStream
[
	object,
	uuid("F4E034C7-B5CD-4852-9A89-011F30CE42CE"),
	dual,	helpstring("IApp2AppStream Interface"),
	helpcontext(9500), 
	pointer_default(unique)
]
__interface IApp2AppStream : IDispatch
{
	[id(1), helpcontext(9501), helpstring("method Disconnect")] HRESULT Disconnect(void);
	[id(2), helpcontext(9502), helpstring("method WriteData")] HRESULT WriteData([in] BSTR TextData);
	[id(3), helpcontext(9503), helpstring("method WriteDatagram")] HRESULT WriteDatagram([in] BSTR TextData);
	[propget, id(4), helpcontext(9504), helpstring("property StreamID")] HRESULT StreamID([out, retval] BSTR* pVal);
	[propget, id(5), helpcontext(9505), helpstring("property User")] HRESULT User([out, retval] IUser** pVal);
	[id(6), helpcontext(9506), helpstring("method WriteBinaryData"), satype(BYTE)] HRESULT WriteBinaryData([in] BSTR ID, [in] SAFEARRAY ** Array);
};



// App2AppStream

[
	coclass,
	noncreatable,	
	default(IApp2AppStream),
	threading(apartment),
	support_error_info("IApp2AppStream"),
	vi_progid("ActiveST.App2AppStream"),
	progid("ActiveST.App2AppStream.1"),
	version(1.0),
	uuid("0CC09B21-449C-4d70-A304-5838500E108F"),
	helpstring("App2AppStream Class")
]
class ATL_NO_VTABLE App2AppStream : 
	public IApp2AppStream
{
public:
	App2AppStream()
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

	void Initialize(LPCWSTR AppName, LPCWSTR StreamID);

private:
	wstring m_strAppName;
	wstring m_strStreamID;

public:

	STDMETHOD(Disconnect)(void);
	STDMETHOD(WriteData)(BSTR TextData);
	STDMETHOD(WriteDatagram)(BSTR TextData);
	STDMETHOD(get_StreamID)(BSTR * pVal);
	STDMETHOD(get_User)(IUser** pVal);
	STDMETHOD(WriteBinaryData)(BSTR ID, SAFEARRAY ** Array);
};

}