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
	CAsyncSocket m_socket;		// ��M�p�\�P�b�g
	CPtrList *m_plistOfBuffer;	// ��M���X�g�o�b�t�@
	BOOL m_blWorkThread;		// �f�[�^��M�X���b�h�̐������
	BOOL m_blPause;				// �ꎞ��~�t���O
	BOOL m_blStop;				// ���~�t���O
	char m_szBuf[SOCKET_IN_BUFFER_SIZE];	// ���[�J���o�b�t�@

	bool AddNewBuffer(void *pData, long lngSize);

public:
	CServerSocket();
	~CServerSocket();
	void OnAccept(int nErrorCode);
	//void OnReceive(int nErrorCode);

	void SetListBuffer(CPtrList *m_plb) { m_plistOfBuffer = m_plb; }
	void Pause() { m_blPause = TRUE; }		// �ꎞ��~
	void Continue() { m_blPause = FALSE; }	// �ꎞ��~�ĊJ
	BOOL IsPause() { return m_blPause; }
	BOOL IsWorking()  { return m_blWorkThread; }
	void Stop() { m_blStop = TRUE; }		// ���~
};


#endif // !defined(AFX_SERVERSOCKET_H__2473839B_76B0_45EB_9F9A_386D27903BB1__INCLUDED_)
