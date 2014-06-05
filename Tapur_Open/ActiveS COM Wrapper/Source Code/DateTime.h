/*
Copyright (c) 2005, Jason Terando
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the parties distributing the library nor the names of its contributors may be
      used to endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
//////////////////////////////////////////////////////////////////////
// DateTime.h: interface for the CDateTime class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <comdef.h>
#include <time.h>
#include <math.h>

class CDateTime;
class CDateTimeSpan;
/////////////////////////////////////////////////////////////////////////////
// CDateTime class helpers

#define AFX_OLE_DATETIME_ERROR (-1)

BOOL OleDateFromTm(WORD wYear, WORD wMonth, WORD wDay,
 WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest);
BOOL TmFromOleDate(DATE dtSrc, struct tm& tmDest);
void TmConvertToStandardFormat(struct tm& tmSrc);
double DoubleFromDate(DATE dt);
DATE DateFromDouble(double dbl);

/////////////////////////////////////////////////////////////////////////////
// CDateTime class

class CDateTime
{
// Constructors
public:
 static CDateTime PASCAL GetCurrentTime();

 CDateTime();

 CDateTime(const CDateTime& dateSrc);
 CDateTime(const VARIANT& varSrc);
 CDateTime(DATE dtSrc);

 CDateTime(time_t& timeSrc);
 CDateTime(const SYSTEMTIME& systimeSrc);
 CDateTime(const FILETIME& filetimeSrc);

 CDateTime(int nYear, int nMonth, int nDay,
  int nHour, int nMin, int nSec);
 CDateTime(WORD wDosDate, WORD wDosTime);

// Attributes
public:
 enum DateTimeStatus
 {
  valid = 0,
  invalid = 1,    // Invalid date (out of range, etc.)
  null = 2,       // Literally has no value
 };

 DATE m_dt;
 DateTimeStatus m_status;

 void SetStatus(DateTimeStatus status) { m_status=status;}
 DateTimeStatus GetStatus() const { return m_status;}

 int GetYear() const;
 int GetMonth() const;       // month of year (1 = Jan)
 int GetDay() const;         // day of month (0-31)
 int GetHour() const;        // hour in day (0-23)
 int GetMinute() const;      // minute in hour (0-59)
 int GetSecond() const;      // second in minute (0-59)
 int GetDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat
 int GetDayOfYear() const;   // days since start of year, Jan 1 = 1

// Operations
public:
 const CDateTime& operator=(const CDateTime& dateSrc);
 const CDateTime& operator=(const VARIANT& varSrc);
 const CDateTime& operator=(DATE dtSrc);

 const CDateTime& operator=(const time_t& timeSrc);
 const CDateTime& operator=(const SYSTEMTIME& systimeSrc);
 const CDateTime& operator=(const FILETIME& filetimeSrc);

 BOOL operator==(const CDateTime& date) const;
 BOOL operator!=(const CDateTime& date) const;
 BOOL operator<(const CDateTime& date) const;
 BOOL operator>(const CDateTime& date) const;
 BOOL operator<=(const CDateTime& date) const;
 BOOL operator>=(const CDateTime& date) const;

 // DateTime math
 CDateTime operator+(const CDateTimeSpan& dateSpan) const;
 CDateTime operator-(const CDateTimeSpan& dateSpan) const;
 const CDateTime& operator+=(const CDateTimeSpan dateSpan);
 const CDateTime& operator-=(const CDateTimeSpan dateSpan);

 // DateTimeSpan math
 CDateTimeSpan operator-(const CDateTime& date) const;

 operator DATE() const;

 int SetDateTime(int nYear, int nMonth, int nDay,
  int nHour, int nMin, int nSec);
 int SetDate(int nYear, int nMonth, int nDay);
 int SetTime(int nHour, int nMin, int nSec);
 BOOL ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags = 0,
  LCID lcid = LANG_USER_DEFAULT);

 // formatting
 _bstr_t Format(DWORD dwFlags = 0, LCID lcid = LANG_USER_DEFAULT) const;
 _bstr_t Format(LPCTSTR lpszFormat) const;
 //CComString Format(UINT nFormatID) const;

// Implementation
protected:
 void CheckRange();
 friend CDateTimeSpan;
};



/////////////////////////////////////////////////////////////////////////////
// CDateTimeSpan class
class CDateTimeSpan
{
// Constructors
public:
 CDateTimeSpan();

 CDateTimeSpan(double dblSpanSrc);
 CDateTimeSpan(const CDateTimeSpan& dateSpanSrc);
 CDateTimeSpan(long lDays, int nHours, int nMins, int nSecs);

// Attributes
public:
 enum DateTimeSpanStatus
 {
  valid = 0,
  invalid = 1,    // Invalid span (out of range, etc.)
  null = 2,       // Literally has no value
 };

 double m_span;
 DateTimeSpanStatus m_status;

 void SetStatus(DateTimeSpanStatus status) { m_status=status;}
 DateTimeSpanStatus GetStatus() const { return m_status;}


 double GetTotalDays() const;    // span in days (about -3.65e6 to 3.65e6)
 double GetTotalHours() const;   // span in hours (about -8.77e7 to 8.77e6)
 double GetTotalMinutes() const; // span in minutes (about -5.26e9 to 5.26e9)
 double GetTotalSeconds() const; // span in seconds (about -3.16e11 to 3.16e11)

 long GetDays() const;       // component days in span
 long GetHours() const;      // component hours in span (-23 to 23)
 long GetMinutes() const;    // component minutes in span (-59 to 59)
 long GetSeconds() const;    // component seconds in span (-59 to 59)

// Operations
public:
 const CDateTimeSpan& operator=(double dblSpanSrc);
 const CDateTimeSpan& operator=(const CDateTimeSpan& dateSpanSrc);

 BOOL operator==(const CDateTimeSpan& dateSpan) const;
 BOOL operator!=(const CDateTimeSpan& dateSpan) const;
 BOOL operator<(const CDateTimeSpan& dateSpan) const;
 BOOL operator>(const CDateTimeSpan& dateSpan) const;
 BOOL operator<=(const CDateTimeSpan& dateSpan) const;
 BOOL operator>=(const CDateTimeSpan& dateSpan) const;

 // DateTimeSpan math
 CDateTimeSpan operator+(const CDateTimeSpan& dateSpan) const;
 CDateTimeSpan operator-(const CDateTimeSpan& dateSpan) const;
 const CDateTimeSpan& operator+=(const CDateTimeSpan dateSpan);
 const CDateTimeSpan& operator-=(const CDateTimeSpan dateSpan);
 CDateTimeSpan operator-() const;

 operator double() const;

 void SetDateTimeSpan(long lDays, int nHours, int nMins, int nSecs);

 // formatting
 _bstr_t Format(LPCTSTR pFormat) const;
 //CComString Format(UINT nID) const;

// Implementation
public:
 void CheckRange();
 friend CDateTime;
};

