// COggVorbis.cpp 
//

#include "stdafx.h"
#include "OggVorbis.h"
#include "Wave.h"
#include "ProgressWnd.h"

// memory leak 検出
#ifdef	__AFX_H__            // MFCのみ
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

#define READ 1024
signed char readbuffer[READ*4+44]; /* out of the data segment, not the stack */


COggVorbis::COggVorbis()
{
}

COggVorbis::~COggVorbis() 
{
}

//
//  機能     : Wave->OggVorbisエンコード
//  
//  機能説明 : 
//  
//  返り値   : 1:OK, 0:cancel -1:eror
//  
//  備考     : 
//  
int COggVorbis::Encode(
CWnd *hWin,					// メッセージ表示用ハンドル
CString strWaveFile,		// Waveファイル
CString strOggFile, 		// Oggファイル
double dblQualityBase)		// Quality Base（-0.1-1.0）、0.4≒128kbps VBR
{
	ogg_stream_state os; /* take physical pages, weld into a logical stream of packets */
	ogg_page         og; /* one Ogg bitstream page.  Vorbis packets are inside */
	ogg_packet       op; /* one raw packet of data for decode */
	vorbis_info      vi; /* struct that stores all the static vorbis bitstream settings */
	vorbis_comment   vc; /* struct that stores all the user comments */
	vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
	vorbis_block     vb; /* local working space for packet->PCM decode */
	int eos=0;
	int i, k, founddata;
	int ret;

	CFileStatus fileStatus;
	DWORD nInputFileSize = 0;
	DWORD nOutputSize = 0;
	int intPercent = 0;
	CString strMsg;
	CProgressWnd m_wndProgress;

	// Check Wave Header
	CWave wv;
	if (!wv.LoadFormat(strWaveFile, FALSE)) return -1;
	WAVEFORMATEX wformat = wv.GetFormat();

	FILE *fin, *fout;
	_tfopen_s(&fin, strWaveFile, _T("rb"));
	_tfopen_s(&fout, strOggFile, _T("wb"));

	TRY {
		readbuffer[0] = '\0';
		for (i=0, founddata=0; i<30 && ! feof(fin) && ! ferror(fin); i++)
		{
		fread(readbuffer,1,2,fin);

		if ( ! strncmp((char*)readbuffer, "da", 2) )
		{
		  founddata = 1;
		  fread(readbuffer,1,6,fin);
		  break;
		}
		}

		/********** Encode setup ************/

		vorbis_info_init(&vi);

		/* choose an encoding mode.  A few possibilities commented out, one
		 actually used: */

		/*********************************************************************
		Encoding using a VBR quality mode.  The usable range is -.1
		(lowest quality, smallest file) to 1. (highest quality, largest file).
		Example quality mode .4: 44kHz stereo coupled, roughly 128kbps VBR 

		ret = vorbis_encode_init_vbr(&vi,2,44100,.4);

		---------------------------------------------------------------------

		Encoding using an average bitrate mode (ABR).
		example: 44kHz stereo coupled, average 128kbps VBR 

		ret = vorbis_encode_init(&vi,2,44100,-1,128000,-1);

		---------------------------------------------------------------------

		Encode using a quality mode, but select that quality mode by asking for
		an approximate bitrate.  This is not ABR, it is true VBR, but selected
		using the bitrate interface, and then turning bitrate management off:

		ret = ( vorbis_encode_setup_managed(&vi,2,44100,-1,128000,-1) ||
			   vorbis_encode_ctl(&vi,OV_ECTL_RATEMANAGE2_SET,NULL) ||
			   vorbis_encode_setup_init(&vi));

		*********************************************************************/

		ret=vorbis_encode_init_vbr(&vi, wformat.nChannels, wformat.nSamplesPerSec, (float)dblQualityBase);

		/* do not continue if setup failed; this can happen if we ask for a
		 mode that libVorbis does not support (eg, too low a bitrate, etc,
		 will return 'OV_EIMPL') */
		if(ret) {
			ret = -1;
			goto Encode_END;
		}

		/* add a comment */
		vorbis_comment_init(&vc);
		vorbis_comment_add_tag(&vc,"ENCODER","Tapur");

		/* set up the analysis state and auxiliary encoding storage */
		vorbis_analysis_init(&vd,&vi);
		vorbis_block_init(&vd,&vb);

		/* set up our packet->stream encoder */
		/* pick a random serial number; that way we can more likely build
		 chained streams just by concatenation */
		srand((unsigned int)time(NULL));
		ogg_stream_init(&os,rand());

		/* Vorbis streams begin with three headers; the initial header (with
		 most of the codec setup parameters) which is mandated by the Ogg
		 bitstream spec.  The second header holds any comment fields.  The
		 third header holds the bitstream codebook.  We merely need to
		 make the headers, then pass them to libvorbis one at a time;
		 libvorbis handles the additional Ogg bitstream constraints */
		{
			ogg_packet header;
			ogg_packet header_comm;
			ogg_packet header_code;

			vorbis_analysis_headerout(&vd,&vc,&header,&header_comm,&header_code);
			ogg_stream_packetin(&os,&header); /* automatically placed in its own page */
			ogg_stream_packetin(&os,&header_comm);
			ogg_stream_packetin(&os,&header_code);

			/* This ensures the actual
			 * audio data will start on a new page, as per spec
			 */
			while(!eos){
				int result=ogg_stream_flush(&os,&og);
				if(result==0)break;
				fwrite(og.header,1,og.header_len,fout);
				fwrite(og.body,1,og.body_len,fout);
			}
		}

		// Calculate output file size
		CFile::GetStatus(strWaveFile, fileStatus);
		nInputFileSize = (DWORD)fileStatus.m_size;
		nOutputSize = 0;
		/// プログレスメッセージ作成
		m_wndProgress.Create(hWin);
		//m_wndProgress.GoModal();
		strMsg.LoadString(IDS_MSGBOX91);
		strMsg = _T("Ogg Vorbis") + strMsg;
		m_wndProgress.SetText(strMsg);
		k = 0;

		while(!eos) {
			/// read WAVE data
			long i;
			int szAry = wformat.nChannels * 2;	// mono:2, stereo:4
			long bytes=fread(readbuffer,1,READ*szAry,fin);
			nOutputSize += READ*szAry;

			if(bytes==0){
			  /* end of file.  this can be done implicitly in the mainline,
				 but it's easier to see here in non-clever fashion.
				 Tell the library we're at end of stream so that it can handle
				 the last frame and mark end of stream in the output properly */
			  vorbis_analysis_wrote(&vd,0);
			}else{
				/* data to encode */
				/* expose the buffer to submit data */
				float **buffer=vorbis_analysis_buffer(&vd,READ);
				/* uninterleave samples */
				for(i=0;i<bytes/szAry;i++){
					buffer[0][i]=((readbuffer[i*szAry+1]<<8) | (0x00ff&(int)readbuffer[i*szAry]))/32768.f;
					if (szAry == 4) {
						buffer[1][i]=((readbuffer[i*szAry+3]<<8) | (0x00ff&(int)readbuffer[i*szAry+2]))/32768.f;
					}
				}
				/* tell the library how much we actually submitted */
				vorbis_analysis_wrote(&vd,i);
			}

			/* vorbis does some data preanalysis, then divvies up blocks for
			   more involved (potentially parallel) processing.  Get a single
			   block for encoding now */
			while(vorbis_analysis_blockout(&vd,&vb)==1){
				/* analysis, assume we want to use bitrate management */
				vorbis_analysis(&vb,NULL);
				vorbis_bitrate_addblock(&vb);
				while(vorbis_bitrate_flushpacket(&vd,&op)){
					/* weld the packet into the bitstream */
					ogg_stream_packetin(&os,&op);
					/* write out pages (if any) */
					while(!eos){
						int result=ogg_stream_pageout(&os,&og);
						if(result==0) break;
						fwrite(og.header,1,og.header_len,fout);
						fwrite(og.body,1,og.body_len,fout);

						/* this could be set above, but for illustrative purposes, I do
						 it here (to show that vorbis does know where the stream ends) */
						if(ogg_page_eos(&og))eos=1;
					}
				}
			}

			// Report Progress
			if (m_wndProgress.Cancelled()) {
				break;
			}
			int per = (int)(((double)nOutputSize*100.0)/(double)nInputFileSize);
			if (per > intPercent) {
				intPercent = per;
				CString szPercent;
				szPercent.Format(_T("%d%%"), intPercent);
				m_wndProgress.SetText(strMsg, szPercent);
				m_wndProgress.SetPos(intPercent);
				m_wndProgress.PeekAndPump();
			} else {
				if (++k > 1000) {
					m_wndProgress.PeekAndPump();
					k = 0;
				}
			}
		}

		if (m_wndProgress.Cancelled()) {
			// Hide()はCancelイベント内でコール済み
			ret = 0;
		} else {
			m_wndProgress.Hide();
			ret = 1;
		}

		/* clean up and exit.  vorbis_info_clear() must be called last */
		ogg_stream_clear(&os);
		vorbis_block_clear(&vb);
		vorbis_dsp_clear(&vd);
		vorbis_comment_clear(&vc);
		vorbis_info_clear(&vi);
	}
    CATCH_ALL(e)
    {
		CString strErr;
		_TCHAR   szCause[1024];
 		e->GetErrorMessage(szCause, 1024);
		strErr.Format(_T("File encoder error:%s\n"), szCause);
		AfxMessageBox(strErr);
		ret = -1;
    }
	END_CATCH_ALL

Encode_END:
	fclose(fin);
	fclose(fout);
	return(ret);
}
