//
// tapmgr.cpp : Tapur.exe�̊Ď��v���O����
//	�E����I��Tapur.exe�̃v���Z�X���`�F�b�N���A���݂��Ȃ��ꍇ�͋N��
//
// tapur.exe�Ɠ���t�H���_�Ŏ��s


#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <shlobj.h>		// SHGetSpecialFolderPath()
#include "KillProcess.h"
#include "FileSpec.h"
#include "Registry.h"

#define APP_NAME	"Tapur.exe"
//#define APP_PATH	"C:\\Program Files\\Tapur\\"


// CreateProcess()���g���ĊO���A�v���P�[�V�������N������
static int ExeProcess(LPTSTR cmdstr)
{
	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;

	ZeroMemory( &sinfo, sizeof( STARTUPINFO ) );
	ZeroMemory( &pinfo, sizeof( PROCESS_INFORMATION ) );
	sinfo.cb = sizeof( STARTUPINFO );
	sinfo.dwFlags = STARTF_USESHOWWINDOW;
	sinfo.wShowWindow = SW_SHOWNORMAL;

	if ( !CreateProcess( NULL,
						 cmdstr,
						 NULL,
						 NULL,
						 FALSE,
						 0,
						 NULL,
						 NULL,
						 &sinfo,
						 &pinfo ) ) { 
		// �N�����s
		return -1;
	}

	// �������I���܂Ő����߂��܂���
//	WaitForInputIdle( pinfo.hProcess, INFINITE );
//	WaitForSingleObject( pinfo.hProcess, INFINITE );
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
	_TCHAR tstrExe[_MAX_PATH];	
	CKillProcess kp;
	DWORD dwId;	// return value
	HANDLE hProcess;

	// Tapur,exe�̃p�X
	CFileSpec fs(FS_APPDIR);	// application folder
	_stprintf(tstrExe, _T("%s%s"), fs.GetFullSpec(), APP_NAME);

	/// ��d�N���̖h�~
	// �~���[�e�b�N�X�I�u�W�F�N�g�����B�~���[�e�b�N�X�I�u�W�F�N�g��
	// ���łɑ��݂��Ă���ꍇ�́A�A�v���P�[�V������2�ڂ̃C���X�^���X
	// �ł���B�~���[�e�b�N�X�n���h���́A�v���Z�X���I������Ǝ����I��
	// �����邱�Ƃɒ���
	::CreateMutex(NULL, TRUE, "tapmgr.exe");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		hProcess = kp.FindProcess(APP_NAME, dwId);
		if(hProcess) {
			// Tapur.exe���N���ς݂Ȃ��ʃI�[�v��
			ExeProcess(tstrExe);
		}
		return 0;
	}

	// �Ď��C���^�[�o��
	CRegDWORD rgInterval("Software\\Tapur\\Manager\\Interval", 30, TRUE);	// �����l:30sec
	DWORD dwInterval = rgInterval * 1000;	// mm sec

	while(1) {
		/// Tapur���N�����Ă��Ȃ���΋N��
		hProcess = kp.FindProcess(APP_NAME, dwId);
		if(!hProcess) {
			//::Sleep(1000);		// Skype�̋N���Ƃ��炷����:���ʂȂ��H
			ExeProcess(tstrExe);
		}

		::Sleep(dwInterval);	// �ҋ@����
	}

	return 0;
}

