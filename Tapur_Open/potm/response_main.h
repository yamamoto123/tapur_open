/*

	response_main.h
	2002/10/09 -paak-
	
*/
#ifndef __PO_RESPONSE_MAIN_H
#define __PO_RESPONSE_MAIN_H

#include "file_access.h"

// file for communication with DTE<->DCE
#define ATA_ESCAPE_FILENAME "/var/tmp/esc_ata.comm"
#define VRX1_ESCAPE_FILENAME "/var/tmp/esc_vrx1.comm"
#define VRX2_ESCAPE_FILENAME "/var/tmp/esc_vrx2.comm"
#define VRX3_ESCAPE_FILENAME "/var/tmp/esc_vrx3.comm"
#define VTX1_ESCAPE_FILENAME "/var/tmp/esc_vtx1.comm"
#define VTX2_ESCAPE_FILENAME "/var/tmp/esc_vtx2.comm"
#define VTX3_ESCAPE_FILENAME "/var/tmp/esc_vtx3.comm"
#define CID_OUT_FILENAME "/var/tmp/out_cid.comm"
#define VRX_OUT_FILENAME "/var/tmp/out_vrx.comm"

// definition for response
#define CALLID_TAG "NMBR"
#define TONE_CHARACTERS "1234567890#"
#define WAITCALL_TIME 3600	// wateing call time(sec)
#define PLAYVOICE_TIMEOUT 60	// timeout(sec) when play voice
#define RECORDVOICE_TIMEOUT 120	// timeout(sec) when record voice
#define RECORDTONE_TIMEOUT 60	// timeout(sec) when record tone
#define HANDSET_TIMEOUT 3600	// timeout(sec) when use handset
#define HANDSET_CALL_CNT 120		// max count of calling in useHandest()
#define HANDSET_CALL_FILE "../conf/handset_call.adpcm"	// sound file of calling in useHandest()

// definition for response_main
#define ERR_SLEEP 60	// interval(sec) when err occure
#define LINK_ORDER_NEST 10	// max nest of order linkage

#ifdef __cplusplus
extern "C" {
#endif
// respoinse_main.c
//extern int responseMain(ST_ConfFile *st_confFile);
//extern void responseQuit(void);
extern int getRSF_RESP(ST_RSF *pst_rsf, ST_RSF_RESP *pst_rsf_resp, char *order);
extern int getRSF(ST_RSF *pst_rsf, char *rsf_fn);
extern int getRDF(ST_RDF *pst_rdf, char *rdf_fn);

// response.c
/*
extern int getCallId(char *fn, char *get_buf, int maxlen);
extern int openDCE(ST_ConfFile *st_confFile);
extern int closeDCE(ST_ConfFile *st_confFile);
extern int waitRings(ST_ConfFile *st_confFile);
extern int answerCall(ST_ConfFile *st_confFile);
extern int playVoice(ST_ConfFile *st_confFile, char *adpcm_file, char *termstr, char *term, int chkfax);
extern int playBeep(ST_ConfFile *st_confFile, int duration);
extern int recordVoice(ST_ConfFile *st_confFile, char *termstr, char *adpcm_file, char *term);
extern int recordTone(ST_ConfFile *st_confFile, char *termstr, char *tonestr, char *term);
extern int useHandset(ST_ConfFile *st_confFile, int record_flag, char *adpcm_file);
extern int answerFax(ST_ConfFile *st_confFile);
*/
#ifdef __cplusplus
}
#endif

#endif	// __PO_RESPONSE_MAIN_H
