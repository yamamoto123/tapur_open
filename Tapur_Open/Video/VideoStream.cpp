
#include "stdafx.h"
#include "VideoStream.h"

/*
/// <summary>Initialize an empty VideoStream</summary>
/// <param name="aviFile">The file that contains the stream</param>
/// <param name="writeCompressed">true: Create a compressed stream before adding frames</param>
/// <param name="frameRate">Frames per second</param>
/// <param name="frameSize">Size of one frame in bytes</param>
/// <param name="width">Width of each image</param>
/// <param name="height">Height of each image</param>
/// <param name="format">PixelFormat of the images</param>
CVideoStream::CVideoStream(int aviFile, bool writeCompressed, double frameRate, int frameSize, int width, int height, PixelFormat format) {
	aviFile = aviFile;
	writeCompressed = writeCompressed;
	frameRate = frameRate;
	frameSize = frameSize;
	width = width;
	height = height;
	countBitsPerPixel = ConvertPixelFormatToBitCount(format);
	firstFrame = 0;
	countFrames = 0;

	CreateStream();
}

/// <summary>Initialize a new VideoStream and add the first frame</summary>
/// <param name="aviFile">The file that contains the stream</param>
/// <param name="writeCompressed">true: create a compressed stream before adding frames</param>
/// <param name="frameRate">Frames per second</param>
/// <param name="firstFrame">Image to write into the stream as the first frame</param>
CVideoStream::CVideoStream(int aviFile, bool writeCompressed, double frameRate, Bitmap firstFrame) {
    Initialize(aviFile, writeCompressed, frameRate, firstFrame);
    CreateStream();
	AddFrame(firstFrame);
}

/// <summary>Initialize a new VideoStream and add the first frame</summary>
/// <param name="aviFile">The file that contains the stream</param>
/// <param name="writeCompressed">true: create a compressed stream before adding frames</param>
/// <param name="frameRate">Frames per second</param>
/// <param name="firstFrame">Image to write into the stream as the first frame</param>
CVideoStream::CVideoStream(int aviFile, AVICOMPRESSOPTIONS compressOptions, double frameRate, Bitmap firstFrame) {
    Initialize(aviFile, true, frameRate, firstFrame);
    CreateStream(compressOptions);
    AddFrame(firstFrame);
}
*/
/// <summary>Initialize a VideoStream for an existing stream</summary>
/// <param name="aviFile">The file that contains the stream</param>
/// <param name="aviStream">An IAVISTREAM from [aviFile]</param>
CVideoStream::CVideoStream(PAVIFILE pAviFile, PAVISTREAM pAviStream){
	m_pAviFile = pAviFile;
	m_pAviStream = pAviStream;
	
	BITMAPINFOHEADER bih;
	long size = sizeof(BITMAPINFOHEADER);
	AVIStreamReadFormat(pAviStream, 0, &bih, &size);
	AVISTREAMINFO streamInfo = GetStreamInfo(pAviStream);
	
	frameRate = (float)streamInfo.dwRate / (float)streamInfo.dwScale;
	width = (int)streamInfo.rcFrame.right;
	height = (int)streamInfo.rcFrame.bottom;
	frameSize = bih.biSizeImage;
	countBitsPerPixel = bih.biBitCount;
	firstFrame = AVIStreamStart(pAviStream);
	countFrames = AVIStreamLength(pAviStream);
}
/*
/// <summary>Copy all properties from one VideoStream to another one</summary>
/// <remarks>Used by EditableVideoStream</remarks>
/// <param name="frameSize"></param><param name="frameRate"></param>
/// <param name="width"></param><param name="height"></param>
/// <param name="countBitsPerPixel"></param>
/// <param name="countFrames"></param><param name="compressOptions"></param>
CVideoStream::CVideoStream(int frameSize, double frameRate, int width, int height, Int16 countBitsPerPixel, int countFrames, AVICOMPRESSOPTIONS compressOptions, bool writeCompressed) {
    frameSize = frameSize;
    frameRate = frameRate;
    width = width;
    height = height;
    countBitsPerPixel = countBitsPerPixel;
    countFrames = countFrames;
    compressOptions = compressOptions;
    writeCompressed = writeCompressed;
	firstFrame = 0;
	countFrames = 0;
}

/// <summary>Initialize a new VideoStream</summary>
/// <remarks>Used only by constructors</remarks>
/// <param name="aviFile">The file that contains the stream</param>
/// <param name="writeCompressed">true: create a compressed stream before adding frames</param>
/// <param name="frameRate">Frames per second</param>
/// <param name="firstFrame">Image to write into the stream as the first frame</param>
void CVideoStream::Initialize(int aviFile, bool writeCompressed, double frameRate, CBitmap firstFrameBitmap) {
    aviFile = aviFile;
    writeCompressed = writeCompressed;
    frameRate = frameRate;
	firstFrame = 0;
	countFrames = 0;

	CBitmapData bmpData = firstFrameBitmap.LockBits(new Rectangle(
		0, 0, firstFrameBitmap.Width, firstFrameBitmap.Height),
		ImageLockMode.ReadOnly, firstFrameBitmap.PixelFormat);

    frameSize = bmpData.Stride * bmpData.Height;
	width = firstFrameBitmap.Width;
	height = firstFrameBitmap.Height;
	countBitsPerPixel = ConvertPixelFormatToBitCount(firstFrameBitmap.PixelFormat);

	firstFrameBitmap.UnlockBits(bmpData);
}

/// <summary>Get the count of bits per pixel from a PixelFormat value</summary>
/// <param name="format">One of the PixelFormat members beginning with "Format..." - all others are not supported</param>
/// <returns>bit count</returns>
short CVideoStream::ConvertPixelFormatToBitCount(CPixelFormat format){
	CString formatName = format.ToString();
	if(formatName.Substring(0, 6) != "Format"){
		throw new Exception("Unknown pixel format: "+formatName);
	}

	formatName = formatName.Substring(6, 2);
	Int16 bitCount = 0;
	if( Char.IsNumber(formatName[1]) ){	//16, 32, 48
		bitCount = Int16.Parse(formatName);
	}else{								//4, 8
		bitCount = Int16.Parse(formatName[0].ToString());
	}

	return bitCount;
}

/// <summary>Returns a PixelFormat value for a specific bit count</summary>
/// <param name="bitCount">count of bits per pixel</param>
/// <returns>A PixelFormat value for [bitCount]</returns>
PixelFormat CVideoStream::ConvertBitCountToPixelFormat(int bitCount){
	CString formatName;
	if(bitCount > 16){
		formatName = String.Format("Format{0}bppRgb", bitCount);
	}else if(bitCount == 16){
		formatName = "Format16bppRgb555";
	}else{ // < 16
		formatName = String.Format("Format{0}bppIndexed", bitCount);
	}
	
	return (PixelFormat)Enum.Parse(typeof(PixelFormat), formatName);
}
*/
AVISTREAMINFO CVideoStream::GetStreamInfo(PAVISTREAM pAviStream){
	AVISTREAMINFO streamInfo;
	HRESULT result = AVIStreamInfo(pAviStream, &streamInfo, sizeof(AVISTREAMINFO));
	if(result) {
		AfxMessageBox(_T("Exception in VideoStreamInfo"));
	}
	return streamInfo;
}
/*
void CVideoStream::GetRateAndScale(ref double frameRate, ref int scale) {
    scale = 1;
    while (frameRate != (long)frameRate) {
        frameRate = frameRate * 10;
        scale *= 10;
    }
}

/// <summary>Create a new stream</summary>
void CVideoStream::CreateStreamWithoutFormat() {
    int scale = 1;
    double rate = frameRate;
    GetRateAndScale(ref rate, ref scale);

    AVISTREAMINFO strhdr = new AVISTREAMINFO();
    strhdr.fccType = mmioStringToFOURCC("vids", 0);
    strhdr.fccHandler = mmioStringToFOURCC("CVID", 0);
    strhdr.dwFlags = 0;
    strhdr.dwCaps = 0;
    strhdr.wPriority = 0;
    strhdr.wLanguage = 0;
    strhdr.dwScale = (int)scale;
    strhdr.dwRate = (int)rate; // Frames per Second
    strhdr.dwStart = 0;
    strhdr.dwLength = 0;
    strhdr.dwInitialFrames = 0;
    strhdr.dwSuggestedBufferSize = frameSize; //height_ * stride_;
    strhdr.dwQuality = -1;        //default
    strhdr.dwSampleSize = 0;
    strhdr.rcFrame.top = 0;
    strhdr.rcFrame.left = 0;
    strhdr.rcFrame.bottom = (uint)height;
    strhdr.rcFrame.right = (uint)width;
    strhdr.dwEditCount = 0;
    strhdr.dwFormatChangeCount = 0;
    strhdr.szName = new UInt16[64];

    HRESULT result = AVIFileCreateStream(aviFile, out aviStream, ref strhdr);

    if(result) {
		AfxMessageBox("Exception in VideoStreamInfo");
        throw new Exception("Exception in AVIFileCreateStream: " + result.ToString());
    }
}

        /// <summary>Create a new stream</summary>
void CVideoStream::CreateStream() {
    CreateStreamWithoutFormat();

    if (writeCompressed) {
        CreateCompressedStream();
    } else {
        SetFormat(aviStream);
    }
}

/// <summary>Create a new stream</summary>
void CVideoStream::CreateStream(AVICOMPRESSOPTIONS options){
    CreateStreamWithoutFormat();
    CreateCompressedStream(options);
}

/// <summary>Create a compressed stream from an uncompressed stream</summary>
void CVideoStream::CreateCompressedStream(){
	//display the compression options dialog...
	AVICOMPRESSOPTIONS_CLASS options = new AVICOMPRESSOPTIONS_CLASS();
	options.fccType = (uint)streamtypeVIDEO;

    options.lpParms = IntPtr.Zero;
	options.lpFormat = IntPtr.Zero;
	AVISaveOptions(IntPtr.Zero, ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_DATARATE, 1, ref aviStream, ref options);
	AVISaveOptionsFree(1, ref options);

    //..or set static options
	//ICOMPRESSOPTIONS opts = new AVICOMPRESSOPTIONS();
//	opts.fccType         = (UInt32)mmioStringToFOURCC("vids", 0);
//	opts.fccHandler      = (UInt32)mmioStringToFOURCC("CVID", 0);
//	opts.dwKeyFrameEvery = 0;
//	opts.dwQuality       = 0;  // 0 .. 10000
//	opts.dwFlags         = 0;  // AVICOMRPESSF_KEYFRAMES = 4
//	opts.dwBytesPerSecond= 0;
//	opts.lpFormat        = new IntPtr(0);
//	opts.cbFormat        = 0;
//	opts.lpParms         = new IntPtr(0);
//	opts.cbParms         = 0;
//	opts.dwInterleaveEvery = 0;

	//get the compressed stream
    compressOptions = options.ToStruct();
    HRESULT result = AVIMakeCompressedStream(out compressedStream, aviStream, ref compressOptions, 0);
    if(result) {
		AfxMessageBox("Exception in AVIMakeCompressedStream");
	}

	SetFormat(compressedStream);
}

/// <summary>Create a compressed stream from an uncompressed stream</summary>
void CVideoStream::CreateCompressedStream(AVICOMPRESSOPTIONS options) {
    HRESULT result = AVIMakeCompressedStream(out compressedStream, aviStream, ref options, 0);
    if(result) {
		AfxMessageBox("Exception in AVIMakeCompressedStream");
    }

    compressOptions = options;

    SetFormat(compressedStream);
}

/// <summary>Add one frame to a new stream</summary>
/// <param name="bmp"></param>
/// <remarks>
/// This works only with uncompressed streams,
/// and compressed streams that have not been saved yet.
/// Use DecompressToNewFile to edit saved compressed streams.
/// </remarks>
void CVideoStream::AddFrame(Bitmap bmp){
	bmp.RotateFlip(RotateFlipType.RotateNoneFlipY);

	BitmapData bmpDat = bmp.LockBits(
		new Rectangle(
		0,0, bmp.Width, bmp.Height),
		ImageLockMode.ReadOnly, bmp.PixelFormat);

    HRESULT result = AVIStreamWrite(writeCompressed ? compressedStream : StreamPointer,
        countFrames, 1, 
		bmpDat.Scan0, 
		(Int32)(bmpDat.Stride * bmpDat.Height), 
		0, 0, 0); 

	if (result) {
		AfxMessageBox("Exception in VideoStreamWrite");
	}

	bmp.UnlockBits(bmpDat);

	countFrames++;
}

/// <summary>Apply a format to a new stream</summary>
/// <param name="aviStream">The IAVISTREAM</param>
/// <remarks>
/// The format must be set before the first frame can be written,
/// and it cannot be changed later.
/// </remarks>
void CVideoStream::SetFormat(IntPtr aviStream){
	BITMAPINFOHEADER bi = new BITMAPINFOHEADER();
	bi.biSize      = Marshal.SizeOf(bi);
	bi.biWidth     = width;
	bi.biHeight    = height;
	bi.biPlanes    = 1;
	bi.biBitCount  = countBitsPerPixel;
	bi.biSizeImage = frameSize;

	HRESULT result = AVIStreamSetFormat(aviStream, 0, ref bi, bi.biSize);
	if(result){ 
		AfxMessageBox("Exception in VideoStreamSetFormat");
	}
}

/// <summary>Prepare for decompressing frames</summary>
/// <remarks>
/// This method has to be called before GetBitmap and ExportBitmap.
/// Release ressources with GetFrameClose.
/// </remarks>
void CVideoStream::GetFrameOpen(){
    AVISTREAMINFO streamInfo = GetStreamInfo(StreamPointer);

    //Open frames

	BITMAPINFOHEADER bih = new BITMAPINFOHEADER();
	bih.biBitCount = countBitsPerPixel;
	bih.biClrImportant = 0;
	bih.biClrUsed = 0;
	bih.biCompression = 0;
	bih.biPlanes = 1;
	bih.biSize = Marshal.SizeOf(bih);
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;

	// Corrections by M. Covington:
	// If these are pre-set, interlaced video is not handled correctly.
	// Better to give zeroes and let Windows fill them in.
	bih.biHeight = 0; // was (Int32)streamInfo.rcFrame.bottom;
	bih.biWidth = 0; // was (Int32)streamInfo.rcFrame.right;

	// Corrections by M. Covington:
	// Validate the bit count, because some AVI files give a bit count
	// that is not one of the allowed values in a BitmapInfoHeader.
	// Here 0 means for Windows to figure it out from other information.
	if (bih.biBitCount > 24)
	{
		bih.biBitCount = 32;
	}
	else if (bih.biBitCount > 16)
	{
		bih.biBitCount = 24;
	}
	else if (bih.biBitCount > 8)
	{
		bih.biBitCount = 16;
	}
	else if (bih.biBitCount > 4)
	{
		bih.biBitCount = 8;
	}
	else if (bih.biBitCount > 0)
	{
		bih.biBitCount = 4;
	}

    getFrameObject = AVIStreamGetFrameOpen(StreamPointer, ref bih);

    if(getFrameObject == 0){ 
		AfxMessageBox("Exception in VideoStreamGetFrameOpen");
	}
}

/// <summary>Export a frame into a bitmap file</summary>
/// <param name="position">Position of the frame</param>
/// <param name="dstFileName">Name of the file to store the bitmap</param>
void CVideoStream::ExportBitmap(int position, String dstFileName){
	Bitmap bmp = GetBitmap(position);
	bmp.Save(dstFileName, ImageFormat.Bmp);
	bmp.Dispose();
}

/// <summary>Export a frame into a bitmap</summary>
/// <param name="position">Position of the frame</param>
Bitmap CVideoStream::GetBitmap(int position){
	if(position > countFrames){
		throw new Exception("Invalid frame position: "+position);
	}

    AVISTREAMINFO streamInfo = GetStreamInfo(StreamPointer);

    //Decompress the frame and return a pointer to the DIB
    int dib = AVIStreamGetFrame(getFrameObject, firstFrame + position);
	//Copy the bitmap header into a managed struct
	BITMAPINFOHEADER bih = new BITMAPINFOHEADER();
	bih = (BITMAPINFOHEADER)Marshal.PtrToStructure(new IntPtr(dib), bih.GetType());

	if(bih.biSizeImage < 1){
		AfxMessageBox("Exception in VideoStreamGetFrame");
	}

	//copy the image
	
	byte[] bitmapData;
	int address = dib + Marshal.SizeOf(bih);
	if(bih.biBitCount < 16){
		//copy palette and pixels
		bitmapData = new byte[bih.biSizeImage + PALETTE_SIZE];
	}else{
		//copy only pixels
		bitmapData = new byte[bih.biSizeImage];
	}

    Marshal.Copy(new IntPtr(address), bitmapData, 0, bitmapData.Length);

	//copy bitmap info
	byte[] bitmapInfo = new byte[Marshal.SizeOf(bih)];
	IntPtr ptr;
	ptr = Marshal.AllocHGlobal(bitmapInfo.Length);
	Marshal.StructureToPtr(bih, ptr, false);
	address = ptr.ToInt32();
    Marshal.Copy(new IntPtr(address), bitmapInfo, 0, bitmapInfo.Length);

    Marshal.FreeHGlobal(ptr);

	//create file header
	BITMAPFILEHEADER bfh = new BITMAPFILEHEADER();
	bfh.bfType = BMP_MAGIC_COOKIE;
	bfh.bfSize = (Int32)(55 + bih.biSizeImage); //size of file as written to disk
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = Marshal.SizeOf(bih) + Marshal.SizeOf(bfh);
	if(bih.biBitCount < 16){
		//There is a palette between header and pixel data
		bfh.bfOffBits += PALETTE_SIZE;
	}

	//write a bitmap stream
	BinaryWriter bw = new BinaryWriter( new MemoryStream() );

	//write header
	bw.Write(bfh.bfType);
	bw.Write(bfh.bfSize);
	bw.Write(bfh.bfReserved1);
	bw.Write(bfh.bfReserved2);
	bw.Write(bfh.bfOffBits);
	//write bitmap info
	bw.Write(bitmapInfo);
	//write bitmap data
	bw.Write(bitmapData);
	
	Bitmap bmp = (Bitmap)Image.FromStream(bw.BaseStream);
	Bitmap saveableBitmap = new Bitmap(bmp.Width, bmp.Height);
	Graphics g = Graphics.FromImage(saveableBitmap);
	g.DrawImage(bmp, 0,0);
	g.Dispose();
	bmp.Dispose();

	bw.Close();
	return saveableBitmap;
}

/// <summary>Free ressources that have been used by GetFrameOpen</summary>
void CVideoStream::GetFrameClose(){
	if(getFrameObject != 0){
		AVIStreamGetFrameClose(getFrameObject);
		getFrameObject = 0;
	}
}

/// <summary>Copy all frames into a new file</summary>
/// <param name="fileName">Name of the new file</param>
/// <param name="recompress">true: Compress the new stream</param>
/// <returns>AviManager for the new file</returns>
/// <remarks>Use this method if you want to append frames to an existing, compressed stream</remarks>
CAviManager CVideoStream::DecompressToNewFile(String fileName, bool recompress, out VideoStream newStream2){
	AviManager newFile = new AviManager(fileName, false);
	
	GetFrameOpen();
	
	Bitmap frame = GetBitmap(0);
	VideoStream newStream = newFile.AddVideoStream(recompress, frameRate, frame);
	frame.Dispose();

	for(int n=1; n<countFrames; n++){
		frame = GetBitmap(n);
		newStream.AddFrame(frame);
		frame.Dispose();
	}

	GetFrameClose();
	
	newStream2 = newStream;
	return newFile;
}
/// <summary>Copy the stream into a new file</summary>
/// <param name="fileName">Name of the new file</param>
void CVideoStream::ExportStream(CString fileName){
	AVICOMPRESSOPTIONS opts;
	opts.fccType = (unsigned int)streamtypeVIDEO;
	opts.lpParms = NULL;
	opts.lpFormat = NULL;
    LPVOID streamPointer = StreamPointer;
    AVISaveOptions(NULL, ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_DATARATE, 1, &streamPointer, &opts);
    AVISaveOptionsFree(1, &opts);

	AVISaveV(fileName, 0, 0, 1, pAviStream, &opts);
}

*/

