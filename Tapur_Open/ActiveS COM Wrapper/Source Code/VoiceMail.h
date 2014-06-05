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
// VoiceMail.h : Declaration of the VoiceMail

#pragma once
#include "resource.h"       // main symbols

namespace SkypeAPILib 
{

// IVoiceMail
[
	object,
	uuid("5A45154B-09C9-4708-82F6-8D70F0FA6BC6"),
	dual,	helpstring("IVoiceMail Interface"),
	helpcontext(8000),
	pointer_default(unique)
]
__interface IVoiceMail : IDispatch
{
	[propget, id(1), helpstring("property ID"), helpcontext(8001)] HRESULT ID([out, retval] LONG* pVal);
	[propget, id(2), helpstring("property Type"), helpcontext(8002)] HRESULT Type([out, retval] SkypeVoiceMailType* pVal);
	[propget, id(3), helpstring("property PartnerHandle"), helpcontext(8003)] HRESULT PartnerHandle([out, retval] BSTR* pVal);
	[propget, id(4), helpstring("property PartnerDisplayName"), helpcontext(8004)] HRESULT PartnerDisplayName([out, retval] BSTR* pVal);
	[propget, id(5), helpstring("property Status"), helpcontext(8005)] HRESULT Status([out, retval] SkypeVoiceMailStatus* pVal);
	[propget, id(6), helpstring("property FailureReason"), helpcontext(8006)] HRESULT FailureReason([out, retval] SkypeVoiceMailFailure* pVal);
	[propget, id(7), helpstring("property Subject"), helpcontext(8007)] HRESULT Subject([out, retval] BSTR* pVal);
	[propget, id(8), helpstring("property Timestamp"), helpcontext(8008)] HRESULT Timestamp([out, retval] DATE* pVal);
	[propget, id(9), helpstring("property Duration"), helpcontext(8009)] HRESULT Duration([out, retval] LONG* pVal);
	[propget, id(10), helpstring("property AllowedDuration"), helpcontext(8010)] HRESULT AllowedDuration([out, retval] LONG* pVal);
	[id(11), helpstring("method OpenAndPlay"), helpcontext(8011)] HRESULT OpenAndPlay();
};



// VoiceMail

[
	coclass,
	noncreatable,	
	default(IVoiceMail),
	threading(apartment),
	support_error_info("IVoiceMail"),
	vi_progid("ActiveST.VoiceMail"),
	progid("ActiveST.VoiceMail.1"),
	version(1.0),
	uuid("8B7ED723-0B44-4d6c-8E6B-1F1E424FD549"),
	helpstring("VoiceMail Class")
]
class ATL_NO_VTABLE VoiceMail : 
	public IVoiceMail
{
public:
	VoiceMail()
	{
		m_lVoiceMailID = -1;
	}

	void SetID(long ID);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
	STDMETHOD(get_ID)(LONG* pVal);
	STDMETHOD(get_Type)(SkypeVoiceMailType* pVal);
	STDMETHOD(get_PartnerHandle)(BSTR* pVal);
	STDMETHOD(get_PartnerDisplayName)(BSTR* pVal);
	STDMETHOD(get_Status)(SkypeVoiceMailStatus* pVal);
	STDMETHOD(get_FailureReason)(SkypeVoiceMailFailure* pVal);
	STDMETHOD(get_Subject)(BSTR* pVal);
	STDMETHOD(get_Timestamp)(DATE* pVal);
	STDMETHOD(get_Duration)(LONG* pVal);
	STDMETHOD(get_AllowedDuration)(LONG* pVal);
	STDMETHOD(OpenAndPlay)();

private:
	long m_lVoiceMailID;
};

} 