// SettingVRecTab.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Tapur.h"
#include "SettingVRecTab.h"
#include "SettingDialog.h"
#include "SetChatDialog.h"
#include "TabCtrlSSL.h"
#include "TPutil.h"

// memory leak ���o
#ifdef	__AFX_H__            // MFC�̂�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingVRecTab �_�C�A���O


CSettingVRecTab::CSettingVRecTab(CWnd* pParent /*=NULL*/)
	: CTabPageSSL (CSettingVRecTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingVRecTab)
	m_idxVRecordLimit = 0;
	m_idxSepSize = 0;
	m_idxVCmpName = 0;
	m_strFps = _T("");
	m_blVRecWarning = TRUE;
	m_blVRecStopWarning = TRUE;
	m_blVSetTop = TRUE;
	m_blNoOverlay = TRUE;
	m_blSep = TRUE;
	m_blAudio = TRUE;
	//}}AFX_DATA_INIT
}


void CSettingVRecTab::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingVRecTab)
	DDX_CBIndex(pDX, IDC_CMB_VRECORD_LIMIT, m_idxVRecordLimit);
	DDX_Check(pDX, IDC_CHK_VREC_WARNING, m_blVRecWarning);
	DDX_Control(pDX, IDC_BTN_VREC_WARNING, m_btnVRecWarning);
	DDX_Check(pDX, IDC_CHK_VRECSTOP_WARNING, m_blVRecStopWarning);
	DDX_Control(pDX, IDC_BTN_VRECSTOP_WARNING, m_btnVRecStopWarning);
	DDX_Check(pDX, IDC_CHK_VSETTOP, m_blVSetTop);
	DDX_Check(pDX, IDC_CHK_OVERLAY, m_blNoOverlay);
	DDX_Check(pDX, IDC_CHK_SEP, m_blSep);
	DDX_CBIndex(pDX, IDC_CMB_SEP, m_idxSepSize);
	DDX_Check(pDX, IDC_CHK_AUDIO, m_blAudio);
	DDX_CBIndex(pDX, IDC_CMB_CMP, m_idxVCmpName);
	DDX_CBString(pDX, IDC_CMB_FPS, m_strFps);
	DDX_Control(pDX, IDC_STATIC_LINK_LICENSE, m_LinkLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingVRecTab, CDialog)
	//{{AFX_MSG_MAP(CSettingVRecTab)
	ON_BN_CLICKED(IDC_CHK_VREC_WARNING, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_VRECSTOP_WARNING, OnChgForm)
	ON_BN_CLICKED(IDC_BTN_VREC_WARNING, OnBtnVRecWarning)
	ON_BN_CLICKED(IDC_BTN_VRECSTOP_WARNING, OnBtnVRecStopWarning)
	ON_CBN_SELCHANGE(IDC_CMB_VRECORD_LIMIT, OnChgForm)
	ON_CBN_SELCHANGE(IDC_CMB_SEP, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_VSETTOP, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_OVERLAY, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_SEP, OnChgForm)
	ON_BN_CLICKED(IDC_CHK_AUDIO, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_NOCMP, OnChgForm)
	ON_BN_CLICKED(IDC_RDO_CMP, OnChgForm)
	ON_CBN_SELCHANGE(IDC_CMB_CMP, OnChgForm)
	ON_CBN_SELCHANGE(IDC_CMB_FPS, OnChgForm)
	ON_CBN_EDITCHANGE(IDC_CMB_FPS, OnChgFps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingVRecTab ���b�Z�[�W �n���h��

BOOL CSettingVRecTab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	int i;
	CString str;
	DWORD dw;

	/// �R���{�{�b�N�X������
	CComboBox *pcmb;
	//// �^�掞��
	m_aryVRecordLimit.Add(_T("1"));
    m_aryVRecordLimit.Add(_T("2"));
    m_aryVRecordLimit.Add(_T("3"));
    m_aryVRecordLimit.Add(_T("5"));
    m_aryVRecordLimit.Add(_T("10"));
    m_aryVRecordLimit.Add(_T("15"));
    m_aryVRecordLimit.Add(_T("20"));
    m_aryVRecordLimit.Add(_T("30"));
    m_aryVRecordLimit.Add(_T("45"));
    m_aryVRecordLimit.Add(_T("60"));
    m_aryVRecordLimit.Add(_T("90"));
    m_aryVRecordLimit.Add(_T("120"));
    m_aryVRecordLimit.Add(_T("180"));
    m_aryVRecordLimit.Add(_T("240"));
    m_aryVRecordLimit.Add(_T("360"));
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_VRECORD_LIMIT);
	for(i = 0; i <  m_aryVRecordLimit.GetSize(); i++) {
		pcmb->AddString(m_aryVRecordLimit.GetAt(i));
	}
	//// �^��T�C�Y
	m_arySepSize.Add(_T("1"));
    m_arySepSize.Add(_T("2"));
    m_arySepSize.Add(_T("4"));
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_SEP);
	for(i = 0; i <  m_arySepSize.GetSize(); i++) {
		pcmb->AddString(m_arySepSize.GetAt(i));
	}
	//// FPS
	m_aryFps.Add(_T("5"));
    m_aryFps.Add(_T("10"));
    m_aryFps.Add(_T("15"));
    m_aryFps.Add(_T("25"));
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_FPS);
	for(i = 0; i <  m_aryFps.GetSize(); i++) {
		pcmb->AddString(m_aryFps.GetAt(i));
	}
	//// ���k�`��
	pcmb = (CComboBox*)GetDlgItem(IDC_CMB_CMP);
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	CVideo *pVideo = pFrame->m_pVideo;
	//AssignListToComboBox(pcmb, pVideo->GetCompressors(), pVideo->CountCompressor());
	CStringList oStrList;
	pVideo->GetCompressors(&oStrList);
	AssignListToComboBox(pcmb, &oStrList);
	int intCnt = pcmb->GetCount();
	m_aryVCmpName.SetSize(intCnt);
	for(i = 0; i < pcmb->GetCount(); i++) {
		pcmb->GetLBText(i, str);
		m_aryVCmpName.SetAt(i, str);
	}

	/// ���W�X�g���ǂݍ���
	//// �����񃌃W�X�g���ł͋󔒓��͂����������ꍇ������̂�force=TRUE��
	m_rgEnableVRecWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVideoRecordWarning"), 1, TRUE);	// �����l:1(True)
	m_rgEnableVRecStopWarning = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\EnableVideoRecordStopWarning"), 1, TRUE);	// �����l:1(True)
 	//// �^�掞�x���`���b�g
	dw = m_rgEnableVRecWarning;
	m_blVRecWarning = (dw == 1) ? TRUE : FALSE;
	dw = m_rgEnableVRecStopWarning;
	m_blVRecStopWarning = (dw == 1) ? TRUE : FALSE;
	//// �r�f�I��ʂ�O�ʕ\��
	m_rgVSetTop = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordSetTop"), 1, TRUE);	// �����l:1(True)
	dw = m_rgVSetTop;
	m_blVSetTop = (dw == 1) ? TRUE : FALSE;
	//// �I�[�o�[���C�^��
	m_rgOverlay = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordOverlay"), 1, TRUE);	// �����l:1(�I�[�o�[���C����)
	dw = m_rgOverlay;
	m_blNoOverlay = (dw == 1) ? FALSE : TRUE;
	//// �t�@�C�������ۑ�
	m_rgSep = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordSep"), 1, TRUE);	// �����l:1(��������)
	dw = m_rgSep;
	m_blSep = (dw == 1) ? TRUE : FALSE;
	//// ��������
	m_rgAudio = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordWithAudio"), 1, TRUE);	// �����l:1(�����t�^��)
	dw = m_rgAudio;
	m_blAudio = (dw == 1) ? TRUE : FALSE;
 	//// �񈳏k or ���k
	m_rgVCmp = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordCompress"), 1, TRUE);		// �����l:1(���k)
	m_rgVCmpName = CRegString(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordCompressName"), _T("Lagarith lossless codec"), TRUE);		// �����l:"Lagarith"
 	//// FPS
	m_rgFps = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordFps"), 5, TRUE);			// �����l:5(fps)

	/// ���W�X�g���l�ɂ��R���{�{�b�N�X��I��
 	//// �^�掞�Ԏ��ԃR���{�{�b�N�X��I��
	m_rgVRecordLimit = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VRecordLimitTime"), 600, TRUE);	// �����l:10min
	dw = m_rgVRecordLimit;
	dw /= 60;	// sec -> min
	for(i = 0; i <  m_aryVRecordLimit.GetSize(); i++) {
		str = m_aryVRecordLimit.GetAt(i);
		if (dw == (DWORD)_ttoi(str)) {
			m_idxVRecordLimit = i;
			break;
		}
	}
 	//// �^��T�C�Y�R���{�{�b�N�X��I��
	m_rgSepSize = CRegDWORD(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordSepSize"), 2000, TRUE);	// �����l:2000Mbyte
	dw = m_rgSepSize;
	dw /= 1000;	// Mbyte -> Gbyte
	for(i = 0; i <  m_arySepSize.GetSize(); i++) {
		str = m_arySepSize.GetAt(i);
		if (dw == (DWORD)_ttoi(str)) {
			m_idxSepSize = i;
			break;
		}
	}
 	//// ���k�`���R���{�{�b�N�X��I��
	str = m_rgVCmpName;
	str.MakeUpper();
	for(i = 0; i <  m_aryVCmpName.GetSize(); i++) {
		CString strName = m_aryVCmpName.GetAt(i);
		strName.MakeUpper();
		if (strName.Find(str) >= 0) {
			m_idxVCmpName = i;
			break;
		}
	}
 	//// FPS�R���{�{�b�N�X��I��
	dw = m_rgFps;
	m_strFps.Format(_T("%d"), dw);

 	//// �񈳏k or ���k��I��
	dw = m_rgVCmp;
	// ���W�I�{�^���̎Q�ƒ�`�iDDX�͈����ɂ����̂Łj
	// �i�N���X�����o�[�Ő錾����Ɛ���ɓ��삵�Ȃ��j
	CButton *rd_nocmp = (CButton *)GetDlgItem(IDC_RDO_NOCMP);
	CButton *rd_cmp = (CButton *)GetDlgItem(IDC_RDO_CMP);
	if (dw > 0) {
		rd_nocmp->SetCheck(0);
		rd_cmp->SetCheck(1);
	} else {
		rd_nocmp->SetCheck(1);
		rd_cmp->SetCheck(0);
	}

	//// ���C�Z���X�����N
	m_LinkLine.SetTextColor(RGB(0,0,255));
	m_LinkLine.SetFontUnderline(TRUE);
	str.LoadString(IDS_URL_LICENSE);
	m_LinkLine.SetHyperLink(str);
	HCURSOR cursor = AfxGetApp()->LoadCursor(IDC_LINK);
	m_LinkLine.SetLinkCursor(cursor);
	m_LinkLine.SetLink(TRUE, FALSE);

	UpdateData(FALSE);	// DDX�X�V
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// �^�掞�x�����b�Z�[�W�̐ݒ�
void CSettingVRecTab::OnBtnVRecWarning() 
{
	UpdateData(TRUE);	// DDX�X�V

	CSetChatDialog dlg;
	dlg.SetRegKey(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordWarning"));
	if (dlg.DoModal() != IDOK) return;
}
void CSettingVRecTab::OnBtnVRecStopWarning() 
{
	UpdateData(TRUE);	// DDX�X�V

	CSetChatDialog dlg;
	dlg.SetRegKey(_T("Software\\Tapur\\Tapur\\Settings\\VideoRecordStopWarning"));
	if (dlg.DoModal() != IDOK) return;
}

// FPS�ύX�`�F�b�N�C�x���g
void CSettingVRecTab::OnChgFps() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CString strOld = m_strFps;
	UpdateData(TRUE);	// DDX�X�V

	// �����̂ݗL��
	if (!ChkNaturalNumber(m_strFps)) {
		m_strFps = strOld;
		UpdateData(FALSE);	// DDX�X�V
		return;
	}

	UpdateData(FALSE);	// DDX�X�V

	/// �K�p�{�^���̗L����
	OnChgForm();
}

// �t�H�[�����̓C�x���g
void CSettingVRecTab::OnChgForm() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	/// �K�p�{�^���̗L����
	CWnd *cWnd;
	cWnd = GetParentOwner();
	CSettingDialog *pDlg = (CSettingDialog *)cWnd;
	pDlg->EnableApply();
}

/////////////////////////////////////////////////////////////////////////////
// local function

//
//  �@�\     : ���s�t���������ComboBox�ւ̃��X�g�o�^
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
//void CSettingVRecTab::AssignListToComboBox(CComboBox *Dest, CString Source, long Indx, long MaxIndxLimit) 
//{
//    Dest->ResetContent();
//
//	long MaxIndxCount = (MaxIndxLimit>0) ? MaxIndxLimit : 99999;
//	long Startpos = 0;
//	long Endpos = Source.Find(_T("\r"), Startpos);
//
//	while ((Endpos >= 0) && (MaxIndxCount > 0))
//	{
//		Dest->AddString(Source.Mid(Startpos, Endpos - Startpos));
//		Startpos = Endpos + 2;	// skip CR/LF
//		Endpos = Source.Find(_T("\r"), Startpos);	// InStr(Startpos, Source, vbCr)
//		MaxIndxCount--;
//	}
//
//	if (Indx < Dest->GetCount())
//		Dest->SetCurSel(Indx);
//}
void CSettingVRecTab::AssignListToComboBox(CComboBox *Dest, CStringList *Source) 
{
    Dest->ResetContent();

	while (Source->GetCount() > 0) {
		POSITION pos = Source->GetHeadPosition();
		if (pos == NULL)  break;
		Dest->AddString(Source->GetAt(pos));
		Source->RemoveAt(pos);
	}

	if (Dest->GetCount() > 0)
		Dest->SetCurSel(0);
}

//
//  �@�\     : ���W�X�g���֕ۑ�
//  
//  �@�\���� : 
//  
//  �Ԃ�l   : BOOL
//  
//  ���l     : 
//  
void CSettingVRecTab::Save() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	CString str;
	DWORD dw;

	UpdateData(TRUE);	// DDX�X�V

	/// ���W�X�g���X�V
 	//// �^�掞�x�����b�Z�[�W
	m_rgEnableVRecWarning = (m_blVRecWarning) ? 1 : 0;
	m_rgEnableVRecStopWarning = (m_blVRecStopWarning) ? 1 : 0;
	//// �r�f�I��ʂ�O�ʕ\��
	m_rgVSetTop = (m_blVSetTop) ? 1 : 0;
	//// �I�[�o�[���C�^��
	m_rgOverlay = (m_blNoOverlay) ? 0 : 1;
	//// �t�@�C�������ۑ�
	m_rgSep = (m_blSep) ? 1 : 0;
	str = m_arySepSize.GetAt(m_idxSepSize);
	dw = (DWORD)_ttoi(str);
	dw *= 1000;	// Gbyte -> Mbyte
	m_rgSepSize = dw;
	//// ��������
	m_rgAudio = (m_blAudio) ? 1 : 0;
 	//// �^�搧������
	str = m_aryVRecordLimit.GetAt(m_idxVRecordLimit);
	dw = (DWORD)_ttoi(str);
	dw *= 60;	// min -> sec
	m_rgVRecordLimit = dw;
 	//// �񈳏k or ���k
	// ���W�I�{�^���̎Q�ƒ�`�iDDX�͈����ɂ����̂Łj
	// �i�N���X�����o�[�Ő錾����Ɛ���ɓ��삵�Ȃ��j
	CButton *rd_cmp = (CButton *)GetDlgItem(IDC_RDO_CMP);
	if (rd_cmp->GetCheck()) {
		m_rgVCmp = 1;
	} else {
		m_rgVCmp = 0;
	}
 	//// ���k�`��
	str = m_aryVCmpName.GetAt(m_idxVCmpName);
	m_rgVCmpName = str;
 	//// FPS
	m_rgFps = (DWORD)_ttoi(m_strFps);
}


BOOL CSettingVRecTab::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	SHORT rc;
	CWnd *cWnd;

    if (pMsg->message == WM_KEYDOWN) {
		int nVirtKey = (int)pMsg->wParam;    // virtual-key code 
		/// TAB KEY : ��ʃt�H�[���փt�H�[�J�X�J��
		/// TAB+Ctrl KEY : �R���g���[���^�u�J��
		if (nVirtKey == VK_TAB) {
			cWnd = GetFocus();
			rc = ::GetKeyState(VK_CONTROL);
			if( ( rc & 0x8000 ) != 0 ) {
				cWnd = cWnd->GetParentOwner();
				CSettingDialog *pDlg = (CSettingDialog *)cWnd;
				CTabCtrlSSL *pTabCtrl = &(pDlg->m_tabCtrl);
				int pageid = pTabCtrl->GetSSLActivePage();
				int idx = pTabCtrl->GetSSLPageIndex(pageid);
				int idxMax = pTabCtrl->GetSSLPageCount();
				idx = (idx == (idxMax-1)) ? 0 : idx+1;
				pTabCtrl->ActivateSSLPage(idx);
				return TRUE;
			} else {
				//// �t�H�[�J�X�Ō�̃R���g���[���̂ݗL��
				if (cWnd->m_hWnd == GetDlgItem(IDC_CMB_CMP)->m_hWnd) {
					cWnd = cWnd->GetParentOwner();
					cWnd->SetFocus();
					return TRUE;
				}
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
