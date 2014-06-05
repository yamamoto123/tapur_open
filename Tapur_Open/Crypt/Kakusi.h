// Kakusi.h: CKakusi クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(KAKUSI_H__572DA33B_B04C_4E8C_B2FC_A74DA054F699__INCLUDED_)
#define KAKUSI_H__572DA33B_B04C_4E8C_B2FC_A74DA054F699__INCLUDED_

#include <string>
#include "BlowFish.h"

class CKakusi  
{
public:
	CKakusi();
	virtual ~CKakusi();

	unsigned char *KakusiData(unsigned char* _Input, long _Size);
	unsigned char *ModosiData(unsigned char* _Input, long _Size);
	bool KakusiFile( char* _Filename, unsigned char* _Datastruct, long _Datasize );
	bool ModosiFile( char* _Filename, unsigned char* _Output );
	long GetCryptSize(long _Size) {
		return kakusibako.GetOutputLength( _Size, BF_PKCS5 );
	}

private:
	CBlowFish kakusibako;	// blowfishの実体部
};

extern bool ModosiFile( char* _Filename, unsigned char* _Output );
extern bool KakusiFile( char* _Filename, unsigned char* _Datastruct, long _Datasize );

#endif // !defined(KAKUSI_H__572DA33B_B04C_4E8C_B2FC_A74DA054F699__INCLUDED_)
