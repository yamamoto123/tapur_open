// "winmm.lib" and "msacm32.lib" must be imported

// #include <mmsystem.h>
// #include <mmreg.h>
// #include <msacm.h>

#include "Forcelib.h"
#include <windows.h>
#include "WaveFileACM.h"

#define	RESERVED	0
#define	LAW_SIZE	0xc000
#define	CMP_SIZE	0x3000

CWaveFileACM::CWaveFileACM()
{
	FileName[0]=0x00;
	hmmio=NULL;
	FlagW=FALSE;
	ZeroMemory(&acmsh1,sizeof(ACMSTREAMHEADER));
	acmsh1.cbStruct=sizeof(ACMSTREAMHEADER);
	ZeroMemory(&acmsh2,sizeof(ACMSTREAMHEADER));
	acmsh2.cbStruct=sizeof(ACMSTREAMHEADER);
	pwfex1=pwfex2=NULL;
	hacms1=hacms2=NULL;
	ckRIFF.ckid=FOURCC_RIFF;
	ckRIFF.fccType=mmioFOURCC('W','A','V','E');
	ckfmt.ckid=mmioFOURCC('f','m','t',' ');
	ckfact.ckid=mmioFOURCC('f','a','c','t');
	ckdata.ckid=mmioFOURCC('d','a','t','a');
}

CWaveFileACM::~CWaveFileACM()
{
	Close();
}

BOOL CWaveFileACM::Open(const TCHAR *pFileName)
{
	if(hmmio!=NULL)
		return FALSE;
	FlagW=FALSE;
	if(pFileName!=NULL)
		lstrcpyn(FileName, pFileName, MAX_PATH);
	if((hmmio=mmioOpen(FileName, NULL,MMIO_READ | MMIO_ALLOCBUF))==NULL)
		return FALSE;
//	if(mmioSetBuffer(hmmio,NULL,MMIO_DEFAULTBUFFER*4,RESERVED)==MMIOERR_OUTOFMEMORY)
//		if(mmioSetBuffer(hmmio,NULL,MMIO_DEFAULTBUFFER*2,RESERVED)==MMIOERR_OUTOFMEMORY)
//			if(mmioSetBuffer(hmmio,NULL,MMIO_DEFAULTBUFFER,RESERVED)==MMIOERR_OUTOFMEMORY)
//				return FALSE;
	if(mmioDescend(hmmio, &ckRIFF, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR)
		goto closeANDreturn;
	if(mmioDescend(hmmio, &ckfmt, &ckRIFF, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
		goto closeANDreturn;
	if(ckfmt.cksize < sizeof(PCMWAVEFORMAT))
		goto closeANDreturn;
	if(mmioRead(hmmio, (HPSTR)&wfex, sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT))
		goto closeANDreturn;
	if(wfex.wFormatTag == WAVE_FORMAT_PCM)
		wfex.cbSize=0;
	else {
		if(mmioRead(hmmio,(HPSTR)&wfex.cbSize,sizeof(WORD))!=sizeof(WORD))
			goto closeANDreturn;
		pwfex1=(LPWAVEFORMATEX)GlobalAlloc(GMEM_FIXED,sizeof(WAVEFORMATEX)+wfex.cbSize);
		if(pwfex1==NULL)
			goto closeANDreturn;
		CopyMemory(pwfex1,&wfex,sizeof(WAVEFORMATEX));
		if(mmioRead(hmmio,(HPSTR)(pwfex1)+sizeof(WAVEFORMATEX),(int)(wfex.cbSize)) != (int)(wfex.cbSize) )
			goto closeANDreturn;
		wfex.wFormatTag=WAVE_FORMAT_PCM;
		wfex.cbSize=0;
// 出力フォーマットを得る
		if(acmFormatSuggest(NULL,pwfex1,&wfex,sizeof(WAVEFORMATEX),ACM_FORMATSUGGESTF_WFORMATTAG)!=0)
			goto closeANDreturn;
// ACMストリームを開く
		if(acmStreamOpen(&hacms1,NULL,pwfex1,&wfex,NULL,0,0,ACM_STREAMOPENF_NONREALTIME)!=0)
			goto closeANDreturn;
// 入力バッファのサイズ
		acmsh1.dwSrcUser=LAW_SIZE-LAW_SIZE%pwfex1->nBlockAlign;
		acmsh1.cbSrcLengthUsed=acmsh1.cbSrcLength=acmsh1.dwSrcUser;
// 出力バッファのサイズを得る
		if(acmStreamSize(hacms1,acmsh1.dwSrcUser,&acmsh1.cbDstLength,ACM_STREAMSIZEF_SOURCE)!=0)
			goto closeANDreturn;
		if((acmsh1.cbDstLength & 0x0fff) != 0) {
			acmsh1.cbDstLength&=0xfffff000;
			acmsh1.cbDstLength+=0x1000;
		}
// 出力バッファの開始位置
		acmsh1.dwDstUser=0;
		acmsh1.pbSrc=(LPBYTE)GlobalAlloc(GMEM_FIXED,acmsh1.cbSrcLength);
		if(acmsh1.pbSrc==NULL)
			goto closeANDreturn;
		acmsh1.pbDst=(LPBYTE)GlobalAlloc(GMEM_FIXED,acmsh1.cbDstLength);
		if(acmsh1.pbDst==NULL)
			goto closeANDreturn;
	}
	if(mmioAscend(hmmio, &ckfmt, RESERVED) != MMSYSERR_NOERROR)
		goto closeANDreturn;
//	if(mmioSeek(hmmio, ckRIFF.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)
//		goto closeANDreturn;
	if(mmioDescend(hmmio,&ckdata,&ckRIFF,MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
		goto closeANDreturn;
	dataCP=0;
	dataEP=ckdata.cksize;
	return TRUE;
closeANDreturn:
	Close();
	return FALSE;
}

BOOL CWaveFileACM::Create(const TCHAR *pFileName)
{
	if(hmmio!=NULL)
		return FALSE;
	FlagW=TRUE;
	dataCP=0;
// wfex.wFormatTag==0 ならソフト側のフォーマットはACMが決める
	if(pwfex1==NULL) {
		if(wfex.wFormatTag!=WAVE_FORMAT_PCM)
			return FALSE;
	}
	else {
		if(wfex.wFormatTag==WAVE_FORMAT_PCM) {
			if(pwfex1->wFormatTag!=WAVE_FORMAT_PCM) {
// wfex -> pwfex2 -> pwfex1 の順で変換
				if(pwfex2==NULL) {
					pwfex2=(LPWAVEFORMATEX)GlobalAlloc(GMEM_FIXED,sizeof(WAVEFORMATEX));
					if(pwfex2==NULL)
						return FALSE;
				}
					pwfex2->wFormatTag=WAVE_FORMAT_PCM;
				pwfex2->cbSize=0;
				if(acmFormatSuggest(NULL,pwfex1,pwfex2,sizeof(WAVEFORMATEX),ACM_FORMATSUGGESTF_WFORMATTAG)!=0)
					return FALSE;
				if(memcmp(pwfex2,&wfex,sizeof(WAVEFORMATEX))==0)
					pwfex2=(LPWAVEFORMATEX)GlobalFree((HGLOBAL)pwfex2);
			}
			else {	// pwfex1->wFormatTag==WAVE_FORMAT_PCM
				if(memcmp(&wfex,pwfex1,sizeof(WAVEFORMATEX))==0)
					pwfex1=(LPWAVEFORMATEX)GlobalFree((HGLOBAL)pwfex1);
			}
		}
		else {	// wfex.wFormatTag!=WAVE_FORMAT_PCM
			if(pwfex1->wFormatTag==WAVE_FORMAT_PCM) {
				CopyMemory(&wfex,pwfex1,sizeof(WAVEFORMATEX));
				pwfex1=(LPWAVEFORMATEX)GlobalFree((HGLOBAL)pwfex1);
			}
			else {	// pwfex1->wFormatTag!=WAVE_FORMAT_PCM
				wfex.wFormatTag=WAVE_FORMAT_PCM;
				wfex.cbSize=0;
				if(acmFormatSuggest(NULL,pwfex1,&wfex,sizeof(WAVEFORMATEX),ACM_FORMATSUGGESTF_WFORMATTAG)!=0)
					return FALSE;
			}
		}
	}
	if(pFileName!=NULL)
		lstrcpyn(FileName,pFileName,256);
	if((hmmio=mmioOpen(FileName,NULL,MMIO_READWRITE | MMIO_CREATE | MMIO_ALLOCBUF))==NULL)
		return FALSE;
//	if(mmioSetBuffer(hmmio,NULL,MMIO_DEFAULTBUFFER*4,RESERVED)==MMIOERR_OUTOFMEMORY)
//		if(mmioSetBuffer(hmmio,NULL,MMIO_DEFAULTBUFFER*2,RESERVED)==MMIOERR_OUTOFMEMORY)
//			if(mmioSetBuffer(hmmio,NULL,MMIO_DEFAULTBUFFER,RESERVED)==MMIOERR_OUTOFMEMORY)
//				return FALSE;
	ckRIFF.cksize=0;
	if(mmioCreateChunk(hmmio,&ckRIFF,MMIO_CREATERIFF) != MMSYSERR_NOERROR)
		goto closeANDreturn;
	if(pwfex1==NULL)
		ckfmt.cksize=sizeof(PCMWAVEFORMAT);
	else
		if(pwfex1->wFormatTag==WAVE_FORMAT_PCM)
			ckfmt.cksize=sizeof(PCMWAVEFORMAT);
		else
			ckfmt.cksize=sizeof(WAVEFORMATEX)+pwfex1->cbSize;
	if(mmioCreateChunk(hmmio,&ckfmt,0) != MMSYSERR_NOERROR)
		goto closeANDreturn;
	if(pwfex1==NULL) {
		if(mmioWrite(hmmio,(HPSTR)&wfex,sizeof(PCMWAVEFORMAT))!= sizeof(PCMWAVEFORMAT))
			goto closeANDreturn;
	}
	else {
		if(mmioWrite(hmmio,(HPSTR)pwfex1,ckfmt.cksize)!=(int)ckfmt.cksize)
			goto closeANDreturn;
		acmsh1.cbDstLength=CMP_SIZE-CMP_SIZE%pwfex1->nBlockAlign;
		acmsh1.pbDst=(LPBYTE)GlobalAlloc(GMEM_FIXED,acmsh1.cbDstLength);
		if(acmsh1.pbDst==NULL)
			goto closeANDreturn;
		if(pwfex2==NULL) {
			if(acmStreamOpen(&hacms1,NULL,&wfex,pwfex1,NULL,0,0,ACM_STREAMOPENF_NONREALTIME)!=0)
				goto closeANDreturn;
		}
		else {
			if(acmStreamOpen(&hacms2,NULL,&wfex,pwfex2,NULL,0,0,ACM_STREAMOPENF_NONREALTIME)!=0)
				goto closeANDreturn;
			if(acmStreamOpen(&hacms1,NULL,pwfex2,pwfex1,NULL,0,0,ACM_STREAMOPENF_NONREALTIME)!=0)
				goto closeANDreturn;
		}
		if(acmStreamSize(hacms1,acmsh1.cbDstLength,&acmsh1.cbSrcLength,ACM_STREAMSIZEF_DESTINATION)!=0)
			goto closeANDreturn;
		acmsh1.pbSrc=(LPBYTE)GlobalAlloc(GMEM_FIXED,acmsh1.cbSrcLength);
		if(acmsh1.pbSrc==NULL)
			goto closeANDreturn;
	}
	if(mmioAscend(hmmio,&ckfmt,RESERVED) != MMSYSERR_NOERROR)
		goto closeANDreturn;
	ckfact.cksize=sizeof(DWORD);
	if(mmioCreateChunk(hmmio,&ckfact,0) != MMSYSERR_NOERROR)
		goto closeANDreturn;
	if(mmioWrite(hmmio,(HPSTR)&dataCP,sizeof(DWORD)) != sizeof(DWORD))
		goto closeANDreturn;
	if(mmioAscend(hmmio,&ckfact,RESERVED) != MMSYSERR_NOERROR)
		goto closeANDreturn;
	ckdata.cksize=0;
	if(mmioCreateChunk(hmmio,&ckdata,0) != MMSYSERR_NOERROR)
		goto closeANDreturn;
	if(mmioFlush(hmmio,MMIO_EMPTYBUF)!=0)
		goto closeANDreturn;
	return TRUE;
closeANDreturn:
	Close();
	return FALSE;
}

BOOL CWaveFileACM::FormatChooseDlg(HWND hWnd)
{
	ACMFORMATCHOOSE acmfc;
	int max;

	if(hmmio!=NULL)
		return FALSE;
	if(acmMetrics(NULL,ACM_METRIC_MAX_SIZE_FORMAT,&max)!=0)
		max=256;
	if(pwfex1==NULL) {
		pwfex1=(LPWAVEFORMATEX)GlobalAlloc(GMEM_FIXED,max);
		if(pwfex1==NULL)
			return FALSE;
		pwfex1->wFormatTag=0;
	}
	ZeroMemory(&acmfc,sizeof(ACMFORMATCHOOSE));
	acmfc.cbStruct=sizeof(ACMFORMATCHOOSE);
	if(pwfex1->wFormatTag!=0)
		acmfc.fdwStyle=ACMFORMATCHOOSE_STYLEF_INITTOWFXSTRUCT;
	acmfc.hwndOwner=hWnd;
	acmfc.pwfx=pwfex1;
	acmfc.cbwfx=max;
//	acmfc.pszTitle=NULL;
//	acmfc.szFormatTag[0]=0;
//	acmfc.szFormat[0]=0;
//	acmfc.pszName=NULL;
//	acmfc.cchName=0;
//	acmfc.fdwEnum=0;
//	acmfc.pwfxEnum=NULL;
//	acmfc.hInstance=NULL;
//	acmfc.pszTemplateName=NULL;
//	acmfc.lCustData=0;
//	acmfc.pfnHook=NULL;
	acmFormatChoose(&acmfc);
	if(pwfex1->wFormatTag==0) {
		pwfex1=(LPWAVEFORMATEX)GlobalFree((HGLOBAL)pwfex1);
		return FALSE;
	}
	if(pwfex1->wFormatTag==WAVE_FORMAT_PCM)
		pwfex1->cbSize=0;
	return TRUE;
}

// paak
BOOL CWaveFileACM::FormatChoose(int SamplesPerSec,int BitsPerSample,int Channels)
{
	int max;

	if(acmMetrics(NULL,ACM_METRIC_MAX_SIZE_FORMAT,&max)!=0)
		max=256;
	if(pwfex1==NULL) {
		pwfex1=(LPWAVEFORMATEX)GlobalAlloc(GMEM_FIXED,max);
		if(pwfex1==NULL) return FALSE;
	}

	pwfex1->wFormatTag=WAVE_FORMAT_PCM;
	pwfex1->nChannels=Channels;
	pwfex1->nSamplesPerSec=SamplesPerSec;
	pwfex1->nBlockAlign=Channels*(BitsPerSample>>3);
	pwfex1->nAvgBytesPerSec=SamplesPerSec*pwfex1->nBlockAlign;
	pwfex1->wBitsPerSample=BitsPerSample;
	pwfex1->cbSize=0;

	return TRUE;
}

void CWaveFileACM::FormatSet(const WAVEFORMATEX *pwfex)
{
	if(pwfex!=NULL)
		CopyMemory(&wfex,pwfex,sizeof(WAVEFORMATEX));
	else
		ZeroMemory(&wfex,sizeof(WAVEFORMATEX));
}

BOOL CWaveFileACM::FormatSet(int SamplesPerSec,int BitsPerSample,int Channels)
{
	wfex.wFormatTag=WAVE_FORMAT_PCM;
	wfex.nChannels=Channels;
	wfex.nSamplesPerSec=SamplesPerSec;
	wfex.nBlockAlign=Channels*(BitsPerSample>>3);
	wfex.nAvgBytesPerSec=SamplesPerSec*wfex.nBlockAlign;
	wfex.wBitsPerSample=BitsPerSample;
	wfex.cbSize=0;
	return TRUE;
}

void CWaveFileACM::Close(void)
{
	DWORD Samples;

	if(hmmio==NULL)
		goto close;
	if(FlagW) {
		if(hacms1!=NULL)
			if(acmsh1.cbSrcLengthUsed!=0) {
				acmsh1.fdwStatus=0;
				acmsh1.cbSrcLength=acmsh1.cbSrcLengthUsed;
				if(acmStreamPrepareHeader(hacms1,&acmsh1,RESERVED)!=0)
					goto close;
				Samples=acmStreamConvert(hacms1,&acmsh1,0);
				acmStreamUnprepareHeader(hacms1,&acmsh1,RESERVED);
				if(Samples!=0)
					goto close;
				dataCP+=mmioWrite(hmmio,(HPSTR)acmsh1.pbDst,acmsh1.cbDstLengthUsed);
			}
		if(mmioAscend(hmmio,&ckdata,RESERVED) != MMSYSERR_NOERROR)
			goto close;
		if(mmioAscend(hmmio,&ckRIFF,RESERVED) != MMSYSERR_NOERROR)
			goto close;
		mmioSeek(hmmio,0,SEEK_SET);
		if(mmioDescend(hmmio,&ckRIFF,NULL,MMIO_FINDRIFF) != MMSYSERR_NOERROR)
			goto close;
		if(mmioDescend(hmmio,&ckfact,&ckRIFF,MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
			goto close;
		if(pwfex1==NULL)
			Samples=dataCP/wfex.nBlockAlign;
		else {
			if(pwfex2==NULL)
				acmStreamSize(hacms1,dataCP,&Samples,ACM_STREAMSIZEF_DESTINATION);
			else {
				acmStreamSize(hacms1,dataCP,&Samples,ACM_STREAMSIZEF_DESTINATION);
				acmStreamSize(hacms2,Samples,&Samples,ACM_STREAMSIZEF_DESTINATION);
			}
			Samples/=wfex.nBlockAlign;
		}
		mmioWrite(hmmio,(HPSTR)&Samples,sizeof(DWORD));
		ckfact.dwFlags=MMIO_DIRTY;
		if(mmioAscend(hmmio,&ckfact,RESERVED) != MMSYSERR_NOERROR)
			goto close;
		if(mmioAscend(hmmio,&ckRIFF,RESERVED) != MMSYSERR_NOERROR)
			goto close;
	}
	else {
		if(mmioAscend(hmmio,&ckdata,RESERVED) != MMSYSERR_NOERROR)
			goto close;
		if(mmioAscend(hmmio,&ckRIFF,RESERVED) != MMSYSERR_NOERROR)
			goto close;
	}
close:
	if(acmsh1.pbSrc!=NULL)
		acmsh1.pbSrc=(LPBYTE)GlobalFree((HGLOBAL)acmsh1.pbSrc);
	if(acmsh1.pbDst!=NULL)
		acmsh1.pbDst=(LPBYTE)GlobalFree((HGLOBAL)acmsh1.pbDst);
	ZeroMemory(&acmsh1,sizeof(ACMSTREAMHEADER));
	acmsh1.cbStruct=sizeof(ACMSTREAMHEADER);
	ZeroMemory(&acmsh2,sizeof(ACMSTREAMHEADER));
	acmsh2.cbStruct=sizeof(ACMSTREAMHEADER);
	if(hacms2!=NULL) {
		acmStreamClose(hacms2,RESERVED);
		hacms2=NULL;
	}
	if(hacms1!=NULL) {
		acmStreamClose(hacms1,RESERVED);
		hacms1=NULL;
	}
	if(hmmio!=NULL) {
		mmioClose(hmmio,0);
		hmmio=NULL;
	}
	if(pwfex2!=NULL)
		pwfex2=(LPWAVEFORMATEX)GlobalFree((HGLOBAL)pwfex2);
	if(pwfex1!=NULL)
		pwfex1=(LPWAVEFORMATEX)GlobalFree((HGLOBAL)pwfex1);
}

int CWaveFileACM::Read(void *p,int c)
{
	int i,n;

	if(hmmio==NULL)
		return 0;
	if(FlagW)
		return 0;
	if(c==0)
		return 0;
	if(hacms1==NULL) {
		if(c+dataCP>dataEP)
			c=dataEP-dataCP;
		n=(unsigned int)mmioRead(hmmio,(HPSTR)p,c);
		if(n==-1)
			return -1;
		dataCP+=n;
		return n;
	}
	n=0;
	for(;;) {
		if((DWORD)c<acmsh1.cbDstLengthUsed) {
			CopyMemory(p,acmsh1.pbDst+acmsh1.dwDstUser,c);
			acmsh1.cbDstLengthUsed-=c;
			acmsh1.dwDstUser+=c;
			n+=c;
			break;
		}
		if(acmsh1.cbDstLengthUsed!=0) {
			CopyMemory(p,acmsh1.pbDst+acmsh1.dwDstUser,acmsh1.cbDstLengthUsed);
			c-=acmsh1.cbDstLengthUsed;
			n+=acmsh1.cbDstLengthUsed;
			p=(LPBYTE)p+acmsh1.cbDstLengthUsed;
			acmsh1.cbDstLengthUsed=0;
			acmsh1.dwDstUser=0;
		}
		if(acmsh1.cbSrcLength!=acmsh1.dwSrcUser)
			break;
		i=acmsh1.cbSrcLengthUsed;
		if(i+dataCP>dataEP)
			i=dataEP-dataCP;
		if(acmsh1.cbSrcLength!=acmsh1.cbSrcLengthUsed)
			CopyMemory(acmsh1.pbSrc,acmsh1.pbSrc+acmsh1.cbSrcLengthUsed,acmsh1.cbSrcLength-acmsh1.cbSrcLengthUsed);
		i=(int)mmioRead(hmmio,(HPSTR)acmsh1.pbSrc+acmsh1.cbSrcLength-acmsh1.cbSrcLengthUsed,i);
		if(i==-1)
			return -1;
		dataCP+=i;
		acmsh1.fdwStatus=0;
		if((DWORD)c<acmsh1.cbDstLength)
			if((DWORD)i==acmsh1.cbSrcLengthUsed) {
				if(acmStreamPrepareHeader(hacms1,&acmsh1,RESERVED)!=0)
					return -1;
				i=acmStreamConvert(hacms1,&acmsh1,ACM_STREAMCONVERTF_BLOCKALIGN);
				acmStreamUnprepareHeader(hacms1,&acmsh1,RESERVED);
				if(i!=0)
					return -1;
			}
			else {
				acmsh1.cbSrcLength=i;
				if(acmStreamPrepareHeader(hacms1,&acmsh1,RESERVED)!=0)
					return -1;
				i=acmStreamConvert(hacms1,&acmsh1,0);
				acmStreamUnprepareHeader(hacms1,&acmsh1,RESERVED);
				if(i!=0)
					return -1;
			}
		else {
			acmsh1.dwDstUser=(DWORD)acmsh1.pbDst;
			acmsh1.pbDst=(LPBYTE)p;
			if(acmStreamPrepareHeader(hacms1,&acmsh1,RESERVED)!=0)
				return -1;
			i=acmStreamConvert(hacms1,&acmsh1,ACM_STREAMCONVERTF_BLOCKALIGN);
			acmStreamUnprepareHeader(hacms1,&acmsh1,RESERVED);
			if(i!=0)
				return -1;
			c-=acmsh1.cbDstLengthUsed;
			n+=acmsh1.cbDstLengthUsed;
			p=(LPBYTE)p+acmsh1.cbDstLengthUsed;
			acmsh1.pbDst=(LPBYTE)acmsh1.dwDstUser;
			acmsh1.cbDstLengthUsed=0;
			acmsh1.dwDstUser=0;
		}
	}
	return n;
}

int CWaveFileACM::Write(const void *p,int c)
{
	int i,j,n;

	if(hmmio==NULL)
		return 0;
	if(!FlagW)
		return 0;
	if(c==0)
		return 0;
	if(hacms1==NULL) {
		n=(int)mmioWrite(hmmio,(HPSTR)p,c);
		if(n==-1)
			return -1;
		dataCP+=n;
		return n;
	}
	n=0;
	if(hacms2==NULL)
		while(c!=0) {
// acmsh1.cbSrcLengthUsed は格納済みのデータ量
			i=acmsh1.cbSrcLength-acmsh1.cbSrcLengthUsed;
			if(c<i) {
				CopyMemory(acmsh1.pbSrc+acmsh1.cbSrcLengthUsed,p,c);
				acmsh1.cbSrcLengthUsed+=c;
				n+=c;
				break;
			}
			acmsh1.fdwStatus=0;
			if(acmsh1.cbSrcLengthUsed!=0) {
				CopyMemory(acmsh1.pbSrc+acmsh1.cbSrcLengthUsed,p,i);
				n+=i;
				c-=i;
				p=(LPBYTE)p+i;
				if(acmStreamPrepareHeader(hacms1,&acmsh1,RESERVED)!=0)
					return -1;
				i=acmStreamConvert(hacms1,&acmsh1,ACM_STREAMCONVERTF_BLOCKALIGN);
				acmStreamUnprepareHeader(hacms1,&acmsh1,RESERVED);
				if(i!=0)
					return -1;
				i=acmsh1.cbSrcLength-acmsh1.cbSrcLengthUsed;
				if(i!=0)
					CopyMemory(acmsh1.pbSrc,acmsh1.pbSrc+acmsh1.cbSrcLengthUsed,i);
				acmsh1.cbSrcLengthUsed=i;
			}
			else {
				acmsh1.dwSrcUser=(DWORD)acmsh1.pbSrc;
				acmsh1.pbSrc=(LPBYTE)p;
				if(acmStreamPrepareHeader(hacms1,&acmsh1,RESERVED)!=0)
					return -1;
				i=acmStreamConvert(hacms1,&acmsh1,ACM_STREAMCONVERTF_BLOCKALIGN);
				acmStreamUnprepareHeader(hacms1,&acmsh1,RESERVED);
				if(i!=0)
					return -1;
				acmsh1.pbSrc=(LPBYTE)acmsh1.dwSrcUser;
				n+=acmsh1.cbSrcLengthUsed;
				c-=acmsh1.cbSrcLengthUsed;
				p=(LPBYTE)p+acmsh1.cbSrcLengthUsed;
				acmsh1.cbSrcLengthUsed=0;
			}
			i=mmioWrite(hmmio,(HPSTR)acmsh1.pbDst,acmsh1.cbDstLengthUsed);
			if(i!=-1)
				dataCP+=i;
			else
				return -1;
		}
	else	// hacms2!=NULL
		while(c!=0) {
			i=acmsh1.cbSrcLength-acmsh1.cbSrcLengthUsed;
			acmStreamSize(hacms2,i,(LPDWORD)&j,ACM_STREAMSIZEF_DESTINATION);
			acmsh2.fdwStatus=0;
			acmsh2.pbDst=acmsh1.pbSrc+acmsh1.cbSrcLengthUsed;
			acmsh2.pbSrc=(LPBYTE)p;
			if(c<j) {
				acmsh2.cbSrcLength=c;
				acmsh2.cbDstLength=i;
				if(acmStreamPrepareHeader(hacms2,&acmsh2,RESERVED)!=0)
					return -1;
				i=acmStreamConvert(hacms2,&acmsh2,ACM_STREAMCONVERTF_BLOCKALIGN);
				acmStreamUnprepareHeader(hacms2,&acmsh2,RESERVED);
				if(i!=0)
					return -1;
				acmsh1.cbSrcLengthUsed+=acmsh2.cbDstLengthUsed;
				n+=c;
				break;
			}
			acmsh2.cbSrcLength=j;
			acmsh2.cbDstLength=i;
			if(acmStreamPrepareHeader(hacms2,&acmsh2,RESERVED)!=0)
				return -1;
			i=acmStreamConvert(hacms2,&acmsh2,ACM_STREAMCONVERTF_BLOCKALIGN);
			acmStreamUnprepareHeader(hacms2,&acmsh2,RESERVED);
			if(i!=0)
				return -1;
			n+=j;
			c-=j;
			p=(LPBYTE)p+j;
			acmsh1.fdwStatus=0;
			if(acmStreamPrepareHeader(hacms1,&acmsh1,RESERVED)!=0)
				return -1;
			i=acmStreamConvert(hacms1,&acmsh1,ACM_STREAMCONVERTF_BLOCKALIGN);
			acmStreamUnprepareHeader(hacms1,&acmsh1,RESERVED);
			if(i!=0)
				return -1;
			i=acmsh1.cbSrcLength-acmsh1.cbSrcLengthUsed;
			if(i!=0)
				CopyMemory(acmsh1.pbSrc,acmsh1.pbSrc+acmsh1.cbSrcLengthUsed,i);
			acmsh1.cbSrcLengthUsed=i;
			i=mmioWrite(hmmio,(HPSTR)acmsh1.pbDst,acmsh1.cbDstLengthUsed);
			if(i!=-1)
				dataCP+=i;
			else
				return -1;
		}
	return n;
}

int CWaveFileACM::Seek(unsigned int dataP)
{
	if(hmmio==NULL)
		return -1;
	if(FlagW)
		return -1;
	if(pwfex1!=NULL) {
		acmStreamSize(hacms1,dataP,(LPDWORD)&dataP,ACM_STREAMSIZEF_DESTINATION);
		dataP-=dataP%pwfex1->nBlockAlign;
		if(mmioSeek(hmmio,dataP+ckdata.dwDataOffset,SEEK_SET)==-1)
			return -1;
		acmsh1.cbSrcLength=acmsh1.dwSrcUser;
		acmsh1.cbSrcLengthUsed=0;
		acmsh1.cbDstLengthUsed=0;
		acmsh1.dwDstUser=0;
	}
	else
		if(mmioSeek(hmmio,dataP+ckdata.dwDataOffset,SEEK_SET)==-1)
			return -1;
	dataCP=dataP;
	return (int)dataP;
}


/*
typedef struct {
    FOURCC ckid;
    DWORD  cksize;
    FOURCC fccType;
    DWORD  dwDataOffset;
    DWORD  dwFlags;
} MMCKINFO;

typedef struct {
    WAVEFORMAT wf;
    WORD       wBitsPerSample;
} PCMWAVEFORMAT;

typedef struct {
    WORD  wFormatTag;
    WORD  nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD  nBlockAlign;
    WORD  wBitsPerSample;
    WORD  cbSize;
} WAVEFORMATEX;

typedef struct {
    DWORD  cbStruct;
    DWORD  fdwStatus;
    DWORD  dwUser;
    LPBYTE pbSrc;
    DWORD  cbSrcLength;
    DWORD  cbSrcLengthUsed;
    DWORD  dwSrcUser;
    LPBYTE pbDst;
    DWORD  cbDstLength;
    DWORD  cbDstLengthUsed;
    DWORD  dwDstUser;
    DWORD  dwReservedDriver[10];
} ACMSTREAMHEADER;

typedef struct {
    DWORD                   cbStruct;
    DWORD                   fdwStyle;
    HWND                    hwndOwner;
    LPWAVEFORMATEX          pwfx;
    DWORD                   cbwfx;
    LPCSTR                  pszTitle;
    char szFormatTag[ACMFORMATTAGDETAILS_FORMATTAG_CHARS];
    char szFormat[ACMFORMATDETAILS_FORMAT_CHARS];
    LPSTR                   pszName;
    DWORD                   cchName;
    DWORD                   fdwEnum;
    LPWAVEFORMATEX          pwfxEnum;
    HINSTANCE               hInstance;
    LPCSTR                  pszTemplateName;
    LPARAM                  lCustData;
    ACMFORMATCHOOSEHOOKPROC pfnHook;
} ACMFORMATCHOOSE;

#define  WAVE_FORMAT_UNKNOWN    0x0000
#define  WAVE_FORMAT_PCM        0x0001

#define ACMERR_BASE         (512)
#define ACMERR_NOTPOSSIBLE  (ACMERR_BASE + 0)
#define ACMERR_BUSY         (ACMERR_BASE + 1)
#define ACMERR_UNPREPARED   (ACMERR_BASE + 2)
#define ACMERR_CANCELED     (ACMERR_BASE + 3)

#define MMSYSERR_NOERROR       0
#define MMSYSERR_BASE          0
#define MMSYSERR_ERROR        (MMSYSERR_BASE + 1)
#define MMSYSERR_BADDEVICEID  (MMSYSERR_BASE + 2)
#define MMSYSERR_NOTENABLED   (MMSYSERR_BASE + 3)
#define MMSYSERR_ALLOCATED    (MMSYSERR_BASE + 4)
#define MMSYSERR_INVALHANDLE  (MMSYSERR_BASE + 5)
#define MMSYSERR_NODRIVER     (MMSYSERR_BASE + 6)
#define MMSYSERR_NOMEM        (MMSYSERR_BASE + 7)
#define MMSYSERR_NOTSUPPORTED (MMSYSERR_BASE + 8)
#define MMSYSERR_BADERRNUM    (MMSYSERR_BASE + 9)
#define MMSYSERR_INVALFLAG    (MMSYSERR_BASE + 10)
#define MMSYSERR_INVALPARAM   (MMSYSERR_BASE + 11)
#define MMSYSERR_HANDLEBUSY   (MMSYSERR_BASE + 12)
#define MMSYSERR_INVALIDALIAS (MMSYSERR_BASE + 13)
#define MMSYSERR_BADDB        (MMSYSERR_BASE + 14)
#define MMSYSERR_KEYNOTFOUND  (MMSYSERR_BASE + 15)
#define MMSYSERR_READERROR    (MMSYSERR_BASE + 16)
#define MMSYSERR_WRITEERROR   (MMSYSERR_BASE + 17)
#define MMSYSERR_DELETEERROR  (MMSYSERR_BASE + 18)
#define MMSYSERR_VALNOTFOUND  (MMSYSERR_BASE + 19)
#define MMSYSERR_NODRIVERCB   (MMSYSERR_BASE + 20)
#define MMSYSERR_LASTERROR    (MMSYSERR_BASE + 20)
*/
