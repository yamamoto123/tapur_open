// SkypeApi.h

#ifndef __SKYPEAPI_H__
#define __SKYPEAPI_H__

#include "TPerror.h"
//#include "TPsetup.h"
//#include "CallStatus.h"


class CSkypeApi
{
private:
	CTPerror te;
	CString m_strOutDev;	// 音声出力デバイス

public:
	//CSkypeApi();
	//~CSkypeApi();

// VoiceAccessAPI対応（Skype3.x以上）
	BOOL Play(ICallPtr pCall, CString strFile);		// WaveFile再生
	BOOL PlayStop(ICallPtr pCall);					// WaveFile再生停止
	BOOL Record(ICallPtr pCall, CString strFile, CString strPort = _T(""), CString strDev = _T("default"));		// 通話相手の音声録音
	BOOL RecordStop(ICallPtr pCall);					// 通話相手の音声録音停止
	BOOL RecordMic(ICallPtr pCall, CString strFile, CString strPort = _T(""));	// マイクの音声録音
	BOOL RecordStopMic(ICallPtr pCall);				// マイクの音声録音停止
};


#endif	// __SKYPEAPI_H__

