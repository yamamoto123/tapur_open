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
// Access.h : Declaration of the Access
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>

// Disable this warning because we're overriding some attribute-injected stuff
#pragma warning(disable:4199)

namespace SkypeAPILib 
{
// _IAccessEvents
[
	uuid("F2858E1B-90D5-4457-A096-2B19898ADAB2"),
	dispinterface,
	helpcontext(1500),
	helpstring("_IAccessEvents Interface")
]
__interface _IAccessEvents
{
	[id(1), helpcontext(1501), helpstring("method Result")] HRESULT Result([in] BSTR Description);
	[id(2), helpcontext(1502), helpstring("method CallStatusChanged")] HRESULT CallStatusChanged([in] ICall * ChangedCall, [in] SkypeCallProgress Status);
	[id(3), helpcontext(1503), helpstring("method Command")] HRESULT Command([in] BSTR SkypeCommand);
	[id(4), helpcontext(1504), helpstring("method UserStatusChanged")] HRESULT UserStatusChanged([in] SkypeOnlineStatus Status);
	[id(5), helpcontext(1505), helpstring("method MuteChanged")] HRESULT MuteChanged(VARIANT_BOOL Mute);
	[id(6), helpcontext(1506), helpstring("method ConnectionStatusChanged")] HRESULT ConnectionStatusChanged([in] SkypeConnectionStatus Status);
	[id(7), helpcontext(1507), helpstring("method MessageReceived")] HRESULT MessageReceived([in] IMessage * SkypeMessage);
	[id(8), helpcontext(1508), helpstring("method MessageSent")] HRESULT MessageSent([in] IMessage * SkypeMessage);
	[id(9), helpcontext(1509), helpstring("method APIStatusChanged")] HRESULT APIStatusChanged(SkypeAPIAttachmentStatus Status);
	[id(10), helpcontext(1510), helpstring("method CallHistoryChanged")] HRESULT CallHistoryChanged();
	[id(11), helpcontext(1511), helpstring("method IMHistoryChanged")] HRESULT IMHistoryChanged();
	[id(12), helpcontext(1512), helpstring("method Error")] HRESULT Error([in] LONG Number, [in] BSTR Description);
	[id(13), helpcontext(1513), helpstring("method OnlineStatusChanged")] HRESULT OnlineStatusChanged([in] IUser * SkypeUser, [in] SkypeOnlineStatus Status);
	[id(14), helpcontext(1514), helpstring("method ChatMessageReceived")] HRESULT ChatMessageReceived([in] IChatMessage * SkypeChatMessage);
	[id(15), helpcontext(1515), helpstring("method ChatMessageSent")] HRESULT ChatMessageSent([in] IChatMessage * SkypeChatMessage);
	[id(16), helpcontext(1516), helpstring("method IncomingDTMF")] HRESULT IncomingDTMF([in] ICall * ConnectedCall, [in] BSTR DTMF);
	[id(17), helpcontext(1517), helpstring("method AppConnecting")] HRESULT AppConnecting([in] IApp2App * Application, [in] IUserCollection * Users);
	[id(18), helpcontext(1518), helpstring("method AppStreamOpened")] HRESULT AppStreamOpened([in] IApp2App * Application, [in] IApp2AppStream * Stream);
	[id(19), helpcontext(1519), helpstring("method AppDataReceived")] HRESULT AppDataReceived([in] IApp2AppStream * Stream, [in] BSTR Data);
	[id(20), helpcontext(1520), helpstring("method AppDatagramReceived")] HRESULT AppDatagramReceived([in] IApp2AppStream * Stream, [in] BSTR Data);
	[id(21), helpcontext(1521), helpstring("method AppStreamClosed")] HRESULT AppStreamClosed([in] IApp2App * Application, [in] IApp2AppStream * Stream);
	[id(22), helpcontext(1522), helpstring("method ContactFocused")] HRESULT ContactFocused([in] IUser * Contact);
	[id(23), helpcontext(1523), helpstring("method BinaryTransferStart")] HRESULT BinaryTransferStart([in] BSTR TransferID, [in] LONG BytesTotal);
	[id(24), helpcontext(1524), helpstring("method BinaryTransferData")] HRESULT BinaryTransferData([in] BSTR TransferID, [in] LONG BytesTransferred, [in] LONG BytesTotal);
	[id(25), helpcontext(1525), helpstring("method BinaryTransferEnd")] HRESULT BinaryTransferEnd([in] BSTR TransferID, [in, satype("BYTE")] SAFEARRAY ** Data);
	[id(26), helpcontext(1526), helpstring("method BinaryTransferError")] HRESULT BinaryTransferError([in] BSTR TransferID, [in] BSTR Description);
};

// IAccess
[
	object,
	uuid("56559E11-B2E0-4251-991D-9DA23BFC1C18"),
	dual,
	helpstring("IAccess Interface"),
	helpcontext(1000),
	pointer_default(unique)
]
__interface IAccess : public IDispatch
{
	[id(1), helpcontext(1001), helpstring("method SendCommand")] HRESULT SendCommand([in] BSTR Command);
	[id(2), helpcontext(1002), helpstring("method GetCall")] HRESULT GetCall([in] LONG CallID, [out, retval] ICall ** Call);
	[id(3), helpcontext(1003), helpstring("method PlaceCall")] HRESULT PlaceCall([in] BSTR UserName, [out, retval] ICall ** Call);
	[id(4), helpcontext(1004), helpstring("method SearchForUser")] HRESULT SearchForUser([in] BSTR UserName, [out, retval] IUserCollection ** List);
	[id(5), helpcontext(1005), helpstring("method GetCallList")] HRESULT GetCallList([out, retval] ICallCollection ** List);
	[id(6), helpcontext(1006), helpstring("method GetFriendList")] HRESULT GetFriendList([out, retval] IUserCollection ** List);
	[propget, id(7), helpcontext(1007), helpstring("property ConnectionStatus")] HRESULT ConnectionStatus([out, retval] SkypeConnectionStatus *pVal);
	[propget, id(8), helpcontext(1008), helpstring("property AudioIn")] HRESULT AudioIn([out, retval] BSTR *pVal);
	[propput, id(8), helpcontext(1008), helpstring("property AudioIn")] HRESULT AudioIn([in] BSTR Val);
	[propget, id(9), helpcontext(1009), helpstring("property AudioOut")] HRESULT AudioOut([out, retval] BSTR *pVal);
	[propput, id(9), helpcontext(1008), helpstring("property AudioOut")] HRESULT AudioOut([in] BSTR Val);
	[propget, id(10), helpcontext(1010), helpstring("property CurrentUserHandle")] HRESULT CurrentUserHandle([out, retval] BSTR *pVal);
	[propget, id(11), helpcontext(1011), helpstring("property CurrentUserStatus")] HRESULT CurrentUserStatus([out, retval] SkypeOnlineStatus *pVal);
	[propput, id(11), helpcontext(1011), helpstring("property CurrentUserStatus")] HRESULT CurrentUserStatus([in] SkypeOnlineStatus newVal);
	[propget, id(12), helpcontext(1012), helpstring("property Mute")] HRESULT Mute([out, retval] VARIANT_BOOL *pVal);
	[propput, id(12), helpcontext(1012), helpstring("property Mute")] HRESULT Mute([in] VARIANT_BOOL newVal);
	[id(13), helpcontext(1013), helpstring("method GetMessage")] HRESULT GetMessage([in] LONG MessageID, [out, retval] IMessage ** Message);
	[id(14), helpcontext(1014), helpstring("method SendMessage")] HRESULT SendMessage([in] BSTR UserHandle, [in] BSTR Text, [out, retval] IMessage ** Message);
	[id(15), helpcontext(1015), helpstring("method Connect")] HRESULT Connect();
	[id(16), helpcontext(1016), helpstring("method GetActiveCallList")] HRESULT GetActiveCallList([out, retval] ICallCollection ** List);
	[id(17), helpcontext(1017), helpstring("method GetMissedCallList")] HRESULT GetMissedCallList([out, retval] ICallCollection **List);
	[id(18), helpcontext(1018), helpstring("method GetMessageList")] HRESULT GetMessageList([out, retval] IMessageCollection **List);
	[id(19), helpcontext(1019), helpstring("method GetMissedMessageList")] HRESULT GetMissedMessageList([out, retval] IMessageCollection **List);
	[id(20), helpcontext(1020), helpstring("method SearchForMessages")] HRESULT SearchForMessages([in] BSTR UserHandle, [out, retval] IMessageCollection **List);
	[propget, id(21), helpcontext(1021), helpstring("property Version")] HRESULT Version([out, retval] BSTR *pVal);
	[propget, id(22), helpcontext(1022), helpstring("property Privilege")] HRESULT Privilege([in] BSTR Name, [out, retval] VARIANT_BOOL *pVal);
	[id(23), helpcontext(1023), helpstring("method GetUser")] HRESULT GetUser([in] BSTR Handle, [out, retval] IUser ** User);
	[propget, id(24), helpcontext(1024), helpstring("property SkypeExecutablePath")] HRESULT SkypeExecutablePath([out, retval] BSTR *pVal);
	[propget, id(25), helpcontext(1025), helpstring("property Protocol")] HRESULT Protocol([out, retval] long *pVal);
	[propput, id(25), helpcontext(1025), helpstring("property Protocol")] HRESULT Protocol([in] long lngVal);
	[id(26), helpcontext(1026), helpstring("method GetChatList")] HRESULT GetChatList([out, retval] IChatCollection **List);
	[id(27), helpcontext(1027), helpstring("method GetChat")] HRESULT GetChat([in] BSTR Name, [out, retval] IChat **Chat);
	[id(28), helpcontext(1028), helpstring("method GetChatMessageList")] HRESULT GetChatMessageList([out, retval] IChatMessageCollection ** List);
	[id(29), helpcontext(1029), helpstring("method GetMissedChatMessageList")] HRESULT GetMissedChatMessageList([out, retval] IChatMessageCollection ** List);
	[id(30), helpcontext(1030), helpstring("method GetChatMessage")] HRESULT GetChatMessage([in] LONG ID, [out, retval] IChatMessage ** Message);
	[id(31), helpcontext(1031), helpstring("method CloseSkype")] HRESULT CloseSkype();
	[propget, id(32), helpcontext(1032), helpstring("property SkypeOutBalance")] HRESULT SkypeOutBalance([out, retval] long *pVal);
	[propget, id(33), helpcontext(1033), helpstring("property SkypeOutCurrency")] HRESULT SkypeOutCurrency([out, retval] SkypeCurrencyType *pVal);
	[propget, id(34), helpcontext(1034), helpstring("property APIAttachmentStatus")] HRESULT APIAttachmentStatus([out, retval] SkypeAPIAttachmentStatus * APIStatus);
	[id(35), helpcontext(1035), helpstring("method PlaceConferenceCall")] HRESULT PlaceConferenceCall([in, satype(BSTR)] SAFEARRAY ** UserHandles, [out, retval] ICall ** Call);
	[id(36), helpcontext(1036), helpstring("method LaunchChat")] HRESULT LaunchChat([in, satype(BSTR)] SAFEARRAY ** UserHandles, [out, retval] IChat ** Chat);
	[id(37), helpcontext(1037), helpstring("method OpenIMDialog")] HRESULT OpenIMDialog([in] BSTR UserHandle, [in] BSTR Message);
	[id(38), helpcontext(1038), helpstring("method FocusSkypeWindow")] HRESULT FocusSkypeWindow();
	[id(39), helpcontext(1039), helpstring("method LaunchAddAFriend")] HRESULT LaunchAddAFriend([in] BSTR UserHandle);
	[propget, id(40), helpcontext(1040), helpstring("property Conversion")] HRESULT Conversion([out, retval] IConversion ** pVal);
	[id(41), helpcontext(1041), helpstring("method MinmizeSkype")] HRESULT MinimizeSkype();
	[id(42), helpcontext(1042), helpstring("method SearchForChatMessages")] HRESULT SearchForChatMessages([in] BSTR UserHandle, [out, retval] IChatMessageCollection **List);
	[id(43), helpcontext(1043), helpstring("method GetActiveChatList")] HRESULT GetActiveChatList([out, retval] IChatCollection ** pVal);
	[id(44), helpcontext(1044), helpstring("method GetMissedChatList")] HRESULT GetMissedChatList([out, retval] IChatCollection ** pVal);
	[id(45), helpcontext(1045), helpstring("method GetRecentChatList")] HRESULT GetRecentChatList([out, retval] IChatCollection ** pVal);
	[id(46), helpcontext(1046), helpstring("method GetBookmarkedChatList")] HRESULT GetBookmarkedChatList([out, retval] IChatCollection ** pVal);
	[id(47), helpcontext(1047), helpstring("method GetVoiceMailList")] HRESULT GetVoiceMailList([out, retval] IVoiceMailCollection ** pVal);
	[id(48), helpcontext(1048), helpstring("method SendVoiceMail")] HRESULT SendVoiceMail([in] BSTR UserHandle, [out, retval] IVoiceMail ** pVal);
	[id(49), helpcontext(1049), helpstring("method GetUsersWaitingForAuthorization")] HRESULT GetUsersWaitingForAuthorization([out, retval] IUserCollection ** pVal);
	[id(50), helpcontext(1050), helpstring("method ClearChatHistory")] HRESULT ClearChatHistory();
	[id(51), helpcontext(1051), helpstring("method ClearVoiceMailHistory")] HRESULT ClearVoiceMailHistory();
	[id(52), helpcontext(1052), helpstring("method ClearCallHistory")] HRESULT ClearCallHistory([in] SkypeClearCallHistory CallType);
	[propget, id(53), helpcontext(1053), helpstring("property Ringer")] HRESULT Ringer([out, retval] BSTR* pVal);
	[propput, id(53), helpcontext(1053), helpstring("property Ringer")] HRESULT Ringer([in] BSTR Val);
	[id(54), helpcontext(1054), helpstring("method ConnectAndWait")] HRESULT ConnectAndWait([in] LONG TimeoutInSeconds);
	[id(55), helpcontext(1055), helpstring("method OpenFileTransferDialog")] HRESULT OpenFileTransferDialog([in, satype(BSTR)] SAFEARRAY ** UserHandles, [in] BSTR Folder);
	[id(56), helpcontext(1056), helpstring("method SendBlockingCommand")] HRESULT SendBlockingCommand([in] BSTR Command, [out, retval] BSTR * pVal);
	[propget, id(57), helpcontext(1057), helpstring("property CommandTimeout")] HRESULT CommandTimeout([out, retval] LONG* pVal);
	[propput, id(57), helpcontext(1057), helpstring("property CommandTimeout")] HRESULT CommandTimeout([in] LONG Val);
	[id(58), helpstring("method OpenApplication")] HRESULT OpenApplication([in] BSTR ApplicationName, [out,retval] IApp2App ** App);
	[propget, id(59), helpcontext(1059), helpstring("property Profile")] HRESULT Profile([out, retval] IProfile ** pVal);
	[propget, id(60), helpcontext(1060), helpstring("property UI")] HRESULT UI([out, retval] IUI ** pVal);
	[propget, id(61), helpcontext(1061), helpstring("property VideoIn")] HRESULT VideoIn([out, retval] BSTR* pVal);
	[propput, id(61), helpcontext(1061), helpstring("property VideoIn")] HRESULT VideoIn([in] BSTR Val);
};


// Access
[
	coclass,
	threading(apartment),
	vi_progid("ActiveST.Access"),
	progid("ActiveST.Access.1"),
	version(1.0),
	uuid("06406675-E635-4fe4-8B2C-82C5C0BA6F40"),
	helpstring("Access Class"),
	event_source(com),
	support_error_info("IAccess"),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE Access : 
	public _IAccessEvents,
	public IAccess,
	public IPersistStreamInitImpl<Access>,
	public IOleControlImpl<Access>,
	public IOleObjectImpl<Access>,
	public IOleInPlaceActiveObjectImpl<Access>,
	public IViewObjectExImpl<Access>,
	public IOleInPlaceObjectWindowlessImpl<Access>,
	public CComControl<Access>,
	public CSkypeMessageSink

{
public:

	Access() : CSkypeMessageSink(gSkypeQueue)
	{
		m_objConversion = NULL;
		m_objProfile = NULL;
		m_objUI = NULL;
		m_hWndEvent = NULL;
		SIZE sz = {16, 16};
		AtlPixelToHiMetric(& sz, & m_sizeExtent);
		m_sizeNatural = m_sizeExtent;
		m_bAutoSize = true;
		m_bmpIcon = LoadBitmap(_AtlBaseModule.GetModuleInstance(), 
			MAKEINTRESOURCE(IBMP_ACCESS));
	}


DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(Access)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(Access)
	CHAIN_MSG_MAP(CComControl<Access>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	__event __interface _IAccessEvents;
// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IAccess
public:
	/*
	Basically, we're overriding the attribute-inserted registry code
	because it doesn't honor the OLE MISC flags being set
	*/
    static RGSStrings* GetOpcodeStringVals() 
    {
        static RGSStrings rgStrings[] = 
		{
			{_T(""),0 }, 
            {_T("ActiveST.Access.1"),0 }, 
            {_T("%FriendlyName%"),1 }, 
            {_T("CLSID"),0 }, 
			{_T("{06406675-E635-4fe4-8B2C-82C5C0BA6F40}"),0 }, 
            {_T("ActiveST.Access"),0 }, 
            {_T("CurVer"),0 }, 
            {_T("ProgID"),0 }, 
            {_T("VersionIndependentProgID"),0 }, 
            {_T("%MODULETYPE%"),1 }, 
            {_T("%MODULE%"),1 }, 
            {_T("ThreadingModel"),0 }, 
            {_T("apartment"),0 }, 
            {_T("Control"),0 }, 
            {_T("ToolboxBitmap32"),0 }, 
            {_T("%MODULE%, 1"),1 }, 
            {_T("MiscStatus"),0 }, 
            {_T("0"),0 }, 
            {_T("1"),0 }, 
            {_T("132497"),0 }, 
            {_T("AppID"),0 }, 
            {_T("%APPID%"),1 }, 
            {_T("TypeLib"),0 }, 
            {_T("%MODULEGUID%"),1 }, 
            {_T("Version"),0 }, 
            {_T("1.0"),0 }, 
            {NULL, 0}
        };
        return rgStrings;
    }

	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		HDC hdc = di.hdcDraw;
		HDC hdcMem = CreateCompatibleDC(hdc);
		HGDIOBJ hOldBitmap = SelectObject(hdcMem, m_bmpIcon);
		BitBlt(hdc, di.prcBounds->left, di.prcBounds->top, m_sizeExtent.cx, m_sizeExtent.cy, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, hOldBitmap);
		DeleteDC(hdcMem);
		return S_OK;
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		if(m_hWndEvent == NULL) {
			RECT r;
			memset(& r, 0, sizeof(RECT));
			m_hWndEvent = m_wndEvent.Create(NULL, r, _T("Skype Event Window"), WS_POPUP);
			if(m_hWndEvent == NULL) {
				ATLTRACE(_T("Create Event Error: %d\n"), GetLastError());
				throw "Unable to create callback window";
			}

			m_wndEvent.AssignAccess(this);
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		if(m_objConversion != NULL) {
			m_objConversion->Release();
			m_objConversion = NULL;
		}
		if(m_objProfile != NULL) {
			m_objProfile->Release();
			m_objProfile = NULL;
		}
		if(m_objUI != NULL) {
			m_objUI ->Release();
			m_objUI = NULL;
		}

		if(m_hWndEvent != NULL) {
			m_wndEvent.DestroyWindow();
			m_hWndEvent = NULL;
		}
	}

	STDMETHOD(SendCommand)(/*[in]*/ BSTR Command);
	STDMETHOD(GetCall)(/*[in]*/ LONG CallID, /*[out, retval]*/ ICall ** Call);
	STDMETHOD(PlaceCall)(/*[in]*/ BSTR UserName, /*[out, retval]*/ ICall ** Message);
	STDMETHOD(SearchForUser)(/*[in]*/ BSTR UserName, /*[out, retval]*/ IUserCollection ** List);
	STDMETHOD(GetCallList)(/*[out, retval]*/ ICallCollection ** List);
	STDMETHOD(GetFriendList)(/*[out, retval]*/ IUserCollection ** List);
	STDMETHOD(CloseSkype)();
	STDMETHOD(MinimizeSkype)();
	STDMETHOD(GetChatMessage)(/*[in]*/ LONG ID, /*[out, retval]*/ IChatMessage ** Message);
	STDMETHOD(GetMissedChatMessageList)(/*[out, retval]*/ IChatMessageCollection ** List);
	STDMETHOD(GetChatMessageList)(/*[out, retval]*/ IChatMessageCollection ** List);
	STDMETHOD(GetChat)(BSTR Name, IChat **Chat);
	STDMETHOD(GetChatList)(/*[out, retval]*/ IChatCollection **List);
	STDMETHOD(get_Protocol)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Protocol)(/*[in]*/ long lngVal);
	STDMETHOD(get_SkypeExecutablePath)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(GetUser)(/*[in]*/ BSTR Handle, /*[out, retval]*/ IUser ** User);
	STDMETHOD(get_Privilege)(/*[in]*/ BSTR Name, /*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_Version)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(SearchForMessages)(/*[in]*/ BSTR UserName, /*[out, retval]*/ IMessageCollection **List);
	STDMETHOD(SearchForChatMessages)(/*[in]*/ BSTR UserName, /*[out, retval]*/IChatMessageCollection **List);
	STDMETHOD(GetMissedMessageList)(/*[out, retval]*/ IMessageCollection **List);
	STDMETHOD(GetMessageList)(/*[out, retval]*/ IMessageCollection **List);
	STDMETHOD(GetMissedCallList)(/*[out, retval]*/ ICallCollection **List);
	STDMETHOD(GetActiveCallList)(/*[out, retval]*/ ICallCollection ** List);
	STDMETHOD(Connect)();
	STDMETHOD(SendMessage)(/*[in]*/ BSTR UserName, /*[in]*/ BSTR Text, /*[out, retval]*/ IMessage ** Message);
	STDMETHOD(get_Mute)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Mute)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_CurrentUserStatus)(/*[out, retval]*/ SkypeOnlineStatus *pVal);
	STDMETHOD(put_CurrentUserStatus)(/*[in]*/ SkypeOnlineStatus newVal);
	STDMETHOD(get_CurrentUserHandle)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_AudioOut)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_AudioOut)(/*[in]*/ BSTR Val);
	STDMETHOD(get_AudioIn)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_AudioIn)(/*[in]*/ BSTR Val);
	STDMETHOD(get_ConnectionStatus)(/*[out, retval]*/ SkypeConnectionStatus *pVal);
	STDMETHOD(GetMessage)(/*[in]*/ LONG MessageID, /*[out, retval]*/ IMessage ** Message);
	STDMETHOD(get_SkypeOutBalance)(/*[out, retval]*/LONG *pVal);
	STDMETHOD(get_SkypeOutCurrency)(/*[out, retval]*/SkypeCurrencyType * pVal);
	STDMETHOD(get_APIAttachmentStatus)(/*[out, retval]*/SkypeAPIAttachmentStatus * APIStatus);
	STDMETHOD(PlaceConferenceCall)(/*[in]*/ SAFEARRAY ** UserHandles, /*[out, retval]*/ ICall ** Call);
	STDMETHOD(LaunchChat)(/*[in]*/ SAFEARRAY ** UserHandles, /*[out, retval]*/ IChat ** Chat);
	STDMETHOD(OpenIMDialog)(/*[in]*/ BSTR UserHandle, /*[in]*/ BSTR Message);
	STDMETHOD(FocusSkypeWindow)();
	STDMETHOD(LaunchAddAFriend)(/*[in]*/ BSTR UserHandle);
	STDMETHOD(get_Conversion)(/*[out, retval]*/ IConversion ** pVal);
	STDMETHOD(GetActiveChatList)(/*[out, retval]*/ IChatCollection ** List);
	STDMETHOD(GetMissedChatList)(/*[out, retval]*/ IChatCollection ** List);
	STDMETHOD(GetRecentChatList)(/*[out, retval]*/ IChatCollection ** List);
	STDMETHOD(GetBookmarkedChatList)(/*[out, retval]*/ IChatCollection ** List);
	STDMETHOD(GetVoiceMailList)(/*[out, retval]*/ IVoiceMailCollection ** List);
	STDMETHOD(SendVoiceMail)(/*[in]*/ BSTR UserHandle, /*[out, retval]*/ IVoiceMail ** pVal);
	STDMETHOD(GetUsersWaitingForAuthorization)(/*[out, retval]*/ IUserCollection ** pVal);
	STDMETHOD(ClearChatHistory)();
	STDMETHOD(ClearVoiceMailHistory)();
	STDMETHOD(ClearCallHistory)(/*[in]*/ SkypeClearCallHistory CallType);
	STDMETHOD(get_Ringer)(/*[out, retval]*/ BSTR* pVal);
	STDMETHOD(put_Ringer)(/*[in]*/ BSTR Val);
	STDMETHOD(ConnectAndWait)(/*[in]*/ LONG TimeoutInSeconds);
	STDMETHOD(OpenFileTransferDialog)(/*[in]*/ SAFEARRAY ** UserHandles, /*[in]*/ BSTR Folder);
	STDMETHOD(SendBlockingCommand)(/*[in]*/ BSTR Command, /*[out, retval]*/ BSTR * pVal);
	STDMETHOD(get_CommandTimeout)(/*[out, retval]*/ LONG* pVal);
	STDMETHOD(put_CommandTimeout)(/*[in]*/ LONG Val);
	STDMETHOD(OpenApplication)(/*[in]*/ BSTR ApplicationName, /*[out,retval]*/ IApp2App ** App);
	STDMETHOD(get_Profile)(/*[out, retval]*/ IProfile ** pVal);
	STDMETHOD(get_UI)(/*[out, retval]*/ IUI ** pVal);
	STDMETHOD(get_VideoIn)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_VideoIn)(/*[in]*/ BSTR Val);

	virtual void ProcessCommand(LPCWSTR Command);
	virtual void ProcessResponse(LPCWSTR Response, BOOL ProcessCallbacks=TRUE);
	virtual void ProcessAPIStatusChange(SkypeAPIAttachmentStatus Status);

private:
	
	CSkypeAPIEventWindow m_wndEvent;
	HWND m_hWndEvent;
	HBITMAP m_bmpIcon;
	CComObject<Conversion> * m_objConversion;
	CComObject<Profile> * m_objProfile;
	CComObject<UI> * m_objUI;

	HRESULT InError(LPCOLESTR lpszDesc, const IID &iid,  HRESULT hres);
	HRESULT InError(LPCTSTR lpszDesc, const IID &iid,  HRESULT hres);

};

}

#pragma warning(default:4199)
