// TPutil.h: utility tools.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_TPUTIL_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
#define _TPUTIL_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern int GetDateId(_TCHAR *date_id);
extern int CnvDateString(const _TCHAR *date_id, _TCHAR *date_str);
extern void CnvBSTR_CString(BSTR bStr, CString* pCStr);
extern void CnvCString_Bytes(CString str, char *mstr, DWORD maxlen);
extern BOOL ChkNumber(CString str);
extern BOOL ChkNaturalNumber(CString str);
extern CString ChkFolder(CString str); 
extern BOOL ChkSkypeId(CString str);

#endif // !defined(_TPUTIL_H__7C0BA2B7_B95D_46B2_A3B9_40AFACAD18D1__INCLUDED_)
