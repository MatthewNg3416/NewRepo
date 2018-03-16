// MLCNet.cpp: implementation of the MLCNet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MLCNet.h"
#include "MLCNetDlg.h"
#include "MLCNetData.h"

#include "MSystemData.h"
#include "MPlatformOlbSystem.h"	

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern MPlatformOlbSystem MOlbSystemPre;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MLCNet::MLCNet(CString szIP, int nPort)
{
	//Object Initial
	m_pDlg = NULL;
	m_pMainWnd = NULL;
	m_bShow = FALSE;

	//Data Initial
	m_szIP = szIP;
	m_nPort = nPort;
	m_bActive = TRUE;
	m_bSpool = FALSE;
	
	m_bConnected = false;

	m_nUnit = 0;
	m_eOpMode = eOpNothing;
	m_eEqState     = eEqNothing;
	m_eEqProcState = eEqPNothing;
	m_eMaterialPortState = 	eMaterialPortNothing;

	m_nMaxPanel = 0;
	m_szOnlineMode = "OFFLINE";

	m_pSVList = new CPtrList;		
	m_pECList = new CPtrList;		
	m_pEOList = new CPtrList;
	
	m_pPPList = new CPtrList;

	// 2006.08.16 GrouchyKim Add.
	m_fUpdateLCDateTime = false;

	//110620.KKY.Add_____
	m_pReceivedPanelData = new MGlassDatas;
	//___________________

    //2007.10.15 GrouchyKim Add.[v1.3]
	m_nLastAlarmOnOff = 0; 
	m_nLastAlarmId = 0;
	m_nLastAlarmCode = 0;
	m_strLastAlarmText = "NONE"; 

	m_saMultiTerminalCmd.RemoveAll();

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}

MLCNet::~MLCNet()
{
	MStateVariable*		pSV = NULL;
	MEqConstant*		pEC = NULL;
	MEqOnlineParam*		pEO = NULL;
	MProcessProgram*	pPP = NULL;


	if (m_pDlg != NULL)
	{
		m_pDlg->Detach();
		m_pDlg->DestroyWindow();
		delete m_pDlg; 
		m_pDlg = NULL;
	}

	//110620.KKY_____
	if (m_pReceivedPanelData)
		delete m_pReceivedPanelData;
	//_______________

	while (!m_pSVList->IsEmpty())
	{
		pSV = (MStateVariable*)m_pSVList->RemoveHead();
		delete pSV; 

		pSV = NULL;
	}
	delete m_pSVList; 
	m_pSVList = NULL;

	while (!m_pECList->IsEmpty())
	{
		pEC = (MEqConstant*)m_pECList->RemoveHead();
		delete pEC; 
		
		pEC = NULL;
	}
	delete m_pECList; 
	
	m_pECList = NULL;

	while (!m_pEOList->IsEmpty())
	{
		pEO = (MEqOnlineParam*)m_pEOList->RemoveHead();
		delete pEO; 
		
		pEO = NULL;
	}
	delete m_pEOList; 
	m_pEOList = NULL;

	while (!m_pPPList->IsEmpty())
	{
		pPP = (MProcessProgram*)m_pPPList->RemoveHead();
		delete pPP;
		
		pPP = NULL;
	}
	delete m_pPPList; 
	m_pPPList = NULL;

	MStateVariable* p1;
	while (m_caChangeParamStateVariable.GetSize() != 0)
	{
		p1 = (MStateVariable*)m_caChangeParamStateVariable.GetAt(0);
		m_caChangeParamStateVariable.RemoveAt(0);
		delete p1; 
		
		p1 = NULL;
	}

	MProcessProgram* p2;
	while (m_caProcessProgram.GetSize() != 0)
	{
		p2 = (MProcessProgram*)m_caProcessProgram.GetAt(0);
		m_caProcessProgram.RemoveAt(0);
		delete p2; 
		
		p2 = NULL;
	}

	MProductPlanBatchData* p3;
	while (m_caProductPlanBatchData.GetSize() != 0)
	{
		p3 = (MProductPlanBatchData*)m_caProductPlanBatchData.GetAt(0);
		m_caProductPlanBatchData.RemoveAt(0);
		delete p3; 
		
		p3 = NULL;
	}


	MBarCodeParsingInfo* p4;
	while (m_caBarCodeParsingInfo.GetSize() != 0)
	{
		p4 = (MBarCodeParsingInfo*)m_caBarCodeParsingInfo.GetAt(0);
		m_caBarCodeParsingInfo.RemoveAt(0);
		delete p4; 
		
		p4 = NULL;
	}

	//100807.ECID______________________________
	MEqConstant* p5;
	while(m_caEqConstData.GetSize() != 0)
	{
		p5 = (MEqConstant*)m_caEqConstData.GetAt(0);
		m_caEqConstData.RemoveAt(0);
		delete p5;

		p5 = NULL;
	}
	//_________________________________________

	//110621.KKY.Add_________
	MMaterialCodeInfo* p6;
	while(m_caMaterialCodeInfo.GetSize() != 0)
	{
		p6 = (MMaterialCodeInfo*)m_caMaterialCodeInfo.GetAt(0);
		p6->m_asM_CODE.RemoveAll();
		m_caMaterialCodeInfo.RemoveAt(0);
		delete p6;
		p6 = NULL;
	}
	//_______________________

	//110629.KKY_____________
	MProductPlanBatchData* p7;
	while(m_caNextProductPlan.GetSize() != 0)
	{
		p7 = (MProductPlanBatchData*)m_caNextProductPlan.GetAt(0);
		m_caNextProductPlan.RemoveAt(0);
		delete p7;
		p7 = NULL;
	}

	MProductPlanBatchData* p8;
	while(m_caEndProductPlan.GetSize() != 0)
	{
		p8 = (MProductPlanBatchData*)m_caEndProductPlan.GetAt(0);
		m_caEndProductPlan.RemoveAt(0);
		delete p8;
		p8 = NULL;
	}
	//_______________________
}

/*========================================================================================
	FUNCTION : MLCNet::Create()
	DESCRIPT : Create Instance
	RETURN	 : 0 Only
	ARGUMENT :
		pWnd	= window main handle
		szIP	= target pc ip address
		nPort	= tartget pc port
		bActive	= active or passive(default = passive)
		sSpool	= spool on/off(default = off)
	FIRST	 : 2004/07/20, JamesPark
	UPDATE	 : 2004/08/10, KJH
========================================================================================*/
long MLCNet::Create()
{
	CString	sLog;

	//Prev Dialog Delete
	if (m_pDlg)
	{
		m_pDlg->m_cXNet.Stop();
		m_pDlg->DestroyWindow();
		m_pDlg = NULL;
	}

	/** Dialog Create	*/
	m_pDlg = new MLCNetDlg;
	m_pDlg->m_pMLCNet = this;
	m_pDlg->Create(IDD_MLCNET);

	/** Dialog Visibled */
	if (m_bShow)
	{
		m_pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDlg->ShowWindow(SW_HIDE);
	}

	/** XNet Initial */
	m_pDlg->m_cXNet.SetIp((LPCTSTR)m_szIP);
	m_pDlg->m_cXNet.SetPort(m_nPort);
	m_pDlg->m_cXNet.SetActive(m_bActive);
	m_pDlg->m_cXNet.SetSpoolDir(_T("."));
	m_pDlg->m_cXNet.SetSpoolEnabled(m_bSpool);

	sLog.Format(_T("XNet create completed"));
	LogTrace(0, sLog);

	return ERR_MNET_SUCCESS;
}


/*========================================================================================
	FUNCTION : MLCNet::LogTrace()
	DESCRIPT : Trace log display
	RETURN	 : None
	ARGUMENT :
		nType	= Log type(level)
		szLog	= Log string
	FIRST	 : 2004/07/20, JamesPark
	UPDATE	 : 2004/08/10, KJH
========================================================================================*/
void MLCNet::LogTrace(int nType, CString szLog)
{
	if (m_pDlg)
	{
		m_pDlg->LogTrace(0, szLog);
	}
}

/*========================================================================================
	FUNCTION : MLCNet::Start()
	DESCRIPT : Start for xnet
	RETURN	 : long
	ARGUMENT :
		None
	FIRST	 : 2004/07/20, JamesPark
	UPDATE	 : 2004/08/10, KJH
========================================================================================*/
long MLCNet::Start()
{
	long	lRet;
	CString sLog;

	if (m_pDlg)
	{
		lRet = m_pDlg->m_cXNet.Start();
		if (!lRet)
		{
			m_pDlg->m_btnStart.EnableWindow(FALSE);
			m_pDlg->m_btnStop.EnableWindow(TRUE);
		}
	}
	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::Stop()
	DESCRIPT : Stop for xnet
	RETURN	 : long
	ARGUMENT :
		NONE
	FIRST	 : 2004/07/20, JamesPark
	UPDATE	 : 2004/08/10, KJH
========================================================================================*/
long MLCNet::Stop()
{
	long	lRet;
	CString sLog;

	if (m_pDlg)
	{
		lRet = m_pDlg->m_cXNet.Stop();
		if (!lRet)
		{
			m_pDlg->m_btnStart.EnableWindow(TRUE);
			m_pDlg->m_btnStop.EnableWindow(FALSE);
		}
	}

	sLog.Format(_T("XNet Stop=%d - %s"), lRet, (LPCTSTR)m_pDlg->m_cXNet.GetLastErrorString());
	LogTrace(0, sLog);

	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::ViewVisible()
	DESCRIPT : Visible On/Off for dialog
	RETURN	 : none
	ARGUMENT :
		NONE
	FIRST	 : 2004/07/20, JamesPark
	UPDATE	 : 2004/08/10, KJH
========================================================================================*/
void MLCNet::ViewVisible(BOOL bView)
{
	m_bShow = bView;
	if (bView) 
	{
		if (m_pDlg) m_pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		if (m_pDlg) m_pDlg->ShowWindow(SW_HIDE);
	}
}

/*========================================================================================
	FUNCTION : MLCNet::SetUnitNo()
	DESCRIPT : Unit identity number for equipment
	RETURN	 : none
	ARGUMENT :
		nUnit	= Unit identity number
	FIRST	 : 2004/07/20, JamesPark
	UPDATE	 : 2004/08/10, KJH
========================================================================================*/
void MLCNet::SetUnitNo(int nUnit)
{
	m_nUnit = nUnit;
}

/*========================================================================================
	FUNCTION : MLCNet::SetMaxPanel()
	DESCRIPT : Max working count for panel
	RETURN	 : none
	ARGUMENT :
		nMax	= Max panel count
	FIRST	 : 2004/07/20, JamesPark
	UPDATE	 : 2004/10/12, KJH
========================================================================================*/
void MLCNet::SetMaxPanel(int nMax)
{
	m_nMaxPanel = nMax;
}

/*========================================================================================
	FUNCTION : MLCNet::AlarmDataReport()
	DESCRIPT : Alarm data report
	RETURN	 : long
	ARGUMENT :
		nOnOff	= Alarm type(set=1,reset=0)
		nId		= Alarm Identity number
		nCode	= Alarm category
		szText	= Alarm description
	FIRST	 : 2004/07/20, JamesPark
	UPDATE	 : 2007/10/15, GrouchyKim
========================================================================================*/
long MLCNet::AlarmDataReport(int nOnOff, int nId, int nCode, CString szText)
{
	CString sMsg, sLog;
	long	lRet;
	
	/**	Data Assign */
	//sMsg.Format(_T("@UA##%d##%d##%d##%d##%s$"), m_nUnit, nOnOff, nId, nCode, szText);
	sMsg.Format(_T("@UA##%s##%d##%d##%d##%s$"), m_strReportID, nOnOff, nId, nCode, szText);

    //2007.10.15 GrouchyKim Add.[v1.3]
	m_nLastAlarmOnOff = nOnOff;
	m_nLastAlarmId = nId;
	m_nLastAlarmCode = nCode;
	m_strLastAlarmText = szText; 

	/** Data Send */
	lRet = SendString(sMsg);

	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::SendString()
	DESCRIPT : send string to target machine(xnet)
	RETURN	 : long
	ARGUMENT :
		szMsg	= Sending string message
	FIRST	 : 2004/07/20, JamesPark
	UPDATE	 : 2007/11/07, GrouchyKim
========================================================================================*/
long MLCNet::SendString(CString szMsg)
{
//	CString* psMsg = NULL;
	long	lRet = 0;
	
	if (m_pDlg != NULL)
	{
	  //2007.11.07 GrouchyKim Modify.[v1.4]
		//m_pDlg->m_srgSendMsg.Add(szMsg);
		m_pDlg->AddSendMsg(szMsg);
		
		m_pDlg->PostMessage(WMU_RCV_SEND_MSG, 0, 0);
	}
//	psMsg = new CString;
//	*psMsg = szMsg;
//	if (m_pDlg != NULL)
//		m_pDlg->PostMessage(WMU_RCV_SEND_MSG, (long)psMsg);
//	else
//		lRet = 1;
	
	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::EqStateReport()
	DESCRIPT : Current equipment state report
	RETURN	 : long
	ARGUMENT :
		eOpMode	= Current operation mode(auto/manual)
		eEqState= Current equipment state(1:run,...,10:wait)
	FIRST	 : 2004/07/20, JamesPark
	UPDATE	 : 2007/10/15, GrouchyKim
				2011.06.27 KKY ---> AlarmID, AlarmText추가
========================================================================================*/
//long MLCNet::EqStateReport(EEqState eEqState, CString sPMCode)
long MLCNet::EqStateReport(EEqState eEqState, CString sPMCode, int nAlarmID, CString szAlarmText)
{
	CString sMsg;
	long	lRet;

	/**Data Assign */
	m_eEqState = eEqState;
	//2007.10.15 GrouchyKim Modify.[v1.3]
	//110627.KKY________________________________
//	sMsg.Format(_T("@US##%d##%d##%s$"), m_nUnit, m_eEqState, GetPMCode());
	if (nAlarmID == -1)	//Alarm이 발생하지 않았을 경우
//		sMsg.Format(_T("@US##%d##%d##%s####$"), m_nUnit, m_eEqState, GetPMCode());
		sMsg.Format(_T("@US##%s##%d##%s####$"), m_strReportID, m_eEqState, GetPMCode());
	else
//		sMsg.Format(_T("@US##%d##%d##%s##%d##%s$"), m_nUnit, m_eEqState, GetPMCode(), nAlarmID, szAlarmText);
		sMsg.Format(_T("@US##%s##%d##%s##%d##%s$"), m_strReportID, m_eEqState, GetPMCode(), nAlarmID, szAlarmText);
	//__________________________________________
	/** Data Send */
	lRet = SendString(sMsg);

	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::MaterialReport()
	DESCRIPT : Material Event report
	RETURN	 : long
	ARGUMENT :
		eEventType	= Material Event Type
		pData		= Material Event Data
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2007/10/15, GrouchyKim
========================================================================================*/
long MLCNet::MaterialReport(EMatEvent eEventType, MMaterial* pData)
{
	CString sMsg, sTemp;
	long	lRet;
	
	/** Data Assign */
//	sMsg.Format(_T("@UM##%d##%d"), m_nUnit, eEventType);
	sMsg.Format(_T("@UM##%s##%d"), m_strReportID, eEventType);	
	sTemp.Format(_T("##%s##%s##%s##%s##%s")
		_T("##%d##%d##%s##%s##%d")
		_T("##%d##%d##%d##%d##%d")
		_T("##%d##%d##%d##%s##%s")
		_T("##%d##%s"),
		pData->m_sPORTID,
		pData->m_sEQP_STATE,
		pData->m_sPORT_STATE,
		pData->m_sPORT_TYPE,
		pData->m_sCSTID,
		pData->m_nMAP_STIF,
		pData->m_nCUR_STIF,
		pData->m_sM_BATCHID,
		pData->m_sM_MAKER,
		pData->m_nM_KIND,
		pData->m_nM_TYPE,
		pData->m_nM_STATE,
		pData->m_nT_QTY,
		pData->m_nU_QTY,
		pData->m_nR_QTY,
		pData->m_nN_QTY,
		pData->m_nA_QTY,
		pData->m_nE_FLAG,
		pData->m_sC_CODE,
		pData->m_sM_STEP,
		pData->m_nS_QTY,
		pData->m_sM_CODE);
	sMsg += sTemp;
	sMsg += "$";
	
	/** Data Send */
	lRet = SendString(sMsg);

	//110705.kms Modify____________
	if (m_plnkSystemData->m_strModulePortID[0] == pData->m_sPORTID)
	{
		m_oMat_Feeder1.CopyData(pData);
	}
	else if (m_plnkSystemData->m_strModulePortID[1] == pData->m_sPORTID)
	{
		m_oMat_Feeder2.CopyData(pData);
	}
    //2007.10.15 GrouchyKim Add.[v1.3]
	else if (m_plnkSystemData->m_strModulePortID[2] == pData->m_sPORTID)
	{
		m_oMat_Feeder3.CopyData(pData);
	}
	else if (m_plnkSystemData->m_strModulePortID[3] == pData->m_sPORTID)
	{
		m_oMat_Feeder4.CopyData(pData);
	}	
	//20121105 SJ_HJG 추가
	else if (m_plnkSystemData->m_strModulePortID[5] == pData->m_sPORTID)
	{
		m_oMat_ZigFeeder1.CopyData(pData);
	}
	else if (m_plnkSystemData->m_strModulePortID[6] == pData->m_sPORTID)
	{
		m_oMat_ZigFeeder2.CopyData(pData);
	}	
	//_____________________

	/*
	if ("FEEDER1" == pData->m_sPORTID)
	{
		m_oMat_Feeder1.CopyData(pData);
	}
	else if ("FEEDER2" == pData->m_sPORTID)
	{
		m_oMat_Feeder2.CopyData(pData);
	}
    //2007.10.15 GrouchyKim Add.[v1.3]
	else if ("FEEDER3" == pData->m_sPORTID)
	{
		m_oMat_Feeder3.CopyData(pData);
	}
	else if ("FEEDER4" == pData->m_sPORTID)
	{
		m_oMat_Feeder4.CopyData(pData);
	}*/
	
	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::PanelIdTransfer()
	DESCRIPT : Panel data shift in current equipment
	RETURN	 : none
	ARGUMENT :
	nPrevPos 	= Previous unit
	nCurPos		= Current unit(or next unit)
	FIRST	 : 2004/07/20, JamesPark; First work!
	LAST	 : 2004/08/20
========================================================================================*/
long MLCNet::PanelIdTransfer(int nPrevPos, int nCurPos, CString szPanelID)
{
	CString sMsg, sTemp;
	long	lRet=0;

//	if (nCurPos < m_nMaxPanel) 
	{
		m_rgPanel[nCurPos] = m_rgPanel[nPrevPos];
		m_rgPanel[nPrevPos].ResetData();

		/** Data Assign */
//		sMsg.Format(_T("@UI##%d##%d##%d##%s$"), m_nUnit, nPrevPos, nCurPos, szPanelID);
		sMsg.Format(_T("@UI##%s##%d##%d##%s$"), m_strReportID, nPrevPos, nCurPos, szPanelID);
		/** Data Send */
		lRet = SendString(sMsg);
	}

	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::PanelTransferReport()
	DESCRIPT : Panel transfer data report
	RETURN	 : long
	ARGUMENT :
		eEventType	= Event type(In,Out,Remove,VcrRead,BcrRead)
		szPanelId	= Current Panel id
	FIRST	 : 2004/07/20, JamesPark
	UPDATE	 : 2004/08/10, KJH
========================================================================================*/
long MLCNet::PanelTransferReport(EPanelEvent eEventType, CString szPanelId)
{
	CString sMsg, sLog;
	long	lRet;
	
	/** Data Assign */
//	sMsg.Format(_T("@UC##%d##%d##%s##%d$"), m_nUnit, eEventType, szPanelId, m_nMaxPanel);
	sMsg.Format(_T("@UC##%s##%d##%s##%d$"), m_strReportID, eEventType, szPanelId, m_nMaxPanel);

	/** Data Send */
	lRet = SendString(sMsg);

	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::PanelTransferReport()
	DESCRIPT : Panel transfer data report
	RETURN	 : long
	ARGUMENT :
		eEventType	= Event type(In,Out)
		pData		= Current Panel Data Object
	FIRST	 : 2004/09/20, KJH
	UPDATE	 : 2004/09/20, KJH
========================================================================================*/
long MLCNet::PanelTransferReport(EPanelEvent eEventType, MPanel *p)
{
	CString sMsg, sLog, sTemp;
	long	lRet;
	
	//Data Assign
//	sMsg.Format(_T("@UC##%d##%d"), m_nUnit, eEventType);
	sMsg.Format(_T("@UC##%s##%d"), m_strReportID, eEventType);
	sTemp.Format(_T("##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s")
				  _T("##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s")
				  _T("##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s")
				  _T("##%s"),
	       	p->m_strHPanelID,		// Ascil  12byte					
	       	p->m_strEPanelID,		// Ascil  12byte
	       	p->m_strBatchID,		// Ascil  12byte				
	       	p->m_strProdType,		// Ascil  2byte
	       	p->m_strProdKind,		// Ascil  2byte
	       	p->m_strDeviceID,		// Ascil  18byte
	       	p->m_strStepID,			// Ascil  10byte
	       	p->m_strPPID,			// Ascil  16byte
	       	p->m_strThickness,		// Binary 2byte
	       	p->m_strInsFlag,		// Ascil  2byte
	       	
	       	p->m_strPanelSize,		// Binary 4byte
	       	p->m_strPanelPosition,	// Ascil  2byte
	       	p->m_strCount1,			// Ascil  2byte
	       	p->m_strCount2,			// Ascil  2byte
	       	p->m_strGrade,			// Bits	  6byte
	       	p->m_strComment,		// Ascil  16byte
	       	p->m_strCompSize,		// Binary 2byte
	       	p->m_strReadingFlag,	// Ascil  2byte
	       	p->m_strPanelState,		// Binary 2byte
	       	p->m_strJudgement,		// Ascil  4byte
	       	
	       	p->m_strCode,			// Ascil  4byte
	       	p->m_strRunLine,		// Binary 20byte
	       	p->m_strUniqueId,		// Binary 4byte
	       	p->m_strPairHPanelID,	// Ascil  12byte
	       	p->m_strPairEPanelID,	// Ascil  12byte
	       	p->m_strPairGrade,		// Bits   6byte
	       	p->m_strPairUniqueId,	// Binary 4byte
	       	p->m_strFlowRecipe,		// Ascil  2byte
	       	p->m_strReserved0,		// Any    12byte
	       	p->m_strBitsSignals,	// Bits   2byte
	       	
	       	p->m_strReferData		// Any    4byte
	       );	       			
	sMsg += sTemp;
	sMsg += "$";

	//Data Send
	lRet = SendString(sMsg);

	if (ePanelIn == eEventType)
	{
		m_rgPanel[0].CopyData(p);
	}
	else if ((ePanelOut == eEventType) || (ePanelRemove == eEventType))
	{
		for (int i=0; i<10; i++)
		{
			if (p->m_strHPanelID == m_rgPanel[i].m_strHPanelID)
			{
				m_rgPanel[i].ResetData();
				break;
			}
		}
	}// else if (ePanelOut==eEventType){

	return lRet;
}


/*========================================================================================
	FUNCTION : MLCNet::InspectionReport()
	DESCRIPT : Inspection result data report
	RETURN	 : long
	ARGUMENT :
		pData		= Current Panel Data Object
		szRawPath	= Raw file save path
		szSumPath	= Sum file save path
		szImagePath	= Image file save path
		szDisk		= File save location(FILESERVER, LOCALDISK)
	FIRST	 : 2004/10/07, PJI
========================================================================================*/
long MLCNet::InspectionReport(MPanel* pData, 
							  CString szRawPath, 
							  CString szSumPath,
							  CString szImagePath,
							  CString szDisk)
{
	CString sMsg, sLog, sTemp;
	long	lRet;
	
	//Data Assign
//	sMsg.Format(_T("@IR##%d"), m_nUnit);
	sMsg.Format(_T("@IR##%s"), m_strReportID);
/*
	sTemp.Format("##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s"
				  "##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s"
				  "##%s##%s##%s##%s##%s##%s##%s##%s##%s##%s"
				  "##%s##%s##%s",
						pData->m_strHPanelID,				
						pData->m_strEPanelID,					
						pData->m_strBatchID,
						pData->m_strProdType,					
						pData->m_strProdKind,				
						pData->m_strDeviceID,					
						pData->m_strStepID,		
						pData->m_strPPID,							
						pData->m_strThickness,			
						pData->m_strInsFlag,
											
						pData->m_strPanelSize,					
						pData->m_strPanelPosition,				
						pData->m_strCount1,			
						pData->m_strCount2,						
						pData->m_strGrade,						
						pData->m_strComment,		
						pData->m_strCompSize,						
						pData->m_strReadingFlag,					
						pData->m_strPanelState,				
						pData->m_strJudgement,				
						
						pData->m_strCode,							
						pData->m_strRunLine,				
						pData->m_strUniqueId,				
						pData->m_strPairHPanelID,		
						pData->m_strPairEPanelID,					
						pData->m_strPairGrade,				
						pData->m_strPairUniqueId,				
						pData->m_strFlowRecipe,	
						pData->m_strReserved0,		
						pData->m_strBitsSignals,	
						
						pData->m_strReferData,
//						szRawPa,   // T7-2 얘는 뭘까? AST_KSM
						szRawPath,
						szSumPath,
						szImagePath,
						szDisk);	 
*/
	sMsg += sTemp;
	sMsg += "$";

	//Data Send
	lRet = SendString(sMsg);

	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::ProcessProgramReport()
	DESCRIPT : ProcessProgram(PPID) Event report
	RETURN	 : long
	ARGUMENT :
		nMode		= Add/Delete/Modify Type
		pData		= Material Event Data
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2004/08/10, KJH
========================================================================================*/
long MLCNet::ProcessProgramReport(int nMode, CString szPPID, MProcessProgram* pPP)
{
	CString sMsg, sTemp;
	long	lRet;


	/** Data Assign */
//	sMsg.Format(_T("@PP##%d##%d##%s##%d"), m_nUnit, nMode, szPPID, pPP->m_nCCodeQty);
	sMsg.Format(_T("@PP##%s##%d##%s##%d"), m_strReportID, nMode, szPPID, pPP->m_nCCodeQty);
	for (int i = 0; i < pPP->m_nCCodeQty; i++)
	{
		sTemp.Format(_T("##%d\\%d"),
					pPP->m_paCCode[i].m_nCCode, 
					pPP->m_paCCode[i].m_nPPBodyQty);  
		sMsg += sTemp;
		for (int j = 0; j < pPP->m_paCCode[i].m_nPPBodyQty; j++)
		{
			sTemp.Format(_T("\\%s&&%s"),
				pPP->m_paCCode[i].m_paPPBoby[j].m_strPParamName,
				pPP->m_paCCode[i].m_paPPBoby[j].m_strPParamValue); 
			sMsg += sTemp;
		}
	}
	sMsg += "$";

	/** Data Send */
	lRet = SendString(sMsg);

	return lRet;
}

//100807.ECID_____________________________________________________________________________
//
/*========================================================================================
	FUNCTION : MLCNet::ParamECChangeReport()
	DESCRIPT : Parameter Event report
	RETURN	 : long
	ARGUMENT :
		pData		= Event Data
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2004/08/20, KJH
========================================================================================*/
long MLCNet::ParamECChangeReport(EEqConstCmd eEqConstCmd, MEqConstant* pData, int nCount)
{
	CString sMsg, sTemp;
	long	lRet;

	/** Data Assign */
//	sMsg.Format(_T("@EC##%d##%d##%d"), m_nUnit, eEqConstCmd, nCount);   //nCount 가 보고 횟수 설정
	sMsg.Format(_T("@EC##%s##%d##%d"), m_strReportID, eEqConstCmd, nCount);   //nCount 가 보고 횟수 설정

	sTemp.Format(
		_T("##%s##%s##%s##%s##%s##%s##%s$"),
		pData->m_strECID,
		pData->m_strECName,
		pData->m_strECDef,
		pData->m_strECSLL,
		pData->m_strECSUL,
		pData->m_strECWLL,
		pData->m_strECWUL);
	sMsg += sTemp;

	/** Data Send */
	lRet = SendString(sMsg);

	return lRet;
}

// 100807. DEL
//long MLCNet::ParamECChangeReport(EEditType eEdit, MEqConstant* pData)
//{
//	CString sMsg, sTemp;
//	long	lRet;
//
//	/** Data Assign */
//	sMsg.Format(_T("@UD##%d##%d##%d"), m_nUnit, eEqConstant, eEdit);
//	sTemp.Format(
//		_T("##%d##%s##%s##%s##%s##%s##%s$"),
//		pData->m_nECID,
//		pData->m_strECName,
//		pData->m_strECDef,
//		pData->m_strECSLL,
//		pData->m_strECSUL,
//		pData->m_strECWLL,
//		pData->m_strECWUL); 
//	sMsg += sTemp;
//
//	/** Data Send */
//	lRet = SendString(sMsg);
//
//	return lRet;
//}

/*========================================================================================
	FUNCTION : MLCNet::ParamECReportAll()
	DESCRIPT : All EC Parameter Event report
	RETURN	 : long
	ARGUMENT :
		pData		= Event Data
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2004/08/20, KJH
========================================================================================*/
long MLCNet::ParamECReportAll(EEqConstCmd eEqConstCmd, int nSize)
{
	CString		sMsg, sTemp;
	long		lRet;
	int			nCount, nDataCount, nLastCount;
	POSITION	Pos;
	MEqConstant	*pECData = NULL; 
	pECData = new MEqConstant;

	nCount = nDataCount = 0;

	int nValue = nSize;	

	// 총 데이타 개수
	for (;;)
	{
		if (nValue > 0)
			nCount++;
		else
			break;

		nValue -= 10;
	}

	nLastCount = nSize;
	nLastCount %= 10;

	if(eInitRequest == eEqConstCmd)
	{
		Pos = m_pECList->GetHeadPosition();

		for (; nCount > 0; nCount--)
		{
//			nCount == 0 ? nDataCount = nLastCount : nDataCount = 10;

			if(nCount == 1)
			{
				nDataCount = nLastCount;
			}
			else
			{
				nDataCount = 10;
			}

			/** Data Assign */
//			sMsg.Format(_T("@EC##%d##%d##%d"), m_nUnit, eEqConstCmd, nDataCount);
			sMsg.Format(_T("@EC##%s##%d##%d"), m_strReportID, eEqConstCmd, nDataCount);

			for(; nDataCount > 0; nDataCount--)
			{
				if (Pos == NULL) {
					//nDataCount++;
					continue;
				}

				pECData = (MEqConstant *)m_pECList->GetAt(Pos);	

				//if (pECData->m_strECID != "") {
					sTemp.Format(
						_T("##%s##%s##%s##%s##%s##%s##%s"),
						pECData->m_strECID,
						pECData->m_strECName,
						pECData->m_strECDef,
						pECData->m_strECSLL,
						pECData->m_strECSUL,
						pECData->m_strECWLL,
						pECData->m_strECWUL);
					sMsg += sTemp;
				//}

				m_pECList->GetNext(Pos);
			}

			// Add End String
			sMsg += "$";

			/** Data Send */
			lRet = SendString(sMsg);

			pECData->ResetData();
		}
	}

	m_pECList->RemoveAll();

	delete pECData;
	return lRet;
}
//________________________________________________________________________________________

/*========================================================================================
	FUNCTION : MLCNet::ParamEOChangeReport()
	DESCRIPT : Parameter Event report
	RETURN	 : long
	ARGUMENT :
		pData		= Event Data
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2004/08/20, KJH
========================================================================================*/
long MLCNet::ParamEOChangeReport(EEditType eEdit, MEqOnlineParam* pData)
{
	CString sMsg, sTemp;
	long	lRet;


	/** Data Assign */
//	sMsg.Format(_T("@UD##%d##%d##%d"), m_nUnit, eEqOnline, eEdit);
	sMsg.Format(_T("@UD##%s##%d##%d"), m_strReportID, eEqOnline, eEdit);
	sTemp.Format(
		_T("##%d##%d##%d$"),
		pData->m_nEOID,
		pData->m_nEOMD,
		pData->m_nEOV); 
	sMsg += sTemp;

	/** Data Send */
	lRet = SendString(sMsg);

	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::ParamSVChangeReport()
	DESCRIPT : Parameter Event report
	RETURN	 : long
	ARGUMENT :
		pData		= Event Data
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2004/08/20, KJH
========================================================================================*/
long MLCNet::ParamSVChangeReport(EEditType eEdit, MStateVariable* pData)
{
	CString sMsg, sTemp;
	long	lRet;


	/** Data Assign */
//	sMsg.Format(_T("@UD##%d##%d##%d"), m_nUnit, eEqStatus, eEdit);
	sMsg.Format(_T("@UD##%s##%d##%d"), m_strReportID, eEqStatus, eEdit);
	sTemp.Format(
		_T("##%d##%s##%s$"),
		pData->m_nSVID,
		pData->m_strSV,
		pData->m_strSVName); 
	sMsg += sTemp;

	/** Data Send */
	lRet = SendString(sMsg);

	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::SetParamEC()
	DESCRIPT : Set Equipment Parameter  = Init EC Report
	RETURN	 : long
	ARGUMENT :
		pData		= Event Data
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2004/08/20, KJH
========================================================================================*/
void MLCNet::SetParamEC(MEqConstant* pData)
{
	CString sMsg, sTemp;
	long	lRet = 0;
	MEqConstant*		pEC = NULL;

	/** Data Assign */
	pEC = new MEqConstant;
	pEC->CopyData(pData); 
	m_pECList->AddTail(pEC);

	/** Data Assign */
//100807. ECID_______________________
// DEL
//	sMsg.Format(_T("@ID##%d##%d##%d"), m_nUnit, eEqConstant);
//	sTemp.Format(
//		_T("##%d##%s##%s##%s##%s##%s##%s$"),
//		pData->m_nECID,
//		pData->m_strECName,
//		pData->m_strECDef,
//		pData->m_strECSLL,
//		pData->m_strECSUL,
//		pData->m_strECWLL,
//		pData->m_strECWUL); 
//	sMsg += sTemp;
//
//	/** Data Send */
//	lRet = SendString(sMsg);
//___________________________________
}

/*========================================================================================
	FUNCTION : MLCNet::SetParamSV()
	DESCRIPT : Set Equipment Parameter = 
	RETURN	 : long
	ARGUMENT :
		pData		= Event Data
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2004/08/20, KJH
========================================================================================*/
void MLCNet::SetParamSV(MStateVariable* pData)
{
	CString sMsg, sTemp;
	long	lRet;
	MStateVariable* pSV = NULL;


	/** Data Assign */
	pSV = new MStateVariable;
	pSV->CopyData(pData); 
	m_pSVList->AddTail(pSV);

	/** Data Assign */
//	sMsg.Format(_T("@ID##%d##%d##%d"), m_nUnit, eEqStatus);
	sMsg.Format(_T("@ID##%s##%d##%d"), m_strReportID, eEqStatus);
	sTemp.Format(
		_T("##%d##%s##%s$"),
		pData->m_nSVID,
		pData->m_strSV,
		pData->m_strSVName); 
	sMsg += sTemp;

	/** Data Send */
	lRet = SendString(sMsg);
}

/*========================================================================================
	FUNCTION : MLCNet::SetParamEO()
	DESCRIPT : Set Equipment Parameter
	RETURN	 : long
	ARGUMENT :
		pData		= Event Data
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2004/08/20, KJH
========================================================================================*/
void MLCNet::SetParamEO(MEqOnlineParam* pData)
{
	CString sMsg, sTemp;
	long	lRet;
	MEqOnlineParam* pEO = NULL;

	/** Data Assign */
	pEO = new MEqOnlineParam;
	pEO->CopyData(pData); 
	m_pEOList->AddTail(pEO);

	/** Data Assign */
//	sMsg.Format(_T("@ID##%d##%d##%d"), m_nUnit, eEqOnline);
	sMsg.Format(_T("@ID##%s##%d##%d"), m_strReportID, eEqOnline);
	sTemp.Format(
		_T("##%d##%d##%d$"),
		pData->m_nEOID,
		pData->m_nEOMD,
		pData->m_nEOV); 
	sMsg += sTemp;

	/** Data Send */
	lRet = SendString(sMsg);
}

/*========================================================================================
	FUNCTION : MLCNet::SetParamVariable()
	DESCRIPT : Set Equipment Parameter = Report 
	RETURN	 : long
	ARGUMENT :
		eType		= EC/EO/SV
		nIndex		= List Index
		pData		= Event Data
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2004/08/10, KJH
========================================================================================*/
void MLCNet::SetProcessProgram(CString szPPID, MProcessProgram* pPP)
{
	MProcessProgram* pData = NULL;

	pData = new MProcessProgram;
	pData->CopyData(pPP); 

	m_pPPList->AddTail(pData);
}

/*========================================================================================
	FUNCTION : MLCNet::RequestEquipmentCmd()
	DESCRIPT : Request equipment command
	RETURN	 : none
	ARGUMENT :
		eCmdId	= Equipment command id
		szText	= Display message
	FIRST	 : 2004/07/20, JamesPark; First work!
	LAST	 : 2004/08/10, KJH
	LAST	 : 2005/06.28, KJS
========================================================================================*/
void MLCNet::RequestEquipmentCmd(int nCmdId, CString szText)
{
	CString*		psMsg = NULL;

	if(szText == "")
		return;

	if (nCmdId==81 || nCmdId==82 || nCmdId == 53)
	{
		m_iaEquipmentCmd.Add(nCmdId);
		m_saEquipmentCmd.Add(szText);
	}
	else
	{
		m_iaEquipmentCmd.Add(nCmdId);
		m_saEquipmentCmd.Add(szText);
	}
}

/*========================================================================================
	FUNCTION : MLCNet::RequestProcessCmd()
	DESCRIPT : Request process command
	RETURN	 : none
	ARGUMENT :
		eCmdId		= Process command id
		szPanelId	= Current panel id
	UPDATE	 : 2004/07/20, JamesPark; First work!
========================================================================================*/
void MLCNet::RequestProcessCmd(int nCmdId, CString szPanelId)
{
	m_iaProcessCmd.Add(nCmdId);
	m_saProcessCmd.Add(szPanelId);
}

/*========================================================================================
	FUNCTION : MLCNet::RequestMaterialCmd()
	DESCRIPT : Request Material command
	RETURN	 : none
	ARGUMENT :
		eCmdId	= Material command id
		szText	= Display message
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2004/08/10, KJH
========================================================================================*/
void MLCNet::RequestMaterialCmd(int nCmdId, CString szBatchID)
{
	m_iaMaterialCmd.Add(nCmdId);
	m_saMaterialCmd.Add(szBatchID);
}

/*========================================================================================
	FUNCTION : MLCNet::RequestOnlineMode()
	DESCRIPT : Request(update) online mode command
	RETURN	 : none
	ARGUMENT :
		szOnlineMode	= Online mode(offline, local, remote)
	UPDATE	 : 2004/07/20, JamesPark; First work!
========================================================================================*/
void MLCNet::RequestOnlineMode(CString szOnlineMode)
{
	if (szOnlineMode == "REMOTE")
		m_iaOnlineCmdMode.Add((UINT)eRemote);
	else if (szOnlineMode == "LOCAL") 
		m_iaOnlineCmdMode.Add((UINT)eLocal);
	else
		m_iaOnlineCmdMode.Add((UINT)eOffline);
}

/*========================================================================================
	FUNCTION : MLCNet::RequestParamChange()
	DESCRIPT : Request Parameter change
	RETURN	 : none
	ARGUMENT :
		szOnlineMode	= Online mode(offline, local, remote)
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2004/08/10, KJH
========================================================================================*/
void MLCNet::RequestParamChange(EParamType eType, LPVOID lpvData)
{
	m_iaChangeParamType.Add((UINT)eType);
	m_caChangeParamStateVariable.Add(lpvData);
}

/*========================================================================================
	FUNCTION : MLCNet::RequestProcessProgram()
	DESCRIPT : Request ProcessProgram change
	RETURN	 : none
	ARGUMENT :
		nMode	= Create/Delete/Modify/Change
		pPP		= ProcessProgram Data
	FIRST	 : 2004/08/10, KJH
	LAST	 : 2004/08/10, KJH
========================================================================================*/
void MLCNet::RequestProcessProgram(int nMode, MProcessProgram* pPP)
{
	m_iaProcessProgramMode.Add(nMode);
	m_caProcessProgram.Add((void*)pPP);
}

/*========================================================================================
	FUNCTION : MLCNet::RequestProcessProgram()
	DESCRIPT : Request ProcessProgram change
	RETURN	 : none
	ARGUMENT :
		nMode	= Create/Delete/Modify/Change
		pPP		= ProcessProgram Data
	UPDATE	 : 2005/01/21, KJH
========================================================================================*/
void MLCNet::RequestProductPlan(int nPlanCode, MProductPlanBatchData* pBI)
{
	/*110620.KKY.Del_________
	if (eCreate==(EEditType)nPlanCode)
	{
		m_CurProductPlan.SetData(pBI);
	}  
	else if (eModify==(EEditType)nPlanCode)
	{
		m_CurProductPlan.SetData(pBI);
	}  
	else if (eChange==(EEditType)nPlanCode)
	{
		m_CurProductPlan.SetData(pBI);
	}  
	else if (eDelete==(EEditType)nPlanCode)
	{
		m_CurProductPlan.Clear();
	}  
	_________________________*/

	m_iaProductPlanCode.Add(nPlanCode);
	m_caProductPlanBatchData.Add((void*)pBI);
	m_iPlanQty =  pBI->m_nPlanQty;
}

/*========================================================================================
	FUNCTION : MLCNet::RequestBarCodeParsingInfo()
	DESCRIPT : TABIC Barcode Parsing Information
	RETURN	 : none
	ARGUMENT :
		p	= Parsing Information Data
	FIRST	 : 2004/10/13, KJH
	LAST	 : 2004/10/13, KJH
========================================================================================*/
void MLCNet::RequestBarCodeParsingInfo(MBarCodeParsingInfo* p)
{
	m_caBarCodeParsingInfo.Add((void*)p);
}

//110621.KKY.Add___________________________________________
void MLCNet::RequestMaterialCodeInfo(MMaterialCodeInfo* p)
{
	m_caMaterialCodeInfo.Add((void*)p);
}
//_________________________________________________________

/*========================================================================================
	FUNCTION : MLCNet::RequestMultiTerminalCmd()
	DESCRIPT : Request Multi Terminal command
	RETURN	 : none
	ARGUMENT :
		szText	= Display message
	FIRST	 : 2017/09/12, KDW
	LAST	 : 
	LAST	 : 
========================================================================================*/
void MLCNet::RequestMultiTerminalCmd(CString szText)
{
	if(szText == "")
		return;
	m_saMultiTerminalCmd.Add(szText);
}
/*========================================================================================
	FUNCTION : MLCNet::RequestEqConstChange()
	DESCRIPT : Equipment Constant Change
	RETURN	 : none
	ARGUMENT :
		nCmdID = InitRequest/ChangeRequest
		pEC	= Equipment Constant
	FIRST	 : 2010/08/07
	LAST	 : 2010/08/07
========================================================================================*/
//100807.ECID_______________________
void MLCNet::RequestEqConstChange(int nCmdID, MEqConstant* pEC)
{
	m_iaEqConstCmd.Add(nCmdID);
	if (pEC != NULL) {
		m_caEqConstData.Add((void*)pEC);
	}
}
//__________________________________

/*========================================================================================
	FUNCTION : MLCNet::ProcessParamReport()
	DESCRIPT : Current Panel's Process Parameter Report
	RETURN	 : long
	ARGUMENT :
		pData	= Current Process Parameter
		szPanelId = Current Panel ID
	FIRST	 : 2009/10/15, SJ_Chi
// 091028 SJ_Chi, Data Collection Report 추가
========================================================================================*/
long MLCNet::ProcessParamReport(MProcessParamInfo* pData, CString szPanelId)
{
	// Data 개수를 30개로 제한
	if (pData->m_iDataCount > 1000)	return -1;

	CString sMsg, sTemp, sParamName;
	long	lRet;

	/** Data Assign */
	// Key Parameter Report : 2
	//sMsg.Format(_T("@DC##%d##%d##%s##%d##"), m_nUnit, 2, szPanelId, pData->m_iDataCount); 
	sMsg.Format(_T("@DC##%s##%d##%s##%d##"), m_strReportID, 2, szPanelId, pData->m_iDataCount); 
	// Parameter Data Assign
	for (int i = 0; i < pData->m_iDataCount; i++)
	{
		//120508.kms______

		if("Ionizer" == pData->m_rgsProcessParam[i].m_strParamName)
		{	
			sTemp.Format(_T("%s/%0.0f/%d##"), pData->m_rgsProcessParam[i].m_strParamName, pData->m_rgsProcessParam[i].m_dSettingValue, pData->m_rgsProcessParam[i].m_iIonizerState);
			sMsg += sTemp;
		}
		else
		{
			sTemp.Format(_T("%s/%0.3f/%0.3f##"), pData->m_rgsProcessParam[i].m_strParamName, pData->m_rgsProcessParam[i].m_dSettingValue, pData->m_rgsProcessParam[i].m_dMeasureValue);
			sMsg += sTemp;
		}
		//________________
	}
	sMsg += "$";

	/** Data Send */
	lRet = SendString(sMsg);

	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::GetSubItem()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/17, KJH
	LAST	 : 2004/08/18, KJH
========================================================================================*/
bool MLCNet::GetSubItem(CString sMsg, CStringArray &asList, int nType)
{
	int		nItem, nPrev;	//Msg Index
	int		nItemCnt;		//Body Count
	bool	bRtn = false;


	if (1 == nType){
		nPrev = nItemCnt = 0;
		for (nItem = sMsg.Find(_T("\\"),1); nPrev != 1; nItem = sMsg.Find(_T("\\"),nItem + 1))
		{
			if (nItem == -1)
			{
				asList.Add(sMsg.Mid(nPrev,sMsg.GetLength()-nPrev));
				break;
			}
			else
			{
				asList.Add(sMsg.Mid(nPrev,nItem-nPrev));
			}
			nPrev = nItem + 1;
		}
	}
	else
	{
		nPrev = nItemCnt = 0;
		for (nItem = sMsg.Find(_T("&&"),1); nPrev != 1; nItem = sMsg.Find(_T("&&"),nItem+2))
		{
			if (nItem == -1) 
			{
				asList.Add(sMsg.Mid(nPrev,sMsg.GetLength()-nPrev));
				break;
			}
			else 
			{
				asList.Add(sMsg.Mid(nPrev,nItem-nPrev));
			}
			nPrev = nItem + 2;
		}
	}

	if (0 < asList.GetSize())
	{ 
		bRtn = true; 
	}

	return bRtn;
}

/*========================================================================================
	FUNCTION : MLCNet::EqProcessStateReport()
	DESCRIPT : Current equipment Process state report
	RETURN	 : long
	ARGUMENT :
		eEqProcessState= Current equipment state(1:Init,...,6:Pause)
	FIRST	 : 2004/10/01, JungjinPark
	UPDATE	 : 2011.06.27 KKY ---> AlarmID, AlarmText추가
========================================================================================*/
//long MLCNet::EqProcessStateReport(EEqProcState eEqProcState)
long MLCNet::EqProcessStateReport(EEqProcState eEqProcState, int nAlarmID, CString szAlarmText)
{
	CString sMsg;
	long	lRet;

	/** Data Assign */
	m_eEqProcState = eEqProcState;
	//110627.KKY________________________
//	sMsg.Format(_T("@UP##%d##%d$"), m_nUnit, m_eEqProcState);
	if (nAlarmID == -1)	//Alarm이 발생하지 않았을 경우
//		sMsg.Format(_T("@UP##%d##%d####$"), m_nUnit, m_eEqProcState);
		sMsg.Format(_T("@UP##%s##%d####$"), m_strReportID, m_eEqProcState);
	else
//		sMsg.Format(_T("@UP##%d##%d######%d##%s$"), m_nUnit, m_eEqProcState, nAlarmID, szAlarmText);
		sMsg.Format(_T("@UP##%s##%d######%d##%s$"), m_strReportID, m_eEqProcState, nAlarmID, szAlarmText);
	//__________________________________
	/** Data Send */
	lRet = SendString(sMsg);

	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::EqProcessStopReport()
	DESCRIPT : Current equipment Process state report
	RETURN	 : long
	ARGUMENT :
		eEqProcessState= Current equipment state(1:Init,...,6:Pause)
		m_nCode : Stop Code
		m_strItem : Stop Item
	FIRST	 : 2010/01/20, SJ_YYK
	//100121 SJ_YYK EQ_STOP_DATA
========================================================================================*/
long MLCNet::EqProcessStopReport(EEqProcState eEqProcState, int m_nCode, CString m_strItem)
{
	CString sMsg;
	long	lRet;
	int m_nSelectCode;
	CString sItem;
	m_nSelectCode = m_nCode;
	sItem = m_strItem;

	//SJ_YYK 160512 Add.. 설비 정지시 PM 상태보고..
	m_eEqState = ePM;
	sMsg.Format(_T("@US##%s##%d##%d##%s$"), m_strReportID, m_eEqState, m_nSelectCode, sItem);
	lRet = SendString(sMsg);
	//__________________________________*/

	/** Data Assign */
	m_eEqProcState = eEqProcState;
//	sMsg.Format(_T("@UP##%d##%d##%d##%s$"), m_nUnit, m_eEqProcState,m_nSelectCode,sItem);
	sMsg.Format(_T("@UP##%s##%d##%d##%s$"), m_strReportID, m_eEqProcState,m_nSelectCode,sItem);

	/** Data Send */
	lRet = SendString(sMsg);

	return lRet;
}

/*========================================================================================
	FUNCTION : MLCNet::MaterialPortStateReport()
	DESCRIPT : Current Material Port State Report
	RETURN	 : long
	ARGUMENT :
		eOpMode	= Current operation mode(auto/manual)
		eMaterialPortState = Current Equipment Materila Port state(1:run,...,10:wait)
	FIRST	 : 2004/10/01, JungjinPark
========================================================================================*/
long MLCNet::MaterialPortStateReport(EMaterialPortState eMaterialPortState)
{
	CString sMsg;
	long	lRet;

	/** Data Assign */
	m_eMaterialPortState = eMaterialPortState;
//	sMsg.Format(_T("@UL##%d##%d$"), m_nUnit, m_eMaterialPortState);
	sMsg.Format(_T("@UL##%s##%d$"), m_strReportID, m_eMaterialPortState);

	/** Data Send */
	lRet = SendString(sMsg);

	return lRet;
}

//SS chang
/*========================================================================================
	FUNCTION : MLCNet::EqSpecialStateReport()
	DESCRIPT : Current equipment Special state report
	RETURN	 : long
	ARGUMENT :
		eEqSpecialState= Current equipment state(1:Panel Supply Start,...,10:PC Reboot)
	FIRST	 : 2010/08/25, chang
========================================================================================*/
//SJ_YYK 110421 Modify.. Ellas
long MLCNet::EqSpecialStateReport(EEqSpecialState eEqSpecialState, EMaterialType eMaterialType, CString csDataHistory)
{

	return 0;  //SJ_YYK temp..

	CString sMsg;
	long	lRet;

	// Data Assign //
	m_eEqSpecialState = eEqSpecialState;
//	sMsg.Format(_T("@SS##%d##%d##%d##%s##$"), m_nUnit, m_eEqSpecialState, eMaterialType, csDataHistory);
	sMsg.Format(_T("@SS##%s##%d##%d##%s##$"), m_strReportID, m_eEqSpecialState, eMaterialType, csDataHistory);

	// Data Send //
	lRet = SendString(sMsg);

	return lRet;
}

long MLCNet::EqSpecialStateClear()
{
	CString sMsg;
	long	lRet = 0;
	EEqSpecialState	tmpState = m_eEqSpecialState;

	switch (m_eEqSpecialState)
	{
	case eProcessConditionChangeStart: tmpState = eProcessConditionChangeEnd;
		break;
	}

	if (m_eEqSpecialState != tmpState)
	{
		// Data Assign //
		m_eEqSpecialState = tmpState;
//		sMsg.Format(_T("@SS##%d##%d$"), m_nUnit, m_eEqSpecialState);
		sMsg.Format(_T("@SS##%s##%d$"), m_strReportID, m_eEqSpecialState);

		// Data Send //
		lRet = SendString(sMsg);
	}

	return lRet;
}

CString MLCNet::GetPMCode()
{
	return m_strPM_MSG;
}

// OLB 추가된 내용
/*========================================================================================
	FUNCTION : MLCNet::GetItem_XMLParse()
	DESCRIPT : 입력된 문자열에서 원하는 항목(Tag)의 데이터값을 구한다.
	RETURN	 : 
	        string : 문자열내에 피싱한 항목(태그) 데이터
	ARGUMENT :
		    (i)sBody= 분석해야할 문자열 
		    (i)sTag= 분석해야할 항목 
	FIRST	 : 2004/12/06, KJH
========================================================================================*/
// EX) GetItem("<ID>12</ID>", "ID")  
//     Result==>12
CString MLCNet::GetItem_XMLParse(CString sBody, CString sTag)
{
	int		nStart = 0;
	int		nEnd = 0;
	CString sResult = "";
	
	nStart = FindStartIndex_XMLParse(sBody,sTag);
	nEnd = FindEndIndex_XMLParse(sBody,sTag);
	if (nStart == -1 || nEnd == -1)
		return sResult;
	sResult = sBody.Mid(nStart, nEnd-nStart);
	return sResult;
}

/*========================================================================================
FUNCTION : MLCNet::GetItems_XMLParse()
DESCRIPT : 입력된 문자열에서 원하는 항목(Tag)의 데이터값을 구한다.
RETURN	 : 
string array : 문자열내에 피싱한 항목(태그) 데이터 배열
ARGUMENT :
(i)sBody= 분석해야할 문자열 
(i)sTag= 분석해야할 항목 
FIRST	 : 2004/12/06, KJH
========================================================================================*/
// EX) GetItem("<IDS><ID>12</ID><ID>13</ID><ID>14</ID></IDS>", "IDS")  
//     Result==>1.<ID>12</ID> 2.<ID>13</ID> 3.<ID>14</ID>
BOOL MLCNet::GetItems_XMLParse(CString sBody, CString sTag, CStringArray &srgItems)
{
	int i=0;
	int nStart = 0;
	int nEnd = 0;
	
	do
	{
		nStart = FindStartIndex_XMLParse(sBody,sTag);
		nEnd = FindEndIndex_XMLParse(sBody,sTag);
		if (nStart == -1 || nEnd == -1)
			break;
		
		CString sTemp = sBody.Mid(nStart, nEnd-nStart);
		srgItems.Add(sTemp);
		
		sBody = sBody.Mid(nEnd + sTag.GetLength() + 3, 
			              sBody.GetLength() - nEnd - sTag.GetLength() - 3);
	}
	while (sBody != "" || nStart != -1 || nEnd != -1);
	
	return true;
}

/*========================================================================================
FUNCTION : MLCNet::FindStartIndex_XMLParse()
DESCRIPT : 입력된 문자열에서 원하는 항목의 데이터 시작 위치을 구한다.
RETURN	 : 
n : 문자열내의 태그의 마지막 위치 
ARGUMENT :
(i)sBody= 분석해야할 문자열 
(i)sTag= 분석해야할 항목 
FIRST	 : 2004/12/06, KJH
========================================================================================*/
// EX) GetItem("<ID>12</ID>", "ID")  
//     Result==>1
int MLCNet::FindStartIndex_XMLParse(CString sBody, CString sTag)
{
	int nStart=0;
	sTag = "<"+sTag+">";
	nStart = sBody.Find(sTag);
	if (nStart == -1)
		return nStart;
	return nStart+sTag.GetLength();
}

/*========================================================================================
FUNCTION : MLCNet::FindEndIndex_XMLParse()
DESCRIPT : 입력된 문자열에서 원하는 항목의 데이터 마지막 위치을 구한다.
RETURN	 : 
n : 문자열내의 태그의 마지막 위치 
ARGUMENT :
(i)sBody= 분석해야할 문자열 
(i)sTag= 분석해야할 항목 
FIRST	 : 2004/12/06, KJH
========================================================================================*/
// EX) GetItem("<ID>12</ID>", "ID")  
//     Result==>7
int MLCNet::FindEndIndex_XMLParse(CString sBody, CString sTag)
{
	int nEnd = 0;
	sTag = "</"+sTag+">";
	nEnd = sBody.Find(sTag);
	return nEnd;
}

/*========================================================================================
FUNCTION : MLCNet::AnaysisMessage_SelfFormat()
DESCRIPT : L/C에서 받은 문자열을 분석해서 각각의 해당 처리을 한다.
RETURN	 : 
ARGUMENT :
(i)sRcvMsg= L/C에서 받은 문자열(자체 정의 포맷) 
UPDATER	 : 2005/01/22, KJH
========================================================================================*/
void  MLCNet::AnaysisMessage_SelfFormat(CString sRcvMsg)
{
//	Lock();	//110629.KKY

	CString sLog;			//Log Msg
	CString sMsg;			//Recv Msg Data
	CString sMsgType;		//Recv Msg Type
	CString sItem[1024];	//Msg Body
	int		nItem, nPrev;	//Msg Index
	int		nItemCnt;		//Body Count
	int		nIndex = 0;
	int j;
	
	/** Get Recv Msg */
	sMsg = sRcvMsg;
	/** Msg Parsing Rule */
	sMsg = sMsg.Mid(1,sMsg.GetLength() - 2);	//Remove head,tail
	nPrev = nItemCnt = 0;
	for (nItem = sMsg.Find(_T("##"),1); nPrev != 1; nItem = sMsg.Find(_T("##"),nItem + 2))
	{
		if (nPrev == 0)
		{
			sMsgType = sMsg.Mid(nPrev,nItem);
		}
		else if (nItem == -1) 
		{
			sItem[nItemCnt++] = sMsg.Mid(nPrev,sMsg.GetLength()-nPrev);
		}
		else
		{
			sItem[nItemCnt++] = sMsg.Mid(nPrev,nItem-nPrev);
		}

		nPrev = nItem + 2;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////
	////Online Mode Command
	////////////////////////////////////////////////////////////////////////////////////////
	if (sMsgType == "LO")  // Online Mode Command
	{			
		/**Item Count & Unit No Validation Check */
		if (nItemCnt == 2) {
			RequestOnlineMode(sItem[1]);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////
	else if (sMsgType == "LS") // Current Equipment State Request
	{		
		/** Item Count & Unit No Validation Check */
		//if ((nItemCnt == 1) && (atoi(sItem[0]) == m_nUnit)) 
//		if ((nItemCnt == 1) && (_wtoi(sItem[0]) == m_nUnit)) 
		if ((nItemCnt == 1) && (_wtoi(sItem[0]) == m_strReportID)) //20110811 SJ_HJG
		{
			//Current Equipment State Report
			//			EqStateReport(eEqNothing, eEqProcNothing);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Production Information Command
	////////////////////////////////////////////////////////////////////////////////////////
	else if (sMsgType == "LP")  // Product Information Request
	{
		/** Item Count & Unit No Validation Check */
		if (nItemCnt == 2) 
		{
			//Production Information Code Validation
			//switch (atoi(sItem[1])) 
			switch (_wtoi(sItem[1])) 
			{
			case 1:		//Production Information Request
				//				ProductDataReport();
				break;
			case 2:		//Total Production Information Clear Request
				//				ClearTotalProduct();
				break;
			case 3:		//Shift Production Information Clear Request
				//				ClearShiftProduct();
				break;
			}
		}
		//110620.KKY.Add_____
		else
		{
			nIndex = 1;
			m_pReceivedPanelData->m_strHPanelID  = sItem[nIndex++];		// Ascil  12byte					
			m_pReceivedPanelData->m_strEPanelID	= sItem[nIndex++];		// Ascil  12byte
			m_pReceivedPanelData->m_strBatchID	= sItem[nIndex++];		// Ascil  12byte				
			m_pReceivedPanelData->m_strProdType  = sItem[nIndex++];		// Ascil  2byte
			m_pReceivedPanelData->m_strProdKind	= sItem[nIndex++];		// Ascil  2byte
			m_pReceivedPanelData->m_strDeviceID	= sItem[nIndex++];		// Ascil  18byte
			m_pReceivedPanelData->m_strStepID	= sItem[nIndex++];		// Ascil  10byte
			m_pReceivedPanelData->m_strPPID		= sItem[nIndex++];		// Ascil  16byte
			m_pReceivedPanelData->m_strThickness	= sItem[nIndex++];		// Binary 2byte
			m_pReceivedPanelData->m_strInsFlag	= sItem[nIndex++];		// Ascil  2byte
			m_pReceivedPanelData->m_strPanelSize	= sItem[nIndex++];		// Binary 4byte
			
			m_pReceivedPanelData->m_strPanelPosition	= sItem[nIndex++];	// Ascil  2byte
			m_pReceivedPanelData->m_strCount1		= sItem[nIndex++];	// Ascil  2byte
			m_pReceivedPanelData->m_strCount2		= sItem[nIndex++];	// Ascil  2byte
			m_pReceivedPanelData->m_strGrade			= sItem[nIndex++];	// Bits	  6byte
			m_pReceivedPanelData->m_strComment		= sItem[nIndex++];	// Ascil  16byte
			m_pReceivedPanelData->m_strCompSize		= sItem[nIndex++];	// Binary 2byte
			m_pReceivedPanelData->m_strReadingFlag	= sItem[nIndex++];	// Ascil  2byte
			m_pReceivedPanelData->m_strPanelState	= sItem[nIndex++];	// Binary 2byte
			m_pReceivedPanelData->m_strJudgement		= sItem[nIndex++];	// Ascil  4byte
			m_pReceivedPanelData->m_strCode			= sItem[nIndex++];	// Ascil  4byte
			m_pReceivedPanelData->m_strRunLine		= sItem[nIndex++];	// Binary 20byte
			m_pReceivedPanelData->m_strUniqueId		= sItem[nIndex++];	// Binary 4byte
			m_pReceivedPanelData->m_strPairHPanelID	= sItem[nIndex++];	// Ascil  12byte
			m_pReceivedPanelData->m_strPairEPanelID	= sItem[nIndex++];	// Ascil  12byte
			m_pReceivedPanelData->m_strPairGrade		= sItem[nIndex++];	// Bits   6byte
			m_pReceivedPanelData->m_strPairUniqueId	= sItem[nIndex++];	// Binary 4byte
			m_pReceivedPanelData->m_strFlowRecipe	= sItem[nIndex++];	// Ascil  2byte
			m_pReceivedPanelData->m_strReserved0		= sItem[nIndex++];	// Any    12byte
			m_pReceivedPanelData->m_strBitsSignals	= sItem[nIndex++];	// Bits   2byte
			m_pReceivedPanelData->m_strReferData		= sItem[nIndex++];	// Any    4byte			

			TRACE(_T("m_pReceivedPanelData->m_strHPanelID: %s\n"), m_pReceivedPanelData->m_strHPanelID);
			TRACE(_T("m_pReceivedPanelData->m_strEPanelID: %s\n"), m_pReceivedPanelData->m_strEPanelID);
			TRACE(_T("m_pReceivedPanelData->m_strBatchID: %s\n"), m_pReceivedPanelData->m_strBatchID);
			// 정상  Panel 투입 나중에 L/C 에서 Command를 주기로 함
			m_iaEquipmentCmd.Add(eEqCmdRcvPanelData);
		}
		//___________________
	}
	//100807.ECID__________________________________________________________________

	////////////////////////////////////////////////////////////////////////////////////////
	// Equipment Constant Request Command
	////////////////////////////////////////////////////////////////////////////////////////
	else if (sMsgType == "EC")
	{
		// Initial Request
		if ((EEqConstCmd)_wtoi(sItem[1]) == eInitRequest)
		{
			RequestEqConstChange(eInitRequest);
		}
		// Change Request
		else if ((EEqConstCmd)_wtoi(sItem[1]) == eChangeRequest)
		{
			MEqConstant* pEC = NULL;
			pEC = new MEqConstant;

			/** Item Count & Unit No Validation Check */
			if (nItemCnt == 10) 
			{
				// 변경되는 EC Data가 하나인 경우
				pEC->m_strECID = sItem[3];
				pEC->m_strECName = sItem[4];
				pEC->m_strECDef = sItem[5];
				pEC->m_strECSLL = sItem[6];
				pEC->m_strECSUL = sItem[7];
				pEC->m_strECWLL = sItem[8];
				pEC->m_strECWUL = sItem[9];

				RequestEqConstChange(eChangeRequest, pEC);
			}
			else if (nItemCnt > 10)
			{
				j = 3; // EC Data
				int nDataCount = _wtoi(sItem[2]);
				for (int i=0; i<nDataCount; i++)
				{
					// 3~9, 10~16, 17~23,...
					pEC->m_strECID = sItem[j++];
					pEC->m_strECName = sItem[j++];
					pEC->m_strECDef = sItem[j++];
					pEC->m_strECSLL = sItem[j++];
					pEC->m_strECSUL = sItem[j++];
					pEC->m_strECWLL = sItem[j++];
					pEC->m_strECWUL = sItem[j++];
					// Add to List
					SetParamEC(pEC);
				}

				RequestEqConstChange(eChangeRequest);
			}

			delete pEC;
		}
	}
	//______________________________________________________________________

	////////////////////////////////////////////////////////////////////////////////////////
	// Parameter Request Command
	////////////////////////////////////////////////////////////////////////////////////////
	else if (sMsgType == "LD")  //SV, ECV, EOV Update(or Display) Request
	{		
		/** Item Count & Unit No Validation Check */
		if (nItemCnt == 2) 
		{
			/** Equipment State Variable */
			//if ((EParamType)atoi(sItem[1]) == eEqStatus)
			if ((EParamType)_wtoi(sItem[1]) == eEqStatus)
			{		
				MStateVariable*	pSV = NULL;
				pSV = new MStateVariable;
				/** Get SV Data */
				RequestParamChange(eEqStatus, pSV);
			}
			/** Equipment Constant */
			//else if ((EParamType)atoi(sItem[1]) == eEqConstant)
			else if ((EParamType)_wtoi(sItem[1]) == eEqConstant)
			{
				MEqConstant* pEC = NULL;
				pEC = new MEqConstant;
				/** Get EC Data */
				RequestParamChange(eEqStatus, pEC);
			}
			/** Equipment Online Parameter */
			//else if ((EParamType)atoi(sItem[1]) == eEqOnline)
			else if ((EParamType)_wtoi(sItem[1]) == eEqOnline)
			{
				MEqOnlineParam* pEO = NULL;
				pEO = new MEqOnlineParam;
				/** Get EO Data */
				RequestParamChange(eEqOnline, pEO);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Process Command
	////////////////////////////////////////////////////////////////////////////////////////
	else if (sMsgType == "LJ")  //Job Process Command
	{		
		/** Item Count & Unit No Validation Check */
		if (nItemCnt == 3) 
		{
			/** Paramenter Type Validation	*/
			//RequestProcessCmd((EProcCmd)atoi(sItem[1]), sItem[2]);
			RequestProcessCmd((EProcCmd)_wtoi(sItem[1]), sItem[2]);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Equipment Command
	////////////////////////////////////////////////////////////////////////////////////////
	else if (sMsgType == "LE")  // Equipment Command
	{		
		/** Item Count & Unit No Validation Check */
		if (nItemCnt == 3 || nItemCnt == 4)
		{
			/** Paramenter Type Validation */
			//RequestEquipmentCmd((EEqCmd)atoi(sItem[1]), sItem[2]);
			RequestEquipmentCmd((EEqCmd)_wtoi(sItem[1]), sItem[2]);

			//110817 SJ_KJS
			if ((EEqCmd)_wtoi(sItem[1]) == eEqCmdPause
				 || (EEqCmd)_wtoi(sItem[1]) == eEqCmdResume)
			{
				m_strPauseMessage2ND = sItem[2];
			}

			if ((EEqCmd)_wtoi(sItem[1]) == eEqCmdOperatorCall)
			{
				m_strPauseMessage = sItem[2];
			}
			//____________________________


			//2007.11.13 GrouchyKim Modify.[v1.5]
			//2007.10.15 GrouchyKim Add.[v1.3]
			/** PM Command */
			//if ((EEqCmd)atoi(sItem[1]) == eEqCmdChgPM)
			if ((EEqCmd)_wtoi(sItem[1]) == eEqCmdChgPM)
			{		
				m_strPM_MSG = sItem[2];
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Material Command
	////////////////////////////////////////////////////////////////////////////////////////
	else if (sMsgType == "LM") 
	{		
		/** Item Count & Unit No Validation Check */
		if (nItemCnt == 3) 
		{
			/** Paramenter Type Validation */
			//RequestMaterialCmd((EEqCmd)atoi(sItem[1]), sItem[2]);
			RequestMaterialCmd((EEqCmd)_wtoi(sItem[1]), sItem[2]);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Panel Transfer Command
	////////////////////////////////////////////////////////////////////////////////////////
	else if (sMsgType == "LC")  // Component(Panel) Transfer Command
	{		
		/** Item Count & Unit No Validation Check */
		if (nItemCnt == 2) 
		{
			/** Paramenter Type Validation */
			//			RequestTransferCmd(sItem[1]);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// PPID control command
	////////////////////////////////////////////////////////////////////////////////////////
	else if (sMsgType == "PP") // PPID Control Command
	{		
/*		CStringArray	asCCode;
		CStringArray	asBody;
		MProcessProgram*	 pPP = NULL;
		EPPIDModeType		eMode;
*/		
		/** Item Count & Unit No Validation Check */
/*
		pPP = new MProcessProgram;
		
		nIndex = 1;
		eMode = (EPPIDModeType)atoi(sItem[nIndex++]);
		
		pPP->m_strPPID = sItem[nIndex++];
		pPP->SetCount(atoi(sItem[nIndex++])); 
		for (int i = 0; i < pPP->m_nCCodeQty; i++)
		{
			GetSubItem(sItem[nIndex++], asCCode);
			pPP->m_paCCode[i].m_nCCode = atoi(asCCode.GetAt(0)); 
			pPP->m_paCCode[i].SetCount(atoi(asCCode.GetAt(1)));  
			for (int j = 0; j < pPP->m_paCCode[i].m_nPPBodyQty; j++)
			{
				GetSubItem(asCCode.GetAt(j+2), asBody, 2);
				pPP->m_paCCode[i].m_paPPBoby[j].m_strPParamName = asBody.GetAt(0); 
				pPP->m_paCCode[i].m_paPPBoby[j].m_strPParamValue = asBody.GetAt(1); 
				asBody.RemoveAll(); 
			}
			asCCode.RemoveAll();
		}
		
		RequestProcessProgram(eMode, pPP);
*/	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Product Plan(Batch) Information
	////////////////////////////////////////////////////////////////////////////////////////
	else if (sMsgType == "LB") 
	{
		Lock();

		int i = 0;

		/** Item Count & Unit No Validation Check */
		//110803 SJ_KJS
//		if (nItemCnt == 3) 
		if (nItemCnt == 42 || nItemCnt == 22 ) 
		{
			MProductPlanBatchData		*pBI = NULL;
			EEditType		eType;
			
			//eType = (EEditType)atoi(sItem[nIndex++]);
			eType = (EEditType)_wtoi(sItem[nIndex++]);
//			pBI = new MProductPlanBatchData;
			
//			pBI->m_nPlanCode			= _wtoi(sItem[nIndex++]);		 
//			pBI->m_nPlanQty			= _wtoi(sItem[nIndex++]);
			int iPlanQty = _wtoi(sItem[nIndex++]);
//			for (int i=0; i<iPlanQty; i++)
//			{
				pBI = new MProductPlanBatchData;
				pBI->m_nPlanQty			= iPlanQty;
				pBI->m_nPlanOrderID			= _wtoi(sItem[nIndex++]);		 
				pBI->m_nPriorty			= _wtoi(sItem[nIndex++]);		 
				pBI->m_sProdKind		= sItem[nIndex++];		
				pBI->m_sProdType		= sItem[nIndex++];	 
				pBI->m_sDeviceID		= sItem[nIndex++];	 
				pBI->m_sStepID		= sItem[nIndex++];		
				pBI->m_sPPID		= sItem[nIndex++];		
				pBI->m_sBatchID		= sItem[nIndex++];	 
				//pBI->m_nBatchSize		= atoi(sItem[nIndex++]);	 
				//pBI->m_nBatchState		= atoi(sItem[nIndex++]);		 
				pBI->m_nBatchSize		= _wtoi(sItem[nIndex++]);	 
				pBI->m_nBatchState		= _wtoi(sItem[nIndex++]);		 
				pBI->m_sPanelMaker		= sItem[nIndex++];	
				//pBI->m_nPanelThickness		= atoi(sItem[nIndex++]);		 
				pBI->m_nPanelThickness		= _wtoi(sItem[nIndex++]);		 
				pBI->m_sFirstGlassID		= sItem[nIndex++];
				pBI->m_sCurGlassID		= sItem[nIndex++];
				pBI->m_sInputLineNo		= sItem[nIndex++];	 
				//pBI->m_nValidFlag	= atoi(sItem[nIndex++]); 
				//pBI->m_nCQTY	= atoi(sItem[nIndex++]);	 
				//pBI->m_nOQTY		= atoi(sItem[nIndex++]);		
				//pBI->m_nRQTY		= atoi(sItem[nIndex++]);		
				//pBI->m_nNQTY		= atoi(sItem[nIndex++]);		
				pBI->m_nValidFlag	= _wtoi(sItem[nIndex++]);
				pBI->m_nCQTY		= _wtoi(sItem[nIndex++]);
				pBI->m_nOQTY		= _wtoi(sItem[nIndex++]);
				pBI->m_nRQTY		= _wtoi(sItem[nIndex++]);
				pBI->m_nNQTY		= _wtoi(sItem[nIndex++]);

				//110803 SJ_KJS
				if(pBI->m_nPlanQty > 1)
				{
					pBI->m_nPlanOrderID2			= _wtoi(sItem[nIndex++]);		 
					pBI->m_nPriorty2			= _wtoi(sItem[nIndex++]);		 
					pBI->m_sProdKind2		= sItem[nIndex++];		
					pBI->m_sProdType2		= sItem[nIndex++];	 
					pBI->m_sDeviceID2		= sItem[nIndex++];	 
					pBI->m_sStepID2		= sItem[nIndex++];		
					pBI->m_sPPID2		= sItem[nIndex++];		
					pBI->m_sBatchID2		= sItem[nIndex++];	 
					pBI->m_nBatchSize2		= _wtoi(sItem[nIndex++]);	 
					pBI->m_nBatchState2		= _wtoi(sItem[nIndex++]);		 
					pBI->m_sPanelMaker2		= sItem[nIndex++];	
					pBI->m_nPanelThickness2		= _wtoi(sItem[nIndex++]);		 
					pBI->m_sFirstGlassID2		= sItem[nIndex++];
					pBI->m_sCurGlassID2		= sItem[nIndex++];
					pBI->m_sInputLineNo2		= sItem[nIndex++];	 
					pBI->m_nValidFlag2	= _wtoi(sItem[nIndex++]);
					pBI->m_nCQTY2		= _wtoi(sItem[nIndex++]);
					pBI->m_nOQTY2		= _wtoi(sItem[nIndex++]);
					pBI->m_nRQTY2		= _wtoi(sItem[nIndex++]);
					pBI->m_nNQTY2		= _wtoi(sItem[nIndex++]);
				}
				RequestProductPlan(eType, pBI);
//			}

		}
		Unlock();
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// TABIC Barcode Parsing Information
	////////////////////////////////////////////////////////////////////////////////////////
	else if (sMsgType == "LT") 
	{		
		MBarCodeParsingInfo*	pParseInfo = NULL;
		
		pParseInfo = new MBarCodeParsingInfo;
		pParseInfo->m_sMaker			= sItem[nIndex++];		 
		//pParseInfo->m_nStartIndex_ID	= atoi(sItem[nIndex++]);		 
		//pParseInfo->m_nEndIndex_ID		= atoi(sItem[nIndex++]);		
		//pParseInfo->m_nStartIndex_Qty	= atoi(sItem[nIndex++]);		 
		//pParseInfo->m_nEndIndex_Qty		= atoi(sItem[nIndex++]);		
		pParseInfo->m_nStartIndex_ID	= _wtoi(sItem[nIndex++]);		 
		pParseInfo->m_nEndIndex_ID		= _wtoi(sItem[nIndex++]);		
		pParseInfo->m_nStartIndex_Qty	= _wtoi(sItem[nIndex++]);		 
		pParseInfo->m_nEndIndex_Qty		= _wtoi(sItem[nIndex++]);		

		//110817 SJ_KJS
		m_strMaker = pParseInfo->m_sMaker;

		/** Paramenter Type Validation */
		for(int i = 0; i < nItemCnt; i++)
		{
			RequestMultiTerminalCmd(sItem[i]);		
		}
		
		//		RequestProductPlan(eType, pBI);
	}

	//110621.KKY.Add_____
	////////////////////////////////////////////////////////////////////////////////////////
	// Material CodeInfo Send [LineController에서 MC(Material CodeInfo)값을 설비로 전송
	////////////////////////////////////////////////////////////////////////////////////////
	else if (sMsgType == "MC")
	{
		MMaterialCodeInfo*	pMatInfo = NULL;

		pMatInfo	= new MMaterialCodeInfo;

		nIndex = 1;
		pMatInfo->m_sDeviceID	= sItem[nIndex++];
		pMatInfo->m_nM_KIND		= EMaterialKind(_wtoi(sItem[nIndex++]));
		pMatInfo->m_sM_STEP		= sItem[nIndex++];
		int nMCodeCnt			= _wtoi(sItem[nIndex++]);
		for (int i=0; i<nMCodeCnt; i++)
			pMatInfo->m_asM_CODE.Add(sItem[nIndex+i]);

		RequestMaterialCodeInfo(pMatInfo);
	}
	//___________________
	else 
	{
		//not used
	}
	////////////////////////////////////////////////////////////////////////////////////////

//	Unlock();	//110629.KKY
}

/*========================================================================================
FUNCTION : MLCNet::AnaysisMessage_XMLFormat()
DESCRIPT : L/C에서 받은 문자열을 분석해서 각각의 해당 처리을 한다.
RETURN	 : 
ARGUMENT :
(i)sRcvMsg= L/C에서 받은 문자열(XML 포맷) 
UPDATE	 : 2006/08/17, GrouchyKim
========================================================================================*/
void MLCNet::AnaysisMessage_XMLFormat(CString sRcvMsg)
{
//	Lock();	//110629.KKY

	CString	sMsgId="";
	CString	sValue="";
	CString	sLog="";
	CString sSubMsg="";
	CString sMsg="";
	
	
	sMsgId = GetItem_XMLParse(sRcvMsg, "MSG_ID");
	////////////////////////////////////////////////////////////////////////////////////////
	// Update Material Parameter
	////////////////////////////////////////////////////////////////////////////////////////
	if ("UpdateMatParam" == sMsgId) 
	{	
		int nQty = 0;
		
		// TABIC.
		sMsg=GetItem_XMLParse(sRcvMsg, "TABIC");
		sValue=GetItem_XMLParse(sMsg, "WARNING_LIMIT");
		//nQty = atoi(sValue);
		nQty = _wtoi(sValue);

		if (0 < nQty)
		{
			m_nWarningEvtLimitQty_TABIC = nQty;
		}
		sValue = GetItem_XMLParse(sMsg, "PREHAND_LIMIT");
		//nQty = atoi(sValue);
		nQty = _wtoi(sValue);
		if (0 < nQty)
		{
			m_nPreHandEvtLimitQty_TABIC = nQty;
		}
		
		// ZIG.
		sMsg = GetItem_XMLParse(sRcvMsg, "ZIG");
		sValue = GetItem_XMLParse(sMsg, "WARNING_LIMIT");
		//nQty = atoi(sValue);
		nQty = _wtoi(sValue);
		if (0 < nQty)
		{
			m_nWarningEvtLimitQty_ZIG = nQty;
		}
		sValue=GetItem_XMLParse(sMsg, "PREHAND_LIMIT");
		//nQty = atoi(sValue);
		nQty = _wtoi(sValue);
		if (0 < nQty)
		{
			m_nPreHandEvtLimitQty_ZIG = nQty;
		}
		
		// Inform AutoManager.
		RequestMaterialCmd(eMatCmdQty,"Update");
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Request Material Parameter from L/C
	////////////////////////////////////////////////////////////////////////////////////////
	if ("UpdateMatInfo" == sMsgId) 
	{	
		MMaterial   Mat;
		
		sValue=GetItem_XMLParse(sRcvMsg, "PORTID");
		Mat.m_sPORTID = sValue;			
		sValue=GetItem_XMLParse(sRcvMsg, "EQP_STATE");
		Mat.m_sEQP_STATE = sValue;		
		sValue=GetItem_XMLParse(sRcvMsg, "PORT_STATE");
		Mat.m_sPORT_STATE = sValue;	
		sValue=GetItem_XMLParse(sRcvMsg, "PORT_TYPE");
		Mat.m_sPORT_TYPE = sValue;		
		sValue=GetItem_XMLParse(sRcvMsg, "CSTID");
		Mat.m_sCSTID = sValue;			
		sValue=GetItem_XMLParse(sRcvMsg, "MAP_STIF");
		//Mat.m_nMAP_STIF = atoi(sValue);		
		Mat.m_nMAP_STIF = _wtoi(sValue);		
		sValue=GetItem_XMLParse(sRcvMsg, "CUR_STIF");
		//Mat.m_nCUR_STIF = atoi(sValue);		
		Mat.m_nCUR_STIF = _wtoi(sValue);		
		sValue=GetItem_XMLParse(sRcvMsg, "M_BATCHID");
		Mat.m_sM_BATCHID = sValue;		
		sValue=GetItem_XMLParse(sRcvMsg, "M_MAKER");
		Mat.m_sM_MAKER = sValue;			
		sValue=GetItem_XMLParse(sRcvMsg, "M_KIND");
		//Mat.m_nM_KIND = atoi(sValue);			
		Mat.m_nM_KIND = _wtoi(sValue);			
		sValue=GetItem_XMLParse(sRcvMsg, "M_TYPE");
		//Mat.m_nM_TYPE = atoi(sValue);			
		Mat.m_nM_TYPE = _wtoi(sValue);			
		sValue=GetItem_XMLParse(sRcvMsg, "M_STATE");
		//Mat.m_nM_STATE = atoi(sValue);			
		Mat.m_nM_STATE = _wtoi(sValue);			
		sValue=GetItem_XMLParse(sRcvMsg, "T_QTY");
		//Mat.m_nT_QTY = atoi(sValue);			
		Mat.m_nT_QTY = _wtoi(sValue);			
		sValue=GetItem_XMLParse(sRcvMsg, "U_QTY");
		//Mat.m_nU_QTY = atoi(sValue);			
		Mat.m_nU_QTY = _wtoi(sValue);			
		sValue=GetItem_XMLParse(sRcvMsg, "R_QTY");
		//Mat.m_nR_QTY = atoi(sValue);			
		Mat.m_nR_QTY = _wtoi(sValue);			
		sValue=GetItem_XMLParse(sRcvMsg, "N_QTY");
		//Mat.m_nN_QTY = atoi(sValue);			
		Mat.m_nN_QTY = _wtoi(sValue);			
		sValue=GetItem_XMLParse(sRcvMsg, "A_QTY");
		//Mat.m_nA_QTY = atoi(sValue);			
		Mat.m_nA_QTY = _wtoi(sValue);			
		sValue=GetItem_XMLParse(sRcvMsg, "E_FLAG");
		//Mat.m_nE_FLAG = atoi(sValue);		
		Mat.m_nE_FLAG = _wtoi(sValue);		
		sValue=GetItem_XMLParse(sRcvMsg, "C_CODE");
		Mat.m_sC_CODE = sValue;		
		sValue=GetItem_XMLParse(sRcvMsg, "M_STEP");
		Mat.m_sM_STEP = sValue;		
		sValue=GetItem_XMLParse(sRcvMsg, "S_QTY");
		//Mat.m_nS_QTY = atoi(sValue);			
		Mat.m_nS_QTY = _wtoi(sValue);			
		sValue=GetItem_XMLParse(sRcvMsg, "M_CODE");
		Mat.m_sM_CODE = sValue;		
		
		m_MaterialData.CopyData(&Mat);
		
		// Inform AutoManager.
		RequestMaterialCmd(eMatCmdUpdate,"Update");
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Request Material Parameter from L/C
	////////////////////////////////////////////////////////////////////////////////////////
	if ("RequestMatParam"==sMsgId) 
	{	
		CString  sSndMsg = "";
		
		/** Data Assign */
		//sSndMsg.Format(
		//	"<MESSAGE>"
		//	"<HEADER>"
		//	"<MSG_ID>ReportMatParam</MSG_ID>"
		//	"</HEADER>"
		//	"<BODY>"
		//	"<TABIC>"
		//	"<WARNING_LIMIT>%d</WARNING_LIMIT>"
		//	"<PREHAND_LIMIT>%d</PREHAND_LIMIT>"
		//	"</TABIC>"
		//	"<ZIG>"
		//	"<WARNING_LIMIT>%d</WARNING_LIMIT>"
		//	"<PREHAND_LIMIT>%d</PREHAND_LIMIT>"
		//	"</ZIG>"
		//	"</BODY>"
		//	"</MESSAGE>",
		//	m_nWarningEvtLimitQty_TABIC, m_nPreHandEvtLimitQty_TABIC,
		//	m_nWarningEvtLimitQty_ZIG, m_nPreHandEvtLimitQty_ZIG);  
		sSndMsg.Format(
			_T("<MESSAGE>")
			_T("<HEADER>")
			_T("<MSG_ID>ReportMatParam</MSG_ID>")
			_T("</HEADER>")
			_T("<BODY>")
			_T("<TABIC>")
			_T("<WARNING_LIMIT>%d</WARNING_LIMIT>")
			_T("<PREHAND_LIMIT>%d</PREHAND_LIMIT>")
			_T("</TABIC>")
			_T("<ZIG>")
			_T("<WARNING_LIMIT>%d</WARNING_LIMIT>")
			_T("<PREHAND_LIMIT>%d</PREHAND_LIMIT>")
			_T("</ZIG>")
			_T("</BODY>")
			_T("</MESSAGE>"),
			m_nWarningEvtLimitQty_TABIC, m_nPreHandEvtLimitQty_TABIC,
			m_nWarningEvtLimitQty_ZIG, m_nPreHandEvtLimitQty_ZIG);  
		
		/** Data Send */
		SendString(sSndMsg);	  
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// No Parsing Information 없는 Maker를 받아서 저장한다.
	////////////////////////////////////////////////////////////////////////////////////////
	if ("InitNoParsingInfoMakerList" == sMsgId) 
	{	
		CStringArray	srgItems;
		
		m_srgNoParsingInfoMaker.RemoveAll();
		sMsg=GetItem_XMLParse(sRcvMsg, "MAKER_LIST");
		Split(sMsg, '/', srgItems);
		for (int ii = 0; ii < srgItems.GetSize(); ii++)
		{
			m_srgNoParsingInfoMaker.Add(srgItems.GetAt(ii));
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// LC에서 받은 현재시간을 받아서 저장한다. 2006.08.17 GrouchyKim Add.
	////////////////////////////////////////////////////////////////////////////////////////
	
	//110701.KKY_____
//	if ("InitCurDateTime" == sMsgId && false==m_fUpdateLCDateTime) 
	if ("InitCurDateTime" == sMsgId) 
	//_______________
	{	
	  	//2007.11.07 GrouchyKim Add.[v1.4]
	  	m_fUpdateLCDateTime = true;
	  	
		sMsg=GetItem_XMLParse(sRcvMsg, "DATE_TIME");
		if (14==sMsg.GetLength()){ m_saLCDateTime.Add(sMsg); }
	}
	////////////////////////////////////////////////////////////////////////////////////////

//	Unlock();	//110629.KKY
}

/*========================================================================================
FUNCTION : MLCNet::OnConnectionEvent()
DESCRIPT : L/C와 연결이 되면 초기 정보를 L/C로 보낸다.
RETURN	 : 
ARGUMENT :
FIRST	 : 2004/12/06, KJH
LAST	 : 2007/10/15, GrouchyKim
========================================================================================*/
void  MLCNet::OnConnectionEvent()
{
	CString   sSndMsg = "";
	CString   sTemp = "";
	CString   sModelName = "";
	CString   sPanel = "";
	MProcessProgram* pData = NULL;
	POSITION		Pos;
	
	
	// Connection 완료시 현재 상태를 L/C로 보고한다.
	/** Data Assign */
	//sSndMsg.Format(
	//	"<MESSAGE>"
	//	"<HEADER>"
	//	"<MSG_ID>InitStateData</MSG_ID>"
	//	"</HEADER>"
	//	"<BODY>"
	//	"<EQ_STATE>%d</EQ_STATE>"
	//	"<PM_CODE>%s</PM_CODE>"
	//	"<PROCESS_STATE>%d</PROCESS_STATE>"
	//	"</BODY>"
	//	"</MESSAGE>",
	//	(int)m_eEqState, GetPMCode(), (int)m_eEqProcState);
	sSndMsg.Format(
		_T("<MESSAGE>")
		_T("<HEADER>")
		_T("<MSG_ID>InitStateData</MSG_ID>")
		_T("</HEADER>")
		_T("<BODY>")
		_T("<EQ_STATE>%d</EQ_STATE>")
		_T("<PM_CODE>%s</PM_CODE>")
		_T("<PROCESS_STATE>%d</PROCESS_STATE>")
		_T("</BODY>")
		_T("</MESSAGE>"),
		(int)m_eEqState, GetPMCode(), (int)m_eEqProcState);
	
	/** Data Send */
	SendString(sSndMsg);

	//2007.10.15 GrouchyKim Add.[v1.3]
	if(m_eEqState == eFault)
	{
		// Connection 완료시 현재 발생된 알람을 L/C로 보고한다.
		/** Data Assign */
		//sSndMsg.Format(
		//	"<MESSAGE>"
		//	"<HEADER>"
		//	"<MSG_ID>InitAlarmData</MSG_ID>"
		//	"</HEADER>"
		//	"<BODY>"
		//	"<ONOFF>%d</ONOFF>"
		//	"<ID>%d</ID>"
		//	"<CODE>%d</CODE>"
		//	"<TEXT>%s</TEXT>"
		//	"</BODY>"
		//	"</MESSAGE>",
		//	m_nLastAlarmOnOff, m_nLastAlarmId, m_nLastAlarmCode, m_strLastAlarmText);
		sSndMsg.Format(
			_T("<MESSAGE>")
			_T("<HEADER>")
			_T("<MSG_ID>InitAlarmData</MSG_ID>")
			_T("</HEADER>")
			_T("<BODY>")
			_T("<ONOFF>%d</ONOFF>")
			_T("<ID>%d</ID>")
			_T("<CODE>%d</CODE>")
			_T("<TEXT>%s</TEXT>")
			_T("</BODY>")
			_T("</MESSAGE>"),
			m_nLastAlarmOnOff, m_nLastAlarmId, m_nLastAlarmCode, m_strLastAlarmText);		
		/** Data Send */
		SendString(sSndMsg);
	}
	
	Pos = m_pPPList->GetHeadPosition();
	for (; Pos != NULL; m_pPPList->GetNext(Pos))
	{
		pData = (MProcessProgram*)m_pPPList->GetAt(Pos);
		sTemp.Format(_T("%s/"), pData->m_strPPID);
		sModelName = sModelName + sTemp;
	}
	/** Data Assign */
	//sSndMsg.Format(
	//	"<MESSAGE>"
	//	"<HEADER>"
	//	"<MSG_ID>InitModelData</MSG_ID>"
	//	"</HEADER>"
	//	"<BODY>"
	//	"<CURRENT_MODEL_NAME>%s</CURRENT_MODEL_NAME>"
	//	"<ASSEMBLE_QTY_FEEDER1>%d</ASSEMBLE_QTY_FEEDER1>"
	//	"<ASSEMBLE_QTY_FEEDER2>%d</ASSEMBLE_QTY_FEEDER2>"
	//	"<MAT_TYPE_FEEDER1>%s</MAT_TYPE_FEEDER1>"
	//	"<MAT_TYPE_FEEDER2>%s</MAT_TYPE_FEEDER2>"
	//	"<MODEL_LIST>%s</MODEL_LIST>"
	//	"</BODY>"
	//	"</MESSAGE>",
	//	m_sCurModelName, m_nAssembleQty_Feeder1, m_nAssembleQty_Feeder2,
	//	m_sFeeder1MaterialType, m_sFeeder2MaterialType, sModelName);
	sSndMsg.Format(
		_T("<MESSAGE>")
		_T("<HEADER>")
		_T("<MSG_ID>InitModelData</MSG_ID>")
		_T("</HEADER>")
		_T("<BODY>")
		_T("<CURRENT_MODEL_NAME>%s</CURRENT_MODEL_NAME>")
		_T("<ASSEMBLE_QTY_FEEDER1>%d</ASSEMBLE_QTY_FEEDER1>")
		_T("<ASSEMBLE_QTY_FEEDER2>%d</ASSEMBLE_QTY_FEEDER2>")
		_T("<MAT_TYPE_FEEDER1>%s</MAT_TYPE_FEEDER1>")
		_T("<MAT_TYPE_FEEDER2>%s</MAT_TYPE_FEEDER2>")
		_T("<MODEL_LIST>%s</MODEL_LIST>")
		_T("</BODY>")
		_T("</MESSAGE>"),
		m_sCurModelName, m_nAssembleQty_Feeder1, m_nAssembleQty_Feeder2,
		m_sFeeder1MaterialType, m_sFeeder2MaterialType, sModelName);
	/** Data Send */
	SendString(sSndMsg);
	
	//Data Assign
//  sSndMsg.Format(
//	  "<MESSAGE>"
//    		"<HEADER>"
//    			"<MSG_ID>InitPanelData</MSG_ID>"
//    		"</HEADER>"
//    		"<BODY>"
//    		  "<PANEL_LIST>");
    sSndMsg.Format(
	  _T("<MESSAGE>")
    		_T("<HEADER>")
    			_T("<MSG_ID>InitPanelData</MSG_ID>")
    		_T("</HEADER>")
    		_T("<BODY>")
    		  _T("<PANEL_LIST>"));

	for (int i=0; i<MAX_UNIT; i++)
	{
		if (""!=m_rgPanel[i].m_strEPanelID)
		{
			sPanel.Format(
				//"<PANEL>"
				//"<INDEX>%d</INDEX>"
				//"<H_PANEL_ID>%s</H_PANEL_ID>"
				//"<E_PANEL_ID>%s</E_PANEL_ID>"
				//"<BATCH_ID>%s</BATCH_ID>"
				//"<PROD_TYPE>%s</PROD_TYPE>"
				//"<PROD_KIND>%s</PROD_KIND>"
				//"<PROCESS_ID>%s</PROCESS_ID>"
				//"<STEP_ID>%s</STEP_ID>"
				//"<PPID>%s</PPID>"
				//"<PANEL_THICKNESS>%s</PANEL_THICKNESS>"
				//"<INSPECTION_FLAG>%s</INSPECTION_FLAG>"
				//"<PANEL_SIZE>%s</PANEL_SIZE>"
				//"<PANEL_POSITION>%s</PANEL_POSITION>"
				//"<COUNT1>%s</COUNT1>"
				//"<COUNT2>%s</COUNT2>"
				//"<GRADE>%s</GRADE>"
				//"<COMMENT>%s</COMMENT>"
				//"<COMPSIZE>%s</COMPSIZE>"
				//"<READINGFLAG>%s</READINGFLAG>"
				//"<PANELSTATE>%s</PANELSTATE>"
				//"<JUDGMENT>%s</JUDGMENT>"
				//"<CODE>%s</CODE>"
				//"<RUNNING_HISTORY>%s</RUNNING_HISTORY>"
				//"<PANEL_UNIQUE_ID_>%s</PANEL_UNIQUE_ID_>"
				//"<PAIR_H_PANEL_ID>%s</PAIR_H_PANEL_ID>"
				//"<PAIR_E_PANEL_ID>%s</PAIR_E_PANEL_ID>"
				//"<PAIR_PANEL_GRADE>%s</PAIR_PANEL_GRADE>"
				//"<PAIR_PANEL_UNIQUE_ID>%s</PAIR_PANEL_UNIQUE_ID>"
				//"<FLOW_RECIPE>%s</FLOW_RECIPE>"
				//"<RESERVED0>%s</RESERVED0>"
				//"<BITSIGNALS>%s</BITSIGNALS>"
				//"<REFER_DATA>%s</REFER_DATA>"
				//"</PANEL>",
				_T("<PANEL>")
				_T("<INDEX>%d</INDEX>")
				_T("<H_PANEL_ID>%s</H_PANEL_ID>")
				_T("<E_PANEL_ID>%s</E_PANEL_ID>")
				_T("<BATCH_ID>%s</BATCH_ID>")
				_T("<PROD_TYPE>%s</PROD_TYPE>")
				_T("<PROD_KIND>%s</PROD_KIND>")
				_T("<PROCESS_ID>%s</PROCESS_ID>")
				_T("<STEP_ID>%s</STEP_ID>")
				_T("<PPID>%s</PPID>")
				_T("<PANEL_THICKNESS>%s</PANEL_THICKNESS>")
				_T("<INSPECTION_FLAG>%s</INSPECTION_FLAG>")
				_T("<PANEL_SIZE>%s</PANEL_SIZE>")
				_T("<PANEL_POSITION>%s</PANEL_POSITION>")
				_T("<COUNT1>%s</COUNT1>")
				_T("<COUNT2>%s</COUNT2>")
				_T("<GRADE>%s</GRADE>")
				_T("<COMMENT>%s</COMMENT>")
				_T("<COMPSIZE>%s</COMPSIZE>")
				_T("<READINGFLAG>%s</READINGFLAG>")
				_T("<PANELSTATE>%s</PANELSTATE>")
				_T("<JUDGMENT>%s</JUDGMENT>")
				_T("<CODE>%s</CODE>")
				_T("<RUNNING_HISTORY>%s</RUNNING_HISTORY>")
				_T("<PANEL_UNIQUE_ID_>%s</PANEL_UNIQUE_ID_>")
				_T("<PAIR_H_PANEL_ID>%s</PAIR_H_PANEL_ID>")
				_T("<PAIR_E_PANEL_ID>%s</PAIR_E_PANEL_ID>")
				_T("<PAIR_PANEL_GRADE>%s</PAIR_PANEL_GRADE>")
				_T("<PAIR_PANEL_UNIQUE_ID>%s</PAIR_PANEL_UNIQUE_ID>")
				_T("<FLOW_RECIPE>%s</FLOW_RECIPE>")
				_T("<RESERVED0>%s</RESERVED0>")
				_T("<BITSIGNALS>%s</BITSIGNALS>")
				_T("<REFER_DATA>%s</REFER_DATA>")
				_T("</PANEL>"),
				i+1,
				m_rgPanel[i].m_strHPanelID,		// Ascil  12byte					
				m_rgPanel[i].m_strEPanelID,		// Ascil  12byte
				m_rgPanel[i].m_strBatchID,		// Ascil  12byte				
				m_rgPanel[i].m_strProdType,		// Ascil  2byte
				m_rgPanel[i].m_strProdKind,		// Ascil  2byte
				m_rgPanel[i].m_strDeviceID,		// Ascil  18byte
				m_rgPanel[i].m_strStepID,		// Ascil  10byte
				m_rgPanel[i].m_strPPID,		// Ascil  16byte
				m_rgPanel[i].m_strThickness,		// Binary 2byte
				m_rgPanel[i].m_strInsFlag,		// Ascil  2byte

				m_rgPanel[i].m_strPanelSize,		// Binary 4byte
				m_rgPanel[i].m_strPanelPosition,	// Ascil  2byte
				m_rgPanel[i].m_strCount1,		// Ascil  2byte
				m_rgPanel[i].m_strCount2,		// Ascil  2byte
				m_rgPanel[i].m_strGrade,		// Bits	  6byte
				m_rgPanel[i].m_strComment,		// Ascil  16byte
				m_rgPanel[i].m_strCompSize,		// Binary 2byte
				m_rgPanel[i].m_strReadingFlag,	// Ascil  2byte
				m_rgPanel[i].m_strPanelState,	// Binary 2byte
				m_rgPanel[i].m_strJudgement,		// Ascil  4byte

				m_rgPanel[i].m_strCode,		// Ascil  4byte
				m_rgPanel[i].m_strRunLine,		// Binary 20byte
				m_rgPanel[i].m_strUniqueId,		// Binary 4byte
				m_rgPanel[i].m_strPairHPanelID,	// Ascil  12byte
				m_rgPanel[i].m_strPairEPanelID,	// Ascil  12byte
				m_rgPanel[i].m_strPairGrade,		// Bits   6byte
				m_rgPanel[i].m_strPairUniqueId,	// Binary 4byte
				m_rgPanel[i].m_strFlowRecipe,	// Ascil  2byte
				m_rgPanel[i].m_strReserved0,		// Any    12byte
				m_rgPanel[i].m_strBitsSignals,	// Bits   2byte

				m_rgPanel[i].m_strReferData);		// Any    4byte
				
			sSndMsg += sPanel;
		}
	}
//           sTemp.Format(  
//                  		  "</PANEL_LIST>"
//                  		"</BODY>"
//              	  "</MESSAGE>");
           sTemp.Format(  
                  		  _T("</PANEL_LIST>")
                  		_T("</BODY>")
              	  _T("</MESSAGE>"));
              	  
    sSndMsg += sTemp;
	//Data Send
	SendString(sSndMsg);
	
	
	// L/C 재연결시 설비<=>L/C간의 데이터 동기화를 위한 부분.
	/** Data Assign */
	sSndMsg.Format(
//		"<MESSAGE>"
//      		"<HEADER>"
//			"<MSG_ID>UpdateMatInfo</MSG_ID>"
//			"</HEADER>"
//			"<BODY>"
//			"<FEEDER1>"
//			"<PORTID>%s</PORTID>"
//			"<EQP_STATE>%s</EQP_STATE>"
//			"<PORT_STATE>%s</PORT_STATE>"
//			"<PORT_TYPE>%s</PORT_TYPE>"
//			"<CSTID>%s</CSTID>"
//			"<MAP_STIF>%d</MAP_STIF>"
//			"<CUR_STIF>%d</CUR_STIF>"
//			"<M_BATCHID>%s</M_BATCHID>"
//			"<M_MAKER>%s</M_MAKER>"
//			"<M_KIND>%d</M_KIND>"
//			"<M_TYPE>%d</M_TYPE>"
//			"<M_STATE>%d</M_STATE>"
//			"<T_QTY>%d</T_QTY>"
//			"<U_QTY>%d</U_QTY>"
//			"<R_QTY>%d</R_QTY>"
//			"<N_QTY>%d</N_QTY>"
//			"<A_QTY>%d</A_QTY>"
//			"<E_FLAG>%d</E_FLAG>"
//			"<C_CODE>%s</C_CODE>"
//			"<M_STEP>%s</M_STEP>"
//			"<S_QTY>%d</S_QTY>"
//			"<M_CODE>%s</M_CODE>"
//			"</FEEDER1>"
//			"<FEEDER2>"
//			"<PORTID>%s</PORTID>"
//			"<EQP_STATE>%s</EQP_STATE>"
//			"<PORT_STATE>%s</PORT_STATE>"
//			"<PORT_TYPE>%s</PORT_TYPE>"
//			"<CSTID>%s</CSTID>"
//			"<MAP_STIF>%d</MAP_STIF>"
//			"<CUR_STIF>%d</CUR_STIF>"
//			"<M_BATCHID>%s</M_BATCHID>"
//			"<M_MAKER>%s</M_MAKER>"
//			"<M_KIND>%d</M_KIND>"
//			"<M_TYPE>%d</M_TYPE>"
//			"<M_STATE>%d</M_STATE>"
//			"<T_QTY>%d</T_QTY>"
//			"<U_QTY>%d</U_QTY>"
//			"<R_QTY>%d</R_QTY>"
//			"<N_QTY>%d</N_QTY>"
//			"<A_QTY>%d</A_QTY>"
//			"<E_FLAG>%d</E_FLAG>"
//			"<C_CODE>%s</C_CODE>"
//			"<M_STEP>%s</M_STEP>"
//			"<S_QTY>%d</S_QTY>"
//			"<M_CODE>%s</M_CODE>"
//			"</FEEDER2>"
//			"</BODY>"
//			"</MESSAGE>",
//
		_T("<MESSAGE>")
      		_T("<HEADER>")
			_T("<MSG_ID>UpdateMatInfo</MSG_ID>")
			_T("</HEADER>")
			_T("<BODY>")
			_T("<FEEDER1>")
			_T("<PORTID>%s</PORTID>")
			_T("<EQP_STATE>%s</EQP_STATE>")
			_T("<PORT_STATE>%s</PORT_STATE>")
			_T("<PORT_TYPE>%s</PORT_TYPE>")
			_T("<CSTID>%s</CSTID>")
			_T("<MAP_STIF>%d</MAP_STIF>")
			_T("<CUR_STIF>%d</CUR_STIF>")
			_T("<M_BATCHID>%s</M_BATCHID>")
			_T("<M_MAKER>%s</M_MAKER>")
			_T("<M_KIND>%d</M_KIND>")
			_T("<M_TYPE>%d</M_TYPE>")
			_T("<M_STATE>%d</M_STATE>")
			_T("<T_QTY>%d</T_QTY>")
			_T("<U_QTY>%d</U_QTY>")
			_T("<R_QTY>%d</R_QTY>")
			_T("<N_QTY>%d</N_QTY>")
			_T("<A_QTY>%d</A_QTY>")
			_T("<E_FLAG>%d</E_FLAG>")
			_T("<C_CODE>%s</C_CODE>")
			_T("<M_STEP>%s</M_STEP>")
			_T("<S_QTY>%d</S_QTY>")
			_T("<M_CODE>%s</M_CODE>")
			_T("</FEEDER1>")
			_T("<FEEDER2>")
			_T("<PORTID>%s</PORTID>")
			_T("<EQP_STATE>%s</EQP_STATE>")
			_T("<PORT_STATE>%s</PORT_STATE>")
			_T("<PORT_TYPE>%s</PORT_TYPE>")
			_T("<CSTID>%s</CSTID>")
			_T("<MAP_STIF>%d</MAP_STIF>")
			_T("<CUR_STIF>%d</CUR_STIF>")
			_T("<M_BATCHID>%s</M_BATCHID>")
			_T("<M_MAKER>%s</M_MAKER>")
			_T("<M_KIND>%d</M_KIND>")
			_T("<M_TYPE>%d</M_TYPE>")
			_T("<M_STATE>%d</M_STATE>")
			_T("<T_QTY>%d</T_QTY>")
			_T("<U_QTY>%d</U_QTY>")
			_T("<R_QTY>%d</R_QTY>")
			_T("<N_QTY>%d</N_QTY>")
			_T("<A_QTY>%d</A_QTY>")
			_T("<E_FLAG>%d</E_FLAG>")
			_T("<C_CODE>%s</C_CODE>")
			_T("<M_STEP>%s</M_STEP>")
			_T("<S_QTY>%d</S_QTY>")
			_T("<M_CODE>%s</M_CODE>")
			_T("</FEEDER2>")
			_T("</BODY>")
			_T("</MESSAGE>"),
			m_oMat_Feeder1.m_sPORTID,			
			m_oMat_Feeder1.m_sEQP_STATE,		
			m_oMat_Feeder1.m_sPORT_STATE,		
			m_oMat_Feeder1.m_sPORT_TYPE,		
			m_oMat_Feeder1.m_sCSTID,			
			m_oMat_Feeder1.m_nMAP_STIF,		
			m_oMat_Feeder1.m_nCUR_STIF,		
			m_oMat_Feeder1.m_sM_BATCHID,		
			m_oMat_Feeder1.m_sM_MAKER,			
			m_oMat_Feeder1.m_nM_KIND,			
			m_oMat_Feeder1.m_nM_TYPE,			
			m_oMat_Feeder1.m_nM_STATE,			
			m_oMat_Feeder1.m_nT_QTY,			
			m_oMat_Feeder1.m_nU_QTY,			
			m_oMat_Feeder1.m_nR_QTY,			
			m_oMat_Feeder1.m_nN_QTY,			
			m_oMat_Feeder1.m_nA_QTY,			
			m_oMat_Feeder1.m_nE_FLAG,			
			m_oMat_Feeder1.m_sC_CODE,			
			m_oMat_Feeder1.m_sM_STEP,			
			m_oMat_Feeder1.m_nS_QTY,			
			m_oMat_Feeder1.m_sM_CODE,			
			m_oMat_Feeder2.m_sPORTID,			
			m_oMat_Feeder2.m_sEQP_STATE,		
			m_oMat_Feeder2.m_sPORT_STATE,		
			m_oMat_Feeder2.m_sPORT_TYPE,		
			m_oMat_Feeder2.m_sCSTID,			
			m_oMat_Feeder2.m_nMAP_STIF,		
			m_oMat_Feeder2.m_nCUR_STIF,		
			m_oMat_Feeder2.m_sM_BATCHID,		
			m_oMat_Feeder2.m_sM_MAKER,			
			m_oMat_Feeder2.m_nM_KIND,			
			m_oMat_Feeder2.m_nM_TYPE,			
			m_oMat_Feeder2.m_nM_STATE,			
			m_oMat_Feeder2.m_nT_QTY,			
			m_oMat_Feeder2.m_nU_QTY,			
			m_oMat_Feeder2.m_nR_QTY,			
			m_oMat_Feeder2.m_nN_QTY,			
			m_oMat_Feeder2.m_nA_QTY,			
			m_oMat_Feeder2.m_nE_FLAG,			
			m_oMat_Feeder2.m_sC_CODE,			
			m_oMat_Feeder2.m_sM_STEP,			
			m_oMat_Feeder2.m_nS_QTY,			
			m_oMat_Feeder2.m_sM_CODE);	
	
		/** Data Send */
			SendString(sSndMsg);

	//2007.10.15 GrouchyKim Add.[v1.3]
	// L/C 재연결시 설비<=>L/C간의 데이터 동기화를 위한 부분.
	/** Data Assign */
	sSndMsg.Format(
//		"<MESSAGE>"
//      		"<HEADER>"
//			"<MSG_ID>UpdateMatInfo</MSG_ID>"
//			"</HEADER>"
//			"<BODY>"
//			"<FEEDER1>"
//			"<PORTID>%s</PORTID>"
//			"<EQP_STATE>%s</EQP_STATE>"
//			"<PORT_STATE>%s</PORT_STATE>"
//			"<PORT_TYPE>%s</PORT_TYPE>"
//			"<CSTID>%s</CSTID>"
//			"<MAP_STIF>%d</MAP_STIF>"
//			"<CUR_STIF>%d</CUR_STIF>"
//			"<M_BATCHID>%s</M_BATCHID>"
//			"<M_MAKER>%s</M_MAKER>"
//			"<M_KIND>%d</M_KIND>"
//			"<M_TYPE>%d</M_TYPE>"
//			"<M_STATE>%d</M_STATE>"
//			"<T_QTY>%d</T_QTY>"
//			"<U_QTY>%d</U_QTY>"
//			"<R_QTY>%d</R_QTY>"
//			"<N_QTY>%d</N_QTY>"
//			"<A_QTY>%d</A_QTY>"
//			"<E_FLAG>%d</E_FLAG>"
//			"<C_CODE>%s</C_CODE>"
//			"<M_STEP>%s</M_STEP>"
//			"<S_QTY>%d</S_QTY>"
//			"<M_CODE>%s</M_CODE>"
//			"</FEEDER1>"
//			"<FEEDER2>"
//			"<PORTID>%s</PORTID>"
//			"<EQP_STATE>%s</EQP_STATE>"
//			"<PORT_STATE>%s</PORT_STATE>"
//			"<PORT_TYPE>%s</PORT_TYPE>"
//			"<CSTID>%s</CSTID>"
//			"<MAP_STIF>%d</MAP_STIF>"
//			"<CUR_STIF>%d</CUR_STIF>"
//			"<M_BATCHID>%s</M_BATCHID>"
//			"<M_MAKER>%s</M_MAKER>"
//			"<M_KIND>%d</M_KIND>"
//			"<M_TYPE>%d</M_TYPE>"
//			"<M_STATE>%d</M_STATE>"
//			"<T_QTY>%d</T_QTY>"
//			"<U_QTY>%d</U_QTY>"
//			"<R_QTY>%d</R_QTY>"
//			"<N_QTY>%d</N_QTY>"
//			"<A_QTY>%d</A_QTY>"
//			"<E_FLAG>%d</E_FLAG>"
//			"<C_CODE>%s</C_CODE>"
//			"<M_STEP>%s</M_STEP>"
//			"<S_QTY>%d</S_QTY>"
//			"<M_CODE>%s</M_CODE>"
//			"</FEEDER2>"
//			"</BODY>"
//			"</MESSAGE>",
//
		_T("<MESSAGE>")
      		_T("<HEADER>")
			_T("<MSG_ID>UpdateMatInfo</MSG_ID>")
			_T("</HEADER>")
			_T("<BODY>")
			_T("<FEEDER1>")
			_T("<PORTID>%s</PORTID>")
			_T("<EQP_STATE>%s</EQP_STATE>")
			_T("<PORT_STATE>%s</PORT_STATE>")
			_T("<PORT_TYPE>%s</PORT_TYPE>")
			_T("<CSTID>%s</CSTID>")
			_T("<MAP_STIF>%d</MAP_STIF>")
			_T("<CUR_STIF>%d</CUR_STIF>")
			_T("<M_BATCHID>%s</M_BATCHID>")
			_T("<M_MAKER>%s</M_MAKER>")
			_T("<M_KIND>%d</M_KIND>")
			_T("<M_TYPE>%d</M_TYPE>")
			_T("<M_STATE>%d</M_STATE>")
			_T("<T_QTY>%d</T_QTY>")
			_T("<U_QTY>%d</U_QTY>")
			_T("<R_QTY>%d</R_QTY>")
			_T("<N_QTY>%d</N_QTY>")
			_T("<A_QTY>%d</A_QTY>")
			_T("<E_FLAG>%d</E_FLAG>")
			_T("<C_CODE>%s</C_CODE>")
			_T("<M_STEP>%s</M_STEP>")
			_T("<S_QTY>%d</S_QTY>")
			_T("<M_CODE>%s</M_CODE>")
			_T("</FEEDER1>")
			_T("<FEEDER2>")
			_T("<PORTID>%s</PORTID>")
			_T("<EQP_STATE>%s</EQP_STATE>")
			_T("<PORT_STATE>%s</PORT_STATE>")
			_T("<PORT_TYPE>%s</PORT_TYPE>")
			_T("<CSTID>%s</CSTID>")
			_T("<MAP_STIF>%d</MAP_STIF>")
			_T("<CUR_STIF>%d</CUR_STIF>")
			_T("<M_BATCHID>%s</M_BATCHID>")
			_T("<M_MAKER>%s</M_MAKER>")
			_T("<M_KIND>%d</M_KIND>")
			_T("<M_TYPE>%d</M_TYPE>")
			_T("<M_STATE>%d</M_STATE>")
			_T("<T_QTY>%d</T_QTY>")
			_T("<U_QTY>%d</U_QTY>")
			_T("<R_QTY>%d</R_QTY>")
			_T("<N_QTY>%d</N_QTY>")
			_T("<A_QTY>%d</A_QTY>")
			_T("<E_FLAG>%d</E_FLAG>")
			_T("<C_CODE>%s</C_CODE>")
			_T("<M_STEP>%s</M_STEP>")
			_T("<S_QTY>%d</S_QTY>")
			_T("<M_CODE>%s</M_CODE>")
			_T("</FEEDER2>")
			_T("</BODY>")
			_T("</MESSAGE>"),
			m_oMat_Feeder3.m_sPORTID,			
			m_oMat_Feeder3.m_sEQP_STATE,		
			m_oMat_Feeder3.m_sPORT_STATE,		
			m_oMat_Feeder3.m_sPORT_TYPE,		
			m_oMat_Feeder3.m_sCSTID,			
			m_oMat_Feeder3.m_nMAP_STIF,		
			m_oMat_Feeder3.m_nCUR_STIF,		
			m_oMat_Feeder3.m_sM_BATCHID,		
			m_oMat_Feeder3.m_sM_MAKER,			
			m_oMat_Feeder3.m_nM_KIND,			
			m_oMat_Feeder3.m_nM_TYPE,			
			m_oMat_Feeder3.m_nM_STATE,			
			m_oMat_Feeder3.m_nT_QTY,			
			m_oMat_Feeder3.m_nU_QTY,			
			m_oMat_Feeder3.m_nR_QTY,			
			m_oMat_Feeder3.m_nN_QTY,			
			m_oMat_Feeder3.m_nA_QTY,			
			m_oMat_Feeder3.m_nE_FLAG,			
			m_oMat_Feeder3.m_sC_CODE,			
			m_oMat_Feeder3.m_sM_STEP,			
			m_oMat_Feeder3.m_nS_QTY,			
			m_oMat_Feeder3.m_sM_CODE,			
			m_oMat_Feeder4.m_sPORTID,			
			m_oMat_Feeder4.m_sEQP_STATE,		
			m_oMat_Feeder4.m_sPORT_STATE,		
			m_oMat_Feeder4.m_sPORT_TYPE,		
			m_oMat_Feeder4.m_sCSTID,			
			m_oMat_Feeder4.m_nMAP_STIF,		
			m_oMat_Feeder4.m_nCUR_STIF,		
			m_oMat_Feeder4.m_sM_BATCHID,		
			m_oMat_Feeder4.m_sM_MAKER,			
			m_oMat_Feeder4.m_nM_KIND,			
			m_oMat_Feeder4.m_nM_TYPE,			
			m_oMat_Feeder4.m_nM_STATE,			
			m_oMat_Feeder4.m_nT_QTY,			
			m_oMat_Feeder4.m_nU_QTY,			
			m_oMat_Feeder4.m_nR_QTY,			
			m_oMat_Feeder4.m_nN_QTY,			
			m_oMat_Feeder4.m_nA_QTY,			
			m_oMat_Feeder4.m_nE_FLAG,			
			m_oMat_Feeder4.m_sC_CODE,			
			m_oMat_Feeder4.m_sM_STEP,			
			m_oMat_Feeder4.m_nS_QTY,			
			m_oMat_Feeder4.m_sM_CODE);	
	
		/** Data Send */
			SendString(sSndMsg);

	// L/C 재연결시 설비<=>L/C간의 데이터 동기화를 위한 부분.
	/** Data Assign */
	sSndMsg.Format(
//		"<MESSAGE>"
//      		"<HEADER>"
//			"<MSG_ID>UpdateMatInfo</MSG_ID>"
//			"</HEADER>"
//			"<BODY>"
//			"<FEEDER1>"
//			"<PORTID>%s</PORTID>"
//			"<EQP_STATE>%s</EQP_STATE>"
//			"<PORT_STATE>%s</PORT_STATE>"
//			"<PORT_TYPE>%s</PORT_TYPE>"
//			"<CSTID>%s</CSTID>"
//			"<MAP_STIF>%d</MAP_STIF>"
//			"<CUR_STIF>%d</CUR_STIF>"
//			"<M_BATCHID>%s</M_BATCHID>"
//			"<M_MAKER>%s</M_MAKER>"
//			"<M_KIND>%d</M_KIND>"
//			"<M_TYPE>%d</M_TYPE>"
//			"<M_STATE>%d</M_STATE>"
//			"<T_QTY>%d</T_QTY>"
//			"<U_QTY>%d</U_QTY>"
//			"<R_QTY>%d</R_QTY>"
//			"<N_QTY>%d</N_QTY>"
//			"<A_QTY>%d</A_QTY>"
//			"<E_FLAG>%d</E_FLAG>"
//			"<C_CODE>%s</C_CODE>"
//			"<M_STEP>%s</M_STEP>"
//			"<S_QTY>%d</S_QTY>"
//			"<M_CODE>%s</M_CODE>"
//			"</FEEDER1>"
//			"<FEEDER2>"
//			"<PORTID>%s</PORTID>"
//			"<EQP_STATE>%s</EQP_STATE>"
//			"<PORT_STATE>%s</PORT_STATE>"
//			"<PORT_TYPE>%s</PORT_TYPE>"
//			"<CSTID>%s</CSTID>"
//			"<MAP_STIF>%d</MAP_STIF>"
//			"<CUR_STIF>%d</CUR_STIF>"
//			"<M_BATCHID>%s</M_BATCHID>"
//			"<M_MAKER>%s</M_MAKER>"
//			"<M_KIND>%d</M_KIND>"
//			"<M_TYPE>%d</M_TYPE>"
//			"<M_STATE>%d</M_STATE>"
//			"<T_QTY>%d</T_QTY>"
//			"<U_QTY>%d</U_QTY>"
//			"<R_QTY>%d</R_QTY>"
//			"<N_QTY>%d</N_QTY>"
//			"<A_QTY>%d</A_QTY>"
//			"<E_FLAG>%d</E_FLAG>"
//			"<C_CODE>%s</C_CODE>"
//			"<M_STEP>%s</M_STEP>"
//			"<S_QTY>%d</S_QTY>"
//			"<M_CODE>%s</M_CODE>"
//			"</FEEDER2>"
//			"</BODY>"
//			"</MESSAGE>",
//
		_T("<MESSAGE>")
      		_T("<HEADER>")
			_T("<MSG_ID>UpdateMatInfo</MSG_ID>")
			_T("</HEADER>")
			_T("<BODY>")
			_T("<FEEDER1>")
			_T("<PORTID>%s</PORTID>")
			_T("<EQP_STATE>%s</EQP_STATE>")
			_T("<PORT_STATE>%s</PORT_STATE>")
			_T("<PORT_TYPE>%s</PORT_TYPE>")
			_T("<CSTID>%s</CSTID>")
			_T("<MAP_STIF>%d</MAP_STIF>")
			_T("<CUR_STIF>%d</CUR_STIF>")
			_T("<M_BATCHID>%s</M_BATCHID>")
			_T("<M_MAKER>%s</M_MAKER>")
			_T("<M_KIND>%d</M_KIND>")
			_T("<M_TYPE>%d</M_TYPE>")
			_T("<M_STATE>%d</M_STATE>")
			_T("<T_QTY>%d</T_QTY>")
			_T("<U_QTY>%d</U_QTY>")
			_T("<R_QTY>%d</R_QTY>")
			_T("<N_QTY>%d</N_QTY>")
			_T("<A_QTY>%d</A_QTY>")
			_T("<E_FLAG>%d</E_FLAG>")
			_T("<C_CODE>%s</C_CODE>")
			_T("<M_STEP>%s</M_STEP>")
			_T("<S_QTY>%d</S_QTY>")
			_T("<M_CODE>%s</M_CODE>")
			_T("</FEEDER1>")
			_T("<FEEDER2>")
			_T("<PORTID>%s</PORTID>")
			_T("<EQP_STATE>%s</EQP_STATE>")
			_T("<PORT_STATE>%s</PORT_STATE>")
			_T("<PORT_TYPE>%s</PORT_TYPE>")
			_T("<CSTID>%s</CSTID>")
			_T("<MAP_STIF>%d</MAP_STIF>")
			_T("<CUR_STIF>%d</CUR_STIF>")
			_T("<M_BATCHID>%s</M_BATCHID>")
			_T("<M_MAKER>%s</M_MAKER>")
			_T("<M_KIND>%d</M_KIND>")
			_T("<M_TYPE>%d</M_TYPE>")
			_T("<M_STATE>%d</M_STATE>")
			_T("<T_QTY>%d</T_QTY>")
			_T("<U_QTY>%d</U_QTY>")
			_T("<R_QTY>%d</R_QTY>")
			_T("<N_QTY>%d</N_QTY>")
			_T("<A_QTY>%d</A_QTY>")
			_T("<E_FLAG>%d</E_FLAG>")
			_T("<C_CODE>%s</C_CODE>")
			_T("<M_STEP>%s</M_STEP>")
			_T("<S_QTY>%d</S_QTY>")
			_T("<M_CODE>%s</M_CODE>")
			_T("</FEEDER2>")
			_T("</BODY>")
			_T("</MESSAGE>"),
			m_oMat_ZigFeeder1.m_sPORTID,			
			m_oMat_ZigFeeder1.m_sEQP_STATE,		
			m_oMat_ZigFeeder1.m_sPORT_STATE,		
			m_oMat_ZigFeeder1.m_sPORT_TYPE,		
			m_oMat_ZigFeeder1.m_sCSTID,			
			m_oMat_ZigFeeder1.m_nMAP_STIF,		
			m_oMat_ZigFeeder1.m_nCUR_STIF,		
			m_oMat_ZigFeeder1.m_sM_BATCHID,		
			m_oMat_ZigFeeder1.m_sM_MAKER,			
			m_oMat_ZigFeeder1.m_nM_KIND,			
			m_oMat_ZigFeeder1.m_nM_TYPE,			
			m_oMat_ZigFeeder1.m_nM_STATE,			
			m_oMat_ZigFeeder1.m_nT_QTY,			
			m_oMat_ZigFeeder1.m_nU_QTY,			
			m_oMat_ZigFeeder1.m_nR_QTY,			
			m_oMat_ZigFeeder1.m_nN_QTY,			
			m_oMat_ZigFeeder1.m_nA_QTY,			
			m_oMat_ZigFeeder1.m_nE_FLAG,			
			m_oMat_ZigFeeder1.m_sC_CODE,			
			m_oMat_ZigFeeder1.m_sM_STEP,			
			m_oMat_ZigFeeder1.m_nS_QTY,			
			m_oMat_ZigFeeder1.m_sM_CODE,			
			m_oMat_ZigFeeder2.m_sPORTID,			
			m_oMat_ZigFeeder2.m_sEQP_STATE,		
			m_oMat_ZigFeeder2.m_sPORT_STATE,		
			m_oMat_ZigFeeder2.m_sPORT_TYPE,		
			m_oMat_ZigFeeder2.m_sCSTID,			
			m_oMat_ZigFeeder2.m_nMAP_STIF,		
			m_oMat_ZigFeeder2.m_nCUR_STIF,		
			m_oMat_ZigFeeder2.m_sM_BATCHID,		
			m_oMat_ZigFeeder2.m_sM_MAKER,			
			m_oMat_ZigFeeder2.m_nM_KIND,			
			m_oMat_ZigFeeder2.m_nM_TYPE,			
			m_oMat_ZigFeeder2.m_nM_STATE,			
			m_oMat_ZigFeeder2.m_nT_QTY,			
			m_oMat_ZigFeeder2.m_nU_QTY,			
			m_oMat_ZigFeeder2.m_nR_QTY,			
			m_oMat_ZigFeeder2.m_nN_QTY,			
			m_oMat_ZigFeeder2.m_nA_QTY,			
			m_oMat_ZigFeeder2.m_nE_FLAG,			
			m_oMat_ZigFeeder2.m_sC_CODE,			
			m_oMat_ZigFeeder2.m_sM_STEP,			
			m_oMat_ZigFeeder2.m_nS_QTY,			
			m_oMat_ZigFeeder2.m_sM_CODE);	
	
		/** Data Send */
			SendString(sSndMsg);
}

/*========================================================================================
	FUNCTION : MLCNet::GetCurProductPlan()
	DESCRIPT : 현재 진행중인 생산정보를 구한다.
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/01/21, KJH
========================================================================================*/
int  MLCNet::GetCurProductPlan(MProductPlanBatchData* pBI)
{
	int   nRtn = 0;
	
	pBI->SetData(&m_CurProductPlan);
	
	return nRtn;
}

/*========================================================================================
	FUNCTION : MLCNet::GetRcvMaterialInfo()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/01/22, KJH
========================================================================================*/
int  MLCNet::GetRcvMaterialInfo(MMaterial* pMat)
{
	int   nRtn= 0 ;
	
	pMat->CopyData(&m_MaterialData);
	
	return nRtn;
}

/*========================================================================================
	FUNCTION : MLCNet::GetWarningLimitQty_TABIC()
	DESCRIPT : Get current Limit Qty(Material Warning Event)
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/01/22, KJH
========================================================================================*/
int  MLCNet::GetWarningLimitQty_TABIC()
{
	// 2005.02.18 hyun 끊어졌을때를 대비하여 .....
	if ((m_nWarningEvtLimitQty_TABIC <= 0) || (m_nWarningEvtLimitQty_TABIC > 5000))
		return 100; 
	else	
		return m_nWarningEvtLimitQty_TABIC;
}

/*========================================================================================
	FUNCTION : MLCNet::GetPreHandLimitQty_TABIC()
	DESCRIPT : Get current Limit Qty(Material PreHand Event)
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/01/22, KJH
========================================================================================*/
int  MLCNet::GetPreHandLimitQty_TABIC()
{
	if ((m_nPreHandEvtLimitQty_TABIC <= 0) || (m_nPreHandEvtLimitQty_TABIC > 5000))
		return 200; 
	else
		return  m_nPreHandEvtLimitQty_TABIC;
}


/*========================================================================================
	FUNCTION : MLCNet::GetWarningLimitQty_ZIG()
	DESCRIPT : Get current Limit Qty(Material Warning Event)
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/01/22, KJH
========================================================================================*/
int  MLCNet::GetWarningLimitQty_ZIG()
{
	return m_nWarningEvtLimitQty_ZIG;
}

/*========================================================================================
	FUNCTION : MLCNet::GetPreHandLimitQty_ZIG()
	DESCRIPT : Get current Limit Qty(Material PreHand Event)
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/01/22, KJH
========================================================================================*/
int  MLCNet::GetPreHandLimitQty_ZIG()
{
	return  m_nPreHandEvtLimitQty_ZIG;
}

/*========================================================================================
	FUNCTION : MLCNet::SetWarningLimitQty_TABIC()
	DESCRIPT : Set current Limit Qty(Material Warning Event)
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/01/22, KJH
========================================================================================*/
int  MLCNet::SetWarningLimitQty_TABIC(int nQty)
{
	m_nWarningEvtLimitQty_TABIC = nQty;
	return ERR_MNET_SUCCESS;
}

/*========================================================================================
	FUNCTION : MLCNet::SetPreHandLimitQty_TABIC()
	DESCRIPT : set current Limit Qty(Material PreHand Event)
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/01/22, KJH
========================================================================================*/
int  MLCNet::SetPreHandLimitQty_TABIC(int nQty)
{
	m_nPreHandEvtLimitQty_TABIC = nQty;
	return ERR_MNET_SUCCESS;
}


/*========================================================================================
	FUNCTION : MLCNet::SetWarningLimitQty_ZIG()
	DESCRIPT : Set current Limit Qty(Material Warning Event)
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/01/22, KJH
========================================================================================*/
int  MLCNet::SetWarningLimitQty_ZIG(int nQty)
{
	m_nWarningEvtLimitQty_ZIG = nQty;
	return ERR_MNET_SUCCESS;
}

/*========================================================================================
	FUNCTION : MLCNet::SetPreHandLimitQty_ZIG()
	DESCRIPT : Set current Limit Qty(Material PreHand Event)
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/01/22, KJH
========================================================================================*/
int  MLCNet::SetPreHandLimitQty_ZIG(int nQty)
{
	m_nPreHandEvtLimitQty_ZIG = nQty;
	return ERR_MNET_SUCCESS;
}

void MLCNet::ModelDataReport(CString sModelName, 
							 int nAssmbleQty_Feeder1,
							 int nAssmbleQty_Feeder2,
							 CString sFeeder1MaterialType,
							 CString sFeeder2MaterialType)
{
	if ("" != sModelName)
	{
		m_sCurModelName = sModelName;
	}
	if (-1 < nAssmbleQty_Feeder1)
	{
		m_nAssembleQty_Feeder1 = nAssmbleQty_Feeder1;
	}
	if (-1 < nAssmbleQty_Feeder2)
	{
		m_nAssembleQty_Feeder2 = nAssmbleQty_Feeder2;
	}
	if ("" != sFeeder1MaterialType)
	{
		m_sFeeder1MaterialType = sFeeder1MaterialType;
	}
	if ("" != sFeeder2MaterialType)
	{
		m_sFeeder2MaterialType = sFeeder2MaterialType;
	}
	
	CString   sSndMsg = "";
	
	/** Data Assign */
	sSndMsg.Format(
//		"<MESSAGE>"
//		"<HEADER>"
//      		"<MSG_ID>NewModelData</MSG_ID>"
//			"</HEADER>"
//			"<BODY>"
//			"<CURRENT_MODEL_NAME>%s</CURRENT_MODEL_NAME>"
//			"<ASSEMBLE_QTY_FEEDER1>%d</ASSEMBLE_QTY_FEEDER1>"
//			"<ASSEMBLE_QTY_FEEDER2>%d</ASSEMBLE_QTY_FEEDER2>"
//			"<MAT_TYPE_FEEDER1>%s</MAT_TYPE_FEEDER1>"
//			"<MAT_TYPE_FEEDER2>%s</MAT_TYPE_FEEDER2>"
//			"</BODY>"
//			"</MESSAGE>",
//
		_T("<MESSAGE>")
		_T("<HEADER>")
      		_T("<MSG_ID>NewModelData</MSG_ID>")
			_T("</HEADER>")
			_T("<BODY>")
			_T("<CURRENT_MODEL_NAME>%s</CURRENT_MODEL_NAME>")
			_T("<ASSEMBLE_QTY_FEEDER1>%d</ASSEMBLE_QTY_FEEDER1>")
			_T("<ASSEMBLE_QTY_FEEDER2>%d</ASSEMBLE_QTY_FEEDER2>")
			_T("<MAT_TYPE_FEEDER1>%s</MAT_TYPE_FEEDER1>")
			_T("<MAT_TYPE_FEEDER2>%s</MAT_TYPE_FEEDER2>")
			_T("</BODY>")
			_T("</MESSAGE>"),
			m_sCurModelName, m_nAssembleQty_Feeder1, m_nAssembleQty_Feeder2,
			m_sFeeder1MaterialType, m_sFeeder2MaterialType);
	
	/** Data Send */
	SendString(sSndMsg);
}

void MLCNet::ModelDataReport(CString sModelName)
{
	if (""!=sModelName)
	{
		m_sCurModelName = sModelName;
	}
	
	CString   sSndMsg="";
	
	/** Data Assign */
	sSndMsg.Format(
//		"<MESSAGE>"
//		"<HEADER>"
//    		"<MSG_ID>NewModelData</MSG_ID>"
//			"</HEADER>"
//			"<BODY>"
//			"<CURRENT_MODEL_NAME>%s</CURRENT_MODEL_NAME>"
//			"</BODY>"
//			"</MESSAGE>",
		_T("<MESSAGE>")
		_T("<HEADER>")
      		_T("<MSG_ID>NewModelData</MSG_ID>")
			_T("</HEADER>")
			_T("<BODY>")
			_T("<CURRENT_MODEL_NAME>%s</CURRENT_MODEL_NAME>")
			_T("</BODY>")
			_T("</MESSAGE>"),
			m_sCurModelName);
	
	/** Data Send */
	SendString(sSndMsg);
}

/*========================================================================================
	FUNCTION : MLCNet::Split()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT : 
	(i) sStrLine: 문자열
	(i) chSep: 구분자(ex:,)
	(o) asItem: String Array
	UPDATE	 : 2004/6/1 KJH
========================================================================================*/
BOOL  MLCNet::Split(CString sStrLine, TCHAR chSep, CStringArray &asItem)
{
	int			nIndex=0;
	CString		sStr="";
	
	if (sStrLine.IsEmpty()) 
	{
		return false; 
	}
	
	AfxExtractSubString(sStr, sStrLine, nIndex++, chSep); 
	while (!sStr.IsEmpty())
	{
		sStr.Remove('"');
		asItem.Add(sStr); 
		AfxExtractSubString(sStr, sStrLine, nIndex++, chSep); 
	}
	
	return true;
}

/*========================================================================================
	FUNCTION : MLCNet::IsNoParsingInfoMaker()
	DESCRIPT : TABIC 업체중에서 Barcode에 수량정보가 없는 경우가 있어서 수동으로 입력받을지 
	여부를 판단한다.             
	RETURN	 : 
	ARGUMENT : 
	UPDATE	 : 2005/03/23 KJH
========================================================================================*/
BOOL  MLCNet::IsNoParsingInfoMaker(CString sMaker)
{
	int			i = 0;
	bool    bRtn=false;
	
	for (int i = 0; i < m_srgNoParsingInfoMaker.GetSize(); i++)
	{
		if (sMaker == m_srgNoParsingInfoMaker.GetAt(i))
		{ 
			bRtn = true; 
			break;
		}
	}
	
	return bRtn;
}

/*========================================================================================
	FUNCTION : MLCNet::UpdateLCDateTime()
	DESCRIPT : LC와 연결이 되면 LC에서 날짜&시간을 받아서 동기화한다.
	RETURN	 : 
	ARGUMENT : 
	UPDATE	 : 2007/11/07 GrouchyKim
========================================================================================*/
void  MLCNet::UpdateLCDateTime()
{
	char	szMsgii[5];
	CString sTime="";
	SYSTEMTIME	tLocalTime;	
	int		nRtn=0;
	
	//2007.11.07 GrouchyKim Modify.[v1.4]
	//if (true==m_fUpdateLCDateTime || 0==m_saLCDateTime.GetSize()) { return; }
	if (0==m_saLCDateTime.GetSize()) { return; }
	
	sTime = m_saLCDateTime.GetAt(0);
	m_saLCDateTime.RemoveAll();
	if (14!=sTime.GetLength()){ return; }

	memset(szMsgii, 0, sizeof(char)*5);
	sprintf(szMsgii, "%s", LPSTR(LPCTSTR(sTime.Mid(0, 4))));	tLocalTime.wYear   = (WORD)atoi(szMsgii);
	sprintf(szMsgii, "%s", LPSTR(LPCTSTR(sTime.Mid(4, 2))));	tLocalTime.wMonth  = (WORD)atoi(szMsgii);
	sprintf(szMsgii, "%s", LPSTR(LPCTSTR(sTime.Mid(6, 2))));	tLocalTime.wDay    = (WORD)atoi(szMsgii);
	sprintf(szMsgii, "%s", LPSTR(LPCTSTR(sTime.Mid(8, 2))));	tLocalTime.wHour   = (WORD)atoi(szMsgii);
	sprintf(szMsgii, "%s", LPSTR(LPCTSTR(sTime.Mid(10, 2))));	tLocalTime.wMinute = (WORD)atoi(szMsgii);
	sprintf(szMsgii, "%s", LPSTR(LPCTSTR(sTime.Mid(12, 2))));	tLocalTime.wSecond = (WORD)atoi(szMsgii);
	tLocalTime.wDayOfWeek = 0;		tLocalTime.wMilliseconds = 0;
	
//	::SetLocalTime(&tLocalTime);
	nRtn = SetLocalTime(&tLocalTime);

	//2007.11.07 GrouchyKim Modify.[v1.4]
	//m_fUpdateLCDateTime = true;
}

//NSMC KJS
// Event ID
// "53" : Job Change Reserve
// "54" : Job Change Start
// "55" : Job Change End
// "56" : Job Change Ng
// EQ State
// "0" : Equipment Nothing
// "1" : Normal  
// "2" : Fault  
// "3" : PM
// EQ Process State
// "0" : Equipment Nothing
// "1" : Init 
// "2" : Idle	
// "3" : Setup
// "4" : Ready
// "5" : Executing
// "6" : Pause
// #Reason Code 
// 
// Message Sample
// - Equipment Job Change "Reserve" 일때.
// ☞ @UJ##53##1##2##2001$
// - Equipment Job Change "Start" 일때.
// ☞ @UJ##54##1##2##1234$
// - Equipment Job Change "End" 일때.
// ☞ @UJ##55##1##2##1234$
// - Equipment Job Change DOWN으로 인해 Job Change 안될 경우.
// ☞ @UJ##56##1##2##DOWN$
long MLCNet::ModelChangeReport(EMCState eSts, CString strReasonCode)
{
	CString sMsg;
	long	lRet;
	
	sMsg.Format(_T("@UJ##%d##%d##%d##%s$"), eSts, m_eEqState, m_eEqProcState, strReasonCode);
	
	/** Data Send */
	lRet = SendString(sMsg);
	
	return lRet;
}


//110620.KKY.Add_____
//설비에서 생산계획을 L/C에 요청한다...
long MLCNet::ProductPlanRequest(EProductPlanType ePlanType, MPlanRequestData *pPR)
{
	CString sMsg, sTemp;
	long	lRet;
	
	/** Data Assign */
//	sMsg.Format(_T("@PR##%d##%d"), m_nUnit, ePlanType);
	sMsg.Format(_T("@PR##%s##%d"), m_strReportID, ePlanType);
	sTemp.Format(_T("##%s##%s##%s##%s##%d##%s$"),
		pPR->m_sLINE_NO,
		pPR->m_sEQPID,
		pPR->m_sCarrierID,
		pPR->m_sDeviceID,
		pPR->m_nM_KIND,
		pPR->m_sM_STEP
		);
	sMsg += sTemp;

	/** Data Send */
	lRet = SendString(sMsg);
	
	return lRet;
}

///설비에서 생산 계획의 시작과 종료를 L/C에 보고한다...
long MLCNet::ProductPlanReport(EProductPlanEvent ePlanEvent, MProductPlanBatchData *pBI)
{
	CString sMsg, sTemp;
	long	lRet;
	
	/** Data Assign */
	if (ePlanEvent == eProductPlan46CodeUpdate)
//		sMsg.Format(_T("@PR##%d##%d"), m_nUnit, ePlanEvent);
		sMsg.Format(_T("@PR##%s##%d"), m_strReportID, ePlanEvent);
	else
//		sMsg.Format(_T("@PE##%d##%d"), m_nUnit, ePlanEvent);
		sMsg.Format(_T("@PE##%s##%d"), m_strReportID, ePlanEvent);

	sTemp.Format(
		_T("##%d##%d##%s##%s##%s##%s##%s")
		_T("##%s##%d##%d##%s##%d##%s##%s")
		_T("##%s##%d##%d##%d##%d##%d$"),
		pBI->m_nPlanOrderID,	
		pBI->m_nPriorty,
		pBI->m_sProdKind,
		pBI->m_sProdType,
		pBI->m_sDeviceID,
		pBI->m_sStepID,
		pBI->m_sPPID,
		pBI->m_sBatchID,
		pBI->m_nBatchSize,
		pBI->m_nBatchState,
		pBI->m_sPanelMaker,
		pBI->m_nPanelThickness,
		pBI->m_sFirstGlassID,
		pBI->m_sCurGlassID,
		pBI->m_sInputLineNo,
		pBI->m_nValidFlag,
		pBI->m_nCQTY,
		pBI->m_nOQTY,
		pBI->m_nRQTY,
		pBI->m_nNQTY
		);
	sMsg += sTemp;

	/** Data Send */
	lRet = SendString(sMsg);
	
	return lRet;
}

/** 
 *	Product Code Report : Unit#1 - Vericode, Unit#2 - Barcode 
 *	
 */
long MLCNet::ProductCodeReport(EPanelEvent eCodeType, EProductCodeState eCodeState, MProductCodeData *pPC)
{
	CString sMsg;
	long	lRet;
	int		iUnitID = m_nUnit;

	if (eCodeType == ePanelVcrRead)
		iUnitID = m_nUnit - 1;  // Loader VCR 인식 보고
	else
		iUnitID = m_nUnit;  // Loabeller Panel 투입보고

//	sMsg.Format(_T("@VR##%d##%d##%s##%d##%d##%s$"),	iUnitID, eCodeState, pPC->m_strCSTID, pPC->m_iCSTTYPE, pPC->m_iReq_Qty, pPC->m_strPanelID);
	sMsg.Format(_T("@VR##%s##%d##%s##%d##%d##%s$"),	m_strReportID, eCodeState, pPC->m_strCSTID, pPC->m_iCSTTYPE, pPC->m_iReq_Qty, pPC->m_strPanelID);
	
	/** Data Send */
	lRet = SendString(sMsg);
	
	return lRet;
}

//110629.KKY_____
void MLCNet::Lock()
{
	m_csLock.Lock();
}

void MLCNet::Unlock()
{
	m_csLock.Unlock();
}

void MLCNet::ManageNextProductPlanData()
{
	int size = m_caNextProductPlan.GetSize();
	if (size > 5)
	{
		MProductPlanBatchData* pFirst = (MProductPlanBatchData*)m_caNextProductPlan.GetAt(0);
		m_caNextProductPlan.RemoveAt(0);
		delete pFirst;
		pFirst = NULL;
	}
}

void MLCNet::ManageEndProductPlanData()
{
	int size = m_caEndProductPlan.GetSize();
	if (size > 5)
	{
		MProductPlanBatchData* pFirst = (MProductPlanBatchData*)m_caEndProductPlan.GetAt(0);
		m_caEndProductPlan.RemoveAt(0);
		delete pFirst;
		pFirst = NULL;
	}
}

void MLCNet::TransferCurToEndProductPlanData()
{
	// 현재 생산 계획 종료
	m_CurProductPlan.m_nBatchState = eProductPlanEnd;
	MProductPlanBatchData* p = new MProductPlanBatchData;
	p->SetData(&m_CurProductPlan);
	m_EndProductPlan.SetData(&m_CurProductPlan);
	m_caEndProductPlan.Add(p);
	ManageEndProductPlanData();
	m_CurProductPlan.Clear();

#ifdef DEF_PROCESS_MC_FIRST	//투입설비일 경우 BatchEnd보고한다
	ProductPlanReport(eProductPlanBatchEnd, p);
#endif
}

void MLCNet::TransferNextToCurProductPlanData()
{
	// 다음 생산 계획을 현재 생산 계획으로 Update
	m_NextProductPlan.Clear();
	int iNextCnt = m_caNextProductPlan.GetSize();
	if (iNextCnt > 0)
	{
		MProductPlanBatchData* p = (MProductPlanBatchData*)m_caNextProductPlan.GetAt(0);
		m_CurProductPlan.SetData(p);
		m_caNextProductPlan.RemoveAt(0);
		delete p;
		if (iNextCnt > 1)
			m_NextProductPlan.SetData((MProductPlanBatchData*)m_caNextProductPlan.GetAt(0));
	}
}

void MLCNet::TrsnaferCurMCodeToEndMCode()
{
	m_EndMaterialCodeInfo_S.CopyData(&m_CurMaterialCodeInfo_S);
	m_EndMaterialCodeInfo_G.CopyData(&m_CurMaterialCodeInfo_G);
	m_CurMaterialCodeInfo_S.ResetData();
	m_CurMaterialCodeInfo_G.ResetData();
}
void MLCNet::TrsnaferCurMCodeToEndMCodeJIG()
{	
	m_EndMaterialCodeInfo_ZS.CopyData(&m_CurMaterialCodeInfo_ZS);
	m_EndMaterialCodeInfo_ZG.CopyData(&m_CurMaterialCodeInfo_ZG);
	m_CurMaterialCodeInfo_ZS.ResetData();
	m_CurMaterialCodeInfo_ZG.ResetData();
}
void MLCNet::UpdateCurrentMCodePlan(MMaterialCodeInfo* pInfo)
{
	//DEF_MCODE_STEP_S
	//DEF_MCODE_STEP_G

	MMaterialCodeInfo* pCurMInfo;
	MMaterialCodeInfo* pCurMInfo2;
#ifdef DEF_SOURCE_SYSTEM
	if(pInfo->m_nM_KIND == eMatKind_TABIC)
	{
		pCurMInfo = &m_CurMaterialCodeInfo_S;
		pCurMInfo2 = &m_CurMaterialCodeInfo_G;
	}
	else if(pInfo->m_nM_KIND == eMatKind_JIG)
	{
		pCurMInfo = &m_CurMaterialCodeInfo_ZS;
		pCurMInfo2 = &m_CurMaterialCodeInfo_ZG;
	}
#else
	if(pInfo->m_nM_KIND == eMatKind_TABIC)
	{
		pCurMInfo = &m_CurMaterialCodeInfo_G;
		pCurMInfo2 = &m_CurMaterialCodeInfo_S;
	}
	else if(pInfo->m_nM_KIND == eMatKind_JIG)
	{
		pCurMInfo = &m_CurMaterialCodeInfo_ZG;
		pCurMInfo2 = &m_CurMaterialCodeInfo_ZS;
	}
#endif

	pCurMInfo->ResetData();
	pCurMInfo2->ResetData();
	
	pCurMInfo->m_sDeviceID = pCurMInfo2->m_sDeviceID = pInfo->m_sDeviceID;
	pCurMInfo->m_nWorkOrder = pCurMInfo2->m_nWorkOrder = pInfo->m_nWorkOrder;

	pCurMInfo->CopyData(pInfo);
}

void MLCNet::UpdateNextMCodePlan(MMaterialCodeInfo* pInfo)
{
#ifdef DEF_SOURCE_SYSTEM
	MMaterialCodeInfo* pNextMInfo = &m_NextMaterialCodeInfo_S;
	MMaterialCodeInfo* pNextMInfo2 = &m_NextMaterialCodeInfo_G;
#else
	MMaterialCodeInfo* pNextMInfo = &m_NextMaterialCodeInfo_G;
	MMaterialCodeInfo* pNextMInfo2 = &m_NextMaterialCodeInfo_S;
#endif

	pNextMInfo->ResetData();
	pNextMInfo2->ResetData();
	
	pNextMInfo->m_sDeviceID = pNextMInfo2->m_sDeviceID = pInfo->m_sDeviceID;
	pNextMInfo->m_nWorkOrder = pNextMInfo2->m_nWorkOrder = pInfo->m_nWorkOrder;

	pNextMInfo->CopyData(pInfo);
}

void MLCNet::SetUnitReportID(CString ReportID)
{
	m_strReportID = ReportID;
}

//_______________
