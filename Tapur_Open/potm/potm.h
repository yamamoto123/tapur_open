/*
	
	potm.h main header file
	2002/02/21 -zdy-
	
*/
#ifndef __POTM_H
#define __POTM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
//#include <dlfcn.h>
#include "debug.h"

// database file name
#define DBFILE_NAME	"tapur.db"

/* format version, save in table */
#define POTM_APPNAME "Tapur"
#define POTM_VERSION "5.00"
#define POTM_VERSION_OLD1 "0.90"		// first Beta1 (don't delete this
#define POTM_VERSION_OLD2 "0.92"		// Beta2 (don't delete this
#define POTM_VERSION_OLD3 "0.95"		// Beta3 (don't delete this
#define POTM_VERSION_OLD4 "0.97"		// Beta4 (don't delete this
#define POTM_VERSION_OLD5 "0.99"		// Beta5 (don't delete this
#define POTM_VERSION_OLD6 "1.00"		// first Official (don't delete this
#define POTM_VERSION_OLD7 "1.20"		// Beta7 (don't delete this
#define POTM_VERSION_OLD8 "1.30"		// Beta8 (don't delete this
#define POTM_VERSION_OLD9 "2.00"		// Beta9 (don't delete this
#define POTM_VERSION_OLD10 "2.10"		// 2nd Official (don't delete this
#define POTM_VERSION_OLD11 "2.20"		// 3rd Official (don't delete this
#define POTM_VERSION_OLD12 "4.20"		// Beta (don't delete this
#define POTM_OPERATOR "Tapur.exe"

/* export of so */
//#define EXP_SO(type) type

/* file access */
#define TAPUR_DATA_PATH "\\Tapur\\data\\"
#define DOWNLOAD_PATH "\\download\\"
#define UPLOAD_PATH "\\upload\\"
#define TSF_PATH "\\set\\ref\\"
#define RSF_PATH "\\set\\ref\\"
#define RSF_TMP_PATH "\\set\\tmp\\"
#define RSF_WAV_PATH "\\set\\wav\\"
#define RSF_AVI_PATH "\\set\\avi\\"
#define RSF_IMG_PATH "\\set\\img\\"
#define RSF_TXT_PATH "\\set\\txt\\"
#define RSF_CSV_PATH "\\set\\csv\\"
#define RSF_GSM_PATH "\\set\\gsm\\"
#define RSF_ADPCM_PATH "\\set\\adpcm\\"
#define RDF_PATH "\\rec\\ref\\"
#define RDF_TMP_PATH "\\rec\\tmp\\"
#define RDF_WAV_PATH "\\rec\\wav\\"
#define RDF_MP3_PATH "\\rec\\mp3\\"
#define RDF_AVI_PATH "\\rec\\avi\\"
#define RDF_GSM_PATH "\\rec\\gsm\\"
#define RDF_ADPCM_PATH "\\rec\\adpcm\\"
#define RDF_TIFF_PATH "\\rec\\tiff\\"
#define RDF_PNG_PATH "\\rec\\png\\"
#define POTM_CONF_FILENAME "\\home\\pomato\\POTerminal\\conf\\potm.conf"
#define RDFL_FILENAME "\\list\\rdfl.lst"
#define TDFL_FILENAME "\\list\\tdfl.lst"
#define UFL_FILENAME "\\list\\ufl.lst"
#define TSF_FILENAME "tsf.ref"

// license file name
#define LICENSE_FILENAME	"license"

#ifndef MAX_PATH
#define MAX_PATH 260				/* max length of path name */
//#define MAX_PATH 1024				/* max length of path name */
#endif
#define MAX_LEN 1024				/* max length of buffer */
#define MAX_FILE 64					/* max length of file name without folder */
#define MIN_FILE 8					/* min length of file name without folder */
#define MAX_WORD_NUM 255			/* max words in a line */
#define MAX_WORD_LEN 1024			/* max length of a word */
#define MAX_DATE_LEN 16				/* max length of long date string */
#define MAX_BIN_NUM 32				/* max binary files with a rdf file */
#define CHK_RDFL_NUM 200			/* chk number of rdfl file */
#define CHK_TDFL_NUM 200			/* chk number of tdfl file */

/* module and handler */
#define MAX_MODULES 10				/* max modules */
#define MAX_HANDLES 255				/* max handlers of one module */
#define MAX_HANDLER_PARAM 20		/* parameter of handler */
#define MAX_HANDLER_PARAM_LEN 1024	/* max length of one parameter */

/* etc */
//#define TRUE 1
//#define FALSE 0

#endif	// __POTM_H

