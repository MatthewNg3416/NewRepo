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
// 삭제 예정. #include "MTabData.h"
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
//    ICylinder*		m_plnkReelRecoveryUDCyl;	//Ver2. 추가 실린더
//    ICylinder*		m_plnkReelRecoveryFBCyl;	//Ver2. 추가 실린더

/************************************************************************/
/*                             Motion                                   */
/************************************************************************/

    IAxis*				m_plnkPressAxis;
	//Mech Ver2에서만_______
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

	//Mech Ver2에서만_________
	unsigned short	m_usOAddrSprocket1Clutch;

	unsigned short	m_usOPReelMotorPower;
	unsigned short	m_usOPReelMotorWind;
	unsigned short	m_usOPReelMotorRelease;
	unsigned short	m_usOPReelMotorBreak;
	//________________________
//_________________________________________________________________________________________________
// Input
	/** BadTab 감지를 위한 IO Address */
	unsigned short	m_usIAddrBadTabDetect[DEF_BADTABDETECTOR_MAX_SENSOR];

	/** Tension 추 상부 감지를 위한 IO Address */
	unsigned short	m_usIAddrTensionUpperLimitDetect;

	/** Tension 추 중간 감지를 위한 IO Address */
	unsigned short	m_usIAddrTensionUpperDetect;

	/** Tension Lower Detect */
	unsigned short	m_usIAddrTensionLowerDetect;

	/** Tension 추 하부 감지를 위한 IO Address */
	unsigned short	m_usIAddrTensionLowerLimitDetect;

	/** Supplier 감지를 위한 IO Address */
	unsigned short	m_usIAddrSupplierLargeDetect;
	unsigned short	m_usIAddrSupplierMiddleDetect;
	unsigned short	m_usIAddrSupplierSmallDetect;

	/** Reel 감지를 위한 IO Address */
	unsigned short	m_usIAddrSupplierCurrentReelDetect;
	unsigned short	m_usIAddrSupplierNewReelDetect;

	/** Rail 감지를 위한 IO Address */
	unsigned short	m_usIAddrRailUnfoldDetect;
//	unsigned short	m_usIAddrRailDiscardReelDetect;
	// Mold 감지를 위한 IO Address
	unsigned short	m_usIAddrMoldDetect;

	// 보호 필름 감지를 위한 IO Address
	unsigned short	m_usIAddrProtectDownDetect;

	// 보호 필름 회수부 Arm Up/Down센서(Mech Ver2에서 사용)___
	unsigned short	m_usIAddrProtectRecoveryArmUpDetect;
	unsigned short	m_usIAddrProtectRecoveryArmDownDetect;
	//________________________________________________________

/************************************************************************/
/*                             Position                                 */
/************************************************************************/
	CString			m_strPressPosFileName;
	CString			m_strPressOffsetPosFileName;
	CString			m_strPressOffsetPosFilePath;

	/** Press 축 Position 정보 */
    STeachingInfo	m_sPressTeachingInfo;
	double			m_rgsPressFixedPos[DEF_PRESS_MAX_POSITION];
	double			m_rgsPressOffsetPos[DEF_PRESS_MAX_POSITION];
	double			m_rgsPressModelPos[DEF_PRESS_MAX_POSITION];
	
	//Mech Ver2에서만___________
	CString			m_strMoldPinPosFileName;
	CString			m_strMoldPinOffsetPosFileName;
	CString			m_strMoldPinOffsetPosFilePath;

	/** MoldUD 축 Position 정보 */
    STeachingInfo	m_sMoldPinTeachingInfo;
	double			m_rgsMoldPinFixedPos[DEF_MOLDPIN_MAX_POSITION];
	double			m_rgsMoldPinOffsetPos[DEF_MOLDPIN_MAX_POSITION];
	double			m_rgsMoldPinModelPos[DEF_MOLDPIN_MAX_POSITION];
	//__________________________

	CString			m_strBadTabPosFileName;
	CString			m_strBadTabOffsetPosFileName;
	CString			m_strBadTabOffsetPosFilePath;

	/** BadTab 축 Position 정보 */
    STeachingInfo	m_sBadTabTeachingInfo;
	double			m_rgsBadTabFixedPos[DEF_BADTABDETECTOR_MAX_POSITION];
	double			m_rgsBadTabOffsetPos[DEF_BADTABDETECTOR_MAX_POSITION];
	double			m_rgsBadTabModelPos[DEF_BADTABDETECTOR_MAX_POSITION];
	
	CString			m_strSprocket1PosFileName;
	CString			m_strSprocket1OffsetPosFileName;
	CString			m_strSprocket1OffsetPosFilePath;

	/** Sprocket1 축 Position 정보 */
    STeachingInfo	m_sSprocket1TeachingInfo;
	double			m_rgsSprocket1FixedPos[DEF_SPROCKET_MAX_POSITION];
	double			m_rgsSprocket1OffsetPos[DEF_SPROCKET_MAX_POSITION];
	double			m_rgsSprocket1ModelPos[DEF_SPROCKET_MAX_POSITION];

	CString			m_strSprocket2PosFileName;
	CString			m_strSprocket2OffsetPosFileName;
	CString			m_strSprocket2OffsetPosFilePath;

	/** Sprocket2 축 Position 정보 */
    STeachingInfo	m_sSprocket2TeachingInfo;
	double			m_rgsSprocket2FixedPos[DEF_SPROCKET_MAX_POSITION];
	double			m_rgsSprocket2OffsetPos[DEF_SPROCKET_MAX_POSITION];
	double			m_rgsSprocket2ModelPos[DEF_SPROCKET_MAX_POSITION];

	CString			m_strSupplierPosFileName;
	CString			m_strSupplierOffsetPosFileName;
	CString			m_strSupplierOffsetPosFilePath;

	/** Supplier 축 Position 정보 */
    STeachingInfo	m_sSupplierTeachingInfo;
	double			m_rgsSupplierFixedPos[DEF_SUPPLIER_MAX_POSITION];
	double			m_rgsSupplierOffsetPos[DEF_SUPPLIER_MAX_POSITION];
	double			m_rgsSupplierModelPos[DEF_SUPPLIER_MAX_POSITION];

/************************************************************************/
/*                             기타			                            */
/************************************************************************/
	/** Log Message	*/
// jdy sesl	char	m_szLogMsg[250];

	// Sprocket이 얼마나 돌았는지 알려주는 코드..
	int		m_iSprocketRotateCount;

	// JDY
	BOOL			m_bSprocket1ReverseRotation;

	double				m_iOldTabEndMarkDistance;

/************************************************************************/
/*                             Flag									      */
/************************************************************************/
	BOOL	m_bJogStats;

/************************************************************************/
/*      TabFeeder Component 내부에서만 사용할 Operation                 */
/************************************************************************/
	/**
	 * Press축 Model 좌표를 계산한다..
	 *
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int calculatePressModelPosParameter();

	/**
	 * BadTab축 Model 좌표를 계산한다..
	 *
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int calculateBadTabModelPosParameter();

	/**
	 * Sprocket1축 Model 좌표를 계산한다..
	 *
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int calculateSprocket1ModelPosParameter();

	/**
	 * Sprocket2축 Model 좌표를 계산한다..
	 *
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int calculateSprocket2ModelPosParameter();

	/**
	 * Supplier축 Model 좌표를 계산한다..
	 *
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int calculateSupplierModelPosParameter();

	int calculateMoldPinModelPosParameter();

	double getRotationAlgne();

public:
	MTabFeeder();

	/** 
	 * 생성자
	 *
	 * @param commonData : ObjectI나 Log와 같은 Component Common Data
	 * @param listRefComponent : 참조할 Component 포인터 리스트
	 * @param datComponent : TabFeeder Data
	 */
	MTabFeeder(SCommonAttribute commonData, STabFeederRefCompList listRefComponents, STabFeederData datComponent);

    ~MTabFeeder();    

	// Log 찍기 위해서..임시로..hongju
	int WriteMultiMoveLog(CString strLog);
	int WriteAutoChangeLog(CString strLog, int iResult);

/************************************************************************/
/*        Component 공통 제공 Interface                                 */
/************************************************************************/

	/**
	 * TabFeeder에 Data Parameter를 설정한다. 
	 *
	 * @param	datcomponent : 설정할 TabFeeder Parameter
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SetData(STabFeederData datcomponent);

	/**
	 * TabFeeder에 Component List를 설정한다.
	 *
	 * @param	TabFeederCompList : 설정할 TabFeeder Component List
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AssignComponents(STabFeederRefCompList TabFeederCompList);

	/**
	 * TabFeeder에 State 확인할 Component List를 설정한다.
	 *
	 * @param	sCompList : 설정할 State 확인을 위한 Component List
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AssignStateCheckComponents(SStateCheckCompListForTabFeeder sCompList);

	int AssignCtrlComponents(SCtrlCompListForTabFeeder sCompList);	//110610.KKY
	/**
	 * TabFeeder의 Motion Component를 돌려 받는다.
	 *
	 * @param nAxisID : 축 ID (0: X축, 1:Y축 , 2:T축, 3:Z축)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	IAxis* GetMotionComponent(int nAxisID);
	
	/**
	 * Model을 변경함을 TabFeeder에 알린다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int ChangeModel(void);

/************************************************************************/
/*        Teaching 관련 제공 Interface                                  */
/************************************************************************/
	/**
	 * Press 축 관련 고정좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadPressFixedPosParameter(void);
	/**
	 * Press 축 관련 Offset좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadPressOffsetPosParameter(void);
	/**
	 * Press 축 관련 고정좌표 Data를 File에 저장한다.
	 *
	 * @param	rgsFixedPos : 저장할 고정좌표 배열 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SavePressFixedPosParameter(double* rgsFixedPos);
	/**
	 * Press 축 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	rgsOffsetPos : 저장할 Offset좌표 배열 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SavePressOffsetPosParameter(double* rgsOffsetPos);
	/**
	 * Press 축 좌표 관련 모든 Data를 반환 한다.
	 *
	 * @param	dFixedPos : 고정좌표 배열
	 * @param	dOffsetPos : Offset좌표 배열 
	 * @param	dModelPos : Model좌표 배열 
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int GetPressPosParameter(double* rgsFixedPos, double* rgsOffsetPos);
	/**
     * Press Teaching 정보를 읽는다.
	 *
	 * @return	STeachingInfo : Teaching 정보
     */
	STeachingInfo GetPressTeachingInfo();

	/**
	 * BadTab 축 관련 Offset좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadBadTabOffsetPosParameter();

	/**
	 * BadTab 축 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	rgsOffsetPos : 저장할 Offset좌표 배열 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveBadTabOffsetPosParameter(double* rgsOffsetPos);
	/**
	 * TabFeeder의 BadTab 축 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	iPosition : 저장할 Offset Position
	 * @param	rgsOffsetPos : 저장할 Offset좌표 값
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveBadTabOffsetPosParameter(int iPosition, double dOffsetPos);
	/**
	 * BadTab 축 좌표 관련 모든 Data를 반환 한다.
	 *
	 * @param	dFixedPos : 고정좌표 배열 
	 * @param	dOffsetPos : Offset좌표 배열 
	 * @param	dModelPos : Model좌표 배열 
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int GetBadTabPosParameter(double* rgsOffsetPos, double* rgsModelPos);
	/**
     * BadTab Z Teaching 정보를 읽는다.
	 *
	 * @return	STeachingInfo : Teaching 정보
     */
	STeachingInfo GetBadTabTeachingInfo();

	//_________________________________________________________________________________________________________
	// Sprocket1 관련 함수
	/**
	 * TabFeeder의 Sprocket1 축 관련 Offset좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadSprocket1OffsetPosParameter(void);

	/**
	 * TabFeeder의 Sprocket1 축 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	rgsOffsetPos : 저장할 Offset좌표 배열
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveSprocket1OffsetPosParameter(double* rgsOffsetPos);

	/**
	 * TabFeeder의 Sprocket1 축 좌표 관련 모든 Data를 반환 한다.
	 *
	 * @param	rgsFixedPos : 고정좌표 배열
	 * @param	rgsOffsetPos : Offset좌표 배열
	 * @param	rgsModelPos : Model좌표 배열
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int GetSprocket1PosParameter(double* rgsOffsetPos, double* rgsModelPos);

	/**
	 * TabFeeder Sprocket1 Teaching 정보를 읽는다.
	 *
	 * @return	STeachingInfo : Teaching 정보
	 */
	STeachingInfo GetSprocket1TeachingInfo();

	//_________________________________________________________________________________________________________
	// Sprocket2 관련 함수
	/**
	 * TabFeeder의 Sprocket2 축 관련 Offset좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadSprocket2OffsetPosParameter(void);

	/**
	 * TabFeeder의 Sprocket1 축 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	rgsOffsetPos : 저장할 Offset좌표 배열 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveSprocket2OffsetPosParameter(double* rgsOffsetPos);

	/**
	 * TabFeeder의 Sprocket1 축 좌표 관련 모든 Data를 반환 한다.
	 *
	 * @param	rgsFixedPos : 고정좌표 배열
	 * @param	rgsOffsetPos : Offset좌표 배열
	 * @param	rgsModelPos : Model좌표 배열
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int GetSprocket2PosParameter(double* rgsOffsetPos, double* rgsModelPos);

	/**
	 * TabFeeder Sprocket1 Teaching 정보를 읽는다.
	 *
	 * @return	STeachingInfo : Teaching 정보
	 */
	STeachingInfo GetSprocket2TeachingInfo();

	/**
	 * Supplier 축 관련 고정좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadSupplierFixedPosParameter(void);
	/**
	 * Supplier 축 관련 Offset좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadSupplierOffsetPosParameter(void);
	/**
	 * Supplier 축 관련 고정좌표 Data를 File에 저장한다.
	 *
	 * @param	rgsFixedPos : 저장할 고정좌표 배열 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveSupplierFixedPosParameter(double* rgsFixedPos);
	/**
	 * Supplier 축 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	rgsOffsetPos : 저장할 Offset좌표 배열 (X, Y, T)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveSupplierOffsetPosParameter(double* rgsOffsetPos);
	/**
	 * Supplier 축 좌표 관련 모든 Data를 반환 한다.
	 *
	 * @param	dFixedPos : 고정좌표 배열
	 * @param	dOffsetPos : Offset좌표 배열 
	 * @param	dModelPos : Model좌표 배열 
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int GetSupplierPosParameter(double* rgsFixedPos, double* rgsOffsetPos);
	/**
     * Supplier Teaching 정보를 읽는다.
	 *
	 * @return	STeachingInfo : Teaching 정보
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
	 * Press의 Vacuum이 정상인지 확인한다. 정상이면 Success Return.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckPressVacuum();
	/**
	 * Press의 Vacuum을 압축한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AbsorbPress(BOOL bSkipSensor = FALSE);
	/**
	 * Press의 Vacuum을 해제한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int ReleasePress(BOOL bSkipSensor = FALSE);
	/**
	 * Press의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
	BOOL IsAbsorbPress();
	/**
	 * TabFeeder의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
	BOOL IsReleasePress();

	/**
	 * AttachNewReel의 Vacuum이 정상인지 확인한다. 정상이면 Success Return.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckNewReelVacuum();
	/**
	 * AttachNewReel의 Vacuum을 압축한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AbsorbNewReel(BOOL bSkipSensor = FALSE);
	/**
	 * AttachNewReel의 Vacuum을 해제한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int ReleaseNewReel(BOOL bSkipSensor = FALSE);
	/**
	 * AttachNewReel의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
	BOOL IsAbsorbNewReel();
	/**
	 * TabFeeder의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
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
	 * @return	BOOL : (TRUE = Panel 있음, FALSE = Panel 없음)
	 */
	BOOL IsBadTabDetected(int iSensorNo);

	BOOL IsPressMoldDetect(void);

	// 보호지 감지 센서...
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
	 * Press Z를 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SafeMovePressPos(int iPosID);
	/**
	 * Press Z를 이동시킨다.
	 *
	 * @param	posTarget : 목표 위치
	 * @param	iPosID : 이동할 Position Index
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int MovePressPos(double posTarget, int iPosID, BOOL wait = TRUE);

	int StartMovePressPos(int iPosID);

	int Wait4DoneMovePressPos(int iPosID);

	/**
	 * BadTab Z를 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SafeMoveBadTabPos(int iPosID);
	/**
	 * BadTab Y를 이동시킨다.
	 *
	 * @param	posTarget : 목표 위치
	 * @param	iPosID : 이동할 Position Index
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int MoveBadTabPos(double posTarget, int iPosID);

	/**
	 * Supply T를 이동시킨다. 
	 *
	 * @param	dVelocity : VMode 속도
	 * @param	bDir : 정/역 방향
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int VMoveSprocket1(double dVelocity = 0.0, BOOL bDir = TRUE);
	/**
	 * Supply T를 정지시킨다.. 
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int VStopSprocket1();

	/**
	 * Supply T를 이동시킨다. 
	 *
	 * @param	dVelocity : VMode 속도
	 * @param	bDir : 정/역 방향
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int VMoveSprocket2(double dVelocity = 0.0, BOOL bDir = TRUE);
	/**
	 * Supply T를 정지시킨다.. 
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int VStopSprocket2();

	/**
	 * Sprocket T를 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SafeRMoveSprocket1Pos(int iPosID);

	/**
	 * Sprocket T를 TabType에 따라 이동시킨다.
	 *
	 * @param	posTarget : 목표 위치
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int RMoveSprocket1Pos(double posTarget, BOOL bControlSprocket2 = TRUE, double dVelocity = 0.0);

	/**
	 * Sprocket2 를 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SafeRMoveSprocket2Pos(int iPosID);

	/**
	 * Sprocket2 를 TabType에 따라 이동시킨다.
	 *
	 * @param	posTarget : 목표 위치
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int RMoveSprocket2Pos(double posTarget, BOOL bControlSprocket1 = TRUE, double dVelocity = 0.0);

	/**
	 * Supplier Z를 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SafeMoveSupplierPos(int iPosID);
	/**
	 * Supplier Z를 이동시킨다.
	 *
	 * @param	posTarget : 목표 위치
	 * @param	iPosID : 이동할 Position Index
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int MoveSupplierPos(double posTarget, int iPosID);

	/**
	 * Sprocket1,2 를 동시 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SafeRMultiMoveSprocketPos(int iPosID);

	/**
	 * Sprocket1,2를 동시 이동시킨다.
	 *
	 * @param	posTarget : 목표 위치
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int RMultiMoveSprocketPos(double posTarget, BOOL bWaitOption = TRUE);

	BOOL IsInMotionSprocket2();


	int SafeMoveMoldPinPos(int iPosID, BOOL wait = TRUE);
	int MoveMoldPinPos(double posTarget, int iPosID, BOOL wait = TRUE);
	int Wait4DoneMoldPin();
/************************************************************************/
/*        TabFeeder Position확인 Interface                                    */
/************************************************************************/
	/**
	 * Press 원점복귀 여부를 확인한다. 모두 원점복귀가 되어 있으면 Success Return, 아니면 안되어 있는 축 Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckPressOrigin(void);
	/**
	 * Press Z의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : TabFeeder의 현재 위치 (X, Y, T축)
	 */
	double GetPressCurrentPos(BOOL bType = FALSE);
	/**
	 * Press Z의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabFeeder의 목표 위치 (X, Y, T축)
	 */
	double GetPressTargetPos(int iPosID);
	/**
	 * Press Z의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInPressPos(int iPosID, double* dTolerance = NULL);

	/**
	 * BadTab 원점복귀 여부를 확인한다. 모두 원점복귀가 되어 있으면 Success Return, 아니면 안되어 있는 축 Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckBadTabOrigin();
	/**
	/**
	 * BadTab의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : TabFeeder의 현재 위치
	 */
	double GetBadTabCurrentPos(BOOL bType = FALSE);
	/**
	 * BadTab Y의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabFeeder의 목표 위치
	 */
	double GetBadTabTargetPos(int iPosID);
	/**
	 * BadTab Y의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInBadTabPos(int iPosID, double* dTolerance = NULL);

	/**
	 * Sprocket 원점복귀 여부를 확인한다. 모두 원점복귀가 되어 있으면 Success Return, 아니면 안되어 있는 축 Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckSprocket1Origin(void);
	/**
	 * Sprocket1의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : TabFeeder의 현재 위치 (X, Y, T축)
	 */
	double GetSprocket1CurrentPos(BOOL bType = FALSE);
	/**
	 * Sprocket1의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabFeeder의 목표 위치
	 */
	double GetSprocket1TargetPos(int iPosID);
	/**
	 * Sprocket1의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInSprocket1Pos(int iPosID, double* dTolerance = NULL);

	/**
	 * Sprocket 원점복귀 여부를 확인한다. 모두 원점복귀가 되어 있으면 Success Return, 아니면 안되어 있는 축 Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckSprocket2Origin(void);
	/**
	 * Sprocket1의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : TabFeeder의 현재 위치 (X, Y, T축)
	 */
	double GetSprocket2CurrentPos(BOOL bType = FALSE);
	/**
	 * Sprocket1의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabFeeder의 목표 위치
	 */
	double GetSprocket2TargetPos(int iPosID);
	/**
	 * Sprocket1의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInSprocket2Pos(int iPosID, double* dTolerance = NULL);

	/**
	 * Supplier 원점복귀 여부를 확인한다. 모두 원점복귀가 되어 있으면 Success Return, 아니면 안되어 있는 축 Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckSupplierOrigin(void);
	/**
	 * Supplier Z의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : TabFeeder의 현재 위치 (X, Y, T축)
	 */
	double GetSupplierCurrentPos(BOOL bType = FALSE);
	/**
	 * Supplier Z의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabFeeder의 목표 위치 (X, Y, T축)
	 */
	double GetSupplierTargetPos(int iPosID);
	/**
	 * Supplier Z의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInSupplierPos(int iPosID, double* dTolerance = NULL);

	// Sprocket1의 현재 위치를 입력된 값으로 반영한다.
	void SetSprocket1CurrentPos (double dTarget);

	// Sprocket2의 현재 위치를 입력된 값으로 반영한다.
	void SetSprocket2CurrentPos (double dTarget);

	// Sprocket1과 Sprocket2의 Tension을 유지한다.
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