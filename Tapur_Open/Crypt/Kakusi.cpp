// Kakusi.cpp: CKakusi �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "Forcelib.h"
#include "Kakusi.h"

//���̕���������Ƃ��āA�f�[�^�̈Í��E�������s���܂�
const unsigned char TEGATA[] = "50eee31fdce90df47881c45602329d8f086e317e";

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CKakusi::CKakusi()
{
	//�Í��̃L�[��ݒ�
	kakusibako.Initialize( TEGATA, sizeof(TEGATA) );
}

CKakusi::~CKakusi()
{
}

//�f�[�^���Í�������
unsigned char *CKakusi::KakusiData( unsigned char* _Input, long _Size)
{
	const int size = kakusibako.GetOutputLength( _Size, BF_PKCS5 );

	unsigned char *_Output = new unsigned char[size];

	kakusibako.Encrypt( _Input, _Output, size );

	return _Output;
}

//�Í������ꂽ���̂𕜌�����
unsigned char *CKakusi::ModosiData( unsigned char* _Input, long _Size)
{
	const int size = kakusibako.GetOutputLength( _Size, BF_PKCS5 );

	unsigned char *_Output = new unsigned char[size];

	kakusibako.Decrypt( _Input, _Output, size );

	return _Output;
}

//��������̃f�[�^���Í����t�@�C���ɏ�������
bool CKakusi::KakusiFile( char* _Filename, unsigned char* _Datastruct, long _Datasize )
{
	FILE* _fp;
	unsigned char *pBuffer = NULL;
	bool ret;

	try {
		if (fopen_s(&_fp, _Filename, "wb")) return false;

		long size = GetCryptSize(_Datasize);
		pBuffer = new unsigned char[size];
		memset(pBuffer, NULL, size);
		kakusibako.Encrypt(_Datastruct, pBuffer, _Datasize);

		fwrite(pBuffer, size, 1, _fp);
		ret = true;
	} catch(...) {
		ret = false;
	}

	if (_fp) fclose( _fp );
	delete [] pBuffer;
	return ret;
}

// �Í������ꂽ�t�@�C����ǂݍ���ŁA_Output���Ɋi�[
bool CKakusi::ModosiFile( char* _Filename, unsigned char* _Output )
{
	FILE* _fp;
	unsigned char *pBuffer = NULL;
	bool ret;

	try {
		if (fopen_s(&_fp, _Filename, "rb")) return false;

		// file size
		long size = 0;
		fseek( _fp, 0, SEEK_END );
		size = ftell( _fp );
		fseek( _fp, 0, SEEK_SET );
		if ( size <= 0 ) {
			fclose( _fp );
			return false;
		}		

		pBuffer = new unsigned char[size];

		fread(pBuffer, size, 1, _fp);

		kakusibako.Decrypt(pBuffer, _Output, size);

		ret = true;
	} catch(...) {
		ret = false;
	}

	if (_fp) fclose( _fp );
	delete [] pBuffer;
	return ret;
}

// C ���ꂩ��̌Ăяo���p
bool ModosiFile( char* _Filename, unsigned char* _Output )
{
	CKakusi ks;
	return ks.ModosiFile(_Filename, _Output);
}
bool KakusiFile( char* _Filename, unsigned char* _Datastruct, long _Datasize )
{
	CKakusi ks;
	return ks.KakusiFile(_Filename, _Datastruct, _Datasize);
}
