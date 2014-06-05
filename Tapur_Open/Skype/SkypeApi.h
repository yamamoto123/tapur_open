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
	CString m_strOutDev;	// �����o�̓f�o�C�X

public:
	//CSkypeApi();
	//~CSkypeApi();

// VoiceAccessAPI�Ή��iSkype3.x�ȏ�j
	BOOL Play(ICallPtr pCall, CString strFile);		// WaveFile�Đ�
	BOOL PlayStop(ICallPtr pCall);					// WaveFile�Đ���~
	BOOL Record(ICallPtr pCall, CString strFile, CString strPort = _T(""), CString strDev = _T("default"));		// �ʘb����̉����^��
	BOOL RecordStop(ICallPtr pCall);					// �ʘb����̉����^����~
	BOOL RecordMic(ICallPtr pCall, CString strFile, CString strPort = _T(""));	// �}�C�N�̉����^��
	BOOL RecordStopMic(ICallPtr pCall);				// �}�C�N�̉����^����~
};


#endif	// __SKYPEAPI_H__

