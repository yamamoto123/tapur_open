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
// Message.h : Declaration of the Message

#pragma once
#include "resource.h"       // main symbols

namespace SkypeAPILib
{

// IMessage
[
	object,
	uuid("B4E8AFEF-B5CE-4681-A0F5-53A32B168676"),
	dual,	helpstring("IMessage Interface"),
	helpcontext(7000),
	pointer_default(unique)
]
__interface IMessage : IDispatch
{
	[propget, id(1), helpcontext(7001), helpstring("property ID")] HRESULT ID([out, retval] LONG *pVal);
	[propget, id(2), helpcontext(7002), helpstring("property Status")] HRESULT Status([out, retval] SkypeMessageStatus *pVal);
	[propget, id(3), helpcontext(7003), helpstring("property Timestamp")] HRESULT Timestamp([out, retval] DATE *pVal);
	[propget, id(4), helpcontext(7004), helpstring("property PartnerHandle")] HRESULT PartnerHandle([out, retval] BSTR *pVal);
	[propget, id(5), helpcontext(7005), helpstring("property PartnerDisplayName")] HRESULT PartnerDisplayName([out, retval] BSTR *pVal);
	[propget, id(6), helpcontext(7006), helpstring("property ConferenceID")] HRESULT ConferenceID([out, retval] LONG *pVal);
	[propget, id(7), helpcontext(7007), helpstring("property Type")] HRESULT Type([out, retval] SkypeMessageType *pVal);
	[propget, id(8), helpcontext(7008), helpstring("property TypeAsText")] HRESULT TypeAsText([out, retval] BSTR *pVal);
	[propget, id(9), helpcontext(7009), helpstring("property Body")] HRESULT Body([out, retval] BSTR *pVal);
	[propget, id(10), helpcontext(7010), helpstring("property Seen")] HRESULT Seen([out, retval] VARIANT_BOOL *pVal);
	[propput, id(10), helpcontext(7010), helpstring("property Seen")] HRESULT Seen([in] VARIANT_BOOL newVal);
};


// Message

[
	coclass,
	noncreatable,	
	default(IMessage),
	threading(apartment),
	support_error_info("IMessage"),
	vi_progid("ActiveST.Message"),
	progid("ActiveST.Message.1"),
	version(1.0),
	uuid("625229B6-53B2-4f05-9062-1E36C117B373"),
	helpstring("Message Class")
]
class ATL_NO_VTABLE Message : 
	public IMessage
{
public:
	Message()
	{
		m_lMessageID = -1;
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
	long m_lMessageID;
public:
	STDMETHOD(get_Seen)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Seen)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Body)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_TypeAsText)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Type)(/*[out, retval]*/ SkypeMessageType *pVal);
	STDMETHOD(get_ConferenceID)(/*[out, retval]*/ LONG *pVal);
	STDMETHOD(get_PartnerDisplayName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_PartnerHandle)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Timestamp)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_Status)(/*[out, retval]*/ SkypeMessageStatus *pVal);
	STDMETHOD(get_ID)(/*[out, retval]*/ LONG *pVal);
	void SetMessageID(LONG ID);
};

}