/* 
 * MPos_XYT Class
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
 * MPos_XYT.h : This file defines the class of X, Y, T position.
 *			  �� ������ X, Y, T position class�� �����Ѵ�.
 *
 * @author Yong-seog Pahn (dragon.pahn@samsung.com )
 * @version $Revision$
 */

#ifndef MPOS_XYT_H
#define MPOS_XYT_H

/**
 * X, Y, T ��ǥ ���� ���� +/- ������ �� �� ������ ���õ� ����� �����Ѵ�.
 */
class MPos_XYT
{
public:

	/** X ��ǥ �� */
	double dX;

	/** Y ��ǥ �� */
	double dY;

	/** T ��ǥ �� */
	double dT;

public:
	/**
	 * Constructor : Data�� �ʱ�ȭ�ϰ�, ���� ��ü���� �ʱ�ȭ�Ѵ�.
	 *
	 * @stereotype constructor 
	 */
	MPos_XYT()
	{
		dX = 0.0L;
		dY = 0.0L;
		dT = 0.0L;
	}

    /**
	 * Constructor : X, Y, T ���� �־��� �μ� ���� �����Ͽ� �����ȴ�.
	 *
	 * @param	double dx : X ��ǥ ��
	 * @param	double dy : Y ��ǥ ��
	 * @param	double dt : T ��ǥ ��
	 * @stereotype constructor 
     */
	MPos_XYT(double dx, double dy, double dt)
	{
		dX = dx;
		dY = dy;
		dT = dt;
	}

	/**
	 * Attribute�� �־��� �μ��� �����Ѵ�. (float type)
	 *
	 * @param	float fx : X ��ǥ ��
	 * @param	float fy : Y ��ǥ ��
	 * @param	float ft : T ��ǥ ��
	 */
	void Init(float fx, float fy, float ft)
	{
		dX = (double)fx;
		dY = (double)fy;
		dT = (double)ft;
	}

	/**
	 * Attribute�� �־��� �μ��� �����Ѵ�. (double type)
	 *
	 * @param	double dx : X ��ǥ ��
	 * @param	double dy : Y ��ǥ ��
	 * @param	double dt : T ��ǥ ��
	 */
	void Init(double dx, double dy, double dt)
	{
		dX = dx;
		dY = dy;
		dT = dt;
	}

	/**
	 * ���� ������ �迭 ������ �Ѱ��ش�.
	 *
	 * @param	double dVal[3] : ��ǥ ��
	 */
	void TransToArray(double dVal[3])
	{
		dVal[0] = dX;
		dVal[1] = dY;
		dVal[2] = dT;
	}

    /**
	 * + ������ ������
	 *
	 * @param	MPos_XYT : �� Class�� ������ X, Y, T ��
	 * @return	MPos_XYT : �� Class�� ������ ���
	 */
    MPos_XYT operator+ (MPos_XYT sPos2)
	{
		double tmp_x = dX + sPos2.dX;
		double tmp_y = dY + sPos2.dY;
		double tmp_t = dT + sPos2.dT;

		return MPos_XYT(tmp_x, tmp_y, tmp_t);
	}

    /**
	 * - ������ ������
	 *
	 * @param	MPos_XYT : �� Class���� �� X, Y, T ��
	 * @return	MPos_XYT : �� Class���� �� ���
	 */
    MPos_XYT operator- (MPos_XYT sPos2)
	{
		double tmp_x = dX - sPos2.dX;
		double tmp_y = dY - sPos2.dY;
		double tmp_t = dT - sPos2.dT;

		return MPos_XYT(tmp_x, tmp_y, tmp_t);
	}
};

#endif //MPOS_XYT_H
