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
// Chat.h : Declaration of the Chat

#pragma once
#include "resource.h"       // main symbols

namespace SkypeAPILib 
{

// IChat
[
	object,
	uuid("F3600A6E-3B69-4871-9AF7-AF0881B3A760"),
	dual, 
	helpstring("IChat Interface"),
	helpcontext(5000),
	pointer_default(unique)
]
__interface IChat : IDispatch
{
	[propget, id(1), helpcontext(5001), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
	[propget, id(2), helpcontext(5002), helpstring("property Timestamp")] HRESULT Timestamp([out, retval] DATE *pVal);
	[propget, id(3), helpcontext(5003), helpstring("property Adder")] HRESULT Adder([out, retval] IUser **pVal);
	[propget, id(4), helpcontext(5004), helpstring("property Posters")] HRESULT Posters([out, retval] IUserCollection **pVal);
	[propget, id(5), helpcontext(5005), helpstring("property Members")] HRESULT Members([out, retval] IUserCollection **pVal);
	[propget, id(6), helpcontext(5006), helpstring("property Topic")] HRESULT Topic([out, retval] BSTR *pVal);
	[propput, id(6), helpcontext(5006), helpstring("property Topic")] HRESULT Topic([in] BSTR Topic);
	[propget, id(7), helpcontext(5007), helpstring("property ActiveMembers")] HRESULT ActiveMembers([out, retval] IUserCollection **pVal);
	[propget, id(8), helpcontext(5008), helpstring("property FriendlyName")] HRESULT FriendlyName([out, retval] BSTR *pVal);
	[propget, id(9), helpcontext(5009), helpstring("property Status")] HRESULT Status([out, retval] SkypeChatStatus *pVal);
	[id(10), helpcontext(5010), helpstring("method SendMessage")] HRESULT SendMessage([in] BSTR Message);
	[id(11), helpcontext(5011), helpstring("method Leave")] HRESULT Leave();
	[id(12), helpcontext(5012), helpstring("method AddMembers")] HRESULT AddMembers([in, satype(BSTR)] SAFEARRAY **UserHandles);
	[id(13), helpcontext(5013), helpstring("method LaunchChatWindow")] HRESULT LaunchChatWindow();
	[id(14), helpcontext(5014), helpstring("method GetChatMessageList")] HRESULT GetChatMessageList([out, retval] IChatMessageCollection **List);
	[id(15), helpcontext(5015), helpstring("method GetRecentChatMessageList")] HRESULT GetRecentChatMessageList([out, retval] IChatMessageCollection **List);
};



// Chat

[
	coclass,
	noncreatable,	
	default(IChat),
	threading(apartment),
	support_error_info("IChat"),
	vi_progid("ActiveST.Chat"),
	progid("ActiveST.Chat.1"),
	version(1.0),
	uuid("0FA5B4B9-DE42-4e96-9EEE-6515DB9DBA0E"),
	helpstring("Chat Class")
]
class ATL_NO_VTABLE Chat : 
	public IChat
{
public:
	Chat()
	{
		m_strName = L"";
	}

	// This needs to be called before using other methods of this class
	void SetName(LPCWSTR Name) {
		m_strName = Name;
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

protected:
	std::wstring m_strName;

// IChat
public:
	STDMETHOD(get_FriendlyName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ActiveMembers)(/*[out, retval]*/ IUserCollection **pVal);
	STDMETHOD(get_Topic)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Topic)(/*[in]*/ BSTR Topic);
	STDMETHOD(get_Members)(/*[out, retval]*/ IUserCollection **pVal);
	STDMETHOD(get_Posters)(/*[out, retval]*/ IUserCollection **pVal);
	STDMETHOD(get_Status)(/*[out, retval]*/ SkypeChatStatus *pVal);
	STDMETHOD(get_Adder)(/*[out, retval]*/ IUser **pVal);
	STDMETHOD(get_Timestamp)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(SendMessage)(/*[in]*/ BSTR Message);
	STDMETHOD(Leave)();
	STDMETHOD(AddMembers)(/*[in]*/ SAFEARRAY **UserHandles);
	STDMETHOD(LaunchChatWindow)();
	STDMETHOD(GetChatMessageList)(/*[out, retval]*/ IChatMessageCollection **List);
	STDMETHOD(GetRecentChatMessageList)(/*[out, retval]*/ IChatMessageCollection **List);
};

}