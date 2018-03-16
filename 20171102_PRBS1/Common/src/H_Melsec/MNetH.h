//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// for T7-2
// MNetH.h: interface for the MNetH class.
//
//
//		Version		Updated		 Author		 Note
//      -------     -------      ------      ----
//		   1.0      2005/12/20   Grouchy	 Create	
//		   1.1      2006/02/07   cha		 Modify
//		   1.2      2006/02/09   cha		 Modify(TrimSpace추가)
//		   1.3      2006/02/15   cha		 Modify(FillSpace수정)
//		   1.4      2006/02/27   cha		 SetJobOrder_ToLowerEqWord()함수 추가
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNETH_H__50FAA8A4_1B84_45A6_88C5_D84DFE668D13__INCLUDED_)
#define AFX_MNETH_H__50FAA8A4_1B84_45A6_88C5_D84DFE668D13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		DEFAULT_CHANNEL		51
#define		DEFAULT_STATION		255

#define		RV_SUCCESS			0


#include "MNetHData.h"
#include "ICommon.h"
#include "MLCNetData.h" //SJ_YYK 150821 Add..

class MNetHDlg;

class MNetH : public virtual ICommon
{
public:
	MNetH(CString sIniFile="..\\Data\\MNetH.ini");	//
	virtual ~MNetH();
	
	void DestroyDlg();

public:
	int				m_nChannel;
	int				m_nStation;
	long			m_lPath;
	BOOL			m_fActive;
	CString			m_sIniFile;

	CStringArray	m_asLocalName;
	CStringArray	m_asUnitName;
	
	int				m_nCurLocal;
	int				m_nCurUnit;
	int				m_nPrevLocal;
	int				m_nNextLocal;
	int				m_nToUpperEqQty;
	int				m_nToLowerEqQty;

	int				m_nUnitQty;
	int				m_nLocal_Qty;

	bool			m_bSetGlassDataFlag;

	int				m_nT1;
	int				m_nT2;
	int				m_nT3;
	int				m_nDefaultTimeOut;
	DWORD			m_dwTransferStart;

	//	Common Variable for XSECNetDlg
	MNetHDlg		*m_pDlg;		//	Dialog Box Handle
	CWnd			*m_pMainWnd;	//	Main Window Handle
	bool			m_bShow;		//	Show type(true:Visible, false:Invisible)
	bool			m_bUseDialog;
	CRITICAL_SECTION	m_csMelsec; //SJ_YYK 160222 Add...
public:
	MNetHDlg**		GetMNetHDlg();
private:
	HANDLE		m_hMutex;
	short		MelsecOpen(); 
	short		MelsecClose(); 
	int			ReadConfig();

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PLC Address Function.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		GetPLCAddress(EGlassSummarys eItem, short *pnAddr, short *pnPoints, short nLocal=-1);
	long		GetPLCAddress(EToUpperEqs eItem, short *pnAddr, short *pnPoints, int nLocal=-1, int nUnit=-1);
	long		GetPLCAddress(EToLowerEqs eItem, short *pnAddr, short *pnPoints, int nLocal=-1, int nUnit=-1);
	short		GetPLCItemLen(EGlassSummarys eItem);
	short		GetPLCItemLen(EToLowerEqs eItem);
	short		GetPLCItemLen(EToUpperEqs eItem);
	short		GetPLCItemLen(EEqStates eItem);
	//SJ_YYK 160225 Modify..
	//short		GetPLCAddressBit(int nLocal, int nUnit, short *pnAddr, short *pnPoints, bool bUpperEq);
	short		GetPLCAddressBit(int nLocal, int nUnit, short *pnAddr, short *pnPoints, bool bUpperEq, BOOL bAOCBit = FALSE);
	short		GetPLCAddressWord_EO(short *pnAddr, short *pnPoints, int nLocal=-1);
	long		GetPLCAddressWord_EO(EEqStates eItem, short *pnAddr, short *pnPoints, int nLocal=-1);
	short		GetPLCAddressWord_ER(short *pnAddr, short *pnPoints, int nLocal=-1);
	long		GetPLCAddressWord_ER(EEqStates eItem, short *pnAddr, short *pnPoints, int nLocal=-1);
	short		GetPLCAddressWord_GS(short *pnAddr, short *pnPoints, int nLocal=-1);
	short		GetPLCAddressWord_Unit(short *pnAddr, short *pnPoints, bool bUpperEq, int nLocal=-1, int nUnit=-1);

	//SJ_YYK 150821 Add...
	long		GetPLCAddress(EMaterialInfo eMInfo, short *pnAddr, short *pnPoints, short nLocal=-1, int nUnit=-1);
	short		GetPLCItemLen(EMaterialInfo eItem);
	short		GetPLCAddressWord_MInfo(short *pnAddr, short *pnPoints, int nLocal=-1, int nUnit=-1);
	long		GetPLCAddressWord_MInfo(EMaterialInfo eItem, short *pnAddr, short *pnPoints, int nLocal=-1, int nUnit=-1);

	long		GetPLCAddress(eFDCinfo eFinfo, short *pnAddr, short *pnPoints, short nLocal=-1, int nUnit=-1);
	short		GetPLCItemLen(eFDCinfo eItem);
	short		GetPLCAddressWord_FDCInfo(short *pnAddr, short *pnPoints, int nLocal=-1, int nUnit=-1);
	long		GetPLCAddressWord_FDCInfo(eFDCinfo eItem, short *pnAddr, short *pnPoints, int nLocal=-1, int nUnit=-1);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Data Handling.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString		MakeString(short nValue, bool bSwap=true);
	long		MakeShortArray(CString sData, short *pnArray, short nLen, short nIndex=0, bool bSwap=true);


public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Initialize & Basic Function
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		Start();
	long		Stop();
	BOOL		IsConnected();
	void		ViewVisible(bool bView);
	void		MonitoringPLCArea();

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Data Conversion Function
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		HexToBin(CString sHex, CString& sBin);
	long		BinToHex(CString sBin, CString& sHex);
	long		HexToDec(CString sHex, long& lDec);
	long		HexToDec(CString sHex, CString& sDec);
	long		DecToHex(long lDec, CString& sHex);
	long		DecToHex(CString sDec, CString& sHex);

	// Add Function by cha 2006/02/07 
	void AscToString(char *pszOut, short *pnaBuf, short nPoints);
	void StringToAsc(WCHAR *pszIn, short *pnabuf, short nPoints);
	void FillSpace(WCHAR *pszIn, short nStr);
	void TrimSpace(char *pszIn);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Glass Function
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		SetGlassData(MToLowerEqs *pGD, EEqLocs eLoc=eOwnEq);
	long		GetGlassData(MToLowerEqs *pGD, EEqLocs eLoc=eUpperEq);


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Read Function
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		ReadLB(short nAddr, short nPoints, short *nRBuf, short nBufSize);
	long		ReadLW(short nAddr, short nPoints, short *nRBuf, short nBufSize);


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Write Function 
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		WriteLB(short nAddr, short nPoints, short *nRBuf, short nBufSize);
	long		WriteLW(short nAddr, short nPoints, short *nRBuf, short nBufSize);


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// To Upper Equipment Bit.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		GetToUpperEqBits(MToUpperEqBit *pUE, int nLocal=-1, int nUnit=-1);
	
	long		GetReserved0_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetMachinePause_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetMachineDown_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetMachineAlarm_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReceiveAble_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReceiveStart_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReceiveComplete_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetExchangeFlag_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReturnSendStart_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReturnSendComplete_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetImmediatelyPauseRequest_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetImmediatelyStopRequest_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReceiveAbleRemainedStep0_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReceiveAbleRemainedStep1_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReceiveAbleRemainedStep2_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReceiveAbleRemainedStep3_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetGlassIdReadComplete_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetLoadingStop_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetTransferStop_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved1_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved2_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved3_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);	
	long		GetReserved4_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);	
	long		GetReserved5_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetHandShakeCancelRequest_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetHandShakeAbortRequest_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetHandShakeResumeRequest_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetHandShakeRecoveryAckReply_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetHandShakeRecoveryNckReply_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetAction1Complete_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);	
	long		GetAction2Complete_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);	
	long		GetReceiveRefuse_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);

	long		SetToUpperEqBit_ToUpperEqBit(MToUpperEqBit *pUE, int nLocal=-1, int nUnit=-1);

	long		SetReserved0_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetMachinePause_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetMachineDown_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetMachineAlarm_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReceiveAble_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReceiveStart_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReceiveComplete_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetExchangeFlag_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReturnSendStart_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReturnSendComplete_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetImmediatelyPauseRequest_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetImmediatelyStopRequest_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReceiveAbleRemainedStep0_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReceiveAbleRemainedStep1_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReceiveAbleRemainedStep2_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReceiveAbleRemainedStep3_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetGlassIdReadComplete_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetLoadingStop_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetTransferStop_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved1_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved2_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved3_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);	
	long		SetReserved4_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);	
	long		SetReserved5_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetHandShakeCancelRequest_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetHandShakeAbortRequest_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetHandShakeResumeRequest_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetHandShakeRecoveryAckReply_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetHandShakeRecoveryNckReply_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetAction1Complete_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);	
	long		SetAction2Complete_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);	
	long		SetReceiveRefuse_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);



	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// To Lower Equipment Bit.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		GetToLowerEqBits(MToLowerEqBit *pLE, int nLocal=-1, int nUnit=-1);

	long		GetReserved0_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetMachinePause_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetMachineDown_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetMachineAlarm_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetSendAble_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetSendStart_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetSendComplete_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetExchangeFlag_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReturnReceiveStart_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReturnReceiveComplete_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetAction1Request_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetAction2Request_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);	
	long		GetReserved1_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);	
	long		GetReserved2_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetWorkStart_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetWorkCancel_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved3_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);	
	long		GetReserved4_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);	
	long		GetImmediatelyPauseRequest_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetImmediatelyStopRequest_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetSendAbleRemainedStep0_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetSendAbleRemainedStep1_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetSendAbleRemainedStep2_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetSendAbleRemainedStep3_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetHandShakeCancelRequest_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetHandShakeAbortRequest_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetHandShakeResumeRequest_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetHandShakeRecoveryAckReply_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetHandShakeRecoveryNckReply_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved5_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved6_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved7_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);

	long		SetToLowerEqBit_ToLowerEqBit(MToLowerEqBit *pLE, int nLocal=-1, int nUnit=-1);

	long		SetReserved0_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetMachinePause_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetMachineDown_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetMachineAlarm_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetSendAble_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetSendStart_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetSendComplete_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetExchangeFlag_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReturnReceiveStart_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReturnReceiveComplete_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetAction1Request_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetAction2Request_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);	
	long		SetReserved1_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);	
	long		SetReserved2_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetWorkStart_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetWorkCancel_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved3_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);	
	long		SetReserved4_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);	
	long		SetImmediatelyPauseRequest_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetImmediatelyStopRequest_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetSendAbleRemainedStep0_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetSendAbleRemainedStep1_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetSendAbleRemainedStep2_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetSendAbleRemainedStep3_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetHandShakeCancelRequest_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetHandShakeAbortRequest_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetHandShakeResumeRequest_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetHandShakeRecoveryAckReply_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetHandShakeRecoveryNckReply_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved5_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved6_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved7_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ContactState Bit.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//======================================================================================================
	// To Upper Eqp
	//======================================================================================================
	long		GetContactStateBit_ToUpperEq(BOOL *pnBit, int nPoint, int nLocal=-1, int nUnit=-1);
	long		SetContactStateBit_ToUpperEq(BOOL nBit, int nPoint, int nLocal=-1, int nUnit=-1);
	//======================================================================================================
	// To Lower Eqp
	//======================================================================================================
	long		GetContactStateBit_ToLowerEq(BOOL *pnBit, int nPoint, int nLocal=-1, int nUnit=-1);
	long		SetContactStateBit_ToLowerEq(BOOL nBit, int nPoint, int nLocal=-1, int nUnit=-1);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Arm Bit.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//======================================================================================================
	// To Upper Eqp
	//======================================================================================================
	long		GetRobotArmStateBit_ToUpperEq(MRobotArmStateBit *pRS, int nLocal=-1, int nUnit=-1);

	long		GetRobotAbnormal_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetTypeOfArm_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetTypeOfStageConveyor_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetArm1Violate_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetArm2Violate_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetArm1FoldComplete_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetArm2FoldComplete_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetArm1GlassCheckSensorOn_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetArm2GlassCheckSensorOn_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved0_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved1_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved2_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetManualOperation_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetRobotDirection_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved3_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved4_Robot_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);

	long		SetRobotArmStateBit_ToUpperEq(MRobotArmStateBit *pRS, int nLocal=-1, int nUnit=-1);

	long		SetRobotAbnormal_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetTypeOfArm_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetTypeOfStageConveyor_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetArm1Violate_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetArm2Violate_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetArm1FoldComplete_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetArm2FoldComplete_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetArm1GlassCheckSensorOn_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetArm2GlassCheckSensorOn_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved0_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved1_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved2_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetManualOperation_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetRobotDirection_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved3_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved4_Robot_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);

	//======================================================================================================
	// To Lower Eqp
	//======================================================================================================
	long		GetRobotArmStateBit_ToLowerEq(MRobotArmStateBit *pRS, int nLocal=-1, int nUnit=-1);

	long		GetRobotAbnormal_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetTypeOfArm_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetTypeOfStageConveyor_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetArm1Violate_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetArm2Violate_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetArm1FoldComplete_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetArm2FoldComplete_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetArm1GlassCheckSensorOn_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetArm2GlassCheckSensorOn_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved0_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved1_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved2_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetManualOperation_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetRobotDirection_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved3_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved4_Robot_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);

	long		SetRobotArmStateBit_ToLowerEq(MRobotArmStateBit *pRS, int nLocal=-1, int nUnit=-1);

	long		SetRobotAbnormal_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetTypeOfArm_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetTypeOfStageConveyor_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetArm1Violate_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetArm2Violate_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetArm1FoldComplete_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetArm2FoldComplete_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetArm1GlassCheckSensorOn_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetArm2GlassCheckSensorOn_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved0_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved1_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved2_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetManualOperation_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetRobotDirection_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved3_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved4_Robot_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Stage/Conveyor Bit.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//======================================================================================================
	// To Upper Eqp
	//======================================================================================================
	long		GetStageStateBit_ToUpperEq(MStageStateBit *pSS, int nLocal=-1, int nUnit=-1);
 
	long		GetStageAbnormal_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetTypeOfArm_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetTypeOfStageConveyor_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetEmpty_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetIdle_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetBusy_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetPinUp_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetPinDown_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetStopperUp_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetStopperDown_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetDoorOpen_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetDoorClose_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetManualOperation_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetBodyMoving_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetGlassCheckSensorOn_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved0_Stage_ToUpperEqBit(short *nValue, int nLocal=-1, int nUnit=-1);

	long		SetStageStateBit_ToUpperEq(MStageStateBit *pSS, int nLocal=-1, int nUnit=-1);

	long		SetStageAbnormal_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetTypeOfArm_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetTypeOfStageConveyor_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetEmpty_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetIdle_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetBusy_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetPinUp_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetPinDown_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetStopperUp_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetStopperDown_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetDoorOpen_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetDoorClose_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetManualOperation_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetBodyMoving_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetGlassCheckSensorOn_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved0_Stage_ToUpperEqBit(short nValue, int nLocal=-1, int nUnit=-1);

	//======================================================================================================
	// To Lower Eqp
	//======================================================================================================
	long		GetStageStateBit_ToLowerEq(MStageStateBit *pSS, int nLocal=-1, int nUnit=-1);
 
	long		GetStageAbnormal_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetTypeOfArm_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetTypeOfStageConveyor_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetEmpty_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetIdle_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetBusy_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetPinUp_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetPinDown_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetStopperUp_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetStopperDown_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetDoorOpen_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetDoorClose_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetManualOperation_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetBodyMoving_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetGlassCheckSensorOn_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved0_Stage_ToLowerEqBit(short *nValue, int nLocal=-1, int nUnit=-1);

	long		SetStageStateBit_ToLowerEq(MStageStateBit *pSS, int nLocal=-1, int nUnit=-1);

	long		SetStageAbnormal_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetTypeOfArm_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetTypeOfStageConveyor_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetEmpty_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetIdle_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetBusy_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetPinUp_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetPinDown_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetStopperUp_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetStopperDown_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetDoorOpen_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetDoorClose_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetManualOperation_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetBodyMoving_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetGlassCheckSensorOn_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved0_Stage_ToLowerEqBit(short nValue, int nLocal=-1, int nUnit=-1);


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Robot or Stage/Conveyor Bit.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		GetContactPointStateBit(MRobotArmStateBit *pRS, MStageStateBit *pSS, bool bUpperEq, int nLocal=-1, int nUnit=-1);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Job Summary Word.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		GetGlassSummaryWord(MGlassSummarys *pGS, int nLocal=-1);

	long		GetTGlassCount_GlassSum(CString *psValue, int nLocal=-1);
	long		GetTFTCount_GlassSum(CString *psValue, int nLocal=-1);				
	long		GetCFCount_GlassSum(CString *psValue, int nLocal=-1);					
	long		GetDummyCount_GlassSum(CString *psValue, int nLocal=-1);				
	long		GetBPGCount_GlassSum(CString *psValue, int nLocal=-1);					
	long		GetProcessID1_GlassSum(CString *psValue, int nLocal=-1);				
	long		GetStepID1_GlassSum(CString *psValue, int nLocal=-1);					
	long		GetBatchID1_GlassSum(CString *psValue, int nLocal=-1);				
	long		GetGlassCount1_GlassSum(CString *psValue, int nLocal=-1);					
	long		GetProcessID2_GlassSum(CString *psValue, int nLocal=-1);				
	long		GetStepID2_GlassSum(CString *psValue, int nLocal=-1);					
	long		GetBatchID2_GlassSum(CString *psValue, int nLocal=-1);				
	long		GetGlassCount2_GlassSum(CString *psValue, int nLocal=-1);					
	long		GetProcessID3_GlassSum(CString *psValue, int nLocal=-1);				
	long		GetStepID3_GlassSum(CString *psValue, int nLocal=-1);				
	long		GetBatchID3_GlassSum(CString *psValue, int nLocal=-1);				
	long		GetGlassCount3_GlassSum(CString *psValue, int nLocal=-1);	
	long		GetReserved_GlassSum(CString *psValue, int nLocal=-1);		

	long		SetGlassSummaryWord(MGlassSummarys *pGS, int nLocal=-1);

	long		SetTGlassCount_GlassSum(CString sValue, int nLocal=-1);
	long		SetTFTCount_GlassSum(CString sValue, int nLocal=-1);				
	long		SetCFCount_GlassSum(CString sValue, int nLocal=-1);					
	long		SetDummyCount_GlassSum(CString sValue, int nLocal=-1);				
	long		SetBPGCount_GlassSum(CString sValue, int nLocal=-1);					
	long		SetProcessID1_GlassSum(CString sValue, int nLocal=-1);				
	long		SetStepID1_GlassSum(CString sValue, int nLocal=-1);					
	long		SetBatchID1_GlassSum(CString sValue, int nLocal=-1);				
	long		SetGlassCount1_GlassSum(CString sValue, int nLocal=-1);					
	long		SetProcessID2_GlassSum(CString sValue, int nLocal=-1);				
	long		SetStepID2_GlassSum(CString sValue, int nLocal=-1);					
	long		SetBatchID2_GlassSum(CString sValue, int nLocal=-1);				
	long		SetGlassCount2_GlassSum(CString sValue, int nLocal=-1);					
	long		SetProcessID3_GlassSum(CString sValue, int nLocal=-1);				
	long		SetStepID3_GlassSum(CString sValue, int nLocal=-1);				
	long		SetBatchID3_GlassSum(CString sValue, int nLocal=-1);				
	long		SetGlassCount3_GlassSum(CString sValue, int nLocal=-1);	
	long		SetReserved_GlassSum(CString sValue, int nLocal=-1);		


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// EqState(Own) Word.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		GetEqStateWord_EO(MEqStates *pES, int nLocal=-1);

	long		GetEqState_EqState_EO(CString *psValue, int nLocal=-1);
	long		GetProcState_EqState_EO(CString *psValue, int nLocal=-1);			
	long		GetPPID_EqState_EO(CString *psValue, int nLocal=-1);					
	long		GetGlassSize_EqState_EO(CString *psValue, int nLocal=-1);			
	long		GetGlassThickness_EqState_EO(CString *psValue, int nLocal=-1);				
	long		GetTACTSet_EqState_EO(CString *psValue, int nLocal=-1);				
	long		GetTACTCurrent_EqState_EO(CString *psValue, int nLocal=-1);					
	long		GetRCDG_EqState_EO(CString *psValue, int nLocal=-1);		
	long		GetEOMode_EqState_EO(CString *psValue, int nLocal=-1);			
	long		GetERCMode_EqState_EO(CString *psValue, int nLocal=-1);			
	long		GetGMCMode_EqState_EO(CString *psValue, int nLocal=-1);			
	long		GetOperateMode_EqState_EO(CString *psValue, int nLocal=-1);			
	long		GetReserved_EqState_EO(CString *psValue, int nLocal=-1);

	long		SetEqStateWord_EO(MEqStates *pES, int nLocal=-1);

	long		SetEqState_EqState_EO(CString sValue, int nLocal=-1);
	long		SetProcState_EqState_EO(CString sValue, int nLocal=-1);			
	long		SetPPID_EqState_EO(CString sValue, int nLocal=-1);					
	long		SetGlassSize_EqState_EO(CString sValue, int nLocal=-1);			
	long		SetGlassThickness_EqState_EO(CString sValue, int nLocal=-1);				
	long		SetTACTSet_EqState_EO(CString sValue, int nLocal=-1);				
	long		SetTACTCurrent_EqState_EO(CString sValue, int nLocal=-1);					
	long		SetRCDG_EqState_EO(CString sValue, int nLocal=-1);		
	long		SetEOMode_EqState_EO(CString sValue, int nLocal=-1);			
	long		SetERCMode_EqState_EO(CString sValue, int nLocal=-1);			
	long		SetGMCMode_EqState_EO(CString sValue, int nLocal=-1);			
	long		SetOperateMode_EqState_EO(CString sValue, int nLocal=-1);			
	long		SetReserved_EqState_EO(CString sValue, int nLocal=-1);


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// EqState(Related) Word.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		GetEqStateWord_ER(MEqStates *pES, int nLocal=-1);

	long		GetEqState_EqState_ER(CString *psValue, int nLocal=-1);
	long		GetProcState_EqState_ER(CString *psValue, int nLocal=-1);			
	long		GetPPID_EqState_ER(CString *psValue, int nLocal=-1);					
	long		GetGlassSize_EqState_ER(CString *psValue, int nLocal=-1);			
	long		GetGlassThickness_EqState_ER(CString *psValue, int nLocal=-1);				
	long		GetTACTSet_EqState_ER(CString *psValue, int nLocal=-1);				
	long		GetTACTCurrent_EqState_ER(CString *psValue, int nLocal=-1);					
	long		GetRCDG_EqState_ER(CString *psValue, int nLocal=-1);		
	long		GetEOMode_EqState_ER(CString *psValue, int nLocal=-1);			
	long		GetERCMode_EqState_ER(CString *psValue, int nLocal=-1);			
	long		GetGMCMode_EqState_ER(CString *psValue, int nLocal=-1);			
	long		GetOperateMode_EqState_ER(CString *psValue, int nLocal=-1);			
	long		GetReserved_EqState_ER(CString *psValue, int nLocal=-1);

	long		SetEqStateWord_ER(MEqStates *pES, int nLocal=-1);

	long		SetEqState_EqState_ER(CString sValue, int nLocal=-1);
	long		SetProcState_EqState_ER(CString sValue, int nLocal=-1);			
	long		SetPPID_EqState_ER(CString sValue, int nLocal=-1);					
	long		SetGlassSize_EqState_ER(CString sValue, int nLocal=-1);			
	long		SetGlassThickness_EqState_ER(CString sValue, int nLocal=-1);				
	long		SetTACTSet_EqState_ER(CString sValue, int nLocal=-1);				
	long		SetTACTCurrent_EqState_ER(CString sValue, int nLocal=-1);					
	long		SetRCDG_EqState_ER(CString sValue, int nLocal=-1);		
	long		SetEOMode_EqState_ER(CString sValue, int nLocal=-1);			
	long		SetERCMode_EqState_ER(CString sValue, int nLocal=-1);			
	long		SetGMCMode_EqState_ER(CString sValue, int nLocal=-1);			
	long		SetOperateMode_EqState_ER(CString sValue, int nLocal=-1);			
	long		SetReserved_EqState_ER(CString sValue, int nLocal=-1);


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// To Upper Equipment Word.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		GetToUpperEqWord(MToUpperEqs *pUE, int nLocal=-1, int nUnit=-1);

	long		GetReadingGlassID_UpperEq(CString *psValue, int nLocal=-1, int nUnit=-1);
	long		GetReadingGlassIDType_UpperEq(CString *psValue, int nLocal=-1, int nUnit=-1);			
	long		GetGlassType_UpperEq(CString *psValue, int nLocal=-1, int nUnit=-1);
	long		GetReceiveRefuseCode_UpperEq(CString *psValue, int nLocal=-1, int nUnit=-1);
	long		GetReserved_UpperEq(CString *psValue, int nLocal=-1, int nUnit=-1);		
	long		GetCSIF_UpperEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetAS_UpperEq(CString *psValue, int nLocal=-1, int nUnit=-1);			
	long		GetAPS_UpperEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	
	long		SetToUpperEqWord(MToUpperEqs *pUE, int nLocal=-1, int nUnit=-1);
	
	long		SetReadingGlassID_UpperEq(CString sValue, int nLocal=-1, int nUnit=-1);
	long		SetReadingGlassIDType_UpperEq(CString sValue, int nLocal=-1, int nUnit=-1);			
	long		SetGlassType_UpperEq(CString sValue, int nLocal=-1, int nUnit=-1);
	long		SetReceiveRefuseCode_UpperEq(CString sValue, int nLocal=-1, int nUnit=-1);
	long		SetReserved_UpperEq(CString sValue, int nLocal=-1, int nUnit=-1);		
	long		SetCSIF_UpperEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetAS_UpperEq(CString sValue, int nLocal=-1, int nUnit=-1);			
	long		SetAPS_UpperEq(CString sValue, int nLocal=-1, int nUnit=-1);				



	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// To Lower Equipment Word.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		GetToLowerEqWord(MToLowerEqs *pLE, int nLocal=-1, int nUnit=-1);

	long		GetHPanelID_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);
	long		GetEPanelID_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);			
	long		GetBatchID_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetProdType_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);	
	long		GetProdKind_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetDeviceID_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);			
	long		GetStepID_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetPPID_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);			
	long		GetThickness_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetInsFlag_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetPanelSize_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetPanelPosition_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);			
	long		GetCount1_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetCount2_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetGrade_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetComment_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetCompSize_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetReadingFlag_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);					
	long		GetPanelState_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetJudgement_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);					
	long		GetCode_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);			
	long		GetRunLine_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);					
	long		GetUniqueID_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetPairHPanelID_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);					
	long		GetPairEPanelID_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);			
	long		GetPairGrade_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);					
	long		GetPairUniqueID_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);			
	long		GetFlowRecipe_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);					
	long		GetReserved0_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);	
	long		GetBitsSignals_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);
	long		GetReferData_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);
	long		GetCSIF_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);					
	long		GetAS_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);				
	long		GetAPS_LowerEq(CString *psValue, int nLocal=-1, int nUnit=-1);
	
	long		SetToLowerEqWord(MToLowerEqs *pLE, int nLocal=-1, int nUnit=-1);
	
	long		SetHPanelID_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);
	long		SetEPanelID_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);			
	long		SetBatchID_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetProdType_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);	
	long		SetProdKind_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetDeviceID_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);			
	long		SetStepID_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetPPID_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);			
	long		SetThickness_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetInsFlag_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetPanelSize_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetPanelPosition_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);			
	long		SetCount1_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetCount2_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetGrade_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetComment_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetCompSize_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetReadingFlag_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);					
	long		SetPanelState_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetJudgement_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);					
	long		SetCode_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);			
	long		SetRunLine_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);					
	long		SetUniqueId_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetPairHPanelID_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);					
	long		SetPairEPanelID_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);			
	long		SetPairGrade_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);					
	long		SetPairUniqueID_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);			
	long		SetFlowRecipe_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);					
	long		SetReserved0_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);	
	long		SetBitsSignals_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);
	long		SetReferData_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);
	long		SetCSIF_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);					
	long		SetAS_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);				
	long		SetAPS_LowerEq(CString sValue, int nLocal=-1, int nUnit=-1);

	//SJ_YYK 150821 Add..
	long		GetMaterialInfo(MMaterial *pMat, int nLocal=-1, int nUnit=-1);
	long		SetMaterialInfo(MMaterial *pMat, int nLocal=-1, int nUnit=-1);

	long		GetFDCInfo(MProcessParamInfo *pParam, int nLocal=-1, int nUnit=-1);
	long		SetFDCInfo(MProcessParamInfo *pParam, int nLocal=-1, int nUnit=-1);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Last Panel Cancel
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	long		SetJobOrder_ToLowerEqWord(bool fLastGlass, int nLocal=-1, int nUnit=-1);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Last Panel Cancel
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//SJ_YYK 160225 Modify..
	/*
	long		SetUpperEqBit(short eBitAddr, short nValue, int nUnit=0);
	long		SetLowerEqBit(short eBitAddr, short nValue, int nUnit=0);
	BOOL		IsOnUpperEqBit(short eBitAddr, int nUnit=0);
	BOOL		IsOnLowerEqBit(short eBitAddr, int nUnit=0);
	*/
	long		SetUpperEqBit(short eBitAddr, short nValue, int nUnit=0, BOOL bAOCBit = FALSE);
	long		SetLowerEqBit(short eBitAddr, short nValue, int nUnit=0, BOOL bAOCBit = FALSE);
	BOOL		IsOnUpperEqBit(short eBitAddr, int nUnit=0, BOOL bAOCBit = FALSE);
	BOOL		IsOnLowerEqBit(short eBitAddr, int nUnit=0, BOOL bAOCBit = FALSE);

	//NSMC KJS
	short GetPLCAddressNSMCBit(int nLocal, int nUnit, short *pnAddr, short *pnPoints);
	short GetPLCAddressNSMCWord(short *pnAddr, short *pnPoints, int nLocal, int nUnit);
	long GetNSMCWord(MModelChangeWordDatas *pLE, int nLocal = -1, int nUnit = -1);
//	BOOL IsOnNSMCEqBit(short eBitAddr, int nLocal = -1, int nUnit = -1);
		// Modify 110525 모델 예약 인자 추가
	BOOL IsOnNSMCEqBit(short eBitAddr, int nLocal = -1, int nUnit = -1, bool bReserve = false);
	long SetNSMCBit(short eBitAddr, short nValue);
	long SetNSMCWord(MModelChangeWordDatas *pLE, int nLocal = -1, int nUnit = -1);
};

#endif // !defined(AFX_MNETH_H__50FAA8A4_1B84_45A6_88C5_D84DFE668D13__INCLUDED_)
