// VisionBlobThresholdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "MPlatformOlbSystem.h"
#include "IVision.h"
#include "VisionBlobThresholdDlg.h"
#include "MLCNet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisionBlobThresholdDlg dialog

extern	MPlatformOlbSystem	MOlbSystemPre;

CVisionBlobThresholdDlg::CVisionBlobThresholdDlg(IVision* plnkVision, int iCameraNo, int iModelNo, int* piReturn, CWnd* pParent /*=NULL*/)
	: CDialog(CVisionBlobThresholdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVisionBlobThresholdDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ASSERT(piReturn);

	m_plnkVision = plnkVision;
	m_iCameraNo = iCameraNo;
	m_iModelNo = iModelNo;
	m_piReturn = piReturn;

	// Histogram View Initialize
	unsigned char rgucTemp[256][256];
	int x,y,j;
	for (x=0; x<256; x++)
		for (y=0; y<256; y++)
		{
			m_rgucHistogram[x][y] = 0;
			rgucTemp[x][y] = 0;
		}

	// Vision Component 로 부터 Blob 영역에 대한 Histogram 얻어 오는 부분
	unsigned char rgucHist[256];
	for (j=0; j<256; j++)
		rgucHist[j] = 0;

	int iResult = m_plnkVision->GetBlobHistogram(iCameraNo, iModelNo, rgucHist);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		//MyMessageBox(strError.GetBuffer(strError.GetLength()), "Set Threshold Error", M_ICONERROR);
// jdy sesl		MyMessageBox(iResult, _T("Set Threshold Error"), M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);

		m_bTerminateDlg = TRUE;
		return;
	}

	m_bTerminateDlg = FALSE;

	// Histogram Y Value 제한 (Divider 구하기) & GrayScale Mean Level 구하기.
	long int lSum = 0;
	long int lArea = 0;
	int iMaxY = 0;
	for (x=0; x<256; x++)
	{
		iMaxY = max(iMaxY, rgucHist[x]);
		lSum += x * rgucHist[x];
		lArea += rgucHist[x];
	}
	if (lArea == 0)
	{
		m_dMeanValue = 0;
	}
	else
	{
		m_dMeanValue = lSum / lArea;
	}

	int iDivider = iMaxY / 256 + 1;

	// Temp View Buffer 할당
	int iY;
	for (x=0; x<256; x++)
	{
		iY = rgucHist[x] / iDivider;

		for (int y=0; y<iY; y++)
			rgucTemp[x][y] = 128;		
	}

	// Flip Histogram
	int iFlipY;
	for (y=0; y<256; y++)
	{
		for (int x=0; x<256; x++)
		{
			iFlipY = 255 - y;
			m_rgucHistogram[x][iFlipY] = rgucTemp[x][y];
		}
	}

	m_iThreshold = *piReturn;
}


void CVisionBlobThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisionBlobThresholdDlg)
	DDX_Control(pDX, IDS_THRESHOLD, m_ctrlLblThreshold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVisionBlobThresholdDlg, CDialog)
	//{{AFX_MSG_MAP(CVisionBlobThresholdDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionBlobThresholdDlg message handlers

BEGIN_EVENTSINK_MAP(CVisionBlobThresholdDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVisionBlobThresholdDlg)
	ON_EVENT(CVisionBlobThresholdDlg, IDB_MINUS_10, -600 /* Click */, OnClickMinus10, VTS_NONE)
	ON_EVENT(CVisionBlobThresholdDlg, IDB_MINUS_1, -600 /* Click */, OnClickMinus1, VTS_NONE)
	ON_EVENT(CVisionBlobThresholdDlg, IDB_PLUS_1, -600 /* Click */, OnClickPlus1, VTS_NONE)
	ON_EVENT(CVisionBlobThresholdDlg, IDB_PLUS_10, -600 /* Click */, OnClickPlus10, VTS_NONE)
	ON_EVENT(CVisionBlobThresholdDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CVisionBlobThresholdDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CVisionBlobThresholdDlg::OnClickMinus10() 
{
	// TODO: Add your control notification handler code here
	m_iThreshold -= 10;
	if (m_iThreshold < 0)
		m_iThreshold = 0;
	Invalidate(FALSE);
}

void CVisionBlobThresholdDlg::OnClickMinus1() 
{
	// TODO: Add your control notification handler code here
	m_iThreshold -= 1;
	if (m_iThreshold < 0)
		m_iThreshold = 0;
	Invalidate(FALSE);
}

void CVisionBlobThresholdDlg::OnClickPlus1() 
{
	// TODO: Add your control notification handler code here
	m_iThreshold += 1;
	if (m_iThreshold > 255)
		m_iThreshold = 255;
	Invalidate(FALSE);
}

void CVisionBlobThresholdDlg::OnClickPlus10() 
{
	// TODO: Add your control notification handler code here
	m_iThreshold += 10;
	if (m_iThreshold > 255)
		m_iThreshold = 255;
	Invalidate(FALSE);
}

void CVisionBlobThresholdDlg::OnClickSave() 
{
	// TODO: Add your control notification handler code here
	*m_piReturn = m_iThreshold;
	
	CDialog::OnOK();
}

void CVisionBlobThresholdDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CVisionBlobThresholdDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	drawHistogram(m_iThreshold);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CVisionBlobThresholdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_bTerminateDlg)
		CDialog::OnCancel();
	else
		drawHistogram(m_iThreshold);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	
	CString szTextTemp;
	//확인
	ReadDisplayMessage(SUB_DLG, _T("VisionBlobThresholdDlg_1"), &szTextTemp);
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	//나가기
	ReadDisplayMessage(SUB_DLG, _T("VisionBlobThresholdDlg_2"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	
	// 수정 (이 Dlg를 열면 항상 보고 해야 하나?)
	// 2011.11.05 Park, YK - //ELLAS_111103
	//MOlbSystemPre.GetLCNetComponent()->EqSpecialStateReport(eAlignCheck);
	MOlbSystemPre.GetLCNetComponent()->EqSpecialStateReport(eBondingCheck);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVisionBlobThresholdDlg::drawHistogram(int iThreshold)
{
	CWnd* pHistogramWnd = GetDlgItem(IDC_HISTOGRAM);
	CDC*  pDC = pHistogramWnd->GetDC();

	for (int x=0; x<256; x++)
	{
		for (int y=0; y<256; y++)
		{
			if (x == iThreshold)
				pDC->SetPixel(x, y, RGB(255, 0, 0));
			else
				pDC->SetPixel(x, y, RGB(m_rgucHistogram[x][y], m_rgucHistogram[x][y], m_rgucHistogram[x][y]));
		}
	}

	CString strThreshold;
	strThreshold.Format(_T("GS Mean: %.1lf, Threshold: %d"), m_dMeanValue, m_iThreshold);
	m_ctrlLblThreshold.SetWindowText(strThreshold);
}
