// ServerSocket.cpp : 実装ファイル
//

#include "ServerSocket.h"
#include "WaveBuffer.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


//////////////////////////////////////////////////////////////////////
// ソケットからデータ受信スレッド
UINT ServerSocketWorkerThread( LPVOID pParam )
{
	CServerSocket* pSvSocket = (CServerSocket*)pParam;
	int nRec = SOCKET_IN_BUFFER_SIZE;
	int nPos = 0;
	int nLen = 0;

	TRY {
		// ソケットから何かを受信する
		while(1)
		{
			Sleep(10);  //CPU負荷減少

			// 中止フラグのチェック
			if (pSvSocket->m_blStop) {
				break;
			}

			nLen = pSvSocket->m_socket.Receive(pSvSocket->m_szBuf + nPos, nRec);
			if(nLen == 0) {
				// 受信データなし
				Sleep(100);
				continue;
			} else if(nLen < 0) {
				int intErr = pSvSocket->m_socket.GetLastError();
				if (intErr == WSAEWOULDBLOCK) {
					// 一時的なブロック状態はエラーとは見なさない
					Sleep(100);
					continue;
				} else {
					// エラー
					_stprintf_s(pSvSocket->te.errstr, MAX_ERRSTR, _T("Socket Receive error:%d"), intErr);
					pSvSocket->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, pSvSocket->te.errstr);
					break;
				}
			}

			// 受信データ量がバッファサイズに満たない場合、追加受信する
			nPos += nLen;
			if (nPos < SOCKET_IN_BUFFER_SIZE) {
				nRec = SOCKET_IN_BUFFER_SIZE - nPos;
				continue;
			}

			// 受信した内容をバッファにコピー
			pSvSocket->AddNewBuffer(pSvSocket->m_szBuf, SOCKET_IN_BUFFER_SIZE);
			nRec = SOCKET_IN_BUFFER_SIZE;
			nPos = 0;
		}

		// ソケットをクローズ
// エラーになる。クラスを再生成するので不要？
//		pSvSocket->m_socket.Close();
	}
    CATCH_ALL(e)
    {
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		_stprintf_s(pSvSocket->te.errstr, MAX_ERRSTR, _T("Socket Receive error:%s"), szCause);
		pSvSocket->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, pSvSocket->te.errstr);
    }
	END_CATCH_ALL

//THREAD_END:
	pSvSocket->m_blWorkThread = FALSE;
	return 0;
};


// CServerSocket
CServerSocket::CServerSocket()
{
	m_blWorkThread = FALSE;		
	m_blPause = FALSE;				
	m_blStop = FALSE;				

	m_plistOfBuffer = NULL;
}

CServerSocket::~CServerSocket()
{
	m_blWorkThread = FALSE;		
	m_blPause = FALSE;				
	m_blStop = FALSE;				

	m_plistOfBuffer = NULL;
}


// CServerSocket メンバ関数

// ??OnReceiveイベントが発生しない
//void CServerSocket::OnReceive(int nErrorCode)                                                
//{
//	// エラーがある場合は、エラーを表示
//    if( nErrorCode != 0)
//    {
//		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket OnReceive error:%d"), nErrorCode);
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
//        goto PROC_END;
//    }
//
//	// 中止フラグのチェック
//	if (m_blStop) {
//        goto SOCKET_CLOSE;	// ソケットをクローズ
//	}
//
//	// データ受信
//	int nLen = m_socket.Receive(m_szBuf, sizeof(m_szBuf));
//	if(nLen == 0) {
//		// データ受信終了
//        goto SOCKET_CLOSE;	// ソケットをクローズ
//	} else if(nLen < 0) {
//		int intErr = GetLastError();
//		if (intErr == WSAEWOULDBLOCK) {
//			// 一時的なブロック状態はエラーとは見なさない
//			goto PROC_END;
//		} else {
//			// エラー
//			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket Receive error:%d"), intErr);
//			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
//			goto SOCKET_CLOSE;	// ソケットをクローズ
//		}
//	}
//
//	// 受信した内容をバッファにコピー
//	AddNewBuffer(m_szBuf, nLen);
//	goto PROC_END;
//
//SOCKET_CLOSE:
//	// ソケットをクローズ
//	m_socket.Close();
//PROC_END:
//	CAsyncSocket::OnReceive(nErrorCode);
//}

void CServerSocket::OnAccept(int nErrorCode)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

    // エラーがある場合は、エラーを表示
    if( nErrorCode != 0)
    {
        CString strErr;
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket OnAccept error:%d"), nErrorCode);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
        goto PROC_END;
    }

	m_blStop = FALSE;

	// 接続用のソケットを用意する
	if (Accept(m_socket) == FALSE) goto PROC_END;

	// ソケットからデータ受信をスレッド化
	if (m_blWorkThread == FALSE) {
		CWinThread* pThread = AfxBeginThread(ServerSocketWorkerThread, this);
		m_blWorkThread = (pThread == NULL) ? FALSE : TRUE;
	}

PROC_END:
	CAsyncSocket::OnAccept(nErrorCode);
}

// 受信データをバッファ化
bool CServerSocket::AddNewBuffer(void *pData, long lngSize)
{
	CWaveBuffer *pBuf = new CWaveBuffer;

	pBuf->m_blActive = TRUE;	// スレッドによるアクセスを制限するフラグ
	if (m_blPause) {
		// 停止の場合はデータを無効/無音にする
		pBuf->m_blDead = TRUE;	
		memset(pData, '\0', lngSize);
	}
	m_plistOfBuffer->AddTail(pBuf);
	( (CWaveBuffer*)m_plistOfBuffer->GetTail() )->CopyBuffer(pData, lngSize / 4, 4);	// 4:16bit stereo
	pBuf->m_blActive = FALSE;

	return true;
}
