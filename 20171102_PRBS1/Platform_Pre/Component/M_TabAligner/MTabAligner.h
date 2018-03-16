/* 
 * TabAligner Component
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
 * TabAligner.h : Headerfile of stage component.
 */


#ifndef MALIGNER_H
#define MALIGNER_H

#include "ICommon.h"
#include "DefTabAligner.h"
#include "IStateTabAligner.h"

class MTabAligner : public virtual ICommon, public IStateTabAligner
{
private:   
	/** Axis1 Object */
    IAxis*			m_plnkAxis1;

	/** Axis2 Object */
	IAxis*			m_plnkAxis2;

	/** Expand/Shrink Cycliner Object */
	ICylinder*		m_plnkESCyl;	// Only Use for Mech ver2.0

	/** System Data ���� Class */
	MSystemData*	m_plnkSystemData;

	/** PositionInfo.inf File ���� ����ü */
	STeachingInfo	m_sTeachingInfo;

	/** Offset Data�� ����� File Name */
	CString			m_strOffsetPosFileName;

	/** �Ķ���� ����Ÿ�� ����� File Name */
	CString			m_strPosFileName;
	CString			m_strOffsetPosFilePath;

/**********
 * ��ǥ
 */
	/** TabAligner - ������ǥ  X */
    MPos_XYT		m_rgsFixedPos[DEF_TABALIGNER_MAX_POS];	
	/** TabAligner - ����ǥ X */
    MPos_XYT		m_rgsModelPos[DEF_TABALIGNER_MAX_POS];	
	/** TabAligner - Offset ��ǥ X */
    MPos_XYT		m_rgsOffsetPos[DEF_TABALIGNER_MAX_POS];	

/************************************************************************/
/*        ��Ÿ
/************************************************************************/
// jdy sesl	char			m_szLogMsg[250];

	/** �� TabAligner �� TabIC Prealign �۾��� �� �� ����ϴ� Camera ��ȣ */
	int				m_iPrealignCameraNo;

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
private :
	/**
	 * �̵����� Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : ��ǥ ��ġ ID
	 * @return	int (0 = Safety, Error Code = �� ��)
	 */
	int checkInterlock(int iPosID);

	/**
	 * Model ��ǥ�� ����Ѵ�..
	 *
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int calculateModelPosParameter(void);

public:
	/** 
	 * ������
	 *
	 * @param commonData : ObjectI�� Log�� ���� Component Common Data
	 * @param listRefComponent : ������ Component ������ ����Ʈ
	 * @param datComponent : TabAligner Data
	 */
	MTabAligner(SCommonAttribute commonData, STabAlignerRefCompList listRefComponents, STabAlignerData datComponent);

    ~MTabAligner(void);
	
/************************************************************************/
/*        Component ���� ���� Interface                                 */
/************************************************************************/

	/**
	 * TabAligner �� Data Parameter �� �����Ѵ�. 
	 *
	 * @param	datcomponent : ������ TabAligner Parameter
	 */
	void SetData(STabAlignerData datcomponent);

	/**
	 * TabAligner�� Component List�� �����Ѵ�.
	 *
	 * @param	TabAlignerCompList : ������ TabAligner Component List
	 */
	void AssignComponents(STabAlignerRefCompList TabAlignerCompList);

	/**
	 * TabAligner�� State Ȯ���� Component List�� �����Ѵ�.
	 *
	 * @param	sCompList : ������ State Ȯ���� ���� Component List
	 */
	void AssignStateCheckComponents(SStateCheckCompListForTabAligner sCompList);

	/**
	 * TabAligner �� Motion Component�� ���� �޴´�.
	 *
	 * @param nAxisID : �� ID (0: , 1:Y�� , 2:T��, 3:)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	IAxis* GetMotionComponent(int nAxisID);

	/**
	 * Model�� �������� TabAligner�� �˸���.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int ChangeModel(void);

/************************************************************************/
/*        Teaching ���� ���� Interface                                  */
/************************************************************************/

	/**
	 * TabAligner �� X �� ���� Offset��ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadOffsetPosParameter(void);

	/**
	 * TabAligner �� X �� ���� Fixed��ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadFixedPosParameter(void);

	/**
	 * TabAligner �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgdOffsetPos : ������ Offset��ǥ �迭 (Z)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveOffsetPosParameter(MPos_XYT* pOffsetPos);

	/**
	 * TabAligner �� ���� Fixed��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgdOffsetPos : ������ Fixed��ǥ �迭 (Z)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveFixedPosParameter(MPos_XYT* pFixedPos);

	/**
	 * TabAligner �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
	 *
	 * @param	rgsFixedPos : ������ǥ �迭 (Z)
	 * @param	rgsOffsetPos : Offset��ǥ �迭 (Z)
	 * @param	rgsModelPos : Model��ǥ �迭 (Z)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int GetPosParameter(MPos_XYT* pFixedPos, MPos_XYT* pOffsetPos, MPos_XYT* pModelPos);

	/**
     * TabAligner Teaching ������ �д´�.
	 *
	 * @return	STeachingInfo : Teaching ����
     */
	STeachingInfo GetTeachingInfo(void);


/************************************************************************/
/*        TabAligner Move Operation Interface                           */
/************************************************************************/
	/**
	 * TabAligner �� Index ��ġ�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bWaitOption : ���� �ϷḦ ��ٸ� ������ ����.
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SafeMovePos(int iPosID, BOOL bWaitOption = TRUE);

	/**
	 * TabAligner �� Index ��ġ�� �̵���Ų��.
	 *
	 * @param	dTarget : ��ǥ ��ġ
	 * @param	iPosID : �̵��� Position Index
	 * @param	bWaitOption : ���� �ϷḦ ��ٸ� ������ ����.
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int MovePos(MPos_XYT posTarget, int iPosID, BOOL bWaitOption = TRUE);

	/**
	 * TabAligner Motion ���� �ϷḦ ��ٸ���.
	 */
	int Wait4Done();

/************************************************************************/
/*        TabAligner Position Ȯ�� Interface                            */
/************************************************************************/
	
	/**
	 * �������� ���θ� Ȯ���Ѵ�. ��� �������Ͱ� �Ǿ� ������ Success Return, �ƴϸ� �ȵǾ� �ִ� �� Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckOrigin(void);

	/**
	 * TabAligner �� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : TabAligner �� ���� ��ġ 
	 */
	MPos_XYT GetCurrentPos(BOOL bType = FALSE);

	/**
	 * TabAligner �� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabAligner �� ��ǥ ��ġ 
	 */
	MPos_XYT GetTargetPos(int iPosID);

	/**
	 * TabAligner �� ������ġ�� Index ��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInPos(int iPosID, MPos_XYT* pTolerance = NULL);

	/**
	 * TabAligner �� ������ġ�� ������ġ�� ���Ѵ�.
	 *
	 * @param	dTargetPos : ������ġ�� ���� ������ġ
	 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInPos(MPos_XYT dTargetPos, MPos_XYT* pTolerance = NULL);

/************************************************************************/
/*        Cylinder														*/
/************************************************************************/

	/**
	 * TabAligner ES Cyl�� Expand ��Ų��.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) �Ǹ��� ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int Expand(BOOL bSkipSensor = FALSE);

	/**
	 * TabAligner ES Cyl Shrink ��Ų��.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) �Ǹ��� ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int Shrink(BOOL bSkipSensor = FALSE);

	/**
	 * TabAligner Expand/Shrink Cylinder �� Expand ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsExpand(void);

	/**
	 * TabAligner Expand/Shrink Cylinder �� Shrink ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Shrink , FALSE = Expand)
	 */
	BOOL IsShrink(void);

/************************************************************************/
/*        ��Ÿ															*/
/************************************************************************/

	/** �� TabAligner �� TabIC Prealign �۾��� �� �� ����ϴ� Camera ��ȣ�� ��ȯ�Ѵ�.
	 */
	int GetPrealignCamNo(void);
};

#endif // MTabAligner_H
