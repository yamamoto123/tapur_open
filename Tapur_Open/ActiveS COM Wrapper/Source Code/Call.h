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
// Call.h : Declaration of the Call

#pragma once
#include "resource.h"       // main symbols

namespace SkypeAPILib
{

// ICall
[
	object,
	uuid("76B64C71-D601-4d72-A253-FCFE372ECC62"),
	dual,	helpstring("ICall Interface"),
	pointer_default(unique)
]
__interface ICall : IDispatch
{
	[propget, id(1), helpcontext(3001), helpstring("property ID")] HRESULT ID([out, retval] LONG *pVal);
	[propget, id(2), helpcontext(3002), helpstring("property Timestamp")] HRESULT Timestamp([out, retval] DATE *pVal);
	[propget, id(3), helpcontext(3003), helpstring("property PartnerHandle")] HRESULT PartnerHandle([out, retval] BSTR *pVal);
	[propget, id(4), helpcontext(3004), helpstring("property PartnerDisplayName")] HRESULT PartnerDisplayName([out, retval] BSTR *pVal);
	[propget, id(5), helpcontext(3005), helpstring("property ConferenceID")] HRESULT ConferenceID([out, retval] LONG *pVal);
	[propget, id(6), helpcontext(3006), helpstring("property Type")] HRESULT Type([out, retval] SkypeCallType *pVal);
	[propget, id(7), helpcontext(3007), helpstring("property Status")] HRESULT Status([out, retval] SkypeCallProgress *pVal);
	[propput, id(7), helpcontext(3007), helpstring("property Status")] HRESULT Status([in] SkypeCallProgress newVal);
	[propget, id(8), helpcontext(3008), helpstring("property FailureReason")] HRESULT FailureReason([out, retval] BSTR *pVal);
	[propget, id(9), helpcontext(3009), helpstring("property Subject")] HRESULT Subject([out, retval] BSTR *pVal);
	[propget, id(10), helpcontext(3010), helpstring("property PSTN_Number")] HRESULT PSTN_Number([out, retval] BSTR *pVal);
	[propget, id(11), helpcontext(3011), helpstring("property Duration")] HRESULT Duration([out, retval] LONG *pVal);
	[propget, id(12), helpcontext(3012), helpstring("property PSTN_Status")] HRESULT PSTN_Status([out, retval] BSTR *pVal);
	[propget, id(13), helpcontext(3013), helpstring("property ParticipantCount")] HRESULT ParticipantCount([out, retval] long *pVal);
	[id(14), helpcontext(3014), helpstring("method GetParticipantInformation")] HRESULT GetParticipantInformation([in] LONG Number, [out] BSTR * Handle, [out] SkypeCallType * Type, [out] SkypeCallProgress * Status, [out] BSTR * DisplayName);
	[id(15), helpcontext(3015), helpstring("method JoinConference")] HRESULT JoinConference([in] LONG CallID);
	[propget, id(16), helpcontext(3016), helpstring("property VoiceMailDuration")] HRESULT VoiceMailDuration([out, retval] LONG *pVal);
	[propget, id(17), helpcontext(3017), helpstring("property VoiceMailAllowedDuration")] HRESULT VoiceMailAllowedDuration([out, retval] LONG *pVal);
	[id(18), helpcontext(3018), helpstring("method SendDTMF")] HRESULT SendDTMF([in] BSTR Codes);
	[id(19), helpcontext(3019), helpstring("method GetInput")] HRESULT GetInput([out] BSTR *Soundcard, [out] BSTR *Port, [out] BSTR *File);
	[id(20), helpcontext(3020), helpstring("method GetOutput")] HRESULT GetOutput([out] BSTR *Soundcard, [out] BSTR *Port, [out] BSTR *File);
	[id(21), helpcontext(3021), helpstring("method GetCaptureMic")] HRESULT GetCaptureMic([out] BSTR *Port, [out] BSTR *File);
	[id(22), helpcontext(3022), helpstring("method SetInput")] HRESULT SetInput([in] BSTR Soundcard, [in] BSTR Port, [in] BSTR File);
	[id(23), helpcontext(3023), helpstring("method SetOutput")] HRESULT SetOutput([in] BSTR Soundcard, [in] BSTR Port, [in] BSTR File);
	[id(24), helpcontext(3024), helpstring("method SetCaptureMic")] HRESULT SetCaptureMic([in] BSTR Port, [in] BSTR File);
	[propget, id(25), helpcontext(3025), helpstring("property VideoStatus")] HRESULT VideoStatus([out, retval] BSTR *pVal);
	[id(26), helpcontext(3026), helpstring("method StartVideoSend")] HRESULT StartVideoSend();
	[id(27), helpcontext(3027), helpstring("method StopVideoSend")] HRESULT StopVideoSend();
	[id(28), helpcontext(3028), helpstring("method StartVideoRecieve")] HRESULT StartVideoRecieve();
	[id(29), helpcontext(3029), helpstring("method StopVideoRecieve")] HRESULT StopVideoRecieve();
};



// Call

[
	coclass,
	default(ICall),
	threading(apartment),
	support_error_info("ICall"),
	vi_progid("ActiveST.Call"),
	progid("ActiveST.Call.1"),
	version(1.0),
	uuid("4EE25E18-0517-4bfd-A74B-D35BC8DF9F6D"),
	helpstring("Call Class")
]
class ATL_NO_VTABLE Call : 
	public ICall
{
public:
	Call()
	{
		m_lCallID = -1;
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
	STDMETHOD(GetParticipantInformation)(/*[in]*/ LONG Number, /*[out]*/ BSTR * Handle, /*[out]*/ SkypeCallType * Type, /*[out]*/ SkypeCallProgress * Status, /*[out]*/ BSTR * DisplayName);
	STDMETHOD(get_ParticipantCount)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_PSTN_Status)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Duration)(/*[out, retval]*/ LONG *pVal);
	STDMETHOD(get_PSTN_Number)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Subject)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Status)(/*[out, retval]*/ SkypeCallProgress *pVal);
	STDMETHOD(put_Status)(/*[in]*/ SkypeCallProgress newVal);
	STDMETHOD(get_Type)(/*[out, retval]*/ SkypeCallType *pVal);
	STDMETHOD(get_ConferenceID)(/*[out, retval]*/ LONG *pVal);
	STDMETHOD(get_PartnerDisplayName)(/*[out, retval]*/ BSTR *pVal);
	void SetCallID(long ID);
	STDMETHOD(get_PartnerHandle)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ID)(/*[out, retval]*/ LONG *pVal);
	STDMETHOD(get_Timestamp)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_FailureReason)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(JoinConference)(/*[in]*/ LONG CallID);
	STDMETHOD(get_VoiceMailDuration)(/*[out, retval]*/ LONG *pVal);
	STDMETHOD(get_VoiceMailAllowedDuration)(/*[out, retval]*/ LONG *pVal);
	STDMETHOD(SendDTMF)(/*[in]*/ BSTR Codes);
	STDMETHOD(GetInput)(/*[out]*/ BSTR *Soundcard, /*[out]*/ BSTR *Port, /*[out]*/ BSTR *File);
	STDMETHOD(GetOutput)(/*[out]*/ BSTR *Soundcard, /*[out]*/ BSTR *Port, /*[out]*/ BSTR *File);
	STDMETHOD(GetCaptureMic)(/*[out]*/ BSTR *Port, /*[out]*/ BSTR *File);
	STDMETHOD(SetInput)(/*[in]*/ BSTR Soundcard, /*[in]*/ BSTR Port, /*[in]*/ BSTR File);
	STDMETHOD(SetOutput)(/*[in]*/ BSTR Soundcard, /*[in]*/ BSTR Port, /*[in]*/ BSTR File);
	STDMETHOD(SetCaptureMic)(/*[in]*/ BSTR Port, /*[in]*/ BSTR File);
	STDMETHOD(get_VideoStatus)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(StartVideoSend)();
	STDMETHOD(StopVideoSend)();
	STDMETHOD(StartVideoRecieve)();
	STDMETHOD(StopVideoRecieve)();
private:
	long m_lCallID;

};

}