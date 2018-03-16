/* 
 * TrsTabCarrier Component
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
 * ITI_TabCarrier.h : ITI InterfaceClass of MTrsTabCarrier component.
 */

#ifndef ITI_TABCARRIER_H
#define ITI_TABCARRIER_H

/** @interface */
class ITI_TabCarrier
{
public:

	//111229 Add_____________
	//Unloading위치에서 Loading위치로이동시 미리 타발하도록
	virtual BOOL IsTabICReceiveAbleToTFRequest()	= 0;
	//_______________________
	virtual BOOL IsTabICReceiveAbleToTF()		= 0;
	virtual BOOL IsTabICReceiveStartToTF()		= 0;
	virtual BOOL IsTabICReceiveCompleteToTF()	= 0;

	virtual BOOL IsTabICSendAbleToTM()			= 0;
	virtual BOOL IsTabICSendStartToTM()			= 0;
	virtual BOOL IsLoadingMoveCompleteToTM()	= 0;
	virtual BOOL IsLoadingCompleteToTM()		= 0;
	virtual BOOL IsUnloadingMoveCompleteToTM()	= 0;
	virtual	void SetTabNo(int nTabNo)			= 0;//TabMounter가 Carrier에서 TabLoading전 Tab번호를 지정하면 TabCarrier가 TabOffset만큼 이동

	virtual BOOL IsTabICTrash_ReceiveAbleToTM()	= 0;
	virtual BOOL IsTabICTrash_ReceiveCompleteToTM() = 0;

	// NSMC
	virtual BOOL IsModelChangeReady()	= 0;

	virtual void SetTabICUnloadToTabMounter(int iGroupNo, int iWorkNo, BOOL Data) = 0;//170830 JSH
};
#endif //ITI_TABCARRIER1_H
