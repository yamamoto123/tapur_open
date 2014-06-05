#if !defined BASE64_H_INC_
#define BASE64_H_INC_

#include <string.h>


#include <iostream>
#include <fstream>
using namespace std;

// Base64 Encodeing
class base64
{
	int iLen;
	char *lpszOutputString;	// 結果出力先
	static const char *szB64; // Base64変換テーブル

	static int FindIndexInB64(char c);//szB64の Base64変換テーブルのなかの、どれか、探し出す。ない場合、-1
public:
	base64();
	~base64();
	const char *Encode(const char *szStr, int iLens = -1);	// エンコード(文字列用)。ただし、データの長さを指定すれば、バイナリデータが可能
	const char *Decode(const char *szStr);	// デコード(文字列用)
	int GetLenght() const { return iLen; }	// 出力された長さ
	const char *Get() const { return lpszOutputString; }	// 文字列用の結果を取得

	static void Encode(istream& istr, ostream& ostr, int iRet = 76);	// istrはバイナリストリーム推奨
		// エンコード(ストリーム用) iRetは、何文字目で改行するか。-1なら、改行しない。
	static void Decode(istream& istr, ostream& ostr);	// ostrはバイナリストリーム推奨
		// デコード(ストリーム用)

	static void Encode(const char *lpszImputFileName, const char *lpszOutputFileName, int iRet = 76)
	{
		ifstream f(lpszImputFileName,ios::in | ios::binary);
		ofstream of(lpszOutputFileName);
		Encode(f,of);
	}
	static void Decode(const char *lpszImputFileName, const char *lpszOutputFileName)
	{
		ifstream f(lpszImputFileName);
		ofstream of(lpszOutputFileName,ios::out | ios::trunc | ios::binary);
		Decode(f,of);
	}
};

#endif // #if !defined BASE64_H_INC_