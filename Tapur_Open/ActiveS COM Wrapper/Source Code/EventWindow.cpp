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
#include "stdafx.h"
#include ".\conversion.h"
#include ".\skypemessagequeue.h"
#include ".\usercollection.h"
#include ".\app2app.h"
#include ".\app2appstream.h"
#include ".\callcollection.h"
#include ".\call.h"
#include ".\messagecollection.h"
#include ".\message.h"
#include ".\user.h"
#include ".\chatmessagecollection.h"
#include ".\chatmessage.h"
#include ".\chatcollection.h"
#include ".\chat.h"
#include ".\voicemailcollection.h"
#include ".\voicemail.h"
#include ".\usercollection.h"
#include ".\eventwindow.h"
#include ".\profile.h"
#include ".\ui.h"
#include ".\access.h"

namespace SkypeAPILib 
{

LRESULT CSkypeAPIEventWindow::OnFireResult(UINT, WPARAM, LPARAM lLPWSTRResult, BOOL& bHandled) {
	try {
		m_ptrAccess->Result(SysAllocString((LPWSTR) lLPWSTRResult));
		free((WCHAR *) lLPWSTRResult);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireCallStatusChanged(UINT, WPARAM wProgress, LPARAM lCallID, BOOL& bHandled) {
	CComObject<Call> * ptrCall;
	try {
		HRESULT hr = CComObject<Call>::CreateInstance(& ptrCall);
		if(SUCCEEDED(hr)) {
			ptrCall->SetCallID((long) lCallID);
			m_ptrAccess->CallStatusChanged((ICall *) ptrCall, (SkypeCallProgress) wProgress);
		}
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireOnlineStatusChanged(UINT, WPARAM wStatus, LPARAM lLPWSTRUserID, BOOL& bHandled)
{
	CComObject<User> * ptrUser;
	try {
		HRESULT hr = CComObject<User>::CreateInstance(& ptrUser);
		if(SUCCEEDED(hr)) {
			ptrUser->SetUserName((WCHAR *) lLPWSTRUserID);
			m_ptrAccess->OnlineStatusChanged(ptrUser, (SkypeOnlineStatus) wStatus);
			free((WCHAR *) lLPWSTRUserID);
		}
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireCommand(UINT, WPARAM, LPARAM lLPWSTRCommand, BOOL& bHandled)
{
	try {
		m_ptrAccess->Command(SysAllocString((LPWSTR) lLPWSTRCommand));
		free((WCHAR *) lLPWSTRCommand);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireUserStatusChanged(UINT, WPARAM wStatus, LPARAM, BOOL& bHandled)
{
	try {
		m_ptrAccess->UserStatusChanged((SkypeOnlineStatus) wStatus);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireMuteChanged(UINT, WPARAM wOnOff, LPARAM, BOOL& bHandled)
{
	try {
		m_ptrAccess->MuteChanged((VARIANT_BOOL) wOnOff);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireConnectionStatusChanged(UINT, WPARAM wStatus, LPARAM, BOOL & bHandled)
{
	try {
		m_ptrAccess->ConnectionStatusChanged((SkypeConnectionStatus) wStatus);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireMessageReceived(UINT, WPARAM, LPARAM lMessageID, BOOL& bHandled)
{
	CComObject<Message> * ptrMessage;
	try {
		HRESULT hr = CComObject<Message>::CreateInstance(& ptrMessage);
		if(SUCCEEDED(hr)) {
			ptrMessage->SetMessageID((LONG) lMessageID);
			m_ptrAccess->MessageReceived(ptrMessage);
		}
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireMessageSent(UINT, WPARAM, LPARAM lMessageID, BOOL& bHandled)
{
	CComObject<Message> * ptrMessage;
	try {
		HRESULT hr = CComObject<Message>::CreateInstance(& ptrMessage);
		if(SUCCEEDED(hr)) {
			ptrMessage->SetMessageID((LONG) lMessageID);
			m_ptrAccess->MessageSent(ptrMessage);
		}
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireAPIStatusChanged(UINT, WPARAM wStatus, LPARAM, BOOL& bHandled)
{
	try {
		m_ptrAccess->APIStatusChanged((SkypeAPIAttachmentStatus) wStatus);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireCallHistoryChanged(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	try {
		m_ptrAccess->CallHistoryChanged();
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireIMHistoryChanged(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	try {
		m_ptrAccess->IMHistoryChanged();
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireError(UINT, WPARAM wErrorID, LPARAM lLPWSTRError, BOOL& bHandled)
{
	try {
		m_ptrAccess->Error((LONG)wErrorID, SysAllocString((WCHAR *)lLPWSTRError));
		free((char *) lLPWSTRError);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireChatMessageReceived(UINT, WPARAM, LPARAM lMessageID, BOOL& bHandled)
{
	CComObject<ChatMessage> * ptrMessage;
	try {
		HRESULT hr = CComObject<ChatMessage>::CreateInstance(& ptrMessage);
		if(SUCCEEDED(hr)) {
			ptrMessage->SetID((LONG) lMessageID);
			m_ptrAccess->ChatMessageReceived(ptrMessage);
		}
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireChatMessageSent(UINT, WPARAM, LPARAM lMessageID, BOOL& bHandled)
{
	CComObject<ChatMessage> * ptrMessage;
	try {
		HRESULT hr = CComObject<ChatMessage>::CreateInstance(& ptrMessage);
		if(SUCCEEDED(hr)) {
			ptrMessage->SetID((LONG) lMessageID);
			m_ptrAccess->ChatMessageSent(ptrMessage);
		}
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireIncomingDTMF(UINT, WPARAM wLPWSTRDTMF, LPARAM lCallID, BOOL& bHandled) {
	CComObject<Call> * ptrCall;
	try {
		HRESULT hr = CComObject<Call>::CreateInstance(& ptrCall);
		if(SUCCEEDED(hr)) {
			ptrCall->SetCallID((LONG) lCallID);
			m_ptrAccess->IncomingDTMF(ptrCall, SysAllocString((LPWSTR) wLPWSTRDTMF));
			free((WCHAR *) wLPWSTRDTMF);
		}
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireAppConnecting(UINT, WPARAM wLPWSTRAppName, LPARAM lLPWSTRUserList, BOOL& bHandled) {
	CComObject<App2App> * ptrApp = NULL;
	CComObject<UserCollection> * ptrUsers = NULL;
	try {
		HRESULT hr = CComObject<App2App>::CreateInstance(& ptrApp);
		if(SUCCEEDED(hr)) {
			hr = CComObject<UserCollection>::CreateInstance(& ptrUsers);
		}
		if(SUCCEEDED(hr)) {
			ptrApp->SetAppName((LPWSTR) wLPWSTRAppName);
			ptrUsers->SetItemList((LPWSTR) lLPWSTRUserList);
			m_ptrAccess->AppConnecting(ptrApp, ptrUsers);
		}
		free((WCHAR *) wLPWSTRAppName);
		free((WCHAR *) lLPWSTRUserList);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireAppStreamOpened(UINT, WPARAM wLPWSTRAppName, LPARAM lLPWSTRStreamID, BOOL& bHandled)
{
	CComObject<App2App> * ptrApp = NULL;
	CComObject<App2AppStream> * ptrStream = NULL;
	try {
		HRESULT hr = CComObject<App2App>::CreateInstance(& ptrApp);
		if(SUCCEEDED(hr)) {
			hr = CComObject<App2AppStream>::CreateInstance(& ptrStream);
		}
		if(SUCCEEDED(hr)) {
			ptrApp->SetAppName((LPWSTR) wLPWSTRAppName);
			ptrStream->Initialize((LPWSTR) wLPWSTRAppName, (LPWSTR) lLPWSTRStreamID);
			m_ptrAccess->AppStreamOpened(ptrApp, (IApp2AppStream *) ptrStream);
		}
		free((WCHAR *) wLPWSTRAppName);
		free((WCHAR *) lLPWSTRStreamID);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireAppStreamClosed(UINT, WPARAM wLPWSTRAppName, LPARAM lLPWSTRStreamID, BOOL& bHandled)
{
	CComObject<App2App> * ptrApp = NULL;
	CComObject<App2AppStream> * ptrStream = NULL;
	try {
		HRESULT hr = CComObject<App2App>::CreateInstance(& ptrApp);
		if(SUCCEEDED(hr)) {
			hr = CComObject<App2AppStream>::CreateInstance(& ptrStream);
		}
		if(SUCCEEDED(hr)) {
			ptrApp->SetAppName((LPWSTR) wLPWSTRAppName);
			ptrStream->Initialize((LPWSTR) wLPWSTRAppName, (LPWSTR) lLPWSTRStreamID);
			m_ptrAccess->AppStreamClosed(ptrApp, (IApp2AppStream *) ptrStream);
		}
		free((WCHAR *) wLPWSTRAppName);
		free((WCHAR *) lLPWSTRStreamID);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireAppDataReceived(UINT, WPARAM wLPWSTRInfo, LPARAM lLPWSTRData, BOOL& bHandled)
{
	CComObject<App2AppStream> * ptrStream;
	try {
		HRESULT hr = CComObject<App2AppStream>::CreateInstance(& ptrStream);
		if(SUCCEEDED(hr)) {
			WCHAR * ptrTab = wcsstr((LPWSTR) wLPWSTRInfo, L"\t");
			if(ptrTab) {
				*ptrTab = L'\0';
				ptrStream->Initialize((LPWSTR) wLPWSTRInfo, ptrTab + 1);
				m_ptrAccess->AppDataReceived((IApp2AppStream *) ptrStream, 
					SysAllocString((LPWSTR) lLPWSTRData));
			}
		}
		free((WCHAR *) wLPWSTRInfo);
		free((WCHAR *) lLPWSTRData);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireAppDatagramReceived(UINT, WPARAM wLPWSTRInfo, LPARAM lLPWSTRData, BOOL& bHandled)
{
	CComObject<App2AppStream> * ptrStream;
	try {
		HRESULT hr = CComObject<App2AppStream>::CreateInstance(& ptrStream);
		if(SUCCEEDED(hr)) {
			WCHAR * ptrTab = wcsstr((LPWSTR) wLPWSTRInfo, L"\t");
			if(ptrTab) {
				*ptrTab = L'\0';
				ptrStream->Initialize((LPWSTR) wLPWSTRInfo, ptrTab + 1);
				m_ptrAccess->AppDatagramReceived((IApp2AppStream *) ptrStream, 
					SysAllocString((LPWSTR) lLPWSTRData));
			}
		}
		free((WCHAR *) wLPWSTRInfo);
		free((WCHAR *) lLPWSTRData);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireContactFocused(UINT nMsg, WPARAM wLPWSTRUserID, LPARAM lUnused, BOOL& bHandled)
{
	CComObject<User> * ptrUser;
	try {
		HRESULT hr = CComObject<User>::CreateInstance(& ptrUser);
		if(SUCCEEDED(hr)) {
			ptrUser->SetUserName((WCHAR *) wLPWSTRUserID);
			m_ptrAccess->ContactFocused((IUser *) ptrUser);
		}
		free((WCHAR *) wLPWSTRUserID);
	} catch (...) {
	}
	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireBinaryTransferStart(UINT nMsg, WPARAM wLPWSTRTransferID, LPARAM lSize, BOOL& bHandled)
{
	try {
		m_ptrAccess->BinaryTransferStart(SysAllocString((LPWSTR) wLPWSTRTransferID), (LONG) lSize);
		free((LPWSTR) wLPWSTRTransferID);
	} catch(...) {
	}

	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireBinaryTransferData(UINT nMsg, WPARAM wLPWSTRTransferID, LPARAM lProgress, BOOL& bHandled)
{
	try {
		BinaryTransferStatus * ptrStatus = (BinaryTransferStatus *) lProgress;
		m_ptrAccess->BinaryTransferData(SysAllocString((LPWSTR) wLPWSTRTransferID), ptrStatus->lBytesTransferred, ptrStatus->lBytesTotal);
		free((LPWSTR) wLPWSTRTransferID);
	} catch(...) {
	}

	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireBinaryTransferEnd(UINT nMsg, WPARAM wLPWSTRTransferID, LPARAM lPtrSafeArray, BOOL& bHandled)
{
	try {
		SAFEARRAY * psa = (SAFEARRAY *) lPtrSafeArray;
		m_ptrAccess->BinaryTransferEnd(SysAllocString((LPWSTR) wLPWSTRTransferID), & psa);
		free((LPWSTR) wLPWSTRTransferID);
		SafeArrayDestroy(psa);
	} catch(...) {
	}

	bHandled = true;
	return 0;
}

LRESULT CSkypeAPIEventWindow::OnFireBinaryTransferError(UINT nMsg, WPARAM wLPWSTRTransferID, LPARAM lLPWSTRDescription, BOOL& bHandled)
{
	try {
		m_ptrAccess->BinaryTransferError(SysAllocString((LPWSTR) wLPWSTRTransferID),
			SysAllocString((LPWSTR) lLPWSTRDescription));
		free((LPWSTR) wLPWSTRTransferID);
		free((LPWSTR) lLPWSTRDescription);
	} catch(...) {
	}

	bHandled = true;
	return 0;
}

}
