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
// SkypeMessageQueue.h: interface for the CSkypeMessageQueue class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include <queue>
#include <vector>
#include <string>

#ifndef _NO_USE_LIST
#include <map>
typedef std::map<long, SkypeCallProgress> CallList;
typedef std::map<std::wstring, SkypeOnlineStatus> OnlineUserList;
#endif

#define TIMER_SKYPE_DISCOVERY 112

//#define CURRENT_SKYPE_PROTOCOL 3

#include <queue>
typedef std::queue<std::wstring> CommandQueue;
typedef std::vector<std::wstring> StreamList;

class CSkypeMessageQueue;

struct BinaryTransferStatus {
	long lBytesTransferred;
	long lBytesTotal;
};

class BinaryTransfer {

private:

	wstring m_strID;
	wstring m_strStreamID;
	string m_strMD5;
	unsigned long m_ulSize, m_ulPos;
	BYTE * m_ptrBuffer;

public:
	BinaryTransfer(LPCWSTR ID, LPCWSTR StreamID,  LPCSTR MD5, ULONG Size);
	~BinaryTransfer();
	void AppendData(BYTE * Data, ULONG Size);
	bool MD5Match();

	LPCWSTR ID() {
		return m_strID.c_str();
	}

	LPCWSTR StreamID() {
		return m_strStreamID.c_str();
	}

	LPCSTR MD5() {
		return m_strMD5.c_str();
	}

	ULONG Length() {
		return m_ulPos;
	}

	ULONG Size() {
		return m_ulSize;
	}

	LPBYTE Buffer() {
		return m_ptrBuffer;
	}
};

typedef BinaryTransfer * BinaryTransferPTR;
typedef list<BinaryTransferPTR> BinaryTransferPTRList;

class CSkypeMessageSink
{
public:

	CSkypeMessageSink(CSkypeMessageQueue & Queue);
	virtual ~CSkypeMessageSink();
	virtual void ProcessCommand(LPCWSTR Command) = 0;
	virtual void ProcessResponse(LPCWSTR Response, BOOL ProcessCallbacks) = 0;
	virtual void ProcessAPIStatusChange(SkypeAPIAttachmentStatus Status) = 0;

private:
	CSkypeMessageQueue * m_ptrQueue;
};

typedef CSkypeMessageSink * CSkypeMessageSinkPtr;
typedef std::list<CSkypeMessageSinkPtr> CSkypeMessageSinkList;
typedef std::list<CSkypeMessageSinkPtr> CSkypeMessageSinkList;
typedef std::queue<SkypeAPIAttachmentStatus> SkypeAPIAttachmentStatusQueue;

class CSkypeMessageWindow : public CWindowImpl<CSkypeMessageWindow>
{
public:
	HWND m_hWndSkype;
	static UINT WM_SKYPE_API_ATTACH;
	static UINT WM_SKYPE_API_DISCOVER;

	DECLARE_WND_CLASS(_T("SkypeCallbackWindow"));

	BEGIN_MSG_MAP(CSkypeMessageWindow)
#ifdef USE_OLD_API
		MESSAGE_HANDLER(WM_SKYPE, OnSkype)
#endif
		MESSAGE_HANDLER(WM_SKYPE_API_ATTACH, OnSkypeAPIAttach)
		MESSAGE_HANDLER(WM_COPYDATA, OnCopyData)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)

	END_MSG_MAP()

	CSkypeMessageWindow() {
		m_hWndSkype = NULL;
		m_ptrQueue = NULL;
		m_bBusy = false;
		m_blConnect = false;		// スカイプ接続待ちタイマフラグ
	}
	
	void AssignQueue(CSkypeMessageQueue * Queue) {
		m_ptrQueue = Queue;
	}

	SkypeAPIAttachmentStatus GetAPIStatus() {
		return m_apiStatus;
	}

	bool StartConnectTimer(long lngTime);	// スカイプ接続待ちタイマ開始
	bool StopConnectTimer();				// スカイプ接続待ちタイマ終了

protected:
	bool m_bBusy;
	long m_lStatus;
	bool m_blConnect;						// スカイプ接続待ちタイマフラグ
	SkypeAPIAttachmentStatus m_apiStatus;
	CSkypeMessageQueue * m_ptrQueue;
	LRESULT OnSkypeAPIAttach(UINT, WPARAM, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCopyData(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT, WPARAM wParam, LPARAM, BOOL& bHandled);
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL& bHandled);
	void LaunchSkype();
};


/*
*/
class CSkypeMessageQueue  
{
public:
	HANDLE m_hThread;
	DWORD  m_dwThreadID;
	BOOL m_bForceLaunch;

	CSkypeMessageQueue();
	virtual ~CSkypeMessageQueue();

	void Initialize();
	void Shutdown();
	void CreateMessageWindow();
	void DeleteMessageWindow();
	void RestartTimer();

	void CloseSkype();
	void MinimizeSkype();

	HWND GetMessageWindow() { return m_wndMsg.m_hWnd; }

	void SendCommand(LPCWSTR Command);
	
	LPWSTR ExecuteBlockingCommand(LPCWSTR Command, 
		LPCWSTR WaitForResult, BOOL DoNotUseCmdID = FALSE);

	LPWSTR RetrieveProperty(LPCWSTR ObjectType, LONG ObjectID, 
		LPCWSTR PropertyName);
	LPWSTR RetrieveProperty(LPCWSTR ObjectType, LPCWSTR ObjectID, 
		LPCWSTR PropertyName);

	void SetProperty(LPCWSTR ObjectType, LONG ObjectID, 
		LPCWSTR PropertyName, LPCWSTR PropertyValue, 
		LPCWSTR OverrideResponses=NULL);
	void SetProperty(LPCWSTR ObjectType, LPCWSTR ObjectID, 
		LPCWSTR PropertyName, LPCWSTR PropertyValue, 
		LPCWSTR OverrideResponses=NULL);
	void SetPropertyNoWait(LPCWSTR ObjectType, LONG ObjectID, 
		LPCWSTR PropertyName, LPCWSTR PropertyValue);

	void AlterMethodNoWait(LPCWSTR ObjectType, LONG ObjectID,
		LPCWSTR MethodName, LPCWSTR MethodValue);

	void RegisterMessageSink(CSkypeMessageSink * Sink);
	void UnregisterMessageSink(CSkypeMessageSink * Sink);

	void ProcessAPIStatus(SkypeAPIAttachmentStatus status, HWND hWndSkype);
	void ProcessResponse(LPCWSTR Response);
	void ProcessQueue();
	long SupportedProtocol();

	long GetTimeout() { return m_lTimeout; }
	void SetTimeout(long Timeout) { m_lTimeout = Timeout; }

	StreamList & GetStreamList() {
		return m_listStreams;
	}

	void SetStreamList(StreamList & List) {
		m_listStreams = List;
	}

	SkypeAPIAttachmentStatus GetAPIStatus() {
		if(m_wndMsg.IsWindow()) {
			return m_wndMsg.GetAPIStatus();
		} else {
			return apiAttachUnknown;
		}
	}

	void StartBinaryTransfer(LPCWSTR ID, LPCWSTR StreamID, LPCSTR MD5Checksum, ULONG TotalSize);
	BinaryTransferStatus * AppendBinaryTransfer(LPCWSTR ID, BYTE * Buffer, ULONG Size);
	void EndBinaryTransfer(LPCWSTR ID, BOOL Cancelled);
	BinaryTransfer * GetBinaryTransfer(LPCWSTR ID);
	void DeleteAllBinaryTransfers();
	void DeleteStreamBinaryTransfers(LPCWSTR StreamID);

#ifndef _NO_USE_LIST
	const CallList & GetCallList();
	void RebuildCallList();
	SkypeCallProgress GetCallProgress(long CallID);	
	void UpdateCallProgress(long CallID, SkypeCallProgress Progress);

	const OnlineUserList & GetUserList();
	void RebuildUserList();
	SkypeOnlineStatus GetUserOnlineStatus(LPCWSTR UserID);	
	void UpdateUserOnlineStatus(LPCWSTR UserID, SkypeOnlineStatus Progress);
protected:
	void InternalRebuildCallList();
	void InternalRebuildUserList();
#endif

protected:

#ifndef _NO_USE_LIST
	static CallList m_listCalls;
	bool m_bNeedRebuildCallList;
	static OnlineUserList m_listUsers;
	bool m_bNeedRebuildUserList;
#endif

	bool m_bBlocking, m_bBlockingError, m_blockingCommandID;
	WCHAR m_strCommandID[32];
	std::wstring m_strDesiredResult;
	std::wstring m_strCommandResult;
	CSkypeMessageWindow m_wndMsg;
	CommandQueue m_queueIncoming;
	CommandQueue m_queueOutgoing;
	SkypeAPIAttachmentStatusQueue m_queueStatus;
	long m_lSupportedProtocol;
	long m_lTimeout;
	StreamList m_listStreams;
	BinaryTransferPTRList m_listTransfers;

	HWND m_hWndSkype;

	bool InternalProcessCommand(LPCWSTR Command);
	void WaitForBlockingToComplete();

private:	
	CSkypeMessageSinkList m_MessageSinks;
	void CheckSkypeAPI();
};

extern CSkypeMessageQueue gSkypeQueue;

#define CHECK_PROTOCOL(x) \
	if(gSkypeQueue.SupportedProtocol() < x) { \
		return AtlReportError(GetObjectCLSID(), L"This command not supported by your version of Skype", \
			GUID_NULL, E_FAIL); \
	}

