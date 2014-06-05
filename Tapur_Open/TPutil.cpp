
#include "stdafx.h"
#include <time.h>

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


//
//  �@�\     : �����������O�����`���̕�����Ő���
//  
//  �@�\���� : ex) "200501010100"
//  
//  �Ԃ�l   : 0:ok -1:error
//  
//  ���l     : 
//  
int GetDateId(
	_TCHAR *date_id)	// ����������̕ۑ��o�b�t�@
{
	time_t ct = time(NULL);
	struct tm lst;
	localtime_s(&lst, &ct);

	_stprintf_s(date_id, 15, _T("%04d%02d%02d%02d%02d%02d"), (1900+lst.tm_year), (1+lst.tm_mon),
		lst.tm_mday, lst.tm_hour, lst.tm_min, lst.tm_sec);

	return 0;
}

//
//  �@�\     : �����O�����`���̕�����𐮌`
//  
//  �@�\���� : ex) "2005/01/01/ 01:00"
//  
//  �Ԃ�l   : 0:ok -1:error
//  
//  ���l     : 
//  
int CnvDateString(
const _TCHAR *date_id, 	// �����O�����`���̕�����
_TCHAR *date_str)	// ����������̕ۑ��o�b�t�@
{
	_TCHAR strTmp[64];

	ZeroMemory(date_str, 64);
	_tcscpy_s(strTmp, sizeof(strTmp)/sizeof(strTmp[0]), date_id);
	_tcsncpy_s(date_str, sizeof(strTmp)/sizeof(strTmp[0]), strTmp, 4);
	_tcscat_s(date_str, sizeof(strTmp)/sizeof(strTmp[0]), _T("/"));
	_tcsncat_s(date_str, sizeof(strTmp)/sizeof(strTmp[0]), strTmp+4, 2);
	_tcscat_s(date_str, sizeof(strTmp)/sizeof(strTmp[0]), _T("/"));
	_tcsncat_s(date_str, sizeof(strTmp)/sizeof(strTmp[0]), strTmp+6, 2);
	_tcscat_s(date_str, sizeof(strTmp)/sizeof(strTmp[0]), _T(" "));
	_tcsncat_s(date_str, sizeof(strTmp)/sizeof(strTmp[0]), strTmp+8, 2);
	_tcscat_s(date_str, sizeof(strTmp)/sizeof(strTmp[0]), _T(":"));
	_tcsncat_s(date_str, sizeof(strTmp)/sizeof(strTmp[0]), strTmp+10, 2);

	return 0;
}

//
//  �@�\     : BSTR����CString�ւ̌^�ϊ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
void CnvBSTR_CString(BSTR bStr, CString* pCStr)
{ 
    int nBytes;
    LPTSTR lpsz;
    int nLen;

    TRY {
        nLen = SysStringLen(bStr);
        nBytes = WideCharToMultiByte(CP_ACP, 0, bStr,
                   nLen, NULL, NULL, NULL, NULL);
        lpsz = pCStr->GetBufferSetLength(nBytes);
        WideCharToMultiByte(CP_ACP, 0, bStr, nLen,
                   (LPSTR)lpsz, nBytes, NULL, NULL);
    }
    CATCH( CMemoryException, e ) {
		AfxMessageBox(_T("Memory error."), MB_OK | MB_APPLMODAL | MB_ICONSTOP);
    } END_CATCH

    return;
}

//
//  �@�\     : CString����MultiByteCharacter�ւ̌^�ϊ�
//  
//  �@�\���� : �o�̓o�b�t�@�͌Ăяo�����Ŋm��
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
void CnvCString_Bytes(CString str, char *mstr, DWORD maxlen)
{ 
#ifdef _UNICODE
	DWORD dwLen = ::WideCharToMultiByte(CP_ACP,0,str,-1,NULL,0,NULL,NULL);
	if (dwLen < maxlen) dwLen = maxlen;
	::WideCharToMultiByte(CP_ACP, 0, str, -1, mstr, dwLen, NULL, NULL);
#else
	wcstombs(mstr, str, str.GetLength()+2);
#endif

    return;
}

//
//  �@�\     : �����񂪐������`�F�b�N
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL ChkNumber(CString str) 
{
	char mstr[1024];
	char c;
	UINT i;

#ifdef _UNICODE
	DWORD dwLen = ::WideCharToMultiByte(CP_ACP,0,str,-1,NULL,0,NULL,NULL);
	::WideCharToMultiByte(CP_ACP, 0, str, -1, mstr, dwLen, NULL, NULL);
#else
	wcstombs(mstr, str, str.GetLength()+2);
#endif

	for(i = 0; i < strlen(mstr); i++) {
		c = mstr[i];
		if (i == 0) {
			if (!((c >= '0' && c <= '9') || c == '-')) return FALSE;	// �擪����
		} else {
			if (!(c >= '0' && c <= '9')) return FALSE;
		}
	}
	if (i == 0) return FALSE;	// �󕶎��̏ꍇ

	return TRUE;
}

//
//  �@�\     : �����񂪎��R�����`�F�b�N
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL ChkNaturalNumber(CString str) 
{
	char mstr[1024];
	char c;
	UINT i;

#ifdef _UNICODE
	DWORD dwLen = ::WideCharToMultiByte(CP_ACP,0,str,-1,NULL,0,NULL,NULL);
	::WideCharToMultiByte(CP_ACP, 0, str, -1, mstr, dwLen, NULL, NULL);
#else
	wcstombs(mstr, str, str.GetLength()+2);
#endif

	for(i = 0; i < strlen(mstr); i++) {
		c = mstr[i];
		if (i == 0) {
			if (!(c >= '1' && c <= '9')) return FALSE;	// �擪����
		} else {
			if (!(c >= '0' && c <= '9')) return FALSE;
		}
	}
	if (i == 0) return FALSE;	// �󕶎��̏ꍇ

	return TRUE;
}

//
//  �@�\     : �����񂪃t�H���_���Ƃ��Ďg�p�\���`�F�b�N
//  
//  �@�\���� : �����̃s���I�h"."�͕s��
//  
//  �Ԃ�l   : �ϊ��㕶����
//  
//  ���l     : 
//  
CString ChkFolder(CString str) 
{
	CString strChar;
	UINT i;
	UINT pos;

	UINT intCnt = str.GetLength(); 
	for(i = 0; i < intCnt; i++) {
		pos = intCnt - i - 1;
		strChar = str.Mid(pos, 1);
		if (strChar == _T(".")) {
			str.Delete(pos);
			str.Insert(pos, _T("_"));
		} else {
			break;
		}
	}

	return str;
}

//
//  �@�\     : �����񂪃X�J�C�vID�Ƃ��Ďg�p�\���`�F�b�N
//  
//  �@�\���� : ���p�p�����������ƃA���_�X�R�A�A�n�C�t���̂ݗL��
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
BOOL ChkSkypeId(CString str) 
{
	char mstr[1024];
	char c;
	UINT i;

#ifdef _UNICODE
	DWORD dwLen = ::WideCharToMultiByte(CP_ACP,0,str,-1,NULL,0,NULL,NULL);
	::WideCharToMultiByte(CP_ACP, 0, str, -1, mstr, dwLen, NULL, NULL);
#else
	wcstombs(mstr, str, str.GetLength()+2);
#endif

	for(i = 0; i < strlen(mstr); i++) {
		c = mstr[i];
		if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '-' || c == '+' || c == '.' || c == ',')) return FALSE;
	}
	if (i == 0) return FALSE;	// �󕶎��̏ꍇ

	return TRUE;
}

