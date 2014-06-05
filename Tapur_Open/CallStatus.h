// CallStatus.h: CCallStatus �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALLSTATUS_H__CA3CF4BD_6792_4C0F_BF5A_0F659DF657A1__INCLUDED_)
#define AFX_CALLSTATUS_H__CA3CF4BD_6792_4C0F_BF5A_0F659DF657A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Skype Call��ԃN���X
class CCallStatus  
{
public:
	CCallStatus();
	virtual ~CCallStatus();

	BOOL SetCall(ICallPtr pCall);
	BOOL SetUser(IUserPtr pUser);

	/// Call ���
	long lngCallId;			// �ʘbID
	CString strUsrId;		// ���[�U(�X�J�C�vID)
	CString strUsrName;		// ���[�U(�X�J�C�v��)
	CString strPartnerId;	// �ʘb����(�X�J�C�vID)
	CString strPartnerName;	// �ʘb����(�X�J�C�v��)
	BOOL blSenderCall;		// ���[�U���M�t���O

	/// User ���
	CString strPartnerFullName;	// �ʘb����(�t����)
	long lngBirthday;		// �a����
	long lngSex;			// ���ʁiusexUnknown = -1, usexNotSpecified = 0, usexMale = 1, usexFemale = 2)
	CString strLanguage;	// ����
	CString strLanguageCode;// ����R�[�h
	CString strCountry;		// �Z���i���j
	CString strCountryCode;	// �Z���i���R�[�h�j
	CString strProvince;	// �Z���i�B/���j
	CString strCity;		// �Z���i�s�j
	CString strPhoneHome;	// �d�b�i����j
	CString strPhoneOffice;	// �d�b�i�I�t�B�X�j
	CString strPhoneMobile;	// �d�b�i���o�C���j
	CString strHomepage;	// HP
	CString strAbout;		// ���ȏЉ�
	CString strMoodText;	// MoodText
    long lngTimezone;		// Timezone
};


#endif // !defined(AFX_CALLSTATUS_H__CA3CF4BD_6792_4C0F_BF5A_0F659DF657A1__INCLUDED_)
