// AviStream.h : ヘッダー ファイル
//

#if !defined(VIDEOSTREAM_H__2A51C317_11C8_4D0E_ACDE_8DBA3F4FFEF8__INCLUDED_)
#define VIDEOSTREAM_H__2A51C317_11C8_4D0E_ACDE_8DBA3F4FFEF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vfw.h>
#include "AviStream.h"

/////////////////////////////////////////////////////////////////////////////
// CAviStream 

class CVideoStream : public CAviStream
{
private:
	/// <summary>handle for AVIStreamGetFrame</summary>
	int getFrameObject;
	/// <summary>size of an imge in bytes, stride*height</summary>
	int frameSize;
	int width;
	int height;
	short countBitsPerPixel;
    AVICOMPRESSOPTIONS compressOptions;

//	void Initialize(int aviFile, bool writeCompressed, double frameRate, CBitmap firstFrameBitmap);
//	short ConvertPixelFormatToBitCount(CPixelFormat format);
//	PixelFormat ConvertBitCountToPixelFormat(int bitCount);
    void GetRateAndScale(double &frameRate, int &scale);
    void CreateStreamWithoutFormat();
    void CreateStream();
	void CreateStream(AVICOMPRESSOPTIONS options);
	void CreateCompressedStream();
    void CreateCompressedStream(AVICOMPRESSOPTIONS options);
	void SetFormat(PAVISTREAM  pAviStream);

protected:
	double frameRate;
	/// <summary>count of frames in the stream</summary>
	int countFrames;
	/// <summary>initial frame index</summary>
	int firstFrame;

public:
	int FrameSize() { return frameSize; }
	double FrameRate() { return frameRate; }
	int Width() { return width; }
	int Height() { return height; }
    short CountBitsPerPixel() { return countBitsPerPixel; }
	int CountFrames() { return countFrames; }
	int FirstFrame() { return firstFrame; }
    AVICOMPRESSOPTIONS CompressOptions() { return compressOptions; }
	AVISTREAMINFO GetStreamInfo(PAVISTREAM  pAviStream);
//    AVISTREAMINFO StreamInfo() { return GetStreamInfo(m_pAviStream); }

//    CVideoStream(int aviFile, bool writeCompressed, double frameRate, int frameSize, int width, int height, PixelFormat format);
//    CVideoStream(int aviFile, bool writeCompressed, double frameRate, Bitmap firstFrame);
//    CVideoStream(int aviFile, Avi.AVICOMPRESSOPTIONS compressOptions, double frameRate, Bitmap firstFrame);
	CVideoStream(PAVIFILE pAviFile, PAVISTREAM pAviStream);
//    CVideoStream(int frameSize, double frameRate, int width, int height, short countBitsPerPixel, int countFrames, Avi.AVICOMPRESSOPTIONS compressOptions, bool writeCompressed);
//	void AddFrame(CBitmap bmp);
//	void GetFrameOpen();
//	void ExportBitmap(int position, CString dstFileName);
//	CBitmap GetBitmap(int position);
//	void GetFrameClose();
//	CAviManager *DecompressToNewFile(CString fileName, bool recompress, CVideoStream &newStream2);
//	void ExportStream(CString fileName);
};


#endif // !defined(VIDEOSTREAM_H__2A51C317_11C8_4D0E_ACDE_8DBA3F4FFEF8__INCLUDED_)

