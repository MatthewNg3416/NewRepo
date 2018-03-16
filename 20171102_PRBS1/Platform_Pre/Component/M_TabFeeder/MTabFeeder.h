/* 
 * TabFeeder Component
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
 * TabFeeder.h : Headerfile of stage component.
 */

#ifndef MTABFEEDER_H
#define MTABFEEDER_H

#include "ICommon.h"
#include "DefTabFeeder.h"
// ���� ����. #include "MTabData.h"
#include "IStateTabFeeder.h"

class MTabFeeder : public virtual ICommon, public IStateTabFeeder
{
private:   

/************************************************************************/
/*                             IO                                       */
/************************************************************************/
	IIO*			m_plnkIO;

/************************************************************************/
/*                             Vacuum                                   */
/************************************************************************/
	IVacuum*		m_plnkPressVac;			
	IVacuum*		m_plnkNewReelVac;			

/************************************************************************/
/*                             Cylinder                                 */
/************************************************************************/
	ICylinder*		m_plnkRecoverySprocketUDCyl;
	ICylinder*		m_plnkPressReelUDCyl;
	ICylinder*		m_plnkPressClampUDCyl;

	ICylinder*		m_plnkAttachCuttingSupportLeftUDCyl;
	ICylinder*		m_plnkAttachCuttingSupportRightUDCyl;
    ICylinder*		m_plnkAttachPressUDCyl;
    ICylinder*		m_plnkAttachReelCutterUDCyl;
    ICylinder*		m_plnkAttachReelFBCyl;
    ICylinder*		m_plnkSupplierChuckOCCyl;
//    ICylinder*		m_plnkReelRecoveryUDCyl;	//Ver2. �߰� �Ǹ���
//    ICylinder*		m_plnkReelRecoveryFBCyl;	//Ver2. �߰� �Ǹ���

/************************************************************************/
/*                             Motion                                   */
/************************************************************************/

    IAxis*				m_plnkPressAxis;
	//Mech Ver2������_______
	IAxis*				m_plnkMoldPinAxis;
	IAxis*				m_plnkFeederReelAxis;
	//______________________
    IAxis*				m_plnkBadTabAxis;
    IAxis*				m_plnkSprocket1Axis;
    IAxis*				m_plnkSprocket2Axis;
	IAxis*				m_plnkSupplierAxis;

	/** SprocketT MultiMotion Object */
	ISercosMultiAxis*	m_plnkMultiSprocketAxis;

/************************************************************************/
/*                             State Interface                          */
/************************************************************************/
	/**  Carrier Pointer	*/
//	IStateTabCarrier*	m_plnkCarrier;
	IStateTabCarrier*	m_plnkCarrier[DEF_MAX_TABCARRIER];

	IOpPanel*			m_plnkOpPanel;
	int					m_iGroupNo;

	MCtrlTabFeeder*		m_plnkCtrlTabFeeder;

/************************************************************************/
/*                             Data                                     */
/************************************************************************/
	/** System Data Object */
	MSystemData*		m_plnkSystemData;
	MPreBonderData*		m_plnkPreBonderData;

/************************************************************************/
/*                             IO Address                               */
/************************************************************************/

//_________________________________________________________________________________________________
// Output
	/** Mold Blow IO Address */
	unsigned short	m_usOAddrMoldBlowOn;

	/** Recovery Tension Motor On IO Address */
	unsigned short	m_usOAddrRecoveryTensionMotorOn;

	/** Supplier Tension Motor On IO Address */
	unsigned short	m_usOAddrSupplierTensionMotorOn;
	/** Supplier Large Tension IO Address */
	unsigned short	m_usOAddrSupplierLargeTension;
	/** Supplier Middle Tension IO Address */
	unsigned short	m_usOAddrSupplierMiddleTension;
	/** Supplier Small Tension IO Address */
	unsigned short	m_usOAddrSupplierSmallTension;

	/** Cleaning Motor On IO Address */
	unsigned short	m_usOAddrCleaningMotorOn;

	unsigned short	m_usOAddrPReelTensionMotorOn;
//	unsigned short	m_usOAddrPReelReverseMotorOn;

	/** RFID Reset IO Address */
	unsigned short	m_usOAddrPressRFIDReset;

	//Mech Ver2������_________
	unsigned short	m_usOAddrSprocket1Clutch;

	unsigned short	m_usOPReelMotorPower;
	unsigned short	m_usOPReelMotorWind;
	unsigned short	m_usOPReelMotorRelease;
	unsigned short	m_usOPReelMotorBreak;
	//________________________
//_________________________________________________________________________________________________
// Input
	/** BadTab ������ ���� IO Address */
	unsigned short	m_usIAddrBadTabDetect[DEF_BADTABDETECTOR_MAX_SENSOR];

	/** Tension �� ��� ������ ���� IO Address */
	unsigned short	m_usIAddrTensionUpperLimitDetect;

	/** Tension �� �߰� ������ ���� IO Address */
	unsigned short	m_usIAddrTensionUpperDetect;

	/** Tension Lower Detect */
	unsigned short	m_usIAddrTensionLowerDetect;

	/** Tension �� �Ϻ� ������ ���� IO Address */
	unsigned short	m_usIAddrTensionLowerLimitDetect;

	/** Supplier ������ ���� IO Address */
	unsigned short	m_usIAddrSupplierLargeDetect;
	unsigned short	m_usIAddrSupplierMiddleDetect;
	unsigned short	m_usIAddrSupplierSmallDetect;

	/** Reel ������ ���� IO Address */
	unsigned short	m_usIAddrSupplierCurrentReelDetect;
	unsigned short	m_usIAddrSupplierNewReelDetect;

	/** Rail ������ ���� IO Address */
	unsigned short	m_usIAddrRailUnfoldDetect;
//	unsigned short	m_usIAddrRailDiscardReelDetect;
	// Mold ������ ���� IO Address
	unsigned short	m_usIAddrMoldDetect;

	// ��ȣ �ʸ� ������ ���� IO Address
	unsigned short	m_usIAddrProtectDownDetect;

	// ��ȣ �ʸ� ȸ���� Arm Up/Down����(Mech Ver2���� ���)___
	unsigned short	m_usIAddrProtectRecoveryArmUpDetect;
	unsigned short	m_usIAddrProtectRecoveryArmDownDetect;
	//________________________________________________________

/************************************************************************/
/*                             Position                                 */
/************************************************************************/
	CString			m_strPressPosFileName;
	CString			m_strPressOffsetPosFileName;
	CString			m_strPressOffsetPosFilePath;

	/** Press �� Position ���� */
    STeachingInfo	m_sPressTeachingInfo;
	double			m_rgsPressFixedPos[DEF_PRESS_MAX_POSITION];
	double			m_rgsPressOffsetPos[DEF_PRESS_MAX_POSITION];
	double			m_rgsPressModelPos[DEF_PRESS_MAX_POSITION];
	
	//Mech Ver2������___________
	CString			m_strMoldPinPosFileName;
	CString			m_strMoldPinOffsetPosFileName;
	CString			m_strMoldPinOffsetPosFilePath;

	/** MoldUD �� Position ���� */
    STeachingInfo	m_sMoldPinTeachingInfo;
	double			m_rgsMoldPinFixedPos[DEF_MOLDPIN_MAX_POSITION];
	double			m_rgsMoldPinOffsetPos[DEF_MOLDPIN_MAX_POSITION];
	double			m_rgsMoldPinModelPos[DEF_MOLDPIN_MAX_POSITION];
	//__________________________

	CString			m_strBadTabPosFileName;
	CString			m_strBadTabOffsetPosFileName;
	CString			m_strBadTabOffsetPosFilePath;

	/** BadTab �� Position ���� */
    STeachingInfo	m_sBadTabTeachingInfo;
	double			m_rgsBadTabFixedPos[DEF_BADTABDETECTOR_MAX_POSITION];
	double			m_rgsBadTabOffsetPos[DEF_BADTABDETECTOR_MAX_POSITION];
	double			m_rgsBadTabModelPos[DEF_BADTABDETECTOR_MAX_POSITION];
	
	CString			m_strSprocket1PosFileName;
	CString			m_strSprocket1OffsetPosFileName;
	CString			m_strSprocket1OffsetPosFilePath;

	/** Sprocket1 �� Position ���� */
    STeachingInfo	m_sSprocket1TeachingInfo;
	double			m_rgsSprocket1FixedPos[DEF_SPROCKET_MAX_POSITION];
	double			m_rgsSprocket1OffsetPos[DEF_SPROCKET_MAX_POSITION];
	double			m_rgsSprocket1ModelPos[DEF_SPROCKET_MAX_POSITION];

	CString			m_strSprocket2PosFileName;
	CString			m_strSprocket2OffsetPosFileName;
	CString			m_strSprocket2OffsetPosFilePath;

	/** Sprocket2 �� Position ���� */
    STeachingInfo	m_sSprocket2TeachingInfo;
	double			m_rgsSprocket2FixedPos[DEF_SPROCKET_MAX_POSITION];
	double			m_rgsSprocket2OffsetPos[DEF_SPROCKET_MAX_POSITION];
	double			m_rgsSprocket2ModelPos[DEF_SPROCKET_MAX_POSITION];

	CString			m_strSupplierPosFileName;
	CString			m_strSupplierOffsetPosFileName;
	CString			m_strSupplierOffsetPosFilePath;

	/** Supplier �� Position ���� */
    STeachingInfo	m_sSupplierTeachingInfo;
	double			m_rgsSupplierFixedPos[DEF_SUPPLIER_MAX_POSITION];
	double			m_rgsSupplierOffsetPos[DEF_SUPPLIER_MAX_POSITION];
	double			m_rgsSupplierModelPos[DEF_SUPPLIER_MAX_POSITION];

/************************************************************************/
/*                             ��Ÿ			                            */
/************************************************************************/
	/** Log Message	*/
// jdy sesl	char	m_szLogMsg[250];

	// Sprocket�� �󸶳� ���Ҵ��� �˷��ִ� �ڵ�..
	int		m_iSprocketRotateCount;

	// JDY
	BOOL			m_bSprocket1ReverseRotation;

	double				m_iOldTabEndMarkDistance;

/************************************************************************/
/*                             Flag									      */
/************************************************************************/
	BOOL	m_bJogStats;

/************************************************************************/
/*      TabFeeder Component ���ο����� ����� Operation                 */
/************************************************************************/
	/**
	 * Press�� Model ��ǥ�� ����Ѵ�..
	 *
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int calculatePressModelPosParameter();

	/**
	 * BadTab�� Model ��ǥ�� ����Ѵ�..
	 *
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int calculateBadTabModelPosParameter();

	/**
	 * Sprocket1�� Model ��ǥ�� ����Ѵ�..
	 *
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int calculateSprocket1ModelPosParameter();

	/**
	 * Sprocket2�� Model ��ǥ�� ����Ѵ�..
	 *
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int calculateSprocket2ModelPosParameter();

	/**
	 * Supplier�� Model ��ǥ�� ����Ѵ�..
	 *
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int calculateSupplierModelPosParameter();

	int calculateMoldPinModelPosParameter();

	double getRotationAlgne();

public:
	MTabFeeder();

	/** 
	 * ������
	 *
	 * @param commonData : ObjectI�� Log�� ���� Component Common Data
	 * @param listRefComponent : ������ Component ������ ����Ʈ
	 * @param datComponent : TabFeeder Data
	 */
	MTabFeeder(SCommonAttribute commonData, STabFeederRefCompList listRefComponents, STabFeederData datComponent);

    ~MTabFeeder();    

	// Log ��� ���ؼ�..�ӽ÷�..hongju
	int WriteMultiMoveLog(CString strLog);
	int WriteAutoChangeLog(CString strLog, int iResult);

/************************************************************************/
/*        Component ���� ���� Interface                                 */
/************************************************************************/

	/**
	 * TabFeeder�� Data Parameter�� �����Ѵ�. 
	 *
	 * @param	datcomponent : ������ TabFeeder Parameter
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SetData(STabFeederData datcomponent);

	/**
	 * TabFeeder�� Component List�� �����Ѵ�.
	 *
	 * @param	TabFeederCompList : ������ TabFeeder Component List
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AssignComponents(STabFeederRefCompList TabFeederCompList);

	/**
	 * TabFeeder�� State Ȯ���� Component List�� �����Ѵ�.
	 *
	 * @param	sCompList : ������ State Ȯ���� ���� Component List
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AssignStateCheckComponents(SStateCheckCompListForTabFeeder sCompList);

	int AssignCtrlComponents(SCtrlCompListForTabFeeder sCompList);	//110610.KKY
	/**
	 * TabFeeder�� Motion Component�� ���� �޴´�.
	 *
	 * @param nAxisID : �� ID (0: X��, 1:Y�� , 2:T��, 3:Z��)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	IAxis* GetMotionComponent(int nAxisID);
	
	/**
	 * Model�� �������� TabFeeder�� �˸���.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int ChangeModel(void);

/************************************************************************/
/*        Teaching ���� ���� Interface                                  */
/************************************************************************/
	/**
	 * Press �� ���� ������ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadPressFixedPosParameter(void);
	/**
	 * Press �� ���� Offset��ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadPressOffsetPosParameter(void);
	/**
	 * Press �� ���� ������ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgsFixedPos : ������ ������ǥ �迭 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SavePressFixedPosParameter(double* rgsFixedPos);
	/**
	 * Press �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgsOffsetPos : ������ Offset��ǥ �迭 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SavePressOffsetPosParameter(double* rgsOffsetPos);
	/**
	 * Press �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
	 *
	 * @param	dFixedPos : ������ǥ �迭
	 * @param	dOffsetPos : Offset��ǥ �迭 
	 * @param	dModelPos : Model��ǥ �迭 
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int GetPressPosParameter(double* rgsFixedPos, double* rgsOffsetPos);
	/**
     * Press Teaching ������ �д´�.
	 *
	 * @return	STeachingInfo : Teaching ����
     */
	STeachingInfo GetPressTeachingInfo();

	/**
	 * BadTab �� ���� Offset��ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadBadTabOffsetPosParameter();

	/**
	 * BadTab �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgsOffsetPos : ������ Offset��ǥ �迭 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveBadTabOffsetPosParameter(double* rgsOffsetPos);
	/**
	 * TabFeeder�� BadTab �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	iPosition : ������ Offset Position
	 * @param	rgsOffsetPos : ������ Offset��ǥ ��
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveBadTabOffsetPosParameter(int iPosition, double dOffsetPos);
	/**
	 * BadTab �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
	 *
	 * @param	dFixedPos : ������ǥ �迭 
	 * @param	dOffsetPos : Offset��ǥ �迭 
	 * @param	dModelPos : Model��ǥ �迭 
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int GetBadTabPosParameter(double* rgsOffsetPos, double* rgsModelPos);
	/**
     * BadTab Z Teaching ������ �д´�.
	 *
	 * @return	STeachingInfo : Teaching ����
     */
	STeachingInfo GetBadTabTeachingInfo();

	//_________________________________________________________________________________________________________
	// Sprocket1 ���� �Լ�
	/**
	 * TabFeeder�� Sprocket1 �� ���� Offset��ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadSprocket1OffsetPosParameter(void);

	/**
	 * TabFeeder�� Sprocket1 �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgsOffsetPos : ������ Offset��ǥ �迭
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveSprocket1OffsetPosParameter(double* rgsOffsetPos);

	/**
	 * TabFeeder�� Sprocket1 �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
	 *
	 * @param	rgsFixedPos : ������ǥ �迭
	 * @param	rgsOffsetPos : Offset��ǥ �迭
	 * @param	rgsModelPos : Model��ǥ �迭
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int GetSprocket1PosParameter(double* rgsOffsetPos, double* rgsModelPos);

	/**
	 * TabFeeder Sprocket1 Teaching ������ �д´�.
	 *
	 * @return	STeachingInfo : Teaching ����
	 */
	STeachingInfo GetSprocket1TeachingInfo();

	//_________________________________________________________________________________________________________
	// Sprocket2 ���� �Լ�
	/**
	 * TabFeeder�� Sprocket2 �� ���� Offset��ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadSprocket2OffsetPosParameter(void);

	/**
	 * TabFeeder�� Sprocket1 �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgsOffsetPos : ������ Offset��ǥ �迭 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveSprocket2OffsetPosParameter(double* rgsOffsetPos);

	/**
	 * TabFeeder�� Sprocket1 �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
	 *
	 * @param	rgsFixedPos : ������ǥ �迭
	 * @param	rgsOffsetPos : Offset��ǥ �迭
	 * @param	rgsModelPos : Model��ǥ �迭
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int GetSprocket2PosParameter(double* rgsOffsetPos, double* rgsModelPos);

	/**
	 * TabFeeder Sprocket1 Teaching ������ �д´�.
	 *
	 * @return	STeachingInfo : Teaching ����
	 */
	STeachingInfo GetSprocket2TeachingInfo();

	/**
	 * Supplier �� ���� ������ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadSupplierFixedPosParameter(void);
	/**
	 * Supplier �� ���� Offset��ǥ Data�� File���� �о�´�.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int LoadSupplierOffsetPosParameter(void);
	/**
	 * Supplier �� ���� ������ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgsFixedPos : ������ ������ǥ �迭 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveSupplierFixedPosParameter(double* rgsFixedPos);
	/**
	 * Supplier �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
	 *
	 * @param	rgsOffsetPos : ������ Offset��ǥ �迭 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SaveSupplierOffsetPosParameter(double* rgsOffsetPos);
	/**
	 * Supplier �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
	 *
	 * @param	dFixedPos : ������ǥ �迭
	 * @param	dOffsetPos : Offset��ǥ �迭 
	 * @param	dModelPos : Model��ǥ �迭 
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int GetSupplierPosParameter(double* rgsFixedPos, double* rgsOffsetPos);
	/**
     * Supplier Teaching ������ �д´�.
	 *
	 * @return	STeachingInfo : Teaching ����
     */
	STeachingInfo GetSupplierTeachingInfo();


	int LoadMoldPinFixedPosParameter(void);
	int LoadMoldPinOffsetPosParameter(void);
	int SaveMoldPinFixedPosParameter(double* rgsFixedPos);
	int SaveMoldPinOffsetPosParameter(double* rgsOffsetPos);
	int GetMoldPinPosParameter(double* rgsFixedPos, double* rgsOffssetPos);
	STeachingInfo GetMoldPinTeachingInfo();
/************************************************************************/
/*        Vacuum Operation Interface                                    */
/************************************************************************/
	/**
	 * Press�� Vacuum�� �������� Ȯ���Ѵ�. �����̸� Success Return.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckPressVacuum();
	/**
	 * Press�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AbsorbPress(BOOL bSkipSensor = FALSE);
	/**
	 * Press�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int ReleasePress(BOOL bSkipSensor = FALSE);
	/**
	 * Press�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsAbsorbPress();
	/**
	 * TabFeeder�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsReleasePress();

	/**
	 * AttachNewReel�� Vacuum�� �������� Ȯ���Ѵ�. �����̸� Success Return.
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckNewReelVacuum();
	/**
	 * AttachNewReel�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int AbsorbNewReel(BOOL bSkipSensor = FALSE);
	/**
	 * AttachNewReel�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int ReleaseNewReel(BOOL bSkipSensor = FALSE);
	/**
	 * AttachNewReel�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsAbsorbNewReel();
	/**
	 * TabFeeder�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	BOOL IsReleaseNewReel();

	int HomingSprocket(int iSprocket);

/************************************************************************/
/*        Cylinder Operation Interface                                  */
/************************************************************************/

//	int OpenFeederSprocketCyl(BOOL bSkipSensor = FALSE);
//	int CloseFeederSprocketCyl(BOOL bSkipSensor = FALSE);
//	BOOL IsOpenFeederSprocketCyl();
//	BOOL IsCloseFeederSprocketCyl();

//	int OpenTensionSprocketCyl(BOOL bSkipSensor = FALSE);
//	int CloseTensionSprocketCyl(BOOL bSkipSensor = FALSE);
//	BOOL IsOpenTensionSprocketCyl();
//	BOOL IsCloseTensionSprocketCyl();

	int OpenRecoverySprocketCyl(BOOL bSkipSensor = FALSE);
	int CloseRecoverySprocketCyl(BOOL bSkipSensor = FALSE);
	BOOL IsOpenRecoverySprocketCyl();
	BOOL IsCloseRecoverySprocketCyl();

 
	int UnlockSupplierChuckCyl(BOOL bSkipSensor = FALSE);
	int LockSupplierChuckCyl(BOOL bSkipSensor = FALSE);
	BOOL IsUnlockSupplierChuckCyl();
	BOOL IsLockSupplierChuckCyl();

//	int UpReelRecoveryCyl(BOOL bSkipSensor = FALSE);
//	int DownReelRecoveryCyl(BOOL bSkipSensor = FALSE);
//	BOOL IsUpReelRecoveryCyl();
//	BOOL IsDownReelRecoveryCyl();

//	int ForwardReelRecoveryCyl(BOOL bSkipSensor = FALSE);
//	int BackwardReelRecoveryCyl(BOOL bSkipSensor = FALSE);
//	BOOL IsForwardReelRecoveryCyl();
//	BOOL IsBackwardReelRecoveryCyl();

	BOOL IsDetectProtectRecoveryArmUp();
	BOOL IsDetectProtectRecoveryArmDown();
	BOOL IsClutchOnSprocket1Axis();
	int	 ClutchOnSprocket1Axis();
	int  ClutchOffSprocket1Axis();
	int  PReelBreakOnOff(BOOL bOn);//110630
	BOOL IsPReelBreakOn();//110630

	//@____________________
	int	 PReelMotorWind();
	int	 PReelMotorRelease();
	int	 PReelMotorStop();
	BOOL IsPReelMotorWind();
	BOOL ISPReelMotorRelease();
	BOOL IsPReelMotorStop();
	//@____________________
/************************************************************************/
/*        IO Operation Interface							            */
/************************************************************************/

	/**
	 * BadTab Detect
	 *
	 * @return	BOOL : (TRUE = Panel ����, FALSE = Panel ����)
	 */
	BOOL IsBadTabDetected(int iSensorNo);

	BOOL IsPressMoldDetect(void);

	// ��ȣ�� ���� ����...
	BOOL IsDownProtectDetect(void);

	BOOL IsTensionUpperLimitDetected(void);
	BOOL IsTensionUpperDetected(void);
	BOOL IsTensionLowerDetected(void);
	BOOL IsTensionLowerLimitDetected(void);

	BOOL IsRailUnfoldDetected(void);
//	BOOL IsRailDiscardReelDtetcted(void);
	BOOL IsCurrentReelDtetcted(void);
	BOOL IsNewReelDtetcted(void);

	BOOL IsSupplierSmallTensionDetected(void);
	BOOL IsSupplierMiddleTensionDetected(void);
	BOOL IsSupplierLargeTensionDetected(void);

/************************************************************************/
/*        TabFeeder Move Operation Interface                            */
/************************************************************************/
	/**
	 * Press Z�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SafeMovePressPos(int iPosID);
	/**
	 * Press Z�� �̵���Ų��.
	 *
	 * @param	posTarget : ��ǥ ��ġ
	 * @param	iPosID : �̵��� Position Index
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int MovePressPos(double posTarget, int iPosID, BOOL wait = TRUE);

	int StartMovePressPos(int iPosID);

	int Wait4DoneMovePressPos(int iPosID);

	/**
	 * BadTab Z�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SafeMoveBadTabPos(int iPosID);
	/**
	 * BadTab Y�� �̵���Ų��.
	 *
	 * @param	posTarget : ��ǥ ��ġ
	 * @param	iPosID : �̵��� Position Index
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int MoveBadTabPos(double posTarget, int iPosID);

	/**
	 * Supply T�� �̵���Ų��. 
	 *
	 * @param	dVelocity : VMode �ӵ�
	 * @param	bDir : ��/�� ����
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int VMoveSprocket1(double dVelocity = 0.0, BOOL bDir = TRUE);
	/**
	 * Supply T�� ������Ų��.. 
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int VStopSprocket1();

	/**
	 * Supply T�� �̵���Ų��. 
	 *
	 * @param	dVelocity : VMode �ӵ�
	 * @param	bDir : ��/�� ����
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int VMoveSprocket2(double dVelocity = 0.0, BOOL bDir = TRUE);
	/**
	 * Supply T�� ������Ų��.. 
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int VStopSprocket2();

	/**
	 * Sprocket T�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SafeRMoveSprocket1Pos(int iPosID);

	/**
	 * Sprocket T�� TabType�� ���� �̵���Ų��.
	 *
	 * @param	posTarget : ��ǥ ��ġ
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int RMoveSprocket1Pos(double posTarget, BOOL bControlSprocket2 = TRUE, double dVelocity = 0.0);

	/**
	 * Sprocket2 �� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SafeRMoveSprocket2Pos(int iPosID);

	/**
	 * Sprocket2 �� TabType�� ���� �̵���Ų��.
	 *
	 * @param	posTarget : ��ǥ ��ġ
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int RMoveSprocket2Pos(double posTarget, BOOL bControlSprocket1 = TRUE, double dVelocity = 0.0);

	/**
	 * Supplier Z�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SafeMoveSupplierPos(int iPosID);
	/**
	 * Supplier Z�� �̵���Ų��.
	 *
	 * @param	posTarget : ��ǥ ��ġ
	 * @param	iPosID : �̵��� Position Index
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int MoveSupplierPos(double posTarget, int iPosID);

	/**
	 * Sprocket1,2 �� ���� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int SafeRMultiMoveSprocketPos(int iPosID);

	/**
	 * Sprocket1,2�� ���� �̵���Ų��.
	 *
	 * @param	posTarget : ��ǥ ��ġ
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int RMultiMoveSprocketPos(double posTarget, BOOL bWaitOption = TRUE);

	BOOL IsInMotionSprocket2();


	int SafeMoveMoldPinPos(int iPosID, BOOL wait = TRUE);
	int MoveMoldPinPos(double posTarget, int iPosID, BOOL wait = TRUE);
	int Wait4DoneMoldPin();
/************************************************************************/
/*        TabFeeder PositionȮ�� Interface                                    */
/************************************************************************/
	/**
	 * Press �������� ���θ� Ȯ���Ѵ�. ��� �������Ͱ� �Ǿ� ������ Success Return, �ƴϸ� �ȵǾ� �ִ� �� Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckPressOrigin(void);
	/**
	 * Press Z�� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : TabFeeder�� ���� ��ġ (X, Y, T��)
	 */
	double GetPressCurrentPos(BOOL bType = FALSE);
	/**
	 * Press Z�� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabFeeder�� ��ǥ ��ġ (X, Y, T��)
	 */
	double GetPressTargetPos(int iPosID);
	/**
	 * Press Z�� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInPressPos(int iPosID, double* dTolerance = NULL);

	/**
	 * BadTab �������� ���θ� Ȯ���Ѵ�. ��� �������Ͱ� �Ǿ� ������ Success Return, �ƴϸ� �ȵǾ� �ִ� �� Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckBadTabOrigin();
	/**
	/**
	 * BadTab�� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : TabFeeder�� ���� ��ġ
	 */
	double GetBadTabCurrentPos(BOOL bType = FALSE);
	/**
	 * BadTab Y�� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabFeeder�� ��ǥ ��ġ
	 */
	double GetBadTabTargetPos(int iPosID);
	/**
	 * BadTab Y�� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInBadTabPos(int iPosID, double* dTolerance = NULL);

	/**
	 * Sprocket �������� ���θ� Ȯ���Ѵ�. ��� �������Ͱ� �Ǿ� ������ Success Return, �ƴϸ� �ȵǾ� �ִ� �� Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckSprocket1Origin(void);
	/**
	 * Sprocket1�� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : TabFeeder�� ���� ��ġ (X, Y, T��)
	 */
	double GetSprocket1CurrentPos(BOOL bType = FALSE);
	/**
	 * Sprocket1�� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabFeeder�� ��ǥ ��ġ
	 */
	double GetSprocket1TargetPos(int iPosID);
	/**
	 * Sprocket1�� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInSprocket1Pos(int iPosID, double* dTolerance = NULL);

	/**
	 * Sprocket �������� ���θ� Ȯ���Ѵ�. ��� �������Ͱ� �Ǿ� ������ Success Return, �ƴϸ� �ȵǾ� �ִ� �� Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckSprocket2Origin(void);
	/**
	 * Sprocket1�� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : TabFeeder�� ���� ��ġ (X, Y, T��)
	 */
	double GetSprocket2CurrentPos(BOOL bType = FALSE);
	/**
	 * Sprocket1�� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabFeeder�� ��ǥ ��ġ
	 */
	double GetSprocket2TargetPos(int iPosID);
	/**
	 * Sprocket1�� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInSprocket2Pos(int iPosID, double* dTolerance = NULL);

	/**
	 * Supplier �������� ���θ� Ȯ���Ѵ�. ��� �������Ͱ� �Ǿ� ������ Success Return, �ƴϸ� �ȵǾ� �ִ� �� Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	int CheckSupplierOrigin(void);
	/**
	 * Supplier Z�� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : TabFeeder�� ���� ��ġ (X, Y, T��)
	 */
	double GetSupplierCurrentPos(BOOL bType = FALSE);
	/**
	 * Supplier Z�� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabFeeder�� ��ǥ ��ġ (X, Y, T��)
	 */
	double GetSupplierTargetPos(int iPosID);
	/**
	 * Supplier Z�� ������ġ�� Index��ġ�� ���Ѵ�.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
	 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
	 */
	BOOL IsInSupplierPos(int iPosID, double* dTolerance = NULL);

	// Sprocket1�� ���� ��ġ�� �Էµ� ������ �ݿ��Ѵ�.
	void SetSprocket1CurrentPos (double dTarget);

	// Sprocket2�� ���� ��ġ�� �Էµ� ������ �ݿ��Ѵ�.
	void SetSprocket2CurrentPos (double dTarget);

	// Sprocket1�� Sprocket2�� Tension�� �����Ѵ�.
	int TensionMaintenance(int iSprocket);


	int CheckMoldPinOrigin(void);
	double GetMoldPinCurrentPos(BOOL bType = FALSE);
	double GetMoldPinTargetPos(int iPosID);
	BOOL IsInMoldPinPos(int iPosID, double *dTolerance = NULL);
/***********************************************************************/
/*        Induction Motor, Tension Motor, Press						   */
/***********************************************************************/

	int MoldBlowOn();
	int MoldBlowOff();
	int RecoveryIndMotorOn();
	int RecoveryIndMotorOff();
	BOOL IsRecoveryIndMotorOn();
	BOOL IsRecoveryIndMotorOff();

	int BrushMotorOn();
	int BrushMotorOff();
	BOOL IsBrushMotorOn();
	BOOL IsBrushMotorOff();

	int PReelMotorOn();
	int PReelMotorOff(BOOL bBreakOn = TRUE);
	BOOL IsPReelMotorOn();
	BOOL IsPReelMotorOff();

//	int PReelReverseMotorOn();
//	int PReelReverseMotorOff();
//	BOOL IsPReelReverseMotorOn();
//	BOOL IsPReelReverseMotorOff();

	int MoveOriginFast(UINT nSel, BOOL dir);
	int MoveOriginSlow(UINT nSel, BOOL dir);

	// JDY
	const BOOL IsSprocket1ReverseRotation() const;

	BOOL IsServoOnFeederReel();
	BOOL IsServoOffFeederReel();
	int	 ServoOnOffFeederReel(BOOL bOn);
	int	 VMove_FeederReel(int Dir, double dVel = 0.0, int iAcl = 0);
	int	 VStop_FeederReel();
	int  CheckFeederReelOrigin();
};
#endif // MTABFEEDER_H