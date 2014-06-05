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
	char *lpszOutputString;	// ���ʏo�͐�
	static const char *szB64; // Base64�ϊ��e�[�u��

	static int FindIndexInB64(char c);//szB64�� Base64�ϊ��e�[�u���̂Ȃ��́A�ǂꂩ�A�T���o���B�Ȃ��ꍇ�A-1
public:
	base64();
	~base64();
	const char *Encode(const char *szStr, int iLens = -1);	// �G���R�[�h(������p)�B�������A�f�[�^�̒������w�肷��΁A�o�C�i���f�[�^���\
	const char *Decode(const char *szStr);	// �f�R�[�h(������p)
	int GetLenght() const { return iLen; }	// �o�͂��ꂽ����
	const char *Get() const { return lpszOutputString; }	// ������p�̌��ʂ��擾

	static void Encode(istream& istr, ostream& ostr, int iRet = 76);	// istr�̓o�C�i���X�g���[������
		// �G���R�[�h(�X�g���[���p) iRet�́A�������ڂŉ��s���邩�B-1�Ȃ�A���s���Ȃ��B
	static void Decode(istream& istr, ostream& ostr);	// ostr�̓o�C�i���X�g���[������
		// �f�R�[�h(�X�g���[���p)

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