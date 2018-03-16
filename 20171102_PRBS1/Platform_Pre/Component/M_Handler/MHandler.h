/* 
 * Handler Component
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
 * Handler.h : Headerfile of stage component.
 */


#ifndef MHANDLER_H
#define MHANDLER_H

#include "DefHandler.h"

#include "MHandlerCommon.h"
#include "MTeachingPosData.h"
class MAcfData;

// jdy 15. �߸��� �ּ� ó�� �ʿ�.
/** ���ۿ� Stage, Stage�� Stage ���̿� Panel�� �߰��ϴ� ��Ȱ�� �����ϴ�
 *  Handler ��� �𵨸��� Class�̴�.
 */
class MHandler : public MHandlerCommon
{
private:   

/**********
 *  Mechanical Layer State Check Components
 */
	/**  Stage�� Pointer		*/
//@	IStateStage*	m_plnkStage;

	// kss22 WorkBench ���� �ʿ�
	IStateWorkBench*	m_plnkWorkBench;
	//SJ_YYK 161104 Add..
	IStatePanelTransfer*	m_plnkPanelTransferCenter;
	IStatePanelTransfer*	m_plnkPanelTransferIn;

	/** OpPanel�� Pointer		*/
	IOpPanel*		m_plnkOpPanel;

/**********
 *  Data
 */

	/** ACF Data									*/
//@    MAcfData*			m_plnkAcfData;	// kss 21 ���񸶴� �ٸ�. 
	
/**********
 *  Data
 */

public:
	int calculateHandlerXYTModelPosParameter();
/***********************************************************************************************
/*
/* ������ & �Ҹ���
/*
/************************************************************************************************/

	MHandler(SCommonAttribute commonData, SHandlerRefCompList listRefComponent, SHandlerData datComponent);

	MHandler();

	virtual ~MHandler();

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

	void	GetData(SHandlerData *pData);

	int		SetData(SHandlerData datcomponent);

	int		AssignStateCheckComponents(SStateCheckCompListForHandler sCompList);

	int		ChangeModel(void);	 	

//@	int		ReleasePanel(BOOL bSkipSensor = FALSE);

	int		Initialize();

//	int	LoadXYTOffsetPosParameter(void);

/***********************************************************************************************
/*
/* Teaching ���� ���� Interface
/*
/************************************************************************************************/ 

/***********************************************************************************************
/*
/* Operation Interface
/*
/************************************************************************************************/ 

/**********
 * Move
 */

	MPos_XYT GetXYTTargetPos(int iPosID, int iTabNum = -1);
    
	int MoveXYTPos(MPos_XYT posTarget, int iPosID, BOOL bUseAuto = TRUE);

	int MoveXPos(double dTarget, int iPosID = -1, BOOL bUseAuto = TRUE);

	int MoveZPos(double dTarget, int iPosID, BOOL bUseAuto = FALSE);

	int MoveXYTurnPos();

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
private :

	int moveXYTPos(MPos_XYT posTarget, int iPosID, BOOL bUseAuto = FALSE);

	int moveZPos(double dTgtPos, int iPosID, BOOL bUseAuto = FALSE);

	int checkInterlockForZ(int iPosID);

	int checkInterlockForXYT(int iPosID,BOOL bUseAuto = FALSE);

	int checkInterlockForVacuumOff();

//	int calculateHandlerXYTModelPosParameter();

};

#endif // MHANDLER_H
