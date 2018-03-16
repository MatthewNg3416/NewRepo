/* 
 * Manage OpPanel Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MManageOpPanel.h : This file defines the implements class of manage oppanel component.
 *			  �� ������ manage oppanel Class�� ���� implements class�� �����Ѵ�.
 *
 * @author Yong-seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */

#ifndef MMANAGEOPPANEL_H
#define MMANAGEOPPANEL_H

#include "MTickTimer.h"
#include "DefManageOpPanel.h"
#include "ICommon.h"
//20110222 SJ_HJG
#include "MTowerLampData.h"

class MSystemData;
class IOpPanel;

/** This class is defined manage oppanel component implements class as a abstract class.
 * �� Class�� manage oppanel Class�� ���� implements Class�� �����Ѵ�.
 * 
 */
class MManageOpPanel : public virtual ICommon
{
private:
/**********
 *  Reference Components
 */
	/** OpPanel Component Pointer Parameter */
	IOpPanel* m_plnkOpPanel;

/**********
 *  ManageOpPanel Data
 */
	/** System Data Link Pointer */
	MSystemData* m_plnkSystemData;	

	//20110222 SJ_HJG
	MTowerLampData* m_plnkTowerLampData;

/**********
 *  Jog
 */
	/** Jog�� �̵��� Motion�� ���� ���� Index */
	int m_iJogIndex;

	/** Jog Key ���� �� ���� �� ���� */
	int m_iPrevJogVal_X;
	int m_iPrevJogVal_Y;
	int m_iPrevJogVal_T;
	int m_iPrevJogVal_Z;

	BOOL m_bPrevDir_X;
	BOOL m_bPrevDir_Y;
	BOOL m_bPrevDir_T;
	BOOL m_bPrevDir_Z;

/**********
 *  Blink
 */
	/** Blink Rate */
	double m_dBlinkRate;

	/** Tower Lamp Blink ������ ���� Timer */
	MTickTimer m_BlinkTimer;

	/** Blink Interval ����� ���� ���� */
	BOOL m_bBlinkState;

	volatile BOOL m_bBuzzerMode;

#ifdef SIMULATION
/**********
 * Simulation Switch
 */
	BOOL m_bSimulationStartSW;
	BOOL m_bSimulationStopSW;
	BOOL m_bSimulationResetSW;
#endif

public:    
/***********************************************************************************************
/*
/* ������ & �Ҹ���
/*
/************************************************************************************************/

    /**
	 * ������
     * @stereotype constructor 
     */
    MManageOpPanel();

	MManageOpPanel(SCommonAttribute commonData, IOpPanel *pOpPanel, MSystemData *pSystemData);

    /**
	 * �Ҹ���
     * @stereotype destructor 
     */
    virtual ~MManageOpPanel();    

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

    /**
     * OpPanel Component Object�� �����Ѵ�. 
	 *
	 * @param	*pOpPanel : ������ OpPanel Component Object Pointer
     */
     void AssignComponents(IOpPanel *pOpPanel);

   /**
     *  ���� �ʱ� ���·� �����ȴ�.
	 *
	 * @return	Error Code : 0 = SUCCESS, �� �� = Error
     */
    int Initialize();

/***********************************************************************************************
/*
/* ���� ���� üũ
/*
/************************************************************************************************/ 

	/**
	 * Door Sensor ���˿��θ� �����Ѵ�.
	 *
	 * @param bFlag : ���� ���� (TRUE:����, FALSE:����)
	 * @param iGroup : (OPTION=-1) Door Sensor Group ��ȣ (-1�̸� ��� Group�� ����)
	 * @param iIndex : Door Snesor Group �� Index ��ȣ (-1�̸� Group�� ��� Index ����)
	 */
	void SetDoorCheckFlag(BOOL bFlag, int iGourp = -1, int iIndex = -1);

	/**
	 * Door�� ���¸� �о�´�.
	 *
	 * @return	: Door ���� (TRUE:OPEN, FALSE:CLOSE)
	 */
	BOOL GetDoorSWStatus();


	/* 110411.KKY.Add. Material Change Door_____
	*/
	void SetMaterialChangeFlag(BOOL bFlag, int iGroup = -1, int iIndex = -1);	
/***********************************************************************************************
/*
/* Switch ���� Ȯ��
/*
/************************************************************************************************/ 

	/**
	 * Start Switch�� ���� ���� �о�´�.
	 *
	 * @param	*pbStatus : Start Switch ���� ���� (TRUE:ON, FALSE:OFF)
	 * @return	Error Code : 0=SUCCESS, �׿�=Error
	 */
	BOOL GetStartSWStatus();

	/**
	 * Stop Switch�� ���� ���� �о�´�.
	 *
	 * @param	*pbStatus : Stop Switch ���� ���� (TRUE:ON, FALSE:OFF)
	 * @return	Error Code : 0=SUCCESS, �׿�=Error
	 */
	BOOL GetStopSWStatus();

	/**
	 * E-Stop Switch�� ���� ���� �о�´�.
	 *
	 * @param	*pbStatus : E-Stop Switch ���� ���� (TRUE:ON, FALSE:OFF)
	 * @return	Error Code : 0=SUCCESS, �׿�=Error
	 */
	BOOL GetEStopSWStatus();

	/**
	 * Teaching Pendant ��� ���¸� �о�´�.
	 *
	 * @return	Error Code : 0=SUCCESS, �׿�=Error
	 */
	BOOL GetTPStatus();

	/**
	 * Reset Switch�� ���� ���� �о�´�.
	 *
	 * @param	*pbStatus : Reset Switch ���� ���� (TRUE:ON, FALSE:OFF)
	 * @return	Error Code : 0=SUCCESS, �׿�=Error
	 */
	BOOL GetResetSWStatus();

/***********************************************************************************************
/*
/* OpPanel (Operation Panel) ����
/*
/************************************************************************************************/ 

	/**
	 * Tower Lamp Blink �ӵ� �����ϱ�
	 *
	 * @param	dRate : (OPTION=0.5) ������ Blink �ӵ�
	 */
	void SetBlinkRate(double dRate = 0.5);

	/**
	 * OpPanel�� Switch �� Tower Lamp�� On/OIff�� �����Ѵ�.
	 *
	 * @param	iState : On/Off Mode
	 *		   Mode                     Start	Step	TowerR	TowerY	TowerG	Buzzer
	 *		1:Step Stop �Ϸ� ����		  X		 O		   O	  X		  X		  X
	 *		2:Start(Run Ready) ����		  O		 X		   X	  O		  O		  X
	 *		3:Run ����					  O		 X		   X	  X		  O		  X
	 *		4:Step Stop ���� ����		  X		 O		   B	  X		  X		  X
	 *		5:Error Stop ���� ����		  O		 X		   B	  X		  B		  O
	 *		6:Cycle Stop ���� ����		  O		 X		   X	  X		  B		  X
	 *		7:Parts Empty ����			  O		 X		   X	  B		  O		  O
	 *		  (Operator Call ����)
	 * @return	Error Code : 0=SUCCESS, �׿�=Error
	 */
	ErrorInfo SetOpPanel(int iState);
	
	/** 
	 * Buzzer Mode�� Set �Ѵ�.
	 * @param BOOL : TRUE = Buzzer Mode, FALSE = Buzzer Off Mode
	 */
	void SetBuzzerMode(BOOL bMode);

	/** 
	 * Buzzer Mode�� ��ȯ �Ѵ�.
	 * @return BOOL : TRUE = Buzzer Mode, FALSE = Buzzer Off Mode
	 */
	BOOL IsBuzzerMode();

/***********************************************************************************************
/*
/* Jog Operation
/*
/************************************************************************************************/ 

	/**
	 * Jog Key Ȯ���Ͽ� �ش� Jog ���� �̵�/�����Ѵ�.
	 *
	 * @return	Error Code : 0=SUCCESS, �׿�=Error
	 */
	int MoveJog(void);

	/**
	 * Jog�� ����� Unit Index�� �����Ѵ�.
	 *
	 * @param	iUnitIndex : (OPTION=-1) Jog�� ����� Unit Index
	 */
	void SetJogUnit(int iUnitIndex = DEF_MNGOPPANEL_JOG_NO_USE);

	/** 
	 * ������ Jog�� ����� Unit Index�� �д´�.
	 *
	 * @return	Error Code : 0=SUCCESS, �׿�=Error
	 */
	int GetJogUnit(void);

#ifdef SIMULATION
/***********************************************************************************************
/*
/* Simulation Operation
/*
/************************************************************************************************/ 
	void SetSimulationStartSW(BOOL bState);

	void SetSimulationStopSW(BOOL bState);
	
	void SetSimulationResetSW(BOOL bState);
#endif
};
#endif //MMANAGEOPPANEL_H
