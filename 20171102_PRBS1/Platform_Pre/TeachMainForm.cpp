// TeachMainForm.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include "IVision.h"
#include "MVisionCalibration.h"
#include "TeachMainForm.h"
#include "TeachPanelAlignerDlg.h"
#include "TeachPanelAlignerOffsetDlg.h"
#include "TeachCarrierDlg.h"
#include "TeachPressDlg.h"
#include "TeachBadTabDetectorDlg.h"
#include "TeachTabMounterDlg.h"
#include "TeachCameraCarrierDlg.h"
#include "TeachTransferDlg.h"
#include "TeachModelChangeDlg.h"
#include "DlgViewWorkSchedule.h"
#include "TeachMoldPinDlg.h"
#include "TeachCarrierCameraExpandDlg.h"
#include "TeachHandlerDlg.h" //SJ_YYK 161104 Add..

#include "MProcessData.h"
#include "MPanelData.h"
#include "MCameraCarrier.h"
#include "MTabCarrier.h"
#include "MCtrlPanelAligner.h"
#include "MCtrlInspectionCamera.h"
#include "IAxis.h"

#include <math.h>

#include "MTrsAutoManager.h"
#include "MSystemData.h" //171009 JSh

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTeachMainForm
extern	MPlatformOlbSystem	MOlbSystemPre;

IMPLEMENT_DYNCREATE(CTeachMainForm, CFormView)

CTeachMainForm::CTeachMainForm()
	: CFormView(CTeachMainForm::IDD)
{
	//{{AFX_DATA_INIT(CTeachMainForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_plnkProcessData = MOlbSystemPre.GetProcessData();
	m_plnkPanelData = MOlbSystemPre.GetPanelData();
	
	m_bInitialized = FALSE;
}

CTeachMainForm::~CTeachMainForm()
{
}

void CTeachMainForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachMainForm)
	DDX_Control(pDX, IDC_FIX_TEACH, m_lblFixedGroup);
	DDX_Control(pDX, IDC_MODEL_TEACH, m_lblOffsetGroup);
	DDX_Control(pDX, IDC_OFFSET_TEACH, m_lblOffsetDirectInputGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTeachMainForm, CFormView)
	//{{AFX_MSG_MAP(CTeachMainForm)
	ON_WM_PAINT()
	ON_MESSAGE(WM_DISP_MULTI_LANGUAGE_DLG_REQ, OnDisplayLanguage)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachMainForm diagnostics

#ifdef _DEBUG
void CTeachMainForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CTeachMainForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTeachMainForm message handlers

void CTeachMainForm::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CFormView::OnPaint() for painting messages
}

BEGIN_EVENTSINK_MAP(CTeachMainForm, CFormView)
    //{{AFX_EVENTSINK_MAP(CTeachMainForm)
	ON_EVENT(CTeachMainForm, IDB_PANEL_ALIGNER_FIX, -600 /* Click */, OnPanelAlignerFix, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_PANEL_ALIGNER_MODEL, -600 /* Click */, OnPanelAlignerModel, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_PANEL_ALIGNER_OFFSET, -600 /* Click */, OnPanelAlignerOffset, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_CARRIER_FIX, -600 /* Click */, OnTabCarrierFix, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_CARRIER_MODEL, -600 /* Click */, OnTabCarrierModel, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_FEEDER_PRESS_FIX, -600 /* Click */, OnTabFeederPressFix, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_FEEDER_SUPPLIER_FIX, -600 /* Click */, OnTabFeederSupplierFix, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_FEEDER_BADTAB_MODEL, -600 /* Click */, OnTabFeederBadTabModel, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_MOUNTER_FIX, -600 /* Click */, OnTabMounterFix, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_MOUNTER_MODEL, -600 /* Click */, OnTabMounterModel, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_INSPECT_CAM_MODEL2, -600 /* Click */, OnInspectCamModel2, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_MOUNTER_FIX2, -600 /* Click */, OnClickTabMounterFix2, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_MOUNTER_MODEL2, -600 /* Click */, OnClickTabMounterModel2, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_INSPECT_CAM_FIX1, -600 /* Click */, OnClickInspectCamFix1, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_INSPECT_CAM_FIX2, -600 /* Click */, OnClickInspectCamFix2, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_INSPECT_CAM_MODEL1, -600 /* Click */, OnClickInspectCamModel1, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_CARRIER_FIX2, -600 /* Click */, OnClickTabCarrierFix2, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_CARRIER_MODEL2, -600 /* Click */, OnClickTabCarrierModel2, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_FEEDER_PRESS_FIX2, -600 /* Click */, OnClickTabFeederPressFix2, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_FEEDER_BADTAB_MODEL2, -600 /* Click */, OnClickTabFeederBadtabModel2, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_PANEL_CAM_FIX, -600 /* Click */, OnClickPanelCamFix, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_PANEL_CAM_MODEL, -600 /* Click */, OnClickPanelCamModel, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_MODELCHANGE_FIX, -600 /* Click */, OnClickModelChangeFix, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_MODELCHANGE_MODEL, -600 /* Click */, OnClickModelChangeModel, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_FEEDER_SUPPLIER_FIX2, -600 /* Click */, OnTabFeederSupplierFix2, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_PANEL_PRESS_GET_T_OFFSET, -600 /* Click */, OnClickPanelPressGetTOffset, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_CARRIER_GET_LOAD_OFFSET, -600 /* Click */, OnClickTabCarrierGetLoadOffset, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_PANEL_TRANSFER_CENTER_FIX, -600 /* Click */, OnClickPanelTransferCenterFix, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_PANEL_TRANSFER_IN_FIX, -600 /* Click */, OnClickPanelTransferInFix, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_PANEL_TRANSFER_OUT_FIX, -600 /* Click */, OnClickPanelTransferOutFix, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_PANEL_TRANSFER_CENTER_MODEL, -600 /* Click */, OnClickPanelTransferCenterModel, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_PANEL_TRANSFER_IN_MODEL, -600 /* Click */, OnClickPanelTransferInModel, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_PANEL_TRANSFER_OUT_MODEL, -600 /* Click */, OnClickPanelTransferOutModel, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_FRONT_WORK_SCHEDULE, -600 /* Click */, OnClickFrontWorkSchedule, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_REAR_WORK_SCHEDULE, -600 /* Click */, OnClickRearWorkSchedule, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_MODELCHANGE2_FIX, -600 /* Click */, OnClickModelchange2Fix, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_MODELCHANGE2_MODEL, -600 /* Click */, OnClickModelchange2Model, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_FRONT_WORK_SCHEDULE2, -600 /* Click */, OnClickFrontWorkSchedule2, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_REAR_WORK_SCHEDULE2, -600 /* Click */, OnClickRearWorkSchedule2, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_CARRIER_CAM_EXPAND, -600 /* Click */, OnClickTabCarrierCamExpand, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_RTAB_CARRIER_CAM_EXPAND, -600 /* Click */, OnClickRtabCarrierCamExpand, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TAB_CIDB_TURN_HANDLER_FIXARRIER_FIX3, -600 /* Click */, OnClickTabCidbTurnHandlerFixarrierFix3, VTS_NONE)
	ON_EVENT(CTeachMainForm, IDB_TURN_HANDLER_MODEL, -600 /* Click */, OnClickTurnHandlerModel, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachMainForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	/*/
#ifdef DEF_SOURCE_SYSTEM
	GetDlgItem(IDB_TAB_MOUNTER_FIX2)->ShowWindow(FALSE);
	GetDlgItem(IDB_TAB_MOUNTER_MODEL2)->ShowWindow(FALSE);
	GetDlgItem(IDB_INSPECT_CAM_FIX2)->ShowWindow(FALSE);
	GetDlgItem(IDB_INSPECT_CAM_MODEL2)->ShowWindow(FALSE);
#endif
	/*/

		//2009.07.24 CYJ
	GetDlgItem(IDB_PANEL_PRESS_GET_T_OFFSET)->ShowWindow(SW_HIDE);

		//2009.07.24 CYJ
	if (SYSTEM_TYPE_GATE == MOlbSystemPre.GetSystemData()->m_eSystemType)
		GetDlgItem(IDB_TAB_CARRIER_GET_LOAD_OFFSET)->ShowWindow(SW_HIDE);

// ML_hongju
	MOlbSystemPre.GetTrsAutoManagerComponent()->SetTeachViewHandle(m_hWnd);
	this->SendMessage(WM_DISP_MULTI_LANGUAGE_DLG_REQ);


	GetDlgItem(IDB_TAB_FEEDER_SUPPLIER_FIX)->SetWindowText(_T("Mold Pin\r\nFixed"));
	GetDlgItem(IDB_TAB_FEEDER_SUPPLIER_FIX2)->SetWindowText(_T("Mold Pin\r\nFixed(Rear)"));

#ifdef DEF_GATE_SYSTEM
	GetDlgItem(IDB_FRONT_WORK_SCHEDULE2)->EnableWindow(FALSE);
	GetDlgItem(IDB_REAR_WORK_SCHEDULE2)->EnableWindow(FALSE);

	GetDlgItem(IDB_PANEL_TRANSFER_IN_FIX)->EnableWindow(FALSE);
	GetDlgItem(IDB_PANEL_TRANSFER_IN_MODEL)->EnableWindow(FALSE);
#endif

	//SJ_YYK 150109 Add..
	GetDlgItem(IDB_PANEL_TRANSFER_IN_FIX)->EnableWindow(FALSE);
	GetDlgItem(IDB_PANEL_TRANSFER_IN_MODEL)->EnableWindow(FALSE);
	//@__________________
//@	GetDlgItem(IDB_MODELCHANGE_FIX)->EnableWindow(FALSE);
//@	GetDlgItem(IDB_MODELCHANGE2_FIX)->EnableWindow(FALSE);
	//@__________________

	GetDlgItem(IDB_PANEL_TRANSFER_IN_FIX)->EnableWindow(TRUE);
	GetDlgItem(IDB_PANEL_TRANSFER_IN_MODEL)->EnableWindow(TRUE);
	GetDlgItem(IDB_TAB_MOUNTER_FIX2)->EnableWindow(FALSE);
	GetDlgItem(IDB_TAB_CARRIER_FIX2)->EnableWindow(FALSE);
	GetDlgItem(IDB_TAB_MOUNTER_MODEL2)->EnableWindow(FALSE);
	GetDlgItem(IDB_TAB_CARRIER_MODEL2)->EnableWindow(FALSE);
	GetDlgItem(IDB_RTAB_CARRIER_CAM_EXPAND)->EnableWindow(FALSE);
	GetDlgItem(IDB_REAR_WORK_SCHEDULE)->EnableWindow(FALSE);
	GetDlgItem(IDB_REAR_WORK_SCHEDULE2)->EnableWindow(FALSE);

#ifndef DEF_USE_TURN_HANDLER
	GetDlgItem(IDB_TURN_HANDLER_MODEL)->EnableWindow(FALSE);
	GetDlgItem(IDB_TAB_CIDB_TURN_HANDLER_FIXARRIER_FIX3)->EnableWindow(FALSE);
#endif
#ifdef DEF_SOURCE_SYSTEM
#	ifndef DEF_USE_TRANSFER_CENTER
	GetDlgItem(IDB_PANEL_TRANSFER_CENTER_FIX)->EnableWindow(FALSE);
	GetDlgItem(IDB_PANEL_TRANSFER_CENTER_MODEL)->EnableWindow(FALSE);
#	endif
#endif
	m_bInitialized = TRUE;
}

void CTeachMainForm::OnPanelAlignerFix() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50300,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachPanelAlignerDlg dlg(DEF_FIX_MODE);
	dlg.DoModal();
}

void CTeachMainForm::OnPanelAlignerModel() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50301,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachPanelAlignerDlg dlg(DEF_MODEL_MODE);
	dlg.DoModal();
}

void CTeachMainForm::OnPanelAlignerOffset() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50302,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CTeachPanelAlignerOffsetDlg dlg(0);
	dlg.DoModal();
}

void CTeachMainForm::OnTabCarrierFix() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50303,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CTeachCarrierDlg dlg(DEF_FIX_MODE, DEF_FRONT_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnClickTabCarrierFix2() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50304,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CTeachCarrierDlg dlg(DEF_FIX_MODE, DEF_REAR_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnTabCarrierModel() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50305,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CTeachCarrierDlg dlg(DEF_MODEL_MODE, DEF_FRONT_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnClickTabCarrierModel2() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50306,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CTeachCarrierDlg dlg(DEF_MODEL_MODE, DEF_REAR_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnTabFeederPressFix() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50307,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CTeachPressDlg dlg(DEF_FIX_MODE, DEF_FRONT_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnTabFeederSupplierFix() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50308,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CTeachMoldPinDlg dlg(DEF_FIX_MODE, DEF_FRONT_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnClickTabFeederPressFix2() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50309,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CTeachPressDlg dlg(DEF_FIX_MODE, DEF_REAR_GROUP);
	dlg.DoModal();	
}

void CTeachMainForm::OnTabFeederBadTabModel() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50310,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CTeachBadTabDetectorDlg dlg(DEF_MODEL_MODE, DEF_FRONT_GROUP);
	dlg.DoModal();	
}

void CTeachMainForm::OnClickTabFeederBadtabModel2() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50311,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachBadTabDetectorDlg dlg(DEF_MODEL_MODE, DEF_REAR_GROUP);
	dlg.DoModal();	
}

void CTeachMainForm::OnTabMounterFix() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50312,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachTabMounterDlg dlg(DEF_FIX_MODE, DEF_FRONT_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnClickTabMounterFix2() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50313,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachTabMounterDlg dlg(DEF_FIX_MODE, DEF_REAR_GROUP);
	dlg.DoModal();	
}

void CTeachMainForm::OnTabMounterModel() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50314,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachTabMounterDlg dlg(DEF_MODEL_MODE, DEF_FRONT_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnClickTabMounterModel2() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50315,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachTabMounterDlg dlg(DEF_MODEL_MODE, DEF_REAR_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnClickInspectCamFix1() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50320,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachCameraCarrierDlg dlg(DEF_FIX_MODE, DEF_FRONT_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnClickInspectCamFix2() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50321,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachCameraCarrierDlg dlg(DEF_FIX_MODE, DEF_REAR_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnClickInspectCamModel1() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50322,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachCameraCarrierDlg dlg(DEF_MODEL_MODE, DEF_FRONT_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnInspectCamModel2() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50323,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachCameraCarrierDlg dlg(DEF_MODEL_MODE, DEF_REAR_GROUP);
	dlg.DoModal();	
}

void CTeachMainForm::OnClickPanelCamFix() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50324,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachCameraCarrierDlg dlg(DEF_FIX_MODE, DEF_INSPECTION_CAMERA_NONE_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnClickPanelCamModel() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50325,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachCameraCarrierDlg dlg(DEF_MODEL_MODE, DEF_INSPECTION_CAMERA_NONE_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnClickModelChangeFix() 
{
	//Setup시외 수정 불가
	if (MOlbSystemPre.GetSystemData()->m_bSafetySensor == TRUE)
	{
		//모델 체인지 축 고정 좌표는 수정할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 50333, _T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50326,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachModelChangeDlg dlg(DEF_FRONT_GROUP, DEF_FIX_MODE);
	dlg.DoModal();
}

void CTeachMainForm::OnClickModelchange2Fix() 
{

	//Setup시외 수정 불가
	if (MOlbSystemPre.GetSystemData()->m_bSafetySensor == TRUE)
	{
		//모델 체인지 축 고정 좌표는 수정할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 50333, _T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50326,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachModelChangeDlg dlg(DEF_REAR_GROUP, DEF_FIX_MODE);
	dlg.DoModal();	
}

void CTeachMainForm::OnClickModelChangeModel()
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50327,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachModelChangeDlg dlg(DEF_FRONT_GROUP, DEF_MODEL_MODE);
	dlg.DoModal();
}

void CTeachMainForm::OnClickModelchange2Model() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50327,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CTeachModelChangeDlg dlg(DEF_REAR_GROUP, DEF_MODEL_MODE);
	dlg.DoModal();	
}

void CTeachMainForm::OnTabFeederSupplierFix2() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
//		이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50328,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CTeachMoldPinDlg dlg(DEF_FIX_MODE, DEF_REAR_GROUP);
	dlg.DoModal();	
}


LRESULT CTeachMainForm::OnDisplayLanguage(UINT wParam, LPARAM lParam)
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	//고정좌표 Teaching
	SetTitleLT(&m_lblFixedGroup, MAIN_TEACH, _T("TeachMainForm_1"));
	//Offset Teaching
	SetTitleLT(&m_lblOffsetGroup, MAIN_TEACH, _T("TeachMainForm_2"));
	//Offset 직접 입력
	SetTitleLT(&m_lblOffsetDirectInputGroup, MAIN_TEACH, _T("TeachMainForm_3"));
	return 0;

}

//////////////////////////////////////////////
//2009.07.13 CYJ
void CTeachMainForm::OnClickPanelPressGetTOffset() 
{
#if FALSE	//KKY

	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50327,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	
    ///////////////// TabMounterLoadTabIC //////////////////// 

	CString strMsg;

//	TabIC 를 TabMounter 로 공급하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30309, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	EWorkingSide eWorkSide = WORKING_SIDE_UNKNOWN;
	int iWorkGroupNo = DEF_FRONT_GROUP;
	int iTabNo = -1;

	int iResult = getWorkOption(&eWorkSide, &iWorkGroupNo, &iTabNo);
	if (iResult)
		return;

	int iCarrierVacNo = -1;
	int iWorkTabMounterNo = -1;
	if (-1 != iTabNo)
	{
		iWorkTabMounterNo = m_plnkProcessData->GetWorkTabMounterNo(eWorkSide, iTabNo);
		if (DEF_NONE_TABMOUNTER == iWorkTabMounterNo)
		{
//		선택된 Tab 번호는 작업하지 않는 Tab 번호임.
			MyMessageBox(MY_MESSAGE_BOX, 30310, _T("Tab No select error"), M_ICONERROR);
			return;
		}
		
		iCarrierVacNo = iWorkTabMounterNo - iWorkGroupNo * DEF_MAX_WORKER_PER_GROUP;
	}

//	일련 동작 중 입니다.\n잠시만 기다려 주세요...
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_13"), &strMsg);

	CMyProgressWnd ProgWnd(NULL, _T("TabMounter Panel Press Get T Offset..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	iResult = MOlbSystemPre.LoadTabCarrierTabIC(iWorkGroupNo, iCarrierVacNo);
	if (iResult)
	{
		ProgWnd.DestroyWindow();

		ASSERT(NOT_USE_WORKSIDE != iResult);
		if (USER_STOP == iResult)
		{
//			사용자 요청에 의해 작업을 중단함.
			MyMessageBox(MY_MESSAGE_BOX, 30311, _T("Information"), M_ICONINFORMATION);
			return;
		}

// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = MOlbSystemPre.LoadTabMounterTabIC(iWorkGroupNo, iWorkTabMounterNo);
	if (iResult)
	{
		ProgWnd.DestroyWindow();

		ASSERT(NOT_USE_WORKSIDE != iResult);
		if (USER_STOP == iResult)
		{
//			사용자 요청에 의해 작업을 중단함.
			MyMessageBox(MY_MESSAGE_BOX, 30312, _T("Information"), M_ICONINFORMATION);
			return;
		}

// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	TabMounter TabIC Load 작업 완료

    /////////////// PrealignTabIC ///////////////////////

	eWorkSide = WORKING_SIDE_UNKNOWN;
	iWorkGroupNo = DEF_FRONT_GROUP;
	iTabNo = -1;

	iResult = getWorkOption(&eWorkSide, &iWorkGroupNo, &iTabNo);
	if (iResult)
		return;

	iCarrierVacNo = -1;
	iWorkTabMounterNo = -1;
	if (-1 != iTabNo)
	{
		iWorkTabMounterNo = m_plnkProcessData->GetWorkTabMounterNo(eWorkSide, iTabNo);
		if (DEF_NONE_TABMOUNTER == iWorkTabMounterNo)
		{
//			선택된 Tab 번호는 작업하지 않는 Tab 번호임.
			MyMessageBox(MY_MESSAGE_BOX, 30316, _T("Tab No select error"), M_ICONERROR);
			return;
		}
		
		iCarrierVacNo = iWorkTabMounterNo - iWorkGroupNo * DEF_MAX_WORKER_PER_GROUP;
	}

	if (-1 == iWorkTabMounterNo
		|| FALSE == MOlbSystemPre.GetTabMounterComponent(iWorkTabMounterNo)->IsAbsorbTabIC())
	{
		iResult = MOlbSystemPre.LoadTabCarrierTabIC(iWorkGroupNo, iCarrierVacNo);
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			
			ASSERT(NOT_USE_WORKSIDE != iResult);
			if (USER_STOP == iResult)
			{
//				사용자 요청에 의해 작업을 중단함.
				MyMessageBox(MY_MESSAGE_BOX, 30317, _T("Information"), M_ICONINFORMATION);
				return;
			}
			
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		
		iResult = MOlbSystemPre.LoadTabMounterTabIC(iWorkGroupNo, iWorkTabMounterNo);
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			
			ASSERT(NOT_USE_WORKSIDE != iResult);
			if (USER_STOP == iResult)
			{
//				사용자 요청에 의해 작업을 중단함.
				MyMessageBox(MY_MESSAGE_BOX, 30318, _T("Information"), M_ICONINFORMATION);
				return;
			}
			
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}

	iResult = MOlbSystemPre.PrealignTabIC(iWorkGroupNo, iTabNo, FALSE, TRUE);
	if (iResult)
	{
		ProgWnd.DestroyWindow();

		ASSERT(NOT_USE_WORKSIDE != iResult);
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return;

// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.DestroyWindow();

//	TabMounter TabIC Pre Align 작업 완료

    /////////////////////// InspectTabIC ////////////////////////////

	eWorkSide = WORKING_SIDE_UNKNOWN;
	iWorkGroupNo = DEF_FRONT_GROUP;
	iTabNo = -1;

	iResult = getWorkOption(&eWorkSide, &iWorkGroupNo, &iTabNo);
	if (iResult)
		return;

	// Panel 준비 상태 확인.
	MPanelAligner* plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();
//	Panel Aligner 가 Panel 을 흡착하고 있지 않습니다.
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_17"), &strMsg);

	if (FALSE == plnkPanelAligner->IsPanelAbsorbed())
	{
		MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Panel 의 현재 위치를 유지하겠습니까?\r\n[아니오]를 선택하면 Panel 을 Inspection 위치로 이동합니다.
	iResult = MyMessageBox(MY_MESSAGE_BOX, 30346, _T("confirm"), M_ICONQUESTION|M_YESNO);
	if (IDNO == iResult)
	{
		if (FALSE == plnkPanelAligner->IsAligned())
		{
//			Panel Align이 수행되지 않았습니다.
			MyMessageBox(MY_MESSAGE_BOX, 30347, _T("Error"), M_ICONERROR);
			return;
		}
		
		// 확인 필요 : 항상 Xu Pos 로 계속 사용할 것인가?		
		if (FALSE == plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_INSPECT_XU_POS))
		{
			MCtrlPanelAligner* plnkCtrlPanelAligner = MOlbSystemPre.GetCtrlPanelAlignerComponent();
			iResult = plnkCtrlPanelAligner->MovetoInspectPos(DEF_PANEL_ALIGNER_INSPECT_XU_POS, FALSE);
			if (iResult)
			{
// jdy sesl				MyMessageBox(iResult, _T("Error"), M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
		}
	}

	iResult = operateTabICInspection(eWorkSide, iWorkGroupNo, iTabNo, DEF_BEFORE_MOUNT);
	if (iResult)
	{
		if (ERR_TEACH_MAINFORM_SKIP_ERROR_DISPLAY == iResult)
			return;
		
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	TabMounter Inspection 작업 완료

	//	TabMounter Inspection 검사 Offset Panel Align Cam2 Panel Align Offset Data 반영
	//  반영 전 질문 확인
	//m_plnkProcessData->GetInspectionOffsetStoragePointer(eWorkSide, iTempTabNo)->dY;

	int iStartMounterNo = -1;
	int iEndMounterNo = -1;
	int iGap = 0;
	
	if (DEF_FRONT_GROUP == iWorkGroupNo)
	{
		iStartMounterNo = DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[iWorkGroupNo];
		iEndMounterNo = DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[iWorkGroupNo] - 1;
		iGap = -1;
	}
	else // if (DEF_REAR_GROUP == iWorkGroupNo)
	{
		iStartMounterNo = DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[iWorkGroupNo];
		iEndMounterNo = DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[iWorkGroupNo] + 1;
		iGap = 1;
	}

	int iTempTabNo = -1;
	double dTempPanelAlignerCamYOffset[2];
	MPos_XYT	m_rgXYTFixedPosData[DEF_PANEL_ALIGNER_TEACH_MAX_POS];
	MPos_XYT	m_rgXYTOffsetPosData[DEF_PANEL_ALIGNER_TEACH_MAX_POS];
	MPos_XYT	m_rgXYTModelPosData[DEF_PANEL_ALIGNER_TEACH_MAX_POS];

	MOlbSystemPre.GetPanelAlignerComponent()->GetXYTPosParameter(m_rgXYTFixedPosData, m_rgXYTOffsetPosData, m_rgXYTModelPosData);

	dTempPanelAlignerCamYOffset[0] = m_plnkProcessData->GetTabPrealignOffsetStoragePointer(eWorkSide, iStartMounterNo)->dY;

	dTempPanelAlignerCamYOffset[1] = m_plnkProcessData->GetTabPrealignOffsetStoragePointer(eWorkSide, iEndMounterNo-1)->dY;

	double dOriginDistBetweenAligners = m_plnkSystemData->m_dOriginDistBetweenAligners;
	double dFiduMarkDist = m_plnkPanelData->m_dDistance_FiduMark_Xu;
	double dModelChangeOriginDist = m_plnkSystemData->m_dModelChangeOriginDist;
	double dModelChangeModelPosOffset = MOlbSystemPre.GetModelChangeComponent()->GetModelPosOffset();

	double dDistBetweenAligners = dOriginDistBetweenAligners + dFiduMarkDist - dModelChangeOriginDist + dModelChangeModelPosOffset;

	double dDegreeT = -RAD2DEG(asin((dTempPanelAlignerCamYOffset[1] - dTempPanelAlignerCamYOffset[0]) / dDistBetweenAligners));

	m_rgXYTFixedPosData[DEF_PANEL_ALIGNER_FIDU_MARK_XU_POS].dT = dDegreeT + m_rgXYTFixedPosData[DEF_PANEL_ALIGNER_FIDU_MARK_XU_POS].dT;

	MOlbSystemPre.GetPanelAlignerComponent()->SaveXYTFixedPosParameter(m_rgXYTFixedPosData);

	// 작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30350, _T("confirm"), M_ICONINFORMATION);

#endif	//#if FALSE
}

//2009.07.13 CYJ
void CTeachMainForm::OnClickTabCarrierGetLoadOffset() 
{
#if FALSE	//KKY

	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
//		현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,50327,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

    ///////////////// TabMounterLoadTabIC //////////////////// 

	CString strMsg;

//Message_30386=Tab Carrier Load Offset 자동 얻기를 실행하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30386, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	EWorkingSide eWorkSide = WORKING_SIDE_UNKNOWN;
	int iWorkGroupNo = DEF_FRONT_GROUP;
	int iTabNo = -1;

	int iResult = getWorkOption(&eWorkSide, &iWorkGroupNo, &iTabNo);
	if (iResult)
		return;

	int iCarrierVacNo = -1;
	int iWorkTabMounterNo = -1;
	if (-1 != iTabNo)
	{
		iWorkTabMounterNo = m_plnkProcessData->GetWorkTabMounterNo(eWorkSide, iTabNo);
		if (DEF_NONE_TABMOUNTER == iWorkTabMounterNo)
		{
//		선택된 Tab 번호는 작업하지 않는 Tab 번호임.
			MyMessageBox(MY_MESSAGE_BOX, 30310, _T("Tab No select error"), M_ICONERROR);
			return;
		}
		
		iCarrierVacNo = iWorkTabMounterNo - iWorkGroupNo * DEF_MAX_WORKER_PER_GROUP;
	}

//	일련 동작 중 입니다.\n잠시만 기다려 주세요...
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_13"), &strMsg);

	CMyProgressWnd ProgWnd(NULL, _T("TabMounter Panel Press Get T Offset..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	iResult = MOlbSystemPre.LoadTabCarrierTabIC(iWorkGroupNo, iCarrierVacNo);
	if (iResult)
	{
		ProgWnd.DestroyWindow();

		ASSERT(NOT_USE_WORKSIDE != iResult);
		if (USER_STOP == iResult)
		{
//			사용자 요청에 의해 작업을 중단함.
			MyMessageBox(MY_MESSAGE_BOX, 30311, _T("Information"), M_ICONINFORMATION);
			return;
		}

// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = MOlbSystemPre.LoadTabMounterTabIC(iWorkGroupNo, iWorkTabMounterNo);
	if (iResult)
	{
		ProgWnd.DestroyWindow();

		ASSERT(NOT_USE_WORKSIDE != iResult);
		if (USER_STOP == iResult)
		{
//			사용자 요청에 의해 작업을 중단함.
			MyMessageBox(MY_MESSAGE_BOX, 30312, _T("Information"), M_ICONINFORMATION);
			return;
		}

// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	TabMounter TabIC Load 작업 완료

    /////////////// get X offset pre align cam ///////////////////////

	eWorkSide = WORKING_SIDE_UNKNOWN;
	iWorkGroupNo = DEF_FRONT_GROUP;
	iTabNo = -1;

	iResult = getWorkOption(&eWorkSide, &iWorkGroupNo, &iTabNo);
	if (iResult)
		return;

	iCarrierVacNo = -1;
	iWorkTabMounterNo = -1;
	if (-1 != iTabNo)
	{
		iWorkTabMounterNo = m_plnkProcessData->GetWorkTabMounterNo(eWorkSide, iTabNo);
		if (DEF_NONE_TABMOUNTER == iWorkTabMounterNo)
		{
//			선택된 Tab 번호는 작업하지 않는 Tab 번호임.
			MyMessageBox(MY_MESSAGE_BOX, 30316, _T("Tab No select error"), M_ICONERROR);
			return;
		}
		
		iCarrierVacNo = iWorkTabMounterNo - iWorkGroupNo * DEF_MAX_WORKER_PER_GROUP;
	}

/*	if (-1 == iWorkTabMounterNo
		|| FALSE == MOlbSystemPre.GetTabMounterComponent(iWorkTabMounterNo)->IsAbsorbTabIC())
	{
		iResult = MOlbSystemPre.LoadTabCarrierTabIC(iWorkGroupNo, iCarrierVacNo);
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			
			ASSERT(NOT_USE_WORKSIDE != iResult);
			if (USER_STOP == iResult)
			{
//				사용자 요청에 의해 작업을 중단함.
				MyMessageBox(MY_MESSAGE_BOX, 30317, _T("Information"), M_ICONINFORMATION);
				return;
			}
			
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		
		iResult = MOlbSystemPre.LoadTabMounterTabIC(iWorkGroupNo, iWorkTabMounterNo);
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			
			ASSERT(NOT_USE_WORKSIDE != iResult);
			if (USER_STOP == iResult)
			{
//				사용자 요청에 의해 작업을 중단함.
				MyMessageBox(MY_MESSAGE_BOX, 30318, _T("Information"), M_ICONINFORMATION);
				return;
			}
			
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}
*/
	iResult = MOlbSystemPre.GetXOffsetPrealignTabIC(iWorkGroupNo, iTabNo);
	if (iResult)
	{
		ProgWnd.DestroyWindow();

		ASSERT(NOT_USE_WORKSIDE != iResult);
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return;

// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	get X Offset prealign cam 작업 완료

	//	TabMounter Prealign 검사 X Offset Carrier Load Offset Data 반영
	//  반영 전 질문 확인
//Message_30387=Tab Carrier Load Offset 자동 얻기 값을 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30387, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
	{
		m_plnkProcessData->Initialize();
		return;
	}

	int iStartMounterNo = -1;
	int iEndMounterNo = -1;
	int iGap = 0;
	
	if (DEF_FRONT_GROUP == iWorkGroupNo)
	{
		iStartMounterNo = DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[iWorkGroupNo];
		iEndMounterNo = DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[iWorkGroupNo] - 1;
		iGap = -1;
	}
	else // if (DEF_REAR_GROUP == iWorkGroupNo)
	{
		iStartMounterNo = DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[iWorkGroupNo];
		iEndMounterNo = DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[iWorkGroupNo] + 1;
		iGap = 1;
	}

	int iTempTabNo = -1;
	double dTempTabCarrierLoadXOffset = 0.0;
	MPos_XY	m_rgdFixedPosData[DEF_TABCARRIER_MAX_POSITION];
	MPos_XY	m_rgdModelPosData[DEF_TABCARRIER_MAX_POSITION];
	
	MPos_XY	m_rgdOffsetPosOriginData[DEF_TABCARRIER_MAX_POSITION];
	MPos_XY	m_rgdOffsetPosData[DEF_TABCARRIER_MAX_POSITION];


	// 전체 Position
	for (int i = 0; i < DEF_TABCARRIER_MAX_POSITION; i++)
	{
		m_rgdFixedPosData[i].Init(0.0,0.0);
		m_rgdModelPosData[i].Init(0.0,0.0);

		m_rgdOffsetPosOriginData[i].Init(0.0,0.0);
		m_rgdOffsetPosData[i].Init(0.0,0.0);
	}

	MOlbSystemPre.GetTabCarrierComponent(DEF_FRONT_GROUP)->GetXYPosParameter(m_rgdFixedPosData, m_rgdOffsetPosOriginData, m_rgdModelPosData);

	for (int i = iStartMounterNo; i != iEndMounterNo; i += iGap)
	{
		iTempTabNo = m_plnkProcessData->GetMounterDefaultTabNo(eWorkSide, i);

		dTempTabCarrierLoadXOffset = m_plnkProcessData->GetTabPrealignOffsetStoragePointer(eWorkSide, iTempTabNo)->dX;

		// index 0,1 은 Unload, Turn
		m_rgdOffsetPosData[i+DEF_TABCARRIER_LOAD_POS1].dY = m_rgdOffsetPosOriginData[i+DEF_TABCARRIER_LOAD_POS1].dY - dTempTabCarrierLoadXOffset;

	}

	MOlbSystemPre.GetTabCarrierComponent(DEF_FRONT_GROUP)->SaveXYOffsetPosParameter(m_rgdOffsetPosData);

	m_plnkProcessData->Initialize();

	ProgWnd.DestroyWindow();

#endif	//#if FALSE

}

int CTeachMainForm::getWorkOption(EWorkingSide* peWorkSide, int* piGroupNo, int* piTab)		//@--->미사용 함수
{
	int iResult = ERR_TEACH_MAINFORM_SUCCESS;
	*piGroupNo = DEF_FRONT_GROUP;

	if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
//		작업변을 선택하세요.
		iResult = MyMessageBox(MY_MESSAGE_BOX, 30300, _T("Using work side selection"), M_ICONQUESTION|M_YESNO, _T("Yl"), _T("Yr"));
		EWorkingSide eWorkSide = (IDYES == iResult) ? WORKING_SIDE_Y_LEFT : WORKING_SIDE_Y_RIGHT;

#ifndef DEF_MIRROR_
		if (WORKING_SIDE_Y_RIGHT == eWorkSide)
#else
		if (WORKING_SIDE_Y_LEFT == eWorkSide)
#endif
		{
			*piGroupNo = DEF_REAR_GROUP;
		}
	}

	*peWorkSide = m_plnkProcessData->GetCurrentWorkingSide(*piGroupNo);
	if (WORKING_SIDE_UNKNOWN == *peWorkSide)
	{
//		작업 수행 불가합니다. [해당 작업변 사용하지 않음.]
		MyMessageBox(MY_MESSAGE_BOX, 30301, _T("Error"), M_ICONERROR);
		return NOT_USE_WORKSIDE;
	}

	// Teach Main Form에서는 개별 Tab 선택 불필요
/*	CString strMsg;
	//	TabIC Loading 방식을 결정하세요.
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_9"), &strMsg);
	CString strBtn1;
	//	전체 Loading
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_10"), &strBtn1);
	CString strBtn2;
	//	Tab 번호 선택
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_11"), &strBtn2);
	
	iResult = MyMessageBox(strMsg, _T("TabIC Loading form select"), M_ICONQUESTION|M_YESNO, strBtn1.GetBuffer(strBtn1.GetLength()), strBtn2.GetBuffer(strBtn2.GetLength())); // 수정할 필요 없는 메시지박스_영락


	if (IDNO == iResult)
	{
		WCHAR cAfterVal[10];
		memset(cAfterVal, 0, sizeof(cAfterVal));

		if (FALSE == GetNumberBox(cAfterVal, 10, _T("1")))
			return ERR_MANUAL_MAINFORM_FAILURE;
		
		int iTabNo = _wtoi(cAfterVal) - 1;

		if (iTabNo < 0 || iTabNo >= m_plnkProcessData->GetMaxTabQty(*peWorkSide))
		{
//			Tab 번호 입력 오류
			MyMessageBox(MY_MESSAGE_BOX, 30302, _T("Input Error"), M_ICONERROR);
			return ERR_MANUAL_MAINFORM_FAILURE;
		}

		*piTab = iTabNo;
	}
	else
*/
		*piTab = -1;

	return ERR_TEACH_MAINFORM_SUCCESS;
}

int CTeachMainForm::operateTabICInspection(EWorkingSide eWorkSide, int iGroupNo, int iWorkTabNo, int iTargetStatus, MPos_XY* prgAlignOffset)
{
	return ERR_TEACH_MAINFORM_SUCCESS;
}

#if FALSE
int CTeachMainForm::moveGateInspectionCameraToSafePosition(EWorkingSide eWorkSide, int iGroupNo)
{
#ifdef DEF_GATE_SYSTEM
	// 주의 : 이 함수는 Gate 변 작업 Tab 이 한 개인 경우는 대응 못함.
	
	int i = 0;
	int iResult = SUCCESS;
	MCameraCarrier* rgCamCarrier[DEF_MAX_INSPECTION_CAM_PER_GROUP] = { NULL, NULL, NULL };
	int rgiWorkTabNo[DEF_MAX_INSPECTION_CAM_PER_GROUP] = { -1, -1, -1 };
	double rgdTargetPos[DEF_MAX_INSPECTION_CAM_PER_GROUP] = { -9999.0, -9999.0, -9999.0 };

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_PER_GROUP; i++)
	{
		rgCamCarrier[i] = MOlbSystemPre.GetCameraCarrierComponent(DEF_CAMERACARRIER3 + i + iGroupNo * DEF_MAX_INSPECTION_CAM_PER_GROUP);

		rgiWorkTabNo[i] = m_plnkProcessData->GetInspectorDefaultTabNo(eWorkSide, i + iGroupNo * DEF_MAX_INSPECTION_CAM_PER_GROUP);

		if (-1 != rgdTargetPos[i])
			rgdTargetPos[i] = rgCamCarrier[i]->GetTargetPos(DEF_CAMERACARRIER_TAB_MARK_POS, rgiWorkTabNo[i]);
		else
		{
			ASSERT((i + 1 + iGroupNo * DEF_MAX_INSPECTION_CAM_PER_GROUP) % DEF_MAX_INSPECTION_CAM_PER_GROUP == 0);
			rgdTargetPos[i] = rgCamCarrier[i]->GetMotionComponent()->GetPositiveSWLimit() - 5.0;
		}
	}

	// 이동 순서 주의
	int rgiWorkOrder[DEF_MAX_INSPECTION_CAM_PER_GROUP] = { 0, 2, 1 };

	int j = 0;
	for (int j = 0; j < DEF_MAX_INSPECTION_CAM_PER_GROUP; j++)
	{
		iResult = rgCamCarrier[rgiWorkOrder[j]]->MovePos(rgdTargetPos[rgiWorkOrder[j]], -1);
		if (iResult)
			return iResult;
	}
#endif
	return SUCCESS;
}
#endif

void CTeachMainForm::OnClickPanelTransferCenterFix() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//Operator,Engineer 권한으로는 Stage 고정좌표를 수정할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 50300, _T("Login Authority Information"), M_ICONINFORMATION);

		return;
	}
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TRANSFER_CENTER
	CTeachTransferDlg dlg(DEF_FIX_MODE, DEF_PANEL_TRANSFER_CENTER);
	dlg.DoModal();
#	endif
#else
	CTeachModelChangeDlg dlg(DEF_FRONT_GROUP, DEF_FIX_MODE);
	dlg.DoModal();
#endif
}

void CTeachMainForm::OnClickPanelTransferInFix() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//Operator,Engineer 권한으로는 Stage 고정좌표를 수정할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 50300, _T("Login Authority Information"), M_ICONINFORMATION);

		return;
	}
	
	CTeachTransferDlg dlg(DEF_FIX_MODE, DEF_PANEL_TRANSFER_IN);
	dlg.DoModal();	
}

void CTeachMainForm::OnClickPanelTransferOutFix() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//Operator,Engineer 권한으로는 Stage 고정좌표를 수정할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 50300, _T("Login Authority Information"), M_ICONINFORMATION);

		return;
	}
	
	CTeachTransferDlg dlg(DEF_FIX_MODE, DEF_PANEL_TRANSFER_OUT);
	dlg.DoModal();
	
}

void CTeachMainForm::OnClickPanelTransferCenterModel() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
	  //Operator 권한으로는 Data 를 수정할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 50301, _T("Login Authority Information"), M_ICONINFORMATION);

		return;
	}
#ifdef DEF_SOURCE_SYSTEM	
#	ifdef DEF_USE_TRANSFER_CENTER
	CTeachTransferDlg dlg(DEF_MODEL_MODE, DEF_PANEL_TRANSFER_CENTER);
	dlg.DoModal();
#	endif
#else
	CTeachModelChangeDlg dlg(DEF_FRONT_GROUP, DEF_MODEL_MODE);
	dlg.DoModal();
#endif
}

void CTeachMainForm::OnClickPanelTransferInModel() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
	  //Operator 권한으로는 Data 를 수정할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 50301, _T("Login Authority Information"), M_ICONINFORMATION);

		return;
	}
	CTeachTransferDlg dlg(DEF_MODEL_MODE, DEF_PANEL_TRANSFER_IN);
	dlg.DoModal();
}

void CTeachMainForm::OnClickPanelTransferOutModel() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
	  //Operator 권한으로는 Data 를 수정할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 50301, _T("Login Authority Information"), M_ICONINFORMATION);

		return;
	}
	
	CTeachTransferDlg dlg(DEF_MODEL_MODE, DEF_PANEL_TRANSFER_OUT);
	dlg.DoModal();
	
}

void CTeachMainForm::OnClickFrontWorkSchedule() 
{
	// TODO: Add your control notification handler code here
	CDlgViewWorkSchedule dlg(MOlbSystemPre.GetWorkSchedule(DEF_FRONT_GROUP));
	dlg.DoModal();
}

void CTeachMainForm::OnClickRearWorkSchedule() 
{
	// TODO: Add your control notification handler code here
	CDlgViewWorkSchedule dlg(MOlbSystemPre.GetWorkSchedule(DEF_REAR_GROUP));
	dlg.DoModal();	
}

void CTeachMainForm::OnClickFrontWorkSchedule2() 
{
	// TODO: Add your control notification handler code here
	CDlgViewWorkSchedule dlg(MOlbSystemPre.GetWorkScheduleSecond(DEF_FRONT_GROUP));
	dlg.DoModal();
}

void CTeachMainForm::OnClickRearWorkSchedule2() 
{
	// TODO: Add your control notification handler code here
	CDlgViewWorkSchedule dlg(MOlbSystemPre.GetWorkScheduleSecond(DEF_REAR_GROUP));
	dlg.DoModal();		
}

void CTeachMainForm::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFormView::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (bShow && m_bInitialized)
	{
//@#ifdef DEF_SOURCE_SYSTEM
//@		GetDlgItem(IDB_REAR_WORK_SCHEDULE)->EnableWindow(FALSE);
//@#else
		if (FALSE == MOlbSystemPre.GetWorkSchedule(DEF_FRONT_GROUP)->GetUseWorkSide())
		{
//			MOlbSystemPre.GetWorkSchedule(DEF_FRONT_GROUP)->
			GetDlgItem(IDB_FRONT_WORK_SCHEDULE)->EnableWindow(FALSE);
			GetDlgItem(IDB_FRONT_WORK_SCHEDULE2)->EnableWindow(FALSE);
		}
		else
		{
			//170720_KDH Work Schedule이 없을경우 들어가지 못하도록 수정 
			MListWorkTab ListWorkTab;
			MOlbSystemPre.GetWorkSchedule(DEF_FRONT_GROUP)->GetOrgWorkList(&ListWorkTab);
			if(ListWorkTab.GetTotalWorkTabCount() > 0)
				GetDlgItem(IDB_FRONT_WORK_SCHEDULE)->EnableWindow(TRUE);
			else
				GetDlgItem(IDB_FRONT_WORK_SCHEDULE)->EnableWindow(FALSE);
			
#ifdef DEF_SOURCE_SYSTEM
			MOlbSystemPre.GetWorkScheduleSecond(DEF_FRONT_GROUP)->GetOrgWorkList(&ListWorkTab);
			if(ListWorkTab.GetTotalWorkTabCount() > 0)
				GetDlgItem(IDB_FRONT_WORK_SCHEDULE2)->EnableWindow(TRUE);
			else
				GetDlgItem(IDB_FRONT_WORK_SCHEDULE2)->EnableWindow(FALSE);
#endif
			//________________________
		}

		if (FALSE == MOlbSystemPre.GetWorkSchedule(DEF_REAR_GROUP)->GetUseWorkSide())
		{
			GetDlgItem(IDB_REAR_WORK_SCHEDULE)->EnableWindow(FALSE);
			GetDlgItem(IDB_REAR_WORK_SCHEDULE2)->EnableWindow(FALSE);
		}
		else
		{
			//170720_KDH Work Schedule 사용하지 않을 경우 비활성화 추가
			MListWorkTab ListWorkTab;
			MOlbSystemPre.GetWorkSchedule(DEF_REAR_GROUP)->GetOrgWorkList(&ListWorkTab);
			if(ListWorkTab.GetTotalWorkTabCount() > 0)
				GetDlgItem(IDB_REAR_WORK_SCHEDULE)->EnableWindow(TRUE);
			else
				GetDlgItem(IDB_REAR_WORK_SCHEDULE)->EnableWindow(FALSE);
#ifdef DEF_SOURCE_SYSTEM
			MOlbSystemPre.GetWorkScheduleSecond(DEF_REAR_GROUP)->GetOrgWorkList(&ListWorkTab);
			if(ListWorkTab.GetTotalWorkTabCount() > 0)
				GetDlgItem(IDB_REAR_WORK_SCHEDULE2)->EnableWindow(TRUE);
			else
				GetDlgItem(IDB_REAR_WORK_SCHEDULE2)->EnableWindow(FALSE);
			//_______________________
#endif
		}

//@#endif
	}
}

void CTeachMainForm::OnClickTabCarrierCamExpand() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
	  //Operator 권한으로는 Data 를 수정할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 50301, _T("Login Authority Information"), M_ICONINFORMATION);
		
		return;
	}
	
	CTeachCarrierCameraExpandDlg dlg(DEF_MODEL_MODE, DEF_FRONT_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnClickRtabCarrierCamExpand() 
{
	// TODO: Add your control notification handler code here	
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
	  //Operator 권한으로는 Data 를 수정할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 50301, _T("Login Authority Information"), M_ICONINFORMATION);
		
		return;
	}
	
	CTeachCarrierCameraExpandDlg dlg(DEF_MODEL_MODE, DEF_REAR_GROUP);
	dlg.DoModal();
}

void CTeachMainForm::OnClickTabCidbTurnHandlerFixarrierFix3() 
{
	// TODO: Add your control notification handler code here
#ifdef DEF_USE_TURN_HANDLER
	CTeachHandlerDlg dlg(DEF_HANDLER_UNLOADING, DEF_FIX_MODE);
	dlg.DoModal();
#endif
}

void CTeachMainForm::OnClickTurnHandlerModel() 
{
	// TODO: Add your control notification handler code here
#ifdef DEF_USE_TURN_HANDLER
	CTeachHandlerDlg dlg(DEF_HANDLER_UNLOADING, DEF_MODEL_MODE);
	dlg.DoModal();
#endif
}
