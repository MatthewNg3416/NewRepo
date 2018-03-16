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
 *			  �� ������ X, Y, T, Z position class�� �����Ѵ�.
 *
 * @author Yong-seog Pahn (dragon.pahn@samsung.com )
 * @version $Revision$
 */

#ifndef MPOS_XYTZ_H
#define MPOS_XYTZ_H

/**
 * X, Y, T, Z ��ǥ ���� ���� +/- ������ �� �� ������ ���õ� ����� �����Ѵ�.
 */
class MPos_XYTZ
{
public:

	/** X ��ǥ �� */
	double dX;

	/** Y ��ǥ �� */
	double dY;

	/** T ��ǥ �� */
	double dT;

	/** Z ��ǥ �� */
	double dZ;

public:
	/**
	 * Constructor : Data�� �ʱ�ȭ�ϰ�, ���� ��ü���� �ʱ�ȭ�Ѵ�.
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
	 * Constructor : X, Y, T, Z ���� �־��� �μ� ���� �����Ͽ� �����ȴ�.
	 *
	 * @param	double dx : X ��ǥ ��
	 * @param	double dy : Y ��ǥ ��
	 * @param	double dt : T ��ǥ ��
	 * @param	double dz : Z ��ǥ ��
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
	 * Attribute�� �־��� �μ��� �����Ѵ�. (float type)
	 *
	 * @param	float fx : X ��ǥ ��
	 * @param	float fy : Y ��ǥ ��
	 * @param	float ft : T ��ǥ ��
	 * @param	float fz : Z ��ǥ ��
	 */
	void Init(float fx, float fy, float ft, float fz)
	{
		dX = (double)fx;
		dY = (double)fy;
		dT = (double)ft;
		dZ = (double)fz;
	}

	/**
	 * Attribute�� �־��� �μ��� �����Ѵ�. (double type)
	 *
	 * @param	double dx : X ��ǥ ��
	 * @param	double dy : Y ��ǥ ��
	 * @param	double dt : T ��ǥ ��
	 * @param	double dz : Z ��ǥ ��
	 */
	void Init(double dx, double dy, double dt, double dz)
	{
		dX = dx;
		dY = dy;
		dT = dt;
		dZ = dz;
	}
	
	/**
	 * ���� ������ �迭 ������ �Ѱ��ش�.
	 *
	 * @param	double dVal[4] : ��ǥ ��
	 */
	void TransToArray(double dVal[4])
	{
		dVal[0] = dX;
		dVal[1] = dY;
		dVal[2] = dT;
		dVal[3] = dZ;
	}

    /**
	 * + ������ ������
	 *
	 * @param	MPos_XYTZ : �� Class�� ������ X, Y, T, Z ��
	 * @return	MPos_XYTZ : �� Class�� ������ ���
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
	 * - ������ ������
	 *
	 * @param	MPos_XYTZ : �� Class���� �� X, Y, T, Z ��
	 * @return	MPos_XYTZ : �� Class���� �� ���
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
