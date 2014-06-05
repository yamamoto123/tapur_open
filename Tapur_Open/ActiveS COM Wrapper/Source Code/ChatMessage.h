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
// ChatMessage.h : Declaration of the ChatMessage

#pragma once
#include "resource.h"       // main symbols

namespace SkypeAPILib 
{

// Forward declarations
[object]
__interface IChat; 

// IChatMessage
[
	object,
	uuid("5BF1A928-2A65-4482-892D-599047A57D2B"),
	dual,	helpstring("IChatMessage Interface"),
	helpcontext(6000),
	pointer_default(unique)
]
__interface IChatMessage : IDispatch
{
	[propget, id(1), helpcontext(6001), helpstring("property ID")] HRESULT ID([out, retval] long *pVal);
	[propget, id(2), helpcontext(6002), helpstring("property Chat")] HRESULT Chat([out, retval] IChat **pVal);
	[propget, id(3), helpcontext(6003), helpstring("property Timestamp")] HRESULT Timestamp([out, retval] DATE *pVal);
	[propget, id(4), helpcontext(6004), helpstring("property MessageFrom")] HRESULT MessageFrom([out, retval] IUser **pVal);
	[propget, id(5), helpcontext(6005), helpstring("property MessageType")] HRESULT MessageType([out, retval] SkypeChatMessageType *pVal);
	[propget, id(6), helpcontext(6006), helpstring("property Users")] HRESULT Users([out, retval] IUserCollection **pVal);
	[propget, id(7), helpcontext(6007), helpstring("property LeftReason")] HRESULT LeftReason([out, retval] SkypeChatMessageLeftReason *pVal);
	[propget, id(8), helpcontext(6008), helpstring("property Body")] HRESULT Body([out, retval] BSTR *pVal);
	[propget, id(9), helpcontext(6009), helpstring("property Status")] HRESULT Status([out, retval] SkypeChatMessageStatus *pVal);
	[propput, id(9), helpcontext(6009), helpstring("property Status")] HRESULT Status([in] SkypeChatMessageStatus newVal);
};



// ChatMessage

[
	coclass,
	noncreatable,	
	default(IChatMessage),
	threading(apartment),
	support_error_info("IChatMessage"),
	vi_progid("ActiveST.ChatMessage"),
	progid("ActiveST.ChatMessage.1"),
	version(1.0),
	uuid("A396E674-9241-463f-A71E-AD57BA579AD0"),
	helpstring("ChatMessage Class")
]
class ATL_NO_VTABLE ChatMessage : 
	public IChatMessage
{
public:
	ChatMessage()
	{
		m_lID = -1;
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
	long m_lID;
public:
	STDMETHOD(get_MessageType)(/*[out, retval]*/ SkypeChatMessageType *pVal);
	STDMETHOD(get_Status)(/*[out, retval]*/ SkypeChatMessageStatus *pVal);
	STDMETHOD(put_Status)(/*[in]*/ SkypeChatMessageStatus /*newVal*/);
	STDMETHOD(get_Body)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_LeftReason)(/*[out, retval]*/ SkypeChatMessageLeftReason *pVal);
	STDMETHOD(get_Users)(/*[out, retval]*/ IUserCollection **pVal);
	STDMETHOD(get_MessageFrom)(/*[out, retval]*/ IUser **pVal);
	STDMETHOD(get_Timestamp)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_Chat)(/*[out, retval]*/ IChat **pVal);
	STDMETHOD(get_ID)(/*[out, retval]*/ long *pVal);

	void SetID(long ID);
};

}