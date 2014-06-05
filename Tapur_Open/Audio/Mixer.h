// Mixer.h 
//

#ifndef __CMixer_H__
#define __CMixer_H__

#include <mmsystem.h>
#include "TPerror.h"


class CMixer
{
// Construction
public:
	CMixer();	
	~CMixer();	

// Implementation
private:
	CTPerror te;
	UINT m_nNumMixers;
	HMIXER m_hMixer;
	MIXERCAPS m_mxcaps;

	CString m_strDstLineName;
	CString m_strSelectControlName;
	CString m_strSrcName;
	DWORD m_fdwControl;
	DWORD m_dwChannels;
	DWORD m_dwControlType;
	DWORD m_dwSelectControlID;
	DWORD m_dwMultipleItems;
	DWORD m_dwIndex;

public:
	BOOL Uninitialize();
	BOOL Initialize();
	BOOL FindControl(DWORD DstType, DWORD SrcType, DWORD CntType);
	BOOL IsSelect();
	BOOL SetSelect(BOOL bVal);
//	DWORD GetControlValue();
//	BOOL SetControlValue(DWORD dw);
	BOOL SetControlValue(BOOL dw);
	CString GetDeviceName() { return CString(m_mxcaps.szPname); }
};

#endif // __CMixer_H__
