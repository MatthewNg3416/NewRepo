// TeachPanelAlignerOffsetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "TeachPanelAlignerOffsetDlg.h"
#include "MPlatformOlbSystem.h"
#include "TextDescriptor.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CTeachPanelAlignerOffsetDlg dialog

CTeachPanelAlignerOffsetDlg::CTeachPanelAlignerOffsetDlg(int iUnitID, CWnd* pParent /*=NULL*/)
	: CDialog(CTeachPanelAlignerOffsetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTeachPanelAlignerOffsetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iSelectedUnitID = iUnitID;
	m_iSelPos = 0;
	m_iRealPos = 0;
	m_bAllInput = FALSE;
	m_iCurrentPage = 0;

	m_plnkPanelAligner	= MOlbSystemPre.GetPanelAlignerComponent();

	/** PanelAligner에서 Offset 좌표 얻어오기 */
	m_plnkPanelAligner->GetXYTPosParameter(NULL, m_rgXYTOffsetPosData, NULL);
}

void CTeachPanelAlignerOffsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachPanelAlignerOffsetDlg)
	DDX_Control(pDX, IDB_ALL_INPUT, m_btnAllInput);
	DDX_Control(pDX, IDB_NEXT, m_btnNext);
	DDX_Control(pDX, IDB_PREVIOUS, m_btnPrev);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDB_POS_0, m_BtnPos[0]);
	DDX_Control(pDX, IDB_POS_1, m_BtnPos[1]);
	DDX_Control(pDX, IDB_POS_2, m_BtnPos[2]);
	DDX_Control(pDX, IDB_POS_3, m_BtnPos[3]);
	DDX_Control(pDX, IDB_POS_4, m_BtnPos[4]);
	DDX_Control(pDX, IDB_POS_5, m_BtnPos[5]);
	DDX_Control(pDX, IDB_POS_6, m_BtnPos[6]);
	DDX_Control(pDX, IDB_POS_7, m_BtnPos[7]);
	DDX_Control(pDX, IDB_POS_8, m_BtnPos[8]);
	DDX_Control(pDX, IDB_POS_9, m_BtnPos[9]);
	DDX_Control(pDX, IDB_POS_10, m_BtnPos[10]);
	DDX_Control(pDX, IDB_POS_11, m_BtnPos[11]);

	DDX_Control(pDX, IDC_POS_0_X, m_sCoord[0][0]);
	DDX_Control(pDX, IDC_POS_1_X, m_sCoord[0][1]);
	DDX_Control(pDX, IDC_POS_2_X, m_sCoord[0][2]);
	DDX_Control(pDX, IDC_POS_3_X, m_sCoord[0][3]);
	DDX_Control(pDX, IDC_POS_4_X, m_sCoord[0][4]);
	DDX_Control(pDX, IDC_POS_5_X, m_sCoord[0][5]);
	DDX_Control(pDX, IDC_POS_6_X, m_sCoord[0][6]);
	DDX_Control(pDX, IDC_POS_7_X, m_sCoord[0][7]);
	DDX_Control(pDX, IDC_POS_8_X, m_sCoord[0][8]);
	DDX_Control(pDX, IDC_POS_9_X, m_sCoord[0][9]);
	DDX_Control(pDX, IDC_POS_10_X, m_sCoord[0][10]);
	DDX_Control(pDX, IDC_POS_11_X, m_sCoord[0][11]);
	DDX_Control(pDX, IDC_POS_0_Y, m_sCoord[1][0]);
	DDX_Control(pDX, IDC_POS_1_Y, m_sCoord[1][1]);
	DDX_Control(pDX, IDC_POS_2_Y, m_sCoord[1][2]);
	DDX_Control(pDX, IDC_POS_3_Y, m_sCoord[1][3]);
	DDX_Control(pDX, IDC_POS_4_Y, m_sCoord[1][4]);
	DDX_Control(pDX, IDC_POS_5_Y, m_sCoord[1][5]);
	DDX_Control(pDX, IDC_POS_6_Y, m_sCoord[1][6]);
	DDX_Control(pDX, IDC_POS_7_Y, m_sCoord[1][7]);
	DDX_Control(pDX, IDC_POS_8_Y, m_sCoord[1][8]);
	DDX_Control(pDX, IDC_POS_9_Y, m_sCoord[1][9]);
	DDX_Control(pDX, IDC_POS_10_Y, m_sCoord[1][10]);
	DDX_Control(pDX, IDC_POS_11_Y, m_sCoord[1][11]);
	DDX_Control(pDX, IDC_POS_0_T, m_sCoord[2][0]);
	DDX_Control(pDX, IDC_POS_1_T, m_sCoord[2][1]);
	DDX_Control(pDX, IDC_POS_2_T, m_sCoord[2][2]);
	DDX_Control(pDX, IDC_POS_3_T, m_sCoord[2][3]);
	DDX_Control(pDX, IDC_POS_4_T, m_sCoord[2][4]);
	DDX_Control(pDX, IDC_POS_5_T, m_sCoord[2][5]);
	DDX_Control(pDX, IDC_POS_6_T, m_sCoord[2][6]);
	DDX_Control(pDX, IDC_POS_7_T, m_sCoord[2][7]);
	DDX_Control(pDX, IDC_POS_8_T, m_sCoord[2][8]);
	DDX_Control(pDX, IDC_POS_9_T, m_sCoord[2][9]);
	DDX_Control(pDX, IDC_POS_10_T, m_sCoord[2][10]);
	DDX_Control(pDX, IDC_POS_11_T, m_sCoord[2][11]);
}


BEGIN_MESSAGE_MAP(CTeachPanelAlignerOffsetDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachPanelAlignerOffsetDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachPanelAlignerOffsetDlg message handlers

void CTeachPanelAlignerOffsetDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CTeachPanelAlignerOffsetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateButton();
	UpdateCoordinate();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerOffsetDlg_1"), &szTextTemp);
	//Panel Aligner Offset 좌표 설정
	SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerOffsetDlg_2"), &szTextTemp);
	//X 축
	GetDlgItem(IDB_X)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerOffsetDlg_3"), &szTextTemp);
	//Y 축
	GetDlgItem(IDB_Y)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerOffsetDlg_4"), &szTextTemp);
	//T 축
	GetDlgItem(IDB_T)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerOffsetDlg_5"), &szTextTemp);
	//이전
	GetDlgItem(IDB_PREVIOUS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerOffsetDlg_6"), &szTextTemp);
	//다음
	GetDlgItem(IDB_NEXT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerOffsetDlg_7"), &szTextTemp);
	//일괄입력
	GetDlgItem(IDB_ALL_INPUT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerOffsetDlg_8"), &szTextTemp);
	//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerOffsetDlg_9"), &szTextTemp);
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CTeachPanelAlignerOffsetDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachPanelAlignerOffsetDlg)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_PREVIOUS, -600 /* Click */, OnPrevious, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_NEXT, -600 /* Click */, OnNext, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_ALL_INPUT, -600 /* Click */, OnAllInput, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_X, -600 /* Click */, OnX, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_Y, -600 /* Click */, OnY, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_T, -600 /* Click */, OnT, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_POS_0, -600 /* Click */, OnPos0, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_POS_1, -600 /* Click */, OnPos1, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_POS_2, -600 /* Click */, OnPos2, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_POS_3, -600 /* Click */, OnPos3, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_POS_4, -600 /* Click */, OnPos4, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_POS_5, -600 /* Click */, OnPos5, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_POS_6, -600 /* Click */, OnPos6, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_POS_7, -600 /* Click */, OnPos7, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_POS_8, -600 /* Click */, OnPos8, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_POS_9, -600 /* Click */, OnPos9, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_POS_10, -600 /* Click */, OnPos10, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerOffsetDlg, IDB_POS_11, -600 /* Click */, OnPos11, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachPanelAlignerOffsetDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CTeachPanelAlignerOffsetDlg::OnSave() 
{
//	 Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50600,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_plnkPanelAligner->SaveXYTOffsetPosParameter(m_rgXYTOffsetPosData);
}

void CTeachPanelAlignerOffsetDlg::OnPrevious() 
{
	// TODO: Add your control notification handler code here
	if ((m_iCurrentPage * DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE) > 0)
		m_iCurrentPage--;

	UpdateButton();
	UpdateCoordinate();
}

void CTeachPanelAlignerOffsetDlg::OnNext() 
{
	// TODO: Add your control notification handler code here
	if (((m_iCurrentPage + 1) * DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE) < DEF_PANEL_ALIGNER_MAX_POSITION)
		m_iCurrentPage++;

	UpdateButton();
	UpdateCoordinate();
}

void CTeachPanelAlignerOffsetDlg::OnAllInput() 
{
	// TODO: Add your control notification handler code here
	if (m_btnAllInput.GetValue() == 0)
		m_bAllInput = FALSE;
	else
		m_bAllInput = TRUE;
}

void CTeachPanelAlignerOffsetDlg::OnX() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_sCoord[0][m_iSelPos].GetWindowText(cBeforeVal, 30);
	
	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);

	if (m_bAllInput == TRUE)	// 일괄 입력 선택
	{
		for (int i = 0; i < DEF_PANEL_ALIGNER_MAX_POSITION; i++)
			m_rgXYTOffsetPosData[i].dX = dNewTargetPos;														

		for (int i = 0; i < DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE; i++)
		{
			m_sCoord[0][i].SetCaption(cAfterVal);

			swprintf(szTemp, _T("%.3f"), m_rgXYTOffsetPosData[i].dX);
			m_sCoord[0][i].SetCaption(szTemp);
		}
	}
	else
	{
		m_sCoord[0][m_iSelPos].SetCaption(cAfterVal);

		m_rgXYTOffsetPosData[m_iRealPos].dX = dNewTargetPos;														

		swprintf(szTemp, _T("%.3f"), m_rgXYTOffsetPosData[m_iRealPos].dX);
		m_sCoord[0][m_iSelPos].SetCaption(szTemp);
	}
}

void CTeachPanelAlignerOffsetDlg::OnY() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_sCoord[1][m_iSelPos].GetWindowText(cBeforeVal, 30);
	
	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);

	if (m_bAllInput == TRUE)	// 일괄 입력 선택
	{
		for (int i = 0; i < DEF_PANEL_ALIGNER_MAX_POSITION; i++)
			m_rgXYTOffsetPosData[i].dY = dNewTargetPos;														

		for (int i = 0; i < DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE; i++)
		{
			m_sCoord[1][i].SetCaption(cAfterVal);

			swprintf(szTemp, _T("%.3f"), m_rgXYTOffsetPosData[i].dY);
			m_sCoord[1][i].SetCaption(szTemp);
		}
	}
	else
	{
		m_sCoord[1][m_iSelPos].SetCaption(cAfterVal);

		m_rgXYTOffsetPosData[m_iRealPos].dY = dNewTargetPos;														

		swprintf(szTemp, _T("%.3f"), m_rgXYTOffsetPosData[m_iRealPos].dY);
		m_sCoord[1][m_iSelPos].SetCaption(szTemp);
	}
}

void CTeachPanelAlignerOffsetDlg::OnT() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;
	WCHAR szTemp[250];

	if (m_bAllInput == FALSE
		&& (m_iRealPos == DEF_PANEL_ALIGNER_FIRST_MARK_POS	
			|| m_iRealPos == DEF_PANEL_ALIGNER_SECOND_MARK_POS))
	{
//		 VisionMark 위치의 T 축은 수정할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50601,_T("Error"), M_ICONERROR);
		return;
	}

	m_sCoord[2][m_iSelPos].GetWindowText(cBeforeVal, 30);
	
	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);

	if (m_bAllInput == TRUE)	// 일괄 입력 선택
	{
		for (int i =0 ; i < DEF_PANEL_ALIGNER_MAX_POSITION; i++)
		{
			if (i == DEF_PANEL_ALIGNER_FIRST_MARK_POS
				|| i == DEF_PANEL_ALIGNER_SECOND_MARK_POS)
			{
				; // Null Operation
			}
			else
			{
				m_rgXYTOffsetPosData[i].dT = dNewTargetPos;														
			}
		}

		for (int i = 0; i < DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE; i++)
		{
			m_sCoord[2][i].SetCaption(cAfterVal);

			swprintf(szTemp, _T("%.3f"), m_rgXYTOffsetPosData[i].dT);
			m_sCoord[2][i].SetCaption(szTemp);
		}
	}
	else
	{
		m_sCoord[2][m_iSelPos].SetCaption(cAfterVal);

		m_rgXYTOffsetPosData[m_iRealPos].dT = dNewTargetPos;														

		swprintf(szTemp, _T("%.3f"), m_rgXYTOffsetPosData[m_iRealPos].dT);
		m_sCoord[2][m_iSelPos].SetCaption(szTemp);
	}
}

void CTeachPanelAlignerOffsetDlg::OnPos0() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(0);
}

void CTeachPanelAlignerOffsetDlg::OnPos1() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(1);
}

void CTeachPanelAlignerOffsetDlg::OnPos2() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(2);
}

void CTeachPanelAlignerOffsetDlg::OnPos3() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(3);
}

void CTeachPanelAlignerOffsetDlg::OnPos4() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(4);
}

void CTeachPanelAlignerOffsetDlg::OnPos5() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(5);
}

void CTeachPanelAlignerOffsetDlg::OnPos6() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(6);
}

void CTeachPanelAlignerOffsetDlg::OnPos7() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(7);
}

void CTeachPanelAlignerOffsetDlg::OnPos8() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(8);
}

void CTeachPanelAlignerOffsetDlg::OnPos9() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(9);
}

void CTeachPanelAlignerOffsetDlg::OnPos10() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(10);
}

void CTeachPanelAlignerOffsetDlg::OnPos11() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(11);
}

/**
 * 선택 Position 변경 (XYT축)
 */
void CTeachPanelAlignerOffsetDlg::SelectPosition(int iPosIndex)
{
	m_BtnPos[m_iSelPos].SetValue(FALSE);
	m_iSelPos = iPosIndex;
	m_BtnPos[iPosIndex].SetValue(TRUE);

	m_iRealPos = m_iSelPos + (m_iCurrentPage * DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE);

	UpdateCoordinate();
}

/**
 * Offset좌표 Display (XYT축)
 */
void CTeachPanelAlignerOffsetDlg::UpdateCoordinate()
{
	WCHAR szTemp[250];

	for(int i = 0; i < DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE; i++)
	{
		/** 옵셋좌표 화면 Display */
		if ((i + (m_iCurrentPage * DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE)) < DEF_PANEL_ALIGNER_MAX_POSITION)
		{
			swprintf(szTemp, _T("%.3f"), m_rgXYTOffsetPosData[i + (m_iCurrentPage * DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE)].dX);
			m_sCoord[0][i].SetCaption(szTemp);
			swprintf(szTemp, _T("%.3f"), m_rgXYTOffsetPosData[i + (m_iCurrentPage * DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE)].dY);
			m_sCoord[1][i].SetCaption(szTemp);
			swprintf(szTemp, _T("%.3f"), m_rgXYTOffsetPosData[i + (m_iCurrentPage * DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE)].dT);
			m_sCoord[2][i].SetCaption(szTemp);
		}
	}
}

/**
 * Position Button 상태 Update
 */
void CTeachPanelAlignerOffsetDlg::UpdateButton()
{
	/** Teaching Position Name Reading */
	STeachingInfo sTeachingInfo = m_plnkPanelAligner->GetXYTTeachingInfo();

	int iBtnNoUpToCurrentPage = m_iCurrentPage * DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE;
	int iIndex = -1;
	for (int i = 0; i < DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE; i++)
	{
		iIndex = i + iBtnNoUpToCurrentPage;

		/** 옵셋좌표 화면 Display */
		if (iIndex < DEF_PANEL_ALIGNER_MAX_POSITION
			&& (sTeachingInfo.m_sPosition[iIndex]).m_bUse == TRUE)
		{
			m_BtnPos[i].SetWindowText((LPCTSTR)((sTeachingInfo.m_sPosition[iIndex]).m_strNewLineName[0]));
			
			m_sCoord[0][i].ShowWindow(SW_SHOW);
			m_sCoord[1][i].ShowWindow(SW_SHOW);
			m_sCoord[2][i].ShowWindow(SW_SHOW);
			
			m_BtnPos[i].ShowWindow(SW_SHOW);
		}
		else
		{
			m_sCoord[0][i].ShowWindow(SW_HIDE);
			m_sCoord[1][i].ShowWindow(SW_HIDE);
			m_sCoord[2][i].ShowWindow(SW_HIDE);

			m_BtnPos[i].ShowWindow(SW_HIDE);
		}
	}

	for (int i = 0; i < DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE; i++)
	{
		if (m_BtnPos[i].GetEnabled() == TRUE)
		{
			SelectPosition(i);		// Position 버튼 정렬 후 선택위치는 항상 처음이 되게..
			break;
		}
	}

	// Prev, Next Button 처리
	if (iBtnNoUpToCurrentPage <= 0)
		m_btnPrev.ShowWindow(SW_HIDE);
	else
		m_btnPrev.ShowWindow(SW_SHOW);
	
	if ((m_iCurrentPage + 1) * DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE >= DEF_PANEL_ALIGNER_MAX_POSITION)
		m_btnNext.ShowWindow(SW_HIDE);
	else
		m_btnNext.ShowWindow(SW_SHOW);
}
