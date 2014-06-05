#if !defined(AFX_RSFIMGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
#define AFX_RSFIMGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsfImgDialog.h : ヘッダー ファイル
//
//#include <imm.h> 
#include "Picture.h"

/////////////////////////////////////////////////////////////////////////////
// CRsfImgDialog ダイアログ

class CRsfImgDialog : public CDialog
{
// コンストラクション
public:
	CRsfImgDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CRsfImgDialog();      

// ダイアログ データ
	//{{AFX_DATA(CRsfImgDialog)
	enum { IDD = IDD_RSF_IMG };
	CButton	m_Ok;
	CButton	m_Cancel;
	CStatic	m_Pict;
	CButton	m_File;
	//}}AFX_DATA

private:
	CString m_strPlayFile;	// 応答画像ファイル名
	CString m_strTmpFile;	// バックアップ画像ファイル名
	CString m_strOutFile;	// 出力画像ファイル名（拡張子なし）
	CPicture m_Cpict;	// 画像ロードクラス
	long m_width;		// 画像サイズ
	long m_height;		// 画像サイズ
	double m_wrate;		// 画像縮小率
	double m_hrate;		// 画像縮小率

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRsfImgDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRsfImgDialog)
	afx_msg void OnBtnFile();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetFile(CString strFile) { m_strPlayFile = strFile; }
	void SetFileNoExt(CString strFile) { m_strOutFile = strFile; }
	BOOL UpdateForm();	// 画面の更新
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RSFIMGDIALOG_H__B137E72A_B5F1_4D60_80B8_526F5F81F9F6__INCLUDED_)
