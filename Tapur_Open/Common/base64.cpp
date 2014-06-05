
#include "Forcelib.h"
#include "base64.h"

const char *base64::szB64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

base64::base64() : lpszOutputString(NULL)
{
}

base64::~base64()
{
	if(lpszOutputString)
		delete[] lpszOutputString;
}

const char *base64::Encode(const char *szStr, int iLens)
{
	int i,j;
	
	if(lpszOutputString)
		delete[] lpszOutputString;

	if(iLens == -1)
		iLen = strlen(szStr);
	else
		iLen = iLens;

	lpszOutputString =
		new char[(int)((double)(iLen)*1.5)+10];
	for(i = 0,j = 0; i < (iLen - (iLen % 3)); i+=3)
	{
		lpszOutputString[j] = szB64[(szStr[i] & 0xfc) >> 2];
		lpszOutputString[j+1] = szB64[((szStr[i] &0x03) << 4) |
									((szStr[i+1] & 0xf0) >> 4)];
		lpszOutputString[j+2] = szB64[((szStr[i+1] & 0x0f) <<2 ) |
									((szStr[i+2] & 0xc0) >> 6)];
		lpszOutputString[j+3] = szB64[(szStr[i+2] & 0x3f)];
		j += 4;
	}
	i = iLen-(iLen % 3);	// 残りのサイズを計算
	switch(iLen % 3)
	{
	case 2:	// 1文字分パディングが必要
	{
		lpszOutputString[j] = szB64[(szStr[i] & 0xfc) >> 2];
		lpszOutputString[j+1] = szB64[((szStr[i] &0x03) << 4) |
									((szStr[i+1] & 0xf0) >> 4)];
		lpszOutputString[j+2] = szB64[((szStr[i+1] & 0x0f) <<2 )];
		lpszOutputString[j+3] = szB64[64];	// Pad
		lpszOutputString[j+4] = '\0';
	}
	break;
	case 1:	// 2文字分パディングが必要
	{
		lpszOutputString[j] = szB64[(szStr[i] & 0xfc) >> 2];
		lpszOutputString[j+1] = szB64[((szStr[i] &0x03) << 4)];
		lpszOutputString[j+2] = szB64[64];	// Pad
		lpszOutputString[j+3] = szB64[64];	// Pad
		lpszOutputString[j+4] = '\0';
	}
	break;
	}
	lpszOutputString[j+4] = '\0';

	return lpszOutputString;
}

void base64::Encode(istream& istr, ostream& ostr, int iRet)
{
	int i;
	char c[3];

	i = 0;
	while(!istr.eof())
	{
		c[0] = c[1] = c[2] = '\0';
		istr.read(c,3);

		ostr << szB64[(c[0] & 0xfc) >> 2];
		i++;	if(i >= iRet && iRet != -1){ ostr << endl; i = 0; }
		ostr << szB64[((c[0] &0x03) << 4) | ((c[1] & 0xf0) >> 4)];
		i++;	if(i >= iRet && iRet != -1){ ostr << endl; i = 0; }
		if(istr.gcount() == 1)
			ostr << szB64[64];
		else
			ostr << szB64[((c[1] & 0x0f) <<2 ) | ((c[2] & 0xc0) >> 6)];
		i++;	if(i >= iRet && iRet != -1){ ostr << endl; i = 0; }
		if(istr.gcount() == 3)
			ostr << szB64[(c[2] & 0x3f)];
		else
			ostr << szB64[64];
		i++;	if(i >= iRet && iRet != -1){ ostr << endl; i = 0; }
	}
	ostr.flush();
}


int base64::FindIndexInB64(char c)
{
	int index = 0;
	while(szB64[index] != '\0' && szB64[index] != c)
		index++;

	if(szB64[index] == '\0')
		return -1;	// 見つからず

	return index;	// 見つかった。
}

const char *base64::Decode(const char *szStr)
{
	//lpszOutputString
	int i, j, l,iWriteCount,len;
	char c;
	char buf[4];

	len = iLen = strlen(szStr);
	if(lpszOutputString)
		delete[] lpszOutputString;

	iLen = (int)(((double)(iLen)/4.0)*3.0) + 4;
	
	lpszOutputString = new char[iLen];

	for(i = 0, j = 0; i < len; i+=4)
	{
		iWriteCount = 3;
		for(l = 0; l < 4 && i+l<len; l++)
		{
			c = szStr[i+l];
			if(c == szB64[64])	// Pad
				iWriteCount--;
			else {
				buf[l] = FindIndexInB64(c);
				//if(buf[l] == -1) error!;
			}
		}
		lpszOutputString[j] = ((buf[0] << 2) & 0xfc) | ((buf[1] >> 4) & 0x03);
		if(iWriteCount >= 2)
			lpszOutputString[j+1] = ((buf[1] << 4) & 0xf0) | ((buf[2] >> 2) & 0x0f);
		if(iWriteCount == 3)		
			lpszOutputString[j+2] = ((buf[2] << 6) & 0xc0) | (buf[3] & 0x3f);
		j+=iWriteCount;
	}
	iLen = j;
	lpszOutputString[j] = '\0';

	return lpszOutputString;
}

void base64::Decode(istream& istr, ostream& ostr)
{
	int i,iWriteCount;
	char c;
	char buf[4];
	char out[3];

	while(1)
	{
		iWriteCount = 3;
		for(i = 0; i < 4; i++)
		{
			istr >> c;
			if(istr.eof())
			{
				ostr.flush();
				return;
			}
			if(c == szB64[64])	// Pad
				iWriteCount--;
			else {
				buf[i] = FindIndexInB64(c);
				//if(buf[i] == -1) error!;
			}
		}
		out[0] = ((buf[0] << 2) & 0xfc) | ((buf[1] >> 4) & 0x03);
		out[1] = ((buf[1] << 4) & 0xf0) | ((buf[2] >> 2) & 0x0f);
		out[2] = ((buf[2] << 6) & 0xc0) | (buf[3] & 0x3f);
		ostr.write(out, iWriteCount);
	}
}
