/* 
 * MPos_XY Class
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
 * MPos_XY.h : This file defines the class of X, Y position.
 *			  이 파일은 X, Y position class를 정의한다.
 *
 * @author Yong-seog Pahn (dragon.pahn@samsung.com )
 * @version $Revision$
 */

#ifndef MPOS_XY_H
#define MPOS_XY_H

/**
 * X, Y 좌표 값에 대한 +/- 연산자 및 값 설정에 관련된 기능을 수행한다.
 */
class MPos_XY
{
public:

	/** X 좌표 값 */
	double dX;

	/** Y 좌표 값 */
	double dY;

public:
	/**
	 * Constructor : Data를 초기화하고, 내부 개체들을 초기화한다.
	 *
	 * @stereotype constructor 
	 */
	MPos_XY()
	{
		dX = 0.0L;
		dY = 0.0L;
	}

    /**
	 * Constructor : X, Y 값에 주어진 인수 값을 설정하여 생성된다.
	 *
	 * @param	double dx : X 좌표 값
	 * @param	double dy : Y 좌표 값
	 * @stereotype constructor 
     */
	MPos_XY(double dx, double dy)
	{
		dX = dx;
		dY = dy;
	}

	/**
	 * Attribute를 주어진 인수로 설정한다. (float type)
	 *
	 * @param	float fx : X 좌표 값
	 * @param	float fy : Y 좌표 값
	 */
	void Init(float fx, float fy)
	{
		dX = (double)fx;
		dY = (double)fy;
	}

	/**
	 * Attribute를 주어진 인수로 설정한다. (double type)
	 *
	 * @param	double dx : X 좌표 값
	 * @param	double dy : Y 좌표 값
	 */
	void Init(double dx, double dy)
	{
		dX = dx;
		dY = dy;
	}

	/**
	 * 내부 값들을 배열 구조로 넘겨준다.
	 *
	 * @param	double dVal[2] : 좌표 값
	 */
	void TransToArray(double dVal[2])
	{
		dVal[0] = dX;
		dVal[1] = dY;
	}

    /**
	 * + 연산자 재정의
	 *
	 * @param	MPos_XY : 본 Class에 더해질 X, Y 값
	 * @return	MPos_XY : 본 Class에 더해진 결과
	 */
    MPos_XY operator+ (MPos_XY sPos2)
	{
		double tmp_x = dX + sPos2.dX;
		double tmp_y = dY + sPos2.dY;

		return MPos_XY(tmp_x, tmp_y);
	}


    /**
	 * - 연산자 재정의
	 *
	 * @param	MPos_XY : 본 Class에서 뺄 X, Y 값
	 * @return	MPos_XY : 본 Class에서 뺀 결과
	 */
    MPos_XY operator- (MPos_XY sPos2)
	{
		double tmp_x = dX - sPos2.dX;
		double tmp_y = dY - sPos2.dY;

		return MPos_XY(tmp_x, tmp_y);
	}
};

#endif //MPOS_XY_H
