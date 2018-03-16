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
	//Unloading��ġ���� Loading��ġ���̵��� �̸� Ÿ���ϵ���
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
	virtual	void SetTabNo(int nTabNo)			= 0;//TabMounter�� Carrier���� TabLoading�� Tab��ȣ�� �����ϸ� TabCarrier�� TabOffset��ŭ �̵�

	virtual BOOL IsTabICTrash_ReceiveAbleToTM()	= 0;
	virtual BOOL IsTabICTrash_ReceiveCompleteToTM() = 0;

	// NSMC
	virtual BOOL IsModelChangeReady()	= 0;

	virtual void SetTabICUnloadToTabMounter(int iGroupNo, int iWorkNo, BOOL Data) = 0;//170830 JSH
};
#endif //ITI_TABCARRIER1_H
