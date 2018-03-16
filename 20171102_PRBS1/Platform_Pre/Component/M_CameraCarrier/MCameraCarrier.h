/* 
 * CameraCarrier Component
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
 * CameraCarrier.h : Headerfile of stage component.
 */
#ifndef MCAMERACARRIER_H
#define MCAMERACARRIER_H

#include "ICommon.h"
#include "DefCameraCarrier.h"
#include "IStateCameraCarrier.h"
#include <afxmt.h>

class MCameraCarrier : public virtual ICommon, public IStateCameraCarrier
{
private:   
    IAxis*			m_plnkAxis;
	IAxis*			m_plnkAxisBackupZ;
	IAxis*			m_plnkAxisExpand;
	ICylinder*		m_plnkESCyl;

	MSystemData*	m_plnkSystemData;
	MPanelData*		m_plnkPanelData;
	MPreBonderData*	m_plnkPreBonderData;
	MProcessData*	m_plnkProcessData;
	MTabData*		m_plnkTabData; //SJ_YYK 150109 Add..
//@	MWorkSchedule*	m_plnkWorkSchedule;
	MWorkSchedule*	m_plnkWorkSchedule[DEF_MAX_WORK_SCHEDULE];

	/** PositionInfo.inf File ���� ����ü */
	STeachingInfo	m_sTeachingInfo;
	STeachingInfo	m_sTeachingInfoBackupZ;
	STeachingInfo	m_sTeachingInfoExpand;

	/** Offset Data�� ����� File Name */
	CString			m_strOffsetPosFileName;

	/** �Ķ���� ����Ÿ�� ����� File Name */
	CString			m_strPosFileName;
	CString			m_strOffsetPosFilePath;

	/** ���� �� ���� Ȯ���ؾ� �ϴ� ���� Camera Carrier �� ���� link
	 *	- �Ҵ� ���� : �ش� �׷쿡 ���ϴ� TabMounter ���� ���¿� ��ġ��Ŵ.
	 */
	IStateCameraCarrier*	m_plnkPreCamCarrier;
	IStateCameraCarrier*	m_plnkNextCamCarrier;
//	IStateTabMounter*		m_plnkTabMounter;
	IStateTabMounter*		m_plnkTabMounter[DEF_MAX_TABMOUNTER];
	IStatePanelAligner*		m_plnkPanelAligner;

/**********
 * ��ǥ
 */
	/** CameraCarrier - ������ǥ */
    double			m_rgdFixedPos[DEF_CAMERACARRIER_MAX_POS];
	/** CameraCarrier - ����ǥ */
//@    double			m_rgdModelPos[DEF_CAMERACARRIER_MAX_POS];
    double			m_rgdModelPos[DEF_MAX_WORK_SCHEDULE][DEF_CAMERACARRIER_MAX_POS];
	/** CameraCarrier - Offset ��ǥ */
    double			m_rgdOffsetPos[DEF_CAMERACARRIER_MAX_POS];

	/** CameraCarrier - ������ǥ */
    double			m_rgdFixedPosBackupZ[DEF_CAMERACARRIER_BACKUP_MAX_POS];
	/** CameraCarrier - ����ǥ */
    double			m_rgdModelPosBackupZ[DEF_CAMERACARRIER_BACKUP_MAX_POS];
	/** CameraCarrier - Offset ��ǥ */
    double			m_rgdOffsetPosBackupZ[DEF_CAMERACARRIER_BACKUP_MAX_POS];

	/** CameraCarrier ExpandY- ������ǥ */
    double			m_rgdFixedPosExpand[DEF_CAMERACARRIER_EXPAND_MAX_POS];
	/** CameraCarrier ExpandY- ����ǥ */
    double			m_rgdModelPosExpand[DEF_CAMERACARRIER_EXPAND_MAX_POS];
	/** CameraCarrier ExpandY- Offset ��ǥ */
    double			m_rgdOffsetPosExpand[DEF_CAMERACARRIER_EXPAND_MAX_POS];

/**********
 * ��Ÿ
 */
	/** CameraCarrier �� ���� Group ��ȣ */
	int				m_iGroupNo;
	int				m_iWorkInstance;

	/** �۾��� ���� */
	EWorkingSide	m_eWorkSide;

	/** �����ϰ� �ִ� Camera ��ȣ */
//	int				m_iCamNo;
	int				m_iCamNo1;
	int				m_iCamNo2;

	BOOL			m_isPanelAlignUnit;

	/**(KKY)
	 * �̵��� �α� Carrier�� �浹���� Ȯ���ϴµ� ���
	 **/
	double						m_posTargetMove;
	BOOL						m_bMoving;
	static CCriticalSection		m_csMoveLock;
/************************************************************************/
/*
/* Private Operation
/*
/************************************************************************/ 
private :

	//KKY
	void					SetMovingY(BOOL bMove, double posTarget);

	/**
	 * �� �̵����� Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	dTarget : ��ǥ ��ġ
	 * @return	int (0 = Safety, Error Code = �� ��)
	 */
	int checkInterlock(double dTarget);

	int checkInterlockForBackup(int iPosID, double dTarget, BOOL bAuto = FALSE);

	/**
	 * Model ��ǥ�� ����Ѵ�.
	 *
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int calculateModelPosParameter(void);

	int calculateExpandModelPosParameter(void);

public:
	/** 
	 * ������
	 *
	 * @param commonData : ObjectI�� Log�� ���� Component Common Data
	 * @param listRefComponent : ������ Component ������ ����Ʈ
	 * @param datComponent : CameraCarrier Data
	 */
	MCameraCarrier(SCommonAttribute commonData, SCameraCarrierRefCompList listRefComponents, SCameraCarrierData datComponent);

    ~MCameraCarrier(void);    
	
/************************************************************************/
/*        Component ���� ���� Interface                                 */
/************************************************************************/

	/**
	 * CameraCarrier�� Data Parameter�� �����Ѵ�. 
	 *
	 * @param	datcomponent : ������ CameraCarrier Parameter
	 */
	void SetData(SCameraCarrierData datcomponent);

	/**
	 * CameraCarrier�� Component List�� �����Ѵ�.
	 *
	 * @param	CameraCarrierCompList : ������ CameraCarrier Component List
	 */
	void AssignComponents(SCameraCarrierRefCompList CameraCarrierCompList);

	/**
	 * CameraCarrier�� State Ȯ���� Component List�� �����Ѵ�.
	 *
	 * @param	sCompList : ������ State Ȯ���� ���� Component List
	 */
	void AssignStateCheckComponents(SStateCheckCompListForCameraCarrier sCompList);

	/**
	 * CameraCarrier�� Motion Component�� ���� �޴´�.
	 */
	IAxis* GetMotionComponent(void);

	IAxis* GetMotionComponentBackup(void);

	IAxis* GetMotionComponentExpand();

	/**
	 * Model�� �������� CameraCarrier�� �˸���.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int ChangeModel(void);

/************************************************************************/
/*        Teaching ���� ���� Interface                                  */
/************************************************************************/

	int LoadFixedPosParameter(void);

	int LoadFixedPosParameterBackup(void);

	int LoadFixedPosParameterExpand(void);

	/**
	 * CameraCarrier�� �� ���� Offset��ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadOffsetPosParameter(void);
	
	int LoadOffsetPosParameterBackup(void);

	int LoadOffsetPosParameterExpand(void);

	int SaveFixPosParameter(double* pdFixPos);

	int SaveFixedPosParameterBackup(double* dFixedPos);

	int SaveFixedPosParameterExpand(double* dFixedPos);

	/**
	 * CameraCarrier�� �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgdOffsetPos : ������ Offset��ǥ �迭
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveOffsetPosParameter(double* pdOffsetPos);

	int SaveOffsetPosParameterBackup(double* pdOffsetPos);

	int SaveOffsetPosParameterExpand(double* pdOffsetPos);

	/**
	 * CameraCarrier�� �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
	 *
	 * @param	rgsFixedPos : ������ǥ �迭
	 * @param	rgsOffsetPos : Offset��ǥ �迭
	 * @param	rgsModelPos : Model��ǥ �迭
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//@	int GetPosParameter(double* pdFixedPos, double* pdOffsetPos, double* pdModelPos);
	int GetPosParameter(int iScheduleNo, double* pdFixedPos, double* pdOffsetPos, double* pdModelPos);

	int GetPosParameterBackup(double* pdFixedPos, double* pdOffsetPos, double* pdModelPos);

	int GetPosParameterExpand(double* pdFixedPos, double* pdOffsetPos, double* pdModelPos);

	/**
     * CameraCarrier Teaching ������ �д´�.
	 *
	 * @return	STeachingInfo : Teaching ����
     */
	STeachingInfo GetTeachingInfo(void);

	STeachingInfo GetTeachingInfoBackup(void);

	STeachingInfo GetTeachingInfoExpand(void);

	/**
	 *	�ⱸ ���� �Ÿ�(SystemData) �� �����Ǵ� Fixed Offset �� �����Ѵ�.
	 */
	void SetFixedOffsetBySystemData();

/************************************************************************/
/*        CameraCarrier Move Operation Interface                        */
/************************************************************************/
	/**
	 * CameraCarrier �� ��ǥ ��ġ�� �̵���Ų��.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bWaitOption : ���� �ϷḦ ��ٸ� ������ ����.
	 * @param	iTabNumber : �۾� Tab No.
	 * @param	bIgnoreInterlock : Check Interlock ����.
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//@	int SafeMovePos(int iPosID, BOOL bWaitOption = TRUE, int iTabNumber = -1, BOOL bIgnoreInterlock = FALSE);
	int SafeMovePos(int iScheduleNo, int iPosID, BOOL bWaitOption = TRUE, int iTabNumber = -1, BOOL bIgnoreInterlock = FALSE);

	int SafeMoveBackupPos(int iBackupPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);

	int SafeMoveExpand(int iExpandPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);

	/**
	 * CameraCarrier �̵� ���� �Ϸ� ���
	 */
	int Wait4Done(BOOL bSkipMode = FALSE);

	int Wait4DoneBackup(BOOL bSkipMode = FALSE);

	int Wait4DoneExpand(BOOL bSkipMode = FALSE);

	/**
	 * CameraCarrier �� ��ǥ ��ġ�� �̵���Ų��.
	 *
	 * @param	dTarget : ��ǥ ��ġ
	 * @param	iPosID : �̵��� Position Index
	 * @param	bWaitOption : ���� �ϷḦ ��ٸ� ������ ����.
	 * @param	bIgnoreInterlock : Check Interlock ����.
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int MovePos(double dTarget, int iPosID, BOOL bWaitOption = TRUE, BOOL bIgnoreInterlock = FALSE);

	int MoveBackupPos(double dBackupTargetPos, int iBackupPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);

	int MoveExpandPos(double dExpandTargetPos, int iExpandPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);

/************************************************************************/
/*        CameraCarrier Position Ȯ�� Interface                         */
/************************************************************************/
	
	/**
	 * �������� ���θ� Ȯ���Ѵ�. ��� �������Ͱ� �Ǿ� ������ Success Return, �ƴϸ� �ȵǾ� �ִ� �� Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckOrigin(void);

	int CheckOriginBackup(void);

	int CheckOriginExpand(void);

	/**
	 * CameraCarrier �� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : CameraCarrier�� ���� ��ġ 
	 */
	double GetCurrentPos(BOOL bType = FALSE);

	double GetCurrentPosBackup(BOOL bType = FALSE);

	double GetCurrentPosExpand(BOOL bType = FALSE);

	/**
	 * CameraCarrier �� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : CameraCarrier�� ��ǥ ��ġ 
	 */
//@	double GetTargetPos(int iPosID, int iTabNumber = -1);
	double GetTargetPos(int iScheduleNo, int iPosID, int iTabNumber = -1);

	double GetTargetPosBackup(int iPosID, int iTabNumber = -1);

	double GetTargetPosExpand(int iPosID, int iTabNumber = -1);

//@	double CalculateInspectPos(double dInspectModelPos);
	double CalculateInspectPos(int iScheduleNo, double dInspectModelPos);
	double CalculateMountPos(int iScheduleNo, double dInspectModelPos);  //170826 JSh.s

	/**
	 * CameraCarrier �� ������ġ�� Index ��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
//@	BOOL IsInPos(int iPosID, double* pdTolerance = NULL);
	BOOL IsInPos(int iScheduleNo, int iPosID, double* pdTolerance = NULL);

	BOOL IsInPosBackup(int iPosID, double* pdTolerance = NULL);

	BOOL IsInPosExpand(int iPosID, double* pdTolerance = NULL);

	/**
	 * CameraCarrier �� ������ġ�� ������ġ�� ���Ѵ�.
	 *
	 * @param	dTargetPos : ������ġ�� ���� ������ġ
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInPos(double dTargetPos, double* pdTolerance = NULL);

	BOOL IsInPosBackup(double dTargetPos, double* pdTolerance = NULL);

	BOOL IsInPosExpand(double dTargetPos, double* pdTolerance = NULL);

/************************************************************************/
/*        Cylinder														*/
/************************************************************************/

	int ExpandShrinkForTabAlign(BOOL bSkipSensor = FALSE);
	int ExpandShrinkForPanelAlign(BOOL bSkipSensor = FALSE);

	/**
	 * CameraCarrier ES Cyl�� Expand ��Ų��.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) �Ǹ��� ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int Expand(BOOL bSkipSensor = FALSE);

	/**
	 * CameraCarrier ES Cyl Shrink ��Ų��.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) �Ǹ��� ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int Shrink(BOOL bSkipSensor = FALSE);

	/**
	 * CameraCarrier Expand/Shrink Cylinder �� Expand ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsExpand(void);

	/**
	 * CameraCarrier Expand/Shrink Cylinder �� Shrink ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Shrink , FALSE = Expand)
	 */
	BOOL IsShrink(void);

/************************************************************************/
/*        ETC. Interface					                            */
/************************************************************************/

	/**
	 * CameraCarrier �� ���� Group ��ȣ�� ��ȯ�Ѵ�.
	 */
	const int GetCameraCarrierGroupNo(void) const
	{
		return m_iGroupNo;
	}

	/**
	 * CameraCarrier �� �۾��ϴ� �۾����� ��ȯ�Ѵ�.
	 */
	const EWorkingSide GetCameraCarrierWorkSide(void) const
	{
		return m_eWorkSide;
	}

	/**
	 * CameraCarrier �� �����ϰ� �ִ� Camera ��ȣ�� ��ȯ�Ѵ�.
	 */
//	const int GetOwnedCameraNo(void) const
//	{
//		return m_iCamNo;
//	}
	const int GetOwnedCameraNo1(void) const
	{
		return m_iCamNo1;
	}
	const int GetOwnedCameraNo2(void) const
	{
		return m_iCamNo2;
	}

	/**
	 * Pre Camera Carrier �� Inspect Camera ��ȣ�� ��ȯ�Ѵ�.
	 */
	const int GetPreCamCarrierNo(void);

	/**
	 * Next Camera Carrier �� Inspect Camera ��ȣ�� ��ȯ�Ѵ�.
	 */
	const int GetNextCamCarrierNo(void);

	/**
	 *	���� Camera Carrier �� Inspection Camera Carrier �� ���,
	 *	�ٸ� Camera Carrier �� ���� �����ϵ���
	 *	���� Camera Carrier �� �ش��ϴ� Inspection Camera Carrier ��ȣ�� ��ȯ�Ѵ�.
	 */
	const int GetInspectCamCarrierNo(void) const ;

	/**(KKY)
	 * �̵��� �α� Carrier�� �浹���� Ȯ���ϴ� �Լ�
	 **/
	int	CheckCollisionOther(int iScheduleNo, int iPosID, int iTabNumber);
	int CheckCollisionOtherD(double posTarget);
	double GetMovingTgPos();
	BOOL IsMovingY();//Y���� �̵�������...

	int GetCameraNo(int iCameraId);

	BOOL IsPanelAlignUnit() { return m_isPanelAlignUnit; }
};

#endif // MCAMERACARRIER_H
