/* 
 * MPos_XYTZ Class
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
 * MPos_XYTZ.h : This file defines the class of X, Y, T, Z position.
 *			  이 파일은 X, Y, T, Z position class를 정의한다.
 *
 * @author Yong-seog Pahn (dragon.pahn@samsung.com )
 * @version $Revision$
 */

#ifndef MPOS_XYTZ_H
#define MPOS_XYTZ_H

/**
 * X, Y, T, Z 좌표 값에 대한 +/- 연산자 및 값 설정에 관련된 기능을 수행한다.
 */
class MPos_XYTZ
{
public:

	/** X 좌표 값 */
	double dX;

	/** Y 좌표 값 */
	double dY;

	/** T 좌표 값 */
	double dT;

	/** Z 좌표 값 */
	double dZ;

public:
	/**
	 * Constructor : Data를 초기화하고, 내부 개체들을 초기화한다.
	 *
	 * @stereotype constructor 
	 */
	MPos_XYTZ()
	{
		dX = 0.0L;
		dY = 0.0L;
		dT = 0.0L;
		dZ = 0.0L;
	}

    /**
	 * Constructor : X, Y, T, Z 값에 주어진 인수 값을 설정하여 생성된다.
	 *
	 * @param	double dx : X 좌표 값
	 * @param	double dy : Y 좌표 값
	 * @param	double dt : T 좌표 값
	 * @param	double dz : Z 좌표 값
	 * @stereotype constructor 
     */
	MPos_XYTZ(double dx, double dy, double dt, double dz)
	{
		dX = dx;
		dY = dy;
		dT = dt;
		dZ = dz;
	}

	/**
	 * Attribute를 주어진 인수로 설정한다. (float type)
	 *
	 * @param	float fx : X 좌표 값
	 * @param	float fy : Y 좌표 값
	 * @param	float ft : T 좌표 값
	 * @param	float fz : Z 좌표 값
	 */
	void Init(float fx, float fy, float ft, float fz)
	{
		dX = (double)fx;
		dY = (double)fy;
		dT = (double)ft;
		dZ = (double)fz;
	}

	/**
	 * Attribute를 주어진 인수로 설정한다. (double type)
	 *
	 * @param	double dx : X 좌표 값
	 * @param	double dy : Y 좌표 값
	 * @param	double dt : T 좌표 값
	 * @param	double dz : Z 좌표 값
	 */
	void Init(double dx, double dy, double dt, double dz)
	{
		dX = dx;
		dY = dy;
		dT = dt;
		dZ = dz;
	}
	
	/**
	 * 내부 값들을 배열 구조로 넘겨준다.
	 *
	 * @param	double dVal[4] : 좌표 값
	 */
	void TransToArray(double dVal[4])
	{
		dVal[0] = dX;
		dVal[1] = dY;
		dVal[2] = dT;
		dVal[3] = dZ;
	}

    /**
	 * + 연산자 재정의
	 *
	 * @param	MPos_XYTZ : 본 Class에 더해질 X, Y, T, Z 값
	 * @return	MPos_XYTZ : 본 Class에 더해진 결과
	 */
    MPos_XYTZ operator+ (MPos_XYTZ sPos2)
	{
		double tmp_x = dX + sPos2.dX;
		double tmp_y = dY + sPos2.dY;
		double tmp_t = dT + sPos2.dT;
		double tmp_z = dZ + sPos2.dZ;

		return MPos_XYTZ(tmp_x, tmp_y, tmp_t, tmp_z);
	}

    /**
	 * - 연산자 재정의
	 *
	 * @param	MPos_XYTZ : 본 Class에서 뺄 X, Y, T, Z 값
	 * @return	MPos_XYTZ : 본 Class에서 뺀 결과
	 */
    MPos_XYTZ operator- (MPos_XYTZ sPos2)
	{
		double tmp_x = dX - sPos2.dX;
		double tmp_y = dY - sPos2.dY;
		double tmp_t = dT - sPos2.dT;
		double tmp_z = dZ - sPos2.dZ;

		return MPos_XYTZ(tmp_x, tmp_y, tmp_t, tmp_z);
	}
};

#endif //MPOS_XYTZ_H
