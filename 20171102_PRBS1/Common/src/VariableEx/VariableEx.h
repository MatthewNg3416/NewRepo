/***
* CVariableEx - Debug Variable Controller 
*
*
*       Copyright (c) Doolin. All rights reserved.
*
* Created	: Kwangilshiin, Kwangilshin@Naver.com
*			: 2017. 08. 02.
*
* Purpose	: Control method using variables and flags
*
*
*		Base	2017. 08. 02. Kwangilshin.
*
*******************************************************************************/

#if !defined(AFX_VARIABLEEX_H__132E1BC4_7EEE_4F2A_B1E7_5FCE0860FCD3__INCLUDED_)
#define AFX_VARIABLEEX_H__132E1BC4_7EEE_4F2A_B1E7_5FCE0860FCD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxmt.h>


/* ------------------------------------------------------------------------------------- */
// Class Definitions 
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// Namespace 
/* ------------------------------------------------------------------------------------- */

// Pre-condition state check
//
namespace PreCheckEx
{
	enum Status
	{
		eCheck			= 200,	// Nomal - Free interlock check
		eExecute		= 300,	// Function Execute Run
		eNotOK			= 400,	// Function Error - Free interlock Error
		eEnd			= 500,	// Function End
		eErr			= 600,	// Function Error
		eStop			= 700,	// Function Stop
		eInit			= 800,	// Function Init
		eDefault		= 900
	};
};


// Function Status
//
namespace ProcRetEx
{
	enum Status
	{
		eError			= 0,
		eComplete		= 1,
		eExecute		= 2,
		eStop			= 3,
		eNone			= 4
	};
};


/* ------------------------------------------------------------------------------------- */
// Thread Function definitions. 
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// Enum definitions. 
/* ------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------- */
// Definitions 
/* ------------------------------------------------------------------------------------- */

// Array Size.
//
#define ARR_SIZE(Var) (sizeof(Var) / sizeof(Var[0]))

/* ------------------------------------------------------------------------------------- */
// typedef enum definitions
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// Event ID
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// STD Class
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// typedef struct definitions
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// CDrv_Scheduler Class DLL
/* ------------------------------------------------------------------------------------- */
class CVariableEx  
{
public:

	/* --------------------------------------------------------------------------------- */
	// Construction
	/* --------------------------------------------------------------------------------- */
	CVariableEx();

	/* --------------------------------------------------------------------------------- */
	// Destruction
	/* --------------------------------------------------------------------------------- */
	virtual				~CVariableEx();

	/* Initialization.
	--------------------------------------------- */
	BOOL				InitInstance();

	/* Destroy.
	--------------------------------------------- */
	void				Destroy();

	/* ================================================================================= */
	// Typedef definitions
	/* ================================================================================= */

public:	
	
	/* Thread Function Addr.
	--------------------------------------------- */
	typedef BOOL (CALLBACK *pThreadAddress) (LPVOID lpParam, int nAction, int nAxisIdx);

	// Sqn Idx.
	//
	typedef enum SqnIdxEx
	{
		eSqn01			= 0,
		eSqn02			= 1,
		eSqn03			= 2,
		eSqn04			= 3,
		eSqn05			= 4,
		eSqn06			= 5,
		eSqn07			= 6,
		eSqn08			= 7,
		eSqn09			= 8,
		eSqn10			= 9,
		eSqn11			= 10,
		eSqn12			= 11,
		eSqn13			= 12,
		eSqn14			= 13,
		eSqn15			= 14,
		eSqn16			= 15,
		eSqn17			= 16,
		eSqn18			= 17,
		eSqn19			= 18,
		eSqn20			= 19,
		eMaxSqn
			
	} _eSqnIdx;

	// Execute Param Var Flg
	//
	typedef struct VarParaEx
	{
		int				nAct;			// Action.
		
		BOOL			bExe;			// Function Running.
		BOOL			bRun;			// Process Runing.
		int				nSqn;			// Function Step No.
		BOOL			bOnce;			// Once Running.
		BOOL			bCmpt;			// Function Complete.
		BOOL			bErr;			// Function Error.
		
		int				nErrCode;		// Error Code.
		
		int				nPreCheck;		// Pre-condition state check.

		DWORD			dwActCnt;		// Action Count.
		DWORD			dwCnt;			// Sqn Step Count.
		
		pThreadAddress	pTrdExecute;	// Call Back Function.

		VarParaEx()
		{
			Destroy();
		}
		
		void Destroy()
		{
			nAct		= -1;
			bExe		= FALSE;
			bRun		= FALSE;
			nSqn		= -1;
			bOnce		= TRUE;
			bCmpt		= FALSE;
			bErr		= FALSE;
			
			nErrCode	= 0;
			
			nPreCheck	= PreCheckEx::eDefault;

			dwActCnt	= 0;
			dwCnt		= 0;
		}
		
	} _stVarPara;


protected:
private:	


	/* ================================================================================= */
	// Variable definitions
	/* ================================================================================= */
public:	

	_stVarPara			m_stVarPara;		

protected:
private:

	CCriticalSection	m_csVar;

	/* ================================================================================= */
	// Function definitions
	/* ================================================================================= */

public:

	/* The Callback function activation status check.
	---------------------------------------------*/
	BOOL				AddRegistered(pThreadAddress pTrdAddress);

	/* The Callback function execute.
	---------------------------------------------*/
	BOOL				SetExecute(LPVOID lpParam, int nAction, int nAxisIdx);

	/* The Callback function force execute.
	---------------------------------------------*/
	BOOL				SetForceExecute(LPVOID lpParam, int nAction, int nAxisIdx);

	/* The Callback function activation status check.
	---------------------------------------------*/
	BOOL				Check_IsExecute();

	/* The Callback function process status check.
	---------------------------------------------*/
	BOOL				Check_IsProc(int nAction = -1 /* -1 */);

	/* The Callback function process all end ( done ) status check.
	---------------------------------------------*/
	BOOL				Check_IsProcDone(int nAction = -1 /* -1 */);

	/* The Callback function process error status check.
	---------------------------------------------*/
	BOOL				Check_IsProcErr(int nAction = -1 /* -1 */);

	/* Read the running action.
	---------------------------------------------*/
	int					GetProcAct();

	/* Thread Function Get Error Code.
	---------------------------------------------*/
	int					GetErrCode();

	/* Read the running prec check.
	---------------------------------------------*/
	int					GetPrecCheck();

	/* Set the running prec check.
	---------------------------------------------*/
	void				SetPrecCheck(PreCheckEx::Status enumState);

	/* Read the running sequence step.
	---------------------------------------------*/
	int					GetProcSqnStep();

public:	

	/* Param Var Param.
	---------------------------------------------*/
	_stVarPara			VarGetParam();

	/* Param Var All Init.
	---------------------------------------------*/
	void				VarSetObjsIdle();

	/* Thread Function Init.
	---------------------------------------------*/
	BOOL				VarCtrlInit();

	/* Thread Fucntion All Init First Start.
	---------------------------------------------*/
	BOOL				VarInitVariableToRun(int nAction = -1 /* -1 */);

	/* Thread Function Next Step.
	---------------------------------------------*/
	void				VarGoNextSqn();
	
	/* Thread Function Next Step : Param nTgtSqn : Go to Step.
	---------------------------------------------*/
	void				VarGoNextSqn(int nTgtSqn);

	/* Thread Function One Run Chrek.
	---------------------------------------------*/
	BOOL				VarIsOnceRun();

	/* Thread Function One Run.
	---------------------------------------------*/
	BOOL				VarIsOnceRunReset();

	/* Thread Function Start.
	---------------------------------------------*/
	BOOL				VarCtrlStart();

	/* Thread Function Stop.
	---------------------------------------------*/
	BOOL				VarCtrlStop();

	/* Thread Function Resume.
	---------------------------------------------*/
	BOOL				VarCtrlResume();

	/* Thread Function Ready or Step State Error.
	---------------------------------------------*/
	int					VarCtrlNotOK(int nErrCode = -1 /* = -1 */);

	/* Thread Function End.
	---------------------------------------------*/
	int					VarCtrlEnd();

	/* Thread Function Time
	---------------------------------------------*/
	DWORD				VarGetActTime();

	/* Thread Function Time
	---------------------------------------------*/
	DWORD				VarGetStepTime();

	/* ================================================================================= */
	// Event Message definitions
	/* ================================================================================= */
public:	
protected:
private:
};

#endif // !defined(AFX_VARIABLEEX_H__132E1BC4_7EEE_4F2A_B1E7_5FCE0860FCD3__INCLUDED_)
