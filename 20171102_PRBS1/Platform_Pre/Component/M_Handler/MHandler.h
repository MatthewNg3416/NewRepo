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

// jdy 15. 잘못된 주석 처리 필요.
/** 버퍼와 Stage, Stage와 Stage 사이에 Panel을 중계하는 역활을 수행하는
 *  Handler 장비를 모델링한 Class이다.
 */
class MHandler : public MHandlerCommon
{
private:   

/**********
 *  Mechanical Layer State Check Components
 */
	/**  Stage의 Pointer		*/
//@	IStateStage*	m_plnkStage;

	// kss22 WorkBench 상태 필요
	IStateWorkBench*	m_plnkWorkBench;
	//SJ_YYK 161104 Add..
	IStatePanelTransfer*	m_plnkPanelTransferCenter;
	IStatePanelTransfer*	m_plnkPanelTransferIn;

	/** OpPanel의 Pointer		*/
	IOpPanel*		m_plnkOpPanel;

/**********
 *  Data
 */

	/** ACF Data									*/
//@    MAcfData*			m_plnkAcfData;	// kss 21 설비마다 다름. 
	
/**********
 *  Data
 */

public:
	int calculateHandlerXYTModelPosParameter();
/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

	MHandler(SCommonAttribute commonData, SHandlerRefCompList listRefComponent, SHandlerData datComponent);

	MHandler();

	virtual ~MHandler();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
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
/* Teaching 관련 제공 Interface
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
