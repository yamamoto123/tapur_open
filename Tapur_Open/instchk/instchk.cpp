// instchk.cpp : �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include <lmaccess.h>
#include <lmapibuf.h>
#include <LMErr.h>
#include "KillProcess.h"
//#include "FileSpec.h"
//#include "Ini.h"

#define MGR_NAME	"tapmgr.exe"
#define APP_NAME	"tapur.exe"
#define SKYPE_NAME	"skype.exe"

//
//  �@�\     : Begin Setup�`�F�b�N�pINI�t�@�C���쐬
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : 
//  
//  ���l     : XP-SP1�ł�[Message]�L�[������ɏ�������Ȃ����߁A�v���O�������Ń��b�Z�[�W�\��
//  
//static void SetIni(
//bool blContinue,	// �C���X�g�[�����s��
//LPCTSTR lpStrErr)	// �C���X�g�[�����~���̃��b�Z�[�W
//{
//	CFileSpec fs(FS_WINDIR);
//	fs.SetFullSpec(fs.GetFullSpec() + _T("\\BSRESULT.INI"));
////	if (fs.Exist()) fs.FileDelete();
//
//	CIni ini;
//	ini.SetPathName(fs.GetFullSpec());
//	if (blContinue) {
//		ini.WriteString(_T("CheckResult"), _T("Result"), _T("ContinueInstall"));
////		ini.DeleteKey(_T("CheckResult"), _T("Message"));
//	} else {
//		ini.WriteString(_T("CheckResult"), _T("Result"), _T("NoInstall"));
////		ini.WriteString(_T("CheckResult"), _T("Message"), lpStrErr);
//AfxMessageBox(lpStrErr, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
//	}
//}


//
//  �@�\     : Begin Setup�C���X�g�[���� �`�F�b�N����
//  
//  �@�\���� : �`�F�b�N�̓C���X�g�[���O��̂Q��s����i�H�j���߁A�u�C���X�g�[���ρv�`�F�b�N�ɂ͎g�p�s��
//  
//  �Ԃ�l   : 
//  
//  ���l     : 
//  
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
 	// TODO: ���̈ʒu�ɃR�[�h���L�q���Ă��������B

	// �N������Tapur.exe��KILL
	CKillProcess kp;
	kp.KillProcess(MGR_NAME);	// ��������ɍ폜
	kp.KillProcess(APP_NAME);
	::Sleep(1000);		// ��~�҂�

/* ���b�Z�[�W�͏o�邪�A�����͒��f����Ȃ�
	if(kp.KillProcess(APP_NAME)) {
		SetIni(false, _T("Please uninstall the old Tapur."));
		return 1;
	}

	// �C���X�g�[���ς�Tapur�̃`�F�b�N
	CFileSpec fs(FS_PROGRAM_FILES);
	CString strPath = fs.GetFullSpec() + _T("\\Tapur\\Tapur.exe");
	fs.SetFullSpec(strPath);
	if (fs.Exist()) {
		SetIni(false, _T("Please uninstall the old Tapur."));
		return 1;
	}
*/

	// ADMIN���[�h�̃`�F�b�N
	//_TCHAR usrName[MAX_PATH];
	//DWORD dwSize = MAX_PATH;
	//if (GetUserName(usrName, &dwSize)) {
	//	DWORD dwLevel = 1;
	//	LPUSER_INFO_1 pBuf = NULL;
	//	NET_API_STATUS nStatus;
	//	/// ���[�U����UNICODE���ϊ�
	//	wchar_t wc[MAX_PATH*2];
	//	DWORD dwLen = ::MultiByteToWideChar(CP_UTF8,0,usrName,-1,NULL,0);
	//	::MultiByteToWideChar(CP_UTF8, 0, usrName, -1, wc, dwLen);
	//	nStatus = NetUserGetInfo(NULL, wc, dwLevel, (LPBYTE *)&pBuf);
	//	if (nStatus == NERR_Success && pBuf != NULL) {
	//		if (pBuf->usri1_priv != USER_PRIV_ADMIN) {
	//			NetApiBufferFree(pBuf);	// ���蓖�čς݂̃����������
	//			SetIni(false, _T("Please install Tapur by the administrator privilege."));
	//			return 1;
	//		}
	//	}
	//	if (pBuf != NULL) NetApiBufferFree(pBuf);	// ���蓖�čς݂̃����������
	//}

	//// �N������Skype.exe��KILL
	//kp.KillProcess(SKYPE_NAME);

	//// OK
	//SetIni(true, _T(""));
	return 0;
}



