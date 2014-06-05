// @(h) TPerror.h        ver 1.0 ( '99.2.28  )

// @(s)
// �G���[�n���h�����O�N���X��`
// �G���[���b�Z�[�W�⃍�O����������B
//

#ifndef __TPerror_H__
#define __TPerror_H__

#include <stdlib.h>		// _MAX_PATH

// �G���[�R�[�h
#define ERR_TAPUR			0x10000
#define ERR_SKYPE			0x20000
#define ERR_WAVE			0x30000
#define ERR_AVI				0x40000
#define ERR_SOCK			0x50000
#define ERR_ACM				0x60000


// �G���[�^�C�v
#define ERRTYPE_ERROR		0x01	// �G���[�i���O�j
#define ERRTYPE_WARNING		0x02	// �x���i���O�j
#define ERRTYPE_INFO		0x04	// �x���i���O�Ȃ��j
#define ERRTYPE_LOGONLY		0x08	// ���O�̂�

#define LOGDIR_NAME		_T("log")		// ���O�f�B���N�g������
#define LOG_EXPIRE		10				// ���O�L�����ԁidays�j

#define MAX_ERRSTR		1024

class CTPerror {	// �G���[�n���h�����O�N���X
private:
//	_TCHAR PrgDir[_MAX_PATH];	// ���s�v���Z�X�̃f�B���N�g���i"\"�t���j
	_TCHAR LogDir[_MAX_PATH];	// ���O�f�B���N�g���i"\"�����j
public:
	_TCHAR errstr[MAX_ERRSTR];		// ���[�U�����R�Ɏg�p�ł���o�b�t�@(�N���X���ł͖��g�p)

private:
	BOOL WriteLog(				// ���O��������
		const _TCHAR* str);	// ���O���b�Z�[�W
public:
	CTPerror();		// �R���X�g���N�^

	virtual void ErrorMessage(	// �G���[���b�Z�[�W����
		const byte	type,	// �G���[�^�C�v
		const _TCHAR* file,		// �\�[�X�t�@�C����
		const int line,		// �G���[�����s
		const long code,	// �G���[�R�[�h
		const _TCHAR* message);	// ���b�Z�[�W
	bool DelOldLog();	// �Â����O�̍폜
};

#endif	// __TPerror_H__

