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
// SkypeMessageQueue.cpp: implementation of the CSkypeMessageQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include ".\SkypeMessageQueue.h"
#include ".\conversion.h"
#include ".\md5.h"
#include <comdef.h>

#define SUPPORTED_PROTOCOL 6	//PAAK
//#define SUPPORTED_PROTOCOL 5

CSkypeMessageQueue gSkypeQueue;

// Get the Skype windows message ID's
UINT CSkypeMessageWindow::WM_SKYPE_API_DISCOVER  = RegisterWindowMessage(_T("SkypeControlAPIDiscover"));
UINT CSkypeMessageWindow::WM_SKYPE_API_ATTACH = RegisterWindowMessage(_T("SkypeControlAPIAttach"));

#ifndef _NO_USE_LIST
CallList CSkypeMessageQueue::m_listCalls;
OnlineUserList CSkypeMessageQueue::m_listUsers;
#endif

// Receives a handle to the Skype application window 
LRESULT CSkypeMessageWindow::OnSkypeAPIAttach(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	StopConnectTimer();
	// KillTimer(TIMER_SKYPE_DISCOVERY);
	gSkypeQueue.m_bForceLaunch = FALSE;

	m_apiStatus = (SkypeAPIAttachmentStatus) lParam;
	switch(m_apiStatus) {
	case apiAttachSuccess:
		m_hWndSkype = (HWND) wParam;
		break;
	case apiAttachPendingAuth:
	case apiAttachRefused:
	case apiAttachNotAvailable:
		m_hWndSkype = NULL;
		break;
	// This message comes across when Skype is starting, so we need to trigger 
	// another search for the Skype window
	case apiAttachAPIAvail:
		m_hWndSkype = NULL;
//paak		SetTimer(TIMER_SKYPE_DISCOVERY, 5000);
//		::PostMessage(HWND_BROADCAST, WM_SKYPE_API_DISCOVER, (WPARAM) m_hWnd, 0);
		// 接続完了しない場合、リトライし続ける
		if (m_hWndSkype == NULL) {
			StartConnectTimer(5000);
		}
		break;
	default:
		m_apiStatus = apiAttachUnknown;
		m_hWndSkype = NULL;
		break;
	}

	if(m_ptrQueue) {
		m_ptrQueue->ProcessAPIStatus(m_apiStatus, m_hWndSkype);
	}

	bHandled = true;
	return 1;
}

// Skype sends its data via WM_COPYDATA
LRESULT CSkypeMessageWindow::OnCopyData(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
	// Return the string received for processing
	if(m_ptrQueue) {
		COPYDATASTRUCT * lpCS = (COPYDATASTRUCT *) lParam;
		if((lpCS->cbData > 0) && (m_ptrQueue != NULL)) {
 			int result = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) lpCS->lpData, 
				lpCS->cbData, NULL, 0);
			if (result != 0) {
				WCHAR * pMsg = new WCHAR[result];
				result = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) lpCS->lpData, 
					lpCS->cbData, pMsg, result);
				if(result != 0) {
					m_ptrQueue->ProcessResponse(pMsg);
				}
				delete [] pMsg;
			}
		}
	}
	bHandled = true;
	return 1;
}

LRESULT CSkypeMessageWindow::OnTimer(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	if (wParam == TIMER_SKYPE_DISCOVERY) {
		if(gSkypeQueue.m_bForceLaunch) {
			StopConnectTimer();
			//KillTimer(TIMER_SKYPE_DISCOVERY);
			LaunchSkype();
		} else {
			::PostMessage(HWND_BROADCAST, CSkypeMessageWindow::WM_SKYPE_API_DISCOVER, (WPARAM) m_hWnd, 0);
		}
	}
	bHandled = true;
	return 0;
}

//
//  機能     : スカイプ接続待ちタイマ開始
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
bool CSkypeMessageWindow::StartConnectTimer(long lngTime)
{
	if (m_blConnect)  return false;

	SetTimer(TIMER_SKYPE_DISCOVERY, lngTime);	
	m_blConnect = true;		// タイマー実行中フラグ

	return true;
}

//
//  機能     : スカイプ接続待ちタイマ終了
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
//  
bool CSkypeMessageWindow::StopConnectTimer()
{
	if (!m_blConnect)  return false;
			
	try {
		if (!::IsWindow(m_hWnd)) return false;
		KillTimer(TIMER_SKYPE_DISCOVERY);
		m_blConnect = false;		// タイマー実行中フラグ
	} catch(...) {
		// Wndの生成が完了する前に呼ばれる場合があるため
		return false;
	}

	return true;
}

LRESULT CSkypeMessageWindow::OnClose(UINT, WPARAM, LPARAM, BOOL &bHandled)
{
	AssignQueue(NULL);
	bHandled = false;
	return 0;
}

void CSkypeMessageWindow::LaunchSkype()
{
	HKEY hKey;
	TCHAR strSkypePath[_MAX_PATH];
	DWORD dwType, dwSize = _MAX_PATH;
	STARTUPINFO startup;
	PROCESS_INFORMATION process;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Skype\\Phone"), 0, KEY_READ, & hKey) != ERROR_SUCCESS) {
		return;
	}

	if((RegQueryValueEx(hKey, _T("SkypePath"), 0, & dwType, (LPBYTE) strSkypePath, & dwSize) == ERROR_SUCCESS) && (dwType == REG_SZ)) {
		memset(& startup, 0, sizeof(STARTUPINFO));
		_tcscat_s(strSkypePath, _MAX_PATH, _T(" /nosplash /minimized"));
		CreateProcess(NULL, strSkypePath, NULL, NULL, TRUE, 0, NULL, NULL, & startup, & process);
		Sleep(5000);
	}

	RegCloseKey(hKey);

	// Trigger a search for the Skype window
	m_apiStatus = apiAttachUnknown;
	::PostMessage(HWND_BROADCAST, WM_SKYPE_API_DISCOVER, (WPARAM) m_hWnd, 0);

	// Allow 5 seconds for a response of any type
	StartConnectTimer(5000);
	//SetTimer(TIMER_SKYPE_DISCOVERY, 5000);
}

DWORD WINAPI SkypeProcess (LPVOID lpSkypeMessageQueue) 
{
//	CoInitialize(NULL);
	
	CSkypeMessageQueue * pQueue = (CSkypeMessageQueue *) lpSkypeMessageQueue;
	pQueue->CreateMessageWindow();
	MSG msg;
	int iCtr = 0;

	for(;;) {
		if(iCtr++ < 5) {
			if(PeekMessage(& msg, 0, 0, 0, PM_REMOVE)) {
				AtlTrace("Message: %d, WPARAM: %d, LPARAM: %d\n", msg.message, msg.wParam, msg.lParam);
				if(msg.message == WM_QUIT) {
					PostQuitMessage(0);	//11.02.26 paak
					break;
				}
				TranslateMessage(& msg);
				DispatchMessage(& msg);
			}
		} else {
			iCtr = 0;
		}
		pQueue->ProcessQueue();
		Sleep(1);
	};

	pQueue->DeleteMessageWindow();
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkypeMessageSink::CSkypeMessageSink(CSkypeMessageQueue & Queue)
{
	Queue.RegisterMessageSink(this);
	m_ptrQueue = & Queue;
}

CSkypeMessageSink::~CSkypeMessageSink()
{
	m_ptrQueue->UnregisterMessageSink(this);
}

CSkypeMessageQueue::CSkypeMessageQueue()
{
	m_bBlocking = false;
	m_bBlockingError = false;
	m_blockingCommandID = false;
	m_lTimeout = 25000;
	m_strCommandID[0] = 0;
	m_wndMsg.m_hWnd = NULL;
	m_hWndSkype = NULL;
#ifndef _NO_USE_LIST
	m_bNeedRebuildCallList = true;
	m_bNeedRebuildUserList = true;
#endif
	m_hThread = NULL;
	m_dwThreadID = NULL;
	m_lSupportedProtocol = -1;
}

CSkypeMessageQueue::~CSkypeMessageQueue()
{
	Shutdown();
}

/*
	Initialize the Skype API hooks
*/

void CSkypeMessageQueue::Initialize() 
{
	Shutdown();

	try {
		m_bForceLaunch = TRUE;
		if(m_hThread == NULL) {
			m_hThread = CreateThread(NULL, 0, SkypeProcess, this, 0, & m_dwThreadID);
			SetThreadPriority(m_hThread, THREAD_PRIORITY_LOWEST);
		}
	} catch(...) {
		return;
	}
}

void CSkypeMessageQueue::CreateMessageWindow()
{
	// Create our message window 
	if(! m_wndMsg.IsWindow()) {
		RECT r;
		memset(& r, 0, sizeof(RECT));
		m_wndMsg.Create(NULL, r, _T("Skype Callback Window"), WS_POPUP);
		if(m_wndMsg.m_hWnd == NULL) {
			ATLTRACE("Create Window Error: %d\n", GetLastError());
			throw L"Unable to create callback window";
		}

		// Allow 1/10th second for a response of any type
		m_wndMsg.StartConnectTimer(100);
//		m_wndMsg.SetTimer(TIMER_SKYPE_DISCOVERY, 100);

		// Assign the queue to the message window 
		m_wndMsg.AssignQueue(this);

		// Trigger a search for the Skype window
		::PostMessage(HWND_BROADCAST, CSkypeMessageWindow::WM_SKYPE_API_DISCOVER, (WPARAM) m_wndMsg.m_hWnd, 0);
	}
}

void CSkypeMessageQueue::DeleteMessageWindow()
{
	if(m_wndMsg.IsWindow()) {
		m_wndMsg.DestroyWindow();
		m_wndMsg.m_hWnd = NULL;
	}
}

/*
	Clean up message pump window 
*/
void CSkypeMessageQueue::Shutdown() 
{
	try {
		// Signal thread to shut down and wait for clean up
		if(m_hThread != NULL) {
			m_hThread = NULL;
			DeleteAllBinaryTransfers();
			PostThreadMessage(m_dwThreadID, WM_QUIT, 0, 0);
			m_wndMsg.Detach();
			m_dwThreadID = 0;
			m_hWndSkype = NULL;
			Sleep(500);
		}
	} catch(...) {
		return;
	}
}

void CSkypeMessageQueue::RestartTimer()
{
	m_bForceLaunch = FALSE;

	// Allow 1/10th second for a response of any type
	m_wndMsg.StartConnectTimer(100);
	//m_wndMsg.SetTimer(TIMER_SKYPE_DISCOVERY, 100);

	// Trigger a search for the Skype window
	::PostMessage(HWND_BROADCAST, CSkypeMessageWindow::WM_SKYPE_API_DISCOVER, (WPARAM) m_wndMsg.m_hWnd, 0);
}

// Resultイベント以外はイベントを生成しない
LPWSTR CSkypeMessageQueue::ExecuteBlockingCommand(LPCWSTR Command, 
	LPCWSTR WaitForResult, BOOL DoNotUseCmdID)
{
	COPYDATASTRUCT cs;
	cs.dwData = 0;
	bool bResult;

	CheckSkypeAPI();

	WaitForBlockingToComplete();

	m_bBlocking = true;
	m_bBlockingError = false;
	m_strDesiredResult = WaitForResult;

	if((m_lSupportedProtocol >= 4) && (! DoNotUseCmdID)) {
		DWORD dwTicks = GetTickCount();
		swprintf_s(m_strCommandID, sizeof(m_strCommandID)/sizeof(m_strCommandID[0]), L"#%d ", dwTicks);
		wstring wstrCommand = m_strCommandID;
		wstrCommand += Command;
		m_blockingCommandID = true;
		bResult = InternalProcessCommand(wstrCommand.c_str());
	} else {
		bResult = InternalProcessCommand(Command);
		m_blockingCommandID = false;
	}

	if(! bResult) {
		m_bBlocking = false;
		m_blockingCommandID = false;
		m_bBlockingError = true;
		m_strCommandResult = L"Skype did not acknowledge command";
	} else {
		// Crank up our own message queue to wait for response
		DWORD dwStart = GetTickCount();
		while(m_bBlocking) {
			if(GetTickCount() - dwStart > (DWORD) m_lTimeout) {
				m_bBlocking = false;
				throw L"No response returned from Skype";
			}
//paak			Sleep(1);
			Sleep(10);
		}
	}

	// If there was an error, throw an exception
	if(m_bBlockingError) {
		throw m_strCommandResult.c_str();
	} 
	
	return _wcsdup(m_strCommandResult.c_str());
}

LPWSTR CSkypeMessageQueue::RetrieveProperty(LPCWSTR ObjectType, 
	LONG ObjectID, LPCWSTR PropertyName)
{
	WCHAR strMessage[128], * ptrReturn;

	CheckSkypeAPI();

	try {
		swprintf_s(strMessage, 128, L"GET %s %d %s", ObjectType, ObjectID, PropertyName);
		WCHAR * ptrResult = ExecuteBlockingCommand(strMessage, & strMessage[4]);
		if(ptrResult == NULL) {
			ptrReturn = _wcsdup(L"");
		} else {
			ptrReturn = _wcsdup(& ptrResult[wcslen(strMessage) - 3]);
			free(ptrResult);
		}
	} catch(...) {
		return NULL;
	}

	return ptrReturn;
}

LPWSTR CSkypeMessageQueue::RetrieveProperty(LPCWSTR ObjectType, 
	LPCWSTR ObjectID, LPCWSTR PropertyName)
{
	WCHAR strMessage[128], * ptrReturn;

	CheckSkypeAPI();

	try {
		swprintf_s(strMessage, 128, L"GET %s %s%s%s", ObjectType, ObjectID, 
			wcslen(ObjectID) > 0 ? L" " : L"", PropertyName);
		WCHAR * ptrResult = ExecuteBlockingCommand(strMessage, & strMessage[4]);
		if(ptrResult == NULL) {
			ptrReturn = _wcsdup(L"");
		} else {
			ptrReturn = _wcsdup(& ptrResult[wcslen(strMessage) - 3]);
			free(ptrResult);
		}
	} catch(...) {
		return NULL;
	}

	return ptrReturn;
}

void CSkypeMessageQueue::SetProperty(LPCWSTR ObjectType, 
	LONG ObjectID, LPCWSTR PropertyName, LPCWSTR PropertyValue, 
	LPCWSTR OverrideResponses)
{
	WCHAR strMessage[256];

	CheckSkypeAPI();

	try {
		swprintf_s(strMessage, 256, L"SET %s %d %s %s", ObjectType, ObjectID, PropertyName, PropertyValue);
		ExecuteBlockingCommand(strMessage, 
			(OverrideResponses == NULL) ? (& strMessage[4]) : OverrideResponses);
	} catch(...) {
	}
}

void CSkypeMessageQueue::SetPropertyNoWait(LPCWSTR ObjectType, 
	LONG ObjectID, LPCWSTR PropertyName, LPCWSTR PropertyValue) 
{
	WCHAR strMessage[256];

	CheckSkypeAPI();

	swprintf_s(strMessage, 256, L"SET %s %d %s %s", ObjectType, ObjectID, PropertyName, PropertyValue);
	SendCommand(strMessage);
}

void CSkypeMessageQueue::SetProperty(LPCWSTR ObjectType, 
	LPCWSTR ObjectID, LPCWSTR PropertyName, LPCWSTR PropertyValue, 
	LPCWSTR OverrideResponses)
{
	WCHAR strMessage[256];

	CheckSkypeAPI();

	try {
		swprintf_s(strMessage, 256, L"SET %s %s%s%s %s", ObjectType, ObjectID, 
			wcslen(ObjectID) > 0 ? L" " : L"", PropertyName, PropertyValue);
		ExecuteBlockingCommand(strMessage, 
			(OverrideResponses == NULL) ? (& strMessage[4]) : OverrideResponses);
	} catch(...) {
	}
}

void CSkypeMessageQueue::AlterMethodNoWait(LPCWSTR ObjectType, 
	LONG ObjectID, LPCWSTR MethodName, LPCWSTR MethodValue) 
{
	WCHAR strMessage[_MAX_LINE];

	CheckSkypeAPI();

	swprintf_s(strMessage, _MAX_LINE, L"ALTER %s %d %s %s", ObjectType, ObjectID, MethodName, MethodValue);
	SendCommand(strMessage);
}

// Add the sink object to notification list
void CSkypeMessageQueue::RegisterMessageSink(CSkypeMessageSink * Sink)
{
	bool bExists = false;
	for(CSkypeMessageSinkList::iterator it = m_MessageSinks.begin(); it != m_MessageSinks.end(); it++) {
		if(*it == Sink) {
			bExists = true;
			break;
		}
	}
	if(! bExists) {
		m_MessageSinks.push_back(Sink);
	}
}

// Remove a sink object from notification list
void CSkypeMessageQueue::UnregisterMessageSink(CSkypeMessageSink * Sink)
{
	m_MessageSinks.remove(Sink);
}


/*
	Make sure we're set up for Skype
*/
void CSkypeMessageQueue::CheckSkypeAPI() {

	if((m_hWndSkype != NULL) && (m_hThread != NULL)) {
		return;
	}

	// Trigger an initilaization
	Initialize();

	// Trigger a search for the Skype window
	m_hWndSkype = NULL;
	::PostMessage(HWND_BROADCAST, CSkypeMessageWindow::WM_SKYPE_API_DISCOVER, (WPARAM) m_wndMsg.m_hWnd, 0);
	
	// Wait 20 seconds and see if skype responds
	DWORD dwNow = GetTickCount();
	while((m_hWndSkype == NULL) && (GetTickCount() - dwNow < 60000)) {
		Sleep(10);
	}

	if(m_hWndSkype == NULL) {
		throw("Skype Application Interface Not Available");
	}
}

/*
	Add a response to the queue for processing
*/
void CSkypeMessageQueue::ProcessResponse(LPCWSTR Response)
{
if(Response == NULL) {
	return;
}
if(wcslen(Response) == 0) {
	return;
}

#ifdef _TRACE_BINARY
	WCHAR strHold[500], strTemp[10];
	wcscpy(strHold, L"Received: ");
	for(ULONG i = 0; i < wcslen(Response); i++) {
		if(wcslen(strHold) > 490) {
			wcscat_s(strHold, 500, L"...");
			break;
		}

		WCHAR ch = Response[i];
		if((ch < 32) || (ch > 128)) {
			swprintf(strTemp, L"[%0.4X]", ch);
		} else {
			swprintf(strTemp, L"%c", ch);
		}
		wcscat_s(strHold, 500, strTemp);
	}
	wcscat(strHold, 500, L"\n");
	ATLTRACE(strHold);
#endif

	m_queueIncoming.push(Response);
}

/*
	Add a command to the queue for processing
*/
void CSkypeMessageQueue::SendCommand(LPCWSTR Command)
{
	m_queueOutgoing.push(Command);
}

void CSkypeMessageQueue::ProcessAPIStatus(SkypeAPIAttachmentStatus status, HWND hWndSkype)
{
	m_hWndSkype = hWndSkype;
	m_queueStatus.push(status);
	if(status == apiAttachSuccess) {
		// Force a protocol command
		m_lSupportedProtocol = -1;
		InternalProcessCommand(L"PROTOCOL 99999");
	}
}

void CSkypeMessageQueue::ProcessQueue()
{
	std::wstring strFull;
	std::wstring str;
	std::wstring strID;

	// Retrieve the next change in status
	if(! m_queueStatus.empty()) {
		SkypeAPIAttachmentStatus status = m_queueStatus.front();
		m_queueStatus.pop();

		for(CSkypeMessageSinkList::iterator it = m_MessageSinks.begin(); it != m_MessageSinks.end(); it++) {
			(*it)->ProcessAPIStatusChange(status);
		}
	}
	
	// Retrieve the next response, if there is one
	if(! m_queueIncoming.empty()) {
		strFull = m_queueIncoming.front();
		str = strFull;
		strID = L"";
		m_queueIncoming.pop();

		if(str[0] == L'#') {
			size_t iSpace = str.find(L" ");
			if(iSpace != -1) {
				strID = str.substr(0, iSpace + 1);
				str = str.substr(iSpace + 1, str.length() - iSpace);
			}
		}

		if(_wcsicmp(str.c_str(), L"CONNSTATUS LOGGEDOUT") == 0) {
			// When connection status is logged out, start rechecking for skype
			gSkypeQueue.DeleteAllBinaryTransfers();
			RestartTimer();
		}

		if(_wcsnicmp(str.c_str(), L"PROTOCOL ", 9) == 0) {
 			if(str.length() > 8) {
				bool bSendProtocol = (m_lSupportedProtocol == -1);
				m_lSupportedProtocol = _wtol(& str.c_str()[9]);
				if(bSendProtocol) {
					WCHAR strMsg[16];
					swprintf_s(strMsg, 16, L"PROTOCOL %d", SUPPORTED_PROTOCOL);
					InternalProcessCommand(strMsg);
				}
			}
		}

#ifndef _NO_USE_LIST
		if(! m_bNeedRebuildCallList) {
			if(_wcsnicmp(str.c_str(), L"CALL", 4) == 0) {
				WCHAR * ptrStatus;
				WCHAR * ptrStatusInd = wcsstr(str.c_str(), L" STATUS ");
				if(ptrStatusInd != NULL) {
					ptrStatus = & ptrStatusInd[8];
					if(ptrStatus != NULL) { 
						long lCallID = 0;
						if(swscanf_s(str.c_str(), L"CALL %d STATUS", & lCallID) == 1) {
							if(lCallID > 0) {
								SkypeCallProgress progress = TextToSkypeCallProgress(ptrStatus);
								UpdateCallProgress(lCallID, progress);
							}
						}
					}
				}
			}
		}
		if(! m_bNeedRebuildUserList) {
			if(_wcsnicmp(str.c_str(), L"USER", 4) == 0) {
				WCHAR * ptrOnlineStatus;
				WCHAR * ptrOnlineStatusInd = wcsstr(str.c_str(), L" ONLINESTATUS ");
				if(ptrOnlineStatusInd != NULL) {
					ptrOnlineStatus = & ptrOnlineStatusInd[14];
					if(ptrOnlineStatus != NULL) {
						WCHAR userID[_MAX_LINE];
						if(swscanf_s(str.c_str(), L"USER %s ONLINESTATUS", userID, _MAX_LINE) == 1) {
							if(wcslen(userID) > 0) {
								SkypeOnlineStatus status = TextToSkypeOnlineStatus(ptrOnlineStatus);
								UpdateUserOnlineStatus(userID, status);
							}
						}
					}
				}
			}
		}
#endif
		
		bool bProcessCallbacks = ! m_bBlocking;
		bool bMatch = false;
	
		// If we are waiting on a blocking call, set up the response
		if(m_bBlocking) {
			if((m_strDesiredResult.empty()) && (! m_blockingCommandID)) {
				m_strCommandResult = str;
			} else {
				if(m_blockingCommandID) {
					if(_wcsicmp(strID.c_str(), m_strCommandID) == 0) {
						m_strCommandResult = str;
						ATLTRACE(L"Response received: %s\n", strID.c_str());
						bMatch = true;	// イベントのブロックを解除
						
						if(_wcsnicmp(str.c_str(), L"ERROR", 5) == 0) {
							ATLTRACE(L"Error for %s detected: %s\n", strID.c_str(),
								str.c_str());
							m_bBlockingError = true;
						}
					}
				} else {
					WCHAR * pCopy = _wcsdup(m_strDesiredResult.c_str());
					WCHAR *next_token = NULL;
					WCHAR * pSearch = wcstok_s(pCopy, L"|", &next_token);
					while(pSearch && (! bMatch)) {
						if(wcsstr(str.c_str(), pSearch) != NULL) {
							m_strCommandResult = str;
							bMatch = true;
							break;
						}
						pSearch = wcstok_s(NULL, L"|", &next_token);
					}
					try {
						free(pCopy);
					} catch(...) {
					}

					if(! bMatch) {
						if(_wcsnicmp(str.c_str(), L"ERROR", 5) == 0) {
							m_strCommandResult = str;
							m_bBlockingError = true;
							bMatch = true;
						}
					}
				}
			}
		}	

		// Let the blocking routine know if a match was found
		if(m_bBlocking) {
			m_bBlocking = ! bMatch;
		}

// paak
// ブロックコマンド実行中のイベントの抑制は行わない。
// イベントは常に発生させ、コールバック内で有効性をチェックすべき。
bProcessCallbacks = true;

		// Trigger the response event sinks
		for(CSkypeMessageSinkList::iterator it = m_MessageSinks.begin(); it != m_MessageSinks.end(); it++) {
			(*it)->ProcessResponse(strFull.c_str(), bProcessCallbacks);
		}
	}

	if((! m_queueOutgoing.empty()) && (m_hWndSkype != NULL)) {

		str = m_queueOutgoing.front();
		m_queueOutgoing.pop();
		
		InternalProcessCommand(str.c_str());
	}

}

bool CSkypeMessageQueue::InternalProcessCommand(LPCWSTR Command)
{
	COPYDATASTRUCT cs;
	bool bSuccess = false;
#ifdef _TRACE_BINARY
	CHAR strHold[500], strTemp[10];
#endif

	LRESULT result = WideCharToMultiByte(CP_UTF8, 0, Command, (int) wcslen(Command), NULL, NULL, NULL, FALSE);
	if(result != 0) {
		char * pMsg = new char[result + 1];
		memset(pMsg, 0, result + 1);
		result = WideCharToMultiByte(CP_UTF8, 0, Command, (int) wcslen(Command), pMsg, (int) result, NULL, FALSE);
		if(result != 0) {
			cs.dwData = 0;
			cs.cbData = (DWORD) result + 1;
			cs.lpData = pMsg;
#ifdef _TRACE_BINARY
			strcpy(strHold, "Sent: ");
			for(ULONG i = 0; i < strlen(pMsg); i++) {
				if(strlen(strHold) > 490) {
					strcat(strHold, "...");
					break;
				}
				BYTE ch = (BYTE) pMsg[i];
				if((ch < 32) || (ch > 128)) {
					sprintf(strTemp, "[%0.2X]", ch);
				} else {
					sprintf(strTemp, "%c", ch);
				}
				strcat(strHold, strTemp);
			}
			strcat(strHold, "\n");
			ATLTRACE(strHold);
#endif
			result = ::SendMessage(m_hWndSkype, WM_COPYDATA, (WPARAM) m_wndMsg.m_hWnd, (LPARAM) & cs);
			if(result == 1) {
				for(CSkypeMessageSinkList::iterator it = m_MessageSinks.begin(); it != m_MessageSinks.end(); it++) {
					(*it)->ProcessCommand(Command);
				}
				bSuccess = true;
			}
		}
		delete [] pMsg;
	}

	return bSuccess;
}


void CSkypeMessageQueue::WaitForBlockingToComplete()
{
	if(! m_bBlocking)
		return;

	DWORD dwNow = GetTickCount();
	while(m_bBlocking) {
		MSG msg;
		if(GetMessage(& msg, m_wndMsg.m_hWnd, 0, 0) != FALSE) {
			TranslateMessage(& msg);
			DispatchMessage(& msg);
		}
		Sleep(10);
		if(GetTickCount() - dwNow > (DWORD) m_lTimeout) {
			throw "Blocking command already in progress";
			return;
		}
	}

	return;
}

#ifndef _NO_USE_LIST
void CSkypeMessageQueue::InternalRebuildCallList()
{
	m_bNeedRebuildCallList = false;
	m_listCalls.clear();

	try {
		WCHAR * pStrCalls = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH CALLS", L"CALLS");
		WCHAR * list = pStrCalls;
		WCHAR *next_token = NULL;
		WCHAR * pItem = wcstok_s(list, L", ", &next_token);
		while(pItem) {
			long lID = _wtol(pItem);
			if(lID > 0) {
				m_listCalls[lID] = prgUnknown;
			}
			pItem = wcstok_s(NULL, L", ", &next_token);
		}
		free(pStrCalls);
	} catch(const WCHAR * err) {
	}
}
#endif

#ifndef _NO_USE_LIST
const CallList & CSkypeMessageQueue::GetCallList()
{
	if(m_bNeedRebuildCallList) {
		InternalRebuildCallList();
	}

	return m_listCalls;
}
#endif

#ifndef _NO_USE_LIST
SkypeCallProgress CSkypeMessageQueue::GetCallProgress(long CallID)
{
	SkypeCallProgress retVal = prgUnknown;
	if(m_bNeedRebuildCallList) {
		InternalRebuildCallList();
	}

	CallList::iterator it = m_listCalls.find(CallID);
	if(it != m_listCalls.end()) {
		retVal = it->second;
	}
		
	try {
		if(retVal == prgUnknown) {
			WCHAR * pResult = RetrieveProperty(L"CALL", CallID, L"STATUS");
			retVal = TextToSkypeCallProgress(pResult);
			free(pResult);
			m_listCalls[CallID] = retVal;
		}
	} catch(...) {
	}

	return retVal;
}
#endif

#ifndef _NO_USE_LIST
void CSkypeMessageQueue::UpdateCallProgress(long CallID, SkypeCallProgress Progress)
{
	if(m_bNeedRebuildCallList) {
		InternalRebuildCallList();
	}
	m_listCalls[CallID] = Progress;
}
#endif

#ifndef _NO_USE_LIST
void CSkypeMessageQueue::InternalRebuildUserList()
{
	m_bNeedRebuildUserList = false;
	m_listUsers.clear();

	try {
		WCHAR * pStrUsers = gSkypeQueue.ExecuteBlockingCommand(L"SEARCH FRIENDS", L"USERS");
		WCHAR *next_token = NULL;
		WCHAR * pItem = wcstok_s(pStrUsers, L", ", &next_token);
		while(pItem) {
			if(wcslen(pItem) > 0) {
				m_listUsers[std::wstring(pItem)] = olsUnknown;
			}
			pItem = wcstok_s(NULL, L", ", &next_token);
		}
		free(pStrUsers);
	} catch(const WCHAR * err) {
	}
}
#endif

#ifndef _NO_USE_LIST
const OnlineUserList & CSkypeMessageQueue::GetUserList()
{
	if(m_bNeedRebuildUserList) {
		InternalRebuildUserList();
	}

	return m_listUsers;
}
#endif

#ifndef _NO_USE_LIST
SkypeOnlineStatus CSkypeMessageQueue::GetUserOnlineStatus(LPCWSTR UserID)
{
	SkypeOnlineStatus retVal = olsUnknown;
	if(m_bNeedRebuildUserList) {
		InternalRebuildUserList();
	}

	std::wstring strUserID(UserID);
	OnlineUserList::iterator it = m_listUsers.find(strUserID);
	if(it != m_listUsers.end()) {
		retVal = it->second;
	}
		
	try {
		if(retVal == prgUnknown) {
			WCHAR * ptrResponse = gSkypeQueue.RetrieveProperty(L"USER", strUserID.c_str(), L"ONLINESTATUS");
			retVal = TextToSkypeOnlineStatus(ptrResponse);
			free(ptrResponse);
			m_listUsers[strUserID] = retVal;
		}
	} catch(...) {
	}

	return retVal;
}
#endif

#ifndef _NO_USE_LIST
void CSkypeMessageQueue::UpdateUserOnlineStatus(LPCWSTR UserID, SkypeOnlineStatus Status)
{
	if(m_bNeedRebuildUserList) {
		InternalRebuildUserList();
	}
	m_listUsers[std::wstring(UserID)] = Status;
}
#endif

long CSkypeMessageQueue::SupportedProtocol()
{
	try {
		if(m_lSupportedProtocol == -1) {
			WCHAR * pResult = ExecuteBlockingCommand(L"PROTOCOL 99999", L"PROTOCOL");
			if(wcslen(pResult) > 8) {
				m_lSupportedProtocol = _wtol(& pResult[9]);
				WCHAR strMsg[16];
				swprintf_s(strMsg, 16, L"PROTOCOL %d", SUPPORTED_PROTOCOL);
				InternalProcessCommand(strMsg);
			}
			free(pResult);
		}
	} catch(...) {
		return SUPPORTED_PROTOCOL;
	}

	return m_lSupportedProtocol; 
}

BOOL CALLBACK FindSkypeProcess(
  HWND hwnd,      // handle to parent window
  LPARAM lParam   // application-defined value
)
{
	TCHAR strClassName[_MAX_LINE];
	if(GetClassName(hwnd, strClassName, _MAX_LINE) != 0) {
		if(_tcsicmp(strClassName, _T("tSkMainForm.UnicodeClass")) == 0) {
			GetWindowThreadProcessId(hwnd, (DWORD *) lParam);
			return FALSE;
		}
		
	}
	return TRUE;
}

BOOL CALLBACK MinimizeSkypeProcess(
  HWND hwnd,      // handle to parent window
  LPARAM lParam   // application-defined value
)
{
	TCHAR strClassName[_MAX_LINE];
	if(GetClassName(hwnd, strClassName, _MAX_LINE) != 0) {
		if(_tcsicmp(strClassName, _T("tSkMainForm.UnicodeClass")) == 0) {
			GetWindowThreadProcessId(hwnd, (DWORD *) lParam);
//			CloseWindow(hwnd);

			SendMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
			return FALSE;
		}
		
	}
	return TRUE;
}

BOOL CALLBACK TriggerQuit(
  HWND hwnd,      // handle to parent window
  LPARAM lParam   // application-defined value
)
{
	TCHAR strClassName[_MAX_LINE];
	if(GetClassName(hwnd, strClassName, _MAX_LINE) != 0) {
		if(_tcsicmp(strClassName, _T("TPUtilWindow")) == 0) {
			DWORD dwID;
			GetWindowThreadProcessId(hwnd, & dwID);
			if(dwID == (DWORD) lParam) {
				PostMessage(hwnd, WM_COMMAND, 0x20, 0);
				PostMessage(hwnd, WM_COMMAND, 0x21, 0);
			}
		}
		
	}
	return TRUE;
}

void CSkypeMessageQueue::CloseSkype()
{
	DWORD dwProcessID = 0;
	// Close the Skype application window
	EnumWindows(FindSkypeProcess, (LPARAM) & dwProcessID);
	// Force a redraw the system tray, which is a toolbar
	EnumWindows(TriggerQuit, dwProcessID);
}

void CSkypeMessageQueue::MinimizeSkype()
{
	DWORD dwProcessID = 0;
	// Close the Skype application window
	EnumWindows(MinimizeSkypeProcess, (LPARAM) & dwProcessID);
}

void CSkypeMessageQueue::DeleteAllBinaryTransfers()
{
	for(BinaryTransferPTRList::iterator it = m_listTransfers.begin(); 
		it != m_listTransfers.end(); it++) {
		delete * it;
	}

	m_listTransfers.clear();
}

void CSkypeMessageQueue::DeleteStreamBinaryTransfers(LPCWSTR StreamID)
{
	for(BinaryTransferPTRList::iterator it = m_listTransfers.begin(); 
		it != m_listTransfers.end(); it++) {
		BinaryTransferPTR ptrItem = * it;
		if(_wcsicmp(ptrItem->StreamID(), StreamID) == 0) {
			m_listTransfers.remove(ptrItem);
			delete(ptrItem);
		}
	}
}

BinaryTransfer * CSkypeMessageQueue::GetBinaryTransfer(LPCWSTR ID)
{
	for(BinaryTransferPTRList::iterator it = m_listTransfers.begin();
		it != m_listTransfers.end(); it++) {
		BinaryTransfer * ptrItem = * it;
		if(_wcsicmp(ptrItem->ID(), ID) == 0) {
			return ptrItem;
		}
	}

	return NULL;
}

void CSkypeMessageQueue::StartBinaryTransfer(LPCWSTR ID, LPCWSTR StreamID, LPCSTR MD5Checksum, ULONG TotalSize)
{
	if(GetBinaryTransfer(ID) != NULL) {
		throw L"This binary transfer has already been started";
	}
	BinaryTransferPTR ptrNewItem = new BinaryTransfer(ID, StreamID, MD5Checksum, TotalSize);
	m_listTransfers.push_back(ptrNewItem);
}

BinaryTransferStatus * CSkypeMessageQueue::AppendBinaryTransfer(LPCWSTR ID, BYTE * Buffer, ULONG Size)
{
	BinaryTransferPTR ptrItem = GetBinaryTransfer(ID);
	if(ptrItem == NULL) {
		throw L"This binary transfer has not been started yet";
	}

	ptrItem->AppendData(Buffer, Size);
	BinaryTransferStatus * ptrStatus = new BinaryTransferStatus;
	ptrStatus->lBytesTransferred = ptrItem->Length();
	ptrStatus->lBytesTotal = ptrItem->Size();
	return ptrStatus;
}

void CSkypeMessageQueue::EndBinaryTransfer(LPCWSTR ID, BOOL Cancelled)
{
	BinaryTransferPTR ptrItem = GetBinaryTransfer(ID);
	if(ptrItem == NULL) {
		throw L"This binary transfer has not been started yet";
	}

	try {
		m_listTransfers.remove(ptrItem);
		delete ptrItem;
	} catch(...) {
		throw L"Unable to remove binary transfer item from list";
	}
}


BinaryTransfer::BinaryTransfer(LPCWSTR ID, LPCWSTR StreamID, LPCSTR MD5, ULONG Size)
{
	m_strID = ID;
	m_strStreamID = StreamID;
	m_strMD5 = MD5;
	m_ulSize = Size;
	m_ulPos = 0;
	m_ptrBuffer = new byte[m_ulSize];
}

BinaryTransfer::~BinaryTransfer()
{
	delete [] m_ptrBuffer;
}

void BinaryTransfer::AppendData(BYTE * Data, ULONG Size)
{
	bool bLastFF = false;

	for(unsigned long i=0; i < Size; i++) {

		if(m_ulPos >= m_ulSize) {
			throw L"Error transfering data: buffer overrun";
		}

		switch(Data[i]) {
			case 0xFF:
				if(bLastFF) {
					m_ptrBuffer[m_ulPos++] = 0xFF;
					bLastFF = false;
				} else {
					bLastFF = true;
				}
				break;
			case 0xFE:
				if(bLastFF) {
					m_ptrBuffer[m_ulPos++] = 0;
					bLastFF = false;
				} else {
					m_ptrBuffer[m_ulPos++] = 0xFE;
				}
				break;
			default:
				if(bLastFF) {
					throw L"Unexpected character encountered during binary transfer";
				}
				m_ptrBuffer[m_ulPos++] = Data[i];
				break;
		}
	}
}

bool BinaryTransfer::MD5Match()
{
	bool bResult = false;

	::MD5 objMD5;
	LPCSTR ptrMD5 = objMD5.CalcMD5FromBuffer(m_ptrBuffer, m_ulPos);
	bResult = _stricmp(ptrMD5, m_strMD5.c_str()) == 0;
	return bResult;
}


