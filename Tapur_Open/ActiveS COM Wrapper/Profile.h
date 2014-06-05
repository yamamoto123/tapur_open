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
// Profile.h : Declaration of the Profile

#pragma once
#include "resource.h"       // main symbols

[
	export,
	uuid("1F2CC8D7-37CA-4e76-88DD-F79486265F59")
]
struct ForwardingRule {
	LONG StartTime;
	LONG EndTime;
	BSTR Handle;
};

// IProfile
[
	object,
	uuid("5106D5C0-C57A-4ac7-A361-ECC530A55BDE"),
	dual,	helpstring("IProfile Interface"),
	helpcontext(10000),
	pointer_default(unique)
]
__interface IProfile : IDispatch
{
	[propget, id(1), helpstring("property FullName"), helpcontext(10001)] HRESULT FullName([out, retval] BSTR* pVal);
	[propput, id(1), helpstring("property FullName"), helpcontext(10001)] HRESULT FullName([in] BSTR newVal);
	[propget, id(2), helpstring("property Birthday"), helpcontext(10002)] HRESULT Birthday([out, retval] DATE* pVal);
	[propput, id(2), helpstring("property Birthday"), helpcontext(10002)] HRESULT Birthday([in] DATE newVal);
	[propget, id(3), helpstring("property Sex"), helpcontext(10003)] HRESULT Sex([out, retval] SkypeUserSex* pVal);
	[propput, id(3), helpstring("property Sex"), helpcontext(10003)] HRESULT Sex([in] SkypeUserSex newVal);
	[propget, id(4), helpstring("property Languages"), helpcontext(10004), satype(BSTR)] HRESULT Languages([out, retval, satype(BSTR)] SAFEARRAY ** pVal);
	[propput, id(4), helpstring("property Languages"), helpcontext(10004), satype(BSTR)] HRESULT Languages([in, satype(BSTR)] SAFEARRAY * newVal);
	[propget, id(5), helpstring("property Country"), helpcontext(10005), satype(BSTR)] HRESULT Country([out, retval] BSTR * pVal);
	[propput, id(5), helpstring("property Country"), helpcontext(10005), satype(BSTR)] HRESULT Country([in] BSTR newVal);
	[propget, id(6), helpstring("property Province"), helpcontext(10006)] HRESULT Province([out, retval] BSTR* pVal);
	[propput, id(6), helpstring("property Province"), helpcontext(10006)] HRESULT Province([in] BSTR newVal);
	[propget, id(7), helpstring("property City"), helpcontext(10007)] HRESULT City([out, retval] BSTR* pVal);
	[propput, id(7), helpstring("property City"), helpcontext(10007)] HRESULT City([in] BSTR newVal);
	[propget, id(8), helpstring("property HomePhone"), helpcontext(10008)] HRESULT HomePhone([out, retval] BSTR* pVal);
	[propput, id(8), helpstring("property HomePhone"), helpcontext(10008)] HRESULT HomePhone([in] BSTR newVal);
	[propget, id(9), helpstring("property OfficePhone")] HRESULT OfficePhone([out, retval] BSTR* pVal);
	[propput, id(9), helpstring("property OfficePhone")] HRESULT OfficePhone([in] BSTR newVal);
	[propget, id(10), helpstring("property MobilePhone"), helpcontext(10010)] HRESULT MobilePhone([out, retval] BSTR* pVal);
	[propput, id(10), helpstring("property MobilePhone"), helpcontext(10010)] HRESULT MobilePhone([in] BSTR newVal);
	[propget, id(11), helpstring("property Homepage"), helpcontext(10011)] HRESULT Homepage([out, retval] BSTR* pVal);
	[propput, id(11), helpstring("property Homepage"), helpcontext(10011)] HRESULT Homepage([in] BSTR newVal);
	[propget, id(12), helpstring("property About")] HRESULT About([out, retval] BSTR* pVal);
	[propput, id(12), helpstring("property About")] HRESULT About([in] BSTR newVal);
	[propget, id(13), helpstring("property MoodText"), helpcontext(10013)] HRESULT MoodText([out, retval] BSTR* pVal);
	[propput, id(13), helpstring("property MoodText"), helpcontext(10013)] HRESULT MoodText([in] BSTR newVal);
	[propget, id(14), helpstring("property Timezone"), helpcontext(10014)] HRESULT Timezone([out, retval] LONG* pVal);
	[propput, id(14), helpstring("property Timezone"), helpcontext(10014)] HRESULT Timezone([in] LONG newVal);
	[propget, id(15), helpstring("property ForwardingRules")] HRESULT ForwardingRules([out, retval, satype(struct ForwardingRule)] SAFEARRAY ** pVal);
	[propput, id(15), helpstring("property ForwardingRules")] HRESULT ForwardingRules([in, satype(struct ForwardingRule)] SAFEARRAY * newVal);
	[propget, id(16), helpstring("property NoAnswerTimeout"), helpcontext(10016)] HRESULT NoAnswerTimeout([out, retval] LONG* pVal);
	[propput, id(16), helpstring("property NoAnswerTimeout"), helpcontext(10016)] HRESULT NoAnswerTimeout([in] LONG newVal);
	[propget, id(17), helpstring("property CallForwardingEnabled"), helpcontext(10017)] HRESULT CallForwardingEnabled([out, retval] VARIANT_BOOL* pVal);
	[propput, id(17), helpstring("property CallForwardingEnabled"), helpcontext(10017)] HRESULT CallForwardingEnabled([in] VARIANT_BOOL newVal);
	[propget, id(18), helpstring("property VoicemailEnabled"), helpcontext(10018)] HRESULT VoicemailEnabled([out, retval] VARIANT_BOOL* pVal);
	[propput, id(18), helpstring("property VoicemailEnabled"), helpcontext(10018)] HRESULT VoicemailEnabled([in] VARIANT_BOOL newVal);
	[propget, id(19), helpcontext(10019), helpstring("property RingtoneStatus")] HRESULT RingtoneStatus([in] SkypeRingtone Tone, [out, retval] VARIANT_BOOL *pVal);
	[propget, id(20), helpcontext(10020), helpstring("property RingtoneFileName")] HRESULT RingtoneFileName([in] SkypeRingtone Tone, [out, retval] BSTR * FileName);
	[propput, id(20), helpcontext(10020), helpstring("property RingtoneFileName")] HRESULT RingtoneFileName([in] SkypeRingtone Tone, [in] BSTR FileName);
	[propput, id(21), helpcontext(10021), helpstring("property AvatarFileName")] HRESULT AvatarFileName([in] LONG Index, [in] BSTR FileName);
};



// Profile

[
	coclass,
	threading(apartment),
	support_error_info("IProfile"),
	vi_progid("ActiveST.Profile"),
	progid("ActiveST.Profile.1"),
	version(1.0),
	uuid("154ED84A-F2A2-407a-BB81-BCFEAAAD463D"),
	helpstring("Profile Class")
]
class ATL_NO_VTABLE Profile : 
	public IProfile
{
public:
	Profile()
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

	STDMETHOD(get_FullName)(BSTR* pVal);
	STDMETHOD(put_FullName)(BSTR newVal);
	STDMETHOD(get_Birthday)(DOUBLE* pVal);
	STDMETHOD(put_Birthday)(DOUBLE newVal);
	STDMETHOD(get_Sex)(SkypeUserSex* pVal);
	STDMETHOD(put_Sex)(SkypeUserSex newVal);
	STDMETHOD(get_Languages)(SAFEARRAY ** pVal);
	STDMETHOD(put_Languages)(SAFEARRAY * newVal);
	STDMETHOD(get_Country)(BSTR* pVal);
	STDMETHOD(put_Country)(BSTR newVal);
	STDMETHOD(get_Province)(BSTR* pVal);
	STDMETHOD(put_Province)(BSTR newVal);
	STDMETHOD(get_City)(BSTR* pVal);
	STDMETHOD(put_City)(BSTR newVal);
	STDMETHOD(get_HomePhone)(BSTR* pVal);
	STDMETHOD(put_HomePhone)(BSTR newVal);
	STDMETHOD(get_OfficePhone)(BSTR* pVal);
	STDMETHOD(put_OfficePhone)(BSTR newVal);
	STDMETHOD(get_MobilePhone)(BSTR* pVal);
	STDMETHOD(put_MobilePhone)(BSTR newVal);
	STDMETHOD(get_Homepage)(BSTR* pVal);
	STDMETHOD(put_Homepage)(BSTR newVal);
	STDMETHOD(get_About)(BSTR* pVal);
	STDMETHOD(put_About)(BSTR newVal);
	STDMETHOD(get_MoodText)(BSTR* pVal);
	STDMETHOD(put_MoodText)(BSTR newVal);
	STDMETHOD(get_Timezone)(LONG* pVal);
	STDMETHOD(put_Timezone)(LONG newVal);
	STDMETHOD(get_ForwardingRules)(SAFEARRAY ** pVal);
	STDMETHOD(put_ForwardingRules)(SAFEARRAY * newVal);
	STDMETHOD(get_NoAnswerTimeout)(LONG* pVal);
	STDMETHOD(put_NoAnswerTimeout)(LONG newVal);
	STDMETHOD(get_CallForwardingEnabled)(VARIANT_BOOL * pVal);
	STDMETHOD(put_CallForwardingEnabled)(VARIANT_BOOL newVal);
	STDMETHOD(get_VoicemailEnabled)(VARIANT_BOOL * pVal);
	STDMETHOD(put_VoicemailEnabled)(VARIANT_BOOL newVal);
	STDMETHOD(get_RingtoneStatus)( SkypeRingtone Tone, VARIANT_BOOL *pVal);
	STDMETHOD(get_RingtoneFileName)( SkypeRingtone Tone, BSTR * FileName);
	STDMETHOD(put_RingtoneFileName)( SkypeRingtone Tone,  BSTR FileName);
//	STDMETHOD(get_AvatarFileName)( SkypeRingtone Tone, BSTR * FileName);
	STDMETHOD(put_AvatarFileName)( LONG Index,  BSTR FileName);
};

