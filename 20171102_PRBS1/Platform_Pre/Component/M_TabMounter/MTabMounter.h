/* 
 * TabMounter Component
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
 * TabMounter.h : Headerfile of stage component.
 */

#ifndef MTABMOUNTER_H
#define MTABMOUNTER_H

#include "ICommon.h"
#include "DefTabMounter.h"
#include "IStateTabMounter.h"
#include <afxmt.h>
#include "MProcessData.h"

class MTabMounter : public virtual ICommon, public IStateTabMounter
{
private:   
	/** Axis Object */
    IAxis*		m_plnkAxis;				// Mounter Y Axis Object
	IAxis*		m_plnkAxisZ;			// Mounter Z Axis Object

	IAxis*		m_plnkGantryAxis;	// Gantry Axis Object

	/** Vacuum Object */
	IVacuum*		m_plnkVac;

	/** Cylinder Object */
    ICylinder*		m_plnk1stUDCyl;

	ICylinder*		m_plnkPusherUDCyl;

#if FALSE	//==============================================
	ICylinder*		m_plnk2ndUDCyl;
	ICylinder*		m_plnkBackupUDCyl;
	ICylinder*		m_plnkBackupFBCyl;
#endif		//#if FALSE	//==================================

	/** System Data ���� Class */
	MSystemData*	m_plnkSystemData;
	MPanelData*		m_plnkPanelData;
	MPreBonderData* m_plnkPreBonderData;
	MProcessData*	m_plnkProcessData;
//@	MWorkSchedule*	m_plnkWorkSchedule;
	MWorkSchedule*	m_plnkWorkSchedule[DEF_MAX_WORK_SCHEDULE];

	MTabOffsetData*	m_plnkTabOffsetData;
	//120208.kms________
	MTabOffsetData* m_plnkRearTabOffsetData;

	/** PositionInfo.inf File ���� ����ü : TabMounterY = 0, TabMounterZ = 1 */
	STeachingInfo	m_sTeachingInfo[DEF_TABMOUNTER_AXIS_TYPE];
	

	/** Offset Data�� ����� File Name */
	CString			m_strOffsetPosFileName;

	/** �Ķ���� ����Ÿ�� ����� File Name */
	CString			m_strPosFileName;
	CString			m_strOffsetPosFilePath;

/**********
 *  Mechanical Layer State Check Components
 */
	IStateCameraCarrier*	m_plnkCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	IStateTabMounter*		m_plnkTabMounter[DEF_MAX_WORKER_PER_GROUP];
	IStatePanelAligner*		m_plnkPanelAligner;
#ifdef DEF_SOURCE_SYSTEM
	IStatePanelTransfer*	m_plnkPanelTransfer;
//@	IStateTabCarrier*		m_plnkTabCarrier;
//@	IStateTabCarrier*		m_plnkTabCarrierOther;
#endif
	IStateTabCarrier*		m_plnkTabCarrier[DEF_MAX_TABCARRIER];
	IStatePanelTransfer*	m_plnkPanelTransferOut;

/**********
 * ��ǥ
 */
	/** TabMounter - ������ǥ */
//    double			m_rgdFixedPos[DEF_TABMOUNTER_AXIS_TYPE][DEF_TABMOUNTER_MAX_POS];	
	/** TabMounter - ����ǥ */
//    double			m_rgdModelPos[DEF_TABMOUNTER_AXIS_TYPE][DEF_TABMOUNTER_MAX_POS];	
	/** TabMounter - Offset ��ǥ */
//    double			m_rgdOffsetPos[DEF_TABMOUNTER_AXIS_TYPE][DEF_TABMOUNTER_MAX_POS];

	/** TabMounter, Gantry - ������ǥ -> TabMounterY, TabMounterZ ������ǥ */
	double*			m_rgdFixedPos[DEF_TABMOUNTER_AXIS_TYPE];
	/** TabMounter, Gantry - ����ǥ -> TabMounterY, TabMounterZ ����ǥ */
	double*			m_rgdModelPos[DEF_TABMOUNTER_AXIS_TYPE];
	/** TabMounter, Gantry - Offset ��ǥ -> TabMounterY, TabMounterZ Offset��ǥ */
	double*			m_rgdOffsetPos[DEF_TABMOUNTER_AXIS_TYPE];

	/** TabMounter - ������ǥ */
    double			m_rgdFixedPosMounter[DEF_TABMOUNTER_MAX_POS];
	// �ⱸ ������ ���ؼ� �����Ǵ� TabMounter �� ������ǥ (SystemData �κ��� ����)
	double			m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_MAX_POS];
	/** TabMounter - ����ǥ */
    double			m_rgdModelPosMounter[DEF_TABMOUNTER_MAX_POS];	
	/** TabMounter - Offset ��ǥ */
    double			m_rgdOffsetPosMounter[DEF_TABMOUNTER_MAX_POS];

	/** TabMounter Z - ������ǥ */
    double			m_rgdFixedPosMounterZ[DEF_TABMOUNTER_Z_MAX_POS];
	/** TabMounter - ����ǥ */
    double			m_rgdModelPosMounterZ[DEF_TABMOUNTER_Z_MAX_POS];	
	/** TabMounter - Offset ��ǥ */
    double			m_rgdOffsetPosMounterZ[DEF_TABMOUNTER_Z_MAX_POS];


	// Front
	/** Gantry - ������ǥ */
    static double	sm_rgdFixedGantryPos1[DEF_TABMOUNTER_GANTRY_MAX_POS];	
	/** Gantry - ����ǥ */
    static double	sm_rgdModelGantryPos1[DEF_TABMOUNTER_GANTRY_MAX_POS];	
	/** Gantry - Offset ��ǥ */
    static double	sm_rgdOffsetGantryPos1[DEF_TABMOUNTER_GANTRY_MAX_POS];

	// Rear
	/** Gantry - ������ǥ */
    static double	sm_rgdFixedGantryPos2[DEF_TABMOUNTER_GANTRY_MAX_POS];	
	/** Gantry - ����ǥ */
    static double	sm_rgdModelGantryPos2[DEF_TABMOUNTER_GANTRY_MAX_POS];	
	/** Gantry - Offset ��ǥ */
    static double	sm_rgdOffsetGantryPos2[DEF_TABMOUNTER_GANTRY_MAX_POS];

/**********
 * ��Ÿ
 */
	/** TabMounter �� ���� Group ��ȣ */
	int				m_iGroupNo;

	/** �۾��� ���� */
	EWorkingSide	m_eWorkSide;

	/** Gantry Axis Motion Enable Flag */
	BOOL			m_bGantryMotionEnable;

	/** �� �� �� �ִ� Position ���� ���� */
	int				m_iMaxPos[DEF_TABMOUNTER_AXIS_TYPE];

	/** **************************************************************************
	 *	����/���� TabMounter ��ȣ ����
	 *  �� ����/���� ���� ����
	 *		- �ڽź��� ���� Prealign ��ġ�� �̵��ϴ� TabMounter �� ���� TabMounter ��.
	 *		- Gate �� ��� TabCarrier Unload ��ġ�� ���� Front/Rear Group �� TabMounter ��� ��ġ�� �޶���.
	 */

	/** ���� TabMounter ��ȣ : 1�� TabMounter �� [���� TabMounter ��ȣ]�� -1 �̴�.
	 *	: TabMounter �� Mount ��ġ�� �̵��ϱ� ����, Previous TabMounter �� ��ġ�� Ȯ���ؾ� �Ѵ�.
	 */
	int				m_iPreTabMounterNo;

	/** ���� TabMounter ��ȣ
	 *	: �� Group �� ������ TabMounter �� [���� TabMounter ��ȣ]�� DEF_LAST_PER_GROUP �̴�.
	 *	: TabMounter �� Load ��ġ�� �̵��ϱ� ����, Next TabMounter �� ��ġ�� Ȯ���ؾ� �Ѵ�.
	 */
	int				m_iNextTabMounterNo;


	/**(KKY)
	 * �̵��� �α� Carrier�� �浹���� Ȯ���ϴµ� ���
	 **/
	double						m_posTargetMove;
	BOOL						m_bMoving;
	static CCriticalSection		m_csMoveLock;

	MTabInfo					m_TabInfo;

	int	m_iWorkGroup;
	int m_iWorkInstance;

//	int m_iGantryLoadCarrierID;

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
private :

	//KKY
	void					SetMovingY(BOOL bMove, double posTarget);

	/**
	 * �� �̵����� Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : ��ǥ ��ġ ID
	 * @return	int (0 = Safety, Error Code = �� ��)
	 */
	int checkInterlock(int iPosID, double dTarget, BOOL bAuto = FALSE);
	int checkInterlockZ(int iPosID, double dTarget, BOOL bAuto = FALSE);

	// TabMounter UD cylinder Down Interlock
	int checkInterlockForDown(int iDownType);

	/**
	 * �ⱸ Data �� �����Ǵ� Fixed ��ǥ�� ����Ѵ�.
	 */
	void calculateFixedPosParameter(int iAxisType);

	/**
	 * Model ��ǥ�� ����Ѵ�.
	 */
	void calculateModelPosParameter(int iAxisType);


	int checkInterlockForGantry(int iPosID, double dTarget, BOOL bAuto = FALSE);

	/**
	 * TabMounter �� Position ID �κ��� Gantry �� Position ID �� ��´�.
	 */
	int getGantryPosID(int iTabMounterPosID);
	/**
	 *	Interlock Check �� ������ Gantry Move ������ �����Ѵ�.
	 */
	int moveGantryPos(double dGantryTargetPos, int iGantryPosID, BOOL bWaitOption, BOOL bAuto);

public:
	/** 
	 * ������
	 *
	 * @param commonData : ObjectI�� Log�� ���� Component Common Data
	 * @param listRefComponent : ������ Component ������ ����Ʈ
	 * @param datComponent : TabMounter Data
	 */
	MTabMounter(SCommonAttribute commonData, STabMounterRefCompList listRefComponents, STabMounterData datComponent);

    ~MTabMounter(void);    
	
/************************************************************************/
/*        Component ���� ���� Interface                                 */
/************************************************************************/

	/**
	 * TabMounter�� Data Parameter�� �����Ѵ�. 
	 *
	 * @param	datcomponent : ������ TabMounter Parameter
	 */
	void SetData(STabMounterData datcomponent);

	/**
	 * TabMounter�� Component List�� �����Ѵ�.
	 *
	 * @param	TabMounterCompList : ������ TabMounter Component List
	 */
	void AssignComponents(STabMounterRefCompList TabMounterCompList);

	/**
	 * TabMounter�� State Ȯ���� Component List�� �����Ѵ�.
	 *
	 * @param	sCompList : ������ State Ȯ���� ���� Component List
	 */
	void AssignStateCheckComponents(SStateCheckCompListForTabMounter sCompList);

	/**
	 * TabMounter�� Motion Component�� ���� �޴´�.
	 */
	IAxis* GetMotionComponent(void);

	IAxis* GetMotionComponentZ();

	// Doolin kwangilshin 2017. 09. 05.
	//
	IAxis* GetMotionComponentGantry();
	// End.
	/**
	 * Model�� �������� TabMounter�� �˸���.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int ChangeModel(void);

/************************************************************************/
/*        Teaching ���� ���� Interface                                  */
/************************************************************************/

	/**
	 * TabMounter, Gantry, Backup(mech ver3) �� �� ���� Offset��ǥ Data�� File���� �о�´�.
	 *
	 * @param	int : (0 = TabMounter Axis, 1 = Gantry Axis, 2 = Backup Axis)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadOffsetPosParameter(int iAxisType);

	/**
	 * TabMounter, Gantry, Backup(mech ver3) �� �� ���� ������ǥ Data�� File���� �о�´�.
	 *
	 * @param	int : (0 = TabMounter Axis, 1 = Gantry Axis, 2 = Backup Axis)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadFixedPosParameter(int iAxisType);

	/**
	 * TabMounter, Gantry, Backup(mech ver3) �� �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	int : (0 = TabMounter Axis, 1 = Gantry Axis, 2 = Backup Axis)
	 * @param	rgdOffsetPos : ������ Offset��ǥ �迭
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveOffsetPosParameter(int iAxisType, const double* const pdOffsetPos);

	/**
	 * TabMounter, Gantry, Backup(mech ver3) �� �� ���� ������ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	int : (0 = TabMounter Axis, 1 = Gantry Axis, 2 = Backup Axis)
	 * @param	dFixedPos : ������ ������ǥ �迭 (X)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveFixedPosParameter(int iAxisType, const double* const dFixedPos);

	/**
	 * TabMounter, Gantry, Backup(mech ver3) �� �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
	 *
	 * @param	int : (0 = TabMounter Axis, 1 = Gantry Axis, 2 = Backup Axis)
	 * @param	rgsFixedPos : ������ǥ �迭
	 * @param	rgsOffsetPos : Offset��ǥ �迭
	 * @param	rgsModelPos : Model��ǥ �迭
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int GetPosParameter(int iAxisType, double* pdFixedPos, double* pdOffsetPos, double* pdModelPos);

	/**
     * TabMounter, Gantry, Backup(mech ver3) Teaching ������ �д´�.
	 *
	 * @param	int : (0 = TabMounter Axis, 1 = Gantry Axis, 2 = Backup Axis)
	 * @return	STeachingInfo : Teaching ����
     */
	STeachingInfo GetTeachingInfo(int iAxisType);

	/**
	 *	�ⱸ ���� �Ÿ�(SystemData) �� �����Ǵ� Fixed Offset �� �����Ѵ�.
	 */
	void SetFixedOffsetBySystemData();

/************************************************************************/
/*        Cylinder Operation Interface                                  */
/************************************************************************/

	int UpCyl(BOOL bSkipSensor = FALSE);
	int DownCyl(BOOL bSkipSensor = FALSE);

	BOOL IsUpCyl(void);
	BOOL IsDownCyl(void);

	int UpPusherCyl(BOOL bSkipSensor = FALSE);
	int DownPusherCyl(BOOL bSkipSensor = FALSE);

	BOOL IsUpPusherCyl(void);
	BOOL IsDownPusherCyl(void);

/************************************************************************/
/*        Vacuum Operation Interface                                    */
/************************************************************************/
	/**
	 * TabMounter�� Vacuum�� �������� Ȯ���Ѵ�. �����̸� Success Return.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckVacuum(void);

	/**
	 * TabMounter�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AbsorbTabIC(BOOL bSkipSensor = FALSE);

	/**
	 * TabMounter�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int ReleaseTabIC(BOOL bSkipSensor = FALSE);

	/**
	 * TabMounter�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsAbsorbTabIC(void);

	/**
	 * TabMounter�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsReleaseTabIC(void);

/************************************************************************/
/*        TabMounter Move Operation Interface                           */
/************************************************************************/
	/**
	 * TabMounter�� ��ǥ��ġ�� �̵���Ų��.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bWaitOption : ���� �ϷḦ ��ٸ� ������ ����.
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SafeMovePos(int iScheduleNo, int iPosID, BOOL bWaitOption = TRUE, int iTabNumber = -1, BOOL bAuto = TRUE);

	int SafeMoveZPos(int iPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);
	/**
	 * TabMounter�� ��ǥ��ġ�� �̵���Ų��.
	 *
	 * @param	dTarget : ��ǥ ��ġ
	 * @param	iPosID : �̵��� Position Index
	 * @param	bWaitOption : ���� �ϷḦ ��ٸ� ������ ����.
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int MovePos(double dTarget, int iPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);

	int MoveZPos(double dTarget, int iPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);


	/**
	 * TabMounter, Gantry �� �̵� ���� �ϷḦ ��ٸ���.
	 */
	int Wait4Done(BOOL bSkipMode = FALSE);

	int Wait4DoneZ(BOOL bSkipMode = FALSE);


	/**
	 * Gantry �� ��ǥ��ġ�� �̵���Ų��.
	 *
	 * @param	iGantryPosID : Position Index ID
	 * @param	bWaitOption : ���� �ϷḦ ��ٸ� ������ ����.
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//	int SafeMoveGantryPos(int iGantryPosID, int iLoadCarrierID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);
	int SafeMoveGantryPos(int iGantryPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);


	/**
	 * Gantry �� ��ǥ��ġ�� �̵���Ų��.
	 *
	 * @param	dGantryTargetPos : ��ǥ ��ġ
	 * @param	iGantryPosID : �̵��� Position Index
	 * @param	bWaitOption : ���� �ϷḦ ��ٸ� ������ ����.
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int MoveGantryPos(double dGantryTargetPos, int iGantryPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);

/************************************************************************/
/*        TabMounter Position Ȯ�� Interface                            */
/************************************************************************/
	
	/**
	 * �������� ���θ� Ȯ���Ѵ�. ��� �������Ͱ� �Ǿ� ������ Success Return, �ƴϸ� �ȵǾ� �ִ� �� Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckOrigin(void);

	int CheckOriginZ(void);

	/**
	 * TabMounter �� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : TabMounter�� ���� ��ġ
	 */
	double GetCurrentPos(BOOL bType = FALSE);

	double GetCurrentPosZ(BOOL bType = FALSE);

	/**
	 * TabMounter �� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabMounter�� ��ǥ ��ġ
	 */
	double GetTargetPos(int iScheduleNo, int iPosID, int iTabNumber = -1);

	double GetTargetPosZ(int iPosID);

	double CalculateMountPos(double dModelPos);
	/**
	 * Gantry �� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : Gantry �� ���� ��ġ
	 */
	double GetCurrentGantryPos(BOOL bType = FALSE);

	/**
	 * Gantry �� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iGantryPosID : Gantry Position Index ID
	 * @return	double : Gantry �� ��ǥ ��ġ
	 */
//	double GetTargetGantryPos(int iGantryPosID, int iCarrierID);
	double GetTargetGantryPos(int iGantryPosID);

	/**
	 * TabMounter ���� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInPos(int iScheduleNo, int iPosID, double* pdTolerance = NULL);

	BOOL IsInPosZ(int iPosID, double* pdTolerance = NULL);
	/**
	 * TabMounter ���� ������ġ�� ������ġ�� ���Ѵ�.
	 *
	 * @param	dTargetPos : ������ġ�� ���� ������ġ
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInPos(double dTargetPos, double* pdTolerance = NULL);

	BOOL IsInPosZ(double dTargetPos, double* pdTolerance = NULL);

	/**
	 * Gantry ���� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iGantryPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
//	BOOL IsInGantryPos(int iGantryPosID, int iLoadCarrierID, double* pdTolerance = NULL);
	BOOL IsInGantryPos(int iGantryPosID, double* pdTolerance = NULL);

	/**
	 * Gantry ���� ������ġ�� ������ġ�� ���Ѵ�.
	 *
	 * @param	dGantryTargetPos : ������ġ�� ���� ������ġ
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInGantryPos(double dGantryTargetPos, double* pdTolerance = NULL);

/************************************************************************/
/*        ETC. Interface					                            */
/************************************************************************/

	/**
	 * TabMounter �� ���� Group ��ȣ�� ��ȯ�Ѵ�.
	 */
	const int GetTabMounterGroupNo(void) const
	{
		return m_iGroupNo;
	}

	/**
	 * TabMounter �� �۾��ϴ� �۾����� ��ȯ�Ѵ�.
	 */
	const EWorkingSide GetTabMounterWorkSide(void) const
	{
		return m_eWorkSide;
	}

	/**
	 *	Gantry Axis Motion Enable Flag �� �����Ѵ�.
	 */
//	void SetGantryMotionEnable(BOOL bState, int iLoadCarrierID);
	void SetGantryMotionEnable(BOOL bState);

	/**
	 *	Gantry Axis Motion Enable Flag �� ��ȯ�Ѵ�.
	 */
	BOOL IsGantryMotionEnable(void)
	{
		return m_bGantryMotionEnable;
	}

	/**
	 *	���ڷ� ���޵� �࿡ ���� Position ������ ��ȯ�Ѵ�.
	 */
	const int GetMaxPosNum(int iAxisType) const
	{
		return m_iMaxPos[iAxisType];
	}

	/**
	 *	�ٸ� Mounter �� ���� Mounter �� Instance No �� �� �� �ֵ��� InstanceNo �� ��ȯ�Ѵ�.
	 */
	const int GetTabMounterNo(void) const
	{
		return m_iInstanceNo;
	}

	/** **************************************************************************
	 *	����/���� TabMounter ��ȣ ����
	 *  �� ����/���� ���� ����
	 *		- �ڽź��� ���� Prealign ��ġ�� �̵��ϴ� TabMounter �� ���� TabMounter ��.
	 *		- Gate �� ��� TabCarrier Unload ��ġ�� ���� Front/Rear Group �� TabMounter ��� ��ġ�� �޶���.
	 */

	/**
	 *	Pre Tab Mounter �� TabMounter ��ȣ�� ��ȯ�Ѵ�.
	 */
	int GetPreTabMounterNo(void)
	{
		return m_iPreTabMounterNo;
	}

	/**
	 *	Next Tab Mounter �� TabMounter ��ȣ�� ��ȯ�Ѵ�.
	 */
	int GetNextTabMounterNo(void)
	{
		return m_iNextTabMounterNo;
	}

	/** **************************************************************************
	 */

	const BOOL IsUpAllTabMounterInGroup();

	int CheckGantryState(int iGantryPosID);

	const BOOL IsInSafePosTabMounterFromAligner(int nToolID);

	const BOOL IsIn_FrontTabLoadPos_TabMounter(int iScheduleNo, int nToolID);
	
	const BOOL IsIn_RearTabLoadPos_TabMounter(int iScheduleNo, int nToolID);	

	BOOL IsInDangerPosWithTabCarrier();

	/**(KKY)
	 * �̵��� �α� Mount�� �浹���� Ȯ���ϴ� �Լ�
	 **/
	int	CheckCollisionOther(int iScheduleNo, int iPosID, int iTabNumber);
	int CheckCollisionOtherD(double posTarget);
	double GetMovingTgPos();
	BOOL IsMovingY();//Y���� �̵�������...

	void		SetTabInfo(MTabInfo tabinfo);
	MTabInfo	GetTabInfo();

	int	GetWorkGroup() { return m_iWorkGroup; }
	int	GetWorkInstance() { return m_iWorkInstance; }

	double GetLoadReadyPos();

	//20111205 SJ_HJG
	double GetReceiveAkdata(EWorkingSide eWorkSide, int iTabNo);
//	static double sm_rgdFinalTabOffset[20];
	static double sm_rgdOldAkReceiveData[20];
};

#endif // MTABMOUNTER_H
