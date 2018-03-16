// ManualCameraExpShrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "MPlatformOlbSystem.h"
#include "ManualCameraExpShrDlg.h"
//#include "MTabAligner.h"
#include "MTabCarrier.h"
#include "MCameraCarrier.h"
#include "IIO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	MPlatformOlbSystem	MOlbSystemPre; 

/////////////////////////////////////////////////////////////////////////////
// CManualCameraExpShrDlg dialog


CManualCameraExpShrDlg::CManualCameraExpShrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManualCameraExpShrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualCameraExpShrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	//for (int i = 0; i < DEF_MAX_GROUP; i++)
		//m_rgpTabAligner[i] = MOlbSystemPre.GetTabAlignerComponent(i);
	m_rgpTabCarrier[0] = MOlbSystemPre.GetTabCarrierComponent(0);
	m_rgpTabCarrier[1] = MOlbSystemPre.GetRTabCarrierComponent(0);

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		m_rgpInspectCamCarrier[i] = MOlbSystemPre.GetCameraCarrierComponent(i);
	}

	m_bChangeGroup = FALSE;
}


void CManualCameraExpShrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualCameraExpShrDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_EXPAND, m_btnExpand[0]);
	DDX_Control(pDX, IDB_EXPAND2, m_btnExpand[1]);
	DDX_Control(pDX, IDB_EXPAND3, m_btnExpand[2]);
	DDX_Control(pDX, IDB_EXPAND4, m_btnExpand[3]);
	DDX_Control(pDX, IDB_EXPAND5, m_btnExpand[4]);
	DDX_Control(pDX, IDB_EXPAND6, m_btnExpand[5]);

	DDX_Control(pDX, IDB_SHRINK, m_btnShrink[0]);
	DDX_Control(pDX, IDB_SHRINK2, m_btnShrink[1]);
	DDX_Control(pDX, IDB_SHRINK3, m_btnShrink[2]);
	DDX_Control(pDX, IDB_SHRINK4, m_btnShrink[3]);
	DDX_Control(pDX, IDB_SHRINK5, m_btnShrink[4]);
	DDX_Control(pDX, IDB_SHRINK6, m_btnShrink[5]);

	DDX_Control(pDX, IDB_FRONT_GROUP, m_btnSelectGroup[DEF_FRONT_GROUP]);
	DDX_Control(pDX, IDB_REAR_GROUP, m_btnSelectGroup[DEF_REAR_GROUP]);

}


BEGIN_MESSAGE_MAP(CManualCameraExpShrDlg, CDialog)
	//{{AFX_MSG_MAP(CManualCameraExpShrDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualCameraExpShrDlg message handlers

void CManualCameraExpShrDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BEGIN_EVENTSINK_MAP(CManualCameraExpShrDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CManualCameraExpShrDlg)
	ON_EVENT(CManualCameraExpShrDlg, IDB_EXPAND, -600 /* Click */, OnExpandTabAligner, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_SHRINK, -600 /* Click */, OnShrinkTabAligner, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_EXPAND2, -600 /* Click */, OnExpandInspection, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_SHRINK2, -600 /* Click */, OnShrinkInspection, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_EXPAND3, -600 /* Click */, OnClickExpand3, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_SHRINK3, -600 /* Click */, OnClickShrink3, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_EXPAND4, -600 /* Click */, OnClickExpand4, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_SHRINK4, -600 /* Click */, OnClickShrink4, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_EXPAND5, -600 /* Click */, OnClickExpand5, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_SHRINK5, -600 /* Click */, OnClickShrink5, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_EXPAND6, -600 /* Click */, OnClickExpand6, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_SHRINK6, -600 /* Click */, OnClickShrink6, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_FRONT_GROUP, -600 /* Click */, OnClickFrontGroup, VTS_NONE)
	ON_EVENT(CManualCameraExpShrDlg, IDB_REAR_GROUP, -600 /* Click */, OnClickRearGroup, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CManualCameraExpShrDlg::OnExpandTabAligner() 
{
	int iResult = m_rgpTabCarrier[0]->ExpandCamera();
	if (iResult)
	{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()),_T("Error"), M_ICONERROR);
		return;
	}
}

void CManualCameraExpShrDlg::OnShrinkTabAligner() 
{
	int iResult = m_rgpTabCarrier[0]->ShrinkCamera();
	if (iResult)
	{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()),_T("Error"), M_ICONERROR);
		return;
	}
}

void CManualCameraExpShrDlg::OnExpandInspection() 
{
	int iResult = m_rgpTabCarrier[1]->ExpandCamera();
	if (iResult)
	{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()),_T("Error"), M_ICONERROR);
		return;
	}
}

void CManualCameraExpShrDlg::OnShrinkInspection() 
{
	int iResult = m_rgpTabCarrier[1]->ShrinkCamera();
	if (iResult)
	{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()),_T("Error"), M_ICONERROR);
		return;
	}
}

void CManualCameraExpShrDlg::OnClickExpand3()
{
	// TODO: Add your control notification handler code here
	int iResult = m_rgpInspectCamCarrier[0]->Expand();
	if (iResult)
	{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()),_T("Error"), M_ICONERROR);
		return;
	}
}

void CManualCameraExpShrDlg::OnClickShrink3() 
{
	// TODO: Add your control notification handler code here
	int iResult = m_rgpInspectCamCarrier[0]->Shrink();
	if (iResult)
	{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()),_T("Error"), M_ICONERROR);
		return;
	}	
}

void CManualCameraExpShrDlg::OnClickExpand4() 
{
	// TODO: Add your control notification handler code here
	int iResult = m_rgpInspectCamCarrier[1]->Expand();
	if (iResult)
	{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()),_T("Error"), M_ICONERROR);
		return;
	}	
}

void CManualCameraExpShrDlg::OnClickShrink4() 
{
	// TODO: Add your control notification handler code here
	int iResult = m_rgpInspectCamCarrier[1]->Shrink();
	if (iResult)
	{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()),_T("Error"), M_ICONERROR);
		return;
	}	
}

void CManualCameraExpShrDlg::OnClickExpand5() 
{
	// TODO: Add your control notification handler code here
	int iResult = m_rgpInspectCamCarrier[2]->Expand();
	if (iResult)
	{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()),_T("Error"), M_ICONERROR);
		return;
	}	
}

void CManualCameraExpShrDlg::OnClickShrink5() 
{
	// TODO: Add your control notification handler code here
	int iResult = m_rgpInspectCamCarrier[2]->Shrink();
	if (iResult)
	{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()),_T("Error"), M_ICONERROR);
		return;
	}	
}

void CManualCameraExpShrDlg::OnClickExpand6() 
{
	// TODO: Add your control notification handler code here
	int iResult = m_rgpInspectCamCarrier[3]->Expand();
	if (iResult)
	{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()),_T("Error"), M_ICONERROR);
		return;
	}	
}

void CManualCameraExpShrDlg::OnClickShrink6() 
{
	// TODO: Add your control notification handler code here
	int iResult = m_rgpInspectCamCarrier[3]->Shrink();
	if (iResult)
	{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()),_T("Error"), M_ICONERROR);
		return;
	}	
}

void CManualCameraExpShrDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == m_uiTimerID)
	{
		if (m_bChangeGroup == TRUE)
		{
			if (m_iSelectGroup == DEF_FRONT_GROUP)
			{
				m_btnSelectGroup[DEF_REAR_GROUP].SetValue(0);
				m_btnSelectGroup[DEF_FRONT_GROUP].SetValue(1);
				for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
				{
					m_rgpInspectCamCarrier[i] = MOlbSystemPre.GetCameraCarrierComponent(i);
				}
			}
			else
			{
				m_btnSelectGroup[DEF_FRONT_GROUP].SetValue(0);
				m_btnSelectGroup[DEF_REAR_GROUP].SetValue(1);
				for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
				{
					m_rgpInspectCamCarrier[i] = MOlbSystemPre.GetRCameraCarrierComponent(i);
				}
			}
			m_bChangeGroup = FALSE;
		}

		m_btnExpand[0].SetValue(m_rgpTabCarrier[0]->IsExpandCamera());
		m_btnExpand[1].SetValue(m_rgpTabCarrier[1]->IsExpandCamera());

		m_btnShrink[0].SetValue(m_rgpTabCarrier[0]->IsShrinkCamera());
		m_btnShrink[1].SetValue(m_rgpTabCarrier[1]->IsShrinkCamera());

		for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		{
			m_btnExpand[2+i].SetValue(m_rgpInspectCamCarrier[0]->IsExpand());
			m_btnShrink[2+i].SetValue(m_rgpInspectCamCarrier[0]->IsShrink());
		}
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CManualCameraExpShrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬
	// Multi Language Button Title Display                                                                早
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭
	CString szTextTemp;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualCameraExpShrDlg_1"), &szTextTemp);
	//釭陛晦
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

//@#ifdef DEF_SOURCE_SYSTEM
//@	for (int i = 0; i < DEF_MAX_GROUP; i++)
//@		m_btnSelectGroup[i].EnableWindow(FALSE);
//@#else
	m_iSelectGroup = DEF_NONE_GROUP;
	OnClickFrontGroup();
//@#endif
	for (int i=DEF_MAX_INSPECTION_CAM_CARRIER; i<DEF_MAX_MANUAL_INSPECT_CAM_CARRIER; i++)
	{
		m_btnExpand[2+i].EnableWindow(FALSE);
		m_btnShrink[2+i].EnableWindow(FALSE);
	}

	
	m_uiTimerID = SetTimer(1, 200, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CManualCameraExpShrDlg::OnExit() 
{
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	CManualCameraExpShrDlg::OnCancel();		
}

void CManualCameraExpShrDlg::OnClickFrontGroup() 
{
	// TODO: Add your control notification handler code here
	if (m_iSelectGroup == DEF_FRONT_GROUP)
		return;
	
	m_iSelectGroup = DEF_FRONT_GROUP;
	m_bChangeGroup = TRUE;
}

void CManualCameraExpShrDlg::OnClickRearGroup() 
{
	// TODO: Add your control notification handler code here
	if (m_iSelectGroup == DEF_REAR_GROUP)
		return;

	m_iSelectGroup = DEF_REAR_GROUP;
	m_bChangeGroup = TRUE;
}
