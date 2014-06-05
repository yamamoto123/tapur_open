// SkypeEvents.cpp

#include "stdafx.h"
#include "SkypeApi.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////

//
//  機能     : WaveFile再生
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : VoiceAccessAPI対応（Skype3.x以上）
//  
BOOL CSkypeApi::Play(ICallPtr pCall, CString strFile)
{
	_bstr_t bStrFile;
	_bstr_t bStrSoundcard;
	_bstr_t bStrPort;

	try {
		if (pCall == NULL) return FALSE;

		bStrFile = _bstr_t((LPCTSTR)strFile);
		bStrSoundcard = _bstr_t("");	// 入力デバイス（default or "" しか指定できない）
		bStrPort = _bstr_t(_T(""));		// 入力ポート

		pCall->SetInput(bStrSoundcard, bStrPort, bStrFile);

	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Play error:%s"), strFile);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : WaveFile再生停止
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : VoiceAccessAPI対応（Skype3.x以上）
//  
BOOL CSkypeApi::PlayStop(ICallPtr pCall)				
{
	_bstr_t bStrFile;
	_bstr_t bStrSoundcard;
	_bstr_t bStrPort;

	try {
		if (pCall == NULL) return FALSE;

		bStrFile = _bstr_t(_T(""));
		bStrSoundcard = _bstr_t(_T("default"));
		bStrPort = _bstr_t(_T(""));	

		pCall->SetInput(bStrSoundcard, bStrPort, bStrFile);

	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("PlayStop error"));
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 通話相手の音声録音
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : VoiceAccessAPI対応（Skype3.x以上）
//  
BOOL CSkypeApi::Record(ICallPtr pCall, CString strFile, CString strPort, CString strDev)	
{
	_bstr_t bStrFile;
	_bstr_t bStrSoundcard;
	_bstr_t bStrPort;

	try {
		if (pCall == NULL) return FALSE;

		m_strOutDev = strDev;
		bStrFile = _bstr_t((LPCTSTR)strFile);
		// "default"以外は無効（Skype3.5）
		//bStrSoundcard = _bstr_t((LPCTSTR)strDev);
		bStrSoundcard = _bstr_t(_T("default"));
		bStrPort = _bstr_t((LPCTSTR)strPort);

		pCall->SetOutput(bStrSoundcard, bStrPort, bStrFile);

	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("Record error:%s"), strFile);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : 通話相手の音声録音停止
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : VoiceAccessAPI対応（Skype3.x以上）
//  
BOOL CSkypeApi::RecordStop(ICallPtr pCall)				
{
	_bstr_t bStrFile;
	_bstr_t bStrSoundcard;
	_bstr_t bStrPort;

	try {
		if (pCall == NULL) return FALSE;

		bStrFile = _bstr_t(_T(""));
		// "default"以外は無効（Skype3.5）
		//bStrSoundcard = _bstr_t((LPCTSTR)m_strOutDev);
		bStrSoundcard = _bstr_t(_T("default"));
		bStrPort = _bstr_t(_T(""));	

		pCall->SetOutput(bStrSoundcard, bStrPort, bStrFile);

	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordStop error"));
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : マイクの音声録音
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : VoiceAccessAPI対応（Skype3.x以上）
//  
BOOL CSkypeApi::RecordMic(ICallPtr pCall, CString strFile, CString strPort)	
{
	_bstr_t bStrFile;
	_bstr_t bStrPort;

	try {
		if (pCall == NULL) return FALSE;

		bStrFile = _bstr_t((LPCTSTR)strFile);
		bStrPort = _bstr_t((LPCTSTR)strPort);	

		pCall->SetCaptureMic(bStrPort, bStrFile);

	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordMic error:%s"), strFile);
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

//
//  機能     : マイクの音声録音停止
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : VoiceAccessAPI対応（Skype3.x以上）
//  
BOOL CSkypeApi::RecordStopMic(ICallPtr pCall)
{
	_bstr_t bStrFile;
	_bstr_t bStrPort;

	try {
		if (pCall == NULL) return FALSE;

		bStrFile = _bstr_t(_T(""));
		bStrPort = _bstr_t(_T(""));	

		pCall->SetCaptureMic(bStrPort, bStrFile);

	} catch(_com_error& e) {
		if (e.ErrorInfo()) {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.Description());
		} else {
			_stprintf_s(te.errstr, MAX_ERRSTR, _T("COM error:%s"), e.ErrorMessage());
		}
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;   
	} catch(...) {
		_stprintf_s(te.errstr, MAX_ERRSTR, _T("RecordStopMic error"));
		te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	return TRUE;
}

