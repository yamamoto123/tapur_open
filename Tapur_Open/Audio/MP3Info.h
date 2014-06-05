////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////                  Copyright (C) 2002, Roman Nurik                       ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef __MP3INFO_H_
#define __MP3INFO_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <afxtempl.h>

// list of genres
static _TCHAR* g_arrMP3Genre[] = {
	_T("Blues"),_T("Classic Rock"),_T("Country"),_T("Dance"),_T("Disco"),_T("Funk"),_T("Grunge"),_T("Hip-Hop"),
	_T("Jazz"),_T("Metal"),_T("New Age"),_T("Oldies"),_T("Other"),_T("Pop"),_T("R&B"),_T("Rap"),_T("Reggae"),_T("Rock"),
	_T("Techno"),_T("Industrial"),_T("Alternative"),_T("Ska"),_T("Death Metal"),_T("Pranks"),_T("Soundtrack"),
	_T("Euro-Techno"),_T("Ambient"),_T("Trip Hop"),_T("Vocal"),_T("Jazz Funk"),_T("Fusion"),_T("Trance"),
	_T("Classical"),_T("Instrumental"),_T("Acid"),_T("House"),_T("Game"),_T("Sound Clip"),_T("Gospel"),_T("Noise"),
	_T("Alternative Rock"),_T("Bass"),_T("Soul"),_T("Punk"),_T("Space"),_T("Meditative"),_T("Instrumental Pop"),
	_T("Instrumental Rock"),_T("Ethnic"),_T("Gothic"),_T("Darkwave"),_T("Techno-Industrial"),_T("Electronic"),
	_T("Pop Folk"),_T("Eurodance"),_T("Dream"),_T("Southern Rock"),_T("Comedy"),_T("Cult"),_T("Gangsta"),_T("Top 40"),
	_T("Christian Rap"),_T("Pop Funk"),_T("Jungle"),_T("Native American"),_T("Cabaret"),_T("New Wave"),
	_T("Psychadelic"),_T("Rave"),_T("ShowTunes"),_T("Trailer"),_T("Lo-Fi"),_T("Tribal"),_T("Acid Punk"),_T("Acid Jazz"),
	_T("Polka"),_T("Retro"),_T("Musical"),_T("Rock & Roll"),_T("Hard Rock"),_T("Folk"),_T("Folk Rock"),
	_T("National Folk"),_T("Swing"),_T("Fast Fusion"),_T("Bebob"),_T("Latin"),_T("Revival"),_T("Celtic"),
	_T("Bluegrass"),_T("Avantgarde"),_T("Gothic Rock"),_T("Progressive Rock"),_T("Psychedelic Rock"),
	_T("Symphonic Rock"),_T("Slow Rock"),_T("Big Band"),_T("Chorus"),_T("Easy Listening"),_T("Acoustic"),
	_T("Humour"),_T("Speech"),_T("Chanson"),_T("Opera"),_T("Chamber Music"),_T("Sonata"),_T("Symphony"),_T("Booty Bass"),
	_T("Primus"),_T("Porn Groove"),_T("Satire"),_T("Slow Jam"),_T("Club"),_T("Tango"),_T("Samba"),_T("Folklore"),_T("Ballad"),
	_T("Power Ballad"),_T("Rhytmic Soul"),_T("Freestyle"),_T("Duet"),_T("Punk Rock"),_T("Drum Solo"),_T("A Capella"),
	_T("Euro House"),_T("Dance Hall"),_T("Goa"),_T("Drum & Bass"),_T("Club House"),_T("Hardcore"),_T("Terror"),
	_T("Indie"),_T("BritPop"),_T("Negerpunk"),_T("Polsk Punk"),_T("Beat"),_T("Christian Gangsta Rap"),
	_T("Heavy Metal"),_T("Black Metal"),_T("Crossover"),_T("Contemporary Christian"),_T("Christian Rock"),
	_T("Merengue"),_T("Salsa"),_T("Trash Metal"),_T("Anime"),_T("JPop"),_T("SynthPop")
};

static int g_nMP3GenreCount = 148;

// matrix of bitrates [based on MPEG version data][bitrate index]
static int g_nMP3BitRate[6][16] = {
	{0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,-1},
	{0,32,48,56, 64, 80, 96,112,128,160,192,224,256,320,384,-1},
	{0,32,40,48, 56, 64, 80, 96,112,128,160,192,224,256,320,-1},
	{0,32,48,56, 64, 80, 96,112,128,144,160,176,192,224,256,-1},
	{0, 8,16,24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,-1},
	{0, 8,16,24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,-1},
	//paak {0, 8,16,24, 32, 64, 80, 56, 64,128,160,112,128,256,320,-1},
};

// MP3FRAMEHEADER structure
struct MP3FRAMEHEADER
{
	unsigned emphasis : 2;			// M
	unsigned original : 1;			// L
	unsigned copyright : 1;			// K
	unsigned modeext : 2;			// J
	unsigned chanmode : 2;			// I
	unsigned privbit : 1;			// H
	unsigned padding : 1;			// G
	unsigned samplerate : 2;		// F
	unsigned bitrate : 4;			// E
	unsigned hascrc : 1;			// D
	unsigned mpeglayer : 2;			// C
	unsigned mpegver : 2;			// B
	unsigned framesync : 11;		// A
};

// MP3ID3V1TAG structure
struct MP3ID3V1TAG
{
	char ident[3]; // TAG
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[28];
	BYTE reserved;
	BYTE tracknum;
	BYTE genre;
};

// ENMPEGVER enumeration
enum ENMPEGVER
{
	MPEGVER_NA,		// reserved, N/A
	MPEGVER_25,		// 2.5
	MPEGVER_2,		// 2.0 (ISO/IEC 13818-3)
	MPEGVER_1		// 1.0 (ISO/IEC 11172-3)
};

// ENCHANNELMODE enumeration
enum ENCHANNELMODE
{
	MP3CM_STEREO,
	MP3CM_JOINT_STEREO,
	MP3CM_DUAL_CHANNEL,
	MP3CM_SINGLE_CHANNEL
};

// ENEMPHASIS enumeration
enum ENEMPHASIS
{
	MP3EM_NONE,
	MP3EM_50_15_MS,		// 50/15 ms
	MP3EM_RESERVED,
	MP3EM_CCIT_J17		// CCIT J.17
};

// bitmask of validity of files
#define MP3VLD_INVALID			0x0000
#define MP3VLD_ID3V1_VALID		0x0001
#define MP3VLD_DATA_VALID		0x0002

// CMP3Info class
class CMP3Info
{
private:
	CString m_strFile;
	DWORD m_dwValidity;

	// ID3V1 information
	CString m_strArtist;
	CString m_strTitle;
	CString m_strAlbum;
	CString m_strComment;
	CString m_strYear;
	int m_nTrack;
	int m_nGenre;

	// MP3 frame information
	UINT m_nFrames;
	UINT m_nLength;					// in seconds
	ENMPEGVER m_enMPEGVersion;
	int m_nMPEGLayer;
	BOOL m_bHasCRC;
	int m_nBitRate;					// average if VBR, 0 if "free"
	int m_nSampleRate;
	ENCHANNELMODE m_enChannelMode;
	ENEMPHASIS m_enEmphasis;
	BOOL m_bCopyrighted;
	BOOL m_bOriginal;

public:
	CMP3Info(CString strFile = _T(""));
	virtual ~CMP3Info();

// operations
	BOOL Open(CString strFile);
	BOOL WriteID3V1(CString strDestFile = _T(""), BOOL bClear = FALSE); // defaults to current file
	void Clear();

	void SetArtist(CString strArtist){ m_strArtist = strArtist; }
	void SetTitle(CString strTitle){ m_strTitle = strTitle; }
	void SetAlbum(CString strAlbum){ m_strAlbum = strAlbum; }
	void SetComment(CString strComment){ m_strComment = strComment; }
	void SetYear(CString strYear){ m_strYear = strYear; }
	void SetTrack(int nTrack){ m_nTrack = nTrack; }
	void SetGenreIndex(int nGenre){ m_nGenre = nGenre; }

// attributes
	CString GetFileName() const { return m_strFile; }
	DWORD GetValidity() const { return m_dwValidity; }

	CString GetArtist() const { return m_strArtist; }
	CString GetTitle() const { return m_strTitle; }
	CString GetAlbum() const { return m_strAlbum; }
	CString GetComment() const { return m_strComment; }
	CString GetYear() const { return m_strYear; }
	int GetTrack() const { return m_nTrack; }
	int GetGenreIndex() const { return m_nGenre; }

	UINT GetNumFrames() const { return m_nFrames; }
	UINT GetLength() const { return m_nLength; }
	ENMPEGVER GetMPEGVersion() const { return m_enMPEGVersion; }
	int GetMPEGLayer() const { return m_nMPEGLayer; }
	BOOL HasCRC() const { return m_bHasCRC; }
	int GetBitRate() const { return m_nBitRate; }
	int GetSampleRate() const { return m_nSampleRate; }
	ENCHANNELMODE GetChannelMode() const { return m_enChannelMode; }
	ENEMPHASIS GetEmphasis() const { return m_enEmphasis; }
	BOOL IsCopyrighted() const { return m_bCopyrighted; }
	BOOL IsOriginal() const { return m_bOriginal; }

// functions
	static CString GetGenreString(int nIndex);
	static CString GetLengthString(int nSeconds);

private:
// helper functions
	BOOL GetNextFrameHeader(HANDLE hFile, MP3FRAMEHEADER* pHeader, int nPassBytes = 0);
	void ChangeEndian(void* pBuffer, int nBufSize);
};

#endif