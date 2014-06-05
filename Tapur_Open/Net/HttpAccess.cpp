// @(h) HttpAccess.cpp        ver 1.0 ( '99.2.28  )

// @(s)
// エラーハンドリングクラス メンバー関数定義
// エラーメッセージやログを処理する。
//

#include "stdafx.h"
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <share.h>
#include <fcntl.h>
#include "potm.h"
#include "HttpAccess.h"

_TCHAR errbuf[MAX_LEN];

static const char B64[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static char *postdata = NULL;
static int bad = 0;
static char *b64authInfo;


//
//  機能     : コンストラクタ
//  
//  機能説明 : 初期処理
//  
//  返り値   : なし
//  
//  備考     : 
//  
CHttpAccess::CHttpAccess()
{

	WORD wVersionRequested;
	int  nErrorStatus;
	WSADATA wsaData;

	/* WinSockの初期化を行う */
	wVersionRequested = MAKEWORD(1, 1);			/* バージョン 1.1 を要求する */
	nErrorStatus = WSAStartup(wVersionRequested, &wsaData);
	if (atexit((void (*)(void))(WSACleanup))) {		/* 終了時にWinSockのリソースを解放するようにしておく */
		fprintf(stderr,"atexit(WSACleanup)失敗＼n");
		exit(-1);
	}
	if ( nErrorStatus != 0 ) {
		fprintf(stderr,"WinSockの初期化失敗＼n");
		exit(-1);
	}
}

//
//  機能     : デストラクタ
//  
//  機能説明 : 終了処理
//  
//  返り値   : なし
//  
//  備考     : 
//  
CHttpAccess::~CHttpAccess()
{
	/* WinSockのリソースを解放します */
	WSACleanup();
}


int CHttpAccess::getHttpHeader(int s, respHead *rh)
{
	int i, k, linecnt;
	headField *p = NULL;
	char line[BUF_LEN];
	char *token;
	char *next_token = NULL;
	size_t sz;

	memset(rh, '\0', sizeof(respHead));
	memset(line, '\0', BUF_LEN);

	for(linecnt = 0, i = 0; linecnt <= HEAD_CNT;){
		if(recv(s,line+i,1, 0) == SOCKET_ERROR){
			TRACE(_T("Can't read HTTP header information.\n"));
			return (-1);
		}
		if( *(line+i) == ((char)('\n')) ){
			if( strcmp(line,"\r\n") == 0 ){
				break; // end of data
			}
mbstowcs_s(&sz, errbuf, MAX_LEN, line, 255);
TRACE(_T("line=%s\n"),errbuf);
			next_token = NULL;
			if(linecnt == 0){
				token = strtok_s(line, "/", &next_token);
				strcpy_s(rh->protocol, sizeof(rh->protocol)/sizeof(rh->protocol[0]), token);
				for(k = 0; k < 2; k++){
					token = strtok_s(NULL, " ", &next_token);
					if(k == 0) strcpy_s(rh->ver, sizeof(rh->ver)/sizeof(rh->ver[0]), token);
					if(k == 1) strcpy_s(rh->statusCode, sizeof(rh->statusCode)/sizeof(rh->statusCode[0]), token);
				}
				token = strtok_s(NULL, "\n", &next_token);
				strcpy_s(rh->status, sizeof(rh->status)/sizeof(rh->status[0]), token);
			} else {
				token = strtok_s(line, ":", &next_token);
				rh->hf_cnt = linecnt;
				p = (headField *)(rh->hf + rh->hf_cnt - 1);
				strcpy_s(p->header, sizeof(p->header)/sizeof(p->header[0]), token);
				while( token != NULL){
					token = strtok_s(NULL, "\n", &next_token);
					if(token != NULL) strcpy_s(p->content, sizeof(p->content)/sizeof(p->content[0]), token);
				}
			}
			i = 0;
			linecnt++;
			memset(line, '\0', BUF_LEN);
		} else {
			i++;
		}
	}
	
	if (linecnt > HEAD_CNT) {
		TRACE(_T("Can't read HTTP header information, header is too long.\n"));
		return (-1);
	}

	return (0);
}


char *CHttpAccess::getHeaderContent(respHead *rh, char *headerName)
{
	int i;
	headField *p;
	
	if (rh == NULL) return (NULL);

	if( 	 strcmp(headerName,"Protocol")	== 0) return rh->protocol;
	else if( strcmp(headerName,"Version")	== 0) return rh->ver;
	else if( strcmp(headerName,"StatusCode")== 0) return rh->statusCode;
	else if( strcmp(headerName,"Status") 	== 0) return rh->status;
	else {
		for(i = 0; i < rh->hf_cnt ; i++){
			p = (headField *)(rh->hf + i);
			if( strcmp(headerName,p->header) == 0){
				return (p->content);
			}
		}
	}

	return (NULL);
}


int CHttpAccess::getSrvInfo(char *host, unsigned short port)
{

	servhost = gethostbyname(host);
	if ( servhost == NULL ){
		TRACE(_T("Unknown host name:%s\n"), host);
		return (-1);
	}
        
	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	memmove((char *)&server.sin_addr, servhost->h_addr, servhost->h_length);
   
	if ( port != 0 ){
		server.sin_port = htons(port);
	}
	else {
		service = getservbyname("http", "tcp");
		if ( service != NULL ){
			server.sin_port = service->s_port;
		}	                
		else{
			server.sin_port = htons(80);
		}
	}

	return (0);	
}

int CHttpAccess::start_connect()
{
	int s;	

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0){
		TRACE(_T("Error creating socket.\n"));
		return (-1);
	}

	/* サーバにコネクトする */
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
		if(WSAGetLastError() != WSAEWOULDBLOCK){
			return (-1);
		}
		else {
			closesocket(s);
			if (bad++ > 10) {
				TRACE(_T("Connect aborted after 10 failures.\n"));
				return (-1);
			}
			start_connect();
		}
	}
	return (s);
}

void CHttpAccess::toBase64(char *plane, char *base64)
{
	int j=0;
	int l,i;

	l = strlen(plane);
	for (i=0;i<l;i+=3){
		base64[j]   =(char)(0==(plane[i] >> 2))
				? '=' :( B64[ plane[i] >> 2 ]);
		base64[j+1] =(char)(0==((plane[i] & 0x03)<<4 | (plane[i+1]>>4)))
				? '=' :( B64[ (plane[i] & 0x03)<<4 | (plane[i+1]>>4) ]);
		base64[j+2] =(char)(0==((plane[i+1] & 0x0f)<<2 | (plane[i+2] >>6)))
				? '=' :( B64[ (plane[i+1] & 0x0f)<<2 | (plane[i+2] >>6)]);
		base64[j+3] =(char)(0==(plane[i+2] & 0x3f))
				?'=':( B64[ plane[i+2] & 0x3f]);
		j += 4;
	}
	base64[j] = '\0';
}

char *CHttpAccess::createAuthInfo(char *name, char *passwd)
{
	
	static char base64[(NAME_LEN+PWD_LEN)*2];
	char buf[NAME_LEN + PWD_LEN + 4];
	
	if( strlen(name) > NAME_LEN || strlen(passwd) > PWD_LEN ) {
		TRACE(_T("Username or Password is too long.\n"));
		return (NULL);
	}
	memset(buf, 0, sizeof(buf));
	
	strcat_s(buf, sizeof(buf)/sizeof(buf[0]), name);
	strcat_s(buf, sizeof(buf)/sizeof(buf[0]), ":");
	strcat_s(buf, sizeof(buf)/sizeof(buf[0]), passwd);
	strcat_s(buf, sizeof(buf)/sizeof(buf[0]), "\n\n");
	
	toBase64(buf,base64);
	
	return (base64);

}


int CHttpAccess::HttpConnect(char *host, char *path, unsigned short port, char *name, char *pass)
{

	int so;
	char request[REQ_LEN];
	int reqlen;
	int icode;
	char *scode;
	respHead header;
	size_t sz;
	
	if( getSrvInfo(host, port) < 0 ) return (-1);
	if( (so = start_connect()) < 0 ) return (-1);

	if( !(b64authInfo = createAuthInfo(name, pass))  ){
		TRACE(_T("Can't create Authority Infomation.\n"));
		goto HttpConnect_ERR;
	}

	sprintf_s(request, REQ_LEN, "HEAD %s HTTP/1.1\r\n"
			 "Authorization: %s %s\r\n" 
			 "Host: %s\r\n"
			 "\r\n",
			 path, AUTH_TYPE, b64authInfo,host);
       
	reqlen = strlen(request);
	
mbstowcs_s(&sz, errbuf, MAX_LEN, request, 255);
TRACE(_T("request=%s\n"),errbuf);
	send(so,request,reqlen,0);

	if (getHttpHeader(so, &header) < 0) goto HttpConnect_ERR;
	scode  = getHeaderContent(&header,"StatusCode");
	icode  = atoi(scode);

	switch(icode){
	case AuthReq:
		TRACE(_T("Check user name or password:%s\n"),scode);
		//memset(b64authInfo,0,sizeof(b64authInfo));
		goto HttpConnect_ERR;
	case OK:
	case Forbidden:	// no target file, but connection is OK.
		closesocket(so);
		if( (so = start_connect()) < 0 ) return (-1);
		break;
	case NotFound:
		goto HttpConnect_ERR;
	default:
		TRACE(_T("%s:%s\n"),getHeaderContent(&header,"Status"),scode);
		goto HttpConnect_ERR;
	}

	return (so);

HttpConnect_ERR:
	closesocket(so);
	return (-1);
}

int CHttpAccess::HttpDisConnect(int sock){

	_close(sock);
	return (0);
}

int CHttpAccess::openPostFile(LPCTSTR filename, int *postlen)
{
	int postfd, status;
	struct _stat postfilestat;

	if (_tsopen_s(&postfd, filename, _O_RDONLY | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE )) {
		TRACE(_T("Invalid postfile name:%s\n"), filename);
		return (-1);
	}
	if ((status = _fstat(postfd, &postfilestat)) == -1) {
		TRACE(_T("Can't stat postfile.\n")); 
		return (-1);
	}
	postdata = (char *)malloc(postfilestat.st_size);
	if (postdata == NULL) {
		TRACE(_T("Can't alloc postfile buffer.\n"));
		return (-1);
	}
	if (_read(postfd, postdata, postfilestat.st_size) != postfilestat.st_size) {
		TRACE(_T("error reading postfile.\n"));
		return (-1);
	}
    
	*postlen = postfilestat.st_size;
	_close(postfd);	

	return (0);

}

int CHttpAccess::HttpUpLoad(int upSock, char *host, char *path, LPCTSTR filename){
	
	char request[REQ_LEN];
	int reqlen;
	int postlen = 0;
	size_t sz;

	respHead header;
	char *con;

	if( upSock < 0 ) return (-1);

	if( openPostFile(filename,&postlen) < 0 ) return (-1);

	sprintf_s(request, REQ_LEN, "PUT %s HTTP/1.1\r\n"
			"Authorization: %s %s\r\n"
			"User-Agent: PoClient/%s\r\n"
			"Connection: Keep-Alive\r\n"
			"Host: %s\r\n"
			"Accept: */*\r\n"
			"Content-length: %d\r\n"
			"Content-type: text/plain\r\n"
			"\r\n",
			path,
			AUTH_TYPE, b64authInfo,
			VERSION,
			host, postlen);
        
	reqlen = strlen(request);
        
mbstowcs_s(&sz, errbuf, MAX_LEN, request, 255);
TRACE(_T("request=%s\n"),errbuf);
	send(upSock,request,reqlen,0);
	send(upSock,postdata,postlen,0);

	if (postdata != NULL) {
		free(postdata);
		postdata = NULL;
	}

	if (getHttpHeader(upSock, &header) < 0) return (-1);
	if( (con = getHeaderContent(&header,"Connection")) != NULL ){
		if( strncmp(con, " close", strlen(con)-1) == 0 ){
			return (-1);
		}	
	}
	else{
		TRACE(_T("Can't get Connection.\n"));
		return (-1);
	}	

	return (0);
}

int CHttpAccess::readBodyAndSave(int sock, int bodyLen, char *filename)
{
	FILE *fp;
	int accum=0;
	char buf2[BUF_LEN];
	int read_size;

	if (fopen_s(&fp, filename,"wb")) {
		TRACE(_T("file open error:%s\n"),filename);
		fclose(fp);
		return (-1);
	}

	while (1){
		memset(buf2,0,BUF_LEN);
		read_size = recv(sock, buf2, BUF_LEN, 0);
		accum += read_size;		

		if ( accum < bodyLen ){
			fwrite(buf2, read_size, 1, fp);
		}
		else{
			fwrite(buf2, read_size, 1, fp);
			break;
		}
	}
	fclose(fp);
	return (0);
}



int CHttpAccess::HttpDownLoad(int downSock, char *host, char *path, char *filename)
{

	char request[REQ_LEN];
	int reqlen;
	respHead header;
	int code=0,len=0;	
	char *ghc;
	char *con;
	size_t sz;
	
	if( downSock < 0 ) return (-1);

	sprintf_s(request, REQ_LEN, "GET %s HTTP/1.1\r\n"
                         "Authorization: %s %s\r\n"
                         "Connection: Keep-Alive\r\n"
                         "Host: %s\r\n"
                         "\r\n",
                         path,
                         AUTH_TYPE, b64authInfo,host);

	reqlen = strlen(request);
	
mbstowcs_s(&sz, errbuf, MAX_LEN, request, 255);
TRACE(_T("request=%s\n"),errbuf);
	send(downSock,request,reqlen,0);

	if (getHttpHeader(downSock, &header) < 0) return (-1);
	if( (ghc = getHeaderContent(&header,"StatusCode")) == NULL ){
		TRACE(_T("Can't get StatusCode.\n"));
		return (-1);	
	}
	else{
		code = atoi( ghc );
	}


	switch(code){
	case OK:
		if( (ghc = getHeaderContent(&header,"Content-Length")) != NULL ){
			len = atoi( ghc );
			if( readBodyAndSave(downSock,len,filename) < 0 ){
				return (-1);
			}
		}
		else{
			TRACE(_T("Can't get Content-Lenght.\n"));
			return (-1);
		}
		break;	
			
	case NotFound:
//		err( getHeaderContent(&header,"Status"),path );
		return (-1);
		break;

	default:
		break;

	}	
	
	if( (con = getHeaderContent(&header,"Connection")) != NULL ){
		if(0 == strncmp(con," close",strlen(con)-1)){
			return (-1);
		}	
	}
	else{
		TRACE(_T("Can't get Connection.\n"));
		return (-1);
	}

	return (0);
}


int CHttpAccess::HttpDelete(int deleteSock, char *host, char *path)
{

	char request[REQ_LEN];
	int reqlen;
	respHead header;
	size_t sz;
	
	if( deleteSock < 0 ) return (-1);
	
	sprintf_s(request, REQ_LEN, "DELETE %s HTTP/1.1\r\n"
                         "Authorization: %s %s\r\n"
                         "Connection: Keep-Alive\r\n"
                         "Host: %s\r\n"
                         "\r\n",
                         path,
                         AUTH_TYPE, b64authInfo,host);

	reqlen = strlen(request);

mbstowcs_s(&sz, errbuf, MAX_LEN, request, 255);
TRACE(_T("request=%s\n"),errbuf);
	send(deleteSock,request,reqlen,0);
	if (getHttpHeader(deleteSock, &header) < 0) return (-1);

	return (0);

}

/*
	execute CGI
	no need Httpconnect() & Httpdisconnect()
*/
int CHttpAccess::HttpCgi(char *host, char *cgistr, char *cgiparam, unsigned short port, char *name, char *pass)
{

	int so;
	char request[REQ_LEN];
	int reqlen;
	int postlen;
	int icode;
	char *scode;
	respHead header;
	size_t sz;
	
	if( getSrvInfo(host, port) < 0 ) return (-1);
	if( (so = start_connect()) < 0 ) return (-1);

	if( !(b64authInfo = createAuthInfo(name, pass))  ){
		TRACE(_T("Can't create Authority Infomation.\n"));
		goto HttpCgi_ERR;
	}

	postlen = strlen(cgiparam);

	sprintf_s(request, REQ_LEN, "POST %s HTTP/1.1\r\n"
			"Authorization: %s %s\r\n"
			"User-Agent: PoClient/%s\r\n"
			"Connection: Keep-Alive\r\n"
			"Host: %s\r\n"
			"Accept: */*\r\n"
			"Content-length: %d\r\n"
			"Content-type: text/plain\r\n"
			"\r\n"
			"%s",
			cgistr,
			AUTH_TYPE, b64authInfo,
			VERSION,
			host, 
			postlen,
			cgiparam);

	reqlen = strlen(request);

mbstowcs_s(&sz, errbuf, MAX_LEN, request, 255);
TRACE(_T("request=%s\n"),errbuf);
	send(so,request,reqlen,0);

	if (getHttpHeader(so, &header) < 0) goto HttpCgi_ERR;
	scode  = getHeaderContent(&header,"StatusCode");
	icode  = atoi(scode);

	switch(icode){
	case AuthReq:
		TRACE(_T("Check user name or password:%s\n"), scode);
		//memset(b64authInfo,0,sizeof(b64authInfo));
		goto HttpCgi_ERR;
	case OK:
		break;
	case Forbidden:
	case NotFound:
		goto HttpCgi_ERR;
	default:
		TRACE(_T("%s:%s\n"), getHeaderContent(&header,"Status"),scode);
		goto HttpCgi_ERR;
	}

	_close(so);
	return (1);

HttpCgi_ERR:
	_close(so);
	return (-1);
}

/*
	get host & path string from URL.
	url:URL string
	host: buffer for host string.
	path :buffer for path string
	return -> 0:OK, -1:error
*/
int CHttpAccess::AnalyzeURL(const char *url, char *host, char *path)
{
	char *p, buff[BUF_LEN];
	char prot1[] = "http://";
	char prot2[] = "https://";

	if (strlen(url) == 0) return -1;

	// cut head
	if (strstr(url, prot1) != NULL) {
		strncpy_s(buff, BUF_LEN, url + strlen(prot1), BUF_LEN-1);
	} else if (strstr(url, prot2) != NULL) {
		strncpy_s(buff, BUF_LEN, url + strlen(prot2), BUF_LEN-1);
	} else if (*url == '/') {
		strncpy_s(buff, BUF_LEN, url + 1, BUF_LEN-1);
	}

	p= strchr(buff, '/');
	if (p == NULL) {
		strcpy_s(host, BUF_LEN, buff);
		strcpy_s(path, BUF_LEN, "/");
	} else {
		strcpy_s(path, BUF_LEN, p);
		*p = '\0';
		strcpy_s(host, BUF_LEN, buff);
	}
	
	return 0;
}

