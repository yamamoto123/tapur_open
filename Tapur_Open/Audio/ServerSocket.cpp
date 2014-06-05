// ServerSocket.cpp : �����t�@�C��
//

#include "ServerSocket.h"
#include "WaveBuffer.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


//////////////////////////////////////////////////////////////////////
// �\�P�b�g����f�[�^��M�X���b�h
UINT ServerSocketWorkerThread( LPVOID pParam )
{
	CServerSocket* pSvSocket = (CServerSocket*)pParam;
	int nRec = SOCKET_IN_BUFFER_SIZE;
	int nPos = 0;
	int nLen = 0;

	TRY {
		// �\�P�b�g���牽������M����
		while(1)
		{
			Sleep(10);  //CPU���׌���

			// ���~�t���O�̃`�F�b�N
			if (pSvSocket->m_blStop) {
				break;
			}

			nLen = pSvSocket->m_socket.Receive(pSvSocket->m_szBuf + nPos, nRec);
			if(nLen == 0) {
				// ��M�f�[�^�Ȃ�
				Sleep(100);
				continue;
			} else if(nLen < 0) {
				int intErr = pSvSocket->m_socket.GetLastError();
				if (intErr == WSAEWOULDBLOCK) {
					// �ꎞ�I�ȃu���b�N��Ԃ̓G���[�Ƃ͌��Ȃ��Ȃ�
					Sleep(100);
					continue;
				} else {
					// �G���[
					_stprintf_s(pSvSocket->te.errstr, MAX_ERRSTR, _T("Socket Receive error:%d"), intErr);
					pSvSocket->te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, pSvSocket->te.errstr);
					break;
				}
			}

			// ��M�f�[�^�ʂ��o�b�t�@�T�C�Y�ɖ����Ȃ��ꍇ�A�ǉ���M����
			nPos += nLen;
			if (nPos < SOCKET_IN_BUFFER_SIZE) {
				nRec = SOCKET_IN_BUFFER_SIZE - nPos;
				continue;
			}

			// ��M�������e���o�b�t�@�ɃR�s�[
			pSvSocket->AddNewBuffer(pSvSocket->m_szBuf, SOCKET_IN_BUFFER_SIZE);
			nRec = SOCKET_IN_BUFFER_SIZE;
			nPos = 0;
		}

		// �\�P�b�g���N���[�Y
// �G���[�ɂȂ�B�N���X���Đ�������̂ŕs�v�H
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


// CServerSocket �����o�֐�

// ??OnReceive�C�x���g���������Ȃ�
//void CServerSocket::OnReceive(int nErrorCode)                                                
//{
//	// �G���[������ꍇ�́A�G���[��\��
//    if( nErrorCode != 0)
//    {
//		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket OnReceive error:%d"), nErrorCode);
//		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
//        goto PROC_END;
//    }
//
//	// ���~�t���O�̃`�F�b�N
//	if (m_blStop) {
//        goto SOCKET_CLOSE;	// �\�P�b�g���N���[�Y
//	}
//
//	// �f�[�^��M
//	int nLen = m_socket.Receive(m_szBuf, sizeof(m_szBuf));
//	if(nLen == 0) {
//		// �f�[�^��M�I��
//        goto SOCKET_CLOSE;	// �\�P�b�g���N���[�Y
//	} else if(nLen < 0) {
//		int intErr = GetLastError();
//		if (intErr == WSAEWOULDBLOCK) {
//			// �ꎞ�I�ȃu���b�N��Ԃ̓G���[�Ƃ͌��Ȃ��Ȃ�
//			goto PROC_END;
//		} else {
//			// �G���[
//			_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket Receive error:%d"), intErr);
//			te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
//			goto SOCKET_CLOSE;	// �\�P�b�g���N���[�Y
//		}
//	}
//
//	// ��M�������e���o�b�t�@�ɃR�s�[
//	AddNewBuffer(m_szBuf, nLen);
//	goto PROC_END;
//
//SOCKET_CLOSE:
//	// �\�P�b�g���N���[�Y
//	m_socket.Close();
//PROC_END:
//	CAsyncSocket::OnReceive(nErrorCode);
//}

void CServerSocket::OnAccept(int nErrorCode)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

    // �G���[������ꍇ�́A�G���[��\��
    if( nErrorCode != 0)
    {
        CString strErr;
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Socket OnAccept error:%d"), nErrorCode);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SOCK, te.errstr);
        goto PROC_END;
    }

	m_blStop = FALSE;

	// �ڑ��p�̃\�P�b�g��p�ӂ���
	if (Accept(m_socket) == FALSE) goto PROC_END;

	// �\�P�b�g����f�[�^��M���X���b�h��
	if (m_blWorkThread == FALSE) {
		CWinThread* pThread = AfxBeginThread(ServerSocketWorkerThread, this);
		m_blWorkThread = (pThread == NULL) ? FALSE : TRUE;
	}

PROC_END:
	CAsyncSocket::OnAccept(nErrorCode);
}

// ��M�f�[�^���o�b�t�@��
bool CServerSocket::AddNewBuffer(void *pData, long lngSize)
{
	CWaveBuffer *pBuf = new CWaveBuffer;

	pBuf->m_blActive = TRUE;	// �X���b�h�ɂ��A�N�Z�X�𐧌�����t���O
	if (m_blPause) {
		// ��~�̏ꍇ�̓f�[�^�𖳌�/�����ɂ���
		pBuf->m_blDead = TRUE;	
		memset(pData, '\0', lngSize);
	}
	m_plistOfBuffer->AddTail(pBuf);
	( (CWaveBuffer*)m_plistOfBuffer->GetTail() )->CopyBuffer(pData, lngSize / 4, 4);	// 4:16bit stereo
	pBuf->m_blActive = FALSE;

	return true;
}
