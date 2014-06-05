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
// Conversion.h : Declaration of the Conversion

#pragma once
#include "resource.h"       // main symbols

DATE UnixTimestampToDATE(time_t t);

SkypeCallProgress TextToSkypeCallProgress(LPCWSTR Text);
SkypeCallType TextToSkypeCallType(LPCWSTR Text);
SkypeMessageStatus TextToSkypeMesasgeStatus(LPCWSTR Text);
SkypeMessageType TextToSkypeMessageType(LPCWSTR Text);
SkypeUserSex TextToSkypeUserSex(LPCWSTR Text);
SkypeBuddyStatus TextToSkypeBuddyStatus(LPCWSTR Text);
SkypeOnlineStatus TextToSkypeOnlineStatus(LPCWSTR Text);
SkypeConnectionStatus TextToSkypeConnectionStatus(LPCWSTR Text);
SkypeChatStatus TextToSkypeChatStatus(LPCWSTR Text);
SkypeChatMessageStatus TextToSkypeChatMessageStatus(LPCWSTR Text);
SkypeChatMessageLeftReason TextToSkypeChatMessageLeftReason(LPCWSTR Text);
SkypeCurrencyType TextToSkypeCurrencyType(LPCWSTR Text);
SkypeChatMessageType TextToSkypeChatMessageType(LPCWSTR Text);
SkypeVoiceMailType TextToSkypeVoiceMailType(LPCWSTR Text);
SkypeVoiceMailStatus TextToSkypeVoiceMailStatus(LPCWSTR Text);
SkypeVoiceMailFailure TextToSkypeVoiceMailFailure(LPCWSTR Text);
SkypeNoAnswerAction TextToSkypeNoAnswerAction(LPCWSTR Text);

BSTR ErrorCodeToBSTR(LONG ErrorCode);

// IConversion
[
	object,
	uuid("E943F65C-CAD7-4e98-B380-54BCA0AB8030"),
	dual,	helpstring("IConversion Interface"),
	helpcontext(2000),
	pointer_default(unique)
]
__interface IConversion : IDispatch
{
	[id(1),  helpcontext(2001), helpstring("method SkypeCallProgressAsText")] HRESULT SkypeCallProgressAsText([in] SkypeCallProgress Progress, [out, retval] BSTR * Text);
	[id(2),  helpcontext(2002), helpstring("method SkypeCallTypeAsText")] HRESULT SkypeCallTypeAsText([in] SkypeCallType CallType, [out, retval] BSTR * Text);
	[id(3),  helpcontext(2003), helpstring("method SkypeMessageStatusAsText")] HRESULT SkypeMessageStatusAsText([in] SkypeMessageStatus Status, [out, retval] BSTR * Text);
	[id(4),  helpcontext(2004), helpstring("method SkypeMessageTypeAsText")] HRESULT SkypeMessageTypeAsText([in] SkypeMessageType Type, [out, retval] BSTR * Text);
	[id(5),  helpcontext(2005), helpstring("method SkypeUserSexAsText")] HRESULT SkypeUserSexAsText([in] SkypeUserSex Sex, [out, retval] BSTR * Text);
	[id(6),  helpcontext(2006), helpstring("method SkypeBuddyStatusAsText")] HRESULT SkypeBuddyStatusAsText([in] SkypeBuddyStatus Status, [out, retval] BSTR * Text);
	[id(7),  helpcontext(2007), helpstring("method SkypeOnlineStatusAsText")] HRESULT SkypeOnlineStatusAsText([in] SkypeOnlineStatus Status, [out, retval] BSTR * Text);
	[id(8),  helpcontext(2008), helpstring("method SkypeConnectionStatusAsText")] HRESULT SkypeConnectionStatusAsText([in] SkypeConnectionStatus Status, [out, retval] BSTR * Text);
	[id(9),  helpcontext(2009), helpstring("method SkypeAPIAttachmentStatusAsText")] HRESULT SkypeAPIAttachmentStatusAsText([in] SkypeAPIAttachmentStatus Status, [out, retval] BSTR *Text);
	[id(10), helpcontext(2010), helpstring("method SkypeChatStatusAsText")] HRESULT SkypeChatStatusAsText([in] SkypeChatStatus ChatStatus, [retval, out] BSTR * Text);
	[id(11), helpcontext(2011), helpstring("method SkypeChatMessageStatusAsText")] HRESULT SkypeChatMessageStatusAsText([in] SkypeChatMessageStatus Status, [out, retval] BSTR * Text);
	[id(12), helpcontext(2012), helpstring("method SkypeChatMessageLeftReasonAsText")] HRESULT SkypeChatMessageLeftReasonAsText([in] SkypeChatMessageLeftReason Reason, [out, retval] BSTR * Text);
	[id(13), helpcontext(2013), helpstring("method SkypeCurrencyTypeAsText")] HRESULT SkypeCurrencyTypeAsText([in] SkypeCurrencyType Currency, [out, retval] BSTR * Text);
	[id(14), helpcontext(2014), helpstring("method SkypeChatMessageTypeAsText")] HRESULT SkypeChatMessageTypeAsText([in] SkypeChatMessageType MessageType, [out, retval] BSTR * Text);
	[id(15), helpcontext(2015), helpstring("method SkypeVoiceMailTypeAsText")] HRESULT SkypeVoiceMailTypeAsText([in] SkypeVoiceMailType VoiceMailType, [out, retval] BSTR * Text);
	[id(16), helpcontext(2016), helpstring("method SkypeVoiceMailStatusAsText")] HRESULT SkypeVoiceMailStatusAsText([in] SkypeVoiceMailStatus VoiceMailStatus, [out, retval] BSTR * Text);
	[id(17), helpcontext(2017), helpstring("method SkypeVoiceMailFailureAsText")] HRESULT SkypeVoiceMailFailureAsText([in] SkypeVoiceMailFailure FailureReason, [out, retval] BSTR * Text);
	[id(18), helpcontext(2018), helpstring("method SkypeNoAnswerActionAsText")] HRESULT SkypeNoAnswerActionAsText([in] SkypeNoAnswerAction Action, [out, retval] BSTR * Text);
};



// Conversion

[
	coclass,
	default(IConversion),
	threading(apartment),
	support_error_info("IConversion"),
	vi_progid("ActiveST.Conversion"),
	progid("ActiveST.Conversion.1"),
	version(1.0),
	uuid("AFFEF374-D73A-4e17-8CAC-72ABDA7008BC"),
	helpstring("Conversion Class")
]
class ATL_NO_VTABLE Conversion : 
	public IConversion
{
public:
	Conversion()
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

	STDMETHOD(SkypeChatMessageLeftReasonAsText)(/*[in]*/ SkypeChatMessageLeftReason Reason, /*[out, retval]*/ BSTR * Text);
	STDMETHOD(SkypeChatMessageStatusAsText)(/*[in]*/ SkypeChatMessageStatus Status, BSTR * Text);
	STDMETHOD(SkypeAPIAttachmentStatusAsText)(/*[in]*/ SkypeAPIAttachmentStatus Status, /*[out, retval]*/ BSTR *Text);
	STDMETHOD(SkypeConnectionStatusAsText)(/*[in]*/ SkypeConnectionStatus Status, /*[out, retval]*/ BSTR * Text);
	STDMETHOD(SkypeOnlineStatusAsText)(/*[in]*/ SkypeOnlineStatus Status, /*[out, retval]*/ BSTR * Text);
	STDMETHOD(SkypeBuddyStatusAsText)(/*[in]*/ SkypeBuddyStatus Status, /*[out, retval]*/ BSTR * Text);
	STDMETHOD(SkypeUserSexAsText)(/*[in]*/ SkypeUserSex Sex, /*[out, retval]*/ BSTR * Text);
	STDMETHOD(SkypeMessageTypeAsText)(/*[in]*/ SkypeMessageType Type, /*[retval, out]*/ BSTR * Text);
	STDMETHOD(SkypeMessageStatusAsText)(/*[in]*/ SkypeMessageStatus Status, /*[out, retval]*/ BSTR * Text);
	STDMETHOD(SkypeCallProgressAsText) (/*[in]*/ SkypeCallProgress Progress, /*[retval, out]*/ BSTR *Text);
	STDMETHOD(SkypeCallTypeAsText) (/*[in]*/ SkypeCallType CallType, /*[retval, out]*/ BSTR * Text);
	STDMETHOD(SkypeChatStatusAsText) (/*[in]*/ SkypeChatStatus ChatStatus, /*[retval, out]*/ BSTR * Text);
	STDMETHOD(SkypeCurrencyTypeAsText) (/*[in]*/ SkypeCurrencyType Currency, /*[retval, out]*/ BSTR * Text);
	STDMETHOD(SkypeChatMessageTypeAsText) (/*[in]*/ SkypeChatMessageType MessageType, /*[retval, out]*/BSTR * Text);
	STDMETHOD(SkypeVoiceMailTypeAsText) (/*[in*/ SkypeVoiceMailType VoiceMailType, /*[retval, out]*/ BSTR * Text);
	STDMETHOD(SkypeVoiceMailStatusAsText) (/*[in*/ SkypeVoiceMailStatus VoiceMailStatus, /*[retval, out]*/ BSTR * Text);
	STDMETHOD(SkypeVoiceMailFailureAsText) (/*[in*/ SkypeVoiceMailFailure VoiceMailFailure, /*[retval, out]*/ BSTR * Text);
	STDMETHOD(SkypeNoAnswerActionAsText) (/*[in*/ SkypeNoAnswerAction Action, /*[retval, out]*/ BSTR * Text);
};

