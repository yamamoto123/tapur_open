// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ヘッダーから使用されていない部分を除外します。
#endif

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef WINVER				// Windows XP 以降のバージョンに固有の機能の使用を許可します。
#define WINVER 0x0500		// Win2000
#endif

#ifndef _WIN32_WINNT		// Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0500	// Win2000
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINDOWS 0x0410 // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_IE			// IE 6.0 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_IE 0x0500	// Win2000
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

// 一般的で無視しても安全な MFC の警告メッセージの一部の非表示を解除します。
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分


#include <afxdisp.h>        // MFC オートメーション クラス



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

//#define _ATL_APARTMENT_THREADED		// ATLの使用を許可する

//#include <afxdisp.h>        // MFC のオートメーション クラス
//#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
//#include <io.h>
//#endif // _AFX_NO_AFXCMN_SUPPORT
//#include <afxtempl.h>		// CArray


// paak
#include <atlbase.h>		// ATLの使用を許可する（このヘッダはafx系ヘッダの後に配置すること）
#include <atlcom.h>

// memory leak 検出
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// リソースヘッダ
#include "resource.h"       

// 何かオーバーライドしたい場合、CComModule からクラスを派生させて
// それを使用することが可能ですが _Module の名前は変更しないでください。
class CTapurModule : public CComModule
{
public:
	LONG Unlock();
	LONG Lock();
	LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
	DWORD dwThreadID;
};
extern CTapurModule _Module;

// タイマー管理クラス
typedef struct _ST_TIMER
{
	BOOL blOnTimer;		// タイマー実行中フラグ
	long lngPassTime;	// タイマーイベント実行回数
	long lngLimitTime;	// タイマー制限時間(s)
	CTime tmStart;		// タイマー開始時刻
	CTime tmPass;		// タイマーイベント実行時刻
	long lngSpanTime;	// タイマー経過時間(s)
} ST_TIMER;

// タイマーID管理
#define ID_TIMER_APP			10
#define ID_TIMER_CONNECT		11
#define ID_TIMER_WAIT_ANSWER	12
#define ID_TIMER_RECORD_MAX		13
#define ID_TIMER_PLAY_CHK		14
#define ID_TIMER_MAINTOOL		20
#define ID_TIMER_MSGFORM		30
//#define ID_TIMER_RSFFORM		40
#define ID_TIMER_RSF_AUDIO		41
#define ID_TIMER_RSF_VIDEO		42
#define ID_TIMER_VIDEO			50
#define ID_TIMER_SCREENCAP		55
#define ID_TIMER_AVIMGR			60


#ifdef _DEBUG
#import ".\Debug\ActiveST.dll"  no_namespace, named_guids  
#else
#import ".\Release\ActiveST.dll"  no_namespace, named_guids  
#endif
//#import "c:\\windows\\system32\\ActiveS.dll"  no_namespace, named_guids  
//#import "c:\\windows\\system32\\skypeapi.dll"  no_namespace, named_guids  
//#import "c:\\windows\\system32\\skypeapi.dll" named_guids  
//using namespace SKYPEAPILib;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。
