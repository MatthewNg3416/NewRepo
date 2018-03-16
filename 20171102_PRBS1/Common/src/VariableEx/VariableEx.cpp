/***
* CVariableEx.cpp - Kwangil, Shin
*******************************************************************************/

/* ------------------------------------------------------------------------------------- */
// Dependencies
/* ------------------------------------------------------------------------------------- */
#include "stdafx.h"
#include "mplatformolb.h"
#include "VariableEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/* ------------------------------------------------------------------------------------- */
// Construction
/* ------------------------------------------------------------------------------------- */
CVariableEx::CVariableEx()
{
	ASSERT (InitInstance());
}


/* ------------------------------------------------------------------------------------- */
// class Destructor
/* ------------------------------------------------------------------------------------- */
CVariableEx::~CVariableEx()
{
	Destroy();
}


/* ------------------------------------------------------------------------------------- */
// class Event Map
/* ------------------------------------------------------------------------------------- */


/* ##################################################################################### */

/* ===================================================================================== */
// System Control Service - Public Function start
/* ------------------------------------------------------------------------------------- */

/* Initialization.
	--------------------------------------------- */
BOOL CVariableEx::InitInstance()
{
	m_stVarPara.Destroy();

	return TRUE;
}

/* Destroy.
--------------------------------------------- */
void CVariableEx::Destroy()
{
	m_stVarPara.Destroy();
}

/* The Callback function activation status check.
---------------------------------------------*/
BOOL CVariableEx::AddRegistered(pThreadAddress pTrdAddress)
{
	BOOL bResult = FALSE;

	if (m_csVar.Lock())
	{
		m_stVarPara.pTrdExecute = pTrdAddress;

		ASSERT (m_stVarPara.pTrdExecute);
		
		if (m_stVarPara.pTrdExecute)
		{
			bResult = TRUE;
		}

		m_csVar.Unlock();
	}
	
	return bResult;
}

/* The Callback function execute.
	---------------------------------------------*/
BOOL CVariableEx::SetExecute(LPVOID lpParam, int nAction, int nAxisIdx)
{
	BOOL bResult = FALSE;
	
	if (m_csVar.Lock())
	{		
		ASSERT (m_stVarPara.pTrdExecute);
		
		if (m_stVarPara.pTrdExecute)
		{
			bResult = m_stVarPara.pTrdExecute(lpParam, nAction, nAxisIdx);
		}
		
		m_csVar.Unlock();
	}
	
	return bResult;
}

/* The Callback function force execute.
---------------------------------------------*/
BOOL CVariableEx::SetForceExecute(LPVOID lpParam, int nAction, int nAxisIdx)
{
	BOOL bResult = FALSE;
	
	VarSetObjsIdle();

	if (m_csVar.Lock())
	{		
		ASSERT (m_stVarPara.pTrdExecute);
		
		if (m_stVarPara.pTrdExecute)
		{
			bResult = m_stVarPara.pTrdExecute(lpParam, nAction, nAxisIdx);
		}
		
		m_csVar.Unlock();
	}
	
	return bResult;
}

/* The Callback function activation status check.
---------------------------------------------*/
BOOL CVariableEx::Check_IsExecute()
{
	BOOL bResult = FALSE;

	if (m_csVar.Lock())
	{
		bResult = m_stVarPara.bExe;

		m_csVar.Unlock();
	}

	return bResult;
}


/* The Callback function process status check.
---------------------------------------------*/
BOOL CVariableEx::Check_IsProc(int nAction /* -1 */)
{
	BOOL bResult = FALSE;
	
	if (m_csVar.Lock())
	{
		if (nAction == -1 || m_stVarPara.nAct == nAction)
		{
			bResult = m_stVarPara.bExe & m_stVarPara.bRun;
		}

		m_csVar.Unlock();
	}
	
	return bResult;
}


/* The Callback function process all end ( done ) status check.
---------------------------------------------*/
BOOL CVariableEx::Check_IsProcDone(int nAction /* -1 */)
{
	BOOL bResult = FALSE;
	
	if (m_csVar.Lock())
	{
		if (nAction == -1 || m_stVarPara.nAct == nAction)
		{
			if (!m_stVarPara.bExe && !m_stVarPara.bRun && !m_stVarPara.bErr && m_stVarPara.bCmpt)
			{
				bResult = TRUE;
			}
		}
		
		m_csVar.Unlock();
	}
	
	return bResult;	
}


/* The Callback function process error status check.
---------------------------------------------*/
BOOL CVariableEx::Check_IsProcErr(int nAction /* -1 */)
{
	BOOL bResult = FALSE;
	
	if (m_csVar.Lock())
	{
		if (nAction == -1 || m_stVarPara.nAct == nAction)
		{
			if (!m_stVarPara.bExe && !m_stVarPara.bRun && m_stVarPara.bErr && !m_stVarPara.bCmpt)
			{
				bResult = TRUE;
			}
		}
		
		m_csVar.Unlock();
	}
	
	return bResult;	
}


/* Read the running action.
// Param 1 lpszExecute		: The function name.
---------------------------------------------*/
int	CVariableEx::GetProcAct()
{
	int nResult = -1;
	
	if (m_csVar.Lock())
	{
		nResult = m_stVarPara.nAct;
			
		m_csVar.Unlock();
	}
	
	return nResult;	
}


/* Thread Function Get Error Code.
---------------------------------------------*/
int	CVariableEx::GetErrCode()
{
	int nResult = 0;
	
	if (m_csVar.Lock())
	{
		if (m_stVarPara.bErr)
		{
			nResult = m_stVarPara.nErrCode;
		}
		
		m_csVar.Unlock();
	}
	
	return nResult;
}


/* Read the running prec check.
---------------------------------------------*/
int	CVariableEx::GetPrecCheck()
{
	int nResult = PreCheckEx::eDefault;
	
	if (m_csVar.Lock())
	{
		nResult = m_stVarPara.nPreCheck;
		
		m_csVar.Unlock();
	}
	
	return nResult;	
}


/* Set the running prec check.
---------------------------------------------*/
void CVariableEx::SetPrecCheck(PreCheckEx::Status enumState)
{	
	if (m_csVar.Lock())
	{
		m_stVarPara.nPreCheck = enumState;
		
		m_csVar.Unlock();
	}
}


/* Read the running sequence step.
---------------------------------------------*/
int	CVariableEx::GetProcSqnStep()
{
	int nResult = -1;
	
	if (m_csVar.Lock())
	{
		nResult = m_stVarPara.nSqn;
		
		m_csVar.Unlock();
	}
	
	return nResult;	
}


/* Param Var Param.
---------------------------------------------*/
CVariableEx::_stVarPara CVariableEx::VarGetParam()
{
	CVariableEx::_stVarPara stResult;
	
	if (m_csVar.Lock())
	{
		stResult = m_stVarPara;
		
		m_csVar.Unlock();
	}
	
	return stResult;	
}


/* Param Var All Init.
---------------------------------------------*/
void CVariableEx::VarSetObjsIdle()
{
	if (m_csVar.Lock())
	{
		m_stVarPara.nAct		= -1;
		m_stVarPara.bExe		= FALSE;
		m_stVarPara.bRun		= FALSE;
		m_stVarPara.nSqn		= -1;
		m_stVarPara.bOnce		= TRUE;
		m_stVarPara.bCmpt		= FALSE;
		m_stVarPara.bErr		= FALSE;
		
		m_stVarPara.nErrCode	= 0;
		
		m_stVarPara.nPreCheck	= PreCheckEx::eDefault;
		
		m_stVarPara.dwActCnt	= 0;
		m_stVarPara.dwCnt		= 0;

		m_csVar.Unlock();
	}
}


/* Thread Function Init.
---------------------------------------------*/
BOOL CVariableEx::VarCtrlInit()
{
	BOOL bResult = FALSE;
	
	if (m_csVar.Lock())
	{
		m_stVarPara.nPreCheck	= PreCheckEx::eInit;
		m_stVarPara.bExe		= FALSE;
		m_stVarPara.bRun		= FALSE;
		
		bResult					= TRUE;
		
		m_csVar.Unlock();
	}
	
	return bResult;
}


/* Thread Fucntion All Init First Start.
---------------------------------------------*/
BOOL CVariableEx::VarInitVariableToRun(int nAction /* -1 */)
{
	BOOL bResult = FALSE;

	if (m_csVar.Lock())
	{
		VarSetObjsIdle();

		m_stVarPara.nPreCheck	= PreCheckEx::eCheck;
		
		m_stVarPara.nAct		= nAction;
		m_stVarPara.bExe		= TRUE;
		m_stVarPara.bRun		= FALSE;
		m_stVarPara.bOnce		= TRUE;
		m_stVarPara.bCmpt		= FALSE;

		m_stVarPara.dwActCnt	= GetTickCount();
		
		bResult					= m_stVarPara.bExe;

		m_csVar.Unlock();
	}

	return bResult;
}


/* Thread Function Next Step.
---------------------------------------------*/
void CVariableEx::VarGoNextSqn()
{
	if (m_csVar.Lock())
	{
		m_stVarPara.bOnce		= TRUE;
		m_stVarPara.bErr		= FALSE;
		m_stVarPara.nSqn++;
		
		m_stVarPara.dwCnt		= GetTickCount();

		m_csVar.Unlock();
	}
}


/* Thread Function Next Step : Param nTgtSqn : Go to Step.
---------------------------------------------*/
void CVariableEx::VarGoNextSqn(int nTgtSqn)
{
	if (m_csVar.Lock())
	{
		m_stVarPara.bOnce		= TRUE;
		m_stVarPara.bErr		= FALSE;
		m_stVarPara.nSqn		= nTgtSqn;
		
		m_stVarPara.dwCnt		= GetTickCount();

		m_csVar.Unlock();
	}
}


/* Thread Function One Run Chrek.
---------------------------------------------*/
BOOL CVariableEx::VarIsOnceRun()
{
	BOOL bResult = FALSE;

	if (m_csVar.Lock())
	{
		if (m_stVarPara.bOnce)
		{
			m_stVarPara.bOnce		= FALSE;

			bResult = TRUE;
		}

		m_csVar.Unlock();
	}
	
	return bResult;
}


/* Thread Function One Run.
---------------------------------------------*/
BOOL CVariableEx::VarIsOnceRunReset()
{
	BOOL bResult = FALSE;
	
	if (m_csVar.Lock())
	{
		m_stVarPara.bOnce = TRUE;
		
		bResult = TRUE;

		m_csVar.Unlock();
	}
	
	return bResult;
}


/* Thread Function Start.
---------------------------------------------*/
BOOL CVariableEx::VarCtrlStart()
{
	BOOL bResult = FALSE;

	if (m_csVar.Lock())
	{
		if (m_stVarPara.bExe)
		{
			m_stVarPara.nPreCheck	= PreCheckEx::eExecute;
			m_stVarPara.bRun		= TRUE;
			m_stVarPara.nSqn		= 1;
			m_stVarPara.bCmpt		= FALSE;
			
			m_stVarPara.dwCnt		= GetTickCount();
		}
		else
		{
			m_stVarPara.bRun		= FALSE;
		}

		bResult					= m_stVarPara.bRun;

		m_csVar.Unlock();
	}

	return bResult;
}


/* Thread Function Stop.
---------------------------------------------*/
BOOL CVariableEx::VarCtrlStop()
{
	BOOL bResult = FALSE;
	
	if (m_csVar.Lock())
	{
		m_stVarPara.nPreCheck	= PreCheckEx::eStop;
		m_stVarPara.bExe		= FALSE;
		m_stVarPara.bRun		= FALSE;
		
		bResult					= m_stVarPara.bExe;
		
		m_csVar.Unlock();
	}
	
	return bResult;
}


/* Thread Function Resume.
---------------------------------------------*/
BOOL CVariableEx::VarCtrlResume()
{
	BOOL bResult = FALSE;
	
	if (m_csVar.Lock())
	{
		m_stVarPara.nPreCheck	= PreCheckEx::eExecute;
		m_stVarPara.bExe		= TRUE;
		m_stVarPara.bRun		= TRUE;
		
		m_stVarPara.dwCnt		= GetTickCount();
		
		bResult					= m_stVarPara.bRun;
		
		m_csVar.Unlock();
	}
	
	return bResult;
}

	
/* Thread Function Ready or Step State Error.
---------------------------------------------*/
int CVariableEx::VarCtrlNotOK(int nErrCode /* = -1 */)
{
	int	nResult = 0;

	if (m_csVar.Lock())
	{
		m_stVarPara.bErr		= TRUE;
		m_stVarPara.nErrCode	= nErrCode;

		m_stVarPara.nPreCheck	= PreCheckEx::eErr;
		m_stVarPara.bExe		= FALSE;
		m_stVarPara.bRun		= FALSE;
		
		nResult					= nErrCode;

		m_csVar.Unlock();
	}

	return nResult;
}


/* Thread Function End.
---------------------------------------------*/
int CVariableEx::VarCtrlEnd()
{
	if (m_csVar.Lock())
	{
		m_stVarPara.bErr		= FALSE;
		m_stVarPara.nErrCode	= 0;
		
		m_stVarPara.nPreCheck	= PreCheckEx::eEnd;
		m_stVarPara.bExe		= FALSE;
		m_stVarPara.bRun		= FALSE;
		m_stVarPara.nSqn		= -1;
		
		m_stVarPara.bCmpt		= TRUE;
		
		m_csVar.Unlock();
	}

	return 0;
}


/* Thread Function Time
---------------------------------------------*/
DWORD CVariableEx::VarGetActTime()
{
	DWORD dwResult = 0;
	
	if (m_csVar.Lock())
	{
		if (m_stVarPara.bExe)
		{
			dwResult = GetTickCount() - m_stVarPara.dwActCnt;
		}
		
		m_csVar.Unlock();
	}
	
	return dwResult;
}


/* Thread Function Time
---------------------------------------------*/
DWORD CVariableEx::VarGetStepTime()
{
	DWORD dwResult = 0;
	
	if (m_csVar.Lock())
	{
		if (m_stVarPara.bExe && m_stVarPara.bRun)
		{
			dwResult = GetTickCount() - m_stVarPara.dwCnt;
		}
		
		m_csVar.Unlock();
	}
	
	return dwResult;
}


/* ------------------------------------------------------------------------------------- */
// System Control Service - Public Function End
/* ===================================================================================== */


/* ===================================================================================== */
// System Control Service - Protected Function start
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// System Control Service - Protected Function End
/* ===================================================================================== */



/* ===================================================================================== */
// System Control Service - private Function start
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// System Control Service - private Function End
/* ===================================================================================== */


/* ##################################################################################### */