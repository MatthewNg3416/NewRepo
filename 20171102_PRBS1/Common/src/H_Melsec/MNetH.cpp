// MNetH.cpp: implementation of the MNetH class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "MapiUtil.h" //171228 JSH
#include "MNetH.h"
#include "MNetHDlg.h"
#include "Mdfunc.h"
#include "DefSystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MNetH::MNetH(CString sIniFile)
{
	//	Create Mutex
	m_hMutex = ::CreateMutex(NULL, FALSE, NULL);

	//	Initial Variable for XNet
	m_nChannel = DEFAULT_CHANNEL;
	m_nStation = DEFAULT_STATION;
	m_lPath = 0;
	m_fActive = FALSE;
	m_sIniFile = sIniFile;

	m_nCurLocal = 1;
	m_nCurUnit = 1;
	m_nPrevLocal = m_nCurLocal-1;
	m_nNextLocal = m_nCurLocal+1;
	m_nToUpperEqQty = 1;
	m_nToLowerEqQty = 1;
	
	m_bSetGlassDataFlag = false;

	m_dwTransferStart = 0;

	//	Common Variable for XSECNetDlg
	m_pDlg = NULL;		
	m_pMainWnd = NULL;	
	m_bShow = false;	

	//	Reading configuration file
	ReadConfig();

	//Prev Dialog Delete
	if (m_pDlg){ m_pDlg->DestroyWindow(); delete m_pDlg; m_pDlg=NULL; }

	if (m_bUseDialog){
		//Dialog Create
		m_pDlg = new MNetHDlg;
		m_pDlg->m_pMNetH = this;
		m_pDlg->Create(IDD_MNETH);

		//Dialog Visibled
		if (m_bShow){ m_pDlg->ShowWindow(SW_SHOW);	}
		else { m_pDlg->ShowWindow(SW_HIDE); }
	}

	InitializeCriticalSection(&m_csMelsec); //SJ_YYK 160222 Add..
	Start();
}

MNetH::~MNetH()
{
	DeleteCriticalSection(&m_csMelsec); //SJ_YYK 160222 Add..
	MelsecClose();
	::CloseHandle(m_hMutex);
}

MNetHDlg** MNetH::GetMNetHDlg()
{
	return &m_pDlg;
}

void MNetH::DestroyDlg()
{
	if (m_pDlg)
	{
		m_pDlg->MonitoringStop();
		
		if (m_pDlg->IsWindowVisible())
			m_pDlg->ShowWindow(SW_HIDE);

		m_pDlg->DestroyWindow();
		delete m_pDlg;
		m_pDlg = NULL;
	}
}

/*========================================================================================
	FUNCTION : MNetH::ViewVisible()
	DESCRIPT : Visible On/Off for dialog
	RETURN	 : none
	ARGUMENT :
		NONE
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
void MNetH::ViewVisible(bool bView)
{
	if (!m_bUseDialog){ return; }

	m_bShow = bView;
	if (bView) 
	{
		if (m_pDlg)
		{
			m_pDlg->ShowWindow(SW_SHOW);
			m_pDlg->SetFocus();
		}
	}
	else
	{
		if (m_pDlg) 
			m_pDlg->ShowWindow(SW_HIDE);
	}
}

/*========================================================================================
	FUNCTION : MNetH::DecToHex()
	DESCRIPT : Dec to Hex
	RETURN	 : long
	ARGUMENT :
		None
	UPDATE	 : 2004/11/26, James Park; First work!
========================================================================================*/
long MNetH::DecToHex(long lDec, CString& sHex)
{
	long lRet=0;

	sHex = "";
	sHex.Format(_T("%X"), lDec);

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::DecToHex()
	DESCRIPT : Dec(String) to Hex
	RETURN	 : long
	ARGUMENT :
		None
	UPDATE	 : 2004/11/26, James Park; First work!
========================================================================================*/
long MNetH::DecToHex(CString sDec, CString& sHex)
{
	long lRet=0;
	long lValue=0;
	WCHAR   *ptr;
 
	sHex = "";

	//lValue = strtol(sDec, &ptr, 10);
	lValue = wcstol(sDec, &ptr, 10);
	sHex.Format(_T("%X"), lValue);

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::HexToDec()
	DESCRIPT : Hex to Dec
	RETURN	 : long
	ARGUMENT :
		None
	UPDATE	 : 2004/11/26, James Park; First work!
========================================================================================*/
long MNetH::HexToDec(CString sHex, long& lDec)
{
	long	lRet=0;
	WCHAR   *ptr;
	
	//lDec = strtol(sHex, &ptr, 16);
	lDec = wcstol(sHex, &ptr, 16);

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::HexToDec()
	DESCRIPT : Hex to Dec(String)
	RETURN	 : long
	ARGUMENT :
		None
	UPDATE	 : 2004/11/26, James Park; First work!
========================================================================================*/
long MNetH::HexToDec(CString sHex, CString& sDec)
{
	long	lRet=0;
	long	lValue;
	WCHAR   *ptr;
	
	//lValue = strtol(sHex, &ptr, 16);
	lValue = wcstol(sHex, &ptr, 16);
	sDec.Format(_T("%d"),lValue);

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::HexToBin()
	DESCRIPT : Hex to Binary string
	RETURN	 : long
	ARGUMENT :
		None
	UPDATE	 : 2004/11/18, James Park; First work!
========================================================================================*/
long MNetH::HexToBin(CString sHex, CString& sBin)
{
	long lRet=0;
	long n;
	char   *str, *ptr;
	CString	sTmp;

	str = new char[2];
	memset(str, NULL, sizeof(char)*2);
	for (int i=0; i < sHex.GetLength(); i++)
	{
		memcpy(str, sHex.Mid(i,1), 1);
		n = strtol(str, &ptr, 16);
		for (int j=0; j < 4; j++)
		{
			itoa(n & 0x1, str, 10);
			sTmp = sTmp + str;
			n = n>>1;
		}
		sTmp.MakeReverse();
		sBin = sBin + sTmp;
		sTmp = "";
	}

	delete [] str; str = NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::BinToHex()
	DESCRIPT : Binary to Hex string
	RETURN	 : long
	ARGUMENT :
		None
	UPDATE	 : 2004/11/18, James Park; First work!
========================================================================================*/
long MNetH::BinToHex(CString sBin, CString& sHex)
{
	long lRet=0;
	long n;
	char   *str, *ptr;
	CString	sTmp;

	str = new char[5];
	memset(str, NULL, sizeof(char)*5);
	for (int i=0; i < sBin.GetLength(); i=i+4)
	{
		memcpy(str, sBin.Mid(i,4), 4);
		n = strtol(str, &ptr, 2);
		sTmp.Format(_T("%X"), n);
		sHex = sHex + sTmp;
	}
	delete [] str; str = NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::Start()
	DESCRIPT : Start for xnet
	RETURN	 : long
	ARGUMENT :
		None
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::Start()
{
	long	lRet;

	lRet = MelsecOpen();
	if (m_pDlg && !lRet)
	{
		m_pDlg->MonitoringStart(); 
	}
	return lRet;




}

/*========================================================================================
	FUNCTION : MNetH::Stop()
	DESCRIPT : Stop for xnet
	RETURN	 : long
	ARGUMENT :
		NONE
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::Stop()
{
	long	lRet;

	if (m_pDlg)
	{
		m_pDlg->MonitoringStop(); 
	}
	lRet = MelsecClose();
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::IsConnected()
	DESCRIPT : Connection State
	RETURN	 : TRUE/FALSE
	ARGUMENT :
		NONE
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
BOOL MNetH::IsConnected()
{
	return m_fActive;
}	

/*========================================================================================
	FUNCTION : MNetH::ReadLB()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/07/04, KJH
========================================================================================*/
long MNetH::ReadLB(short nAddr, short nPoints, short *pnRBuf, short nBufSize)
{
	if (m_fActive == FALSE) { return -1; }
	// TODO: Add your dispatch handler code here
	short	nDev[4];
	short	nRet = 0;
	short	nCount = 0;
	short	nIndex = 0;
	unsigned short	nDigit = 0x0001;
	short	i = 0;

	nCount = nBufSize / sizeof(short);
	if (nCount < nPoints) { return -10001; }

	short	*pnBuf = new short[nCount];
	for (int i = 0; i < nCount; i++) { pnBuf[i] = 0; }

	nDev[0] = 1;
	nDev[1] = DevB;
	nDev[2] = nAddr;
	nDev[3] = nPoints;

	if (::WaitForSingleObject(m_hMutex, 1000) != WAIT_OBJECT_0) return -10002;
	nRet = mdRandR(m_lPath, m_nStation, nDev, pnBuf, nBufSize);
	for (int i = 0; i < nCount; i++) {
		pnRBuf[i] = (pnBuf[nIndex] & nDigit) ? 1 : 0;

		if (nDigit != 0x8000) { nDigit <<= 1; }
		else {
			nDigit = 0x0001;
			nIndex++;
		}
	}
	delete [] pnBuf;
	::ReleaseMutex(m_hMutex);
	return nRet;
}

/*========================================================================================
	FUNCTION : MNetH::ReadLW()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/07/04, KJH
========================================================================================*/
long MNetH::ReadLW(short nAddr, short nPoints, short *pnRBuf, short nBufSize)
{
	if (m_fActive == FALSE) { return -1; }
	// TODO: Add your dispatch handler code here
	short	nDev[4];
	short	nRet = 0;
	short	nCount = 0;

	nCount = nBufSize / sizeof(short);
	if (nCount < nPoints) { return -10001; }

	nDev[0] = 1;
	nDev[1] = DevW;
	nDev[2] = nAddr;
	nDev[3] = nPoints;

	if (::WaitForSingleObject(m_hMutex, 1000) != WAIT_OBJECT_0) return -10002;
	nRet = mdRandR(m_lPath, m_nStation, nDev, pnRBuf, nBufSize);
	::ReleaseMutex(m_hMutex);
	return nRet;
}	

/*========================================================================================
	FUNCTION : MNetH::GetPLCAddressBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2005/12/19, Grouchy
========================================================================================*/
//SJ_YYK 160225 Modify..
//short MNetH::GetPLCAddressBit(int nLocal, int nUnit, short *pnAddr, short *pnPoints, bool bUpperEq)
short MNetH::GetPLCAddressBit(int nLocal, int nUnit, short *pnAddr, short *pnPoints, bool bUpperEq, BOOL bAOCBit)
{
	short	nRet=0;

	if (-1==nLocal)
		nLocal=m_nCurLocal;
	if (-1==nUnit)
		nUnit=m_nCurUnit;

	//SJ_YYK 160225 Add..
	if(bAOCBit == TRUE)
	{
		if (1==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_M01_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_M01_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (2==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_M02_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_M02_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (3==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_M03_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_M03_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (4==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_M04_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_M04_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (5==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_M05_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_M05_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
	}
	else
	{
		if (1==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L01_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L01_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (2==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L02_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L02_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (3==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L03_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L03_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (4==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L04_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L04_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (5==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L05_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L05_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (6==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L06_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L06_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (7==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L07_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L07_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (8==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L08_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L08_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (9==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L09_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L09_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (10==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L10_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L10_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (11==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L11_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L11_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (12==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L12_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L12_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (13==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L13_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L13_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (14==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L14_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L14_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (15==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L15_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L15_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (16==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L16_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L16_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (17==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L17_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L17_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (18==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L18_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L18_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (19==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L19_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L19_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (20==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L20_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L20_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (21==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L21_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L21_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (22==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L22_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L22_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (23==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L23_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L23_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (24==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L24_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L24_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
		else if (25==nLocal){
			if (true==bUpperEq){
				*pnAddr = B_L25_U1 + (B_L_SIZE * (nUnit-1));
			}
			else{
				*pnAddr = B_L25_L1 + (B_L_SIZE * (nUnit-1));
			}
		}
	}


	*pnPoints = B_L_SIZE;

	return nRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCAddressWord_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2005/12/19, Grouchy
========================================================================================*/
short MNetH::GetPLCAddressWord_EO(short *pnAddr, short *pnPoints, int nLocal)
{
	short	nRet=0;

	if (-1==nLocal){ nLocal=m_nCurLocal; }

	if (1==nLocal){ *pnAddr=W_L01_EO; }
	else if (2==nLocal){ *pnAddr=W_L02_EO; }
	else if (3==nLocal){ *pnAddr=W_L03_EO; }
	else if (4==nLocal){ *pnAddr=W_L04_EO; }
	else if (5==nLocal){ *pnAddr=W_L05_EO; }
	else if (6==nLocal){ *pnAddr=W_L06_EO; }
	else if (7==nLocal){ *pnAddr=W_L07_EO; }
	else if (8==nLocal){ *pnAddr=W_L08_EO; }
	else if (9==nLocal){ *pnAddr=W_L09_EO; }
	else if (10==nLocal){ *pnAddr=W_L10_EO; }
	else if (11==nLocal){ *pnAddr=W_L11_EO; }
	else if (12==nLocal){ *pnAddr=W_L12_EO; }
	else if (13==nLocal){ *pnAddr=W_L13_EO; }
	else if (14==nLocal){ *pnAddr=W_L14_EO; }
	else if (15==nLocal){ *pnAddr=W_L15_EO; }
	else if (16==nLocal){ *pnAddr=W_L16_EO; }
	else if (17==nLocal){ *pnAddr=W_L17_EO; }
	else if (18==nLocal){ *pnAddr=W_L18_EO; }
	else if (19==nLocal){ *pnAddr=W_L19_EO; }
	else if (20==nLocal){ *pnAddr=W_L20_EO; }
	else if (21==nLocal){ *pnAddr=W_L21_EO; }
	else if (22==nLocal){ *pnAddr=W_L22_EO; }
	else if (23==nLocal){ *pnAddr=W_L23_EO; }
	else if (24==nLocal){ *pnAddr=W_L24_EO; }
	else if (25==nLocal){ *pnAddr=W_L25_EO; }
	else{ *pnAddr=W_L01_EO; }
	*pnPoints = W_EO_SIZE;

	return nRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCAddressWord_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2005/12/19, Grouchy
========================================================================================*/
short MNetH::GetPLCAddressWord_ER(short *pnAddr, short *pnPoints, int nLocal)
{
	short	nRet=0;

	if (-1==nLocal){ nLocal=m_nCurLocal; }

	if (1==nLocal){ *pnAddr=W_L01_ER; }
	else if (2==nLocal){ *pnAddr=W_L02_ER; }
	else if (3==nLocal){ *pnAddr=W_L03_ER; }
	else if (4==nLocal){ *pnAddr=W_L04_ER; }
	else if (5==nLocal){ *pnAddr=W_L05_ER; }
	else if (6==nLocal){ *pnAddr=W_L06_ER; }
	else if (7==nLocal){ *pnAddr=W_L07_ER; }
	else if (8==nLocal){ *pnAddr=W_L08_ER; }
	else if (9==nLocal){ *pnAddr=W_L09_ER; }
	else if (10==nLocal){ *pnAddr=W_L10_ER; }
	else if (11==nLocal){ *pnAddr=W_L11_ER; }
	else if (12==nLocal){ *pnAddr=W_L12_ER; }
	else if (13==nLocal){ *pnAddr=W_L13_ER; }
	else if (14==nLocal){ *pnAddr=W_L14_ER; }
	else if (15==nLocal){ *pnAddr=W_L15_ER; }
	else if (16==nLocal){ *pnAddr=W_L16_ER; }
	else if (17==nLocal){ *pnAddr=W_L17_ER; }
	else if (18==nLocal){ *pnAddr=W_L18_ER; }
	else if (19==nLocal){ *pnAddr=W_L19_ER; }
	else if (20==nLocal){ *pnAddr=W_L20_ER; }
	else if (21==nLocal){ *pnAddr=W_L21_ER; }
	else if (22==nLocal){ *pnAddr=W_L22_ER; }
	else if (23==nLocal){ *pnAddr=W_L23_ER; }
	else if (24==nLocal){ *pnAddr=W_L24_ER; }
	else if (25==nLocal){ *pnAddr=W_L25_ER; }
	else{ *pnAddr=W_L01_ER; }
	*pnPoints = W_ER_SIZE;

	return nRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCAddressWord_GS()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2005/12/19, Grouchy
========================================================================================*/
short MNetH::GetPLCAddressWord_GS(short *pnAddr, short *pnPoints, int nLocal)
{
	short	nRet=0;

	if (-1==nLocal){ nLocal=m_nCurLocal; }

	if (1==nLocal){ *pnAddr=W_L01_GS; }
	else if (2==nLocal){ *pnAddr=W_L02_GS; }
	else if (3==nLocal){ *pnAddr=W_L03_GS; }
	else if (4==nLocal){ *pnAddr=W_L04_GS; }
	else if (5==nLocal){ *pnAddr=W_L05_GS; }
	else if (6==nLocal){ *pnAddr=W_L06_GS; }
	else if (7==nLocal){ *pnAddr=W_L07_GS; }
	else if (8==nLocal){ *pnAddr=W_L08_GS; }
	else if (9==nLocal){ *pnAddr=W_L09_GS; }
	else if (10==nLocal){ *pnAddr=W_L10_GS; }
	else if (11==nLocal){ *pnAddr=W_L11_GS; }
	else if (12==nLocal){ *pnAddr=W_L12_GS; }
	else if (13==nLocal){ *pnAddr=W_L13_GS; }
	else if (14==nLocal){ *pnAddr=W_L14_GS; }
	else if (15==nLocal){ *pnAddr=W_L15_GS; }
	else if (16==nLocal){ *pnAddr=W_L16_GS; }
	else if (17==nLocal){ *pnAddr=W_L17_GS; }
	else if (18==nLocal){ *pnAddr=W_L18_GS; }
	else if (19==nLocal){ *pnAddr=W_L19_GS; }
	else if (20==nLocal){ *pnAddr=W_L20_GS; }
	else if (21==nLocal){ *pnAddr=W_L21_GS; }
	else if (22==nLocal){ *pnAddr=W_L22_GS; }
	else if (23==nLocal){ *pnAddr=W_L23_GS; }
	else if (24==nLocal){ *pnAddr=W_L24_GS; }
	else if (25==nLocal){ *pnAddr=W_L25_GS; }
	else{ *pnAddr=W_L01_GS; }
	*pnPoints = W_GS_SIZE;

	return nRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCAddressWord_Unit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2005/12/19, Grouchy
========================================================================================*/
short MNetH::GetPLCAddressWord_Unit(short *pnAddr, short *pnPoints, bool bUpperEq, int nLocal, int nUnit)
{
	short	nRet=0;

	if (-1==nLocal)
		nLocal=m_nCurLocal;
	if (-1==nUnit)
		nUnit=m_nCurUnit;

	if (1==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L01_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L01_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (2==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L02_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L02_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (3==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L03_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L03_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (4==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L04_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L04_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (5==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L05_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L05_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (6==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L06_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L06_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (7==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L07_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L07_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (8==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L08_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L08_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (9==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L09_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L09_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (10==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L10_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L10_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (11==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L11_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L11_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (12==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L12_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L12_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (13==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L13_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L13_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (14==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L14_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L14_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (15==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L15_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L15_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (16==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L16_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L16_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (17==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L17_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L17_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (18==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L18_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L18_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (19==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L19_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L19_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (20==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L20_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L20_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (21==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L21_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L21_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (22==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L22_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L22_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (23==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L23_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L23_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (24==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L24_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L24_L1 + (W_L_SIZE * (nUnit-1));
		}
	}
	else if (25==nLocal){
		if (true==bUpperEq){
			*pnAddr = W_L25_U1 + (W_U_SIZE * (nUnit-1));
		}
		else{
			*pnAddr = W_L25_L1 + (W_L_SIZE * (nUnit-1));
		}
	}

	if (true==bUpperEq){
		*pnPoints = W_U_SIZE;
	}
	else{
		*pnPoints = W_L_SIZE;
	}

	return nRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCAddressWord_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPLCAddressWord_EO(EEqStates eItem, short *pnAddr, short *pnPoints, int nLocal)
{
	long	lRet=0;
	short	nItemLen=0;
	short	nAddr=0;
	short	nPoints=0;

	GetPLCAddressWord_EO(&nAddr, &nPoints, nLocal);
	*pnPoints = nItemLen = GetPLCItemLen(eItem);
	*pnAddr = nAddr + eItem - nItemLen;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCAddressWord_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPLCAddressWord_ER(EEqStates eItem, short *pnAddr, short *pnPoints, int nLocal)
{
	long	lRet=0;
	short	nItemLen=0;
	short	nAddr=0;
	short	nPoints=0;

	GetPLCAddressWord_ER(&nAddr, &nPoints, nLocal);
	*pnPoints = nItemLen = GetPLCItemLen(eItem);
	*pnAddr = nAddr + W_EO_SIZE + eItem - nItemLen;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCAddress()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPLCAddress(EGlassSummarys eItem, short *pnAddr, short *pnPoints, short nLocal)
{
	long	lRet=0;
 	short	nItemLen=0;
	short	nAddr=0;
	short	nPoints=0;

	GetPLCAddressWord_GS(&nAddr, &nPoints, nLocal);

	*pnPoints = nItemLen = GetPLCItemLen(eItem);
	*pnAddr = nAddr + eItem - nItemLen;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCAddress()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPLCAddress(EToUpperEqs eItem, short *pnAddr, short *pnPoints, int nLocal, int nUnit)
{
	long	lRet=0;
 	short	nItemLen=0;
	short	nAddr=0;
	short	nPoints=0;

	GetPLCAddressWord_Unit(&nAddr, &nPoints, true, nLocal, nUnit);
	*pnPoints = nItemLen = GetPLCItemLen(eItem);
	*pnAddr = nAddr + eItem - nItemLen + ((nUnit-1) * W_U_SIZE);

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCAddress()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPLCAddress(EToLowerEqs eItem, short *pnAddr, short *pnPoints, int nLocal, int nUnit)
{
	long	lRet=0;
 	short	nItemLen=0;
	short	nAddr=0;
	short	nPoints=0;

	GetPLCAddressWord_Unit(&nAddr, &nPoints, false, nLocal, nUnit);
	*pnPoints = nItemLen = GetPLCItemLen(eItem);
	*pnAddr = nAddr + eItem - nItemLen + ((nUnit-1) * W_L_SIZE);

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCItemLen()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
short MNetH::GetPLCItemLen(EGlassSummarys eItem)
{
	short	nRtn=0;

	if (eItem==eTGlassCount_GlassSum){
		nRtn = eTGlassCount_GlassSum;
	}
	else if (eItem==eTFTCount_GlassSum){
		nRtn = eTFTCount_GlassSum - eTGlassCount_GlassSum;
	}
	else if (eItem==eCFCount_GlassSum){
		nRtn = eCFCount_GlassSum - eTFTCount_GlassSum;
	}
	else if (eItem==eDummyCount_GlassSum){
		nRtn = eDummyCount_GlassSum - eCFCount_GlassSum;
	}
	else if (eItem==eBPGCount_GlassSum){
		nRtn = eBPGCount_GlassSum - eDummyCount_GlassSum;
	}
	else if (eItem==eProcessID1_GlassSum){
		nRtn = eProcessID1_GlassSum - eBPGCount_GlassSum;
	}
	else if (eItem==eStepID1_GlassSum){
		nRtn = eStepID1_GlassSum - eProcessID1_GlassSum;
	}
	else if (eItem==eBatchID1_GlassSum){
		nRtn = eBatchID1_GlassSum - eStepID1_GlassSum;
	}
	else if (eItem==eGlassCount1_GlassSum){
		nRtn = eGlassCount1_GlassSum - eBatchID1_GlassSum;
	}
	else if (eItem==eProcessID2_GlassSum){
		nRtn = eProcessID2_GlassSum - eGlassCount1_GlassSum;
	}
	else if (eItem==eStepID2_GlassSum){
		nRtn = eStepID2_GlassSum - eProcessID2_GlassSum;
	}
	else if (eItem==eBatchID2_GlassSum){
		nRtn = eBatchID2_GlassSum - eStepID2_GlassSum;
	}
	else if (eItem==eGlassCount2_GlassSum){
		nRtn = eGlassCount2_GlassSum - eBatchID2_GlassSum;
	}
	else if (eItem==eProcessID3_GlassSum){
		nRtn = eProcessID3_GlassSum - eGlassCount2_GlassSum;
	}
	else if (eItem==eStepID3_GlassSum){
		nRtn = eStepID3_GlassSum - eProcessID3_GlassSum;
	}
	else if (eItem==eBatchID3_GlassSum){
		nRtn = eBatchID3_GlassSum - eStepID3_GlassSum;
	}
	else if (eItem==eGlassCount3_GlassSum){
		nRtn = eGlassCount3_GlassSum - eBatchID3_GlassSum;
	}
	else if (eItem==eReserved_GlassSum){
		nRtn = eReserved_GlassSum - eGlassCount3_GlassSum;
	}

	return nRtn;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCItemLen()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
short MNetH::GetPLCItemLen(EToLowerEqs eItem)
{
	short	nRtn=0;

	if (eItem==eHPanelID_LowerEq){
		nRtn = eHPanelID_LowerEq;
	}
	else if (eItem==eEPanelID_LowerEq){
		nRtn = eEPanelID_LowerEq - eHPanelID_LowerEq;
	}
	else if (eItem==eBatchID_LowerEq){
		nRtn = eBatchID_LowerEq - eEPanelID_LowerEq;
	}
	else if (eItem==eProdType_LowerEq){
		nRtn = eProdType_LowerEq - eBatchID_LowerEq;
	}
	else if (eItem==eProdKind_LowerEq){
		nRtn = eProdKind_LowerEq - eProdType_LowerEq;
	}
	else if (eItem==eDeviceID_LowerEq){
		nRtn = eDeviceID_LowerEq - eProdKind_LowerEq;
	}
	else if (eItem==eStepID_LowerEq){
		nRtn = eStepID_LowerEq - eDeviceID_LowerEq;
	}
	else if (eItem==ePPID_LowerEq){
		nRtn = ePPID_LowerEq - eStepID_LowerEq;
	}
	else if (eItem==eThickness_LowerEq){
		nRtn = eThickness_LowerEq - ePPID_LowerEq;
	}
	else if (eItem==eInsFlag_LowerEq){
		nRtn = eInsFlag_LowerEq - eThickness_LowerEq;
	}
	else if (eItem==ePanelSize_LowerEq){
		nRtn = ePanelSize_LowerEq - eInsFlag_LowerEq;
	}
	else if (eItem==ePanelPosition_LowerEq){
		nRtn = ePanelPosition_LowerEq - ePanelSize_LowerEq;
	}
	else if (eItem==eCount1_LowerEq){
		nRtn = eCount1_LowerEq - ePanelPosition_LowerEq;
	}
	else if (eItem==eCount2_LowerEq){
		nRtn = eCount2_LowerEq - eCount1_LowerEq;
	}
	else if (eItem==eGrade_LowerEq){
		nRtn = eGrade_LowerEq - eCount2_LowerEq;
	}
	else if (eItem==eComment_LowerEq){
		nRtn = eComment_LowerEq - eGrade_LowerEq;
	}
	else if (eItem==eCompSize_LowerEq){
		nRtn = eCompSize_LowerEq - eComment_LowerEq;
	}
	else if (eItem==eReadingFlag_LowerEq){
		nRtn = eReadingFlag_LowerEq - eCompSize_LowerEq;
	}
	else if (eItem==ePanelState_LowerEq){
		nRtn = ePanelState_LowerEq - eReadingFlag_LowerEq;
	}
	else if (eItem==eJudgement_LowerEq){
		nRtn = eJudgement_LowerEq - ePanelState_LowerEq;
	}
	else if (eItem==eCode_LowerEq){
		nRtn = eCode_LowerEq - eJudgement_LowerEq;
	}
	else if (eItem==eRunLine_LowerEq){
		nRtn = eRunLine_LowerEq - eCode_LowerEq;
	}
	else if (eItem==eUniqueId_LowerEq){
		nRtn = eUniqueId_LowerEq - eRunLine_LowerEq;
	}
	else if (eItem==ePairHPanelID_LowerEq){
		nRtn = ePairHPanelID_LowerEq - eUniqueId_LowerEq;
	}
	else if (eItem==ePairEPanelID_LowerEq){
		nRtn = ePairEPanelID_LowerEq - ePairHPanelID_LowerEq;
	}
	else if (eItem==ePairGrade_LowerEq){
		nRtn = ePairGrade_LowerEq - ePairEPanelID_LowerEq;
	}
	else if (eItem==ePairUniqueId_LowerEq){
		nRtn = ePairUniqueId_LowerEq - ePairGrade_LowerEq;
	}
	else if (eItem==eFlowRecipe_LowerEq){
		nRtn = eFlowRecipe_LowerEq - ePairUniqueId_LowerEq;
	}
	else if (eItem==eReserved0_LowerEq){
		nRtn = eReserved0_LowerEq - eFlowRecipe_LowerEq;
	}
	else if (eItem==eBitsSignals_LowerEq){
		nRtn = eBitsSignals_LowerEq - eReserved0_LowerEq;
	}
	else if (eItem==eReferData_LowerEq){
		nRtn = eReferData_LowerEq - eBitsSignals_LowerEq;
	}
	else if (eItem==eCSIF_LowerEq){
		nRtn = eCSIF_LowerEq - eReferData_LowerEq;
	}
	else if (eItem==eAS_LowerEq){
		nRtn = eAS_LowerEq - eCSIF_LowerEq;
	}
	else if (eItem==eAPS_LowerEq){
		nRtn = eAPS_LowerEq - eAS_LowerEq;
	}

	return nRtn;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCItemLen()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
short MNetH::GetPLCItemLen(EToUpperEqs eItem)
{
	short	nRtn=0;

	if (eItem==eReadingGlassID_UpperEq){
		nRtn = eReadingGlassID_UpperEq;
	}
	else if (eItem==eReadingGlassIDType_UpperEq){
		nRtn = eReadingGlassIDType_UpperEq - eReadingGlassID_UpperEq;
	}
	else if (eItem==eGlassType_UpperEq){
		nRtn = eGlassType_UpperEq - eReadingGlassIDType_UpperEq;
	}
	else if (eItem==eReceiveRefuseCode_UpperEq){
		nRtn = eReceiveRefuseCode_UpperEq - eGlassType_UpperEq;
	}
	else if (eItem==eReserved_UpperEq){
		nRtn = eReserved_UpperEq - eReceiveRefuseCode_UpperEq;
	}
	else if (eItem==eCSIF_UpperEq){
		nRtn = eCSIF_UpperEq - eReserved_UpperEq;
	}
	else if (eItem==eAS_UpperEq){
		nRtn = eAS_UpperEq - eCSIF_UpperEq;
	}
	else if (eItem==eAPS_UpperEq){
		nRtn = eAPS_UpperEq - eAS_UpperEq;
	}

	return nRtn;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCItemLen()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
short MNetH::GetPLCItemLen(EEqStates eItem)
{
	short	nRtn=0;

	if (eItem==eEqState_EqState){
		nRtn = eEqState_EqState;
	}
	else if (eItem==eProcState_EqState){
		nRtn = eProcState_EqState - eEqState_EqState;
	}
	else if (eItem==ePPID_EqState){
		nRtn = ePPID_EqState - eProcState_EqState;
	}
	else if (eItem==eGlassSize_EqState){
		nRtn = eGlassSize_EqState - ePPID_EqState;
	}
	else if (eItem==eGlassThickness_EqState){
		nRtn = eGlassThickness_EqState - eGlassSize_EqState;
	}
	else if (eItem==eTACTSet_EqState){
		nRtn = eTACTSet_EqState - eGlassThickness_EqState;
	}
	else if (eItem==eTACTCurrent_EqState){
		nRtn = eTACTCurrent_EqState - eTACTSet_EqState;
	}
	else if (eItem==eRCDG_EqState){
		nRtn = eRCDG_EqState - eTACTCurrent_EqState;
	}
	else if (eItem==eEOMode_EqState){
		nRtn = eEOMode_EqState - eRCDG_EqState;
	}
	else if (eItem==eERCMode_EqState){
		nRtn = eERCMode_EqState - eEOMode_EqState;
	}
	else if (eItem==eGMCMode_EqState){
		nRtn = eGMCMode_EqState - eERCMode_EqState;
	}
	else if (eItem==eOperateMode_EqState){
		nRtn = eOperateMode_EqState - eGMCMode_EqState;
	}
	else if (eItem==eReserved_EqState){
		nRtn = eReserved_EqState - eOperateMode_EqState;
	}

	return nRtn;
}
//SJ_YYK 150821 Add...
long MNetH::GetPLCAddress(EMaterialInfo eItem, short *pnAddr, short *pnPoints, short nLocal, int nUnit)
{
	long	lRet=0;
 	short	nItemLen=0;
	short	nAddr=0;
	short	nPoints=0;

	GetPLCAddressWord_MInfo(&nAddr, &nPoints, nLocal, nUnit);
	*pnPoints = nItemLen = GetPLCItemLen(eItem);
	*pnAddr = nAddr + eItem - nItemLen + ((nUnit-1) * W_MAT_SIZE);

	return lRet;
}

long MNetH::GetPLCAddress(eFDCinfo eItem, short *pnAddr, short *pnPoints, short nLocal, int nUnit)
{
	long	lRet=0;
 	short	nItemLen=0;
	short	nAddr=0;
	short	nPoints=0;

	nUnit = 13; //Fix..

	GetPLCAddressWord_FDCInfo(&nAddr, &nPoints, nLocal, nUnit);
	*pnPoints = nItemLen = GetPLCItemLen(eItem);
	*pnAddr = nAddr + eItem - nItemLen + ((nUnit-1) * W_MAT_SIZE);

	return lRet;
}

short MNetH::GetPLCItemLen(EMaterialInfo eItem)
{
	short	nRtn=0;

	if (eItem==e_sPORTID){
		nRtn = e_sPORTID;
	}
	else if (eItem==e_sEQP_STATE){
		nRtn = e_sEQP_STATE - e_sPORTID;
	}
	else if (eItem==e_sPORT_STATE){
		nRtn = e_sPORT_STATE - e_sEQP_STATE;
	}
	else if (eItem==e_sPORT_TYPE){
		nRtn = e_sPORT_TYPE - e_sPORT_STATE;
	}
	else if (eItem==e_sCSTID){
		nRtn = e_sCSTID - e_sPORT_TYPE;
	}
	else if (eItem==e_nMAP_STIF){
		nRtn = e_nMAP_STIF - e_sCSTID;
	}
	else if (eItem==e_nCUR_STIF){
		nRtn = e_nCUR_STIF - e_nMAP_STIF;
	}
	else if (eItem==e_sM_BATCHID){
		nRtn = e_sM_BATCHID - e_nCUR_STIF;
	}
	else if (eItem==e_sM_MAKER){
		nRtn = e_sM_MAKER - e_sM_BATCHID;
	}
	else if (eItem==e_nM_KIND){
		nRtn = e_nM_KIND - e_sM_MAKER;
	}
	else if (eItem==e_nM_TYPE){
		nRtn = e_nM_TYPE - e_nM_KIND;
	}
	else if (eItem==e_nM_STATE){
		nRtn = e_nM_STATE - e_nM_TYPE;
	}
	else if (eItem==e_nT_QTY){
		nRtn = e_nT_QTY - e_nM_STATE;
	}
	else if (eItem==e_nU_QTY){
		nRtn = e_nU_QTY - e_nT_QTY;
	}
	else if (eItem==e_nR_QTY){
		nRtn = e_nR_QTY - e_nU_QTY;
	}
	else if (eItem==e_nN_QTY){
		nRtn = e_nN_QTY - e_nR_QTY;
	}
	else if (eItem==e_nA_QTY){
		nRtn = e_nA_QTY - e_nN_QTY;
	}
	else if (eItem==e_nE_FLAG){
		nRtn = e_nE_FLAG - e_nA_QTY;
	}
	else if (eItem==e_sC_CODE){
		nRtn = e_sC_CODE - e_nE_FLAG;
	}
	else if (eItem==e_sM_STEP){
		nRtn = e_sM_STEP - e_sC_CODE;
	}
	else if (eItem==e_nS_QTY){
		nRtn = e_nS_QTY - e_sM_STEP;
	}
	else if (eItem==e_sM_CODE){
		nRtn = e_sM_CODE - e_nS_QTY;
	}

	return nRtn;
}

short MNetH::GetPLCItemLen(eFDCinfo eItem)
{
	short	nRtn=0;

	if (eItem==e_ToolTempM01){
		nRtn = e_ToolTempM01;
	}
	else if (eItem==e_ToolTempM02){
		nRtn = e_ToolTempM02 - e_ToolTempM01;
	}
	else if (eItem==e_ToolTempM03){
		nRtn = e_ToolTempM03 - e_ToolTempM02;
	}
	else if (eItem==e_ToolTempM04){
		nRtn = e_ToolTempM04 - e_ToolTempM03;
	}
	else if (eItem==e_IndexTempM01){
		nRtn = e_IndexTempM01 - e_ToolTempM04;
	}
	else if (eItem==e_IndexTempM02){
		nRtn = e_IndexTempM02 - e_IndexTempM01;
	}
	else if (eItem==e_IndexTempM03){
		nRtn = e_IndexTempM03 - e_IndexTempM02;
	}
	else if (eItem==e_IndexTempM04){
		nRtn = e_IndexTempM04 - e_IndexTempM03;
	}
	else if (eItem==e_IndexTempM05){
		nRtn = e_IndexTempM05 - e_IndexTempM04;
	}
	else if (eItem==e_IndexTempM06){
		nRtn = e_IndexTempM06 - e_IndexTempM05;
	}
	else if (eItem==e_IndexTempM07){
		nRtn = e_IndexTempM07 - e_IndexTempM06;
	}
	else if (eItem==e_IndexTempM08){
		nRtn = e_IndexTempM08 - e_IndexTempM07;
	}
	else if (eItem==e_IndexTempM09){
		nRtn = e_IndexTempM09 - e_IndexTempM08;
	}
	else if (eItem==e_IndexTempM10){
		nRtn = e_IndexTempM10 - e_IndexTempM09;
	}
	else if (eItem==e_IndexTempM11){
		nRtn = e_IndexTempM11 - e_IndexTempM10;
	}
	else if (eItem==e_IndexTempM12){
		nRtn = e_IndexTempM12 - e_IndexTempM11;
	}
	else if (eItem==e_IndexTempM13){
		nRtn = e_IndexTempM13 - e_IndexTempM12;
	}
	else if (eItem==e_IndexTempM14){
		nRtn = e_IndexTempM14 - e_IndexTempM13;
	}
	else if (eItem==e_IndexTempM15){
		nRtn = e_IndexTempM15 - e_IndexTempM14;
	}
	else if (eItem==e_IndexTempM16){
		nRtn = e_IndexTempM16 - e_IndexTempM15;
	}
	else if (eItem==e_ToolTempS01){
		nRtn = e_ToolTempS01 - e_IndexTempM16;
	}
	else if (eItem==e_ToolTempS02){
		nRtn = e_ToolTempS02 - e_ToolTempS01;
	}
	else if (eItem==e_ToolTempS03){
		nRtn = e_ToolTempS03 - e_ToolTempS02;
	}
	else if (eItem==e_ToolTempS04){
		nRtn = e_ToolTempS04 - e_ToolTempS03;
	}
	else if (eItem==e_IndexTempS01){
		nRtn = e_IndexTempS01 - e_ToolTempS04;
	}
	else if (eItem==e_IndexTempS02){
		nRtn = e_IndexTempS02 - e_IndexTempS01;
	}
	else if (eItem==e_IndexTempS03){
		nRtn = e_IndexTempS03 - e_IndexTempS02;
	}
	else if (eItem==e_IndexTempS04){
		nRtn = e_IndexTempS04 - e_IndexTempS03;
	}
	else if (eItem==e_IndexTempS05){
		nRtn = e_IndexTempS05 - e_IndexTempS04;
	}
	else if (eItem==e_IndexTempS06){
		nRtn = e_IndexTempS06 - e_IndexTempS05;
	}
	else if (eItem==e_IndexTempS07){
		nRtn = e_IndexTempS07 - e_IndexTempS06;
	}
	else if (eItem==e_IndexTempS08){
		nRtn = e_IndexTempS08 - e_IndexTempS07;
	}
	else if (eItem==e_IndexTempS09){
		nRtn = e_IndexTempS09 - e_IndexTempS08;
	}
	else if (eItem==e_IndexTempS10){
		nRtn = e_IndexTempM10 - e_IndexTempS09;
	}
	else if (eItem==e_IndexTempS11){
		nRtn = e_IndexTempS11 - e_IndexTempM10;
	}
	else if (eItem==e_IndexTempS12){
		nRtn = e_IndexTempS12 - e_IndexTempS11;
	}
	else if (eItem==e_IndexTempS13){
		nRtn = e_IndexTempS13 - e_IndexTempS12;
	}
	else if (eItem==e_IndexTempS14){
		nRtn = e_IndexTempS14 - e_IndexTempS13;
	}
	else if (eItem==e_IndexTempS15){
		nRtn = e_IndexTempS15 - e_IndexTempS14;
	}
	else if (eItem==e_IndexTempS16){
		nRtn = e_IndexTempS16 - e_IndexTempS15;
	}
	return nRtn;
}

short MNetH::GetPLCAddressWord_MInfo(short *pnAddr, short *pnPoints, int nLocal, int nUnit)
{
	short	nRet=0;

	if (-1==nLocal)
		nLocal=m_nCurLocal; 
	if (-1==nUnit)
		nUnit=m_nCurUnit;

	if (1==nLocal)
	{
		*pnAddr=W_L01_MT+ (W_MAT_SIZE * (nUnit-1)); 
	}
	else if (2==nLocal)
	{
		*pnAddr=W_L02_MT+ (W_MAT_SIZE * (nUnit-1));
	}
	else{ 
		*pnAddr=W_L01_MT+ (W_MAT_SIZE * (nUnit-1));
	}

	*pnPoints = W_MAT_SIZE;

	return nRet;
}

long MNetH::GetPLCAddressWord_MInfo(EMaterialInfo eItem, short *pnAddr, short *pnPoints, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nItemLen=0;
	short	nAddr=0;
	short	nPoints=0;

	GetPLCAddressWord_MInfo(&nAddr, &nPoints, nLocal);
	*pnPoints = nItemLen = GetPLCItemLen(eItem);
	*pnAddr = nAddr + W_MAT_SIZE+ eItem - nItemLen;

	return lRet;
}

short MNetH::GetPLCAddressWord_FDCInfo(short *pnAddr, short *pnPoints, int nLocal, int nUnit)
{
	short	nRet=0;

	if (-1==nLocal)
		nLocal=m_nCurLocal; 
	if (-1==nUnit)
		nUnit = 13;

	if (1==nLocal)
	{
		*pnAddr=W_L01_MT+ (W_MAT_SIZE * (nUnit-1)); 
	}
	else if (2==nLocal)
	{
		*pnAddr=W_L02_MT+ (W_MAT_SIZE * (nUnit-1));
	}
	else{ 
		*pnAddr=W_L01_MT+ (W_MAT_SIZE * (nUnit-1));
	}

	*pnPoints = W_FDC_SIZE;

	return nRet;
}

long MNetH::GetPLCAddressWord_FDCInfo(eFDCinfo eItem, short *pnAddr, short *pnPoints, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nItemLen=0;
	short	nAddr=0;
	short	nPoints=0;

	GetPLCAddressWord_FDCInfo(&nAddr, &nPoints, nLocal);
	*pnPoints = nItemLen = GetPLCItemLen(eItem);
//kdh Test
	*pnAddr = nAddr + W_FDC_SIZE + eItem - nItemLen;

	return lRet;
}
//_________________________________________*/

/*========================================================================================
	FUNCTION : MNetH::GetRobotArmStateBit_ToUpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::GetRobotArmStateBit_ToUpperEq(MRobotArmStateBit *pRS, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += 32;
	nPoints = 16;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		pRS->m_nRobotAbnormal = pnRBuf[nIndex++];	
		pRS->m_nTypeOfArm = pnRBuf[nIndex++];
		pRS->m_nTypeOfStageConveyor = pnRBuf[nIndex++];	
		pRS->m_nArm1Violate = pnRBuf[nIndex++];
		pRS->m_nArm2Violate = pnRBuf[nIndex++];
		pRS->m_nArm1FoldComplete = pnRBuf[nIndex++];
		pRS->m_nArm2FoldComplete = pnRBuf[nIndex++];
		pRS->m_nArm1GlassCheckSensor = pnRBuf[nIndex++];
		pRS->m_nArm2GlassCheckSensor = pnRBuf[nIndex++];
		pRS->m_nReserved0 = pnRBuf[nIndex++];
		pRS->m_nReserved1 = pnRBuf[nIndex++];
		pRS->m_nReserved2 = pnRBuf[nIndex++];
		pRS->m_nManualOperation = pnRBuf[nIndex++];
		pRS->m_nRobotDirection = pnRBuf[nIndex++];
		pRS->m_nReserved3 = pnRBuf[nIndex++];
		pRS->m_nReserved4 = pnRBuf[nIndex++];
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetRobotArmStateBit_ToLowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::GetRobotArmStateBit_ToLowerEq(MRobotArmStateBit *pRS, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += 32;
	nPoints = 16;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		pRS->m_nRobotAbnormal = pnRBuf[nIndex++];	
		pRS->m_nTypeOfArm = pnRBuf[nIndex++];
		pRS->m_nTypeOfStageConveyor = pnRBuf[nIndex++];	
		pRS->m_nArm1Violate = pnRBuf[nIndex++];
		pRS->m_nArm2Violate = pnRBuf[nIndex++];
		pRS->m_nArm1FoldComplete = pnRBuf[nIndex++];
		pRS->m_nArm2FoldComplete = pnRBuf[nIndex++];
		pRS->m_nArm1GlassCheckSensor = pnRBuf[nIndex++];
		pRS->m_nArm2GlassCheckSensor = pnRBuf[nIndex++];
		pRS->m_nReserved0 = pnRBuf[nIndex++];
		pRS->m_nReserved1 = pnRBuf[nIndex++];
		pRS->m_nReserved2 = pnRBuf[nIndex++];
		pRS->m_nManualOperation = pnRBuf[nIndex++];
		pRS->m_nRobotDirection = pnRBuf[nIndex++];
		pRS->m_nReserved3 = pnRBuf[nIndex++];
		pRS->m_nReserved4 = pnRBuf[nIndex++];
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetStageStateBit_ToUpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::GetStageStateBit_ToUpperEq(MStageStateBit *pSS, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += 32;
	nPoints = 16;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		pSS->m_nStageCVAbnormal = pnRBuf[nIndex++];	
		pSS->m_nTypeOfArm = pnRBuf[nIndex++];
		pSS->m_nTypeOfStageConveyor = pnRBuf[nIndex++];	
		pSS->m_nEmpty = pnRBuf[nIndex++];
		pSS->m_nIdle = pnRBuf[nIndex++];
		pSS->m_nBusy = pnRBuf[nIndex++];
		pSS->m_nPinUp = pnRBuf[nIndex++];
		pSS->m_nPinDown = pnRBuf[nIndex++];
		pSS->m_nStopperUp = pnRBuf[nIndex++];
		pSS->m_nStopperDown = pnRBuf[nIndex++];
		pSS->m_nDoorOpen = pnRBuf[nIndex++];
		pSS->m_nDoorClose = pnRBuf[nIndex++];
		pSS->m_nManualOperation = pnRBuf[nIndex++];
		pSS->m_nBodyMoving = pnRBuf[nIndex++];
		pSS->m_nGlassCheckSensorOn = pnRBuf[nIndex++];
		pSS->m_nReserved0 = pnRBuf[nIndex++];
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetStageStateBit_ToLowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::GetStageStateBit_ToLowerEq(MStageStateBit *pSS, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += 32;
	nPoints = 16;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		pSS->m_nStageCVAbnormal = pnRBuf[nIndex++];	
		pSS->m_nTypeOfArm = pnRBuf[nIndex++];
		pSS->m_nTypeOfStageConveyor = pnRBuf[nIndex++];	
		pSS->m_nEmpty = pnRBuf[nIndex++];
		pSS->m_nIdle = pnRBuf[nIndex++];
		pSS->m_nBusy = pnRBuf[nIndex++];
		pSS->m_nPinUp = pnRBuf[nIndex++];
		pSS->m_nPinDown = pnRBuf[nIndex++];
		pSS->m_nStopperUp = pnRBuf[nIndex++];
		pSS->m_nStopperDown = pnRBuf[nIndex++];
		pSS->m_nDoorOpen = pnRBuf[nIndex++];
		pSS->m_nDoorClose = pnRBuf[nIndex++];
		pSS->m_nManualOperation = pnRBuf[nIndex++];
		pSS->m_nBodyMoving = pnRBuf[nIndex++];
		pSS->m_nGlassCheckSensorOn = pnRBuf[nIndex++];
		pSS->m_nReserved0 = pnRBuf[nIndex++];
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetRobotArmStateBit_ToUpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::SetRobotArmStateBit_ToUpperEq(MRobotArmStateBit *pRS, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += 32;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	pnRBuf[nIndex++] = pRS->m_nRobotAbnormal;	
	pnRBuf[nIndex++] = pRS->m_nTypeOfArm;
	pnRBuf[nIndex++] = pRS->m_nTypeOfStageConveyor;	
	pnRBuf[nIndex++] = pRS->m_nArm1Violate;
	pnRBuf[nIndex++] = pRS->m_nArm2Violate;
	pnRBuf[nIndex++] = pRS->m_nArm1FoldComplete;
	pnRBuf[nIndex++] = pRS->m_nArm2FoldComplete;
	pnRBuf[nIndex++] = pRS->m_nArm1GlassCheckSensor;
	pnRBuf[nIndex++] = pRS->m_nArm2GlassCheckSensor;
	pnRBuf[nIndex++] = pRS->m_nReserved0;
	pnRBuf[nIndex++] = pRS->m_nReserved1;
	pnRBuf[nIndex++] = pRS->m_nReserved2;
	pnRBuf[nIndex++] = pRS->m_nManualOperation;
	pnRBuf[nIndex++] = pRS->m_nRobotDirection;
	pnRBuf[nIndex++] = pRS->m_nReserved3;
	pnRBuf[nIndex++] = pRS->m_nReserved4;

	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetRobotArmStateBit_ToLowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::SetRobotArmStateBit_ToLowerEq(MRobotArmStateBit *pRS, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += 32;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	pnRBuf[nIndex++] = pRS->m_nRobotAbnormal;	
	pnRBuf[nIndex++] = pRS->m_nTypeOfArm;
	pnRBuf[nIndex++] = pRS->m_nTypeOfStageConveyor;	
	pnRBuf[nIndex++] = pRS->m_nArm1Violate;
	pnRBuf[nIndex++] = pRS->m_nArm2Violate;
	pnRBuf[nIndex++] = pRS->m_nArm1FoldComplete;
	pnRBuf[nIndex++] = pRS->m_nArm2FoldComplete;
	pnRBuf[nIndex++] = pRS->m_nArm1GlassCheckSensor;
	pnRBuf[nIndex++] = pRS->m_nArm2GlassCheckSensor;
	pnRBuf[nIndex++] = pRS->m_nReserved0;
	pnRBuf[nIndex++] = pRS->m_nReserved1;
	pnRBuf[nIndex++] = pRS->m_nReserved2;
	pnRBuf[nIndex++] = pRS->m_nManualOperation;
	pnRBuf[nIndex++] = pRS->m_nRobotDirection;
	pnRBuf[nIndex++] = pRS->m_nReserved3;
	pnRBuf[nIndex++] = pRS->m_nReserved4;

	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetStageStateBit_ToUpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::SetStageStateBit_ToUpperEq(MStageStateBit *pSS, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += 32;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	pnRBuf[nIndex++] = pSS->m_nStageCVAbnormal;	
	pnRBuf[nIndex++] = pSS->m_nTypeOfArm;
	pnRBuf[nIndex++] = pSS->m_nTypeOfStageConveyor;	
	pnRBuf[nIndex++] = pSS->m_nEmpty;
	pnRBuf[nIndex++] = pSS->m_nIdle;
	pnRBuf[nIndex++] = pSS->m_nBusy;
	pnRBuf[nIndex++] = pSS->m_nPinUp;
	pnRBuf[nIndex++] = pSS->m_nPinDown;
	pnRBuf[nIndex++] = pSS->m_nStopperUp;
	pnRBuf[nIndex++] = pSS->m_nStopperDown;
	pnRBuf[nIndex++] = pSS->m_nDoorOpen;
	pnRBuf[nIndex++] = pSS->m_nDoorClose;
	pnRBuf[nIndex++] = pSS->m_nManualOperation;
	pnRBuf[nIndex++] = pSS->m_nBodyMoving;
	pnRBuf[nIndex++] = pSS->m_nGlassCheckSensorOn;
	pnRBuf[nIndex++] = pSS->m_nReserved0;

	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetStageStateBit_ToLowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::SetStageStateBit_ToLowerEq(MStageStateBit *pSS, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += 32;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	pnRBuf[nIndex++] = pSS->m_nStageCVAbnormal;	
	pnRBuf[nIndex++] = pSS->m_nTypeOfArm;
	pnRBuf[nIndex++] = pSS->m_nTypeOfStageConveyor;	
	pnRBuf[nIndex++] = pSS->m_nEmpty;
	pnRBuf[nIndex++] = pSS->m_nIdle;
	pnRBuf[nIndex++] = pSS->m_nBusy;
	pnRBuf[nIndex++] = pSS->m_nPinUp;
	pnRBuf[nIndex++] = pSS->m_nPinDown;
	pnRBuf[nIndex++] = pSS->m_nStopperUp;
	pnRBuf[nIndex++] = pSS->m_nStopperDown;
	pnRBuf[nIndex++] = pSS->m_nDoorOpen;
	pnRBuf[nIndex++] = pSS->m_nDoorClose;
	pnRBuf[nIndex++] = pSS->m_nManualOperation;
	pnRBuf[nIndex++] = pSS->m_nBodyMoving;
	pnRBuf[nIndex++] = pSS->m_nGlassCheckSensorOn;
	pnRBuf[nIndex++] = pSS->m_nReserved0;

	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetContactStateBit_ToUpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/11/17, James Park; First work!
========================================================================================*/
long MNetH::SetContactStateBit_ToUpperEq(BOOL nBit, int nPoint, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += 32;
    nAddr += nPoint;
    nPoints = 2;
    pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
    pnRBuf[0] = nBit;

	lRet = WriteLB(nAddr, 1, pnRBuf, sizeof(short)*1); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetContactStateBit_ToLowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/11/17, James Park; First work!
========================================================================================*/
long MNetH::SetContactStateBit_ToLowerEq(BOOL nBit, int nPoint, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += 32;
    nAddr += nPoint;
    nPoints = 2;
    pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
    pnRBuf[0] = nBit;

	lRet = WriteLB(nAddr, 1, pnRBuf, sizeof(short)*1); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetStageStateBit_ToUpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetContactStateBit_ToUpperEq(BOOL *pnBit, int nPoint, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += 32;
    nAddr += nPoint;
    nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, 1, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
        *pnBit = pnRBuf[0];
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetContactStateBit_ToLowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetContactStateBit_ToLowerEq(BOOL *pnBit, int nPoint, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += 32;
    nAddr += nPoint;
    nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, 1, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		*pnBit = pnRBuf[0];
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetToUpperEqBit_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::SetToUpperEqBit_ToUpperEqBit(MToUpperEqBit *pUE, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nPoints = 32;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	pnRBuf[nIndex++] = pUE->m_nReserved0;
	pnRBuf[nIndex++] = pUE->m_nMachinePause;
	pnRBuf[nIndex++] = pUE->m_nMachineDown;
	pnRBuf[nIndex++] = pUE->m_nMachineAlarm;
	pnRBuf[nIndex++] = pUE->m_nReceiveAble;
	pnRBuf[nIndex++] = pUE->m_nReceiveStart;
	pnRBuf[nIndex++] = pUE->m_nReceiveComplete;
	pnRBuf[nIndex++] = pUE->m_nAction1Complete;
	pnRBuf[nIndex++] = pUE->m_nAction2Complete;
	pnRBuf[nIndex++] = pUE->m_nReserved1;
	pnRBuf[nIndex++] = pUE->m_nReserved2;
	pnRBuf[nIndex++] = pUE->m_nReceiveRefuse;
	pnRBuf[nIndex++] = pUE->m_nGlassIdReadComplete;
	pnRBuf[nIndex++] = pUE->m_nLoadingStop;
	pnRBuf[nIndex++] = pUE->m_nTransferStop;
	pnRBuf[nIndex++] = pUE->m_nImmediatelyPauseRequest;
	pnRBuf[nIndex++] = pUE->m_nImmediatelyStopRequest;
	pnRBuf[nIndex++] = pUE->m_nReceiveAbleRemainedStep0;
	pnRBuf[nIndex++] = pUE->m_nReceiveAbleRemainedStep1;
	pnRBuf[nIndex++] = pUE->m_nReceiveAbleRemainedStep2;
	pnRBuf[nIndex++] = pUE->m_nReceiveAbleRemainedStep3;
	pnRBuf[nIndex++] = pUE->m_nHandShakeCancelRequest;
	pnRBuf[nIndex++] = pUE->m_nHandShakeAbortRequest;
	pnRBuf[nIndex++] = pUE->m_nHandShakeResumeRequest;
	pnRBuf[nIndex++] = pUE->m_nHandShakeCancelRequest;	
	pnRBuf[nIndex++] = pUE->m_nHandShakeAbortRequest;	
	pnRBuf[nIndex++] = pUE->m_nHandShakeResumeRequest;	
	pnRBuf[nIndex++] = pUE->m_nHandShakeRecoveryAckReply;
	pnRBuf[nIndex++] = pUE->m_nHandShakeRecoveryNckReply;	
	pnRBuf[nIndex++] = pUE->m_nReserved3;
	pnRBuf[nIndex++] = pUE->m_nReserved4;
	pnRBuf[nIndex++] = pUE->m_nReserved5;
	
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){

	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetToLowerEqBit_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::SetToLowerEqBit_ToLowerEqBit(MToLowerEqBit *pLE, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nPoints = 32;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[nIndex++] = pLE->m_nReserved0;
	pnRBuf[nIndex++] = pLE->m_nMachinePause;
	pnRBuf[nIndex++] = pLE->m_nMachineDown;
	pnRBuf[nIndex++] = pLE->m_nMachineAlarm;
	pnRBuf[nIndex++] = pLE->m_nSendAble;
	pnRBuf[nIndex++] = pLE->m_nSendStart;
	pnRBuf[nIndex++] = pLE->m_nSendComplete;
	pnRBuf[nIndex++] = pLE->m_nExchangeFlag;
	pnRBuf[nIndex++] = pLE->m_nReturnReceiveStart;
	pnRBuf[nIndex++] = pLE->m_nReturnReceiveComplete;
	pnRBuf[nIndex++] = pLE->m_nAction1Request;
	pnRBuf[nIndex++] = pLE->m_nAction2Request;
	pnRBuf[nIndex++] = pLE->m_nReserved1;
	pnRBuf[nIndex++] = pLE->m_nReserved2;
	pnRBuf[nIndex++] = pLE->m_nWorkStart;
	pnRBuf[nIndex++] = pLE->m_nWorkCancel;
	pnRBuf[nIndex++] = pLE->m_nReserved3;
	pnRBuf[nIndex++] = pLE->m_nReserved4;
	pnRBuf[nIndex++] = pLE->m_nImmediatelyPauseRequest;
	pnRBuf[nIndex++] = pLE->m_nImmediatelyStopRequest;
	pnRBuf[nIndex++] = pLE->m_nSendAbleRemainedStep0;
	pnRBuf[nIndex++] = pLE->m_nSendAbleRemainedStep1;
	pnRBuf[nIndex++] = pLE->m_nSendAbleRemainedStep2;
	pnRBuf[nIndex++] = pLE->m_nSendAbleRemainedStep3;
	pnRBuf[nIndex++] = pLE->m_nHandShakeCancelRequest;	
	pnRBuf[nIndex++] = pLE->m_nHandShakeAbortRequest;
	pnRBuf[nIndex++] = pLE->m_nHandShakeResumeRequest;	
	pnRBuf[nIndex++] = pLE->m_nHandShakeRecoveryAckReply;	
	pnRBuf[nIndex++] = pLE->m_nHandShakeRecoveryNckReply;
	pnRBuf[nIndex++] = pLE->m_nReserved5;
	pnRBuf[nIndex++] = pLE->m_nReserved6;
	pnRBuf[nIndex++] = pLE->m_nReserved7;

	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){

	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGlassSummaryWord()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/11/26, James Park
========================================================================================*/
long MNetH::GetGlassSummaryWord(MGlassSummarys *pGS, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	CString	sTmp;
	
	GetPLCAddressWord_GS(&nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet)
	{
		//20111206 SJ_HJG
		for (int i = 0; i < DEF_MAX_TAB; i++)
		{
			pGS->m_strReceiveTabOffsetDataA[i].Format(_T("%d"), pnRBuf[i]);
			pGS->m_strReceiveTabOffsetDataB[i].Format(_T("%d"), pnRBuf[i+DEF_MAX_TAB]);
		}

		/*
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eTGlassCount_GlassSum); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}						
		HexToDec(sTmp, pGS->m_strTGlassCount);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eTFTCount_GlassSum); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}					
		HexToDec(sTmp, pGS->m_strTFTCount);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eCFCount_GlassSum); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}						
		HexToDec(sTmp, pGS->m_strCFCount);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eDummyCount_GlassSum); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}					
		HexToDec(sTmp, pGS->m_strDummyCount);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eBPGCount_GlassSum); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}	
		HexToDec(sTmp, pGS->m_strBPGCount);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eProcessID1_GlassSum); i++){
			pGS->m_strProcessID1 += MakeString(pnRBuf[nIndex++]);
		}				
		for (int i = 0; i < GetPLCItemLen(eStepID1_GlassSum); i++){
			pGS->m_strStepID1 += MakeString(pnRBuf[nIndex++]);
		}				
		for (int i = 0; i < GetPLCItemLen(eBatchID1_GlassSum); i++){
			pGS->m_strBatchID1 += MakeString(pnRBuf[nIndex++]);
		}				
		for (int i = 0; i < GetPLCItemLen(eGlassCount1_GlassSum); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}						
		HexToDec(sTmp, pGS->m_strGlassCount1);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eProcessID2_GlassSum); i++){
			pGS->m_strProcessID2 += MakeString(pnRBuf[nIndex++]);
		}				
		for (int i = 0; i < GetPLCItemLen(eStepID2_GlassSum); i++){
			pGS->m_strStepID2 += MakeString(pnRBuf[nIndex++]);
		}				
		for (int i = 0; i < GetPLCItemLen(eBatchID2_GlassSum); i++){
			pGS->m_strBatchID2 += MakeString(pnRBuf[nIndex++]);
		}				
		for (int i = 0; i < GetPLCItemLen(eGlassCount2_GlassSum); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}						
		HexToDec(sTmp, pGS->m_strGlassCount2);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eProcessID3_GlassSum); i++){
			pGS->m_strProcessID3 += MakeString(pnRBuf[nIndex++]);
		}				
		for (int i = 0; i < GetPLCItemLen(eStepID3_GlassSum); i++){
			pGS->m_strStepID3 += MakeString(pnRBuf[nIndex++]);
		}				
		for (int i = 0; i < GetPLCItemLen(eBatchID3_GlassSum); i++){
			pGS->m_strBatchID3 += MakeString(pnRBuf[nIndex++]);
		}				
		for (int i = 0; i < GetPLCItemLen(eGlassCount3_GlassSum); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}	
		HexToDec(sTmp, pGS->m_strGlassCount3);
		sTmp = "";
		*/		
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetToLowerEqWord()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2006/02/07, cha
========================================================================================*/
long MNetH::GetToLowerEqWord(MToLowerEqs *pLE, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	int		i = 0;
	CString	sTmp="";
	long lValue = 0;
	char szAscii[128];

	memset(szAscii, 0, sizeof(szAscii));
	
	GetPLCAddressWord_Unit(&nAddr, &nPoints, false, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
#ifndef DEF_MELSEC_VER_2014		
		AscToString(szAscii, &pnRBuf[0], 6);	TrimSpace(szAscii);		pLE->m_strHPanelID	= szAscii;	
		AscToString(szAscii, &pnRBuf[6], 6);	TrimSpace(szAscii);		pLE->m_strEPanelID	= szAscii;	
		AscToString(szAscii, &pnRBuf[12], 6);	TrimSpace(szAscii);		pLE->m_strBatchID	= szAscii;	
		AscToString(szAscii, &pnRBuf[18], 1);	TrimSpace(szAscii);		pLE->m_strProdType	= szAscii;	
		AscToString(szAscii, &pnRBuf[19], 1);	TrimSpace(szAscii);		pLE->m_strProdKind	= szAscii;	
		AscToString(szAscii, &pnRBuf[20], 9);	TrimSpace(szAscii);		pLE->m_strDeviceID	= szAscii;	
		AscToString(szAscii, &pnRBuf[29], 5);	TrimSpace(szAscii);		pLE->m_strStepID	= szAscii;	
		AscToString(szAscii, &pnRBuf[34], 8);	TrimSpace(szAscii);		pLE->m_strPPID		= szAscii;		
		pLE->m_strThickness.Format(_T("%d"), pnRBuf[42]);
		AscToString(szAscii, &pnRBuf[43], 1);	TrimSpace(szAscii);		pLE->m_strInsFlag	= szAscii;		

		pLE->m_strPanelSize.Format(_T("%d/%d"), pnRBuf[44], pnRBuf[45]);
		AscToString(szAscii, &pnRBuf[46], 1);	TrimSpace(szAscii);		pLE->m_strPanelPosition	= szAscii;	
		AscToString(szAscii, &pnRBuf[47], 1);	TrimSpace(szAscii);		pLE->m_strCount1	= szAscii;	
		AscToString(szAscii, &pnRBuf[48], 1);	TrimSpace(szAscii);		pLE->m_strCount2	= szAscii;	
		
		for (int i = 0; i < 16; i++) { szAscii[i + 0 ] = (pnRBuf[49] & (0x0001 << i)) ? '1' : '0'; }
		for (int i = 0; i < 16; i++) { szAscii[i + 16] = (pnRBuf[50] & (0x0001 << i)) ? '1' : '0'; }
		for (int i = 0; i < 16; i++) { szAscii[i + 32] = (pnRBuf[51] & (0x0001 << i)) ? '1' : '0'; }
		szAscii[48] = '\0';
		pLE->m_strGrade = szAscii;
		AscToString(szAscii, &pnRBuf[52], 8);	TrimSpace(szAscii);		pLE->m_strComment	= szAscii;	
		pLE->m_strCompSize.Format(_T("%d"), pnRBuf[60]);
		AscToString(szAscii, &pnRBuf[61], 1);	TrimSpace(szAscii);		pLE->m_strReadingFlag	= szAscii;	
		pLE->m_strPanelState.Format(_T("%d"), pnRBuf[62]);
		AscToString(szAscii, &pnRBuf[63], 2);	TrimSpace(szAscii);		pLE->m_strJudgement	= szAscii;	
		AscToString(szAscii, &pnRBuf[65], 2);	TrimSpace(szAscii);		pLE->m_strCode	= szAscii;	

	// NSMC 2009.04.07 SESL_CYJ 탕정 NSMC 진행시 RunLine 값을 Binary에서 Ascii로 Spec 변경, 다른 사이트는 현재까지 아님. 
//#ifdef DEF_TANGJUNG_SITE
//		AscToString(szAscii, &pnRBuf[67], 10);	TrimSpace(szAscii);		pLE->m_strRunLine	= szAscii;	
//#else
		//	 20081111 jdy
		for (int i = 0; i < 10; i++ ) {
			if ( i == 0) { sTmp.Format(_T("%d"), LOBYTE(pnRBuf[67 + i])); }
			if ( i != 0) { sTmp.Format(_T("/%d"), LOBYTE(pnRBuf[67 + i])); }
			pLE->m_strRunLine += sTmp;
			sTmp.Format(_T("/%d"), HIBYTE(pnRBuf[67 + i]));
			pLE->m_strRunLine += sTmp;
		}
//#endif

		for (int i = 0; i < 2; i++) {
			if (i == 0) { sTmp.Format(_T("%d"), LOBYTE(pnRBuf[77 + i])); }
			if (i != 0) { sTmp.Format(_T("/%d"), LOBYTE(pnRBuf[77 + i])); }
			pLE->m_strUniqueId += sTmp;
			sTmp.Format(_T("/%d"), HIBYTE(pnRBuf[77 + i]));
			pLE->m_strUniqueId += sTmp;
		}

		AscToString(szAscii, &pnRBuf[79], 6);	TrimSpace(szAscii);		pLE->m_strPairHPanelID	= szAscii;	
		AscToString(szAscii, &pnRBuf[85], 6);	TrimSpace(szAscii);		pLE->m_strPairEPanelID	= szAscii;	

		for (int i = 0; i < 16; i++) { szAscii[i + 0 ] = (pnRBuf[91] & (0x0001 << i)) ? '1' : '0'; }
		for (int i = 0; i < 16; i++) { szAscii[i + 16] = (pnRBuf[92] & (0x0001 << i)) ? '1' : '0'; }
		for (int i = 0; i < 16; i++) { szAscii[i + 32] = (pnRBuf[93] & (0x0001 << i)) ? '1' : '0'; }
		szAscii[48] = '\0';
		pLE->m_strPairGrade = szAscii;

		for (int i = 0; i < 2; i++) {
			if (i == 0) { sTmp.Format(_T("%d"), LOBYTE(pnRBuf[94 + i])); }
			if (i != 0) { sTmp.Format(_T("/%d"), LOBYTE(pnRBuf[94 + i])); }
			pLE->m_strPairUniqueId += sTmp;
			sTmp.Format(_T("/%d"), HIBYTE(pnRBuf[94 + i]));
			pLE->m_strPairUniqueId += sTmp;
		}

		pLE->m_strFlowRecipe.Format(_T("%d"), pnRBuf[96]);
		AscToString(szAscii, &pnRBuf[97], 6);	TrimSpace(szAscii);		pLE->m_strReserved0	= szAscii;	
		pLE->m_strBitsSignals.Format(_T("%d"), pnRBuf[103]);
		lValue = ((unsigned short)pnRBuf[105] << 16) + ((unsigned short)pnRBuf[104]);
		pLE->m_strReferData.Format(_T("%d"), lValue);

/* sm
		for (int i = 0; i < GetPLCItemLen(eCSIF_LowerEq); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}	
		HexToBin(sTmp, pLE->m_strCSIF);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eAS_LowerEq); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}				
		HexToBin(sTmp, pLE->m_strAS);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eAPS_LowerEq); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}				
		HexToBin(sTmp, pLE->m_strAPS);
		sTmp = "";
*/
#else
		AscToString(szAscii, &pnRBuf[0], 10);	TrimSpace(szAscii);		pLE->m_strHPanelID	= szAscii;	
		AscToString(szAscii, &pnRBuf[10], 10);	TrimSpace(szAscii);		pLE->m_strEPanelID	= szAscii;	
		AscToString(szAscii, &pnRBuf[20], 6);	TrimSpace(szAscii);		pLE->m_strBatchID	= szAscii;	
		AscToString(szAscii, &pnRBuf[26], 1);	TrimSpace(szAscii);		pLE->m_strProdType	= szAscii;	
		AscToString(szAscii, &pnRBuf[27], 1);	TrimSpace(szAscii);		pLE->m_strProdKind	= szAscii;	
		AscToString(szAscii, &pnRBuf[28], 9);	TrimSpace(szAscii);		pLE->m_strDeviceID	= szAscii;	
		AscToString(szAscii, &pnRBuf[37], 5);	TrimSpace(szAscii);		pLE->m_strStepID	= szAscii;	
		AscToString(szAscii, &pnRBuf[42], 8);	TrimSpace(szAscii);		pLE->m_strPPID		= szAscii;		
		pLE->m_strThickness.Format(_T("%d"), pnRBuf[50]);
		AscToString(szAscii, &pnRBuf[51], 1);	TrimSpace(szAscii);		pLE->m_strInsFlag	= szAscii;		

		pLE->m_strPanelSize.Format(_T("%d/%d"), pnRBuf[52], pnRBuf[53]);
		AscToString(szAscii, &pnRBuf[54], 1);	TrimSpace(szAscii);		pLE->m_strPanelPosition	= szAscii;	
		AscToString(szAscii, &pnRBuf[55], 1);	TrimSpace(szAscii);		pLE->m_strCount1	= szAscii;	
		AscToString(szAscii, &pnRBuf[56], 1);	TrimSpace(szAscii);		pLE->m_strCount2	= szAscii;	
		
		for (int i = 0; i < 16; i++) { szAscii[i + 0 ] = (pnRBuf[57] & (0x0001 << i)) ? '1' : '0'; }
		for (int i = 0; i < 16; i++) { szAscii[i + 16] = (pnRBuf[58] & (0x0001 << i)) ? '1' : '0'; }
		for (int i = 0; i < 16; i++) { szAscii[i + 32] = (pnRBuf[59] & (0x0001 << i)) ? '1' : '0'; }
		szAscii[57] = '\0';
		pLE->m_strGrade = szAscii;

		AscToString(szAscii, &pnRBuf[60], 8);	TrimSpace(szAscii);		pLE->m_strComment	= szAscii;	
		pLE->m_strCompSize.Format(_T("%d"), pnRBuf[68]);
		AscToString(szAscii, &pnRBuf[69], 1);	TrimSpace(szAscii);		pLE->m_strReadingFlag	= szAscii;	
		pLE->m_strPanelState.Format(_T("%d"), pnRBuf[70]);
		AscToString(szAscii, &pnRBuf[71], 2);	TrimSpace(szAscii);		pLE->m_strJudgement	= szAscii;	
		AscToString(szAscii, &pnRBuf[73], 2);	TrimSpace(szAscii);		pLE->m_strCode	= szAscii;	

		for (int i = 0; i < 10; i++ ) {
			if ( i == 0) { sTmp.Format(_T("%d"), LOBYTE(pnRBuf[75 + i])); }
			if ( i != 0) { sTmp.Format(_T("/%d"), LOBYTE(pnRBuf[75 + i])); }
			pLE->m_strRunLine += sTmp;
			sTmp.Format(_T("/%d"), HIBYTE(pnRBuf[75 + i]));
			pLE->m_strRunLine += sTmp;
		}

		for (int i = 0; i < 2; i++) {
			if (i == 0) { sTmp.Format(_T("%d"), LOBYTE(pnRBuf[85 + i])); }
			if (i != 0) { sTmp.Format(_T("/%d"), LOBYTE(pnRBuf[85 + i])); }
			pLE->m_strUniqueId += sTmp;
			sTmp.Format(_T("/%d"), HIBYTE(pnRBuf[85 + i]));
			pLE->m_strUniqueId += sTmp;
		}

		AscToString(szAscii, &pnRBuf[87], 10);	TrimSpace(szAscii);		pLE->m_strPairHPanelID	= szAscii;	
		AscToString(szAscii, &pnRBuf[97], 10);	TrimSpace(szAscii);		pLE->m_strPairEPanelID	= szAscii;	

		for (int i = 0; i < 16; i++) { szAscii[i + 0 ] = (pnRBuf[107] & (0x0001 << i)) ? '1' : '0'; }
		for (int i = 0; i < 16; i++) { szAscii[i + 16] = (pnRBuf[108] & (0x0001 << i)) ? '1' : '0'; }
		for (int i = 0; i < 16; i++) { szAscii[i + 32] = (pnRBuf[109] & (0x0001 << i)) ? '1' : '0'; }
		szAscii[107] = '\0';
		pLE->m_strPairGrade = szAscii;

		for (int i = 0; i < 2; i++) {
			if (i == 0) { sTmp.Format(_T("%d"), LOBYTE(pnRBuf[110 + i])); }
			if (i != 0) { sTmp.Format(_T("/%d"), LOBYTE(pnRBuf[110 + i])); }
			pLE->m_strPairUniqueId += sTmp;
			sTmp.Format(_T("/%d"), HIBYTE(pnRBuf[110 + i]));
			pLE->m_strPairUniqueId += sTmp;
		}

		pLE->m_strFlowRecipe.Format(_T("%d"), pnRBuf[112]);
//		AscToString(szAscii, &pnRBuf[97], 6);	TrimSpace(szAscii);		pLE->m_strReserved0	= szAscii;	
		pLE->m_strBitsSignals.Format(_T("%d"), pnRBuf[113]);
		lValue = ((unsigned short)pnRBuf[115] << 16) + ((unsigned short)pnRBuf[114]);
		pLE->m_strReferData.Format(_T("%d"), lValue);
#endif
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetToUpperEqWord()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::GetToUpperEqWord(MToUpperEqs *pUE, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	CString	sTmp="";
	
	GetPLCAddressWord_Unit(&nAddr, &nPoints, true, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		for (int i = 0; i < GetPLCItemLen(eReadingGlassID_UpperEq); i++){
			pUE->m_strReadingGlassID += MakeString(pnRBuf[nIndex++]);
		}						
		for (int i = 0; i < GetPLCItemLen(eReadingGlassIDType_UpperEq); i++){
			pUE->m_strReadingGlassIDType += MakeString(pnRBuf[nIndex++]);
		}					
		for (int i = 0; i < GetPLCItemLen(eGlassType_UpperEq); i++){
			pUE->m_strGlassType += MakeString(pnRBuf[nIndex++]);
		}						
		for (int i = 0; i < GetPLCItemLen(eReceiveRefuseCode_UpperEq); i++){
			pUE->m_strReceiveRefuseCode += MakeString(pnRBuf[nIndex++]);
		}						
		for (int i = 0; i < GetPLCItemLen(eReserved_UpperEq); i++){
			pUE->m_strReserved += MakeString(pnRBuf[nIndex++]);
		}
/*
		for (int i = 0; i < GetPLCItemLen(eCSIF_UpperEq); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}	
		HexToBin(sTmp, pUE->m_strCSIF);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eAS_UpperEq); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}				
		HexToBin(sTmp, pUE->m_strAS);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eAPS_UpperEq); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}				
		HexToBin(sTmp, pUE->m_strAPS);
		sTmp = "";
*/
  }
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::MakeString()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
CString MNetH::MakeString(short nValue, bool bSwap)
{
	CString		sRtnStr="";
	char		szTemp[2] = {0, };
	int			nTemp = 0;

	if (nValue==0){
		if (bSwap)
			return "    ";
		else
			return "0000";
	}
	szTemp[1] = nValue / 0x100;		nTemp = szTemp[1];
	szTemp[0] = nValue - (nTemp * 0x100);
	if (bSwap==true){
		sRtnStr += szTemp[0]; 
		sRtnStr += szTemp[1];
	}
	else{
		sRtnStr.Format(_T("%02X%02X"), szTemp[1], szTemp[0]);
	}

	return sRtnStr;
}


/*========================================================================================
	FUNCTION : MNetH::GetEqStateWord_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::GetEqStateWord_EO(MEqStates *pES, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	CString	sTmp="";
	
	GetPLCAddressWord_EO(&nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		for (int i = 0; i < GetPLCItemLen(eEqState_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}						
		HexToDec(sTmp, pES->m_strEqState);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eProcState_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}					
		HexToDec(sTmp, pES->m_strProcState);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(ePPID_EqState); i++){
			pES->m_strPPID += MakeString(pnRBuf[nIndex++]);
		}						
		for (int i = 0; i < GetPLCItemLen(eGlassSize_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}					
		HexToDec(sTmp, pES->m_strGlassSize);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eGlassThickness_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}	
		HexToDec(sTmp, pES->m_strGlassThickness);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eTACTSet_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}				
		HexToDec(sTmp, pES->m_strTACTSet);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eTACTCurrent_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}				
		HexToDec(sTmp, pES->m_strTACTCurrent);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eRCDG_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++]);
		}						
		HexToDec(sTmp, pES->m_strRCDG);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eEOMode_EqState); i++){
			pES->m_strEOMode += MakeString(pnRBuf[nIndex++]);
		}					
		for (int i = 0; i < GetPLCItemLen(eERCMode_EqState); i++){
			pES->m_strERCMode += MakeString(pnRBuf[nIndex++]);
		}						
		for (int i = 0; i < GetPLCItemLen(eGMCMode_EqState); i++){
			pES->m_strGMCMode += MakeString(pnRBuf[nIndex++]);
		}					
		for (int i = 0; i < GetPLCItemLen(eOperateMode_EqState); i++){
			pES->m_strOperateMode += MakeString(pnRBuf[nIndex++]);
		}	
		for (int i = 0; i < GetPLCItemLen(eReserved_EqState); i++){
			pES->m_strReserved += MakeString(pnRBuf[nIndex++]);
		}				
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetEqStateWord_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::GetEqStateWord_ER(MEqStates *pES, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	CString	sTmp="";
	
	GetPLCAddressWord_ER(&nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		for (int i = 0; i < GetPLCItemLen(eEqState_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}						
		HexToDec(sTmp, pES->m_strEqState);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eProcState_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}					
		HexToDec(sTmp, pES->m_strProcState);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(ePPID_EqState); i++){
			pES->m_strPPID += MakeString(pnRBuf[nIndex++]);
		}						
		for (int i = 0; i < GetPLCItemLen(eGlassSize_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}					
		HexToDec(sTmp, pES->m_strGlassSize);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eGlassThickness_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}	
		HexToDec(sTmp, pES->m_strGlassThickness);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eTACTSet_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}				
		HexToDec(sTmp, pES->m_strTACTSet);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eTACTCurrent_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}				
		HexToDec(sTmp, pES->m_strTACTCurrent);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eRCDG_EqState); i++){
			sTmp += MakeString(pnRBuf[nIndex++], false);
		}						
		HexToDec(sTmp, pES->m_strRCDG);
		sTmp = "";
		for (int i = 0; i < GetPLCItemLen(eEOMode_EqState); i++){
			pES->m_strEOMode += MakeString(pnRBuf[nIndex++]);
		}					
		for (int i = 0; i < GetPLCItemLen(eERCMode_EqState); i++){
			pES->m_strERCMode += MakeString(pnRBuf[nIndex++]);
		}						
		for (int i = 0; i < GetPLCItemLen(eGMCMode_EqState); i++){
			pES->m_strGMCMode += MakeString(pnRBuf[nIndex++]);
		}					
		for (int i = 0; i < GetPLCItemLen(eOperateMode_EqState); i++){
			pES->m_strOperateMode += MakeString(pnRBuf[nIndex++]);
		}	
		for (int i = 0; i < GetPLCItemLen(eReserved_EqState); i++){
			pES->m_strReserved += MakeString(pnRBuf[nIndex++]);
		}				
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetToUpperEqWord()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::SetToUpperEqWord(MToUpperEqs *pUE, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	short	nIndex=0;
	short	nItemLen=0;
	CString sTmp="";
	
	GetPLCAddressWord_Unit(&nAddr, &nPoints, true, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);


	nItemLen = GetPLCItemLen(eReadingGlassID_UpperEq); 
	MakeShortArray(pUE->m_strReadingGlassID, pnRBuf, nItemLen, 0); nIndex +=nItemLen; 
	
	nItemLen = GetPLCItemLen(eReadingGlassIDType_UpperEq); 
	MakeShortArray(pUE->m_strReadingGlassIDType, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eGlassType_UpperEq); 
	MakeShortArray(pUE->m_strGlassType, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eReceiveRefuseCode_UpperEq); 
	MakeShortArray(pUE->m_strReceiveRefuseCode, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;

	nItemLen = GetPLCItemLen(eReserved_UpperEq); 
	MakeShortArray(pUE->m_strReserved, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
/*
	nItemLen = GetPLCItemLen(eCSIF_UpperEq);
	BinToHex(pUE->m_strCSIF, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eAS_UpperEq); 
	BinToHex(pUE->m_strAS, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eAPS_UpperEq); 
	BinToHex(pUE->m_strAPS, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
*/
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::MakeShortArray()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::MakeShortArray(CString sData, short *pnArray, short nLen, short nIndex, bool bSwap)
{
	WCHAR		*psTemp;
	int			nTemp1=0, nTemp2=0;
	int			nType=0, nQty=0;
	int			nStep=0;
	CString		sStr=_T("");

	// Check Data Length.
	if ((nLen*2)<sData.GetLength()){ sStr = sData.Left(nLen*2);}
	else						  { sStr = sData; }

	psTemp = new WCHAR[(nLen*2)+1];
	memset(psTemp, NULL, sizeof(WCHAR)*((nLen*2)+1));
	if (bSwap)
		memset(psTemp, 0x20, sizeof(WCHAR)*((nLen*2)+1));
	//strcpy(psTemp, sStr);
	_tcscpy(psTemp, sStr);
	if ((nLen*2)>sStr.GetLength())
	{
		if (bSwap)
			psTemp[sStr.GetLength()] = 0x20;
	}

	nType=sStr.GetLength()%2; nQty=sStr.GetLength()/2;
	if (nType==0){
		for (int i = 0; i < nQty; i++){
			nTemp1 = psTemp[nStep]; nTemp2 = psTemp[nStep+1];
			if (bSwap==true){
				pnArray[nIndex++] = nTemp2 * 0x100 + nTemp1;
			}
			else{
				pnArray[nIndex++] = nTemp1 * 0x100 + nTemp2;
			}
			nStep += 2;
		}
	}
	else{
		for (int i = 0; i < nQty; i++){
			nTemp1 = psTemp[nStep]; nTemp2 = psTemp[nStep+1];
			if (bSwap==true){
				pnArray[nIndex++] = nTemp2 * 0x100 + nTemp1;
			}
			else{
				pnArray[nIndex++] = nTemp1 * 0x100 + nTemp2;
			}
			nStep += 2;
		}
		nTemp1 = psTemp[nStep]; nTemp2 = 0;
		if (bSwap==true){
			pnArray[nIndex++] = nTemp2 * 0x100 + nTemp1;
		}
		else{
			pnArray[nIndex++] = nTemp1 * 0x100 + nTemp2;
		}
	}
	delete [] psTemp; psTemp = NULL;
	return 0;
}

/*========================================================================================
	FUNCTION : MNetH::SetToLowerEqWord()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2006/02/07, cha
========================================================================================*/
long MNetH::SetToLowerEqWord(MToLowerEqs *pLE, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	short	nIndex=0;
	short	nItemLen=0;
	CString	sTmp="";
	short   nStart = 0;
	short   nCount = 0;
	short   i = 0;
	WCHAR	szAscii[64];

	// 2009.04.07 SESL_CYJ
//	memset(szAscii, 0, sizeof(szAscii));//아래에서 개별 사용시 초기화
	
	GetPLCAddressWord_Unit(&nAddr, &nPoints, false, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
#ifndef DEF_MELSEC_VER_2014
	// 2009.04.07 SESL_CYJ 아래에서 szAscii 개별 사용시 초기화, 앞서 사용한 변수의 크기가 더 크면 Data가 없는 경우 FillSpace 후 뒷부분에 Garbage(앞 변수 뒷자리)가 남아 쌓임.
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strHPanelID);	FillSpace(szAscii, 12);	StringToAsc(szAscii, &pnRBuf[0], 6);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strEPanelID);	FillSpace(szAscii, 12);	StringToAsc(szAscii, &pnRBuf[6], 6);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strBatchID);	FillSpace(szAscii, 12);	StringToAsc(szAscii, &pnRBuf[12], 6);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strProdType);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[18], 1);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strProdKind);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[19], 1);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strDeviceID);	FillSpace(szAscii, 18);	StringToAsc(szAscii, &pnRBuf[20], 9);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strStepID);	FillSpace(szAscii, 10);	StringToAsc(szAscii, &pnRBuf[29], 5);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strPPID);		FillSpace(szAscii, 16);	StringToAsc(szAscii, &pnRBuf[34], 8);
	//pnRBuf[42] = atoi(pLE->m_strThickness);
	pnRBuf[42] = _wtoi(pLE->m_strThickness);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strInsFlag);FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[43], 1);
	
	nStart = 0;	nCount = 0;
	for (int i = 0; i < 2 ; i++) {
		nCount = pLE->m_strPanelSize.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strPanelSize.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strPanelSize.Mid(nStart, pLE->m_strPanelSize.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		//pnRBuf[44 + i] = atoi(sTmp);
		pnRBuf[44 + i] = _wtoi(sTmp);
	}
	
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strPanelPosition);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[46], 1);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strCount1);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[47], 1);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strCount2);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[48], 1);
	
	swprintf(szAscii, pLE->m_strGrade);
	for (int i = 0; i < 16; i++) { pnRBuf[49] += (szAscii[i + 0 ] == '1') ? (0x0001 << i) : 0 ; }
	for (int i = 0; i < 16; i++) { pnRBuf[50] += (szAscii[i + 16 ] == '1') ? (0x0001 << i) : 0 ; }
	for (int i = 0; i < 16; i++) { pnRBuf[51] += (szAscii[i + 32 ] == '1') ? (0x0001 << i) : 0 ; }

	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strComment);	FillSpace(szAscii, 16);	StringToAsc(szAscii, &pnRBuf[52], 8);
	//pnRBuf[60] = atoi(pLE->m_strCompSize);
	pnRBuf[60] = _wtoi(pLE->m_strCompSize);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strReadingFlag);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[61], 1);
	//pnRBuf[62] = atoi(pLE->m_strPanelState);
	pnRBuf[62] = _wtoi(pLE->m_strPanelState);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strJudgement);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[63], 2);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strCode);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[65], 2);

	// NSMC 2009.04.07 SESL_CYJ 탕정 NSMC 진행시 RunLine 값을 Binary에서 Ascii로 Spec 변경, 다른 사이트는 현재까지 아님. 

	//	 20081111 JDY NSMC 
	nStart = 0;	nCount = 0;
	for (int i = 0; i < 10 ; i++) {
		nCount = pLE->m_strRunLine.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strRunLine.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strRunLine.Mid(nStart, pLE->m_strRunLine.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		pnRBuf[67 + i] = _wtoi(sTmp);

		nCount = pLE->m_strRunLine.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strRunLine.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strRunLine.Mid(nStart, pLE->m_strRunLine.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		pnRBuf[67 + i] += _wtoi(sTmp) << 8;
	}

	nStart = 0;	nCount = 0;
	for (int i = 0; i < 2 ; i++) {
		nCount = pLE->m_strUniqueId.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strUniqueId.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strUniqueId.Mid(nStart, pLE->m_strUniqueId.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		//pnRBuf[77 + i] = atoi(sTmp);
		pnRBuf[77 + i] = _wtoi(sTmp);

		nCount = pLE->m_strUniqueId.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strUniqueId.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strUniqueId.Mid(nStart, pLE->m_strUniqueId.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		//pnRBuf[77 + i] += atoi(sTmp) << 8;
		pnRBuf[77 + i] += _wtoi(sTmp) << 8;
	}

	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strPairHPanelID);	FillSpace(szAscii, 12);	StringToAsc(szAscii, &pnRBuf[79], 6);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strPairEPanelID);	FillSpace(szAscii, 12);	StringToAsc(szAscii, &pnRBuf[85], 6);
	
	swprintf(szAscii, pLE->m_strPairGrade);
	for (int i = 0; i < 16; i++) { pnRBuf[91] += (szAscii[i + 0 ] == '1') ? (0x0001 << i) : 0 ; }
	for (int i = 0; i < 16; i++) { pnRBuf[92] += (szAscii[i + 16 ] == '1') ? (0x0001 << i) : 0 ; }
	for (int i = 0; i < 16; i++) { pnRBuf[93] += (szAscii[i + 32 ] == '1') ? (0x0001 << i) : 0 ; }

	nStart = 0;	nCount = 0;
	for (int i = 0; i < 2 ; i++) {
		nCount = pLE->m_strPairUniqueId.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strPairUniqueId.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strPairUniqueId.Mid(nStart, pLE->m_strPairUniqueId.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		//pnRBuf[94 + i] = atoi(sTmp);
		pnRBuf[94 + i] = _wtoi(sTmp);

		nCount = pLE->m_strPairUniqueId.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strPairUniqueId.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strPairUniqueId.Mid(nStart, pLE->m_strPairUniqueId.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		//pnRBuf[94 + i] += atoi(sTmp) << 8;
		pnRBuf[94 + i] += _wtoi(sTmp) << 8;
	}

	//pnRBuf[96] = atoi(pLE->m_strFlowRecipe);
	pnRBuf[96] = _wtoi(pLE->m_strFlowRecipe);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strReserved0);	FillSpace(szAscii, 12);	StringToAsc(szAscii, &pnRBuf[97], 6);
	//pnRBuf[103] = atoi(pLE->m_strBitsSignals);
	//pnRBuf[104] = atoi(pLE->m_strReferData) & 0xFFFF;
	//pnRBuf[105] = (atoi(pLE->m_strReferData) >> 16) & 0xFFFF;
	pnRBuf[103] = _wtoi(pLE->m_strBitsSignals);
	pnRBuf[104] = _wtoi(pLE->m_strReferData) & 0xFFFF;
	pnRBuf[105] = (_wtoi(pLE->m_strReferData) >> 16) & 0xFFFF;
/*
	nItemLen = GetPLCItemLen(eCSIF_LowerEq); 
	BinToHex(pLE->m_strCSIF, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;

	nItemLen = GetPLCItemLen(eAS_LowerEq); 
	BinToHex(pLE->m_strAS, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;

	nItemLen = GetPLCItemLen(eAPS_LowerEq); 
	BinToHex(pLE->m_strAPS, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
*/
#else // melsec 2014 Ver.

	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strHPanelID);	FillSpace(szAscii, 20);	StringToAsc(szAscii, &pnRBuf[0], 10);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strEPanelID);	FillSpace(szAscii, 20);	StringToAsc(szAscii, &pnRBuf[10], 10);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strBatchID);	FillSpace(szAscii, 12);	StringToAsc(szAscii, &pnRBuf[20], 6);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strProdType);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[26], 1);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strProdKind);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[27], 1);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strDeviceID);	FillSpace(szAscii, 18);	StringToAsc(szAscii, &pnRBuf[28], 9);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strStepID);	FillSpace(szAscii, 10);	StringToAsc(szAscii, &pnRBuf[37], 5);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strPPID);		FillSpace(szAscii, 16);	StringToAsc(szAscii, &pnRBuf[42], 8);
	//pnRBuf[42] = atoi(pLE->m_strThickness);
	pnRBuf[50] = _wtoi(pLE->m_strThickness);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strInsFlag);FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[51], 1);
	
	nStart = 0;	nCount = 0;
	for (int i = 0; i < 2 ; i++) {
		nCount = pLE->m_strPanelSize.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strPanelSize.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strPanelSize.Mid(nStart, pLE->m_strPanelSize.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		//pnRBuf[44 + i] = atoi(sTmp);
		pnRBuf[52 + i] = _wtoi(sTmp);
	}
	
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strPanelPosition);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[54], 1);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strCount1);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[55], 1);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strCount2);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[56], 1);
	
	swprintf(szAscii, pLE->m_strGrade);
	for (int i = 0; i < 16; i++) { pnRBuf[57] += (szAscii[i + 0 ] == '1') ? (0x0001 << i) : 0 ; }
	for (int i = 0; i < 16; i++) { pnRBuf[58] += (szAscii[i + 16 ] == '1') ? (0x0001 << i) : 0 ; }
	for (int i = 0; i < 16; i++) { pnRBuf[59] += (szAscii[i + 32 ] == '1') ? (0x0001 << i) : 0 ; }

	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strComment);	FillSpace(szAscii, 16);	StringToAsc(szAscii, &pnRBuf[60], 8);
	//pnRBuf[60] = atoi(pLE->m_strCompSize);
	pnRBuf[68] = _wtoi(pLE->m_strCompSize);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strReadingFlag);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[69], 1);
	//pnRBuf[62] = atoi(pLE->m_strPanelState);
	pnRBuf[70] = _wtoi(pLE->m_strPanelState);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strJudgement);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[71], 2);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strCode);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[73], 2);

	nStart = 0;	nCount = 0;
	for (int i = 0; i < 10 ; i++) {
		nCount = pLE->m_strRunLine.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strRunLine.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strRunLine.Mid(nStart, pLE->m_strRunLine.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		pnRBuf[75 + i] = _wtoi(sTmp);

		nCount = pLE->m_strRunLine.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strRunLine.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strRunLine.Mid(nStart, pLE->m_strRunLine.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		pnRBuf[75 + i] += _wtoi(sTmp) << 8;
	}

	nStart = 0;	nCount = 0;
	for (int i = 0; i < 2 ; i++) {
		nCount = pLE->m_strUniqueId.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strUniqueId.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strUniqueId.Mid(nStart, pLE->m_strUniqueId.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		//pnRBuf[77 + i] = atoi(sTmp);
		pnRBuf[85 + i] = _wtoi(sTmp);

		nCount = pLE->m_strUniqueId.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strUniqueId.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strUniqueId.Mid(nStart, pLE->m_strUniqueId.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		//pnRBuf[77 + i] += atoi(sTmp) << 8;
		pnRBuf[85 + i] += _wtoi(sTmp) << 8;
	}

	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strPairHPanelID);	FillSpace(szAscii, 20);	StringToAsc(szAscii, &pnRBuf[87], 10);
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strPairEPanelID);	FillSpace(szAscii, 20);	StringToAsc(szAscii, &pnRBuf[97], 10);
	
	swprintf(szAscii, pLE->m_strPairGrade);
	for (int i = 0; i < 16; i++) { pnRBuf[107] += (szAscii[i + 0 ] == '1') ? (0x0001 << i) : 0 ; }
	for (int i = 0; i < 16; i++) { pnRBuf[108] += (szAscii[i + 16 ] == '1') ? (0x0001 << i) : 0 ; }
	for (int i = 0; i < 16; i++) { pnRBuf[109] += (szAscii[i + 32 ] == '1') ? (0x0001 << i) : 0 ; }

	nStart = 0;	nCount = 0;
	for (int i = 0; i < 2 ; i++) {
		nCount = pLE->m_strPairUniqueId.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strPairUniqueId.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strPairUniqueId.Mid(nStart, pLE->m_strPairUniqueId.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		//pnRBuf[94 + i] = atoi(sTmp);
		pnRBuf[110 + i] = _wtoi(sTmp);

		nCount = pLE->m_strPairUniqueId.Find('/', nCount);
		if (nCount != -1) {	sTmp = pLE->m_strPairUniqueId.Mid(nStart, nCount - nStart); } 
		if (nCount == -1) {	sTmp = pLE->m_strPairUniqueId.Mid(nStart, pLE->m_strPairUniqueId.GetLength() - nStart); } 
		nCount++; nStart = nCount;
		//pnRBuf[94 + i] += atoi(sTmp) << 8;
		pnRBuf[110 + i] += _wtoi(sTmp) << 8;
	}

	//pnRBuf[96] = atoi(pLE->m_strFlowRecipe);
	pnRBuf[112] = _wtoi(pLE->m_strFlowRecipe);
	//170906 Reserved는 무엇인가????
	/*
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strReserved0);	FillSpace(szAscii, 12);	StringToAsc(szAscii, &pnRBuf[113], 6);
	*/
	//pnRBuf[103] = atoi(pLE->m_strBitsSignals);
	//pnRBuf[104] = atoi(pLE->m_strReferData) & 0xFFFF;
	//pnRBuf[105] = (atoi(pLE->m_strReferData) >> 16) & 0xFFFF;
	pnRBuf[113] = _wtoi(pLE->m_strBitsSignals);
	pnRBuf[114] = _wtoi(pLE->m_strReferData) & 0xFFFF;
	pnRBuf[115] = (_wtoi(pLE->m_strReferData) >> 16) & 0xFFFF;
/*
	nItemLen = GetPLCItemLen(eCSIF_LowerEq); 
	BinToHex(pLE->m_strCSIF, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;

	nItemLen = GetPLCItemLen(eAS_LowerEq); 
	BinToHex(pLE->m_strAS, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;

	nItemLen = GetPLCItemLen(eAPS_LowerEq); 
	BinToHex(pLE->m_strAPS, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
*/
#endif
	
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}


/*========================================================================================
	FUNCTION : MNetH::SetGlassSummaryWord()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/11/26, James Park
========================================================================================*/
long MNetH::SetGlassSummaryWord(MGlassSummarys *pGS, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	short	nIndex=0;
	short	nItemLen=0;
	CString	sTmp="";
	
	GetPLCAddressWord_GS(&nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	//20111206 SJ_HJG 

	/*
	nItemLen = GetPLCItemLen(eTGlassCount_GlassSum); 
	DecToHex(pGS->m_strTGlassCount, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, 0, false); nIndex +=nItemLen; 

	nItemLen = GetPLCItemLen(eTFTCount_GlassSum); 
	DecToHex(pGS->m_strTFTCount, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;

	nItemLen = GetPLCItemLen(eCFCount_GlassSum ); 
	DecToHex(pGS->m_strCFCount, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eDummyCount_GlassSum ); 
	DecToHex(pGS->m_strDummyCount, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eBPGCount_GlassSum); 
	DecToHex(pGS->m_strBPGCount, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eProcessID1_GlassSum); 
	MakeShortArray(pGS->m_strProcessID1, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eStepID1_GlassSum); 
	MakeShortArray(pGS->m_strStepID1, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eBatchID1_GlassSum); 
	MakeShortArray(pGS->m_strBatchID1, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen; 
	
	nItemLen = GetPLCItemLen(eGlassCount1_GlassSum); 
	DecToHex(pGS->m_strGlassCount1, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eProcessID2_GlassSum); 
	MakeShortArray(pGS->m_strProcessID2, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eStepID2_GlassSum); 
	MakeShortArray(pGS->m_strStepID2, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eBatchID2_GlassSum); 
	MakeShortArray(pGS->m_strBatchID2, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen; 
	
	nItemLen = GetPLCItemLen(eGlassCount2_GlassSum); 
	DecToHex(pGS->m_strGlassCount2, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eProcessID3_GlassSum); 
	MakeShortArray(pGS->m_strProcessID3, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eStepID3_GlassSum); 
	MakeShortArray(pGS->m_strStepID3, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eBatchID3_GlassSum); 
	MakeShortArray(pGS->m_strBatchID3, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen; 
	
	nItemLen = GetPLCItemLen(eGlassCount3_GlassSum); 
	DecToHex(pGS->m_strGlassCount3, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;

	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	*/
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetEqStateWord_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::SetEqStateWord_EO(MEqStates *pES, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	short	nIndex=0;
	short	nItemLen=0;
	CString	sTmp="";
	
	GetPLCAddressWord_EO(&nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	nItemLen = GetPLCItemLen(eEqState_EqState); 
	DecToHex(pES->m_strEqState, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, 0, false); nIndex +=nItemLen; 

	nItemLen = GetPLCItemLen(eProcState_EqState); 
	DecToHex(pES->m_strProcState, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;

	nItemLen = GetPLCItemLen(ePPID_EqState); 
	MakeShortArray(pES->m_strPPID, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eGlassSize_EqState); 
	DecToHex(pES->m_strGlassSize, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eGlassThickness_EqState); 
	DecToHex(pES->m_strGlassThickness, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eTACTSet_EqState); 
	DecToHex(pES->m_strTACTSet, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eTACTCurrent_EqState); 
	DecToHex(pES->m_strTACTCurrent, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eRCDG_EqState); 
	DecToHex(pES->m_strRCDG, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen; 
	
	nItemLen = GetPLCItemLen(eEOMode_EqState); 
	MakeShortArray(pES->m_strEOMode, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eERCMode_EqState); 
	MakeShortArray(pES->m_strERCMode, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eGMCMode_EqState); 
	MakeShortArray(pES->m_strGMCMode, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eOperateMode_EqState); 
	MakeShortArray(pES->m_strOperateMode, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen; 
	
	nItemLen = GetPLCItemLen(eReserved_EqState); 
	MakeShortArray(pES->m_strReserved, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;


	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}


/*========================================================================================
	FUNCTION : MNetH::SetEqStateWord_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::SetEqStateWord_ER(MEqStates *pES, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	short	nIndex=0;
	short	nItemLen=0;
	CString	sTmp="";
	
	GetPLCAddressWord_ER(&nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	nItemLen = GetPLCItemLen(eEqState_EqState); 
	DecToHex(pES->m_strEqState, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, 0, false); nIndex +=nItemLen; 

	nItemLen = GetPLCItemLen(eProcState_EqState); 
	DecToHex(pES->m_strProcState, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;

	nItemLen = GetPLCItemLen(ePPID_EqState); 
	MakeShortArray(pES->m_strPPID, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eGlassSize_EqState); 
	DecToHex(pES->m_strGlassSize, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eGlassThickness_EqState); 
	DecToHex(pES->m_strGlassThickness, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eTACTSet_EqState); 
	DecToHex(pES->m_strTACTSet, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eTACTCurrent_EqState); 
	DecToHex(pES->m_strTACTCurrent, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eRCDG_EqState); 
	DecToHex(pES->m_strRCDG, sTmp);
	MakeShortArray(sTmp, pnRBuf, nItemLen, nIndex, false); nIndex +=nItemLen; 
	
	nItemLen = GetPLCItemLen(eEOMode_EqState); 
	MakeShortArray(pES->m_strEOMode, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eERCMode_EqState); 
	MakeShortArray(pES->m_strERCMode, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eGMCMode_EqState); 
	MakeShortArray(pES->m_strGMCMode, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;
	
	nItemLen = GetPLCItemLen(eOperateMode_EqState); 
	MakeShortArray(pES->m_strOperateMode, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen; 
	
	nItemLen = GetPLCItemLen(eReserved_EqState); 
	MakeShortArray(pES->m_strReserved, pnRBuf, nItemLen, nIndex); nIndex +=nItemLen;


	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::WriteLB()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/07/04, KJH
========================================================================================*/
long MNetH::WriteLB(short nAddr, short nPoints, short *pnWBuf, short nBufSize)
{
	if (m_fActive == FALSE) { return -1; }
	// TODO: Add your dispatch handler code here
	short	nDev[4];
	short	nRet = 0;
	short	nCount = 0;
	short	nIndex = 0;
	unsigned short	nDigit = 0x0001;
	short	i = 0;

	nCount = nBufSize / sizeof(short);
	if (nCount < nPoints) { return -10001; }

	short	*pnBuf = new short[nCount];
	for (int i = 0; i < nCount; i++) { pnBuf[i] = 0; }

	nDev[0] = 1;
	nDev[1] = DevB;
	nDev[2] = nAddr;
	nDev[3] = nPoints;

	for (int i = 0; i < nCount; i++) {
		pnBuf[nIndex] = pnBuf[nIndex] | (pnWBuf[i] * nDigit);

		if (nDigit != 0x8000) { nDigit <<= 1; }
		else {
			nDigit = 0x0001;
			nIndex++;
		}
	}
	if (::WaitForSingleObject(m_hMutex, 1000) != WAIT_OBJECT_0) return -10002;
	nRet = mdRandW(m_lPath, m_nStation, nDev, pnBuf, nBufSize);
	delete [] pnBuf;
	::ReleaseMutex(m_hMutex);
	return nRet;
}

/*========================================================================================
	FUNCTION : MNetH::WriteLW()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/07/04, 
========================================================================================*/
long MNetH::WriteLW(short nAddr, short nPoints, short *pnWBuf, short nBufSize)
{
	if (m_fActive == FALSE) { return -1; }
	// TODO: Add your dispatch handler code here
	short	nDev[4];
	short	nRet = 0;
	short	nCount = 0;

	nCount = nBufSize / sizeof(short);
	if (nCount < nPoints) { return -10001; }

	nDev[0] = 1;
	nDev[1] = DevW;
	nDev[2] = nAddr;
	nDev[3] = nPoints;

	if (::WaitForSingleObject(m_hMutex, 1000) != WAIT_OBJECT_0) return -10002;
	nRet = mdRandW(m_lPath, m_nStation, nDev, pnWBuf, nBufSize);
	::ReleaseMutex(m_hMutex);
	return nRet;
}	

/*========================================================================================
	FUNCTION : MNetH::SetGlassData()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetGlassData(MToLowerEqs *pGD, EEqLocs eLoc)
{
	long	lRet=0;

	lRet = SetToLowerEqWord(pGD); 
	if (lRet == 0){
//		SetWorkSkip_ToLowerEqBit(pGD->m_nWorkSkip);
//		SetJobStart_ToLowerEqBit(pGD->m_nJobStart);
//		SetJobEnd_ToLowerEqBit(pGD->m_nJobEnd);
//		SetHotFlow_ToLowerEqBit(pGD->m_nHotFlow);
	}
	m_bSetGlassDataFlag = true;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGlassData()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetGlassData(MToLowerEqs *pGD, EEqLocs eLoc)
{
	long	lRet=0;

	if (eLoc == eUpperEq)
	{
		lRet = GetToLowerEqWord(pGD, m_nPrevLocal);
	}
	else
	{
		lRet = GetToLowerEqWord(pGD);
	}
	if (lRet == 0){
		if (eLoc == eUpperEq)
		{
//			GetWorkSkip_ToLowerEqBit(&pGD->m_nWorkSkip, m_nPrevLocal);
//			GetJobStart_ToLowerEqBit(&pGD->m_nJobStart, m_nPrevLocal);
//			GetJobEnd_ToLowerEqBit(&pGD->m_nJobEnd, m_nPrevLocal);
//			GetHotFlow_ToLowerEqBit(&pGD->m_nHotFlow, m_nPrevLocal);
		}
		else
		{
//			GetWorkSkip_ToLowerEqBit(&pGD->m_nWorkSkip);
//			GetJobStart_ToLowerEqBit(&pGD->m_nJobStart);
//			GetJobEnd_ToLowerEqBit(&pGD->m_nJobEnd);
//			GetHotFlow_ToLowerEqBit(&pGD->m_nHotFlow);
		}
	}
	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::MonitoringPLCArea()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
void MNetH::MonitoringPLCArea()
{
	/////////////////////////////////////////////////////////////////
	// Bit Area
	if (0==m_pDlg->GetCurSelAreaTab()){ 
		//===========================================================
		// To Upper Eqp
		if (0==m_pDlg->GetCurSelDataTab()){
			MToUpperEqBit UE;

			GetToUpperEqBits(&UE, m_pDlg->GetCurSelLocalTab()+1, m_pDlg->GetCurSelUnitTab()+1);
			m_pDlg->UpdateBitData(&UE);
		}
		//===========================================================
		// To Lower Eqp
		else if (1==m_pDlg->GetCurSelDataTab()){
			MToLowerEqBit LE;

			GetToLowerEqBits(&LE, m_pDlg->GetCurSelLocalTab()+1, m_pDlg->GetCurSelUnitTab()+1);
			m_pDlg->UpdateBitData(&LE);
		}
		//===========================================================
		// Contact Point State(to Upper)
		else if (2==m_pDlg->GetCurSelDataTab()){
			MRobotArmStateBit	RS;
			MStageStateBit		SS;

			GetContactPointStateBit(&RS, &SS, true, m_pDlg->GetCurSelLocalTab()+1, m_pDlg->GetCurSelUnitTab()+1);
			m_pDlg->UpdateBitData(&RS);
			m_pDlg->UpdateBitData(&SS);
		}
		//===========================================================
		// Contact Point State(to Lower)
		else if (3==m_pDlg->GetCurSelDataTab()){
			MRobotArmStateBit	RS;
			MStageStateBit		SS;
			
			GetContactPointStateBit(&RS, &SS, false, m_pDlg->GetCurSelLocalTab()+1, m_pDlg->GetCurSelUnitTab()+1);
			m_pDlg->UpdateBitData(&RS);
			m_pDlg->UpdateBitData(&SS);
		}
		else if (4==m_pDlg->GetCurSelDataTab()){
			MModelChangeBitDatas	BD;

			BD.m_iModelChangeReserve = IsOnNSMCEqBit(eModelChangeReserve,m_pDlg->GetCurSelLocalTab()+1);
			BD.m_iModelChangeStart = IsOnNSMCEqBit(eModelChangeStart,m_pDlg->GetCurSelLocalTab()+1);
			BD.m_iModelChangeComplete = IsOnNSMCEqBit(eModelChangeComplete,m_pDlg->GetCurSelLocalTab()+1);
			BD.m_iModelChangeDisable = IsOnNSMCEqBit(eModelChangeDisable,m_pDlg->GetCurSelLocalTab()+1);

			// Add. 110525 기종변경 예약 Bit 추가
			BD.m_iModelReserveComplete = IsOnNSMCEqBit(eModelReserveComplete,m_pDlg->GetCurSelLocalTab()+1);
			BD.m_iModelReserveDisable = IsOnNSMCEqBit(eModelReserveDisable,m_pDlg->GetCurSelLocalTab()+1);
			
			m_pDlg->UpdateBitData(&BD);
		}
	}

	/////////////////////////////////////////////////////////////////
	// Word Area
	else if (1==m_pDlg->GetCurSelAreaTab()){ 
		//===========================================================
		// Equipment State(Own)
		if (0==m_pDlg->GetCurSelDataTab()){
			MEqStates ES;

			GetEqStateWord_EO(&ES, m_pDlg->GetCurSelLocalTab()+1);
			m_pDlg->UpdateEqState_Own(&ES);
		}
		//===========================================================
		// Equipment State(Related)
		else if (1==m_pDlg->GetCurSelDataTab()){
			MEqStates ES;

			GetEqStateWord_ER(&ES, m_pDlg->GetCurSelLocalTab()+1);
			m_pDlg->UpdateEqState_Related(&ES);
		}
		//===========================================================
		// Job Summary
		else if (2==m_pDlg->GetCurSelDataTab()){
			MGlassSummarys GS;

			GetGlassSummaryWord(&GS, m_pDlg->GetCurSelLocalTab()+1);
			m_pDlg->UpdateJobSummary(&GS);
		}
		//===========================================================
		// To Upper Equipment
		else if (3==m_pDlg->GetCurSelDataTab()){
			MToUpperEqs UE;

			GetToUpperEqWord(&UE, m_pDlg->GetCurSelLocalTab()+1, m_pDlg->GetCurSelUnitTab()+1);
			m_pDlg->UpdateUpperEqData(&UE);
		}
		//===========================================================
		// To Lower Equipment
		else if (4==m_pDlg->GetCurSelDataTab()){
			MToLowerEqs LE;
			
			GetToLowerEqWord(&LE, m_pDlg->GetCurSelLocalTab()+1, m_pDlg->GetCurSelUnitTab()+1);
			m_pDlg->UpdateLowerEqData(&LE);
		}
		else if (5==m_pDlg->GetCurSelDataTab()){
			MModelChangeWordDatas WD;

			GetNSMCWord(&WD, m_pDlg->GetCurSelLocalTab()+1);
			
			m_pDlg->UpdateModelChangeWord(&WD);
		}
		//SJ_YYK 150821 Add...
		else if(6 == m_pDlg->GetCurSelDataTab()){
			MMaterial	Mat;
			GetMaterialInfo(&Mat, m_pDlg->GetCurSelLocalTab()+1, m_pDlg->GetCurSelUnitTab()+1);

			m_pDlg->UpdateMaterialInfo(&Mat);
		}
		else if(7 == m_pDlg->GetCurSelDataTab()){
			MProcessParamInfo	pParam;
			GetFDCInfo(&pParam, m_pDlg->GetCurSelLocalTab()+1, m_pDlg->GetCurSelUnitTab()+1);

			m_pDlg->UpdateFDCInfo(&pParam);
		}
	}
}	

/*========================================================================================
	FUNCTION : MNetH::SetReserved0_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved0_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved0_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetMachinePause_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetMachinePause_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eMachinePause_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetMachineDown_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetMachineDown_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eMachineDown_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetMachineAlarm_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetMachineAlarm_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eMachineAlarm_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetMachineAlarm_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReceiveAble_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveAble_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetReceiveStart_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReceiveStart_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveStart_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetReceiveComplete_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReceiveComplete_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveComplete_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::SetExchangeFlag_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetExchangeFlag_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eExchangeFlag_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReturnSendStart_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReturnSendStart_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReturnSendStart_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReturnSendComplete_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReturnSendComplete_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReturnSendComplete_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetImmediatelyPauseRequest_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetImmediatelyPauseRequest_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eImmediatelyPauseRequest_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::SetImmediatelyStopRequest_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetImmediatelyStopRequest_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eImmediatelyStopRequest_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReceiveAbleRemainedStep0_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReceiveAbleRemainedStep0_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveAbleRemainedStep0_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReceiveAbleRemainedStep1_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReceiveAbleRemainedStep1_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveAbleRemainedStep1_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReceiveAbleRemainedStep2_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReceiveAbleRemainedStep2_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveAbleRemainedStep2_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReceiveAbleRemainedStep3_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReceiveAbleRemainedStep3_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveAbleRemainedStep3_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetGlassIdReadComplete_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetGlassIdReadComplete_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eGlassIdReadComplete_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetLoadingStop_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetLoadingStop_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eLoadingStop_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetTransferStop_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetTransferStop_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eTransferStop_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReserved1_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved1_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved1_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReserved2_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved2_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved2_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReserved3_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved3_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved3_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReserved4_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved4_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved4_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::SetReserved5_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved5_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved5_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::SetHandShakeCancelRequest_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetHandShakeCancelRequest_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eHandShakeCancelRequest_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetHandShakeAbortRequest_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetHandShakeAbortRequest_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eHandShakeAbortRequest_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetHandShakeResumeRequest_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetHandShakeResumeRequest_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eHandShakeResumeRequest_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::SetHandShakeRecoveryAckReply_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetHandShakeRecoveryAckReply_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eHandShakeRecoveryAckReply_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetHandShakeRecoveryNckReply_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetHandShakeRecoveryNckReply_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eHandShakeRecoveryNckReply_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetAction1Complete_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::SetAction1Complete_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eAction1Complete_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetAction2Complete_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::SetAction2Complete_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eAction2Complete_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReceiveRefuse_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::SetReceiveRefuse_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveRefuse_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	


/*========================================================================================
	FUNCTION : MNetH::SetReserved0_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved0_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved0_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetMachinePause_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetMachinePause_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eMachinePause_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetMachineDown_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetMachineDown_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eMachineDown_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetMachineAlarm_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetMachineAlarm_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eMachineAlarm_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetSendAble_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetSendAble_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendAble_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetSendStart_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetSendStart_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendStart_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetSendComplete_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetSendComplete_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendComplete_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::SetExchangeFlag_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetExchangeFlag_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eExchangeFlag_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReturnReceiveStart_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReturnReceiveStart_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReturnReceiveStart_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReturnReceiveComplete_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReturnReceiveComplete_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReturnReceiveComplete_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetImmediatelyPauseRequest_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetImmediatelyPauseRequest_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eImmediatelyPauseRequest_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::SetImmediatelyStopRequest_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetImmediatelyStopRequest_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eImmediatelyStopRequest_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetSendAbleRemainedStep0_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetSendAbleRemainedStep0_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendAbleRemainedStep0_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetSendAbleRemainedStep1_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetSendAbleRemainedStep1_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendAbleRemainedStep1_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetSendAbleRemainedStep2_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetSendAbleRemainedStep2_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendAbleRemainedStep2_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetSendAbleRemainedStep3_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetSendAbleRemainedStep3_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendAbleRemainedStep3_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetWorkStart_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetWorkStart_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eWorkStart_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetWorkCancel_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetWorkCancel_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eWorkCancel_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReserved6_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved6_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved6_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReserved7_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved7_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved7_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetAction1Request_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetAction1Request_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eAction1Request_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetAction2Request_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetAction2Request_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eAction2Request_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReserved1_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved1_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved1_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::SetReserved2_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved2_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved2_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::SetHandShakeCancelRequest_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetHandShakeCancelRequest_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eHandShakeCancelRequest_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetHandShakeAbortRequest_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetHandShakeAbortRequest_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eHandShakeAbortRequest_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetHandShakeResumeRequest_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetHandShakeResumeRequest_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eHandShakeResumeRequest_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::SetHandShakeRecoveryAckReply_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetHandShakeRecoveryAckReply_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eHandShakeRecoveryAckReply_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetHandShakeRecoveryNckReply_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetHandShakeRecoveryNckReply_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eHandShakeRecoveryNckReply_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReserved3_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved3_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved3_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReserved4_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved4_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved4_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReserved5_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved5_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved5_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::SetRobotAbnormal_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetRobotAbnormal_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eRobotAbnormal_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetTypeOfArm_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetTypeOfArm_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eTypeOfArm_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetTypeOfStageConveyor_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetTypeOfStageConveyor_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eTypeOfStageConveyor_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetArm1Violate_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetArm1Violate_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm1Violate_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetArm2Violate_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetArm2Violate_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm2Violate_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetArm1FoldComplete_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetArm1FoldComplete_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm1FoldComplete_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetArm2FoldComplete_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetArm2FoldComplete_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm2FoldComplete_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetArm1GlassCheckSensor_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetArm1GlassCheckSensorOn_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm1GlassCheckSensor_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::SetArm2GlassCheckSensor_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetArm2GlassCheckSensorOn_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm2GlassCheckSensor_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReserved0_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved0_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved0_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReserved1_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved1_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved1_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReserved2_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved2_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved2_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::SetManualOperation_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetManualOperation_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eManualOperation_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetRobotDirection_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetRobotDirection_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eRobotDirection_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReserved3_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved3_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved3_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReserved4_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved4_Robot_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved4_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetRobotAbnormal_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetRobotAbnormal_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eRobotAbnormal_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetTypeOfArm_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetTypeOfArm_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eTypeOfArm_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetTypeOfStageConveyor_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetTypeOfStageConveyor_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eTypeOfStageConveyor_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetArm1Violate_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetArm1Violate_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eArm1Violate_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetArm2Violate_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetArm2Violate_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eArm2Violate_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetArm1FoldComplete_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetArm1FoldComplete_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eArm1FoldComplete_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetArm2FoldComplete_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetArm2FoldComplete_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eArm2FoldComplete_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetArm1GlassCheckSensor_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetArm1GlassCheckSensorOn_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eArm1GlassCheckSensor_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::SetArm2GlassCheckSensor_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetArm2GlassCheckSensorOn_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eArm2GlassCheckSensor_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetReserved0_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved0_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved0_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReserved1_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved1_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved1_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReserved2_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved2_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved2_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::SetManualOperation_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetManualOperation_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eManualOperation_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetRobotDirection_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetRobotDirection_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eRobotDirection_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReserved3_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved3_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved3_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetReserved4_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved4_Robot_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved4_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		
	


/*========================================================================================
	FUNCTION : MNetH::SetStageAbnormal_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetStageAbnormal_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eStageAbnormal_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetTypeOfArm_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetTypeOfArm_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eTypeOfArm_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetTypeOfStageConveyor_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetTypeOfStageConveyor_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eTypeOfStageConveyor_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetEmpty_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetEmpty_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eEmpty_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetIdle_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetIdle_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eIdle_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetBusy_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetBusy_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eBusy_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetPinUp_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetPinUp_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += ePinUp_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::SetPinDown_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetPinDown_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += ePinDown_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetStopperUp_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetStopperUp_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eStopperUp_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetStopperDown_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetStopperDown_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eStopperDown_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetDoorOpen_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetDoorOpen_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eDoorOpen_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetDoorClose_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetDoorClose_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eDoorClose_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetManualOperation_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetManualOperation_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eManualOperation_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetBodyMoving_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetBodyMoving_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eBodyMoving_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetGlassCheckSensorOn_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetGlassCheckSensorOn_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eGlassCheckSensorOn_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::SetReserved0_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved0_Stage_ToUpperEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved0_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		


/*========================================================================================
	FUNCTION : MNetH::SetStageAbnormal_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetStageAbnormal_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eStageAbnormal_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetTypeOfArm_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetTypeOfArm_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eTypeOfArm_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetTypeOfStageConveyor_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetTypeOfStageConveyor_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eTypeOfStageConveyor_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetEmpty_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetEmpty_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eEmpty_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetIdle_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetIdle_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eIdle_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetBusy_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetBusy_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eBusy_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetPinUp_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetPinUp_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += ePinUp_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::SetPinDown_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetPinDown_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += ePinDown_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::SetStopperUp_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetStopperUp_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eStopperUp_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetStopperDown_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetStopperDown_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eStopperDown_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetDoorOpen_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetDoorOpen_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eDoorOpen_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetDoorClose_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetDoorClose_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eDoorClose_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetManualOperation_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetManualOperation_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eManualOperation_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetBodyMoving_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetBodyMoving_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eBodyMoving_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::SetGlassCheckSensorOn_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetGlassCheckSensorOn_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eGlassCheckSensorOn_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::SetReserved0_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::SetReserved0_Stage_ToLowerEqBit(short nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved0_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		
	

/*========================================================================================
	FUNCTION : MNetH::GetToUpperEqBits()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::GetToUpperEqBits(MToUpperEqBit *pUE, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		pUE->m_nReserved0 = pnRBuf[nIndex++];
		pUE->m_nMachinePause = pnRBuf[nIndex++];
		pUE->m_nMachineDown = pnRBuf[nIndex++];
		pUE->m_nMachineAlarm = pnRBuf[nIndex++];	
		pUE->m_nReceiveAble = pnRBuf[nIndex++];
		pUE->m_nReceiveStart = pnRBuf[nIndex++];
		pUE->m_nReceiveComplete = pnRBuf[nIndex++];
		pUE->m_nExchangeFlag = pnRBuf[nIndex++];
		pUE->m_nReturnSendStart = pnRBuf[nIndex++];
		pUE->m_nReturnSendComplete = pnRBuf[nIndex++];
		pUE->m_nAction1Complete = pnRBuf[nIndex++];
		pUE->m_nAction2Complete = pnRBuf[nIndex++];
		pUE->m_nReserved1 = pnRBuf[nIndex++];
		pUE->m_nReserved2 = pnRBuf[nIndex++];
		pUE->m_nReceiveRefuse = pnRBuf[nIndex++];
		pUE->m_nGlassIdReadComplete = pnRBuf[nIndex++];
		pUE->m_nLoadingStop = pnRBuf[nIndex++];
		pUE->m_nTransferStop = pnRBuf[nIndex++];
		pUE->m_nImmediatelyPauseRequest = pnRBuf[nIndex++];
		pUE->m_nImmediatelyStopRequest = pnRBuf[nIndex++];
		pUE->m_nReceiveAbleRemainedStep0 = pnRBuf[nIndex++];
		pUE->m_nReceiveAbleRemainedStep1 = pnRBuf[nIndex++];
		pUE->m_nReceiveAbleRemainedStep2 = pnRBuf[nIndex++];
		pUE->m_nReceiveAbleRemainedStep3 = pnRBuf[nIndex++];
		pUE->m_nHandShakeCancelRequest = pnRBuf[nIndex++];
		pUE->m_nHandShakeAbortRequest = pnRBuf[nIndex++];
		pUE->m_nHandShakeResumeRequest = pnRBuf[nIndex++];
		pUE->m_nHandShakeRecoveryAckReply = pnRBuf[nIndex++];
		pUE->m_nHandShakeRecoveryNckReply = pnRBuf[nIndex++];
		pUE->m_nReserved3 = pnRBuf[nIndex++];	
		pUE->m_nReserved4 = pnRBuf[nIndex++];	
		pUE->m_nReserved5 = pnRBuf[nIndex++];
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetToLowerEqBits()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
long MNetH::GetToLowerEqBits(MToLowerEqBit *pLE, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		pLE->m_nReserved0 = pnRBuf[nIndex++];
		pLE->m_nMachinePause = pnRBuf[nIndex++];
		pLE->m_nMachineDown = pnRBuf[nIndex++];
		pLE->m_nMachineAlarm = pnRBuf[nIndex++];	
		pLE->m_nSendAble = pnRBuf[nIndex++];
		pLE->m_nSendStart = pnRBuf[nIndex++];
		pLE->m_nSendComplete = pnRBuf[nIndex++];
		pLE->m_nExchangeFlag = pnRBuf[nIndex++];
		pLE->m_nReturnReceiveStart = pnRBuf[nIndex++];
		pLE->m_nReturnReceiveComplete = pnRBuf[nIndex++];
		pLE->m_nAction1Request = pnRBuf[nIndex++];
		pLE->m_nAction2Request = pnRBuf[nIndex++];
		pLE->m_nReserved1 = pnRBuf[nIndex++];
		pLE->m_nReserved2 = pnRBuf[nIndex++];
		pLE->m_nWorkStart = pnRBuf[nIndex++];
		pLE->m_nWorkCancel = pnRBuf[nIndex++];
		pLE->m_nReserved3 = pnRBuf[nIndex++];
		pLE->m_nReserved4 = pnRBuf[nIndex++];
		pLE->m_nImmediatelyPauseRequest = pnRBuf[nIndex++];
		pLE->m_nImmediatelyStopRequest = pnRBuf[nIndex++];
		pLE->m_nSendAbleRemainedStep0 = pnRBuf[nIndex++];
		pLE->m_nSendAbleRemainedStep1 = pnRBuf[nIndex++];
		pLE->m_nSendAbleRemainedStep2 = pnRBuf[nIndex++];
		pLE->m_nSendAbleRemainedStep3 = pnRBuf[nIndex++];
		pLE->m_nHandShakeCancelRequest = pnRBuf[nIndex++];
		pLE->m_nHandShakeAbortRequest = pnRBuf[nIndex++];
		pLE->m_nHandShakeResumeRequest = pnRBuf[nIndex++];
		pLE->m_nHandShakeRecoveryAckReply = pnRBuf[nIndex++];
		pLE->m_nHandShakeRecoveryNckReply = pnRBuf[nIndex++];
		pLE->m_nReserved5 = pnRBuf[nIndex++];
		pLE->m_nReserved6 = pnRBuf[nIndex++];
		pLE->m_nReserved7 = pnRBuf[nIndex++];
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}


/*========================================================================================
	FUNCTION : MNetH::GetContactPointStateBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetContactPointStateBit(MRobotArmStateBit *pRS, MStageStateBit *pSS, bool bUpperEq, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, bUpperEq);
	nAddr += 32;
	nPoints = 16;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		pRS->m_nRobotAbnormal = pnRBuf[nIndex++];	
		pRS->m_nTypeOfArm = pnRBuf[nIndex++];
		pRS->m_nTypeOfStageConveyor = pnRBuf[nIndex++];	
		pRS->m_nArm1Violate = pnRBuf[nIndex++];
		pRS->m_nArm2Violate = pnRBuf[nIndex++];
		pRS->m_nArm1FoldComplete = pnRBuf[nIndex++];
		pRS->m_nArm2FoldComplete = pnRBuf[nIndex++];
		pRS->m_nArm1GlassCheckSensor = pnRBuf[nIndex++];
		pRS->m_nArm2GlassCheckSensor = pnRBuf[nIndex++];
		pRS->m_nReserved0 = pnRBuf[nIndex++];
		pRS->m_nReserved1 = pnRBuf[nIndex++];
		pRS->m_nReserved2 = pnRBuf[nIndex++];
		pRS->m_nManualOperation = pnRBuf[nIndex++];
		pRS->m_nRobotDirection = pnRBuf[nIndex++];
		pRS->m_nReserved3 = pnRBuf[nIndex++];
		pRS->m_nReserved4 = pnRBuf[nIndex++];

		nIndex=0;
		pSS->m_nStageCVAbnormal = pnRBuf[nIndex++];	
		pSS->m_nTypeOfArm = pnRBuf[nIndex++];
		pSS->m_nTypeOfStageConveyor = pnRBuf[nIndex++];	
		pSS->m_nEmpty = pnRBuf[nIndex++];
		pSS->m_nIdle = pnRBuf[nIndex++];
		pSS->m_nBusy = pnRBuf[nIndex++];
		pSS->m_nPinUp = pnRBuf[nIndex++];
		pSS->m_nPinDown = pnRBuf[nIndex++];
		pSS->m_nStopperUp = pnRBuf[nIndex++];
		pSS->m_nStopperDown = pnRBuf[nIndex++];
		pSS->m_nDoorOpen = pnRBuf[nIndex++];
		pSS->m_nDoorClose = pnRBuf[nIndex++];
		pSS->m_nManualOperation = pnRBuf[nIndex++];
		pSS->m_nBodyMoving = pnRBuf[nIndex++];
		pSS->m_nGlassCheckSensorOn = pnRBuf[nIndex++];
		pSS->m_nReserved0 = pnRBuf[nIndex++];
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}


/*========================================================================================
	FUNCTION : MNetH::MelsecClose()
	DESCRIPT : Close Melsec Comm
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
short MNetH::MelsecClose() 
{
	short nRet = 0;
	
	// TODO: Add your dispatch handler code here
	if (m_fActive == FALSE) { return RV_SUCCESS; }
	nRet = mdClose(m_lPath);

	if (nRet == RV_SUCCESS) { m_fActive = FALSE; }
	return nRet;
}


/*========================================================================================
	FUNCTION : MNetH::MelsecOpen()
	DESCRIPT : Open Melsec Comm
	RETURN	 : 
	ARGUMENT :
		nChannel : 51 - first melsec board, 52 - second melsec board, ...
		nStation : 255 - own's station
	UPDATE	 : 2002/07/04, Inhyeok
			   2004/05/02, Hubri; Second work! (Change variable attribute from property)
========================================================================================*/
short MNetH::MelsecOpen() 
{
	short nRet = 0;
	
	if (m_fActive == TRUE) { MelsecClose(); }
	nRet = mdOpen(m_nChannel, -1, &m_lPath);

	if (nRet == RV_SUCCESS) { m_fActive = TRUE; }
	return nRet;
}


/*========================================================================================
	FUNCTION : MNetH::ReadConfig()
	DESCRIPT : Get Configuration Information from INI File.
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2005/12/19, Grouchy
========================================================================================*/
int MNetH::ReadConfig()
{
	WCHAR	szFile[256];	//	Configuration file name
	WCHAR	szSection[256], szKey[256];	//	Section & key
	WCHAR	szValue[256];	//	Configuration value
	WCHAR	szAppDir[255];	//	Application Folder
	CString	sAppDir;
	CString sStr;
	int		nListQty=0;

	memset(szAppDir, NULL, sizeof(szAppDir));
	//	Get the application folder.
	::GetCurrentDirectory(sizeof(szAppDir) - 1, szAppDir);
	sAppDir = szAppDir;
	memset(szFile, NULL, sizeof(szFile));
	//	Make the ini file path.
	swprintf(szFile, _T("%s\\%s"), sAppDir.GetBuffer(), m_sIniFile.GetBuffer());

	//////////////////////////////////////////////////////////////////////////////////////////
	// Basic information
	//strcpy(szSection, "BASIC");
	_tcscpy(szSection, _T("BASIC"));
	// Chanel
	//strcpy(szKey, "Chanel");
	_tcscpy(szKey, _T("Chanel"));
	::GetPrivateProfileString(szSection, szKey, _T("51"), szValue, sizeof(szValue) - 1, szFile);
	//m_nChannel = atoi(szValue);
	m_nChannel = _wtoi(szValue);
	// Local
	//strcpy(szKey, "Local");
	_tcscpy(szKey, _T("Local"));
	::GetPrivateProfileString(szSection, szKey, _T("2"), szValue, sizeof(szValue) - 1, szFile);
	//m_nCurLocal = atoi(szValue);
	m_nCurLocal = _wtoi(szValue);
	// Unit
	//strcpy(szKey, "Unit");
	_tcscpy(szKey, _T("Unit"));
	::GetPrivateProfileString(szSection, szKey, _T("1"), szValue, sizeof(szValue) - 1, szFile);
	//m_nCurUnit = atoi(szValue);
	m_nCurUnit = _wtoi(szValue);
	// Previous Local
	//strcpy(szKey, "PrevLocal");
	_tcscpy(szKey, _T("PrevLocal"));
	::GetPrivateProfileString(szSection, szKey, _T("0"), szValue, sizeof(szValue) - 1, szFile);
	//m_nPrevLocal = atoi(szValue);
	m_nPrevLocal = _wtoi(szValue);
	if (m_nPrevLocal==0)
	{
		m_nPrevLocal = m_nCurLocal - 1;
	}
	// Next Local
	//strcpy(szKey, "NextLocal");
	_tcscpy(szKey, _T("NextLocal"));
	::GetPrivateProfileString(szSection, szKey, _T("0"), szValue, sizeof(szValue) - 1, szFile);
	//m_nNextLocal = atoi(szValue);
	m_nNextLocal = _wtoi(szValue);
	if (m_nNextLocal==0)
	{
		m_nNextLocal = m_nCurLocal + 1;
	}
	// UseDialog
	//strcpy(szKey, "UseDialog");
	_tcscpy(szKey, _T("UseDialog"));
	::GetPrivateProfileString(szSection, szKey, _T("true"), szValue, sizeof(szValue) - 1, szFile);
	sStr = szValue;
	if ("true"==sStr){ m_bUseDialog=true; } else { m_bUseDialog=false; }
	// ShowDialog
	//strcpy(szKey, "ShowDialog");
	_tcscpy(szKey, _T("ShowDialog"));
	::GetPrivateProfileString(szSection, szKey, _T("true"), szValue, sizeof(szValue) - 1, szFile);
	sStr = szValue;
	if ("true"==sStr){ m_bShow=true; } else { m_bShow=false; }

	//////////////////////////////////////////////////////////////////////////////////////////
	// Local Name List
	//strcpy(szSection, "LOCAL_TITLE");
	_tcscpy(szSection, _T("LOCAL_TITLE"));
	// Name Count.
	//strcpy(szKey, "Local_Qty");
	_tcscpy(szKey, _T("Local_Qty"));
	::GetPrivateProfileString(szSection, szKey, _T(""), szValue, sizeof(szValue) - 1, szFile);
	//nListQty = atoi(szValue);
	nListQty = _wtoi(szValue);
	m_nLocal_Qty = nListQty;
	for (int i = 0; i < nListQty; i++){
		// Protocol
		sStr.Format(_T("Local_%02d"), i+1);
		//strcpy(szKey, sStr);
		_tcscpy(szKey, sStr);
		::GetPrivateProfileString(szSection, szKey, _T(""), szValue, sizeof(szValue) - 1, szFile);
		m_asLocalName.Add(szValue);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// Unit Name List
	//strcpy(szSection, "UNIT_TITLE");
	_tcscpy(szSection, _T("UNIT_TITLE"));
	// Name Count.
	//strcpy(szKey, "Unit_Qty");
	_tcscpy(szKey, _T("Unit_Qty"));
	::GetPrivateProfileString(szSection, szKey, _T(""), szValue, sizeof(szValue) - 1, szFile);
	//nListQty = atoi(szValue);
	nListQty = _wtoi(szValue);
	m_nUnitQty = nListQty;
	for (int i = 0; i < nListQty; i++){
		// Protocol
		sStr.Format(_T("Unit_%02d"), i+1);
		//strcpy(szKey, sStr);
		_tcscpy(szKey, sStr);
		::GetPrivateProfileString(szSection, szKey, _T(""), szValue, sizeof(szValue) - 1, szFile);
		m_asUnitName.Add(szValue);
	}

	return	0;
}



/*========================================================================================
	FUNCTION : MNetH::GetStageAbnormal_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetStageAbnormal_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eStageAbnormal_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetTypeOfArm_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetTypeOfArm_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eTypeOfArm_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetTypeOfStageConveyor_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetTypeOfStageConveyor_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eTypeOfStageConveyor_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetEmpty_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetEmpty_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eEmpty_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetIdle_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetIdle_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eIdle_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetBusy_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetBusy_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eBusy_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetPinUp_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetPinUp_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += ePinUp_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::GetPinDown_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetPinDown_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += ePinDown_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetStopperUp_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetStopperUp_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eStopperUp_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetStopperDown_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetStopperDown_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eStopperDown_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetDoorOpen_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetDoorOpen_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eDoorOpen_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetDoorClose_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetDoorClose_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eDoorClose_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetManualOperation_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetManualOperation_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eManualOperation_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetBodyMoving_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetBodyMoving_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eBodyMoving_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetGlassCheckSensorOn_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetGlassCheckSensorOn_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eGlassCheckSensorOn_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				


/*========================================================================================
	FUNCTION : MNetH::GetReserved0_Stage_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved0_Stage_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved0_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		


/*========================================================================================
	FUNCTION : MNetH::GetStageAbnormal_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetStageAbnormal_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eStageAbnormal_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetTypeOfArm_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetTypeOfArm_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eTypeOfArm_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetTypeOfStageConveyor_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetTypeOfStageConveyor_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eTypeOfStageConveyor_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetEmpty_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetEmpty_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eEmpty_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetIdle_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetIdle_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eIdle_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetBusy_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetBusy_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eBusy_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetPinUp_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetPinUp_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += ePinUp_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::GetPinDown_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetPinDown_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += ePinDown_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetStopperUp_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetStopperUp_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eStopperUp_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetStopperDown_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetStopperDown_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eStopperDown_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetDoorOpen_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetDoorOpen_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eDoorOpen_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetDoorClose_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetDoorClose_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eDoorClose_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetManualOperation_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetManualOperation_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eManualOperation_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetBodyMoving_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetBodyMoving_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eBodyMoving_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetGlassCheckSensorOn_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetGlassCheckSensorOn_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eGlassCheckSensorOn_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				


/*========================================================================================
	FUNCTION : MNetH::GetReserved0_Stage_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved0_Stage_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved0_Stage;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		
	


/*========================================================================================
	FUNCTION : MNetH::GetRobotAbnormal_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetRobotAbnormal_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eRobotAbnormal_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetTypeOfArm_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetTypeOfArm_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eTypeOfArm_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetTypeOfStageConveyor_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetTypeOfStageConveyor_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eTypeOfStageConveyor_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetArm1Violate_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetArm1Violate_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm1Violate_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetArm2Violate_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetArm2Violate_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm2Violate_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetArm1FoldComplete_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetArm1FoldComplete_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm1FoldComplete_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetArm2FoldComplete_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetArm2FoldComplete_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm2FoldComplete_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetArm1GlassCheckSensor_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetArm1GlassCheckSensorOn_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm1GlassCheckSensor_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::GetArm2GlassCheckSensor_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetArm2GlassCheckSensorOn_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm2GlassCheckSensor_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved0_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved0_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved0_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetReserved1_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved1_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved1_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetReserved2_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved2_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved2_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::GetManualOperation_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetManualOperation_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eManualOperation_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetRobotDirection_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetRobotDirection_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eRobotDirection_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetReserved3_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved3_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved3_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetReserved4_Robot_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved4_Robot_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved4_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetRobotAbnormal_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetRobotAbnormal_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eRobotAbnormal_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetTypeOfArm_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetTypeOfArm_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eTypeOfArm_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetTypeOfStageConveyor_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetTypeOfStageConveyor_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eTypeOfStageConveyor_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetArm1Violate_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetArm1Violate_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm1Violate_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetArm2Violate_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetArm2Violate_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm2Violate_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetArm1FoldComplete_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetArm1FoldComplete_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eArm1FoldComplete_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetArm2FoldComplete_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetArm2FoldComplete_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm2FoldComplete_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetArm1GlassCheckSensor_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetArm1GlassCheckSensorOn_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm1GlassCheckSensor_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::GetArm2GlassCheckSensor_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetArm2GlassCheckSensorOn_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eArm2GlassCheckSensor_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved0_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved0_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved0_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetReserved1_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved1_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved1_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetReserved2_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved2_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved2_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}				

/*========================================================================================
	FUNCTION : MNetH::GetManualOperation_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetManualOperation_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eManualOperation_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetRobotDirection_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetRobotDirection_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eRobotDirection_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetReserved3_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved3_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved3_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetReserved4_Robot_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved4_Robot_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved4_Robot;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		


/*========================================================================================
	FUNCTION : MNetH::GetReserved0_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved0_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved0_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetMachinePause_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetMachinePause_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eMachinePause_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetMachineDown_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetMachineDown_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eMachineDown_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetMachineAlarm_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetMachineAlarm_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eMachineAlarm_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetSendAble_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetSendAble_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendAble_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetSendStart_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetSendStart_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendStart_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetSendComplete_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetSendComplete_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendComplete_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::GetExchangeFlag_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetExchangeFlag_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eExchangeFlag_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReturnReceiveStart_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReturnReceiveStart_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReturnReceiveStart_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetReturnReceiveComplete_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReturnReceiveComplete_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReturnReceiveComplete_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetImmediatelyPauseRequest_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetImmediatelyPauseRequest_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eImmediatelyPauseRequest_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::GetImmediatelyStopRequest_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetImmediatelyStopRequest_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eImmediatelyStopRequest_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetSendAbleRemainedStep0_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetSendAbleRemainedStep0_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendAbleRemainedStep0_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetSendAbleRemainedStep1_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetSendAbleRemainedStep1_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendAbleRemainedStep1_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetSendAbleRemainedStep2_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetSendAbleRemainedStep2_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendAbleRemainedStep2_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetSendAbleRemainedStep3_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetSendAbleRemainedStep3_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eSendAbleRemainedStep3_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetWorkStart_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetWorkStart_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eWorkStart_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetWorkCancel_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetWorkCancel_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eWorkCancel_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetAction1Request_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetAction1Request_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eAction1Request_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetAction2Request_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetAction2Request_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eAction2Request_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved6_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved6_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved6_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved7_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved7_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved7_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved1_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved1_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved1_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::GetReserved2_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved2_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved2_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::GetHandShakeCancelRequest_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetHandShakeCancelRequest_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eHandShakeCancelRequest_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetHandShakeAbortRequest_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetHandShakeAbortRequest_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eHandShakeAbortRequest_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetHandShakeResumeRequest_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetHandShakeResumeRequest_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eHandShakeResumeRequest_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::GetHandShakeRecoveryAckReply_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetHandShakeRecoveryAckReply_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eHandShakeRecoveryAckReply_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetHandShakeRecoveryNckReply_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetHandShakeRecoveryNckReply_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eHandShakeRecoveryNckReply_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved3_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved3_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved3_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved4_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved4_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved4_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved5_ToLowerEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved5_ToLowerEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, false);
	nAddr += eReserved5_LowerEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::GetReserved0_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved0_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved0_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetMachinePause_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetMachinePause_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eMachinePause_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetMachineDown_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetMachineDown_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eMachineDown_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetMachineAlarm_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetMachineAlarm_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eMachineAlarm_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetMachineAlarm_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReceiveAble_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveAble_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetReceiveStart_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReceiveStart_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveStart_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetReceiveComplete_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReceiveComplete_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveComplete_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::GetExchangeFlag_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetExchangeFlag_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eExchangeFlag_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReturnSendStart_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReturnSendStart_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReturnSendStart_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetReturnSendComplete_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReturnSendComplete_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReturnSendComplete_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetAction1Complete_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetAction1Complete_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eAction1Complete_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::GetAction2Complete_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetAction2Complete_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eAction2Complete_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved1_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved1_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved1_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved2_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved2_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved2_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReceiveRefuse_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReceiveRefuse_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveRefuse_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetGlassIdReadComplete_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetGlassIdReadComplete_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eGlassIdReadComplete_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetLoadingStop_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetLoadingStop_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eLoadingStop_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetTransferStop_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetTransferStop_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eTransferStop_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetImmediatelyPauseRequest_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetImmediatelyPauseRequest_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eImmediatelyPauseRequest_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetImmediatelyStopRequest_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetImmediatelyStopRequest_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eImmediatelyStopRequest_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReceiveAbleRemainedStep0_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReceiveAbleRemainedStep0_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveAbleRemainedStep0_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReceiveAbleRemainedStep1_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReceiveAbleRemainedStep1_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveAbleRemainedStep1_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReceiveAbleRemainedStep2_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReceiveAbleRemainedStep2_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveAbleRemainedStep2_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::GetReceiveAbleRemainedStep3_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReceiveAbleRemainedStep3_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReceiveAbleRemainedStep3_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::GetHandShakeCancelRequest_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetHandShakeCancelRequest_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eHandShakeCancelRequest_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetHandShakeAbortRequest_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetHandShakeAbortRequest_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eHandShakeAbortRequest_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}			

/*========================================================================================
	FUNCTION : MNetH::GetHandShakeResumeRequest_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetHandShakeResumeRequest_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eHandShakeResumeRequest_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	

/*========================================================================================
	FUNCTION : MNetH::GetHandShakeRecoveryAckReply_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetHandShakeRecoveryAckReply_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eHandShakeRecoveryAckReply_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetHandShakeRecoveryNckReply_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetHandShakeRecoveryNckReply_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eHandShakeRecoveryNckReply_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved3_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved3_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved3_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved4_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved4_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved4_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}		

/*========================================================================================
	FUNCTION : MNetH::GetReserved5_UpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/02, KJH
	LAST	 : 2004/09/02, KJH
========================================================================================*/
long MNetH::GetReserved5_ToUpperEqBit(short *nValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	
	GetPLCAddressBit(nLocal, nUnit, &nAddr, &nPoints, true);
	nAddr += eReserved5_UpperEqBit;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ *nValue=pnRBuf[0]; }
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}	


/*========================================================================================
	FUNCTION : MNetH::GetTGlassCount_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetTGlassCount_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eTGlassCount_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			//*psValue += MakeString(pnRBuf[i]);
			sTmp += MakeString(pnRBuf[i], false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetTFTCount_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetTFTCount_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;

	GetPLCAddress(eTFTCount_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			//*psValue += MakeString(pnRBuf[i]);
			sTmp += MakeString(pnRBuf[i], false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetCFCount_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetCFCount_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCFCount_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			//*psValue += MakeString(pnRBuf[i]);
			sTmp += MakeString(pnRBuf[i], false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetDummyCount_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetDummyCount_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eDummyCount_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			//*psValue += MakeString(pnRBuf[i]);
			sTmp += MakeString(pnRBuf[i], false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetBPGCount_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetBPGCount_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eBPGCount_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			//*psValue += MakeString(pnRBuf[i]);
			sTmp += MakeString(pnRBuf[i], false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetProcessID1_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::GetProcessID1_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eProcessID1_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetStepID1_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::GetStepID1_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eStepID1_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetBatchID1_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::GetBatchID1_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eBatchID1_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGlassCount1_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetGlassCount1_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eGlassCount1_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			//*psValue += MakeString(pnRBuf[i]);
			sTmp += MakeString(pnRBuf[i], false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetProcessID2_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::GetProcessID2_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eProcessID2_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetStepID2_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::GetStepID2_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eStepID2_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetBatchID2_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::GetBatchID2_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eBatchID2_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGlassCount2_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetGlassCount2_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eGlassCount2_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			//*psValue += MakeString(pnRBuf[i]);
			sTmp += MakeString(pnRBuf[i], false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetProcessID3_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::GetProcessID3_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eProcessID3_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetStepID3_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::GetStepID3_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eStepID3_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetBatchID3_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::GetBatchID3_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eBatchID3_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGlassCount3_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetGlassCount3_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eGlassCount3_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			//*psValue += MakeString(pnRBuf[i]);
			sTmp += MakeString(pnRBuf[i], false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetReserved_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::GetReserved_GlassSum(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReserved_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetTFTCount_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetTFTCount_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eTFTCount_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetCFCount_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetCFCount_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCFCount_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetDummyCount_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetDummyCount_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eDummyCount_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetBPGCount_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetBPGCount_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eBPGCount_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetProcessID1_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::SetProcessID1_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eProcessID1_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetStepID1_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::SetStepID1_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eStepID1_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetBatchID1_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::SetBatchID1_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eBatchID1_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetGlassCount1_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetGlassCount1_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eGlassCount1_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetProcessID2_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::SetProcessID2_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eProcessID2_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetStepID2_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::SetStepID2_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eStepID2_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetBatchID2_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::SetBatchID2_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eBatchID2_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetGlassCount2_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetGlassCount2_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eGlassCount2_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetProcessID3_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::SetProcessID3_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eProcessID3_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetStepID3_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::SetStepID3_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eStepID3_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetBatchID3_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::SetBatchID3_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eBatchID3_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetGlassCount3_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetGlassCount3_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eGlassCount3_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetReserved_GlassSum()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/09/06, KJH; First work!
========================================================================================*/
long MNetH::SetReserved_GlassSum(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReserved_GlassSum, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetEqState_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetEqState_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eEqState_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetProcState_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetProcState_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eProcState_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPPID_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPPID_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(ePPID_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGlassSize_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetGlassSize_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eGlassSize_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGlassThickness_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetGlassThickness_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eGlassThickness_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetTACTSet_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetTACTSet_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eTACTSet_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetTACTCurrent_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetTACTCurrent_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eTACTCurrent_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetRCDG_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetRCDG_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eRCDG_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetEOMode_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetEOMode_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eEOMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetERCMode_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetERCMode_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eERCMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGMCMode_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetGMCMode_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eGMCMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetOperateMode_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetOperateMode_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eOperateMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}


/*========================================================================================
	FUNCTION : MNetH::GetReserved_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetReserved_EqState_EO(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eReserved_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}


/*========================================================================================
	FUNCTION : MNetH::SetEqState_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetEqState_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eEqState_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetProcState_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetProcState_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eProcState_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetPPID_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetPPID_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(ePPID_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetGlassSize_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetGlassSize_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eGlassSize_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetGlassThickness_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetGlassThickness_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eGlassThickness_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetTACTSet_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetTACTSet_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eTACTSet_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetTACTCurrent_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetTACTCurrent_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eTACTCurrent_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetRCDG_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetRCDG_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eRCDG_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetEOMode_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetEOMode_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eEOMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetERCMode_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetERCMode_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eERCMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetGMCMode_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetGMCMode_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eGMCMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetOperateMode_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetOperateMode_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eOperateMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetReserved_EqState_EO()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetReserved_EqState_EO(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_EO(eReserved_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetEqState_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetEqState_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eEqState_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetProcState_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetProcState_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eProcState_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPPID_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPPID_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(ePPID_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGlassSize_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetGlassSize_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eGlassSize_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGlassThickness_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetGlassThickness_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eGlassThickness_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetTACTSet_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetTACTSet_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eTACTSet_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetTACTCurrent_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetTACTCurrent_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eTACTCurrent_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetRCDG_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetRCDG_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eRCDG_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetEOMode_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetEOMode_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eEOMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetERCMode_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetERCMode_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eERCMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGMCMode_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetGMCMode_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eGMCMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetOperateMode_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetOperateMode_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eOperateMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}


/*========================================================================================
	FUNCTION : MNetH::GetReserved_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetReserved_EqState_ER(CString *psValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eReserved_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}


/*========================================================================================
	FUNCTION : MNetH::SetEqState_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetEqState_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eEqState_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetProcState_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetProcState_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eProcState_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetPPID_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetPPID_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(ePPID_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetGlassSize_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetGlassSize_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eGlassSize_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetGlassThickness_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetGlassThickness_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eGlassThickness_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetTACTSet_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetTACTSet_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eTACTSet_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetTACTCurrent_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetTACTCurrent_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eTACTCurrent_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetRCDG_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetRCDG_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eRCDG_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetEOMode_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetEOMode_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eEOMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetERCMode_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetERCMode_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eERCMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetGMCMode_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetGMCMode_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eGMCMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetOperateMode_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetOperateMode_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eOperateMode_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}


/*========================================================================================
	FUNCTION : MNetH::SetReserved_EqState_ER()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetReserved_EqState_ER(CString sValue, int nLocal)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddressWord_ER(eReserved_EqState, &nAddr, &nPoints, nLocal);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetReadingGlassID_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetReadingGlassID_UpperEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReadingGlassID_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetReadingGlassIDType_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetReadingGlassIDType_UpperEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReadingGlassIDType_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGlassType_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetGlassType_UpperEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eGlassType_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetReceiveRefuseCode_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetReceiveRefuseCode_UpperEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReceiveRefuseCode_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetReserved_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetReserved_UpperEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReserved_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetCSIF_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetCSIF_UpperEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCSIF_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			sTmp += MakeString(pnRBuf[i],false);
		}
		HexToBin(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetAS_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetAS_UpperEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eAS_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			sTmp += MakeString(pnRBuf[i],false);
		}
		HexToBin(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetAPS_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetAPS_UpperEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eAPS_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			sTmp += MakeString(pnRBuf[i],false);
		}
		HexToBin(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetReadingGlassID_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetReadingGlassID_UpperEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReadingGlassID_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetReadingGlassIDType_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetReadingGlassIDType_UpperEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReadingGlassIDType_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetGlassType_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetGlassType_UpperEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eGlassType_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetReceiveRefuseCode_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetReceiveRefuseCode_UpperEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReceiveRefuseCode_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetReserved_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetReserved_UpperEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReserved_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetCSIF_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetCSIF_UpperEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCSIF_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	BinToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetAS_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetAS_UpperEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eAS_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	BinToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetAPS_UpperEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetAPS_UpperEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eAPS_UpperEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	BinToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}




/*========================================================================================
	FUNCTION : MNetH::GetHPanelID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetHPanelID_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eHPanelID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetEPanelID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetEPanelID_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eEPanelID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetBatchID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetBatchID_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eBatchID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetProdType_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetProdType_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eProdType_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetProdKind_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetProdKind_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eProdKind_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetDeviceID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetDeviceID_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eDeviceID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetStepID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetStepID_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eStepID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPPID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetPPID_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePPID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			 *psValue += MakeString(pnRBuf[i],false);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetThickness_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetThickness_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eThickness_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetInsFlag_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetInsFlag_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eInsFlag_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPanelSize_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPanelSize_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePanelSize_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPanelPosition_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPanelPosition_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePanelPosition_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetCount1_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetCount1_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCount1_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetCount2_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetCount2_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCount2_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetGrade_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetGrade_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eGrade_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetComment_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetComment_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eComment_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			 *psValue += MakeString(pnRBuf[i],false);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetCompSize_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetCompSize_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCompSize_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetReadingFlag_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetReadingFlag_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReadingFlag_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPanelState_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetPanelState_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePanelState_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetJudgement_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetJudgement_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eJudgement_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetCode_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetCode_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCode_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetRunLine_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetRunLine_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eRunLine_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToBin(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetUniqueId_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetUniqueID_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eUniqueId_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPairHPanelID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPairHPanelID_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePairHPanelID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPairEPanelID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPairEPanelID_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePairEPanelID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPairGrade_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPairGrade_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePairGrade_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPairUniqueId_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetPairUniqueID_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePairUniqueId_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToDec(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetFlowRecipe_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetFlowRecipe_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eFlowRecipe_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			 *psValue += MakeString(pnRBuf[i],false);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetReserved0_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetReserved0_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReserved0_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetBitsSignals_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetBitsSignals_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eBitsSignals_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToBin(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetReferData_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::GetReferData_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReferData_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		for (int i = 0; i < nPoints; i++){
			*psValue += MakeString(pnRBuf[i]);
		}
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetCSIF_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetCSIF_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCSIF_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToBin(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetAS_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetAS_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eAS_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToBin(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetAPS_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::GetAPS_LowerEq(CString *psValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eAPS_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	*psValue = "";
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 

	if (!lRet){
		CString sTmp="";
		for (int i = 0; i < nPoints; i++){
			 sTmp += MakeString(pnRBuf[i],false);
		}
		HexToBin(sTmp, *psValue);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetHPanelID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetHPanelID_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eHPanelID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetEPanelID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetEPanelID_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eEPanelID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetBatchID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetBatchID_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eBatchID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetProdType_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetProdType_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eProdType_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetProdKind_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetProdKind_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eProdKind_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetDeviceID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetDeviceID_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eDeviceID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetStepID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetStepID_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eStepID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetPPID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetPPID_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePPID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetThickness_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetThickness_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eThickness_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetInsFlag_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetInsFlag_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eInsFlag_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetPanelSize_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetPanelSize_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePanelSize_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetPanelPosition_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetPanelPosition_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePanelPosition_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetCount1_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetCount1_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCount1_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetCount2_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetCount2_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCount2_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetGrade_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetGrade_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eGrade_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetComment_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetComment_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eComment_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetCompSize_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetCompSize_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCompSize_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetReadingFlag_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetReadingFlag_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReadingFlag_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetPanelState_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetPanelState_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePanelState_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetJudgement_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetJudgement_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eJudgement_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetCode_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetCode_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCode_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetRunLine_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetRunLine_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eRunLine_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	BinToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetUniqueId_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetUniqueId_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eUniqueId_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetPairHPanelID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetPairHPanelID_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePairHPanelID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetPairEPanelID_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetPairEPanelID_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePairEPanelID_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetPairGrade_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetPairGrade_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePairGrade_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetPairUniqueId_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetPairUniqueID_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(ePairUniqueId_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	DecToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetFlowRecipe_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetFlowRecipe_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eFlowRecipe_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetReserved0_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetReserved0_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReserved0_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetBitsSignals_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetBitsSignals_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eBitsSignals_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	BinToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetReferData_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/08/12, KJH; First work!
========================================================================================*/
long MNetH::SetReferData_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eReferData_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	MakeShortArray(sValue, pnRBuf, nPoints);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetCSIF_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetCSIF_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eCSIF_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	BinToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetAS_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetAS_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eAS_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	BinToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetAPS_LowerEq()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2004/12/02, James Park
========================================================================================*/
long MNetH::SetAPS_LowerEq(CString sValue, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	
	GetPLCAddress(eAPS_LowerEq, &nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	CString sHex="";
	BinToHex(sValue, sHex);
	MakeShortArray(sHex, pnRBuf, nPoints, 0, false);
	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

//SJ_YYK 150821 Add..
long MNetH::GetMaterialInfo(MMaterial *pMat, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	CString	sTmp="";
	long	lValue = 0;
	char szAscii[256];
	
	GetPLCAddressWord_MInfo(&nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		AscToString(szAscii, &pnRBuf[0], 4);	TrimSpace(szAscii);		pMat->m_sPORTID	= szAscii;	
		AscToString(szAscii, &pnRBuf[4], 1);	TrimSpace(szAscii);		pMat->m_sEQP_STATE	= szAscii;	
		AscToString(szAscii, &pnRBuf[5], 1);	TrimSpace(szAscii);		pMat->m_sPORT_STATE	= szAscii;	
		AscToString(szAscii, &pnRBuf[6], 1);	TrimSpace(szAscii);		pMat->m_sPORT_TYPE	= szAscii;	
		AscToString(szAscii, &pnRBuf[7], 16);	TrimSpace(szAscii);		pMat->m_sCSTID	= szAscii;	
		AscToString(szAscii, &pnRBuf[23], 80);	TrimSpace(szAscii);		pMat->m_nMAP_STIF	= atoi(szAscii);	
		AscToString(szAscii, &pnRBuf[103], 80);	TrimSpace(szAscii);		pMat->m_nCUR_STIF	= atoi(szAscii);
		AscToString(szAscii, &pnRBuf[183], 20);	TrimSpace(szAscii);		pMat->m_sM_BATCHID	= szAscii;	
		AscToString(szAscii, &pnRBuf[203], 18);	TrimSpace(szAscii);		pMat->m_sM_MAKER	= szAscii;	
		AscToString(szAscii, &pnRBuf[221], 1);	TrimSpace(szAscii);		pMat->m_nM_KIND	= atoi(szAscii);	
		AscToString(szAscii, &pnRBuf[222], 1);	TrimSpace(szAscii);		pMat->m_nM_TYPE		= atoi(szAscii);		
		AscToString(szAscii, &pnRBuf[223], 1);	TrimSpace(szAscii);		pMat->m_nM_STATE	= atoi(szAscii);		
		AscToString(szAscii, &pnRBuf[224], 2);	TrimSpace(szAscii);		pMat->m_nT_QTY		= atoi(szAscii);		
		AscToString(szAscii, &pnRBuf[226], 2);	TrimSpace(szAscii);		pMat->m_nU_QTY		= atoi(szAscii);
		AscToString(szAscii, &pnRBuf[228], 2);	TrimSpace(szAscii);		pMat->m_nR_QTY		= atoi(szAscii);
		AscToString(szAscii, &pnRBuf[230], 2);	TrimSpace(szAscii);		pMat->m_nN_QTY		= atoi(szAscii);
		AscToString(szAscii, &pnRBuf[232], 2);	TrimSpace(szAscii);		pMat->m_nA_QTY		= atoi(szAscii);
		AscToString(szAscii, &pnRBuf[234], 1);	TrimSpace(szAscii);		pMat->m_nE_FLAG		= atoi(szAscii);		
		AscToString(szAscii, &pnRBuf[235], 4);	TrimSpace(szAscii);		pMat->m_sC_CODE		= szAscii;		
		AscToString(szAscii, &pnRBuf[239], 10);	TrimSpace(szAscii);		pMat->m_sM_STEP		= szAscii;		
		AscToString(szAscii, &pnRBuf[249], 2);	TrimSpace(szAscii);		pMat->m_nS_QTY		= atoi(szAscii);
		AscToString(szAscii, &pnRBuf[251], 16);	TrimSpace(szAscii);		pMat->m_sM_CODE		= szAscii;
	}	
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;	
}

long MNetH::SetMaterialInfo(MMaterial *pMat, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	short	nIndex=0;
	short	nItemLen=0;
	CString	sTmp="";
	
	char sTemp[255];
	WCHAR szAscii[64];
	memset(sTemp, 0, sizeof(sTemp));

	GetPLCAddressWord_MInfo(&nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pMat->m_sPORTID);	FillSpace(szAscii, 8);	StringToAsc(szAscii, &pnRBuf[0], 4);

	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pMat->m_sEQP_STATE);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[4],1);

	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pMat->m_sPORT_STATE);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[5], 1);

	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pMat->m_sPORT_TYPE);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[6], 1);
	
	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pMat->m_sCSTID);	FillSpace(szAscii, 32);	StringToAsc(szAscii, &pnRBuf[7], 16);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%d"), pMat->m_nMAP_STIF);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 160);	StringToAsc(szAscii, &pnRBuf[23], 80);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%d"), pMat->m_nCUR_STIF);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 160);	StringToAsc(szAscii, &pnRBuf[103], 80);

	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pMat->m_sM_BATCHID);	FillSpace(szAscii, 40);	StringToAsc(szAscii, &pnRBuf[183], 20);

	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pMat->m_sM_MAKER);	FillSpace(szAscii, 36);	StringToAsc(szAscii, &pnRBuf[203], 18);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%d"), pMat->m_nM_KIND);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[221], 1);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%d"), pMat->m_nM_TYPE);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[222], 1);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%d"), pMat->m_nM_STATE);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[223], 1);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%d"), pMat->m_nT_QTY);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[224], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%d"), pMat->m_nU_QTY);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[226], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%d"), pMat->m_nR_QTY);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[228], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%d"), pMat->m_nN_QTY);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[230], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%d"), pMat->m_nA_QTY);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[232], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%d"), pMat->m_nE_FLAG);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[234], 1);

	memset(szAscii, 0, sizeof(szAscii));		
	swprintf(szAscii, pMat->m_sC_CODE);	FillSpace(szAscii, 8);	StringToAsc(szAscii, &pnRBuf[235], 4);

	memset(szAscii, 0, sizeof(szAscii));		
	swprintf(szAscii, pMat->m_sM_STEP);	FillSpace(szAscii, 20);	StringToAsc(szAscii, &pnRBuf[239], 10);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%d"), pMat->m_nS_QTY);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[249], 2);

	memset(szAscii, 0, sizeof(szAscii));		
	swprintf(szAscii, pMat->m_sM_CODE);	FillSpace(szAscii, 32);	StringToAsc(szAscii, &pnRBuf[251], 16);
	

	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

long MNetH::GetFDCInfo(MProcessParamInfo *pParam, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	CString	sTmp="";
	long	lValue = 0;
	char szAscii[256];

	nUnit = 13;
	
	GetPLCAddressWord_FDCInfo(&nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){ 
		AscToString(szAscii, &pnRBuf[0], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[0].m_dMeasureValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[2], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[1].m_dMeasureValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[4], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[2].m_dMeasureValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[6], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[3].m_dMeasureValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[8], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[4].m_dMeasureValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[10], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[5].m_dMeasureValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[12], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[6].m_dMeasureValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[14], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[7].m_dMeasureValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[16], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[8].m_dMeasureValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[18], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[9].m_dMeasureValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[20], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[10].m_dMeasureValue	= atof(szAscii);		
		AscToString(szAscii, &pnRBuf[22], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[11].m_dMeasureValue	= atof(szAscii);		
		AscToString(szAscii, &pnRBuf[24], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[12].m_dMeasureValue	= atof(szAscii);		
		AscToString(szAscii, &pnRBuf[26], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[13].m_dMeasureValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[28], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[14].m_dMeasureValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[30], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[15].m_dMeasureValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[32], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[16].m_dMeasureValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[34], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[17].m_dMeasureValue	= atof(szAscii);		
		AscToString(szAscii, &pnRBuf[36], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[18].m_dMeasureValue	= atof(szAscii);		
		AscToString(szAscii, &pnRBuf[38], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[19].m_dMeasureValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[40], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[20].m_dMeasureValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[42], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[21].m_dMeasureValue	= atof(szAscii);	

		AscToString(szAscii, &pnRBuf[44], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[0].m_dSettingValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[46], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[1].m_dSettingValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[48], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[2].m_dSettingValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[50], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[3].m_dSettingValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[52], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[4].m_dSettingValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[54], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[5].m_dSettingValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[56], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[6].m_dSettingValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[58], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[7].m_dSettingValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[60], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[8].m_dSettingValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[62], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[9].m_dSettingValue	= atof(szAscii);	
		AscToString(szAscii, &pnRBuf[64], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[10].m_dSettingValue	= atof(szAscii);		
		AscToString(szAscii, &pnRBuf[66], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[11].m_dSettingValue	= atof(szAscii);		
		AscToString(szAscii, &pnRBuf[68], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[12].m_dSettingValue	= atof(szAscii);		
		AscToString(szAscii, &pnRBuf[70], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[13].m_dSettingValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[72], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[14].m_dSettingValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[74], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[15].m_dSettingValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[76], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[16].m_dSettingValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[78], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[17].m_dSettingValue	= atof(szAscii);		
		AscToString(szAscii, &pnRBuf[80], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[18].m_dSettingValue	= atof(szAscii);		
		AscToString(szAscii, &pnRBuf[82], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[19].m_dSettingValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[84], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[20].m_dSettingValue	= atof(szAscii);
		AscToString(szAscii, &pnRBuf[86], 2);	TrimSpace(szAscii);		pParam->m_rgsProcessParam[21].m_dSettingValue	= atof(szAscii);
	}	
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;	
}

long MNetH::SetFDCInfo(MProcessParamInfo *pParam, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	short	nIndex=0;
	short	nItemLen=0;
	CString	sTmp="";
	
	char sTemp[255];
	WCHAR szAscii[64];
	memset(sTemp, 0, sizeof(sTemp));

	nUnit = 13;

	GetPLCAddressWord_FDCInfo(&nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[0].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[0], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[1].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[2], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[2].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[4], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[3].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[6], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[4].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[8], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[5].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[10], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[6].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[12], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[7].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[14], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[8].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[16], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[9].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[18], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[10].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[20], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[11].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[22], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[12].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[24], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[13].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[26], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[14].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[28], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[15].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[30], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[16].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[32], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[17].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[34], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[18].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[36], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[19].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[38], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[20].m_dMeasureValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[40], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[0].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[42], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[1].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[44], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[2].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[46], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[3].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[48], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[4].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[50], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[5].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[52], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[6].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[54], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[7].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[56], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[8].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[58], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[9].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[60], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[10].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[62], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[11].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[64], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[12].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[66], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[13].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[68], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[14].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[70], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[15].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[72], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[16].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[74], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[17].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[76], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[18].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[78], 2);

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[19].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[80], 2);	

	memset(szAscii, 0, sizeof(szAscii));	
	sTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[19].m_dSettingValue);
	swprintf(szAscii, sTmp);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[82], 2);	

	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}
//_____________________________________________*/
// Add Function by cha 2006/02/07 
/*========================================================================================
	FUNCTION : MNetH::AscToString()
	DESCRIPT : Ascii를 String로 변환하는 함수.
	RETURN	 : None
	ARGUMENT : 
	UPDATE	 : 2005/07/07, Hubri; First work!
========================================================================================*/
void MNetH::AscToString(char *pszOut, short *pnaBuf, short nPoints)
{
	short	i = 0;

	for (int i = 0; i < nPoints; i++) {
		pszOut[(i * 2) + 1] = pnaBuf[i] / 0x0100;
		pszOut[(i * 2) + 0] = pnaBuf[i] - ((pnaBuf[i] / 0x0100) * 0x0100);
	}
	pszOut[(i * 2) + 0] = '\0';
}


// Add Function by cha 2006/02/07 
/*========================================================================================
	FUNCTION: StringToAsc()
	DESCRIPT: String을 Ascii로 변환하는 함수.
	RETURN	: void
	ARGUMENT: 
	UPDATE	: 2005/6/22, KyeongWhan; First work!
			  2008/10/01, LEEKIHO; Second work!(char -> WCHAR) 
========================================================================================*/
void MNetH::StringToAsc(WCHAR *pszIn, short *pnabuf, short nPoints)
{
	for (int i = 0; i < nPoints; i++){
		pnabuf[i] = MAKEWORD(pszIn[2 * i], pszIn[(2 * i) + 1]);
	}
}


// Add Function by cha 2006/02/07 
/*========================================================================================
	FUNCTION : FillSpace()
	DESCRIPT : Make the space filled ASCII item.
	RETURN	 : None
	ARGUMENT :
		pszIn  	: (i)Input string
		nLen	: (i)Total output length
	UPDATE	 : 2002/09/13, Wontae; First work!
			   2004/07/03, Hubri; 2nd work!
			   2005/01/27, Hubri; 3rd work!
			   2006/02/15, cha;   4th work!
========================================================================================*/
void MNetH::FillSpace(WCHAR *pszIn, short nStr)
{
	//WCHAR *pszDm = new WCHAR[strlen(pszIn) + 1];
	WCHAR *pszDm = new WCHAR[_tcslen(pszIn) + 1];
	//short	nLen = strlen(pszIn);
	short	nLen = _tcslen(pszIn);

	memset(pszDm, 0, sizeof(WCHAR) * (nLen + 1));
	//if (nLen != 0) { strncpy(pszDm, pszIn, nLen); }
	if (nLen != 0) { _tcsncpy(pszDm, pszIn, nLen); }

	memset(pszIn, ' ', nStr);	*(pszIn + nStr) = '\0';
	if (nLen == 0) { 
		delete [] pszDm;	pszDm = NULL; 
		return; 
	}
	if (nStr == 0) { 
		delete [] pszDm;	pszDm = NULL;
		return; 
	}
	if (nLen > nStr) { nLen = nStr; }

	//strncpy(pszIn, pszDm, nLen);	
	_tcsncpy(pszIn, pszDm, nLen);
	delete [] pszDm;	pszDm = NULL;
}


/*========================================================================================
	FUNCTION : TrimSpace()
	DESCRIPT : Trim the space ASCII item.
	RETURN	 : None
	ARGUMENT :
		pszIn  	: (i)Input string
	UPDATE	 : 2004/07/03, Hubri; First work!
			   2005/01/27, Hubri; 2nd work!
========================================================================================*/
void MNetH::TrimSpace(char *pszIn)
{
	char *pszDm = new char[strlen(pszIn) + 1];
	short	nLen = strlen(pszIn);
	short	nStr = 0;
	short	nEnd = 0;
	short	nCnt = 0;
	short	i = 0;

	memset(pszDm, 0, sizeof(char) * (nLen + 1));
	if (nLen == 0) {
		delete [] pszDm;	pszDm = NULL;
		return;
	}

	strncpy(pszDm, pszIn, nLen);

	memset(pszIn, 0, sizeof(char) * (nLen + 1));
	for (int i = 0; i < nLen; i++) {	//	Trim-Left
		if (*(pszDm + i) != ' ') {
			nStr = i;
			break;
		}
	}
	for (int i = 0; i < nLen; i++) {	//	Trim-Right
		if (*(pszDm + (nLen - (i + 1))) != ' ') {
			nEnd = nLen - i;
			break;
		}
	}
	nCnt = nEnd - nStr;
	if (nCnt == 0) { 
		delete [] pszDm;	pszDm = NULL;
		return; 
	}
	strncpy(pszIn, pszDm + nStr, nCnt);
	delete [] pszDm;	pszDm = NULL;
}


/*========================================================================================
	FUNCTION : MNetH::SetJobOrder_ToLowerEqWord()
	DESCRIPT : ToLowerEq(Glass Data) 영역의 JobOrder항목에 값을 설정한다.
	RETURN	 : 
	ARGUMENT : 
			(i) fLastGlass: Last Glass Flag ON/OFF
			(i) nLocal: current Local number
			(i) nUnit: current Local number
	UPDATE	 : 2006/02/10, Grouchy
========================================================================================*/
long MNetH::SetJobOrder_ToLowerEqWord(bool fLastGlass, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	short	nIndex=0;
	short	nItemLen=0;
	CString	strJobOrder="";

	GetPLCAddressWord_Unit(&nAddr, &nPoints, false, nLocal, nUnit);
	nPoints = nItemLen = GetPLCItemLen(eBitsSignals_LowerEq); 

	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	ReadLW(nAddr, nPoints, pnRBuf, sizeof(short) * nPoints);

	if		(fLastGlass == true)	{ pnRBuf[0] = pnRBuf[0] | 0x04; }
	else if (fLastGlass == false)	{ pnRBuf[0] = pnRBuf[0] & (0xFFFF - 0x04); }

	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetMachineAlarm_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
			(1) Current Eq에서 Upper Eq의 Bit를 On/Off 시킨다.
			(2) GetPLCAddressBit: Local->m_nCurLocal, bUpperEq->true

	FIRST	 : 2006/06/22, KJS
	LAST	 : 2006/06/22, KJS
========================================================================================*/
//SJ_YYK 160225 Modify..
//long MNetH::SetUpperEqBit(short eBitAddr, short nValue, int nUnit)
long MNetH::SetUpperEqBit(short eBitAddr, short nValue, int nUnit, BOOL bAOCBit)
{	
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	EnterCriticalSection(&m_csMelsec); //SJ_YYK 160222 Add..
	//SJ_YYK 160225 Modify..
	//GetPLCAddressBit(m_nCurLocal, m_nCurUnit + nUnit, &nAddr, &nPoints, true);
	GetPLCAddressBit(m_nCurLocal, m_nCurUnit + nUnit, &nAddr, &nPoints, true, bAOCBit);
	nAddr += eBitAddr;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	LeaveCriticalSection(&m_csMelsec); //SJ_YYK 160222 Add..
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetMachineAlarm_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
			(1) Current Eq에서 Lower Eq의 Bit를 On/Off 시킨다.
			(2) GetPLCAddressBit: Local->m_nCurLocal, bUpperEq->false

	FIRST	 : 2006/06/22, KJS
	LAST	 : 2006/06/22, KJS
========================================================================================*/
//SJ_YYK 160225 Modify..
//long MNetH::SetLowerEqBit(short eBitAddr, short nValue, int nUnit)
long MNetH::SetLowerEqBit(short eBitAddr, short nValue, int nUnit, BOOL bAOCBit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	EnterCriticalSection(&m_csMelsec); //SJ_YYK 160222 Add..
	//SJ_YYK 160225 Modify..
	//GetPLCAddressBit(m_nCurLocal, m_nCurUnit + nUnit, &nAddr, &nPoints, false);
	GetPLCAddressBit(m_nCurLocal, m_nCurUnit + nUnit, &nAddr, &nPoints, false, bAOCBit);
	nAddr += eBitAddr;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	LeaveCriticalSection(&m_csMelsec); //SJ_YYK 160222 Add..
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetMachineAlarm_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
			(1) Upper Eq에서 Current Eq에 On/Off한 신호를 읽는다.
			(2) GetPLCAddressBit: Local->m_nPrevLocal, bUpperEq->false

	FIRST	 : 2006/06/22, KJS
	LAST	 : 2006/06/22, KJS
========================================================================================*/
//SJ_YYK 160225 Modify..
//BOOL MNetH::IsOnUpperEqBit(short eBitAddr, int nUnit)
BOOL MNetH::IsOnUpperEqBit(short eBitAddr, int nUnit, BOOL bAOCBit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	BOOL	bResult;
	EnterCriticalSection(&m_csMelsec); //SJ_YYK 160222 Add..	
	//SJ_YYK 160225 Modify..
	//GetPLCAddressBit(m_nPrevLocal, m_nCurUnit + nUnit, &nAddr, &nPoints, false);
	GetPLCAddressBit(m_nPrevLocal, m_nCurUnit + nUnit, &nAddr, &nPoints, false, bAOCBit);
	nAddr += eBitAddr;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	bResult = *pnRBuf;
	delete [] pnRBuf; pnRBuf=NULL;
	LeaveCriticalSection(&m_csMelsec); //SJ_YYK 160222 Add..
	return bResult;
}

/*========================================================================================
	FUNCTION : MNetH::SetMachineAlarm_ToUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
			(1) Lower Eq에서 Current Eq에 On/Off한 신호를 읽는다.
			(2) GetPLCAddressBit: Local->m_nPrevLocal, bUpperEq->false

	FIRST	 : 2006/06/22, KJS
	LAST	 : 2006/06/22, KJS
========================================================================================*/
//SJ_YYK 160225 Modify..
//BOOL MNetH::IsOnLowerEqBit(short eBitAddr, int nUnit)
BOOL MNetH::IsOnLowerEqBit(short eBitAddr, int nUnit, BOOL bAOCBit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	BOOL	bResult;
	
	EnterCriticalSection(&m_csMelsec); //SJ_YYK 160222 Add..
	//SJ_YYK 160225 Modify..
	//GetPLCAddressBit(m_nNextLocal, m_nCurUnit + nUnit, &nAddr, &nPoints, true);
	GetPLCAddressBit(m_nNextLocal, m_nCurUnit + nUnit, &nAddr, &nPoints, true, bAOCBit);
	nAddr += eBitAddr;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	bResult = *pnRBuf;
	delete [] pnRBuf; pnRBuf=NULL;
	LeaveCriticalSection(&m_csMelsec); //SJ_YYK 160222 Add..
	return bResult;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCAddressNSMCBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2009/05/21, KJS
	LAST	 : 
========================================================================================*/
short MNetH::GetPLCAddressNSMCBit(int nLocal, int nUnit, short *pnAddr, short *pnPoints)
{
	short	nRet=0;

	if (-1==nLocal)
		nLocal=m_nCurLocal;
	if (-1==nUnit)
		nUnit=m_nCurUnit;

	int nFirstBitAddr;
	nFirstBitAddr = B_NSMC_01 + (nLocal-1)*B_NSMC_SIZE + (B_NSMC_SIZE * (nUnit-1));

	*pnAddr = nFirstBitAddr;

	*pnPoints = B_NSMC_SIZE;

	return nRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetPLCAddressNSMCWord()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2009/05/21, KJS
	LAST	 : 
========================================================================================*/
short MNetH::GetPLCAddressNSMCWord(short *pnAddr, short *pnPoints, int nLocal, int nUnit)
{
	short	nRet=0;

	if (-1==nLocal)
		nLocal=m_nCurLocal;
	if (-1==nUnit)
		nUnit=m_nCurUnit;

	int nFirstWordAddr;
	nFirstWordAddr = W_NSMC_01 + (nLocal-1)*W_NSMC_SIZE + (W_NSMC_SIZE * (nUnit-1));

	*pnAddr = nFirstWordAddr;

	*pnPoints = W_NSMC_SIZE;

	return nRet;
}

/*========================================================================================
	FUNCTION : MNetH::IsOnUpperEqBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
			(1) Upper Eq에서 Current Eq에 On/Off한 신호를 읽는다.
			(2) GetPLCAddressBit: Local->m_nPrevLocal, bUpperEq->false

	FIRST	 : 2009/05/21, KJS
	LAST	 : 
========================================================================================*/
//BOOL MNetH::IsOnNSMCEqBit(short eBitAddr, int nLocal, int nUnit)
BOOL MNetH::IsOnNSMCEqBit(short eBitAddr, int nLocal, int nUnit, bool bReserve)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	BOOL	bResult;
	
	if (-1==nLocal)
		nLocal=m_nCurLocal;
	if (-1==nUnit)
		nUnit=m_nCurUnit;

	// Modify 110525 모델 예약 인자 추가
	if (bReserve == true) {
		nUnit = m_nCurUnit -1;
	}

	GetPLCAddressNSMCBit(nLocal, nUnit, &nAddr, &nPoints);
	nAddr += eBitAddr;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	
	lRet = ReadLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	bResult = *pnRBuf;
	delete [] pnRBuf; pnRBuf=NULL;

	return bResult;
}

/*========================================================================================
	FUNCTION : MNetH::SetNSMCBit()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
			(1) Current Eq에서 Upper Eq의 Bit를 On/Off 시킨다.
			(2) GetPLCAddressBit: Local->m_nCurLocal, bUpperEq->true

	FIRST	 : 2009/05/21, KJS
	LAST	 : 
========================================================================================*/
long MNetH::SetNSMCBit(short eBitAddr, short nValue)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	nLen=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;

	GetPLCAddressNSMCBit(m_nCurLocal, m_nCurUnit, &nAddr, &nPoints);
	nAddr += eBitAddr;
	nPoints = 1;
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	pnRBuf[0] = nValue;
	lRet = WriteLB(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;

	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::GetNSMCWord()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2009/05/21, KJS
	LAST	 : 
========================================================================================*/
long MNetH::GetNSMCWord(MModelChangeWordDatas *pLE, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	int		nIndex=0;
	int		i = 0;
	CString	sTmp="";
	long lValue = 0;
	char szAscii[64];

	memset(szAscii, 0, sizeof(szAscii));
	
	GetPLCAddressNSMCWord(&nAddr, &nPoints, nLocal, nUnit);
	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);
	lRet = ReadLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if (!lRet){
		AscToString(szAscii, &pnRBuf[0], 10);	TrimSpace(szAscii);		pLE->m_strDeviceID	= szAscii;	
//		AscToString(szAscii, &pnRBuf[10], 1);	TrimSpace(szAscii);		pLE->m_strEQState	= szAscii;	
		pLE->m_strEQState.Format(_T("%d"), pnRBuf[10]);
		AscToString(szAscii, &pnRBuf[11], 2);	TrimSpace(szAscii);		pLE->m_strReplyCode	= szAscii;	
//		AscToString(szAscii, &pnRBuf[13], 1);	TrimSpace(szAscii);		pLE->m_strPanelWIP	= szAscii;	
		pLE->m_strPanelWIP.Format(_T("%d"), pnRBuf[13]);
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}

/*========================================================================================
	FUNCTION : MNetH::SetNextDeviceIDInfo()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	UPDATE	 : 2009/04/03, KJS
========================================================================================*/
long MNetH::SetNSMCWord(MModelChangeWordDatas *pLE, int nLocal, int nUnit)
{
	long	lRet=0;
	short	nAddr=0;
	short	nPoints=0;
	short	*pnRBuf=NULL;
	short	nIndex=0;
	short	nItemLen=0;
	CString	sTmp="";
	short   nStart = 0;
	short   nCount = 0;
	short   i = 0;
	WCHAR	szAscii[64];

	GetPLCAddressNSMCWord(&nAddr, &nPoints, nLocal, nUnit);

	pnRBuf = new short[nPoints];
	memset(pnRBuf, 0, sizeof(short)*nPoints);

	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strDeviceID);	FillSpace(szAscii, 20);	StringToAsc(szAscii, &pnRBuf[0], 20);

//	memset(szAscii, 0, sizeof(szAscii));	
//	swprintf(szAscii, pLE->m_strEQState);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[10], 2);
	pnRBuf[10] = _wtoi(pLE->m_strEQState.GetBuffer(1));

	memset(szAscii, 0, sizeof(szAscii));	
	swprintf(szAscii, pLE->m_strReplyCode);	FillSpace(szAscii, 4);	StringToAsc(szAscii, &pnRBuf[11], 4);

//	memset(szAscii, 0, sizeof(szAscii));	
//	swprintf(szAscii, pLE->m_strPanelWIP);	FillSpace(szAscii, 2);	StringToAsc(szAscii, &pnRBuf[13], 2);
	pnRBuf[13] = _wtoi(pLE->m_strPanelWIP.GetBuffer(1));

	lRet = WriteLW(nAddr, nPoints, pnRBuf, sizeof(short)*nPoints); 
	if(!lRet){
	}
	delete [] pnRBuf; pnRBuf=NULL;
	return lRet;
}
