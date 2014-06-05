/*
	
	file_access.h for file_access.c 
	2002/02/17 -zdy-
	
*/
#ifndef __PO_FILE_ACCESS_H
#define __PO_FILE_ACCESS_H

#include "potm.h"
#include "spconfig.h"

/* version */
#define RDF_VERSION "0.01"

#define RDFL_FLAG_WAIT 0		/* wating transaction */
#define RDFL_FLAG_REUSE 9		/* reusable */

#define TDFL_FLAG_WAIT 0		/* wating transaction */
#define TDFL_FLAG_RETRY 1		/* wating retry transaction */
#define TDFL_FLAG_CGI 2			/* waiting for doing cgi to set in DB */
#define TDFL_FLAG_REUSE 9		/* reusable */

#define UFL_FLAG_TSF 1			/* TSF file */
#define UFL_FLAG_RSF 2			/* compressed RSF files */

// RDF received data type
#define RDF_TYPE_NOTHING	0
#define RDF_TYPE_AUDIO		1	// audio data
//#define RDF_TYPE_IMAGE		2	// image data
#define RDF_TYPE_VIDEO		3	// video data

/* for TSF, RSF, RDF structure */
#define MAX_VER_LEN 8				/* max length of version number string */
#define MAX_TEL_LEN 32				/* max length of telephone number string */
#define MAX_USRNAME_LEN 64			/* max length of username */
#define MAX_PASSWD_LEN 64			/* max length of password */
#define MAX_URL_LEN 256				/* max length of URL */
#define MAX_TYPE_LEN 64				/* max length of mime type string */
#define MAX_DTMF_LEN 64				/* max length of DTMF tone */
#define MAX_DTMFTERM_LEN 16			/* max length of DTMF term tone */
#define MAX_RSF_RESP 1024			/* max count of RSF setting reponse */
#define MAX_RDF_RESP 32				/* max count of RDF reponse */
#define MAX_FAX_PAGE 32				/* max page of one FAX */

// RSF & RDF response type
#define RESP_TYPE_NOTHING	0
#define RESP_TYPE_RECORD	1
#define RESP_TYPE_DTMF		2
#define RESP_TYPE_DISCONNECT	3
#define RESP_TYPE_PLAY		4
#define RESP_TYPE_HANDSET	5
#define RESP_TYPE_VRECORD	6
#define RESP_TYPE_EXEC		7
#define RESP_TYPE_LINK		90	// RSF only

// RSF response play type, conbination is possible
#define RESP_PLAY_TYPE_AUDIO	0x01
#define RESP_PLAY_TYPE_VIDEO	0x02
#define RESP_PLAY_TYPE_IMAGE	0x04
#define RESP_PLAY_TYPE_CHAT		0x08

// RSF binary file type
#define RESP_FILE_TYPE_BMP		0x01
#define RESP_FILE_TYPE_JPEG		0x02
#define RESP_FILE_TYPE_GIF		0x03
#define RESP_FILE_TYPE_WAVE		0x10
#define RESP_FILE_TYPE_AVI		0x20

// RSF base response type
#define RESPBASE_TYPE_NOTHING	0
#define RESPBASE_TYPE_SIMPLE	1	// simple response
#define RESPBASE_TYPE_NORMAL	2	// normal complex response

typedef struct {
	char pid_filename[MAX_PATH];
	int mod_cnt;					/* count of LoadModule */
	char mod_filename[MAX_MODULES][MAX_PATH];	/* filename of LoadModule */
	char device_filename[MAX_PATH];			/* device filename */
	int baud_rate;					/* DTE <-> DCE baud rate */
	char inter_telcode[8];			/* FAX:International Tel CODE */
	int fax_baud_rate;				/* FAX:DTE <-> DCE baud rate */
} ST_ConfFile;

typedef struct {
	char xml_filename[MAX_FILE];			/* filename of XML file */
	spConfigNode *root;				/* root node of XML tree */
} ST_XmlFile;


// binary file struct
typedef struct {
	char filename[MAX_FILE];			/* binary filename */
	char bin_type[MAX_TYPE_LEN];			/* binary type string */
	long size;					/* file size */
} ST_BIN_FILE;


typedef struct {
	char url[MAX_URL_LEN];				/* URL */
	char username[MAX_USRNAME_LEN];			/* username */
	char password[MAX_PASSWD_LEN];			/* password */
	int interval;					/* access interval(sec) */
} ST_TSF_SERVER;

typedef struct {
	int id;						/* telephony terminal ID, unique key */
	char version[MAX_VER_LEN];			/* TSF format version */
	char telno[MAX_TEL_LEN];			/* telephone number */
	int service;					/* 1:CTI only, 2:FAX only, 3:CTI & FAX */
	int tel_type;					/* 1:modem */
	int net_type;					/* 0:lan */
	ST_TSF_SERVER download;				/* download setting */
	ST_TSF_SERVER upload;				/* upload setting */
} ST_TSF;


typedef struct {
	char order[MAX_RDF_RESP];		/* response order, unique key */
	int type;						/* response type -> see definitions */
	char term[MAX_DTMFTERM_LEN];	/* termination tone character list */
	int play_type;					/* play type -> see definitions */
	ST_BIN_FILE play_audio_file;	/* play audio file */
	ST_BIN_FILE play_video_file;	/* play video file */
	ST_BIN_FILE play_image_file;	/* play image file */
	ST_BIN_FILE play_chat_file;		/* play chat file */
	int beep;						/* beep before response flag on(1), off(0) */
	int exec_id;					/* id for "exec" response type */
	int record_flag;				/* flag to record(1) or not(0).(when use handset only) */
	char link_order[MAX_RDF_RESP];	/* linked response order */
} ST_RSF_RESP;

typedef struct {
	char telno[MAX_TEL_LEN];			/* telephone number, unique key */
	char version[MAX_VER_LEN];			/* RSF format version */
	int resp_cnt;						/* count of response */
	ST_RSF_RESP resp[MAX_RSF_RESP];		/* response setting list */
} ST_RSF;


typedef struct {
	char order[MAX_RDF_RESP];	/* response order, unique key */
	int type;					/* response type -> see definitions */
	char rsf_order[MAX_RDF_RESP];	/* RSF response order (if linked response used, not same with order) */
	char term[2];					/* termination tone character */
	union {
		ST_BIN_FILE record_file;		/* recorded voice file */
		char dtmf[MAX_DTMF_LEN];		/* dtmf tone string */
	} data;
} ST_RDF_RESP;

typedef struct {
	int file_cnt;					/* count of fax file */
	ST_BIN_FILE fax_file[MAX_FAX_PAGE];		/* fax image file list */
} ST_FAX_DATA;

typedef struct {
	char id_date[MAX_DATE_LEN];		/* Date received data, unique key */
	char telno[MAX_TEL_LEN];		/* caller's telephone number */
	char version[MAX_VER_LEN];		/* RDF format version */
	int type;						/* received data type -> see definitions */
	ST_RSF *st_rsf;					/* RSF setting used by call response */
	int resp_cnt;					/* count of response */
	union {
		ST_RDF_RESP call_resp[MAX_RDF_RESP];	/* call response list */
		ST_FAX_DATA fax_data;					/* fax data */
	} data;
} ST_RDF;


typedef struct {
	char id_date[MAX_DATE_LEN];			/* Date received data, unique key */
	char rdf_filename[MAX_FILE];			/* filename of RDF file */
	int flag;					/* status -> see flag definitions */
	int bin_cnt;					/* count of binary filename */
	char bin_filename[MAX_BIN_NUM][MAX_FILE];	/* binary filename */
} ST_RDFL;

typedef struct {
	char id_date[MAX_DATE_LEN];			/* Date received data, unique key */
	char rdf_filename[MAX_FILE];			/* filename of RDF file */
	int flag;					/* status -> see flag definitions */
	char cmp_filename[MAX_FILE];			/* compressed filename */
	char send_date[MAX_DATE_LEN];			/* Date send data */
} ST_TDFL;

typedef struct {
	int flag;					/* type -> see flag definitions */
	char filename[MAX_FILE];			/* filename to download */
} ST_UFL;

//extern int writePIDFile(char *pid_filename, pid_t pid, char *mode);
extern int readConfFile(char *conf_filename, ST_ConfFile *st_confFile);
extern int newXML(ST_XmlFile *st_xml);
extern int freeXML(ST_XmlFile *st_xml);
extern int readXML(char *xml_filename, ST_XmlFile *st_xml);
extern int writeXML(char *xml_filename, ST_XmlFile *st_xml);
extern int cnvXML_TSF(ST_XmlFile *st_xml, ST_TSF *st_tsf);
extern int cnvXML_RSF(ST_XmlFile *st_xml, ST_RSF *st_rsf);
extern int cnvXML_RDF(ST_XmlFile *st_xml, ST_RDF *st_rdf);
extern int cnvRSF_XML(ST_RSF *st_rsf, ST_XmlFile *st_xml);
extern int cnvRDF_XML(ST_RDF *st_rdf, ST_XmlFile *st_xml);
extern ST_RDFL *loadRDFL(ST_RDFL *ary_rdfl, int *ary_cnt);
extern int saveRDFL(ST_RDFL *ary_rdfl, int arry_cnt);
extern int addRDFL(ST_RDFL *tmp_rdfl);
extern int cleanRDFL(void);
extern ST_TDFL *loadTDFL(ST_TDFL *ary_tdfl, int *ary_cnt);
extern int saveTDFL(ST_TDFL *ary_tdfl, int arry_cnt);
extern int addTDFL(ST_TDFL *tmp_tdfl);
extern int cleanTDFL(void);
extern ST_UFL *loadUFL(ST_UFL *ary_ufl, int *ary_cnt);

// fil_util.c
extern char *trim(char *s);
extern char *tolowers(char *s);
extern int escstr(char *in_str, char *out_str);
extern int getword(char *str, char *keyword, char wrd[MAX_WORD_NUM][MAX_WORD_LEN]);
extern long fsize(const char *fn);
extern int fexist(const char *fn);
extern int fcopy(char *in_file, char *out_file);
extern char *getex(const char *fn);
extern char *delex(char *fn);
extern char *delfilename(char *fn);
extern char *deldirname(char *fn);
//extern int cleardir(char *folder);
//extern int movefiles(char *from_folder, char *to_folder, char *ext);

// COMPRESS.C
//extern int compress(char *zfilename, int n, char filename[][MAX_PATH]);
//extern int decompress(char *zfilename, char *extdir);

// wave_convert.c
//extern int Adpcm_Wave(char *adpcm_filename, char *wave_filename);
//extern int Wave_Adpcm(char *wave_filename, char *adpcm_filename);

// tiff2jpeg.c
//extern int Tiff_Png(char *tiffname, char *pngname)

#endif	// __PO_FILE_ACCESS_H
