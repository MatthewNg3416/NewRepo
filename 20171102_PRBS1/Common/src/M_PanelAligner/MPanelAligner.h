/* 
 * PanelAligner Component
 *
 * Copyright 2005 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MPanelAligner.h : Headerfile of PanelAligner component.
 */

#ifndef M_PANEL_ALIGNER_H
#define M_PANEL_ALIGNER_H

#include "DefSystem.h"
#include "DefPanelAligner.h"
#include "ICommon.h"
#include "IStatePanelAligner.h"

const int DEF_CHECK_CURRENT_POS = 0;
const int DEF_CHECK_TARGET_POS = 1;



const double COMPARE_PANEL_SIZE_MODEL_TOLERANCE = 5.0;

/** XYTPanelTransferX 4축을 가지는 PanelAligner Unit을 모델링한 Class이다. */
class MPanelAligner : public IStatePanelAligner, public virtual ICommon
{
protected:

/************************************************************************/
/*                             IO                                       */
/************************************************************************/
	IIO*		m_plnkIO;

/************************************************************************/
/*                             Vacuum                                   */
/************************************************************************/
	/** PanelAligner Vacuum Object */
	IVacuum*	m_plnkVac1;
	IVacuum*	m_plnkVac2;
	IVacuum*	m_plnkVac3;

/************************************************************************/
/*                             Cylinder                                 */
/************************************************************************/
//110102 HJG 추가
#ifdef DEF_USE_PANEL_PUSHER
	ICylinder*	m_plnkPanelPusherUD;
#endif
/************************************************************************/
/*                             Motion                                   */
/************************************************************************/
	/** PanelAligner X, Y, T Motion Object */
	IAxis*	m_plnkX;
	IAxis*	m_plnkY;
	IAxis*	m_plnkT;
	IAxis*	m_plnkZ;

	/** PanelAligner Multi X Motion Object */
	//ISercosMultiAxis*	m_plnkMultiYT;
	IRSNMCMultiAxis*	m_plnkMultiYT;			//161024 Modify...

/************************************************************************/
/*                             Data                                     */
/************************************************************************/
	/** System Data Object */
	MSystemData*		m_plnkSystemData;

	/** Panel Data Object */
	MPanelData*			m_plnkPanelData;
	
	MPreBonderData*		m_plnkPreBonderData;

	MTabData*			m_plnkTabData;

/************************************************************************/
/*                             IO Address                               */
/************************************************************************/

	/** Air CVBlow Air Blow Output IO Address	*/
	unsigned short		m_usOAddrAirBlow1;
	unsigned short		m_usOAddrAirBlow2;
//@	unsigned short		m_usOAddrAirBlow3;
//@	unsigned short		m_usOAddrAirBlow4;

	/** Air CVBlow Air Blow Input IO Address	*/
	unsigned short		m_usIAddrAirBlow1;
	unsigned short		m_usIAddrAirBlow2;
//@	unsigned short		m_usIAddrAirBlow3;
//@	unsigned short		m_usIAddrAirBlow4;

	/** Air CVBlow Air Vacuum Output IO Address	*/
	unsigned short		m_usOAddrAirVacuum1;
	unsigned short		m_usOAddrAirVacuum2;
	unsigned short		m_usOAddrAirVacuum3;
	unsigned short		m_usOAddrAirVacuum4;

	/** Air CVBlow Air Vacuum Input IO Address	*/
	unsigned short		m_usIAddrAirVacuum1;
	unsigned short		m_usIAddrAirVacuum2;
	unsigned short		m_usIAddrAirVacuum3;
	unsigned short		m_usIAddrAirVacuum4;

	unsigned short		m_usIPanelDetect;

//120224.KMS____________
	unsigned short		m_usIPanelDetectLoading;
	unsigned short		m_usIPanelDetectWorking;

	//120508.KMS_________
	unsigned short		m_usIIonizerState;

	//170724_KDH Spot Cleaner 추가
	unsigned short		m_usOAddrSpotCleanerOn;
	unsigned short		m_usIAddrSpotCleanerState;
	//_________________________


/************************************************************************/
/*                             Position                                 */
/************************************************************************/
	CString		m_strPosFileName;
	CString		m_strOffsetPosFileName;
	CString		m_strOffsetPosFilePath;

	/** PanelAligner X, Y, T, Z 축 Position 정보 */
    STeachingInfo	m_sXYTZTeachingInfo;

	/** PanelAligner Z 축 Position 정보 */
	STeachingInfo	m_sZTeachingInfo;

	/** PanelAligner XYT축 고정좌표계 */
	MPos_XYT	m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_MAX_POSITION];
	/** PanelAligner XYT축 모델좌표계 */
	MPos_XYT	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_MAX_POSITION];
	/** PanelAligner XYT축 Offset 좌표계 */
	MPos_XYT	m_rgsXYTOffsetPos[DEF_PANEL_ALIGNER_MAX_POSITION];

	/** PanelAligner Z축 고정 좌표 */
	double		m_rgdZFixedPos[DEF_PANEL_ALIGNER_Z_MAX_POSITION];
	/** PanelAligner Z축 Offset 좌표 */
	double		m_rgdZOffsetPos[DEF_PANEL_ALIGNER_Z_MAX_POSITION];
	/** PanelAligner Z축 모델 좌표 */
	double		m_rgdZModelPos[DEF_PANEL_ALIGNER_Z_MAX_POSITION];

/************************************************************************/
/*                             Align                                    */
/************************************************************************/
	/** PanelAligner Align Mark Offset*/
	MPos_XYT	m_rgAlignMarkOffset;

	/** Align 수행 여부 */
	BOOL		m_bMarkAligned;

	/** Align하기 전의 T 축 값 */
	double		m_dValueThetaAlign;

	MPos_XYT	m_rgPanelCenterOffset;//PanelAlign한후 Panel의 중심위치를 구한다
	
	// 각 작업변 Tab의 작업 시작 번호/ 끝 번호
	int		m_iSourceWorkTabStart[DEF_MAX_WORK_SCHEDULE];
	int		m_iSourceWorkTabLast[DEF_MAX_WORK_SCHEDULE];

/************************************************************************/
/*        기타
/************************************************************************/
// jdy sesl	char		m_szLogMsg[250];

private:

/**********
 *  Mechanical Layer State Check Components
 */
	// 설비별 Unit 설정.

	/** Panel Transfer */
#ifdef DEF_SOURCE_SYSTEM
	IStatePanelTransfer*	m_plnkStatePanelTransfer;
#endif
	IStatePanelTransfer*	m_plnkStatePanelTransferOut;

	/** Tab Mounter */
	IStateTabMounter*		m_plnkStateTabMounter[DEF_MAX_TABMOUNTER];
	IStateTabMounter*		m_plnkStateRTabMounter[DEF_MAX_TABMOUNTER];
	IStateCameraCarrier*	m_plnkStateInspectCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	IStateCameraCarrier*	m_plnkStateRInspectCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];

	IStateModelChange*		m_plnkStateModelChange1;
	IStateModelChange*		m_plnkStateModelChange2;

	MTrsAutoManager*		m_plnkTrsAutoManager;
public:

/************************************************************************/
/*        Component 공통 제공 Interface                                 */
/************************************************************************/

	/**
	 * PanelAligner에 Component List를 설정한다.
	 *
	 * @param	SPanelAlignerRefCompList : 설정할 PanelAligner Component List
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AssignComponents(SPanelAlignerRefCompList PanelAlignerCompList);

	int AssignProcess(SPanelAlignerRefProcess PanelAlignerRefProcess);


	/**
	 * PanelAligner의 Motion Component를 돌려 받는다.
	 *
	 * @param nAxisID : 축 ID (0: X축, 1:Y축, 2:T축)
	 * @return	IAixs : 해당하는 축 ID에 맞는 Axis 
	 */
	IAxis* GetMotionComponent(int nAxisID);

	/**
	 * PanelAlignerZ의 Motion Component를 돌려 받는다.
	 *
	 * @return	IAixs : 해당하는 축 ID에 맞는 Axis 
	 */
	IAxis* GetZMotionComponent();

	/**
	 * BackupX의 Motion Component를 돌려 받는다.
	 *
	 * @return	IAixs : 해당하는 축 ID에 맞는 Axis 
	 */
	IAxis* GetBackupXMotionComponent();

	/**
	 * PanelAligner의 MultiMotion Component를 돌려 받는다.
	 *
	 * @return	ISercosMultiAxis : MultiAxis
	 */
	//161024 Modify...
	//ISercosMultiAxis* GetMultiMotionComponent();
	IRSNMCMultiAxis*	GetMultiMotionComponent();

/************************************************************************/
/*        Teaching 관련 제공 Interface                                  */
/************************************************************************/
	/* PanelAligner의 XYT 축 관련 고정좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadXYTFixedPosParameter(void);
	
	int LoadZFixedPosParameter(void);

//@	int LoadBackupMoveXFixedPosParameter(void);

	/**
	 * PanelAligner의 XYT 축 관련 Offset좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadXYTOffsetPosParameter(void);

	int LoadZOffsetPosParameter(void);

//@	int LoadBackupMoveXOffsetPosParameter(void);


	/**
	 * PanelAligner의 XYT 축 관련 고정좌표 Data를 File에 저장한다.
	 *
	 * @param	rgsFixedPos : 저장할 고정좌표 배열 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveXYTFixedPosParameter(MPos_XYT* rgsFixedPos);

	int SaveZFixedPosParameter(double* rgdFixedPos);

//@	int SaveBackupMoveXFixedPosParameter(double* rgdFixedPos);

	/**
	 * PanelAligner의 XYT 축 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	rgsOffsetPos : 저장할 Offset좌표 배열 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveXYTOffsetPosParameter(MPos_XYT* rgsOffsetPos);

	int SaveZOffsetPosParameter(double* rgdOffsetPos);

//@	int SaveBackupMoveXOffsetPosParameter(double* rgdOffsetPos);

	/**
	 * PanelAligner의 XYT 축 좌표 관련 Model Data를 반환한다. 
	 *
	 * @param	iPos = Position ID
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	MPos_XYT GetXYTModelPosParameter(int iPos);

	double	GetZModelPosParameter(int iPos);

//@	double	GetBackupMoveXModelPosParameter(int iPos);


	/**
	 * PanelAligner의 XYT 축 좌표 관련 Offset Data를 반환한다. 
	 *
	 * @param	iPos = Position ID
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	MPos_XYT GetXYTOffsetPosParameter(int iPos);

	double	GetZOffsetPosParameter(int iPos);

//@	double	GetBackupMoveXOffsetPosParameter(int iPos);

	/**
	 * PanelAligner의 XYT 축 좌표 관련 모든 Data를 반환 한다.
	 *
	 * @param	rgsFixedPos : 고정좌표 배열 (X, Y, T)
	 * @param	rgsOffsetPos : Offset좌표 배열 (X, Y, T)
	 * @param	rgsModelPos : Model좌표 배열 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int GetXYTPosParameter(	MPos_XYT* rgsFixedPos = NULL,
							MPos_XYT* rgsOffsetPos = NULL,
							MPos_XYT* rgsModelPos = NULL);

	int GetZPosParameter(	double* rgdFixedPos = NULL,
							double* rgdOffsetPos = NULL,
							double* rgdModelPos = NULL);

//@	int GetBackupMoveXPosParameter(	double* rgdFixedPos = NULL,
//@									double* rgdOffsetPos = NULL,
//@									double* rgdModelPos = NULL);

#ifdef DEF_GATE_SYSTEM
	BOOL IsPanelLoadingDetected();	//Gate에서(Source는 TransferIn에서 LoadingDetected())
#endif
	//120224.kms___________
	BOOL IsPanelWorkingDetected();	//Gate & Source 에서
	//_____________________

	/**
	 * PanelAligner XYT Teaching 정보를 읽는다.
	 *
	 * @return	STeachingInfo : Teaching 정보
	 */
	STeachingInfo GetXYTTeachingInfo();

	STeachingInfo GetZTeachingInfo();

//@	STeachingInfo GetBackupMoveXTeachingInfo();

/************************************************************************/
/*        Vacuum Operation Interface                                    */
/************************************************************************/

	/**
	 * PanelAligner의 Vacuum을 압축한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AbsorbPanel(BOOL bSkipSensor = FALSE);

	/**
	 * PanelAligner의 Vacuum을 해제한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int ReleasePanel(BOOL bSkipSensor = FALSE);

	/**
	 * PanelAligner의 Vacuum이 정상인지 확인한다. 정상이면 Success Return.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckVacuum(void);

	/**
	 * PanelAligner의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
	const BOOL IsPanelAbsorbed(void);

	/**
	 * PanelAligner의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
	const BOOL IsPanelReleased(void);

	/**
	 * PanelAligner의 Plate Unit에서 Air Blow 부상한다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AirCVBlowOn();

	/**
	 * PanelAligner의 Plate Unit에서 Air Blow 부상 해제한다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AirCVBlowOff();

	/**
	 * PanelAligner의 Plate Unit에서 Air Blow 부상 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 부상 상태, FALSE = 해제 상태)
	 */
	BOOL IsAirCVBlowOn();

	/**
	 * PanelAligner의 Plate Unit에서 Air Blow 부상해제 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 부상 상태)
	 */
	BOOL IsAirCVBlowOff();
	
	/**
	 * PanelAligner의 Plate Unit 전체의 Vacuum을 압축한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AirCVVacAllOn(int iPanelWorkNo);

	/**
	 * PanelAligner의 Plate Unit 전체의 Vacuum을 해제한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AirCVVacAllOff();

	/**
	 * PanelAligner의 Plate Unit 전체의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
	BOOL IsAirCVVacAllOn(int iPanelWorkNo);

	/**
	 * PanelAligner의 Plate Unit 전체의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
	BOOL IsAirCVVacAllOff();

	/**
	 * PanelAligner의 Plate Unit 1의 Vacuum을 압축한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AirCVVacPlate1On();

	/**
	 * PanelAligner의 Plate Unit 1의 Vacuum을 해제한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AirCVVacPlate1Off();

	/**
	 * PanelAligner의 Plate Unit 1의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
	BOOL IsAirCVVacPlate1On();

	/**
	 * PanelAligner의 Plate Unit 1의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
	BOOL IsAirCVVacPlate1Off();

	/**
	 * PanelAligner의 Plate Unit 2의 Vacuum을 압축한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AirCVVacPlate2On();

	/**
	 * PanelAligner의 Plate Unit 2의 Vacuum을 해제한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AirCVVacPlate2Off();

	/**
	 * PanelAligner의 Plate Unit 2의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
	BOOL IsAirCVVacPlate2On();

	/**
	 * PanelAligner의 Plate Unit 2의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
	BOOL IsAirCVVacPlate2Off();

//#if ( defined(DEF_PLATFORM_PREBONDER_SYSTEM) && defined(DEF_SOURCE_SYSTEM) )
	/**
	 * PanelAligner의 Plate Unit 3의 Vacuum을 압축한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AirCVVacPlate3On();

	/**
	 * PanelAligner의 Plate Unit 3의 Vacuum을 해제한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AirCVVacPlate3Off();

	/**
	 * PanelAligner의 Plate Unit 2의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
	BOOL IsAirCVVacPlate3On();

	/**
	 * PanelAligner의 Plate Unit 2의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
	BOOL IsAirCVVacPlate3Off();


	int AirCVVacPlate4On();
	int AirCVVacPlate4Off();
	BOOL IsAirCVVacPlate4On();
	BOOL IsAirCVVacPlate4Off();


	int AirCVBlowOn1();
	int AirCVBlowOff1();
	int AirCVBlowOn2();
	int AirCVBlowOff2();
//@	int AirCVBlowOn3();
//@	int AirCVBlowOff3();
//@	int AirCVBlowOn4();
//@	int AirCVBlowOff4();

	// Air Conveyor 부상
	int AirCVFloatingOn();

	// Air Conveyor 흡착
	int AirCVFloatingOff();

	//170724_KDH Spot Cleaner 추가
	int SpotCleanerOn();
	int SpotCleanerOff();
	BOOL IsSpotCleanerState();


//@	int AbsorbPanelRotator(BOOL bSkipSensor = FALSE);
//@	int ReleasePanelRotator(BOOL bSkipSensor = FALSE);
//@	int CheckVacuumRotator(void);
//@	const BOOL IsPanelAbsorbedRotator(void);
//@	const BOOL IsPanelReleasedRotator(void);

/************************************************************************/
/*        Cylinder Operation Interface                                  */
/************************************************************************/

	/**
	 * PanelAligner 의 Panel Pickup Unit Up/Down Cylinder 를 Up 시킨다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int UpPickUpUnit(BOOL bWaitOption = TRUE);

	/**
	 * PanelAligner Panel Pickup Unit 를 Down 시킨다.
	 *
	 * @return	int : (0 = Success, Er ror Code = 그 외)
	 */
	int DownPickUpUnit(BOOL bWaitOption = TRUE);

	/**
	 * PanelAligner Panel Pickup Unit Up/Down Cylinder의 U/D 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Up Output I/O On , FALSE = Up Output I/O Not On)
	 */
	BOOL IsUpPickUpUnit(void);

	/**
	 * PanelAligner Panel Pickup Unit Up/Down Cylinder의 U/D 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Down , FALSE = Not Down)
	 */
	BOOL IsDownPickUpUnit(void);


//@	int UpRotatorUnit(BOOL bSkipSensor = FALSE);
//@	int DownRotatorUnit(BOOL bSkipSensor = FALSE);
//@	BOOL IsUpRotatorUnit(void);
//@	BOOL IsDownRotatorUnit(void);

//@	int TurnRotatorUnit(BOOL bSkipSensor = FALSE);
//@	int ReturnRotatorUnit(BOOL bSkipSensor = FALSE);
//@	BOOL IsTurnRotatorUnit(void);
//@	BOOL IsReturnRotatorUnit(void);

	/**
	 * PanelAligner Panel Pickup Unit 를 Forward 시킨다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) 실린더 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Er ror Code = 그 외)
	 */

	/**
	 * PanelAligner Panel Pickup Unit 를 Backward 시킨다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) 실린더 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Er ror Code = 그 외)
	 */

	/**
	 * PanelAligner Panel Pickup Unit Up/Down Cylinder의 F/B 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Forward , FALSE = Not Forward)
	 */

	/**
	 * PanelAligner Panel Pickup Unit Up/Down Cylinder의 F/B 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Backward , FALSE = Not Backward)
	 */

/************************************************************************/
/*        PanelAligner Move Operation Interface							*/
/************************************************************************/

	/**
	 * PanelAligner(X,Y,T축)를 Index위치로 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bUseAuto : Manual Mode로 사용 할 것인지 않할 것인지 확인 (FLASE : Manual Mode, FALSE : Auto Mode)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SafeMoveXYTPos(int iPosID, BOOL bUseAuto = TRUE);

	/**
	 * PanelAligner(MultiXT,Y축)를 Index위치로 동기이동시킨다. Interlock을 확인한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bUseAuto : Manual Mode로 사용 할 것인지 않할 것인지 확인 (FLASE : Manual Mode, FALSE : Auto Mode)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SafeSyncMoveXYTPos(int iPosID, BOOL bUseAuto = TRUE);

	int RMoveXPosOnly(double dXDistance, int iPosID = DEF_PANEL_ALIGNER_NONE_POS, BOOL bUseAuto = TRUE);

	int RMoveYPosOnly(double dYDistance, int iPosID = DEF_PANEL_ALIGNER_NONE_POS, BOOL bUseAuto = TRUE);

	int SafeMoveZPos(int iPosID, BOOL bUseAuto = TRUE, BOOL bWaitOption = TRUE);

//@	int SafeMoveBackupMoveXPos(int iPosID, BOOL bUseAuto = TRUE);

/************************************************************************/
/*        PanelAligner Position 확인 Interface                                 */
/************************************************************************/

	/**
	 * 원점복귀 여부를 확인한다. 모두 원점복귀가 되어 있으면 Success Return, 
	 * 아니면 안되어 있는 축 Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckOrigin(void);

	int CheckOriginZ(void);

//@	int CheckOriginBackupMoveX(void);

	/**
	 * PanelAligner(X, Y, T축)의 현재 위치를 읽는다.
	 *
	 * @param	bType		: (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	MPos_XYT : PanelAligner의 현재 위치 (X, Y, T축)
	 */
	MPos_XYT GetXYTCurrentPos(BOOL bType = FALSE);

	double	GetZCurrentPos(BOOL bType = FALSE);

//@	double	GetBackupMoveXCurrentPos(BOOL bType = FALSE);

	/**
	 * PanelAligner(X, Y, T축)의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	MPos_XYT : PanelAligner의 목표 위치 (X, Y, T축)
	 */
	MPos_XYT GetXYTTargetPos(int iPosID);

	double	GetZTargetPos(int iPosID);

//	double	GetBackupMoveXTargetPos(int iPosID);

	/**
	 * PanelAligner(X, Y, T축)의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInXYTPos(int iPosID, MPos_XYT* pTolerance = NULL);

	BOOL IsInZPos(int iPosID, double* pTolerance = NULL);

//@	BOOL IsInBackupMoveXPos(int iPosID, double* pTolerance = NULL);

	/**
	 * PanelAligner(X, Y, T축)의 현재위치와 전달위치를 비교한다.
	 *
	 * @param	sTargetPos : 현재위치와 비교할 전달위치
	 * @param	pTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance = NULL);

	BOOL IsInZPos(double dTargetPos, double* pTolerance = NULL);

//@	BOOL IsInBackupMoveXPos(double dTargetPos, double* pTolerance = NULL);

	/**
	 * PanelAligner X축의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL)  비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInXPos(int iPosID, double* pdTolerance = NULL);

	/**
	 * PanelAligner X축의 현재위치와 전달위치를 비교한다.
	 *
	 * @param	dTargetPos : 현재위치와 비교할 전달위치
	 * @param	pdTolerance : 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInXPos(double dTargetPos, double* pdTolerance = NULL);

	/**
	 * PanelAligner Y축의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInYPos(int iPosID, double* pdTolerance = NULL);

	/**
	 * PanelAligner Y축의 현재위치와 전달위치를 비교한다.
	 *
	 * @param	dTargetPos : 현재위치와 비교할 전달위치
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInYPos(double dTargetPos, double* pdTolerance = NULL);

	/**
	 * PanelAligner T축의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInTPos(int iPosID, double* pdTolerance = NULL);

	/**
	 * PanelAligner T축의 현재위치와 전달위치를 비교한다.
	 *
	 * @param	dTargetPos : 현재위치와 비교할 전달위치
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInTPos(double dTargetPos, double* pdTolerance = NULL);

	/**
	 *	PanelAligner XYT Motion 중인지 확인한다.
	 */
	BOOL IsInXYTMotion();	

/************************************************************************/
/*        Align Operation Interface										*/
/************************************************************************/

	/**
	 * Align 결과인 Align Mark Offset을 초기화한다.
	 * Align되었다는 결과 Flag인 m_bMarkAligned를 FALSE로 설정하고
	 * Align Mark Offset을 "0.0"으로 설정한다.
	 * Thread에서 PanelAligner를 진행할 때 Loading Position으로 이동하면서 사용한다. 
	 */
	void InitAlignMarkOffset(void);

	/**
	 * 기록된 Align 결과인 Align Mark Offset을 읽는다.
	 * Align 결과가 설정되어 있으면 SUCCESS를 return하고 Align Mark Offset을 전달한다.
	 * Align 결과가 설정되어 있지 않으면 Error를 return하고 *psPos에는 0.0을 설정한다. 
	 *
	 * @param	*posxytTarget: 목표 위치 값
	 * @return	MPos_XYT	 : 설정되어 있는 Align Offset 좌표 값
	 */
	MPos_XYT CalculateAlignMarkOffset(MPos_XYT posxytTarget);
	
	/**
	 * Align Mark Offset을 설정한다.
	 * m_bMarkAligned을 TRUE로 설정한다.
	 * Align Panel에서 Align 수행 후 기록해주어야 하는 함수 
	 *
	 * @param	sPos		: 설정할 Alaign Mark Offset
	 * @param	dValueThetaAlign	: Align시의  T축의 설정값
	 * @return	int	: 0 = SUCCESS, 그 외 = Error
	 */
	void SetAlignMarkOffset(MPos_XYT sPos, double dValueThetaAlign);
	
	/**
	 * 기록된 Align 결과인 Align Mark Offset을 읽는다.
	 * Align 결과가 설정되어 있으면 SUCCESS를 return하고 Align Mark Offset을 전달한다.
	 * Align 결과가 설정되어 있지 않으면 Error를 return하고 *psPos에는 0.0을 설정한다. 
	 *
	 * @return	Error Code	: 0 = SUCCESS, 그 외 = Error
	 */
	MPos_XYT GetAlignMarkOffset();
	
	/**
	 * Align 수행여부를 알린다.
	 *
	 * @return BOOL		: TRUE(Aligned), FALSE(Not Aligned)
	 */
	BOOL IsAligned();

//	void		CalculatePanelCenterOffset();
//	MPos_XYT	GetPanelCenterOffset();
//	MPos_XYT	GetPanelCenterPosition(MPos_XYT ComparePos);
//	MPos_XYT	GetPanelRotationPosition();
//	MPos_XYT	ConvertPanelAlignerToRotator(MPos_XYT PanelAlignerTgPos);
//	MPos_XYT	ConvertRotatorToPanelAligner(MPos_XYT RotatorTgPos);
	//110102 HJG
#ifdef DEF_USE_PANEL_PUSHER
	int UpPanelPusherUDCyl(BOOL bSkipSensor = FALSE);
	int DownPanelPusherUDCyl(BOOL bSkipSensor = FALSE);
	BOOL IsUpPanelPusherUDCyl();
	BOOL IsDownPanelPusherUDCyl();
#endif

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
private:
	/**
	 * PanelAligner(X,Y,T축) 이동전에 Interlock을 확인한다.
	 *
	 * @param	iPosID		: 목표 위치 ID
	 * @param	posTarget	: Target 좌표
	 * @param	bUseAuto	: Run Mode (default = TRUE)
	 * @return	int (0 = Safety, Error Code = 그 외)
	 */
	int checkInterlockForXYT(int iPosID, MPos_XYT posTarget, BOOL bUseAuto = TRUE);

	int checkInterlockWithTransferCenter(int iPosID, MPos_XYT posTarget, BOOL bUseAuto);
	int	checkInterlockWithTransferIn(int iPosID, MPos_XYT posTarget, BOOL bUseAuto);
	int	checkInterlockWithTransferOut(int iPosID, MPos_XYT posTarget, BOOL bUseAuto);
	int	checkInterlockWithAirFloat(int iPosID, MPos_XYT posTarget, BOOL bUseAuto);
	int checkInterlockWithACFUnit(int iPosID, MPos_XYT posTarget, BOOL bUseAuto);
	int checkInterlockWithPRBUnit(int iPosID, MPos_XYT posTarget, BOOL bUseAuto);
	int checkInterlockWithFNBUnit(int iPosID, MPos_XYT posTarget, BOOL bUseAuto);
	int checkInterlockWithPrevUnit(int iPosID, MPos_XYT posTarget, BOOL bUseAuto);
	int checkInterlockWithNextUnit(int iPosID, MPos_XYT posTarget, BOOL bUseAuto);

	int checkInterlockForZ(int iPosID, double posTarget, BOOL bUseAuto = TRUE);

//@	int checkInterlockForBackupMoveX(int iPosID, double posTarget, BOOL bUseAuto = TRUE);
	
	/**
	 * PanelAligner XYT축 관련 좌표를 계산한다.
	 *
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int calculatePanelAlignerXYTModelPosParameter();

	/**
	 * PanelAligner T축 각도를 이동길이로 변환한다.
	 *
	 * @return	double : 길이
	 */
//	double convertTtoX2(double dDegreeT);
	double convertTtoY2(double dDegreeT);

	/**
	 * PanelAligner T축 이동길이를 각도로 변환한다.
	 *
	 * @return	double : 각도
	 */
//	double convertX2toT(double dX);
	double convertY2toT(double dY);

public:
	/**
	 * PanelAligner T축의 현재 위치를 읽는다.
	 *
	 * @param	bType	: (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double	: PanelAligner T축의 현재 위치
	 */
//	double GetPanelAlignerX2CurrentPos(BOOL bType = FALSE);
	double GetPanelAlignerY2CurrentPos(BOOL bType = FALSE);

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

	/** 
	 * 생성자
	 * @param commonData		: ObjectI나 Log와 같은 Component Common Data
	 * @param listRefComponent	: 참조할 Component 포인터 리스트
	 * @param SMainToolData		: MainTool Data
	 */
	MPanelAligner(SCommonAttribute commonData, SPanelAlignerRefCompList listRefComponents, SPanelAlignerData datComponent);

	/** 소멸자 함수 */
	~MPanelAligner();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

	/**
	 * PanelAligner에 Data Parameter를 설정한다. 
	 *
	 * @param	SPanelAlignerParam	: 설정할 PanelAligner Parameter
	 * @return	Error Code	: 0 = SUCCESS, 그 외 = Error
	 */
	int	SetData(SPanelAlignerData datcomponent);

	/**
	 * PanelAligner가 상태 체크를 위해 필요한  Component List를 Assign 한다. 
	 *
	 * @param	SStateCheckCompList : Component List
	 */
	int	AssignStateCheckComponents(SStateCheckCompListForPanelAligner sCompList);

	/**
	 * Model 변경시 작업을 수행한다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int	ChangeModel();

	/**
	 * PanelAligner 운전 초기 상태로 이동한다.
	 *
	 * @return	int : 0 = SUCCESS, 그 외 = Error
	 */
	int	Initialize();

/***********************************************************************************************
/*
/* Operation Interface
/*
/************************************************************************************************/ 

/**********
 * Move
 */

	/**
	 * PanelAligner(X,Y,T축)를 Index위치로 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	posTarget	: Target 좌표
	 * @param	iPosID		: Position Index ID
	 * @param	bUseAuto	: Auto mode 사용 여부를 설정 (default = TRUE)
	 * @param	bOnMarkAutoSearch : Panel Fiducial Mark Auto Search 를 위한 Motion 동작 여부 설정 (default = FALSE)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int MoveXYTPos(MPos_XYT posTarget, int iPosID = -1, BOOL bUseAuto = TRUE, BOOL bOnMarkAutoSearch = FALSE);

	int MoveZPos(double posTarget, int iPosID = -1, BOOL bUseAuto = TRUE, BOOL bWaitOption = TRUE);

	int Wait4DoneZ(BOOL bSkipMode = FALSE);

//@	int MoveBackupMoveXPos(double posTarget, int iPosID = -1, BOOL bUseAuto = TRUE);

	/**
	 * PanelAligner(MultiXT,Y축)를 Index위치로 동기이동시킨다. Interlock을 확인한다.
	 *
	 * @param	posTarget	: Target 좌표
	 * @param	iPosID		: Position Index ID
	 * @param	bUseAuto	: Auto mode 사용 여부를 설정 (default = TRUE)
	 * @param	bOnMarkAutoSearch : Panel Fiducial Mark Auto Search 를 위한 Motion 동작 여부 설정 (default = FALSE)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SyncMoveXYTPos(MPos_XYT posTarget, int iPosID = -1, BOOL bUseAuto = TRUE, BOOL bOnMarkAutoSearch = FALSE);

	//120508.KMS____________
	BOOL IsIonizerState();

	int	GetPanelWorkNoFromCurPos();

	int	CalculateWorkTabNo();
	int GetStartTabNo(int iPressWorkNo);
	int GetLastTabNo(int iPressWorkNo);
};
#endif //M_PANEL_ALIGNER_H
