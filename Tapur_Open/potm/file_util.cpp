/*

	file_util.c
	2002/10/17 -paak-

	file control utilities
*/
#include "Forcelib.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>	// tolower()
#include <share.h>
#include "potm.h"
#include "file_access.h"

static char errbuf[MAX_LEN];

/********************  string utility  *****************/

/*
	trim string (" ABC " -> "ABC")
*/
char *trim(char *s)
{
	int n;
	char *p;

	// trim bakward
	for (n = strlen(s); n > 0; n--) {
		if (isspace(s[n-1])) s[n-1] = '\0';
	}

	// trim forward
	p = s;
	while(isspace(s[0])) s++;
	if (s != p) {
		memmove(p, s, 1+strlen(s));
	}

	return p;
}

/*
	convert string to lower string ("ABC" -> "abc")
*/
char *tolowers(char *s)
{
	char *const p = s;

	while(*s) {
		*s = tolower(*s);
		s++;
	}

	return p;
}

/*
	input escape code into string.
	return -> 1:input escape code 
		  0:no need escape code
*/
int escstr(char *in_str, char *out_str)
{
	int i, len, ret = 0;
	char *p;

	len = strlen(in_str);
	for(i = 0; i < len; i++) {
		p = in_str + i;
		switch(*p) {
		case '$':
		case '\\':
		case '[':
		case ']':
		case '^':
		case '*':
			*out_str = '\\';
			out_str++;
			ret = 1;
		default:
			*out_str = *p;
			out_str++;
			break;
		}
	}
	*out_str = '\0';

	return ret;
}

/*
	get word from buffer
	abc def -> wrd[1]=abc wrd[2]=def
*/
int getword(char *str, char *keyword, char wrd[MAX_WORD_NUM][MAX_WORD_LEN])
{
	int wrd_num=0;
	char *token;
	char *next_token = NULL;
	char newstr[MAX_LEN];

	strcpy_s(newstr, MAX_LEN, str);
	token = strtok_s( newstr, keyword, &next_token );
	while( token != NULL )
	{
		wrd_num++;
		/* loop */
		if (wrd_num > MAX_WORD_NUM) return -1;
		sprintf_s(wrd[wrd_num], MAX_WORD_LEN, "%s" , token);
		/* get next */
		token = strtok_s( NULL, keyword, &next_token );
	}
	return wrd_num;
}


/********************  file utility  *****************/
/*
	file size
	return -> file size, 0:no exist
*/
long fsize(const char *fn)
{
	long size = 0;
	FILE *fin;
	if (fopen_s(&fin, fn, "rb")) return 0;

	// file size
	fseek(fin, 0, SEEK_END );
	size = ftell( fin );
	fclose( fin );

	return size;
}

/*
	file exist
	return -> 1:exist 0:no exist
*/
int fexist(const char *fn)
{
	int d = 1;
	FILE *fin;
	if (fopen_s(&fin, fn, "r")) {
		d = 0;
	} else {
		fclose(fin);
	}
	return d;
}

/*
	file copy with flock
	return -> 0:ok -1:error
*/
int fcopy(char *in_file, char *out_file)
{
	FILE *fp_in, *fp_out;
	char buf[MAX_LEN];
	int len;
	int ret=0;

	// open input file
	fp_in = _fsopen(in_file, "rb", _SH_DENYWR);
	if (fp_in == NULL) {
		sprintf_s(errbuf, MAX_LEN, "Can't open %s", in_file);
		LOG_ERR(errbuf);
		return -1;
	}

	// open output file
	fp_out = _fsopen(out_file, "wb", _SH_DENYRW);
	if (fp_out == NULL) {
		sprintf_s(errbuf, MAX_LEN, "Can't open %s", out_file);
		LOG_ERR(errbuf);
		fclose(fp_in);
		return -1;
	}

	// copy
	while((len = fread(buf, 1, MAX_LEN, fp_in)) > 0) {
		fwrite(buf, 1, len, fp_out);
	}

	// close
//	flock(fileno(fp_out), LOCK_UN);
	fclose(fp_out);
//	flock(fileno(fp_in), LOCK_UN);
	fclose(fp_in);

	return (ret);
}

/*
	get extension
	return -> pointer of extension, NULL:no extension
*/
char *getex(const char *fn)
{
	char *ex = (char *)strrchr(fn, '.');
	char *p =(char * )strrchr(fn, '/');
	if (ex == NULL || (p != NULL && p > ex)) return NULL;
	return ex+1;
}

/*
	delete extension
	return -> pointer of filename
*/
char *delex(char *fn)
{
	char *p = getex(fn);
	if (p != NULL) *(p-1) = '\0';
	return fn;
}

/*
	Get direcotry name.
	fn:full path filename, changed by this function.
	return -> pointer of directory name
*/
char *delfilename(char *fn)
{
	char *p = strrchr(fn, '/');
	if (p != NULL) *p = '\0';
	return fn;
}

/*
	Get file name.
	fn:full path filename.
	return -> pointer of file name
*/
char *deldirname(char *fn)
{
	char *p = strrchr(fn, '/');
	if (p != NULL) return p+1;
	return fn;
}

/*
	delet files in direcotry.
	folder:directory name.
	return -> 0:OK, -1:error, 1:no files
*/
/*
int cleardir(char *folder)
{
	int ret = 1;
	DIR *fp;
	struct dirent *p;
	char buf[MAX_LEN]="";

	fp=opendir(folder);
	if (fp == NULL) return -1;

	while((p=readdir(fp))!=NULL){
		if (p->d_ino == 0) continue;	// removed file
		if (p->d_ino == 2) continue;	// root
		if (strcmp(p->d_name, ".") == 0) continue;	// current directory
		if (strcmp(p->d_name, "..") == 0) continue;	// parent directory

		sprintf(buf, "%s/%s", folder, p->d_name);
		remove(buf);
		ret = 0;
	}

	closedir(fp);

	return ret;
}
*/

/*
	move all files from one direcotry to another.
	from_folder:directory name moved from.
	to_folder:directory name moved to.
	ext;extention, if NULL all files.
	return -> 0:OK, -1:error, 1:no files
*/
/*
int movefiles(char *from_folder, char *to_folder, char *ext)
{
	int ret = 1;
	DIR *fp;
	struct dirent *p;
	char buf1[MAX_LEN], buf2[MAX_LEN];

	fp=opendir(from_folder);
	if (fp == NULL) return -1;

	while((p=readdir(fp))!=NULL){
		if (p->d_ino == 0) continue;	// removed file
		if (p->d_ino == 2) continue;	// root
		if (strcmp(p->d_name, ".") == 0) continue;	// current directory
		if (strcmp(p->d_name, "..") == 0) continue;	// parent directory

		if (strlen(ext) == 0 || strcmp(getex(p->d_name), ext) == 0) {
			sprintf(buf1, "%s/%s", from_folder, p->d_name);
			sprintf(buf2, "%s/%s", to_folder, p->d_name);
			rename(buf1, buf2);
			ret = 0;
		}
	}

	closedir(fp);

	return ret;
}
*/

