// CallStatus.cpp: CCallStatus �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tapur.h"
#include "CallStatus.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CCallStatus::CCallStatus()
{
	/// Call ���
	lngCallId = 0;
	strPartnerId = _T("");
	strPartnerName = _T("");	
	blSenderCall = FALSE;

	// User���̐ݒ�
	strPartnerFullName = _T("");
	lngBirthday = 0;
	lngSex = 0;
	strCountry = _T("");
	strCountryCode = _T("");
	strProvince = _T("");
	strCity = _T("");
	strPhoneHome = _T("");
	strPhoneOffice = _T("");
	strPhoneMobile = _T("");
	strHomepage = _T("");
	strAbout = _T("");
	strMoodText = _T("");
    lngTimezone = 0;
}

CCallStatus::~CCallStatus()
{

}
	
// Call���̐ݒ�
BOOL CCallStatus::SetCall(ICallPtr pCall)
{
	CString str;
	_bstr_t bStr;

	if (pCall == NULL) return FALSE;

	// �ʘbID
	lngCallId = pCall->GetID();
	// �ʘb����(�X�J�C�vID)			
	bStr = pCall->PartnerHandle;
	strPartnerId.Format(_T("%s"), (LPCTSTR)bStr);
	// �ʘb����(�X�J�C�v��)
	bStr = pCall->PartnerDisplayName;
	strPartnerName.Format(_T("%s"), (LPCTSTR)bStr);	
	// ���[�U���M�t���O
	SkypeCallType type = pCall->GetType();
	blSenderCall = (type == ctypIncomingPSTN || type == ctypIncomingP2P) ? FALSE : TRUE;

	return TRUE;
}

// User���̐ݒ�
BOOL CCallStatus::SetUser(IUserPtr pUser)
{
	CString str;
	_bstr_t bStr;

	if (pUser == NULL) return FALSE;

	// �ʘb����(�t����)
    bStr = pUser->GetFullName();
	strPartnerFullName.Format(_T("%s"), (LPCTSTR)bStr);
	// �a����
    DATE dtDate = pUser->GetBirthdate();
	COleDateTime oDate(dtDate);
	long lngYear = (long)oDate.GetYear();
	long lngMonth = (long)oDate.GetMonth();
	long lngDay = (long)oDate.GetDay();
	lngBirthday = (lngYear * 1000) + (lngMonth * 100) + lngDay;
	// ���ʁiusexUnknown = -1, usexNotSpecified = 0, usexMale = 1, usexFemale = 2)
    SkypeUserSex oSex = pUser->GetSex();
	lngSex = (long)oSex;
	// ����
	bStr = pUser->GetLanguage();
	str.Format(_T("%s"), (LPCTSTR)bStr);
	if (!str.IsEmpty()) {
		strLanguageCode = str.Left(2);
		strLanguage = str.Right(str.GetLength() - 3);
	}
	// �Z���֘A
    bStr = pUser->GetCountry();
	str.Format(_T("%s"), (LPCTSTR)bStr);
	if (!str.IsEmpty()) {
		strCountryCode = str.Left(2);
		strCountry = str.Right(str.GetLength() - 3);
	}
    bStr = pUser->GetProvince();
	strProvince.Format(_T("%s"), (LPCTSTR)bStr);
    bStr = pUser->GetCity();
	strCity.Format(_T("%s"), (LPCTSTR)bStr);
    bStr = pUser->GetPhone_Home();
	// �d�b�֘A
	strPhoneHome.Format(_T("%s"), (LPCTSTR)bStr);
    bStr = pUser->GetPhone_Office();
	strPhoneOffice.Format(_T("%s"), (LPCTSTR)bStr);
    bStr = pUser->GetPhone_Mobile();
	strPhoneMobile.Format(_T("%s"), (LPCTSTR)bStr);
    bStr = pUser->GetHomepage();
	// HP
	strHomepage.Format(_T("%s"), (LPCTSTR)bStr);
    bStr = pUser->GetAbout();
	// ���ȏЉ�
	strAbout.Format(_T("%s"), (LPCTSTR)bStr);
    bStr = pUser->GetMoodText();
	// MoodText
	strMoodText.Format(_T("%s"), (LPCTSTR)bStr);
    lngTimezone = pUser->GetTimezone();

	return TRUE;
}

