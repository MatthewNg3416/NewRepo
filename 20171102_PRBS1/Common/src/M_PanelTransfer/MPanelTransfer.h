/* 
 * PanelTransfer Component
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
 * MPanelTransfer.h : Headerfile of stage component.
 */

#ifndef M_TRANSFER_H
#define M_TRANSFER_H

#include "ICommon.h"
#include "DefPanelTransfer.h"
#include "IStatePanelTransfer.h"
#include "IStateCameraCarrier.h"

/** PanelTransfer X ���� ������ PanelTransfer Unit�� �𵨸��� Class�̴�. */
class MPanelTransfer : public IStatePanelTransfer, public virtual ICommon
{
protected:

/************************************************************************/
/*                             IO                                       */
/************************************************************************/
	IIO*		m_plnkIO;

/************************************************************************/
/*                             Vacuum                                   */
/************************************************************************/
	/** PanelTransfer Vacuum */
	IVacuum*	m_plnkVac;
	IVacuum*	m_plnkVac2;

/************************************************************************/
/*                             Cylinder                                 */
/************************************************************************/
	/** PanelTransfer Up/Down Cylinder */
	ICylinder*	m_plnkUDCyl;

	/** Stopper Up/Down Cylinder */
	ICylinder*	m_plnkStopperUDCyl;

	ICylinder*	m_plnkAirRailFB1;
	ICylinder*	m_plnkAirRailFB2;

/************************************************************************/
/*                             Motion                                   */
/************************************************************************/
	/** PanelTransfer X Motion Object */
	IAxis*	m_plnkX1;
	IAxis*	m_plnkX2;

	/** PanelTransfer X MultiMotion Object */
	//ISercosMultiAxis*	m_plnkMultiX;
	IRSNMCMultiAxis*	m_plnkMultiX;

/************************************************************************/
/*                             Data                                     */
/************************************************************************/
	/** System Data Object */
	MSystemData*	m_plnkSystemData;

	/** Panel Data Object */
	MPanelData*		m_plnkPanelData;

/************************************************************************/
/*                             IO Address                               */
/************************************************************************/
	
	/** Air CVBlow Air Blow Output IO Address	*/
//@	unsigned short		m_usOAddrAirBlow1;
//@	unsigned short		m_usOAddrAirBlow2;

	/** Air CVBlow Air Blow Input IO Address	*/
//@	unsigned short		m_usIAddrAirBlow1;
//@	unsigned short		m_usIAddrAirBlow2;

	/** Air CVBlow Air Vacuum Output IO Address	*/
//@	unsigned short		m_usOAddrAirVacuum1;
//@	unsigned short		m_usOAddrAirVacuum2;

	/** Air CVBlow Air Vacuum Input IO Address	*/
//@	unsigned short		m_usIAddrAirVacuum1;
//@	unsigned short		m_usIAddrAirVacuum2;

	unsigned short		m_usIPanelDetect;

	//120224.kms_______
	unsigned short      m_usIPanelDetectLoading;
	unsigned short      m_usIPanelDetectUnLoading;
	//_________________

	// Transfer Type
	UINT				m_uiTransferType;

/************************************************************************/
/*                             Position                                 */
/************************************************************************/
	CString		m_strPosFileName;
	CString		m_strOffsetPosFileName;
	CString		m_strOffsetPosFilePath;

	/** PanelTransfer X�� Teaching ���� */
	STeachingInfo	m_sXTeachingInfo;

	/** PanelTransfer X�� ���� ��ǥ */
	double		m_rgdXFixedPos[DEF_PANEL_TRANSFER_MAX_POSITION];
	/** PanelTransfer X�� Offset ��ǥ */
	double		m_rgdXOffsetPos[DEF_PANEL_TRANSFER_MAX_POSITION];
	/** PanelTransfer X�� �� ��ǥ */
	double		m_rgdXModelPos[DEF_PANEL_TRANSFER_MAX_POSITION];

/************************************************************************/
/*        ��Ÿ
/************************************************************************/
// jdy sesl	char		m_szLogMsg[250];

public:

/************************************************************************/
/*        Component ���� ���� Interface                                 */
/************************************************************************/

	/**
	 * PanelTransfer�� Component List�� �����Ѵ�.
	 *
	 * @param	SPanelTransferRefCompList : ������ PanelTransfer Component List
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AssignComponents(SPanelTransferRefCompList PanelTransferCompList);

	/**
	 * PanelTransfer�� Motion Component�� ���� �޴´�.
	 *
	 * @param nAxisID : �� ID (0: X��, 1:Y�� , 2:T��, 3:PanelX��)
	 * @return	IAixs : �ش��ϴ� �� ID�� �´� Axis 
	 */
	IAxis* GetMotionComponent(int nAxisID);

	/**
	 * PanelTransfer�� MultiMotion Component�� ���� �޴´�.
	 *
	 * @return	ISercosMultiAxis : �ش��ϴ� Multi�� ID�� �´� Axis 
	 */
	//ISercosMultiAxis* GetMultiMotionComponent();
	IRSNMCMultiAxis* GetMultiMotionComponent();

/************************************************************************/
/*        Teaching ���� ���� Interface                                  */
/************************************************************************/

	/**
	 * PanelTransfer X �� ���� ������ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadXFixedPosParameter(void);

	/**
	 * PanelTransfer X �� ���� ������ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgdFixedPos : ������ ������ǥ �迭 (X)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveXFixedPosParameter(double* rgdFixedPos);

	/**
	 * PanelTransfer X �� ���� Offset��ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadXOffsetPosParameter(void);

	/**
	 * PanelTransfer X �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgdOffsetPos : ������ Offset��ǥ �迭 (X)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveXOffsetPosParameter(double* rgdOffsetPos);

	/**
	 * PanelTransfer X �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
	 *
	 * @param	rgdFixedPos : ������ǥ �迭 (X)
	 * @param	rgdOffsetPos : Offset��ǥ �迭 (X)
	 * @param	rgdModelPos : Model��ǥ �迭 (X)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int GetXPosParameter(double* rgdFixedPos, double* rgdOffsetPos, double* rgdModelPos);

	/**
	 * PanelTransfer X Teaching ������ �д´�.
	 *
	 * @return	STeachingInfo : Teaching ����
	 */
	STeachingInfo GetXTeachingInfo();

/************************************************************************/
/*        Vacuum Operation Interface                                    */
/************************************************************************/

	/**
	 * PanelTransfer �� Vacuum�� �������� Ȯ���Ѵ�. �����̸� Success Return.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckVacuum(void);

	/**
	 * PanelTransfer �� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AbsorbPanel(BOOL bSkipSensor = FALSE);

	/**
	 * PanelTransfer �� Vacuum�� �����Ѵ�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int ReleasePanel(BOOL bSkipSensor = FALSE);

	/**
	 * PanelTransfer �� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsPanelAbsorbed(void);

	/**
	 * PanelTransfer �� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsPanelReleased(void);

	/**
	 * PanelAligner�� Plate Unit���� Air Blow �λ��Ѵ�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//@	int AirCVBlowOn();

	/**
	 * PanelAligner�� Plate Unit���� Air Blow �λ� �����Ѵ�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//@	int AirCVBlowOff();

	/**
	 * PanelAligner�� Plate Unit���� Air Blow �λ� ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = �λ� ����, FALSE = ���� ����)
	 */
//@	BOOL IsAirCVBlowOn();

	/**
	 * PanelAligner�� Plate Unit���� Air Blow �λ����� ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = �λ� ����)
	 */
//@	BOOL IsAirCVBlowOff();

	/**
	 * PanelAligner�� Plate Unit ��ü�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//@	int AirCVVacAllOn();

	/**
	 * PanelAligner�� Plate Unit ��ü�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//@	int AirCVVacAllOff();

	/**
	 * PanelAligner�� Plate Unit ��ü�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
//@	BOOL IsAirCVVacAllOn();

	/**
	 * PanelAligner�� Plate Unit ��ü�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
//@	BOOL IsAirCVVacAllOff();

	/**
	 * PanelAligner�� Plate Unit 1,2�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//@	int AirCVVacPlate1On();

	/**
	 * PanelAligner�� Plate Unit 1,2�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//@	int AirCVVacPlate1Off();

	/**
	 * PanelAligner�� Plate Unit 1,2�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
//@	BOOL IsAirCVVacPlate1On();

	/**
	 * PanelAligner�� Plate Unit 1,2�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
//@	BOOL IsAirCVVacPlate1Off();

	/**
	 * PanelAligner�� Plate Unit 3,4�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//@	int AirCVVacPlate2On();

	/**
	 * PanelAligner�� Plate Unit 3,4�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//@	int AirCVVacPlate2Off();

	/**
	 * PanelAligner�� Plate Unit 3,4�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
//@	BOOL IsAirCVVacPlate2On();

	/**
	 * PanelAligner�� Plate Unit 3,4�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
//@	BOOL IsAirCVVacPlate2Off();

	// Air Conveyor �λ�
//@	int AirCVFloatingOn();

	// Air Conveyor ����
//@	int AirCVFloatingOff();

/************************************************************************/
/*        Cylinder Operation Interface                                  */
/************************************************************************/

	/**
	 * PanelTransfer UD Cyl�� Up ��Ų��.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) �Ǹ��� ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int UpPickUpUDCyl(BOOL bSkipSensor = FALSE);

	/**
	 * PanelTransfer UD Cyl Down ��Ų��.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) �Ǹ��� ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int DownPickUpUDCyl(BOOL bSkipSensor = FALSE);

	/**
	 * PanelTransfer Up/Down Cylinder �� U/D ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsUpPickUpUDCyl(void);

	/**
	 * PanelTransfer Up/Down Cylinder �� U/D ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsDownPickUpUDCyl(void);


	int ForAirRailFB1(BOOL bSkipSensor = FALSE);
	int ForAirRailFB2(BOOL bSkipSensor = FALSE);

	int BackAirRailFB1(BOOL bSkipSensor = FALSE);
	int BackAirRailFB2(BOOL bSkipSensor = FALSE);

	BOOL IsForAirRailFB1();
	BOOL IsForAirRailFB2();

	BOOL IsBackAirRailFB1();
	BOOL IsBackAirRailFB2();

/************************************************************************/
/*        PanelTransfer Move Operation Interface                                */
/************************************************************************/

	/**
	 * PanelTransfer X ���� Index ��ġ�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bUseAuto : Manual Mode�� ��� �� ������ ���� ������ Ȯ�� (FLASE : Manual Mode, FALSE : Auto Mode)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SafeSyncMoveXPos(int iPosID, BOOL bUseAuto = TRUE);
	int SafeMoveXPos(int iPosID, BOOL bUseAuto = TRUE);

/************************************************************************/
/*        PanelTransfer Position Ȯ�� Interface                                 */
/************************************************************************/

	/**
	 * PanelTransfer X �� �������� ���θ� Ȯ���Ѵ�. ��� �������Ͱ� �Ǿ� ������ Success Return, 
	 * �ƴϸ� �ȵǾ� �ִ� �� Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckOrigin(void);

	int ResetOrigin(void); //171004 JSH

	/**
	 * X ���� ���� ��ġ�� �д´�.
	 *
	 * @param	bType	: (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double	: PanelTransfer�� ���� ��ġ (Z��)
	 */
	double GetXCurrentPos(BOOL bType = FALSE);

	/**
	 * X ���� ���� ��ġ�� �д´�.
	 *
	 * @param	bType	: (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double	: PanelTransfer�� ���� ��ġ (Z��)
	 */
	double GetX2CurrentPos(BOOL bType = FALSE);

	/**
	 * X ���� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : PanelTransfer�� ��ǥ ��ġ (X��)
	 */
	double GetXTargetPos(int iPosID);

	/**
	 * PanelTransfer X ���� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInXPos(int iPosID, double* pdTolerance = NULL);

	/**
	 * PanelTransfer X ���� ������ġ�� ������ġ�� ���Ѵ�.
	 *
	 * @param	dTargetPos : ������ġ�� ���� ������ġ
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInXPos(double dTargetPos, double* pdTolerance = NULL);

private:

/**********
 *  Mechanical Layer State Check Components
 */

	// ���� Unit ����.
	/** Panel Aligner */
	IStatePanelAligner*		m_plnkStatePanelAligner;

#ifdef DEF_SOURCE_SYSTEM
	IStatePanelTransfer*	m_plnkStatePanelTransferOut;
	IStatePanelTransfer*	m_plnkStatePanelTransferIn;
#endif

	/** Tab Mounter */
	IStateTabMounter*		m_plnkStateTabMounter[DEF_MAX_TABMOUNTER];
	IStateTabMounter*		m_plnkStateRTabMounter[DEF_MAX_TABMOUNTER];
	IStateCameraCarrier*	m_plnkStateInspectCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	IStateCameraCarrier*	m_plnkStateRInspectCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
private:
	/**
	 * PanelTransfer(PanelX��) �̵����� Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : ��ǥ ��ġ ID (iPosID == DEF_PANEL_TRANSFER_NONE_POS �̸� Interlock check ����.)
	 * @param	bUseAuto : Auto mode ��� ���θ� ���� (default = TRUE)
	 * @return	int (0 = Safety, Error Code = �� ��)
	 */
	int checkInterlockForPanelX(int iPosID, BOOL bUseAuto);

	int checkInterlockForTransferIn(int iPosID, BOOL bUseAuto);	//Source Only
	int checkInterlockForTransferOut(int iPosID, BOOL bUseAuto);

	int checkInterlockForRailMove();

	/**
	 * PanelTransfer PanelX�� ���� ��ǥ�� ����Ѵ�.
	 *
	 * @return	int (0 = Safety, Error Code = �� ��)
	 */
	int calculatePanelTransferPanelXModelPosParameter();

public:
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
	MPanelTransfer(SCommonAttribute commonData, SPanelTransferRefCompList listRefComponents, SPanelTransferData datComponent);

	/**
	 * �Ҹ��� �Լ�
	 */
	~MPanelTransfer();

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

	/**
	 * PanelTransfer�� Data Parameter�� �����Ѵ�. 
	 *
	 * @param	SPanelTransferParam	: ������ PanelTransfer Parameter
	 * @return	Error Code	: 0 = SUCCESS, �� �� = Error
	 */
	int	SetData(SPanelTransferData datcomponent);

	/**
	 * PanelTransfer�� ���� üũ�� ���� �ʿ���  Component List�� Assign �Ѵ�. 
	 *
	 * @param	SStateCheckCompList : Component List
	 */
	int	AssignStateCheckComponents(SStateCheckCompListForPanelTransfer sCompList);

	/**
	 * Model ����� �۾��� �����Ѵ�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int	ChangeModel();

	/**
	 * PanelTransfer ���� �ʱ� ���·� �̵��Ѵ�.
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
	 * PanelTransfer(PanelX��)�� Index��ġ�� �����̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	dTarget	: ��ǥ
	 * @param	iPosID	: Position Index ID
	 * @param	bUseAuto : Auto Mode�� ��� �� ������ ���� ���� (default = TRUE)
	 *						(FLASE : Manual Mode, TRUE : Auto Mode)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SyncMoveXPos(double dTarget, int iPosID, BOOL bUseAuto = TRUE);
	int MoveXPos(double dTarget, int iPosID, BOOL bUseAuto = TRUE);

/**********
 * Sensor
 */

	/**
	 * PanelTransfer Decel or Stop Sensor �� ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Panel Exists , FALSE = Panel not Exists)
	 */
	BOOL IsPanelDetected();

	//120224.kms__________
	BOOL IsPanelLoadingDetected();
//@	BOOL IsPanelUnLoadingDetected();
	//____________________

	//@�𵨴��� ��ġ�� ���� ã�� ����
	int	FindSensorTransferInAirRail();

};
#endif //M_TRANSFER_H
