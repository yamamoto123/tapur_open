////////////////////////////////////////////////////////////////////////////////
// CppSQLite3 - A C++ wrapper around the SQLite3 embedded database library.
//
// Copyright (c) 2004 Rob Groves. All Rights Reserved. rob.groves@btinternet.com
// 
// Permission to use, copy, modify, and distribute this software and its
// documentation for any purpose, without fee, and without a written
// agreement, is hereby granted, provided that the above copyright notice, 
// this paragraph and the following two paragraphs appear in all copies, 
// modifications, and distributions.
//
// IN NO EVENT SHALL THE AUTHOR BE LIABLE TO ANY PARTY FOR DIRECT,
// INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
// PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
// EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// THE AUTHOR SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
// ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". THE AUTHOR HAS NO OBLIGATION
// TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
//
// V3.0		03/08/2004	-Initial Version for sqlite3
//
// V3.1		16/09/2004	-Implemented getXXXXField using sqlite3 functions
//						-Added CppSQLiteDB3::tableExists()
////////////////////////////////////////////////////////////////////////////////
#ifndef _CppSQLite3_H_
#define _CppSQLite3_H_

#include <TCHAR.H>
#include "sqlite3.h"
#include <cstdio>
#include <cstring>

#define CPPSQLITE_ERROR 1000

class CppSQLite3Exception
{
public:

    CppSQLite3Exception(const int nErrCode,
                    const char *szErrMess,
                    bool bDeleteMsg=true);

    CppSQLite3Exception(const CppSQLite3Exception&  e);

    virtual ~CppSQLite3Exception();

    const int errorCode() { return mnErrCode; }

    const _TCHAR* errorMessage() { return mpszErrMess; }

    static const _TCHAR* errorCodeAsString(int nErrCode);

private:

    int mnErrCode;
    _TCHAR* mpszErrMess;
};


class CppSQLite3Buffer
{
public:

    CppSQLite3Buffer();

    ~CppSQLite3Buffer();

    const _TCHAR* format(const _TCHAR* szFormat, ...);

    operator const _TCHAR*() { return mpBuf; }

    void clear();

private:

    _TCHAR* mpBuf;
};


class CppSQLite3Binary
{
public:

    CppSQLite3Binary();

    ~CppSQLite3Binary();

    void setBinary(const _TUCHAR* pBuf, int nLen);
    void setEncoded(const _TUCHAR* pBuf);

    const _TUCHAR* getEncoded();
    const _TUCHAR* getBinary();

    int getBinaryLength();

    _TUCHAR* allocBuffer(int nLen);

    void clear();

private:

    _TUCHAR* mpBuf;
    int mnBinaryLen;
    int mnBufferLen;
    int mnEncodedLen;
    bool mbEncoded;
};


class CppSQLite3Query
{
public:

    CppSQLite3Query();

    CppSQLite3Query(const CppSQLite3Query& rQuery);

    CppSQLite3Query(sqlite3* pDB,
				sqlite3_stmt* pVM,
                bool bEof,
                bool bOwnVM=true);

    CppSQLite3Query& operator=(const CppSQLite3Query& rQuery);

    virtual ~CppSQLite3Query();

    int numFields();

    int fieldIndex(const _TCHAR* szField);
    const _TCHAR* fieldName(int nCol);

    const _TCHAR* fieldDeclType(int nCol);
    int fieldDataType(int nCol);

    const _TCHAR* fieldValue(int nField);
    const _TCHAR* fieldValue(const _TCHAR* szField);

    int getIntField(int nField, int nNullValue=0);
    int getIntField(const _TCHAR* szField, int nNullValue=0);

    double getFloatField(int nField, double fNullValue=0.0);
    double getFloatField(const _TCHAR* szField, double fNullValue=0.0);

    const _TCHAR* getStringField(int nField, const _TCHAR* szNullValue=_T(""));
    const _TCHAR* getStringField(const _TCHAR* szField, const _TCHAR* szNullValue=_T(""));

    const _TUCHAR* getBlobField(int nField, int& nLen);
    const _TUCHAR* getBlobField(const _TCHAR* szField, int& nLen);

    bool fieldIsNull(int nField);
    bool fieldIsNull(const _TCHAR* szField);

    bool eof();

    void nextRow();

    void finalize();

private:

    void checkVM();

	sqlite3* mpDB;
    sqlite3_stmt* mpVM;
    bool mbEof;
    int mnCols;
    bool mbOwnVM;
};


class CppSQLite3Table
{
public:

    CppSQLite3Table();

    CppSQLite3Table(const CppSQLite3Table& rTable);

    CppSQLite3Table(_TCHAR** paszResults, int nRows, int nCols);

    virtual ~CppSQLite3Table();

    CppSQLite3Table& operator=(const CppSQLite3Table& rTable);

    int numFields();

    int numRows();

    const _TCHAR* fieldName(int nCol);

    const _TCHAR* fieldValue(int nField);
    const _TCHAR* fieldValue(const _TCHAR* szField);

    int getIntField(int nField, int nNullValue=0);
    int getIntField(const _TCHAR* szField, int nNullValue=0);

    double getFloatField(int nField, double fNullValue=0.0);
    double getFloatField(const _TCHAR* szField, double fNullValue=0.0);

    const _TCHAR* getStringField(int nField, const _TCHAR* szNullValue=_T(""));
    const _TCHAR* getStringField(const _TCHAR* szField, const _TCHAR* szNullValue=_T(""));

    bool fieldIsNull(int nField);
    bool fieldIsNull(const _TCHAR* szField);

    void setRow(int nRow);

    void finalize();

private:

    void checkResults();

    int mnCols;
    int mnRows;
    int mnCurrentRow;
    _TCHAR** mpaszResults;
};


class CppSQLite3Statement
{
public:

    CppSQLite3Statement();

    CppSQLite3Statement(const CppSQLite3Statement& rStatement);

    CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM);

    virtual ~CppSQLite3Statement();

    CppSQLite3Statement& operator=(const CppSQLite3Statement& rStatement);

    int execDML();

    CppSQLite3Query execQuery();

    void bind(int nParam, const _TCHAR* szValue);
    void bind(int nParam, const int nValue);
    void bind(int nParam, const double dwValue);
    void bind(int nParam, const _TUCHAR* blobValue, int nLen);
    void bindNull(int nParam);

    void reset();

    void finalize();

private:

    void checkDB();
    void checkVM();

    sqlite3* mpDB;
    sqlite3_stmt* mpVM;
};


class CppSQLite3DB
{
public:

    CppSQLite3DB();

    virtual ~CppSQLite3DB();

    void open(const _TCHAR* szFile);

    void close();

	bool tableExists(const _TCHAR* szTable);

    int execDML(const _TCHAR* szSQL);

    CppSQLite3Query execQuery(const _TCHAR* szSQL);

    int execScalar(const _TCHAR* szSQL);

    CppSQLite3Table getTable(const _TCHAR* szSQL);

    CppSQLite3Statement compileStatement(const _TCHAR* szSQL);

    sqlite_int64 lastRowId();

    void interrupt() { sqlite3_interrupt(mpDB); }

    void setBusyTimeout(int nMillisecs);

    static const _TCHAR* SQLiteVersion() { return _T(SQLITE_VERSION); }

private:

    CppSQLite3DB(const CppSQLite3DB& db);
    CppSQLite3DB& operator=(const CppSQLite3DB& db);

    sqlite3_stmt* compile(const _TCHAR* szSQL);

    void checkDB();

    sqlite3* mpDB;
    int mnBusyTimeoutMs;
};

#endif
