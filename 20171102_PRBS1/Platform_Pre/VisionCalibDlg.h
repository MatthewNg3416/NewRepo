//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_VISIONCALIBDLG_H__45852C3E_BE19_4190_AA85_5D77A245FF0D__INCLUDED_)
#define AFX_VISIONCALIBDLG_H__45852C3E_BE19_4190_AA85_5D77A245FF0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisionCalibDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVisionCalibDlg dialog
#include "DefSystem.h"
#include "MTeachingPosData.h"
#include "DefPanelAligner.h"
#include "DefModelChange.h"
#include "MPos_XY.h"
#include "MPos_XYT.h"
#include "MVisionCalibration.h"
#include "MHW_Teach.h"

class MCtrlPanelAligner;

// Source, Gate 통틀어 Camera 가 가장 많을 때의 Camera 개수.
const int DEF_ALL_CAM							= 12;

// Calibration Type
const int DEF_CALIBRATION_NONE					= -1;
const int DEF_CALIBRATION_2D					= 0;
const int DEF_CALIBRATION_MOUNTER_ALIGNER_2D	= 1;
const int DEF_CALIBRATION_GANTRY_2D				= 2;
const int DEF_CALIBRATION_GANTRY_REVERSE_2D		= 3;
const int DEF_CALIBRATION_TABCARRIER_2D			= 4;

// Calibration Move Point
const int DEF_CALIBRATION_MOVE_POINT			= 3;

// Prism 사용 시, 한 Camera 에서 볼 수 있는 최대 Image 개수
const int DEF_MAX_PRISM_IMAGE					= 2;

const int DEF_PRISM_SIDE_ALL					= -1;
const int DEF_PRISM_SIDE_1						= 0;
const int DEF_PRISM_SIDE_2						= 1;

typedef struct
{
	int m_iType;
	BOOL m_bUsePrism;

} SCalibrationTypeData;

class MCameraCarrier;

class CVisionCalibDlg : public CDialog
{
private:
	MSystemData*	m_plnkSystemData;
	MPanelData*		m_plnkPanelData;
	MPreBonderData*	m_plnkPreBonderData;
	MPanelAligner*	m_plnkPanelAligner;
	MModelChange*	m_plnkModelChange;
	MModelChange*	m_plnkModelChange2;
	MTabMounter*	m_plnkTabMounter;
//KKY.Del	MTabAligner*	m_plnkTabAligner;
	MCameraCarrier* m_plnkCamCarrier;

	MTabCarrier*	m_plnkTabCarrier;
	
	MCtrlPanelAligner*	m_plnkCtrlPanelAligner;

	IVision*		m_plnkVision;
	MVisionCalibration* m_plnkCalibration;
	SCalibrationOperationData m_sCalibData;
	double			m_rgdPortingFactor[DEF_VISION_CALIBRATION_UNIT_PER_CAM][DEF_FACTOR_NUMBER];
	int				m_iTargetCamNo;
	int				m_iSelectedMarkNo;
	double			m_dAngle;
	int				m_iViewID;
	BOOL			m_bIsOnLive;
	
	int	m_iWorkGroup;//Front/Rear
	int m_iGroupID;
	int m_iInstanceID;
	//TabAlign 카메라	: 각 TabCarrier Instance
	//Inspection 카메라 : PanelAligner가 0, TabMounter가 1
	int m_iCalibUnit;

	SCalibrationTypeData m_rgsType[DEF_ALL_CAM];

	MPos_XYT	m_rgFixedPos[DEF_PANEL_ALIGNER_MAX_POSITION];
	MPos_XYT	m_rgModelPos[DEF_PANEL_ALIGNER_MAX_POSITION];
	MPos_XYT	m_rgOffsetPos[DEF_PANEL_ALIGNER_MAX_POSITION];
	MPos_XYT	m_rgEtcOffset[DEF_PANEL_ALIGNER_MAX_POSITION];

	/** Vision Pixel 좌표 */
	MPos_XY	m_rgxyPixelPos[DEF_VISION_CALIBRATION_UNIT_PER_CAM][DEF_VCALIB_MAX_MOVE_POSITION];

	/** Robot Motion 좌표 */
	// 2D
	MPos_XY	m_rgxyRobotPos[DEF_VCALIB_MAX_MOVE_POSITION];
	// Virtual 2D
	MPos_XYT m_rgxytRobotPos[DEF_VCALIB_MAX_MOVE_POSITION];

	/** Offset Motion 좌표 */
	// 2D
	MPos_XY	m_rgxyOffsetPos[DEF_VCALIB_MAX_MOVE_POSITION];
	
	/** 이동 시작 위치 */
	// 2D
	MPos_XY	m_xyStartPos;
	// 1D
	MPos_XYT m_xytStartPos;

	MPos_XYT m_dMovePosition;		// 모션의 좌표를 저장한다.

	MTeachingPosData	m_FixedData;
	MTeachingPosData	m_OffsetData;
	MTeachingPosData	m_ModelData;

	MHW_Teach*			m_pC_HWTeach;

	BOOL	m_bOnStillContinue;
	UINT m_KeyTimerID;

private:
	void setWorkGroup();
	void CalculateCarrierCenterPos(int iGroupID);
	void displayLabel();
	void stillImage(int iCamNo);
	void haltImage(int iCamNo);
	void liveImage(int iCamNo);
	int setupMovePositions();
	void setWorkingUnit(int iCamNo);
	int executeCalibrationMoves(int iPrismSideNo);	// Pixel vs. Robot 좌표 구하기
	int expandTabAlignerCamera();
	int expandInspectCamera();
	int expandTabCarrierCamera();
	void GetCarrierFixPos(int iGroupID);
	int GetTabCarrierCameraFixedPosition(int iCarrierID, 
								int iCamNo,
								int iMarkNo,
								double dTAngle,
								double* pX,
								double* pY,
								double* pCamDistX);

	void FindCenter(double dTAngle, 
		            double dRadius, 
					MPos_XYT posFirst, 
					MPos_XYT posSecond, 
					MPos_XYT* posCenter);

	void Rotation(double dAngle, 
		          double dOldX, 
				  double dOldY, 
				  double* dNewX,
				  double* dNewY);

// Construction
public:
	CVisionCalibDlg(int iTargetCamNo, int iGroupID, int iInstanceID, CWnd* pParent = NULL);
		
// Dialog Data
	//{{AFX_DATA(CVisionCalibDlg)
	enum { IDD = IDD_VISION_CALIBRATION };
	CListBox	m_listResult;
	CBtnEnh	m_ctrlBtnMovePointX;
	CBtnEnh	m_ctrlBtnMoveWidthX;
	CBtnEnh	m_ctrlMovePointY;
	CBtnEnh	m_ctrlBtnMoveWidthY;
	CBtnEnh	m_ctrlLblMovePointX;
	CBtnEnh	m_ctrlLblMoveWidthX;
	CBtnEnh	m_ctrlLblMovePointY;
	CBtnEnh	m_ctrlLblMoveWidthY;
	CBtnEnh	m_ctrlLblCameraNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionCalibDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CVisionCalibDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSave();
	afx_msg void OnCameraNum();
	afx_msg void OnXMovePoint();
	afx_msg void OnXMoveWidth();
	afx_msg void OnYMovePoint();
	afx_msg void OnYMoveWidth();
	afx_msg void OnExecuteCalib();
	afx_msg void OnShowMark();
	afx_msg void OnDeleteMark();
	afx_msg void OnMakeMark();
	afx_msg void OnOptions();
	afx_msg void OnTest();
	afx_msg void OnLive();
	afx_msg void OnFixCoord();
	afx_msg void OnCameraDist2();
	afx_msg void OnCameraDist3();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickCameraDist4();
	afx_msg void OnClickFixCoordTabcarrier();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISIONCALIBDLG_H__45852C3E_BE19_4190_AA85_5D77A245FF0D__INCLUDED_)
