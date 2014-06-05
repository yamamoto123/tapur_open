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
#pragma once

#define WM_FIRE_RESULT WM_USER + 10
#define WM_FIRE_CALLSTATUSCHANGED WM_USER + 11
#define WM_FIRE_ONLINESTATUSCHANGED WM_USER + 12
#define WM_FIRE_COMMAND WM_USER + 13
#define WM_FIRE_USERSTATUSCHANGED WM_USER + 14
#define WM_FIRE_MUTECHANGED WM_USER + 15
#define WM_FIRE_CONNECTIONSTATUSCHANGED WM_USER + 16
#define WM_FIRE_MESSAGERECEIVED WM_USER + 17
#define WM_FIRE_MESSAGESENT WM_USER + 18
#define WM_FIRE_APISTATUSCHANGED WM_USER + 19
#define WM_FIRE_CALLHISTORYCHANGED WM_USER + 20
#define WM_FIRE_IMHISTORYCHANGED WM_USER + 21
#define WM_FIRE_ERROR WM_USER + 22
#define WM_FIRE_CHATMESSAGERECEIVED WM_USER + 23
#define WM_FIRE_CHATMESSAGESENT WM_USER + 24
#define WM_FIRE_INCOMINGDTMF WM_USER + 25
#define WM_FIRE_APP_CONNECTING WM_USER + 26
#define WM_FIRE_APP_STREAM_OPENED WM_USER + 27
#define WM_FIRE_APP_DATA_RECEIVED WM_USER + 28
#define WM_FIRE_APP_DATAGRAM_RECEIVED WM_USER + 29
#define WM_FIRE_APP_STREAM_CLOSED WM_USER + 30
#define WM_FIRE_CONTACT_FOCUSED WM_USER + 31
#define WM_FIRE_BINARY_TRANSFER_START WM_USER + 32
#define WM_FIRE_BINARY_TRANSFER_DATA WM_USER + 33
#define WM_FIRE_BINARY_TRANSFER_END WM_USER + 34
#define WM_FIRE_BINARY_TRANSFER_ERROR WM_USER + 35

namespace SkypeAPILib 
{

class Access;

class CSkypeAPIEventWindow : public CWindowImpl<CSkypeAPIEventWindow>
{
protected:
	Access * m_ptrAccess;

public:
	DECLARE_WND_CLASS(_T("SkypeAPIEventWindow"));

	BEGIN_MSG_MAP(CSkypeAPIEventWindow)
		MESSAGE_HANDLER(WM_FIRE_RESULT, OnFireResult)
		MESSAGE_HANDLER(WM_FIRE_CALLSTATUSCHANGED, OnFireCallStatusChanged)
		MESSAGE_HANDLER(WM_FIRE_ONLINESTATUSCHANGED, OnFireOnlineStatusChanged)
		MESSAGE_HANDLER(WM_FIRE_COMMAND, OnFireCommand)
		MESSAGE_HANDLER(WM_FIRE_USERSTATUSCHANGED, OnFireUserStatusChanged)
		MESSAGE_HANDLER(WM_FIRE_MUTECHANGED, OnFireMuteChanged)
		MESSAGE_HANDLER(WM_FIRE_CONNECTIONSTATUSCHANGED, OnFireConnectionStatusChanged)
		MESSAGE_HANDLER(WM_FIRE_MESSAGERECEIVED, OnFireMessageReceived)
		MESSAGE_HANDLER(WM_FIRE_MESSAGESENT, OnFireMessageSent)
		MESSAGE_HANDLER(WM_FIRE_APISTATUSCHANGED, OnFireAPIStatusChanged)
		MESSAGE_HANDLER(WM_FIRE_CALLHISTORYCHANGED, OnFireCallHistoryChanged)
		MESSAGE_HANDLER(WM_FIRE_IMHISTORYCHANGED, OnFireIMHistoryChanged)
		MESSAGE_HANDLER(WM_FIRE_ERROR, OnFireError)
		MESSAGE_HANDLER(WM_FIRE_CHATMESSAGERECEIVED, OnFireChatMessageReceived)
		MESSAGE_HANDLER(WM_FIRE_CHATMESSAGESENT, OnFireChatMessageSent)
		MESSAGE_HANDLER(WM_FIRE_CHATMESSAGESENT, OnFireChatMessageSent)
		MESSAGE_HANDLER(WM_FIRE_INCOMINGDTMF, OnFireIncomingDTMF)
		MESSAGE_HANDLER(WM_FIRE_APP_CONNECTING, OnFireAppConnecting)
		MESSAGE_HANDLER(WM_FIRE_APP_STREAM_OPENED, OnFireAppStreamOpened)
		MESSAGE_HANDLER(WM_FIRE_APP_DATA_RECEIVED, OnFireAppDataReceived)
		MESSAGE_HANDLER(WM_FIRE_APP_DATAGRAM_RECEIVED, OnFireAppDatagramReceived)
		MESSAGE_HANDLER(WM_FIRE_APP_STREAM_CLOSED, OnFireAppStreamClosed)
		MESSAGE_HANDLER(WM_FIRE_CONTACT_FOCUSED, OnFireContactFocused)
		MESSAGE_HANDLER(WM_FIRE_BINARY_TRANSFER_START, OnFireBinaryTransferStart)
		MESSAGE_HANDLER(WM_FIRE_BINARY_TRANSFER_DATA, OnFireBinaryTransferData)
		MESSAGE_HANDLER(WM_FIRE_BINARY_TRANSFER_END, OnFireBinaryTransferEnd)
		MESSAGE_HANDLER(WM_FIRE_BINARY_TRANSFER_ERROR, OnFireBinaryTransferError)
	END_MSG_MAP()

	CSkypeAPIEventWindow() {
	}

	void AssignAccess(Access * Access) {
		m_ptrAccess = Access;
	}

protected:

	LRESULT OnFireResult(UINT nMsg, WPARAM wParam, LPARAM lLPWSTRResult, BOOL& bHandled);
	LRESULT OnFireCallStatusChanged(UINT nMsg, WPARAM wProgress, LPARAM lCallID, BOOL& bHandled);
	LRESULT OnFireOnlineStatusChanged(UINT nMsg, WPARAM wStatus, LPARAM lLPWSTRUserID, BOOL& bHandled);
	LRESULT OnFireCommand(UINT nMsg, WPARAM wParam, LPARAM lLPWSTRCommand, BOOL& bHandled);
	LRESULT OnFireUserStatusChanged(UINT nMsg, WPARAM wStatus, LPARAM lParam, BOOL& bHandled);
	LRESULT OnFireMuteChanged(UINT nMsg, WPARAM wOnOff, LPARAM lParam, BOOL& bHandled);
	LRESULT OnFireConnectionStatusChanged(UINT nMsg, WPARAM wStatus, LPARAM lParam, BOOL & bHandled);
	LRESULT OnFireMessageReceived(UINT nMsg, WPARAM wProgress, LPARAM lMessageID, BOOL& bHandled);
	LRESULT OnFireMessageSent(UINT nMsg, WPARAM wProgress, LPARAM lMessageID, BOOL& bHandled);
	LRESULT OnFireAPIStatusChanged(UINT nMsg, WPARAM wStatus, LPARAM lParam, BOOL& bHandled);
	LRESULT OnFireCallHistoryChanged(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnFireIMHistoryChanged(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnFireError(UINT nMsg, WPARAM wErrorID, LPARAM lLPWSTRError, BOOL& bHandled);
	LRESULT OnFireChatMessageReceived(UINT nMsg, WPARAM wProgress, LPARAM lMessageID, BOOL& bHandled);
	LRESULT OnFireChatMessageSent(UINT nMsg, WPARAM wProgress, LPARAM lMessageID, BOOL& bHandled);
	LRESULT OnFireIncomingDTMF(UINT nMsg, WPARAM wLPWSTRDTMF, LPARAM lCallID, BOOL& bHandled);
	LRESULT OnFireAppConnecting(UINT nMsg, WPARAM wLPWSTRAppName, LPARAM lLPWSTRUserList, BOOL& bHandled);
	LRESULT OnFireAppStreamOpened(UINT nMsg, WPARAM wLPWSTRAppName, LPARAM lLPWSTRStreamID, BOOL& bHandled);
	LRESULT OnFireAppDataReceived(UINT nMsg, WPARAM wLPWSTRInfo, LPARAM lLPWSTRData, BOOL& bHandled);
	LRESULT OnFireAppDatagramReceived(UINT nMsg, WPARAM wLPWSTRInfo, LPARAM lLPWSTRData, BOOL& bHandled);
	LRESULT OnFireAppStreamClosed(UINT nMsg, WPARAM wLPWSTRAppName, LPARAM lLPWSTRStreamID, BOOL& bHandled);
	LRESULT OnFireContactFocused(UINT nMsg, WPARAM wLPWSTRUserID, LPARAM lUnused, BOOL& bHandled);
	LRESULT OnFireBinaryTransferStart(UINT nMsg, WPARAM wLPWSTRTransferID, LPARAM lSize, BOOL& bHandled);
	LRESULT OnFireBinaryTransferData(UINT nMsg, WPARAM wLPWSTRTransferID, LPARAM lProgress, BOOL& bHandled);
	LRESULT OnFireBinaryTransferEnd(UINT nMsg, WPARAM wLPWSTRTransferID, LPARAM lPtrSafeArray, BOOL& bHandled);
	LRESULT OnFireBinaryTransferError(UINT nMsg, WPARAM wLPWSTRTransferID, LPARAM lLPWSTRDescription, BOOL& bHandled);
};

}