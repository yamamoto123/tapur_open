// ServerSocketIn.h: interface for the CSocketIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSOCKET_H__2473839B_76B0_45EB_9F9A_386D27903BB1__INCLUDED_)
#define AFX_SERVERSOCKET_H__2473839B_76B0_45EB_9F9A_386D27903BB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
#include <afxsock.h>
#include "TPerror.h"

//////////////////////////////////////////////////////////////////////
#define SOCKET_IN_BUFFER_SIZE 4096

//////////////////////////////////////////////////////////////////////
class CServerSocket : public CAsyncSocket
{
	friend UINT ServerSocketWorkerThread( LPVOID pParam );

private:
	CTPerror te;
	CAsyncSocket m_socket;		// 受信用ソケット
	CPtrList *m_plistOfBuffer;	// 受信リストバッファ
	BOOL m_blWorkThread;		// データ受信スレッドの生存状態
	BOOL m_blPause;				// 一時停止フラグ
	BOOL m_blStop;				// 中止フラグ
	char m_szBuf[SOCKET_IN_BUFFER_SIZE];	// ローカルバッファ

	bool AddNewBuffer(void *pData, long lngSize);

public:
	CServerSocket();
	~CServerSocket();
	void OnAccept(int nErrorCode);
	//void OnReceive(int nErrorCode);

	void SetListBuffer(CPtrList *m_plb) { m_plistOfBuffer = m_plb; }
	void Pause() { m_blPause = TRUE; }		// 一時停止
	void Continue() { m_blPause = FALSE; }	// 一時停止再開
	BOOL IsPause() { return m_blPause; }
	BOOL IsWorking()  { return m_blWorkThread; }
	void Stop() { m_blStop = TRUE; }		// 中止
};


#endif // !defined(AFX_SERVERSOCKET_H__2473839B_76B0_45EB_9F9A_386D27903BB1__INCLUDED_)
