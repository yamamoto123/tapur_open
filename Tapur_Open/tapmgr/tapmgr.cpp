//
// tapmgr.cpp : Tapur.exeの監視プログラム
//	・定期的にTapur.exeのプロセスをチェックし、存在しない場合は起動
//
// tapur.exeと同一フォルダで実行


#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <shlobj.h>		// SHGetSpecialFolderPath()
#include "KillProcess.h"
#include "FileSpec.h"
#include "Registry.h"

#define APP_NAME	"Tapur.exe"
//#define APP_PATH	"C:\\Program Files\\Tapur\\"


// CreateProcess()を使って外部アプリケーションを起動する
static int ExeProcess(LPTSTR cmdstr)
{
	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;

	ZeroMemory( &sinfo, sizeof( STARTUPINFO ) );
	ZeroMemory( &pinfo, sizeof( PROCESS_INFORMATION ) );
	sinfo.cb = sizeof( STARTUPINFO );
	sinfo.dwFlags = STARTF_USESHOWWINDOW;
	sinfo.wShowWindow = SW_SHOWNORMAL;

	if ( !CreateProcess( NULL,
						 cmdstr,
						 NULL,
						 NULL,
						 FALSE,
						 0,
						 NULL,
						 NULL,
						 &sinfo,
						 &pinfo ) ) { 
		// 起動失敗
		return -1;
	}

	// 処理が終わるまで制御を戻しません
//	WaitForInputIdle( pinfo.hProcess, INFINITE );
//	WaitForSingleObject( pinfo.hProcess, INFINITE );
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
	_TCHAR tstrExe[_MAX_PATH];	
	CKillProcess kp;
	DWORD dwId;	// return value
	HANDLE hProcess;

	// Tapur,exeのパス
	CFileSpec fs(FS_APPDIR);	// application folder
	_stprintf(tstrExe, _T("%s%s"), fs.GetFullSpec(), APP_NAME);

	/// 二重起動の防止
	// ミューテックスオブジェクトを作る。ミューテックスオブジェクトが
	// すでに存在している場合は、アプリケーションの2つ目のインスタンス
	// である。ミューテックスハンドルは、プロセスが終了すると自動的に
	// 閉じられることに注意
	::CreateMutex(NULL, TRUE, "tapmgr.exe");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		hProcess = kp.FindProcess(APP_NAME, dwId);
		if(hProcess) {
			// Tapur.exeが起動済みなら画面オープン
			ExeProcess(tstrExe);
		}
		return 0;
	}

	// 監視インターバル
	CRegDWORD rgInterval("Software\\Tapur\\Manager\\Interval", 30, TRUE);	// 初期値:30sec
	DWORD dwInterval = rgInterval * 1000;	// mm sec

	while(1) {
		/// Tapurが起動していなければ起動
		hProcess = kp.FindProcess(APP_NAME, dwId);
		if(!hProcess) {
			//::Sleep(1000);		// Skypeの起動とずらすため:効果なし？
			ExeProcess(tstrExe);
		}

		::Sleep(dwInterval);	// 待機時間
	}

	return 0;
}

