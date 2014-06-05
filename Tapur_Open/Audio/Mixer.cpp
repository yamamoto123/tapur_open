// CMixer.cpp 
//

#include "stdafx.h"
#include <limits.h>				// ULONG_MAX
#include "Mixer.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


CMixer::CMixer()
{
	if (!(this->Initialize())) {
		ASSERT(FALSE);
	}
}

CMixer::~CMixer() 
{
	VERIFY(this->Uninitialize());
}

//
//  機能     : Mixerデバイスのオープン
//  
//  機能説明 : 最初のものを使用
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CMixer::Initialize()
{
	// get the number of mixer devices present in the system
	m_nNumMixers = ::mixerGetNumDevs();

	m_hMixer = NULL;
	::ZeroMemory(&m_mxcaps, sizeof(MIXERCAPS));

	m_strDstLineName.Empty();
	m_strSelectControlName.Empty();
	m_strSrcName.Empty();
	m_dwChannels = 0;
	m_dwControlType = 0;
	m_dwSelectControlID = 0;
	m_dwMultipleItems = 0;
	m_dwIndex = ULONG_MAX;

	// open the first mixer
	// A "mapper" for audio mixer devices does not currently exist.
	if (m_nNumMixers != 0)
	{
		if (::mixerOpen(&m_hMixer,
						0,
						0,
//						reinterpret_cast<DWORD>(this->GetSafeHwnd()),
						NULL,
						MIXER_OBJECTF_MIXER | CALLBACK_WINDOW)
			!= MMSYSERR_NOERROR)
		{
			return FALSE;
		}

		if (::mixerGetDevCaps(reinterpret_cast<UINT>(m_hMixer),
							  &m_mxcaps, sizeof(MIXERCAPS))
			!= MMSYSERR_NOERROR)
		{
			return FALSE;
		}
	}
TRACE(_T("Mixer Device Name:%s\n"), m_mxcaps.szPname);
	return TRUE;
}

//
//  機能     : Mixerデバイスのクローズ
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CMixer::Uninitialize()
{
	BOOL bSucc = TRUE;

	if (m_hMixer != NULL)
	{
		bSucc = (::mixerClose(m_hMixer) == MMSYSERR_NOERROR);
		m_hMixer = NULL;
	}

	return bSucc;
}

//
//  機能     : コントロールの選択
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : 
//  
BOOL CMixer::FindControl(
DWORD DstType,		// ディスティネーションライン（ex, MIXERLINE_COMPONENTTYPE_DST_WAVEIN)
DWORD SrcType,		// ソースライン（ex, MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE)
DWORD CntType)		// コントロールタイプ(ex,MIXERCONTROL_CONTROLTYPE_MIXER)
{
	if (m_hMixer == NULL) return FALSE;

_stprintf_s(te.errstr, MAX_ERRSTR, _T("Mixer FindControl:%d,%d,%d"), DstType,SrcType,CntType);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);

	// get dwLineID
	MIXERLINE mxl;
	::ZeroMemory(&mxl, sizeof(MIXERLINE));
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = DstType;
	if (::mixerGetLineInfo(reinterpret_cast<HMIXEROBJ>(m_hMixer),
						   &mxl,
						   MIXER_OBJECTF_HMIXER |
						   MIXER_GETLINEINFOF_COMPONENTTYPE)
		!= MMSYSERR_NOERROR)
	{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("Mixer Error:mixerGetLineInfo"));
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
		return FALSE;
	}

	// get dwControlID
	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	::ZeroMemory(&mxc, sizeof(MIXERCONTROL));
	::ZeroMemory(&mxlc, sizeof(MIXERLINECONTROLS));
	m_dwControlType = CntType;
	mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID = mxl.dwLineID;
	mxlc.dwControlType = m_dwControlType;
	mxlc.cControls = 1;
	mxlc.cbmxctrl = sizeof(MIXERCONTROL);
	mxlc.pamxctrl = &mxc;
	if (::mixerGetLineControls(reinterpret_cast<HMIXEROBJ>(m_hMixer),
							   &mxlc,
							   MIXER_OBJECTF_HMIXER |
							   MIXER_GETLINECONTROLSF_ONEBYTYPE)
		!= MMSYSERR_NOERROR)
	{
		// no mixer, try MUX
		m_dwControlType = MIXERCONTROL_CONTROLTYPE_MUX;
		mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
		mxlc.dwLineID = mxl.dwLineID;
		mxlc.dwControlType = m_dwControlType;
		mxlc.cControls = 1;
		mxlc.cbmxctrl = sizeof(MIXERCONTROL);
		mxlc.pamxctrl = &mxc;
		if (::mixerGetLineControls(reinterpret_cast<HMIXEROBJ>(m_hMixer),
								   &mxlc,
								   MIXER_OBJECTF_HMIXER |
								   MIXER_GETLINECONTROLSF_ONEBYTYPE)
			!= MMSYSERR_NOERROR)
		{
			return FALSE;
		}
	}

	// store dwControlID, cMultipleItems
	m_dwChannels = mxl.cChannels;
	m_strDstLineName = mxl.szName;
	m_fdwControl = mxc.fdwControl;
	m_strSelectControlName = mxc.szName;
	m_dwSelectControlID = mxc.dwControlID;
	m_dwMultipleItems = mxc.cMultipleItems;
	if (m_dwMultipleItems == 0) return TRUE;	// no multi items

	// get the index of the Select control
	m_dwIndex = 999;
	DWORD dwSize = (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) ? 
		(m_dwChannels * m_dwMultipleItems) : m_dwChannels;
	MIXERCONTROLDETAILS_LISTTEXT *pmxcdSelectText = new MIXERCONTROLDETAILS_LISTTEXT[dwSize];
	if (pmxcdSelectText == NULL) return FALSE;
	::ZeroMemory(pmxcdSelectText, sizeof(MIXERCONTROLDETAILS_LISTTEXT) * dwSize);

	MIXERCONTROLDETAILS mxcd;
	::ZeroMemory(&mxcd, sizeof(MIXERCONTROLDETAILS));
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_dwSelectControlID;
	mxcd.cChannels = m_dwChannels;
	if (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) {
		mxcd.cMultipleItems = m_dwMultipleItems;
	}
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_LISTTEXT);
	mxcd.paDetails = pmxcdSelectText;
	MMRESULT mResult = ::mixerGetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer),
								 &mxcd,
								 MIXER_OBJECTF_HMIXER |
								 MIXER_GETCONTROLDETAILSF_LISTTEXT);
	if (mResult == MMSYSERR_NOERROR) {
		for (DWORD dwi = 0; dwi < m_dwMultipleItems; dwi++)
		{
_stprintf_s(te.errstr, MAX_ERRSTR, _T("Mixer List:type=%d, name=%s\n"), pmxcdSelectText[dwi].dwParam2, pmxcdSelectText[dwi].szName);
te.ErrorMessage(ERRTYPE_LOGONLY, _T(__FILE__), __LINE__, ERR_SKYPE, te.errstr);
			if (pmxcdSelectText[dwi].dwParam2 == SrcType) {
				// found, dwi is the index.
				m_dwIndex = dwi;
				m_strSrcName = pmxcdSelectText[dwi].szName;
				break;
			}
		}
	}

	delete []pmxcdSelectText;

	if (m_dwIndex > m_dwMultipleItems) return FALSE;
	return TRUE;
}

BOOL CMixer::IsSelect() 
{
	if (m_hMixer == NULL ||
		m_dwMultipleItems == 0 ||
		m_dwIndex >= m_dwMultipleItems)
	{
		ASSERT(FALSE);
	}

	DWORD dwSize = (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) ? 
		(m_dwChannels * m_dwMultipleItems) : m_dwChannels;
	MIXERCONTROLDETAILS_BOOLEAN *pmxcdSelectValue = new MIXERCONTROLDETAILS_BOOLEAN[dwSize];
	if (pmxcdSelectValue == NULL) return FALSE;
	::ZeroMemory(pmxcdSelectValue, sizeof(MIXERCONTROLDETAILS_BOOLEAN) * dwSize);

	BOOL bRetVal = FALSE;
	MIXERCONTROLDETAILS mxcd;
	::ZeroMemory(&mxcd, sizeof(MIXERCONTROLDETAILS));
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_dwSelectControlID;
	mxcd.cChannels = m_dwChannels;
	if (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) {
		mxcd.cMultipleItems = m_dwMultipleItems;
	}
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
	mxcd.paDetails = pmxcdSelectValue;
	if (::mixerGetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer),
								 &mxcd,
								 MIXER_OBJECTF_HMIXER |
								 MIXER_GETCONTROLDETAILSF_VALUE)
		== MMSYSERR_NOERROR)
	{
		bRetVal = (pmxcdSelectValue[m_dwIndex].fValue > 0) ? TRUE : FALSE;
	}

	delete []pmxcdSelectValue;

	return bRetVal;
}

BOOL CMixer::SetSelect(BOOL bVal) 
{
	if (m_hMixer == NULL ||
		m_dwMultipleItems == 0 ||
		m_dwIndex >= m_dwMultipleItems)
	{
		ASSERT(FALSE);
	}


	// get all the values first
	DWORD dwSize = (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) ? 
		(m_dwChannels * m_dwMultipleItems) : m_dwChannels;
	MIXERCONTROLDETAILS_BOOLEAN *pmxcdSelectValue = new MIXERCONTROLDETAILS_BOOLEAN[dwSize];
	if (pmxcdSelectValue == NULL) return FALSE;
	::ZeroMemory(pmxcdSelectValue, sizeof(MIXERCONTROLDETAILS_BOOLEAN) * dwSize);

	BOOL bRetVal = FALSE;
	MIXERCONTROLDETAILS mxcd;
	::ZeroMemory(&mxcd, sizeof(MIXERCONTROLDETAILS));
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_dwSelectControlID;
	mxcd.cChannels = m_dwChannels;
	if (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) {
		mxcd.cMultipleItems = m_dwMultipleItems;
	}
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
	mxcd.paDetails = pmxcdSelectValue;
	if (::mixerGetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer),
								 &mxcd,
								 MIXER_OBJECTF_HMIXER |
								 MIXER_GETCONTROLDETAILSF_VALUE)
		== MMSYSERR_NOERROR)
	{
		ASSERT(m_dwControlType == MIXERCONTROL_CONTROLTYPE_MIXER ||
			   m_dwControlType == MIXERCONTROL_CONTROLTYPE_MUX);

		// MUX restricts the line selection to one source line at a time.
		if (bVal == TRUE && m_dwControlType == MIXERCONTROL_CONTROLTYPE_MUX)
		{
			::ZeroMemory(pmxcdSelectValue,
						 m_dwMultipleItems * sizeof(MIXERCONTROLDETAILS_BOOLEAN));
		}

		// set the Control value
		pmxcdSelectValue[m_dwIndex].fValue = bVal;

		mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mxcd.dwControlID = m_dwSelectControlID;
		mxcd.cChannels = m_dwChannels;
		if (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) {
			mxcd.cMultipleItems = m_dwMultipleItems;
		}
		mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
		mxcd.paDetails = pmxcdSelectValue;
		if (::mixerSetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer),
									 &mxcd,
									 MIXER_OBJECTF_HMIXER |
									 MIXER_SETCONTROLDETAILSF_VALUE)
			== MMSYSERR_NOERROR)
		{
			bRetVal = TRUE;
		}
	}

	delete []pmxcdSelectValue;

	return bRetVal;
}

/*
DWORD CMixer::GetControlValue()
{
	if (m_hMixer == NULL) return 0;

	MIXERCONTROLDETAILS mxcd;
    mxcd.cbStruct = sizeof(mxcd);
	::ZeroMemory(&mxcd, sizeof(MIXERCONTROLDETAILS));

	DWORD dwSize = (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) ? 
		(m_dwChannels * m_dwMultipleItems) : m_dwChannels;
	MIXERCONTROLDETAILS_UNSIGNED *pmxcd_u = new MIXERCONTROLDETAILS_UNSIGNED[dwSize];
	if (pmxcd_u == NULL) return FALSE;
	::ZeroMemory(pmxcd_u, sizeof(MIXERCONTROLDETAILS_UNSIGNED) * dwSize);

	mxcd.dwControlID = m_dwSelectControlID;
	mxcd.cChannels = m_dwChannels;
	if (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) {
		mxcd.cMultipleItems = m_dwMultipleItems;
	}
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails = pmxcd_u;
	if (mixerGetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, 0L) != MMSYSERR_NOERROR) return 0;

	delete []pmxcd_u;

    return mxcd_u.dwValue;
}

BOOL CMixer::SetControlValue(DWORD dw)
{
	BOOL bSuccess = FALSE;

	if (m_hMixer == NULL) return bSuccess;

	MIXERCONTROLDETAILS mxcd;
    mxcd.cbStruct = sizeof(mxcd);
	::ZeroMemory(&mxcd, sizeof(MIXERCONTROLDETAILS));

	DWORD dwSize = (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) ? 
		(m_dwChannels * m_dwMultipleItems) : m_dwChannels;
	MIXERCONTROLDETAILS_UNSIGNED *pmxcd_u = new MIXERCONTROLDETAILS_UNSIGNED[dwSize];
	if (pmxcd_u == NULL) return FALSE;
	::ZeroMemory(pmxcd_u, sizeof(MIXERCONTROLDETAILS_UNSIGNED) * dwSize);
 
    mxcd.cbStruct = sizeof(mxcd);
	mxcd.dwControlID = m_dwSelectControlID;
	mxcd.cChannels = m_dwChannels;
	if (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) {
		mxcd.cMultipleItems = m_dwMultipleItems;
	}
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails = pmxcd_u;
	if (mixerGetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, 0L) != MMSYSERR_NOERROR) return bSuccess;
	mxcd_u.dwValue = dw;
	if (mixerSetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, 0L) != MMSYSERR_NOERROR) return bSuccess;

	delete []pmxcd_u;

    return TRUE;
}
*/

BOOL CMixer::SetControlValue(BOOL dw)
{
	if (m_hMixer == NULL) return FALSE;

	MIXERCONTROLDETAILS mxcd;
	::ZeroMemory(&mxcd, sizeof(MIXERCONTROLDETAILS));

	DWORD dwSize = (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) ? 
		(m_dwChannels * m_dwMultipleItems) : m_dwChannels;
	MIXERCONTROLDETAILS_BOOLEAN *pmxcd_b = new MIXERCONTROLDETAILS_BOOLEAN[dwSize];
	if (pmxcd_b == NULL) return FALSE;
	::ZeroMemory(pmxcd_b, sizeof(MIXERCONTROLDETAILS_BOOLEAN) * dwSize);

	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_dwSelectControlID;
	mxcd.cChannels = 1;
	if (m_fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) {
		mxcd.cMultipleItems = m_dwMultipleItems;
	}
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
	mxcd.paDetails = pmxcd_b;
	if (dw) {
		pmxcd_b->fValue = 1;
	} else {
		pmxcd_b->fValue = 0;
	}
	if (::mixerSetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer),
								 &mxcd,
								 MIXER_OBJECTF_HMIXER |
								 MIXER_SETCONTROLDETAILSF_VALUE)
		!= MMSYSERR_NOERROR)
	{
		return FALSE;
	}

	delete []pmxcd_b;

	return TRUE;
}

