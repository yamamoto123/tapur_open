// @(h) HttpAccess.h        ver 1.0 ( '99.2.28  )

// @(s)
// エラーハンドリングクラス定義
// エラーメッセージやログを処理する。
//

#ifndef __HttpAccess_H__
#define __HttpAccess_H__

#include <winsock.h>

#define HEAD_CNT 64	/* COUNT OF HEADER FEALD */
#define BUF_LEN 2048
#define REQ_LEN 2048
#define NAME_LEN 128	/* ユーザ名の長さ */
#define PWD_LEN 128	/* パスワードの長さ */
#define VERSION "1.0"
#define AUTH_TYPE "Basic"

#define OK 		200 
#define NotFound 	404 
#define AuthReq 	401 
#define Forbidden 	403 

#define HTTP_PORT	80

typedef struct _headField{
	char header[64];
	char content[64];
}headField;

typedef struct _respHead {
	char protocol[5];
	char ver[4];
	char statusCode[4];
	char status[64];
	int hf_cnt;
	headField hf[HEAD_CNT];
} respHead;


class CHttpAccess {	// エラーハンドリングクラス
private:
	hostent *servhost;	/* ホスト名とIPアドレスを扱うための構造体 */
	sockaddr_in server;	/* ソケットを扱うための構造体 */
	servent *service;	/* サービス (http等)を扱うための構造体 */

public:
	CHttpAccess();		// コンストラクタ
	~CHttpAccess();

private:
	int getHttpHeader(int s, respHead *rh);
	char *getHeaderContent(respHead *rh, char *headerName);
	int getSrvInfo(char *host, unsigned short port);
	int start_connect();
	void toBase64(char *plane, char *base64);
	char *createAuthInfo(char *name, char *passwd);
	int openPostFile(LPCTSTR filename, int *postlen);
	int readBodyAndSave(int sock, int bodyLen, char *filename);

public:
	int HttpConnect(char*, char*, unsigned short, char*, char*);
	int HttpUpLoad(int, char*, char*, LPCTSTR);
	int HttpDownLoad(int, char*, char*, char*);
	int HttpDisConnect(int);
	int HttpDelete(int, char*, char*);
	int HttpCgi(char*, char*, char*, unsigned short, char*, char*);
	int AnalyzeURL(const char *url, char *host, char *path);
};

#endif	// __HttpAccess_H__

