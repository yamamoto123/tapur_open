// SkypeEvents.cpp

#include "stdafx.h"
#include "SkypeApi.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////

//
//  �@�\     : WaveFile�Đ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : VoiceAccessAPI�Ή��iSkype3.x�ȏ�j
//  
BOOL CSkypeApi::Play(ICallPtr pCall, CString strFile)
{
	_bstr_t bStrFile;
	_bstr_t bStrSoundcard;
	_bstr_t bStrPort;

	try {
		if (pCall == NULL) return FALSE;

		bStrFile = _bstr_t((LPCTSTR)strFile);
		bStrSoundcard = _bstr_t("");	// ���̓f�o�C�X�idefault or "" �����w��ł��Ȃ��j
		bStrPort = _bstr_t(_T(""));		// ���̓|�[�g

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
//  �@�\     : WaveFile�Đ���~
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : VoiceAccessAPI�Ή��iSkype3.x�ȏ�j
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
//  �@�\     : �ʘb����̉����^��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : VoiceAccessAPI�Ή��iSkype3.x�ȏ�j
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
		// "default"�ȊO�͖����iSkype3.5�j
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
//  �@�\     : �ʘb����̉����^����~
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : VoiceAccessAPI�Ή��iSkype3.x�ȏ�j
//  
BOOL CSkypeApi::RecordStop(ICallPtr pCall)				
{
	_bstr_t bStrFile;
	_bstr_t bStrSoundcard;
	_bstr_t bStrPort;

	try {
		if (pCall == NULL) return FALSE;

		bStrFile = _bstr_t(_T(""));
		// "default"�ȊO�͖����iSkype3.5�j
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
//  �@�\     : �}�C�N�̉����^��
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : VoiceAccessAPI�Ή��iSkype3.x�ȏ�j
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
//  �@�\     : �}�C�N�̉����^����~
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : VoiceAccessAPI�Ή��iSkype3.x�ȏ�j
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

