/* 
 * TabCarrier Component
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
 * TabCarrier.h : Headerfile of stage component.
 */


#ifndef MTABCARRIER_H
#define MTABCARRIER_H

#include "DefTabCarrier.h"
#include "ICommon.h"
#include "IStateTabCarrier.h"
#include <afxmt.h>
#include "MProcessData.h"

class MTabCarrier : public virtual ICommon, public IStateTabCarrier
{
private:   
/************************************************************************/
/*                             Vacuum                                   */
/************************************************************************/
	/** TabCarrier Vacuum Object */
//	IVacuum*	m_plnkVac[DEF_MAX_WORKER_PER_GROUP];
	IVacuum*	m_plnkVac;
	
/************************************************************************/
/*                             Cylinder                                 */
/************************************************************************/
	/** Up/Down Cylinder Object */
    ICylinder*	m_plnkUDCyl;

	ICylinder*	m_plnkUDCyl2; //SJ_YYK 150817 Add...

	ICylinder*	m_plnkFBCyl;

	ICylinder*	m_plnkCamESCyl;

	ICylinder*	m_plnkDumpBasketFBCyl;

/************************************************************************/
/*                             Motion                                   */
/************************************************************************/
	/** TabCarrier X Motion Object */
	IAxis*		m_plnkX;

	/** TabCarrier Y Motion Object */
	IAxis*		m_plnkY;

	/** TabCarrier T Motion Object */
	IAxis*		m_plnkT;

	IAxis*		m_plnkExpandAxis;

	IAxis*		m_plnkZAxis;

	ISercosMultiAxis*	m_plnkXZAxis; //SJ_YYK 150212 Add..

/************************************************************************/
/*                             State Interface                          */
/************************************************************************/
	/** TabFeeder State Object */
	IStateTabFeeder*	m_plnkStateTabFeeder;

	/** TabMounter State Object */
	IStateTabMounter*	m_plnkStateTabMounter[DEF_MAX_WORKER_PER_GROUP];

	IStateTabCarrier*	m_plnkStateTabCarrier[DEF_MAX_TABCARRIER];

/************************************************************************/
/*                             Data                                     */
/************************************************************************/
	/** System Data Object */
	MSystemData*	m_plnkSystemData;
	MPreBonderData*	m_plnkPreBonderData;
	MTabOffsetData*	m_plnkTabOffsetData;


	/** �����ϰ� �ִ� Camera ��ȣ */
	int				m_iCamNo1;
	int				m_iCamNo2;

	// Work Group
	int				m_iWorkInstance;
	int				m_iWorkGroup;

	/** **************************************************************************
	 *	����/���� TabCarrier ��ȣ ����
	 *  �� ����/���� ���� ����
	 *		- �ڽź��� ���� �۾� ��ġ�� �̵��ϴ� TabMounter �� ���� TabMounter ��.
	 */

	/** ���� TabMounter ��ȣ : 1�� TabMounter �� [���� TabMounter ��ȣ]�� -1 �̴�.
	 *	: TabMounter �� Mount ��ġ�� �̵��ϱ� ����, Previous TabMounter �� ��ġ�� Ȯ���ؾ� �Ѵ�.
	 */
	int				m_iPreTabCarrierNo;

	/** ���� TabMounter ��ȣ
	 *	: �� Group �� ������ TabMounter �� [���� TabMounter ��ȣ]�� DEF_LAST_PER_GROUP �̴�.
	 *	: TabMounter �� Load ��ġ�� �̵��ϱ� ����, Next TabMounter �� ��ġ�� Ȯ���ؾ� �Ѵ�.
	 */
	int				m_iNextTabCarrierNo;

/************************************************************************/
/*                             IO Address                               */
/************************************************************************/


/************************************************************************/
/*                             Position                                 */
/************************************************************************/
	CString m_strPosFileName;
	CString m_strOffsetPosFileName;
	CString m_strOffsetPosFilePath;

	/** TabCarrier X,Y �� Position ���� */
    STeachingInfo m_sXYTTeachingInfo;

	STeachingInfo m_sExpandTeachingInfo;

	STeachingInfo m_sZTeachingInfo; //SJ_YYK 150109 Add...

	/** TabCarrier X,Y�� ������ǥ�� */
	MPos_XYT	m_rgsXYTFixedPos[DEF_TABCARRIER_MAX_POSITION];

	/** TabCarrier X,Y�� Offset ��ǥ�� */
	MPos_XYT	m_rgsXYTOffsetPos[DEF_TABCARRIER_MAX_POSITION];

	/** TabCarrier X,Y�� ����ǥ�� */
	MPos_XYT	m_rgsXYTModelPos[DEF_TABCARRIER_MAX_POSITION];

	/** ExpandY�� ������ǥ�� */
	double	m_rgsExpandFixedPos[DEF_TABCARRIER_EXPAND_MAX_POS];

	/** ExpandY�� Offset ��ǥ�� */
	double	m_rgsExpandOffsetPos[DEF_TABCARRIER_EXPAND_MAX_POS];

	/** ExpandY�� ����ǥ�� */
	double	m_rgsExpandModelPos[DEF_TABCARRIER_EXPAND_MAX_POS];
	
	//SJ_YYK 150109 Add...
	double m_rgsZFixedPos[DEF_TABCARRIER_Z_MAX_POS];

	double m_rgsZModelPos[DEF_TABCARRIER_Z_MAX_POS];

	double m_rgsZOffsetPos[DEF_TABCARRIER_Z_MAX_POS];


/************************************************************************/
/*                             Align                                    */
/************************************************************************/
	/** PreAlign Mark Offset*/
	MPos_XYT	m_rgAlignMarkOffset;

	/** Align ���� ���� */
	BOOL		m_bMarkAligned;

	/** Align�ϱ� ���� T �� �� */
	double		m_dValueThetaAlign;
	

/************************************************************************/
/*                             ��Ÿ �޽���                              */
/************************************************************************/
	/** Log Message	*/
// jdy sesl	char	m_szLogMsg[250];

/************************************************************************/
/*                             Flag									    */
/************************************************************************/
	BOOL	m_bJogStats;
	
	/**(KKY)
	 * �̵��� �α� Carrier�� �浹���� Ȯ���ϴµ� ���
	 **/
	MPos_XYT					m_posTargetMove;
	BOOL						m_bMoving;
	static CCriticalSection		m_csMoveLock;

	MTabInfo					m_TabInfo;
/************************************************************************/
/*                             ETC									    */
/************************************************************************/

/************************************************************************/
/*      TabCarrier Component ���ο����� ����� Operation                */
/************************************************************************/

	//KKY
//@	void					SetMovingY(BOOL bMove, MPos_XYT posTarget);

	/**
	 * TabCarrier �̵����� Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : ��ǥ ��ġ ID
	 * @return	int (0 = Safety, Error Code = �� ��)
	 */
//	int checkInterlockForXY(MPos_XY posTarget, int iPosID);
	int checkInterlockForXYT(MPos_XYT posTarget, int iPosID, BOOL bAuto = FALSE);

	// TabCarrier Up ���� Interlock Check.
	int checkInterlockForUp();

	int checkInterlockForDumpBasketFor();
	/**
	 * TabCarrier X,Y�� Model ��ǥ�� ����Ѵ�..
	 *
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int calculateXYTModelPosParameter();

	int calculateExpandPosParameter();

	int checkInterlockTabMounter4MoveUnload(int iPosID);

public:
	MTabCarrier();

	/** 
	 * ������
	 *
	 * @param commonData : ObjectI�� Log�� ���� Component Common Data
	 * @param listRefComponent : ������ Component ������ ����Ʈ
	 * @param datComponent : TabCarrier Data
	 */
	MTabCarrier( SCommonAttribute commonData, STabCarrierRefCompList listRefComponents, STabCarrierData datComponent );

    ~MTabCarrier();    

/************************************************************************/
/*        Component ���� ���� Interface                                 */
/************************************************************************/

	/**
	 * TabCarrier�� Data Parameter�� �����Ѵ�. 
	 *
	 * @param	datcomponent : ������ TabCarrier Parameter
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SetData(STabCarrierData datcomponent);

	/**
	 * TabCarrier�� Component List�� �����Ѵ�.
	 *
	 * @param	TabCarrierCompList : ������ TabCarrier Component List
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AssignComponents(STabCarrierRefCompList TabCarrierCompList);

	/**
	 * TabCarrier�� State Ȯ���� Component List�� �����Ѵ�.
	 *
	 * @param	sCompList : ������ State Ȯ���� ���� Component List
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AssignStateCheckComponents(SStateCheckCompListForTabCarrier sCompList);

	/**
	 * TabCarrier�� Motion Component�� ���� �޴´�.
	 *
	 * @param iAxisID : �� ID (0: X��, 1:Y��)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	IAxis* GetMotionComponent(int iAxisID);

	IAxis* GetExpandMotionComponent();

	IAxis* GetZMotionComponent(); //SJ_YYK 150109 Add...

	/**
	 * Model�� �������� TabCarrier�� �˸���.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int ChangeModel(void);

/************************************************************************/
/*        Teaching ���� ���� Interface                                  */
/************************************************************************/
	/**
	 * TabCarrier�� X,Y �� ���� ������ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	//int LoadXYFixedPosParameter(void);
	int LoadXYTFixedPosParameter(void);

	int LoadExpandFixedPosParameter(void);

	int LoadZFixedPosParameter(void); //SJ_YYK 150109 Add...

	/**
	 * TabCarrier�� X,Y �� ���� Offset��ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//	int LoadXYOffsetPosParameter(void);
	int LoadXYTOffsetPosParameter(void);

	int LoadExpandOffsetPosParameter(void);

	int LoadZOffsetPosParameter(void); //SJ_YYK 150109 Add...

	/**
	 * TabCarrier�� X,Y �� ���� ������ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgsFixedPos : ������ ������ǥ �迭
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//	int SaveXYFixedPosParameter(MPos_XY* rgsFixedPos);
	int SaveXYTFixedPosParameter(MPos_XYT* rgsFixedPos);

	int SaveExpandFixedPosParameter(double* rgsFixedPos);

	int SaveZFixedPosParamter(double* rgsFixedPos); //SJ_YYK 150109 Add..

	/**
	 * TabCarrier�� X,Y �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgsOffsetPos : ������ Offset��ǥ �迭
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//	int SaveXYOffsetPosParameter(MPos_XY* rgsOffsetPos);
	int SaveXYTOffsetPosParameter(MPos_XYT* rgsOffsetPos);

	int SaveExpandOffsetPosParameter(double* rgsOffsetPos);

	int SaveZOffsetPosParameter(double* rgsOffsetPos); //SJ_YYK 150109 Add..

	/**
	 * TabCarrier�� X,Y �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
	 *
	 * @param	rgsFixedPos : ������ǥ
	 * @param	rgsOffsetPos : Offset��ǥ
	 * @param	rgsModelPos : Model��ǥ
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//	int GetXYPosParameter(MPos_XY* rgsFixedPos, MPos_XY* rgsOffsetPos, MPos_XY* rgsModelPos);
	int GetXYTPosParameter(MPos_XYT* rgsFixedPos, MPos_XYT* rgsOffsetPos, MPos_XYT* rgsModelPos);

	int GetExpandPosParameter(double* rgsFixedPos, double* rgsOffsetPos, double* rgsModelPos);

	int GetZPosParameter(double* rgsFixedPos, double* rgsOffsetPos, double* rgsModelPos); //SJ_YYK 150109 Add..

	/**
     * TabCarrier X,Y Teaching ������ �д´�.
	 *
	 * @return	STeachingInfo : Teaching ����
     */
//	STeachingInfo GetXYTeachingInfo();
	STeachingInfo GetXYTTeachingInfo();

	STeachingInfo GetExpandTeachingInfo();

	STeachingInfo GetZTeachingInfo(); //SJ_YYK 150109 Add...

/************************************************************************/
/*        Vacuum Operation Interface                                    */
/************************************************************************/
	/**
	 * TabCarrier�� Vacuum�� �������� Ȯ���Ѵ�. �����̸� Success Return.
	 *
	 * @param	iVacID : Vacuum ID
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//	int CheckVacuum(int iVacID);
	int CheckVacuum();

	/**
	 * TabCarrier�� Vacuum�� �����Ѵ�.
	 *
	 * @param	iVacID : Vacuum ID
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//	int AbsorbTabIC(int iVacID, BOOL bSkipSensor = FALSE);
	int AbsorbTabIC(BOOL bSkipSensor = FALSE);
	
	/**
	 * TabCarrier�� Vacuum�� �����Ѵ�.
	 *
	 * @param	iVacID : Vacuum ID
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//	int ReleaseTabIC(int iVacID, BOOL bSkipSensor = FALSE);
	int ReleaseTabIC(BOOL bSkipSensor = FALSE);

	/**
	 * TabCarrier�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @param	iVacID : Vacuum ID
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
//	BOOL IsAbsorbTabIC(int iVacID);
	BOOL IsAbsorbTabIC();

	/**
	 * TabCarrier�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @param	iVacID : Vacuum ID
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
//	BOOL IsReleaseTabIC(int iVacID);
	BOOL IsReleaseTabIC();

/************************************************************************/
/*        Cylinder Operation Interface                                    */
/************************************************************************/
    /**
     * TabCarrier�� ��½�Ų��.
	 *
     * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
     * @return	int (0 = Success, Error Code = �� ��) 
     */
	int Up(BOOL bSkipSensor = FALSE);

    /**
     * TabCarrier�� �ϰ���Ų��.
	 *
     * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
     * @return	int (0 = Success, Error Code = �� ��) 
     */
	int Down(BOOL bSkipSensor = FALSE);

	/**
     * TabCarrier�� ��½�Ų��.
	 *
     * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
     * @return	int (0 = Success, Error Code = �� ��) 
     */
	int Up2(BOOL bSkipSensor = FALSE);

    /**
     * TabCarrier�� �ϰ���Ų��.
	 *
     * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
     * @return	int (0 = Success, Error Code = �� ��) 
     */
	int Down2(BOOL bSkipSensor = FALSE);

	/**
     * TabCarrier�� ������Ų��.
	 *
     * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
     * @return	int (0 = Success, Error Code = �� ��) 
     */
	int Forward(BOOL bSkipSensor = FALSE);

	/**
     * TabCarrier�� ������Ų��.
	 *
     * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
     * @return	int (0 = Success, Error Code = �� ��) 
     */
	int Backward(BOOL bSkipSensor = FALSE);

	/**
	 * TabCarrier�� ��µ� �������� Ȯ���Ѵ�.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Up, FALSE = �� ��)
	 */
	BOOL IsUp();

	/**
	 * TabCarrier�� �ϰ��� �������� Ȯ���Ѵ�.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Down, FALSE = �� ��)
	 */
	BOOL IsDown();

		/**
	 * TabCarrier�� ��µ� �������� Ȯ���Ѵ�.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Up, FALSE = �� ��)
	 */
	BOOL IsUp2();

	/**
	 * TabCarrier�� �ϰ��� �������� Ȯ���Ѵ�.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Down, FALSE = �� ��)
	 */
	BOOL IsDown2();

	/**
	 * TabCarrier�� ������ �������� Ȯ���Ѵ�.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Up, FALSE = �� ��)
	 */
	BOOL IsForward();

	/**
	 * TabCarrier�� ������ �������� Ȯ���Ѵ�.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Down, FALSE = �� ��)
	 */
	BOOL IsBackward();


	int ExpandShrinkForPreAlign(BOOL bSkipSensor = FALSE);
	int ShrinkCamera(BOOL bSkipSensor = FALSE);
	int ExpandCamera(BOOL bSkipSensor = FALSE);
	BOOL IsShrinkCamera();
	BOOL IsExpandCamera();

	int ForwardDumpBasket(BOOL bSkipSensor = FALSE);
	int BackwardDumpBasket(BOOL bSkipSensor = FALSE);
	BOOL IsForwardDumpBasket();
	BOOL IsBackwardDumpBasket();
/************************************************************************/
/*        IO Operation Interface                                    */
/************************************************************************/


/************************************************************************/
/*        TabCarrier Move Operation Interface                                    */
/************************************************************************/
	/**
	 * TabCarrier�� Index��ġ�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID		: Position Index ID
	 * @return	int			: (0 = Success, Error Code = �� ��)
	 */
//	int SafeMoveXYPos(int iPosID);
	int SafeMoveXYTPos(int iPosID, BOOL bAuto = TRUE);	

	int SafeMoveExpandPos(int iPosID, BOOL bWaitOpt=TRUE, BOOL bAuto = TRUE);

	int SafeMoveZPos(int iPosID, BOOL bWaitOpt = TRUE, BOOL bAuto = TRUE); //SJ_YYK 150109 Add..

	/**
	 * TabCarrier�� Index��ġ�� �̵���Ų��.
	 *
	 * @param	dTarget : ��ǥ ��ġ
	 * @param	iPosID : �̵��� Position Index
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
//	int MoveXYPos(MPos_XY posTarget, int iPosID, BOOL bWaitOpt = TRUE);
	int MoveXYTPos(MPos_XYT posTarget, int iPosID, BOOL bWaitOpt = TRUE, BOOL bAuto = TRUE);

	int MoveYTPos(int iPosID, BOOL bWaitOpt = TRUE, BOOL bAuto = TRUE); //SJ_YYK 150223 Add...

	int MoveXPos(double posTarget);
	int MoveYPos(double posTarget);
	int MoveTPos(double posTarget);

	int MoveExpandPos(double posTarget, int iPosID, BOOL bWaitOpt = TRUE, BOOL bAuto = TRUE);

	int MoveZPos(double posTarget, int iPosID, BOOL bWaitOpt = TRUE, BOOL bAuto = TRUE); //SJ_YYK 150109 Add..

/************************************************************************/
/*        TabCarrier PositionȮ�� Interface                                    */
/************************************************************************/
	/**
	 * �������� ���θ� Ȯ���Ѵ�. ��� �������Ͱ� �Ǿ� ������ Success Return, �ƴϸ� �ȵǾ� �ִ� �� Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckOrigin(void);
	int CheckOriginX(void);
	int CheckOriginY(void);
	int CheckOriginExpand();
	int CheckOriginZ(); //SJ_YYK 150109 Add...

	/**
	 * TabCarrier�� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	MPos_XY : TabCarrier�� ���� ��ġ
	 */
//	MPos_XY GetCurrentXYPos(BOOL bType = FALSE);
	MPos_XYT GetCurrentXYTPos(BOOL bType = FALSE);

	double	GetCurrentExpandPos(BOOL bType = FALSE);

	double GetCurrentZPos(BOOL bType = FALSE); //SJ_YYK 150105 Add.

	/**
	 * TabCarrier�� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	MPos_XY : TabCarrier�� ��ǥ ��ġ
	 */
//	MPos_XY GetTargetXYPos(int iPosID);
	MPos_XYT GetTargetXYTPos(int iPosID);

	double	GetTargetExpandPos(int iPosID);

	double GetTargetZPos(int iPosID); //SJ_YYK 150109 Add..

	/**
	 * TabCarrier X,Y�� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
//	BOOL IsInXYPos(int iPosID, MPos_XY* pTolerance = NULL);
	BOOL IsInXYTPos(int iPosID, MPos_XYT* pTolerance = NULL);
	BOOL IsInXPos(int iPosID, double* pTolerance = NULL);
	BOOL IsInYPos(int iPosID, double* pTolerance = NULL);
	BOOL IsInTPos(int iPosID, double* pTolerance = NULL);

	BOOL IsInExpandPos(int iPosID, double* pTolerance = NULL);

	BOOL IsInZPos(int iPosID, double* pTolerance = NULL); //SJ_YYK 150109 Add..

	/**
	 * TabCarrier X,Y�� ������ġ�� ������ġ�� ���Ѵ�.
	 *
	 * @param	dTargetPos : ������ġ�� ���� ������ġ
	 * @param	pTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
//	BOOL IsInXYPos(MPos_XY sTargetPos, MPos_XY* pTolerance = NULL);
	BOOL IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance = NULL);
	BOOL IsInXPos(double dTargetPos, double* pTolerance = NULL);
	BOOL IsInYPos(double dTargetPos, double* pTolerance = NULL);
	BOOL IsInTPos(double dTargetPos, double* pTolerance = NULL);

	BOOL IsInExpandPos(double dTargetPos, double* pTolerance = NULL);

	BOOL IsInZPos(double dTargetPos, double* pTolerance = NULL); //SJ_YYK 150109 Add..

	//KKY
	//������ ���� ���� ��ġ�� ComparePosID�� ��ġ�� ���Ѵ�.
	BOOL CompareCurrentPos(int CarrierAxisID, int ComparePosID, ECompareType CompareType);
	//������ ���� ���� ��ġ�� dComparePos�� ��ġ�� ���Ѵ�.
	BOOL CompareCurrentPos(int CarrierAxisID, double dComparePos, ECompareType CompareType);
	//������ ���� ��ǥ ��ġ�� ComparePosID�� ��ġ�� ���Ѵ�.
	BOOL CompareTargetPos(int CarrierAxisID, int TargetPosID, int ComparePosID, ECompareType CompareType);


	int VMoveVelocity(UINT nSel, BOOL dir);

	int VMovePitch(UINT nSel, BOOL dir);
	
	int VMoveEnd();

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

	/**(KKY)
	 * �̵��� �α� Carrier�� �浹���� Ȯ���ϴ� �Լ�
	 **/
	int	CheckCollisionOther(int iPosID);
	int CheckCollisionOther(MPos_XYT posTarget);
	MPos_XYT GetMovingTgPos();
	BOOL IsMovingY();//Y���� �̵�������...

	void		SetTabInfo(MTabInfo tabinfo);
	MTabInfo	GetTabInfo();

	int	GetWorkGroup() { return m_iWorkGroup; }
	int	GetWorkInstance() { return m_iWorkInstance; }

	BOOL IsShortPitchTab();	//5Pitch���� Short, 5Pitch�ʰ� Long

	MPos_XYT GetPrealignReadyPos();		//110128.Add
	MPos_XYT GetUnloadingReadyPos();	//110128.Add

	int		Wait4DoneXYT();
	void					SetMovingY(BOOL bMove, MPos_XYT posTarget);

	int StartSplineMove(int iMaxPoint, double *pdPosition, double *pdPosition2, double *pdVelocity, int *piAccelerate,
								BOOL bAutoFlag, BOOL bWaitOpt = TRUE); //SJ_YYK 150212 Add..
};
#endif // MTABCARRIER_H
