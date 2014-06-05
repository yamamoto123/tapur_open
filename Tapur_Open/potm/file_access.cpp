/*

	file_access.c
	2002/02/17 -zdy-

*/
#include "Forcelib.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <share.h>
#include "potm.h"
#include "file_access.h"
#include "Kakusi.h"
#include "spconfig.h"

static char errbuf[MAX_LEN];

// データが暗号化されているかチェックするためのパターン
#define REF_PAT1	"<rsf"
#define REF_PAT2	"<rdf"

/********************  gloval functions  *****************/
/*
	write pid to potm.pid file
*/
/*
int writePIDFile(char *pid_filename, pid_t pid, char *mode)
{
	FILE *pid_fp;
	int ret=-1;
	
	pid_fp=fopen(pid_filename, mode);
	if (pid_fp != NULL) {
		fprintf(pid_fp, "%d\n", pid);
		fflush(pid_fp);
		fclose(pid_fp);
	}

	return (ret);
}
*/

/*
	read conf file (../conf/potm.conf)
*/
int readConfFile(char *conf_filename, ST_ConfFile *st_confFile)
{
	FILE *pid_fp;
	char buf[MAX_LEN];
	char wrd1[MAX_WORD_LEN];
	char wrd[MAX_WORD_NUM][MAX_WORD_LEN];
	int ret=0, mod_cnt=0;

	if (fopen_s(&pid_fp, conf_filename, "r") == 0) {
 		while ( fgets( buf, MAX_LEN, pid_fp )!=NULL) {
 			/* cut the "\n" from buf */
 			buf[strlen(buf)-1]=0;
 			/* search "=" */
 			if (getword(buf, "=", wrd)==2) {
 				/* get rid of space */
 				sscanf_s(wrd[1],"%s", wrd1, sizeof(wrd1));
 				/* path of pid file */
 				if (strcmp(wrd1, "potm_pid_file")==0) {
 					sscanf_s(wrd[2],"%s", st_confFile->pid_filename, sizeof(st_confFile->pid_filename));
					sprintf_s(errbuf, MAX_LEN, "pid_filename=%s", st_confFile->pid_filename);
					LOG_FA(errbuf);
				}
 				/* device filename */
 				if (strcmp(wrd1, "Device")==0) {
 					sscanf_s(wrd[2],"%s", st_confFile->device_filename, sizeof(st_confFile->device_filename));
 					sprintf_s(errbuf, MAX_LEN, "device_filename=%s", st_confFile->device_filename);
					LOG_FA(errbuf);
				}
 				/* DTE <-> DCE baud rate */
 				if (strcmp(wrd1, "baud_rate")==0) {
					st_confFile->baud_rate = atoi(wrd[2]);
					sprintf_s(errbuf, MAX_LEN, "baud_rate=%d", st_confFile->baud_rate);
					LOG_FA(errbuf);
				}
				/* LoadModule command */
				if (strcmp(wrd1, "LoadModule")==0) {
 					sscanf_s(wrd[2],"%s", st_confFile->mod_filename[mod_cnt], MAX_PATH);
 					sprintf_s(errbuf, MAX_LEN, "LoadModule=%s", st_confFile->mod_filename[mod_cnt]);
					LOG_FA(errbuf);
 					mod_cnt++;
				}
 				/* FAX:International Tel CODE */
 				if (strcmp(wrd1, "inter_telcode")==0) {
 					sscanf_s(wrd[2],"%s", st_confFile->inter_telcode, sizeof(st_confFile->inter_telcode));
 					sprintf_s(errbuf, MAX_LEN, "inter_telcode=%s", st_confFile->inter_telcode);
					LOG_FA(errbuf);
				}
 				/* FAX:DTE <-> DCE baud rate */
 				if (strcmp(wrd1, "fax_baud_rate")==0) {
					st_confFile->fax_baud_rate = atoi(wrd[2]);
					sprintf_s(errbuf, MAX_LEN, "fax_baud_rate=%d", st_confFile->fax_baud_rate);
					LOG_FA(errbuf);
				}
			}
		}
		st_confFile->mod_cnt=mod_cnt;
		fclose(pid_fp);
	} else {
		ret=-1;
	}

	return (ret);
}

/*
	allocate memory for XML structure
	return -> 0:ok -1:error
*/
int newXML(ST_XmlFile *st_xml)
{
	int ret=0;

	if (st_xml->root == NULL) {
		st_xml->root = spConfigNodeNew();
		if (st_xml->root == NULL) {
			LOG_ERR("spConfigNodeNew");
			return -1;
		}
	}
	return (ret);
}

/*
	free memory for XML structure
	return -> 0:ok -1:error
*/
int freeXML(ST_XmlFile *st_xml)
{
	int ret=0;

	if (st_xml->root != NULL) {
		spConfigNodeFree(st_xml->root);
	}
	return (ret);
}

/*
	read XML file to XML structure
	return -> 0:ok -1:error
*/
int readXML(char *xml_filename, ST_XmlFile *st_xml)
{
	int ret=0;
	long size = 0;
	FILE *fp;
	char *buf;

	if (st_xml->root == NULL) {
		LOG_ERR("root node is NULL");
		return -1;
	}

	/// ファイルサイズチェック
	size = fsize(xml_filename);
	if (size <= 0) return -1;

	/// ファイル内容保存メモリ
	buf = (char *)calloc(size, 1);	// spConfigLoadStrでfreeされる

	/// 暗号化されているかチェック
	if (fopen_s(&fp, xml_filename, "rb")) return -1;
	fread(buf, sizeof(char), 4, fp);
	fclose(fp);
	if (memcmp(buf, REF_PAT1, strlen(REF_PAT1)) == 0 ||
		memcmp(buf, REF_PAT2, strlen(REF_PAT2)) == 0) {
		//// 暗号化されていない
		fopen_s(&fp, xml_filename, "rb");
		fread(buf, 1, size, fp);
		fclose(fp);
	} else {
		//// 暗号化されている
		//// ファイル内容の復号化
		ModosiFile(xml_filename, (unsigned char *)buf);
	}

	/// バッファ内容を構造体に変換
#ifdef _DEBUG
	FILE *fin;
	fopen_s(&fin, "C:\\Users\\paak\\AppData\\Roaming\\Tapur\\log\\spconfig.log", "w");
	DEBUGFD_spconfig(fin);
#endif
	if (spConfigLoadStr(st_xml->root, xml_filename, buf) != 0) {
		sprintf_s(errbuf, MAX_LEN, "some errors in [%s]", xml_filename);
		LOG_ERR(errbuf);
		return -1;
	}
#ifdef _DEBUG
	fclose(fin);
#endif
	strncpy_s(st_xml->xml_filename, MAX_FILE, xml_filename, MAX_FILE-1);

	return (ret);
}

/*
	write XML file from XML structure
	return -> 0:ok -1:error
*/
int writeXML(char *xml_filename, ST_XmlFile *st_xml)
{
	int ret=0;
	char *str = NULL;
//FILE *fp;

	if (st_xml->root == NULL) {
		LOG_ERR("root node is NULL");
		return -1;
	}

	str = spConfigSaveStr(st_xml->root);
	if (str == NULL) {
//	if (spConfigSave(st_xml->root,xml_filename) != 0) {
		LOG_ERR("some errors in st_xml structure");
		return -1;
	}

//fp = fopen("C:\\test.log", "w");
//fputs(str, fp);
//fclose(fp);

	// 暗号化ファイル保存
	KakusiFile(xml_filename, (unsigned char *)str, strlen(str));
	free(str);	// spConfigSaveStr()でallocされたメモリ

	strncpy_s(st_xml->xml_filename, MAX_FILE, xml_filename, MAX_FILE-1);

	return (ret);
}

/*
	convert XML structure to TSF structure
	return -> 0:ok -1:error
*/
int cnvXML_TSF(ST_XmlFile *st_xml, ST_TSF *st_tsf)
{
	spConfigNode *root, *node;
	spConfigAttribute *atrb;
	int val = 0;
	int ret=0;

	memset((void *)st_tsf, '\0', sizeof(ST_TSF));

	if (st_xml == NULL) {
		LOG_ERR("ST_XmlFile structure is NULL");
		return -1;
	}
	if (st_xml->root == NULL) {
		LOG_ERR("ST_XmlFile structure root member is NULL");
		return -1;
	}

	root = st_xml->root;
	if(root->children == 0) {
		LOG_ERR("error in TSF file");
		return -1;
	}

	// <tsf>
	node = spConfigNodeFind(root, "tsf", 1, NULL);
	if(node == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	}
	atrb = spConfigAttributeFind(node, "id", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	}
	st_tsf->id = atoi(atrb->value.string);
	atrb = spConfigAttributeFind(node, "version", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	}
	strncpy_s(st_tsf->version, MAX_VER_LEN, atrb->value.string, MAX_VER_LEN-1);
	atrb = spConfigAttributeFind(node, "telno", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	}
	strncpy_s(st_tsf->telno, MAX_TEL_LEN, atrb->value.string, MAX_TEL_LEN-1);

	// <service>
	val = 0;
	node = spConfigNodeFind(root, "service", 1, NULL);
	if(node != NULL) {
		if (strcmp(tolowers(node->text), "call") == 0) {
			val |= 1;
		} else if (strcmp(tolowers(node->text), "fax") == 0) {
			val |= 2;
		}
		node = spConfigNodeFind(root, "service", 1, node);
		if(node != NULL) {
			if (strcmp(tolowers(node->text), "call") == 0) {
				val |= 1;
			} else if (strcmp(tolowers(node->text), "fax") == 0) {
				val |= 2;
			}
		}
	}
	st_tsf->service = val;

	// <telephone>
	node = spConfigNodeFind(root, "telephone", 1, NULL);
	if(node == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	}
	atrb = spConfigAttributeFind(node, "type", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	} else {
		if (strcmp(tolowers(atrb->value.string), "modem") == 0) {
			st_tsf->tel_type = 1;
		}
	}

	// <network>
	node = spConfigNodeFind(root, "network", 1, NULL);
	if(node == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	}
	atrb = spConfigAttributeFind(node, "type", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	} else {
		if (strcmp(tolowers(atrb->value.string), "ether") == 0) {
			st_tsf->net_type = 0;
		}
	}

	// <download>
	node = spConfigNodeFind(root, "download", 1, NULL);
	if(node == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	}
	atrb = spConfigAttributeFind(node, "interval", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	} else {
		st_tsf->download.interval = atoi(atrb->value.string);
	}
	atrb = spConfigAttributeFind(node, "username", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	} else {
		strncpy_s(st_tsf->download.username, MAX_USRNAME_LEN, atrb->value.string, MAX_USRNAME_LEN-1);
	}
	atrb = spConfigAttributeFind(node, "password", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	} else {
		strncpy_s(st_tsf->download.password, MAX_PASSWD_LEN, atrb->value.string, MAX_PASSWD_LEN-1);
	}
	atrb = spConfigAttributeFind(node, "url", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	} else {
		strncpy_s(st_tsf->download.url, MAX_URL_LEN, atrb->value.string, MAX_URL_LEN-1);
	}

	// <upload>
	node = spConfigNodeFind(root, "upload", 1, NULL);
	if(node == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	}
	atrb = spConfigAttributeFind(node, "interval", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	} else {
		st_tsf->upload.interval = atoi(atrb->value.string);
	}
	atrb = spConfigAttributeFind(node, "username", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	} else {
		strncpy_s(st_tsf->upload.username, MAX_USRNAME_LEN, atrb->value.string, MAX_USRNAME_LEN-1);
	}
	atrb = spConfigAttributeFind(node, "password", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	} else {
		strncpy_s(st_tsf->upload.password, MAX_PASSWD_LEN, atrb->value.string, MAX_PASSWD_LEN-1);
	}
	atrb = spConfigAttributeFind(node, "url", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in TSF file");
		return -1;
	} else {
		strncpy_s(st_tsf->upload.url, MAX_URL_LEN, atrb->value.string, MAX_URL_LEN-1);
	}

	return (ret);
}

/*
	convert XML structure to RSF structure
	return -> 0:ok -1:error
*/
int cnvXML_RSF(ST_XmlFile *st_xml, ST_RSF *st_rsf)
{
	spConfigNode *root, *node, *prev, *tmp_node;
	spConfigAttribute *atrb;
	ST_RSF_RESP *resp;
	ST_BIN_FILE *binf;
	int i;
	int ret=0;

	memset((void *)st_rsf, '\0', sizeof(ST_RSF));

	if (st_xml == NULL) {
		LOG_ERR("ST_XmlFile structure is NULL");
		return -1;
	}
	if (st_xml->root == NULL) {
		LOG_ERR("ST_XmlFile structure root member is NULL");
		return -1;
	}

	root = st_xml->root;
	if(root->children == 0) {
		LOG_ERR("error in RSF file");
		return -1;
	}

	// <rsf>
	node = spConfigNodeFind(root, "rsf", 1, NULL);
	if(node == NULL) {
		LOG_ERR("error in RSF file");
		return -1;
	}
	atrb = spConfigAttributeFind(node, "telno", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in RSF file");
		return -1;
	}
	strncpy_s(st_rsf->telno, MAX_TEL_LEN, atrb->value.string, MAX_TEL_LEN-1);
	atrb = spConfigAttributeFind(node, "version", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in RSF file");
		return -1;
	}
	strncpy_s(st_rsf->version, MAX_VER_LEN, atrb->value.string, MAX_VER_LEN-1);

	// <response>
	prev = NULL;
	for (i = 0; i < MAX_RSF_RESP; i++) {
		resp = st_rsf->resp + i;
		node = spConfigNodeFind(root, "response", 1, prev);
		if(node == NULL) {
			break;	// no more response
		}
		st_rsf->resp_cnt = i+1;
		prev = node;	// for next search
		atrb = spConfigAttributeFind(node, "order", 0, NULL);
		if(atrb == NULL) {
			LOG_ERR("error in RSF file");
			return -1;
		}
		strncpy_s(resp->order, MAX_RDF_RESP, atrb->value.string, MAX_RDF_RESP-1);
		atrb = spConfigAttributeFind(node, "type", 0, NULL);
		if(atrb == NULL) {
			LOG_ERR("error in RSF file");
			return -1;
		}
		if (strcmp(tolowers(atrb->value.string), "record") == 0) {
			resp->type = RESP_TYPE_RECORD;
			node = spConfigNodeFind(node, "record", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			atrb = spConfigAttributeFind(node, "dtmfterm", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			strncpy_s(resp->term, MAX_DTMFTERM_LEN, atrb->value.string, MAX_DTMFTERM_LEN-1);
			atrb = spConfigAttributeFind(node, "beep", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			resp->beep = atoi(atrb->value.string);
		} else if (strcmp(tolowers(atrb->value.string), "vrecord") == 0) {
			resp->type = RESP_TYPE_VRECORD;
			node = spConfigNodeFind(node, "vrecord", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			atrb = spConfigAttributeFind(node, "dtmfterm", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			strncpy_s(resp->term, MAX_DTMFTERM_LEN, atrb->value.string, MAX_DTMFTERM_LEN-1);
			atrb = spConfigAttributeFind(node, "beep", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			resp->beep = atoi(atrb->value.string);
		} else if (strcmp(tolowers(atrb->value.string), "exec") == 0) {
			resp->type = RESP_TYPE_EXEC;
			node = spConfigNodeFind(node, "exec", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			atrb = spConfigAttributeFind(node, "beep", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			resp->beep = atoi(atrb->value.string);
			atrb = spConfigAttributeFind(node, "id", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			resp->exec_id = atoi(atrb->value.string);
		} else if (strcmp(tolowers(atrb->value.string), "dtmf") == 0) {
			resp->type = RESP_TYPE_DTMF;
			node = spConfigNodeFind(node, "dtmf", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			atrb = spConfigAttributeFind(node, "dtmfterm", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			strncpy_s(resp->term, MAX_DTMFTERM_LEN, atrb->value.string, MAX_DTMFTERM_LEN-1);
			atrb = spConfigAttributeFind(node, "beep", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			resp->beep = atoi(atrb->value.string);
		} else if (strcmp(tolowers(atrb->value.string), "disconnect") == 0) {
			resp->type = RESP_TYPE_DISCONNECT;
			node = spConfigNodeFind(node, "disconnect", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			atrb = spConfigAttributeFind(node, "beep", 0, NULL);
			if(atrb) {
				resp->beep = atoi(atrb->value.string);	// ver0950
			}
		} else if (strcmp(tolowers(atrb->value.string), "play") == 0) {
			resp->type = RESP_TYPE_PLAY;
			node = spConfigNodeFind(node, "play", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			atrb = spConfigAttributeFind(node, "dtmfterm", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			strncpy_s(resp->term, MAX_DTMFTERM_LEN, atrb->value.string, MAX_DTMFTERM_LEN-1);
			atrb = spConfigAttributeFind(node, "beep", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			resp->beep = atoi(atrb->value.string);
			// audio file
			tmp_node = node;
			node = spConfigNodeFind(tmp_node, "audio", 1, NULL);
			if(node) {
				binf = &(resp->play_audio_file);
				atrb = spConfigAttributeFind(node, "src", 0, NULL);
				if(atrb == NULL) {
					LOG_ERR("error in RSF file");
					return -1;
				}
				strncpy_s(binf->filename, MAX_FILE, atrb->value.string, MAX_FILE-1);
				atrb = spConfigAttributeFind(node, "type", 0, NULL);
				if(atrb == NULL) {
					LOG_ERR("error in RSF file");
					return -1;
				}
				strncpy_s(binf->bin_type, MAX_TYPE_LEN, atrb->value.string, MAX_TYPE_LEN-1);	
				atrb = spConfigAttributeFind(node, "size", 0, NULL);
				if(atrb == NULL) {
					LOG_ERR("error in RSF file");
					return -1;
				}
				binf->size = atol(atrb->value.string);
				resp->play_type |= RESP_PLAY_TYPE_AUDIO;
			}
			node = spConfigNodeFind(tmp_node, "video", 1, NULL);
			if(node) {
				binf = &(resp->play_video_file);
				atrb = spConfigAttributeFind(node, "src", 0, NULL);
				if(atrb == NULL) {
					LOG_ERR("error in RSF file");
					return -1;
				}
				strncpy_s(binf->filename, MAX_FILE, atrb->value.string, MAX_FILE-1);
				atrb = spConfigAttributeFind(node, "type", 0, NULL);
				if(atrb == NULL) {
					LOG_ERR("error in RSF file");
					return -1;
				}
				strncpy_s(binf->bin_type, MAX_TYPE_LEN, atrb->value.string, MAX_TYPE_LEN-1);	
				atrb = spConfigAttributeFind(node, "size", 0, NULL);
				if(atrb == NULL) {
					LOG_ERR("error in RSF file");
					return -1;
				}
				binf->size = atol(atrb->value.string);
				resp->play_type |= RESP_PLAY_TYPE_VIDEO;
			}
			node = spConfigNodeFind(tmp_node, "image", 1, NULL);
			if(node) {
				binf = &(resp->play_image_file);
				atrb = spConfigAttributeFind(node, "src", 0, NULL);
				if(atrb == NULL) {
					LOG_ERR("error in RSF file");
					return -1;
				}
				strncpy_s(binf->filename, MAX_FILE, atrb->value.string, MAX_FILE-1);
				atrb = spConfigAttributeFind(node, "type", 0, NULL);
				if(atrb == NULL) {
					LOG_ERR("error in RSF file");
					return -1;
				}
				strncpy_s(binf->bin_type, MAX_TYPE_LEN, atrb->value.string, MAX_TYPE_LEN-1);	
				atrb = spConfigAttributeFind(node, "size", 0, NULL);
				if(atrb == NULL) {
					LOG_ERR("error in RSF file");
					return -1;
				}
				binf->size = atol(atrb->value.string);
				resp->play_type |= RESP_PLAY_TYPE_IMAGE;
			}
			node = spConfigNodeFind(tmp_node, "chat", 1, NULL);
			if(node) {
				binf = &(resp->play_chat_file);
				atrb = spConfigAttributeFind(node, "src", 0, NULL);
				if(atrb == NULL) {
					LOG_ERR("error in RSF file");
					return -1;
				}
				strncpy_s(binf->filename, MAX_FILE, atrb->value.string, MAX_FILE-1);
				atrb = spConfigAttributeFind(node, "type", 0, NULL);
				if(atrb == NULL) {
					LOG_ERR("error in RSF file");
					return -1;
				}
				strncpy_s(binf->bin_type, MAX_TYPE_LEN, atrb->value.string, MAX_TYPE_LEN-1);	
				atrb = spConfigAttributeFind(node, "size", 0, NULL);
				if(atrb == NULL) {
					LOG_ERR("error in RSF file");
					return -1;
				}
				binf->size = atol(atrb->value.string);
				resp->play_type |= RESP_PLAY_TYPE_CHAT;
			}
		} else if (strcmp(tolowers(atrb->value.string), "handset") == 0) {
			resp->type = RESP_TYPE_HANDSET;
			node = spConfigNodeFind(node, "handset", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			atrb = spConfigAttributeFind(node, "record_flag", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			if (strcmp(tolowers(atrb->value.string), "true") == 0) {
				resp->record_flag = 1;
			} else {
				resp->record_flag = 0;
			}
		} else if (strcmp(tolowers(atrb->value.string), "link") == 0) {
			resp->type = RESP_TYPE_LINK;
			node = spConfigNodeFind(node, "link", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			atrb = spConfigAttributeFind(node, "order", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RSF file");
				return -1;
			}
			strncpy_s(resp->link_order, MAX_RDF_RESP, atrb->value.string, MAX_RDF_RESP-1);
		}
	}
	if(i >= MAX_RSF_RESP) {
		LOG_ERR("error in RSF file.response setting are too many");
		return -1;
	}

	return (ret);
}

/*
	convert XML structure to RDF structure
	return -> 0:ok -1:error
*/
int cnvXML_RDF(ST_XmlFile *st_xml, ST_RDF *st_rdf)
{
	spConfigNode *root, *node, *prev;
	spConfigAttribute *atrb;
	ST_RDF_RESP *resp;
	int i;
	int ret=0;

	memset((void *)st_rdf, '\0', sizeof(ST_RDF));

	if (st_xml == NULL) {
		LOG_ERR("ST_XmlFile structure is NULL");
		return -1;
	}
	if (st_xml->root == NULL) {
		LOG_ERR("ST_XmlFile structure root member is NULL");
		return -1;
	}

	root = st_xml->root;
	if(root->children == 0) {
		LOG_ERR("error in RDF file");
		return -1;
	}

	// <rdf>
	node = spConfigNodeFind(root, "rdf", 1, NULL);
	if(node == NULL) {
		LOG_ERR("error in RDF file");
		return -1;
	}
	atrb = spConfigAttributeFind(node, "telno", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in RDF file");
		return -1;
	}
	strncpy_s(st_rdf->telno, MAX_TEL_LEN, atrb->value.string, MAX_TEL_LEN-1);
	atrb = spConfigAttributeFind(node, "version", 0, NULL);
	if(atrb == NULL) {
		LOG_ERR("error in RDF file");
		return -1;
	}
	strncpy_s(st_rdf->version, MAX_VER_LEN, atrb->value.string, MAX_VER_LEN-1);

	// <response>
	prev = NULL;
	for (i = 0; i < MAX_RDF_RESP; i++) {
		resp = st_rdf->data.call_resp + i;
		node = spConfigNodeFind(root, "response", 1, prev);
		if(node == NULL) {
			break;	// no more response
		}
		st_rdf->resp_cnt = i+1;
		prev = node;	// for next search
		atrb = spConfigAttributeFind(node, "order", 0, NULL);
		if(atrb == NULL) {
			LOG_ERR("error in RDF file");
			return -1;
		}
		strncpy_s(resp->order, MAX_RDF_RESP, atrb->value.string, MAX_RDF_RESP-1);
		atrb = spConfigAttributeFind(node, "type", 0, NULL);
		if(atrb == NULL) {
			LOG_ERR("error in RDF file");
			return -1;
		}
		if (strcmp(tolowers(atrb->value.string), "record") == 0) {
			resp->type = RESP_TYPE_RECORD;
			node = spConfigNodeFind(node, "record", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RDF file");
				return -1;
			}
			atrb = spConfigAttributeFind(node, "dtmfterm", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RDF file");
				return -1;
			}
		} else if (strcmp(tolowers(atrb->value.string), "dtmf") == 0) {
			resp->type = RESP_TYPE_DTMF;
			node = spConfigNodeFind(node, "dtmf", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RDF file");
				return -1;
			}
			atrb = spConfigAttributeFind(node, "dtmfterm", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RDF file");
				return -1;
			}
		} else if (strcmp(tolowers(atrb->value.string), "disconnect") == 0) {
			resp->type = RESP_TYPE_DISCONNECT;
			node = spConfigNodeFind(node, "disconnect", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RDF file");
				return -1;
			}
		} else if (strcmp(tolowers(atrb->value.string), "play") == 0) {
			resp->type = RESP_TYPE_PLAY;
			node = spConfigNodeFind(node, "play", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RDF file");
				return -1;
			}
			atrb = spConfigAttributeFind(node, "dtmfterm", 0, NULL);
			if(atrb == NULL) {
				LOG_ERR("error in RDF file");
				return -1;
			}
		} else if (strcmp(tolowers(atrb->value.string), "handset") == 0) {
			resp->type = RESP_TYPE_HANDSET;
			node = spConfigNodeFind(node, "handset", 1, NULL);
			if(node == NULL) {
				LOG_ERR("error in RDF file");
				return -1;
			}
		}
	}
	if(i >= MAX_RDF_RESP) {
		LOG_ERR("error in RDF file.response setting are too many");
		return -1;
	}

	return (ret);
}

/*
	convert RSF structure to XML structure
	return -> 0:ok -1:error
*/
int cnvRSF_XML(ST_RSF *st_rsf, ST_XmlFile *st_xml)
{
	spConfigNode *root, *node, *parent;
	ST_RSF_RESP *resp;
	ST_BIN_FILE *binf;
	char buf[MAX_LEN];
	int i;
	int ret=0;

	if (st_rsf == NULL) {
		LOG_ERR("ST_rsfFile structure is NULL");
		return -1;
	}

	root = st_xml->root;
	if (root == NULL) {
		LOG_ERR("ST_XmlFile structure root member is NULL");
		goto cnvRSF_XML_ERR;
	}

	// <rsf>
	node = spConfigNodeNew();
	if (node == NULL) {
		LOG_ERR("out of memory in XML function");
		goto cnvRSF_XML_ERR;
	}
	spConfigNodeName(node,"rsf");
	if (strlen(st_rsf->telno) == 0) {
		LOG_ERR("error in RSF structure");
		goto cnvRSF_XML_ERR;
	}
	if (spConfigNodeAddAttributeString(node,"telno",st_rsf->telno) != 0) {
		LOG_ERR("error in XML function");
		goto cnvRSF_XML_ERR;
	}
	if (strlen(st_rsf->version) == 0) {
		LOG_ERR("error in RSF structure");
		goto cnvRSF_XML_ERR;
	}
	if (spConfigNodeAddAttributeString(node,"version",st_rsf->version) != 0) {
		LOG_ERR("error in XML function");
		goto cnvRSF_XML_ERR;
	}
	if (spConfigNodeAddChild(root,node) != 0) {
		LOG_ERR("error in XML function");
		goto cnvRSF_XML_ERR;
	}
	root = node;

	// <response> tag list
	if (st_rsf->resp_cnt <= 0 || st_rsf->resp_cnt > MAX_RSF_RESP) {
		LOG_ERR("error in RSF structure");
		goto cnvRSF_XML_ERR;
	}
	for (i = 0; i < st_rsf->resp_cnt; i++) {
		resp = st_rsf->resp + i;
		// <response>
		node = spConfigNodeNew();
		if (node == NULL) {
			LOG_ERR("out of memory in XML function");
			goto cnvRSF_XML_ERR;
		}
		spConfigNodeName(node,"response");
		if (strlen(resp->order) == 0) {
			LOG_ERR("error in RSF structure");
			goto cnvRSF_XML_ERR;
		}
		if (spConfigNodeAddAttributeString(node,"order",resp->order) != 0) {
			LOG_ERR("error in XML function");
			goto cnvRSF_XML_ERR;
		}
		if (resp->type == RESP_TYPE_RECORD) {
			strcpy_s(buf, MAX_LEN, "record");
		} else if (resp->type == RESP_TYPE_VRECORD) {
			strcpy_s(buf, MAX_LEN, "vrecord");
		} else if (resp->type == RESP_TYPE_EXEC) {
			strcpy_s(buf, MAX_LEN, "exec");
		} else if (resp->type == RESP_TYPE_DTMF) {
			strcpy_s(buf, MAX_LEN, "dtmf");
		} else if (resp->type == RESP_TYPE_DISCONNECT) {
			strcpy_s(buf, MAX_LEN, "disconnect");
		} else if (resp->type == RESP_TYPE_PLAY) {
			strcpy_s(buf, MAX_LEN, "play");
		} else if (resp->type == RESP_TYPE_HANDSET) {
			strcpy_s(buf, MAX_LEN, "handset");
		} else if (resp->type == RESP_TYPE_LINK) {
			strcpy_s(buf, MAX_LEN, "link");
		} else {
			LOG_ERR("error in RSF structure");
			goto cnvRSF_XML_ERR;
		}
		if (spConfigNodeAddAttributeString(node,"type",buf) != 0) {
			LOG_ERR("error in XML function");
			goto cnvRSF_XML_ERR;
		}
		if (spConfigNodeAddChild(root,node) != 0) {
			LOG_ERR("error in XML function");
			goto cnvRSF_XML_ERR;
		}
		parent = node;
		// sub tag of <response>
		node = spConfigNodeNew();
		if (node == NULL) {
			LOG_ERR("out of memory in XML function");
			goto cnvRSF_XML_ERR;
		}
		if (resp->type == RESP_TYPE_RECORD) {
			spConfigNodeName(node, "record");
		} else if (resp->type == RESP_TYPE_VRECORD) {
			spConfigNodeName(node, "vrecord");
		} else if (resp->type == RESP_TYPE_EXEC) {
			spConfigNodeName(node, "exec");
			sprintf_s(buf, MAX_LEN, "%d", resp->exec_id);
			if (spConfigNodeAddAttributeString(node,"id",buf) != 0) {
				LOG_ERR("error in XML function");
				goto cnvRSF_XML_ERR;
			}
		} else if (resp->type == RESP_TYPE_DTMF) {
			spConfigNodeName(node, "dtmf");
		} else if (resp->type == RESP_TYPE_DISCONNECT) {
			spConfigNodeName(node, "disconnect");
		} else if (resp->type == RESP_TYPE_PLAY) {
			spConfigNodeName(node, "play");
		} else if (resp->type == RESP_TYPE_HANDSET) {
			spConfigNodeName(node, "handset");
		} else if (resp->type == RESP_TYPE_LINK) {
			spConfigNodeName(node, "link");
			if (spConfigNodeAddAttributeString(node,"order",resp->link_order) != 0) {
				LOG_ERR("error in XML function");
				goto cnvRSF_XML_ERR;
			}
		}
		if (resp->type != RESP_TYPE_LINK) {
			sprintf_s(buf, MAX_LEN, "%d", resp->beep);
			if (spConfigNodeAddAttributeString(node,"beep",buf) != 0) {
				LOG_ERR("error in XML function");
				goto cnvRSF_XML_ERR;
			}
			if (resp->type != RESP_TYPE_DISCONNECT && resp->type != RESP_TYPE_EXEC) {
				if (spConfigNodeAddAttributeString(node,"dtmfterm",resp->term) != 0) {
					LOG_ERR("error in XML function");
					goto cnvRSF_XML_ERR;
				}
			}
		}

		if (spConfigNodeAddChild(parent,node) != 0) {
			LOG_ERR("error in XML function");
			goto cnvRSF_XML_ERR;
		}
		if (resp->type == RESP_TYPE_PLAY) {
			parent = node;
			if (resp->play_type & RESP_PLAY_TYPE_AUDIO) {
				binf = &(resp->play_audio_file);
				if (strlen(binf->filename) > MIN_FILE) {
					node = spConfigNodeNew();
					if (node == NULL) {
						LOG_ERR("out of memory in XML function");
						goto cnvRSF_XML_ERR;
					}
					spConfigNodeName(node,"audio");
					if (spConfigNodeAddAttributeString(node,"src",binf->filename) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
					if (spConfigNodeAddAttributeString(node,"type",binf->bin_type) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
					sprintf_s(buf, MAX_LEN, "%ld", binf->size);
					if (spConfigNodeAddAttributeString(node,"size",buf) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
					if (spConfigNodeAddChild(parent,node) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
				}
			}
			if (resp->play_type & RESP_PLAY_TYPE_VIDEO) {
				binf = &(resp->play_video_file);
				if (strlen(binf->filename) > MIN_FILE) {
					node = spConfigNodeNew();
					if (node == NULL) {
						LOG_ERR("out of memory in XML function");
						goto cnvRSF_XML_ERR;
					}
					spConfigNodeName(node,"video");
					if (spConfigNodeAddAttributeString(node,"src",binf->filename) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
					if (spConfigNodeAddAttributeString(node,"type",binf->bin_type) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
					sprintf_s(buf, MAX_LEN, "%ld", binf->size);
					if (spConfigNodeAddAttributeString(node,"size",buf) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
					if (spConfigNodeAddChild(parent,node) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
				}
			}
			if (resp->play_type & RESP_PLAY_TYPE_IMAGE) {
				binf = &(resp->play_image_file);
				if (strlen(binf->filename) > MIN_FILE) {
					node = spConfigNodeNew();
					if (node == NULL) {
						LOG_ERR("out of memory in XML function");
						goto cnvRSF_XML_ERR;
					}
					spConfigNodeName(node,"image");
					if (spConfigNodeAddAttributeString(node,"src",binf->filename) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
					if (spConfigNodeAddAttributeString(node,"type",binf->bin_type) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
					sprintf_s(buf, MAX_LEN, "%ld", binf->size);
					if (spConfigNodeAddAttributeString(node,"size",buf) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
					if (spConfigNodeAddChild(parent,node) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
				}
			}
			if (resp->play_type & RESP_PLAY_TYPE_CHAT) {
				binf = &(resp->play_chat_file);
				if (strlen(binf->filename) > MIN_FILE) {
					node = spConfigNodeNew();
					if (node == NULL) {
						LOG_ERR("out of memory in XML function");
						goto cnvRSF_XML_ERR;
					}
					spConfigNodeName(node,"chat");
					if (spConfigNodeAddAttributeString(node,"src",binf->filename) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
					if (spConfigNodeAddAttributeString(node,"type",binf->bin_type) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
					sprintf_s(buf, MAX_LEN, "%ld", binf->size);
					if (spConfigNodeAddAttributeString(node,"size",buf) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
					if (spConfigNodeAddChild(parent,node) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRSF_XML_ERR;
					}
				}
			}
		}
	}

	return (ret);

cnvRSF_XML_ERR:
	freeXML(st_xml);
	return -1;
}

/*
	convert RDF structure to XML structure
	return -> 0:ok -1:error
*/
int cnvRDF_XML(ST_RDF *st_rdf, ST_XmlFile *st_xml)
{
	spConfigNode *root, *node, *parent;
	ST_RDF_RESP *resp;
	ST_BIN_FILE *binf;
	char buf[MAX_LEN];
	int i;
	int ret=0;

	if (st_rdf == NULL) {
		LOG_ERR("ST_rdfFile structure is NULL");
		return -1;
	}

	root = st_xml->root;
	if (root == NULL) {
		LOG_ERR("ST_XmlFile structure root member is NULL");
		goto cnvRDF_XML_ERR;
	}

	// <rdf>
	node = spConfigNodeNew();
	if (node == NULL) {
		LOG_ERR("out of memory in XML function");
		goto cnvRDF_XML_ERR;
	}
	spConfigNodeName(node,"rdf");
	if (strlen(st_rdf->telno) == 0) {
		LOG_ERR("error in RDF structure");
		goto cnvRDF_XML_ERR;
	}
	if (spConfigNodeAddAttributeString(node,"telno",st_rdf->telno) != 0) {
		LOG_ERR("error in XML function");
		goto cnvRDF_XML_ERR;
	}
	if (st_rdf->type == 1) {
		strcpy_s(buf, MAX_LEN, "call");
	} else if (st_rdf->type == 2) {
		strcpy_s(buf, MAX_LEN, "fax");
	} else {
		LOG_ERR("error in RDF structure");
		goto cnvRDF_XML_ERR;
	}
	if (spConfigNodeAddAttributeString(node,"type",buf) != 0) {
		LOG_ERR("error in XML function");
		goto cnvRDF_XML_ERR;
	}
	if (strlen(st_rdf->id_date) == 0) {
		LOG_ERR("error in RDF structure");
		goto cnvRDF_XML_ERR;
	}
	if (spConfigNodeAddAttributeString(node,"date",st_rdf->id_date) != 0) {
		LOG_ERR("error in XML function");
		goto cnvRDF_XML_ERR;
	}
	if (strlen(st_rdf->version) == 0) {
		LOG_ERR("error in RDF structure");
		goto cnvRDF_XML_ERR;
	}
	if (spConfigNodeAddAttributeString(node,"version",st_rdf->version) != 0) {
		LOG_ERR("error in XML function");
		goto cnvRDF_XML_ERR;
	}
	if (spConfigNodeAddChild(root,node) != 0) {
		LOG_ERR("error in XML function");
		goto cnvRDF_XML_ERR;
	}
	root = node;

	// <rsf>
	node = spConfigNodeNew();
	if (node == NULL) {
		LOG_ERR("out of memory in XML function");
		goto cnvRDF_XML_ERR;
	}
	spConfigNodeName(node,"rsf");
	if (strlen(st_rdf->st_rsf->telno) == 0) {
		LOG_ERR("error in RDF structure");
		goto cnvRDF_XML_ERR;
	}
	if (spConfigNodeAddAttributeString(node,"telno",st_rdf->st_rsf->telno) != 0) {
		LOG_ERR("error in XML function");
		goto cnvRDF_XML_ERR;
	}
	if (strlen(st_rdf->st_rsf->version) == 0) {
		LOG_ERR("error in RDF structure");
		goto cnvRDF_XML_ERR;
	}
	if (spConfigNodeAddAttributeString(node,"version",st_rdf->st_rsf->version) != 0) {
		LOG_ERR("error in XML function");
		goto cnvRDF_XML_ERR;
	}
	if (spConfigNodeAddChild(root,node) != 0) {
		LOG_ERR("error in XML function");
		goto cnvRDF_XML_ERR;
	}

	if (st_rdf->type == 1) {	// call
		// <response> tag list
		if (st_rdf->resp_cnt <= 0 || st_rdf->resp_cnt > MAX_RDF_RESP) {
			LOG_ERR("error in RDF structure");
			goto cnvRDF_XML_ERR;
		}
		for (i = 0; i < st_rdf->resp_cnt; i++) {
			resp = st_rdf->data.call_resp + i;
			// <response>
			node = spConfigNodeNew();
			if (node == NULL) {
				LOG_ERR("out of memory in XML function");
				goto cnvRDF_XML_ERR;
			}
			spConfigNodeName(node,"response");
			if (strlen(resp->order) == 0) {
				LOG_ERR("error in RDF structure");
				goto cnvRDF_XML_ERR;
			}
			if (spConfigNodeAddAttributeString(node,"order",resp->order) != 0) {
				LOG_ERR("error in XML function");
				goto cnvRDF_XML_ERR;
			}
			if (resp->type == RESP_TYPE_RECORD) {
				strcpy_s(buf, MAX_LEN, "record");
			} else if (resp->type == RESP_TYPE_DTMF) {
				strcpy_s(buf, MAX_LEN, "dtmf");
			} else if (resp->type == RESP_TYPE_DISCONNECT) {
				strcpy_s(buf, MAX_LEN, "disconnect");
			} else if (resp->type == RESP_TYPE_PLAY) {
				strcpy_s(buf, MAX_LEN, "play");
			} else if (resp->type == RESP_TYPE_HANDSET) {
				strcpy_s(buf, MAX_LEN, "handset");
			} else {
				LOG_ERR("error in RDF structure");
				goto cnvRDF_XML_ERR;
			}
			if (spConfigNodeAddAttributeString(node,"type",buf) != 0) {
				LOG_ERR("error in XML function");
				goto cnvRDF_XML_ERR;
			}
			if (spConfigNodeAddAttributeString(node,"rsf_order",resp->rsf_order) != 0) {
				LOG_ERR("error in XML function");
				goto cnvRDF_XML_ERR;
			}
			if (spConfigNodeAddChild(root,node) != 0) {
				LOG_ERR("error in XML function");
				goto cnvRDF_XML_ERR;
			}
			parent = node;
			// sub tag of <response>
			node = spConfigNodeNew();
			if (node == NULL) {
				LOG_ERR("out of memory in XML function");
				goto cnvRDF_XML_ERR;
			}
			if (resp->type == RESP_TYPE_RECORD) {
				spConfigNodeName(node, "record");
			} else if (resp->type == RESP_TYPE_DTMF) {
				spConfigNodeName(node, "dtmf");
				if (spConfigNodeAddAttributeString(node,"tone",resp->data.dtmf) != 0) {
					LOG_ERR("error in XML function");
					goto cnvRDF_XML_ERR;
				}
			} else if (resp->type == RESP_TYPE_DISCONNECT) {
				spConfigNodeName(node, "disconnect");
			} else if (resp->type == RESP_TYPE_PLAY) {
				spConfigNodeName(node, "play");
			} else if (resp->type == RESP_TYPE_HANDSET) {
				spConfigNodeName(node, "handset");
			}
			if (resp->type == RESP_TYPE_RECORD || resp->type == RESP_TYPE_DTMF ||
					resp->type == RESP_TYPE_PLAY) {
				sprintf_s(buf, MAX_LEN, "%c", resp->term);
				if (spConfigNodeAddAttributeString(node,"dtmfterm",buf) != 0) {
					LOG_ERR("error in XML function");
					goto cnvRDF_XML_ERR;
				}
			}
			if (spConfigNodeAddChild(parent,node) != 0) {
				LOG_ERR("error in XML function");
				goto cnvRDF_XML_ERR;
			}
			// sub tag of <record>, <handset>
			if (resp->type == RESP_TYPE_RECORD || resp->type == RESP_TYPE_HANDSET) {
				binf = &(resp->data.record_file);
				if (strlen(binf->filename) > MIN_FILE) {
					parent = node;
					node = spConfigNodeNew();
					if (node == NULL) {
						LOG_ERR("out of memory in XML function");
						goto cnvRDF_XML_ERR;
					}
					spConfigNodeName(node,"audio");
					if (spConfigNodeAddAttributeString(node,"src",binf->filename) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRDF_XML_ERR;
					}
					if (spConfigNodeAddAttributeString(node,"type",binf->bin_type) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRDF_XML_ERR;
					}
					sprintf_s(buf, MAX_LEN, "%ld", binf->size);
					if (spConfigNodeAddAttributeString(node,"size",buf) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRDF_XML_ERR;
					}
					if (spConfigNodeAddChild(parent,node) != 0) {
						LOG_ERR("error in XML function");
						goto cnvRDF_XML_ERR;
					}
				}
			}
		}
	} else if (st_rdf->type == 2) {	// fax
		for (i = 0; i < st_rdf->data.fax_data.file_cnt; i++) {
			binf = st_rdf->data.fax_data.fax_file + i;
			node = spConfigNodeNew();
			if (node == NULL) {
				LOG_ERR("out of memory in XML function");
				goto cnvRDF_XML_ERR;
			}
			spConfigNodeName(node,"image");
			if (spConfigNodeAddAttributeString(node,"src",binf->filename) != 0) {
				LOG_ERR("error in XML function");
				goto cnvRDF_XML_ERR;
			}
			if (spConfigNodeAddAttributeString(node,"type",binf->bin_type) != 0) {
				LOG_ERR("error in XML function");
				goto cnvRDF_XML_ERR;
			}
			sprintf_s(buf, MAX_LEN, "%ld", binf->size);
			if (spConfigNodeAddAttributeString(node,"size",buf) != 0) {
				LOG_ERR("error in XML function");
				goto cnvRDF_XML_ERR;
			}
			if (spConfigNodeAddChild(root,node) != 0) {
				LOG_ERR("error in XML function");
				goto cnvRDF_XML_ERR;
			}
		}
	}

	return (ret);

cnvRDF_XML_ERR:
	freeXML(st_xml);
	return -1;
}

/*
	load RDFL file to ary_rdfl[] array.
	array memory is allocated in this function.
	*ary_cnt -> return count of array
	return -> ponter of reallocated array. not NULL:OK, NULL:error
*/
ST_RDFL *loadRDFL(ST_RDFL *ary_rdfl, int *ary_cnt)
{
	static char tokensep[] = ",\n";
	char *next_token = NULL;
	FILE *fp;
	char buf[MAX_LEN], *token;
	int pos, bin_cnt;
	int ret=0;

	fp = _fsopen(RDFL_FILENAME, "r", _SH_DENYWR);
	if (fp == NULL) {
		sprintf_s(errbuf, MAX_LEN, "file [%s] is't exist", RDFL_FILENAME);
		LOG_WRN(errbuf);
		*ary_cnt = 0;
		return ary_rdfl;
	}

	// alloc memory for array
	for (pos = 0; fgets(buf, MAX_LEN, fp) != NULL; pos++);	// count lines
	if (pos == 0) {
		ary_rdfl = (ST_RDFL*)realloc(ary_rdfl, sizeof(ST_RDFL));
		goto loadRDFL_END;
	}
	ary_rdfl = (ST_RDFL*)realloc(ary_rdfl, pos * sizeof(ST_RDFL));
	if (ary_rdfl == NULL) {
		LOG_ERR("RDFL file is too big to allocate");
		ret = -1;
		goto loadRDFL_END;
	}

	// warning for many records
	if (pos > CHK_RDFL_NUM) {
		sprintf_s(errbuf, MAX_LEN, "record count:[%d] is many in RDFL file", pos+1);
		LOG_WRN(errbuf);
	}
	rewind(fp);

	// set datas to array
	for (pos = 0; fgets(buf, MAX_LEN, fp) != NULL; pos++) {
		// Date string
		next_token = NULL;
		token = strtok_s(buf,tokensep, &next_token);
		if (token == NULL) {
			break;	// End of file
		}
		strncpy_s(ary_rdfl[pos].id_date, MAX_DATE_LEN, token, MAX_DATE_LEN-1);
		// RDF filename
		token = strtok_s(NULL, tokensep, &next_token);
		if (token == NULL) {
			sprintf_s(errbuf, MAX_LEN, "record:[%d] has error in RDFL file", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		strncpy_s(ary_rdfl[pos].rdf_filename, MAX_FILE, token, MAX_FILE-1);
		// flag
		token = strtok_s(NULL, tokensep, &next_token);
		if (token == NULL) {
			sprintf_s(errbuf, MAX_LEN, "record:[%d] has error in RDFL file", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		ary_rdfl[pos].flag = atoi(token);
		// binary files
		for (bin_cnt = 0; (token = strtok_s(NULL, tokensep, &next_token)) != NULL; bin_cnt++) {
			if (bin_cnt == MAX_BIN_NUM) {
				sprintf_s(errbuf, MAX_LEN, "record:[%d] has too many binary files in RDFL file", pos+1);
				LOG_WRN(errbuf);
				break;
			}
			strncpy_s(ary_rdfl[pos].bin_filename[bin_cnt], MAX_FILE, token, MAX_FILE-1);
		}
		ary_rdfl[pos].bin_cnt = bin_cnt;
	}

loadRDFL_END:
	// unlock
//	flock(fileno(fp), LOCK_UN);
	fclose(fp);

	if (ret == 0) {
		*ary_cnt = pos;
		return ary_rdfl;
	} else {
		*ary_cnt = 0;
		return NULL;
	}
}

/*
	save RDFL file from ary_rdfl[] array.
	arry_cnt:count of array
	return -> 0:ok -1:error
*/
int saveRDFL(ST_RDFL *ary_rdfl, int arry_cnt)
{
	FILE *fp;
	char buf[MAX_LEN];
	ST_RDFL *tmp_rdfl;
	int pos, bin_cnt, len;
	int ret=0;

	if (ary_rdfl == NULL) {
		LOG_ERR("array is NULL");
		return -1;
	}
	if (arry_cnt <= 0) {
		LOG_ERR("array count is wrong");
		return -1;
	}

	fp = _fsopen(RDFL_FILENAME, "w", _SH_DENYRW);
	if (fp == NULL) {
		// unkown error
		LOG_ERR("unkown error as file open");
		return -1;
	}

	// write datas to file
	for (pos = 0; pos < arry_cnt; pos++) {
		// check data
		tmp_rdfl = ary_rdfl + pos;
		if (tmp_rdfl->id_date == NULL) {
			sprintf_s(errbuf, MAX_LEN, "id_date in record:[%d] is NULL", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		if (tmp_rdfl->rdf_filename == NULL) {
			sprintf_s(errbuf, MAX_LEN, "rdf_filename in record:[%d] is NULL", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		// make buffer
		sprintf_s(buf, MAX_LEN, "%s,%s,%d", tmp_rdfl->id_date, tmp_rdfl->rdf_filename, tmp_rdfl->flag);
		bin_cnt = tmp_rdfl->bin_cnt;
		for (bin_cnt = 0; bin_cnt < tmp_rdfl->bin_cnt; bin_cnt++) {
			if (bin_cnt == MAX_BIN_NUM) {
				sprintf_s(errbuf, MAX_LEN, "structure has too many binary files:[%d]", tmp_rdfl->bin_cnt);
				LOG_WRN(errbuf);
				break;
			}
			len = strlen(buf) + strlen(tmp_rdfl->bin_filename[bin_cnt]) + 1;
			if (len >= MAX_LEN) {
				sprintf_s(errbuf, MAX_LEN, "length of line is too long:[%d]", len);
				LOG_WRN(errbuf);
				break;
			}
			strcat_s(buf, MAX_LEN, ",");
			strcat_s(buf, MAX_LEN, tmp_rdfl->bin_filename[bin_cnt]);
		}
		strcat_s(buf, MAX_LEN, "\n");
		// write
		if (fputs(buf, fp) == EOF) {
			LOG_ERR("can't add Data to RDFL file");
			ret = -1;
			break;
		}
	}

	// unlock
//	flock(fileno(fp), LOCK_UN);
	fclose(fp);

	return (ret);
}

/*
	add RDFL
	return -> 0:ok -1:error
*/
int addRDFL(ST_RDFL *tmp_rdfl)
{
	FILE *fp;
	char buf[MAX_LEN];
	int bin_cnt, len;
	int ret=0;

	if (tmp_rdfl == NULL) {
		LOG_ERR("tmp_rdfl is NULL");
		return -1;
	}
	
	if (tmp_rdfl->id_date == NULL) {
		LOG_ERR("tmp_rdfl->id_date is NULL");
		return -1;
	}
	if (tmp_rdfl->rdf_filename == NULL) {
		LOG_ERR("tmp_rdfl->rdf_filename is NULL");
		return -1;
	}

	fp = _fsopen(RDFL_FILENAME, "a", _SH_DENYRW);
	if (fp == NULL) {
		// file is't exist -> open new file
		if (fopen_s(&fp, RDFL_FILENAME, "w")) {
			// unkown error
			LOG_ERR("unkown error as file open");
			return -1;
		}
	}

	// lock
//	flock(fileno(fp), LOCK_EX);

	// make buffer
	sprintf_s(buf, MAX_LEN, "%s,%s,%d", tmp_rdfl->id_date, tmp_rdfl->rdf_filename, tmp_rdfl->flag);
	bin_cnt = tmp_rdfl->bin_cnt;
	for (bin_cnt = 0; bin_cnt < tmp_rdfl->bin_cnt; bin_cnt++) {
		if (bin_cnt == MAX_BIN_NUM) {
			sprintf_s(errbuf, MAX_LEN, "structure has too many binary files:[%d]", tmp_rdfl->bin_cnt);
			LOG_WRN(errbuf);
			break;
		}
		len = strlen(buf) + strlen(tmp_rdfl->bin_filename[bin_cnt]) + 1;
		if (len >= MAX_LEN) {
			sprintf_s(errbuf, MAX_LEN, "length of line is too long:[%d]", len);
			LOG_WRN(errbuf);
			break;
		}
		strcat_s(buf, MAX_LEN, ",");
		strcat_s(buf, MAX_LEN, tmp_rdfl->bin_filename[bin_cnt]);
	}
	strcat_s(buf, MAX_LEN, "\n");

	// write
	if (fputs(buf, fp) == EOF) {
		LOG_ERR("can't add Data to RDFL file");
		ret = -1;
	}

	// unlock
//	flock(fileno(fp), LOCK_UN);
	fclose(fp);

	return (ret);
}

/*
	clean RDFL file.
	return -> 0:ok -1:error
*/
int cleanRDFL(void)
{
	static char tokensep[] = ",\n";
	char *next_token = NULL;
	FILE *fpr, *fpw;
	char buf[MAX_LEN], tmp[MAX_LEN];
	char tmpfile[MAX_PATH], *token;
	int pos;
	int ret=0;

	sprintf_s(tmpfile, MAX_PATH, "%s.tmp", RDFL_FILENAME);
	if (fopen_s(&fpw, tmpfile, "w")) {
		// unkown error
		LOG_ERR("unkown error as file open");
		return -1;
	}

	if (fopen_s(&fpr, RDFL_FILENAME, "r")) {
		// unkown error
		LOG_ERR("unkown error as file open");
		return -1;
	}

	// write datas to file
	for (pos = 0; fgets(buf, MAX_LEN, fpr) != NULL; pos++) {
		strcpy_s(tmp, MAX_LEN, buf);
		// Date string
		next_token = NULL;
		token = strtok_s(buf, tokensep, &next_token);
		if (token == NULL) {
			break;	// End of file
		}
		// RDF filename
		token = strtok_s(NULL, tokensep, &next_token);
		if (token == NULL) {
			sprintf_s(errbuf, MAX_LEN, "record:[%d] has error in RDFL file", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		// flag
		token = strtok_s(NULL, tokensep, &next_token);
		if (token == NULL) {
			sprintf_s(errbuf, MAX_LEN, "record:[%d] has error in RDFL file", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		if (atoi(token) != RDFL_FLAG_REUSE) {
			// write
			if (fputs(tmp, fpw) == EOF) {
				LOG_ERR("can't write Data to RDFL file");
				ret = -1;
				break;
			}
		}
	}

	fclose(fpr);
	fclose(fpw);

	if (rename(tmpfile,RDFL_FILENAME) != 0) return -1;

	return (ret);
}


/*
	load TDFL file to ary_tdfl[] array.
	array memory is allocated in this function.
	*ary_cnt -> return count of array
	return -> ponter of reallocated array. not NULL:OK, NULL:error
*/
ST_TDFL *loadTDFL(ST_TDFL *ary_tdfl, int *ary_cnt)
{
	static char tokensep[] = ",\n";
	char *next_token = NULL;
	FILE *fp;
	char buf[MAX_LEN], *token;
	int pos;
	int ret=0;

	fp = _fsopen(TDFL_FILENAME, "r", _SH_DENYWR);
	if (fp == NULL) {
		sprintf_s(errbuf, MAX_LEN, "file [%s] is't exist", TDFL_FILENAME);
		LOG_WRN(errbuf);
		*ary_cnt = 0;
		return ary_tdfl;
	}

	// alloc memory for array
	for (pos = 0; fgets(buf, MAX_LEN, fp) != NULL; pos++);	// count lines
	if (pos == 0) {
		ary_tdfl = (ST_TDFL*)realloc(ary_tdfl, sizeof(ST_TDFL));
		goto loadTDFL_END;
	}
	ary_tdfl = (ST_TDFL*)realloc(ary_tdfl, pos * sizeof(ST_TDFL));
	if (ary_tdfl == NULL) {
		LOG_ERR("TDFL file is too big to allocate");
		ret = -1;
		goto loadTDFL_END;
	}

	// warning for many records
	if (pos > CHK_TDFL_NUM) {
		sprintf_s(errbuf, MAX_LEN, "record count:[%d] is many in TDFL file", pos+1);
		LOG_WRN(errbuf);
	}
	rewind(fp);

	// set datas to array
	for (pos = 0; fgets(buf, MAX_LEN, fp) != NULL; pos++) {
		// Date string
		next_token = NULL;
		token = strtok_s(buf, tokensep, &next_token);
		if (token == NULL) {
			break;	// End of file
		}
		strncpy_s(ary_tdfl[pos].id_date, MAX_DATE_LEN, token, MAX_DATE_LEN-1);
		// RDF filename
		token = strtok_s(NULL, tokensep, &next_token);
		if (token == NULL) {
			sprintf_s(errbuf, MAX_LEN, "record:[%d] has error in TDFL file", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		strncpy_s(ary_tdfl[pos].rdf_filename, MAX_FILE, token, MAX_FILE-1);
		// flag
		token = strtok_s(NULL, tokensep, &next_token);
		if (token == NULL) {
			sprintf_s(errbuf, MAX_LEN, "record:[%d] has error in TDFL file", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		ary_tdfl[pos].flag = atoi(token);
		// compress file
		token = strtok_s(NULL, tokensep, &next_token);
		if (token == NULL) {
			sprintf_s(errbuf, MAX_LEN, "record:[%d] has error in TDFL file", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		strncpy_s(ary_tdfl[pos].cmp_filename, MAX_FILE, token, MAX_FILE-1);
		// send date
		token = strtok_s(NULL, tokensep, &next_token);
		if (token != NULL) {
			strncpy_s(ary_tdfl[pos].send_date, MAX_DATE_LEN, token, MAX_DATE_LEN-1);
		}
	}

loadTDFL_END:
	// unlock
//	flock(fileno(fp), LOCK_UN);
	fclose(fp);

	if (ret == 0) {
		*ary_cnt = pos;
		return ary_tdfl;
	} else {
		*ary_cnt = 0;
		return NULL;
	}
}

/*
	save TDFL file from ary_tdfl[] array.
	return -> 0:ok -1:error
*/
int saveTDFL(ST_TDFL *ary_tdfl, int arry_cnt)
{
	FILE *fp;
	char buf[MAX_LEN];
	ST_TDFL *tmp_tdfl;
	int pos;
	int ret=0;

	if (ary_tdfl == NULL) {
		LOG_ERR("array is NULL");
		return -1;
	}
	if (arry_cnt <= 0) {
		LOG_ERR("array count is wrong");
		return -1;
	}

	fp = _fsopen(TDFL_FILENAME, "w", _SH_DENYRW);
	if (fp == NULL) {
		// unkown error
		LOG_ERR("unkown error as file open");
		return -1;
	}

	// write datas to file
	for (pos = 0; pos < arry_cnt; pos++) {
		// check data
		tmp_tdfl = ary_tdfl + pos;
		if (tmp_tdfl->id_date == NULL) {
			sprintf_s(errbuf, MAX_LEN, "id_date in record:[%d] is NULL", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		if (tmp_tdfl->rdf_filename == NULL) {
			sprintf_s(errbuf, MAX_LEN, "rdf_filename in record:[%d] is NULL", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		if (tmp_tdfl->cmp_filename == NULL) {
			sprintf_s(errbuf, MAX_LEN, "cmp_filename in record:[%d] is NULL", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		// make buffer
		sprintf_s(buf, MAX_LEN, "%s,%s,%d,%s",
			tmp_tdfl->id_date, tmp_tdfl->rdf_filename, tmp_tdfl->flag, tmp_tdfl->cmp_filename);
		if (strlen(tmp_tdfl->send_date) > 0) {
			strcat_s(buf, MAX_LEN, ",");
			strcat_s(buf, MAX_LEN, tmp_tdfl->send_date);
		}
		strcat_s(buf, MAX_LEN, "\n");
		// write
		if (fputs(buf, fp) == EOF) {
			LOG_ERR("can't add Data to TDFL file");
			ret = -1;
			break;
		}
	}

	// unlock
//	flock(fileno(fp), LOCK_UN);
	fclose(fp);

	return (ret);
}

/*
	add TDFL
	return -> 0:ok -1:error
*/
int addTDFL(ST_TDFL *tmp_tdfl)
{
	FILE *fp;
	char buf[MAX_LEN];
	int ret=0;

	if (tmp_tdfl == NULL) {
		LOG_ERR("tmp_tdfl is NULL");
		return -1;
	}
	
	if (tmp_tdfl->id_date == NULL) {
		LOG_ERR("tmp_tdfl->id_date is NULL");
		return -1;
	}
	if (tmp_tdfl->rdf_filename == NULL) {
		LOG_ERR("tmp_tdfl->rdf_filename is NULL");
		return -1;
	}
	if (tmp_tdfl->cmp_filename == NULL) {
		LOG_ERR("tmp_tdfl->cmp_filename is NULL");
		return -1;
	}

	fp = _fsopen(TDFL_FILENAME, "a", _SH_DENYRW);
	if (fp == NULL) {
		// file is't exist -> open new file
		if (fopen_s(&fp, TDFL_FILENAME, "w")) {
			// unkown error
			LOG_ERR("unkown error as file open");
			return -1;
		}
	}

	// make buffer
	sprintf_s(buf, MAX_LEN, "%s,%s,%d,%s",
		tmp_tdfl->id_date, tmp_tdfl->rdf_filename, tmp_tdfl->flag, tmp_tdfl->cmp_filename);
	if (tmp_tdfl->send_date != NULL) {
		strcat_s(buf, MAX_LEN, ",");
		strcat_s(buf, MAX_LEN, tmp_tdfl->send_date);
	}
	strcat_s(buf, MAX_LEN, "\n");

	// write
	if (fputs(buf, fp) == EOF) {
		LOG_ERR("can't add Data to TDFL file");
		ret = -1;
	}

	// unlock
//	flock(fileno(fp), LOCK_UN);
	fclose(fp);

	return (ret);
}

/*
	clean TDFL file.
	return -> 0:ok -1:error
*/
int cleanTDFL(void)
{
	static char tokensep[] = ",\n";
	char *next_token = NULL;
	FILE *fpr, *fpw;
	char buf[MAX_LEN], tmp[MAX_LEN];
	char tmpfile[MAX_PATH], *token;
	int pos;
	int ret=0;

	sprintf_s(tmpfile, MAX_PATH, "%s.tmp", TDFL_FILENAME);
	if (fopen_s(&fpw, tmpfile, "w")) {
		// unkown error
		LOG_ERR("unkown error as file open");
		return -1;
	}

	if (fopen_s(&fpr, TDFL_FILENAME, "r")) {
		// unkown error
		LOG_ERR("unkown error as file open");
		return -1;
	}

	// write datas to file
	for (pos = 0; fgets(buf, MAX_LEN, fpr) != NULL; pos++) {
		strcpy_s(tmp, MAX_LEN, buf);
		// Date string
		next_token = NULL;
		token = strtok_s(buf, tokensep, &next_token);
		if (token == NULL) {
			break;	// End of file
		}
		// RDF filename
		token = strtok_s(NULL, tokensep, &next_token);
		if (token == NULL) {
			sprintf_s(errbuf, MAX_LEN, "record:[%d] has error in TDFL file", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		// flag
		token = strtok_s(NULL, tokensep, &next_token);
		if (token == NULL) {
			sprintf_s(errbuf, MAX_LEN, "record:[%d] has error in TDFL file", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		if (atoi(token) != TDFL_FLAG_REUSE) {
			// write
			if (fputs(tmp, fpw) == EOF) {
				LOG_ERR("can't write Data to TDFL file");
				ret = -1;
				break;
			}
		}
	}

	fclose(fpr);
	fclose(fpw);

	if (rename(tmpfile,TDFL_FILENAME) != 0) return -1;

	return (ret);
}


/*
	load UFL file to ary_ufl[] array.
	array memory is allocated in this function.
	*ary_cnt -> return count of array
	return -> ponter of reallocated array. not NULL:OK, NULL:error
*/
ST_UFL *loadUFL(ST_UFL *ary_ufl, int *ary_cnt)
{
	static char tokensep[] = ",\n";
	char *next_token = NULL;
	FILE *fp;
	char buf[MAX_LEN], *token;
	int pos;
	int ret=0;

	fp = _fsopen(UFL_FILENAME, "r", _SH_DENYWR);
	if (fp == NULL) {
		sprintf_s(errbuf, MAX_LEN, "file [%s] is't exist", UFL_FILENAME);
		LOG_WRN(errbuf);
		*ary_cnt = 0;
		return ary_ufl;
	}

	// alloc memory for array
	for (pos = 0; fgets(buf, MAX_LEN, fp) != NULL; pos++);	// count lines
	if (pos == 0) {
		ary_ufl = (ST_UFL*)realloc(ary_ufl, sizeof(ST_UFL));
		goto loadUFL_END;
	}
	ary_ufl = (ST_UFL*)realloc(ary_ufl, pos * sizeof(ST_UFL));
	if (ary_ufl == NULL) {
		LOG_ERR("UFL file is too big to allocate");
		ret = -1;
		goto loadUFL_END;
	}

	rewind(fp);

	// set datas to array
	for (pos = 0; fgets(buf, MAX_LEN, fp) != NULL; pos++) {
		// flag
		next_token = NULL;
		token = strtok_s(buf, tokensep, &next_token);
		if (token == NULL) {
			sprintf_s(errbuf, MAX_LEN, "record:[%d] has error in UFL file", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		ary_ufl[pos].flag = atoi(token);
		// filename
		token = strtok_s(NULL, tokensep, &next_token);
		if (token == NULL) {
			sprintf_s(errbuf, MAX_LEN, "record:[%d] has error in UFL file", pos+1);
			LOG_ERR(errbuf);
			ret = -1;
			break;
		}
		strncpy_s(ary_ufl[pos].filename, MAX_FILE, token, MAX_FILE-1);
	}

loadUFL_END:
	// unlock
//	flock(fileno(fp), LOCK_UN);
	fclose(fp);

	if (ret == 0) {
		*ary_cnt = pos;
		return ary_ufl;
	} else {
		*ary_cnt = 0;
		return NULL;
	}
}


