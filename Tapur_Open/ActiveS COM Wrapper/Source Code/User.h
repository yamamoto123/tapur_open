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
// User.h : Declaration of the User

#pragma once
#include "resource.h"       // main symbols

namespace SkypeAPILib {

// IUser
[
	object,
	uuid("43CCA21E-9F58-4298-B31E-464011A96637"),
	dual,	helpstring("IUser Interface"),
	helpcontext(4001),
	pointer_default(unique)
]
__interface IUser : IDispatch
{
	[propget, id(1), helpcontext(4001), helpstring("property Handle")] HRESULT Handle([out, retval] BSTR *pVal);
	[propget, id(2), helpcontext(4002), helpstring("property FullName")] HRESULT FullName([out, retval] BSTR *pVal);
	[propget, id(3), helpcontext(4003), helpstring("property Birthdate")] HRESULT Birthdate([out, retval] DATE *pVal);
	[propget, id(4), helpcontext(4004), helpstring("property Sex")] HRESULT Sex([out, retval] SkypeUserSex *pVal);
	[propget, id(5), helpcontext(4005), helpstring("property Country")] HRESULT Country([out, retval] BSTR *pVal);
	[propget, id(6), helpcontext(4006), helpstring("property Province")] HRESULT Province([out, retval] BSTR *pVal);
	[propget, id(7), helpcontext(4007), helpstring("property City")] HRESULT City([out, retval] BSTR *pVal);
	[propget, id(8), helpcontext(4008), helpstring("property Phone_Home")] HRESULT Phone_Home([out, retval] BSTR *pVal);
	[propget, id(9), helpcontext(4009), helpstring("property Phone_Office")] HRESULT Phone_Office([out, retval] BSTR *pVal);
	[propget, id(10), helpcontext(4010), helpstring("property Phone_Mobile")] HRESULT Phone_Mobile([out, retval] BSTR *pVal);
	[propget, id(11), helpcontext(4011), helpstring("property Homepage")] HRESULT Homepage([out, retval] BSTR *pVal);
	[propget, id(12), helpcontext(4012), helpstring("property About")] HRESULT About([out, retval] BSTR *pVal);
	[propget, id(13), helpcontext(4013), helpstring("property HasCallEquipment")] HRESULT HasCallEquipment([out, retval] VARIANT_BOOL *pVal);
	[propget, id(14), helpcontext(4014), helpstring("property BuddyStatus")] HRESULT BuddyStatus([out, retval] SkypeBuddyStatus *pVal);
	[propput, id(14), helpcontext(4014), helpstring("property BuddyStatus")] HRESULT BuddyStatus([in] SkypeBuddyStatus Val);
	[propget, id(15), helpcontext(4015), helpstring("property Authorized")] HRESULT Authorized([out, retval] VARIANT_BOOL *pVal);
	[propget, id(16), helpcontext(4016), helpstring("property Blocked")] HRESULT Blocked([out, retval] VARIANT_BOOL *pVal);
	[propget, id(17), helpcontext(4017), helpstring("property DisplayName")] HRESULT DisplayName([out, retval] BSTR *pVal);
	[propget, id(18), helpcontext(4018), helpstring("property OnlineStatus")] HRESULT OnlineStatus([out, retval] SkypeOnlineStatus *pVal);
	[propget, id(19), helpcontext(4019), helpstring("property LastOnline")] HRESULT LastOnline([out, retval] DATE *pVal);
	[propget, id(20), helpcontext(4020), helpstring("property IsSkypeOut")] HRESULT IsSkypeOut([out, retval] VARIANT_BOOL *pVal);
	[propget, id(21), helpcontext(4021), helpstring("property ReceivedAuthorizationRequest")] HRESULT ReceivedAuthorizationRequest([out, retval] BSTR *pVal);
	[propget, id(22), helpcontext(4022), helpstring("property IsAuthorized")] HRESULT IsAuthorized([out, retval] VARIANT_BOOL *pVal);
	[propput, id(22), helpcontext(4022), helpstring("property IsAuthorized")] HRESULT IsAuthorized([in] VARIANT_BOOL Val);
	[propget, id(23), helpcontext(4023), helpstring("property IsBlocked")] HRESULT IsBlocked([out, retval] VARIANT_BOOL *pVal);
	[propput, id(23), helpcontext(4023), helpstring("property IsBlocked")] HRESULT IsBlocked([in] VARIANT_BOOL Val);
	[propget, id(24), helpcontext(4024), helpstring("property SpeedDial")] HRESULT SpeedDial([out, retval] BSTR * pVal);
	[propput, id(24), helpcontext(4024), helpstring("property SpeedDial")] HRESULT SpeedDial([in] BSTR Val);
	[propget, id(25), helpstring("property MoodText"), helpcontext(4025)] HRESULT MoodText([out, retval] BSTR* pVal);
	[propget, id(26), helpstring("property Aliases"), helpcontext(4026)] HRESULT Aliases([out, retval, satype(BSTR)] SAFEARRAY ** pVal);
	[propget, id(27), helpstring("property Timezone"), helpcontext(4027)] HRESULT Timezone([out, retval] LONG* pVal);
	[propget, id(28), helpcontext(4028), helpstring("property Language")] HRESULT Language([out, retval] BSTR *pVal);
};



// User

[
	coclass,
	noncreatable,	
	default(IUser),
	threading(apartment),
	support_error_info("IUser"),
	vi_progid("ActiveST.User"),
	progid("ActiveST.User.1"),
	version(1.0),
	uuid("79ED0EAF-91EF-4eef-A197-0A9374837836"),
	helpstring("User Class")
]
class ATL_NO_VTABLE User : 
	public IUser
{
public:
	User()
	{
		m_strUserName = L"";
	}

	// This needs to be called before using other methods of this class
	void SetUserName(LPCWSTR UserName) {
		m_strUserName = UserName;
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
	std::wstring m_strUserName;

public:

	STDMETHOD(get_IsSkypeOut)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_LastOnline)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_OnlineStatus)(/*[out, retval]*/ SkypeOnlineStatus *pVal);
	STDMETHOD(get_DisplayName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Blocked)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_Authorized)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_BuddyStatus)(/*[out, retval]*/ SkypeBuddyStatus *pVal);
	STDMETHOD(put_BuddyStatus)(/*[in]*/ SkypeBuddyStatus Val);
	STDMETHOD(get_HasCallEquipment)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_About)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Homepage)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Phone_Mobile)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Phone_Office)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Phone_Home)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_City)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Province)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Country)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Sex)(/*[out, retval]*/ SkypeUserSex *pVal);
	STDMETHOD(get_Birthdate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_FullName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Handle)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ReceivedAuthorizationRequest)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_IsAuthorized)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_IsAuthorized)(/*[in]*/ VARIANT_BOOL Val);
	STDMETHOD(get_IsBlocked)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_IsBlocked)(/*[in]*/ VARIANT_BOOL Val);
	STDMETHOD(get_SpeedDial)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_SpeedDial)(/*[in]*/ BSTR Val);
	STDMETHOD(get_MoodText)(/*[out, retval]*/ BSTR* pVal);
	STDMETHOD(get_Aliases)(/*[out, retval]*/ SAFEARRAY ** pVal);
	STDMETHOD(get_Timezone)(/*[out, retval]*/ LONG* pVal);
	STDMETHOD(get_Language)(/*[out, retval]*/ BSTR *pVal);
};

}