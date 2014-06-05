// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
#endif

// ���Ŏw�肳�ꂽ��`�̑O�ɑΏۃv���b�g�t�H�[�����w�肵�Ȃ���΂Ȃ�Ȃ��ꍇ�A�ȉ��̒�`��ύX���Ă��������B
// �قȂ�v���b�g�t�H�[���ɑΉ�����l�Ɋւ���ŐV���ɂ��ẮAMSDN ���Q�Ƃ��Ă��������B
#ifndef WINVER				// Windows XP �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define WINVER 0x0500		// Win2000
#endif

#ifndef _WIN32_WINNT		// Windows XP �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B                   
#define _WIN32_WINNT 0x0500	// Win2000
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_WINDOWS 0x0410 // ����� Windows Me �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_IE			// IE 6.0 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_IE 0x0500	// Win2000
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �ꕔ�� CString �R���X�g���N�^�͖����I�ł��B

// ��ʓI�Ŗ������Ă����S�� MFC �̌x�����b�Z�[�W�̈ꕔ�̔�\�����������܂��B
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������


#include <afxdisp.h>        // MFC �I�[�g���[�V���� �N���X



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT

//#define _ATL_APARTMENT_THREADED		// ATL�̎g�p��������

//#include <afxdisp.h>        // MFC �̃I�[�g���[�V���� �N���X
//#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
//#include <io.h>
//#endif // _AFX_NO_AFXCMN_SUPPORT
//#include <afxtempl.h>		// CArray


// paak
#include <atlbase.h>		// ATL�̎g�p��������i���̃w�b�_��afx�n�w�b�_�̌�ɔz�u���邱�Ɓj
#include <atlcom.h>

// memory leak ���o
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// ���\�[�X�w�b�_
#include "resource.h"       

// �����I�[�o�[���C�h�������ꍇ�ACComModule ����N���X��h��������
// ������g�p���邱�Ƃ��\�ł��� _Module �̖��O�͕ύX���Ȃ��ł��������B
class CTapurModule : public CComModule
{
public:
	LONG Unlock();
	LONG Lock();
	LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
	DWORD dwThreadID;
};
extern CTapurModule _Module;

// �^�C�}�[�Ǘ��N���X
typedef struct _ST_TIMER
{
	BOOL blOnTimer;		// �^�C�}�[���s���t���O
	long lngPassTime;	// �^�C�}�[�C�x���g���s��
	long lngLimitTime;	// �^�C�}�[��������(s)
	CTime tmStart;		// �^�C�}�[�J�n����
	CTime tmPass;		// �^�C�}�[�C�x���g���s����
	long lngSpanTime;	// �^�C�}�[�o�ߎ���(s)
} ST_TIMER;

// �^�C�}�[ID�Ǘ�
#define ID_TIMER_APP			10
#define ID_TIMER_CONNECT		11
#define ID_TIMER_WAIT_ANSWER	12
#define ID_TIMER_RECORD_MAX		13
#define ID_TIMER_PLAY_CHK		14
#define ID_TIMER_MAINTOOL		20
#define ID_TIMER_MSGFORM		30
//#define ID_TIMER_RSFFORM		40
#define ID_TIMER_RSF_AUDIO		41
#define ID_TIMER_RSF_VIDEO		42
#define ID_TIMER_VIDEO			50
#define ID_TIMER_SCREENCAP		55
#define ID_TIMER_AVIMGR			60


#ifdef _DEBUG
#import ".\Debug\ActiveST.dll"  no_namespace, named_guids  
#else
#import ".\Release\ActiveST.dll"  no_namespace, named_guids  
#endif
//#import "c:\\windows\\system32\\ActiveS.dll"  no_namespace, named_guids  
//#import "c:\\windows\\system32\\skypeapi.dll"  no_namespace, named_guids  
//#import "c:\\windows\\system32\\skypeapi.dll" named_guids  
//using namespace SKYPEAPILib;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
