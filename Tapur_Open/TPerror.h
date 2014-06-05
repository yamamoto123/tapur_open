// @(h) TPerror.h        ver 1.0 ( '99.2.28  )

// @(s)
// エラーハンドリングクラス定義
// エラーメッセージやログを処理する。
//

#ifndef __TPerror_H__
#define __TPerror_H__

#include <stdlib.h>		// _MAX_PATH

// エラーコード
#define ERR_TAPUR			0x10000
#define ERR_SKYPE			0x20000
#define ERR_WAVE			0x30000
#define ERR_AVI				0x40000
#define ERR_SOCK			0x50000
#define ERR_ACM				0x60000


// エラータイプ
#define ERRTYPE_ERROR		0x01	// エラー（ログ）
#define ERRTYPE_WARNING		0x02	// 警告（ログ）
#define ERRTYPE_INFO		0x04	// 警告（ログなし）
#define ERRTYPE_LOGONLY		0x08	// ログのみ

#define LOGDIR_NAME		_T("log")		// ログディレクトリ名称
#define LOG_EXPIRE		10				// ログ有効期間（days）

#define MAX_ERRSTR		1024

class CTPerror {	// エラーハンドリングクラス
private:
//	_TCHAR PrgDir[_MAX_PATH];	// 実行プロセスのディレクトリ（"\"付き）
	_TCHAR LogDir[_MAX_PATH];	// ログディレクトリ（"\"無し）
public:
	_TCHAR errstr[MAX_ERRSTR];		// ユーザが自由に使用できるバッファ(クラス内では未使用)

private:
	BOOL WriteLog(				// ログ書き込み
		const _TCHAR* str);	// ログメッセージ
public:
	CTPerror();		// コンストラクタ

	virtual void ErrorMessage(	// エラーメッセージ生成
		const byte	type,	// エラータイプ
		const _TCHAR* file,		// ソースファイル名
		const int line,		// エラー発生行
		const long code,	// エラーコード
		const _TCHAR* message);	// メッセージ
	bool DelOldLog();	// 古いログの削除
};

#endif	// __TPerror_H__

