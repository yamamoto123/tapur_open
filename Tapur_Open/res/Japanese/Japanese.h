// Japanese.h : JAPANESE �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_JAPANESE_H__999A7E0F_7D1D_4644_B7D6_167AAA697032__INCLUDED_)
#define AFX_JAPANESE_H__999A7E0F_7D1D_4644_B7D6_167AAA697032__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CJapaneseApp
// ���̃N���X�̓���̒�`�Ɋւ��Ă� Japanese.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CJapaneseApp : public CWinApp
{
public:
	CJapaneseApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CJapaneseApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CJapaneseApp)
		// ���� -  ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//         ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_JAPANESE_H__999A7E0F_7D1D_4644_B7D6_167AAA697032__INCLUDED_)
