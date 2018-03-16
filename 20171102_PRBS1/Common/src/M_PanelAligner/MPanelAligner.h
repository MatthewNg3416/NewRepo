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

/** XYTPanelTransferX 4���� ������ PanelAligner Unit�� �𵨸��� Class�̴�. */
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
//110102 HJG �߰�
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

	//170724_KDH Spot Cleaner �߰�
	unsigned short		m_usOAddrSpotCleanerOn;
	unsigned short		m_usIAddrSpotCleanerState;
	//_________________________


/************************************************************************/
/*                             Position                                 */
/************************************************************************/
	CString		m_strPosFileName;
	CString		m_strOffsetPosFileName;
	CString		m_strOffsetPosFilePath;

	/** PanelAligner X, Y, T, Z �� Position ���� */
    STeachingInfo	m_sXYTZTeachingInfo;

	/** PanelAligner Z �� Position ���� */
	STeachingInfo	m_sZTeachingInfo;

	/** PanelAligner XYT�� ������ǥ�� */
	MPos_XYT	m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_MAX_POSITION];
	/** PanelAligner XYT�� ����ǥ�� */
	MPos_XYT	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_MAX_POSITION];
	/** PanelAligner XYT�� Offset ��ǥ�� */
	MPos_XYT	m_rgsXYTOffsetPos[DEF_PANEL_ALIGNER_MAX_POSITION];

	/** PanelAligner Z�� ���� ��ǥ */
	double		m_rgdZFixedPos[DEF_PANEL_ALIGNER_Z_MAX_POSITION];
	/** PanelAligner Z�� Offset ��ǥ */
	double		m_rgdZOffsetPos[DEF_PANEL_ALIGNER_Z_MAX_POSITION];
	/** PanelAligner Z�� �� ��ǥ */
	double		m_rgdZModelPos[DEF_PANEL_ALIGNER_Z_MAX_POSITION];

/************************************************************************/
/*                             Align                                    */
/************************************************************************/
	/** PanelAligner Align Mark Offset*/
	MPos_XYT	m_rgAlignMarkOffset;

	/** Align ���� ���� */
	BOOL		m_bMarkAligned;

	/** Align�ϱ� ���� T �� �� */
	double		m_dValueThetaAlign;

	MPos_XYT	m_rgPanelCenterOffset;//PanelAlign���� Panel�� �߽���ġ�� ���Ѵ�
	
	// �� �۾��� Tab�� �۾� ���� ��ȣ/ �� ��ȣ
	int		m_iSourceWorkTabStart[DEF_MAX_WORK_SCHEDULE];
	int		m_iSourceWorkTabLast[DEF_MAX_WORK_SCHEDULE];

/************************************************************************/
/*        ��Ÿ
/************************************************************************/
// jdy sesl	char		m_szLogMsg[250];

private:

/**********
 *  Mechanical Layer State Check Components
 */
	// ���� Unit ����.

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
/*        Component ���� ���� Interface                                 */
/************************************************************************/

	/**
	 * PanelAligner�� Component List�� �����Ѵ�.
	 *
	 * @param	SPanelAlignerRefCompList : ������ PanelAligner Component List
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AssignComponents(SPanelAlignerRefCompList PanelAlignerCompList);

	int AssignProcess(SPanelAlignerRefProcess PanelAlignerRefProcess);


	/**
	 * PanelAligner�� Motion Component�� ���� �޴´�.
	 *
	 * @param nAxisID : �� ID (0: X��, 1:Y��, 2:T��)
	 * @return	IAixs : �ش��ϴ� �� ID�� �´� Axis 
	 */
	IAxis* GetMotionComponent(int nAxisID);

	/**
	 * PanelAlignerZ�� Motion Component�� ���� �޴´�.
	 *
	 * @return	IAixs : �ش��ϴ� �� ID�� �´� Axis 
	 */
	IAxis* GetZMotionComponent();

	/**
	 * BackupX�� Motion Component�� ���� �޴´�.
	 *
	 * @return	IAixs : �ش��ϴ� �� ID�� �´� Axis 
	 */
	IAxis* GetBackupXMotionComponent();

	/**
	 * PanelAligner�� MultiMotion Component�� ���� �޴´�.
	 *
	 * @return	ISercosMultiAxis : MultiAxis
	 */
	//161024 Modify...
	//ISercosMultiAxis* GetMultiMotionComponent();
	IRSNMCMultiAxis*	GetMultiMotionComponent();

/************************************************************************/
/*        Teaching ���� ���� Interface                                  */
/************************************************************************/
	/* PanelAligner�� XYT �� ���� ������ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadXYTFixedPosParameter(void);
	
	int LoadZFixedPosParameter(void);

//@	int LoadBackupMoveXFixedPosParameter(void);

	/**
	 * PanelAligner�� XYT �� ���� Offset��ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadXYTOffsetPosParameter(void);

	int LoadZOffsetPosParameter(void);

//@	int LoadBackupMoveXOffsetPosParameter(void);


	/**
	 * PanelAligner�� XYT �� ���� ������ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgsFixedPos : ������ ������ǥ �迭 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveXYTFixedPosParameter(MPos_XYT* rgsFixedPos);

	int SaveZFixedPosParameter(double* rgdFixedPos);

//@	int SaveBackupMoveXFixedPosParameter(double* rgdFixedPos);

	/**
	 * PanelAligner�� XYT �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgsOffsetPos : ������ Offset��ǥ �迭 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveXYTOffsetPosParameter(MPos_XYT* rgsOffsetPos);

	int SaveZOffsetPosParameter(double* rgdOffsetPos);

//@	int SaveBackupMoveXOffsetPosParameter(double* rgdOffsetPos);

	/**
	 * PanelAligner�� XYT �� ��ǥ ���� Model Data�� ��ȯ�Ѵ�. 
	 *
	 * @param	iPos = Position ID
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	MPos_XYT GetXYTModelPosParameter(int iPos);

	double	GetZModelPosParameter(int iPos);

//@	double	GetBackupMoveXModelPosParameter(int iPos);


	/**
	 * PanelAligner�� XYT �� ��ǥ ���� Offset Data�� ��ȯ�Ѵ�. 
	 *
	 * @param	iPos = Position ID
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	MPos_XYT GetXYTOffsetPosParameter(int iPos);

	double	GetZOffsetPosParameter(int iPos);

//@	double	GetBackupMoveXOffsetPosParameter(int iPos);

	/**
	 * PanelAligner�� XYT �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
	 *
	 * @param	rgsFixedPos : ������ǥ �迭 (X, Y, T)
	 * @param	rgsOffsetPos : Offset��ǥ �迭 (X, Y, T)
	 * @param	rgsModelPos : Model��ǥ �迭 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = �� ��)
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
	BOOL IsPanelLoadingDetected();	//Gate����(Source�� TransferIn���� LoadingDetected())
#endif
	//120224.kms___________
	BOOL IsPanelWorkingDetected();	//Gate & Source ����
	//_____________________

	/**
	 * PanelAligner XYT Teaching ������ �д´�.
	 *
	 * @return	STeachingInfo : Teaching ����
	 */
	STeachingInfo GetXYTTeachingInfo();

	STeachingInfo GetZTeachingInfo();

//@	STeachingInfo GetBackupMoveXTeachingInfo();

/************************************************************************/
/*        Vacuum Operation Interface                                    */
/************************************************************************/

	/**
	 * PanelAligner�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AbsorbPanel(BOOL bSkipSensor = FALSE);

	/**
	 * PanelAligner�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int ReleasePanel(BOOL bSkipSensor = FALSE);

	/**
	 * PanelAligner�� Vacuum�� �������� Ȯ���Ѵ�. �����̸� Success Return.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckVacuum(void);

	/**
	 * PanelAligner�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	const BOOL IsPanelAbsorbed(void);

	/**
	 * PanelAligner�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	const BOOL IsPanelReleased(void);

	/**
	 * PanelAligner�� Plate Unit���� Air Blow �λ��Ѵ�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AirCVBlowOn();

	/**
	 * PanelAligner�� Plate Unit���� Air Blow �λ� �����Ѵ�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AirCVBlowOff();

	/**
	 * PanelAligner�� Plate Unit���� Air Blow �λ� ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = �λ� ����, FALSE = ���� ����)
	 */
	BOOL IsAirCVBlowOn();

	/**
	 * PanelAligner�� Plate Unit���� Air Blow �λ����� ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = �λ� ����)
	 */
	BOOL IsAirCVBlowOff();
	
	/**
	 * PanelAligner�� Plate Unit ��ü�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AirCVVacAllOn(int iPanelWorkNo);

	/**
	 * PanelAligner�� Plate Unit ��ü�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AirCVVacAllOff();

	/**
	 * PanelAligner�� Plate Unit ��ü�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsAirCVVacAllOn(int iPanelWorkNo);

	/**
	 * PanelAligner�� Plate Unit ��ü�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsAirCVVacAllOff();

	/**
	 * PanelAligner�� Plate Unit 1�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AirCVVacPlate1On();

	/**
	 * PanelAligner�� Plate Unit 1�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AirCVVacPlate1Off();

	/**
	 * PanelAligner�� Plate Unit 1�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsAirCVVacPlate1On();

	/**
	 * PanelAligner�� Plate Unit 1�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsAirCVVacPlate1Off();

	/**
	 * PanelAligner�� Plate Unit 2�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AirCVVacPlate2On();

	/**
	 * PanelAligner�� Plate Unit 2�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AirCVVacPlate2Off();

	/**
	 * PanelAligner�� Plate Unit 2�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsAirCVVacPlate2On();

	/**
	 * PanelAligner�� Plate Unit 2�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsAirCVVacPlate2Off();

//#if ( defined(DEF_PLATFORM_PREBONDER_SYSTEM) && defined(DEF_SOURCE_SYSTEM) )
	/**
	 * PanelAligner�� Plate Unit 3�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AirCVVacPlate3On();

	/**
	 * PanelAligner�� Plate Unit 3�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AirCVVacPlate3Off();

	/**
	 * PanelAligner�� Plate Unit 2�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsAirCVVacPlate3On();

	/**
	 * PanelAligner�� Plate Unit 2�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
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

	// Air Conveyor �λ�
	int AirCVFloatingOn();

	// Air Conveyor ����
	int AirCVFloatingOff();

	//170724_KDH Spot Cleaner �߰�
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
	 * PanelAligner �� Panel Pickup Unit Up/Down Cylinder �� Up ��Ų��.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int UpPickUpUnit(BOOL bWaitOption = TRUE);

	/**
	 * PanelAligner Panel Pickup Unit �� Down ��Ų��.
	 *
	 * @return	int : (0 = Success, Er ror Code = �� ��)
	 */
	int DownPickUpUnit(BOOL bWaitOption = TRUE);

	/**
	 * PanelAligner Panel Pickup Unit Up/Down Cylinder�� U/D ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Up Output I/O On , FALSE = Up Output I/O Not On)
	 */
	BOOL IsUpPickUpUnit(void);

	/**
	 * PanelAligner Panel Pickup Unit Up/Down Cylinder�� U/D ���¸� Ȯ���Ѵ�.
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
	 * PanelAligner Panel Pickup Unit �� Forward ��Ų��.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) �Ǹ��� ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Er ror Code = �� ��)
	 */

	/**
	 * PanelAligner Panel Pickup Unit �� Backward ��Ų��.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) �Ǹ��� ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Er ror Code = �� ��)
	 */

	/**
	 * PanelAligner Panel Pickup Unit Up/Down Cylinder�� F/B ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Forward , FALSE = Not Forward)
	 */

	/**
	 * PanelAligner Panel Pickup Unit Up/Down Cylinder�� F/B ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Backward , FALSE = Not Backward)
	 */

/************************************************************************/
/*        PanelAligner Move Operation Interface							*/
/************************************************************************/

	/**
	 * PanelAligner(X,Y,T��)�� Index��ġ�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bUseAuto : Manual Mode�� ��� �� ������ ���� ������ Ȯ�� (FLASE : Manual Mode, FALSE : Auto Mode)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SafeMoveXYTPos(int iPosID, BOOL bUseAuto = TRUE);

	/**
	 * PanelAligner(MultiXT,Y��)�� Index��ġ�� �����̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bUseAuto : Manual Mode�� ��� �� ������ ���� ������ Ȯ�� (FLASE : Manual Mode, FALSE : Auto Mode)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SafeSyncMoveXYTPos(int iPosID, BOOL bUseAuto = TRUE);

	int RMoveXPosOnly(double dXDistance, int iPosID = DEF_PANEL_ALIGNER_NONE_POS, BOOL bUseAuto = TRUE);

	int RMoveYPosOnly(double dYDistance, int iPosID = DEF_PANEL_ALIGNER_NONE_POS, BOOL bUseAuto = TRUE);

	int SafeMoveZPos(int iPosID, BOOL bUseAuto = TRUE, BOOL bWaitOption = TRUE);

//@	int SafeMoveBackupMoveXPos(int iPosID, BOOL bUseAuto = TRUE);

/************************************************************************/
/*        PanelAligner Position Ȯ�� Interface                                 */
/************************************************************************/

	/**
	 * �������� ���θ� Ȯ���Ѵ�. ��� �������Ͱ� �Ǿ� ������ Success Return, 
	 * �ƴϸ� �ȵǾ� �ִ� �� Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckOrigin(void);

	int CheckOriginZ(void);

//@	int CheckOriginBackupMoveX(void);

	/**
	 * PanelAligner(X, Y, T��)�� ���� ��ġ�� �д´�.
	 *
	 * @param	bType		: (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	MPos_XYT : PanelAligner�� ���� ��ġ (X, Y, T��)
	 */
	MPos_XYT GetXYTCurrentPos(BOOL bType = FALSE);

	double	GetZCurrentPos(BOOL bType = FALSE);

//@	double	GetBackupMoveXCurrentPos(BOOL bType = FALSE);

	/**
	 * PanelAligner(X, Y, T��)�� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	MPos_XYT : PanelAligner�� ��ǥ ��ġ (X, Y, T��)
	 */
	MPos_XYT GetXYTTargetPos(int iPosID);

	double	GetZTargetPos(int iPosID);

//	double	GetBackupMoveXTargetPos(int iPosID);

	/**
	 * PanelAligner(X, Y, T��)�� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInXYTPos(int iPosID, MPos_XYT* pTolerance = NULL);

	BOOL IsInZPos(int iPosID, double* pTolerance = NULL);

//@	BOOL IsInBackupMoveXPos(int iPosID, double* pTolerance = NULL);

	/**
	 * PanelAligner(X, Y, T��)�� ������ġ�� ������ġ�� ���Ѵ�.
	 *
	 * @param	sTargetPos : ������ġ�� ���� ������ġ
	 * @param	pTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance = NULL);

	BOOL IsInZPos(double dTargetPos, double* pTolerance = NULL);

//@	BOOL IsInBackupMoveXPos(double dTargetPos, double* pTolerance = NULL);

	/**
	 * PanelAligner X���� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL)  �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInXPos(int iPosID, double* pdTolerance = NULL);

	/**
	 * PanelAligner X���� ������ġ�� ������ġ�� ���Ѵ�.
	 *
	 * @param	dTargetPos : ������ġ�� ���� ������ġ
	 * @param	pdTolerance : �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInXPos(double dTargetPos, double* pdTolerance = NULL);

	/**
	 * PanelAligner Y���� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInYPos(int iPosID, double* pdTolerance = NULL);

	/**
	 * PanelAligner Y���� ������ġ�� ������ġ�� ���Ѵ�.
	 *
	 * @param	dTargetPos : ������ġ�� ���� ������ġ
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInYPos(double dTargetPos, double* pdTolerance = NULL);

	/**
	 * PanelAligner T���� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInTPos(int iPosID, double* pdTolerance = NULL);

	/**
	 * PanelAligner T���� ������ġ�� ������ġ�� ���Ѵ�.
	 *
	 * @param	dTargetPos : ������ġ�� ���� ������ġ
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInTPos(double dTargetPos, double* pdTolerance = NULL);

	/**
	 *	PanelAligner XYT Motion ������ Ȯ���Ѵ�.
	 */
	BOOL IsInXYTMotion();	

/************************************************************************/
/*        Align Operation Interface										*/
/************************************************************************/

	/**
	 * Align ����� Align Mark Offset�� �ʱ�ȭ�Ѵ�.
	 * Align�Ǿ��ٴ� ��� Flag�� m_bMarkAligned�� FALSE�� �����ϰ�
	 * Align Mark Offset�� "0.0"���� �����Ѵ�.
	 * Thread���� PanelAligner�� ������ �� Loading Position���� �̵��ϸ鼭 ����Ѵ�. 
	 */
	void InitAlignMarkOffset(void);

	/**
	 * ��ϵ� Align ����� Align Mark Offset�� �д´�.
	 * Align ����� �����Ǿ� ������ SUCCESS�� return�ϰ� Align Mark Offset�� �����Ѵ�.
	 * Align ����� �����Ǿ� ���� ������ Error�� return�ϰ� *psPos���� 0.0�� �����Ѵ�. 
	 *
	 * @param	*posxytTarget: ��ǥ ��ġ ��
	 * @return	MPos_XYT	 : �����Ǿ� �ִ� Align Offset ��ǥ ��
	 */
	MPos_XYT CalculateAlignMarkOffset(MPos_XYT posxytTarget);
	
	/**
	 * Align Mark Offset�� �����Ѵ�.
	 * m_bMarkAligned�� TRUE�� �����Ѵ�.
	 * Align Panel���� Align ���� �� ������־�� �ϴ� �Լ� 
	 *
	 * @param	sPos		: ������ Alaign Mark Offset
	 * @param	dValueThetaAlign	: Align����  T���� ������
	 * @return	int	: 0 = SUCCESS, �� �� = Error
	 */
	void SetAlignMarkOffset(MPos_XYT sPos, double dValueThetaAlign);
	
	/**
	 * ��ϵ� Align ����� Align Mark Offset�� �д´�.
	 * Align ����� �����Ǿ� ������ SUCCESS�� return�ϰ� Align Mark Offset�� �����Ѵ�.
	 * Align ����� �����Ǿ� ���� ������ Error�� return�ϰ� *psPos���� 0.0�� �����Ѵ�. 
	 *
	 * @return	Error Code	: 0 = SUCCESS, �� �� = Error
	 */
	MPos_XYT GetAlignMarkOffset();
	
	/**
	 * Align ���࿩�θ� �˸���.
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
	 * PanelAligner(X,Y,T��) �̵����� Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID		: ��ǥ ��ġ ID
	 * @param	posTarget	: Target ��ǥ
	 * @param	bUseAuto	: Run Mode (default = TRUE)
	 * @return	int (0 = Safety, Error Code = �� ��)
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
	 * PanelAligner XYT�� ���� ��ǥ�� ����Ѵ�.
	 *
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int calculatePanelAlignerXYTModelPosParameter();

	/**
	 * PanelAligner T�� ������ �̵����̷� ��ȯ�Ѵ�.
	 *
	 * @return	double : ����
	 */
//	double convertTtoX2(double dDegreeT);
	double convertTtoY2(double dDegreeT);

	/**
	 * PanelAligner T�� �̵����̸� ������ ��ȯ�Ѵ�.
	 *
	 * @return	double : ����
	 */
//	double convertX2toT(double dX);
	double convertY2toT(double dY);

public:
	/**
	 * PanelAligner T���� ���� ��ġ�� �д´�.
	 *
	 * @param	bType	: (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double	: PanelAligner T���� ���� ��ġ
	 */
//	double GetPanelAlignerX2CurrentPos(BOOL bType = FALSE);
	double GetPanelAlignerY2CurrentPos(BOOL bType = FALSE);

/***********************************************************************************************
/*
/* ������ & �Ҹ���
/*
/************************************************************************************************/

	/** 
	 * ������
	 * @param commonData		: ObjectI�� Log�� ���� Component Common Data
	 * @param listRefComponent	: ������ Component ������ ����Ʈ
	 * @param SMainToolData		: MainTool Data
	 */
	MPanelAligner(SCommonAttribute commonData, SPanelAlignerRefCompList listRefComponents, SPanelAlignerData datComponent);

	/** �Ҹ��� �Լ� */
	~MPanelAligner();

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

	/**
	 * PanelAligner�� Data Parameter�� �����Ѵ�. 
	 *
	 * @param	SPanelAlignerParam	: ������ PanelAligner Parameter
	 * @return	Error Code	: 0 = SUCCESS, �� �� = Error
	 */
	int	SetData(SPanelAlignerData datcomponent);

	/**
	 * PanelAligner�� ���� üũ�� ���� �ʿ���  Component List�� Assign �Ѵ�. 
	 *
	 * @param	SStateCheckCompList : Component List
	 */
	int	AssignStateCheckComponents(SStateCheckCompListForPanelAligner sCompList);

	/**
	 * Model ����� �۾��� �����Ѵ�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int	ChangeModel();

	/**
	 * PanelAligner ���� �ʱ� ���·� �̵��Ѵ�.
	 *
	 * @return	int : 0 = SUCCESS, �� �� = Error
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
	 * PanelAligner(X,Y,T��)�� Index��ġ�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	posTarget	: Target ��ǥ
	 * @param	iPosID		: Position Index ID
	 * @param	bUseAuto	: Auto mode ��� ���θ� ���� (default = TRUE)
	 * @param	bOnMarkAutoSearch : Panel Fiducial Mark Auto Search �� ���� Motion ���� ���� ���� (default = FALSE)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int MoveXYTPos(MPos_XYT posTarget, int iPosID = -1, BOOL bUseAuto = TRUE, BOOL bOnMarkAutoSearch = FALSE);

	int MoveZPos(double posTarget, int iPosID = -1, BOOL bUseAuto = TRUE, BOOL bWaitOption = TRUE);

	int Wait4DoneZ(BOOL bSkipMode = FALSE);

//@	int MoveBackupMoveXPos(double posTarget, int iPosID = -1, BOOL bUseAuto = TRUE);

	/**
	 * PanelAligner(MultiXT,Y��)�� Index��ġ�� �����̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	posTarget	: Target ��ǥ
	 * @param	iPosID		: Position Index ID
	 * @param	bUseAuto	: Auto mode ��� ���θ� ���� (default = TRUE)
	 * @param	bOnMarkAutoSearch : Panel Fiducial Mark Auto Search �� ���� Motion ���� ���� ���� (default = FALSE)
	 * @return	int : (0 = Success, Error Code = �� ��)
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
