// @(h) TPerror.cpp        ver 1.0 ( '99.2.28  )

// @(s)
// �G���[�n���h�����O�N���X �����o�[�֐���`
// �G���[���b�Z�[�W�⃍�O����������B
//

#include "stdafx.h"
#include <time.h>
//#include <fstream.h>
#include "Tapur.h"
#include "TPerror.h"
#include "FileSpec.h"


//
//  �@�\     : �R���X�g���N�^
//  
//  �@�\���� : ��������
//  
//  �Ԃ�l   : �Ȃ�
//  
//  ���l     : 
//  
CTPerror::CTPerror()
{
	/// ���O�����f�B���N�g��������
	_TCHAR tstrFolder[MAX_PATH];	
	SHGetSpecialFolderPath(NULL, tstrFolder, CSIDL_APPDATA, FALSE);	// Application Data�t�H���_
	CString strFolder = tstrFolder;
	strFolder += _T("\\Tapur\\");
	_stprintf_s(LogDir, _MAX_PATH, _T("%s%s"), strFolder, LOGDIR_NAME);
}

//
//  �@�\     : �Â����O�̍폜
//  
//  �@�\���� : LOG_EXPIRE ���ȏ�O�̃��O�͍폜
//  
//  �Ԃ�l   : true  ����I��
//             false �G���[����
//  
//  ���l     : 
//  
bool CTPerror::DelOldLog()
{
	CString str;
	CFileFind finder;
	CTime ntm;
	CTime tm;
	CTimeSpan tms;
	CFileSpec fs;

	/// LOG_EXPIRE���ԈȑO�̃��O�t�@�C�����폜
	if (!SetCurrentDirectory(LogDir)) return false;
	ntm = CTime::GetCurrentTime();
	BOOL bWorking = finder.FindFile(_T("*.log"));
	if (!bWorking) return false;
	str.Empty();
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory()) {
			str = finder.GetFileName();
			/// ���O�t�@�C���쐬���t�ƌ��݂Ƃ̍���
			finder.GetCreationTime(tm);
			tms = ntm - tm;
			if (tms.GetDays() > LOG_EXPIRE) {
				fs.SetFullSpec(str);
				if (!fs.Exist()) continue;
				fs.FileDelete();
			}
		}
	}
	finder.Close();

	return true;
}

//
//  �@�\     : ���O�̏�������
//  
//  �@�\���� : ���O�͎��s�t�H���_�ɁuLog�v�t�H���_�����鎞�̂ݐ��������B
//             �쐬���̓��t�ɑΉ��������́i"990228.log"�Ȃǁj�ō쐬�����B
//  
//  �Ԃ�l   : true  ����I��
//             false �G���[����
//  
//  ���l     : �����e�i���X�p�@�\
//  
BOOL CTPerror::WriteLog(
	const _TCHAR* str)		// ���O���b�Z�[�W
{
	TRY {
		/// ���O�����t���O�`�F�b�N
		if (theApp.m_intEnableLog < 0) {
			CRegDWORD rgEnableLog;
			DWORD dw;
			rgEnableLog = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableLog"), 1, TRUE);
			dw = rgEnableLog;
			theApp.m_intEnableLog = (int)dw;
		}
		if (theApp.m_intEnableLog == 0) return TRUE;

		/// ���O�t�@�C�����̌���
		time_t tnum;
		struct tm ts;
		time(&tnum);
		if(localtime_s(&ts, &tnum)) return FALSE;
		_TCHAR filename[_MAX_PATH];
		_stprintf_s(filename, _MAX_PATH, _T("%s\\%02d%02d%02d.log"), 
			LogDir, ts.tm_year, (ts.tm_mon + 1), ts.tm_mday);

		//// ���O�f�B���N�g������
		CFileSpec fs;
		fs.SetFullSpec(filename);
		if (!fs.MkFolder()) return FALSE;

		/// ���b�Z�[�W��������
		CString strMsg;
		strMsg.Format(_T("%02d:%02d:%02d:%s"), ts.tm_hour, ts.tm_min, ts.tm_sec, str);
		return fs.SaveString(strMsg, TRUE);
	}
    CATCH_ALL(e)
    {
		return FALSE;
    }
	END_CATCH_ALL

	return TRUE;
}

//
//  �@�\     : �G���[���b�Z�[�W����
//  
//  �@�\���� : ���b�Z�[�W�̕\���⃍�O�ۑ����s���B
//             �G���[�^�C�v�ɂ�菈����؂�ւ���B
//             �����u�\�[�X�t�@�C����:�s�ԍ�:�G���[�R�[�h:���b�Z�[�W�v
//  
//  �Ԃ�l   : �Ȃ�
//  
//  ���l     : �p���N���X�ɂ���ĕp�ɂɎg�p������v�֐�
//  
void CTPerror::ErrorMessage(	// �G���[���b�Z�[�W����
	const byte type,	// �G���[�^�C�v
	const _TCHAR* file,		// �\�[�X�t�@�C����
	const int line,		// �G���[�����s
	const long code,	// �G���[�R�[�h
	const _TCHAR* message)		// ���b�Z�[�W
{
	_TCHAR *p;
	_TCHAR instr[MAX_ERRSTR];
	::ZeroMemory(instr, MAX_ERRSTR);
	_tcsncpy_s(instr, MAX_ERRSTR, message, MAX_ERRSTR-1);	// NULL�I�[�̂Ȃ��f�[�^�������ꍇ�̑Ώ�
	int len = _tcsclen(instr) + 128;
	_TCHAR *str = new _TCHAR[len];
	_TCHAR *logstr = new _TCHAR[len];

	// �\�[�X�t�@�C��������p�X���폜
	p = _tcsrchr((_TCHAR *)file, '\\');
	p = (p == NULL) ? (_TCHAR *)file : (p+1); 

	_stprintf_s(str, len, _T("%s:%d:0x%x:%s"), p, line, code, message);
	if(type == ERRTYPE_ERROR) {
		/// �G���[�i���O�j
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONSTOP);
		_stprintf_s(logstr, len, _T("ERROR:%s\n"), str);
		WriteLog(logstr);
	} else if(type == ERRTYPE_WARNING) {
		/// �x���i���O�j
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		_stprintf_s(logstr, len, _T("WARNING:%s\n"), str);
		WriteLog(logstr);
	} else if(type == ERRTYPE_INFO) {
		/// ���i���O�Ȃ��j
		AfxMessageBox(str, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
	} else if(type == ERRTYPE_LOGONLY) {
		/// ���O�̂�
		_stprintf_s(logstr, len, _T("LOG:%s\n"), str);
		WriteLog(logstr);
	}

	delete [] str;
	delete [] logstr;
}
