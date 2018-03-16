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
 *			  �� ������ X, Y position class�� �����Ѵ�.
 *
 * @author Yong-seog Pahn (dragon.pahn@samsung.com )
 * @version $Revision$
 */

#ifndef MPOS_XY_H
#define MPOS_XY_H

/**
 * X, Y ��ǥ ���� ���� +/- ������ �� �� ������ ���õ� ����� �����Ѵ�.
 */
class MPos_XY
{
public:

	/** X ��ǥ �� */
	double dX;

	/** Y ��ǥ �� */
	double dY;

public:
	/**
	 * Constructor : Data�� �ʱ�ȭ�ϰ�, ���� ��ü���� �ʱ�ȭ�Ѵ�.
	 *
	 * @stereotype constructor 
	 */
	MPos_XY()
	{
		dX = 0.0L;
		dY = 0.0L;
	}

    /**
	 * Constructor : X, Y ���� �־��� �μ� ���� �����Ͽ� �����ȴ�.
	 *
	 * @param	double dx : X ��ǥ ��
	 * @param	double dy : Y ��ǥ ��
	 * @stereotype constructor 
     */
	MPos_XY(double dx, double dy)
	{
		dX = dx;
		dY = dy;
	}

	/**
	 * Attribute�� �־��� �μ��� �����Ѵ�. (float type)
	 *
	 * @param	float fx : X ��ǥ ��
	 * @param	float fy : Y ��ǥ ��
	 */
	void Init(float fx, float fy)
	{
		dX = (double)fx;
		dY = (double)fy;
	}

	/**
	 * Attribute�� �־��� �μ��� �����Ѵ�. (double type)
	 *
	 * @param	double dx : X ��ǥ ��
	 * @param	double dy : Y ��ǥ ��
	 */
	void Init(double dx, double dy)
	{
		dX = dx;
		dY = dy;
	}

	/**
	 * ���� ������ �迭 ������ �Ѱ��ش�.
	 *
	 * @param	double dVal[2] : ��ǥ ��
	 */
	void TransToArray(double dVal[2])
	{
		dVal[0] = dX;
		dVal[1] = dY;
	}

    /**
	 * + ������ ������
	 *
	 * @param	MPos_XY : �� Class�� ������ X, Y ��
	 * @return	MPos_XY : �� Class�� ������ ���
	 */
    MPos_XY operator+ (MPos_XY sPos2)
	{
		double tmp_x = dX + sPos2.dX;
		double tmp_y = dY + sPos2.dY;

		return MPos_XY(tmp_x, tmp_y);
	}


    /**
	 * - ������ ������
	 *
	 * @param	MPos_XY : �� Class���� �� X, Y ��
	 * @return	MPos_XY : �� Class���� �� ���
	 */
    MPos_XY operator- (MPos_XY sPos2)
	{
		double tmp_x = dX - sPos2.dX;
		double tmp_y = dY - sPos2.dY;

		return MPos_XY(tmp_x, tmp_y);
	}
};

#endif //MPOS_XY_H
