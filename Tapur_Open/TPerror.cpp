// @(h) TPerror.cpp        ver 1.0 ( '99.2.28  )

// @(s)
// エラーハンドリングクラス メンバー関数定義
// エラーメッセージやログを処理する。
//

#include "stdafx.h"
#include <time.h>
//#include <fstream.h>
#include "Tapur.h"
#include "TPerror.h"
#include "FileSpec.h"


//
//  機能     : コンストラクタ
//  
//  機能説明 : 初期処理
//  
//  返り値   : なし
//  
//  備考     : 
//  
CTPerror::CTPerror()
{
	/// ログ生成ディレクトリを決定
	_TCHAR tstrFolder[MAX_PATH];	
	SHGetSpecialFolderPath(NULL, tstrFolder, CSIDL_APPDATA, FALSE);	// Application Dataフォルダ
	CString strFolder = tstrFolder;
	strFolder += _T("\\Tapur\\");
	_stprintf_s(LogDir, _MAX_PATH, _T("%s%s"), strFolder, LOGDIR_NAME);
}

//
//  機能     : 古いログの削除
//  
//  機能説明 : LOG_EXPIRE 日以上前のログは削除
//  
//  返り値   : true  正常終了
//             false エラー発生
//  
//  備考     : 
//  
bool CTPerror::DelOldLog()
{
	CString str;
	CFileFind finder;
	CTime ntm;
	CTime tm;
	CTimeSpan tms;
	CFileSpec fs;

	/// LOG_EXPIRE期間以前のログファイルを削除
	if (!SetCurrentDirectory(LogDir)) return false;
	ntm = CTime::GetCurrentTime();
	BOOL bWorking = finder.FindFile(_T("*.log"));
	if (!bWorking) return false;
	str.Empty();
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory()) {
			str = finder.GetFileName();
			/// ログファイル作成日付と現在との差分
			finder.GetCreationTime(tm);
			tms = ntm - tm;
			if (tms.GetDays() > LOG_EXPIRE) {
				fs.SetFullSpec(str);
				if (!fs.Exist()) continue;
				fs.FileDelete();
			}
		}
	}
	finder.Close();

	return true;
}

//
//  機能     : ログの書き込み
//  
//  機能説明 : ログは実行フォルダに「Log」フォルダがある時のみ生成される。
//             作成時の日付に対応した名称（"990228.log"など）で作成される。
//  
//  返り値   : true  正常終了
//             false エラー発生
//  
//  備考     : メンテナンス用機能
//  
BOOL CTPerror::WriteLog(
	const _TCHAR* str)		// ログメッセージ
{
	TRY {
		/// ログ生成フラグチェック
		if (theApp.m_intEnableLog < 0) {
			CRegDWORD rgEnableLog;
			DWORD dw;
			rgEnableLog = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableLog"), 1, TRUE);
			dw = rgEnableLog;
			theApp.m_intEnableLog = (int)dw;
		}
		if (theApp.m_intEnableLog == 0) return TRUE;

		/// ログファイル名の決定
		time_t tnum;
		struct tm ts;
		time(&tnum);
		if(localtime_s(&ts, &tnum)) return FALSE;
		_TCHAR filename[_MAX_PATH];
		_stprintf_s(filename, _MAX_PATH, _T("%s\\%02d%02d%02d.log"), 
			LogDir, ts.tm_year, (ts.tm_mon + 1), ts.tm_mday);

		//// ログディレクトリ生成
		CFileSpec fs;
		fs.SetFullSpec(filename);
		if (!fs.MkFolder()) return FALSE;

		/// メッセージ書き込み
		CString strMsg;
		strMsg.Format(_T("%02d:%02d:%02d:%s"), ts.tm_hour, ts.tm_min, ts.tm_sec, str);
		return fs.SaveString(strMsg, TRUE);
	}
    CATCH_ALL(e)
    {
		return FALSE;
    }
	END_CATCH_ALL

	return TRUE;
}

//
//  機能     : エラーメッセージ処理
//  
//  機能説明 : メッセージの表示やログ保存を行う。
//             エラータイプにより処理を切り替える。
//             書式「ソースファイル名:行番号:エラーコード:メッセージ」
//  
//  返り値   : なし
//  
//  備考     : 継承クラスによって頻繁に使用される主要関数
//  
void CTPerror::ErrorMessage(	// エラーメッセージ生成
	const byte type,	// エラータイプ
	const _TCHAR* file,		// ソースファイル名
	const int line,		// エラー発生行
	const long code,	// エラーコード
	const _TCHAR* message)		// メッセージ
{
	_TCHAR *p;
	_TCHAR instr[MAX_ERRSTR];
	::ZeroMemory(instr, MAX_ERRSTR);
	_tcsncpy_s(instr, MAX_ERRSTR, message, MAX_ERRSTR-1);	// NULL終端のないデータが来た場合の対処
	int len = _tcsclen(instr) + 128;
	_TCHAR *str = new _TCHAR[len];
	_TCHAR *logstr = new _TCHAR[len];

	// ソースファイル名からパスを削除
	p = _tcsrchr((_TCHAR *)file, '\\');
	p = (p == NULL) ? (_TCHAR *)file : (p+1); 

	_stprintf_s(str, len, _T("%s:%d:0x%x:%s"), p, line, code, message);
	if(type == ERRTYPE_ERROR) {
		/// エラー（ログ）
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONSTOP);
		_stprintf_s(logstr, len, _T("ERROR:%s\n"), str);
		WriteLog(logstr);
	} else if(type == ERRTYPE_WARNING) {
		/// 警告（ログ）
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		_stprintf_s(logstr, len, _T("WARNING:%s\n"), str);
		WriteLog(logstr);
	} else if(type == ERRTYPE_INFO) {
		/// 情報（ログなし）
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
	} else if(type == ERRTYPE_LOGONLY) {
		/// ログのみ
		_stprintf_s(logstr, len, _T("LOG:%s\n"), str);
		WriteLog(logstr);
	}

	delete [] str;
	delete [] logstr;
}
