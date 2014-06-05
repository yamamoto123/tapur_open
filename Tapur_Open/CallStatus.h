// CallStatus.h: CCallStatus クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALLSTATUS_H__CA3CF4BD_6792_4C0F_BF5A_0F659DF657A1__INCLUDED_)
#define AFX_CALLSTATUS_H__CA3CF4BD_6792_4C0F_BF5A_0F659DF657A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Skype Call状態クラス
class CCallStatus  
{
public:
	CCallStatus();
	virtual ~CCallStatus();

	BOOL SetCall(ICallPtr pCall);
	BOOL SetUser(IUserPtr pUser);

	/// Call 情報
	long lngCallId;			// 通話ID
	CString strUsrId;		// ユーザ(スカイプID)
	CString strUsrName;		// ユーザ(スカイプ名)
	CString strPartnerId;	// 通話相手(スカイプID)
	CString strPartnerName;	// 通話相手(スカイプ名)
	BOOL blSenderCall;		// ユーザ発信フラグ

	/// User 情報
	CString strPartnerFullName;	// 通話相手(フル名)
	long lngBirthday;		// 誕生日
	long lngSex;			// 性別（usexUnknown = -1, usexNotSpecified = 0, usexMale = 1, usexFemale = 2)
	CString strLanguage;	// 言語
	CString strLanguageCode;// 言語コード
	CString strCountry;		// 住所（国）
	CString strCountryCode;	// 住所（国コード）
	CString strProvince;	// 住所（州/県）
	CString strCity;		// 住所（市）
	CString strPhoneHome;	// 電話（自宅）
	CString strPhoneOffice;	// 電話（オフィス）
	CString strPhoneMobile;	// 電話（モバイル）
	CString strHomepage;	// HP
	CString strAbout;		// 自己紹介
	CString strMoodText;	// MoodText
    long lngTimezone;		// Timezone
};


#endif // !defined(AFX_CALLSTATUS_H__CA3CF4BD_6792_4C0F_BF5A_0F659DF657A1__INCLUDED_)
