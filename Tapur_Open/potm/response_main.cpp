/*
	
	response_main.c 
	2002/03/09 -zdy-
	
*/
#include "Forcelib.h"
#include <stdio.h>
#include <string.h>
//#include <dirent.h>	// DIR
#include <time.h>
#include <signal.h>
#include "potm.h"
#include "file_access.h"
#include "response_main.h"

static char errbuf[MAX_LEN];

// static array to realloc memory without free
static ST_RDFL *st_rdfl;
static int cnt_rdfl;


/********************  utility functions  *****************/
/*
	get long Date string
	return -> 0:ok -1:error
*/
int getDateId(char *date_id)
{
	time_t ct = time(NULL);
	struct tm lst;
	if (localtime_s(&lst, &ct)) return -1;

	sprintf_s(date_id, 15, "%04d%02d%02d%02d%02d%02d", (1900+lst.tm_year), (1+lst.tm_mon),
		lst.tm_mday, lst.tm_hour, lst.tm_min, lst.tm_sec);

	return 0;
}

/********************  local functions  *****************/
/*
	sub func for get RSF_RESP setting data
	return -> 0:ok -1:error
*/
static int get_rsf_resp(ST_RSF *pst_rsf, ST_RSF_RESP *pst_rsf_resp, const char *order)
{
	ST_RSF_RESP *presp;
	char tmp_order[MAX_RDF_RESP];
	int i, nest;
	size_t len;

	strcpy_s(tmp_order, MAX_RDF_RESP, order);

	// try to find target setting
	for (i = nest = 0; i < pst_rsf->resp_cnt; i++) {
		presp = pst_rsf->resp + i;
		len = strlen(tmp_order);
//LOG_ST("i-order:%s, order:%s\n", presp->order, tmp_order);
		if (strlen(presp->order) != len) continue;
		if (strcmp(presp->order, tmp_order) == 0) {
			if (presp->type == RESP_TYPE_LINK) {
				if (++nest > LINK_ORDER_NEST) break;
				strcpy_s(tmp_order, MAX_RDF_RESP, presp->link_order);
				i = -1;
				continue;
			} else {
				memcpy(pst_rsf_resp, presp, sizeof(ST_RSF_RESP));
				return 0;
			}
		}
	}

	return -1;
}

/*
	get RSF_RESP setting data
	pst_rsf : RSF setting
	pst_rsf_resp : buffer to save target Resp
	order : search response order, maybe changed if its result is default.
	return -> 0:ok -1:error
*/
int getRSF_RESP(ST_RSF *pst_rsf, ST_RSF_RESP *pst_rsf_resp, char *order)
{
	// try to find target setting
	if (get_rsf_resp(pst_rsf, pst_rsf_resp, order) == 0) return 0;

	// if not find target order, try to find default setting
	order[strlen(order)-1] = '$';
	if (get_rsf_resp(pst_rsf, pst_rsf_resp, order) == 0) return 0;

	return -1;
}


/*
	get RSF setting data
	return -> 0:ok -1:error
*/
int getRSF(ST_RSF *pst_rsf, char *rsf_fn)
{
	ST_XmlFile st_xml;
	ST_XmlFile *pst_xml = &st_xml;
	int ret = 0;

	memset(pst_xml, '\0', sizeof(ST_XmlFile));
	if (newXML(pst_xml)) {
		LOG_ERR("fatal error\n");
		return -1;
	}

	// read RSF file, initial setting
	if (readXML(rsf_fn, pst_xml)) {
		LOG_ERR("can't load RSF file\n");
		ret = -1;
		goto getRSF_Err;
	}

	if (cnvXML_RSF(pst_xml, pst_rsf)) {
		LOG_ERR("can't analyze RSF file\n");
		ret = -1;
		goto getRSF_Err;
	}

getRSF_Err:
	freeXML(pst_xml);
	return ret;
}

/*
	get RDF setting data
	return -> 0:ok -1:error
*/
int getRDF(ST_RDF *pst_rdf, char *rdf_fn)
{
	ST_XmlFile st_xml;
	ST_XmlFile *pst_xml = &st_xml;
	int ret = 0;

	memset(pst_xml, '\0', sizeof(ST_XmlFile));
	if (newXML(pst_xml)) {
		LOG_ERR("fatal error\n");
		return -1;
	}

	// read RDF file, initial setting
	if (readXML(rdf_fn, pst_xml)) {
		LOG_ERR("can't load RDF file\n");
		ret = -1;
		goto getRDF_Err;
	}

	if (cnvXML_RDF(pst_xml, pst_rdf)) {
		LOG_ERR("can't analyze RDF file\n");
		ret = -1;
		goto getRDF_Err;
	}

getRDF_Err:
	freeXML(pst_xml);
	return ret;
}

/*
	save RDF structure into XML file.
	return -> 0:ok -1:error
*/
static int saveRDF(ST_RDF *pst_rdf, char *rdf_fn)
{
	ST_XmlFile st_xml;
	ST_XmlFile *pst_xml = &st_xml;
	int ret = 0;

	memset(pst_xml, '\0', sizeof(ST_XmlFile));
	if (newXML(pst_xml)) {
		LOG_ERR("fatal error\n");
		return -1;
	}

	if (cnvRDF_XML(pst_rdf, pst_xml)) {
		LOG_ERR("can't convert RDF structure to XML structure\n");
		ret = -1;
		goto saveRDF_Err;
	}

	if (writeXML(rdf_fn, pst_xml)) {
		LOG_ERR("can't save XML file\n");
		ret = -1;
		goto saveRDF_Err;
	}

saveRDF_Err:
	freeXML(pst_xml);
	return ret;
}

/*
	set RDF into RDFL list
	rdf_fn:RDF filename to save
	pst_rdf: RDF date
	return -> 0:ok -1:error
*/
/*
static int setRDFL(char *rdf_fn, ST_RDF *pst_rdf)
{
	ST_RDFL tmp_rdfl;
	ST_RDFL *pst_rdfl = &tmp_rdfl;
	ST_RDF_RESP *pst_rdf_resp;
	ST_FAX_DATA *pfax_dat;
	int i;
	int addflag = 1; // add
	char *fn;

	// load RDFL
	st_rdfl = loadRDFL(st_rdfl, &cnt_rdfl);
	if (st_rdfl == NULL) {
		return -1;
	}

	// find reusable one RDFL structure
	for (i = 0; i < cnt_rdfl; i++) {
		// check flag
		if ((st_rdfl+i)->flag == RDFL_FLAG_REUSE) {
			pst_rdfl = st_rdfl + i;
			addflag = 0;
			break;
		}
	}

	// set RDFL from RDF
	strcpy(pst_rdfl->id_date, pst_rdf->id_date);
	strcpy(pst_rdfl->rdf_filename, rdf_fn);
	pst_rdfl->flag = RDFL_FLAG_WAIT;
	if (pst_rdf->type == RDF_TYPE_SIMPLE) { // call
		pst_rdfl->bin_cnt = 0;
		for (i = 0; i < pst_rdf->resp_cnt; i++) {
			pst_rdf_resp = pst_rdf->data.call_resp + i;
			if (pst_rdf_resp->type == RESP_TYPE_RECORD || pst_rdf_resp->type == RESP_TYPE_HANDSET) {
				fn = pst_rdf_resp->data.record_file.filename;
				if (strlen(fn) > 0) {
					strcpy(pst_rdfl->bin_filename[pst_rdfl->bin_cnt], fn);
					pst_rdfl->bin_cnt++;
				}
			}
		}
	} else if (pst_rdf->type == RDF_TYPE_FAX) { // fax
		pst_rdfl->bin_cnt = 0;
		pfax_dat = &(pst_rdf->data.fax_data);
		for (i = 0; i < pfax_dat->file_cnt; i++) {
			fn = pfax_dat->fax_file[i].filename;
			if (strlen(fn) > 0) {
				strcpy(pst_rdfl->bin_filename[pst_rdfl->bin_cnt], fn);
				pst_rdfl->bin_cnt++;
			}
		}
	}

	// save RDFL file
	if (addflag) {// add
		if (addRDFL(&tmp_rdfl) < 0) {
			return -1;
		}
	} else { // update
		if (saveRDFL(st_rdfl, cnt_rdfl) < 0) {
			return -1;
		}
	}

	return 0;
}
*/


