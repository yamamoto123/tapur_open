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
// DateTime.cpp: implementation of the CDateTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DateTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDateTime class HELPER definitions

// Verifies will fail if the needed buffer size is too large
#define MAX_TIME_BUFFER_SIZE    128         // matches that in timecore.cpp
#define MIN_DATE                (-657434L)  // about year 100
#define MAX_DATE                2958465L    // about year 9999

// Half a second, expressed in days
#define HALF_SECOND  (1.0/172800.0)

// One-based array of days in year at month start
static int rgMonthDays[13] =
 {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

/*
static BOOL OleDateFromTm(WORD wYear, WORD wMonth, WORD wDay,
 WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest);
static BOOL TmFromOleDate(DATE dtSrc, struct tm& tmDest);
static void TmConvertToStandardFormat(struct tm& tmSrc);
static double DoubleFromDate(DATE dt);
static DATE DateFromDouble(double dbl);
*/

/////////////////////////////////////////////////////////////////////////////
// CDateTime class
CDateTime::CDateTime()
{
 m_dt=0;
 SetStatus(invalid);
}

CDateTime::CDateTime(const CDateTime& dateSrc)
{
 m_status=dateSrc.m_status;
 m_dt=dateSrc.m_dt;
}

CDateTime::CDateTime(const VARIANT& varSrc)
{
 if (varSrc.vt != VT_DATE)
 {

   _variant_t varTemp(varSrc);
   varTemp.ChangeType(VT_DATE);
   m_dt = varTemp.date;
 }
 else
 {
  m_dt = varSrc.date;
  SetStatus(valid);
 } 
}

CDateTime::CDateTime(DATE dtSrc)
{
 m_dt = dtSrc;
 SetStatus(valid);
}

CDateTime::CDateTime(time_t& timeSrc)
{
	// Convert time_t to struct tm
	tm tm;

	if (localtime_s(&tm, &timeSrc))
	{
		// Local time must have failed (timsSrc before 1/1/70 12am)
		m_dt=0;
		SetStatus(invalid);
	}
	else
	{
		m_status = OleDateFromTm((WORD)(tm.tm_year + 1900),
		(WORD)(tm.tm_mon + 1), (WORD)tm.tm_mday,
		(WORD)tm.tm_hour, (WORD)tm.tm_min,
		(WORD)tm.tm_sec, m_dt) ? valid : invalid;
	}
}

CDateTime::CDateTime(const SYSTEMTIME& systimeSrc)
{
 m_status = OleDateFromTm(systimeSrc.wYear, systimeSrc.wMonth,
  systimeSrc.wDay, systimeSrc.wHour, systimeSrc.wMinute,
  systimeSrc.wSecond, m_dt) ? valid : invalid;


}

CDateTime::CDateTime(const FILETIME& filetimeSrc)
{
 // Assume UTC FILETIME, so convert to LOCALTIME
 FILETIME filetimeLocal;
 if (!FileTimeToLocalFileTime( &filetimeSrc, &filetimeLocal))
 {
  m_status = invalid;
 }
 else
 {
  // Take advantage of SYSTEMTIME -> FILETIME conversion
  SYSTEMTIME systime;
  m_status = FileTimeToSystemTime(&filetimeLocal, &systime) ?
   valid : invalid;

  // At this point systime should always be valid, but...
  if (GetStatus() == valid)
  {
   m_status = OleDateFromTm(systime.wYear, systime.wMonth,
    systime.wDay, systime.wHour, systime.wMinute,
    systime.wSecond, m_dt) ? valid : invalid;
  }
 }


}

/////

CDateTime PASCAL CDateTime::GetCurrentTime()
{
	time_t tm = ::time(NULL);
	return CDateTime(tm);
}

int CDateTime::GetYear() const
{
 struct tm tmTemp;

 if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
  return tmTemp.tm_year;
 else
  return AFX_OLE_DATETIME_ERROR;
}

int CDateTime::GetMonth() const
{
 struct tm tmTemp;

 if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
  return tmTemp.tm_mon;
 else
  return AFX_OLE_DATETIME_ERROR;
}

int CDateTime::GetDay() const
{
 struct tm tmTemp;

 if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
  return tmTemp.tm_mday;
 else
  return AFX_OLE_DATETIME_ERROR;
}

int CDateTime::GetHour() const
{
 struct tm tmTemp;

 if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
  return tmTemp.tm_hour;
 else
  return AFX_OLE_DATETIME_ERROR;
}

int CDateTime::GetMinute() const
{
 struct tm tmTemp;

 if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
  return tmTemp.tm_min;
 else
  return AFX_OLE_DATETIME_ERROR;
}

int CDateTime::GetSecond() const
{
 struct tm tmTemp;

 if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
  return tmTemp.tm_sec;
 else
  return AFX_OLE_DATETIME_ERROR;
}

int CDateTime::GetDayOfWeek() const
{
 struct tm tmTemp;

 if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
  return tmTemp.tm_wday;
 else
  return AFX_OLE_DATETIME_ERROR;
}

int CDateTime::GetDayOfYear() const
{
 struct tm tmTemp;

 if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
  return tmTemp.tm_yday;
 else
  return AFX_OLE_DATETIME_ERROR;
}

const CDateTime& CDateTime::operator=(const VARIANT& varSrc)
{
 if (varSrc.vt != VT_DATE)
 {

   _variant_t varTemp(varSrc);
   varTemp.ChangeType(VT_DATE);
   m_dt = varTemp.date;
 }
 else
 {
  m_dt = varSrc.date;
  SetStatus(valid);
 }

 return *this;
}

const CDateTime& CDateTime::operator=(DATE dtSrc)
{
 m_dt = dtSrc;
 SetStatus(valid);

 return *this;
}

const CDateTime& CDateTime::operator=(const time_t& timeSrc)
{
	// Convert time_t to struct tm
	tm tm;

	if (localtime_s(&tm, &timeSrc))
	{
		// Local time must have failed (timsSrc before 1/1/70 12am)
	}
	else
	{
		m_status = OleDateFromTm((WORD)(tm.tm_year + 1900),
		(WORD)(tm.tm_mon + 1), (WORD)tm.tm_mday,
		(WORD)tm.tm_hour, (WORD)tm.tm_min,
		(WORD)tm.tm_sec, m_dt) ? valid : invalid;
	}
	
	return *this;
}

const CDateTime& CDateTime::operator=(const SYSTEMTIME& systimeSrc)
{
 m_status = OleDateFromTm(systimeSrc.wYear, systimeSrc.wMonth,
  systimeSrc.wDay, systimeSrc.wHour, systimeSrc.wMinute,
  systimeSrc.wSecond, m_dt) ? valid : invalid;

 return *this;
}

const CDateTime& CDateTime::operator=(const FILETIME& filetimeSrc)
{
 // Assume UTC FILETIME, so convert to LOCALTIME
 FILETIME filetimeLocal;
 if (!FileTimeToLocalFileTime( &filetimeSrc, &filetimeLocal))
 {
  m_status = invalid;
 }
 else
 {
  // Take advantage of SYSTEMTIME -> FILETIME conversion
  SYSTEMTIME systime;
  m_status = FileTimeToSystemTime(&filetimeLocal, &systime) ?
   valid : invalid;

  // At this point systime should always be valid, but...
  if (GetStatus() == valid)
  {
   m_status = OleDateFromTm(systime.wYear, systime.wMonth,
    systime.wDay, systime.wHour, systime.wMinute,
    systime.wSecond, m_dt) ? valid : invalid;
  }
 }

 return *this;
}

BOOL CDateTime::operator<(const CDateTime& date) const
{
 //ASSERT(GetStatus() == valid);
 //ASSERT(date.GetStatus() == valid);

 // Handle negative dates
 return DoubleFromDate(m_dt) < DoubleFromDate(date.m_dt);
}

BOOL CDateTime::operator>(const CDateTime& date) const
{   //ASSERT(GetStatus() == valid);
 //ASSERT(date.GetStatus() == valid);

 // Handle negative dates
 return DoubleFromDate(m_dt) > DoubleFromDate(date.m_dt);
}

BOOL CDateTime::operator<=(const CDateTime& date) const
{
 //ASSERT(GetStatus() == valid);
 //ASSERT(date.GetStatus() == valid);

 // Handle negative dates
 return DoubleFromDate(m_dt) <= DoubleFromDate(date.m_dt);
}

BOOL CDateTime::operator>=(const CDateTime& date) const
{
 //ASSERT(GetStatus() == valid);
 //ASSERT(date.GetStatus() == valid);

 // Handle negative dates
 return DoubleFromDate(m_dt) >= DoubleFromDate(date.m_dt);
}

CDateTime CDateTime::operator+(const CDateTimeSpan& dateSpan) const
{
 CDateTime dateResult;    // Initializes m_status to valid

 // If either operand NULL, result NULL
 if (GetStatus() == null || dateSpan.GetStatus() == null)
 {
  dateResult.SetStatus(null);
  return dateResult;
 }

 // If either operand invalid, result invalid
 if (GetStatus() == invalid || dateSpan.GetStatus() == invalid)
 {
  dateResult.SetStatus(invalid);
  return dateResult;
 }

 // Compute the actual date difference by adding underlying dates
 dateResult = DateFromDouble(DoubleFromDate(m_dt) + dateSpan.m_span);

 // Validate within range
 dateResult.CheckRange();

 return dateResult;
}

CDateTime CDateTime::operator-(const CDateTimeSpan& dateSpan) const
{
 CDateTime dateResult;    // Initializes m_status to valid

 // If either operand NULL, result NULL
 if (GetStatus() == null || dateSpan.GetStatus() == null)
 {
  dateResult.SetStatus(null);
  return dateResult;
 }

 // If either operand invalid, result invalid
 if (GetStatus() == invalid || dateSpan.GetStatus() == invalid)
 {
  dateResult.SetStatus(invalid);
  return dateResult;
 }

 // Compute the actual date difference by subtracting underlying dates
 dateResult = DateFromDouble(DoubleFromDate(m_dt) - dateSpan.m_span);

 // Validate within range
 dateResult.CheckRange();

 return dateResult;
}

CDateTimeSpan CDateTime::operator-(const CDateTime& date) const
{
 CDateTimeSpan spanResult;

 // If either operand NULL, result NULL
 if (GetStatus() == null || date.GetStatus() == null)
 {
  spanResult.SetStatus(CDateTimeSpan::null);
  return spanResult;
 }

 // If either operand invalid, result invalid
 if (GetStatus() == invalid || date.GetStatus() == invalid)
 {
  spanResult.SetStatus(CDateTimeSpan::invalid);
  return spanResult;
 }

 // Return result (span can't be invalid, so don't check range)
 return DoubleFromDate(m_dt) - DoubleFromDate(date.m_dt);
}

int CDateTime::SetDateTime(int nYear, int nMonth, int nDay,
 int nHour, int nMin, int nSec)
{
 return m_status = OleDateFromTm((WORD)nYear, (WORD)nMonth,
  (WORD)nDay, (WORD)nHour, (WORD)nMin, (WORD)nSec, m_dt) ?
  valid : invalid;
}

BOOL CDateTime::ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags, LCID lcid)
{
 SCODE sc;
 if (FAILED(sc = VarDateFromStr(CComBSTR(lpszDate), lcid,
  dwFlags, &m_dt)))
 {
  if (sc == DISP_E_TYPEMISMATCH)
  {
   // Can't convert string to date, set 0 and invalidate
   m_dt = 0;
   SetStatus(invalid);
   return FALSE;
  }
  else if (sc == DISP_E_OVERFLOW)
  {
   // Can't convert string to date, set -1 and invalidate
   m_dt = -1;
   SetStatus(invalid);
   return FALSE;
  }
  else
  {
  // TRACE0("\nCDateTime VarDateFromStr call failed.\n\t");
  // if (sc == E_OUTOFMEMORY)
  //  AfxThrowMemoryException();
  // else
  //  AfxThrowOleException(sc);
   m_dt=0;
   SetStatus(invalid);
   return FALSE;
  }
 }

 SetStatus(valid);
 return TRUE;
}


_bstr_t CDateTime::Format(DWORD dwFlags, LCID lcid) const
{
 _bstr_t strDate;

 // If null, return empty string
 if (GetStatus() == null)
  return strDate;

 // If invalid, return DateTime resource string
 if (GetStatus() == invalid)
 {
 // VERIFY(strDate.LoadString(AFX_IDS_INVALID_DATETIME));
  return strDate;
 }

 _variant_t var;
 // Don't need to trap error. Should not fail due to type mismatch
 VarBstrFromDate(m_dt, lcid, dwFlags, &V_BSTR(&var));
 var.vt = VT_BSTR;
 return V_BSTR(&var);
}

_bstr_t CDateTime::Format(LPCTSTR pFormat) const
{
 _bstr_t strDate;
 struct tm tmTemp;

 // If null, return empty string
 if (GetStatus() == null)
  return strDate;

 // If invalid, return DateTime resource string
 if (GetStatus() == invalid || !TmFromOleDate(m_dt, tmTemp))
 {
 // VERIFY(strDate.LoadString(AFX_IDS_INVALID_DATETIME));
  return strDate;
 }

 // Convert tm from afx internal format to standard format
 TmConvertToStandardFormat(tmTemp);

 // Fill in the buffer, disregard return value as it's not necessary
 LPTSTR lpszTemp = new TCHAR[MAX_TIME_BUFFER_SIZE];
 _tcsftime(lpszTemp, MAX_TIME_BUFFER_SIZE , pFormat, &tmTemp);
 strDate=lpszTemp;
 delete [] lpszTemp;

 return strDate;
}
/*
CComString CDateTime::Format(UINT nFormatID) const
{
 CComString strFormat;
 strFormat.LoadString(nFormatID);
 return Format(strFormat);
}
*/

void CDateTime::CheckRange()
{
 if (m_dt > MAX_DATE || m_dt < MIN_DATE) // about year 100 to about 9999
  SetStatus(invalid);
}


/////////////////////////////////////////////////////////////////////////////
// CDateTimeSpan class helpers

#define MAX_DAYS_IN_SPAN    3615897L

/////////////////////////////////////////////////////////////////////////////
// CDateTimeSpan class
CDateTimeSpan::CDateTimeSpan()
{
 m_span=0;
 SetStatus(invalid);
}

CDateTimeSpan::CDateTimeSpan(double dblSpanSrc)
{
 m_span = dblSpanSrc;
 SetStatus(valid);
}

CDateTimeSpan::CDateTimeSpan(const CDateTimeSpan& dateSpanSrc)
{
 m_span = dateSpanSrc.m_span;
 m_status = dateSpanSrc.m_status;
}

long CDateTimeSpan::GetHours() const
{
// ASSERT(GetStatus() == valid);

 double dblTemp;

 // Truncate days and scale up
 dblTemp = modf(m_span, &dblTemp);
 return (long)(dblTemp * 24);
}

long CDateTimeSpan::GetMinutes() const
{
// ASSERT(GetStatus() == valid);

 double dblTemp;

 // Truncate hours and scale up
 dblTemp = modf(m_span * 24, &dblTemp);
 return (long)(dblTemp * 60);
}

long CDateTimeSpan::GetSeconds() const
{
// ASSERT(GetStatus() == valid);

 double dblTemp;

 // Truncate minutes and scale up
 dblTemp = modf(m_span * 24 * 60, &dblTemp);
 return (long)(dblTemp * 60);
}

const CDateTimeSpan& CDateTimeSpan::operator=(double dblSpanSrc)
{
 m_span = dblSpanSrc;
 SetStatus(valid);
 return *this;
}

const CDateTimeSpan& CDateTimeSpan::operator=(const CDateTimeSpan&
dateSpanSrc)
{
 m_span = dateSpanSrc.m_span;
 m_status = dateSpanSrc.m_status;
 return *this;
}

CDateTimeSpan CDateTimeSpan::operator+(const CDateTimeSpan& dateSpan) const
{
 CDateTimeSpan dateSpanTemp;

 // If either operand Null, result Null
 if (GetStatus() == null || dateSpan.GetStatus() == null)
 {
  dateSpanTemp.SetStatus(null);
  return dateSpanTemp;
 }

 // If either operand Invalid, result Invalid
 if (GetStatus() == invalid || dateSpan.GetStatus() == invalid)
 {
  dateSpanTemp.SetStatus(invalid);
  return dateSpanTemp;
 }

 // Add spans and validate within legal range
 dateSpanTemp.m_span = m_span + dateSpan.m_span;
 dateSpanTemp.CheckRange();

 return dateSpanTemp;
}

CDateTimeSpan CDateTimeSpan::operator-(const CDateTimeSpan& dateSpan) const
{
 CDateTimeSpan dateSpanTemp;

 // If either operand Null, result Null
 if (GetStatus() == null || dateSpan.GetStatus() == null)
 {
  dateSpanTemp.SetStatus(null);
  return dateSpanTemp;
 }

 // If either operand Invalid, result Invalid
 if (GetStatus() == invalid || dateSpan.GetStatus() == invalid)
 {
  dateSpanTemp.SetStatus(invalid);
  return dateSpanTemp;
 }

 // Subtract spans and validate within legal range
 dateSpanTemp.m_span = m_span - dateSpan.m_span;
 dateSpanTemp.CheckRange();

 return dateSpanTemp;
}

void CDateTimeSpan::SetDateTimeSpan(
 long lDays, int nHours, int nMins, int nSecs)
{
 // Set date span by breaking into fractional days (all input ranges valid)
 m_span = lDays + ((double)nHours)/24 + ((double)nMins)/(24*60) +
  ((double)nSecs)/(24*60*60);

 SetStatus(valid);
}

_bstr_t CDateTimeSpan::Format(LPCTSTR pFormat) const
{
 _bstr_t strSpan;
 struct tm tmTemp;

 // If null, return empty string
 if (GetStatus() == null)
  return strSpan;

 // If invalid, return DateTimeSpan resource string
 if (GetStatus() == invalid || !TmFromOleDate(m_span, tmTemp))
 {
  //VERIFY(strSpan.LoadString(AFX_IDS_INVALID_DATETIMESPAN));
  return strSpan;
 }

 // Convert tm from afx internal format to standard format
 TmConvertToStandardFormat(tmTemp);

 // Fill in the buffer, disregard return value as it's not necessary
 LPTSTR lpszTemp = new TCHAR[MAX_TIME_BUFFER_SIZE];
 _tcsftime(lpszTemp, MAX_TIME_BUFFER_SIZE, pFormat, &tmTemp);
 strSpan=lpszTemp;
 delete [] lpszTemp;

 return strSpan;
}

/*
CComString CDateTimeSpan::Format(UINT nFormatID) const
{
 CComString strFormat;
 strFormat.LoadString(nFormatID);
 return Format(strFormat);
}
*/

void CDateTimeSpan::CheckRange()
{
 if(m_span < -MAX_DAYS_IN_SPAN || m_span > MAX_DAYS_IN_SPAN)
  SetStatus(invalid);
}

/////////////////////////////////////////////////////////////////////////////
// CDateTime class HELPERS - implementation

BOOL OleDateFromTm(WORD wYear, WORD wMonth, WORD wDay,
 WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest)
{
 // Validate year and month (ignore day of week and milliseconds)
 if (wYear > 9999 || wMonth < 1 || wMonth > 12)
  return FALSE;

 //  Check for leap year and set the number of days in the month
 BOOL bLeapYear = ((wYear & 3) == 0) &&
  ((wYear % 100) != 0 || (wYear % 400) == 0);

 int nDaysInMonth =
  rgMonthDays[wMonth] - rgMonthDays[wMonth-1] +
  ((bLeapYear && wDay == 29 && wMonth == 2) ? 1 : 0);

 // Finish validating the date
 if (wDay < 1 || wDay > nDaysInMonth ||
  wHour > 23 || wMinute > 59 ||
  wSecond > 59)
 {
  return FALSE;
 }

 // Cache the date in days and time in fractional days
 long nDate;
 double dblTime;

 //It is a valid date; make Jan 1, 1AD be 1
 nDate = wYear*365L + wYear/4 - wYear/100 + wYear/400 +
  rgMonthDays[wMonth-1] + wDay;

 //  If leap year and it's before March, subtract 1:
 if (wMonth <= 2 && bLeapYear)
  --nDate;

 //  Offset so that 12/30/1899 is 0
 nDate -= 693959L;

 dblTime = (((long)wHour * 3600L) +  // hrs in seconds
  ((long)wMinute * 60L) +  // mins in seconds
  ((long)wSecond)) / 86400.;

 dtDest = (double) nDate + ((nDate >= 0) ? dblTime : -dblTime);

 return TRUE;
}

BOOL TmFromOleDate(DATE dtSrc, struct tm& tmDest)
{
 // The legal range does not actually span year 0 to 9999.
 if (dtSrc > MAX_DATE || dtSrc < MIN_DATE) // about year 100 to about 9999
  return FALSE;

 long nDays;             // Number of days since Dec. 30, 1899
 long nDaysAbsolute;     // Number of days since 1/1/0
 long nSecsInDay;        // Time in seconds since midnight
 long nMinutesInDay;     // Minutes in day

 long n400Years;         // Number of 400 year increments since 1/1/0
 long n400Century;       // Century within 400 year block (0,1,2 or 3)
 long n4Years;           // Number of 4 year increments since 1/1/0
 long n4Day;             // Day within 4 year block
       //  (0 is 1/1/yr1, 1460 is 12/31/yr4)
 long n4Yr;              // Year within 4 year block (0,1,2 or 3)
 BOOL bLeap4 = TRUE;     // TRUE if 4 year block includes leap year

 double dblDate = dtSrc; // tempory serial date

 // If a valid date, then this conversion should not overflow
 nDays = (long)dblDate;

 // Round to the second
 dblDate += ((dtSrc > 0.0) ? HALF_SECOND : -HALF_SECOND);

 nDaysAbsolute = (long)dblDate + 693959L; // Add days from 1/1/0 to 12/30/1899

 dblDate = fabs(dblDate);
 nSecsInDay = (long)((dblDate - floor(dblDate)) * 86400.);

 // Calculate the day of week (sun=1, mon=2...)
 //   -1 because 1/1/0 is Sat.  +1 because we want 1-based
 tmDest.tm_wday = (int)((nDaysAbsolute - 1) % 7L) + 1;

 // Leap years every 4 yrs except centuries not multiples of 400.
 n400Years = (long)(nDaysAbsolute / 146097L);

 // Set nDaysAbsolute to day within 400-year block
 nDaysAbsolute %= 146097L;

 // -1 because first century has extra day
 n400Century = (long)((nDaysAbsolute - 1) / 36524L);

 // Non-leap century
 if (n400Century != 0)
 {
  // Set nDaysAbsolute to day within century
  nDaysAbsolute = (nDaysAbsolute - 1) % 36524L;

  // +1 because 1st 4 year increment has 1460 days
  n4Years = (long)((nDaysAbsolute + 1) / 1461L);

  if (n4Years != 0)
   n4Day = (long)((nDaysAbsolute + 1) % 1461L);
  else
  {
   bLeap4 = FALSE;
   n4Day = (long)nDaysAbsolute;
  }
 }
 else
 {
  // Leap century - not special case!
  n4Years = (long)(nDaysAbsolute / 1461L);
  n4Day = (long)(nDaysAbsolute % 1461L);
 }

 if (bLeap4)
 {
  // -1 because first year has 366 days
  n4Yr = (n4Day - 1) / 365;

  if (n4Yr != 0)
   n4Day = (n4Day - 1) % 365;
 }
 else
 {
  n4Yr = n4Day / 365;
  n4Day %= 365;
 }

 // n4Day is now 0-based day of year. Save 1-based day of year, year number
 tmDest.tm_yday = (int)n4Day + 1;
 tmDest.tm_year = n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr;

 // Handle leap year: before, on, and after Feb. 29.
 if (n4Yr == 0 && bLeap4)
 {
  // Leap Year
  if (n4Day == 59)
  {
   /* Feb. 29 */
   tmDest.tm_mon = 2;
   tmDest.tm_mday = 29;
   goto DoTime;
  }

  // Pretend it's not a leap year for month/day comp.
  if (n4Day >= 60)
   --n4Day;
 }

 // Make n4DaY a 1-based day of non-leap year and compute
 //  month/day for everything but Feb. 29.
 ++n4Day;

 // Month number always >= n/32, so save some loop time */
 for (tmDest.tm_mon = (n4Day >> 5) + 1;
  n4Day > rgMonthDays[tmDest.tm_mon]; tmDest.tm_mon++);

 tmDest.tm_mday = (int)(n4Day - rgMonthDays[tmDest.tm_mon-1]);

DoTime:
 if (nSecsInDay == 0)
  tmDest.tm_hour = tmDest.tm_min = tmDest.tm_sec = 0;
 else
 {
  tmDest.tm_sec = (int)nSecsInDay % 60L;
  nMinutesInDay = nSecsInDay / 60L;
  tmDest.tm_min = (int)nMinutesInDay % 60;
  tmDest.tm_hour = (int)nMinutesInDay / 60;
 }

 return TRUE;
}

void TmConvertToStandardFormat(struct tm& tmSrc)
{
 // Convert afx internal tm to format expected by runtimes (_tcsftime, etc)
 tmSrc.tm_year -= 1900;  // year is based on 1900
 tmSrc.tm_mon -= 1;      // month of year is 0-based
 tmSrc.tm_wday -= 1;     // day of week is 0-based
 tmSrc.tm_yday -= 1;     // day of year is 0-based
}

double DoubleFromDate(DATE dt)
{
 // No problem if positive
 if (dt >= 0)
  return dt;

 // If negative, must convert since negative dates not continuous
 // (examples: -1.25 to -.75, -1.50 to -.50, -1.75 to -.25)
 double temp = ceil(dt);
 return temp - (dt - temp);
}

DATE DateFromDouble(double dbl)
{
 // No problem if positive
 if (dbl >= 0)
  return dbl;

 // If negative, must convert since negative dates not continuous
 // (examples: -.75 to -1.25, -.50 to -1.50, -.25 to -1.75)
 double temp = floor(dbl); // dbl is now whole part
 return temp + (temp - dbl);
}