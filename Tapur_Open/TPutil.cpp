
#include "stdafx.h"
#include <time.h>

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


//
//  機能     : 日時をロング整数形式の文字列で生成
//  
//  機能説明 : ex) "200501010100"
//  
//  返り値   : 0:ok -1:error
//  
//  備考     : 
//  
int GetDateId(
	_TCHAR *date_id)	// 生成文字列の保存バッファ
{
	time_t ct = time(NULL);
	struct tm lst;
	localtime_s(&lst, &ct);

	_stprintf_s(date_id, 15, _T("%04d%02d%02d%02d%02d%02d"), (1900+lst.tm_year), (1+lst.tm_mon),
		lst.tm_mday, lst.tm_hour, lst.tm_min, lst.tm_sec);

	return 0;
}

//
//  機能     : ロング整数形式の文字列を整形
//  
//  機能説明 : ex) "2005/01/01/ 01:00"
//  
//  返り値   : 0:ok -1:error
//  
//  備考     : 
//  
int CnvDateString(
const _TCHAR *date_id, 	// ロング整数形式の文字列
_TCHAR *date_str)	// 生成文字列の保存バッファ
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
//  機能     : BSTRからCStringへの型変換
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
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
//  機能     : CStringからMultiByteCharacterへの型変換
//  
//  機能説明 : 出力バッファは呼び出し側で確保
//  
//  返り値   : 
//  
//  備考     : 
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
//  機能     : 文字列が整数かチェック
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
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
			if (!((c >= '0' && c <= '9') || c == '-')) return FALSE;	// 先頭文字
		} else {
			if (!(c >= '0' && c <= '9')) return FALSE;
		}
	}
	if (i == 0) return FALSE;	// 空文字の場合

	return TRUE;
}

//
//  機能     : 文字列が自然数かチェック
//  
//  機能説明 : 
//  
//  返り値   : BOOL
//  
//  備考     : 
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
			if (!(c >= '1' && c <= '9')) return FALSE;	// 先頭文字
		} else {
			if (!(c >= '0' && c <= '9')) return FALSE;
		}
	}
	if (i == 0) return FALSE;	// 空文字の場合

	return TRUE;
}

//
//  機能     : 文字列がフォルダ名として使用可能かチェック
//  
//  機能説明 : 末尾のピリオド"."は不可
//  
//  返り値   : 変換後文字列
//  
//  備考     : 
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
//  機能     : 文字列がスカイプIDとして使用可能かチェック
//  
//  機能説明 : 半角英数字小文字とアンダスコア、ハイフンのみ有効
//  
//  返り値   : BOOL
//  
//  備考     : 
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
	if (i == 0) return FALSE;	// 空文字の場合

	return TRUE;
}

