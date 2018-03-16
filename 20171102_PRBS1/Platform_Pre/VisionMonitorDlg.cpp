// VisionMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Common.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include "VisionMonitorDlg.h"
#include "VisionSelectMarkDlg.h"
#include "VisionOptionDlg.h"
#include "VisionJogKeyDlg.h"
#include "IVision.h"
#include "VisionSearchAreaDlg.h"
#include "MVisionCalibration.h"
#include "CommonGetValueDlg.h"
#include "MTabCarrier.h"
#include "MCameraCarrier.h"
#include <math.h>
#include "atlconv.h"
#include "MLCNet.h"

extern int g_iDisplayReferenceLine;
extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CVisionMonitorDlg dialog

CVisionMonitorDlg::CVisionMonitorDlg(IVision* plnkVision, EMarkType eMarkType, int iSelectedCamNo, CWnd* pParent/*=NULL*/)
 :  CDialog(CVisionMonitorDlg::IDD, pParent),
	m_plnkVision(plnkVision),
	m_iViewID(-1),
	m_eMarkType(eMarkType),
	m_iCurrentCamNo(iSelectedCamNo),
	m_iSelectedMarkNo(0),
	m_bIsOnLive(FALSE)
{
	//{{AFX_DATA_INIT(CVisionMonitorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_plnkVisonCalibration = MOlbSystemPre.GetVisionCalibrationComponent();

	//110214.Add_____
	CPoint cpImageSize = m_plnkVision->GetDistribution(m_iCurrentCamNo);
	m_ptMeasure[0].x = LONG(cpImageSize.x/2.0 + 0.5);
	m_ptMeasure[0].y = LONG(cpImageSize.y/2.0 + 0.5);
	m_ptMeasure[1].x = m_ptMeasure[0].x;
	m_ptMeasure[1].y = m_ptMeasure[0].y;
	m_bMeasureAngle = FALSE;
	//_______________
}


void CVisionMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisionMonitorDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDB_CAM_1, m_rgCtrlCamBtn[0]);
	DDX_Control(pDX, IDB_CAM_2, m_rgCtrlCamBtn[1]);
	DDX_Control(pDX, IDB_CAM_3, m_rgCtrlCamBtn[2]);
	DDX_Control(pDX, IDB_CAM_4, m_rgCtrlCamBtn[3]);
	DDX_Control(pDX, IDB_CAM_5, m_rgCtrlCamBtn[4]);
	DDX_Control(pDX, IDB_CAM_6, m_rgCtrlCamBtn[5]);
	DDX_Control(pDX, IDB_CAM_7, m_rgCtrlCamBtn[6]);
	DDX_Control(pDX, IDB_CAM_8, m_rgCtrlCamBtn[7]);
	DDX_Control(pDX, IDB_CAM_9, m_rgCtrlCamBtn[8]);
	DDX_Control(pDX, IDB_CAM_10, m_rgCtrlCamBtn[9]);
	DDX_Control(pDX, IDB_CAM_11, m_rgCtrlCamBtn[10]);
	DDX_Control(pDX, IDB_CAM_12, m_rgCtrlCamBtn[11]);
	DDX_Control(pDX, IDB_CAM_13, m_rgCtrlCamBtn[12]);
	DDX_Control(pDX, IDB_CAM_14, m_rgCtrlCamBtn[13]);
	DDX_Control(pDX, IDB_CAM_15, m_rgCtrlCamBtn[14]);
	DDX_Control(pDX, IDB_CAM_16, m_rgCtrlCamBtn[15]);
	DDX_Control(pDX, IDB_CAM_17, m_rgCtrlCamBtn[16]);
	DDX_Control(pDX, IDB_CAM_18, m_rgCtrlCamBtn[17]);
	DDX_Control(pDX, IDB_CAM_19, m_rgCtrlCamBtn[18]);
	DDX_Control(pDX, IDB_CAM_20, m_rgCtrlCamBtn[19]);
	DDX_Control(pDX, IDB_CAM_21, m_rgCtrlCamBtn[20]);
	DDX_Control(pDX, IDB_CAM_22, m_rgCtrlCamBtn[21]);
	DDX_Control(pDX, IDB_CAM_23, m_rgCtrlCamBtn[22]);
	DDX_Control(pDX, IDB_CAM_24, m_rgCtrlCamBtn[23]);
	
	DDX_Control(pDX, IDB_PATTERN, m_bDlgType[DEF_PM_MARK]);
	DDX_Control(pDX, IDB_BLOB, m_bDlgType[DEF_BLOB_MARK]);
	DDX_Control(pDX, IDB_CALIPER, m_bDlgType[DEF_CALIPER_MARK]);

	DDX_Control(pDX, IDB_DRAW_POINT, m_btnDrawPoint);
	DDX_Control(pDX, IDB_MEASURE_ANGLE, m_btnMeasureAngle);

}

BEGIN_MESSAGE_MAP(CVisionMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CVisionMonitorDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionMonitorDlg message handlers

BOOL CVisionMonitorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_KeyTimerID = SetTimer(9, 300, NULL);
	// TODO: Add extra initialization here
	switch (m_eMarkType)
	{
	case DEF_BLOB_MARK:
		m_bDlgType[DEF_BLOB_MARK].SetValue(TRUE);
		break;

	case DEF_CALIPER_MARK:
//		m_bDlgType[DEF_CALIPER_MARK].SetValue(TRUE);
		break;

	default:
		m_bDlgType[DEF_PM_MARK].SetValue(TRUE);
		break;
	}
	
	CWnd* pWnd = GetDlgItem(IDC_MONITOR);

	RECT rect;
	pWnd->GetWindowRect(&rect);
	CRect rectView(rect.left, rect.top, rect.right, rect.bottom);
	ScreenToClient(&rectView);

	m_iViewID = m_plnkVision->CreateLocalView(&rectView, this);

	CString strCameraName;
	CString strCameraNo;
	int iMaxCameraNo = m_plnkVision->GetMaxUseCameraNo();
 	ASSERT(iMaxCameraNo <= DEF_DLG_VISION_MONITIOR_MAX_CAM_BTN);
	
	//110214.Add_____
	m_btnMeasureAngle.SetValue(0);
	m_btnDrawPoint.EnableWindow(FALSE);
	//_______________

	int iDigitIndex = -1;
	WCHAR szDigit[] = _T("0123456789");

	for (int i = 0 ; i < DEF_DLG_VISION_MONITIOR_MAX_CAM_BTN ; i++)
	{
		if (i < DEF_CAMERA_19 
			&& i != DEF_CAMERA_5
			&& i != DEF_CAMERA_6
			&& i != DEF_CAMERA_7
			&& i != DEF_CAMERA_8)
		{
			m_rgCtrlCamBtn[i].EnableWindow(TRUE);
			strCameraName=m_plnkVision->GetCameraName(i).c_str();
					
			iDigitIndex = -1;
			for (int j = 0; j < 10; j++)
			{
				iDigitIndex = strCameraName.Find(szDigit[j]);
				if (-1 != iDigitIndex)
					break;
			}

			if (-1 != iDigitIndex)
				strCameraName.Insert(iDigitIndex, _T("\r\n"));

			//101214 Modify,, 진승현 D 요청 사항..
			strCameraNo.Format(_T("[Cam%d]\r\n"), i + 1);
// 			if(i<4)
// 				strCameraNo.Format(_T("[Cam%d-%d]\r\n"), i/2 + 1, i%2 + 1);
// 			else
// 				strCameraNo.Format(_T("[Cam%d-%d]\r\n"), i/2 - 1, i%2 + 1);
// 			//____________________________________________________________*/
			strCameraName = strCameraNo + strCameraName;

			m_rgCtrlCamBtn[i].SetCaption(strCameraName.GetBuffer(strCameraName.GetLength()));
		}
		else
			m_rgCtrlCamBtn[i].EnableWindow(FALSE);

	}

	m_rgCtrlCamBtn[m_iCurrentCamNo].SetValue(TRUE);
	m_bOnStillContinue = TRUE;
	m_bIsOnLive = TRUE;
	int iStartCamNo = m_iCurrentCamNo;
	m_iCurrentCamNo = -1;
	selectCamera(iStartCamNo);
	
	//SJ_YYK 110421 Add...Ellas Align Check 인데..
	//Vision 볼때마다 보고 해야 하는지는 추후 수정 ..
	MOlbSystemPre.GetLCNetComponent()->EqSpecialStateReport(eAlignCheck);
	
	MOlbSystemPre.LightOn(DEF_MAX_CAMERA_NO); //171004 JSH
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(SUB_DLG, _T("VisionMonitorDlg_1"), &szTextTemp);	
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionMonitorDlg_2"), &szTextTemp);	
	//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionMonitorDlg_3"), &szTextTemp);	
	//Mark@@보기
	GetDlgItem(IDB_SHOW_MARK)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionMonitorDlg_4"), &szTextTemp);	
	//Mark@@삭제
	GetDlgItem(IDB_DELETE_MARK)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionMonitorDlg_5"), &szTextTemp);	
	//Mark@@생성
	GetDlgItem(IDB_MAKE_MARK)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionMonitorDlg_6"), &szTextTemp);	
	//실시간@@영상
	GetDlgItem(IDB_LIVE_IMAGE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionMonitorDlg_7"), &szTextTemp);	
	//정지@@영상
	GetDlgItem(IDB_STILL_IMAGE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionMonitorDlg_8"), &szTextTemp);	
	//영역@@검색
	GetDlgItem(IDB_SEARCH_AREA)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionMonitorDlg_9"), &szTextTemp);	
	//영상@@불러오기
	GetDlgItem(IDB_LOAD_IMAGE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionMonitorDlg_10"), &szTextTemp);	
	//영상@@Capture
	GetDlgItem(IDB_CAPTURE_IMAGE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionMonitorDlg_11"), &szTextTemp);	
	//기준선@@설정
	GetDlgItem(IDB_SET_REFERENCE_LINE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionMonitorDlg_12"), &szTextTemp);	
	//기준선@@해제
	GetDlgItem(IDB_REMOVE_REFERENCE_LINE)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CVisionMonitorDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVisionMonitorDlg)
	ON_EVENT(CVisionMonitorDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_PATTERN, -600 /* Click */, OnPattern, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_BLOB, -600 /* Click */, OnBlob, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CALIPER, -600 /* Click */, OnCaliper, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_SHOW_MARK, -600 /* Click */, OnShowMark, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_DELETE_MARK, -600 /* Click */, OnDeleteMark, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_MAKE_MARK, -600 /* Click */, OnMakeMark, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_OPTIONS, -600 /* Click */, OnOptions, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_TEST, -600 /* Click */, OnTest, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_LIVE_IMAGE, -600 /* Click */, OnLiveImage, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_STILL_IMAGE, -600 /* Click */, OnStillImage, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_1, -600 /* Click */, OnCam1, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_2, -600 /* Click */, OnCam2, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_3, -600 /* Click */, OnCam3, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_4, -600 /* Click */, OnCam4, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_5, -600 /* Click */, OnCam5, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_6, -600 /* Click */, OnCam6, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_7, -600 /* Click */, OnCam7, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_8, -600 /* Click */, OnCam8, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_9, -600 /* Click */, OnCam9, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_10, -600 /* Click */, OnCam10, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_11, -600 /* Click */, OnCam11, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_12, -600 /* Click */, OnCam12, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_13, -600 /* Click */, OnCam13, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_14, -600 /* Click */, OnCam14, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_15, -600 /* Click */, OnCam15, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_16, -600 /* Click */, OnCam16, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_17, -600 /* Click */, OnCam17, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_18, -600 /* Click */, OnCam18, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_19, -600 /* Click */, OnCam19, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_20, -600 /* Click */, OnCam20, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_SEARCH_AREA, -600 /* Click */, OnClickSearchArea, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_LOAD_IMAGE, -600 /* Click */, OnClickLoadImage, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAPTURE_IMAGE, -600 /* Click */, OnClickCaptureImage, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_SET_REFERENCE_LINE, -600 /* Click */, OnSetReferenceLine, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_REMOVE_REFERENCE_LINE, -600 /* Click */, OnRemoveReferenceLine, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_MEASURE_ANGLE, -600 /* Click */, OnClickMeasureAngle, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_DRAW_POINT, -600 /* Click */, OnClickDrawPoint, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_21, -600 /* Click */, OnClickCam21, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_22, -600 /* Click */, OnClickCam22, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_23, -600 /* Click */, OnClickCam23, VTS_NONE)
	ON_EVENT(CVisionMonitorDlg, IDB_CAM_24, -600 /* Click */, OnClickCam24, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CVisionMonitorDlg::OnExit() 
{
	OnStillImage();
	m_plnkVision->DestroyView(m_iViewID);

	g_iDisplayReferenceLine = FALSE;

	// 임시 주석 처리 - 일단 Off 하지 않음.
	MOlbSystemPre.LightDefult(DEF_MAX_CAMERA_NO);
	CDialog::OnCancel();
}

void CVisionMonitorDlg::OnSave() 
{
	//현재 모델을 저장하겠습니까?
	//UINT uiResult = MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Vision Model Save", M_ICONQUESTION |M_YESNO);
	UINT uiResult = MyMessageBox(MY_MESSAGE_BOX, 64000, _T("Vision Model Save"), M_ICONQUESTION |M_YESNO);
	if (uiResult == IDYES)
		m_plnkVision->WriteModelData(m_iCurrentCamNo, m_eMarkType, m_iSelectedMarkNo);	
}

void CVisionMonitorDlg::OnPattern() 
{
	m_eMarkType = DEF_PM_MARK;
}

void CVisionMonitorDlg::OnBlob() 
{
	m_eMarkType = DEF_BLOB_MARK;
}

void CVisionMonitorDlg::OnCaliper() 
{
	m_eMarkType = DEF_CALIPER_MARK;
}

void CVisionMonitorDlg::OnShowMark() 
{
	SMarkInfo sMarkInfo = { m_iCurrentCamNo, m_eMarkType, &m_iSelectedMarkNo, };

	CVisionSelectMarkDlg dlg(m_plnkVision, DEF_SHOW_MARK, &sMarkInfo);
	if (dlg.DoModal() == IDCANCEL)
		return ;

	m_plnkVision->DisplayPatternImage(m_iCurrentCamNo, m_eMarkType, m_iSelectedMarkNo);

	// DisplayPatternImage() 내에서 HaltVideo() 수행함.
	m_bIsOnLive = FALSE;
}

void CVisionMonitorDlg::OnDeleteMark() 
{
	SMarkInfo sMarkInfo = { m_iCurrentCamNo, m_eMarkType, &m_iSelectedMarkNo, };

	CVisionSelectMarkDlg dlg(m_plnkVision, DEF_DELETE_MARK, &sMarkInfo);
	if (dlg.DoModal() == IDCANCEL)
		return ;

	//선택한 Mark 를 삭제하겠습니까?
	//UINT uiResult = MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Mark Delete", M_ICONQUESTION |M_YESNO);
	UINT uiResult = MyMessageBox(MY_MESSAGE_BOX, 64004, _T("Mark Delete"), M_ICONQUESTION |M_YESNO);
	if (uiResult == IDYES)
		m_plnkVision->DeleteMark(m_iCurrentCamNo, m_eMarkType, m_iSelectedMarkNo);

	//OnStillImage(); //170717 JSH
}

void CVisionMonitorDlg::OnMakeMark() 
{
	OnStillImage();
	SMarkInfo sMarkInfo = { m_iCurrentCamNo, m_eMarkType, &m_iSelectedMarkNo, };

	CVisionSelectMarkDlg dlg(m_plnkVision, DEF_MAKE_MARK, &sMarkInfo);
	if (dlg.DoModal() == IDCANCEL)
		return ;

	// Model Import 여부 확인
	if (DEF_PM_MARK == m_eMarkType || DEF_BLOB_MARK == m_eMarkType)
	{
		
		CString szText1;
		CString szText2;
		//직접 등록
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64001"), &szText1);
		//연결
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64002"), &szText2);
		//Model 을 직접 등록하시겠습니까?\r\n아니면, 이미 등록된 Model 을 참조하여 Model 을 생성하겠습니까?
		int iResult = MyMessageBox(MY_MESSAGE_BOX, 64003, _T("Question"),
			M_ICONQUESTION|M_YESNO, szText1.GetBuffer(szText1.GetLength()), szText2.GetBuffer(szText2.GetLength()) );
		
		if (IDNO == iResult)
		{
			WCHAR cAfterVal[10];
			
			// 연결할 Mark 가 등록된 Camera 번호
			memset(cAfterVal, 0, sizeof(cAfterVal));
			if (FALSE == GetNumberBox(cAfterVal, 10, _T("1"), _T("Source Camera Number")))
				return;

			int iSrcCamNo = _wtoi(cAfterVal) - 1;
			if (FALSE == m_plnkVision->IsValidCam(iSrcCamNo))
			{
				//잘못된 번호 입니다.
				MyMessageBox(MY_MESSAGE_BOX,64014, _T("Error"), M_ICONERROR);
				return;
			}

			// 연결할 Mark 의 번호
			
			//선택된 Source Camera 에 등록된 Mark 중, 현재 Camera 에서 사용할 Mark 를 선택하세요.
			MyMessageBox(MY_MESSAGE_BOX, 64005, _T("Source Mark Selection"), M_ICONINFORMATION); //syr..SESL Num: 64505->64005
			
			int iSrcMarkNo = -1;
			SMarkInfo sSrcMarkInfo = { iSrcCamNo, m_eMarkType, &iSrcMarkNo, };
			
			CVisionSelectMarkDlg dlg(m_plnkVision, DEF_SHOW_MARK, &sSrcMarkInfo);
			if (dlg.DoModal() == IDCANCEL)
				return;

			iResult = m_plnkVision->ImportModel(m_eMarkType, iSrcCamNo, iSrcMarkNo, m_iCurrentCamNo, m_iSelectedMarkNo);
			if (iResult)
			{
// jdy sesl				MyMessageBox(iResult, "Vision Monitor Error", M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			}

			CString strName(sMarkInfo.m_szName);// 2009.02.11 CYJ
			m_plnkVision->SetModelName(m_iCurrentCamNo, m_eMarkType, m_iSelectedMarkNo, strName.GetBuffer(strName.GetLength()));
			OnSave();
			return;
		}
	}

	CPoint cpImageSize = m_plnkVision->GetDistribution(m_iCurrentCamNo);

	CRect rectLimit;
	rectLimit.SetRect(3, 3, cpImageSize.x-3, cpImageSize.y-3);
	//rectSearch.SetRect(0, 0, cpImageSize.x/2 , cpImageSize.y/2);
	//rectSearch.OffsetRect(cpImageSize.x/4, cpImageSize.y/4);
	CRect rectSearch = rectLimit;	

	CRect rectModel;
	rectModel.SetRect(0, 0, cpImageSize.x/4 , cpImageSize.y/4);
	rectModel.OffsetRect(cpImageSize.x/2-cpImageSize.y/8, cpImageSize.y*3/8);

	int iResult = SUCCESS;
	CString strMsg;
	if (m_eMarkType == DEF_PM_MARK)
	{
		// Search Area 설정
		// Model Area 설정

		//Search Area 를 설정하세요.
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64006"), &strMsg); //syr..SESL Num: 64506->64006
		CVisionJogKeyDlg jogSearchAreaDlg(m_plnkVision, m_iCurrentCamNo, strMsg, rectLimit, &rectSearch, this);
		if (jogSearchAreaDlg.DoModal() == IDCANCEL)
		{
			m_plnkVision->ClearOverlay(m_iCurrentCamNo);
			return ;
		}
		m_plnkVision->DrawOverlayAreaRect(m_iCurrentCamNo, rectSearch);
		
		// Model Area 설정
		
		//Model Area 를 설정하세요.
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64007"), &strMsg); //syr..SESL Num: 64507->64007
		CVisionJogKeyDlg jogModelAreaDlg(m_plnkVision, m_iCurrentCamNo, strMsg, rectSearch, &rectModel, this);
		if (jogModelAreaDlg.DoModal() == IDCANCEL)
		{
			m_plnkVision->ClearOverlay(m_iCurrentCamNo);
			return ;
		}
		m_plnkVision->DrawOverlayAreaRect(m_iCurrentCamNo, rectModel);
		
		// Reference Point 설정
		CPoint pointReference;
		//Reference Point 를 설정하세요.
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64008"), &strMsg); //syr..SESL Num: 64508->64008
		pointReference = rectModel.CenterPoint();
//		CVisionJogKeyDlg jogRefPointDlg(m_plnkVision, m_iCurrentCamNo, strMsg, rectLimit, &pointReference, this);
		CVisionJogKeyDlg jogRefPointDlg(m_plnkVision, m_iCurrentCamNo, strMsg, rectLimit, &pointReference, DEF_POINT, this);
		if (jogRefPointDlg.DoModal() == IDCANCEL)
		{
			m_plnkVision->ClearOverlay(m_iCurrentCamNo);
			return ;
		}
		m_plnkVision->DrawOverlayCrossMark(m_iCurrentCamNo, pointReference.x, pointReference.y);
		
		iResult = m_plnkVision->RegisterPatternMark(m_iCurrentCamNo, m_iSelectedMarkNo, rectSearch, rectModel, pointReference.x, pointReference.y);
	}
	else if (m_eMarkType == DEF_BLOB_MARK)
	{
		//@KKY_____________________________
		int iTabMarkNo = -1;
		switch (m_iSelectedMarkNo)
		{
		case DEF_BLOB_SR_MARK1:
		case DEF_BLOB_TAB_END_MARK1:
		case DEF_BLOB_SR_MARK2:
		case DEF_BLOB_TAB_END_MARK2:
			iTabMarkNo = getBlobRefrenceModelNo();
			break;
		}

		double dVisionX = 0.0;
		double dVisionY = 0.0;
		double dRobotX = 0.0;
		double dRobotY = 0.0;
		double dRefX = 0.0;
		double dRefY = 0.0;
		if (iTabMarkNo != -1)
		{
			iResult = m_plnkVision->RecognitionPatternMark(m_iCurrentCamNo, iTabMarkNo);
			if (iResult != ERR_VISION_SUCCESS)
			{
				MyMessageBox(_T("I can not recognize the Tab Lead Mark. It is necessary to be able to recognize the Tab Lead Mark to create a mark.\r\nTab Lead Mark를 인식할수 없습니다. Tab Lead Mark를 인식할수 있어야 마크 생성이 가능합니다."), _T("Confirm"));
				return;
			}
			dVisionX = m_plnkVision->GetSearchResultX(m_iCurrentCamNo, iTabMarkNo);
			dVisionY = m_plnkVision->GetSearchResultY(m_iCurrentCamNo, iTabMarkNo);

			/*/
			int iCalibUnitNo = MOlbSystemPre.GetCalibrationUnitNo(m_iCurrentCamNo, 0);
			if (1 != m_plnkVisonCalibration->GetPortingFactor(m_iCurrentCamNo, iCalibUnitNo)[8])			
			{
				MyMessageBox(_T("Camera Calibration이 수행되어 있지 않습니다. Calibration후 재시도 하세요."), _T("Confirm"));
				return;
			}
			else
				m_plnkVisonCalibration->ConvertV2R(m_iCurrentCamNo, iCalibUnitNo, dVisionX, dVisionY, dRobotX, dRobotY);
			/*/
		}
		//@________________________________

		// Search Area 설정
		//20121004 SJ_HJG 횡전개
		rectSearch = rectLimit;

		/*/@KKY.Del
		//Search Area 를 설정하세요.
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64006"), &strMsg); //syr..SESL Num: 64506->64006
		CVisionJogKeyDlg jogSearchAreaDlg(m_plnkVision, m_iCurrentCamNo, strMsg, rectLimit, &rectSearch, this);
		if (jogSearchAreaDlg.DoModal() == IDCANCEL)
		{
			m_plnkVision->ClearOverlay(m_iCurrentCamNo);
			return ;
		}
		m_plnkVision->DrawOverlayAreaRect(m_iCurrentCamNo, rectSearch);
		/*/

		//___________________
		// Blob Area 설정
		//Blob Area 를 설정하세요.
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64009Message_64009"), &strMsg); //syr..SESL Num: 64509->64009
		CVisionJogKeyDlg jogBlobAreaDlg(m_plnkVision, m_iCurrentCamNo, strMsg, rectSearch, &rectModel, this);
		if (jogBlobAreaDlg.DoModal() == IDCANCEL)
		{
			m_plnkVision->ClearOverlay(m_iCurrentCamNo);
			return ;
		}

		if (iTabMarkNo != -1)
		{
			//Model의 Left, Top위치가 TabMark위치에서 떨어진 거리를 구한다
			dRefX = rectModel.left - dVisionX;
			dRefY = rectModel.top - dVisionY;
		}

		iResult = m_plnkVision->RegisterBLOBPattern(m_iCurrentCamNo, m_iSelectedMarkNo, rectSearch, rectModel, dRefX, dRefY);
	}
	else if (m_eMarkType == DEF_CALIPER_MARK)
	{
		// Measurement Box 설정
		//Measurement Box 를 설정하세요.
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64010"), &strMsg);  //syr..SESL Num: 64510->64010
		CVisionJogKeyDlg jogMeasurementBoxDlg(m_plnkVision, m_iCurrentCamNo, strMsg, rectLimit, &rectModel, this);
		if (jogMeasurementBoxDlg.DoModal() == IDCANCEL)
		{
			m_plnkVision->ClearOverlay(m_iCurrentCamNo);
			return ;
		}

		iResult = m_plnkVision->RegisterEdgeModel(m_iCurrentCamNo, m_iSelectedMarkNo, rectModel);
	}

	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Vision Monitor Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	CString strName(sMarkInfo.m_szName);// 2009.02.11 CYJ
	m_plnkVision->SetModelName(m_iCurrentCamNo, m_eMarkType, m_iSelectedMarkNo, strName.GetBuffer(strName.GetLength()));
	OnSave();
}

void CVisionMonitorDlg::OnClickSearchArea() 
{
	if (DEF_PM_MARK != m_eMarkType)
	{
		//Pattern Matching Mark 이외의 Mark 에서는 Search Area 를 사용하지 않음.
		MyMessageBox(MY_MESSAGE_BOX, 64011, _T("Error"), M_ICONERROR);
		return;
	}

	SMarkInfo sMarkInfo = { m_iCurrentCamNo, m_eMarkType, &m_iSelectedMarkNo, };

	CVisionSelectMarkDlg dlg(m_plnkVision, DEF_SHOW_MARK, &sMarkInfo);
	if (dlg.DoModal() == IDCANCEL)
		return ;

	CVisionSearchAreaDlg SearchAreaDlg(m_plnkVision, m_iCurrentCamNo, m_iSelectedMarkNo, sMarkInfo.m_szName);
	if (SearchAreaDlg.DoModal() == IDCANCEL)
		return ;
}

void CVisionMonitorDlg::OnOptions() 
{
	CVisionOptionDlg dlg(m_plnkVision, m_eMarkType, m_iCurrentCamNo, m_iSelectedMarkNo);
	dlg.DoModal();
}

void CVisionMonitorDlg::OnTest()
{
	OnStillImage();
	
	int iResult = SUCCESS;
	double dResultY = 0.0 ;	//20121004 SJ_HJG 횡전개
	int iTabMarkNo = -1;
	double dVisionX = 0.0;
	double dVisionY = 0.0;
	double dRefX = 0.0;
	double dRefY = 0.0;

	switch (m_eMarkType)
	{
	case DEF_PM_MARK:
		iResult = m_plnkVision->RecognitionPatternMark(m_iCurrentCamNo, m_iSelectedMarkNo);

		if (ERR_VISION_SUCCESS == iResult)
		{
			
			double dVisionX = m_plnkVision->GetSearchResultX(m_iCurrentCamNo, m_iSelectedMarkNo);
			double dVisionY = m_plnkVision->GetSearchResultY(m_iCurrentCamNo, m_iSelectedMarkNo);

			///임시 
			int iCalibUnitNo = MOlbSystemPre.GetCalibrationUnitNo(m_iCurrentCamNo, 0);

			if (1 == m_plnkVisonCalibration->GetPortingFactor(m_iCurrentCamNo, iCalibUnitNo)[8])
			{
				double dRobotX = 0.0;
				double dRobotY = 0.0;
				
				m_plnkVisonCalibration->ConvertV2R(m_iCurrentCamNo, iCalibUnitNo, dVisionX, dVisionY, dRobotX, dRobotY);
				char szResult[256];
				memset(szResult, 0, sizeof(szResult));
				sprintf(szResult, "V(x,y)=(%.3lf,%.3lf) R(x,y)=(%.3lf,%.3lf)", dVisionX, dVisionY, dRobotX, dRobotY);
				m_plnkVision->DrawOverlayText(m_iCurrentCamNo, szResult);
			}
		}
		break;
		
	case DEF_BLOB_MARK:
		//170616 JSH.s
		if(m_iSelectedMarkNo == DEF_BLOB_TAB_ATTACHED1 || m_iSelectedMarkNo == DEF_BLOB_TAB_ATTACHED2)
		{	
			iResult = m_plnkVision->RecognitionBLOBPattern(m_iCurrentCamNo, m_iSelectedMarkNo);
			break;
		}
		//170616 JSH.e

		//@_____________________________
		switch (m_iSelectedMarkNo)
		{
		case DEF_BLOB_SR_MARK1:
		case DEF_BLOB_TAB_END_MARK1:
		case DEF_BLOB_SR_MARK2:
		case DEF_BLOB_TAB_END_MARK2:
			iTabMarkNo = getBlobRefrenceModelNo();
			break;
		}

		if (iTabMarkNo != -1)
		{
			iResult = m_plnkVision->RecognitionPatternMark(m_iCurrentCamNo, iTabMarkNo);
			if (iResult != ERR_VISION_SUCCESS)
			{
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
			dVisionX = m_plnkVision->GetSearchResultX(m_iCurrentCamNo, iTabMarkNo);
			dVisionY = m_plnkVision->GetSearchResultY(m_iCurrentCamNo, iTabMarkNo);
			if (1 == m_plnkVisonCalibration->GetPortingFactor(m_iCurrentCamNo, 0)[8])
			{				
				m_plnkVisonCalibration->ConvertV2R(m_iCurrentCamNo, 0, dVisionX, dVisionY, dRefX, dRefY);
			}
		}
		//@________________________________

		//iResult = m_plnkVision->RecognitionBLOBPattern(m_iCurrentCamNo, m_iSelectedMarkNo);
		iResult = m_plnkVision->RecognitionEdgeLineY(m_iCurrentCamNo, m_iSelectedMarkNo, TRUE, dVisionX, dVisionY);
		if (iResult == SUCCESS && iTabMarkNo != -1)
		{
			if (1 == m_plnkVisonCalibration->GetPortingFactor(m_iCurrentCamNo, 0)[8])
			{
				double dRobotX = 0.0;
				double dRobotY = 0.0;
				dVisionX = m_plnkVision->GetSearchBlobResultX(m_iCurrentCamNo, m_iSelectedMarkNo);
				dVisionY = m_plnkVision->GetSearchBlobResultY(m_iCurrentCamNo, m_iSelectedMarkNo);
				m_plnkVisonCalibration->ConvertV2R(m_iCurrentCamNo, 0, dVisionX, dVisionY, dRobotX, dRobotY);
				char szResult[256];
				memset(szResult, 0, sizeof(szResult));
				sprintf(szResult, "R(x,y)=(%.3lf,%.3lf), Distance(X,Y)=(%.3lf,%.3lf)", 
					dRobotX, dRobotY, dRobotX-dRefX, dRobotY-dRefY);
				m_plnkVision->DrawOverlayText(m_iCurrentCamNo, szResult);
			}
		}

		//__________
		break;
		
	case DEF_CALIPER_MARK:
		iResult = m_plnkVision->FindEdge(m_iCurrentCamNo, m_iSelectedMarkNo);
		break;
	default:
		break;
	}

	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, "Vision Monitor Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
}

void CVisionMonitorDlg::OnLiveImage() 
{
	if (m_bIsOnLive == TRUE)
		return;

	m_plnkVision->ClearOverlay(m_iCurrentCamNo);
	
	int iIndex = m_iCurrentCamNo % 2;
	m_plnkVision->DrawOverlayCross_Measure(m_iCurrentCamNo, m_ptMeasure[iIndex].x, m_ptMeasure[iIndex].y);

	m_plnkVision->LiveVideo(m_iCurrentCamNo);
	m_bIsOnLive = TRUE;
	m_bOnStillContinue = TRUE;
}

void CVisionMonitorDlg::OnStillImage() 
{
	if (m_bIsOnLive == TRUE)
	{
		m_plnkVision->HaltVideo(m_iCurrentCamNo);
		m_bIsOnLive = FALSE;

		// 확인 필요 : Live 종료를 확인하는 방법 필요.
	}

	m_plnkVision->ClearOverlay(m_iCurrentCamNo);
	int iResult = m_plnkVision->Grab(m_iCurrentCamNo);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, "Vision Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkVision->GrabWait(m_iCurrentCamNo);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, "Vision Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	m_bOnStillContinue = FALSE;
}

void CVisionMonitorDlg::selectCamera(int iCamNo)
{
	if (m_iCurrentCamNo != iCamNo && m_bIsOnLive == TRUE)
	{
		//SJ_YYK 110901 Add..
//@		if(m_iCurrentCamNo != -1)
		if (m_iCurrentCamNo < 0)
			m_iCurrentCamNo = 0;
//@		{
			m_plnkVision->HaltVideo(m_iCurrentCamNo);
			m_plnkVision->ClearOverlay(m_iCurrentCamNo);
			m_bIsOnLive = FALSE;
//@		}
	}

	//110214.Add_____
	if (int(m_iCurrentCamNo/2) != int(iCamNo/2) && m_iCurrentCamNo != -1)
	{
		CPoint cpImageSize = m_plnkVision->GetDistribution(m_iCurrentCamNo);
		m_ptMeasure[0].x = LONG(cpImageSize.x/2.0 + 0.5);
		m_ptMeasure[0].y = LONG(cpImageSize.y/2.0 + 0.5);
		m_ptMeasure[1].x = m_ptMeasure[0].x;
		m_ptMeasure[1].y = m_ptMeasure[0].y;
		m_bMeasureAngle = FALSE;
		m_btnMeasureAngle.SetValue(0);
		m_btnDrawPoint.EnableWindow(FALSE);
		displayMeasureDegree();
	}
	//_______________

	m_iCurrentCamNo = iCamNo;

	MOlbSystemPre.SetLightValue(m_iCurrentCamNo);
	MOlbSystemPre.LightOn(m_iCurrentCamNo);
	
	OnLiveImage();
	//OnStillImage();
}

void CVisionMonitorDlg::OnCam1() 
{
	selectCamera(0);
}

void CVisionMonitorDlg::OnCam2() 
{
	selectCamera(1);	
}

void CVisionMonitorDlg::OnCam3() 
{
	selectCamera(2);
}

void CVisionMonitorDlg::OnCam4() 
{
	selectCamera(3);
}

void CVisionMonitorDlg::OnCam5() 
{
	selectCamera(4);
}

void CVisionMonitorDlg::OnCam6() 
{
	selectCamera(5);
}

void CVisionMonitorDlg::OnCam7() 
{
	selectCamera(6);
}

void CVisionMonitorDlg::OnCam8() 
{
	selectCamera(7);
}

void CVisionMonitorDlg::OnCam9() 
{
	selectCamera(8);
}

void CVisionMonitorDlg::OnCam10() 
{
	selectCamera(9);
}

void CVisionMonitorDlg::OnCam11() 
{
	selectCamera(10);
}

void CVisionMonitorDlg::OnCam12() 
{
	selectCamera(11);
}

void CVisionMonitorDlg::OnCam13() 
{
	selectCamera(12);
}

void CVisionMonitorDlg::OnCam14() 
{
	selectCamera(13);
}

void CVisionMonitorDlg::OnCam15() 
{
	selectCamera(14);
}

void CVisionMonitorDlg::OnCam16() 
{
	selectCamera(15);
}

void CVisionMonitorDlg::OnCam17() 
{
	selectCamera(16);
}

void CVisionMonitorDlg::OnCam18() 
{
	selectCamera(17);
}

void CVisionMonitorDlg::OnCam19() 
{
	selectCamera(18);
}

void CVisionMonitorDlg::OnCam20() 
{
	selectCamera(19);
}

void CVisionMonitorDlg::OnClickLoadImage() 
{
	TCHAR szPath[1024];
	::GetCurrentDirectory(sizeof(szPath), szPath);

	WCHAR szFilter[] = _T("Image (*.bmp)|*.bmp|All File (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		CString strPathName = dlg.GetPathName();
		m_plnkVision->LoadImage(m_iCurrentCamNo, strPathName.GetBuffer(strPathName.GetLength()));
	}

	::SetCurrentDirectory(szPath);
}

void CVisionMonitorDlg::OnClickCaptureImage() 
{
	CString szTemp;
	ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64012"), &szTemp);
	//Image File 이름을 입력하세요. [Max. 32]
	CCommonGetValueDlg	valueDialog(32, _T(""), szTemp.GetBuffer(szTemp.GetLength()) );
	valueDialog.DoModal();

	CString strFileName(valueDialog.GetStringValue());

	int iResult = m_plnkVision->SaveImage(m_iCurrentCamNo, (LPCTSTR)strFileName);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	//Image file 저장 완료.
	MyMessageBox(MY_MESSAGE_BOX, 64013, _T("Information"), M_ICONINFORMATION);
}

void CVisionMonitorDlg::OnSetReferenceLine()
{
	g_iDisplayReferenceLine = TRUE;

	if (FALSE == m_bIsOnLive)
	{
		m_plnkVision->ClearOverlay(m_iCurrentCamNo);
		m_plnkVision->Grab(m_iCurrentCamNo);
		m_plnkVision->GrabWait(m_iCurrentCamNo);
	}
}

void CVisionMonitorDlg::OnRemoveReferenceLine()
{
	g_iDisplayReferenceLine = FALSE;

	if (FALSE == m_bIsOnLive)
	{
		m_plnkVision->ClearOverlay(m_iCurrentCamNo);
		m_plnkVision->Grab(m_iCurrentCamNo);
		m_plnkVision->GrabWait(m_iCurrentCamNo);
	}
}

void CVisionMonitorDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == m_KeyTimerID)
	{
		/*
		if (m_bOnStillContinue == TRUE)
		{
			//OnStillImage();
			m_plnkVision->HaltVideo(m_iCurrentCamNo);
			m_bIsOnLive = FALSE;
						
			m_plnkVision->ClearOverlay(m_iCurrentCamNo);
			int iResult = m_plnkVision->Grab(m_iCurrentCamNo);
			if (iResult)
			{
				///if(MyMessageBox(_T("Live ON:YES    Live OFF:NO"), _T("Select"), M_ICONQUESTION|M_YESNO) == IDNO)
					m_bOnStillContinue = FALSE;
				// jdy sesl		MyMessageBox(iResult, "Vision Error", M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);

				return;
			}
			
			iResult = m_plnkVision->GrabWait(m_iCurrentCamNo);
			if (iResult)
			{
				///if(MyMessageBox(_T("Live ON:YES    Live OFF:NO"), _T("Select"), M_ICONQUESTION|M_YESNO) == IDNO)
					m_bOnStillContinue = FALSE;
				// jdy sesl		MyMessageBox(iResult, "Vision Error", M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
			
		}
		*/
	}
	CDialog::OnTimer(nIDEvent);
}

void CVisionMonitorDlg::displayMeasureDegree()
{
	if (FALSE == m_bMeasureAngle)
	{
		GetDlgItem(IDS_VALUE_DEGREE)->SetWindowText(_T("0.0"));
		return;
	}
	double dMeasureX[2];
	double dMeasureY[2];

	int iCamIndex[2];
	if (m_iCurrentCamNo % 2 == 0)
	{
		iCamIndex[0] = m_iCurrentCamNo;
		iCamIndex[1] = m_iCurrentCamNo+1;
	}
	else
	{
		iCamIndex[0] = m_iCurrentCamNo-1;
		iCamIndex[1] = m_iCurrentCamNo;
	}
	m_plnkVisonCalibration->ConvertV2R(iCamIndex[0], 0, m_ptMeasure[0].x, m_ptMeasure[0].y, dMeasureX[0], dMeasureY[0]);
	m_plnkVisonCalibration->ConvertV2R(iCamIndex[1], 0, m_ptMeasure[1].x, m_ptMeasure[1].y, dMeasureX[1], dMeasureY[1]);

	double dCamDist = 0.0;	
	switch (m_iCurrentCamNo)
	{
		case DEF_F_TABALIGNER_CAMERA_1:
		case DEF_F_TABALIGNER_CAMERA_2:
			dCamDist = MOlbSystemPre.GetTabCarrierComponent(0)->GetCurrentExpandPos();
			break;
		case DEF_R_TABALIGNER_CAMERA_1:
		case DEF_R_TABALIGNER_CAMERA_2:
			dCamDist = MOlbSystemPre.GetRTabCarrierComponent(0)->GetCurrentExpandPos();
			break;
		case DEF_INSPECTION_CAMERA_1:
		case DEF_INSPECTION_CAMERA_2:
			dCamDist = MOlbSystemPre.GetCameraCarrierComponent(0)->GetCurrentPosExpand();
			break;
		case DEF_INSPECTION_CAMERA_3:
		case DEF_INSPECTION_CAMERA_4:
			dCamDist = MOlbSystemPre.GetCameraCarrierComponent(1)->GetCurrentPosExpand();
			break;

#ifdef DEF_SOURCE_SYSTEM
		case DEF_INSPECTION_CAMERA_5:
		case DEF_INSPECTION_CAMERA_6:
			dCamDist = MOlbSystemPre.GetCameraCarrierComponent(2)->GetCurrentPosExpand();
			break;
		case DEF_INSPECTION_CAMERA_7:
		case DEF_INSPECTION_CAMERA_8:
			dCamDist = MOlbSystemPre.GetCameraCarrierComponent(3)->GetCurrentPosExpand();
			break;
#endif

		case DEF_R_INSPECTION_CAMERA_1:
		case DEF_R_INSPECTION_CAMERA_2:
			dCamDist = MOlbSystemPre.GetRCameraCarrierComponent(0)->GetCurrentPosExpand();
			break;
		case DEF_R_INSPECTION_CAMERA_3:
		case DEF_R_INSPECTION_CAMERA_4:
			dCamDist = MOlbSystemPre.GetRCameraCarrierComponent(1)->GetCurrentPosExpand();
			break;

#ifdef DEF_SOURCE_SYSTEM
		case DEF_R_INSPECTION_CAMERA_5:
		case DEF_R_INSPECTION_CAMERA_6:
			dCamDist = MOlbSystemPre.GetRCameraCarrierComponent(2)->GetCurrentPosExpand();
			break;
		case DEF_R_INSPECTION_CAMERA_7:
		case DEF_R_INSPECTION_CAMERA_8:
			dCamDist = MOlbSystemPre.GetRCameraCarrierComponent(3)->GetCurrentPosExpand();
			break;
#endif
	}

	double dDistX = dMeasureX[1] - dMeasureX[0];
	double dDistY = dMeasureY[1] - dMeasureY[0];
//@#ifdef DEF_SOURCE_SYSTEM
//@	dDistY += dCamDist;
//@	if (dDistY == 0.0)
//@		return;
//@	double dDeg = RAD2DEG(atan(dDistX/dDistY));
//@#else
	dDistX += dCamDist;
	if (dDistX == 0.0)
		return;
	double dDeg = RAD2DEG(atan(dDistY/dDistX));
//@#endif
	CString strDeg;
	strDeg.Format(_T("%.5lf"), dDeg);
	GetDlgItem(IDS_VALUE_DEGREE)->SetWindowText(strDeg);
}

void CVisionMonitorDlg::OnClickMeasureAngle() 
{
	// TODO: Add your control notification handler code here
	m_bMeasureAngle = !m_bMeasureAngle;
	if (TRUE == m_bMeasureAngle)
		m_btnDrawPoint.EnableWindow(TRUE);
	else
		m_btnDrawPoint.EnableWindow(FALSE);
}

void CVisionMonitorDlg::OnClickDrawPoint()
{
	// TODO: Add your control notification handler code here
	OnStillImage();

	CString strMsg;
	CPoint pointReference;
	//Reference Point 를 설정하세요.
	ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64008"), &strMsg); //syr..SESL Num: 64508->64008
	int iIndex = m_iCurrentCamNo % 2;
	pointReference = m_ptMeasure[iIndex];

	CPoint cpImageSize = m_plnkVision->GetDistribution(m_iCurrentCamNo);
	CRect rectLimit;
	rectLimit.SetRect(3, 3, cpImageSize.x-3, cpImageSize.y-3);

	CVisionJogKeyDlg jogRefPointDlg(m_plnkVision, m_iCurrentCamNo, strMsg, rectLimit, &pointReference, DEF_POINT_MEASURE, this);
	if (jogRefPointDlg.DoModal() == IDCANCEL)
	{
		m_plnkVision->ClearOverlay(m_iCurrentCamNo);
		return ;
	}
	m_plnkVision->DrawOverlayCross_Measure(m_iCurrentCamNo, pointReference.x, pointReference.y);	
	m_ptMeasure[iIndex] = pointReference;
	displayMeasureDegree();
}

//@130119.KKY____________________________________
int	CVisionMonitorDlg::getBlobRefrenceModelNo()
{
	int iTabMarkNo = -1;
	switch (m_iSelectedMarkNo)
	{
	case DEF_BLOB_SR_MARK1:
	case DEF_BLOB_TAB_END_MARK1:
#ifdef DEF_SOURCE_SYSTEM
		if (m_iCurrentCamNo == DEF_F_TABALIGNER_CAMERA_1)
			iTabMarkNo = DEF_TAB_LEAD_XU_MARK1;
		else if (m_iCurrentCamNo == DEF_R_TABALIGNER_CAMERA_1)
			iTabMarkNo = DEF_TAB_LEAD_XD_MARK1;
#else
		if(m_iCurrentCamNo == DEF_F_TABALIGNER_CAMERA_1)
	#ifdef DEF_MIRROR_
			iTabMarkNo = DEF_TAB_LEAD_YR_MARK1;
	#else
		iTabMarkNo = DEF_TAB_LEAD_YL_MARK1;
	#endif
		else if (m_iCurrentCamNo == DEF_R_TABALIGNER_CAMERA_1)
	#ifdef DEF_MIRROR_
			iTabMarkNo = DEF_TAB_LEAD_YL_MARK1;
	#else
		iTabMarkNo = DEF_TAB_LEAD_YR_MARK1;
	#endif
#endif
		break;
	case DEF_BLOB_SR_MARK2:
	case DEF_BLOB_TAB_END_MARK2:
#ifdef DEF_SOURCE_SYSTEM
		if (m_iCurrentCamNo == DEF_F_TABALIGNER_CAMERA_2)
			iTabMarkNo = DEF_TAB_LEAD_XU_MARK2;
		else if (m_iCurrentCamNo == DEF_R_TABALIGNER_CAMERA_2)
			iTabMarkNo = DEF_TAB_LEAD_XD_MARK2;
#else
		if(m_iCurrentCamNo == DEF_F_TABALIGNER_CAMERA_2)
	#ifdef DEF_MIRROR_
			iTabMarkNo = DEF_TAB_LEAD_YR_MARK2;
	#else
		iTabMarkNo = DEF_TAB_LEAD_YL_MARK2;
	#endif
		else if (m_iCurrentCamNo == DEF_R_TABALIGNER_CAMERA_2)
	#ifdef DEF_MIRROR_
			iTabMarkNo = DEF_TAB_LEAD_YL_MARK2;
	#else
		iTabMarkNo = DEF_TAB_LEAD_YR_MARK2;
	#endif
#endif
		break;
	}
	return iTabMarkNo;
}
//_______________________________________________

void CVisionMonitorDlg::OnClickCam21() 
{
	// TODO: Add your control notification handler code here
	selectCamera(20);
}

void CVisionMonitorDlg::OnClickCam22() 
{
	// TODO: Add your control notification handler code here
	selectCamera(21);
}

void CVisionMonitorDlg::OnClickCam23() 
{
	// TODO: Add your control notification handler code here
	selectCamera(22);
}

void CVisionMonitorDlg::OnClickCam24() 
{
	// TODO: Add your control notification handler code here
	selectCamera(23);
}
