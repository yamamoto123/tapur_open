// instchk.cpp : アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include <lmaccess.h>
#include <lmapibuf.h>
#include <LMErr.h>
#include "KillProcess.h"
//#include "FileSpec.h"
//#include "Ini.h"

#define MGR_NAME	"tapmgr.exe"
#define APP_NAME	"tapur.exe"
#define SKYPE_NAME	"skype.exe"

//
//  機能     : Begin Setupチェック用INIファイル作成
//  
//  機能説明 : 
//  
//  返り値   : 
//  
//  備考     : XP-SP1では[Message]キーが正常に処理されないため、プログラム内でメッセージ表示
//  
//static void SetIni(
//bool blContinue,	// インストール続行か
//LPCTSTR lpStrErr)	// インストール中止時のメッセージ
//{
//	CFileSpec fs(FS_WINDIR);
//	fs.SetFullSpec(fs.GetFullSpec() + _T("\\BSRESULT.INI"));
////	if (fs.Exist()) fs.FileDelete();
//
//	CIni ini;
//	ini.SetPathName(fs.GetFullSpec());
//	if (blContinue) {
//		ini.WriteString(_T("CheckResult"), _T("Result"), _T("ContinueInstall"));
////		ini.DeleteKey(_T("CheckResult"), _T("Message"));
//	} else {
//		ini.WriteString(_T("CheckResult"), _T("Result"), _T("NoInstall"));
////		ini.WriteString(_T("CheckResult"), _T("Message"), lpStrErr);
//AfxMessageBox(lpStrErr, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
//	}
//}


//
//  機能     : Begin Setupインストール時 チェック処理
//  
//  機能説明 : チェックはインストール前後の２回行われる（？）ため、「インストール済」チェックには使用不可
//  
//  返り値   : 
//  
//  備考     : 
//  
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
 	// TODO: この位置にコードを記述してください。

	// 起動中のTapur.exeをKILL
	CKillProcess kp;
	kp.KillProcess(MGR_NAME);	// こちらを先に削除
	kp.KillProcess(APP_NAME);
	::Sleep(1000);		// 停止待ち

/* メッセージは出るが、処理は中断されない
	if(kp.KillProcess(APP_NAME)) {
		SetIni(false, _T("Please uninstall the old Tapur."));
		return 1;
	}

	// インストール済みTapurのチェック
	CFileSpec fs(FS_PROGRAM_FILES);
	CString strPath = fs.GetFullSpec() + _T("\\Tapur\\Tapur.exe");
	fs.SetFullSpec(strPath);
	if (fs.Exist()) {
		SetIni(false, _T("Please uninstall the old Tapur."));
		return 1;
	}
*/

	// ADMINモードのチェック
	//_TCHAR usrName[MAX_PATH];
	//DWORD dwSize = MAX_PATH;
	//if (GetUserName(usrName, &dwSize)) {
	//	DWORD dwLevel = 1;
	//	LPUSER_INFO_1 pBuf = NULL;
	//	NET_API_STATUS nStatus;
	//	/// ユーザ名をUNICODEも変換
	//	wchar_t wc[MAX_PATH*2];
	//	DWORD dwLen = ::MultiByteToWideChar(CP_UTF8,0,usrName,-1,NULL,0);
	//	::MultiByteToWideChar(CP_UTF8, 0, usrName, -1, wc, dwLen);
	//	nStatus = NetUserGetInfo(NULL, wc, dwLevel, (LPBYTE *)&pBuf);
	//	if (nStatus == NERR_Success && pBuf != NULL) {
	//		if (pBuf->usri1_priv != USER_PRIV_ADMIN) {
	//			NetApiBufferFree(pBuf);	// 割り当て済みのメモリを解放
	//			SetIni(false, _T("Please install Tapur by the administrator privilege."));
	//			return 1;
	//		}
	//	}
	//	if (pBuf != NULL) NetApiBufferFree(pBuf);	// 割り当て済みのメモリを解放
	//}

	//// 起動中のSkype.exeをKILL
	//kp.KillProcess(SKYPE_NAME);

	//// OK
	//SetIni(true, _T(""));
	return 0;
}



