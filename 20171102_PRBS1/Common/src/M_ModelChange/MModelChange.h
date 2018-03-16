/* 
 * ModelChange Component
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
 * MModelChange.h : Headerfile of stage component.
 */

#ifndef M_MODEL_CHANGE_H
#define M_MODEL_CHANGE_H

#include "DefModelChange.h"
#include "ICommon.h"
#include "IStateModelChange.h"


const double COMPARE_PANEL_SIZE_TOLERANCE = 5.0;
const double COMPARE_AIR_PLATE_SIZE = 150.0;

/** ModelChange Y ���� ������ ModelChange Unit�� �𵨸��� Class�̴�. */
class MModelChange : public IStateModelChange, public virtual ICommon
{
protected:

/************************************************************************/
/*								I/O										*/
/************************************************************************/
	/** ModelChange Locl/Unlock I/O */
	IIO*		m_plnkIO;

/************************************************************************/
/*								Cylinder								*/
/************************************************************************/
	/** ModelChange Expand/Shrink Cylinder */
	ICylinder*	m_plnkESCyl;

	/** ModelChange UP/DOWN Cylinder */
	ICylinder*	m_plnkUDCyl;

	ICylinder*	m_plnkAirRailFB1;
	ICylinder*	m_plnkAirRailFB2;

/************************************************************************/
/*								Motion									*/
/************************************************************************/
	/** ModelChange Y Motion Object */
	IAxis*	m_plnkY;

/************************************************************************/
/*								Data									*/
/************************************************************************/
	/** System Data Object */
	MSystemData*	m_plnkSystemData;

	/** Panel Data Object */
	MPanelData*		m_plnkPanelData;
	
	int				m_iWorkGroup;
	EWorkingSide	m_eWorkSide;
/************************************************************************/
/*								I/O Output								*/
/************************************************************************/
	/** Lock�� ���� IO Address */
	unsigned short	m_usOLockModelChangeRail;

	/** Unlock�� ���� IO Address */
	unsigned short	m_usOUnlockModelChangeRail;

/************************************************************************/
/*								Position								*/
/************************************************************************/
	CString		m_strPosFileName;
	CString		m_strOffsetPosFileName;
	CString		m_strOffsetPosFilePath;

	/** PanelAlignerXYT�� Teaching ����				*/
    STeachingInfo		m_sPanelAlignerXYTTeachingInfo;	
	/** PanelTransfer X�� Teaching ����				*/
	STeachingInfo		m_sPanelTransferXTeachingInfo;
	/** ModelChange Y�� Teaching ���� */
	STeachingInfo	m_sYTeachingInfo;

	/** ModelChange X�� ���� ��ǥ */
	double		m_rgdYFixedPos[DEF_MODEL_CHANGE_MAX_POSITION];
	/** ModelChange Y�� Offset ��ǥ */
	double		m_rgdYOffsetPos[DEF_MODEL_CHANGE_MAX_POSITION];
	/** ModelChange Y�� �� ��ǥ */
	double		m_rgdYModelPos[DEF_MODEL_CHANGE_MAX_POSITION];

/************************************************************************/
/*		��Ÿ															*/
/************************************************************************/
// jdy sesl	char		m_szLogMsg[250];

public:

/************************************************************************/
/*		Component ���� ���� Interface									*/
/************************************************************************/
	/**
	 * ModelChange�� Component List�� �����Ѵ�.
	 *
	 * @param	SModelChangeRefCompList : ������ ModelChange Component List
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AssignComponents(SModelChangeRefCompList ModelChangeCompList);

	/**
	 * ModelChange�� Motion Component�� ���� �޴´�.
	 *
	 * @return	IAixs : �ش��ϴ� �� ID�� �´� Axis 
	 */
	IAxis* GetMotionComponent();

/************************************************************************/
/*		Teaching ���� ���� Interface									*/
/************************************************************************/
	/**
	 * ModelChange Y �� ���� ������ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadYFixedPosParameter(void);

	/**
	 * ModelChange Y �� ���� Offset��ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadYOffsetPosParameter(void);

	/**
	 * ModelChange Y �� ���� ������ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgdFixedPos : ������ ������ǥ �迭 (Y)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveYFixedPosParameter(double* rgdFixedPos);

	/**
	 * ModelChange Y �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgdOffsetPos : ������ Offset��ǥ �迭 (Y)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveYOffsetPosParameter(double* rgdOffsetPos);

	/**
	 * ModelChange Y �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
	 *
	 * @param	rgdFixedPos : ������ǥ �迭 (Y)
	 * @param	rgdOffsetPos : Offset��ǥ �迭 (Y)
	 * @param	rgdModelPos : Model��ǥ �迭 (Y)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int GetYPosParameter(double* rgdFixedPos, double* rgdOffsetPos, double* rgdModelPos);

	/**
	 * ModelChange Y Teaching ������ �д´�.
	 *
	 * @return	STeachingInfo : Teaching ����
	 */
	STeachingInfo GetYTeachingInfo();

/************************************************************************/
/*		Cylinder Operation Interface									*/
/************************************************************************/

	/**
	 * ModelChange ES Cyl�� Expand ��Ų��.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) �Ǹ��� ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int Expand(BOOL bSkipSensor = FALSE);

	/**
	 * ModelChange ES Cyl Shrink ��Ų��.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) �Ǹ��� ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int Shrink(BOOL bSkipSensor = FALSE);

	/**
	 * ModelChange UD Cyl�� UP ��Ų��.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) �Ǹ��� ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int Up(BOOL bSkipSensor = FALSE);

	/**
	 * ModelChange UD Cyl DOWN ��Ų��.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) �Ǹ��� ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int Down(BOOL bSkipSensor = FALSE);

	/**
	 * ModelChange Expand/Shrink Cylinder �� Expand ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsExpand(void);

	/**
	 * ModelChange Expand/Shrink Cylinder �� Shrink ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Shrink , FALSE = Expand)
	 */
	BOOL IsShrink(void);

	/**
	 * ModelChange Up/Down Cylinder �� Up ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Up , FALSE = Down)
	 */
	BOOL IsUp(void);

	/**
	 * ModelChange Up/Down Cylinder �� Down ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Down , FALSE = Up)
	 */
	BOOL IsDown(void);


	int ForAirRailFB1(BOOL bSkipSensor = FALSE);
	int ForAirRailFB2(BOOL bSkipSensor = FALSE);

	int BackAirRailFB1(BOOL bSkipSensor = FALSE);
	int BackAirRailFB2(BOOL bSkipSensor = FALSE);

	BOOL IsForAirRailFB1();
	BOOL IsForAirRailFB2();

	BOOL IsBackAirRailFB1();
	BOOL IsBackAirRailFB2();

/************************************************************************/
/*		I/O Operation Interface											*/
/************************************************************************/

	/**
	 * ModelChange Rail�� Lock ��Ų��.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LockRail();

	/**
	 * ModelChange Rail�� Unlock ��Ų��.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int UnlockRail();

	/**
	 * ModelChange Rail�� Lock ��Ű�� I/O ��� On�� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = I/O On, FALSE = I/O Off)
	 */
	BOOL IsLockRail(void);

	/**
	 * ModelChange Rail�� Lock ��Ű�� I/O ��� Off�� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = I/O Off, FALSE = I/O On)
	 */
	BOOL IsUnlockRail(void);

/************************************************************************/
/*		ModelChange Move Operation Interface							*/
/************************************************************************/
	/**
	 * ModelChange Y ���� Index ��ġ�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bUseAuto : Manual Mode�� ��� �� ������ ���� ������ Ȯ�� (FLASE : Manual Mode, FALSE : Auto Mode)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SafeMoveYPos(int iPosID, BOOL bUseAuto = TRUE, BOOL bWaitOption = TRUE);

/************************************************************************/
/*		ModelChange Position Ȯ�� Interface								*/
/************************************************************************/
	/**
	 * ModelChange Y �� �������� ���θ� Ȯ���Ѵ�. �������Ͱ� �Ǿ� ������ Success Return, 
	 * �ƴϸ� �ȵǾ� �ִ� �� Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckOrigin(void);

	/**
	 *	ModelChange Y Motion ������ Ȯ���Ѵ�.
	 */
	BOOL IsInYMotion();

	/**
	 * Y ���� ���� ��ġ�� �д´�.
	 *
	 * @param	bType	: (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double	: ModelChange�� ���� ��ġ (Z��)
	 */
	double GetYCurrentPos(BOOL bType = FALSE);

	/**
	 * Y ���� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : ModelChange�� ��ǥ ��ġ (Y��)
	 */
	double GetYTargetPos(int iPosID);

	/**
	 * Y ���� ��ǥ �ʺ� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : ModelChange�� ��ǥ �ʺ� (Y��)
	 */
	double GetTargetWidth(int iPosID);

	/**
	 * Y ���� ���� �ʺ� �д´�.
	 *
	 * @param	bType	: (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double	: ModelChange�� ���� �ʺ� (Y��)
	 */
	double GetCurrentWidth(BOOL bType = FALSE);
	
	/**
	 * ModelChange Y ���� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInYPos(int iPosID, double* pdTolerance = NULL);

	/**
	 * ModelChange Y ���� ������ġ�� ������ġ�� ���Ѵ�.
	 *
	 * @param	dTargetPos : ������ġ�� ���� ������ġ
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInYPos(double dTargetPos, double* pdTolerance = NULL);

private:

/**********
 *  Mechanical Layer State Check Components
 */
	// ���� Unit ����.

	IStatePanelAligner*		m_plnkStatePanelAligner;
//@	IStatePanelTransfer*	m_plnkStatePanelTransfer;
#ifdef DEF_SOURCE_SYSTEM
	IStatePanelTransfer*	m_plnkStatePanelTransfer;
#endif
	IStatePanelTransfer*	m_plnkStatePanelTransferOut;
	
	IStateCameraCarrier*	m_plnkStateCameraCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
private:
	/**
	 * ModelChange(Y��) �̵����� Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : ��ǥ ��ġ ID (iPosID == DEF_PANEL_TRANSFER_NONE_POS �̸� Interlock check ����.)
	 * @param	bUseAuto : Auto mode ��� ���θ� ���� (default = TRUE)
	 * @return	int (0 = Safety, Error Code = �� ��)
	 */
	int checkInterlockForPanelY(int iPosID, BOOL bUseAuto);

	int checkInterlockForRailMove();

	/** ModelChange Y�� ���� ��ǥ�� ����Ѵ�.	*/
	int calculateModelChangePanelYModelPosParameter();

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
	MModelChange(SCommonAttribute commonData, SModelChangeRefCompList listRefComponents, SModelChangeData datComponent);

	/** �Ҹ��� �Լ� */
	~MModelChange();

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 
	/**
	 * ModelChange�� Data Parameter�� �����Ѵ�. 
	 *
	 * @param	SModelChangeParam	: ������ ModelChange Parameter
	 * @return	Error Code	: 0 = SUCCESS, �� �� = Error
	 */
	int	SetData(SModelChangeData datcomponent);

	/**
	 * ModelChange�� ���� üũ�� ���� �ʿ���  Component List�� Assign �Ѵ�. 
	 *
	 * @param	SStateCheckCompList : Component List
	 */
	int	AssignStateCheckComponents(SStateCheckCompListForModelChange sCompList);

	/**
	 * Model ����� �۾��� �����Ѵ�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int	ChangeModel();

	/**
	 * ModelChange ���� �ʱ� ���·� �̵��Ѵ�.
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
	 * ModelChange(Y��)�� Index��ġ�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	dTarget	: ��ǥ
	 * @param	iPosID	: Position Index ID
	 * @param	bUseAuto : Auto Mode�� ��� �� ������ ���� ���� (default = TRUE)
	 *						(FLASE : Manual Mode, TRUE : Auto Mode)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int MoveYPos(double dTarget, int iPosID, BOOL bUseAuto = TRUE, BOOL bWaitOption = TRUE);
	int Wait4DoneY(BOOL bSkipMode = FALSE);
/**********
 * Etc.
 */
	/**
	 *	ModelChange(Y��) Model Position �� ����� Offset ���� ��´�.
	 */
	double GetModelPosOffset();

};
#endif //M_MODEL_CHANGE_H
